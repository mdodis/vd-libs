#define VD_USE_CRT 1
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#define VD_INCLUDE_TESTS 1
#include "vd.h"
#include "vd_fw.h"


#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "User32.lib")
#endif

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

VD_TEST("GL/Multiple Configs") {
    VdFwGlConfig configs[] = {
        {
            .version = VD_FW_GL_VERSION_3_3,
            .req_extensions = (VdFwGlExtension []) {
                { "GL_DRM_threater"},
                {0}
            }
        },
        {
            .version = VD_FW_GL_VERSION_1_5,
            .req_extensions = (VdFwGlExtension []) {
                { "GL_DRM_threater"},
                {0}
            }
        },
        {
            .version = VD_FW_GL_VERSION_3_3,
        },
        0
    };

    VdFwInitInfo info = {0};
    info.gl.configs = configs;
    vd_fw_init(&info);

    VD_TEST_EQ("1st, 2nd configs are bogus; must select 3rd config", info.gl.selected_config, 2);

    vd_fw_exit();

    VD_TEST_OK();
}


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