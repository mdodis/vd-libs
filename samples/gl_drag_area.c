#include "disable_clang_deprecations.h"
#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#include "vd_fw.h"

#define VERTEX_SOURCE \
"#version 330 core                                                                                                 \n" \
"layout (location = 0) in vec2 aPos;                                                                               \n" \
"                                                                                                                  \n" \
"uniform vec2 rect_off;                                                                                            \n" \
"uniform vec2 rect_size;                                                                                           \n" \
"uniform mat4 projection;                                                                                          \n" \
"                                                                                                                  \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    gl_Position = projection * vec4(aPos * rect_size + rect_off, 0.0, 1.0f);                                      \n" \
"}                                                                                                                 \n" \

#define FRAGMENT_SOURCE \
"#version 330 core                                                                                                 \n" \
"out vec4 FragColor;                                                                                               \n" \
"                                                                                                                  \n" \
"uniform vec4 rect_color;                                                                                          \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    FragColor = rect_color;                                                                                       \n" \
"}                                                                                                                 \n" \


int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .version = VD_FW_GL_VERSION_3_3,
            .debug_on = 1,
        },
        .window_options = {
            .borderless = 1,
        }
    });
    vd_fw_set_vsync_on(0);

    const char *vertex_shader_source = VERTEX_SOURCE;
    const char *fragment_shader_source = FRAGMENT_SOURCE;

    // Compile shaders
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, 0);
    glCompileShader(vertex_shader);

    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, 0);
    glCompileShader(fragment_shader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    float rect_vertices[] = {
         0.0f,  0.0f,
        +1.0f,  0.0f,
         0.0f, +1.0f,
        +1.0f, +1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    vd_fw_set_receive_ncmouse(1);

    while (vd_fw_running()) {

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        int w, h;
        vd_fw_get_size(&w, &h);

        glViewport(0, 0, w, h);
        glClearColor(0.5f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);
        glBindVertexArray(VAO);

        {
            float projection[16];
            vd_fw_u_ortho(0.f, (float)w, (float)h, 0.f, -1.f, 1.f, projection);
            glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, projection);
        }

        float mx, my;
        vd_fw_get_mouse_statef(&mx, &my);

        int draggable_rect[4] = {
            0,  // left
            0,  // top
            w,  // right
            30, // bottom
        };

        int exclude_rects[1][4] = {
            {
                w - 30, // left
                0,      // top
                w,      // right
                30,     // bottom
            }
        };
        vd_fw_set_ncrects(draggable_rect, 1, exclude_rects);

        {
            glUniform4f(glGetUniformLocation(program, "rect_color"), 0.2f, 0.2f, 0.2f, 1.f);
            glUniform2f(glGetUniformLocation(program, "rect_size"), (float)w, 30.f);
            glUniform2f(glGetUniformLocation(program, "rect_off"), 0.f, 0.f);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        {
            float button_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
            int mouse_inside_close_button =
                (mx > ((float)w - 30.f)) &&
                (my > (0.f) && my < (30.f));

            if (mouse_inside_close_button) {
                button_color[0] = 0.7f;    
                button_color[1] = 0.0f;    
                button_color[2] = 0.0f;    
                button_color[3] = 1.0f;    
            }

            glUniform4f(glGetUniformLocation(program, "rect_color"), button_color[0], button_color[1], button_color[2], button_color[3]);
            glUniform2f(glGetUniformLocation(program, "rect_size"), 30.f, 30.f);
            glUniform2f(glGetUniformLocation(program, "rect_off"), (float)w - 30.f, 0.f);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        glUseProgram(0);
        glBindVertexArray(0);


        vd_fw_swap_buffers();
    }

    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
#include "disable_clang_deprecations.h"
