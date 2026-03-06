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
    glBindVertexArray(Rect_Vao);
    glBindBuffer(GL_ARRAY_BUFFER, Rect_Vbo);

    // Update vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, Rect_Vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, The_Rect_Count * sizeof(Rect), The_Rects);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArraysInstanced(
        GL_TRIANGLE_STRIP,
        0,
        4,
        The_Rect_Count);
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

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

    vd_ui_set_scale(vd_fw_get_scale());

    // Rect
    {
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)1024, 0, GL_DYNAMIC_DRAW);

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

    for (unsigned int i = 0; i < vd_ft_collection_family_count(ui_font_collection); ++i) {
        VdFtFamily family = vd_ft_collection_family_from_index(ui_font_collection, i);
        printf("%u Family Name: %s\n", i, vd_ft_family_name(family));
    }

    vd_ft_box_begin("Hello! My name is Michael. I'm from Greece.", 0);
    vd_ft_box_end();

    // VdFtFamily *family = vd_ft_family_from_index(0);
    // printf("Using %s", vd_ft_family_name(family));


    VdFtFontId font_id = vd_ft_create_font_from_memory(font_memory, (int)font_size);

    #define TX L"Hello! My name is Michael Dodis."
    #define TXLEN ((sizeof(TX) / 2) - 1)
    const wchar_t *s = TX;
    size_t s_len = TXLEN;
    VdFtAnalysis analysis = vd_ft_font_analyze_utf16(font_id, 32.f, s, s_len);
    VdFtRun *run = &analysis.runs[0];

    FontTex font_textures[TXLEN];

    const float face_size = 32.f;
    VdFtFontMetrics face_metrics = vd_ft_font_get_metrics(font_id, face_size);

    for (size_t i = 0; i < run->glyph_count; ++i) {
        uint16_t glyph_index = analysis.glyph_indices[run->glyph_start + i];

        int aw, ah;
        vd_ft_font_get_glyph_bounds(font_id, face_size, glyph_index, &aw, &ah);

        VdFtBitmapRegion region = vd_ft_font_raster(font_id, face_size, &glyph_index, 1);

        // uint8_t *mem = ((uint8_t*)region.memory) + region.pitch * (region.y) + (region.x) * region.stride;
        uint8_t *new_mem = malloc(sizeof(uint8_t) * region.w * region.h);

        for (unsigned int y = 0; y < region.h; ++y) {
            for (unsigned int x = 0; x < region.w; ++x) {
                uint8_t *line = ((uint8_t*)region.memory) + region.pitch * (region.y + y) + (region.x + x) * region.stride;
                uint8_t pixel = *line;

                uint8_t cv_pixel = pixel;
                new_mem[y * region.w + x] = cv_pixel;
            }
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        GLint  level = 0;
        GLint  internal_format = GL_RED;
        GLint  border = 0;
        GLenum format = GL_RED;
        GLenum type = GL_UNSIGNED_BYTE;
        GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
        GL_CHECK(glTexImage2D(GL_TEXTURE_2D, level, internal_format, region.w, region.h, border, format, type, new_mem));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

        font_textures[i].id = texture;
        font_textures[i].w = region.w;
        font_textures[i].h = region.h;
        font_textures[i].m = vd_ft_font_get_glyph_metrics(font_id, face_size, glyph_index);
    }


    // float total_width = 0.f;
    // float max_height = 0.f;
    // for (size_t i = 0; i < run->glyph_count; ++i) {
    //     size_t glyph_index_index = run->glyph_start + i;
    //     uint16_t glyph_index = analysis.glyph_indices[glyph_index_index];

    //     int aw, ah;
    //     vd_ft_font_get_glyph_bounds(font_id, 32.f, glyph_index, &aw, &ah);
    //     if (max_height < ((float)ah)) {
    //         max_height = (float)ah;
    //     }

    //     total_width += analysis.glyph_advances[glyph_index_index];
    //     // total_width += aw;
    // }

    // int bitmap_width  = ((int)total_width) + 1;
    // int bitmap_height = ((int)max_height) * 4 + 1;
    // uint32_t *memory = malloc(bitmap_width * bitmap_height * 4);

    // int running_width = 0;
    // for (size_t i = 0; i < run->glyph_count; ++i) {

    //     size_t glyph_index_index = run->glyph_start + i;
    //     uint16_t glyph_index = analysis.glyph_indices[glyph_index_index];

    //     VdFtGlyphMetrics glyph_metrics = vd_ft_font_get_glyph_metrics(font_id, 32.f, glyph_index);
    //     VdFtBitmapRegion region = vd_ft_font_raster(font_id, 32.f, &glyph_index, 1);
    //     for (unsigned int y = 0; y < region.h; ++y) {
    //         for (unsigned int x = 0; x < region.w; ++x) {
    //             uint8_t *line = ((uint8_t*)region.memory) + region.pitch * (region.y + y) + (region.x + x) * region.stride;
    //             uint8_t pixel = *line;

    //             uint32_t cv_pixel = 0x00FFFFFF | (pixel << 24);
    //             int yoff = (y + (int)glyph_metrics.baseline_origin_y - region.h + 10);
    //             // int yoff = (y + region.h - 10);
    //             memory[yoff * bitmap_width + x + running_width] = cv_pixel;
    //         }
    //     }


    //     running_width += (int)analysis.glyph_advances[glyph_index_index];
    //     // running_width += region.w;
    // }
    // stbi_write_png("./test.png", bitmap_width, bitmap_height, 4, memory, 4 * bitmap_width);

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

    vd_fw_set_vsync_on(1);
    while (vd_fw_running()) {

        vd_fw_poll(0);

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        vd_fw_compile_or_hotload_program(&Rect_Program, &Rect_Program_time, "./glsl/rect2d.vert", "./glsl/rect2d.frag");

        float delta_seconds = vd_fw_delta_s();

        vd_fw_compile_or_hotload_program(&program, &program_time, "./glsl/ui_basic.vert", "./glsl/ui_basic.frag");

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

        // // Loop through render passes
        // for (unsigned int i = 0; i < num_passes; ++i) {
        //     VdUiRenderPass *pass = &passes[i];
        //     GLuint texture_id = (GLuint)pass->selected_texture->id;
        //     GLint clip_width   = (GLint)pass->clip[2] - (GLint)pass->clip[0];
        //     GLint clip_height  = (GLint)pass->clip[3] - (GLint)pass->clip[1];
        //     GLint clip_x       = (GLint)pass->clip[0];
        //     GLint clip_lower_y = (GLint)h - (GLint)pass->clip[3];

        //     glScissor(clip_x, clip_lower_y, clip_width, clip_height);

        //     glUseProgram(program);
        //     glActiveTexture(GL_TEXTURE0);
        //     glBindTexture(GL_TEXTURE_2D, texture_id);
        //     glUniform2f(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_resolution()), (float)w, (float)h);
        //     glUniform1i(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_texture()), 0);
        //     glUniform2f(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_mouse()), mx, my);

        //     glBindVertexArray(vao);
        //     glBindBuffer(GL_ARRAY_BUFFER, vbo);

        //     // Update vertex buffer
        //     glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //     glBufferSubData(GL_ARRAY_BUFFER, 0, pass->instance_count * sizeof(VdUiVertex), (unsigned char*)buffer + pass->first_instance * sizeof(VdUiVertex));
        //     glBindBuffer(GL_ARRAY_BUFFER, 0);

        //     glDrawArraysInstanced(
        //         GL_TRIANGLE_STRIP,
        //         0,
        //         4,
        //         pass->instance_count);
        // }

        // Layout the text
        

        glUseProgram(Rect_Program);
        glUniform2f(glGetUniformLocation(Rect_Program, "u_resolution"), (float)w, (float)h);
        glUniform1i(glGetUniformLocation(Rect_Program, "u_tex"), 0);
        {
            float c_x = mx;
            float c_y = my - (face_metrics.ascent - face_metrics.descent) + face_metrics.line_gap;

            for (unsigned int i = 0; i < run->glyph_count; ++i) {
                rects_begin(font_textures[i].id);
                VdFtGlyphMetrics *m = &font_textures[i].m;

                float fw = (float)font_textures[i].w; 
                float fh = (float)font_textures[i].h; 

                float x = c_x + analysis.glyph_offsets[i].advance;
                float y = c_y + m->bearing_y - analysis.glyph_offsets[i].ascend;

                Rect r = {
                    {x, y},
                    {x + fw, y + fh},
                    {0, 0},
                    {1, 1},
                    {1,1,1,1},
                };

                rects_push(&r);
                rects_render();

                c_x += analysis.glyph_advances[i];
            }
        }
        
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

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)
