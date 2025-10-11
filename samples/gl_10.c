#include "vd_fw.h"

int main(int argc, char const *argv[]) {
    (void)argc;
    (void)argv;

    VdFwInitInfo init_info = {0};
    init_info.gl.debug_on = 1;
    init_info.gl.version = VD_FW_GL_VERSION_1_0;
    vd_fw_init(&init_info);

    while (vd_fw_running()) {
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        vd_fw_swap_buffers();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"