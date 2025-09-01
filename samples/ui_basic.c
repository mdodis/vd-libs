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


    Vdusize len;
    void *file = vd_dump_file_to_bytes(&arena, "c:/windows/fonts/arial.ttf", &len);
    stbtt_fontinfo font_info;
    stbtt_InitFont(&font_info, file, stbtt_GetFontOffsetForIndex(file, 0));
    float scale = stbtt_ScaleForPixelHeight(&font_info, 32.f);

    stbtt_pack_context spc;
    int width = 512;
    int height = 512;
    unsigned char *pixels = (unsigned char*)malloc(width * height);
    stbtt_PackBegin(&spc, pixels, width, height, 0, 1, 0);
    stbtt_packedchar packed[95]; 
    int result = stbtt_PackFontRange(&spc, file, 0, 64.f, 32, 95, packed);
    stbtt_PackEnd(&spc);

    vd_ui_init();
    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .debug_on = 1,
            .version = VD_FW_GL_VERSION_3_3,
        },
    });

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

    GLuint texture;
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);
    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vd_ui_get_min_vertex_buffer_size(), 0, GL_DYNAMIC_DRAW);

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
        glUseProgram(program);

        // @todo(mdodis): This should be determined by the ui
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform2f(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_resolution()), (float)w, (float)h);
        glUniform1i(glGetUniformLocation(program, "uTexture"), 0);

        // Loop through render passes
        for (int i = 0; i < num_passes; ++i) {
            VdUiRenderPass *pass = &passes[i];
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
