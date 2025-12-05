#include "disable_clang_deprecations.h"
#define VD_FW_NO_CRT 1
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#include "vd_fw.h"

#define VERTEX_SOURCE \
"#version 330 core                                                                                                 \n" \
"layout (location = 0) in vec3 aPos;                                                                               \n" \
"layout (location = 1) in vec2 aTexCoord;                                                                          \n" \
"                                                                                                                  \n" \
"out vec2 TexCoord;                                                                                                \n" \
"                                                                                                                  \n" \
"uniform mat4 projection;                                                                                          \n" \
"uniform mat4 view;                                                                                                \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    gl_Position = projection * view * vec4(aPos, 1.0f);                                                           \n" \
"    TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);                                                              \n" \
"}                                                                                                                 \n" \

#define FRAGMENT_SOURCE \
"#version 330 core                                                                                                 \n" \
"out vec4 FragColor;                                                                                               \n" \
"                                                                                                                  \n" \
"in vec2 TexCoord;                                                                                                 \n" \
"                                                                                                                  \n" \
"uniform sampler2D texture1;                                                                                       \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    FragColor = texture(texture1, TexCoord);                                                                      \n" \
"}                                                                                                                 \n" \

#include <string.h>

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .version = VD_FW_GL_VERSION_4_5,
            .debug_on = 1,
        },
        .window_options = {
            .borderless = 0,
        }
    });
    vd_fw_set_vsync_on(1);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int checkerboard[] = {
        0xFFFFFFFF, 0xFF000000,
        0xFF000000, 0xFFFFFFFF
    };

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, checkerboard);
    glGenerateMipmap(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);

    GLuint vshd = vd_fw_compile_shader(GL_VERTEX_SHADER, VERTEX_SOURCE);
    GLuint fshd = vd_fw_compile_shader(GL_FRAGMENT_SHADER, FRAGMENT_SOURCE);
    GLuint program = glCreateProgram();
    glAttachShader(program, vshd);
    glAttachShader(program, fshd);

    vd_fw_link_program(program);

    glDeleteShader(vshd);
    glDeleteShader(fshd);

    float camera_position[3] = {0.f, 0.f, -2.f};
    float camera_yaw   = 0.f;
    float camera_pitch = 30.f;
    float deg2rad = 3.14159265359f / 180.f;
    float camera_speed = 2.f;

    vd_fw_set_mouse_locked(1);

    vd_fw_set_title("GL Camera - Left Shift + F1 to toggle mouse, WASDQE to move camera");

    while (vd_fw_running()) {

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        float ds = vd_fw_delta_s();

        if (vd_fw_get_key_pressed(VD_FW_KEY_F1) && vd_fw_get_key_down(VD_FW_KEY_LSHIFT)) {
            vd_fw_set_mouse_locked(!vd_fw_get_mouse_locked());
        }

        static int w, h;
        if (vd_fw_get_size(&w, &h)) {
            glViewport(0, 0, w, h);
        }


        float fw, fh;
        fw = (float)w;
        fh = (float)h;

        if (vd_fw_get_mouse_locked()) {
            float mouse_delta_x, mouse_delta_y;
            vd_fw_get_mouse_delta(&mouse_delta_x, &mouse_delta_y);

            camera_yaw   += mouse_delta_x;
            camera_pitch -= mouse_delta_y;
        };

        if (camera_pitch < -89.9f) camera_pitch = -89.9f;
        if (camera_pitch > +89.9f) camera_pitch = +89.9f;

        if (camera_yaw > +360.f) camera_yaw -= 360.f;
        if (camera_yaw < -360.f) camera_yaw += 360.f;

        float camera_forward[3] = {
            VD_FW_COS(deg2rad * camera_pitch) * VD_FW_SIN(deg2rad * camera_yaw),
            VD_FW_SIN(deg2rad * camera_pitch),
            VD_FW_COS(deg2rad * camera_pitch) * VD_FW_COS(deg2rad * camera_yaw)
        };

        float camera_forward_len = VD_FW_SQRT(
            camera_forward[0] * camera_forward[0] +
            camera_forward[1] * camera_forward[1] +
            camera_forward[2] * camera_forward[2]);

        camera_forward[0] = camera_forward[0] / camera_forward_len;
        camera_forward[1] = camera_forward[1] / camera_forward_len;
        camera_forward[2] = camera_forward[2] / camera_forward_len;

        float camera_ref_up[3] = {0.f, 1.f, 0.f};
        float camera_right[3] = {
            camera_forward[1] * camera_ref_up[2] - camera_forward[2] * camera_ref_up[1],
            camera_forward[2] * camera_ref_up[0] - camera_forward[0] * camera_ref_up[2],
            camera_forward[0] * camera_ref_up[1] - camera_forward[1] * camera_ref_up[0],
        };

        float camera_right_len = VD_FW_SQRT(
            camera_right[0] * camera_right[0] +
            camera_right[1] * camera_right[1] +
            camera_right[2] * camera_right[2]);

        camera_right[0] = camera_right[0] / camera_right_len;
        camera_right[1] = camera_right[1] / camera_right_len;
        camera_right[2] = camera_right[2] / camera_right_len;

        float fwdir = (float)(vd_fw_get_key_down('W') - vd_fw_get_key_down('S'));
        float rgdir = (float)(vd_fw_get_key_down('A') - vd_fw_get_key_down('D'));
        float updir = (float)(vd_fw_get_key_down('Q') - vd_fw_get_key_down('E'));

        float camera_move_dir[3] = {
            fwdir * camera_forward[0] + rgdir * camera_right[0] + updir * camera_ref_up[0],
            fwdir * camera_forward[1] + rgdir * camera_right[1] + updir * camera_ref_up[1],
            fwdir * camera_forward[2] + rgdir * camera_right[2] + updir * camera_ref_up[2],
        };

        float camera_dir_lensq = 
            camera_move_dir[0] * camera_move_dir[0] +
            camera_move_dir[1] * camera_move_dir[1] +
            camera_move_dir[2] * camera_move_dir[2];

        if (camera_dir_lensq > 0.0001f) {
            float camera_move_dir_len = VD_FW_SQRT(camera_dir_lensq);

            camera_move_dir[0] = camera_move_dir[0] / camera_move_dir_len;
            camera_move_dir[1] = camera_move_dir[1] / camera_move_dir_len;
            camera_move_dir[2] = camera_move_dir[2] / camera_move_dir_len;

            camera_position[0] += camera_move_dir[0] * camera_speed * ds;
            camera_position[1] += camera_move_dir[1] * camera_speed * ds;
            camera_position[2] += camera_move_dir[2] * camera_speed * ds;
        }

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float projection[16] = {0.f};
        vd_fw_u_perspective(60.f, fw / fh, 0.1f, 100.0f, projection);


        float view[16] = {0.f};
        float ctar[3] = {camera_position[0] + camera_forward[0], camera_position[1] + camera_forward[1], camera_position[2] + camera_forward[2]};
        float cup[3]  = {0.f, 1.f, 0.f};
        vd_fw_u_lookat(camera_position, ctar, cup, view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, projection);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, view);
        glUniform1i(glGetUniformLocation(program, "texture1"), 0);

        glBindVertexArray(VAO);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        vd_fw_swap_buffers();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
#include "disable_clang_deprecations.h"
