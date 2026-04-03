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
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_ABSOLUTE, 600 * vd_ui_get_scale(), 0.f);
    vd_ui_textbox(label, code, &len, len, 0);
    vd_ui_style_size_pop(VD_UI_AXISH);
}

static void vd_ui__demo_section_begin(const char *label)
{
    // vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, vd_ui_coloring_all4(vd_ui_f4(0.1f, 0.1f, 0.1f, 1.f)));
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_ABSOLUTE, 700 * vd_ui_get_scale(), 0.f);
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
        app = vd_ui_div_new(VD_UI_FLAG_BACKGROUND | VD_UI_FLAG_CLICKABLE, VD_UI_LIT("##app"));
    }

    vd_ui_parent_push(app);
    {
        // Menubar
        if (show_menubar) {
            #if 0

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
            int prev_active = demo_menubar_active;
            demo_menubar_hovering = 0;

            VD_UI_WITH_STYLE_SIZE(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f)
            VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 0.f)
            VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.1f, 0.1f, 0.1f, 1.f))
            {
                vd_ui_parent_new(0
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

        VD_UI_WITH_STYLE_SIZE_PERCENT_OF_PARENT(VD_UI_AXISH, 1, 1)
        VD_UI_WITH_STYLE_SIZE_PERCENT_OF_PARENT(VD_UI_AXISV, 1, 1)
        {
            vd_ui_scrollview_begin(VD_UI_LIT("##main-scroll-view"), 0, 1);
        }
        {

            VD_UI_WITH_STYLE_SIZE(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f)
            VD_UI_WITH_STYLE_SIZE(VD_UI_AXISV, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f)
            VD_UI_WITH_STYLE_CHILD_GAP(32.f)
            VD_UI_WITH_STYLE_CHILD_ALIGNMENT(VD_UI_ALIGNMENT_CENTER)
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

                static VdUiAlignment layout_1_alignment = VD_UI_ALIGNMENT_START;
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISH, 1.f)
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 1.f)
                {
                    vd_ui_parent_newf(VD_UI_FLAG_FLEX_HORIZONTAL, "columns");
                }
                {
                    VD_UI_WITH_STYLE_SIZE_ABSOLUTE_WH(400, 200, 0, 0)
                    VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.05f, 0.05f, 0.05f, 1.f))
                    VD_UI_WITH_STYLE_CHILD_ALIGNMENT(layout_1_alignment)
                    {
                        vd_ui_parent_newf(0
                                          | VD_UI_FLAG_FLEX_HORIZONTAL
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

                VD_UI_WITH_STYLE_CHILD_GAP(8)
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 0)
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISH, 0)
                {
                    vd_ui_parent_new(0
                                     , VD_UI_LIT("##layout-1-grp"));
                }
                {
                    vd_ui_radiof(&layout_1_alignment, VD_UI_ALIGNMENT_START, "start");
                    vd_ui_radiof(&layout_1_alignment, VD_UI_ALIGNMENT_CENTER, "center");
                    vd_ui_radiof(&layout_1_alignment, VD_UI_ALIGNMENT_END, "end");
                    vd_ui_radiof(&layout_1_alignment, VD_UI_ALIGNMENT_FILL, "fill");
                }
                vd_ui_parent_pop();

                // vd_ui_checkboxf(&layout_1_align_center, "Center##align-center-1");
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
                    vd_ui__demo_dyn_rect_parent2("dynamic-a", VD_UI_FLAG_FLEX_HORIZONTAL,
                                                 VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                 VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                 vd_ui_f4(0.01f, 0.15f, 0.15f, 1.f));
                    {

                        vd_ui__demo_dyn_rect_parent2("dynamic-b", 0,
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

                        vd_ui__demo_dyn_rect_parent2("dynamic-c", 0,
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
                    VdUiDiv *pa;
                    vd_ui__demo_dyn_rect_parent2("dynamic-a", 0,
                                                 VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                 VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f,
                                                 vd_ui_f4(0.01f, 0.15f, 0.15f, 1.f));
                    {

                        vd_ui__demo_dyn_rect_parent2("dynamic-b", VD_UI_FLAG_FLEX_HORIZONTAL,
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

                        vd_ui__demo_dyn_rect_parent2("dynamic-c", VD_UI_FLAG_FLEX_HORIZONTAL,
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

                vd_ui__demo_section_begin("Combos");
                {
                    const char *options[] = {
                        "AAAA",
                        "BBBB",
                        "CCCC",
                        "DDDD",
                        "EEEE",
                        "FFFF",
                        "GGGG",
                        "HHHH",
                        "IIII",
                        "JJJJ",
                        "KKKK",
                    };
                    int count_options = sizeof(options) / sizeof(options[0]);
                    static int current_option = 0;

                    VdUiStr preview = vd_ui_str((char*)options[current_option], (int)strlen(options[current_option]));

                    VD_UI_WITH_STYLE_SIZE_PERCENT_OF_PARENT(VD_UI_AXISH, 1, 1)
                    VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 0)
                    {
                        if (vd_ui_combo_begin(0, preview, VD_UI_LIT("##combo-basic"))) {
                            VdUiColoring coloring = vd_ui_coloring(vd_ui_gradient1(vd_ui_f4(0.2f, 0.3f, 0.7f, 0.0f)),
                                                                   vd_ui_gradient1(vd_ui_f4(0.2f, 0.3f, 0.7f, 0.5f)),
                                                                   vd_ui_gradient1(vd_ui_f4(0.2f, 0.3f, 0.7f, 0.7f)));
                            for (int i = 0; i < count_options; ++i) {
                                VdUiStr str = vd_ui_str((char*)options[i], (int)strlen(options[i]));
                                int selected = 0;
                                VD_UI_WITH_STYLE_BACKGROUND_COLORING(coloring)
                                VD_UI_WITH_STYLE_SIZE(VD_UI_AXISH, VD_UI_SIZE_MODE_TEXT_CONTENT, 0, 0)
                                VD_UI_WITH_STYLE_SIZE(VD_UI_AXISV, VD_UI_SIZE_MODE_TEXT_CONTENT, 0, 0)
                                {
                                    selected = vd_ui_selectable(0
                                                                | VD_UI_FLAG_BACKGROUND
                                                                | VD_UI_FLAG_TEXT
                                                                , str);
                                }

                                if (selected) {
                                    current_option = i;
                                }

                            }
                            vd_ui_combo_end();
                        }
                    }
                }
                vd_ui__demo_section_end();

                vd_ui__demo_section_begin("Popups");
                VdUiReply button_w_popup = vd_ui_buttonf("Click for Popup");
                if (button_w_popup.clicked) {
                    vd_ui_popup_next_placement(VD_UI_POPUP_PLACEMENT_OVER_ANCHOR);
                    vd_ui_popup_next_anchor(button_w_popup.div);
                    vd_ui_popup_push(VD_UI_LIT("##popup-basic"));
                }

                VdUiReply button_w_popup2 = vd_ui_buttonf("Another Popup");
                if (button_w_popup2.clicked & VD_UI_SIG_MOUSE_RIGHT) {
                    vd_ui_menu_open(VD_UI_LIT("##popup-basic2"));
                }

                VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.05f, 0.05f, 0.05f, 0.87f))
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISH, 0)
                VD_UI_WITH_STYLE_SIZE_CONTAIN_CHILDREN(VD_UI_AXISV, 0)
                {
                    if (vd_ui_popup_begin(VD_UI_LIT("##popup-basic"), VD_UI_FLAG_BACKGROUND)) {
                        vd_ui_labelf("I am a popup!");

                        VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.75f, 0.05f, 0.05f, 1.f))
                        VD_UI_WITH_STYLE_SIZE(VD_UI_AXISH, VD_UI_SIZE_MODE_TEXT_CONTENT, 1, 1)
                        VD_UI_WITH_STYLE_SIZE(VD_UI_AXISV, VD_UI_SIZE_MODE_TEXT_CONTENT, 1, 1)
                        {
                            if (vd_ui_buttonf("Close").clicked) {
                                vd_ui_popup_pop();
                            }
                        }

                        vd_ui_popup_end();
                    }

                    if (vd_ui_menu_begin(VD_UI_LIT("##popup-basic2"))) {
                        VD_UI_WITH_STYLE_BACKGROUND_COLORING(vd_ui_coloring(vd_ui_gradient1(vd_ui_f4(0.2f,0.2f,0.2f,0.0f)),
                                                                            vd_ui_gradient1(vd_ui_f4(0.6f,0.6f,0.6f,0.4f)),
                                                                            vd_ui_gradient1(vd_ui_f4(0.2f,0.2f,0.2f,0.8f))))
                        {
                            if (vd_ui_menu_item(VD_UI_LIT("Operations"), VD_UI_LIT("##ops")))
                            {
                                vd_ui_menu_item(VD_UI_LIT("Copy"), vd_ui_str_null());
                                vd_ui_menu_item(VD_UI_LIT("Paste"), vd_ui_str_null());
                                vd_ui_menu_end();    
                            }

                            vd_ui_menu_item(VD_UI_LIT("Modulate"), vd_ui_str_null());
                            vd_ui_menu_item(VD_UI_LIT("Close"), vd_ui_str_null());
                        }
                        // vd_ui_labelf("I am another popup!");

                        // // VD_UI_WITH_STYLE_BACKGROUND_COLOR(vd_ui_f4(0.75f, 0.05f, 0.05f, 1.f))
                        // // VD_UI_WITH_STYLE_SIZE(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1, 1)
                        // // VD_UI_WITH_STYLE_SIZE(VD_UI_AXISV, VD_UI_SIZE_MODE_TEXT_CONTENT, 1, 1)
                        // {
                        //     VdUiReply ops = vd_ui_buttonf("Operations");
                        //     if (ops.hovered) {
                        //         vd_ui_popup_pop_at_current_level();
                        //         vd_ui_popup_next_anchor(ops.div);
                        //         vd_ui_popup_next_placement(VD_UI_POPUP_PLACEMENT_RIGHT_OF_ANCHOR);
                        //         vd_ui_popup_push(VD_UI_LIT("##popup-ops"));
                        //     }

                        //     if (vd_ui_popup_begin(VD_UI_LIT("##popup-ops"), VD_UI_FLAG_BACKGROUND)) {
                        //         VD_UI_WITH_STYLE_SIZE(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1, 1)
                        //         VD_UI_WITH_STYLE_SIZE(VD_UI_AXISV, VD_UI_SIZE_MODE_TEXT_CONTENT, 1, 1)
                        //         {
                        //             vd_ui_buttonf("Add");
                        //             vd_ui_buttonf("Subtract");
                        //             vd_ui_buttonf("Multiply");
                        //         }
                        //         vd_ui_popup_end();
                        //     }

                        //     if (vd_ui_buttonf("Close").clicked) {
                        //         vd_ui_popup_pop();
                        //     }
                        // }

                        vd_ui_menu_end();
                    }


                    vd_ui_labelf("Popups work like this");
                }
                vd_ui__demo_section_end();

                vd_ui__demo_section_begin("Text Boxes");
                static char buf[128] = "Text Box";
                static size_t len = 8;
                VD_UI_WITH_STYLE_SIZE_PERCENT_OF_PARENT(VD_UI_AXISH, 1, 1)
                VD_UI_WITH_STYLE_SIZE_ABSOLUTE(VD_UI_AXISV, 200, 0)
                {
                    VdUiTextBoxOptions options = {0};
                    options.single_line = 1;
                    vd_ui_textbox(VD_UI_LIT("##textbox"), buf, &len, sizeof(buf), &options);
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

    // vd_ui_call(app);

    return result;
}
