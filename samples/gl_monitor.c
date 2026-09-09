#define VD_STUFF_GRAPHICS_INCLUDE 1
#define VD_STUFF_GRAPHICS_VD_UI_INCLUDE 1
#include "vd_fw.h"
#include "vd_ui.h"
#include "vd_stuff.h"

static VdFwPlacement Last_Placement = {0};

int main(int argc, char const *argv[])
{
    vd_stuff_init();

    while (vd_stuff_poll()) {
        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        vd_fw_lock();
        int w, h;
        vd_fw_get_size(&w, &h);
        vd_ui_frame_begin(vd_fw_delta_s());
        vd_stuff_ui_push_events();
        {
            VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 1.f)
            VD_UI_WITH_STYLE_SIZE_PERCENT_OF_PARENT(VD_UI_AXISH, 1.f, 1.f)
            {
                vd_ui_parent_newf(0
                                  | VD_UI_FLAG_FLEX_VERTICAL
                                  , "##layout");
            }
            {
                if (vd_ui_buttonf("Save Placement##save-placement").clicked) {
                    Last_Placement = vd_fw_get_placement();
                }

                if (Last_Placement.platform == vd_fw_get_platform()) {
                    if (vd_ui_buttonf("Load Placement##load-placement").clicked) {
                        vd_fw_set_placement(&Last_Placement);
                    }
                }


                int monitor_index = vd_fw_get_window_monitor();
                for (int i = 0; i < vd_fw_get_monitor_count(); ++i) {
                    const char *name = vd_fw_get_monitor_name(i);
                    VdUiReply btn_reply;
                    VdUiDiv *btn;

                    VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 1.f)
                    VD_UI_WITH_STYLE_SIZE_PERCENT_OF_PARENT(VD_UI_AXISH, 1.f, 1.f)
                    VD_UI_WITH_STYLE_BACKGROUND_COLORING(vd_ui_coloring(vd_ui_gradient1(vd_ui_f4(0.2f, 0.2f, 0.2f, 1.f)),
                                                                        vd_ui_gradient1(vd_ui_f4(0.3f, 0.3f, 0.3f, 1.f)),
                                                                        vd_ui_gradient1(vd_ui_f4(0.4f, 0.2f, 0.2f, 1.f))))
                    {
                        btn = vd_ui_parent_newf(0
                                                | VD_UI_FLAG_FLEX_VERTICAL
                                                | VD_UI_FLAG_BACKGROUND
                                                | VD_UI_FLAG_CLICKABLE
                                                , "##monitor-%d", i);

                        btn_reply = vd_ui_call(btn);
                    }
                    {
                        if (monitor_index == i) {
                            vd_ui_labelf("Monitor(%d) [CURRENT]##monitor", i);
                        } else {
                            vd_ui_labelf("Monitor(%d)##monitor", i);
                        }
                        vd_ui_labelf("Name = %s##name", name);
                    }
                    vd_ui_parent_pop();

                    if (btn_reply.clicked) {
                        if (monitor_index != i) {
                            vd_fw_move_to_monitor(i);
                        }
                    }
                }
            }
            vd_ui_parent_pop();
        }
        vd_ui_frame_end();
        vd_ui_render_begin();
        vd_ui_render_end();

        glViewport(0, 0, w, h);
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        vd_stuff_ui_render();

        vd_fw_swap();
        vd_fw_unlock();
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"

#define VD_UI_IMPL
#include "vd_ui.h"

#define VD_STUFF_IMPL
#include "vd_stuff.h"
