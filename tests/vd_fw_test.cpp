#define VD_USE_CRT 1
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#define VD_INCLUDE_TESTS 1
#include "vd.h"
#include "vd_fw.h"

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "User32.lib")
#endif

#ifndef ENABLE_SLOW_TESTS
#   define ENABLE_SLOW_TESTS 1
#endif // !ENABLE_SLOW_TESTS

static void get_fw_window_rect(int *left, int *top, int *right, int *bottom);
static void move_cursor(int x, int y);
static void sleep_ms(int s);

VD_TEST("Basic/Init Quit Multiple") {

    for (int i = 0; i < 10; ++i) {
        vd_fw_init(0);

        while (vd_fw_running()) {
            vd_fw_poll(0);

            vd_fw_quit();
            vd_fw_lock();
            vd_fw_unlock();
        }

        vd_fw_exit();
    }

    VD_TEST_OK();
}

#if ENABLE_SLOW_TESTS
VD_TEST("Basic/Borderless Window Mouse locations") {
    VdFwInitInfo init_info = {0};
    init_info.window_options.borderless = 1;
    vd_fw_init(&init_info);

    vd_fw_set_size(640, 480);

    struct {
        int x, y;
    } move_positions[4] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1},
    };

    int i = 0;
    int moved = 0;

    while (vd_fw_running()) {
        sleep_ms(1000);
        vd_fw_poll(0);

        if (i < 4) {
            int left, top, right, bottom;
            get_fw_window_rect(&left, &top, &right, &bottom);

            if (!moved) {
                int x = left + move_positions[i].x * (right - left - 1);
                int y = top  + move_positions[i].y * (bottom - top - 1);

                // VD_TEST_LOG("Moving cursor to %d %d", x, y);
                move_cursor(x, y);
                sleep_ms(1000);

                moved = 1;
            } else {
                int x, y;
                vd_fw_get_mouse_state(&x, &y);

                int e_x = move_positions[i].x * 640;
                if (e_x != 0) {
                    e_x -= 1;
                }

                int e_y = move_positions[i].y * 480;
                if (e_y != 0) {
                    e_y -= 1;
                }

#undef VD_TEST_ERR_CODE
#define VD_TEST_ERR_CODE {vd_fw_exit();}
                // VD_TEST_LOG("Checking %d %d against %d %d", x, y, e_x, e_y);
                VD_TEST_EQ("X Eq to Corner X", x, e_x);
                VD_TEST_EQ("Y Eq to Corner Y", y, e_y);
#undef VD_TEST_ERR_CODE
#define VD_TEST_ERR_CODE

                i++;
                moved = 0;
            }
        } else {
            vd_fw_quit();
        }

        vd_fw_lock();
        vd_fw_unlock();
    }

    vd_fw_exit();

    VD_TEST_OK();
}

#endif // !ENABLE_SLOW_TESTS

VD_TEST("GL/Multiple Configs") {
    VdFwGlExtension bogus_extensions[] = {
        { "GL_DRM_threater" },
        0
    };

    VdFwGlConfig bogus_config1 = {0};
    bogus_config1.version = VD_FW_GL_VERSION_3_3;
    bogus_config1.req_extensions = bogus_extensions;

    VdFwGlConfig bogus_config2 = {0};
    bogus_config2.version = VD_FW_GL_VERSION_1_5;
    bogus_config2.req_extensions = bogus_extensions;

    VdFwGlConfig actual_config = {0};
    actual_config.version = VD_FW_GL_VERSION_3_3;

    VdFwGlConfig configs[] = {
        bogus_config1,
        bogus_config2,
        actual_config,
        {0}
    };

    VdFwInitInfo info = {0};
    info.gl.configs = configs;
    vd_fw_init(&info);

    VD_TEST_EQ("1st, 2nd configs are bogus; must select 3rd config", info.gl.selected_config, 2);

    vd_fw_exit();

    VD_TEST_OK();
}

#ifdef _WIN32
#endif // _WIN32

int main(int argc, char const *argv[])
{
    VD_UNUSED(argc);
    VD_UNUSED(argv);
    vd_test_main();
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"

#define VD_IMPL
#include "vd.h"

#ifdef _WIN32
static void get_fw_window_rect(int *left, int *top, int *right, int *bottom)
{
    RECT r;
    GetWindowRect((HWND)VD_FW_G.hwnd, &r);

    *left = r.left;
    *top = r.top;
    *right = r.right;
    *bottom = r.bottom;
}

static void move_cursor(int x, int y)
{
    SetCursorPos(x, y);
}

static void sleep_ms(int s)
{
    Sleep(s);
}
#endif
