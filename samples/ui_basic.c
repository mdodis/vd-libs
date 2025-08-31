#define VD_FW_NO_CRT 1
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#include "vd_fw.h"

#include <stdio.h>

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init(0);

    vd_fw_set_vsync_on(1);
    float d = 0.f;

    while (vd_fw_running()) {

        unsigned long long ns = vd_fw_delta_ns();
        double milliseconds = (double)ns / 1000000.0;
        double delta_seconds64 = milliseconds / 1000.0;
        float delta_seconds = (float)delta_seconds64;

        // printf("NS: %zu\n", ns);
        // printf("MS: %3.3f\n", milliseconds);
        // printf(" S: %3.3f\n", delta_seconds);

        int w, h;
        vd_fw_get_size(&w, &h);

        d += delta_seconds * 2.f;

        if (d > 1.f) {
            d = 0.f;
        }
        glViewport(0, 0, w, h);
        glClearColor(d, 0.2f, 0.9f, 0.5f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glColor3f(1.f, 0.3f, 0.4f);
        glVertex2f(0.f, 0.f);
        glVertex2f(0.5f, 1.f);
        glVertex2f(1.f, 1.f);
        glEnd();

        vd_fw_swap_buffers();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
