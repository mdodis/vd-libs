#include "vd_fw.h"                                           // Include library
int main() {
    vd_fw_init(NULL);                                        // Initialize library

    while (vd_fw_running()) {                                // Check if the window is running & gather events
        glClearColor(0.5f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vd_fw_swap_buffers();                                // Swap buffers
    }

    return 0;
}

#define VD_FW_IMPL                                           // Include implementation code
#include "vd_fw.h"
