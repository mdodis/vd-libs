#include "vd_fw.h"
#include "vd_ui.h"
#include "vd_dlg.h"

#include <assert.h>
#include <stdio.h>

#define GL_CHECK(expr) do { (expr); int _e_ = glGetError(); if (_e_ != 0) { printf("Check at " __FILE__ ":%d failed with 0x%x\n", __LINE__, _e_); assert(0); }} while(0)

#define CSTR_AND_LEN(x) sizeof(x) - 1, x

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
            .debug_on = 1,
            .version = VD_FW_GL_VERSION_3_3,
        },

        .window_options = {
            .borderless = 0,
        },
    });
    vd_fw_set_vsync_on(0);

    vd_dlg_set_context(vd_fw_get_internal_window_handle());

    vd_fw_set_title("GL Dialogs");

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

    vd_fw_set_vsync_on(1);
    const char *single_open_file_path = NULL;
    while (vd_fw_running()) {

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        float delta_seconds = vd_fw_delta_s();

        vd_fw_compile_or_hotload_program(&program, &program_time, "./glsl/ui_basic.vert", "./glsl/ui_basic.frag");

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


        VdUiDiv *app = vd_ui_div_new(VD_UI_FLAG_BACKGROUND, VD_UI_LIT("##app"));
        app->size[0].mode = VD_UI_SIZE_MODE_PERCENT_OF_PARENT;
        app->size[0].value = 1.f;
        app->size[1].mode = VD_UI_SIZE_MODE_PERCENT_OF_PARENT;
        app->size[1].value = 1.f;
        app->style.background.normal = vd_ui_gradient1(vd_ui_f4(0.1f, 0.1f, 0.1f, 1.f));
        vd_ui_parent_push(app);
        {
            if (vd_ui_buttonf("Message Box (Ok)").clicked) {
                vd_dlg_message_box(CSTR_AND_LEN("<Simple Message Title>"),
                                   CSTR_AND_LEN("This is a simple message with a title, and an OK button."),
                                   VD_DLG_MESSAGE_BOX_OPTION_OK | VD_DLG_MESSAGE_BOX_OPTION_INFO);
            }

            if (vd_ui_buttonf("Message Box (Warning)").clicked) {
                vd_dlg_message_box(CSTR_AND_LEN("<Simple Message Title>"),
                                   CSTR_AND_LEN("This is a simple message with a title, and an OK button."),
                                   VD_DLG_MESSAGE_BOX_OPTION_OK | VD_DLG_MESSAGE_BOX_OPTION_WARNING);
            }

            if (vd_ui_buttonf("Open File").clicked) {
                VdDlgFileFilter filters[2] = {
                    {
                        CSTR_AND_LEN("All Files"),
                        CSTR_AND_LEN("*.*"),
                    },
                    {
                        CSTR_AND_LEN("PNG Files"),
                        CSTR_AND_LEN("*.png"),
                    }
                };

                VdDlgFileResult result = vd_dlg_open_file(CSTR_AND_LEN("Open a File"),
                                                          0, NULL,
                                                          2, filters, 1,
                                                          0);
                single_open_file_path = result.buf;
            }

            if (single_open_file_path) {
                vd_ui_labelf("Open File Result: %s", single_open_file_path);
            }

        }
        vd_ui_parent_pop();

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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
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
        
        vd_fw_swap_buffers();
    }
    return 0;
}

#define VD_UI_IMPL
#include "vd_ui.h"

#define VD_FW_IMPL
#include "vd_fw.h"

#define VD_DLG_IMPL
#include "vd_dlg.h"
