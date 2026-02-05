#include "vd_fw.h"

int main(int argc, char const *argv[]) {
    (void)argc;
    (void)argv;

    VdFwInitInfo init_info = {0};
    init_info.gl.debug_on = 1;
    init_info.gl.version = VD_FW_GL_VERSION_1_0;
    vd_fw_init(&init_info);

    while (vd_fw_running()) {
        vd_fw_poll();

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        vd_fw_lock();
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        vd_fw_unlock();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"