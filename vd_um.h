/* vd_um.h - Manipulation Library
 * ---------------------------------------------------------------------------------------------------------------------
 * zlib License
 * 
 * (C) Copyright 2025-2026 Michael Dodis (michaeldodisgr@gmail.com)
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * ---------------------------------------------------------------------------------------------------------------------
 */
#ifndef VD_UM_H
#define VD_UM_H
#define VD_UM_VERSION_MAJOR    0
#define VD_UM_VERSION_MINOR    0
#define VD_UM_VERSION_PATCH    1
#define VD_UM_VERSION          ((VD_UM_VERSION_MAJOR << 16) | (VD_UM_VERSION_MINOR << 8) | (VD_UM_VERSION_PATCH))

#ifndef VD_UM_API
#   ifdef VD_UM_STATIC
#       define VD_UM_API static
#   else
#       define VD_UM_API extern
#   endif // VD_UM_STATIC
#endif // !VD_UM_API

#ifndef VD_UM_INL
#   define VD_UM_INL static inline
#endif // VD_UM_INL

typedef struct VdUmContext VdUmContext;
typedef struct {
    void* (*alloc)(void *prev, int prev_size, int new_size);
} VdUmContextCreateInfo;

typedef struct {
    int hot;
    int active;
    int manipulated;
} VdUmReply;

#pragma pack(push, 1)
typedef struct {
    float pos0[3];
    float size;
    float pos1[3];
    int   mode;
    float color[4];
    float orientation[4];
} VdUmVertex;
#pragma pack(pop)

enum {
    VD_UM_RENDER_PASS_FLAG_DEPTH_WRITE_AND_TEST = 1 << 0,
};

typedef struct {
    int flags;
    int instance_count;
    int first_instance;
    int vertex_count;
} VdUmRenderPass;

typedef struct {
    float pos_scale[4];
    float orientation[4];
    float color[3];
    int   mode;
} VdUmOpaqueVertex;

typedef struct {
    int instance_count;
    int vertex_count;
} VdUmOpaqueRenderPass;

VD_UM_API void          vd_um_init(void);
VD_UM_API void          vd_um_frame_begin(float dt);
VD_UM_API void          vd_um_viewport_begin(float viewport[4], float inv_projection[16], float view[16]);
VD_UM_API void          vd_um_viewport_end(void);
VD_UM_API void          vd_um_frame_end(void);
VD_UM_API void          vd_um_event_mouse_position(float position[2]);
VD_UM_API void          vd_um_event_mouse_delta(float delta[2]);

VD_UM_API VdUmReply     vd_um_translate(const char *str, float position[3], float direction[3], float up[3]);

VD_UM_API void          vd_um_push_id(const void *str, int len);
VD_UM_API void          vd_um_pop_id(void);

VD_UM_API VdUmContext*  vd_um_context_create(VdUmContextCreateInfo *info);
VD_UM_API void          vd_um_context_set(VdUmContext *ctx);
VD_UM_API VdUmContext*  vd_um_context_get(void);

#endif // !VD_UM_H

#ifdef VD_UM_IMPL

#ifndef VD_UM_VIEWPORT_MAX
#   define VD_UM_VIEWPORT_MAX 4
#endif // !VD_UM_VIEWPORT_MAX

#ifndef VD_UM_CUSTOM_TYPEDEFS
#include <stdint.h>
typedef uint64_t VdUmU64;
typedef uint32_t VdUmU32;
typedef uint8_t  VdUmU8;
#endif // !VD_UM_CUSTOM_TYPEDEFS

typedef enum {
    VD_UM__SHAPE_CUBE,
    VD_UM__SHAPE_CYLINDER,
    VD_UM__SHAPE_MAX,
} VdUm__Shape;

struct VdUmContext {
    void* (*alloc)(void *prev, int prev_size, int new_size);

    // Per Frame Data
    float mouse_delta[2];
    float mouse_pos[2];
    float dt;

    int                  num_opaque_passes;
    VdUmOpaqueRenderPass opaque_passes[VD_UM_VIEWPORT_MAX];

    int                  num_opaque_vertices;
    VdUmOpaqueVertex     *opaque_vertices;

    // Per Viewport Data
    float viewport[4];
    float inv_projection[16];
    float view[16];

    // Id Stack
    VdUmU64 *id_stack;
    int     id_stack_count;
    int     id_stack_cap;
};

static      void    vd_um__abort(const char *message);
static      void*   vd_um__alloc_default(void *prev, int prev_size, int new_size);
static      VdUmU64 vd_um__hash(void *begin, int len);
VD_UM_INL   float   vd_um__sqrt(float x);
VD_UM_INL   float   vd_um__abs(float x);
VD_UM_INL   void    vd_um__add3(float a[3], float b[3], float out[3]);
VD_UM_INL   void    vd_um__sub3(float a[3], float b[3], float out[3]);
VD_UM_INL   float   vd_um__dot3(float a[3], float b[3]);

#define VD_UM_EPSILON 1.19209290e-07f

#ifndef VD_UM_STDLIB
#   define VD_UM_STDLIB 1
#endif // !VD_UM_STDLIB

#ifndef VD_UM_ENABLE_ASSERTIONS
#   define VD_UM_ENABLE_ASSERTIONS 1
#endif // !VD_UM_ENABLE_ASSERTIONS

#if VD_UM_ENABLE_ASSERTIONS
#   define VD_UM_ASSERT_1(x) #x
#   define VD_UM_ASSERT(x) do { if (!(x)) { vd_um__abort("Assertion Failed: " __FILE__ ":" VD_UM_ASSERT_1(__LINE__) " - " #x ); } } while (0)
#else
#   define VD_UM_ASSERT(x)
#endif

VD_UM_API void vd_um_init(void)
{
    vd_um_context_set(vd_um_context_create(NULL));
}

VD_UM_API void vd_um_frame_begin(float dt)
{
    VdUmContext *ctx = vd_um_context_get();
    ctx->mouse_delta[0] = 0.f;
    ctx->mouse_delta[1] = 0.f;
    ctx->dt = dt;
}

VD_UM_API void vd_um_viewport_begin(float viewport[4], float inv_projection[16], float view[16])
{
    for (int i = 0; i <  4; ++i) ctx->viewport[i] = viewport[i];
    for (int i = 0; i < 16; ++i) ctx->inv_projection[i] = inv_projection[i];
    for (int i = 0; i < 16; ++i) ctx->view[i] = view[i];
}

VD_UM_API void vd_um_viewport_end(void)
{
}

VD_UM_API void vd_um_frame_end(void)
{
}

VD_UM_API void vd_um_frame_begin(float viewport[4], float inv_projection[16], float view[16], float dt)
{
}

VD_UM_API void vd_um_event_mouse_position(float position[2])
{
    VdUmContext *ctx = vd_um_context_get();
    ctx->mouse_pos[0] = position[0];
    ctx->mouse_pos[1] = position[1];

    float mouse_viewport_space[2] = {
        ctx->mouse_pos[0] - ctx->viewport[0],
        ctx->mouse_pos[1] - ctx->viewport[1],
    };

}

VD_UM_API void vd_um_event_mouse_delta(float delta[2])
{
    VdUmContext *ctx = vd_um_context_get();
    ctx->mouse_delta[0] += delta[0];
    ctx->mouse_delta[1] += delta[1];
}

VD_UM_API VdUmReply vd_um_translate(const char *str, float position[3], float direction[3], float up[3])
{
}

VD_UM_API void vd_um_frame_end(void)
{

}

VdUmContextCreateInfo Vd_Um__Default_Context = {
    vd_um__alloc_default,
};

VdUmContext *Vd_Um_Context = 0;

VD_UM_API VdUmContext *vd_um_context_create(VdUmContextCreateInfo *info)
{
    if (info == NULL) {
        info = &Vd_Um__Default_Context;
    }

    if (info->alloc == NULL) {
        info->alloc = vd_um__alloc_default;
    }

    VdUmContext *ctx = (VdUmContext*)info->alloc(NULL, 0, sizeof(VdUmContext));
    ctx->viewport[0] = 0.f;
    ctx->viewport[1] = 0.f;
    ctx->viewport[2] = 0.f;
    ctx->viewport[3] = 0.f;

    ctx->mouse_delta[0] = 0.f;
    ctx->mouse_delta[1] = 0.f;

    ctx->mouse_pos[0] = 0.f;
    ctx->mouse_pos[1] = 0.f;

    ctx->id_stack_cap = 64;
    ctx->id_stack_count = 0;
    ctx->id_stack = (VdUmU64*)info->alloc(NULL, 0, sizeof(VdUmU64) * ctx->id_stack_cap);

    return ctx;
}

VD_UM_API void vd_um_context_set(VdUmContext *ctx)
{
    Vd_Um_Context = ctx;
}

VD_UM_API VdUmContext *vd_um_context_get(void)
{
    return Vd_Um_Context;
}

#if VD_UM_STDLIB
#include <stdlib.h>
static void *vd_um__alloc_default(void *prev, int prev_size, int new_size)
{
    (void)prev_size;
    return realloc(prev, new_size);
}

static void vd_um__abort(const char *message)
{
    fprintf(stderr, "[vd_um] Aborted with message: %s\n", message);
}
#else
static void *vd_um__alloc_default(void *prev, int prev_size, int new_size)
{
    (void)prev;
    (void)prev_size;
    (void)new_size;
    VD_UM_ASSERT(0 && "No default allocation method!");
    return 0;
}

static void vd_um__abort(const char *message)
{
    *((char*)0) = *message;
}
#endif // VD_UM_STDLIB

static VdUmU64 vd_um__hash(void *begin, int len)
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int r = 24;

    unsigned long long h = 0x9747b28c ^ len;

    const unsigned char *bytes = (const unsigned char*)begin;

    while (len >= 4)
    {
        unsigned int k = *(unsigned int*)bytes;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        bytes += 4;
        len -= 4;   
    }

    switch (len) {
        case 3: h ^= bytes[2] << 16;
        case 2: h ^= bytes[1] << 8;
        case 1: h ^= bytes[0];
                h *= m;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

#if VD_UM_STDLIB
VD_UM_INL float vd_um__sqrt(float x)
{
    return sqrtf(x);
}
#else
#error "Need stdlib for sqrt"
#endif // !VD_UM_STDLIB

VD_UM_INL float vd_um__abs(float x)
{
    return x > 0.f ? x : -x;
}

VD_UM_INL void vd_um__add3(float a[3], float b[3], float out[3])
{
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
}

VD_UM_INL void vd_um__sub3(float a[3], float b[3], float out[3])
{
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}

VD_UM_INL float vd_um__dot3(float a[3], float b[3])
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

#endif // VD_UM_IMPL