#include "vd_fw.h"
#include "vd_um.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .version = VD_FW_GL_VERSION_4_1,
            .debug_on = 1,
        }
    });
    vd_fw_set_title("UM Basic");

    while (vd_fw_running()) {
        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        int w,h;
        vd_fw_get_size(&w, &h);
        glViewport(0,0,w,h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        vd_fw_swap_buffers();
    }

    return 0;
}

#define VD_UM_IMPL
#include "vd_um.h"

#define VD_FW_IMPL
#include "vd_fw.h"
