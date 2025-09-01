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

#endif // !VD_UI_H

/* ------------------------------------------------------------------------------------------------------------------ */
/* IMPLEMENTATION                                                                                                   - */
/* ------------------------------------------------------------------------------------------------------------------ */

#ifdef VD_UI_IMPL
#define STB_TRUETYPE_IMPLEMENTATION
#include "ext/stb_truetype.h"

#define VD_UI_PARENT_STACK_MAX 256
#define VD_UI_VBUF_COUNT_MAX   128
#define VD_UI_RP_COUNT_MAX     12
#define VD_UI_FONT_COUNT_MAX   4
#define VD_UI_UPDATE_COUNT_MAX 2
static VdUiContext *Vd_Ui_Global_Context = 0;

static void vd_ui__update_all_fonts(VdUiContext *ctx);

typedef struct {
    void           *font_buffer;
    stbtt_fontinfo font_info;
    float          size;
    float          size_scaled;
    int            bounding_box[4];
    VdUiTextureId  texture;
    int            in_gpu;
    int            atlas[2];
    unsigned char  *buffer;
    size_t         buffer_size;
} VdUiFont;

struct VdUiContext {
    VdUiDiv           *parents[VD_UI_PARENT_STACK_MAX];
    VdUiDiv           root;

    VdUiVertex        vbuf[VD_UI_VBUF_COUNT_MAX];
    unsigned int      vbuf_count;
    VdUiRenderPass    passes[VD_UI_RP_COUNT_MAX];

    unsigned int      num_updates;
    VdUiUpdate        updates[VD_UI_UPDATE_COUNT_MAX];

    unsigned int      num_fonts;
    VdUiFont          fonts[VD_UI_FONT_COUNT_MAX];

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

    // Zero immediate mode stuff
    ctx->vbuf_count = 0;
    // ctx->num_passes = 0;
    ctx->num_updates = 0;

    // Process updates
    vd_ui__update_all_fonts(ctx);


    // Write buffer
    ctx->vbuf_count = 3;
    ctx->vbuf[0] = (VdUiVertex) {
        {0.0f,  0.0f}, {200.f, 200.f},      {00.0f, 00.0f}, { 1.0f,  1.0f}, {1.f, 0.f, 0.f, 1.f}
    };
    ctx->vbuf[1] = (VdUiVertex) {
        {200.f, 0.0f}, {400.f, 400.f},      {00.0f, 00.0f}, { 1.0f,  1.0f}, {0.f, 1.f, 0.f, 1.f}
    };
    ctx->vbuf[2] = (VdUiVertex) {
        {mx, my}, {mx + 200.f, my + 200.f}, {00.0f, 00.0f}, { 1.0f,  1.0f}, {0.f, 0.f, 1.f, 1.f}
    };

    // Write render passes
    ctx->passes[0] = (VdUiRenderPass) {
        {0},
        {0},
        0,
        3,
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
        pass->selected_texture = ctx->fonts[pass->cached_id.id].texture;
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
    font->texture.id = 0;
    font->in_gpu = 0;
    font->atlas[0] = 512;
    font->atlas[1] = 512;
    font->buffer_size = font->atlas[0] * font->atlas[1];
    font->buffer = (unsigned char*)VD_MALLOC(font->buffer_size);

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
    for (unsigned int fi = 0; fi < ctx->num_fonts; ++fi) {
        VdUiFont *font = &ctx->fonts[fi];
        if (!font->in_gpu) {
            font->in_gpu = 1;

            stbtt_pack_context pc;
            int first_unicode_char_in_range = 32;
            int num_chars_in_range          = 95;
            stbtt_packedchar packed_chars[95];
            stbtt_PackBegin(&pc,     font->buffer, font->atlas[0], font->atlas[1], 0, 1, 0);
            stbtt_PackFontRange(&pc, font->font_buffer, 0, font->size,
                                     first_unicode_char_in_range, num_chars_in_range, packed_chars);
            stbtt_PackEnd(&pc);

            // If font is not uploaded, then just push it
            VdUiUpdate *update = &ctx->updates[ctx->num_updates++];
            update->type = VD_UI_UPDATE_TYPE_NEW_TEXTURE;
            update->data.new_texture.width    = 512;
            update->data.new_texture.height   = 512;
            update->data.new_texture.format   = VD_UI_TEXTURE_FORMAT_R8;
            update->data.new_texture.buffer   = font->buffer;
            update->data.new_texture.size     = font->buffer_size;
            update->data.new_texture.write_id = &font->texture;
        }
    }
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