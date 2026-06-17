#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif // defined(__clang__)

#define VD_USE_CRT 1
#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#include "vd_fw.h"
#include "vd.h"
#include "stb_image_write.h"

#include "vd_ui.h"
#include "vd_ui_demo.c"

#include <stdio.h>
#include "vd_ft.h"
#include "stb_rect_pack.h"

#define GL_CHECK(expr) do { (expr); int _e_ = glGetError(); if (_e_ != 0) { printf("Check at " __FILE__ ":%d failed with 0x%x\n", __LINE__, _e_); assert(0); }} while(0)

#pragma pack(push, 1)
typedef struct {
    float p0p[2];
    float p1p[2];
    float p0t[2];
    float p1t[2];
    float col[4];
} Rect;
#pragma pack(pop)

typedef struct {
    GLuint id;
    int w, h;
    VdFtGlyphMetrics m;
} FontTex;

typedef struct {
    VdFtFaceKey key;
    uint16_t    index;
    float       size;
} GlyphKey;

typedef struct {
    GlyphKey         key;
    int              x, y;
    int              w, h;
    VdFtGlyphMetrics m;
} Glyph;

typedef struct {
    GlyphKey k;
    Glyph    v;
} GlyphMap;

GLuint Rect_Vbo;
GLuint Rect_Vao;
GLuint Rect_Program;
GLuint Mask1_Texture;
static Rect The_Rects[1024];
static int The_Rect_Count = 0;

static void rects_begin(GLuint texture)
{
    if (texture == 0) {
        texture = Mask1_Texture;
    }
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    The_Rect_Count = 0;
}

static void rects_push(Rect *r)
{
    The_Rects[The_Rect_Count++] = *r;
}

static void rects_render(void)
{
    GL_CHECK(glBindVertexArray(Rect_Vao));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, Rect_Vbo));

    // Update vertex buffer
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, Rect_Vbo));
    GL_CHECK(glBufferSubData(GL_ARRAY_BUFFER, 0, The_Rect_Count * sizeof(Rect), The_Rects));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GL_CHECK(glDrawArraysInstanced(
        GL_TRIANGLE_STRIP,
        0,
        4,
        The_Rect_Count));
}

static unsigned char *atlas = 0;
static VD_KVMAP GlyphMap *glyph_map = 0;
static stbrp_context rp_context;
static int cache_needs_resubmit = 0;
static GLuint cache_atlas;
static int atlas_width = 2048;
static int atlas_height = 2048;
Glyph get_or_create_glyph(VdFtFace face, float size, uint16_t index)
{
    GlyphKey key;
    key.key = vd_ft_face_key(face); 
    key.size = size;
    key.index = index;

    Glyph result;
    if (VD_KVMAP_GET(glyph_map, &key, &result)) {
        return result;
    }

    VdFtExtent extent = vd_ft_face_glyph_bounds(face, size, index);

    stbrp_rect rect = {0};
    rect.w = (int)ceilf(extent.x);
    rect.h = (int)ceilf(extent.y);

    stbrp_pack_rects(&rp_context, &rect, 1);
    result.key = key;
    result.x = rect.x;
    result.y = rect.y;
    result.w = rect.w;
    result.h = rect.h;
    result.m = vd_ft_face_glyph_metrics(face, size, index);

    assert(rect.was_packed != 0);

    VdFtBitmapRegion region = vd_ft_face_raster(face, size, index);
    for (unsigned int y = 0; y < region.h; ++y) {
        for (unsigned int x = 0; x < region.w; ++x) {
            uint8_t in_pixel = *(((uint8_t*)region.memory) + region.pitch * (region.y + y) + (region.x + x) * region.stride);

            uint8_t *out_pixel = (atlas + atlas_width * (result.y + y) + (result.x + x) * sizeof(uint8_t));

            *out_pixel = in_pixel;
        }
    }


    cache_needs_resubmit = 1;

    VD_KVMAP_SET(glyph_map, &key, &result);
    return result;
}

typedef struct {
    float left;
    float top;
    float right;
    float bottom;
} Uv;

Uv get_glyph_subregion(Glyph glyph)
{
    Uv result;
    float ipw = 1.f / atlas_width;
    float iph = 1.f / atlas_height;

    float x0 = (float)glyph.x;
    float x1 = (float)glyph.x + glyph.w;
    float y0 = (float)glyph.y;
    float y1 = (float)glyph.y + glyph.h;

    result.left = x0 * ipw;
    result.top = y0 * iph;
    result.right = x1 * ipw;
    result.bottom = y1 * iph;
    return result;
}

void update_glyph_cache(void)
{
    if (!cache_needs_resubmit)
    {
        return;
    }

    GLint  level = 0;
    GLint  internal_format = GL_RED;
    GLint  border = 0;
    GLenum format = GL_RED;
    GLenum type = GL_UNSIGNED_BYTE;

    GL_CHECK(glBindTexture(GL_TEXTURE_2D, cache_atlas));
    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, level, internal_format, atlas_width, atlas_height, border, format, type, atlas));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

    cache_needs_resubmit = 0;
}

static int utf32_to_utf8(uint32_t codepoint, unsigned char *out)
{
    if (codepoint <= 0x7F) {
        out[0] = (unsigned char)codepoint;
        return 1;
    }
    else if (codepoint <= 0x7FF) {
        out[0] = 0xC0 | (unsigned char)(codepoint >> 6);
        out[1] = 0x80 | (unsigned char)(codepoint & 0x3F);
        return 2;
    }
    else if (codepoint <= 0xFFFF) {
        out[0] = 0xE0 | (unsigned char)(codepoint >> 12);
        out[1] = 0x80 | (unsigned char)((codepoint >> 6) & 0x3F);
        out[2] = 0x80 | (unsigned char)(codepoint & 0x3F);
        return 3;
    }
    else if (codepoint <= 0x10FFFF) {
        out[0] = 0xF0 | (unsigned char)(codepoint >> 18);
        out[1] = 0x80 | (unsigned char)((codepoint >> 12) & 0x3F);
        out[2] = 0x80 | (unsigned char)((codepoint >> 6) & 0x3F);
        out[3] = 0x80 | (unsigned char)(codepoint & 0x3F);
        return 4;
    }

    return 0;
}

static char String_Buf[1024];
static int String_Buf_Len;

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    memcpy(String_Buf, "Hello, World", 12);
    String_Buf_Len = 12;


    vd_init(0);

    VdArena arena = vd_arena_from_malloc(VD_KILOBYTES(128));
    VD_KVMAP_INIT_DEFAULT(glyph_map, &arena);

    atlas = malloc(atlas_width * atlas_height);
    int num_nodes = atlas_width - 2;
    stbrp_node *nodes = malloc(sizeof(*nodes) * num_nodes);
    stbrp_init_target(&rp_context, atlas_width, atlas_height, nodes, num_nodes);

    vd_ui_init();
    vd_ui_debug_set_draw_cursor_on(0);
    vd_ui_debug_set_inspector_on(0);
    vd_ui_debug_set_metrics_on(0);
    vd_ui_debug_set_layout_recompute_vis_on(0);

    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .configs = (VdFwGlConfig[]) {
                {
                    .version = VD_FW_GL_VERSION_3_3,
                },
                0,
            }
        },
        .window_options = {
            .borderless = 0,
        },
    });

    int show_glyph_cache = 0;
    vd_fw_set_title("GL Font (F2: Atlas, F3: Style)");

    glGenTextures(1, &cache_atlas);
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, cache_atlas));
    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, NULL));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

    vd_ui_set_scale(vd_fw_get_scale());

    // Rect
    {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)1024 * sizeof(Rect), 0, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Rect), (void*)offsetof(Rect, p0p));
        glVertexAttribDivisor(0, 1);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Rect), (void*)offsetof(Rect, p1p));
        glVertexAttribDivisor(1, 1);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Rect), (void*)offsetof(Rect, p0t));
        glVertexAttribDivisor(2, 1);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Rect), (void*)offsetof(Rect, p1t));
        glVertexAttribDivisor(3, 1);

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Rect), (void*)offsetof(Rect, col));
        glVertexAttribDivisor(4, 1);

        Rect_Vbo = vbo;
        Rect_Vao = vao;

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        GLint  level = 0;
        GLint  internal_format = GL_RED;
        GLint  border = 0;
        GLenum format = GL_RED;
        GLenum type = GL_UNSIGNED_BYTE;
        unsigned char buffer[2*2] = {0xFF, 0xFF, 0xFF, 0xFF};
        GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, level, internal_format, 2, 2, border, format, type, buffer));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

        Mask1_Texture = texture;

        // GLint size;
        // GLenum type;
        // GLboolean normalized;
        // GLsizei stride;
        // void *pointer;
        // GLuint divisor;
        // vd_ui_gl_get_attribute_properties(i, &size, &type, &normalized, &stride, &pointer, &divisor);
    }

    unsigned long long Rect_Program_time = 0;

    // unsigned int count = vd_ft_family_count();
    // for (unsigned int i = 0; i < count; ++i) {
    //     VdFtFamily *fam = vd_ft_family_from_index(i);
    //     const char *name = vd_ft_family_name(fam);
    //     printf("%s\n", name);
    //     vd_ft_family_free(fam);
    // }

    void    *font_memory;
    size_t  font_size;
    font_memory = vd_ui_font_default(&font_size);

    VdFtCollection ui_font_collection = vd_ft_collection_from_memory(font_memory, font_size);
    VdFtCollection sys_collection = vd_ft_collection_from_system();

    VdFtFamily family;
    if (1) {
        family = vd_ft_collection_family_from_index(sys_collection, 0);
    } else {
        family = vd_ft_collection_family_from_index(ui_font_collection, 0);
    }

    printf("Family Name: %s\n", vd_ft_family_name(family));

    FontTex font_textures[64];

    GLuint program = 0;
    unsigned long long program_time = 0;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vd_ui_get_min_vertex_buffer_size(), 0, GL_DYNAMIC_DRAW);

    for (int i = 0; i < vd_ui_gl_get_num_attributes(); ++i) {
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        void *pointer;
        GLuint divisor;
        vd_ui_gl_get_attribute_properties(i, &size, &type, &normalized, &stride, &pointer, &divisor);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, size, type, normalized, stride, pointer);
        glVertexAttribDivisor(i, divisor);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_MULTISAMPLE);
    // glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    VdFtStyle style = VD_FT_STYLE_NORMAL;

    vd_fw_set_vsync_on(1);
    while (vd_fw_running()) {

        vd_fw_poll(0);

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        vd_fw_compile_or_hotload_program(&Rect_Program, &Rect_Program_time, "./glsl/rect2d.vert", "./glsl/rect2d.frag");

        float delta_seconds = vd_fw_delta_s();

        vd_fw_compile_or_hotload_program(&program, &program_time, "./glsl/ui_basic.vert", "./glsl/ui_basic.frag");

        if (vd_fw_get_key_pressed(VD_FW_KEY_F2)) {
            show_glyph_cache = !show_glyph_cache;
        }

        if (vd_fw_get_key_pressed(VD_FW_KEY_F3)) {
            if (style == VD_FT_STYLE_NORMAL) {
                style = VD_FT_STYLE_ITALIC;
            } else {
                style = VD_FT_STYLE_NORMAL;
            }
        }

        for (unsigned short i = 0; i < vd_fw_get_num_codepoints(); ++i) {
            uint32_t codepoint = vd_fw_get_codepoint(i);

            String_Buf_Len += utf32_to_utf8(codepoint, (unsigned char*)String_Buf + String_Buf_Len);
        }

        if (vd_fw_get_key_pressed(VD_FW_KEY_ESCAPE)) {
            String_Buf_Len = 0;
        }

        vd_fw_lock();

        int w, h;
        vd_fw_get_size(&w, &h);
        vd_ui_frame_begin(delta_seconds);

        float mx, my;
        int mouse_state = vd_fw_get_mouse_statef(&mx, &my);
        VD_UNUSED(mouse_state);

        float wx, wy;
        int wheel_moved = vd_fw_get_mouse_wheel(&wx, &wy);
        (void)wheel_moved;

        int focused;
        if (vd_fw_get_focused(&focused)) {
            vd_ui_event_focus(focused);
        }

        vd_ui_event_size((float)w, (float)h);
        vd_ui_event_mouse_location(mx, my);
        vd_ui_event_mouse_button(VD_UI_MOUSE_LEFT,  mouse_state & VD_FW_MOUSE_STATE_LEFT_BUTTON_DOWN);
        vd_ui_event_mouse_button(VD_UI_MOUSE_RIGHT, mouse_state & VD_FW_MOUSE_STATE_RIGHT_BUTTON_DOWN);
        vd_ui_event_mouse_wheel(wx, wy);


        int window_buttons = vd_ui_demo();

        if (window_buttons == -3) {
            vd_fw_quit();
        }

        vd_ui_frame_end();

        vd_ui_render_begin();
        vd_ui_render_end();

        // Process updates
        size_t num_updates;
        VdUiUpdate *updates = vd_ui_frame_get_updates(&num_updates);

        for (size_t i = 0; i < num_updates; ++i) {
            VdUiUpdate *update = &updates[i];
            switch (update->type) {
                case VD_UI_UPDATE_TYPE_NEW_TEXTURE: {
                    int width                = update->data.new_texture.width;
                    int height               = update->data.new_texture.height;
                    void *buffer             = update->data.new_texture.buffer;
                    size_t buffer_size       = update->data.new_texture.size;
                    VdUiTextureId *id        = update->data.new_texture.write_id;

                    VD_UNUSED(buffer_size);

                    GLuint texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);

                    GLint  level;
                    GLint  internal_format;
                    GLint  border;
                    GLenum format;
                    GLenum type;
                    vd_ui_gl_cv_texture_format(
                        update->data.new_texture.format,
                        &level,
                        &internal_format, 
                        &border, 
                        &format,
                        &type);

                    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, format, type, buffer));
                    // GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
                    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                    // glGenerateMipmap(GL_TEXTURE_2D);
                    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

                    id->id = (uintptr_t)texture;
                } break;

                case VD_UI_UPDATE_TYPE_WRITE_TEXTURE: {
                    GLuint texture = (GLuint)update->data.write_texture.texture.id;

                    GLint  level;
                    GLint  internal_format;
                    GLint  border;
                    GLenum format;
                    GLenum type;
                    vd_ui_gl_cv_texture_format(
                        update->data.write_texture.format,
                        &level,
                        &internal_format, 
                        &border, 
                        &format,
                        &type);

                    glBindTexture(GL_TEXTURE_2D, texture);
                    glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, update->data.write_texture.width, update->data.write_texture.height, format, type, update->data.write_texture.buffer);
                    glBindTexture(GL_TEXTURE_2D, 0);
                } break;

                default: break;
            }
        }

        // Get vertex buffer
        size_t buffer_size;
        void *buffer = vd_ui_frame_get_vertex_buffer(&buffer_size);

        // Get render passes
        unsigned int num_passes;
        VdUiRenderPass *passes = vd_ui_frame_get_render_passes(&num_passes);

        glViewport(0, 0, w, h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vd_ft_box_begin();
        vd_ft_box_family_set(family);
        vd_ft_box_font_size_set(32.f);
        vd_ft_box_font_style_set(style);
        vd_ft_box_push(String_Buf, String_Buf_Len);
        vd_ft_box_end();
        vd_ft_box_wrap(VD_FT_WRAP_NONE);

        VdFtRunResult run_result = vd_ft_box_run();
        VdFtRun *runs = run_result.runs;
        int run_count = run_result.run_count;

        VdFtFontMetrics face_metrics = vd_ft_face_metrics(runs[0].face, runs[0].pixel_scale);

        GL_CHECK(glUseProgram(Rect_Program));
        GL_CHECK(glUniform2f(glGetUniformLocation(Rect_Program, "u_resolution"), (float)w, (float)h));
        GL_CHECK(glUniform1i(glGetUniformLocation(Rect_Program, "u_tex"), 0));
        rects_begin(cache_atlas);
        float baseline_x = mx;
        float baseline_y = my - face_metrics.ascent;
        for (int i = 0; i < run_count; ++i) {
            VdFtRun *run = &runs[i];

            for (uint32_t g = 0; g < run->glyph_count; ++g) {
                uint16_t glyph_index = run_result.indices[run->glyph_start + g];
                Glyph glyph = get_or_create_glyph(run->face, run->pixel_scale, glyph_index);

                float fw = (float)glyph.w; 
                float fh = (float)glyph.h;

                VdFtGlyphOffset offset = run_result.offsets[run->offsets_start + g];

                float x = baseline_x + glyph.m.bearing_x + offset.advance;
                float y = baseline_y + glyph.m.bearing_y - offset.ascend;

                Uv uv = get_glyph_subregion(glyph);

                Rect r = {
                    {x, y},
                    {x + fw, y + fh},
                    {uv.left, uv.top},
                    {uv.right, uv.bottom},
                    {1,1,1,1},
                };
                rects_push(&r);

                baseline_x += run_result.advances[run->advances_start + g];
            }
        }

        update_glyph_cache();
        rects_render();

        if (show_glyph_cache) {
            rects_begin(Mask1_Texture);
            float scale = 0.5f;
            float atlas_width_scaled = (float)atlas_width * scale;
            float atlas_height_scaled = (float)atlas_height * scale;
            Rect r1 = {
                {0, 0},
                {atlas_width_scaled, atlas_height_scaled},
                {0, 0},
                {1, 1},
                {0.2f,0.2f,0.2f,0.7f},
            };
            rects_push(&r1);
            rects_render();

            rects_begin(cache_atlas);
            Rect r = {
                {0, 0},
                {atlas_width_scaled, atlas_height_scaled},
                {0, 0},
                {1, 1},
                {1,1,1,1},
            };
            rects_push(&r);
            rects_render();
        }

        vd_fw_swap();
        vd_fw_unlock();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"

#define VD_FT_IMPL
#include "vd_ft.h"

#define VD_UI_IMPL
#include "vd_ui.h"

#define VD_IMPL
#include "vd.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)
