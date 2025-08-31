#include "vd_fw.h"
#include <math.h>

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init();
    float d = 0.f;

    while (vd_fw_running()) {

        // vd_fw_begin_render();

        int w, h;
        vd_fw_get_size(&w, &h);

        glViewport(0, 0, w, h);
        glClearColor(0.5f * (sinf(d) + 1.f) , 0.2f, 0.9f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.3f, 0.4f);
        glVertex2f(0.f, 0.f);
        glVertex2f(0.5f, 1.f);
        glVertex2f(1.f, 1.f);
        glEnd();

        // vd_fw_end_render();
        vd_fw_swap_buffers();

        d += 0.001f;
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
