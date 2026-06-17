#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#include "vd_fw.h"

float colors[6][4] = {
    {0.5f, 0.3f, 0.2f, 1.0f},
    {0.3f, 0.5f, 0.1f, 1.0f},
    {0.2f, 0.6f, 0.7f, 1.0f},
    {0.8f, 0.2f, 0.1f, 1.0f},
    {0.8f, 0.1f, 0.48f, 1.0f},
};

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    int idx = 0;
    int idx_count = 6;

    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .configs = (VdFwGlConfig[]) {
                {
                    .version = VD_FW_GL_VERSION_3_3,
                },
                0,
            },
        },
        .window_options = {
            .borderless = 0,
        }
    });
    vd_fw_set_vsync_on(0);

    while (vd_fw_running()) {

        vd_fw_wait(0);

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        vd_fw_lock();

        int w, h;
        if (vd_fw_get_size(&w, &h)) {
            glViewport(0, 0, w, h);
            glClearColor(colors[idx][0], colors[idx][1], colors[idx][2], colors[idx][3]);
            glClear(GL_COLOR_BUFFER_BIT);

            idx++;

            if (idx >= idx_count) {
                idx = 0;
            }

            vd_fw_swap();
        }

        vd_fw_unlock();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
