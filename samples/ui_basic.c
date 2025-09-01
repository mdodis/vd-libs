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
"#version 330 core                                                                      \n"
"layout (location = 0) in vec2 a_p0;                                                    \n"
"layout (location = 1) in vec2 a_p1;                                                    \n"
"layout (location = 2) in vec2 a_p0u;                                                   \n"
"layout (location = 3) in vec2 a_p1u;                                                   \n"
"layout (location = 4) in vec4 a_color;                                                 \n"
"uniform vec2 uResolution;                                                              \n"
"out vec4 f_color;                                                                      \n"
"out vec2 f_uv;                                                                         \n"
"                                                                                       \n"
"void main()                                                                            \n"
"{                                                                                      \n"
"   vec2 positions[4] = vec2[](                                                         \n"
"       vec2(-1.0, -1.0),                                                               \n"
"       vec2(-1.0, +1.0),                                                               \n"
"       vec2(+1.0, -1.0),                                                               \n"
"       vec2(+1.0, +1.0)                                                                \n"
"   );                                                                                  \n"
"   vec2 dhs = (a_p1 - a_p0) / 2.0;                                                     \n"
"   vec2 dhc = (a_p1 + a_p0) / 2.0;                                                     \n"
"   vec2 dp  = positions[gl_VertexID] * dhs + dhc;                                      \n"
"   gl_Position = vec4(2.0 * dp.x / uResolution.x - 1.0,                                \n"
"                      1.0 - 2.0 * dp.y / uResolution.y,                                \n"
"                      0.0,                                                             \n"
"                      1.0);                                                            \n"
"                                                                                       \n"
"   vec2 shs = (a_p1u - a_p0u) / 2.0;                                                   \n"
"   vec2 shc = (a_p1u + a_p0u) / 2.0;                                                   \n"
"   vec2 shp = positions[gl_VertexID] * shs + shc;                                      \n"
"   f_uv = shp;                                                                         \n"
"   f_color = a_color;                                                                  \n"
"}                                                                                      \n"
;

const char *Fragment_Shader_Source =
"#version 330 core                             \n"
"in vec4 f_color;                              \n"
"in vec2 f_uv;                                 \n"
"uniform sampler2D uTexture;                   \n"
"out vec4 FragColor;                           \n"
"void main()                                   \n"
"{                                             \n"
"   vec4 sample = texture(uTexture, f_uv);     \n"
"   vec4 color = sample * f_color;             \n"
"   FragColor = color;                         \n"
"}                                             \n"
;

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

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(InputVertex) * 14, 0, GL_DYNAMIC_DRAW);

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

        glViewport(0, 0, w, h);
        glClearColor(0.2f, 0.2f, 0.2f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        float mx, my;
        vd_fw_get_mouse_statef(&mx, &my);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        InputVertex vertices[] = {
            { {0.0f,  0.0f}, {100.f, 100.f},      {00.0f, 00.0f}, { 1.0f,  1.0f}, {1.f, 0.f, 1.f, 1.f} },
            { {200.f, 0.0f}, {300.f, 100.f},      {00.0f, 00.0f}, { 1.0f,  1.0f}, {0.f, 1.f, 1.f, 1.f} },
            { {mx, my}, {mx + 100.f, my + 100.f}, {00.0f, 00.0f}, { 1.0f,  1.0f}, {1.f, 0.f, 0.f, 1.f} },
        };
        void *data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
        VD_MEMCPY(data, vertices, sizeof(vertices));

        glUnmapBuffer(GL_ARRAY_BUFFER);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glUseProgram(program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1i(glGetUniformLocation(program, "uTexture"), 0);
        glUniform2f(glGetUniformLocation(program, "uResolution"), (float)w, (float)h);

        glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, VD_ARRAY_COUNT(vertices));

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
