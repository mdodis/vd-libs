/**
 * vd_ui.h - UI library
 * 
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
 */
#ifndef VD_UI_H
#define VD_UI_H
#define VD_UI_VERSION_MAJOR    0
#define VD_UI_VERSION_MINOR    0
#define VD_UI_VERSION_PATCH    1
#define VD_UI_VERSION          ((VD_UI_VERSION_MAJOR << 16) | (VD_UI_VERSION_MINOR << 8) | (VD_UI_VERSION_PATCH))

#ifndef VD_UNUSED
#define VD_UNUSED(x) ((void)(x))
#endif // !VD_UNUSED

/**
 * To use a custom id for a texture, define a struct like so:
 * typedef struct {
 *     <Your custom texture id stuff>
 * } VdUiTextureId;
 */
#ifndef VD_UI_TEXTURE_ID_STRUCT_DEFINED
#define VD_UI_TEXTURE_ID_STRUCT_DEFINED 0
#endif // VD_UI_TEXTURE_ID_STRUCT_DEFINED

/* ----BASIC--------------------------------------------------------------------------------------------------------- */
typedef struct VdUiDiv VdUiDiv;
struct VdUiDiv {
    VdUiDiv *first;
    VdUiDiv *last;
    VdUiDiv *next;
    VdUiDiv *prev;
    VdUiDiv *parent;
};

/**
 * Initialize the ui.
 */
extern void             vd_ui_init(void);

/**
 * Get the minimum size required to hold the vertex buffer. Using this, you should allocate the buffer
 * @return  The size (in bytes) of the vertex buffer.
 */
extern size_t           vd_ui_get_min_vertex_buffer_size(void);

/**
 * Begin a new frame. Call this before any ui layout or rendering calls
 * @param delta_seconds The delta time, in seconds.
 */
extern void             vd_ui_frame_begin(float delta_seconds);
/**
 * End the frame. Call this after any ui layout or drawing calls.
 */
extern void             vd_ui_frame_end(void);

/* ----RENDERING----------------------------------------------------------------------------------------------------- */
#pragma pack(push, 1)
typedef struct {
    float p0[2];
    float p1[2];
    float p0u[2];
    float p1u[2];
    float color[4];
} VdUiVertex;
#pragma pack(pop)

#if !VD_UI_TEXTURE_ID_STRUCT_DEFINED
typedef struct {
    uintptr_t id;
} VdUiTextureId;
#define VD_UI_TEXTURE_ID_IS_NULL(x) (x.id == 0)
#endif // !VD_UI_TEXTURE_ID_STRUCT_DEFINED

typedef struct {
    uintptr_t id;
} VdUiFontId;

typedef struct {
    VdUiFontId    cached_id;
    VdUiTextureId selected_texture;
    unsigned int  first_instance;
    unsigned int  instance_count;
} VdUiRenderPass;

typedef enum {
    VD_UI_UPDATE_TYPE_NEW_TEXTURE,
    VD_UI_UPDATE_TYPE_WRITE_TEXTURE,
} VdUiUpdateType;

typedef enum {
    VD_UI_TEXTURE_FORMAT_R8,
} VdUiTextureFormat;

typedef struct {
    VdUiUpdateType type;
    union {
        struct {
            int               width, height; // in:  width and height of the texture
            VdUiTextureFormat format;        // in:  format of the texture
            void              *buffer;       // in:  texture buffer
            size_t            size;          // in:  texture buffer size
            VdUiTextureId     *write_id;     // out: write the uploaded texture id
        } new_texture;

        struct {
            int               width, height;
            VdUiTextureFormat format;        // in:  format of the texture
            void              *buffer;
            size_t            size;
            VdUiTextureId     texture;
        } write_texture;
    } data;
} VdUiUpdate;

/**
 * Get the updates required to render the ui. You must handle all of these before any rendering of the UI occurs.
 * @param  num_updates The number of updates.
 * @return             The updates array.
 */
extern VdUiUpdate*      vd_ui_frame_get_updates(size_t *num_updates);

/**
 * Get the vertex buffer for the frame, to pass into your rendering code
 * @param buffer_size The size of the buffer, in bytes.
 * @return The pointer to the vertex buffer data.
 */
extern void*            vd_ui_frame_get_vertex_buffer(size_t *buffer_size);

/**
 * Get the render passes need to draw the whole frame.
 * @param  num_passes The number of passes to draw
 * @return            The pass data
 */
extern VdUiRenderPass*  vd_ui_frame_get_render_passes(int *num_passes);

/* ----FONTS--------------------------------------------------------------------------------------------------------- */
extern VdUiFontId       vd_ui_font_add_ttf(void *buffer, size_t size, float pixel_size);

/* ----INPUT--------------------------------------------------------------------------------------------------------- */
extern void             vd_ui_event_mouse_location(float mx, float my);

/* ----CONTEXT CREATION---------------------------------------------------------------------------------------------- */
typedef struct VdUiContext VdUiContext;
typedef struct {
    int a;
} VdUiContextCreateInfo;

extern VdUiContext*     vd_ui_context_create(VdUiContextCreateInfo *info);
extern void             vd_ui_context_set(VdUiContext *context);
extern VdUiContext*     vd_ui_context_get(void);

/* ----UI LAYOUT----------------------------------------------------------------------------------------------------- */
extern VdUiDiv*         vd_ui_div_new(VdStr string);
extern VdUiDiv*         vd_ui_push_parent(VdUiDiv *div);
extern VdUiDiv*         vd_ui_pop_parent(void);

/* ----GLYPH CACHE--------------------------------------------------------------------------------------------------- */
extern void             vd_ui_measure_text_size(VdUiFontId font, char *str, size_t len, float *w, float *h);

/* ----INTEGRATION - OPENGL------------------------------------------------------------------------------------------ */
/**
 * Gets the default shader sources for OpenGL
 * @param  vertex_shader       The vertex shader source
 * @param  vertex_shader_len   The length of the shader in bytes
 * @param  fragment_shader     The fragment shader source
 * @param  fragment_shader_len The length of the fragment shader in bytes
 */
extern void             vd_ui_gl_get_default_shader_sources(const char **const vertex_shader, size_t *vertex_shader_len,
                                                            const char **const fragment_shader, size_t *fragment_shader_len);
extern const char*      vd_ui_gl_get_uniform_name_resolution(void);
extern const char*      vd_ui_gl_get_uniform_name_texture(void);

/* ----HELPERS------------------------------------------------------------------------------------------------------- */
static inline int vd_ui_strlen(const char *s)
{
    const char *c = s;
    int l = 0;
    while (*c++) l++;
    return l;
}

#endif // !VD_UI_H

/* ------------------------------------------------------------------------------------------------------------------ */
/* IMPLEMENTATION                                                                                                   - */
/* ------------------------------------------------------------------------------------------------------------------ */

#ifdef VD_UI_IMPL
#define STB_TRUETYPE_IMPLEMENTATION
#include "ext/stb_truetype.h"

#define VD_UI_PARENT_STACK_MAX      256
#define VD_UI_VBUF_COUNT_MAX        128
#define VD_UI_RP_COUNT_MAX          12
#define VD_UI_FONT_COUNT_MAX        4
#define VD_UI_UPDATE_COUNT_MAX      2
#define VD_UI_GLYPH_CACHE_COUNT_MAX 2048
#define VD_UI_LOG(x, ...)           printf("VDUI: " x "\n", __VA_ARGS__)
static VdUiContext *Vd_Ui_Global_Context = 0;

typedef struct VdUiFont  VdUiFont;
typedef struct VdUiGlyph VdUiGlyph;

typedef enum {
    VD_UI__TEXTURE_STATE_NULL = 0,
    VD_UI__TEXTURE_STATE_NEEDS_UPDATE,
    VD_UI__TEXTURE_STATE_IN_GPU,
} VdUi__TextureState;

struct VdUiGlyph {
    unsigned int  codepoint;
    VdUiTextureId texture;
    VdUiFontId    font;
    int           x0, y0, x1, y1;
    float         xadvance, xoff, yoff;
    float         xoff2, yoff2;
    int           next;
};

struct VdUiFont {
    void                *font_buffer;
    stbtt_fontinfo      font_info;
    float               size;
    float               size_scaled;
    int                 bounding_box[4];
};

static void vd_ui__update_all_fonts(VdUiContext *ctx);
static void vd_ui__push_vertex(VdUiContext *ctx, float p0[2], float p1[2],
                                                 float u0[2], float u1[2],
                                                 float color[4]);
static void vd_ui__put_line(VdUiContext *ctx, const char *str, int len);

static void vd_ui__get_glyph_quad(VdUiContext *ctx, unsigned int codepoint, VdUiFontId font_id,
                                  float *rx, float *ry,
                                  float *x0, float *y0,
                                  float *x1, float *y1,
                                  float *s0, float *t0,
                                  float *s1, float *t1);
static VdUiGlyph*   vd_ui__push_glyph(VdUiContext *ctx, unsigned int codepoint, VdUiFontId font_id);
static size_t       vd_ui__hash_glyph(unsigned int codepoint, VdUiFontId font_id);

struct VdUiContext {
    VdUiDiv                 *parents[VD_UI_PARENT_STACK_MAX];
    VdUiDiv                 root;

    VdUiVertex              vbuf[VD_UI_VBUF_COUNT_MAX];
    unsigned int            vbuf_count;
    VdUiRenderPass          passes[VD_UI_RP_COUNT_MAX];

    unsigned int            num_updates;
    VdUiUpdate              updates[VD_UI_UPDATE_COUNT_MAX];

    unsigned int            num_fonts;
    VdUiFont                fonts[VD_UI_FONT_COUNT_MAX];

    // Glyph Cache
    // Plan (for now):
    // - LUT for codepoint to glyph index
    // - Dynamically growing list of glyphs that are indexed into
    // - For each glyph, ref count every frame
    // - Anything that hasn't been used for some time gets paged out
    // - If a new character is detected, cache it and store it in a per frame buffer
    // - At the end of the frame, compute the required new chars (if any), and render
    // - to bitmap
    VdUiGlyph               *glyph_cache;
    int                     atlas[2];
    void                    *buffer;
    size_t                  buffer_size;
    VdUiTextureId           texture;
    VdUi__TextureState      state;

    // @todo(mdodis): cache packs to textures and do them all in one go
    stbtt_pack_context      pack_context;

    float             delta_seconds;
    float             mouse[2];
};

void vd_ui_frame_begin(float delta_seconds)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->delta_seconds = delta_seconds;
}

void vd_ui_frame_end(void)
{
    VdUiContext *ctx = vd_ui_context_get();

    float mx = ctx->mouse[0];
    float my = ctx->mouse[1];
    (void)mx;
    (void)my;

    // Zero immediate mode stuff
    ctx->vbuf_count = 0;
    // ctx->num_passes = 0;
    ctx->num_updates = 0;


    vd_ui__put_line(ctx, "Woohoo!", strlen("Woohoo!"));
    // Write buffer
    // ctx->vbuf_count = 3;
    // ctx->vbuf[0] = (VdUiVertex) {
    //     {0.0f,  0.0f}, {200.f, 200.f},      {00.0f, 00.0f}, { 1.0f,  1.0f}, {1.f, 0.f, 0.f, 1.f}
    // };
    // ctx->vbuf[1] = (VdUiVertex) {
    //     {200.f, 0.0f}, {400.f, 400.f},      {00.0f, 00.0f}, { 1.0f,  1.0f}, {0.f, 1.f, 0.f, 1.f}
    // };
    // ctx->vbuf[2] = (VdUiVertex) {
    //     {mx, my}, {mx + 200.f, my + 200.f}, {00.0f, 00.0f}, { 1.0f,  1.0f}, {0.f, 0.f, 1.f, 1.f}
    // };

    // Process updates
    vd_ui__update_all_fonts(ctx);

    // Write render passes
    ctx->passes[0] = (VdUiRenderPass) {
        {0},
        {0},
        0,
        ctx->vbuf_count,
    };

}

size_t vd_ui_get_min_vertex_buffer_size(void)
{
    return sizeof(VdUiVertex) * VD_UI_VBUF_COUNT_MAX;
}

VdUiUpdate *vd_ui_frame_get_updates(size_t *num_updates)
{
    VdUiContext *ctx = vd_ui_context_get();
    *num_updates = ctx->num_updates;
    return ctx->updates;
}

void *vd_ui_frame_get_vertex_buffer(size_t *buffer_size)
{
    VdUiContext *ctx = vd_ui_context_get();

    *buffer_size = ctx->vbuf_count * sizeof(VdUiVertex);
    return (void*)ctx->vbuf;
}

VdUiRenderPass *vd_ui_frame_get_render_passes(int *num_passes)
{
    VdUiContext *ctx = vd_ui_context_get();

    *num_passes = 1;

    // Update font ids in render passes
    for (int i = 0; i < *num_passes; ++i) {
        VdUiRenderPass *pass = &ctx->passes[i];
        pass->selected_texture = ctx->texture;
    }

    return ctx->passes; 
}

/* ----FONTS IMPL---------------------------------------------------------------------------------------------------- */
VdUiFontId vd_ui_font_add_ttf(void *buffer, size_t size, float pixel_size)
{
    VD_UNUSED(size);

    VdUiContext *ctx = vd_ui_context_get();
    VdUiFont *font = &ctx->fonts[ctx->num_fonts++];

    font->font_buffer = buffer; 
    stbtt_InitFont(&font->font_info, buffer, stbtt_GetFontOffsetForIndex(buffer, 0));
    font->size = pixel_size;
    font->size_scaled = stbtt_ScaleForPixelHeight(&font->font_info, pixel_size);
    stbtt_GetFontBoundingBox(&font->font_info, &font->bounding_box[0], &font->bounding_box[1],
                                               &font->bounding_box[2], &font->bounding_box[3]);
    VdUiFontId result;
    result.id = ctx->num_fonts - 1;
    return result;
}

/* ----INPUT IMPL---------------------------------------------------------------------------------------------------- */
void vd_ui_event_mouse_location(float mx, float my)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->mouse[0] = mx;
    ctx->mouse[1] = my;
}

/* ----GLYPH CACHE IMPL---------------------------------------------------------------------------------------------- */
static void vd_ui__update_all_fonts(VdUiContext *ctx)
{
    switch (ctx->state) {
        case VD_UI__TEXTURE_STATE_NULL: {
            VD_UI_LOG("Queued initial texture upload to GPU %d", ctx->state);
            VdUiUpdate *update = &ctx->updates[ctx->num_updates++];
            update->type = VD_UI_UPDATE_TYPE_NEW_TEXTURE;
            update->data.new_texture.width    = ctx->atlas[0];
            update->data.new_texture.height   = ctx->atlas[1];
            update->data.new_texture.format   = VD_UI_TEXTURE_FORMAT_R8;
            update->data.new_texture.buffer   = ctx->buffer;
            update->data.new_texture.size     = ctx->buffer_size;
            update->data.new_texture.write_id = &ctx->texture;
        } break;

        case VD_UI__TEXTURE_STATE_NEEDS_UPDATE: {
            VD_UI_LOG("Queued subsequent texture upload to GPU %d", ctx->state);
            VdUiUpdate *update = &ctx->updates[ctx->num_updates++];
            update->type = VD_UI_UPDATE_TYPE_WRITE_TEXTURE;

            update->data.write_texture.width    = ctx->atlas[0];
            update->data.write_texture.height   = ctx->atlas[1];
            update->data.write_texture.format   = VD_UI_TEXTURE_FORMAT_R8;
            update->data.write_texture.buffer   = ctx->buffer;
            update->data.write_texture.size     = ctx->buffer_size;
            update->data.write_texture.texture  = ctx->texture;
            
        } break;

        default: break;
    }

    ctx->state = VD_UI__TEXTURE_STATE_IN_GPU;
}

static void vd_ui__push_vertex(VdUiContext *ctx, float p0[2], float p1[2],
                                                 float u0[2], float u1[2],
                                                 float color[4])
{
    VdUiVertex *v = &ctx->vbuf[ctx->vbuf_count++];
    v->p0[0] = p0[0];
    v->p0[1] = p0[1];
    v->p1[0] = p1[0];
    v->p1[1] = p1[1];
    v->p0u[0] = u0[0];
    v->p0u[1] = u0[1];
    v->p1u[0] = u1[0];
    v->p1u[1] = u1[1];
    v->color[0] = color[0];
    v->color[1] = color[1];
    v->color[2] = color[2];
    v->color[3] = color[3];
}

static void vd_ui__put_line(VdUiContext *ctx, const char *str, int len)
{
    VdUiFont *font = &ctx->fonts[0];

    float rp[2] = {ctx->mouse[0], ctx->mouse[1]};
    int ascent, descent, linegap;
    stbtt_GetFontVMetrics(&font->font_info, &ascent, &descent, &linegap);

    float vadvance = (ascent - descent + linegap) * font->size_scaled;
    VD_UNUSED(vadvance);
    
    for (int i = 0; i < len; ++i) {
        char c = str[i];

        float p0[2], p1[2];
        float u0[2], u1[2];

        vd_ui__get_glyph_quad(ctx, (int)c, (VdUiFontId) {0},
            &rp[0], &rp[1],
            &p0[0], &p0[1],
            &p1[0], &p1[1],
            &u0[0], &u0[1],
            &u1[0], &u1[1]);

        vd_ui__push_vertex(ctx,
            p0, p1,
            u0, u1,
            (float[]){1.f, 0.f, 1.f, 1.f});
    }
}

static int vd_ui__glyph_eq(VdUiGlyph *glyph, unsigned int codepoint, VdUiFontId font)
{
    return (glyph->codepoint == codepoint) && (glyph->font.id == font.id);
}

static int vd_ui__glyph_free(VdUiGlyph *glyph)
{
    return glyph->codepoint == 0;
}

static void vd_ui__get_glyph_quad(VdUiContext *ctx, unsigned int codepoint, VdUiFontId font_id,
                                  float *rx, float *ry,
                                  float *x0, float *y0,
                                  float *x1, float *y1,
                                  float *s0, float *t0,
                                  float *s1, float *t1)
{
    size_t hash = vd_ui__hash_glyph(codepoint, font_id);
    size_t hindex = hash % VD_UI_GLYPH_CACHE_COUNT_MAX;
    size_t index = hindex;

    // Lookup glyph
    int found_glyph = 1;
    while (!vd_ui__glyph_eq(&ctx->glyph_cache[index], codepoint, font_id)) {
        if (ctx->glyph_cache[index].next == -1) {
            found_glyph = 0;
            break;
        }

        index = ctx->glyph_cache[index].next;
    }

    VdUiGlyph *glyph = &ctx->glyph_cache[index];

    if (!found_glyph) {
        // @todo(mdodis): Glyph paging
        // We didn't find the glyph. Allocate a new one if it fits
        // and assert if it doesn't (for now)
        glyph = vd_ui__push_glyph(ctx, codepoint, font_id);
    }

    int align_to_integer = 1;
    float ipw = 1.0f / (ctx->atlas[0]); float iph = 1.0f / (ctx->atlas[1]);

    if (align_to_integer) {
        float x = ((int)floor((*rx) + glyph->xoff + 0.5f));
        float y = ((int)floor((*ry) + glyph->yoff + 0.5f));
        *x0 = x;               *x1 = x + glyph->xoff2 - glyph->xoff;
        *y0 = y;               *y1 = y + glyph->yoff2 - glyph->yoff;

    } else {
        *x0 = (*rx) + glyph->xoff;     *x1 = (*rx) + glyph->xoff2;
        *y0 = (*ry) + glyph->yoff;     *y1 = (*ry) + glyph->yoff2;
    }

    *s0 = (glyph->x0) * ipw;  *s1 = (glyph->x1) * ipw;
    *t0 = (glyph->y0) * iph;  *t1 = (glyph->y1) * iph;

    *rx += glyph->xadvance;
}

VdUiGlyph *vd_ui__push_glyph(VdUiContext *ctx, unsigned int codepoint, VdUiFontId font_id)
{
    ctx->state = ctx->state == VD_UI__TEXTURE_STATE_IN_GPU ? VD_UI__TEXTURE_STATE_NEEDS_UPDATE : VD_UI__TEXTURE_STATE_NULL;
    VdUiFont *font = &ctx->fonts[font_id.id];

    stbtt_packedchar packed_char;
    int result = stbtt_PackFontRange(
        &ctx->pack_context, font->font_buffer, 0,
        font->size,
        codepoint, 1, &packed_char);

    VD_UI_LOG("Pushed codepoint %c to packed range", (char)codepoint);


    VD_ASSERT(result != 0);

    size_t index = vd_ui__hash_glyph(codepoint, font_id) % VD_UI_GLYPH_CACHE_COUNT_MAX;

    if (vd_ui__glyph_free(&ctx->glyph_cache[index])) {
        VdUiGlyph *glyph = &ctx->glyph_cache[index];
        glyph->codepoint = codepoint;
        glyph->font = font_id;
        glyph->x0 = packed_char.x0; glyph->y0 = packed_char.y0;
        glyph->x1 = packed_char.x1; glyph->y1 = packed_char.y1;
        glyph->xoff = packed_char.xoff;
        glyph->yoff = packed_char.yoff;
        glyph->xoff2 = packed_char.xoff2;
        glyph->yoff2 = packed_char.yoff2;
        glyph->xadvance = packed_char.xadvance;
        return glyph;
    } else {
        VD_TODO();
    }

}

static size_t vd_ui__hash_glyph(unsigned int codepoint, VdUiFontId font_id)
{
    size_t result = vd_dhash64(&codepoint, sizeof(codepoint));
    result = result ^ vd_dhash64(&font_id, sizeof(font_id));
    return result;
}

void vd_ui_measure_text_size(VdUiFontId font, char *str, size_t len, float *w, float *h)
{
    VdUiContext *ctx = vd_ui_context_get();
    (void)ctx;

    // @todo(mdodis): UTF-8
    for (size_t i = 0; i < len; ++i) {
        char c = str[i];
        (void)c;

        // vd_ui_codepoint_metrics((int)c, );
    }
}

void vd_ui_init(void)
{
    vd_ui_context_set(vd_ui_context_create(0));
}

VdUiContext *vd_ui_context_create(VdUiContextCreateInfo *info)
{
    VD_UNUSED(info);
#ifdef VD_H
    VdUiContext *result = VD_MALLOC(sizeof(VdUiContext));
    VD_MEMSET(result, 0, sizeof(*result));

    result->glyph_cache = (VdUiGlyph*)VD_MALLOC(sizeof(VdUiGlyph) * VD_UI_GLYPH_CACHE_COUNT_MAX);
    for (int i = 0; i < VD_UI_GLYPH_CACHE_COUNT_MAX; ++i) {
        result->glyph_cache[i].codepoint = 0;
        result->glyph_cache[i].texture.id = 0;
        result->glyph_cache[i].font.id = 0;
        result->glyph_cache[i].next = -1;
    }

    result->atlas[0] = 512;
    result->atlas[1] = 512;
    result->buffer_size = result->atlas[0] * result->atlas[1];
    result->buffer = (unsigned char*)VD_MALLOC(result->buffer_size);

    stbtt_PackBegin(&result->pack_context, result->buffer,
                    result->atlas[0], result->atlas[1], 0, 1, 0);
#else
#error "todo"
#endif
    return result;
}

void vd_ui_context_set(VdUiContext *context)
{
    Vd_Ui_Global_Context = context;
}

VdUiContext* vd_ui_context_get(void)
{
    return Vd_Ui_Global_Context;
}

/* ----INTEGRATION - OPENGL IMPL------------------------------------------------------------------------------------- */
#define VD_UI_GL_VERTEX_SHADER_SOURCE                                                                                  \
"#version 330 core                                                                                                 \n" \
"layout (location = 0) in vec2 a_p0;                                                                               \n" \
"layout (location = 1) in vec2 a_p1;                                                                               \n" \
"layout (location = 2) in vec2 a_p0u;                                                                              \n" \
"layout (location = 3) in vec2 a_p1u;                                                                              \n" \
"layout (location = 4) in vec4 a_color;                                                                            \n" \
"uniform vec2 uResolution;                                                                                         \n" \
"out vec4 f_color;                                                                                                 \n" \
"out vec2 f_uv;                                                                                                    \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"   vec2 positions[4] = vec2[](                                                                                    \n" \
"       vec2(-1.0, -1.0),                                                                                          \n" \
"       vec2(-1.0, +1.0),                                                                                          \n" \
"       vec2(+1.0, -1.0),                                                                                          \n" \
"       vec2(+1.0, +1.0)                                                                                           \n" \
"   );                                                                                                             \n" \
"   vec2 dhs = (a_p1 - a_p0) / 2.0;                                                                                \n" \
"   vec2 dhc = (a_p1 + a_p0) / 2.0;                                                                                \n" \
"   vec2 dp  = positions[gl_VertexID] * dhs + dhc;                                                                 \n" \
"   gl_Position = vec4(2.0 * dp.x / uResolution.x - 1.0,                                                           \n" \
"                      1.0 - 2.0 * dp.y / uResolution.y,                                                           \n" \
"                      0.0,                                                                                        \n" \
"                      1.0);                                                                                       \n" \
"                                                                                                                  \n" \
"   vec2 shs = (a_p1u - a_p0u) / 2.0;                                                                              \n" \
"   vec2 shc = (a_p1u + a_p0u) / 2.0;                                                                              \n" \
"   vec2 shp = positions[gl_VertexID] * shs + shc;                                                                 \n" \
"   f_uv = shp;                                                                                                    \n" \
"   f_color = a_color;                                                                                             \n" \
"}                                                                                                                 \n" \

#define VD_UI_GL_FRAGMENT_SHADER_SOURCE                                                                                \
"#version 330 core                                                                                                 \n" \
"in vec4 f_color;                                                                                                  \n" \
"in vec2 f_uv;                                                                                                     \n" \
"uniform sampler2D uTexture;                                                                                       \n" \
"out vec4 FragColor;                                                                                               \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"   vec4 sample = texture(uTexture, f_uv);                                                                         \n" \
"   vec4 color = vec4(f_color.xyz, sample.r);                                                                      \n" \
"   FragColor = color;                                                                                             \n" \
"}                                                                                                                 \n" \

void vd_ui_gl_get_default_shader_sources(const char **const vertex_shader, size_t *vertex_shader_len,
                                         const char **const fragment_shader, size_t *fragment_shader_len)
{
    *vertex_shader       = VD_UI_GL_VERTEX_SHADER_SOURCE;
    *fragment_shader     = VD_UI_GL_FRAGMENT_SHADER_SOURCE;
    *vertex_shader_len   = (sizeof(VD_UI_GL_VERTEX_SHADER_SOURCE));
    *fragment_shader_len = (sizeof(VD_UI_GL_FRAGMENT_SHADER_SOURCE));
}

const char *vd_ui_gl_get_uniform_name_resolution(void)
{
    return "uResolution";
}

const char *vd_ui_gl_get_uniform_name_texture(void)
{
    return "uTexture";
}

#endif // VD_UI_IMPL