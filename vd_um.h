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
    float timeout[2];
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

VD_UM_API void              vd_um_init(void);

VD_UM_API void              vd_um_frame_begin(float dt);
VD_UM_API void              vd_um_viewport_begin(float viewport[4], float inv_projection[16], float view[16], float origin[3]);
VD_UM_API void              vd_um_event_mouse_position(float position[2]);
VD_UM_API void              vd_um_event_mouse_delta(float delta[2]);
VD_UM_API void              vd_um_viewport_end(void);
VD_UM_API void              vd_um_frame_end(void);
VD_UM_API VdUmRenderPass*   vd_um_frame_get_passes_for_viewport(int viewport_index, int *num_render_passes);
VD_UM_API VdUmVertex*       vd_um_frame_get_vertex_buffer(int *num_vertices);

VD_UM_API void              vd_um_point(float position[3], float size, float color[4]);
VD_UM_API void              vd_um_segment(float start[3], float end[3], float thickness, float color[4]);
VD_UM_API void              vd_um_grid(float origin[3], float orientation[4], float extent, float color[4]);
VD_UM_API void              vd_um_cylinder(float base[3], float orientation[4], float height, float radius, float color[4]);
VD_UM_API void              vd_um_i_cylinder(float base[3], float orientation[4], float height, float radius, float normal_color[4], float hover_color[4]);

VD_UM_API void              vd_um_get_picking_ray(float origin[3], float direction[3]);

VD_UM_API void              vd_um_push_depth(int on);
VD_UM_API void              vd_um_pop_depth(void);

VD_UM_API void              vd_um_push_id(const void *str, int len);
VD_UM_API void              vd_um_pop_id(void);

VD_UM_API VdUmContext*      vd_um_context_create(VdUmContextCreateInfo *info);
VD_UM_API void              vd_um_context_set(VdUmContext *ctx);
VD_UM_API VdUmContext*      vd_um_context_get(void);

/* ----INTEGRATION - OPENGL------------------------------------------------------------------------------------------ */
typedef enum {
    VD_UM_GL_ATTRIB_POINTER_TYPE_NORMAL,  // glVertexAttribPointer
    VD_UM_GL_ATTRIB_POINTER_TYPE_INTEGER, // glVertexAttribIPointer
} VdUmGlAttribPointerType;

VD_UM_API void          vd_um_gl_get_default_shader_sources(const char **const vertex_shader, size_t *vertex_shader_len,
                                                            const char **const fragment_shader, size_t *fragment_shader_len);
VD_UM_API int           vd_um_gl_get_num_attributes(void);
VD_UM_API void          vd_um_gl_get_attribute_properties(int attribute, int *size, unsigned int *type,
                                                          unsigned char *normalized, int *stride, void **pointer,
                                                          unsigned int *divisor, VdUmGlAttribPointerType *attrib_type);

VD_UM_API const char*   vd_um_gl_get_uniform_name_projection(void);
VD_UM_API const char*   vd_um_gl_get_uniform_name_view(void);

#endif // !VD_UM_H

#ifdef VD_UM_IMPL

#define VD_UM_OFFSET_OF(type, element) ((size_t) & (((type*)0)->element))

#ifndef VD_UM_VIEWPORT_MAX
#   define VD_UM_VIEWPORT_MAX 4
#endif // !VD_UM_VIEWPORT_MAX

#ifndef VD_UM_CUSTOM_TYPEDEFS
#include <stdint.h>
typedef uint64_t VdUmU64;
typedef uint32_t VdUmU32;
typedef uint8_t  VdUmU8;
#endif // !VD_UM_CUSTOM_TYPEDEFS

enum {
    VD_UM__VERTEX_MODE_SEGMENT  = 0,
    VD_UM__VERTEX_MODE_GRID     = 1,
    VD_UM__VERTEX_MODE_POINT    = 2,
    VD_UM__VERTEX_MODE_RING     = 3,
    VD_UM__VERTEX_MODE_CYLINDER = 4,
};

struct VdUmContext {
    void* (*alloc)(void *prev, int prev_size, int new_size);

    // Per Frame Data
    float mouse_delta[2];
    float mouse_pos[2];
    float mouse_pos_in_viewport[2];
    float mouse_origin[3];
    float mouse_direction[3];
    float dt;

    int        num_vertices;
    VdUmVertex *vertices;

    int            num_passes;
    VdUmRenderPass passes[VD_UM_VIEWPORT_MAX];

    // Per Viewport Data
    float viewport[4];
    float inv_projection[16];
    float view[16];

    // Id Stack
    VdUmU64 *id_stack;
    int     id_stack_count;
    int     id_stack_cap;
};

static      void            vd_um__abort(const char *message);
static      void*           vd_um__alloc_default(void *prev, int prev_size, int new_size);
static      VdUmU64         vd_um__hash(void *begin, int len);
VD_UM_INL   float           vd_um__sqrt(float x);
VD_UM_INL   float           vd_um__abs(float x);
VD_UM_INL   void            vd_um__add3(float a[3], float b[3], float out[3]);
VD_UM_INL   void            vd_um__sub3(float a[3], float b[3], float out[3]);
VD_UM_INL   float           vd_um__dot3(float a[3], float b[3]);
VD_UM_INL   float           vd_um__dot4(float a[4], float b[4]);
VD_UM_INL   void            vd_um__div3(float a[3], float s, float out[3]);
VD_UM_INL   float           vd_um__lensq3(float a[3]);
VD_UM_INL   void            vd_um__noz3(float a[3], float out[3]);
VD_UM_INL   void            vd_um__mul_matrix_point(float matrix[16], float point[4], float out[4]);
VD_UM_INL   void            vd_um__conjugate(float q[4], float out[4]);
VD_UM_INL   void            vd_um__mul_quaternion(float q1[4], float q2[4], float out[4]);
VD_UM_INL   void            vd_um__mul_quaternion_vector(float q1[4], float v[3], float out[3]);
VD_UM_INL   int             vd_um__line_vs_cylinder(float line_a[3], float line_b[3], float P[3], float Q[3], float R, float *t);
static      void            vd_um__push_vertex(VdUmContext *ctx, VdUmVertex *vertex);
static      VdUmRenderPass* vd_um__get_current_pass(VdUmContext *ctx);
static      void            vd_um__require_render_pass(VdUmContext *ctx, int flags, int vertex_count);


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
    ctx->num_vertices = 0;
    ctx->num_passes = 0;
}

VD_UM_API void vd_um_viewport_begin(float viewport[4], float inv_projection[16], float view[16], float origin[3])
{
    VdUmContext *ctx = vd_um_context_get();
    for (int i = 0; i <  3; ++i) ctx->mouse_origin[i] = origin[i];
    for (int i = 0; i <  4; ++i) ctx->viewport[i] = viewport[i];
    for (int i = 0; i < 16; ++i) ctx->inv_projection[i] = inv_projection[i];
    for (int i = 0; i < 16; ++i) ctx->view[i] = view[i];

    ctx->num_passes++;
    VdUmRenderPass *pass = vd_um__get_current_pass(ctx);
    pass->flags = 0;
    pass->instance_count = 0;
    pass->first_instance = 0;
    pass->vertex_count = 6;
}

VD_UM_API void vd_um_viewport_end(void)
{
}

VD_UM_API void vd_um_frame_end(void)
{
}

VD_UM_API VdUmRenderPass *vd_um_frame_get_passes_for_viewport(int viewport_index, int *num_render_passes)
{
    VD_UM_ASSERT(viewport_index == 0);
    VdUmContext *ctx = vd_um_context_get();
    *num_render_passes = ctx->num_passes;
    return ctx->passes;
}

VD_UM_API VdUmVertex *vd_um_frame_get_vertex_buffer(int *num_vertices)
{
    VdUmContext *ctx = vd_um_context_get();
    *num_vertices = ctx->num_vertices;
    return ctx->vertices;
}

VD_UM_API void vd_um_event_mouse_position(float position[2])
{
    VdUmContext *ctx = vd_um_context_get();
    ctx->mouse_pos[0] = position[0];
    ctx->mouse_pos[1] = position[1];

    float mouse_viewport_space[2] = {
        + ((ctx->mouse_pos[0] - ctx->viewport[0]) / ctx->viewport[2]) * 2.f - 1.f,
        - ((ctx->mouse_pos[1] - ctx->viewport[1]) / ctx->viewport[3]) * 2.f + 1.f,
    };

    ctx->mouse_pos_in_viewport[0] = mouse_viewport_space[0];
    ctx->mouse_pos_in_viewport[1] = mouse_viewport_space[1];

    float mouse_clip_space[4] = {
        mouse_viewport_space[0],
        mouse_viewport_space[1],
        -1.f,
        +1.f
    };

    float mouse_view_space[4];
    vd_um__mul_matrix_point(ctx->inv_projection, mouse_clip_space, mouse_view_space);

    mouse_view_space[2] = 1.f;
    mouse_view_space[3] = 0.f;

    float mouse_world_space[4];
    vd_um__mul_matrix_point(ctx->view, mouse_view_space, mouse_world_space);

    float mouse_world_space3[3] = { mouse_world_space[0], mouse_world_space[1], mouse_world_space[2] };

    vd_um__noz3(mouse_world_space3, ctx->mouse_direction);
}

VD_UM_API void vd_um_event_mouse_delta(float delta[2])
{
    VdUmContext *ctx = vd_um_context_get();
    ctx->mouse_delta[0] += delta[0];
    ctx->mouse_delta[1] += delta[1];
}

VD_UM_API void vd_um_point(float position[3], float size, float color[4])
{
    VdUmContext *ctx = vd_um_context_get();
    vd_um__require_render_pass(ctx, -1, 6);
    VdUmVertex vertex = {};

    for (int i = 0; i < 3; i++) vertex.pos0[i] = position[i];
    vertex.size = size;
    vertex.mode = VD_UM__VERTEX_MODE_POINT;
    for (int i = 0; i < 4; i++) vertex.color[i] = color[i];

    vd_um__push_vertex(ctx, &vertex);
}

VD_UM_API void vd_um_segment(float start[3], float end[3], float thickness, float color[4])
{
    VdUmContext *ctx = vd_um_context_get();
    vd_um__require_render_pass(ctx, -1, 6);
    VdUmVertex vertex = {};

    for (int i = 0; i < 3; i++) vertex.pos0[i] = start[i];
    vertex.size = thickness;
    for (int i = 0; i < 3; i++) vertex.pos1[i] = end[i];
    vertex.mode = VD_UM__VERTEX_MODE_SEGMENT;
    for (int i = 0; i < 4; i++) vertex.color[i] = color[i];

    vd_um__push_vertex(ctx, &vertex);
}

VD_UM_API void vd_um_grid(float origin[3], float orientation[4], float extent, float color[4])
{
    VdUmContext *ctx = vd_um_context_get();
    vd_um__require_render_pass(ctx, -1, 6);
    VdUmVertex vertex = {};

    for (int i = 0; i < 3; i++) vertex.pos0[i] = origin[i];
    vertex.size = extent;
    vertex.mode = VD_UM__VERTEX_MODE_GRID;
    for (int i = 0; i < 4; i++) vertex.color[i] = color[i];
    for (int i = 0; i < 4; i++) vertex.orientation[i] = orientation[i];

    vd_um__push_vertex(ctx, &vertex);
}

VD_UM_API void vd_um_cylinder(float base[3], float orientation[4], float height, float radius, float color[4])
{
    VdUmContext *ctx = vd_um_context_get();
    vd_um__require_render_pass(ctx, -1, 8 * 6);
    VdUmVertex vertex = {};

    for (int i = 0; i < 3; i++) vertex.pos0[i] = base[i];
    vertex.size = height;
    vertex.mode = VD_UM__VERTEX_MODE_CYLINDER;
    for (int i = 0; i < 4; i++) vertex.color[i] = color[i];
    for (int i = 0; i < 4; i++) vertex.orientation[i] = orientation[i];
    vertex.pos1[0] = radius;

    vd_um__push_vertex(ctx, &vertex);
}

VD_UM_API void vd_um_i_cylinder(float base[3], float orientation[4], float height, float radius, float normal_color[4], float hover_color[4])
{
    float cylinder_forward[3];
    float conventional_normal[3] = {0,0,1};
    vd_um__mul_quaternion_vector(orientation, conventional_normal, cylinder_forward);
    float P[3] = { base[0], base[1], base[2] };
    float Q[3] = {
        base[0] + cylinder_forward[0] * height,
        base[1] + cylinder_forward[1] * height,
        base[2] + cylinder_forward[2] * height,
    };
    float R = radius;
    float ray_length = 100.f;
    // Get line from ray
    VdUmContext *ctx = vd_um_context_get();
    float line_a[3] = { 
        ctx->mouse_origin[0],
        ctx->mouse_origin[1],
        ctx->mouse_origin[2]
    };

    float line_b[3] = { 
        ctx->mouse_origin[0] + ctx->mouse_direction[0] * ray_length,
        ctx->mouse_origin[1] + ctx->mouse_direction[1] * ray_length,
        ctx->mouse_origin[2] + ctx->mouse_direction[2] * ray_length,
    };
    int cylinder_hit = 0;
    float ray_t = -1.f;
    cylinder_hit = vd_um__line_vs_cylinder(line_a, line_b, P, Q, R, &ray_t);

    float *color = normal_color;
    if (cylinder_hit) {
        color = hover_color;
    }

    vd_um_cylinder(base, orientation, height, radius, color);

}

VD_UM_API void vd_um_get_picking_ray(float origin[3], float direction[3])
{
    VdUmContext *ctx = vd_um_context_get();
    for (int i = 0; i < 3; ++i) origin[i] = ctx->mouse_origin[i];
    for (int i = 0; i < 3; ++i) direction[i] = ctx->mouse_direction[i];
}

static void vd_um__push_vertex(VdUmContext *ctx, VdUmVertex *vertex)
{
    VdUmRenderPass *pass = vd_um__get_current_pass(ctx);
    vertex->timeout[0] = vertex->timeout[1] = 1.f;

    VdUmVertex *dst = &ctx->vertices[ctx->num_vertices++];
    *dst = *vertex;

    pass->instance_count++;
}

static VdUmRenderPass *vd_um__get_current_pass(VdUmContext *ctx)
{
    return &ctx->passes[ctx->num_passes - 1];
}

static void vd_um__require_render_pass(VdUmContext *ctx, int flags, int vertex_count)
{
    VdUmRenderPass *pass = vd_um__get_current_pass(ctx);
    int should_push_new_pass = 0;
    if (flags != -1) {
        should_push_new_pass = should_push_new_pass || (flags != pass->flags);
    }

    if (vertex_count != -1) {
        should_push_new_pass = should_push_new_pass || (vertex_count != pass->vertex_count);
    }

    if (!should_push_new_pass) {
        return;
    }

    VdUmRenderPass *new_pass = &ctx->passes[ctx->num_passes++];
    *new_pass = *pass;

    if (flags != -1) {
        new_pass->flags = flags;
    }

    if (vertex_count != -1) {
        new_pass->vertex_count = vertex_count;
    }

    new_pass->first_instance = pass->first_instance + pass->instance_count;
    new_pass->instance_count = 0;
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

    ctx->vertices = (VdUmVertex*)info->alloc(NULL, 0, sizeof(VdUmVertex) * 64);

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

VD_UM_INL void vd_um__div3(float a[3], float s, float out[3])
{
    out[0] = a[0] / s;
    out[1] = a[1] / s;
    out[2] = a[2] / s;
}

VD_UM_INL float vd_um__dot4(float a[4], float b[4])
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

VD_UM_INL float vd_um__lensq3(float a[3])
{
    return vd_um__dot3(a, a);
}

VD_UM_INL void vd_um__noz3(float a[3], float out[3])
{
    float l = vd_um__lensq3(a);
    if (l == 0.f) {
        out[0] = 0.f;
        out[1] = 0.f;
        out[2] = 0.f;
    } else { 
        vd_um__div3(a, vd_um__sqrt(l), out);
    }
}

VD_UM_INL void vd_um__mul_matrix_point(float matrix[16], float point[4], float out[4])
{
    float r0[4] = { matrix[0 * 4 + 0], matrix[1 * 4 + 0], matrix[2 * 4 + 0], matrix[3 * 4 + 0] };
    float r1[4] = { matrix[0 * 4 + 1], matrix[1 * 4 + 1], matrix[2 * 4 + 1], matrix[3 * 4 + 1] };
    float r2[4] = { matrix[0 * 4 + 2], matrix[1 * 4 + 2], matrix[2 * 4 + 2], matrix[3 * 4 + 2] };
    float r3[4] = { matrix[0 * 4 + 3], matrix[1 * 4 + 3], matrix[2 * 4 + 3], matrix[3 * 4 + 3] };

    out[0] = vd_um__dot4(r0, point);
    out[1] = vd_um__dot4(r1, point);
    out[2] = vd_um__dot4(r2, point);
    out[3] = vd_um__dot4(r3, point);
}

VD_UM_INL void vd_um__mul_quaternion(float q1[4], float q2[4], float out[4])
{
    out[0] =  q1[0] * q2[3] + q1[1] * q2[2] - q1[2] * q2[1] + q1[3] * q2[0];
    out[1] = -q1[0] * q2[2] + q1[1] * q2[3] + q1[2] * q2[0] + q1[3] * q2[1];
    out[2] =  q1[0] * q2[1] - q1[1] * q2[0] + q1[2] * q2[3] + q1[3] * q2[2];
    out[3] = -q1[0] * q2[0] - q1[1] * q2[1] - q1[2] * q2[2] + q1[3] * q2[3];
}

VD_UM_INL void vd_um__mul_quaternion_vector(float q1[4], float v[3], float out[3])
{
    float u[4] = { v[0], v[1], v[2], 0.f };
    float qc[4];
    vd_um__conjugate(q1, qc);

    float u1[4];
    vd_um__mul_quaternion(q1, u, u1);
    float u2[4];
    vd_um__mul_quaternion(u1, qc, u2);

    out[0] = u2[0];
    out[1] = u2[1];
    out[2] = u2[2];
}

VD_UM_INL int vd_um__line_vs_cylinder(float line_a[3], float line_b[3], float P[3], float Q[3], float R, float *t)
{
    float d[3];
    {
        float temp[3];
        vd_um__sub3(Q, P, temp);
        vd_um__noz3(temp, d);
    }

    float m[3];
    vd_um__sub3(line_a, P, m);
    float n[3];
    vd_um__sub3(line_b, line_a, n);

    float md = vd_um__dot3(m, d);
    float nd = vd_um__dot3(n, d);
    float dd = vd_um__dot3(d, d);

    if ((md < 0.0f) && ((md + nd) < 0.0f)) {
        return 0;
    }

    if ((md > dd) && ((md + nd) > dd)) {
        return 0;
    }

    float nn = vd_um__dot3(n, n);
    float mn = vd_um__dot3(m, n);
    float  a = dd * nn - nd * nd;
    float  k = vd_um__dot3(m, m) - R * R;
    float  c = dd * k - md * md;

    if (vd_um__abs(a) < VD_UM_EPSILON) {
        if (c > 0.f) {
            return 0;
        }

        if (md < 0.f) {
            *t = -mn / nn;
        } else if (md > dd) {
            *t = (nd - mn) / nn;
        } else {
            *t = 0.f;
        }

        return 1;
    }

    float b = dd * mn - nd * md;
    float discriminant = b * b - a * c;
    if (discriminant < 0.f) {
        return 0;
    }

    *t = (-b - vd_um__sqrt(discriminant)) / a;

    if (((*t) < 0.f) || ((*t) > 1.f)) {
        return 0;
    }

    if ((md + (*t) * nd) < 0.f) {
        if (nd <= 0.f) {
            return 0;
        }

        *t = -md / nd;

        return (k + 2.f * (*t) * (mn + (*t) * nn)) <= 0.f;
    } else if ((md + (*t) * nd) > dd) {
        if (nd >= 0.f) {
            return 0;
        }

        *t = (dd - md) / nd;

        return (k + dd - 2.f * md + (*t) * (2.f * (mn - nd) + (*t) * nn)) <= 0.f;
    }

    return 1;
}

VD_UM_INL void vd_um__conjugate(float q[4], float out[4])
{
    out[0] = -q[0];
    out[1] = -q[1];
    out[2] = -q[2];
    out[3] = q[3];
}


/* ----INTEGRATION - OPENGL------------------------------------------------------------------------------------------ */
#define VD_UM_GL_VERTEX_SHADER_SOURCE                                                                                  \
"#version 330 core                                                                                                 \n" \
"                                                                                                                  \n" \
"layout (location = 0) in vec3 v_v0;                                                                               \n" \
"layout (location = 1) in float thickness;                                                                         \n" \
"layout (location = 2) in vec3 v_v1;                                                                               \n" \
"layout (location = 3) in uint mode;                                                                               \n" \
"layout (location = 4) in vec4 v_col;                                                                              \n" \
"layout (location = 5) in vec4 orientation;                                                                        \n" \
"layout (location = 6) in vec2 timeout;                                                                            \n" \
"                                                                                                                  \n" \
"uniform mat4 u_proj;                                                                                              \n" \
"uniform mat4 u_view;                                                                                              \n" \
"out vec4 f_col;                                                                                                   \n" \
"flat out uint f_mode;                                                                                             \n" \
"out vec2 f_texcoord;                                                                                              \n" \
"out vec2 f_timeout;                                                                                               \n" \
"out vec4 f_param;                                                                                                 \n" \
"                                                                                                                  \n" \
"vec3 quat_rotate(vec3 v, vec4 q) {                                                                                \n" \
"    return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);                                                     \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"void do_line()                                                                                                    \n" \
"{                                                                                                                 \n" \
"    float line_width = thickness;                                                                                 \n" \
"                                                                                                                  \n" \
"    // Transform to view space                                                                                    \n" \
"    // This will allow us to treat 0,0,0 as camera origin                                                         \n" \
"    vec4 Pv0 = u_view * vec4(v_v0, 1.0);                                                                          \n" \
"    vec4 Pv1 = u_view * vec4(v_v1, 1.0);                                                                          \n" \
"                                                                                                                  \n" \
"    // For each of the 6 vertices we do:                                                                          \n" \
"    // V = V0 + Table{ID}x * hL * R + Table{ID}y * L * F                                                          \n" \
"    // Where:                                                                                                     \n" \
"    // - V:  The final output vertex                                                                              \n" \
"    // - V0: The starting vertex                                                                                  \n" \
"    // - ID: gl_VertexID                                                                                          \n" \
"    // - hL: half line width in the direction orthogonal to the normalized line direction                         \n" \
"    // - R:  The right vector. orthogonal to F                                                                    \n" \
"    // - L:  The length of the line in the F direction                                                            \n" \
"    // - F:  The direction of the line                                                                            \n" \
"    vec2 movements[6] = vec2[](                                                                                   \n" \
"        vec2(+1.0, 1.0),                                                                                          \n" \
"        vec2(-1.0, 1.0),                                                                                          \n" \
"        vec2(+1.0, 0.0),                                                                                          \n" \
"        vec2(+1.0, 0.0),                                                                                          \n" \
"        vec2(-1.0, 1.0),                                                                                          \n" \
"        vec2(-1.0, 0.0)                                                                                           \n" \
"    );                                                                                                            \n" \
"                                                                                                                  \n" \
"    float L = distance(Pv1.xyz, Pv0.xyz);                                                                         \n" \
"    // Take the half point between the view relative points                                                       \n" \
"    // This allows us to handle cases where one point starts at 0,0,0 (i.e. the camera)                           \n" \
"    vec3 F = normalize(Pv1.xyz - Pv0.xyz);                                                                        \n" \
"                                                                                                                  \n" \
"    // Find closest point on line Pv0-Pv1 to (0,0,0)                                                              \n" \
"    float t = -dot(Pv0.xyz, F) / dot(F, F);                                                                       \n" \
"    vec3 closest = Pv0.xyz + F * t;                                                                               \n" \
"                                                                                                                  \n" \
"    float H = line_width / 2.0;                                                                                   \n" \
"                                                                                                                  \n" \
"    vec3 M = -normalize(closest);                                                                                 \n" \
"    // vec3 M = normalize(mix(Pv0.xyz, Pv1.xyz, 0.5));                                                            \n" \
"    // vec3 M = normalize(Pv0.xyz);                                                                               \n" \
"    vec3 Ur = M;                                                                                                  \n" \
"    // What M represents here is a direction vector that we know if aligns with line                              \n" \
"    // the line won't be shown anyway.                                                                            \n" \
"                                                                                                                  \n" \
"    // if (abs(dot(F, Ur)) >= 0.9999) {                                                                           \n" \
"    //     Ur = vec3(1,0,0);                                                                                      \n" \
"    // }                                                                                                          \n" \
"    vec3 R = cross(Ur, F);                                                                                        \n" \
"    vec3 vertex = Pv0.xyz + movements[gl_VertexID].x * H * R + movements[gl_VertexID].y * L * F;                  \n" \
"    gl_Position = u_proj * vec4(vertex, 1.0);                                                                     \n" \
"    f_col = v_col;                                                                                                \n" \
"    f_mode = mode;                                                                                                \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"void do_grid() {                                                                                                  \n" \
"    vec3 origin = vec3(v_v0);                                                                                     \n" \
"    vec3 scale = vec3(thickness);                                                                                 \n" \
"                                                                                                                  \n" \
"    vec3 verts[6] = vec3[](                                                                                       \n" \
"        vec3(+0.5, +0.5, 0.0),                                                                                    \n" \
"        vec3(-0.5, +0.5, 0.0),                                                                                    \n" \
"        vec3(+0.5, -0.5, 0.0),                                                                                    \n" \
"        vec3(+0.5, -0.5, 0.0),                                                                                    \n" \
"        vec3(-0.5, +0.5, 0.0),                                                                                    \n" \
"        vec3(-0.5, -0.5, 0.0)                                                                                     \n" \
"    );                                                                                                            \n" \
"    vec2 uvs[6] = vec2[](                                                                                         \n" \
"        vec2(1.0, 1.0),   // top-right                                                                            \n" \
"        vec2(0.0, 1.0),   // top-left                                                                             \n" \
"        vec2(1.0, 0.0),   // bottom-right                                                                         \n" \
"                                                                                                                  \n" \
"        vec2(1.0, 0.0),   // bottom-right                                                                         \n" \
"        vec2(0.0, 1.0),   // top-left                                                                             \n" \
"        vec2(0.0, 0.0)    // bottom-left                                                                          \n" \
"    );                                                                                                            \n" \
"    vec3 position = origin + quat_rotate(verts[gl_VertexID] * scale, orientation);                                \n" \
"    vec4 view_pos = u_view * vec4(position, 1.0);                                                                 \n" \
"    vec4 clip_pos = u_proj * view_pos;                                                                            \n" \
"    gl_Position = clip_pos;                                                                                       \n" \
"    f_col = v_col;                                                                                                \n" \
"    vec3 local_x = quat_rotate(vec3(1,0,0), orientation);                                                         \n" \
"    vec3 local_y = quat_rotate(vec3(0,1,0), orientation);                                                         \n" \
"    vec3 rel = position - origin;                                                                                 \n" \
"    f_texcoord = vec2(dot(rel,local_x),dot(rel, local_y));                                                        \n" \
"    f_mode = mode;                                                                                                \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"vec3 find_orthogonal_vector(vec3 v) {                                                                             \n" \
"    vec3 x = vec3(1,0,0);                                                                                         \n" \
"    if (abs(dot(v, x)) >= 0.9999) {                                                                               \n" \
"        x = vec3(0,1,0);                                                                                          \n" \
"    }                                                                                                             \n" \
"    return normalize(cross(x,v));                                                                                 \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"void do_point() {                                                                                                 \n" \
"    vec4 Pv0 = u_view * vec4(v_v0, 1.0);                                                                          \n" \
"    float point_scale = thickness;                                                                                \n" \
"    vec2 movements[6] = vec2[](                                                                                   \n" \
"        vec2(+0.5, +0.5),                                                                                         \n" \
"        vec2(-0.5, +0.5),                                                                                         \n" \
"        vec2(+0.5, -0.5),                                                                                         \n" \
"        vec2(+0.5, -0.5),                                                                                         \n" \
"        vec2(-0.5, +0.5),                                                                                         \n" \
"        vec2(-0.5, -0.5)                                                                                          \n" \
"    );                                                                                                            \n" \
"    float H = point_scale;                                                                                        \n" \
"                                                                                                                  \n" \
"    vec3 M = -normalize(Pv0.xyz);                                                                                 \n" \
"    vec3 A = find_orthogonal_vector(M);                                                                           \n" \
"    vec3 R = cross(A, M);                                                                                         \n" \
"    vec3 F = cross(M, R);                                                                                         \n" \
"    vec3 vertex = Pv0.xyz + movements[gl_VertexID].x * H * R + movements[gl_VertexID].y * H * F;                  \n" \
"    gl_Position = u_proj * vec4(vertex, 1.0);                                                                     \n" \
"    f_mode = 0u;                                                                                                  \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"void do_ring() {                                                                                                  \n" \
"    vec3 origin = vec3(v_v0);                                                                                     \n" \
"    vec3 scale = vec3(thickness);                                                                                 \n" \
"                                                                                                                  \n" \
"    vec3 verts[6] = vec3[](                                                                                       \n" \
"        vec3(+0.5, +0.5, 0.0),                                                                                    \n" \
"        vec3(-0.5, +0.5, 0.0),                                                                                    \n" \
"        vec3(+0.5, -0.5, 0.0),                                                                                    \n" \
"        vec3(+0.5, -0.5, 0.0),                                                                                    \n" \
"        vec3(-0.5, +0.5, 0.0),                                                                                    \n" \
"        vec3(-0.5, -0.5, 0.0)                                                                                     \n" \
"    );                                                                                                            \n" \
"    vec2 uvs[6] = vec2[](                                                                                         \n" \
"        vec2(1.0, 1.0),   // top-right                                                                            \n" \
"        vec2(0.0, 1.0),   // top-left                                                                             \n" \
"        vec2(1.0, 0.0),   // bottom-right                                                                         \n" \
"                                                                                                                  \n" \
"        vec2(1.0, 0.0),   // bottom-right                                                                         \n" \
"        vec2(0.0, 1.0),   // top-left                                                                             \n" \
"        vec2(0.0, 0.0)    // bottom-left                                                                          \n" \
"    );                                                                                                            \n" \
"    vec3 position = origin + quat_rotate(verts[gl_VertexID] * scale, orientation);                                \n" \
"    vec4 view_pos = u_view * vec4(position, 1.0);                                                                 \n" \
"    vec4 clip_pos = u_proj * view_pos;                                                                            \n" \
"    gl_Position = clip_pos;                                                                                       \n" \
"    f_col = v_col;                                                                                                \n" \
"    f_texcoord = uvs[gl_VertexID];                                                                                \n" \
"    f_mode = 2u;                                                                                                  \n" \
"    f_param = vec4(v_v1.x,0,0,0);                                                                                 \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"void main() {                                                                                                     \n" \
"    if (mode == 0u) { do_line(); }                                                                                \n" \
"    if (mode == 1u) { do_grid(); }                                                                                \n" \
"    if (mode == 2u) { do_point(); }                                                                               \n" \
"    if (mode == 3u) { do_ring(); }                                                                                \n" \
"    f_timeout = timeout;                                                                                          \n" \
"}                                                                                                                 \n" \

#define VD_UM_GL_FRAGMENT_SHADER_SOURCE                                                                                \
"#version 330 core                                                                                                 \n" \
"                                                                                                                  \n" \
"in vec4 f_col;                                                                                                    \n" \
"in vec2 f_texcoord;                                                                                               \n" \
"in vec2 f_timeout;                                                                                                \n" \
"flat in uint f_mode;                                                                                              \n" \
"out vec4 r_col;                                                                                                   \n" \
"in vec4 f_param;                                                                                                  \n" \
"                                                                                                                  \n" \
"void do_line() {                                                                                                  \n" \
"    r_col = f_col;                                                                                                \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"float grid(vec2 lineWidth, vec2 texcoord) {                                                                       \n" \
"    vec2 uv = texcoord * 1;                                                                                       \n" \
"    vec2 ddx = dFdx(uv);                                                                                          \n" \
"    vec2 ddy = dFdy(uv);                                                                                          \n" \
"    vec2 uvDeriv = vec2(length(vec2(ddx.x, ddy.x)), length(vec2(ddx.y, ddy.y)));                                  \n" \
"    bvec2 invertLine = bvec2(lineWidth.x > 0.5, lineWidth.y > 0.5);                                               \n" \
"    vec2 targetWidth = vec2(                                                                                      \n" \
"      invertLine.x ? 1.0 - lineWidth.x : lineWidth.x,                                                             \n" \
"      invertLine.y ? 1.0 - lineWidth.y : lineWidth.y                                                              \n" \
"      );                                                                                                          \n" \
"    vec2 drawWidth = clamp(targetWidth, uvDeriv, vec2(0.5));                                                      \n" \
"    vec2 lineAA = uvDeriv * 1.5;                                                                                  \n" \
"    vec2 gridUV = abs(fract(uv) * 2.0 - 1.0);                                                                     \n" \
"    gridUV.x = invertLine.x ? gridUV.x : 1.0 - gridUV.x;                                                          \n" \
"    gridUV.y = invertLine.y ? gridUV.y : 1.0 - gridUV.y;                                                          \n" \
"    vec2 grid2 = smoothstep(drawWidth + lineAA, drawWidth - lineAA, gridUV);                                      \n" \
"                                                                                                                  \n" \
"    grid2 *= clamp(targetWidth / drawWidth, 0.0, 1.0);                                                            \n" \
"    grid2 = mix(grid2, targetWidth, clamp(uvDeriv * 2.0 - 1.0, 0.0, 1.0));                                        \n" \
"    grid2.x = invertLine.x ? 1.0 - grid2.x : grid2.x;                                                             \n" \
"    grid2.y = invertLine.y ? 1.0 - grid2.y : grid2.y;                                                             \n" \
"    return mix(grid2.x, 1.0, grid2.y);                                                                            \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"void do_grid() {                                                                                                  \n" \
"    float grid1 = grid(vec2(0.02), f_texcoord * 1.0);                                                             \n" \
"    float grid2 = grid(vec2(0.01), f_texcoord * 2.0);                                                             \n" \
"    vec3 color1 = vec3(1.0,1.0,1.0);                                                                              \n" \
"    vec3 color2 = vec3(0.7,0.7,0.7);                                                                              \n" \
"                                                                                                                  \n" \
"    float a1 = grid1;                                                                                             \n" \
"    float a2 = grid2 * (1.0 - a1);                                                                                \n" \
"    vec3 color = mix(color1, color2, a2);                                                                         \n" \
"    float alpha = a1 + a2;                                                                                        \n" \
"    r_col = vec4(color, alpha);                                                                                   \n" \
"    if (alpha < 0.01) discard;                                                                                    \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"void do_ring() {                                                                                                  \n" \
"    vec2 uv = f_texcoord * 2.0 - 1.0;                                                                             \n" \
"    vec2 p = uv;                                                                                                  \n" \
"                                                                                                                  \n" \
"    float lsq = length(uv);                                                                                       \n" \
"    float R1 = 1.0;                                                                                               \n" \
"    float H = f_param.x;                                                                                          \n" \
"    float R2 = R1 - H;                                                                                            \n" \
"    float T = lsq;                                                                                                \n" \
"    float a1 = smoothstep(R2, R2 + 0.001, T);                                                                     \n" \
"    float a2 = 1.0 - smoothstep(R1 - 0.001, R1, T);                                                               \n" \
"    vec4 color = f_col;                                                                                           \n" \
"    color.a *= a1 * a2;                                                                                           \n" \
"    r_col = color;                                                                                                \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    if (f_mode == 0u) { do_line(); }                                                                              \n" \
"    else if (f_mode == 1u) { do_grid(); }                                                                         \n" \
"    else if (f_mode == 2u) { do_ring(); }                                                                         \n" \
"    r_col.a *= (f_timeout.x / f_timeout.y);                                                                       \n" \
"}                                                                                                                 \n" \

VD_UM_API void vd_um_gl_get_default_shader_sources(const char **const vertex_shader, size_t *vertex_shader_len,
                                                   const char **const fragment_shader, size_t *fragment_shader_len)
{
    *vertex_shader = VD_UM_GL_VERTEX_SHADER_SOURCE;
    *vertex_shader_len = sizeof(VD_UM_GL_VERTEX_SHADER_SOURCE);
    *fragment_shader = VD_UM_GL_FRAGMENT_SHADER_SOURCE;
    *fragment_shader_len = sizeof(VD_UM_GL_FRAGMENT_SHADER_SOURCE);
}

VD_UM_API int vd_um_gl_get_num_attributes(void)
{
    return 7;    
}

VD_UM_API void vd_um_gl_get_attribute_properties(int attribute, int *size, unsigned int *type,
                                                 unsigned char *normalized, int *stride, void **pointer,
                                                 unsigned int *divisor, VdUmGlAttribPointerType *attrib_type)
{
    switch (attribute) {
        case 0: *size = 3; *type = 0x1406 /* FLOAT */; *normalized = 0; *stride = sizeof(VdUmVertex); *pointer = (void*)VD_UM_OFFSET_OF(VdUmVertex, pos0);          *divisor = 1; *attrib_type = VD_UM_GL_ATTRIB_POINTER_TYPE_NORMAL; break;
        case 1: *size = 1; *type = 0x1406 /* FLOAT */; *normalized = 0; *stride = sizeof(VdUmVertex); *pointer = (void*)VD_UM_OFFSET_OF(VdUmVertex, size);          *divisor = 1; *attrib_type = VD_UM_GL_ATTRIB_POINTER_TYPE_NORMAL; break;
        case 2: *size = 3; *type = 0x1406 /* FLOAT */; *normalized = 0; *stride = sizeof(VdUmVertex); *pointer = (void*)VD_UM_OFFSET_OF(VdUmVertex, pos1);          *divisor = 1; *attrib_type = VD_UM_GL_ATTRIB_POINTER_TYPE_NORMAL; break;
        case 3: *size = 1; *type = 0x1405 /* UINT  */; *normalized = 0; *stride = sizeof(VdUmVertex); *pointer = (void*)VD_UM_OFFSET_OF(VdUmVertex, mode);          *divisor = 1; *attrib_type = VD_UM_GL_ATTRIB_POINTER_TYPE_INTEGER; break;
        case 4: *size = 4; *type = 0x1406 /* FLOAT */; *normalized = 0; *stride = sizeof(VdUmVertex); *pointer = (void*)VD_UM_OFFSET_OF(VdUmVertex, color);         *divisor = 1; *attrib_type = VD_UM_GL_ATTRIB_POINTER_TYPE_NORMAL; break;
        case 5: *size = 4; *type = 0x1406 /* FLOAT */; *normalized = 0; *stride = sizeof(VdUmVertex); *pointer = (void*)VD_UM_OFFSET_OF(VdUmVertex, orientation);   *divisor = 1; *attrib_type = VD_UM_GL_ATTRIB_POINTER_TYPE_NORMAL; break;
        case 6: *size = 2; *type = 0x1406 /* FLOAT */; *normalized = 0; *stride = sizeof(VdUmVertex); *pointer = (void*)VD_UM_OFFSET_OF(VdUmVertex, timeout);       *divisor = 1; *attrib_type = VD_UM_GL_ATTRIB_POINTER_TYPE_NORMAL; break;
        default: return;
    }
}

VD_UM_API const char *vd_um_gl_get_uniform_name_projection(void)
{
    return "u_proj";
}

VD_UM_API const char *vd_um_gl_get_uniform_name_view(void)
{
    return "u_view";
}


#endif // VD_UM_IMPL