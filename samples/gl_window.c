#define VD_FW_NO_CRT 1
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#include "vd_fw.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .version = VD_FW_GL_VERSION_3_3,
        },
        .window_options = {
            .draw_default_borders = 1,
        }
    });
    vd_fw_set_vsync_on(1);

    while (vd_fw_running()) {
        vd_fw_begin_render();

        int w, h;
        vd_fw_get_size(&w, &h);

        glViewport(0, 0, w, h);
        glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vd_fw_end_render();
    }

    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
