#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif // defined(__clang__)

#define VD_USE_CRT 1
#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#include "vd_fw.h"
#include "vd.h"
#include "vd_ui.h"
#include "vd_fc.h"
#include "ext/stb_image.h"

#include <stdio.h>

#define GL_CHECK(expr) do { (expr); int _e_ = glGetError(); if (_e_ != 0) { printf("Check at " __FILE__ ":%d failed with 0x%x\n", __LINE__, _e_); assert(0); }} while(0)

typedef struct {
    int sidebar;
    int sidebar_overlaid;
    int curr_device_id;
} State;

static State The_State;

static VdUiColoring sidebar_bg(void) {
    return vd_ui_coloring_all4(vd_ui_f4(0.13f, 0.13f, 0.13f, 1.f));
}

static int sidebar_is_overlay(void) {
    return The_State.sidebar_overlaid;
}

static void sidebar_toggle(void) {
    if (The_State.sidebar_overlaid) {
        The_State.sidebar = !The_State.sidebar;
    }
}

static int sidebar_display(void) {
    return The_State.sidebar || !The_State.sidebar_overlaid;
}

static int do_menu()
{
    int result = 1;

    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 1.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f);

    vd_ui_style_padding_push(VD_UI_LEFT, 16.f);

    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, vd_ui_coloring_all4(vd_ui_f4(0.13f, 0.13f, 0.13f, 1.f)));
    VdUiDiv *menu = vd_ui_div_new(VD_UI_FLAG_FLEX_HORIZONTAL
                                  | VD_UI_FLAG_ALIGN_CENTER
                                  | VD_UI_FLAG_BACKGROUND, VD_UI_LIT("##menu"));
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);

    vd_ui_style_padding_pop(VD_UI_LEFT);

    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);

    VdUiColoring icon_coloring = vd_ui_coloring(vd_ui_gradient1(vd_ui_f4(0.2f, 0.2f, 0.2f, 0.f)),
                                                vd_ui_gradient1(vd_ui_f4(0.2f, 0.2f, 0.2f, 0.7f)),
                                                vd_ui_gradient1(vd_ui_f4(0.2f, 0.2f, 0.2f, 1.f)));

    vd_ui_parent_push(menu);
    {
        if (sidebar_is_overlay()) {
            VD_UI_WITH_STYLE_BACKGROUND_COLORING(icon_coloring)
            {
                if (vd_ui_icon_buttonf(vd_ui_symbol((VdUiFontId){1}, 62022), "##toggle-sidebar").clicked) {
                    sidebar_toggle();
                }
            }
        }
        vd_ui_labelf("Game Controllers##title");

        vd_ui_spacer(VD_UI_AXISH);

        VD_UI_WITH_STYLE_BACKGROUND_COLORING(icon_coloring)
        {
            if (vd_ui_icon_buttonf(vd_ui_symbol((VdUiFontId){1}, 59397), "##close").clicked) {
                result = 0;
            }
        }
    }
    vd_ui_parent_pop();

    vd_ui_ws_nc_area_mark(menu);
    return result;
}

static int device_field_button(int i, float comp_pos_rel[2])
{
    int is_current = The_State.curr_device_id == i;

    vd_ui_style_padding_push(VD_UI_LEFT, 16.f);
    vd_ui_style_padding_push(VD_UI_TOP, 8.f);
    vd_ui_style_padding_push(VD_UI_BOTTOM, 8.f);
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_ABSOLUTE, 300.f, 1.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_TEXT_CONTENT, 1.f, 0.f);
    VdUiF4 color_on  = vd_ui_f4(0.32f, 0.32f, 0.32f, 1.f);
    VdUiF4 color_off = vd_ui_f4(0.20f, 0.20f, 0.20f, 1.f);
    VdUiF4 normal_color = color_off;
    if (is_current) {
        normal_color = color_on;
    }
    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND,
                              vd_ui_coloring(vd_ui_gradient1(normal_color),
                                             vd_ui_gradient1(vd_ui_f4(0.52f, 0.52f, 0.52f, 1.f)),
                                             vd_ui_gradient1(vd_ui_f4(0.33f, 0.33f, 0.33f, 1.f))));
    VdUiDiv *btn;
    VD_UI_WITH_STYLE_ROUNDING_ALL(VD_UI_FLAG_BACKGROUND, 8.f)
    {
        btn = vd_ui_div_newf(VD_UI_FLAG_TEXT
                             | VD_UI_FLAG_BACKGROUND
                             | VD_UI_FLAG_CLICKABLE
                             , "%s##dev-%d", vd_fc_name(i), i);
    }
    btn->style.background.edge_softness = 0.002f;
    btn->style.background.border_thickness = 0.f;
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);

    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);
    vd_ui_style_padding_pop(VD_UI_BOTTOM);
    vd_ui_style_padding_pop(VD_UI_TOP);
    vd_ui_style_padding_pop(VD_UI_LEFT);

    comp_pos_rel[0] = btn->comp_pos_rel[0];
    comp_pos_rel[1] = btn->comp_pos_rel[1];

    VdUiReply reply = vd_ui_call(btn);
    return reply.clicked;
}

static void vspacefixed(float f)
{
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_ABSOLUTE, 0.f, 1.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_ABSOLUTE, f, 1.f);
    VdUiStr null_str = {0, 0};
    vd_ui_div_new(0, null_str);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);
}

static void hspacefixed(float f)
{
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_ABSOLUTE, f, 1.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_ABSOLUTE, 0.f, 1.f);
    VdUiStr null_str = {0, 0};
    vd_ui_div_new(0, null_str);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);
}

static int expander(int *on, const char *label)
{
    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND,
                              vd_ui_coloring(vd_ui_gradient1(vd_ui_f4(0.33f, 0.33f, 0.33f, 1.f)),
                                             vd_ui_gradient1(vd_ui_f4(0.52f, 0.52f, 0.52f, 1.f)),
                                             vd_ui_gradient1(vd_ui_f4(0.33f, 0.33f, 0.33f, 1.f))));
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 1.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f);
    vd_ui_style_padding_push(VD_UI_LEFT, 4);
    VdUiDiv *div = vd_ui_div_newf(VD_UI_FLAG_BACKGROUND
                                  | VD_UI_FLAG_FLEX_HORIZONTAL
                                  | VD_UI_FLAG_ALIGN_CENTER
                                  | VD_UI_FLAG_CLICKABLE, label);
    vd_ui_style_padding_pop(VD_UI_LEFT);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);

    int click = 0;

    vd_ui_parent_push(div);

    VdUiDiv *lbl_div = vd_ui_labelf("%s##toggle-label", label);
    lbl_div->style.text.halign = VD_UI_TEXT_HALIGN_CENTER;

    vd_ui_spacer(VD_UI_AXISH);
    const VdUiSymbol sym_off = vd_ui_symbol((VdUiFontId){1}, 59393);
    const VdUiSymbol sym_on  = vd_ui_symbol((VdUiFontId){1}, 59396);
    VdUiSymbol sym = (*on) ? sym_on : sym_off;
    if (vd_ui_icon_buttonf(sym, "##toggle").clicked) {
        click = 1;
    }
    vd_ui_parent_pop();

    VdUiReply reply = vd_ui_call(div);
    if (reply.clicked) {
        click = 1;
    }

    if (click) {
        *on = !(*on);
    }
    return *on;
}

/** Controller layout is in units of a 107.32x74.94 reference (assets/controller.png is 1201x673) */
#define PREVIEW_REF_W           107.32f
#define PREVIEW_REF_H           74.94f
#define PREVIEW_STICK_BASE_DIM  13.86f
#define PREVIEW_STICK_DIM       8.61f
#define PREVIEW_TRIGGER_W       2.21f
#define PREVIEW_TRIGGER_H       12.99f
#define PREVIEW_INSET           16.f
#define PREVIEW_HEIGHT          320.f

enum {
    PREVIEW_TEX_CONTROLLER = 0,
    PREVIEW_TEX_A,
    PREVIEW_TEX_B,
    PREVIEW_TEX_X,
    PREVIEW_TEX_Y,
    PREVIEW_TEX_DUP,
    PREVIEW_TEX_DRIGHT,
    PREVIEW_TEX_DDOWN,
    PREVIEW_TEX_DLEFT,
    PREVIEW_TEX_SELECT,
    PREVIEW_TEX_START,
    PREVIEW_TEX_L1,
    PREVIEW_TEX_R1,
    PREVIEW_TEX_STICK_BASE,
    PREVIEW_TEX_STICK,
    PREVIEW_TEX_TRIGGER,
    PREVIEW_TEX_RADGRAD,
    PREVIEW_TEX_COUNT,
};

static const char *Preview_Texture_Files[PREVIEW_TEX_COUNT] = {
    [PREVIEW_TEX_CONTROLLER] = "assets/controller.png",
    [PREVIEW_TEX_A]          = "assets/controller_a.png",
    [PREVIEW_TEX_B]          = "assets/controller_b.png",
    [PREVIEW_TEX_X]          = "assets/controller_x.png",
    [PREVIEW_TEX_Y]          = "assets/controller_y.png",
    [PREVIEW_TEX_DUP]        = "assets/controller_dup.png",
    [PREVIEW_TEX_DRIGHT]     = "assets/controller_dright.png",
    [PREVIEW_TEX_DDOWN]      = "assets/controller_ddown.png",
    [PREVIEW_TEX_DLEFT]      = "assets/controller_dleft.png",
    [PREVIEW_TEX_SELECT]     = "assets/controller_select.png",
    [PREVIEW_TEX_START]      = "assets/controller_start.png",
    [PREVIEW_TEX_L1]         = "assets/controller_l1.png",
    [PREVIEW_TEX_R1]         = "assets/controller_r1.png",
    [PREVIEW_TEX_STICK_BASE] = "assets/controller_stick_l_base.png",
    [PREVIEW_TEX_STICK]      = "assets/controller_stick_l.png",
    [PREVIEW_TEX_TRIGGER]    = "assets/controller_lt.png",
    [PREVIEW_TEX_RADGRAD]    = "assets/radgrad.png",
};

typedef struct {
    int   button;   // VdFcButton
    int   texture;
    float pos[2];
    float dim[2];
    float off[4];
    float on[4];
    float glow[4];
} PreviewButton;

static const PreviewButton Preview_Buttons[] = {
    {VD_FC_BUTTON_SOUTH,          PREVIEW_TEX_A,      {77.68f, 24.38f}, { 7.61f, 7.61f}, {0.2f, 0.2f, 0.2f, 1.f}, {0.2f, 0.9f, 0.2f, 1.f}, {0.2f, 0.9f, 0.2f, 0.4f}},
    {VD_FC_BUTTON_EAST,           PREVIEW_TEX_B,      {85.08f, 17.44f}, { 7.61f, 7.61f}, {0.2f, 0.2f, 0.2f, 1.f}, {0.9f, 0.2f, 0.2f, 1.f}, {0.9f, 0.2f, 0.2f, 0.4f}},
    {VD_FC_BUTTON_WEST,           PREVIEW_TEX_X,      {70.35f, 16.89f}, { 7.61f, 7.61f}, {0.2f, 0.2f, 0.2f, 1.f}, {0.4f, 0.8f, 1.0f, 1.f}, {0.4f, 0.8f, 0.9f, 0.4f}},
    {VD_FC_BUTTON_NORTH,          PREVIEW_TEX_Y,      {77.86f,  9.91f}, { 7.61f, 7.61f}, {0.2f, 0.2f, 0.2f, 1.f}, {0.9f, 0.9f, 0.2f, 1.f}, {0.9f, 0.9f, 0.2f, 0.4f}},
    {VD_FC_BUTTON_DPAD_UP,        PREVIEW_TEX_DUP,    {36.75f, 29.94f}, { 5.16f, 8.19f}, {0.3f, 0.3f, 0.3f, 1.f}, {0.7f, 0.7f, 0.7f, 1.f}, {1.0f, 1.0f, 1.0f, 0.125f}},
    {VD_FC_BUTTON_DPAD_RIGHT,     PREVIEW_TEX_DRIGHT, {39.33f, 35.56f}, { 8.19f, 5.16f}, {0.3f, 0.3f, 0.3f, 1.f}, {0.7f, 0.7f, 0.7f, 1.f}, {1.0f, 1.0f, 1.0f, 0.125f}},
    {VD_FC_BUTTON_DPAD_DOWN,      PREVIEW_TEX_DDOWN,  {36.75f, 38.14f}, { 5.16f, 8.19f}, {0.3f, 0.3f, 0.3f, 1.f}, {0.7f, 0.7f, 0.7f, 1.f}, {1.0f, 1.0f, 1.0f, 0.125f}},
    {VD_FC_BUTTON_DPAD_LEFT,      PREVIEW_TEX_DLEFT,  {31.14f, 35.56f}, { 8.19f, 5.16f}, {0.3f, 0.3f, 0.3f, 1.f}, {0.7f, 0.7f, 0.7f, 1.f}, {1.0f, 1.0f, 1.0f, 0.125f}},
    {VD_FC_BUTTON_START,          PREVIEW_TEX_START,  {59.01f, 18.06f}, { 4.95f, 4.95f}, {0.3f, 0.3f, 0.3f, 1.f}, {0.7f, 0.7f, 0.7f, 1.f}, {1.0f, 1.0f, 1.0f, 0.125f}},
    {VD_FC_BUTTON_BACK,           PREVIEW_TEX_SELECT, {43.15f, 18.06f}, { 4.95f, 4.95f}, {0.3f, 0.3f, 0.3f, 1.f}, {0.7f, 0.7f, 0.7f, 1.f}, {1.0f, 1.0f, 1.0f, 0.125f}},
    {VD_FC_BUTTON_LEFT_SHOULDER,  PREVIEW_TEX_L1,     {16.34f,  0.00f}, {22.34f, 8.53f}, {0.3f, 0.3f, 0.3f, 1.f}, {0.7f, 0.7f, 0.7f, 1.f}, {1.0f, 1.0f, 1.0f, 0.125f}},
    {VD_FC_BUTTON_RIGHT_SHOULDER, PREVIEW_TEX_R1,     {68.56f,  0.00f}, {22.34f, 8.53f}, {0.3f, 0.3f, 0.3f, 1.f}, {0.7f, 0.7f, 0.7f, 1.f}, {1.0f, 1.0f, 1.0f, 0.125f}},
};

typedef struct {
    int   axis[2];  // VdFcAxis
    int   button;   // VdFcButton
    float base_pos[2];
    float stick_pos[2];
} PreviewStick;

static const PreviewStick Preview_Sticks[] = {
    {{VD_FC_AXIS_LEFT_X,  VD_FC_AXIS_LEFT_Y},  VD_FC_BUTTON_LEFT_STICK,  {18.94f, 13.86f}, {21.56f, 16.20f}},
    {{VD_FC_AXIS_RIGHT_X, VD_FC_AXIS_RIGHT_Y}, VD_FC_BUTTON_RIGHT_STICK, {61.02f, 29.99f}, {63.64f, 32.61f}},
};

typedef struct {
    int   axis;     // VdFcAxis
    float pos[2];
} PreviewTrigger;

static const PreviewTrigger Preview_Triggers[] = {
    {VD_FC_AXIS_LEFT_TRIGGER,  {38.98f, 57.19f}},
    {VD_FC_AXIS_RIGHT_TRIGGER, {67.00f, 57.19f}},
};

typedef struct {
    VdUiTextureId textures[PREVIEW_TEX_COUNT];
    float         glow[VD_FC_BUTTON_MAX];
    int           device_id;
    // Recomputed by controller_preview_draw for the div's rect
    float         origin[2];
    float         scale;
} ControllerPreview;

static ControllerPreview The_Preview;

static void controller_preview_load(void)
{
    for (int i = 0; i < PREVIEW_TEX_COUNT; ++i) {
        int w, h, nc;
        unsigned char *data = stbi_load(Preview_Texture_Files[i], &w, &h, &nc, 4);
        if (data == 0) {
            printf("Failed to load %s\n", Preview_Texture_Files[i]);
            continue;
        }

        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        // The preview is drawn at a fraction of the source resolution
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(data);

        The_Preview.textures[i].id = (uintptr_t)texture;
    }
}

static void preview_put_uv(ControllerPreview *p, int texture, const float pos[2], const float dim[2], float uv[4], const float color[4])
{
    float rect[4] = {
        p->origin[0] + pos[0] * p->scale,
        p->origin[1] + pos[1] * p->scale,
        p->origin[0] + (pos[0] + dim[0]) * p->scale,
        p->origin[1] + (pos[1] + dim[1]) * p->scale,
    };
    float colors[16] = {
        color[0], color[1], color[2], color[3],
        color[0], color[1], color[2], color[3],
        color[0], color[1], color[2], color[3],
        color[0], color[1], color[2], color[3],
    };
    float corner_radius[4] = {0.f, 0.f, 0.f, 0.f};
    vd_ui_push_texslice(rect, uv, colors, p->textures[texture], corner_radius, 0.f, 0.f);
}

static void preview_put(ControllerPreview *p, int texture, const float pos[2], const float dim[2], const float color[4])
{
    float uv[4] = {0.f, 0.f, 1.f, 1.f};
    preview_put_uv(p, texture, pos, dim, uv, color);
}

static void preview_put_glow(ControllerPreview *p, const float pos[2], const float dim[2], const float color[4], float t)
{
    if (t < 0.001f) {
        return;
    }

    float glow_pos[2]   = {pos[0] - dim[0] * 1.5f, pos[1] - dim[1] * 1.5f};
    float glow_dim[2]   = {dim[0] * 4.f, dim[1] * 4.f};
    float glow_color[4] = {color[0], color[1], color[2], color[3] * t};
    preview_put(p, PREVIEW_TEX_RADGRAD, glow_pos, glow_dim, glow_color);
}

static VD_UI_DRAW_PROC(controller_preview_draw)
{
    VD_UNUSED(div);
    ControllerPreview *p = (ControllerPreview*)usr;
    int id = p->device_id;

    float w = (rect[2] - rect[0]) - 2.f * PREVIEW_INSET;
    float h = (rect[3] - rect[1]) - 2.f * PREVIEW_INSET;
    if ((w <= 0.f) || (h <= 0.f)) {
        return;
    }

    float sw = w / PREVIEW_REF_W;
    float sh = h / PREVIEW_REF_H;
    p->scale     = sw < sh ? sw : sh;
    p->origin[0] = rect[0] + PREVIEW_INSET + (w - PREVIEW_REF_W * p->scale) * 0.5f;
    p->origin[1] = rect[1] + PREVIEW_INSET + (h - PREVIEW_REF_H * p->scale) * 0.5f;

    const float body_pos[2]   = {0.f, 0.f};
    const float body_dim[2]   = {PREVIEW_REF_W, PREVIEW_REF_H};
    const float body_color[4] = {0.15f, 0.15f, 0.15f, 1.f};
    preview_put(p, PREVIEW_TEX_CONTROLLER, body_pos, body_dim, body_color);

    for (int i = 0; i < (int)VD_ARRAY_COUNT(Preview_Buttons); ++i) {
        const PreviewButton *b = &Preview_Buttons[i];
        preview_put_glow(p, b->pos, b->dim, b->glow, p->glow[b->button]);
    }

    for (int i = 0; i < (int)VD_ARRAY_COUNT(Preview_Buttons); ++i) {
        const PreviewButton *b = &Preview_Buttons[i];
        preview_put(p, b->texture, b->pos, b->dim, vd_fc_button_down(id, b->button) ? b->on : b->off);
    }

    const float stick_base_dim[2] = {PREVIEW_STICK_BASE_DIM, PREVIEW_STICK_BASE_DIM};
    const float stick_dim[2]      = {PREVIEW_STICK_DIM, PREVIEW_STICK_DIM};
    const float stick_off[4]      = {0.7f, 0.7f, 0.7f, 1.f};
    const float stick_on[4]       = {0.9f, 0.2f, 0.2f, 1.f};
    const float stick_glow[4]     = {0.9f, 0.2f, 0.2f, 0.4f};
    for (int i = 0; i < (int)VD_ARRAY_COUNT(Preview_Sticks); ++i) {
        const PreviewStick *s = &Preview_Sticks[i];
        preview_put(p, PREVIEW_TEX_STICK_BASE, s->base_pos, stick_base_dim, stick_off);
    }

    for (int i = 0; i < (int)VD_ARRAY_COUNT(Preview_Sticks); ++i) {
        const PreviewStick *s = &Preview_Sticks[i];
        float stick_pos[2] = {
            s->stick_pos[0] + vd_fc_axis_value(id, s->axis[0]) * 0.25f * PREVIEW_STICK_DIM,
            s->stick_pos[1] + vd_fc_axis_value(id, s->axis[1]) * 0.25f * PREVIEW_STICK_DIM,
        };
        preview_put_glow(p, stick_pos, stick_dim, stick_glow, p->glow[s->button]);
        preview_put(p, PREVIEW_TEX_STICK, stick_pos, stick_dim, vd_fc_button_down(id, s->button) ? stick_on : stick_off);
    }

    const float trigger_dim[2]  = {PREVIEW_TRIGGER_W, PREVIEW_TRIGGER_H};
    const float trigger_bar[4]  = {0.2f, 0.2f, 0.2f, 1.f};
    const float trigger_fill[4] = {0.7f, 0.7f, 0.7f, 1.f};
    for (int i = 0; i < (int)VD_ARRAY_COUNT(Preview_Triggers); ++i) {
        const PreviewTrigger *t = &Preview_Triggers[i];
        float value = vd_fc_axis_value(id, t->axis);
        preview_put(p, PREVIEW_TEX_TRIGGER, t->pos, trigger_dim, trigger_bar);

        // Slice the bottom of the bar texture instead of squashing all of it into the fill
        float fill_pos[2] = {t->pos[0], t->pos[1] + (1.f - value) * PREVIEW_TRIGGER_H};
        float fill_dim[2] = {PREVIEW_TRIGGER_W, value * PREVIEW_TRIGGER_H};
        float fill_uv[4]  = {0.f, 1.f - value, 1.f, 1.f};
        preview_put_uv(p, PREVIEW_TEX_TRIGGER, fill_pos, fill_dim, fill_uv, trigger_fill);
    }
}

static void do_controller_preview(int id)
{
    float t = vd_fw_delta_s() * 7.f;
    if (t > 1.f) {
        t = 1.f;
    }

    for (int i = 0; i < VD_FC_BUTTON_MAX; ++i) {
        The_Preview.glow[i] = vd_ui_lerp(The_Preview.glow[i], vd_fc_button_down(id, i) ? 1.f : 0.f, t);
    }
    The_Preview.device_id = id;

    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_ABSOLUTE, PREVIEW_HEIGHT, 0.f);
    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, vd_ui_coloring_all4(vd_ui_f4(0.1f, 0.1f, 0.1f, 1.f)));
    VdUiDiv *div;
    VD_UI_WITH_STYLE_ROUNDING_ALL(VD_UI_FLAG_BACKGROUND, 8.f)
    {
        div = vd_ui_div_new(VD_UI_FLAG_BACKGROUND, VD_UI_LIT("##controller-preview"));
    }
    div->style.background.edge_softness = 0.002f;
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);

    vd_ui_set_draw_proc(div, controller_preview_draw, &The_Preview);
}

static void do_device_info(void)
{
    static float scroll_x = 0.f;
    static float scroll_y = 0.f;
    // vd_ui_scroll_begin(VD_UI_LIT("##contents-scroll"), &scroll_x, &scroll_y);

    int curr_device_id = The_State.curr_device_id;
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 1.f);
    vd_ui_parent_newf(0, "##contents");
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);


    // vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 10.f);
    VdUiDiv *name_div = vd_ui_labelf("%s##device-name", vd_fc_name(curr_device_id));
    name_div->style.text.halign = VD_UI_TEXT_HALIGN_CENTER;
    // vd_ui_style_size_pop(VD_UI_AXISH);

    do_controller_preview(curr_device_id);

    static int exp_device_raw = 0;
    static int exp_device_rmb = 0;
    static int exp_device_map = 0;

    if (expander(&exp_device_raw, "Raw Data Properties")) {
        int button_column_count = 8;
        vd_ui_labelf("Buttons: %d", vd_fc_button_count(curr_device_id));
        for (int i = 0; i < vd_fc_button_count(curr_device_id); i += button_column_count) {

            if (i != 0) {
                vspacefixed(4.f);
            }

            vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f);
            vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f);
            vd_ui_parent_newf(VD_UI_FLAG_FLEX_HORIZONTAL, "##button-row-%d", i);
            vd_ui_style_size_pop(VD_UI_AXISV);
            vd_ui_style_size_pop(VD_UI_AXISH);

            for (int j = 0; (j < button_column_count) && ((i + j) < vd_fc_button_count(curr_device_id)); ++j) {

                int button_id = i + j;
                VdUiDiv *div;
                VD_UI_WITH_STYLE_ROUNDING_ALL(VD_UI_FLAG_BACKGROUND, 16.f)
                {
                    div = vd_ui_div_newf(VD_UI_FLAG_BACKGROUND | VD_UI_FLAG_TEXT,
                                        "%d##button-%d", button_id, button_id);
                }

                div->style.size[0].mode  = VD_UI_SIZE_MODE_ABSOLUTE;
                div->style.size[0].value = 32.f;
                div->style.size[0].niceness = 0.f;
                div->style.size[1].mode  = VD_UI_SIZE_MODE_ABSOLUTE;
                div->style.size[1].value = 32.f;
                div->style.size[1].niceness = 0.f;

                div->style.text.halign = VD_UI_TEXT_HALIGN_CENTER;
                div->style.text.valign = VD_UI_TEXT_VALIGN_MIDDLE;

                div->style.background.edge_softness = 0.002f;

                if (vd_fc_raw_button_down(curr_device_id, button_id)) {
                    div->style.background.coloring = vd_ui_coloring_all4(vd_ui_f4(1.f, 0.f, 0.f, 1.f));
                } else {
                    div->style.background.coloring = vd_ui_coloring_all4(vd_ui_f4(0.1f, 0.1f, 0.1f, 1.f));
                }

                hspacefixed(4.f);
            }

            vd_ui_parent_pop();
        }

        vd_ui_labelf("Hats: %d", vd_fc_hat_count(curr_device_id));
        for (int i = 0; i < vd_fc_hat_count(curr_device_id); ++i) {
            vd_ui_labelf("Hat %d: %d", i, vd_fc_raw_hat_value(curr_device_id, i));
        }

        vd_ui_labelf("Axes: %d", vd_fc_axis_count(curr_device_id));
        for (int i = 0; i < vd_fc_axis_count(curr_device_id); ++i) {
            vd_ui_labelf("Axis[%d]: %lld##Axis%d", i, vd_fc_raw_axis_value(curr_device_id, i), i);
        }
    }

    if (expander(&exp_device_map, "Mapping")) {
        vd_ui_labelf("Has Mapping: %d", vd_fc_has_mapping(curr_device_id));
    }


    if (expander(&exp_device_rmb, "Force Feedback")) {
        vd_ui_labelf("Rumble Supported: %d", vd_fc_ff_rumble_supported(curr_device_id));

        vspacefixed(16.f);

        if (vd_fc_ff_rumble_supported(curr_device_id)) {
            vd_ui_labelf("Rumble");
            static float left_motor  = 0.5f;
            static float right_motor = 0.5f;
            vd_ui_sliderf_float(&left_motor, 0.f, 1.f, VD_UI_AXISH, "Left Motor Freq.");
            vd_ui_sliderf_float(&right_motor, 0.f, 1.f, VD_UI_AXISH, "Right Motor Freq.");

            if (vd_ui_buttonf("Toggle Rumble").clicked) {
                vd_fc_ff_rumble_set_motors(curr_device_id, left_motor, right_motor);
            }
        }

    }

    // vspacefixed(1000.f);

    vd_ui_parent_pop();
    // vd_ui_scroll_end();
}

static void do_sidebar(void)
{
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_CONTAIN_CHILDREN, 1.f, 0.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f);
    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, sidebar_bg());
    vd_ui_style_padding_push(VD_UI_LEFT, 8.f);
    vd_ui_style_padding_push(VD_UI_RIGHT, 8.f);
    vd_ui_parent_newf(VD_UI_FLAG_BACKGROUND, "##sidebar-bg");
    vd_ui_style_padding_pop(VD_UI_LEFT);
    vd_ui_style_padding_pop(VD_UI_RIGHT);
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);

    vd_ui_labelf("Devices");

    float mark_point_h = 0.f;
    for (int i = 0; i < vd_fc_count(); ++i) {
        if (i != 0) {
            vspacefixed(8.f);
        }

        float comp_pos_rel[2];
        if (device_field_button(i, comp_pos_rel)) {
            The_State.curr_device_id = i;
        }

        if (The_State.curr_device_id == i) {
            mark_point_h = comp_pos_rel[1];
        }
    }

    VdUiDiv *mark_point;
    VD_UI_WITH_STYLE_ROUNDING_ALL(VD_UI_FLAG_BACKGROUND, 4.f)
    {

        mark_point = vd_ui_div_new(VD_UI_FLAG_BACKGROUND
                                   | VD_UI_FLAG_FLOAT
                                   , VD_UI_LIT("##active-device-indicator"));
    }

    mark_point->style.background.coloring = vd_ui_coloring_all4(vd_ui_f4(0.294f, 0.71f, 0.925f, 1.f));
    mark_point->style.size[0].mode = VD_UI_SIZE_MODE_ABSOLUTE;
    mark_point->style.size[0].value = 4.f;
    mark_point->style.size[1].mode = VD_UI_SIZE_MODE_ABSOLUTE;
    mark_point->style.size[1].value = 20.f;
    mark_point->comp_pos_rel[0] = 8.f;
    mark_point->comp_pos_rel[1] = vd_ui_lerp(mark_point->comp_pos_rel[1], mark_point_h + 6.f, vd_fw_delta_s() * 10.f);
    mark_point->zoffset = 1;
    vd_ui_parent_pop();
}

static void do_content_area(void)
{
    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 0.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 500.f);
    vd_ui_style_coloring_push(VD_UI_FLAG_BACKGROUND, vd_ui_coloring_all4(vd_ui_f4(0.17f, 0.17f, 0.17f, 1.f)));
    VdUiDiv *content = vd_ui_div_new(VD_UI_FLAG_BACKGROUND, VD_UI_LIT("##content"));
    vd_ui_style_coloring_pop(VD_UI_FLAG_BACKGROUND);
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);

    vd_ui_parent_push(content);
    {
        if (vd_fc_count() == 0) {
            vd_ui_style_font_size_push(32.f);
            vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 0.f);
            vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 0.f);
            vd_ui_style_coloring_push(VD_UI_FLAG_TEXT, vd_ui_coloring_all4(vd_ui_f4(1.f, 1.f, 1.f, 0.5f)));
            VdUiDiv *d = vd_ui_parent_newf(VD_UI_FLAG_FLEX_HORIZONTAL
                                           | VD_UI_FLAG_ALIGN_CENTER
                                           | VD_UI_FLAG_TEXT, "No Devices Connected");
            vd_ui_style_coloring_pop(VD_UI_FLAG_TEXT);
            vd_ui_style_size_pop(VD_UI_AXISV);
            vd_ui_style_size_pop(VD_UI_AXISH);
            vd_ui_style_font_size_pop();
            vd_ui_parent_pop();

            d->style.text.halign = VD_UI_TEXT_HALIGN_CENTER;
            d->style.text.valign = VD_UI_TEXT_VALIGN_MIDDLE;
        } else {
            vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f);
            vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 1.f);

            vd_ui_parent_newf(VD_UI_FLAG_FLEX_HORIZONTAL, "##at-least-one-in");

            vd_ui_style_size_pop(VD_UI_AXISV);
            vd_ui_style_size_pop(VD_UI_AXISH);

            if (sidebar_display()) {
                do_sidebar();
            }

            do_device_info();

            vd_ui_parent_pop();
        }
    }
    vd_ui_parent_pop();
}

static int do_app()
{
    int should_continue_running = 1;

    int width, height;
    if (vd_fw_get_size(&width, &height)) {
        The_State.sidebar_overlaid = width < 800;
        if (!The_State.sidebar_overlaid) {
            The_State.sidebar = 1;
        } else {
            The_State.sidebar = 0;
        }
    }

    vd_ui_style_size_push(VD_UI_AXISH, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 0.f);
    vd_ui_style_size_push(VD_UI_AXISV, VD_UI_SIZE_MODE_PERCENT_OF_PARENT, 1.f, 0.f);
    VdUiDiv *app = vd_ui_div_new(0, VD_UI_LIT("##app"));
    vd_ui_style_size_pop(VD_UI_AXISV);
    vd_ui_style_size_pop(VD_UI_AXISH);

    vd_ui_parent_push(app);
    {
        should_continue_running = do_menu();
        do_content_area();
    }
    vd_ui_parent_pop();
    return should_continue_running;
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    The_State.sidebar_overlaid = 1;

    vd_fc_init();
    {

        size_t len;
        FILE *f = fopen("gamecontrollerdb.txt", "rb");
        fseek(f, 0, SEEK_END);
        len = ftell(f);
        fseek(f, 0, SEEK_SET);
        char *file = malloc(len);
        fclose(f);
        vd_fc_mapping_add_db(file, len);
        free(file);
    }

    vd_ui_init();
    vd_ui_debug_set_draw_cursor_on(0);
    vd_ui_debug_set_inspector_on(0);
    vd_ui_debug_set_metrics_on(0);
    vd_ui_debug_set_layout_recompute_vis_on(0);

    vd_fw_init(& (VdFwInitInfo) {
        .window_options = {
            .borderless = 1,
            .block_while_sizing = 0,
        },
    });

    vd_fw_set_resizable(1);

    // float scale;
    // vd_fw_get_scale(&scale);

    // vd_ui_set_scale(scale);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vd_ui_get_min_vertex_buffer_size(), 0, GL_DYNAMIC_DRAW);

    for (int i = 0; i < vd_ui_gl_get_num_attributes(); ++i) {
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        void *pointer;
        GLuint divisor;
        vd_ui_gl_get_attribute_properties(i, &size, &type, &normalized, &stride, &pointer, &divisor);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, size, type, normalized, stride, pointer);
        glVertexAttribDivisor(i, divisor);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // int rects[1][4] = {
    //     {0, 0, 100, 30},
    // };
    // vd_fw_set_ncrects(1, rects);
    GLuint program;
    {
        const char *vshd_src;
        size_t vshd_sz;
        const char *fshd_src;
        size_t fshd_sz;
        vd_ui_gl_get_default_shader_sources(&vshd_src, &vshd_sz, &fshd_src, &fshd_sz);

        GLuint vshd = vd_fw_compile_shader(GL_VERTEX_SHADER, vshd_src);
        GLuint fshd = vd_fw_compile_shader(GL_FRAGMENT_SHADER, fshd_src);
        program = glCreateProgram();
        glAttachShader(program, vshd);
        glAttachShader(program, fshd);
        vd_fw_link_program(program);
    }

    controller_preview_load();

    vd_fw_set_vsync_on(0);
    while (vd_fw_running()) {

        int num_events = 0;
        VdFwEvent *events = vd_fw_poll(&num_events);

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        float dpi_scale;
        if (vd_fw_get_scale(&dpi_scale)) {
            vd_ui_set_scale(dpi_scale);
        }

        vd_fc_poll();

        float delta_seconds = vd_fw_delta_s();

        vd_fw_lock();

        int w, h;
        vd_fw_get_size(&w, &h);
        vd_ui_frame_begin(delta_seconds);

        float mx, my;
        int mouse_state = vd_fw_get_mouse_statef(&mx, &my);
        VD_UNUSED(mouse_state);

        float wx, wy;
        int wheel_moved = vd_fw_get_mouse_wheel(&wx, &wy);
        (void)wheel_moved;

        int focused;
        if (vd_fw_get_focused(&focused)) {
            vd_ui_event_focus(focused);
        }

        vd_ui_event_size((float)w, (float)h);
        for (int i = 0; i < num_events; ++i) {
            VdFwEvent *evt = &events[i];
            switch (evt->type) {
                case VD_FW_EVENT_TYPE_MOUSE_MOVE: {
                    vd_ui_event_mouse_location((float)evt->data.mouse_move.x, (float)evt->data.mouse_move.y);
                } break;

                case VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP: {
                    vd_ui_event_mouse_button(vd_ui_vd_fw_mouse_button_translate(evt->data.mouse_button_up.button), 0);
                } break;

                case VD_FW_EVENT_TYPE_MOUSE_BUTTON_DOWN: {
                    vd_ui_event_mouse_button(vd_ui_vd_fw_mouse_button_translate(evt->data.mouse_button_down.button), 1);
                } break;

                case VD_FW_EVENT_TYPE_KEY_DOWN: {
                    int shift   = (evt->data.key_down.modifiers & VD_FW_MOD_SHIFT)   ? 1 : 0;
                    int control = (evt->data.key_down.modifiers & VD_FW_MOD_CONTROL) ? 1 : 0;
                    int alt     = (evt->data.key_down.modifiers & VD_FW_MOD_ALT)     ? 1 : 0;
                    if (shift && evt->data.key_down.key != VD_FW_KEY_LSHIFT) {
                        printf("SHIFT\n");
                    }
                    vd_ui_event_mod(VD_UI_MOD_SHIFT, shift);
                    vd_ui_event_mod(VD_UI_MOD_CONTROL, control);
                    vd_ui_event_mod(VD_UI_MOD_ALT, alt);
                    vd_ui_event_key_press(vd_ui_vd_fw_key_translate(evt->data.key_down.key));
                } break;

                case VD_FW_EVENT_TYPE_CHARACTER: {
                    vd_ui_event_char(evt->data.character.codepoint);
                } break;

                default: break;
            }
        }
        vd_ui_event_mouse_wheel(wx, wy);

        if (!do_app()) {
            vd_fw_quit();
        }

        vd_ui_frame_end();

        vd_ui_render_begin();
        {
            int nc_rect[4];
            int rects[16][4];
            int written = 0;
            int total = 0;
            int changed = vd_ui_ws_nc_area_get(nc_rect, 16, &total, &written, rects);
            if (changed) {
                vd_fw_set_ncrects(nc_rect, written, rects);
            }
        }
        vd_ui_render_end();

        // Process updates
        size_t num_updates;
        VdUiUpdate *updates = vd_ui_frame_get_updates(&num_updates);

        for (size_t i = 0; i < num_updates; ++i) {
            VdUiUpdate *update = &updates[i];
            switch (update->type) {
                case VD_UI_UPDATE_TYPE_NEW_TEXTURE: {
                    int width                = update->data.new_texture.width;
                    int height               = update->data.new_texture.height;
                    void *buffer             = update->data.new_texture.buffer;
                    size_t buffer_size       = update->data.new_texture.size;
                    VdUiTextureId *id        = update->data.new_texture.write_id;

                    VD_UNUSED(buffer_size);

                    GLuint texture;
                    glGenTextures(1, &texture);
                    glBindTexture(GL_TEXTURE_2D, texture);

                    GLint  level;
                    GLint  internal_format;
                    GLint  border;
                    GLenum format;
                    GLenum type;
                    vd_ui_gl_cv_texture_format(
                        update->data.new_texture.format,
                        &level,
                        &internal_format, 
                        &border, 
                        &format,
                        &type);

                    GL_CHECK(glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, format, type, buffer));
                    // GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
                    GL_CHECK(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
                    GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
                    GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

                    id->id = (uintptr_t)texture;
                } break;

                case VD_UI_UPDATE_TYPE_WRITE_TEXTURE: {
                    GLuint texture = (GLuint)update->data.write_texture.texture.id;

                    GLint  level;
                    GLint  internal_format;
                    GLint  border;
                    GLenum format;
                    GLenum type;
                    vd_ui_gl_cv_texture_format(
                        update->data.write_texture.format,
                        &level,
                        &internal_format, 
                        &border, 
                        &format,
                        &type);

                    glBindTexture(GL_TEXTURE_2D, texture);
                    glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, update->data.write_texture.width, update->data.write_texture.height, format, type, update->data.write_texture.buffer);
                    glBindTexture(GL_TEXTURE_2D, 0);
                } break;

                default: break;
            }
        }

        // Get vertex buffer
        size_t buffer_size;
        void *buffer = vd_ui_frame_get_vertex_buffer(&buffer_size);

        // Get render passes
        unsigned int num_passes;
        VdUiRenderPass *passes = vd_ui_frame_get_render_passes(&num_passes);

        glViewport(0, 0, w, h);
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Loop through render passes
        for (unsigned int i = 0; i < num_passes; ++i) {
            VdUiRenderPass *pass = &passes[i];
            GLuint texture_id = (GLuint)pass->selected_texture.id;
            GLint clip_width   = (GLint)pass->clip[2] - (GLint)pass->clip[0];
            GLint clip_height  = (GLint)pass->clip[3] - (GLint)pass->clip[1];
            GLint clip_x       = (GLint)pass->clip[0];
            GLint clip_lower_y = (GLint)h - (GLint)pass->clip[3];

            glScissor(clip_x, clip_lower_y, clip_width, clip_height);

            glUseProgram(program);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_id);
            glUniform2f(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_resolution()), (float)w, (float)h);
            glUniform1i(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_texture()), 0);
            glUniform2f(glGetUniformLocation(program, vd_ui_gl_get_uniform_name_mouse()), mx, my);

            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);

            // Update vertex buffer
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, pass->instance_count * sizeof(VdUiVertex), (unsigned char*)buffer + pass->first_instance * sizeof(VdUiVertex));
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glDrawArraysInstanced(
                GL_TRIANGLE_STRIP,
                0,
                4,
                pass->instance_count);
        }
        
        vd_fw_swap();
        vd_fw_unlock();
    }

    vd_fc_quit();
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"

#define VD_UI_IMPL
#include "vd_ui.h"

#define VD_IMPL
#include "vd.h"

#define VD_FC_IMPL
#include "vd_fc.h"

#define STB_IMAGE_IMPLEMENTATION
#include "ext/stb_image.h"

#if defined(__clang__)
#pragma clang diagnostic pop
#endif // defined(__clang__)
