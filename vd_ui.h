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
 *
 * @todo(mdodis):
 * - RenderPass (texture) interleaving
 * - Proper ui layout
 * - Mouse Clicks
 * - Buttons and labels
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

#ifdef VD_UI_STATIC
#define VD_UI_API static
#else
#define VD_UI_API extern
#endif // VD_UI_STATIC

/**
 * To use a custom id for a texture, define a struct like so:
 * typedef struct {
 *     <Your custom texture id stuff>
 * } VdUiTextureId;
 */
#ifndef VD_UI_TEXTURE_ID_STRUCT_DEFINED
#define VD_UI_TEXTURE_ID_STRUCT_DEFINED 0
#endif // VD_UI_TEXTURE_ID_STRUCT_DEFINED

/* ----INITIALIZATION------------------------------------------------------------------------------------------------ */
/**
 * Initialize the ui.
 */
VD_UI_API void             vd_ui_init(void);

/**
 * Get the minimum size required to hold the vertex buffer. Using this, you should allocate the buffer
 * @return  The size (in bytes) of the vertex buffer.
 */
VD_UI_API size_t           vd_ui_get_min_vertex_buffer_size(void);

/**
 * Begin a new frame. Call this before any ui layout or rendering calls
 * @param delta_seconds The delta time, in seconds.
 */
VD_UI_API void             vd_ui_frame_begin(float delta_seconds);
/**
 * End the frame. Call this after any ui layout or drawing calls.
 */
VD_UI_API void             vd_ui_frame_end(void);

/* ----UI------------------------------------------------------------------------------------------------------------ */

typedef struct {
    char *s;
    int  l;
} VdUiStr;

typedef struct VdUiDiv VdUiDiv;
struct VdUiDiv {
    /** The first child of this div */
    VdUiDiv *first;
    /** The last child of this div (eqt. first->next->...) */
    VdUiDiv *last;
    /** The next sibling of this div */
    VdUiDiv *next;
    /** The prev sibling of this div */
    VdUiDiv *prev;
    /** The parent of this div */
    VdUiDiv *parent;

    size_t  h;
    size_t  last_frame_touched;

    VdUiDiv *hnext;
    VdUiDiv *hprev;

    float comp_pos_rel[2];
    float comp_size[2];
    float rect[4];
};

typedef struct {
    VdUiDiv *div;
    char clicked;
} VdUiReply;

#define VD_UI_LIT(s) ((VdUiStr) {s, sizeof(s) - 1})

VD_UI_API int              vd_ui_button(VdUiStr str);

VD_UI_API VdUiDiv*         vd_ui_div_new(VdUiStr str);

// Parent Stack
VD_UI_API void             vd_ui_parent_push(VdUiDiv *div);
VD_UI_API void             vd_ui_parent_pop(void);

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
#define VD_UI_TEXTURE_ID_MAKE_NULL(x) ((x).id = 0)
#define VD_UI_TEXTURE_ID_IS_NULL(x)   ((x).id == 0)
#endif // !VD_UI_TEXTURE_ID_STRUCT_DEFINED

typedef struct {
    uintptr_t id;
} VdUiFontId;

typedef struct {
    VdUiFontId    cached_id;
    VdUiTextureId *selected_texture;
    unsigned int  first_instance;
    unsigned int  instance_count;
} VdUiRenderPass;

typedef enum {
    VD_UI_UPDATE_TYPE_NEW_TEXTURE,
    VD_UI_UPDATE_TYPE_WRITE_TEXTURE,
} VdUiUpdateType;

typedef enum {
    VD_UI_TEXTURE_FORMAT_R8,
    VD_UI_TEXTURE_FORMAT_RGBA8,
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
VD_UI_API VdUiUpdate*      vd_ui_frame_get_updates(size_t *num_updates);

/**
 * Get the vertex buffer for the frame, to pass into your rendering code
 * @param buffer_size The size of the buffer, in bytes.
 * @return The pointer to the vertex buffer data.
 */
VD_UI_API void*            vd_ui_frame_get_vertex_buffer(size_t *buffer_size);

/**
 * Get the render passes need to draw the whole frame.
 * @param  num_passes The number of passes to draw
 * @return            The pass data
 */
VD_UI_API VdUiRenderPass*  vd_ui_frame_get_render_passes(int *num_passes);

/* ----FONTS--------------------------------------------------------------------------------------------------------- */
VD_UI_API VdUiFontId       vd_ui_font_add_ttf(void *buffer, size_t size, float pixel_size);

/* ----INPUT--------------------------------------------------------------------------------------------------------- */
VD_UI_API void             vd_ui_event_mouse_location(float mx, float my);
VD_UI_API void             vd_ui_event_size(float width, float height);


/* ----CONTEXT CREATION---------------------------------------------------------------------------------------------- */
typedef struct VdUiContext VdUiContext;
typedef struct {
    int a;
} VdUiContextCreateInfo;

VD_UI_API VdUiContext*     vd_ui_context_create(VdUiContextCreateInfo *info);
VD_UI_API void             vd_ui_context_set(VdUiContext *context);
VD_UI_API VdUiContext*     vd_ui_context_get(void);

/* ----GLYPH CACHE--------------------------------------------------------------------------------------------------- */
VD_UI_API void             vd_ui_measure_text_size(VdUiFontId font, char *str, size_t len, float *w, float *h);

/* ----INTEGRATION - OPENGL------------------------------------------------------------------------------------------ */
/**
 * Gets the default shader sources for OpenGL
 * @param  vertex_shader       The vertex shader source
 * @param  vertex_shader_len   The length of the shader in bytes
 * @param  fragment_shader     The fragment shader source
 * @param  fragment_shader_len The length of the fragment shader in bytes
 */
VD_UI_API void             vd_ui_gl_get_default_shader_sources(const char **const vertex_shader, size_t *vertex_shader_len,
                                                            const char **const fragment_shader, size_t *fragment_shader_len);
VD_UI_API const char*      vd_ui_gl_get_uniform_name_resolution(void);
VD_UI_API const char*      vd_ui_gl_get_uniform_name_texture(void);
/**
 * @param  format          The storage format to pass into OpenGL
 */
/**
 * Converts from VdUiTextureFormat into GLint internalformat and GLEnum format required by glTexImage2D
 * @param  format          The texture format (supplied by VdUiUpdate union members)
 * @param  level           2nd parameter of glTexImage2D
 * @param  internal_format 3rd parameter of glTexImage2D
 * @param  border          6th parameter of glTexImage2D
 * @param  texformat       7th parameter of glTexImage2D
 * @param  type            8th parameter of glTexImage2D
 */
VD_UI_API void             vd_ui_gl_cv_texture_format(VdUiTextureFormat format, int *level, int *internal_format, int *border, unsigned int *texformat, unsigned int *type);

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

static unsigned int Vd_Ui_White_Texture_Buffer[4*4] = {
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
    0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
};

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
static void         vd_ui__layout(VdUiContext *ctx);
static void         vd_ui__queue_render(VdUiContext *ctx);

struct VdUiContext {
    VdUiDiv                 root;

    VdUiDiv                 *parents[VD_UI_PARENT_STACK_MAX];
    unsigned int            parents_next;

    VdUiTextureId           white;

    // Resources
    unsigned int            vbuf_count;
    VdUiVertex              vbuf[VD_UI_VBUF_COUNT_MAX];

    unsigned int            num_passes;
    VdUiRenderPass          passes[VD_UI_RP_COUNT_MAX];

    unsigned int            num_updates;
    VdUiUpdate              updates[VD_UI_UPDATE_COUNT_MAX];

    unsigned int            num_fonts;
    VdUiFont                fonts[VD_UI_FONT_COUNT_MAX];

    // Divs
    VdUiDiv                 *divs;
    unsigned int            divs_cap;
    unsigned int            divs_cap_total;

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

    // Per frame info
    size_t                  frame_index;
    size_t                  last_frame_index;
    float                   delta_seconds;
    float                   mouse[2];
    float                   window[2];
};

VD_UI_API void vd_ui_frame_begin(float delta_seconds)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->root.first   = 0;
    ctx->root.next    = 0;
    ctx->root.last    = 0;
    ctx->root.prev    = 0;
    ctx->root.parent  = 0;
    vd_ui_parent_push(&ctx->root);

    ctx->delta_seconds = delta_seconds;

    ctx->last_frame_index = ctx->frame_index;
    ctx->frame_index++;

    if (ctx->frame_index == 0) {
        ctx->frame_index = 1;
    }
}

VD_UI_API void vd_ui_frame_end(void)
{
    VdUiContext *ctx = vd_ui_context_get();
    vd_ui_parent_pop();

    // Zero immediate mode stuff
    ctx->vbuf_count = 0;
    ctx->num_passes = 0;
    ctx->num_updates = 0;

    // vd_ui__put_line(ctx, "Woohoo!", strlen("Woohoo!"));

    // Layout UI
    vd_ui__layout(ctx);

    {
        VdUiTextureId current_texture_id = ctx->white;
        float r = 1.f;
        float g = 0.f;
        float b = 0.f;
        float a = 1.f;

        for (VdUiDiv *d = ctx->root.last; d != 0; d = d->prev) {

            vd_ui__push_vertex(ctx,
                (float[]){d->rect[0], d->rect[1]}, (float[]){d->rect[2], d->rect[3]},
                (float[]){0.f, 0.f},               (float[]){0.25f, 0.25f},
                (float[]){r, g, b, a});
            g += 0.2f;
        }
    }


    // Process updates
    vd_ui__update_all_fonts(ctx);

    // Write render passes
    ctx->passes[0] = (VdUiRenderPass) {
        {0},
        &ctx->white,
        0,
        ctx->vbuf_count,
    };

}

VD_UI_API size_t vd_ui_get_min_vertex_buffer_size(void)
{
    return sizeof(VdUiVertex) * VD_UI_VBUF_COUNT_MAX;
}

VD_UI_API VdUiUpdate *vd_ui_frame_get_updates(size_t *num_updates)
{
    VdUiContext *ctx = vd_ui_context_get();
    *num_updates = ctx->num_updates;
    return ctx->updates;
}

VD_UI_API void *vd_ui_frame_get_vertex_buffer(size_t *buffer_size)
{
    VdUiContext *ctx = vd_ui_context_get();

    *buffer_size = ctx->vbuf_count * sizeof(VdUiVertex);
    return (void*)ctx->vbuf;
}

VD_UI_API VdUiRenderPass *vd_ui_frame_get_render_passes(int *num_passes)
{
    VdUiContext *ctx = vd_ui_context_get();

    *num_passes = 1;

    // Update font ids in render passes
    for (int i = 0; i < *num_passes; ++i) {
        VdUiRenderPass *pass = &ctx->passes[i];
        // pass->selected_texture = ctx->texture;
    }

    return ctx->passes; 
}

/* ----UI IMPL------------------------------------------------------------------------------------------------------- */
static size_t vd_ui__hash(void *begin, int len)
{
    return vd_dhash64(begin, len);
}

VD_UI_API VdUiDiv *vd_ui_div_new(VdUiStr str)
{
    VdUiContext *ctx = vd_ui_context_get();

    size_t h = vd_ui__hash(str.s, str.l);
    size_t index = h % ctx->divs_cap;

    VdUiDiv *result;

    if (ctx->divs[index].last_frame_touched == ctx->last_frame_index && ctx->divs[index].h == h) {
        result = &ctx->divs[index];
    } else {
        // We need to traverse the hnext chain of index until we find something
        // @todo(mdodis): traversal

        // Otherwise, we need to allocate a new div
        ctx->divs[index].h = h;
        result = &ctx->divs[index];
        VD_UI_LOG("Allocated new Div at index: %zu with id %zu", index, result->h);
    }

    result->last_frame_touched = ctx->frame_index;
    result->first = 0;
    result->next = 0;
    result->last = 0;
    result->prev = 0;

    VdUiDiv *parent = ctx->parents[ctx->parents_next - 1];
    result->parent = parent;

    // If the parent widget never had any children, then its first and last child is this one
    if (!parent->first) {
        parent->first      = result;
        parent->last       = result;
    } else {
        parent->last->next = result;
        result->prev       = parent->last;
        parent->last       = result;
    }

    return result;
}

// Parent Stack Impl
VD_UI_API void vd_ui_parent_push(VdUiDiv *div)
{
    VdUiContext *ctx = vd_ui_context_get();
    VD_ASSERT(ctx->parents_next != VD_UI_PARENT_STACK_MAX && "Too many pushes to parents!");
    ctx->parents[ctx->parents_next++] = div;
}

VD_UI_API void vd_ui_parent_pop(void)
{
    VdUiContext *ctx = vd_ui_context_get();
    VD_ASSERT(ctx->parents_next != 0 && "Too many pops to parents!");
    ctx->parents_next--;
}

static void vd_ui__layout(VdUiContext *ctx)
{
    // Begin at root
    VdUiDiv *root = &ctx->root;
    root->rect[0] = 0.f;             root->rect[1] = 0.f;
    root->rect[2] = ctx->window[0];  root->rect[2] = ctx->window[1];

    // Simple top bottom layout
    float starty = 0.f;

    VdUiDiv *d = root->first;
    while (d != 0) {

        // X0, Y0
        d->rect[0] = 20.f; d->rect[1] = starty;
        d->rect[2] = 30.f; d->rect[3] = starty + 24.f;

        starty += 24.f + 8.f;
        d = d->next;
    }
}

/* ----FONTS IMPL---------------------------------------------------------------------------------------------------- */
VD_UI_API VdUiFontId vd_ui_font_add_ttf(void *buffer, size_t size, float pixel_size)
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
VD_UI_API void vd_ui_event_mouse_location(float mx, float my)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->mouse[0] = mx;
    ctx->mouse[1] = my;
}

VD_UI_API void vd_ui_event_size(float width, float height)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->window[0] = width;
    ctx->window[1] = height;
}

/* ----GLYPH CACHE IMPL---------------------------------------------------------------------------------------------- */
static void vd_ui__update_all_fonts(VdUiContext *ctx)
{
    if (VD_UI_TEXTURE_ID_IS_NULL(ctx->white))
    {
        VD_UI_LOG("%s", "Queued white texture upload to GPU");
        VdUiUpdate *update = &ctx->updates[ctx->num_updates++];
        update->type = VD_UI_UPDATE_TYPE_NEW_TEXTURE;
        update->data.new_texture.width    = 4;
        update->data.new_texture.height   = 4;
        update->data.new_texture.format   = VD_UI_TEXTURE_FORMAT_RGBA8;
        update->data.new_texture.buffer   = Vd_Ui_White_Texture_Buffer;
        update->data.new_texture.size     = sizeof(Vd_Ui_White_Texture_Buffer);
        update->data.new_texture.write_id = &ctx->white;

    }

    // switch (ctx->state) {
    //     case VD_UI__TEXTURE_STATE_NULL: {
    //         VD_UI_LOG("Queued initial texture upload to GPU %d", ctx->state);
    //         VdUiUpdate *update = &ctx->updates[ctx->num_updates++];
    //         update->type = VD_UI_UPDATE_TYPE_NEW_TEXTURE;
    //         update->data.new_texture.width    = ctx->atlas[0];
    //         update->data.new_texture.height   = ctx->atlas[1];
    //         update->data.new_texture.format   = VD_UI_TEXTURE_FORMAT_R8;
    //         update->data.new_texture.buffer   = ctx->buffer;
    //         update->data.new_texture.size     = ctx->buffer_size;
    //         update->data.new_texture.write_id = &ctx->texture;
    //     } break;

    //     case VD_UI__TEXTURE_STATE_NEEDS_UPDATE: {
    //         VD_UI_LOG("Queued subsequent texture upload to GPU %d", ctx->state);
    //         VdUiUpdate *update = &ctx->updates[ctx->num_updates++];
    //         update->type = VD_UI_UPDATE_TYPE_WRITE_TEXTURE;

    //         update->data.write_texture.width    = ctx->atlas[0];
    //         update->data.write_texture.height   = ctx->atlas[1];
    //         update->data.write_texture.format   = VD_UI_TEXTURE_FORMAT_R8;
    //         update->data.write_texture.buffer   = ctx->buffer;
    //         update->data.write_texture.size     = ctx->buffer_size;
    //         update->data.write_texture.texture  = ctx->texture;
            
    //     } break;

    //     default: break;
    // }

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

VD_UI_API void vd_ui_measure_text_size(VdUiFontId font, char *str, size_t len, float *w, float *h)
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

/* ----CONTEXT CREATION IMPL----------------------------------------------------------------------------------------- */
VD_UI_API void vd_ui_init(void)
{
    vd_ui_context_set(vd_ui_context_create(0));
}

VD_UI_API VdUiContext *vd_ui_context_create(VdUiContextCreateInfo *info)
{
    VD_UNUSED(info);
#ifdef VD_H
    // Context creation
    VdUiContext *result = VD_MALLOC(sizeof(VdUiContext));
    VD_MEMSET(result, 0, sizeof(*result));

    // Glyph Cache
    result->glyph_cache = (VdUiGlyph*)VD_MALLOC(sizeof(VdUiGlyph) * VD_UI_GLYPH_CACHE_COUNT_MAX);
    for (int i = 0; i < VD_UI_GLYPH_CACHE_COUNT_MAX; ++i) {
        result->glyph_cache[i].codepoint = 0;
        result->glyph_cache[i].texture.id = 0;
        result->glyph_cache[i].font.id = 0;
        result->glyph_cache[i].next = -1;
    }

    // Font Texture
    result->atlas[0] = 512;
    result->atlas[1] = 512;
    result->buffer_size = result->atlas[0] * result->atlas[1];
    result->buffer = (unsigned char*)VD_MALLOC(result->buffer_size);

    stbtt_PackBegin(&result->pack_context, result->buffer,
                    result->atlas[0], result->atlas[1], 0, 1, 0);

    // Divs & Ids
    result->divs_cap       = 1024;
    result->divs_cap_total = 1000;
    result->divs           = (VdUiDiv*)VD_MALLOC(result->divs_cap_total * sizeof(VdUiDiv));
    VD_MEMSET(result->divs, 0, result->divs_cap_total * sizeof(VdUiDiv));

    //

    VD_UI_TEXTURE_ID_MAKE_NULL(result->white);

#else
#error "todo"
#endif
    return result;
}

VD_UI_API void vd_ui_context_set(VdUiContext *context)
{
    Vd_Ui_Global_Context = context;
}

VD_UI_API VdUiContext* vd_ui_context_get(void)
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

VD_UI_API void vd_ui_gl_get_default_shader_sources(const char **const vertex_shader, size_t *vertex_shader_len,
                                         const char **const fragment_shader, size_t *fragment_shader_len)
{
    *vertex_shader       = VD_UI_GL_VERTEX_SHADER_SOURCE;
    *fragment_shader     = VD_UI_GL_FRAGMENT_SHADER_SOURCE;
    *vertex_shader_len   = (sizeof(VD_UI_GL_VERTEX_SHADER_SOURCE));
    *fragment_shader_len = (sizeof(VD_UI_GL_FRAGMENT_SHADER_SOURCE));
}

VD_UI_API const char *vd_ui_gl_get_uniform_name_resolution(void)
{
    return "uResolution";
}

VD_UI_API const char *vd_ui_gl_get_uniform_name_texture(void)
{
    return "uTexture";
}

VD_UI_API void vd_ui_gl_cv_texture_format(VdUiTextureFormat format, int *level, int *internal_format, int *border, unsigned int *texformat, unsigned int *type)
{
    switch (format) {
        case VD_UI_TEXTURE_FORMAT_R8: {
            *level           = 0;
            *internal_format = 0x1903; /* GL_RED */
            *border          = 0;
            *texformat       = 0x1903; /* GL_RED */
            *type            = 0x1401; /* GL_UNSIGNED_BYTE */
        } break;

        case VD_UI_TEXTURE_FORMAT_RGBA8: {
            *level           = 0;
            *internal_format = 0x1908; /* GL_RGBA8 */
            *border          = 0;
            *texformat       = 0x1908; /* GL_RGBA  */
            *type            = 0x1401; /* GL_UNSIGNED_BYTE */
        } break;

        default: {

        } break;
    }
}

#endif // VD_UI_IMPL