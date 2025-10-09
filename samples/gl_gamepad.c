#include "disable_clang_deprecations.h"
#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#include "vd_fw.h"
#include "ext/stb_image.h"
#include "assert.h"

#define GL_CHECK(expr) do { (expr); int _e_ = glGetError(); if (_e_ != 0) { printf("Check at " __FILE__ ":%d failed with 0x%x\n", __LINE__, _e_); assert(0); }} while(0)

#define VERTEX_SOURCE \
"#version 330 core                                                                                                 \n" \
"layout (location = 0) in vec2 aPos;                                                                               \n" \
"layout (location = 1) in vec2 aTexCoord;                                                                          \n" \
"                                                                                                                  \n" \
"uniform vec2 rect_off;                                                                                            \n" \
"uniform vec2 rect_size;                                                                                           \n" \
"uniform mat4 projection;                                                                                          \n" \
"                                                                                                                  \n" \
"out vec2 rect_uv;                                                                                                 \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    rect_uv = aTexCoord;                                                                                          \n" \
"    gl_Position = projection * vec4(aPos * rect_size + rect_off, 0.0, 1.0f);                                      \n" \
"}                                                                                                                 \n" \

#define FRAGMENT_SOURCE \
"#version 330 core                                                                                                 \n" \
"in  vec2 rect_uv;                                                                                                 \n" \
"out vec4 FragColor;                                                                                               \n" \
"                                                                                                                  \n" \
"uniform vec4 rect_color;                                                                                          \n" \
"uniform sampler2D rect_texture;                                                                                   \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    vec4 tex_color = texture(rect_texture, rect_uv);                                                              \n" \
"    vec4 color = rect_color * tex_color;                                                                          \n" \
"    color.rgb *= color.a;                                                                                         \n" \
"    FragColor = color;                                                                                            \n" \
"}                                                                                                                 \n" \

static GLuint rect_shader;

/** Controller of 151.680x85.120 (1200x673) */
typedef struct {
    float controller_pos[2];
    float controller_dim[2];

    int button_a;
    float button_a_pos[2];
    float button_a_dim[2];

    int button_b;
    float button_b_pos[2];
    float button_b_dim[2];

    int button_x;
    float button_x_pos[2];
    float button_x_dim[2];

    int button_y;
    float button_y_pos[2];
    float button_y_dim[2];

    int button_dup;
    float button_dup_pos[2];
    float button_dup_dim[2];

    int button_dright;
    float button_dright_pos[2];
    float button_dright_dim[2];

    int button_ddown;
    float button_ddown_pos[2];
    float button_ddown_dim[2];

    int button_dleft;
    float button_dleft_pos[2];
    float button_dleft_dim[2];
} ControllerInfo;

typedef union {
    float e[4];
    struct {
        float r, g, b, a;
    } v;
} Color;

Color make_color(float r, float g, float b, float a)
{
    Color result;
    result.v.r = r;
    result.v.g = g;
    result.v.b = b;
    result.v.a = a;
    return result;
}

ControllerInfo Base_Controller_Info = {
    .controller_pos = {
        0.f, 0.f,
    },
    .controller_dim = {
        151.680f, 85.120f,
    },
    .button_a_pos = {
        100.07f, 29.76f, 
    },
    .button_a_dim = {
        8.3549576f,  8.3549576f,
    },

    .button_b_pos = {
        107.51f, 22.32f, 
    },
    .button_b_dim = {
        8.3549576f,  8.3549576f,
    },

    .button_x_pos = {
        92.49f, 22.03f, 
    },
    .button_x_dim = {
        8.3549576f,  8.3549576f,
    },

    .button_y_pos = {
        99.93f, 15.03f, 
    },
    .button_y_dim = {
        8.3549576f,  8.3549576f,
    },

    .button_dup_pos = {
        58.89f, 35.09f,
    },
    .button_dup_dim = {
        5.16f, 8.19f,
    },

    .button_dright_pos = {
        61.46f, 40.65f,
    },
    .button_dright_dim = {
        8.19f, 5.16f,
    },

    .button_ddown_pos = {
        58.88f, 43.23f,
    },
    .button_ddown_dim = {
        5.16f, 8.19f,
    },

    .button_dleft_pos = {
        53.27f, 40.65f,
    },
    .button_dleft_dim = {
        8.19f, 5.16f,
    },
};

static struct {
    float controller_dim[2];
    GLuint tex_controller;
    GLuint tex_button_a, tex_button_b, tex_button_x, tex_button_y;
    GLuint tex_button_dup, tex_button_dright, tex_button_ddown, tex_button_dleft;
} all;

typedef struct {
    int a;
} ControllerState;

static GLuint load_image(const char *file, int *w, int *h);
static void   put_image(GLuint texture, float x, float y, float w, float h, float color[4]);
static void   transform_controller_info(ControllerInfo *info, float x, float y, float wscale);
static void   draw_controller_info(ControllerInfo *info);
static Color  button_a_color(int pressed);
static Color  button_b_color(int pressed);
static Color  button_x_color(int pressed);
static Color  button_y_color(int pressed);
static Color  button_d_color(int pressed);
static Color  switch_color_digital(Color c1, Color c2, int p);

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
            .borderless = 0,
        }
    });
    vd_fw_set_vsync_on(1);
    vd_fw_set_title("Gamepad Sample");

    int iw, ih;
    all.tex_controller = load_image("assets/controller.png", &iw, &ih);
    all.controller_dim[0] = (float)iw;
    all.controller_dim[1] = (float)ih;

    all.tex_button_a = load_image("assets/controller_a.png", &iw, &ih);
    all.tex_button_b = load_image("assets/controller_b.png", &iw, &ih);
    all.tex_button_y = load_image("assets/controller_y.png", &iw, &ih);
    all.tex_button_x = load_image("assets/controller_x.png", &iw, &ih);
    all.tex_button_dup    = load_image("assets/controller_dup.png", &iw, &ih);
    all.tex_button_dright = load_image("assets/controller_dright.png", &iw, &ih);
    all.tex_button_ddown  = load_image("assets/controller_ddown.png", &iw, &ih);
    all.tex_button_dleft  = load_image("assets/controller_dleft.png", &iw, &ih);

    GLuint vs = vd_fw_compile_shader(GL_VERTEX_SHADER, VERTEX_SOURCE);
    GLuint fs = vd_fw_compile_shader(GL_FRAGMENT_SHADER, FRAGMENT_SOURCE);

    rect_shader = glCreateProgram();
    glAttachShader(rect_shader, vs);
    glAttachShader(rect_shader, fs);
    vd_fw_link_program(rect_shader);

    float rect_vertices[] = {
         0.0f,  0.0f, 0.f, 0.f,
        +1.0f,  0.0f, 1.f, 0.f,
         0.0f, +1.0f, 0.f, 1.f,
        +1.0f, +1.0f, 1.f, 1.f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ControllerInfo draw_info;
    while (vd_fw_running()) {

        int w, h;
        vd_fw_get_size(&w, &h);

        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GL_CHECK(glUseProgram(rect_shader));


        {
            float projection[16];
            vd_fw_u_ortho(0.f, (float)w, (float)h, 0.f, -1.f, 1.f, projection);
            glUniformMatrix4fv(glGetUniformLocation(rect_shader, "projection"), 1, GL_FALSE, projection);
        }

        draw_info.button_a      = vd_fw_get_gamepad_down(0, VD_FW_GAMEPAD_A);
        draw_info.button_b      = vd_fw_get_gamepad_down(0, VD_FW_GAMEPAD_B);
        draw_info.button_x      = vd_fw_get_gamepad_down(0, VD_FW_GAMEPAD_X);
        draw_info.button_y      = vd_fw_get_gamepad_down(0, VD_FW_GAMEPAD_Y);
        draw_info.button_dup    = vd_fw_get_gamepad_down(0, VD_FW_GAMEPAD_DUP);
        draw_info.button_dright = vd_fw_get_gamepad_down(0, VD_FW_GAMEPAD_DRIGHT);
        draw_info.button_ddown  = vd_fw_get_gamepad_down(0, VD_FW_GAMEPAD_DDOWN);
        draw_info.button_dleft  = vd_fw_get_gamepad_down(0, VD_FW_GAMEPAD_DLEFT);

        transform_controller_info(&draw_info, 0.f, 0.f, (float)w);
        draw_controller_info(&draw_info);

        vd_fw_swap_buffers();
    }

    return 0;
}

static GLuint load_image(const char *file, int *w, int *h)
{
    int nc;
    unsigned char *data = stbi_load(file, w, h, &nc, 4);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, *w, *h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    // glGenerateMipmap(GL_TEXTURE_2D);

    return texture;
}

static void put_image(GLuint texture, float x, float y, float w, float h, float color[4])
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glUniform4f(glGetUniformLocation(rect_shader, "rect_color"), color[0], color[1], color[2], color[3]);
    glUniform2f(glGetUniformLocation(rect_shader, "rect_size"), w, h);
    glUniform2f(glGetUniformLocation(rect_shader, "rect_off"), x, y);
    glUniform1i(glGetUniformLocation(rect_shader, "rect_texture"), 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

static void transform_controller_info(ControllerInfo *info, float x, float y, float wscale)
{
    float ratio = wscale / 151.680f;

    info->controller_dim[0] = Base_Controller_Info.controller_dim[0] * ratio;
    info->controller_dim[1] = Base_Controller_Info.controller_dim[1] * ratio;
    info->controller_pos[0] = Base_Controller_Info.controller_pos[0] * ratio + x;
    info->controller_pos[1] = Base_Controller_Info.controller_pos[1] * ratio + y;

    // A
    info->button_a_dim[0] = Base_Controller_Info.button_a_dim[0] * ratio;
    info->button_a_dim[1] = Base_Controller_Info.button_a_dim[1] * ratio;
    info->button_a_pos[0] = Base_Controller_Info.button_a_pos[0] * ratio + x - (info->button_a_dim[0] * 0.5f);
    info->button_a_pos[1] = Base_Controller_Info.button_a_pos[1] * ratio + y - (info->button_a_dim[1] * 0.5f);

    // B
    info->button_b_dim[0] = Base_Controller_Info.button_b_dim[0] * ratio;
    info->button_b_dim[1] = Base_Controller_Info.button_b_dim[1] * ratio;
    info->button_b_pos[0] = Base_Controller_Info.button_b_pos[0] * ratio + x - (info->button_b_dim[0] * 0.5f);
    info->button_b_pos[1] = Base_Controller_Info.button_b_pos[1] * ratio + y - (info->button_b_dim[1] * 0.5f);

    // X
    info->button_x_dim[0] = Base_Controller_Info.button_x_dim[0] * ratio;
    info->button_x_dim[1] = Base_Controller_Info.button_x_dim[1] * ratio;
    info->button_x_pos[0] = Base_Controller_Info.button_x_pos[0] * ratio + x - (info->button_x_dim[0] * 0.5f);
    info->button_x_pos[1] = Base_Controller_Info.button_x_pos[1] * ratio + y - (info->button_x_dim[1] * 0.5f);

    // Y
    info->button_y_dim[0] = Base_Controller_Info.button_y_dim[0] * ratio;
    info->button_y_dim[1] = Base_Controller_Info.button_y_dim[1] * ratio;
    info->button_y_pos[0] = Base_Controller_Info.button_y_pos[0] * ratio + x - (info->button_y_dim[0] * 0.5f);
    info->button_y_pos[1] = Base_Controller_Info.button_y_pos[1] * ratio + y - (info->button_y_dim[1] * 0.5f);

    // DUP
    info->button_dup_dim[0] = Base_Controller_Info.button_dup_dim[0] * ratio;
    info->button_dup_dim[1] = Base_Controller_Info.button_dup_dim[1] * ratio;
    info->button_dup_pos[0] = Base_Controller_Info.button_dup_pos[0] * ratio + x;
    info->button_dup_pos[1] = Base_Controller_Info.button_dup_pos[1] * ratio + y;

    // DRIGHT
    info->button_dright_dim[0] = Base_Controller_Info.button_dright_dim[0] * ratio;
    info->button_dright_dim[1] = Base_Controller_Info.button_dright_dim[1] * ratio;
    info->button_dright_pos[0] = Base_Controller_Info.button_dright_pos[0] * ratio + x;
    info->button_dright_pos[1] = Base_Controller_Info.button_dright_pos[1] * ratio + y;

    // DDOWN
    info->button_ddown_dim[0] = Base_Controller_Info.button_ddown_dim[0] * ratio;
    info->button_ddown_dim[1] = Base_Controller_Info.button_ddown_dim[1] * ratio;
    info->button_ddown_pos[0] = Base_Controller_Info.button_ddown_pos[0] * ratio + x;
    info->button_ddown_pos[1] = Base_Controller_Info.button_ddown_pos[1] * ratio + y;

    // DLEFT
    info->button_dleft_dim[0] = Base_Controller_Info.button_dleft_dim[0] * ratio;
    info->button_dleft_dim[1] = Base_Controller_Info.button_dleft_dim[1] * ratio;
    info->button_dleft_pos[0] = Base_Controller_Info.button_dleft_pos[0] * ratio + x;
    info->button_dleft_pos[1] = Base_Controller_Info.button_dleft_pos[1] * ratio + y;
}

static void draw_controller_info(ControllerInfo *info)
{
    put_image(all.tex_controller, info->controller_pos[0], info->controller_pos[1],
                                  info->controller_dim[0], info->controller_dim[1],
                                  (float[]){ 0.2f, 0.2f, 0.2f, 1.0f });

    put_image(all.tex_button_a,   info->button_a_pos[0], info->button_a_pos[1],
                                  info->button_a_dim[0], info->button_a_dim[1],
                                  button_a_color(info->button_a).e);

    put_image(all.tex_button_b,   info->button_b_pos[0], info->button_b_pos[1],
                                  info->button_b_dim[0], info->button_b_dim[1],
                                  button_b_color(info->button_b).e);

    put_image(all.tex_button_x,   info->button_x_pos[0], info->button_x_pos[1],
                                  info->button_x_dim[0], info->button_x_dim[1],
                                  button_x_color(info->button_x).e);

    put_image(all.tex_button_y,   info->button_y_pos[0], info->button_y_pos[1],
                                  info->button_y_dim[0], info->button_y_dim[1],
                                  button_y_color(info->button_y).e);

    put_image(all.tex_button_dup, info->button_dup_pos[0], info->button_dup_pos[1],
                                  info->button_dup_dim[0], info->button_dup_dim[1],
                                  button_d_color(info->button_dup).e);

    put_image(all.tex_button_dright, info->button_dright_pos[0], info->button_dright_pos[1],
                                  info->button_dright_dim[0], info->button_dright_dim[1],
                                  button_d_color(info->button_dright).e);

    put_image(all.tex_button_ddown, info->button_ddown_pos[0], info->button_ddown_pos[1],
                                  info->button_ddown_dim[0], info->button_ddown_dim[1],
                                  button_d_color(info->button_ddown).e);

    put_image(all.tex_button_dleft, info->button_dleft_pos[0], info->button_dleft_pos[1],
                                  info->button_dleft_dim[0], info->button_dleft_dim[1],
                                  button_d_color(info->button_dleft).e);
}

static Color button_a_color(int pressed)
{
    return switch_color_digital(
        make_color(0.2f, 0.2f, 0.2f, 1.0f),
        make_color(0.2f, 0.9f, 0.2f, 1.0f),
        pressed);
}

static Color button_b_color(int pressed)
{
    return switch_color_digital(
        make_color(0.2f, 0.2f, 0.2f, 1.0f),
        make_color(0.9f, 0.2f, 0.2f, 1.0f),
        pressed);
}

static Color button_y_color(int pressed)
{
    return switch_color_digital(
        make_color(0.2f, 0.2f, 0.2f, 1.0f),
        make_color(0.9f, 0.9f, 0.2f, 1.0f),
        pressed);
}

static Color button_x_color(int pressed)
{
    return switch_color_digital(
        make_color(0.2f, 0.2f, 0.2f, 1.0f),
        make_color(0.2f, 0.2f, 0.9f, 1.0f),
        pressed);
}

static Color button_d_color(int pressed)
{
    return switch_color_digital(
        make_color(0.3f, 0.3f, 0.3f, 1.f),
        make_color(0.7f, 0.7f, 0.7f, 1.f),
        pressed);
}

static Color switch_color_digital(Color c1, Color c2, int p)
{
    return p ? c2 : c1;
}

#define VD_FW_IMPL
#include "vd_fw.h"
#include "disable_clang_deprecations.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"
