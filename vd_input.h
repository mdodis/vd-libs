/**
 * \internal
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
 *
 * @todo(mdodis): Add more key handling + modifiers
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
    VD_INPUT_EVENT_TYPE_INVALID = 0,
    VD_INPUT_EVENT_TYPE_KEY_DOWN,
    VD_INPUT_EVENT_TYPE_KEY_UP,
    VD_INPUT_EVENT_TYPE_KEY_PRESS,
    VD_INPUT_EVENT_TYPE_MOUSE_MOVE,
    VD_INPUT_EVENT_TYPE_MOUSE_DOWN,
    VD_INPUT_EVENT_TYPE_MOUSE_UP,
    VD_INPUT_EVENT_TYPE_MOUSE_WHEEL,
    VD_INPUT_EVENT_TYPE_CHARACTER,
    VD_INPUT_EVENT_TYPE_MAX,

#if VD_MACRO_ABBREVIATIONS
    INPUT_EVENT_TYPE_INVALID     = VD_INPUT_EVENT_TYPE_INVALID,
    INPUT_EVENT_TYPE_KEY_DOWN    = VD_INPUT_EVENT_TYPE_KEY_DOWN,
    INPUT_EVENT_TYPE_KEY_UP      = VD_INPUT_EVENT_TYPE_KEY_UP,
    INPUT_EVENT_TYPE_KEY_PRESS   = VD_INPUT_EVENT_TYPE_KEY_PRESS,
    INPUT_EVENT_TYPE_MOUSE_MOVE  = VD_INPUT_EVENT_TYPE_MOUSE_MOVE,
    INPUT_EVENT_TYPE_MOUSE_DOWN  = VD_INPUT_EVENT_TYPE_MOUSE_DOWN,
    INPUT_EVENT_TYPE_MOUSE_UP    = VD_INPUT_EVENT_TYPE_MOUSE_UP,
    INPUT_EVENT_TYPE_MOUSE_WHEEL = VD_INPUT_EVENT_TYPE_MOUSE_WHEEL,
    INPUT_EVENT_TYPE_CHARACTER   = VD_INPUT_EVENT_TYPE_CHARACTER,
    INPUT_EVENT_TYPE_MAX         = VD_INPUT_EVENT_TYPE_MAX,
#endif
} VdInputEventType;

typedef Vdu32 VdInputKeyboardScancode;
typedef Vdu32 VdInputMouseButton;
typedef Vdu32 VdInputKeyboardModifier;

typedef union __VD_Input_EventData {
    struct {
        VdInputKeyboardScancode scancode;
    } key;

    struct {
        Vdf32 delta_x, delta_y;
        Vdf32 loc_x,   loc_y;
    } mouse_move;

    struct {
        Vdf32              loc_x, loc_y;
        VdInputMouseButton button;
    } mouse_button;

    struct {
        Vdf32 dx, dy;
    } mouse_wheel;

    struct {
        Vdu32 codepoint;
    } character;

} VdInputEventData;

typedef struct __VD_Input_Event {
    VdInputEventType type;
    VdInputEventData data;
} VdInputEvent;

enum {
    VD_INPUT_KEY_STATE_WAS_DOWN = 1 << 0,
    VD_INPUT_KEY_STATE_IS_DOWN  = 1 << 1,
#if VD_MACRO_ABBREVIATIONS
    INPUT_KEY_STATE_WAS_DOWN = VD_INPUT_KEY_STATE_WAS_DOWN,
    INPUT_KEY_STATE_IS_DOWN  = VD_INPUT_KEY_STATE_IS_DOWN,
#endif // VD_MACRO_ABBREVIATIONS
};
typedef Vdu8 VdInputKeyStateFlags;

enum {
    VD_INPUT_MOUSE_BUTTON_UNKNOWN = 0,
    VD_INPUT_MOUSE_BUTTON_LEFT,
    VD_INPUT_MOUSE_BUTTON_RIGHT,
    VD_INPUT_MOUSE_BUTTON_MIDDLE,
    VD_INPUT_MOUSE_BUTTON_MAX,
#if VD_MACRO_ABBREVIATIONS
    INPUT_MOUSE_BUTTON_UNKNOWN = VD_INPUT_MOUSE_BUTTON_UNKNOWN,
    INPUT_MOUSE_BUTTON_LEFT = VD_INPUT_MOUSE_BUTTON_LEFT,
    INPUT_MOUSE_BUTTON_RIGHT = VD_INPUT_MOUSE_BUTTON_RIGHT,
    INPUT_MOUSE_BUTTON_MIDDLE = VD_INPUT_MOUSE_BUTTON_MIDDLE,
    INPUT_MOUSE_BUTTON_MAX = VD_INPUT_MOUSE_BUTTON_MAX,
#endif
};

enum {
    VD_INPUT_KEYBOARD_SCANCODE_UNKNOWN = 0,
    VD_INPUT_KEYBOARD_SCANCODE_A,      VD_INPUT_KEYBOARD_SCANCODE_B,
    VD_INPUT_KEYBOARD_SCANCODE_C,      VD_INPUT_KEYBOARD_SCANCODE_D,
    VD_INPUT_KEYBOARD_SCANCODE_E,      VD_INPUT_KEYBOARD_SCANCODE_F,
    VD_INPUT_KEYBOARD_SCANCODE_G,      VD_INPUT_KEYBOARD_SCANCODE_H,
    VD_INPUT_KEYBOARD_SCANCODE_I,      VD_INPUT_KEYBOARD_SCANCODE_J,
    VD_INPUT_KEYBOARD_SCANCODE_K,      VD_INPUT_KEYBOARD_SCANCODE_L,
    VD_INPUT_KEYBOARD_SCANCODE_M,      VD_INPUT_KEYBOARD_SCANCODE_N,
    VD_INPUT_KEYBOARD_SCANCODE_O,      VD_INPUT_KEYBOARD_SCANCODE_P,
    VD_INPUT_KEYBOARD_SCANCODE_Q,      VD_INPUT_KEYBOARD_SCANCODE_R,
    VD_INPUT_KEYBOARD_SCANCODE_S,      VD_INPUT_KEYBOARD_SCANCODE_T,
    VD_INPUT_KEYBOARD_SCANCODE_U,      VD_INPUT_KEYBOARD_SCANCODE_V,
    VD_INPUT_KEYBOARD_SCANCODE_W,      VD_INPUT_KEYBOARD_SCANCODE_X,
    VD_INPUT_KEYBOARD_SCANCODE_Y,      VD_INPUT_KEYBOARD_SCANCODE_Z,
    VD_INPUT_KEYBOARD_SCANCODE_SPACE,  VD_INPUT_KEYBOARD_SCANCODE_ESCAPE, VD_INPUT_KEYBOARD_SCANCODE_BACKSPACE,
    VD_INPUT_KEYBOARD_SCANCODE_F1,     VD_INPUT_KEYBOARD_SCANCODE_F2,
    VD_INPUT_KEYBOARD_SCANCODE_F3,     VD_INPUT_KEYBOARD_SCANCODE_F4,
    VD_INPUT_KEYBOARD_SCANCODE_F5,     VD_INPUT_KEYBOARD_SCANCODE_F6,
    VD_INPUT_KEYBOARD_SCANCODE_F7,     VD_INPUT_KEYBOARD_SCANCODE_F8,
    VD_INPUT_KEYBOARD_SCANCODE_F9,     VD_INPUT_KEYBOARD_SCANCODE_F10,
    VD_INPUT_KEYBOARD_SCANCODE_F11,    VD_INPUT_KEYBOARD_SCANCODE_F12,
    VD_INPUT_KEYBOARD_SCANCODE_F13,    VD_INPUT_KEYBOARD_SCANCODE_F14,
    VD_INPUT_KEYBOARD_SCANCODE_F15,    VD_INPUT_KEYBOARD_SCANCODE_F16,
    VD_INPUT_KEYBOARD_SCANCODE_F17,    VD_INPUT_KEYBOARD_SCANCODE_F18,
    VD_INPUT_KEYBOARD_SCANCODE_F19,    VD_INPUT_KEYBOARD_SCANCODE_F20,
    VD_INPUT_KEYBOARD_SCANCODE_F21,    VD_INPUT_KEYBOARD_SCANCODE_F22,
    VD_INPUT_KEYBOARD_SCANCODE_F23,    VD_INPUT_KEYBOARD_SCANCODE_F24,
    VD_INPUT_KEYBOARD_SCANCODE_MAX,
#if VD_MACRO_ABBREVIATIONS
    INPUT_KEYBOARD_SCANCODE_UNKNOWN = VD_INPUT_KEYBOARD_SCANCODE_UNKNOWN,
    INPUT_KEYBOARD_SCANCODE_A = VD_INPUT_KEYBOARD_SCANCODE_A,      INPUT_KEYBOARD_SCANCODE_B = VD_INPUT_KEYBOARD_SCANCODE_B,
    INPUT_KEYBOARD_SCANCODE_C = VD_INPUT_KEYBOARD_SCANCODE_C,      INPUT_KEYBOARD_SCANCODE_D = VD_INPUT_KEYBOARD_SCANCODE_D,
    INPUT_KEYBOARD_SCANCODE_E = VD_INPUT_KEYBOARD_SCANCODE_E,      INPUT_KEYBOARD_SCANCODE_F = VD_INPUT_KEYBOARD_SCANCODE_F,
    INPUT_KEYBOARD_SCANCODE_G = VD_INPUT_KEYBOARD_SCANCODE_G,      INPUT_KEYBOARD_SCANCODE_H = VD_INPUT_KEYBOARD_SCANCODE_H,
    INPUT_KEYBOARD_SCANCODE_I = VD_INPUT_KEYBOARD_SCANCODE_I,      INPUT_KEYBOARD_SCANCODE_J = VD_INPUT_KEYBOARD_SCANCODE_J,
    INPUT_KEYBOARD_SCANCODE_K = VD_INPUT_KEYBOARD_SCANCODE_K,      INPUT_KEYBOARD_SCANCODE_L = VD_INPUT_KEYBOARD_SCANCODE_L,
    INPUT_KEYBOARD_SCANCODE_M = VD_INPUT_KEYBOARD_SCANCODE_M,      INPUT_KEYBOARD_SCANCODE_N = VD_INPUT_KEYBOARD_SCANCODE_N,
    INPUT_KEYBOARD_SCANCODE_O = VD_INPUT_KEYBOARD_SCANCODE_O,      INPUT_KEYBOARD_SCANCODE_P = VD_INPUT_KEYBOARD_SCANCODE_P,
    INPUT_KEYBOARD_SCANCODE_Q = VD_INPUT_KEYBOARD_SCANCODE_Q,      INPUT_KEYBOARD_SCANCODE_R = VD_INPUT_KEYBOARD_SCANCODE_R,
    INPUT_KEYBOARD_SCANCODE_S = VD_INPUT_KEYBOARD_SCANCODE_S,      INPUT_KEYBOARD_SCANCODE_T = VD_INPUT_KEYBOARD_SCANCODE_T,
    INPUT_KEYBOARD_SCANCODE_U = VD_INPUT_KEYBOARD_SCANCODE_U,      INPUT_KEYBOARD_SCANCODE_V = VD_INPUT_KEYBOARD_SCANCODE_V,
    INPUT_KEYBOARD_SCANCODE_W = VD_INPUT_KEYBOARD_SCANCODE_W,      INPUT_KEYBOARD_SCANCODE_X = VD_INPUT_KEYBOARD_SCANCODE_X,
    INPUT_KEYBOARD_SCANCODE_Y = VD_INPUT_KEYBOARD_SCANCODE_Y,      INPUT_KEYBOARD_SCANCODE_Z = VD_INPUT_KEYBOARD_SCANCODE_Z,
    INPUT_KEYBOARD_SCANCODE_SPACE = VD_INPUT_KEYBOARD_SCANCODE_SPACE,  INPUT_KEYBOARD_SCANCODE_ESCAPE = VD_INPUT_KEYBOARD_SCANCODE_ESCAPE, INPUT_KEYBOARD_SCANCODE_BACKSPACE = VD_INPUT_KEYBOARD_SCANCODE_BACKSPACE,
    INPUT_KEYBOARD_SCANCODE_F1 = VD_INPUT_KEYBOARD_SCANCODE_F1,     INPUT_KEYBOARD_SCANCODE_F2 = VD_INPUT_KEYBOARD_SCANCODE_F2,
    INPUT_KEYBOARD_SCANCODE_F3 = VD_INPUT_KEYBOARD_SCANCODE_F3,     INPUT_KEYBOARD_SCANCODE_F4 = VD_INPUT_KEYBOARD_SCANCODE_F4,
    INPUT_KEYBOARD_SCANCODE_F5 = VD_INPUT_KEYBOARD_SCANCODE_F5,     INPUT_KEYBOARD_SCANCODE_F6 = VD_INPUT_KEYBOARD_SCANCODE_F6,
    INPUT_KEYBOARD_SCANCODE_F7 = VD_INPUT_KEYBOARD_SCANCODE_F7,     INPUT_KEYBOARD_SCANCODE_F8 = VD_INPUT_KEYBOARD_SCANCODE_F8,
    INPUT_KEYBOARD_SCANCODE_F9 = VD_INPUT_KEYBOARD_SCANCODE_F9,     INPUT_KEYBOARD_SCANCODE_F10 = VD_INPUT_KEYBOARD_SCANCODE_F10,
    INPUT_KEYBOARD_SCANCODE_F11 = VD_INPUT_KEYBOARD_SCANCODE_F11,    INPUT_KEYBOARD_SCANCODE_F12 = VD_INPUT_KEYBOARD_SCANCODE_F12,
    INPUT_KEYBOARD_SCANCODE_F13 = VD_INPUT_KEYBOARD_SCANCODE_F13,    INPUT_KEYBOARD_SCANCODE_F14 = VD_INPUT_KEYBOARD_SCANCODE_F14,
    INPUT_KEYBOARD_SCANCODE_F15 = VD_INPUT_KEYBOARD_SCANCODE_F15,    INPUT_KEYBOARD_SCANCODE_F16 = VD_INPUT_KEYBOARD_SCANCODE_F16,
    INPUT_KEYBOARD_SCANCODE_F17 = VD_INPUT_KEYBOARD_SCANCODE_F17,    INPUT_KEYBOARD_SCANCODE_F18 = VD_INPUT_KEYBOARD_SCANCODE_F18,
    INPUT_KEYBOARD_SCANCODE_F19 = VD_INPUT_KEYBOARD_SCANCODE_F19,    INPUT_KEYBOARD_SCANCODE_F20 = VD_INPUT_KEYBOARD_SCANCODE_F20,
    INPUT_KEYBOARD_SCANCODE_F21 = VD_INPUT_KEYBOARD_SCANCODE_F21,    INPUT_KEYBOARD_SCANCODE_F22 = VD_INPUT_KEYBOARD_SCANCODE_F22,
    INPUT_KEYBOARD_SCANCODE_F23 = VD_INPUT_KEYBOARD_SCANCODE_F23,    INPUT_KEYBOARD_SCANCODE_F24 = VD_INPUT_KEYBOARD_SCANCODE_F24,
    INPUT_KEYBOARD_SCANCODE_MAX = VD_INPUT_KEYBOARD_SCANCODE_MAX,
#endif
};

enum {
    VD_INPUT_KEYBOARD_MODIFIER_NONE = 0,
};

typedef struct __VD_Input_KeyboardState {
    Vdb32 states[VD_INPUT_KEYBOARD_SCANCODE_MAX];
} VdInputKeyboardState;

typedef struct __VD_Input_KeyboardStateGetConstantKeyDownEventsInfo {
    VdArena *arena;
    struct {
        Vdu32        *num_events;
        VdInputEvent **events;
    } result;
} VdInputKeyboardStateGetConstantKeyDownEventsInfo;

VdStr vd_input_keyboard_scancode_get_name                  (VdInputKeyboardScancode scancode);
void  vd_input_keyboard_state_init                         (VdInputKeyboardState *kbstate);
void  vd_input_keyboard_state_send_event                   (VdInputKeyboardState *kbstate, VdInputEvent *evt);
void  vd_input_keyboard_state_get_constant_key_down_events (VdInputKeyboardState *kbstate, VdInputKeyboardStateGetConstantKeyDownEventsInfo *info);

#if VD_PLATFORM_MACOS
VdInputKeyboardScancode vd_input_macos_nskeycode_to_keyboard_scancode(unsigned short nskeycode);
#endif // VD_PLATFORM_MACOS

#if VD_PLATFORM_WINDOWS
VdInputKeyboardScancode vd_input_windows_vkcode_to_keyboard_scancode(unsigned int vkcode);
#endif // VD_PLATFORM_WINDOWS

Vdi32 vd_input_keyboard_scancode_to_imgui_key(VdInputKeyboardScancode scancode);
Vdi32 vd_input_mouse_button_to_imgui_mouse_button(VdInputMouseButton button);

#if VD_MACRO_ABBREVIATIONS
#define InputKeyboardScancode                               VdInputKeyboardScancode
#define InputMouseButton                                    VdInputMouseButton
#define InputKeyboardModifier                               VdInputKeyboardModifier
#define InputKeyboardStateGetConstantKeyDownEventsInfo      VdInputKeyboardStateGetConstantKeyDownEventsInfo
#define InputKeyboardState                                  VdInputKeyboardState
#define InputKeyStateFlags                                  VdInputKeyStateFlags
#define InputEventData                                      VdInputEventData
#define InputEventType                                      VdInputEventType
#define InputEvent                                          VdInputEvent
#define input_keyboard_scancode_get_name                    vd_input_keyboard_scancode_get_name 
#define input_keyboard_state_init                           vd_input_keyboard_state_init 
#define input_keyboard_state_send_event                     vd_input_keyboard_state_send_event 
#define input_keyboard_state_get_constant_key_down_events   vd_input_keyboard_state_get_constant_key_down_events 
#define input_macos_nskeycode_to_keyboard_scancode          vd_input_macos_nskeycode_to_keyboard_scancode 
#define input_windows_vkcode_to_keyboard_scancode           vd_input_windows_vkcode_to_keyboard_scancode 
#define input_keyboard_scancode_to_imgui_key                vd_input_keyboard_scancode_to_imgui_key 
#define input_mouse_button_to_imgui_mouse_button            vd_input_mouse_button_to_imgui_mouse_button 
#endif // VD_MACRO_ABBREVIATIONS

#endif // !VD_INPUT_H

#ifdef VD_INPUT_IMPL
static Str Vd__Input_Scancode_To_Name[] = {
    [VD_INPUT_KEYBOARD_SCANCODE_UNKNOWN]    = VD_LIT_INLINE("Unknown"),
    [VD_INPUT_KEYBOARD_SCANCODE_A]          = VD_LIT_INLINE("a"),
    [VD_INPUT_KEYBOARD_SCANCODE_B]          = VD_LIT_INLINE("b"),
    [VD_INPUT_KEYBOARD_SCANCODE_C]          = VD_LIT_INLINE("c"),
    [VD_INPUT_KEYBOARD_SCANCODE_D]          = VD_LIT_INLINE("d"),
    [VD_INPUT_KEYBOARD_SCANCODE_E]          = VD_LIT_INLINE("e"),
    [VD_INPUT_KEYBOARD_SCANCODE_F]          = VD_LIT_INLINE("f"),
    [VD_INPUT_KEYBOARD_SCANCODE_G]          = VD_LIT_INLINE("g"),
    [VD_INPUT_KEYBOARD_SCANCODE_H]          = VD_LIT_INLINE("h"),
    [VD_INPUT_KEYBOARD_SCANCODE_I]          = VD_LIT_INLINE("i"),
    [VD_INPUT_KEYBOARD_SCANCODE_J]          = VD_LIT_INLINE("j"),
    [VD_INPUT_KEYBOARD_SCANCODE_K]          = VD_LIT_INLINE("k"),
    [VD_INPUT_KEYBOARD_SCANCODE_L]          = VD_LIT_INLINE("l"),
    [VD_INPUT_KEYBOARD_SCANCODE_M]          = VD_LIT_INLINE("m"),
    [VD_INPUT_KEYBOARD_SCANCODE_N]          = VD_LIT_INLINE("n"),
    [VD_INPUT_KEYBOARD_SCANCODE_O]          = VD_LIT_INLINE("o"),
    [VD_INPUT_KEYBOARD_SCANCODE_P]          = VD_LIT_INLINE("p"),
    [VD_INPUT_KEYBOARD_SCANCODE_Q]          = VD_LIT_INLINE("q"),
    [VD_INPUT_KEYBOARD_SCANCODE_R]          = VD_LIT_INLINE("r"),
    [VD_INPUT_KEYBOARD_SCANCODE_S]          = VD_LIT_INLINE("s"),
    [VD_INPUT_KEYBOARD_SCANCODE_T]          = VD_LIT_INLINE("t"),
    [VD_INPUT_KEYBOARD_SCANCODE_U]          = VD_LIT_INLINE("u"),
    [VD_INPUT_KEYBOARD_SCANCODE_V]          = VD_LIT_INLINE("v"),
    [VD_INPUT_KEYBOARD_SCANCODE_W]          = VD_LIT_INLINE("w"),
    [VD_INPUT_KEYBOARD_SCANCODE_X]          = VD_LIT_INLINE("x"),
    [VD_INPUT_KEYBOARD_SCANCODE_Y]          = VD_LIT_INLINE("y"),
    [VD_INPUT_KEYBOARD_SCANCODE_Z]          = VD_LIT_INLINE("z"),
    [VD_INPUT_KEYBOARD_SCANCODE_SPACE]      = VD_LIT_INLINE("Spacebar"),
    [VD_INPUT_KEYBOARD_SCANCODE_ESCAPE]     = VD_LIT_INLINE("Escape"),  
    [VD_INPUT_KEYBOARD_SCANCODE_BACKSPACE]  = VD_LIT_INLINE("Backspace"),
    [VD_INPUT_KEYBOARD_SCANCODE_F1]         = VD_LIT_INLINE("F1"),
    [VD_INPUT_KEYBOARD_SCANCODE_F2]         = VD_LIT_INLINE("F2"),
    [VD_INPUT_KEYBOARD_SCANCODE_F3]         = VD_LIT_INLINE("F3"),
    [VD_INPUT_KEYBOARD_SCANCODE_F4]         = VD_LIT_INLINE("F4"),
    [VD_INPUT_KEYBOARD_SCANCODE_F5]         = VD_LIT_INLINE("F5"),
    [VD_INPUT_KEYBOARD_SCANCODE_F6]         = VD_LIT_INLINE("F6"),
    [VD_INPUT_KEYBOARD_SCANCODE_F7]         = VD_LIT_INLINE("F7"),
    [VD_INPUT_KEYBOARD_SCANCODE_F8]         = VD_LIT_INLINE("F8"),
    [VD_INPUT_KEYBOARD_SCANCODE_F9]         = VD_LIT_INLINE("F9"),
    [VD_INPUT_KEYBOARD_SCANCODE_F10]        = VD_LIT_INLINE("F10"),
    [VD_INPUT_KEYBOARD_SCANCODE_F11]        = VD_LIT_INLINE("F11"),
    [VD_INPUT_KEYBOARD_SCANCODE_F12]        = VD_LIT_INLINE("F12"),
    [VD_INPUT_KEYBOARD_SCANCODE_F13]        = VD_LIT_INLINE("F13"),
    [VD_INPUT_KEYBOARD_SCANCODE_F14]        = VD_LIT_INLINE("F14"),
    [VD_INPUT_KEYBOARD_SCANCODE_F15]        = VD_LIT_INLINE("F15"),
    [VD_INPUT_KEYBOARD_SCANCODE_F16]        = VD_LIT_INLINE("F16"),
    [VD_INPUT_KEYBOARD_SCANCODE_F17]        = VD_LIT_INLINE("F17"),
    [VD_INPUT_KEYBOARD_SCANCODE_F18]        = VD_LIT_INLINE("F18"),
    [VD_INPUT_KEYBOARD_SCANCODE_F19]        = VD_LIT_INLINE("F19"),
    [VD_INPUT_KEYBOARD_SCANCODE_F20]        = VD_LIT_INLINE("F20"),
    [VD_INPUT_KEYBOARD_SCANCODE_F21]        = VD_LIT_INLINE("F21"),
    [VD_INPUT_KEYBOARD_SCANCODE_F22]        = VD_LIT_INLINE("F22"),
    [VD_INPUT_KEYBOARD_SCANCODE_F23]        = VD_LIT_INLINE("F23"),
    [VD_INPUT_KEYBOARD_SCANCODE_F24]        = VD_LIT_INLINE("F24"),
};

VdStr vd_input_keyboard_scancode_get_name(VdInputKeyboardScancode scancode)
{
    return Vd__Input_Scancode_To_Name[scancode];
}

void vd_input_keyboard_state_init(VdInputKeyboardState *kbstate)
{
    VD_MEMSET(kbstate->states, 0, sizeof(Vdb32) * VD_INPUT_KEYBOARD_SCANCODE_MAX);
}

void vd_input_keyboard_state_send_event(VdInputKeyboardState *kbstate, VdInputEvent *evt)
{
    if ((evt->type != VD_INPUT_EVENT_TYPE_KEY_DOWN) && (evt->type != VD_INPUT_EVENT_TYPE_KEY_UP)) {
        return;
    }

    kbstate->states[evt->data.key.scancode] = evt->type == VD_INPUT_EVENT_TYPE_KEY_DOWN;
}

void vd_input_keyboard_state_get_constant_key_down_events(VdInputKeyboardState *kbstate, VdInputKeyboardStateGetConstantKeyDownEventsInfo *info)
{
    VD_DYNARRAY VdInputEvent *events = 0;
    VD_DYNARRAY_INIT(events, info->arena);

    for (u32 i = 0; i < VD_INPUT_KEYBOARD_SCANCODE_MAX; ++i) {
        if (kbstate->states[i]) {
            VdInputEvent *evt = VD_DYNARRAY_PUSH(events);
            *evt = (VdInputEvent) {
                .type = VD_INPUT_EVENT_TYPE_KEY_DOWN,
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
VdInputKeyboardScancode vd_input_macos_nskeycode_to_keyboard_scancode(unsigned short nskeycode)
{
    // @note(mdodis): Special thanks to https://gist.github.com/michaelgiraldo/c3ca98f06aeb20acae1fd8d82fab0811

    switch (nskeycode) {
        case 0:   return VD_INPUT_KEYBOARD_SCANCODE_A;
        case 11:  return VD_INPUT_KEYBOARD_SCANCODE_B;
        case 8:   return VD_INPUT_KEYBOARD_SCANCODE_C;
        case 2:   return VD_INPUT_KEYBOARD_SCANCODE_D;
        case 14:  return VD_INPUT_KEYBOARD_SCANCODE_E;
        case 3:   return VD_INPUT_KEYBOARD_SCANCODE_F;
        case 5:   return VD_INPUT_KEYBOARD_SCANCODE_G;
        case 4:   return VD_INPUT_KEYBOARD_SCANCODE_H;
        case 34:  return VD_INPUT_KEYBOARD_SCANCODE_I;
        case 38:  return VD_INPUT_KEYBOARD_SCANCODE_J;
        case 40:  return VD_INPUT_KEYBOARD_SCANCODE_K;
        case 37:  return VD_INPUT_KEYBOARD_SCANCODE_L;
        case 46:  return VD_INPUT_KEYBOARD_SCANCODE_M;
        case 45:  return VD_INPUT_KEYBOARD_SCANCODE_N;
        case 31:  return VD_INPUT_KEYBOARD_SCANCODE_O;
        case 35:  return VD_INPUT_KEYBOARD_SCANCODE_P;
        case 12:  return VD_INPUT_KEYBOARD_SCANCODE_Q;
        case 15:  return VD_INPUT_KEYBOARD_SCANCODE_R;
        case 1:   return VD_INPUT_KEYBOARD_SCANCODE_S;
        case 17:  return VD_INPUT_KEYBOARD_SCANCODE_T;
        case 32:  return VD_INPUT_KEYBOARD_SCANCODE_U;
        case 9:   return VD_INPUT_KEYBOARD_SCANCODE_V;
        case 13:  return VD_INPUT_KEYBOARD_SCANCODE_W;
        case 7:   return VD_INPUT_KEYBOARD_SCANCODE_X;
        case 16:  return VD_INPUT_KEYBOARD_SCANCODE_Y;
        case 6:   return VD_INPUT_KEYBOARD_SCANCODE_Z;
        case 53:  return VD_INPUT_KEYBOARD_SCANCODE_ESCAPE;
        case 49:  return VD_INPUT_KEYBOARD_SCANCODE_SPACE;
        case 51:  return VD_INPUT_KEYBOARD_SCANCODE_BACKSPACE;
        case 122: return VD_INPUT_KEYBOARD_SCANCODE_F1;
        case 120: return VD_INPUT_KEYBOARD_SCANCODE_F2;
        case 99:  return VD_INPUT_KEYBOARD_SCANCODE_F3;
        case 118: return VD_INPUT_KEYBOARD_SCANCODE_F4;
        case 96:  return VD_INPUT_KEYBOARD_SCANCODE_F5;
        case 97:  return VD_INPUT_KEYBOARD_SCANCODE_F6;
        case 98:  return VD_INPUT_KEYBOARD_SCANCODE_F7;
        case 100: return VD_INPUT_KEYBOARD_SCANCODE_F8;
        case 101: return VD_INPUT_KEYBOARD_SCANCODE_F9;
        case 109: return VD_INPUT_KEYBOARD_SCANCODE_F10;
        case 103: return VD_INPUT_KEYBOARD_SCANCODE_F11;
        case 111: return VD_INPUT_KEYBOARD_SCANCODE_F12;
        case 105: return VD_INPUT_KEYBOARD_SCANCODE_F13;
        case 107: return VD_INPUT_KEYBOARD_SCANCODE_F14;
        case 113: return VD_INPUT_KEYBOARD_SCANCODE_F15;
        case 106: return VD_INPUT_KEYBOARD_SCANCODE_F16;
        case 64:  return VD_INPUT_KEYBOARD_SCANCODE_F17;
        case 79:  return VD_INPUT_KEYBOARD_SCANCODE_F18;
        case 90:  return VD_INPUT_KEYBOARD_SCANCODE_F19;
        default:  return VD_INPUT_KEYBOARD_SCANCODE_UNKNOWN;
    }
}

#endif // VD_PLATFORM_MACOS

#if VD_PLATFORM_WINDOWS
VdInputKeyboardScancode vd_input_windows_vkcode_to_keyboard_scancode(unsigned int vkcode)
{
    switch (vkcode)
    {
        case 'A':  return VD_INPUT_KEYBOARD_SCANCODE_A;
        case 'B':  return VD_INPUT_KEYBOARD_SCANCODE_B;
        case 'C':  return VD_INPUT_KEYBOARD_SCANCODE_C;
        case 'D':  return VD_INPUT_KEYBOARD_SCANCODE_D;
        case 'E':  return VD_INPUT_KEYBOARD_SCANCODE_E;
        case 'F':  return VD_INPUT_KEYBOARD_SCANCODE_F;
        case 'G':  return VD_INPUT_KEYBOARD_SCANCODE_G;
        case 'H':  return VD_INPUT_KEYBOARD_SCANCODE_H;
        case 'I':  return VD_INPUT_KEYBOARD_SCANCODE_I;
        case 'J':  return VD_INPUT_KEYBOARD_SCANCODE_J;
        case 'K':  return VD_INPUT_KEYBOARD_SCANCODE_K;
        case 'L':  return VD_INPUT_KEYBOARD_SCANCODE_L;
        case 'M':  return VD_INPUT_KEYBOARD_SCANCODE_M;
        case 'N':  return VD_INPUT_KEYBOARD_SCANCODE_N;
        case 'O':  return VD_INPUT_KEYBOARD_SCANCODE_O;
        case 'P':  return VD_INPUT_KEYBOARD_SCANCODE_P;
        case 'Q':  return VD_INPUT_KEYBOARD_SCANCODE_Q;
        case 'R':  return VD_INPUT_KEYBOARD_SCANCODE_R;
        case 'S':  return VD_INPUT_KEYBOARD_SCANCODE_S;
        case 'T':  return VD_INPUT_KEYBOARD_SCANCODE_T;
        case 'U':  return VD_INPUT_KEYBOARD_SCANCODE_U;
        case 'V':  return VD_INPUT_KEYBOARD_SCANCODE_V;
        case 'W':  return VD_INPUT_KEYBOARD_SCANCODE_W;
        case 'X':  return VD_INPUT_KEYBOARD_SCANCODE_X;
        case 'Y':  return VD_INPUT_KEYBOARD_SCANCODE_Y;
        case 'Z':  return VD_INPUT_KEYBOARD_SCANCODE_Z;
        case 0x1b: return VD_INPUT_KEYBOARD_SCANCODE_ESCAPE;
        case 0x20: return VD_INPUT_KEYBOARD_SCANCODE_SPACE;
        case 0x08: return VD_INPUT_KEYBOARD_SCANCODE_BACKSPACE;
        case 0x70: return VD_INPUT_KEYBOARD_SCANCODE_F1;
        case 0x71: return VD_INPUT_KEYBOARD_SCANCODE_F2;
        case 0x72: return VD_INPUT_KEYBOARD_SCANCODE_F3;
        case 0x73: return VD_INPUT_KEYBOARD_SCANCODE_F4;
        case 0x74: return VD_INPUT_KEYBOARD_SCANCODE_F5;
        case 0x75: return VD_INPUT_KEYBOARD_SCANCODE_F6;
        case 0x76: return VD_INPUT_KEYBOARD_SCANCODE_F7;
        case 0x77: return VD_INPUT_KEYBOARD_SCANCODE_F8;
        case 0x78: return VD_INPUT_KEYBOARD_SCANCODE_F9;
        case 0x79: return VD_INPUT_KEYBOARD_SCANCODE_F10;
        case 0x7a: return VD_INPUT_KEYBOARD_SCANCODE_F11;
        case 0x7b: return VD_INPUT_KEYBOARD_SCANCODE_F12;
        case 0x7c: return VD_INPUT_KEYBOARD_SCANCODE_F13;
        case 0x7d: return VD_INPUT_KEYBOARD_SCANCODE_F14;
        case 0x7e: return VD_INPUT_KEYBOARD_SCANCODE_F15;
        case 0x7f: return VD_INPUT_KEYBOARD_SCANCODE_F16;
        case 0x80: return VD_INPUT_KEYBOARD_SCANCODE_F17;
        case 0x81: return VD_INPUT_KEYBOARD_SCANCODE_F18;
        case 0x82: return VD_INPUT_KEYBOARD_SCANCODE_F19;
        case 0x83: return VD_INPUT_KEYBOARD_SCANCODE_F20;
        case 0x84: return VD_INPUT_KEYBOARD_SCANCODE_F21;
        case 0x85: return VD_INPUT_KEYBOARD_SCANCODE_F22;
        case 0x86: return VD_INPUT_KEYBOARD_SCANCODE_F23;
        case 0x87: return VD_INPUT_KEYBOARD_SCANCODE_F24;
        default: return VD_INPUT_KEYBOARD_SCANCODE_UNKNOWN;
    }
}
#endif // VD_PLATFORM_WINDOWS

enum {
    // Keyboard
    InternalImGuiKey_None = 0,
    InternalImGuiKey_NamedKey_BEGIN = 512,  // First valid key value (other than 0)

    InternalImGuiKey_Tab = 512,             // == InternalImGuiKey_NamedKey_BEGIN
    InternalImGuiKey_LeftArrow,
    InternalImGuiKey_RightArrow,
    InternalImGuiKey_UpArrow,
    InternalImGuiKey_DownArrow,
    InternalImGuiKey_PageUp,
    InternalImGuiKey_PageDown,
    InternalImGuiKey_Home,
    InternalImGuiKey_End,
    InternalImGuiKey_Insert,
    InternalImGuiKey_Delete,
    InternalImGuiKey_Backspace,
    InternalImGuiKey_Space,
    InternalImGuiKey_Enter,
    InternalImGuiKey_Escape,
    InternalImGuiKey_LeftCtrl, InternalImGuiKey_LeftShift, InternalImGuiKey_LeftAlt, InternalImGuiKey_LeftSuper,     // Also see InternalImGuiMod_Ctrl, InternalImGuiMod_Shift, InternalImGuiMod_Alt, InternalImGuiMod_Super below!
    InternalImGuiKey_RightCtrl, InternalImGuiKey_RightShift, InternalImGuiKey_RightAlt, InternalImGuiKey_RightSuper,
    InternalImGuiKey_Menu,
    InternalImGuiKey_0, InternalImGuiKey_1, InternalImGuiKey_2, InternalImGuiKey_3, InternalImGuiKey_4, InternalImGuiKey_5, InternalImGuiKey_6, InternalImGuiKey_7, InternalImGuiKey_8, InternalImGuiKey_9,
    InternalImGuiKey_A, InternalImGuiKey_B, InternalImGuiKey_C, InternalImGuiKey_D, InternalImGuiKey_E, InternalImGuiKey_F, InternalImGuiKey_G, InternalImGuiKey_H, InternalImGuiKey_I, InternalImGuiKey_J,
    InternalImGuiKey_K, InternalImGuiKey_L, InternalImGuiKey_M, InternalImGuiKey_N, InternalImGuiKey_O, InternalImGuiKey_P, InternalImGuiKey_Q, InternalImGuiKey_R, InternalImGuiKey_S, InternalImGuiKey_T,
    InternalImGuiKey_U, InternalImGuiKey_V, InternalImGuiKey_W, InternalImGuiKey_X, InternalImGuiKey_Y, InternalImGuiKey_Z,
    InternalImGuiKey_F1, InternalImGuiKey_F2, InternalImGuiKey_F3, InternalImGuiKey_F4, InternalImGuiKey_F5, InternalImGuiKey_F6,
    InternalImGuiKey_F7, InternalImGuiKey_F8, InternalImGuiKey_F9, InternalImGuiKey_F10, InternalImGuiKey_F11, InternalImGuiKey_F12,
    InternalImGuiKey_F13, InternalImGuiKey_F14, InternalImGuiKey_F15, InternalImGuiKey_F16, InternalImGuiKey_F17, InternalImGuiKey_F18,
    InternalImGuiKey_F19, InternalImGuiKey_F20, InternalImGuiKey_F21, InternalImGuiKey_F22, InternalImGuiKey_F23, InternalImGuiKey_F24,
    InternalImGuiKey_Apostrophe,        // '
    InternalImGuiKey_Comma,             // ,
    InternalImGuiKey_Minus,             // -
    InternalImGuiKey_Period,            // .
    InternalImGuiKey_Slash,             // /
    InternalImGuiKey_Semicolon,         // ;
    InternalImGuiKey_Equal,             // =
    InternalImGuiKey_LeftBracket,       // [
    InternalImGuiKey_Backslash,         // \ (this text inhibit multiline comment caused by backslash)
    InternalImGuiKey_RightBracket,      // ]
    InternalImGuiKey_GraveAccent,       // `
    InternalImGuiKey_CapsLock,
    InternalImGuiKey_ScrollLock,
    InternalImGuiKey_NumLock,
    InternalImGuiKey_PrintScreen,
    InternalImGuiKey_Pause,
    InternalImGuiKey_Keypad0, InternalImGuiKey_Keypad1, InternalImGuiKey_Keypad2, InternalImGuiKey_Keypad3, InternalImGuiKey_Keypad4,
    InternalImGuiKey_Keypad5, InternalImGuiKey_Keypad6, InternalImGuiKey_Keypad7, InternalImGuiKey_Keypad8, InternalImGuiKey_Keypad9,
    InternalImGuiKey_KeypadDecimal,
    InternalImGuiKey_KeypadDivide,
    InternalImGuiKey_KeypadMultiply,
    InternalImGuiKey_KeypadSubtract,
    InternalImGuiKey_KeypadAdd,
    InternalImGuiKey_KeypadEnter,
    InternalImGuiKey_KeypadEqual,
    InternalImGuiKey_AppBack,               // Available on some keyboard/mouses. Often referred as "Browser Back"
    InternalImGuiKey_AppForward,
    InternalImGuiKey_Oem102,                // Non-US backslash.

    // Gamepad
    // (analog values are 0.0f to 1.0f)
    // (download controller mapping PNG/PSD at http://dearimgui.com/controls_sheets)
    //                              // XBOX        | SWITCH  | PLAYSTA. | -> ACTION
    InternalImGuiKey_GamepadStart,          // Menu        | +       | Options  |
    InternalImGuiKey_GamepadBack,           // View        | -       | Share    |
    InternalImGuiKey_GamepadFaceLeft,       // X           | Y       | Square   | Tap: Toggle Menu. Hold: Windowing mode (Focus/Move/Resize windows)
    InternalImGuiKey_GamepadFaceRight,      // B           | A       | Circle   | Cancel / Close / Exit
    InternalImGuiKey_GamepadFaceUp,         // Y           | X       | Triangle | Text Input / On-screen Keyboard
    InternalImGuiKey_GamepadFaceDown,       // A           | B       | Cross    | Activate / Open / Toggle / Tweak
    InternalImGuiKey_GamepadDpadLeft,       // D-pad Left  | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    InternalImGuiKey_GamepadDpadRight,      // D-pad Right | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    InternalImGuiKey_GamepadDpadUp,         // D-pad Up    | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    InternalImGuiKey_GamepadDpadDown,       // D-pad Down  | "       | "        | Move / Tweak / Resize Window (in Windowing mode)
    InternalImGuiKey_GamepadL1,             // L Bumper    | L       | L1       | Tweak Slower / Focus Previous (in Windowing mode)
    InternalImGuiKey_GamepadR1,             // R Bumper    | R       | R1       | Tweak Faster / Focus Next (in Windowing mode)
    InternalImGuiKey_GamepadL2,             // L Trigger   | ZL      | L2       | [Analog]
    InternalImGuiKey_GamepadR2,             // R Trigger   | ZR      | R2       | [Analog]
    InternalImGuiKey_GamepadL3,             // L Stick     | L3      | L3       |
    InternalImGuiKey_GamepadR3,             // R Stick     | R3      | R3       |
    InternalImGuiKey_GamepadLStickLeft,     //             |         |          | [Analog] Move Window (in Windowing mode)
    InternalImGuiKey_GamepadLStickRight,    //             |         |          | [Analog] Move Window (in Windowing mode)
    InternalImGuiKey_GamepadLStickUp,       //             |         |          | [Analog] Move Window (in Windowing mode)
    InternalImGuiKey_GamepadLStickDown,     //             |         |          | [Analog] Move Window (in Windowing mode)
    InternalImGuiKey_GamepadRStickLeft,     //             |         |          | [Analog]
    InternalImGuiKey_GamepadRStickRight,    //             |         |          | [Analog]
    InternalImGuiKey_GamepadRStickUp,       //             |         |          | [Analog]
    InternalImGuiKey_GamepadRStickDown,     //             |         |          | [Analog]

    // Aliases: Mouse Buttons (auto-submitted from AddMouseButtonEvent() calls)
    // - This is mirroring the data also written to io.MouseDown[], io.MouseWheel, in a format allowing them to be accessed via standard key API.
    InternalImGuiKey_MouseLeft, InternalImGuiKey_MouseRight, InternalImGuiKey_MouseMiddle, InternalImGuiKey_MouseX1, InternalImGuiKey_MouseX2, InternalImGuiKey_MouseWheelX, InternalImGuiKey_MouseWheelY,

    // [Internal] Reserved for mod storage
    InternalImGuiKey_ReservedForModCtrl, InternalImGuiKey_ReservedForModShift, InternalImGuiKey_ReservedForModAlt, InternalImGuiKey_ReservedForModSuper,

    // [Internal] If you need to iterate all keys (for e.g. an input mapper) you may use InternalImGuiKey_NamedKey_BEGIN..InternalImGuiKey_NamedKey_END.
    InternalImGuiKey_NamedKey_END,
    InternalImGuiKey_NamedKey_COUNT = InternalImGuiKey_NamedKey_END - InternalImGuiKey_NamedKey_BEGIN,

    // Keyboard Modifiers (explicitly submitted by backend via AddKeyEvent() calls)
    // - Any functions taking a InternalImGuiKeyChord parameter can binary-or those with regular keys, e.g. Shortcut(InternalImGuiMod_Ctrl | InternalImGuiKey_S).
    // - Those are written back into io.KeyCtrl, io.KeyShift, io.KeyAlt, io.KeySuper for convenience,
    //   but may be accessed via standard key API such as IsKeyPressed(), IsKeyReleased(), querying duration etc.
    // - Code polling every key (e.g. an interface to detect a key press for input mapping) might want to ignore those
    //   and prefer using the real keys (e.g. InternalImGuiKey_LeftCtrl, InternalImGuiKey_RightCtrl instead of InternalImGuiMod_Ctrl).
    // - In theory the value of keyboard modifiers should be roughly equivalent to a logical or of the equivalent left/right keys.
    //   In practice: it's complicated; mods are often provided from different sources. Keyboard layout, IME, sticky keys and
    //   backends tend to interfere and break that equivalence. The safer decision is to relay that ambiguity down to the end-user...
    // - On macOS, we swap Cmd(Super) and Ctrl keys at the time of the io.AddKeyEvent() call.
    InternalImGuiMod_None                   = 0,
    InternalImGuiMod_Ctrl                   = 1 << 12, // Ctrl (non-macOS), Cmd (macOS)
    InternalImGuiMod_Shift                  = 1 << 13, // Shift
    InternalImGuiMod_Alt                    = 1 << 14, // Option/Menu
    InternalImGuiMod_Super                  = 1 << 15, // Windows/Super (non-macOS), Ctrl (macOS)
    InternalImGuiMod_Mask_                  = 0xF000,  // 4-bits
};

Vdi32 vd_input_keyboard_scancode_to_imgui_key(VdInputKeyboardScancode scancode)
{
    switch (scancode) {
        case VD_INPUT_KEYBOARD_SCANCODE_A:         return InternalImGuiKey_A;
        case VD_INPUT_KEYBOARD_SCANCODE_B:         return InternalImGuiKey_B;
        case VD_INPUT_KEYBOARD_SCANCODE_C:         return InternalImGuiKey_C;
        case VD_INPUT_KEYBOARD_SCANCODE_D:         return InternalImGuiKey_D;
        case VD_INPUT_KEYBOARD_SCANCODE_E:         return InternalImGuiKey_E;
        case VD_INPUT_KEYBOARD_SCANCODE_F:         return InternalImGuiKey_F;
        case VD_INPUT_KEYBOARD_SCANCODE_G:         return InternalImGuiKey_G;
        case VD_INPUT_KEYBOARD_SCANCODE_H:         return InternalImGuiKey_H;
        case VD_INPUT_KEYBOARD_SCANCODE_I:         return InternalImGuiKey_I;
        case VD_INPUT_KEYBOARD_SCANCODE_J:         return InternalImGuiKey_J;
        case VD_INPUT_KEYBOARD_SCANCODE_K:         return InternalImGuiKey_K;
        case VD_INPUT_KEYBOARD_SCANCODE_L:         return InternalImGuiKey_L;
        case VD_INPUT_KEYBOARD_SCANCODE_M:         return InternalImGuiKey_M;
        case VD_INPUT_KEYBOARD_SCANCODE_N:         return InternalImGuiKey_N;
        case VD_INPUT_KEYBOARD_SCANCODE_O:         return InternalImGuiKey_O;
        case VD_INPUT_KEYBOARD_SCANCODE_P:         return InternalImGuiKey_P;
        case VD_INPUT_KEYBOARD_SCANCODE_Q:         return InternalImGuiKey_Q;
        case VD_INPUT_KEYBOARD_SCANCODE_R:         return InternalImGuiKey_R;
        case VD_INPUT_KEYBOARD_SCANCODE_S:         return InternalImGuiKey_S;
        case VD_INPUT_KEYBOARD_SCANCODE_T:         return InternalImGuiKey_T;
        case VD_INPUT_KEYBOARD_SCANCODE_U:         return InternalImGuiKey_U;
        case VD_INPUT_KEYBOARD_SCANCODE_V:         return InternalImGuiKey_V;
        case VD_INPUT_KEYBOARD_SCANCODE_W:         return InternalImGuiKey_W;
        case VD_INPUT_KEYBOARD_SCANCODE_X:         return InternalImGuiKey_X;
        case VD_INPUT_KEYBOARD_SCANCODE_Y:         return InternalImGuiKey_Y;
        case VD_INPUT_KEYBOARD_SCANCODE_Z:         return InternalImGuiKey_Z;
        case VD_INPUT_KEYBOARD_SCANCODE_SPACE:     return InternalImGuiKey_Space;
        case VD_INPUT_KEYBOARD_SCANCODE_ESCAPE:    return InternalImGuiKey_Escape;
        case VD_INPUT_KEYBOARD_SCANCODE_BACKSPACE: return InternalImGuiKey_Backspace;
        case VD_INPUT_KEYBOARD_SCANCODE_F1:        return InternalImGuiKey_F1;
        case VD_INPUT_KEYBOARD_SCANCODE_F2:        return InternalImGuiKey_F2;
        case VD_INPUT_KEYBOARD_SCANCODE_F3:        return InternalImGuiKey_F3;
        case VD_INPUT_KEYBOARD_SCANCODE_F4:        return InternalImGuiKey_F4;
        case VD_INPUT_KEYBOARD_SCANCODE_F5:        return InternalImGuiKey_F5;
        case VD_INPUT_KEYBOARD_SCANCODE_F6:        return InternalImGuiKey_F6;
        case VD_INPUT_KEYBOARD_SCANCODE_F7:        return InternalImGuiKey_F7;
        case VD_INPUT_KEYBOARD_SCANCODE_F8:        return InternalImGuiKey_F8;
        case VD_INPUT_KEYBOARD_SCANCODE_F9:        return InternalImGuiKey_F9;
        case VD_INPUT_KEYBOARD_SCANCODE_F10:       return InternalImGuiKey_F10;
        case VD_INPUT_KEYBOARD_SCANCODE_F11:       return InternalImGuiKey_F11;
        case VD_INPUT_KEYBOARD_SCANCODE_F12:       return InternalImGuiKey_F12;
        case VD_INPUT_KEYBOARD_SCANCODE_F13:       return InternalImGuiKey_F13;
        case VD_INPUT_KEYBOARD_SCANCODE_F14:       return InternalImGuiKey_F14;
        case VD_INPUT_KEYBOARD_SCANCODE_F15:       return InternalImGuiKey_F15;
        case VD_INPUT_KEYBOARD_SCANCODE_F16:       return InternalImGuiKey_F16;
        case VD_INPUT_KEYBOARD_SCANCODE_F17:       return InternalImGuiKey_F17;
        case VD_INPUT_KEYBOARD_SCANCODE_F18:       return InternalImGuiKey_F18;
        case VD_INPUT_KEYBOARD_SCANCODE_F19:       return InternalImGuiKey_F19;
        case VD_INPUT_KEYBOARD_SCANCODE_F20:       return InternalImGuiKey_F20;
        case VD_INPUT_KEYBOARD_SCANCODE_F21:       return InternalImGuiKey_F21;
        case VD_INPUT_KEYBOARD_SCANCODE_F22:       return InternalImGuiKey_F22;
        case VD_INPUT_KEYBOARD_SCANCODE_F23:       return InternalImGuiKey_F23;
        case VD_INPUT_KEYBOARD_SCANCODE_F24:       return InternalImGuiKey_F24;
        default: return 0;
    }
}

Vdi32 vd_input_mouse_button_to_imgui_mouse_button(VdInputMouseButton button)
{
    Vdi32 result = -1;
    switch (button) {
        case INPUT_MOUSE_BUTTON_LEFT:   result = 0; break;
        case INPUT_MOUSE_BUTTON_RIGHT:  result = 1; break;
        case INPUT_MOUSE_BUTTON_MIDDLE: result = 2; break;
        default: break;
    }

    return result;
}

#endif // VD_INPUT_IMPL
