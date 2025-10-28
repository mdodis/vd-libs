/* vd_ft.h Common Interface between platform-specific & platform-agnostic font backends 
 * ---------------------------------------------------------------------------------------------------------------------
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
#ifndef VD_FT_H
#define VD_FT_H

#ifndef VD_FT_API
#   ifdef VD_FT_STATIC
#       define VD_FT_API static
#   else
#       define VD_FT_API extern
#   endif // VD_FT_STATIC
#endif // !VD_FT_API

typedef struct {
    void *reserved0;
    int  units_per_em;
    int  ascent, descent, linegap;
} VdFtFont;

VD_FT_API int vd_ft_font_load(VdFtFont *font, void *memory, int size, int index);

#endif // !VD_FT_H

#ifdef VD_FT_IMPL

#ifdef _WIN32

#include <Windows.h>

#pragma comment(lib, "Gdi32.lib")

typedef struct {
    int initialized;
    HDC dc;
} Vd_Ft__Win32InternalData;

Vd_Ft__Win32InternalData Vd_Ft_G = {0};

static void vd_ft__win32_init(void);

VD_FT_API int vd_ft_font_load(VdFtFont *font, void *memory, int size, int index)
{
    if (!Vd_Ft_G.initialized) {
        vd_ft__win32_init();
        Vd_Ft_G.initialized = 1;
    }
}

static void vd_ft__win32_init(void)
{
    Vd_Ft_G.dc = CreateCompatibleDC(NULL);
}

#endif // _WIN32

#endif // VD_FT_IMPL