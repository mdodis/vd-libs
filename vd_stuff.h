/**
 * vd_stuff.h - Random Stuff
 * ---------------------------------------------------------------------------------------------------------------------
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
 * ---------------------------------------------------------------------------------------------------------------------
 */
#ifndef VD_STUFF_H
#define VD_STUFF_H
#define VD_STUFF_VERSION_MAJOR 0
#define VD_STUFF_VERSION_MINOR 0
#define VD_STUFF_VERSION_PATCH 1
#define VD_STUFF_VERSION       ((VD_STUFF_VERSION_MAJOR << 16) | (VD_STUFF_VERSION_MINOR << 8) | (VD_STUFF_VERSION_PATCH))

#ifndef VD_STUFF_API
#   ifdef VD_STUFF_STATIC
#       define VD_STUFF_API static
#   else
#       define VD_STUFF_API extern
#   endif // VD_STUFF_STATIC
#endif // !VD_STUFF_API

// Graphics
// | Feature                                  | Include File       | Requirements
// |------------------------------------------|--------------------|---------------------------------|
// | VD_STUFF_GRAPHICS_INCLUDE                | Include vd_fw.h    | None                            |
// | VD_STUFF_GRAPHICS_VD_UM_INCLUDE          | Include vd_um.h    | VD_STUFF_GRAPHICS_INCLUDE       |
#ifndef VD_STUFF_GRAPHICS_INCLUDE
#   define VD_STUFF_GRAPHICS_INCLUDE 0
#endif // !VD_STUFF_GRAPHICS_INCLUDE

#ifndef VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#   define VD_STUFF_GRAPHICS_VD_UM_INCLUDE 0
#endif // !VD_STUFF_GRAPHICS_VD_UM_INCLUDE

#if VD_STUFF_GRAPHICS_INCLUDE
#   ifndef VD_FW_H
#       error "You must include vd_fw.h before using VD_STUFF_GRAPHICS_INCLUDE"
#   endif // !VD_FW_H
#   if VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#       ifndef VD_UM_H
#           error "You must include vd_um.h before using VD_STUFF_GRAPHICS_VD_UM_INCLUDE"
#       endif // !VD_UM_H
#   endif // VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#endif // VD_STUFF_GRAPHICS_INCLUDE

VD_STUFF_API void vd_stuff_init(void);
#if VD_STUFF_GRAPHICS_INCLUDE
VD_STUFF_API int vd_stuff_looping(void);
VD_STUFF_API void vd_stuff_graphics_begin(void);
VD_STUFF_API void vd_stuff_graphics_begin(void);

#   if VD_STUFF_GRAPHICS_VD_UM_INCLUDE
VD_STUFF_API void vd_stuff_um_push_events_to_current_viewport(void);
#   endif // VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#endif // VD_STUFF_GRAPHICS_INCLUDE

#endif // !VD_STUFF_H

#ifdef VD_STUFF_IMPL
typedef struct {
    int initialized;
#if VD_STUFF_GRAPHICS_INCLUDE
    VdFwEvent *events;
    int       num_events;
#endif // VD_STUFF_GRAPHICS_INCLUDE
} VdStuff__InternalData;

VdStuff__InternalData Vd_Stuff_G = {0};

VD_STUFF_API void vd_stuff_init(void)
{
#if VD_STUFF_GRAPHICS_INCLUDE
    VdFwInitInfo fw_init_info = {0};
    fw_init_info.api = VD_FW_GRAPHICS_API_OPENGL;
    vd_fw_init(&fw_init_info);
#if VD_STUFF_GRAPHICS_VD_UM_INCLUDE
    vd_um_init();
#endif // VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#endif // VD_STUFF_GRAPHICS_INCLUDE
}

#if VD_STUFF_GRAPHICS_INCLUDE
VD_STUFF_API int vd_stuff_looping(void)
{
    int running = vd_fw_running();

    if (running) {
        Vd_Stuff_G.events = vd_fw_poll(&Vd_Stuff_G.num_events);
    }

    return running;
}

#   if VD_STUFF_GRAPHICS_VD_UM_INCLUDE
VD_STUFF_API void vd_stuff_um_push_events_to_current_viewport(void)
{

    float mouse_pos[2];
    int mouse_state = vd_fw_get_mouse_statef(&mouse_pos[0], &mouse_pos[0]);
    float mx, my;
    vd_fw_get_mouse_delta(&mx, &my);

    vd_um_event_mouse_position(mouse_pos);
    vd_um_event_mouse_button(0, mouse_state & VD_FW_MOUSE_STATE_LEFT_BUTTON_DOWN);
    float mouse_delta[2] = { mx, my };
    vd_um_event_mouse_delta(mouse_delta);
}
#   endif // VD_STUFF_GRAPHICS_VD_UM_INCLUDE

#endif // VD_STUFF_GRAPHICS_INCLUDE

#if VD_STUFF_GRAPHICS_INCLUDE
#   ifndef VD_FW_IMPL
#       define VD_FW_IMPL
#       include "vd_fw.h"
#   endif // !VD_FW_IMPL
#   ifdef VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#       ifndef VD_UM_IMPL
#           define VD_UM_IMPL
#           include "vd_um.h"
#       endif // !VD_UM_IMPL
#   endif // VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#endif // VD_STUFF_GRAPHICS_INCLUDE
#endif // VD_STUFF_IMPL