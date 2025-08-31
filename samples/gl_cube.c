#define VD_FW_NO_CRT 1
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#include "vd_fw.h"

const char *Vertex_Shader_Source =
"#version 330 core                       \n"
"layout (location = 0) in vec3 aPos;     \n"
"void main()                             \n"
"{                                       \n"
"   gl_Position = vec4(aPos.xyz, 1.0);   \n"
"}                                       \n"
;

const char *Fragment_Shader_Source =
"#version 330 core                             \n"
"out vec3 FragColor;                           \n"
"void main()                                   \n"
"{                                             \n"
"   FragColor = vec3(1.0f, 0.5f, 0.2f);        \n"
"}                                             \n"
;

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .version = VD_FW_GL_VERSION_3_3,
            .debug_on = 1,
        }
    });
    vd_fw_set_vsync_on(1);

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

    while (vd_fw_running()) {

        unsigned long long ns = vd_fw_delta_ns();
        double milliseconds = (double)ns / 1000000.0;
        double delta_seconds64 = milliseconds / 1000.0;
        float delta_seconds = (float)delta_seconds64;

        int w, h;
        vd_fw_get_size(&w, &h);

        glViewport(0, 0, w, h);
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);



        vd_fw_swap_buffers();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
