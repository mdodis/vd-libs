#define VD_FW_WIN32_PROFILE 1
#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#include "vd_fw.h"
#include <assert.h>

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    VdFwInitInfo init_info = {
        .gl = {
            .configs = (VdFwGlConfig[]) {
                {
                    .version = VD_FW_GL_VERSION_4_6,
                    .req_extensions = (VdFwGlExtension[]) {
                        {
                            "GL_DRM_theater",
                        },
                        0
                    }
                },
                {
                    .version = VD_FW_GL_VERSION_3_3,
                    .req_extensions = (VdFwGlExtension[]) {
                        {
                            "GL_OVR_multiview",
                        },
                        0
                    }
                },
                0
            }
        }
    };
    vd_fw_init(&init_info);

    assert(init_info.gl.selected_config == 1);
    vd_fw_set_vsync_on(0);


    while (vd_fw_running()) {

        vd_fw_poll(0);

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        vd_fw_lock();

        int w, h;
        vd_fw_get_size(&w, &h);

        glViewport(0, 0, w, h);
        glClearColor(0.5f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vd_fw_swap();
        vd_fw_unlock();
    }

    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
