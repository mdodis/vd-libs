#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#include "vd_fw.h"
#include <stdio.h>

#if VD_FW_DEFER_MAIN
int defer_main(int argc, char const *argv[]);
int main(int argc, char const *argv[])
{
    return vd_fw_main(argc, argv, defer_main);
}
int defer_main(int argc, char const *argv[])
#else
int main(int argc, char const *argv[])
#endif
{
    (void)argc;
    (void)argv;

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

        vd_fw_poll(0);

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        float scale;
        if (vd_fw_get_scale(&scale)) {
            printf("New Scale: %f\n", scale);
        }

        vd_fw_lock();

        int w, h;
        vd_fw_get_size(&w, &h);

        glViewport(0, 0, w, h);
        glClearColor(0.5f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vd_fw_swap();
        vd_fw_unlock();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
