#define VD_FW_NO_CRT 0
#define VD_MACRO_ABBREVIATIONS 1
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#include "vd_fw.h"
#include "vd_cg.h"
#include <assert.h>

#define GL_CHECK(expr) do {                                                   \
    (expr); int _e_ = glGetError();                                           \
    if (_e_ != 0) {                                                           \
        printf("Check at %s:%d (%s) failed with 0x%x\n", __FILE__, __LINE__, #expr, _e_); \
        assert(0);                                                            \
    }} while(0)



typedef struct {
    float yaw, pitch;
    f3    pos;
    fquat orient;
} Camera;

static fquat camera_get_world_quat(Camera *camera)
{
    return camera->orient;
}

#pragma pack(push, 1)
typedef struct {
    f3 p0;
    f3 p1;
    f4 color;
} DebugInput;
#pragma pack(pop)

static DebugInput Debug_Inputs[1024];
static int num_debug_props = 0;
static GLuint p_debug_prop = 0;
static unsigned long long p_debug_prop_time = 0;
static GLuint vao_debug_prop;
static GLuint vbo_debug_prop;

static void push_line(f3 from, f3 to, f4 color)
{
    Debug_Inputs[num_debug_props++] = (DebugInput) {
        .p0 = from,
        .p1 = to,
        .color = color,
    };
}

static void debug_init(void)
{
    unsigned int VBO, VAO;
    GL_CHECK(glGenVertexArrays(1, &VAO));
    GL_CHECK(glGenBuffers(1, &VBO));
    GL_CHECK(glBindVertexArray(VAO));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(DebugInput) * 1024, NULL, GL_STATIC_DRAW));
    GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugInput), (void*)offsetof(DebugInput, p0)));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribDivisor(0, 1));
    GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugInput), (void*)offsetof(DebugInput, p1)));
    GL_CHECK(glEnableVertexAttribArray(1));
    GL_CHECK(glVertexAttribDivisor(1, 1));
    GL_CHECK(glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(DebugInput), (void*)offsetof(DebugInput, color)));
    GL_CHECK(glEnableVertexAttribArray(2));
    GL_CHECK(glVertexAttribDivisor(2, 1));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK(glBindVertexArray(0));

    vao_debug_prop = VAO;
    vbo_debug_prop = VBO;
}

static void debug_update(void)
{

    glUseProgram(p_debug_prop);

    glEnable(GL_BLEND);
    GL_CHECK(glBindVertexArray(vao_debug_prop));
    glBindBuffer(GL_ARRAY_BUFFER, vbo_debug_prop);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_debug_props * sizeof(DebugInput), (unsigned char*)Debug_Inputs);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArraysInstanced(
        GL_TRIANGLES,
        0,
        6,
        num_debug_props);

    GL_CHECK(glBindVertexArray(0));

    num_debug_props = 0;
}

int main(int argc, char const *argv[])
{
    GLuint              p_3d = 0;
    unsigned long long  p_3d_time = 0;
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 1.f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.f,
    };

    (void)argc;
    (void)argv;

    VdFwInitInfo init_info = {0};
    init_info.gl.version = VD_FW_GL_VERSION_4_1;
    init_info.gl.debug_on = 1;
    vd_fw_init(&init_info);

    unsigned int VBO, VAO;
    GL_CHECK(glGenVertexArrays(1, &VAO));
    GL_CHECK(glGenBuffers(1, &VBO));
    GL_CHECK(glBindVertexArray(VAO));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, VBO));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
    GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0)));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))));
    GL_CHECK(glEnableVertexAttribArray(1));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CHECK(glBindVertexArray(0));

    glEnable(GL_DEPTH_TEST);

    Camera cam = {
        0.f, 0.f,
        fzero3(),
        fidentityquat(),
    };

    debug_init();

    while (vd_fw_running()) {
        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        float delta_seconds = vd_fw_delta_s();

        f2 mouse;
        vd_fw_get_mouse_delta(&mouse.x, &mouse.y);

        f2 wheel;
        vd_fw_get_mouse_wheel(&wheel.x, &wheel.y);

        if (vd_fw_get_mouse_down(VD_FW_MOUSE_BUTTON_RIGHT)) {
            vd_fw_set_mouse_locked(1);

            cam.yaw += mouse.x;
            cam.pitch += mouse.y;

            cam.yaw = fwrap_degrees(cam.yaw);
            cam.pitch = fclamp(-89.f, cam.pitch, +89.f);
            cam.orient = feulerquat(fm3(fdeg2rad(cam.pitch), fdeg2rad(cam.yaw), 0.f));

        } else {
            vd_fw_set_mouse_locked(0);
        }

        static float ey = 0.f;
        if (vd_fw_get_key_down(VD_FW_KEY_ARROW_UP)) {
            ey += delta_seconds * 2.f;
        }
        if (vd_fw_get_key_down(VD_FW_KEY_ARROW_DOWN)) {
            ey -= delta_seconds * 2.f;
        }

        push_line(fm3(0,ey,0), fm3(0,ey,5), fm4(1,0,0,1));
        push_line(fm3(0,0,0), fm3(0,5,0), fm4(0.2f,0.3f,0.5f,1));


        fquat camerarot = camera_get_world_quat(&cam);

        f3 forward = fmulquat_3(camerarot, fm3(0,0,1));
        float FWD = (float)(vd_fw_get_key_down('W') - vd_fw_get_key_down('S'));
        float RGH = (float)(vd_fw_get_key_down('D') - vd_fw_get_key_down('A'));
        float UPW = (float)(vd_fw_get_key_down('Q') - vd_fw_get_key_down('E'));
        cam.pos = fadd3(cam.pos, fscale3(forward, FWD * 5.f * delta_seconds));

        int width, height;
        vd_fw_get_size(&width, &height);

        f4x4 projection = fperspective4x4(fdeg2rad(60.f), (float)width / (float)height, 0.001f, 1000.f);

        f4x4 view       = flookat4x4(cam.pos, fadd3(cam.pos, forward), fm3(0,1,0));
        // f4x4 V_P  = ftranslation4x4(fscale3(cam.pos, -1.f));
        // f4x4 V_R  = fto4x4quat(cam.orient); 
        // f4x4 view = fmul4x4(&V_R, &V_P);


        vd_fw_compile_or_hotload_program(&p_3d, &p_3d_time,
                                         "./glsl/ed_3d.vert", "./glsl/ed_3d.frag");
        vd_fw_compile_or_hotload_program(&p_debug_prop, &p_debug_prop_time,
                                         "./glsl/ed_line.vert", "./glsl/ed_line.frag");

        GL_CHECK(glUseProgram(p_3d));
        GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(p_3d, "u_proj"), 1, GL_FALSE, projection.p));
        GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(p_3d, "u_view"), 1, GL_FALSE, view.p));
        GL_CHECK(glUseProgram(p_debug_prop));
        GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(p_debug_prop, "u_proj"), 1, GL_FALSE, projection.p));
        GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(p_debug_prop, "u_view"), 1, GL_FALSE, view.p));
        GL_CHECK(glUniform2f(glGetUniformLocation(p_debug_prop, "u_resolution"), (float)width, (float)height));

        GL_CHECK(glViewport(0, 0, width, height));
        GL_CHECK(glClearColor(0.2f, 0.2f, 0.2f, 1.f));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        GL_CHECK(glUseProgram(p_3d));

        GL_CHECK(glBindVertexArray(VAO));

        static float T = 0.f;
        f4x4 model = fidentity4x4();
        frotate_yaw4x4(&model, fdeg2rad(T));
        ftranslate4x4(&model, fm3(0,0,5));
        GL_CHECK(glUniformMatrix4fv(glGetUniformLocation(p_3d, "u_modl"), 1, GL_FALSE, model.p));
        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 36));

        GL_CHECK(glBindVertexArray(0));
        GL_CHECK(glUseProgram(0));

        debug_update();

        vd_fw_swap_buffers();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
