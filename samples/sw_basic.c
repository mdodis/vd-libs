#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#include "vd_fw.h"

#define WIDTH 400
#define HEIGHT 300
static unsigned int The_Pixels[WIDTH][HEIGHT] = {0};

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init(& (VdFwInitInfo) {
        .api = VD_FW_GRAPHICS_API_PIXEL_BUFFER,
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

        vd_fw_lock();

        int w, h;
        vd_fw_get_size(&w, &h);


        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                unsigned char red = (unsigned char)(x + 1);
                unsigned char green = (unsigned char)(y + 1);
                unsigned char blue = 0x00;
                unsigned char alpha = 0xFF;
                The_Pixels[y][x] = (red << 24) | (green << 16) | (blue << 8) | (alpha << 0);
            }
        }

        vd_fw_set_pixel_buffer((void*)The_Pixels, WIDTH, HEIGHT);
        vd_fw_unlock();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
