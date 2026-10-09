#include "vd_fw.h"

int main(int argc, char const *argv[]) {
    (void)argc;
    (void)argv;

    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .configs = (VdFwGlConfig[]) {
                {
                    .version = VD_FW_GL_VERSION_1_0,
                },
                0,
            },
        },
        .window_options = {
            .borderless = 0,
        }
    });

    while (vd_fw_running()) {
        vd_fw_poll(0);

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        vd_fw_lock();
        int w, h;
        vd_fw_get_size(&w, &h);
        glViewport(0, 0, w, h);
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex3f(0.5f, -0.5f, 0.0f);
        glVertex3f(-0.5f, +0.5f, 0.0f);
        glVertex3f(+0.5f, +0.5f, 0.0f);
        glEnd();

        vd_fw_swap();
        vd_fw_unlock();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"