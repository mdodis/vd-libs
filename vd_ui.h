/**
 * vd_ui.h - UI library
 * 
 * zlib License
 * 
 * (C) Copyright 2025-2026 Michael Dodis (michaeldodisgr@gmail.com)
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */
#ifndef VD_UI_H
#define VD_UI_H
#define VD_UI_VERSION_MAJOR    0
#define VD_UI_VERSION_MINOR    0
#define VD_UI_VERSION_PATCH    1
#define VD_UI_VERSION          ((VD_UI_VERSION_MAJOR << 16) | (VD_UI_VERSION_MINOR << 8) | (VD_UI_VERSION_PATCH))

#ifndef VD_H
#error "vd_ui.h requires vd.h"
#endif // !VD_H

typedef struct VdUiContext VdUiContext;

typedef struct VdUiDiv VdUiDiv;
struct VdUiDiv {
    VdUiDiv *first;
    VdUiDiv *last;
    VdUiDiv *next;
    VdUiDiv *prev;
    VdUiDiv *parent;
};

typedef struct {
    int a;
} VdUiContextCreateInfo;

extern void         vd_ui_init(void);
extern VdUiContext* vd_ui_context_create(VdUiContextCreateInfo *info);
extern void         vd_ui_context_set(VdUiContext *context);
extern VdUiContext* vd_ui_context_get(void);
extern VdUiDiv*     vd_ui_div_new(VdStr string);
extern VdUiDiv*     vd_ui_push_parent(VdUiDiv *div);
extern VdUiDiv*     vd_ui_pop_parent(void);

#endif // !VD_UI_H

#ifdef VD_UI_IMPL

static VdUiContext *Vd_Ui_Global_Context = 0;

#define VD_UI_PARENT_STACK_MAX 256

struct VdUiContext {
    VD_STRMAP VdUiDiv *map;
    VdUiDiv           *parents[VD_UI_PARENT_STACK_MAX];
    VdUiDiv           root;
};

void vd_ui_init(void)
{
    vd_ui_context_set(vd_ui_context_create(0));
}

VdUiContext *vd_ui_context_create(VdUiContextCreateInfo *info)
{
    VD_UNUSED(info);

    VdUiContext *result = VD_MALLOC(sizeof(VdUiContext));
    VD_MEMSET(result, 0, sizeof(*result));
    return result;
}

void vd_ui_context_set(VdUiContext *context)
{
    Vd_Ui_Global_Context = context;
}

VdUiContext* vd_ui_context_get(void)
{
    return Vd_Ui_Global_Context;
}

#endif // VD_UI_IMPL