#include "disable_clang_deprecations.h"
#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#define VD_FW_PREFER_INTEGRATED_GPU
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

#define VERTEX_SOURCE3D \
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

#define FRAGMENT_SOURCE3D \
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

#define DEADZONE_STICK (0.1f)

static void map_stick_deadzone(float input[2], float deadzone, float output[2])
{
    output[0] = output[1] = 0.f;

    float magnitude = VD_FW_SQRT(input[0] * input[0] + input[1] * input[1]);
    if ((magnitude <= 0.000001f) || (magnitude < deadzone)) {
        return;    
    }

    float norm_x = input[0] / magnitude;
    float norm_y = input[1] / magnitude;

    float norm_magnitude = (magnitude - deadzone) / (1.f - deadzone);
    if (norm_magnitude > 1.f) norm_magnitude = 1.f;

    output[0] = norm_x * norm_magnitude;
    output[1] = norm_y * norm_magnitude;
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .version = VD_FW_GL_VERSION_3_3,
            .debug_on = 0,
        },
        .window_options = {
            .borderless = 1,
        }
    });
    vd_fw_set_vsync_on(1);

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

    const char *vertex_shader_source3d = VERTEX_SOURCE3D;
    const char *fragment_shader_source3d = FRAGMENT_SOURCE3D;

    GLuint vertex_shader3d = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader3d, 1, &vertex_shader_source3d, 0);
    glCompileShader(vertex_shader3d);

    GLuint fragment_shader3d = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader3d, 1, &fragment_shader_source3d, 0);
    glCompileShader(fragment_shader3d);

    GLuint program3d = glCreateProgram();
    glAttachShader(program3d, vertex_shader3d);
    glAttachShader(program3d, fragment_shader3d);
    glLinkProgram(program3d);

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

    float vertices3d[] = {
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

    unsigned int VBO3D, VAO3D;
    glGenVertexArrays(1, &VAO3D);
    glGenBuffers(1, &VBO3D);
    glBindVertexArray(VAO3D);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3D);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3d), vertices3d, GL_STATIC_DRAW);
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

    vd_fw_set_receive_ncmouse(1);

    float camera_position[3] = {0.f, 0.f, -2.f};
    float camera_yaw   = 0.f;
    float camera_pitch = 30.f;
    float deg2rad = 3.14159265359f / 180.f;
    float camera_speed = 2.f;

    // vd_fw_set_mouse_locked(1);

    {
        unsigned int icon_pixels[32*32];
        for (int y = 0; y < 32; ++y) {
            for (int x = 0; x < 32; ++x) {

                float t = ((float)(y * 32 + x)) / (32.f * 32.f);

                t = (VD_FW_SIN(t * 2) + 1.0f) * 0.5f;

                float r = 0.7f * t;
                float g = 0.2f * t;
                float b = 0.0f * t;

                icon_pixels[y * 32 + x] = 0xFF << 24                       |
                                          ((unsigned char)(r * 255)) << 16 |
                                          ((unsigned char)(g * 255)) <<  8 |
                                          ((unsigned char)(b * 255)) <<  0;
            }
        }
        vd_fw_set_app_icon(icon_pixels, 32, 32);
    }

    while (vd_fw_running()) {

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        int w, h;
        vd_fw_get_size(&w, &h);

        float ds = vd_fw_delta_s();

        glViewport(0, 0, w, h);
        glClearColor(0.5f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glDisable(GL_DEPTH_TEST);
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
                w - 30 * 3, // left
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

            if (mouse_inside_close_button && vd_fw_get_mouse_clicked(VD_FW_MOUSE_BUTTON_LEFT)) {
                vd_fw_quit();
            }

            glUniform4f(glGetUniformLocation(program, "rect_color"), button_color[0], button_color[1], button_color[2], button_color[3]);
            glUniform2f(glGetUniformLocation(program, "rect_size"), 30.f, 30.f);
            glUniform2f(glGetUniformLocation(program, "rect_off"), (float)w - 30.f, 0.f);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        {
            float button_color[4] = {0.7f, 0.7f, 0.7f, 1.0f};
            int mouse_inside_maximize_button =
                (mx > ((float)w - 30.f * 2.f)) && (mx < ((float)w - 30.f)) &&
                (my > (0.f) && my < (30.f));

            if (mouse_inside_maximize_button) {
                button_color[0] = 0.9f;
                button_color[1] = 0.9f;
                button_color[2] = 0.9f;
                button_color[3] = 1.0f;
            }

            if (mouse_inside_maximize_button && vd_fw_get_mouse_clicked(VD_FW_MOUSE_BUTTON_LEFT)) {
                int is_maximized;
                vd_fw_get_maximized(&is_maximized);
                if (is_maximized) {
                    vd_fw_normalize();
                } else {
                    vd_fw_maximize();
                }
            }

            glUniform4f(glGetUniformLocation(program, "rect_color"), button_color[0], button_color[1], button_color[2], button_color[3]);
            glUniform2f(glGetUniformLocation(program, "rect_size"), 20.f, 20.f);
            glUniform2f(glGetUniformLocation(program, "rect_off"), (float)w - 30.f * 2.f, 5.f);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        {
            float button_color[4] = {0.7f, 0.7f, 0.7f, 1.0f};
            int mouse_inside_minimize_button =
                (mx > ((float)w - 30.f * 3.f)) && (mx < ((float)w - 30.f * 2.f)) &&
                (my > (0.f) && my < (30.f));

            if (mouse_inside_minimize_button) {
                button_color[0] = 0.9f;
                button_color[1] = 0.9f;
                button_color[2] = 0.9f;
                button_color[3] = 1.0f;
            }

            if (mouse_inside_minimize_button && vd_fw_get_mouse_clicked(VD_FW_MOUSE_BUTTON_LEFT)) {
                vd_fw_minimize();
            }

            glUniform4f(glGetUniformLocation(program, "rect_color"), button_color[0], button_color[1], button_color[2], button_color[3]);
            glUniform2f(glGetUniformLocation(program, "rect_size"), 20.f, 5.f);
            glUniform2f(glGetUniformLocation(program, "rect_off"), (float)w - 30.f * 3.f, 20.f);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        }

        if (vd_fw_get_key_pressed(VD_FW_KEY_F1) && vd_fw_get_key_down(VD_FW_KEY_LSHIFT)) {
            vd_fw_set_mouse_locked(!vd_fw_get_mouse_locked());
        }

        if (vd_fw_get_key_pressed(VD_FW_KEY_F11)) {
            vd_fw_set_fullscreen(!vd_fw_get_fullscreen());
        }

        glEnable(GL_DEPTH_TEST);
        {
            float fw = (float)w;
            float fh = (float)h;

            if (vd_fw_get_mouse_locked()) {
                float mouse_delta_x, mouse_delta_y;
                vd_fw_get_mouse_delta(&mouse_delta_x, &mouse_delta_y);

                camera_yaw   += mouse_delta_x;
                camera_pitch -= mouse_delta_y;
            } else if (vd_fw_get_gamepad_count() > 0) {
                float right_stick_raw[2];
                float right_stick[2];
                vd_fw_get_gamepad_axis(0, VD_FW_GAMEPAD_RH, &right_stick_raw[0]);
                vd_fw_get_gamepad_axis(0, VD_FW_GAMEPAD_RV, &right_stick_raw[1]);

                map_stick_deadzone(right_stick_raw, DEADZONE_STICK, right_stick);

                camera_yaw   += right_stick[0] * ds * 100.f;
                camera_pitch -= right_stick[1] * ds * 100.f;
            }

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

            float fwdir = 0.f;
            float rgdir = 0.f;
            float updir = 0.f;

            fwdir = (float)(vd_fw_get_key_down('W') - vd_fw_get_key_down('S'));
            rgdir = (float)(vd_fw_get_key_down('A') - vd_fw_get_key_down('D'));
            updir = (float)(vd_fw_get_key_down('Q') - vd_fw_get_key_down('E'));

            if (vd_fw_get_gamepad_count() > 0) {

                float left_stick_raw[2];
                float left_stick[2];
                vd_fw_get_gamepad_axis(0, VD_FW_GAMEPAD_LV, &left_stick_raw[0]);
                vd_fw_get_gamepad_axis(0, VD_FW_GAMEPAD_LH, &left_stick_raw[1]);

                map_stick_deadzone(left_stick_raw, DEADZONE_STICK, left_stick);

                fwdir -= left_stick[0];
                rgdir -= left_stick[1];
            }

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

            glViewport(0, 0, w, h - 30);
            glUseProgram(program3d);
            glBindVertexArray(VAO3D);

            float projection[16] = {0.f};
            vd_fw_u_perspective(60.f, fw / fh, 0.1f, 100.0f, projection);

            float view[16] = {0.f};
            float ctar[3] = {camera_position[0] + camera_forward[0], camera_position[1] + camera_forward[1], camera_position[2] + camera_forward[2]};
            float cup[3]  = {0.f, 1.f, 0.f};
            vd_fw_u_lookat(camera_position, ctar, cup, view);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            glUseProgram(program3d);
            glUniformMatrix4fv(glGetUniformLocation(program3d, "projection"), 1, GL_FALSE, projection);
            glUniformMatrix4fv(glGetUniformLocation(program3d, "view"), 1, GL_FALSE, view);
            glUniform1i(glGetUniformLocation(program3d, "texture1"), 0);

            glBindVertexArray(VAO3D);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        vd_fw_swap_buffers();
    }

    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
#include "disable_clang_deprecations.h"
