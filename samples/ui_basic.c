#define VD_USE_CRT 1
#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#include "vd_fw.h"
#include "vd.h"

#include "vd_ui.h"

#include <stdio.h>

#pragma pack(push, 1)
typedef struct {
    float p0[2];
    float p1[2];
    float p0u[2];
    float p1u[2];
    float color[4];
} InputVertex;
#pragma pack(pop)

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    VdArena arena;
    VD_ARENA_FROM_SYSTEM(&arena, VD_MEGABYTES(24));

    vd_ui_init();
    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .debug_on = 1,
            .version = VD_FW_GL_VERSION_3_3,
        },
    });

    Vdusize len;
    void *file = vd_dump_file_to_bytes(&arena, "./ext/LiberationSans-Regular.ttf", &len);
    vd_ui_font_add_ttf(file, len, 32.f);


    GLuint program;
    {

        char *vertex_shader_source;   size_t vertex_shader_len;
        char *fragment_shader_source; size_t fragment_shader_len;
        vd_ui_gl_get_default_shader_sources(&vertex_shader_source, &vertex_shader_len, 
                                            &fragment_shader_source, &fragment_shader_len);

        GLuint vshd = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshd, 1, &vertex_shader_source, 0);
        glCompileShader(vshd);

        GLuint fshd = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshd, 1, &fragment_shader_source, 0);
        glCompileShader(fshd);

        program = glCreateProgram();
        glAttachShader(program, vshd);
        glAttachShader(program, fshd);
        glLinkProgram(program);
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glActiveTexture(GL_TEXTURE0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vd_ui_get_min_vertex_buffer_size(), 0, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(InputVertex), (void*)VD_OFFSET_OF(InputVertex, p0));
    glVertexAttribDivisor(0, 1);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(InputVertex), (void*)VD_OFFSET_OF(InputVertex, p1));
    glVertexAttribDivisor(1, 1);

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(InputVertex), (void*)VD_OFFSET_OF(InputVertex, p0u));
    glVertexAttribDivisor(2, 1);

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(InputVertex), (void*)VD_OFFSET_OF(InputVertex, p1u));
    glVertexAttribDivisor(3, 1);

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(InputVertex) , (void*)VD_OFFSET_OF(InputVertex, color));
    glVertexAttribDivisor(4, 1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    vd_fw_set_vsync_on(1);
    while (vd_fw_running()) {
        float delta_seconds = vd_fw_delta_s();
        (void)delta_seconds;

        int w, h;
        vd_fw_get_size(&w, &h);
        vd_ui_frame_begin(delta_seconds);

        float mx, my;
        vd_fw_get_mouse_statef(&mx, &my);

        vd_ui_event_mouse_location(mx, my);

        vd_ui_frame_end();

        // Process updates
        size_t num_updates;
        VdUiUpdate *updates = vd_ui_frame_get_updates(&num_updates);

        for (size_t i = 0; i < num_updates; ++i) {
            VdUiUpdate *update = &updates[i];
            switch (update->type) {
                case VD_UI_UPDATE_TYPE_NEW_TEXTURE: {
                    int width                = update->data.new_texture.width;
                    int height               = update->data.new_texture.height;
                    VdUiTextureFormat format = update->data.new_texture.format;
                    void *buffer             = update->data.new_texture.buffer;
                    size_t buffer_size       = update->data.new_texture.size;
                    VdUiTextureId *id        = update->data.new_texture.write_id;

                    VD_UNUSED(format);
                    VD_UNUSED(buffer_size);

                    GLuint texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);

                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, buffer);
                    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
                    glBindTexture(GL_TEXTURE_2D, 0);

                    id->id = (uintptr_t)texture;
                } break;
            }
        }

        // Get vertex buffer
        size_t buffer_size;
        void *buffer = vd_ui_frame_get_vertex_buffer(&buffer_size);

        // Get render passes
        int num_passes;
        VdUiRenderPass *passes = vd_ui_frame_get_render_passes(&num_passes);

        // Update vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        void *data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        VD_MEMCPY(data, buffer, buffer_size);
        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glViewport(0, 0, w, h);
        glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);


        // Loop through render passes
        for (int i = 0; i < num_passes; ++i) {
            VdUiRenderPass *pass = &passes[i];
            GLuint texture_id = (GLuint)pass->selected_texture.id;

            glUseProgram(program);
            glActiveTexture(GL_TEXTURE0);
            glUniform2f(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_resolution()), (float)w, (float)h);
            glUniform1i(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_texture()), 0);
            glBindTexture(GL_TEXTURE_2D, texture_id);

            glDrawArraysInstanced(
                GL_TRIANGLE_STRIP,
                pass->first_instance,
                4,
                pass->instance_count);
        }
        
        vd_fw_swap_buffers();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"

#define VD_UI_IMPL
#include "vd_ui.h"

#define VD_IMPL
#include "vd.h"
