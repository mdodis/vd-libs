#include "disable_clang_deprecations.h"
#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#define VD_FW_GAMEPAD_DB_DEFAULT_EXTERNAL
#include "vd_fw.h"
#include "ext/stb_image.h"
#include "assert.h"

#define GL_CHECK(expr) do {                                                   \
    (expr); int _e_ = glGetError();                                           \
    if (_e_ != 0) {                                                           \
        printf("Check at " __FILE__ ":%d failed with 0x%x\n", __LINE__, _e_); \
        assert(0);                                                            \
    }} while(0)

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
"precision highp float;                                                                                            \n" \
"in  vec2 rect_uv;                                                                                                 \n" \
"out vec4 FragColor;                                                                                               \n" \
"                                                                                                                  \n" \
"/* Gradient noise from Jorge Jimenez's presentation: */                                                           \n" \
"/* http://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare */                      \n" \
"float gradientNoise(in vec2 uv)                                                                                   \n" \
"{                                                                                                                 \n" \
"    return fract(52.9829189 * fract(dot(uv, vec2(0.06711056, 0.00583715))));                                      \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"uniform vec4 rect_color;                                                                                          \n" \
"uniform sampler2D rect_texture;                                                                                   \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    vec4 tex_color = texture(rect_texture, rect_uv);                                                              \n" \
"    vec4 color = rect_color * tex_color;                                                                          \n" \
"    color.rgb *= color.a;                                                                                         \n" \
"    color.rgb += (1.0 / 128.0) * gradientNoise(gl_FragCoord.xy) - (0.5 / 128.0);                                  \n" \
"    FragColor = color;                                                                                            \n" \
"}                                                                                                                 \n" \

static GLuint rect_shader;

typedef struct {
    int   state;
    float grad;
    float pos[2];
    float dim[2];
} DigitalInput;

typedef struct {
    float value;
    float pos[2];
    float dim[2];
} AnalogInput;

/** Controller of 107.32x74.94 (1201x673) */
typedef struct {
    float controller_pos[2];
    float controller_dim[2];

    DigitalInput a, b, x, y;
    DigitalInput dup, ddown, dright, dleft;
    DigitalInput start, select;
    DigitalInput l1, r1;

    float stick_l_base_pos[2];
    float stick_l_base_dim[2];

    float stick_r_base_pos[2];
    float stick_r_base_dim[2];

    float stick_l_value[2];
    float stick_l_pos[2];
    float stick_l_dim[2];

    float stick_r_value[2];
    float stick_r_pos[2];
    float stick_r_dim[2];

    float lt_value;
    float lt_bar_pos[2];
    float lt_bar_dim[2];

    float rt_value;
    float rt_bar_pos[2];
    float rt_bar_dim[2];

    int button_l3;
    float button_l3_grad;

    int button_r3;
    float button_r3_grad;
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
        107.32f, 74.94f,
    },

    .a = {  .pos = {77.68f, 24.38f, },
            .dim = {7.61f,  7.61f,  }},
    .b = {  .pos = {85.08f, 17.44f, },
            .dim = {7.61f,  7.61f,  }},
    .x = {  .pos = {70.35f, 16.89f, },
            .dim = {7.61f,  7.61f,  }},
    .y = {  .pos = {77.86f, 9.91f,  },
            .dim = {7.61f,  7.61f,  }},

    .dup = {    .pos = {36.75f, 29.94f,},
                .dim = {5.16f, 8.19f,  }},
    .dright = { .pos = {39.33f, 35.56f,},
                .dim = {8.19f, 5.16f,  }},
    .ddown = {  .pos = {36.75f, 38.14f,},
                .dim = {5.16f, 8.19f,  }},
    .dleft = {  .pos = {31.14f, 35.56f,},
                .dim = {8.19f, 5.16f,  }},

    .start = {  .pos = {59.01f, 18.06f,},
                .dim = {4.95f, 4.95f,  }},
    .select = { .pos = {43.15f, 18.06f,},
                .dim = {4.95f, 4.95f,  }},

    .l1 = { .pos = {16.34f, 0.0f, },
            .dim = {22.34f, 8.53f }},
    .r1 = { .pos = {68.56f, 0.0f, },
            .dim = {22.34f, 8.53f }},

    .stick_l_base_pos = {
        18.94f, 13.86f,
    },
    .stick_l_base_dim = {
        13.86f, 13.86f,
    },
    .stick_r_base_pos = {
        61.02f, 29.99f,
    },
    .stick_r_base_dim = {
        13.86f, 13.86f,
    },

    .stick_l_pos = {
        21.56f, 16.20f,
    },
    .stick_l_dim = {
        8.61f, 8.61f,
    },

    .stick_r_pos = {
        63.64f, 32.61f,
    },
    .stick_r_dim = {
        8.61f, 8.61f,
    },

    .lt_bar_pos = {
        38.98f, 57.19f,
    },
    .lt_bar_dim = {
        2.21f, 12.99f,
    },

    .rt_bar_pos = {
        67.0f, 57.19f,
    },
    .rt_bar_dim = {
        2.21f, 12.99f,
    },
};

static struct {
    float controller_dim[2];
    GLuint tex_controller;
    GLuint tex_button_a, tex_button_b, tex_button_x, tex_button_y;
    GLuint tex_button_dup, tex_button_dright, tex_button_ddown, tex_button_dleft;
    GLuint tex_button_select, tex_button_start;
    GLuint tex_button_l1, tex_button_r1;
    GLuint tex_stick_l_base, tex_stick_r_base;
    GLuint tex_stick_l, tex_stick_r;
    GLuint tex_lt_bar, tex_rt_bar;
    GLuint tex_radgrad;
} all;

typedef struct {
    int a;
} ControllerState;

static GLuint load_image(const char *file, int *w, int *h);
static void   put_image(GLuint texture, float x, float y, float w, float h, float color[4]);
static void   transform_controller_info(ControllerInfo *info, float x, float y, float wscale);
static void   draw_controller_info(ControllerInfo *info, int selected);
static Color  button_a_color(int pressed);
static Color  button_b_color(int pressed);
static Color  button_x_color(int pressed);
static Color  button_y_color(int pressed);
static Color  button_d_color(int pressed);
static Color  button_bumper_color(int pressed);
static Color  stick_base_color(void);
static Color  stick_color(int pressed);
static Color  bar_color(void);
static Color  fill_color(void);
static Color  switch_color_digital(Color c1, Color c2, int p);
static float  move_grad(float t, float a, int pressed);
static Color  reveal_color(Color color, float reveal_t);

static void   digital_input_draw(DigitalInput *input, GLuint texture, Color color);
static void   digital_input_draw_grad(DigitalInput *input, Color color);
static void   digital_input_transform(DigitalInput *input, DigitalInput *base, float x, float y, float ratio);
static void   digital_input_update(DigitalInput *input, float t, int index, int button);

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
    all.tex_controller    = load_image("assets/controller.png", &iw, &ih);
    all.tex_button_a      = load_image("assets/controller_a.png", &iw, &ih);
    all.tex_button_b      = load_image("assets/controller_b.png", &iw, &ih);
    all.tex_button_y      = load_image("assets/controller_y.png", &iw, &ih);
    all.tex_button_x      = load_image("assets/controller_x.png", &iw, &ih);
    all.tex_button_dup    = load_image("assets/controller_dup.png", &iw, &ih);
    all.tex_button_dright = load_image("assets/controller_dright.png", &iw, &ih);
    all.tex_button_ddown  = load_image("assets/controller_ddown.png", &iw, &ih);
    all.tex_button_dleft  = load_image("assets/controller_dleft.png", &iw, &ih);
    all.tex_button_select = load_image("assets/controller_select.png", &iw, &ih);
    all.tex_button_start  = load_image("assets/controller_start.png", &iw, &ih);
    all.tex_button_l1     = load_image("assets/controller_l1.png", &iw, &ih);
    all.tex_button_r1     = load_image("assets/controller_r1.png", &iw, &ih);
    all.tex_stick_l_base  = load_image("assets/controller_stick_l_base.png", &iw, &ih);
    all.tex_stick_r_base  = all.tex_stick_l_base;
    all.tex_stick_l       = load_image("assets/controller_stick_l.png", &iw, &ih);
    all.tex_stick_r       = all.tex_stick_l;
    all.tex_lt_bar        = load_image("assets/controller_lt.png", &iw, &ih);
    all.tex_rt_bar        = all.tex_lt_bar;
    all.tex_radgrad       = load_image("assets/radgrad.png", &iw, &ih);

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

    ControllerInfo draw_infos[VD_FW_GAMEPAD_COUNT_MAX] = {0};
    while (vd_fw_running()) {

        int w, h;
        vd_fw_get_size(&w, &h);

        float t = vd_fw_delta_s();

        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        GL_CHECK(glUseProgram(rect_shader));


        {
            float projection[16];
            vd_fw_u_ortho(0.f, (float)w, (float)h, 0.f, -1.f, 1.f, projection);
            glUniformMatrix4fv(glGetUniformLocation(rect_shader, "projection"), 1, GL_FALSE, projection);
        }

        int num_gamepads = vd_fw_get_gamepad_count();

        float mouse_x, mouse_y;
        vd_fw_get_mouse_statef(&mouse_x, &mouse_y);

        for (int i = 0; i < num_gamepads; ++i) {
            ControllerInfo *draw_info = &draw_infos[i];
            digital_input_update(&draw_info->a, t, i, VD_FW_GAMEPAD_A);
            digital_input_update(&draw_info->b, t, i, VD_FW_GAMEPAD_B);
            digital_input_update(&draw_info->x, t, i, VD_FW_GAMEPAD_X);
            digital_input_update(&draw_info->y, t, i, VD_FW_GAMEPAD_Y);
            digital_input_update(&draw_info->dup,    t, i, VD_FW_GAMEPAD_DUP);
            digital_input_update(&draw_info->dright, t, i, VD_FW_GAMEPAD_DRIGHT);
            digital_input_update(&draw_info->ddown,  t, i, VD_FW_GAMEPAD_DDOWN);
            digital_input_update(&draw_info->dleft,  t, i, VD_FW_GAMEPAD_DLEFT);
            digital_input_update(&draw_info->start,  t, i, VD_FW_GAMEPAD_START);
            digital_input_update(&draw_info->select, t, i, VD_FW_GAMEPAD_BACK);
            digital_input_update(&draw_info->l1, t, i, VD_FW_GAMEPAD_L1);
            digital_input_update(&draw_info->r1, t, i, VD_FW_GAMEPAD_R1);
            draw_info->button_l3       = vd_fw_get_gamepad_down(i, VD_FW_GAMEPAD_L3);
            draw_info->button_l3_grad  = move_grad(t * 7.f, draw_info->button_l3_grad, draw_info->button_l3);
            draw_info->button_r3       = vd_fw_get_gamepad_down(i, VD_FW_GAMEPAD_R3);
            draw_info->button_r3_grad  = move_grad(t * 7.f, draw_info->button_r3_grad, draw_info->button_r3);
            vd_fw_get_gamepad_axis(i, VD_FW_GAMEPAD_LH, &draw_info->stick_l_value[0]);
            vd_fw_get_gamepad_axis(i, VD_FW_GAMEPAD_LV, &draw_info->stick_l_value[1]);
            vd_fw_get_gamepad_axis(i, VD_FW_GAMEPAD_RH, &draw_info->stick_r_value[0]);
            vd_fw_get_gamepad_axis(i, VD_FW_GAMEPAD_RV, &draw_info->stick_r_value[1]);
            vd_fw_get_gamepad_axis(i, VD_FW_GAMEPAD_LT, &draw_info->lt_value);
            vd_fw_get_gamepad_axis(i, VD_FW_GAMEPAD_RT, &draw_info->rt_value);
        }

        {
            int count = num_gamepads;
            float ref_width = Base_Controller_Info.controller_dim[0];
            float ref_height = Base_Controller_Info.controller_dim[1];
            float width = (float)w;
            float height = (float)h;

            float best_scale = 0.f;
            if (count == 1) {
                float s1 = width / (ref_width);
                float s2 = height / (ref_height);
                float s = s1 <= s2 ? s1 : s2;
                best_scale = s;
            }

            int best_cols = 1;
            int best_rows = count;

            for (int cols = 1; cols < count; ++cols) {
                int rows = (count + cols - 1) / cols;
                float s1 = width / (cols * ref_width);
                float s2 = height / (rows * ref_height);

                float s = s1 <= s2 ? s1 : s2;

                if (s > best_scale) {
                    best_scale = s;
                    best_cols = cols;
                    best_rows = rows;
                }

            }

            float scaled_w = best_scale * ref_width;
            float scaled_h = best_scale * ref_height;

            float total_w = best_cols * scaled_w;
            float total_h = best_rows * scaled_h;
            float x0 = (width - total_w) / 2.f;
            float y0 = (height - total_h) / 2.f;

            for (int i = 0; i < count; ++i) {
                int row = i / best_cols;
                int col = i % best_cols;

                float x = x0 + col * scaled_w;
                float y = y0 + row * scaled_h;

                int mouse_inside = 0;
                if ((mouse_x > x) && (mouse_x < (x + scaled_w)) &&
                    (mouse_y > y) && (mouse_y < (y + scaled_h)))
                {
                    mouse_inside = 1;
                }

                transform_controller_info(&draw_infos[i], x, y, scaled_w);
                draw_controller_info(&draw_infos[i], mouse_inside);

                static int change = 0;
                if (mouse_inside && vd_fw_get_mouse_clicked(VD_FW_MOUSE_BUTTON_LEFT)) {
                    if (change == 0) {
                        vd_fw_set_gamepad_rumble(i, 0.5f, 0.5f);
                    } else {
                        vd_fw_set_gamepad_rumble(i, 0.f, 0.f);
                    }
                    change = !change;
                }
            }

        }

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
    float ratio = wscale / 107.32f;

    info->controller_dim[0] = Base_Controller_Info.controller_dim[0] * ratio;
    info->controller_dim[1] = Base_Controller_Info.controller_dim[1] * ratio;
    info->controller_pos[0] = Base_Controller_Info.controller_pos[0] * ratio + x;
    info->controller_pos[1] = Base_Controller_Info.controller_pos[1] * ratio + y;

    // A
    digital_input_transform(&info->a, &Base_Controller_Info.a, x, y, ratio);

    // B
    digital_input_transform(&info->b, &Base_Controller_Info.b, x, y, ratio);

    // X
    digital_input_transform(&info->x, &Base_Controller_Info.x, x, y, ratio);

    // Y
    digital_input_transform(&info->y, &Base_Controller_Info.y, x, y, ratio);

    // D-Pad
    digital_input_transform(&info->dup,     &Base_Controller_Info.dup,    x, y, ratio);
    digital_input_transform(&info->dright,  &Base_Controller_Info.dright, x, y, ratio);
    digital_input_transform(&info->ddown,   &Base_Controller_Info.ddown,  x, y, ratio);
    digital_input_transform(&info->dleft,   &Base_Controller_Info.dleft,  x, y, ratio);

    // Start/Select
    digital_input_transform(&info->start,   &Base_Controller_Info.start,  x, y, ratio);
    digital_input_transform(&info->select,  &Base_Controller_Info.select, x, y, ratio);

    // L1/R1
    digital_input_transform(&info->l1,  &Base_Controller_Info.l1, x, y, ratio);
    digital_input_transform(&info->r1,  &Base_Controller_Info.r1, x, y, ratio);

    // LEFT STICK BASE
    info->stick_l_base_dim[0] = Base_Controller_Info.stick_l_base_dim[0] * ratio;
    info->stick_l_base_dim[1] = Base_Controller_Info.stick_l_base_dim[1] * ratio;
    info->stick_l_base_pos[0] = Base_Controller_Info.stick_l_base_pos[0] * ratio + x;
    info->stick_l_base_pos[1] = Base_Controller_Info.stick_l_base_pos[1] * ratio + y;

    // RIGHT STICK BASE
    info->stick_r_base_dim[0] = Base_Controller_Info.stick_r_base_dim[0] * ratio;
    info->stick_r_base_dim[1] = Base_Controller_Info.stick_r_base_dim[1] * ratio;
    info->stick_r_base_pos[0] = Base_Controller_Info.stick_r_base_pos[0] * ratio + x;
    info->stick_r_base_pos[1] = Base_Controller_Info.stick_r_base_pos[1] * ratio + y;

    // LEFT STICK
    info->stick_l_dim[0] = Base_Controller_Info.stick_l_dim[0] * ratio;
    info->stick_l_dim[1] = Base_Controller_Info.stick_l_dim[1] * ratio;
    info->stick_l_pos[0] = Base_Controller_Info.stick_l_pos[0] * ratio + x + (info->stick_l_value[0] * 0.5f) * info->stick_l_dim[0] * 0.5f;
    info->stick_l_pos[1] = Base_Controller_Info.stick_l_pos[1] * ratio + y + (info->stick_l_value[1] * 0.5f) * info->stick_l_dim[1] * 0.5f;

    // RIGHT STICK
    info->stick_r_dim[0] = Base_Controller_Info.stick_r_dim[0] * ratio;
    info->stick_r_dim[1] = Base_Controller_Info.stick_r_dim[1] * ratio;
    info->stick_r_pos[0] = Base_Controller_Info.stick_r_pos[0] * ratio + x + (info->stick_r_value[0] * 0.5f) * info->stick_r_dim[0] * 0.5f;
    info->stick_r_pos[1] = Base_Controller_Info.stick_r_pos[1] * ratio + y + (info->stick_r_value[1] * 0.5f) * info->stick_r_dim[1] * 0.5f;

    // LT BAR
    info->lt_bar_dim[0] = Base_Controller_Info.lt_bar_dim[0] * ratio;
    info->lt_bar_dim[1] = Base_Controller_Info.lt_bar_dim[1] * ratio;
    info->lt_bar_pos[0] = Base_Controller_Info.lt_bar_pos[0] * ratio + x;
    info->lt_bar_pos[1] = Base_Controller_Info.lt_bar_pos[1] * ratio + y;

    // RT BAR
    info->rt_bar_dim[0] = Base_Controller_Info.rt_bar_dim[0] * ratio;
    info->rt_bar_dim[1] = Base_Controller_Info.rt_bar_dim[1] * ratio;
    info->rt_bar_pos[0] = Base_Controller_Info.rt_bar_pos[0] * ratio + x;
    info->rt_bar_pos[1] = Base_Controller_Info.rt_bar_pos[1] * ratio + y;
}

static void put_button_grad(float *pos, float *dim, Color color, float grad)
{
    float graddim[2] = {
        dim[0], dim[1],
    };
    graddim[0] *= 4;
    graddim[1] *= 4;

    float gradpos[2] = {
        (pos[0] + dim[0] * 0.5f) - graddim[0] * 0.5f,
        (pos[1] + dim[1] * 0.5f) - graddim[1] * 0.5f,
    };
    put_image(all.tex_radgrad, gradpos[0], gradpos[1],
                               graddim[0], graddim[1],
                               reveal_color(color, grad).e);
}

static void put_grad(GLuint tex, float *pos, float *dim, Color color, float grad)
{
    float graddim[2] = {
        dim[0], dim[1],
    };

    float gradpos[2] = {
        (pos[0] + dim[0] * 0.5f) - graddim[0] * 0.5f,
        (pos[1] + dim[1] * 0.5f) - graddim[1] * 0.5f,
    };
    put_image(tex, gradpos[0], gradpos[1],
                   graddim[0], graddim[1],
                   reveal_color(color, grad).e);
}

static void draw_controller_info(ControllerInfo *info, int selected)
{
    put_image(all.tex_controller, info->controller_pos[0], info->controller_pos[1],
                                  info->controller_dim[0], info->controller_dim[1],
                                  switch_color_digital(make_color(0.15f, 0.15f, 0.15f, 1.0f),
                                                       make_color(0.01f, 0.38f, 0.71f, 1.0f),
                                                       selected).e);

    digital_input_draw_grad(&info->a,       make_color(0.2f, 0.9f, 0.2f, 0.4f));
    digital_input_draw_grad(&info->b,       make_color(0.9f, 0.2f, 0.2f, 0.4f));
    digital_input_draw_grad(&info->x,       make_color(0.4f, 0.8f, 0.9f, 0.4f));
    digital_input_draw_grad(&info->y,       make_color(0.9f, 0.9f, 0.2f, 0.4f));
    digital_input_draw_grad(&info->dup,     make_color(1.0f, 1.0f, 1.0f, 0.125f));
    digital_input_draw_grad(&info->dright,  make_color(1.0f, 1.0f, 1.0f, 0.125f));
    digital_input_draw_grad(&info->ddown,   make_color(1.0f, 1.0f, 1.0f, 0.125f));
    digital_input_draw_grad(&info->dleft,   make_color(1.0f, 1.0f, 1.0f, 0.125f));
    digital_input_draw_grad(&info->start,   make_color(1.0f, 1.0f, 1.0f, 0.125f));
    digital_input_draw_grad(&info->select,  make_color(1.0f, 1.0f, 1.0f, 0.125f));
    digital_input_draw_grad(&info->l1,      make_color(1.0f, 1.0f, 1.0f, 0.125f));
    digital_input_draw_grad(&info->r1,      make_color(1.0f, 1.0f, 1.0f, 0.125f));

    digital_input_draw(&info->a, all.tex_button_a, button_a_color(info->a.state));
    digital_input_draw(&info->b, all.tex_button_b, button_b_color(info->b.state));
    digital_input_draw(&info->x, all.tex_button_x, button_x_color(info->x.state));
    digital_input_draw(&info->y, all.tex_button_y, button_y_color(info->y.state));

    digital_input_draw(&info->dup, all.tex_button_dup, button_d_color(info->dup.state));
    digital_input_draw(&info->dright, all.tex_button_dright, button_d_color(info->dright.state));
    digital_input_draw(&info->ddown, all.tex_button_ddown, button_d_color(info->ddown.state));
    digital_input_draw(&info->dleft, all.tex_button_dleft, button_d_color(info->dleft.state));

    digital_input_draw(&info->start, all.tex_button_start, button_d_color(info->start.state));
    digital_input_draw(&info->select, all.tex_button_select, button_d_color(info->select.state));

    digital_input_draw(&info->l1, all.tex_button_l1, button_d_color(info->l1.state));
    digital_input_draw(&info->r1, all.tex_button_r1, button_d_color(info->r1.state));
    put_image(all.tex_stick_l_base, info->stick_l_base_pos[0], info->stick_l_base_pos[1],
                                    info->stick_l_base_dim[0], info->stick_l_base_dim[1],
                                    stick_base_color().e);

    put_image(all.tex_stick_r_base, info->stick_r_base_pos[0], info->stick_r_base_pos[1],
                                    info->stick_r_base_dim[0], info->stick_r_base_dim[1],
                                    stick_base_color().e);

    put_button_grad(info->stick_l_pos,
                              info->stick_l_dim,
                              make_color(0.9f, 0.2f, 0.2f, 0.4f),
                              info->button_l3_grad);
    put_button_grad(info->stick_r_pos,
                              info->stick_r_dim,
                              make_color(0.9f, 0.2f, 0.2f, 0.4f),
                              info->button_r3_grad);

    put_image(all.tex_stick_l, info->stick_l_pos[0], info->stick_l_pos[1],
                                    info->stick_l_dim[0], info->stick_l_dim[1],
                                    stick_color(info->button_l3).e);

    put_image(all.tex_stick_r, info->stick_r_pos[0], info->stick_r_pos[1],
                                    info->stick_r_dim[0], info->stick_r_dim[1],
                                    stick_color(info->button_r3).e);

    {
        put_image(all.tex_lt_bar, info->lt_bar_pos[0], info->lt_bar_pos[1],
                                  info->lt_bar_dim[0], info->lt_bar_dim[1],
                                  bar_color().e);

        float fill_dim[2] = {
            info->lt_bar_dim[0], info->lt_value * info->lt_bar_dim[1],    
        };

        float fill_pos[2] = {
            info->lt_bar_pos[0], info->lt_bar_pos[1] - info->lt_value * info->lt_bar_dim[1] + info->lt_bar_dim[1],
        };

        put_image(all.tex_lt_bar, fill_pos[0], fill_pos[1],
                                  fill_dim[0], fill_dim[1],
                                  fill_color().e);
    }

    {
        put_image(all.tex_rt_bar, info->rt_bar_pos[0], info->rt_bar_pos[1],
                                  info->rt_bar_dim[0], info->rt_bar_dim[1],
                                  bar_color().e);

        float fill_dim[2] = {
            info->rt_bar_dim[0], info->rt_value * info->rt_bar_dim[1],    
        };

        float fill_pos[2] = {
            info->rt_bar_pos[0], info->rt_bar_pos[1] - info->rt_value * info->rt_bar_dim[1] + info->rt_bar_dim[1],
        };

        put_image(all.tex_rt_bar, fill_pos[0], fill_pos[1],
                                  fill_dim[0], fill_dim[1],
                                  fill_color().e);
    }
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
        make_color(0.4f, 0.8f, 1.0f, 1.0f),
        pressed);
}

static Color button_d_color(int pressed)
{
    return switch_color_digital(
        make_color(0.3f, 0.3f, 0.3f, 1.f),
        make_color(0.7f, 0.7f, 0.7f, 1.f),
        pressed);
}

static Color button_bumper_color(int pressed)
{
    return switch_color_digital(
        make_color(0.3f, 0.3f, 0.3f, 1.f),
        make_color(0.7f, 0.7f, 0.7f, 1.f),
        pressed);
}

static Color stick_base_color(void)
{
    return make_color(0.7f, 0.7f, 0.7f, 1.f);
}

static Color stick_color(int pressed)
{
    return switch_color_digital(
        stick_base_color(),
        make_color(0.9f, 0.2f, 0.2f, 1.f),
        pressed);
}

static Color bar_color(void)
{
    return make_color(0.2f, 0.2f, 0.2f, 1.f);
}

static Color fill_color(void)
{
    return make_color(0.7f, 0.7f, 0.7f, 1.f);
}

static Color switch_color_digital(Color c1, Color c2, int p)
{
    return p ? c2 : c1;
}

static float move_grad(float t, float a, int pressed)
{
    return (1.0f - t) * a + t * (pressed ? 1.f : 0.f);
}

static Color reveal_color(Color color, float reveal_t)
{
    Color result; 
    result.v.r = reveal_t * (color.v.r);
    result.v.g = reveal_t * (color.v.g);
    result.v.b = reveal_t * (color.v.b);
    result.v.a = reveal_t * (color.v.a);
    return result;
}

static void digital_input_draw(DigitalInput *input, GLuint texture, Color color)
{
    put_image(texture, input->pos[0], input->pos[1],
                       input->dim[0], input->dim[1],
                       color.e);
}

static void digital_input_draw_grad(DigitalInput *input, Color color)
{
    float graddim[2] = {
        input->dim[0], input->dim[1],
    };
    graddim[0] *= 4;
    graddim[1] *= 4;

    float gradpos[2] = {
        (input->pos[0] + input->dim[0] * 0.5f) - graddim[0] * 0.5f,
        (input->pos[1] + input->dim[1] * 0.5f) - graddim[1] * 0.5f,
    };
    put_image(all.tex_radgrad, gradpos[0], gradpos[1],
                               graddim[0], graddim[1],
                               reveal_color(color, input->grad).e);
}

static void digital_input_transform(DigitalInput *input, DigitalInput *base, float x, float y, float ratio)
{
    input->dim[0] = base->dim[0] * ratio;
    input->dim[1] = base->dim[1] * ratio;
    input->pos[0] = base->pos[0] * ratio + x;
    input->pos[1] = base->pos[1] * ratio + y;
}

static void digital_input_update(DigitalInput *input, float t, int index, int button)
{
    input->state = vd_fw_get_gamepad_down(index, button);
    input->grad  = move_grad(t * 7.f, input->grad, input->state);
}

#define VD_FW_IMPL
#include "vd_fw.h"
#include "disable_clang_deprecations.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"
