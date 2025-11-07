#define VD_FW_NO_CRT 0
#define VD_MACRO_ABBREVIATIONS 1
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#include "vd_fw.h"
#include "vd_cg.h"

typedef struct {
    float yaw, pitch;
    f3    pos;
    f3    target;
    fquat orient;
} Camera;

static fquat camera_get_world_quat(Camera *camera)
{
    return camera->orient;
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
    init_info.gl.version = VD_FW_GL_VERSION_3_3;
    init_info.gl.debug_on = 1;
    vd_fw_init(&init_info);

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    Camera cam = {
        0.f, 0.f,
        fzero3(),
        fm3(0,0,5),
        fidentityquat(),
    };

    while (vd_fw_running()) {
        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        float delta_seconds = vd_fw_delta_s();

        f2 mouse;
        vd_fw_get_mouse_delta(&mouse.x, &mouse.y);

        f2 wheel;
        vd_fw_get_mouse_wheel(&wheel.x, &wheel.y);

        static float zoom = 5.f;

        zoom += wheel.y;

        if (vd_fw_get_mouse_down(VD_FW_MOUSE_BUTTON_LEFT)) {

            #if 1
            // Get initial camera forward vector
            f3 forward = fnoz3(fmulquat_3(cam.orient, fm3(0,0,1)));
            f3 iup     = fnoz3(fmulquat_3(cam.orient, fm3(0,1,0)));

            // Compute target point based on camera forward axis & zoom factor
            float zoom_factor = zoom;
            f3 target = fadd3(cam.pos, fscale3(forward, zoom_factor));

            // Compute B-A to get target vector
            f3 target_to_camera = fsub3(cam.pos, target);
            float distance = fsqrt(flensq3(target_to_camera));

            // Rotate horizontally around world up (yaw)
            fquat q_yaw = faxis_anglequat(fm3(0, 1, 0), fdeg2rad(mouse.x));

            // Rotate vertically around camera right (pitch)
            f3 right = fnoz3(fcross3(fm3(0,1,0), fnoz3(target_to_camera)));
            fquat q_pitch = faxis_anglequat(right, fdeg2rad(-mouse.y));
            fquat q = fmulquat(q_pitch, q_yaw);

            // After we get the TTC vector, rotate it
            // fquat rotate_by = feulerquat(fm3(0.f, fdeg2rad(mouse.x), 0.f));
            f3 new_target_to_camera = fmulquat_3(q, fnoz3(target_to_camera));

            // Set the new point
            cam.pos = fadd3(target, fscale3(new_target_to_camera, distance));

            // Look towards the target
            f3 new_forward = fnoz3(fsub3(target, cam.pos));
            f3 up = fm3(0,1,0);
            float d = fdot3(new_forward, up);
            if (fabs(d) > 0.999f) {
                printf("pick new up");
                up = (fabs(new_forward.y) < 0.9999f) ? fm3(0,0,1) : fm3(1, 0, 0);
                f3 r = fnoz3(fcross3(up, new_forward));
                up = fcross3(new_forward, r);
            }
            cam.orient = vd_flookrotquat(new_forward, up);
            fnozquat(cam.orient);
            #else
            float zoom_factor = 5.f;
            f3 forward = fnoz3(fmulquat_3(cam.orient, fm3(0,0,1)));
            f3 target = fadd3(cam.pos, fscale3(forward, zoom_factor));
            f3 v = fsub3(target, cam.pos);

            fquat q = feulerquat(fm3(fdeg2rad(mouse.y), fdeg2rad(mouse.x), 0));
            v = fmulquat_3(q, v);
            v = fadd3(v, cam.pos);

            cam.pos = v;

            #endif
        }

        if (vd_fw_get_key_pressed('1')) {

            printf("P %f %f %f\n", cam.pos.x, cam.pos.y, cam.pos.z);
            printf("R %f %f %f %f\n", cam.orient.x, cam.orient.y, cam.orient.z, cam.orient.w);
        }


        fquat camerarot = camera_get_world_quat(&cam);

        f3 forward = fnoz3(fmulquat_3(camerarot, fm3(0,0,1)));

        // float FWD = (float)(vd_fw_get_key_down('W') - vd_fw_get_key_down('S'));
        // float RGH = (float)(vd_fw_get_key_down('D') - vd_fw_get_key_down('A'));
        // float UPW = (float)(vd_fw_get_key_down('Q') - vd_fw_get_key_down('E'));
        // camerapos.z += FWD * delta_seconds * 5.f;
        // camerapos.x += RGH * delta_seconds * 5.f;
        // camerapos.y += UPW * delta_seconds * 5.f;


        int width, height;
        vd_fw_get_size(&width, &height);

        f4x4 projection = fperspective4x4(fdeg2rad(60.f), (float)width / (float)height, 0.001f, 1000.f);

        f4x4 view       = flookat4x4(cam.pos, fadd3(cam.pos, forward), fm3(0,1,0));


        vd_fw_compile_or_hotload_program(&p_3d, &p_3d_time, "./glsl/ed_3d.vert", "./glsl/ed_3d.frag");
        glUseProgram(p_3d);
        glUniformMatrix4fv(/* location */ 1, 1, GL_FALSE, projection.p);

        glUniformMatrix4fv(/* location */ 2, 1, GL_FALSE, view.p);

        glViewport(0, 0, width, height);
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static float T = 0.f;
        f4x4 model = fidentity4x4();
        frotate_yaw4x4(&model, fdeg2rad(T));
        ftranslate4x4(&model, fm3(0,0,5));

        glUniformMatrix4fv(/* location */ 0, 1, GL_FALSE, model.p);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        vd_fw_swap_buffers();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
