#include "vd_fw.h"
#include <math.h>

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init();
    float d = 0.f;

    while (vd_fw_running()) {

        vd_fw_begin_render();

        glClearColor(0.5f * (sinf(d) + 1.f) , 0.2f, 0.9f, 0.f);
        glClear(0x00004000);

        vd_fw_end_render();
        vd_fw_swap_buffers();

        d += 0.001f;
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
