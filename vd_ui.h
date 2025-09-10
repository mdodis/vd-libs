/**
 * vd_ui.h - UI library
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
 * @todo(mdodis):
 * - Proper ui layout
 * - Buttons and labels
 * - Glyph Cache collision resolution
 * - Div cache collision resolution
 * - Images
 * - Support more of printf
 *
 * EXAMPLE - OpenGL (@todo)
 * 
 */
#ifndef VD_UI_H
#define VD_UI_H
#define VD_UI_VERSION_MAJOR    0
#define VD_UI_VERSION_MINOR    0
#define VD_UI_VERSION_PATCH    1
#define VD_UI_VERSION          ((VD_UI_VERSION_MAJOR << 16) | (VD_UI_VERSION_MINOR << 8) | (VD_UI_VERSION_PATCH))

#include <stdarg.h>

#ifndef VD_UNUSED
#define VD_UNUSED(x) ((void)(x))
#endif // !VD_UNUSED

#ifdef VD_UI_STATIC
#define VD_UI_API static
#else
#define VD_UI_API extern
#endif // VD_UI_STATIC

#ifndef VD_UI_INL
#define VD_UI_INL static inline
#endif // !VD_UI_INL

typedef char VdUiBool;

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

/* ----HELPERS------------------------------------------------------------------------------------------------------- */
VD_UI_API int              vd_ui_vsnprintf(char *s, size_t n, const char *format, va_list args);
static inline int          vd_ui_snprintf(char *s, size_t n, const char *format, ...);
static inline int          vd_ui_strlen(const char *s);
static inline void*        vd_ui_memcpy(void *dst, void *src, size_t count);

static inline int vd_ui_snprintf(char *s, size_t n, const char *format, ...) {
    va_list args;
    va_start(args, format);    
    int result = vd_ui_vsnprintf(s, n, format, args);
    va_end(args);

    return result;
}

static inline int vd_ui_strlen(const char *s)
{
    const char *c = s;
    int l = 0;
    while (*c++) l++;
    return l;
}

static inline void *vd_ui_memcpy(void *dst, void *src, size_t count)
{
    for (size_t i = 0; i < count; ++i) ((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
    return dst;
}

/* ----UI------------------------------------------------------------------------------------------------------------ */
typedef struct {
    char *s;
    int  l;
} VdUiStr;

typedef union {
    float e[4];
} VdUiF4;

enum {
    // Per div flags
    VD_UI_FLAG_TEXT             = 1 << 0,
    VD_UI_FLAG_BACKGROUND       = 1 << 1,
    VD_UI_FLAG_CLICKABLE        = 1 << 2,
    VD_UI_FLAG_CLIP_CONTENT     = 1 << 3,
    VD_UI_FLAG_FLEX_HORIZONTAL  = 1 << 4, // 0 here means Vertical

    // Mouse Enumerations
    VD_UI_MOUSE_LEFT        = 0,
    VD_UI_MOUSE_RIGHT       = 1,
    VD_UI_MOUSE_MIDDLE      = 2,

    // Positions
    VD_UI_LEFT = 0,
    VD_UI_TOP  = 1,
    VD_UI_RIGHT = 2,
    VD_UI_BOTTOM = 3,
};
typedef int VdUiFlags;

enum {
    VD_UI_SIZE_MODE_ABSOLUTE = 0,
    VD_UI_SIZE_MODE_TEXT_CONTENT = 1,
    VD_UI_SIZE_MODE_CONTAIN_CHILDREN = 2,
};
typedef int VdUiSizeMode;

enum {
    VD_UI_AXISH = 0,
    VD_UI_AXISV = 1,
    VD_UI_AXES = 2,
};

typedef struct {
    VdUiSizeMode mode;
    float        value;
    float        importance;
} VdUiSize;

typedef struct {
    float padding[4];
} VdUiStyle;

typedef struct VdUiDiv VdUiDiv;
struct VdUiDiv {
    /** The first child of this div */
    VdUiDiv     *first;
    /** The last child of this div (eqt. first->next->...) */
    VdUiDiv     *last;
    /** The next sibling of this div */
    VdUiDiv     *next;
    /** The prev sibling of this div */
    VdUiDiv     *prev;
    /** The parent of this div */
    VdUiDiv     *parent;

    VdUiFlags   flags;
    VdUiStyle   style;

    size_t      h;
    size_t      last_frame_touched;

    VdUiDiv     *hnext;
    VdUiDiv     *hprev;

    VdUiStr     content_str;

    VdUiSize    size[VD_UI_AXES];

    float       comp_pos_rel[VD_UI_AXES];
    float       comp_size[VD_UI_AXES];
    float       rect[4];

    float       hot_t;
    float       active_t;
};

typedef struct {
    VdUiDiv  *div;
    float    mouse[2];
    VdUiBool pressed;
    VdUiBool released;
    VdUiBool clicked;
    VdUiBool hovering;
} VdUiReply;

#define VD_UI_CHAR_BUF_COUNT    1024
#define VD_UI_LIT(s)            ((VdUiStr) {s, sizeof(s) - 1})
#define VD_UI_DOTTOSTR(fmt)                                                       \
    va_list args;                                                                 \
    va_start(args, fmt);                                                          \
    int __sz__ = vd_ui_vsnprintf(Vd_Ui_CharBuf, VD_UI_CHAR_BUF_COUNT, fmt, args); \
    va_end(args);                                                                 \
    VdUiStr str = {Vd_Ui_CharBuf, __sz__};


extern char Vd_Ui_CharBuf[VD_UI_CHAR_BUF_COUNT];

VD_UI_API void             vd_ui_demo(void);
VD_UI_API VdUiReply        vd_ui_button(VdUiStr str);
VD_UI_INL VdUiReply        vd_ui_buttonf(const char *label, ...)   { VD_UI_DOTTOSTR(label); return vd_ui_button(str); }
VD_UI_API void             vd_ui_label(VdUiStr str);
VD_UI_API void             vd_ui_labelf(const char *label, ...)    { VD_UI_DOTTOSTR(label); vd_ui_label(str); }
VD_UI_API VdUiReply        vd_ui_checkbox(int *b, VdUiStr str);
VD_UI_API VdUiReply        vd_ui_checkboxf(int *b, const char *label, ...) { VD_UI_DOTTOSTR(label); return vd_ui_checkbox(b, str); }

VD_UI_API VdUiDiv*         vd_ui_div_new(VdUiFlags flags, VdUiStr str);
VD_UI_API VdUiDiv*         vd_ui_div_newf(VdUiFlags flags, const char *fmt, ...);
VD_UI_API VdUiReply        vd_ui_call(VdUiDiv *div);

// Parent Stack
VD_UI_API void             vd_ui_parent_push(VdUiDiv *div);
VD_UI_API void             vd_ui_parent_pop(void);
VD_UI_API int              vd_ui_parent_count(void);
VD_UI_API VdUiDiv*         vd_ui_parent_get(int i);

/* ----RENDERING----------------------------------------------------------------------------------------------------- */
enum {
    VD_UI_VERTEX_FLAG_TEXTURE_IS_ALPHA_BUFFER = 1 << 0,
};
typedef unsigned int VdUiVertexFlags;

#pragma pack(push, 1)
typedef struct {
    float            p0[2];            //  2 x  4 =   8 bytes
    float            p1[2];            //  2 x  4 =   8 bytes
    float            p0u[2];           //  2 x  4 =   8 bytes
    float            p1u[2];           //  2 x  4 =   8 bytes
    float            color[16];        //  4 x 16 =  64 bytes
    float            alpha_mix;        //  1 x  4 =   4 bytes
    float            corner_radius;    //  1 x  4 =   4 bytes
    float            edge_softness;    //  1 x  4 =   4 bytes
    float            border_thickness; //  1 x  4 =   4 bytes
    unsigned char    padd[16];         // 16 x  1 =  16 bytes
} VdUiVertex;                          //         = 128 bytes
#pragma pack(pop)

#if !VD_UI_TEXTURE_ID_STRUCT_DEFINED
typedef struct {
    uintptr_t id;
} VdUiTextureId;
#define VD_UI_TEXTURE_ID_MAKE_NULL(x) ((x).id = 0)
#define VD_UI_TEXTURE_ID_IS_NULL(x)   ((x).id == 0)
#define VD_UI_TEXTURE_ID_COMPARE(x,y) ((x).id == (y).id)
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
VD_UI_API VdUiRenderPass*  vd_ui_frame_get_render_passes(unsigned int *num_passes);

/* ----FONTS--------------------------------------------------------------------------------------------------------- */
VD_UI_API VdUiFontId       vd_ui_font_add_ttf(void *buffer, size_t size, float pixel_size);

/* ----INPUT--------------------------------------------------------------------------------------------------------- */
VD_UI_API void             vd_ui_event_size(float width, float height);
VD_UI_API void             vd_ui_event_mouse_location(float mx, float my);
VD_UI_API void             vd_ui_event_mouse_button(int index, int down);
VD_UI_API void             vd_ui_event_mouse_wheel(float dx, float dy);


/* ----CONTEXT CREATION---------------------------------------------------------------------------------------------- */
typedef struct VdUiContext VdUiContext;
typedef struct {
    int a;
} VdUiContextCreateInfo;

VD_UI_API VdUiContext*     vd_ui_context_create(VdUiContextCreateInfo *info);
VD_UI_API void             vd_ui_context_set(VdUiContext *context);
VD_UI_API VdUiContext*     vd_ui_context_get(void);

/* ----GLYPH CACHE--------------------------------------------------------------------------------------------------- */
VD_UI_API void             vd_ui_measure_text_size(VdUiFontId font, VdUiStr str, float *w, float *h);

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
/**
 * Gets the uniform name for window resolution using the default OpenGL 3.3 core profile shader provided as an example by the library
 * @return  The uniform name to pass into glGetUniformLocation (which you can then store somewhere and pass to glUniform2f)
 */
VD_UI_API const char*      vd_ui_gl_get_uniform_name_resolution(void);

/**
 * Gets the uniform name for texture sampler index using the default OpenGL 3.3 core profile shader provider as an example by the library
 * @return  The uniform name to pass into glGetUniformLocation (which you can then store somewhere and pass to glUniform1i)
 */
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

/**
 * Gets the number of attributes present in the vertex buffer
 * @return  The number of attributes to create
 */
VD_UI_API int              vd_ui_gl_get_num_attributes(void);

/**
 * Gets the ith attribute's properties
 * @param  attribute  The attribute index                (1st parameter to glVertexAttribPointer)
 * @param  size       The number of elements             (2nd parameter to glVertexAttribPointer)
 * @param  type       The type of element                (3rd parameter to glVertexAttribPointer)
 * @param  normalized Whether the elemets are normalized (4th parameter to glVertexAttribPointer)
 * @param  stride     The stride of each attribute       (5th parameter to glVertexAttribPointer)
 * @param  pointer    The offset of the attribute        (6th parameter to glVertexAttribPointer)
 * @param  divisor    The input rate of the attribute    (2nd parameter to glVertexAttribDivisor)
 */
VD_UI_API void             vd_ui_gl_get_attribute_properties(int attribute, int *size, unsigned int *type, unsigned char *normalized, int *stride, void **pointer, unsigned int *divisor);

/* ----DEBUG--------------------------------------------------------------------------------------------------------- */
VD_UI_API void             vd_ui_debug_set_draw_cursor_on(VdUiBool on);
VD_UI_API void             vd_ui_debug_set_inspector_on(VdUiBool on);
VD_UI_API void             vd_ui_debug_set_metrics_on(VdUiBool on);

#endif // !VD_UI_H

/* ------------------------------------------------------------------------------------------------------------------ */
/* IMPLEMENTATION                                                                                                   - */
/* ------------------------------------------------------------------------------------------------------------------ */

#ifdef VD_UI_IMPL
#define STB_TRUETYPE_IMPLEMENTATION
#include "ext/stb_truetype.h"

#define VD_UI_PARENT_STACK_MAX      256
#define VD_UI_VBUF_COUNT_MAX        256
#define VD_UI_RP_COUNT_MAX          64
#define VD_UI_FONT_COUNT_MAX        4
#define VD_UI_UPDATE_COUNT_MAX      2
#define VD_UI_GLYPH_CACHE_COUNT_MAX 2048
#define VD_UI_FBUF_MAX              1024
#define VD_UI_LOG(x, ...)           printf("VDUI: " x "\n", __VA_ARGS__)
static VdUiContext *Vd_Ui_Global_Context = 0;
char Vd_Ui_CharBuf[VD_UI_CHAR_BUF_COUNT];

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
    int                 ascent, descent, linegap;
};

static void vd_ui__update_all_fonts(VdUiContext *ctx);
static void vd_ui__push_rect(VdUiContext *ctx, VdUiTextureId *texture, float rect[4], float color[4]);
static void vd_ui__push_rectgrad(VdUiContext *ctx, VdUiTextureId *texture, float rect[4], float color[16]);
static void vd_ui__push_vertex(VdUiContext *ctx, VdUiTextureId *texture, float p0[2], float p1[2],
                                                                         float u0[2], float u1[2],
                                                                         float color[4],
                                                                         VdUiVertexFlags flags);
static void vd_ui__push_vertexgrad(VdUiContext *ctx, VdUiTextureId *texture, float p0[2], float p1[2],
                                                                             float u0[2], float u1[2],
                                                                             float color[16],
                                                                             VdUiVertexFlags flags,
                                                                             float corner_radius,
                                                                             float edge_softness,
                                                                             float border_thickness);
static void vd_ui__put_line(VdUiContext *ctx, VdUiStr s, float x, float y);
static void vd_ui__put_linef(VdUiContext *ctx, float x, float y, const char *fmt, ...);

static void vd_ui__get_glyph_quad(VdUiContext *ctx, unsigned int codepoint, VdUiFontId font_id,
                                  float *rx, float *ry,
                                  float *x0, float *y0,
                                  float *x1, float *y1,
                                  float *s0, float *t0,
                                  float *s1, float *t1);
static VdUiGlyph*   vd_ui__push_glyph(VdUiContext *ctx, unsigned int codepoint, VdUiFontId font_id);
static size_t       vd_ui__hash_glyph(unsigned int codepoint, VdUiFontId font_id);
static void         vd_ui__layout(VdUiContext *ctx);
static VdUiStr      vd_ui__strbuf_dup(VdUiContext *ctx, VdUiStr str);
static void         vd_ui__traverse_and_render_divs(VdUiContext *ctx, VdUiDiv *curr);
static int          vd_ui__point_in_rect(float point[2], float r[4]);
static float        vd_ui__clampf01(float x);
static float        vd_ui__clampf(float x, float a, float b);
static float        vd_ui__lerp(float a, float b, float t);
static VdUiF4       vd_ui__lerp4(VdUiF4 a, VdUiF4 b, float t);

static void         vd_ui__do_inspector(VdUiContext *ctx);

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
    float                   wheel[2];
    float                   window[2];
    int                     mouse_left;
    int                     mouse_right;
    int                     mouse_middle;
    size_t                  hot;
    size_t                  active;

    // Per frame storage (strings)
    char                    *strbuf;
    size_t                  strbuf_cap;
    size_t                  strbuf_len;

    // Stored to differentiate between passes
    VdUiTextureId          *current_texture_id;

    struct {
        VdUiBool           metrics_on;
        VdUiBool           custom_cursor_on;
        VdUiBool           inspector_on;
    } debug;
};

VD_UI_API void vd_ui_frame_begin(float delta_seconds)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->root.first   = 0;
    ctx->root.next    = 0;
    ctx->root.last    = 0;
    ctx->root.prev    = 0;
    ctx->root.parent  = 0;
    ctx->wheel[0]     = 0.f;
    ctx->wheel[1]     = 0.f;
    vd_ui_parent_push(&ctx->root);

    ctx->delta_seconds = delta_seconds;

    ctx->last_frame_index = ctx->frame_index;
    ctx->frame_index++;

    if (ctx->frame_index == 0) {
        ctx->frame_index = 1;
    }

    ctx->strbuf_len = 0;
}

static void vd_ui__print_tree(VdUiDiv *now, int sp)
{
    if (now == 0) return;

    for (int i = 0; i < sp; ++i) printf("-");
    if (now->content_str.l == 0) {
        printf("<>\n");
    } else {
        printf("%.*s\n", now->content_str.l, now->content_str.s);
    }

    VdUiDiv *child = now->first;
    while (child) {
        vd_ui__print_tree(child, sp + 4);
        child = child->next;
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
    ctx->current_texture_id = 0;
    ctx->root.rect[0] = 0.f;
    ctx->root.rect[1] = 0.f;
    ctx->root.rect[2] = ctx->window[0];
    ctx->root.rect[3] = ctx->window[1];

    // vd_ui__put_line(ctx, "Woohoo!", strlen("Woohoo!"));

    // Layout UI
    vd_ui__layout(ctx);

    // vd_ui__print_tree(&ctx->root, 0);

    // Render
    vd_ui__traverse_and_render_divs(ctx, &ctx->root);

    if (ctx->debug.custom_cursor_on) {
        vd_ui__push_rect(ctx, &ctx->white,
            (float[]) {ctx->mouse[0], ctx->mouse[1], ctx->mouse[0] + 16.f, ctx->mouse[1] + 16.f},
            (float[]) {1.f, 1.f, 1.f , 1.f});
    }

    if (ctx->debug.inspector_on) {
        vd_ui__do_inspector(ctx);
    }

    if (ctx->debug.metrics_on) {
        vd_ui__put_linef(ctx, 0.f, 32.f, "VBUF: %d", ctx->vbuf_count);
    }

    // Process updates
    vd_ui__update_all_fonts(ctx);
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

    *buffer_size = (ctx->vbuf_count) * sizeof(VdUiVertex);
    return (void*)ctx->vbuf;
}

VD_UI_API VdUiRenderPass *vd_ui_frame_get_render_passes(unsigned int *num_passes)
{
    VdUiContext *ctx = vd_ui_context_get();
    *num_passes = ctx->num_passes;
    return ctx->passes; 
}

/* ----UI IMPL------------------------------------------------------------------------------------------------------- */
static size_t vd_ui__hash(void *begin, int len)
{
    return vd_dhash64(begin, len);
}

VD_UI_API VdUiReply vd_ui_button(VdUiStr str)
{
    VdUiDiv *div = vd_ui_div_new(VD_UI_FLAG_TEXT |
                                 VD_UI_FLAG_BACKGROUND |
                                 VD_UI_FLAG_CLICKABLE,
                                 str);

    div->size[0].mode  = VD_UI_SIZE_MODE_TEXT_CONTENT;
    div->size[1].mode  = VD_UI_SIZE_MODE_TEXT_CONTENT;
    div->style.padding[VD_UI_LEFT]   = 4.f;
    div->style.padding[VD_UI_TOP]    = 4.f;
    div->style.padding[VD_UI_RIGHT]  = 4.f;
    div->style.padding[VD_UI_BOTTOM] = 4.f;

    return vd_ui_call(div);
}

VD_UI_API void vd_ui_label(VdUiStr str)
{
    VdUiDiv *div = vd_ui_div_new(VD_UI_FLAG_TEXT,
                                 str);
    div->size[0].mode = VD_UI_SIZE_MODE_TEXT_CONTENT;
    div->size[1].mode = VD_UI_SIZE_MODE_TEXT_CONTENT;
}

VD_UI_API VdUiReply vd_ui_checkbox(int *b, VdUiStr str)
{
    // @todo(mdodis) hash results to same
    VdUiDiv *div = vd_ui_div_new(VD_UI_FLAG_FLEX_HORIZONTAL, str);
    div->size[0].mode = VD_UI_SIZE_MODE_CONTAIN_CHILDREN;
    div->size[1].mode = VD_UI_SIZE_MODE_CONTAIN_CHILDREN;
    vd_ui_parent_push(div);

    VdUiDiv *ckbx = vd_ui_div_newf(VD_UI_FLAG_BACKGROUND | VD_UI_FLAG_CLICKABLE, "##ckbx");
    ckbx->size[0].mode = VD_UI_SIZE_MODE_ABSOLUTE;
    ckbx->size[0].value = 32.f;
    ckbx->size[1].mode = VD_UI_SIZE_MODE_ABSOLUTE;
    ckbx->size[1].value = 32.f;
    VdUiReply reply = vd_ui_call(ckbx);

    if (reply.clicked) {
        *b = !(*b);
    }

    vd_ui_label(str);
    vd_ui_parent_pop();
    return (VdUiReply) {0};
}

VD_UI_API VdUiDiv *vd_ui_div_newf(VdUiFlags flags, const char *fmt, ...)
{
    static char buf[VD_UI_FBUF_MAX];

    va_list args;
    va_start(args, fmt);
    int result = vd_ui_vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    VD_ASSERT(result < sizeof(buf));

    VdUiStr str = {buf, result};

    return vd_ui_div_new(flags, str);
}

VD_UI_API VdUiDiv *vd_ui_div_new(VdUiFlags flags, VdUiStr str)
{
    VdUiContext *ctx = vd_ui_context_get();

    size_t ht = vd_ui__hash(str.s, str.l);
    size_t h = ht;

    for (int i = 0; i < vd_ui_parent_count(); ++i) {
        h = h ^ vd_ui_parent_get(i)->h;
    }

    size_t index = h % ctx->divs_cap;

    VdUiDiv *result;

    if (ctx->divs[index].last_frame_touched == ctx->last_frame_index && ctx->divs[index].h == h) {
        result = &ctx->divs[index];
    } else {
        // We need to traverse the hnext chain of index until we find something
        // @todo(mdodis): traversal

        // Otherwise, we need to allocate a new div
        result = &ctx->divs[index];
        VD_UI_LOG("Allocated new Div at index: %zu with id %zu", index, result->h);
    }

    result->last_frame_touched = ctx->frame_index;
    result->first = 0;
    result->next = 0;
    result->last = 0;
    result->prev = 0;
    result->flags = flags;
    result->style.padding[0] = 0.f;
    result->style.padding[1] = 0.f;
    result->style.padding[2] = 0.f;
    result->style.padding[3] = 0.f;
    result->h = h;
    result->content_str = vd_ui__strbuf_dup(ctx, str);

    VdUiDiv *parent = ctx->parents[ctx->parents_next - 1];
    result->parent = parent;

    // If the parent widget never had any children, then its first and last child is this one
    if (!parent->first) {
        parent->first      = result;
        parent->last       = result;
        // result->next       = result;
        // result->prev       = result;
    } else {
        // Insert result after parent->last in the circular list
        VdUiDiv *last = parent->last;
        last->next = result;
        result->prev = last;
        // result->next = first;
        // first->prev = result;
        parent->last = result;
    }

    return result;
}

VD_UI_API VdUiReply vd_ui_call(VdUiDiv *div)
{
    VdUiContext *ctx = vd_ui_context_get();
    float dt = ctx->delta_seconds;

    VdUiReply reply = {};
    reply.div = div;
    reply.mouse[0]  = ctx->mouse[0];    reply.mouse[1] = ctx->mouse[1];

    int hovered  = vd_ui__point_in_rect(ctx->mouse, div->rect);
    int pressed  = ctx->mouse_left;
    int released = !ctx->mouse_left;
    int clicked  = 0;

    if (div->flags & VD_UI_FLAG_CLICKABLE) {
        if (hovered && !pressed) {
            ctx->hot = div->h;
        }

        if (hovered && pressed) {
            ctx->active = div->h;
        }

        if (released && (ctx->active == div->h) && hovered) {
            clicked = 1;
        }

        if (released && (ctx->active == div->h)) {
            ctx->active = 0;
        }
    }

    float hot_speed = 2.f;
    float active_speed = 2.1f;
    div->hot_t    = vd_ui__lerp(div->hot_t, hovered ? 1.0f : 0.0f, dt * hot_speed);
    div->hot_t    = vd_ui__clampf01(div->hot_t);

    div->active_t = vd_ui__lerp(div->active_t, (pressed && hovered) ? 1.0f : 0.0f, dt * active_speed);
    div->active_t = vd_ui__clampf01(div->active_t);

    reply.hovering = (VdUiBool)hovered;
    reply.pressed = (VdUiBool)hovered && (VdUiBool)pressed;
    reply.clicked = (VdUiBool)clicked;

    return reply;
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

VD_UI_API int vd_ui_parent_count(void)
{
    VdUiContext *ctx = vd_ui_context_get();
    return ctx->parents_next - 1;
}

VD_UI_API VdUiDiv *vd_ui_parent_get(int i)
{
    VdUiContext *ctx = vd_ui_context_get();
    return ctx->parents[i + 1];
}

VD_UI_API void vd_ui_demo(void)
{
    static int button1_click_count = 0;
    if (vd_ui_buttonf("Button 1").clicked) {
        button1_click_count++;
        VD_UI_LOG("Clicked %d times", button1_click_count);
    }

    vd_ui_labelf("Button 1 Clicked: %d times", button1_click_count);

    static int checkbox = 0;
    vd_ui_checkboxf(&checkbox, "Show Hidden");

    if (checkbox) {

        vd_ui_labelf("Hidden content");
    }
}

static void vd_ui__calc_fixed_size(VdUiContext *ctx, VdUiDiv *curr)
{
    if (curr == 0) {
        return;
    }

    VdUiDiv *child = curr->first;
    while (child != 0) {
        vd_ui__calc_fixed_size(ctx, child);
        child = child->next;
    }

    for (int i = 0; i < VD_UI_AXES; ++i) {
        switch (curr->size[i].mode) {

            case VD_UI_SIZE_MODE_ABSOLUTE: {
                curr->comp_size[i] = curr->size[i].value;

                curr->comp_size[0] += curr->style.padding[0] + curr->style.padding[2];
                curr->comp_size[1] += curr->style.padding[1] + curr->style.padding[3];
            } break;

            case VD_UI_SIZE_MODE_TEXT_CONTENT: {
                VdUiFontId font_id = {0};
                float text_sizes[VD_UI_AXES];
                vd_ui_measure_text_size(font_id, curr->content_str, &text_sizes[0], &text_sizes[1]);

                curr->comp_size[i] = text_sizes[i];

                curr->comp_size[0] += curr->style.padding[0] + curr->style.padding[2];
                curr->comp_size[1] += curr->style.padding[1] + curr->style.padding[3];
            } break;

            default: break;
        }
    }

}

static void vd_ui__calc_dyn_size_down(VdUiContext *ctx, VdUiDiv *curr)
{
    if (curr == 0) {
        return;
    }

    float full_size[VD_UI_AXES] = {0.f, 0.f};

    VdUiDiv *child = curr->first;
    while (child != 0) {
        vd_ui__calc_dyn_size_down(ctx, child);
        full_size[0] += child->comp_size[0];
        full_size[1] += child->comp_size[1];
        child = child->next;
    }

    for (int i = 0; i < VD_UI_AXES; ++i) {
        switch (curr->size[i].mode) {

            case VD_UI_SIZE_MODE_CONTAIN_CHILDREN: {
                curr->comp_size[i] = full_size[i];
            } break;

            default: break;
        }
    }
}

static void vd_ui__calc_positions(VdUiContext *ctx, VdUiDiv *curr)
{
    if (curr == 0) {
        return;
    }

    VdUiDiv *child = curr->first;

    int daxis = VD_UI_AXISV;

    if (curr->flags & VD_UI_FLAG_FLEX_HORIZONTAL) {
        daxis = VD_UI_AXISH;
    }

    int faxis = daxis == VD_UI_AXISH ? VD_UI_AXISV : VD_UI_AXISH;
    int daxis2 = 0, faxis2 = 0;

    switch (daxis) {
        case VD_UI_AXISV: daxis2 = 3; faxis2 = 2; break;
        case VD_UI_AXISH: daxis2 = 2; faxis2 = 3; break;
        default: break;
    }

    float cursor[2] = {0.f, 0.f};

    while (child != 0) {

        child->comp_pos_rel[daxis] = cursor[daxis];
        child->comp_pos_rel[faxis] = 0.f;

        child->rect[daxis] = curr->rect[daxis] + child->comp_pos_rel[daxis];
        child->rect[faxis] = curr->rect[faxis] + child->comp_pos_rel[faxis];
        child->rect[daxis2] = child->rect[daxis] + child->comp_size[daxis];
        child->rect[faxis2] = child->rect[faxis] + child->comp_size[faxis];

        cursor[daxis] += child->comp_size[daxis];

        if (child->comp_size[faxis] > cursor[faxis]) {
            cursor[faxis] = child->comp_size[faxis];
        }

        child = child->next;
    }


    child = curr->first;
    while (child != 0) {
        vd_ui__calc_positions(ctx, child);
        child = child->next;
    }
}

static void vd_ui__layout(VdUiContext *ctx)
{
    // Calculate independent sizes
    vd_ui__calc_fixed_size(ctx, &ctx->root);

    // Calculate upwards-dependent sizes

    // Calculate downwards-dependent sizes
    vd_ui__calc_dyn_size_down(ctx, &ctx->root);

    vd_ui__calc_positions(ctx, &ctx->root);
}

static VdUiStr vd_ui__strbuf_dup(VdUiContext *ctx, VdUiStr str)
{
    VD_ASSERT((ctx->strbuf_len + str.l) <= ctx->strbuf_cap);

    char *result = ctx->strbuf + ctx->strbuf_len;
    VD_MEMCPY(result, str.s, str.l);
    ctx->strbuf_len += str.l;

    VdUiStr result_str = { result, str.l};
    return result_str;
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
    stbtt_GetFontVMetrics(&font->font_info, &font->ascent, &font->descent, &font->linegap);
    VdUiFontId result;
    result.id = ctx->num_fonts - 1;
    return result;
}

/* ----INPUT IMPL---------------------------------------------------------------------------------------------------- */
VD_UI_API void vd_ui_event_size(float width, float height)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->window[0] = width;
    ctx->window[1] = height;
}

VD_UI_API void vd_ui_event_mouse_location(float mx, float my)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->mouse[0] = mx;
    ctx->mouse[1] = my;
}

VD_UI_API void vd_ui_event_mouse_wheel(float dx, float dy)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->wheel[0] = dx;
    ctx->wheel[1] = dy;
}

VD_UI_API void vd_ui_event_mouse_button(int index, int down)
{
    VdUiContext *ctx = vd_ui_context_get();
    switch (index)
    {
        case VD_UI_MOUSE_LEFT:   ctx->mouse_left   = down; break;
        case VD_UI_MOUSE_RIGHT:  ctx->mouse_right  = down; break;
        case VD_UI_MOUSE_MIDDLE: ctx->mouse_middle = down; break;
        default: break;
    }
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

static void vd_ui__push_vertexgrad(VdUiContext *ctx, VdUiTextureId *texture, float p0[2], float p1[2],
                                                                             float u0[2], float u1[2],
                                                                             float color[16],
                                                                             VdUiVertexFlags flags,
                                                                             float corner_radius,
                                                                             float edge_softness,
                                                                             float border_thickness)
{
    if (ctx->current_texture_id != texture)
    {
        VD_ASSERT(ctx->num_passes < VD_UI_RP_COUNT_MAX);
        ctx->num_passes++;

        VdUiRenderPass *pass   = &ctx->passes[ctx->num_passes - 1];
        pass->selected_texture = texture;
        pass->first_instance   = ctx->vbuf_count;
        pass->instance_count   = 0;
        ctx->current_texture_id = texture;
    }

    VD_ASSERT(ctx->vbuf_count < VD_UI_VBUF_COUNT_MAX);

    // Ok so clearly there is an issue when submitting separate passes
    VdUiVertex *v = &ctx->vbuf[ctx->vbuf_count++];
    v->p0[0]   = p0[0];     v->p0[1]   = p0[1];
    v->p1[0]   = p1[0];     v->p1[1]   = p1[1];

    v->p0u[0]   = u0[0];    v->p0u[1]   = u0[1];
    v->p1u[0]   = u1[0];    v->p1u[1]   = u1[1];

    v->color[0] = color[0]; v->color[4] = color[4]; v->color[8]  = color[8];  v->color[12] = color[12];
    v->color[1] = color[1]; v->color[5] = color[5]; v->color[9]  = color[9];  v->color[13] = color[13];
    v->color[2] = color[2]; v->color[6] = color[6]; v->color[10] = color[10]; v->color[14] = color[14];
    v->color[3] = color[3]; v->color[7] = color[7]; v->color[11] = color[11]; v->color[15] = color[15];

    if (flags & VD_UI_VERTEX_FLAG_TEXTURE_IS_ALPHA_BUFFER) {
        v->alpha_mix = 1.0;
    } else {
        v->alpha_mix = 0.0;
    }

    v->corner_radius = corner_radius;
    v->edge_softness = edge_softness;
    v->border_thickness = border_thickness;

    ctx->passes[ctx->num_passes - 1].instance_count++;
}

static void vd_ui__push_vertex(VdUiContext *ctx, VdUiTextureId *texture, float p0[2], float p1[2],
                                                                         float u0[2], float u1[2],
                                                                         float color[4],
                                                                         VdUiVertexFlags flags)
{
    float colors[16] = {
        color[0], color[1], color[2], color[3],
        color[0], color[1], color[2], color[3],
        color[0], color[1], color[2], color[3],
        color[0], color[1], color[2], color[3],
    };
    vd_ui__push_vertexgrad(ctx, texture, p0, p1, u0, u1, colors, flags, 0.f, 0.f, 0.f);
}

static void vd_ui__push_rectgrad(VdUiContext *ctx, VdUiTextureId *texture, float rect[4], float color[16])
{

    vd_ui__push_vertexgrad(ctx, texture,
        (float[]){rect[0], rect[1]}, (float[]){rect[2], rect[3]},
        (float[]){0.0f   , 0.0f   }, (float[]){1.0f   , 1.0f   },
        color,
        0,
        2.f, 2.f, 0.f);
}

static void vd_ui__push_rect(VdUiContext *ctx, VdUiTextureId *texture, float rect[4], float color[4])
{
    vd_ui__push_vertex(ctx, texture,
        (float[]){rect[0], rect[1]}, (float[]){rect[2], rect[3]},
        (float[]){0.0f   , 0.0f   }, (float[]){1.0f   , 1.0f   },
        color,
        0);
}

static void vd_ui__put_line(VdUiContext *ctx, VdUiStr s, float x, float y)
{
    VdUiFont *font = &ctx->fonts[0];

    float hadd = (float)font->bounding_box[3] - (float)font->bounding_box[1];
    float rp[2] = {x, y + hadd * font->size_scaled * 0.5f};

    for (int i = 0; i < s.l; ++i) {
        char c = s.s[i];

        float p0[2], p1[2];
        float u0[2], u1[2];

        vd_ui__get_glyph_quad(ctx, (int)c, (VdUiFontId) {0},
            &rp[0], &rp[1],
            &p0[0], &p0[1],
            &p1[0], &p1[1],
            &u0[0], &u0[1],
            &u1[0], &u1[1]);

        vd_ui__push_vertex(ctx, &ctx->texture,
            p0, p1,
            u0, u1,
            (float[]){1.f, 1.f, 1.f, 1.f},
            VD_UI_VERTEX_FLAG_TEXTURE_IS_ALPHA_BUFFER);
    }
}

static void vd_ui__put_linef(VdUiContext *ctx, float x, float y, const char *fmt, ...)
{
    static char buf[VD_UI_FBUF_MAX];
    va_list args;
    va_start(args, fmt);
    int result = vd_ui_vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    VD_ASSERT(result < sizeof(buf));
    VdUiStr str = {buf, result};
    vd_ui__put_line(ctx, str, x, y);
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
        float x = (float)((int)floor((*rx) + glyph->xoff + 0.5f));
        float y = (float)((int)floor((*ry) + glyph->yoff + 0.5f));
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

    if (result == 0) {
        VD_TODO();
    }

    VD_ASSERT(result != 0);

    size_t index = vd_ui__hash_glyph(codepoint, font_id) % VD_UI_GLYPH_CACHE_COUNT_MAX;

    VdUiGlyph *glyph;
    if (vd_ui__glyph_free(&ctx->glyph_cache[index])) {
        glyph = &ctx->glyph_cache[index];
    } else {
        // We didn't find space to allocate the glyph

        // 1. Follow the glyph's next property until we find last glyph with next != -1
        VdUiGlyph *chain_end = &ctx->glyph_cache[index];
        while (!vd_ui__glyph_free(chain_end) && chain_end->next != -1)
        {
            chain_end = &ctx->glyph_cache[chain_end->next];
        }

        // 2. Allocate starting from the cellar
        int cindex = VD_UI_GLYPH_CACHE_COUNT_MAX - 1;

        VdUiGlyph *cglyph = &ctx->glyph_cache[cindex];
        while (!vd_ui__glyph_free(cglyph) && cindex != 0) {
            cindex--;
        }

        if (cindex == 0) {
            // @todo(mdodis): Page out a glyph (or maybe try to do it earlier?)
            VD_TODO();
        }

        glyph = cglyph;
        chain_end->next = cindex;
    }

    glyph->codepoint = codepoint;
    glyph->font = font_id;
    glyph->x0 = packed_char.x0; glyph->y0 = packed_char.y0;
    glyph->x1 = packed_char.x1; glyph->y1 = packed_char.y1;
    glyph->xoff = packed_char.xoff;
    glyph->yoff = packed_char.yoff;
    glyph->xoff2 = packed_char.xoff2;
    glyph->yoff2 = packed_char.yoff2;
    glyph->xadvance = packed_char.xadvance;
    glyph->next = -1;
    return glyph;

}

static size_t vd_ui__hash_glyph(unsigned int codepoint, VdUiFontId font_id)
{
    size_t result = vd_dhash64(&codepoint, sizeof(codepoint));
    result = result ^ vd_dhash64(&font_id, sizeof(font_id));
    return result;
}

VD_UI_API void vd_ui_measure_text_size(VdUiFontId font_id, VdUiStr str, float *w, float *h)
{
    VdUiContext *ctx = vd_ui_context_get();
    (void)ctx;

    *w = *h = 0.f;

    VdUiFont *font = &ctx->fonts[font_id.id];
    *h = (font->ascent - font->descent + font->linegap) * font->size_scaled;


    // @todo(mdodis): UTF-8
    for (size_t i = 0; i < str.l; ++i) {
        char c = str.s[i];

        float x0, y0, x1, y1, s0, t0, s1, t1;
        vd_ui__get_glyph_quad(ctx, (unsigned int)c, font_id,
            w, h,
            &x0, &y0,
            &x1, &y1,
            &s0, &t0,
            &s1, &t1);
    }

}

static void vd_ui__traverse_and_render_divs(VdUiContext *ctx, VdUiDiv *curr)
{
    if (curr == 0) {
        return;
    }

    // Post order: search process child first from last to first
    VdUiDiv *last_child  = curr->last;
    VdUiDiv *child = last_child;

    while (child != 0) {
        vd_ui__traverse_and_render_divs(ctx, child);
        child = child->prev;
    }
    
    if (curr == &ctx->root) {
        return;
    }

    // After we've processed every child, we're ready to render ourselves
    if (curr->flags & VD_UI_FLAG_BACKGROUND) {
        VdUiF4 base_color   = {0.9f, 0.4f, 0.4f, 0.5f};
        VdUiF4 hot_color    = {0.7f, 0.3f, 0.3f, 1.f};
        VdUiF4 active_color = {0.1f, 0.9f, 0.4f, 1.f};

        VdUiF4 final_color = vd_ui__lerp4(base_color, hot_color, curr->hot_t);
        final_color = vd_ui__lerp4(final_color, active_color, curr->active_t);
        vd_ui__push_rectgrad(ctx, &ctx->white, curr->rect, (float[]) {
            0.3f, 0.3f, 0.3f, 1.f,
            0.3f, 0.3f, 0.3f, 1.f,
            0.2f, 0.2f, 0.2f, 1.f,
            0.2f, 0.2f, 0.2f, 1.f,
        });
        // vd_ui__push_rect(ctx, &ctx->white, curr->rect, base_color.e);
    }

    if (curr->flags & VD_UI_FLAG_TEXT) {
        VdUiFont *font = &ctx->fonts[0];
        float ydown = (-font->descent) * font->size_scaled;
        vd_ui__put_line(ctx, curr->content_str, curr->rect[0] + curr->style.padding[0] * 2, curr->rect[1] + curr->style.padding[1] + ydown);
    }
}

static int vd_ui__point_in_rect(float point[2], float r[4])
{
    return (point[0] >= r[0]) && (point[0] <= r[2]) &&
           (point[1] >= r[1]) && (point[1] <= r[3]); 
}

static float vd_ui__clampf01(float x)
{
    return vd_ui__clampf(x, 0.f, 1.f);
}

static float vd_ui__clampf(float x, float a, float b)
{
    if (x < a) return a;
    if (x > b) return b;
    return x;
}

static float vd_ui__lerp(float a, float b, float t)
{
    return (1.0f - t) * a + t * b;
}

static VdUiF4 vd_ui__lerp4(VdUiF4 a, VdUiF4 b, float t)
{
    return (VdUiF4) {
        vd_ui__lerp(a.e[0], b.e[0], t),
        vd_ui__lerp(a.e[1], b.e[1], t),
        vd_ui__lerp(a.e[2], b.e[2], t),
        vd_ui__lerp(a.e[3], b.e[3], t),
    };
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
    result->divs_cap       = 1000;
    result->divs_cap_total = 1024;
    result->divs           = (VdUiDiv*)VD_MALLOC(result->divs_cap_total * sizeof(VdUiDiv));
    VD_MEMSET(result->divs, 0, result->divs_cap_total * sizeof(VdUiDiv));

    // White Texture
    VD_UI_TEXTURE_ID_MAKE_NULL(result->white);

    // Arena
    result->strbuf_cap  = 1024 * 5; // 5 Kilobytes of per frame string storage
    result->strbuf      = (char*)VD_MALLOC(result->strbuf_cap);
    result->strbuf_len  = 0;

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

/* ----HELPERS IMPL-------------------------------------------------------------------------------------------------- */
static void vd_ui__putc(char **buf, size_t *rm, int c, int *count) {
    if (*rm > 1) {
        **buf = (char)c;
        (*buf)++;
        (*rm)--;
    }
    (*count)++;
}

static void vd_ui__itoa(char **buf, size_t *rm, int *count, unsigned int value, unsigned int base, int uppercase)
{
    char tmp[32];
    const char *digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    int pos = 0;

    if (value == 0) {
        tmp[pos++] = '0';
    } else {
        while (value > 0 && pos < sizeof(tmp)) {
            tmp[pos++] = digits[value % base];
            value /= base;
        }
    }

    while (pos > 0) {
        vd_ui__putc(buf, rm, tmp[--pos], count);
    }
}

VD_UI_API int vd_ui_vsnprintf(char *s, size_t n, const char *format, va_list args)
{
    char   *buf  = s;
    size_t rm    = n;
    int    count = 0;

    while (*format) {
        if ((*format) != '%') {
            vd_ui__putc(&buf, &rm, *format++, &count);
            continue;
        }

        format++;
        switch (*format) {
            case 's': {
                const char *str = va_arg(args, const char *);

                while (*str) {
                    vd_ui__putc(&buf, &rm, *str++, &count);
                }
            } break;

            case 'd': {
                int val = va_arg(args, int);
                if (val < 0) {
                    vd_ui__putc(&buf, &rm, '-', &count);
                    val = -val;
                }

                vd_ui__itoa(&buf, &rm, &count, (unsigned)val, 10, 0);
            } break;

            default: {
                vd_ui__putc(&buf, &rm, '%', &count);
                vd_ui__putc(&buf, &rm, *format, &count);
            } break;
        }
        format++;
    }

    if (rm > 0) {
        *buf = '\0';
    } else if (n > 0) {
        s[n - 1] = '\0';
    }

    return count;
}

/* ----DEBUG IMPL---------------------------------------------------------------------------------------------------- */
VD_UI_API void vd_ui_debug_set_draw_cursor_on(VdUiBool on)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->debug.custom_cursor_on = on;
}

VD_UI_API void vd_ui_debug_set_inspector_on(VdUiBool on)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->debug.inspector_on = on;
}

VD_UI_API void vd_ui_debug_set_metrics_on(VdUiBool on)
{
    VdUiContext *ctx = vd_ui_context_get();
    ctx->debug.metrics_on = on;
}

static struct {
    int initialized;
    float height;
    float rect[4];
    struct {
        float offset;
        float size_h;
    } hierarchy;
} Vd_Ui_Inspector = {0};

static void vd_ui__inspector_do_hierarchy(VdUiContext *ctx, VdUiDiv *curr, float depth);

static void vd_ui__do_inspector(VdUiContext *ctx)
{
    if (!Vd_Ui_Inspector.initialized) {
        Vd_Ui_Inspector.initialized = 1;
        Vd_Ui_Inspector.height = 300.f;
        Vd_Ui_Inspector.rect[0] = 0.f;
        Vd_Ui_Inspector.rect[1] = ctx->window[1] - Vd_Ui_Inspector.height;
    }
    Vd_Ui_Inspector.rect[2] = ctx->window[0];
    Vd_Ui_Inspector.rect[3] = ctx->window[1];
    Vd_Ui_Inspector.hierarchy.size_h = Vd_Ui_Inspector.rect[2] - ctx->window[0] * 0.5f;

    Vd_Ui_Inspector.hierarchy.offset = 4.f;

    vd_ui__push_rect(ctx, &ctx->white, Vd_Ui_Inspector.rect, (float[]) {0.7f, 0.8f, 0.7f, 0.7f});

    vd_ui__inspector_do_hierarchy(ctx, &ctx->root, 0.f);
}

static void vd_ui__inspector_do_hierarchy(VdUiContext *ctx, VdUiDiv *curr, float depth)
{
    VdUiDiv *child = curr->first;

    float entry_rect[4] = {
        Vd_Ui_Inspector.rect[0] + depth,
        Vd_Ui_Inspector.rect[1] + Vd_Ui_Inspector.hierarchy.offset,
        Vd_Ui_Inspector.rect[0] + Vd_Ui_Inspector.hierarchy.size_h,
        Vd_Ui_Inspector.rect[1] + Vd_Ui_Inspector.hierarchy.offset + 32.f,
    };


    float entry_color[4] = {0.1f, 0.1f, 0.1f, 1.f};
    float entry_color_hovered[4] = {0.3f, 0.1f, 0.1f, 1.f};
    float *final_color = entry_color;

    if (vd_ui__point_in_rect(ctx->mouse, entry_rect)) {
        final_color = entry_color_hovered;

        float description_pos[2] = {
            Vd_Ui_Inspector.rect[0] + Vd_Ui_Inspector.hierarchy.size_h,
            Vd_Ui_Inspector.rect[1]
        };

        static char buf[1024];

        {
            int sz = vd_ui_snprintf(buf, sizeof(buf), "Rect: {%d %d %d %d}", (int)curr->rect[0], (int)curr->rect[1], (int)curr->rect[2], (int)curr->rect[3]);
            VdUiStr s = {buf, sz};
            vd_ui__put_line(ctx, s, description_pos[0], description_pos[1]);
        }

        {
            int sz = vd_ui_snprintf(buf, sizeof(buf), "Relative Size: {%d %d}", (int)curr->comp_size[0], (int)curr->comp_size[1]);
            VdUiStr s = {buf, sz};
            vd_ui__put_line(ctx, s, description_pos[0], description_pos[1] + 32.f);
        }

        {
            int sz = vd_ui_snprintf(buf, sizeof(buf), "Relative Pos: {%d %d}", (int)curr->comp_pos_rel[0], (int)curr->comp_pos_rel[1]);
            VdUiStr s = {buf, sz};
            vd_ui__put_line(ctx, s, description_pos[0], description_pos[1] + 64.f);
        }

    }

    vd_ui__push_rect(ctx, &ctx->white, entry_rect, final_color);

    if (curr->content_str.l != 0) {
        vd_ui__put_line(ctx, curr->content_str, entry_rect[0], entry_rect[1]);
    } else {
        vd_ui__put_line(ctx, VD_UI_LIT("#id"), entry_rect[0], entry_rect[1]);
    }

    Vd_Ui_Inspector.hierarchy.offset += 32.f;

    while (child != 0) {
        vd_ui__inspector_do_hierarchy(ctx, child, depth + 64.f);
        child = child->next;
    }
}

/* ----INTEGRATION - OPENGL IMPL------------------------------------------------------------------------------------- */
#define VD_UI_GL_VERTEX_SHADER_SOURCE                                                                                  \
"#version 330 core                                                                                                 \n" \
"layout (location = 0) in vec2 a_p0;                                                                               \n" \
"layout (location = 1) in vec2 a_p1;                                                                               \n" \
"layout (location = 2) in vec2 a_p0u;                                                                              \n" \
"layout (location = 3) in vec2 a_p1u;                                                                              \n" \
"layout (location = 4) in vec4 a_color0;                                                                           \n" \
"layout (location = 5) in vec4 a_color1;                                                                           \n" \
"layout (location = 6) in vec4 a_color2;                                                                           \n" \
"layout (location = 7) in vec4 a_color3;                                                                           \n" \
"layout (location = 8) in float a_amix;                                                                            \n" \
"layout (location = 9) in float a_corner_radius;                                                                   \n" \
"layout (location = 10) in float a_edge_softness;                                                                  \n" \
"uniform vec2 uResolution;                                                                                         \n" \
"out vec4 f_color;                                                                                                 \n" \
"out vec2 f_uv;                                                                                                    \n" \
"out float f_amix;                                                                                                 \n" \
"out vec2  f_dp;                                                                                                   \n" \
"out vec2  f_dc;                                                                                                   \n" \
"out vec2  f_dhs;                                                                                                  \n" \
"out float f_corner_radius;                                                                                        \n" \
"out float f_edge_softness;                                                                                        \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    vec2 positions[4] = vec2[](                                                                                   \n" \
"        vec2(-1.0, -1.0),                                                                                         \n" \
"        vec2(+1.0, -1.0),                                                                                         \n" \
"        vec2(-1.0, +1.0),                                                                                         \n" \
"        vec2(+1.0, +1.0)                                                                                          \n" \
"    );                                                                                                            \n" \
"    vec4 colors[4] = vec4[](a_color0, a_color1, a_color2, a_color3);                                              \n" \
"    vec2 dhs = (a_p1 - a_p0) / 2.0;                                                                               \n" \
"    vec2 dc = (a_p1 + a_p0) / 2.0;                                                                                \n" \
"    vec2 dp  = positions[gl_VertexID] * dhs + dc;                                                                 \n" \
"    gl_Position = vec4(2.0 * dp.x / uResolution.x - 1.0,                                                          \n" \
"                       1.0 - 2.0 * dp.y / uResolution.y,                                                          \n" \
"                       0.0,                                                                                       \n" \
"                       1.0);                                                                                      \n" \
"                                                                                                                  \n" \
"    vec2 shs = (a_p1u - a_p0u) / 2.0;                                                                             \n" \
"    vec2 shc = (a_p1u + a_p0u) / 2.0;                                                                             \n" \
"    vec2 shp = positions[gl_VertexID] * shs + shc;                                                                \n" \
"    f_uv = shp;                                                                                                   \n" \
"    f_color = colors[gl_VertexID];                                                                                \n" \
"    f_amix = a_amix;                                                                                              \n" \
"    f_dp = dp;                                                                                                    \n" \
"    f_dc = dc;                                                                                                    \n" \
"    f_dhs = dhs;                                                                                                  \n" \
"    f_corner_radius = a_corner_radius;                                                                            \n" \
"    f_edge_softness = a_edge_softness;                                                                            \n" \
"}                                                                                                                 \n" \

#define VD_UI_GL_FRAGMENT_SHADER_SOURCE                                                                                \
"#version 330 core                                                                                                 \n" \
"in vec4 f_color;                                                                                                  \n" \
"in vec2 f_uv;                                                                                                     \n" \
"in float f_amix;                                                                                                  \n" \
"in vec2  f_dp;                                                                                                    \n" \
"in vec2  f_dc;                                                                                                    \n" \
"in vec2  f_dhs;                                                                                                   \n" \
"in float f_corner_radius;                                                                                         \n" \
"in float f_edge_softness;                                                                                         \n" \
"uniform sampler2D uTexture;                                                                                       \n" \
"out vec4 FragColor;                                                                                               \n" \
"                                                                                                                  \n" \
"float sdf_rounded_rect(vec2 sample_pos, vec2 rect_center, vec2 rect_half_size, float r) {                         \n" \
"    vec2 d2 = (abs(rect_center - sample_pos) - rect_half_size + vec2(r, r));                                      \n" \
"    return min(max(d2.x, d2.y), 0.0) + length(max(d2, 0.0)) -r;                                                   \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    vec2 softness_padding = vec2(max(0, f_edge_softness * 2 - 1), max(0, f_edge_softness * 2 - 1));               \n" \
"    float dist = sdf_rounded_rect(f_dp, f_dc, f_dhs - softness_padding, f_corner_radius);                         \n" \
"    float sdf = 1.f - smoothstep(0, 2 * f_edge_softness, dist);                                                   \n" \
"                                                                                                                  \n" \
"    vec4 sample = texture(uTexture, f_uv);                                                                        \n" \
"    vec4 normal_color = sample * f_color;                                                                         \n" \
"    vec4 mask_color = vec4(f_color.rgb, f_color.a * sample.r);                                                    \n" \
"    vec4 color = mix(normal_color, mask_color, f_amix);                                                           \n" \
"    FragColor = vec4(color) * sdf;                                                                                      \n" \
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
            *internal_format = 0x8058; /* GL_RGBA8 */
            *border          = 0;
            *texformat       = 0x1908; /* GL_RGBA  */
            *type            = 0x1401; /* GL_UNSIGNED_BYTE */
        } break;

        default: {

        } break;
    }
}

VD_UI_API int vd_ui_gl_get_num_attributes(void)
{
    // p0, p1, p0u, p1u, color0, color1, color2, color3, alpha_mix, corner_radius, edge_softness
    return 11;
}

VD_UI_API void vd_ui_gl_get_attribute_properties(int attribute, int *size, unsigned int *type, unsigned char *normalized, int *stride, void **pointer, unsigned int *divisor)
{
    switch (attribute) {
        //                  GL_FLOAT          GL_FALSE
        case 0:  *size = 2; *type = 0x1406;   *normalized = 0; *stride = sizeof(VdUiVertex); *pointer = (void*)VD_OFFSET_OF(VdUiVertex, p0);             *divisor = 1; break;
        //                  GL_FLOAT          GL_FALSE
        case 1:  *size = 2; *type = 0x1406;   *normalized = 0; *stride = sizeof(VdUiVertex); *pointer = (void*)VD_OFFSET_OF(VdUiVertex, p1);             *divisor = 1; break;
        //                  GL_FLOAT          GL_FALSE
        case 2:  *size = 2; *type = 0x1406;   *normalized = 0; *stride = sizeof(VdUiVertex); *pointer = (void*)VD_OFFSET_OF(VdUiVertex, p0u);            *divisor = 1; break;
        //                  GL_FLOAT          GL_FALSE
        case 3:  *size = 2; *type = 0x1406;   *normalized = 0; *stride = sizeof(VdUiVertex); *pointer = (void*)VD_OFFSET_OF(VdUiVertex, p1u);            *divisor = 1; break;
        //                  GL_FLOAT          GL_FALSE
        case 4:  *size = 4; *type = 0x1406;   *normalized = 0; *stride = sizeof(VdUiVertex); *pointer = (void*)VD_OFFSET_OF(VdUiVertex, color);          *divisor = 1; break;
        //                  GL_FLOAT          GL_FALSE
        case 5:  *size = 4; *type = 0x1406;   *normalized = 0; *stride = sizeof(VdUiVertex); *pointer = (void*)(VD_OFFSET_OF(VdUiVertex, color) + 16);   *divisor = 1; break;
        //                  GL_FLOAT          GL_FALSE
        case 6:  *size = 4; *type = 0x1406;   *normalized = 0; *stride = sizeof(VdUiVertex); *pointer = (void*)(VD_OFFSET_OF(VdUiVertex, color) + 32);   *divisor = 1; break;
        //                  GL_FLOAT          GL_FALSE
        case 7:  *size = 4; *type = 0x1406;   *normalized = 0; *stride = sizeof(VdUiVertex); *pointer = (void*)(VD_OFFSET_OF(VdUiVertex, color) + 48);   *divisor = 1; break;
        //                  GL_FLOAT          GL_FALSE
        case 8:  *size = 1; *type = 0x1406;   *normalized = 0; *stride = sizeof(VdUiVertex); *pointer = (void*)VD_OFFSET_OF(VdUiVertex, alpha_mix);      *divisor = 1; break;
        //                  GL_FLOAT          GL_FALSE
        case 9:  *size = 1; *type = 0x1406;   *normalized = 0; *stride = sizeof(VdUiVertex); *pointer = (void*)VD_OFFSET_OF(VdUiVertex, corner_radius);  *divisor = 1; break;
        //                  GL_FLOAT          GL_FALSE
        case 10: *size = 1; *type = 0x1406;   *normalized = 0; *stride = sizeof(VdUiVertex); *pointer = (void*)VD_OFFSET_OF(VdUiVertex, edge_softness);  *divisor = 1; break;
        default: break;
    }
}

#endif // VD_UI_IMPL