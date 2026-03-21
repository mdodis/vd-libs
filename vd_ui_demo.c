#ifndef VD_UI_H
#error "You need to include vd_ui.h to use vd_ui_demo.c"
#endif // !VD_UI_H

static void vd_ui__demo_fixed_rect(const char *name, float width, float height, VdUiF4 color)
{
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_ABSOLUTE, width, 0.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_ABSOLUTE, height, 0.f);
    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, vd_ui_coloring_all4(color));
    vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, "##demo-rect-fixed-%s", name);
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);
}

static VdUiDiv *vd_ui__demo_dyn_rect_parent2(const char *name, int flags,
                                         VdUiSizeMode hsize, float hv, float hn,
                                         VdUiSizeMode vsize, float vv, float vn,
                                         VdUiF4 color)
{
    vd_ui_style_size_push(VD_UI_AXISH, hsize, hv, hn);
    vd_ui_style_size_push(VD_UI_AXISV, vsize, vv, vn);
    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, vd_ui_coloring_all4(color));
    VdUiDiv *div = vd_ui_parent_newf(flags | VD_UI_FLAG_BACKGROUND, "##demo-dyn-rect-custom-%s", name);
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);
    return div;
}

static VdUiDiv *vd_ui__demo_dyn_rect2(const char *name, int flags,
                                  VdUiSizeMode hsize, float hv, float hn,
                                  VdUiSizeMode vsize, float vv, float vn,
                                  VdUiF4 color)
{
    vd_ui_style_size_push(VD_UI_AXISH, hsize, hv, hn);
    vd_ui_style_size_push(VD_UI_AXISV, vsize, vv, vn);
    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, vd_ui_coloring_all4(color));
    VdUiDiv *div = vd_ui_div_newf(flags | VD_UI_FLAG_BACKGROUND, "##demo-dyn-rect-custom-%s", name);
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);
    return div;
}

static void vd_ui__demo_dyn_rect_parenth(const char *name, float height, VdUiF4 color, float niceness)
{
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, niceness);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_ABSOLUTE, height, 0.f);
    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, vd_ui_coloring_all4(color));
    vd_ui_parent_newf(VD_UI_FLAG_BACKGROUND | VD_UI_FLAG_FLEX_HORIZONTAL, "##demo-rect-dyn-%s", name);
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);
}

static void vd_ui__demo_dyn_rect_parentv(const char *name, float width, VdUiF4 color, float niceness)
{
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_ABSOLUTE, width, niceness);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, niceness);
    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, vd_ui_coloring_all4(color));
    vd_ui_parent_newf(VD_UI_FLAG_BACKGROUND, "##demo-rect-dyn-%s", name);
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);
}

static void vd_ui__demo_fixed_rect_parent(const char *name, int flags, float width, float height, VdUiF4 color)
{
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_ABSOLUTE, width, 0.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_ABSOLUTE, height, 0.f);
    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, vd_ui_coloring_all4(color));
    vd_ui_parent_newf(flags | VD_UI_FLAG_BACKGROUND, "##demo-rect-%s", name);
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);
}

static void vd_ui__demo_code(VdUiStr label, char *code)
{
    size_t len = vd_ui_strlen(code);
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_ABSOLUTE, 600, 0.f);
    vd_ui_textbox(label, code, &len, len);
    vd_ui_style_size_pop(VD_UI_AXISH);
}

static void vd_ui__demo_section_begin(const char *label)
{
    // vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, vd_ui_coloring_all4(vd_ui_f4(0.1f, 0.1f, 0.1f, 1.f)));
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_ABSOLUTE, 700, 0.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f);
    vd_ui_parent_newf(0, "##demo-section-%s", label);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);
    // vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);

    vd_ui_labelf("%s", label);
}

static void vd_ui__demo_section_end(void)
{
    vd_ui_parent_pop();
}

static int demo_menubar_active = 0;
static int demo_menubar_hovering = 0;

static int vd_ui__demo_point_in_rect(float x, float y, float rect[4])
{
    return (x >= rect[0]) && (x <= rect[2]) &&
           (y >= rect[1]) && (y <= rect[3]);
}

static int vd_ui__demo_menu_section_begin(const char *label)
{
    VdUiDiv *file_menu;
    int result;

    result = 0;
    VdUiReply file = vd_ui_buttonf("%s", label);

    int my_index;

    my_index = file.div->parent->child_count;

    if (file.clicked) {
        demo_menubar_active = my_index;
    }

    if (demo_menubar_active && file.hovering) {
        demo_menubar_active = my_index;
    }
    if (file.hovering) {
        demo_menubar_hovering = 1;
    }

    if (demo_menubar_active == my_index) {

        // if (vd_ui_mouse_left_clicked() && !file.focused) {
        //     demo_menubar_active = 0;
        // }

        vd_ui_parent_push(file.div);
        VD_UI_WITH_STYLE_SIZE_ABSOLUTE_W(100, 1)
        VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 1.f)
        VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.2f, 0.2f, 0.2f, 1.f))
        {
            file_menu = vd_ui_parent_newf(0
                                          | VD_UI_FLAG_FLOAT
                                          | VD_UI_FLAG_BACKGROUND
                                          , "##menu-section");
            file_menu->comp_pos_rel[0] = 0;
            file_menu->comp_pos_rel[1] = 16.f;

            if (vd_ui__demo_point_in_rect(file.mouse[0], file.mouse[1], file_menu->rect)) {
                demo_menubar_hovering = 1;
            }

            vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f);
        }


        result = 1;
    }
    return result;
}

static void vd_ui__demo_menu_section_end(void)
{
    vd_ui_style_size_pop(VD_UI_AXISH);
    vd_ui_parent_pop();
    vd_ui_parent_pop();
}

int vd_ui_demo(void)
{
    int result = 0;
    VdUiDiv *app;
    static int show_menubar = 0;

    VD_UI_WITH_STYLE_SIZE(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f)
    VD_UI_WITH_STYLE_SIZE(VD_UI_AXISV, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f)
    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.13f, 0.15f, 0.16f, 1.f))
    {
        app = vd_ui_div_new(VD_UI_FLAG_BACKGROUND, VD_UI_LIT("##app"));
    }

    vd_ui_parent_push(app);
    {
        // Menubar
        if (show_menubar) {
            static uint32_t main_menubar = 0;
            #if 1

            VD_UI_WITH_STYLE_SIZE(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f)
            VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 0.f)
            VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.1f, 0.1f, 0.1f, 1.f))
            {
                vd_ui_menu_group_begin(&main_menubar, VD_UI_LIT("##main-menubar"), VD_UI_FLAG_BACKGROUND | VD_UI_FLAG_FLEX_HORIZONTAL);
            }
            {
                if (vd_ui_menu_item(&main_menubar, VD_UI_LIT("File"))) {

                    VD_UI_WITH_STYLE_SIZE_ABSOLUTE_W(100, 1)
                    VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 1.f)
                    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.2f, 0.2f, 0.2f, 1.f))
                    {
                        vd_ui_menu_item_begin(&main_menubar);
                    }

                    VdUiColoring coloring = vd_ui_coloring(vd_ui_gradient1(vd_ui_f4(0.2f, 0.2f, 0.2f, 1.f)),
                                                           vd_ui_gradient1(vd_ui_f4(0.3f, 0.2f, 0.2f, 1.f)),
                                                           vd_ui_gradient1(vd_ui_f4(0.2f, 0.4f, 0.2f, 1.f)));

                    VD_UI_WITH_STYLE_BACKGROUND_COLORING(coloring)
                    VD_UI_WITH_STYLE_SIZE(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f)
                    {
                        if (vd_ui_buttonf("New").clicked) {
                            printf("New File\n");
                            vd_ui_menu_group_close(&main_menubar);
                        }

                        if (vd_ui_buttonf("Open").clicked) {
                            printf("New File\n");
                            vd_ui_menu_group_close(&main_menubar);
                        }

                        if (vd_ui_buttonf("Quit").clicked) {
                            printf("New File\n");
                            vd_ui_menu_group_close(&main_menubar);
                        }
                    }


                    vd_ui_menu_item_end();
                }

                if (vd_ui_menu_item(&main_menubar, VD_UI_LIT("Options"))) {

                    VD_UI_WITH_STYLE_SIZE_ABSOLUTE_W(100, 1)
                    VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 1.f)
                    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.2f, 0.2f, 0.2f, 1.f))
                    {
                        vd_ui_menu_item_begin(&main_menubar);
                    }

                    if (vd_ui_buttonf("Choreo").clicked) {
                        printf("Choreo\n");
                        vd_ui_menu_group_close(&main_menubar);
                    }

                    if (vd_ui_buttonf("Moreo").clicked) {
                        printf("Moreo\n");
                        vd_ui_menu_group_close(&main_menubar);
                    }

                    vd_ui_menu_item_end();
                }
            }
            vd_ui_menu_group_end(&main_menubar);

            #else
            VdUiDiv *menu;

            int prev_active = demo_menubar_active;
            demo_menubar_hovering = 0;

            VD_UI_WITH_STYLE_SIZE(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f)
            VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 0.f)
            VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.1f, 0.1f, 0.1f, 1.f))
            {
                menu = vd_ui_parent_new(0
                                        | VD_UI_FLAG_BACKGROUND
                                        // | VD_UI_FLAG_CLICKABLE
                                        | VD_UI_FLAG_FLEX_HORIZONTAL
                                        , VD_UI_LIT("##menu"));
            }
            {
                if (vd_ui__demo_menu_section_begin("File")) {
                    VdUiColoring coloring = vd_ui_coloring(vd_ui_gradient1(vd_ui_f4(0.2f, 0.2f, 0.2f, 1.f)),
                                                           vd_ui_gradient1(vd_ui_f4(0.3f, 0.2f, 0.2f, 1.f)),
                                                           vd_ui_gradient1(vd_ui_f4(0.2f, 0.4f, 0.2f, 1.f)));

                    VD_UI_WITH_STYLE_BACKGROUND_COLORING(coloring)
                    {
                        if (vd_ui_buttonf("New").clicked) {
                            demo_menubar_active = 0;
                            printf("NEW\n");
                        }
                        vd_ui_buttonf("Open");
                        vd_ui_buttonf("Quit");
                    }

                    vd_ui__demo_menu_section_end();
                }

                if (vd_ui__demo_menu_section_begin("Window")) {
                    vd_ui_buttonf("Debug");
                    vd_ui_buttonf("Choreo");
                    vd_ui__demo_menu_section_end();
                }

                if (vd_ui__demo_menu_section_begin("Help")) {
                    vd_ui_buttonf("Debug");
                    vd_ui_buttonf("Choreo");
                    vd_ui__demo_menu_section_end();
                }

            }
            vd_ui_parent_pop();

            if (prev_active && !demo_menubar_hovering && vd_ui_mouse_left_just_released()) {
                demo_menubar_active = 0;
            }
            #endif
        }

        static float scrollx = 0.f;
        static float scrolly = 0.f;

        VD_UI_WITH_STYLE_SIZE_PERCENT_OF_PARENT(VD_UI_AXISH, 1, 1)
        VD_UI_WITH_STYLE_SIZE_PERCENT_OF_PARENT(VD_UI_AXISV, 1, 1)
        {
            vd_ui_scrollview_begin(VD_UI_LIT("##main-scroll-view"), 0, &scrolly);
        }
        {

            VD_UI_WITH_STYLE_SIZE(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f)
            VD_UI_WITH_STYLE_SIZE(VD_UI_AXISV, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f)
            VD_UI_WITH_STYLE_CHILD_GAP(32.f)
            {
                vd_ui_parent_new(VD_UI_FLAG_ALIGN_CENTER, VD_UI_LIT("##layout-examples"));
            }

            // vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f);
            // vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f);
            // vd_ui_style_size_pop(VD_UI_AXISV);
            // vd_ui_style_size_pop(VD_UI_AXISH);

            VD_UI_WITH_STYLE_TEXT_COLOR(vd_ui_f4(0.93f, 0.35f, 0.34f, 1.f))
            VD_UI_WITH_STYLE_FONT_SIZE(64.f)
            {
                vd_ui_labelf("VD UI");
            }

            #if 1
            {
                vd_ui__demo_section_begin("Basics - Horizontal Layout");
                vd_ui__demo_code(VD_UI_LIT("code"), "VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(400, 200, 0, 0)\nVD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.05f, 0.05f, 0.05f, 1.f))\n{\n    vd_ui_parent_newf(0\n                      | VD_UI_FLAG_FLEX_HORIZONTAL\n                      | (layout_1_align_center ? VD_UI_FLAG_ALIGN_CENTER : 0) \n                      | VD_UI_FLAG_BACKGROUND\n                      , \"##rect-container-0\");\n}\n{\n\n    VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(40, 40, 0, 0)\n    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.5f, 0.3f, 0.2f, 1.f))\n    {\n        vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, \"##rect-0\");\n    }\n\n    VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(80, 40, 0, 0)\n    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.4f, 0.7f, 0.2f, 1.f))\n    {\n        vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, \"##rect-1\");\n    }\n\n    VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(40, 90, 0, 0)\n    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.2f, 0.3f, 0.5f, 1.f))\n    {\n        vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, \"##rect-2\");\n    }\n}\nvd_ui_parent_pop();\n");

                static int layout_1_align_center = 0;
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISH, 1.f)
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 1.f)
                {
                    vd_ui_parent_newf(VD_UI_FLAG_FLEX_HORIZONTAL, "columns");
                }
                {
                    VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(400, 200, 0, 0)
                    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.05f, 0.05f, 0.05f, 1.f))
                    {
                        vd_ui_parent_newf(0
                                          | VD_UI_FLAG_FLEX_HORIZONTAL
                                          | (layout_1_align_center ? VD_UI_FLAG_ALIGN_CENTER : 0) 
                                          | VD_UI_FLAG_BACKGROUND
                                          , "##rect-container-0");
                    }
                    {

                        VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(40, 40, 0, 0)
                        VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.5f, 0.3f, 0.2f, 1.f))
                        {
                            vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, "##rect-0");
                        }

                        VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(80, 40, 0, 0)
                        VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.4f, 0.7f, 0.2f, 1.f))
                        {
                            vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, "##rect-1");
                        }

                        VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(40, 90, 0, 0)
                        VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.2f, 0.3f, 0.5f, 1.f))
                        {
                            vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, "##rect-2");
                        }
                    }
                    vd_ui_parent_pop();
                }
                vd_ui_checkboxf(&layout_1_align_center, "Center##align-center-1");
                vd_ui_parent_pop();
                vd_ui__demo_section_end();

                vd_ui__demo_section_begin("Basics - Vertical Layout");
                vd_ui__demo_code(VD_UI_LIT("code"), "VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(400, 200, 0, 0)\nVD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.05f, 0.05f, 0.05f, 1.f))\n{\n    vd_ui_parent_newf(0\n                      | VD_UI_FLAG_FLEX_VERTICAL\n                      | (layout_2_align_center ? VD_UI_FLAG_ALIGN_CENTER : 0) \n                      | VD_UI_FLAG_BACKGROUND\n                      , \"##rect-container-0\");\n}\n{\n    VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(40, 40, 0, 0)\n    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.5f, 0.3f, 0.2f, 1.f))\n    {\n        vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, \"##rect-0\");\n    }\n\n    VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(80, 40, 0, 0)\n    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.4f, 0.7f, 0.2f, 1.f))\n    {\n        vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, \"##rect-1\");\n    }\n\n    VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(40, 90, 0, 0)\n    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.2f, 0.3f, 0.5f, 1.f))\n    {\n        vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, \"##rect-2\");\n    }\n}\nvd_ui_parent_pop();\n");

                static int layout_2_align_center = 0;
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISH, 1.f)
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 1.f)
                {
                    vd_ui_parent_newf(VD_UI_FLAG_FLEX_HORIZONTAL, "columns");
                }
                {
                    VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(400, 200, 0, 0)
                    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.05f, 0.05f, 0.05f, 1.f))
                    {
                        vd_ui_parent_newf(0
                                          | VD_UI_FLAG_FLEX_VERTICAL
                                          | (layout_2_align_center ? VD_UI_FLAG_ALIGN_CENTER : 0) 
                                          | VD_UI_FLAG_BACKGROUND
                                          , "##rect-container-0");
                    }
                    {
                        VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(40, 40, 0, 0)
                        VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.5f, 0.3f, 0.2f, 1.f))
                        {
                            vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, "##rect-0");
                        }

                        VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(80, 40, 0, 0)
                        VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.4f, 0.7f, 0.2f, 1.f))
                        {
                            vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, "##rect-1");
                        }

                        VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(40, 90, 0, 0)
                        VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.2f, 0.3f, 0.5f, 1.f))
                        {
                            vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, "##rect-2");
                        }
                    }
                    vd_ui_parent_pop();
                }
                vd_ui_checkboxf(&layout_2_align_center, "Center##align-center-2");
                vd_ui_parent_pop();
                vd_ui__demo_section_end();

                static float layout_3_child_3_width = 40.f;
                vd_ui__demo_section_begin("Basics - Contain Children");
                vd_ui__demo_code(VD_UI_LIT("code-3"),
                        "vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f);\n"
                        "vd_ui_div_newf(VD_UI_FLAG_FLEX_HORIZONTAL, \"layout-3\");\n"
                        "vd_ui_style_size_pop(VD_UI_AXISH);"
                        );
                vd_ui_sliderf_float(&layout_3_child_3_width, 40.f, 400.f, VD_UI_AXISH, "Child 3 Size");
                vd_ui__demo_fixed_rect_parent("layout-3", VD_UI_FLAG_FLEX_HORIZONTAL, 400, 200, vd_ui_f4(0.05f, 0.05f, 0.05f, 1.f));
                {
                    vd_ui__demo_fixed_rect("1", 40, 40, vd_ui_f4(0.5f, 0.3f, 0.2f, 1.f));
                    vd_ui__demo_dyn_rect_parenth("2", 100.f, vd_ui_f4(0.01f, 0.15f, 0.15f, 1.f), 0.f);
                    {
                        vd_ui__demo_fixed_rect("2", 40, 40, vd_ui_f4(0.4f, 0.7f, 0.2f, 1.f));
                        vd_ui__demo_fixed_rect("3", layout_3_child_3_width, 40, vd_ui_f4(0.2f, 0.3f, 0.5f, 1.f));
                    }
                    vd_ui_parent_pop();
                }
                vd_ui_parent_pop();

                vd_ui__demo_fixed_rect_parent("layout-3-vertical", 0, 200, 400, vd_ui_f4(0.05f, 0.05f, 0.05f, 1.f));
                {
                    vd_ui__demo_fixed_rect("1", 40, 40, vd_ui_f4(0.5f, 0.3f, 0.2f, 1.f));
                    vd_ui__demo_dyn_rect_parentv("2", 100.f, vd_ui_f4(0.01f, 0.15f, 0.15f, 1.f), 0.f);
                    {
                        vd_ui__demo_fixed_rect("2", 40, 40, vd_ui_f4(0.4f, 0.7f, 0.2f, 1.f));
                        vd_ui__demo_fixed_rect("3", 40, layout_3_child_3_width, vd_ui_f4(0.2f, 0.3f, 0.5f, 1.f));
                    }
                    vd_ui_parent_pop();
                }
                vd_ui_parent_pop();
                vd_ui__demo_section_end();

                static float layout_4_child_3_width = 40.f;
                vd_ui__demo_section_begin("Basics - Percentage of Parent");
                vd_ui__demo_code(VD_UI_LIT("code-4"),
                        "vd_ui_spacer(VD_UI_AXISH);"
                        );
                vd_ui_sliderf_float(&layout_4_child_3_width, 40.f, 520.f, VD_UI_AXISH, "Child 4 Size");
                vd_ui__demo_fixed_rect_parent("layout-4", VD_UI_FLAG_FLEX_HORIZONTAL, 400, 200, vd_ui_f4(0.05f, 0.05f, 0.05f, 1.f));
                {
                    vd_ui__demo_fixed_rect("x", 40, 40, vd_ui_f4(0.5f, 0.3f, 0.2f, 1.f));
                    vd_ui__demo_dyn_rect_parenth("y", 100.f, vd_ui_f4(0.01f, 0.15f, 0.15f, 1.f), 1.f);
                    {
                        vd_ui__demo_fixed_rect("1", 40, 40, vd_ui_f4(0.4f, 0.7f, 0.2f, 1.f));
                        vd_ui_spacer(VD_UI_AXISH);
                        vd_ui__demo_fixed_rect("2", layout_4_child_3_width, 40, vd_ui_f4(0.2f, 0.3f, 0.5f, 1.f));
                    }
                    vd_ui_parent_pop();
                }
                vd_ui_parent_pop();
                vd_ui__demo_section_end();

                vd_ui__demo_section_begin("Nesting - 1");
                vd_ui__demo_fixed_rect_parent("layout-5", 0, 400, 200, vd_ui_f4(0.05f, 0.05f, 0.05f, 1.f));
                {
                    VdUiDiv *pa = 0;
                    VdUiDiv *pb = 0;
                    VdUiDiv *pc = 0;

                    pa = vd_ui__demo_dyn_rect_parent2("dynamic-a", VD_UI_FLAG_FLEX_HORIZONTAL,
                                                      VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                      VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                      vd_ui_f4(0.01f, 0.15f, 0.15f, 1.f));
                    {

                        pb = vd_ui__demo_dyn_rect_parent2("dynamic-b", 0,
                                                          VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f,
                                                          VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                          vd_ui_f4(0.01f, 0.15f, 0.15f, 1.f));
                        {
                            vd_ui__demo_dyn_rect2("x", 0,
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 100.f, 0.f,
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 64.f, 0.f,
                                                       vd_ui_f4(0.5f, 0.3f, 0.2f, 1.f));

                            vd_ui__demo_dyn_rect2("y", 0, 
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 80.f, 0.f,
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 64.f, 0.f,
                                                       vd_ui_f4(0.4f, 0.7f, 0.2f, 1.f));
                        }
                        vd_ui_parent_pop();

                        pc = vd_ui__demo_dyn_rect_parent2("dynamic-c", 0,
                                                          VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                          VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                          vd_ui_f4(0.15f, 0.01f, 0.15f, 1.f));
                        {
                            vd_ui__demo_dyn_rect2("x", 0,
                                                       VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 64.f, 0.f,
                                                       vd_ui_f4(0.7f, 0.3f, 0.2f, 1.f));

                            vd_ui__demo_dyn_rect2("y", 0, 
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 80.f, 0.f,
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 64.f, 0.f,
                                                       vd_ui_f4(0.3f, 0.2f, 0.5f, 1.f));
                        }
                        vd_ui_parent_pop();
                    }
                    vd_ui_parent_pop();
                }
                vd_ui_parent_pop();
                vd_ui__demo_section_end();

                vd_ui__demo_section_begin("Nesting - 2");
                float layout_6_length[2];
                vd_ui__demo_fixed_rect_parent("layout-6", 0, 400, 200, vd_ui_f4(0.05f, 0.05f, 0.05f, 1.f));
                {
                    VdUiDiv *pa = 0;
                    VdUiDiv *pb = 0;
                    VdUiDiv *pc = 0;

                    pa = vd_ui__demo_dyn_rect_parent2("dynamic-a", 0,
                                                      VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                      VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                      vd_ui_f4(0.01f, 0.15f, 0.15f, 1.f));
                    {

                        pb = vd_ui__demo_dyn_rect_parent2("dynamic-b", VD_UI_FLAG_FLEX_HORIZONTAL,
                                                          VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                          VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f,
                                                          vd_ui_f4(0.01f, 0.15f, 0.15f, 1.f));
                        {
                            vd_ui__demo_dyn_rect2("x", 0,
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 64.f, 1.f,
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 100.f, 0.f,
                                                       vd_ui_f4(0.5f, 0.3f, 0.2f, 1.f));

                            pa = vd_ui__demo_dyn_rect2("y", 0, 
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 64.f, 1.f,
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 80.f, 0.f,
                                                       vd_ui_f4(0.4f, 0.7f, 0.2f, 1.f));
                        }
                        vd_ui_parent_pop();

                        pc = vd_ui__demo_dyn_rect_parent2("dynamic-c", VD_UI_FLAG_FLEX_HORIZONTAL,
                                                          VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                          VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                          vd_ui_f4(0.15f, 0.01f, 0.15f, 1.f));
                        {
                            vd_ui__demo_dyn_rect2("x", 0,
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 80.f, 1.f,
                                                       VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                       vd_ui_f4(0.7f, 0.3f, 0.2f, 1.f));

                            vd_ui__demo_dyn_rect2("y", 0, 
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 64.f, 1.f,
                                                       VD_UI_SIZE_MODE_ABSOLUTE, 80.f, 0.f,
                                                       vd_ui_f4(0.3f, 0.2f, 0.5f, 1.f));
                        }
                        vd_ui_parent_pop();
                    }
                    vd_ui_parent_pop();

                    layout_6_length[0] = pa->comp_size[0];
                    layout_6_length[1] = pa->comp_size[1];
                }
                vd_ui_parent_pop();
                VD_UI_WITH_STYLE_TEXT_COLOR(vd_ui_f4(0.8f, 0.8f, 0.8f, 1.f))
                {
                    vd_ui_labelf("Green Rect Size: %f %f", layout_6_length[0], layout_6_length[1]);
                }
                vd_ui__demo_section_end();

                VdUiGradient hue_grad = vd_ui_gradient(
                    vd_ui_f4(1, 0, 0, 1), vd_ui_f4(1, 1, 0, 1),
                    vd_ui_f4(0, 1, 1, 1), vd_ui_f4(1, 0, 1, 1));
                vd_ui__demo_section_begin("Color Picker");
                VD_UI_WITH_STYLE_SIZE_ABSOLUTE(VD_UI_AXISH, 32.f, 1.f)
                VD_UI_WITH_STYLE_SIZE_ABSOLUTE(VD_UI_AXISV, 128.f, 1.f)
                VD_UI_WITH_STYLE_BACKGROUND_COLORING(vd_ui_coloring(hue_grad, hue_grad, hue_grad))
                {
                    vd_ui_div_newf(VD_UI_FLAG_BACKGROUND, "rect");
                }
                vd_ui__demo_section_end();
            }
            #endif

            vd_ui_parent_pop();
        }
        vd_ui_scrollview_end();

        // Floating part
        VD_UI_WITH_STYLE_SIZE_PERCENT_OF_PARENT(VD_UI_AXISH, 1.f, 1.f)
        VD_UI_WITH_STYLE_SIZE_PERCENT_OF_PARENT(VD_UI_AXISV, 1.f, 1.f)
        {
            vd_ui_parent_newf(0
                              | VD_UI_FLAG_FLOAT
                              | VD_UI_FLAG_FLEX_VERTICAL
                              , "overlay");
        }
        {
            vd_ui_spacer(VD_UI_AXISV);

            VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 1.f)
            VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISH, 1.f)
            {
                vd_ui_parent_newf(0
                                  | VD_UI_FLAG_FLEX_VERTICAL
                                  , "vert-container");
            }
            {
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 1.f)
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISH, 1.f)
                VD_UI_WITH_STYLE_PADDING4(0, 0, 48, 16)
                {
                    vd_ui_parent_newf(0
                                      | VD_UI_FLAG_FLEX_HORIZONTAL
                                      , "horiz-container");
                }
                {
                    vd_ui_spacer(VD_UI_AXISH);

                    VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISH, 0.f)
                    VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 0.f)
                    VD_UI_WITH_STYLE_PADDING4(8, 8, 8, 8)
                    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.1f, 0.1f, 0.1f, 1.f))
                    {
                        vd_ui_parent_newf(0 
                                          | VD_UI_FLAG_BACKGROUND 
                                          | VD_UI_FLAG_FLEX_VERTICAL
                                          , "actual-container");
                    }
                    {
                        vd_ui_labelf("Controls");
                        if (vd_ui_buttonf("Toggle Inspector").clicked) {
                            vd_ui_debug_set_inspector_on(!vd_ui_debug_get_inspector_on());
                        }


                        vd_ui_checkbox(&show_menubar, VD_UI_LIT("Show Menu Bar"));
                    }
                    vd_ui_parent_pop();
                }
                vd_ui_parent_pop();
            }
            vd_ui_parent_pop();
        }
        vd_ui_parent_pop();
    }
    vd_ui_parent_pop();

    return result;
}
