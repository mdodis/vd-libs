/**
 * vd_input.h - An OS input translation library
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
#ifndef VD_INPUT_H
#define VD_INPUT_H
#define VD_INPUT_VERSION_MAJOR 0
#define VD_INPUT_VERSION_MINOR 0
#define VD_INPUT_VERSION_PATCH 0
#define VD_INPUT_VERSION       ((VD_INPUT_VERSION_MAJOR << 16) | (VD_INPUT_VERSION_MINOR << 8) | (VD_INPUT_VERSION_PATCH))

#ifndef VD_H
#error "vd_input.h requires vd.h"
#endif // !VD_H

typedef enum {
    VD_(INPUT_EVENT_TYPE_KEYDOWN),
    VD_(INPUT_EVENT_TYPE_KEYUP),
    VD_(INPUT_EVENT_TYPE_KEYPRESS),
    VD_(INPUT_EVENT_TYPE_MOUSE_MOVE),
} VD(InputEventType);

typedef VD(u32) VD(InputKeyboardScancode);

typedef union __VD_Input_EventData {
    struct {
        VD(InputKeyboardScancode) scancode;
    } key;

    struct {
        VD(f32) delta_x, delta_y;
        VD(f32) loc_x,   loc_y;
    } mouse_move;
} VD(InputEventData);

typedef struct __VD_Input_Event {
    VD(InputEventType) type;
    VD(InputEventData) data;
} VD(InputEvent);

typedef enum : VD(u8) {
    VD_(INPUT_KEY_STATE_WAS_DOWN) = 1 << 0,
    VD_(INPUT_KEY_STATE_IS_DOWN)  = 1 << 1,
} VD(InputKeyStateFlags);

enum {
    VD_(INPUT_KEYBOARD_SCANCODE_UNKNOWN) = 0,
    VD_(INPUT_KEYBOARD_SCANCODE_A),
    VD_(INPUT_KEYBOARD_SCANCODE_B),
    VD_(INPUT_KEYBOARD_SCANCODE_C),
    VD_(INPUT_KEYBOARD_SCANCODE_D),
    VD_(INPUT_KEYBOARD_SCANCODE_E),
    VD_(INPUT_KEYBOARD_SCANCODE_F),
    VD_(INPUT_KEYBOARD_SCANCODE_G),
    VD_(INPUT_KEYBOARD_SCANCODE_H),
    VD_(INPUT_KEYBOARD_SCANCODE_I),
    VD_(INPUT_KEYBOARD_SCANCODE_J),
    VD_(INPUT_KEYBOARD_SCANCODE_K),
    VD_(INPUT_KEYBOARD_SCANCODE_L),
    VD_(INPUT_KEYBOARD_SCANCODE_M),
    VD_(INPUT_KEYBOARD_SCANCODE_N),
    VD_(INPUT_KEYBOARD_SCANCODE_O),
    VD_(INPUT_KEYBOARD_SCANCODE_P),
    VD_(INPUT_KEYBOARD_SCANCODE_Q),
    VD_(INPUT_KEYBOARD_SCANCODE_R),
    VD_(INPUT_KEYBOARD_SCANCODE_S),
    VD_(INPUT_KEYBOARD_SCANCODE_T),
    VD_(INPUT_KEYBOARD_SCANCODE_U),
    VD_(INPUT_KEYBOARD_SCANCODE_V),
    VD_(INPUT_KEYBOARD_SCANCODE_W),
    VD_(INPUT_KEYBOARD_SCANCODE_X),
    VD_(INPUT_KEYBOARD_SCANCODE_Y),
    VD_(INPUT_KEYBOARD_SCANCODE_Z),
    VD_(INPUT_KEYBOARD_SCANCODE_SPACE),
    VD_(INPUT_KEYBOARD_SCANCODE_ESCAPE),
    VD_(INPUT_KEYBOARD_SCANCODE_MAX),
};

typedef struct __VD_Input_KeyboardState {
    VD(b32) states[VD_(INPUT_KEYBOARD_SCANCODE_MAX)];
} VD(InputKeyboardState);

typedef struct __VD_Input_KeyboardStateGetConstantKeyDownEventsInfo {
    VD(Arena) *arena;
    struct {
        VD(u32)        *num_events;
        VD(InputEvent) **events;
    } result;
} VD(InputKeyboardStateGetConstantKeyDownEventsInfo);

VD(Str)                   VDF(input_keyboard_scancode_get_name)                  (VD(InputKeyboardScancode) scancode);
void                      VDF(input_keyboard_state_init)                         (VD(InputKeyboardState) *kbstate);
void                      VDF(input_keyboard_state_send_event)                   (VD(InputKeyboardState) *kbstate, VD(InputEvent) *evt);
void                      VDF(input_keyboard_state_get_constant_key_down_events) (VD(InputKeyboardState) *kbstate, VD(InputKeyboardStateGetConstantKeyDownEventsInfo) *info);

#if VD_PLATFORM_MACOS
VD(InputKeyboardScancode) VDF(input_macos_nskeycode_to_keyboard_scancode) (unsigned short nskeycode);

#define VD_INPUT_TRANSLATE_KEYBOARD_SCANCODE(x) VDF(input_macos_nskeycode_to_keyboard_scancode)(x)
#endif

#endif // !VD_INPUT_H

#ifdef VD_INPUT_IMPL
static Str VDI(Input_Scancode_To_Name)[] = {
    [VD_(INPUT_KEYBOARD_SCANCODE_UNKNOWN)] = LIT("Unknown"),
    [VD_(INPUT_KEYBOARD_SCANCODE_A)]       = LIT("a"),
    [VD_(INPUT_KEYBOARD_SCANCODE_B)]       = LIT("b"),
    [VD_(INPUT_KEYBOARD_SCANCODE_C)]       = LIT("c"),
    [VD_(INPUT_KEYBOARD_SCANCODE_D)]       = LIT("d"),
    [VD_(INPUT_KEYBOARD_SCANCODE_E)]       = LIT("e"),
    [VD_(INPUT_KEYBOARD_SCANCODE_F)]       = LIT("f"),
    [VD_(INPUT_KEYBOARD_SCANCODE_G)]       = LIT("g"),
    [VD_(INPUT_KEYBOARD_SCANCODE_H)]       = LIT("h"),
    [VD_(INPUT_KEYBOARD_SCANCODE_I)]       = LIT("i"),
    [VD_(INPUT_KEYBOARD_SCANCODE_J)]       = LIT("j"),
    [VD_(INPUT_KEYBOARD_SCANCODE_K)]       = LIT("k"),
    [VD_(INPUT_KEYBOARD_SCANCODE_L)]       = LIT("l"),
    [VD_(INPUT_KEYBOARD_SCANCODE_M)]       = LIT("m"),
    [VD_(INPUT_KEYBOARD_SCANCODE_N)]       = LIT("n"),
    [VD_(INPUT_KEYBOARD_SCANCODE_O)]       = LIT("o"),
    [VD_(INPUT_KEYBOARD_SCANCODE_P)]       = LIT("p"),
    [VD_(INPUT_KEYBOARD_SCANCODE_Q)]       = LIT("q"),
    [VD_(INPUT_KEYBOARD_SCANCODE_R)]       = LIT("r"),
    [VD_(INPUT_KEYBOARD_SCANCODE_S)]       = LIT("s"),
    [VD_(INPUT_KEYBOARD_SCANCODE_T)]       = LIT("t"),
    [VD_(INPUT_KEYBOARD_SCANCODE_U)]       = LIT("u"),
    [VD_(INPUT_KEYBOARD_SCANCODE_V)]       = LIT("v"),
    [VD_(INPUT_KEYBOARD_SCANCODE_W)]       = LIT("w"),
    [VD_(INPUT_KEYBOARD_SCANCODE_X)]       = LIT("x"),
    [VD_(INPUT_KEYBOARD_SCANCODE_Y)]       = LIT("y"),
    [VD_(INPUT_KEYBOARD_SCANCODE_Z)]       = LIT("z"),
    [VD_(INPUT_KEYBOARD_SCANCODE_SPACE)]   = LIT("Spacebar"),
    [VD_(INPUT_KEYBOARD_SCANCODE_ESCAPE)]  = LIT("Escape"),  
};

VD(Str) VDF(input_keyboard_scancode_get_name)(VD(InputKeyboardScancode) scancode)
{
    return VDI(Input_Scancode_To_Name)[scancode];
}

void VDF(input_keyboard_state_init)(VD(InputKeyboardState) *kbstate)
{
    VD_MEMSET(kbstate->states, 0, sizeof(VD(b32)) * VD_(INPUT_KEYBOARD_SCANCODE_MAX));
}

void VDF(input_keyboard_state_send_event)(VD(InputKeyboardState) *kbstate, VD(InputEvent) *evt)
{
    if ((evt->type != VD_(INPUT_EVENT_TYPE_KEYDOWN)) && (evt->type != VD_(INPUT_EVENT_TYPE_KEYUP))) {
        return;
    }

    kbstate->states[evt->data.key.scancode] = evt->type == VD_(INPUT_EVENT_TYPE_KEYDOWN);
}

void VDF(input_keyboard_state_get_constant_key_down_events)(VD(InputKeyboardState) *kbstate, VD(InputKeyboardStateGetConstantKeyDownEventsInfo) *info)
{
    VD_DYNARRAY VD(InputEvent) *events = 0;
    VD_DYNARRAY_INIT(events, info->arena);

    for (u32 i = 0; i < VD_(INPUT_KEYBOARD_SCANCODE_MAX); ++i) {
        if (kbstate->states[i]) {
            VD(InputEvent) *evt = VD_DYNARRAY_PUSH(events);
            *evt = (VD(InputEvent)) {
                .type = VD_(INPUT_EVENT_TYPE_KEYDOWN),
                .data = {
                    .key = {
                        .scancode = i,
                    }
                }
            };
        }
    }

    *info->result.num_events = VD_DYNARRAY_LEN(events);
    *info->result.events = events;
}

#if VD_PLATFORM_MACOS
VD(InputKeyboardScancode) VDF(input_macos_nskeycode_to_keyboard_scancode)(unsigned short nskeycode)
{
    // @note(mdodis): Special thanks to https://gist.github.com/michaelgiraldo/c3ca98f06aeb20acae1fd8d82fab0811

    switch (nskeycode) {
        case 0:  return VD_(INPUT_KEYBOARD_SCANCODE_A);
        case 11: return VD_(INPUT_KEYBOARD_SCANCODE_B);
        case 8:  return VD_(INPUT_KEYBOARD_SCANCODE_C);
        case 2:  return VD_(INPUT_KEYBOARD_SCANCODE_D);
        case 14: return VD_(INPUT_KEYBOARD_SCANCODE_E);
        case 3:  return VD_(INPUT_KEYBOARD_SCANCODE_F);
        case 5:  return VD_(INPUT_KEYBOARD_SCANCODE_G);
        case 4:  return VD_(INPUT_KEYBOARD_SCANCODE_H);
        case 34: return VD_(INPUT_KEYBOARD_SCANCODE_I);
        case 38: return VD_(INPUT_KEYBOARD_SCANCODE_J);
        case 40: return VD_(INPUT_KEYBOARD_SCANCODE_K);
        case 37: return VD_(INPUT_KEYBOARD_SCANCODE_L);
        case 46: return VD_(INPUT_KEYBOARD_SCANCODE_M);
        case 45: return VD_(INPUT_KEYBOARD_SCANCODE_N);
        case 31: return VD_(INPUT_KEYBOARD_SCANCODE_O);
        case 35: return VD_(INPUT_KEYBOARD_SCANCODE_P);
        case 12: return VD_(INPUT_KEYBOARD_SCANCODE_Q);
        case 15: return VD_(INPUT_KEYBOARD_SCANCODE_R);
        case 1:  return VD_(INPUT_KEYBOARD_SCANCODE_S);
        case 17: return VD_(INPUT_KEYBOARD_SCANCODE_T);
        case 32: return VD_(INPUT_KEYBOARD_SCANCODE_U);
        case 9:  return VD_(INPUT_KEYBOARD_SCANCODE_V);
        case 13: return VD_(INPUT_KEYBOARD_SCANCODE_W);
        case 7:  return VD_(INPUT_KEYBOARD_SCANCODE_X);
        case 16: return VD_(INPUT_KEYBOARD_SCANCODE_Y);
        case 6:  return VD_(INPUT_KEYBOARD_SCANCODE_Z);
        case 53: return VD_(INPUT_KEYBOARD_SCANCODE_ESCAPE);
        case 49: return VD_(INPUT_KEYBOARD_SCANCODE_SPACE);
        default: return VD_(INPUT_KEYBOARD_SCANCODE_UNKNOWN);
    }
}

#endif // VD_PLATFORM_MACOS

#endif // VD_INPUT_IMPL
