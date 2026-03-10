#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif // defined(__clang__)

#define VD_USE_CRT 1
#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#include "vd_fw.h"
#include "vd.h"

#include "vd_ui.h"
#include "vd_ui_demo.c"

#include <stdio.h>

#define GL_CHECK(expr) do { (expr); int _e_ = glGetError(); if (_e_ != 0) { printf("Check at " __FILE__ ":%d failed with 0x%x\n", __LINE__, _e_); assert(0); }} while(0)

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;
    int inspector = 0;

    vd_ui_init();
    vd_ui_debug_set_draw_cursor_on(0);
    vd_ui_debug_set_metrics_on(0);
    vd_ui_debug_set_layout_recompute_vis_on(0);

    vd_fw_init(& (VdFwInitInfo) {
        .window_options = {
            .borderless = 0,
        },
    });

    vd_fw_set_size(1600, 900);

    vd_ui_set_scale(vd_fw_get_scale());

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
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // int rects[1][4] = {
    //     {0, 0, 100, 30},
    // };
    // vd_fw_set_ncrects(1, rects);

    vd_fw_set_vsync_on(1);
    while (vd_fw_running()) {

        int num_events = 0;
        VdFwEvent *events = vd_fw_poll(&num_events);

        if (vd_fw_get_key_pressed(VD_FW_KEY_F11)) {
            inspector = !inspector;
            vd_ui_debug_set_inspector_on(inspector); 
        }

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

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
        for (int i = 0; i < num_events; ++i) {
            VdFwEvent *evt = &events[i];
            switch (evt->type) {
                case VD_FW_EVENT_TYPE_MOUSE_MOVE: {
                    vd_ui_event_mouse_location((float)evt->data.mouse_move.x, (float)evt->data.mouse_move.y);
                } break;

                case VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP: {
                    vd_ui_event_mouse_button(vd_ui_vd_fw_mouse_button_translate(evt->data.mouse_button_up.button), 0);
                } break;

                case VD_FW_EVENT_TYPE_MOUSE_BUTTON_DOWN: {
                    vd_ui_event_mouse_button(vd_ui_vd_fw_mouse_button_translate(evt->data.mouse_button_down.button), 1);
                } break;

                case VD_FW_EVENT_TYPE_KEY_DOWN: {
                    int shift   = (evt->data.key_down.modifiers & VD_FW_MOD_SHIFT)   ? 1 : 0;
                    int control = (evt->data.key_down.modifiers & VD_FW_MOD_CONTROL) ? 1 : 0;
                    int alt     = (evt->data.key_down.modifiers & VD_FW_MOD_ALT)     ? 1 : 0;

                    vd_ui_event_mod(VD_UI_MOD_SHIFT, shift);
                    vd_ui_event_mod(VD_UI_MOD_CONTROL, control);
                    vd_ui_event_mod(VD_UI_MOD_ALT, alt);
                    vd_ui_event_key_press(vd_ui_vd_fw_key_translate(evt->data.key_down.key));
                } break;

                case VD_FW_EVENT_TYPE_CHARACTER: {
                    vd_ui_event_char(evt->data.character.codepoint);
                } break;

                default: break;
            }
        }
        vd_ui_event_mouse_wheel(wx, wy);

        int window_buttons = vd_ui_demo();

        if (window_buttons == -3) {
            vd_fw_quit();
        }

        vd_ui_frame_end();

        vd_ui_render_begin();
        {
            // int nc_rect[4];
            // int rects[16][4];
            // int written = 0;
            // int total = 0;
            // int changed = vd_ui_ws_nc_area_get(nc_rect, 16, &total, &written, rects);
            // if (changed) {
            //     vd_fw_set_ncrects(nc_rect, written, rects);
            // }

            // float nc_rectf[4] = {
            //     (float)nc_rect[0], (float)nc_rect[1],
            //     (float)nc_rect[2], (float)nc_rect[3],
            // };
            // vd_ui_push_rectgrad(nc_rectf, vd_ui_gradient1(vd_ui_f4(0.5f, 0.3f, 0.2f, 0.5f)).e, 0.f, 0.f, 0.f);

            // for (int i = 0; i < written; ++i) {

            //     float rectf[4] = {
            //         (float)rects[i][0], (float)rects[i][1],
            //         (float)rects[i][2], (float)rects[i][3],
            //     };

            //     vd_ui_push_rectgrad(rectf, vd_ui_gradient1(vd_ui_f4(0.8f, 0.1f, 0.1f, 0.8f)).e, 0.f, 0.f, 0.f);
            // }
        }
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

                    // GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
                    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
                    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, format, type, buffer));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
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

        // Loop through render passes
        for (unsigned int i = 0; i < num_passes; ++i) {
            VdUiRenderPass *pass = &passes[i];
            GLuint texture_id = (GLuint)pass->selected_texture->id;
            GLint clip_width   = (GLint)pass->clip[2] - (GLint)pass->clip[0];
            GLint clip_height  = (GLint)pass->clip[3] - (GLint)pass->clip[1];
            GLint clip_x       = (GLint)pass->clip[0];
            GLint clip_lower_y = (GLint)h - (GLint)pass->clip[3];

            glScissor(clip_x, clip_lower_y, clip_width, clip_height);

            glUseProgram(program);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glUniform2f(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_resolution()), (float)w, (float)h);
            glUniform1i(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_texture()), 0);
            glUniform2f(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_mouse()), mx, my);

            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            // Update vertex buffer
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, pass->instance_count * sizeof(VdUiVertex), (unsigned char*)buffer + pass->first_instance * sizeof(VdUiVertex));
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArraysInstanced(
                GL_TRIANGLE_STRIP,
                0,
                4,
                pass->instance_count);
        }
        
        vd_fw_unlock();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"

#define VD_UI_IMPL
#include "vd_ui.h"

#define VD_IMPL
#include "vd.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)
