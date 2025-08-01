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

typedef VD(u32) VD(InputKeyboardScancode);

enum {
    VD_(INPUT_KEYBOARD_SCANCODE_UNKOWN) = 0,
    VD_(INPUT_KEYBOARD_SCANCODE_A)      = 'A',
    VD_(INPUT_KEYBOARD_SCANCODE_B)      = 'B',
    VD_(INPUT_KEYBOARD_SCANCODE_C)      = 'C',
    VD_(INPUT_KEYBOARD_SCANCODE_D)      = 'D',
    VD_(INPUT_KEYBOARD_SCANCODE_E)      = 'E',
    VD_(INPUT_KEYBOARD_SCANCODE_F)      = 'F',
    VD_(INPUT_KEYBOARD_SCANCODE_G)      = 'G',
    VD_(INPUT_KEYBOARD_SCANCODE_H)      = 'H',
    VD_(INPUT_KEYBOARD_SCANCODE_I)      = 'I',
    VD_(INPUT_KEYBOARD_SCANCODE_J)      = 'J',
    VD_(INPUT_KEYBOARD_SCANCODE_K)      = 'K',
    VD_(INPUT_KEYBOARD_SCANCODE_L)      = 'L',
    VD_(INPUT_KEYBOARD_SCANCODE_M)      = 'M',
    VD_(INPUT_KEYBOARD_SCANCODE_N)      = 'N',
    VD_(INPUT_KEYBOARD_SCANCODE_O)      = 'O',
    VD_(INPUT_KEYBOARD_SCANCODE_P)      = 'P',
    VD_(INPUT_KEYBOARD_SCANCODE_Q)      = 'Q',
    VD_(INPUT_KEYBOARD_SCANCODE_R)      = 'R',
    VD_(INPUT_KEYBOARD_SCANCODE_S)      = 'S',
    VD_(INPUT_KEYBOARD_SCANCODE_T)      = 'T',
    VD_(INPUT_KEYBOARD_SCANCODE_U)      = 'U',
    VD_(INPUT_KEYBOARD_SCANCODE_V)      = 'V',
    VD_(INPUT_KEYBOARD_SCANCODE_W)      = 'W',
    VD_(INPUT_KEYBOARD_SCANCODE_X)      = 'X',
    VD_(INPUT_KEYBOARD_SCANCODE_Y)      = 'Y',
    VD_(INPUT_KEYBOARD_SCANCODE_Z)      = 'Z',
    VD_(INPUT_KEYBOARD_SCANCODE_SPACE)  = ' ',
    VD_(INPUT_KEYBOARD_SCANCODE_ESCAPE),
};

VD_INLINE VD(InputKeyboardScancode) VDF(input_translate_scancode_macos)(unsigned short nskeycode)
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
        default: return VD_(INPUT_KEYBOARD_SCANCODE_UNKOWN);
    }
}

#if VD_PLATFORM_MACOS
#define VD_INPUT_TRANSLATE_SCANCODE(x) VDF(input_translate_scancode_macos)(x)
#endif

#endif // !VD_INPUT_H
