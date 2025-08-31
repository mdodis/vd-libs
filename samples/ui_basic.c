#define VD_USE_CRT 1
#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#include "vd_fw.h"
#include "vd.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "ext/stb_truetype.h"

#include "vd_ui.h"

#include <stdio.h>

const char *Vertex_Shader_Source =
"#version 330 core                       \n"
"out vec2 tex;"
"void main()                             \n"
"{                                       \n"
"   vec2 positions[6] = vec2[](              \n"
"       vec2(-0.7, +0.7), // top-left          \n"
"       vec2(+0.7, +0.7), // top-right         \n"
"       vec2(+0.7, -0.7), // bottom-right      \n"
"       vec2(+0.7, -0.7), // bottom-right      \n"
"       vec2(-0.7, -0.7), // bottom-left       \n"
"       vec2(-0.7, +0.7)  // top-left          \n"
"   );                                       \n"
"   vec2 coords[6]    = vec2[](              \n"
"       vec2(0.0, 1.0), // top-left          \n"
"       vec2(1.0, 1.0), // top-right         \n"
"       vec2(1.0, 0.0), // bottom-right      \n"
"       vec2(1.0, 0.0), // bottom-right      \n"
"       vec2(0.0, 0.0), // bottom-left       \n"
"       vec2(0.0, 1.0)  // top-left          \n"
"   );                                       \n"
"   gl_Position = vec4(positions[gl_VertexID].x, positions[gl_VertexID].y, 0.0, 1.0);   \n"
"   tex = coords[gl_VertexID];           \n"
"}                                       \n"
;

const char *Fragment_Shader_Source =
"#version 330 core                             \n"
"in vec2 tex; \n"
"out vec4 FragColor; \n"
"uniform sampler2D uTexture; \n"
"void main()                                   \n"
"{                                             \n"
"   float a = texture(uTexture, tex).r;\n"
"   FragColor = vec4(1.0, 1.0, 1.0, a);\n"
"}                                             \n"
;

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

        GLuint vshd = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshd, 1, &Vertex_Shader_Source, 0);
        glCompileShader(vshd);

        GLuint fshd = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshd, 1, &Fragment_Shader_Source, 0);
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

    vd_fw_set_vsync_on(1);
    while (vd_fw_running()) {
        float delta_seconds = vd_fw_delta_s();
        (void)delta_seconds;

        int w, h;
        vd_fw_get_size(&w, &h);

        glViewport(0, 0, w, h);
        glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(program, "uTexture"), 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

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
