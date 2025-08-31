#include "vd_fw.h"
#include <math.h>

void render(void)
{
    glClearColor(0.5f , 0.2f, 0.9f, 0.f);
    glClear(0x00004000);
}

int main(int argc, char const *argv[])
{
    vd_fw_init();

    while (vd_fw_running()) {
        vd_fw_poll_events();

        render();

        vd_fw_swap_buffers();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
