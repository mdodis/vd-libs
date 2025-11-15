/* vd_fw.h - Gets you a window with OpenGL running on platforms that support it
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
 * NOTES
 * - Do NOT include OpenGL headers, or import an OpenGL loader. This library does that already 
 * - The highest possible OpenGL version you should support if you want it to work (as of 2025...)
 *   on the big threes is OpenGL Core Profile 4.1 (MacOS limitation)
 * 
 * HID GUIDS
 * ╔════════════════════════════════════════════════════════════╗
 * ║ 0300 938d 5e04 0000 ff02 0000 0100 72 00                   ║
 * ║ │    │    │    │    │    │    │    │  │                    ║
 * ║ └────│────│────│────│────│────│────│──│── BUS              ║
 * ║      └────│────│────│────│────│────│──│── CRC              ║
 * ║           └────│────│────│────│────│──│── Vendor ID        ║
 * ║                └────│────│────│────│──│── (empty)          ║
 * ║                     └────│────│────│──│── Product ID       ║
 * ║                          └────│────│──│── (empty)          ║
 * ║                               └────│──│── Version          ║
 * ║                                    └──│── Driver Signature ║
 * ║                                       └── Driver Data      ║
 * ╚════════════════════════════════════════════════════════════╝
 * 
 * TODO
 * - Add option to enable sys keys
 * - Gamepads
 *     - Face Heuristics
 *     - Class Heuristics
 * - Win32: Use DeviceIoControl for XBOX controllers until they're correlated to XINPUT
 * - vd_fw_get_last_mouse_button_pressed
 * - Win32: Allow gamepad input even when window isn't focused?
 *     - Option for that
 * - Vulkan
 *     - Win32: Required extensions function
 *     - Win32: Query queue surface presentation support
 *     - Win32: Implement vkCreateWin32SurfaceKHR
 * - raw hat states
 * - File dialog
 * - OBS Studio breaks ChoosePixelFormat
 * - Make sure we can export functions properly for C++
 * - Expose customizable function pointer if the user needs to do something platform-specific before/after winthread has initialized or before vd_fw_init returns anyways.
 * - Have a way for a user to request OpenGL extensions/versions via a precedence array, and initialize the maximum possible version
 * - Clipboard
 * - Properly handle vd_fw_set_receive_ncmouse for clicks and scrolls
 * - Set mouse cursor to constants (resize, I, etc...)
 * - Have a way to store and load the window placement state (size, position, maximization state)
 * - Should vd_fw_set_receive_ncmouse be default 0 or 1?
 *   - Actually, consider removing it entirely
 * - set window unresizable
 * - MacOS: vd_fw_get_last_key_pressed
 * - MacOS: vd_fw_get_executable_dir()
 * - MacOS: vd_fw_set_fullscreen
 * - MacOS: Gamepad Support
 * - MacOS: Metal Sample
 * - MacOS: Set Graphics API
 * - When window not focused, or minimized, delay drawing?
 * - Allow to request specific framerate?
 * - On borderless, push mouse event right as we lose focus to a value outside of the window space
 */
#ifndef VD_FW_H
#define VD_FW_H
#define VD_FW_VERSION_MAJOR    0
#define VD_FW_VERSION_MINOR    0
#define VD_FW_VERSION_PATCH    2
#define VD_FW_VERSION          ((VD_FW_VERSION_MAJOR << 16) | (VD_FW_VERSION_MINOR << 8) | (VD_FW_VERSION_PATCH))

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#ifndef VD_FW_API
#   ifdef VD_FW_STATIC
#       define VD_FW_API static
#   else
#       define VD_FW_API extern
#   endif // VD_FW_STATIC
#endif // !VD_FW_API

#ifndef VD_FW_INL
#   define VD_FW_INL static inline
#endif // VD_FW_INL

#ifndef VD_FW_NO_CRT
#   define VD_FW_NO_CRT 0
#endif
#define VD_FW_FPI  3.14159265359f
#define VD_FW_FPI2 (2 * VD_FW_FPI)
#define VD_FW_FPIH (0.5f * VD_FW_FPI)

#ifndef VD_FW_SIN
#   if VD_FW_NO_CRT
#       define VD_FW_SIN(x) vd_fw_sin(x)
#   else
#       include <math.h>
#       define VD_FW_SIN(x) sinf(x)
#   endif
#endif // !VD_FW_SIN

#ifndef VD_FW_COS
#   if VD_FW_NO_CRT
#       define VD_FW_COS(x) vd_fw_cos(x)
#   else
#       include <math.h>
#       define VD_FW_COS(x) cosf(x)
#   endif
#endif // !VD_FW_COS

#ifndef VD_FW_TAN
#   if VD_FW_NO_CRT
#       define VD_FW_TAN(x) vd_fw_tan(x)
#   else
#       include <math.h>
#       define VD_FW_TAN(x) tanf(x)
#   endif
#endif // !VD_FW_TAN

#ifndef VD_FW_SQRT
#   if VD_FW_NO_CRT
#       define VD_FW_SQRT(x) vd_fw_sqrt(x)
#   else
#       include <math.h>
#       define VD_FW_SQRT(x) sqrtf(x)
#   endif
#endif // !VD_FW_SQRT

#ifndef VD_FW_MEMCPY
#   if VD_FW_NO_CRT
#       define VD_FW_MEMCPY(dst, src, count) vd_fw_memcpy(dst, src, count)
#   else
#       include <string.h>
#       define VD_FW_MEMCPY(dst, src, count) memcpy(dst, src, count)
#   endif
#endif // !VD_FW_MEMCPY

#ifndef VD_FW_MEMSET
#   if VD_FW_NO_CRT
#       define VD_FW_MEMSET(dst, val, num) vd_fw_memset(dst, val, num)
#   else
#       include <string.h>
#       define VD_FW_MEMSET(dst, val, num) memset(dst, val, num)
#   endif
#endif // !VD_FW_MEMSET

#ifndef VD_FW_LOG
#   if VD_FW_NO_CRT
#       define VD_FW_LOG(fmt, ...)
#   else
#       include <stdio.h>
#       define VD_FW_LOG(fmt, ...) printf("vd_fw: " fmt "\n", ## __VA_ARGS__)
#   endif // VD_FW_NO_CRT
#endif // !VD_FW_LOG

#define VD_FW_ENDIANNESS_LE 1
#define VD_FW_ENDIANNESS_BE 0
#ifndef VD_FW_ENDIANNESS
#   if defined(_MSC_VER)
#       if defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM) || defined(_M_ARM64)
#           define VD_FW_ENDIANNESS VD_FW_ENDIANNESS_LE
#       else
#           define VD_FW_ENDIANNESS VD_FW_ENDIANNESS_BE
#       endif
#   endif
#endif // VD_FW_ENDIANESS

#ifndef VD_FW_CUSTOM_TYPEDEFS
#   define VD_FW_CUSTOM_TYPEDEFS 0
#endif // !VD_FW_CUSTOM_TYPEDEFS

#if !VD_FW_CUSTOM_TYPEDEFS
#   include <stdint.h>
#   include <stddef.h>
#   define VdFwU8   uint8_t
#   define VdFwU16  uint16_t
#   define VdFwU32  uint32_t
#   define VdFwI32  int32_t
#   define VdFwSz   size_t
#   define VdFwU64  uint64_t
#endif // !VD_FW_CUSTOM_TYPEDEFS

#ifndef VD_FW_GAMEPAD_COUNT_MAX
#   define VD_FW_GAMEPAD_COUNT_MAX 16
#endif // !VD_FW_GAMEPAD_COUNT_MAX

#ifndef VD_FW_GAMEPAD_DB_DEFAULT
#   define VD_FW_GAMEPAD_DB_DEFAULT 1
#endif // !VD_FW_GAMEPAD_DB_DEFAULT

#ifndef VD_FW_NCRECTS_MAX
#   define VD_FW_NCRECTS_MAX 16
#endif // !VD_FW_NCRECTS_MAX

#ifndef VD_FW_CODEPOINT_BUFFER_COUNT
#   define VD_FW_CODEPOINT_BUFFER_COUNT 8
#endif // !VD_FW_CODEPOINT_BUFFER_COUNT

#define VD_FW_ARRAY_COUNT(x) (sizeof(x)/sizeof(x[0]))

#define VD_FW_SWAP16(x) ((VdFwU16)((x << 8) | (x >> 8)))
#if VD_FW_ENDIANNESS == VD_FW_ENDIANNESS_LE
#   define VD_FW_SWAP16LE(x) (x)
#else
#   define VD_FW_SWAP16LE(x) VD_FW_SWAP16(x)
#endif

typedef enum {
    VD_FW_GL_VERSION_BASIC = 0,
    VD_FW_GL_VERSION_1_0 = 10, VD_FW_GL_VERSION_1_1 = 11, VD_FW_GL_VERSION_1_2 = 12, VD_FW_GL_VERSION_1_3 = 13,
    VD_FW_GL_VERSION_1_4 = 14, VD_FW_GL_VERSION_1_5 = 15,
    VD_FW_GL_VERSION_2_0 = 20, VD_FW_GL_VERSION_2_1 = 21,
    VD_FW_GL_VERSION_3_0 = 30, VD_FW_GL_VERSION_3_1 = 31, VD_FW_GL_VERSION_3_2 = 32, VD_FW_GL_VERSION_3_3 = 33,
    VD_FW_GL_VERSION_4_0 = 40, VD_FW_GL_VERSION_4_1 = 41, VD_FW_GL_VERSION_4_2 = 42, VD_FW_GL_VERSION_4_3 = 43,
    VD_FW_GL_VERSION_4_4 = 44, VD_FW_GL_VERSION_4_5 = 45, VD_FW_GL_VERSION_4_6 = 46,
} VdFwGlVersion;

enum /*VdFwPlatformEnum*/ {
    VD_FW_PLATFORM_UNKNOWN,
    VD_FW_PLATFORM_WINDOWS,
    VD_FW_PLATFORM_LINUX,
    VD_FW_PLATFORM_MACOS,
    VD_FW_PLATFORM_ANDROID,
    VD_FW_PLATFORM_IOS,
};
typedef VdFwU8 VdFwPlatform;

typedef enum {
    VD_FW_GRAPHICS_API_OPENGL = 0,
    VD_FW_GRAPHICS_API_CUSTOM,
    VD_FW_GRAPHICS_API_PIXEL_BUFFER,
    VD_FW_GRAPHICS_API_INVALID = 100,
} VdFwGraphicsApi;

typedef struct {
    /* What version of OpenGL you'd like to use. 3.3 and upwards recommended. */
    VdFwGlVersion version;

    /* Whether to enable a debug console to show you errors produced by GL calls */
    int           debug_on;
} VdFwOpenGLOptions;

typedef struct {
    /* The graphics API you're planning to use. Defaulted to OpenGL. */
    VdFwGraphicsApi     api;

    VdFwOpenGLOptions   gl;

    struct {
        /* Set to 1 to disable window frame. */
        int             borderless;
    } window_options;
} VdFwInitInfo;

/**
 * @brief Initialize fw. Call this before any other call
 * @param  info Custom options when initializing. Leave null for default
 * @return      (Reserved)
 */
VD_FW_API int                vd_fw_init(VdFwInitInfo *info);

/**
 * @brief Check if the application is running. Call this every frame
 * @return  1 if running, 0 if not
 */
VD_FW_API int                vd_fw_running(void);

/**
 * @brief End rendering and swap buffers. Call this right at the end of your rendering code.
 * @return  (Reserved)
 */
VD_FW_API int                vd_fw_swap_buffers(void);

/**
 * @brief Get if the user requested to close the window
 * @return  Whether the user tried to close the window this frame
 */
VD_FW_API int                vd_fw_close_requested(void);

/**
 * @brief Close the window and end the rendering loop
 */
VD_FW_API void               vd_fw_quit(void);

/**
 * @brief Get the current platform.
 * @return  The current platform.
 */
VD_FW_API VdFwPlatform       vd_fw_get_platform(void);

VD_FW_API void               vd_fw_set_graphics_api(VdFwGraphicsApi api, VdFwOpenGLOptions *gl_options);

/* ----WINDOW-------------------------------------------------------------------------------------------------------- */
/**
 * @brief Get the size of the window, in pixels
 * @param  w The width of the window, in pixels
 * @param  h The height of the window, in pixels
 * @return   (Reserved)
 */
VD_FW_API int                vd_fw_get_size(int *w, int *h);

/**
 * @brief Set the window size, in pixels
 * @param  w The width of the window, in pixels
 * @param  h The height of the window, in pixels
 */
VD_FW_API void               vd_fw_set_size(int w, int h);

/**
 * @brief Set minimum window size, in pixels.
 * @param  w The minimum width of the window, set to 0 to use default.
 * @param  h The minium height of the window, set to 0 to use default.
 */
VD_FW_API void               vd_fw_set_size_min(int w, int h);

/**
 * @brief Set maximum window size, in pixels.
 * @param  w The maximum width of the window, set to 0 to use default.
 * @param  h The maxium height of the window, set to 0 to use default.
 */
VD_FW_API void               vd_fw_set_size_max(int w, int h);

/**
 * @brief Get if the window is minimized
 * @param  minimized Whether the window is minimized
 * @return 1 if the minimization state of the window was changed
 */
VD_FW_API int                vd_fw_get_minimized(int *minimized);

/**
 * @brief Minimize the window
 */
VD_FW_API void               vd_fw_minimize(void);

/**
 * @brief Get if the window is maximized
 * @param  maximized Whether the window is maximized
 * @return 1 if the maximization state of the window was changed
 */
VD_FW_API int                vd_fw_get_maximized(int *maximized);

/**
 * @brief Maximize the window
 */
VD_FW_API void               vd_fw_maximize(void);

/**
 * @brief Restores the window state, if it's minimized or maximized
 */
VD_FW_API void               vd_fw_normalize(void);

/**
 * @brief Enter/exit fullscreen.
 * @param  on Whether to enter or exit fullscreen.
 */
VD_FW_API void               vd_fw_set_fullscreen(int on);

/**
 * @brief Get current fullscreen state.
 * @return  1 if in fullscreen, 0 otherwise.
 */
VD_FW_API int                vd_fw_get_fullscreen(void);

/**
 * @brief Gets whether the window is focused
 * @param  focused Pointer to int which will receive the value of focus
 * @return         1 if the focus has changed. There's no point in checking the value of focused otherwise.
 */
VD_FW_API int                vd_fw_get_focused(int *focused);

/**
 * @brief Set the draggable area of the window, any sub-rectangles to ignore
 * @param  caption  The whole draggable area
 * @param  count    The count of sub-rectangles that will be excluded from dragging
 * @param  rects    An array of count rectangles to exclude
 */
VD_FW_API void               vd_fw_set_ncrects(int caption[4], int count, int (*rects)[4]);

/**
 * @brief Set to receive mouse events outside of the non-client area
 * @param  on 1 if you want to receive those events, 0 otherwise (default)
 */
VD_FW_API void               vd_fw_set_receive_ncmouse(int on);

/**
 * @brief Gets the backing scale factor
 * @return  The backing scale factor (1.0f: 1:1 scale, 2.0f, 2:1 scale, etc...)
 */
VD_FW_API float              vd_fw_get_scale(void);

/**
 * @brief Set the title of the window
 * @param  title The new title of the window
 */
VD_FW_API void               vd_fw_set_title(const char *title);

/**
 * @brief Set the icon of the window and application
 * @param  pixels A packed A8R8G8B8 pixel buffer
 * @param  width  The width of the icon, in pixels, must be at least 16px
 * @param  height The height of the icon, in pixels, must be at least 16px
 */
VD_FW_API void               vd_fw_set_app_icon(void *pixels, int width, int height);

/**
 * @brief Get the time (in nanoseconds) since the last call to vd_fw_swap_buffers
 * @return  The delta time (in nanoseconds)
 */
VD_FW_API unsigned long long vd_fw_delta_ns(void);

/**
 * @brief Get the time (in seconds) since the last call to vd_fw_swap_buffers
 * @return  The delta time (in seconds)
 */
VD_FW_INL float              vd_fw_delta_s(void);

/**
 * @brief Set the number of frames to sync on
 * @param  on Use: 0 for no sync, 1 for sync every frame and 2 for sync every other frame
 * @return 1 if the change was applied successfully
 */
VD_FW_API int                vd_fw_set_vsync_on(int on);

/**
 * @brief Get the fully-qualified path to the executable without the last path separator.
 * @param  len Length of the UTF-8 string, in bytes.
 * @return     A callee-allocated string. There's no need to free it.
 */
VD_FW_API const char*        vd_fw_get_executable_dir(int *len);

/* ----MOUSE--------------------------------------------------------------------------------------------------------- */
enum {
    VD_FW_MOUSE_STATE_LEFT_BUTTON_DOWN   = 1 << 0,
    VD_FW_MOUSE_STATE_RIGHT_BUTTON_DOWN  = 1 << 1,
    VD_FW_MOUSE_STATE_MIDDLE_BUTTON_DOWN = 1 << 2,
    VD_FW_MOUSE_STATE_M1_BUTTON_DOWN     = 1 << 3,
    VD_FW_MOUSE_STATE_M2_BUTTON_DOWN     = 1 << 4,

    VD_FW_MOUSE_BUTTON_LEFT   = VD_FW_MOUSE_STATE_LEFT_BUTTON_DOWN,
    VD_FW_MOUSE_BUTTON_RIGHT  = VD_FW_MOUSE_STATE_RIGHT_BUTTON_DOWN,
    VD_FW_MOUSE_BUTTON_MIDDLE = VD_FW_MOUSE_STATE_MIDDLE_BUTTON_DOWN,
    VD_FW_MOUSE_BUTTON_M1     = VD_FW_MOUSE_STATE_M1_BUTTON_DOWN,
    VD_FW_MOUSE_BUTTON_M2     = VD_FW_MOUSE_STATE_M2_BUTTON_DOWN,
};

/**
 * @brief Read the mouse state.
 * @param  x The horizontal position of the mouse, in pixels (left -> right)
 * @param  y The vertical position of the mouse, in pixels (top -> bottom)
 * @return   The mouse button state
 */
VD_FW_API int                vd_fw_get_mouse_state(int *x, int *y);

/**
 * @brief Read the mouse state (float version).
 * @param  x The horizontal position of the mouse, in pixels (left -> right)
 * @param  y The vertical position of the mouse, in pixels (top -> bottom)
 * @return   The mouse button state
 */
VD_FW_INL int                vd_fw_get_mouse_statef(float *x, float *y);

/**
 * @brief Get whether the mouse button is down
 * @param  button The button
 * @return        1 if the button is down, 0 otherwise
 */
VD_FW_INL int                vd_fw_get_mouse_down(int button);

/**
 * @brief Get if the supplied button was just clicked
 * @param  button The button to check
 * @return        Whether the button was clicked last frame
 */
VD_FW_API int                vd_fw_get_mouse_clicked(int button);


/**
 * @brief Get if the supplied button was just released
 * @param  button The button check
 * @return        Whether the button was released last frame
 */
VD_FW_API int                vd_fw_get_mouse_released(int button);

/**
 * @brief Capture mouse and receive events outside of the window region.
 * @param  on Whether to set this behavior on (default: off).
 */
VD_FW_API void               vd_fw_set_mouse_capture(int on);

/**
 * @brief Get the mouse movement in raw smoothed pixels. Use this over computing delta yourself.
 * @param  dx The horizontal delta movement of the mouse
 * @param  dy The vertical delta movement of the mouse
 */
VD_FW_API void               vd_fw_get_mouse_delta(float *dx, float *dy);

/**
 * @brief Lock/Unlock the mouse to the center of the window, hiding its cursor
 * @param  locked Whether to lock or unlock the mouse (default: unlocked)
 */
VD_FW_API void               vd_fw_set_mouse_locked(int locked);

/**
 * @brief Gets whether the mouse is locked (by vd_fw_set_mouse_locked).
 * @return  Whether the mouse is locked (1 or 0)
 */
VD_FW_API int                vd_fw_get_mouse_locked(void);

/**
 * @brief Read the mouse wheel state.
 * @param  dx The delta of horizontal wheel (either trackpad swipe right, or ctrl + mousewheel)
 * @param  dy The delta of vertical wheel
 * @return    1 if the wheel moved, 0 if not
 */
VD_FW_API int                vd_fw_get_mouse_wheel(float *dx, float *dy);

/* ----KEYBOARD------------------------------------------------------------------------------------------------------ */
enum {
    VD_FW_KEY_UNKNOWN       = 0,
    VD_FW_KEY_F1  = 1,  VD_FW_KEY_F2  = 2,  VD_FW_KEY_F3  = 3,  VD_FW_KEY_F4  = 4,
    VD_FW_KEY_F5  = 5,  VD_FW_KEY_F6  = 6,  VD_FW_KEY_F7  = 7,  VD_FW_KEY_F8  = 8,
    VD_FW_KEY_F9  = 9,  VD_FW_KEY_F10 = 10, VD_FW_KEY_F11 = 11, VD_FW_KEY_F12 = 12,
    VD_FW_KEY_F13 = 13, VD_FW_KEY_F14 = 14, VD_FW_KEY_F15 = 15, VD_FW_KEY_F16 = 16,
    VD_FW_KEY_F17 = 17, VD_FW_KEY_F18 = 18, VD_FW_KEY_F19 = 19, VD_FW_KEY_F20 = 20,
    VD_FW_KEY_F21 = 21, VD_FW_KEY_F22 = 22, VD_FW_KEY_F23 = 23, VD_FW_KEY_F24 = 24,
    VD_FW_KEY_BACKSPACE     = 25,  
    VD_FW_KEY_INS = 26, VD_FW_KEY_HOME = 27, VD_FW_KEY_PGUP = 28,
    VD_FW_KEY_DEL = 29, VD_FW_KEY_END  = 30, VD_FW_KEY_PGDN = 31,
    VD_FW_KEY_SPACE         = 32,  /* ' ' */
    VD_FW_KEY_LCONTROL = 33, VD_FW_KEY_RCONTROL = 34,
    VD_FW_KEY_LALT     = 35, VD_FW_KEY_RALT     = 36,
    VD_FW_KEY_LSHIFT   = 37, VD_FW_KEY_RSHIFT   = 38,
    VD_FW_KEY_QUOTE         = 39,  /* '\''*/
    VD_FW_KEY_ARROW_UP   = 40,
    VD_FW_KEY_ARROW_LEFT = 41, VD_FW_KEY_ARROW_DOWN = 42, VD_FW_KEY_ARROW_RIGHT = 43,
    VD_FW_KEY_COMMA         = 44,  /* ','*/
    VD_FW_KEY_MINUS         = 45,  /* '-' */
    VD_FW_KEY_DOT           = 46,  /* '.' */
    VD_FW_KEY_SLASH_FORWARD = 47,  /* '/' */
    VD_FW_KEY_0 = 48,  /* '0' */ VD_FW_KEY_1 = 49,  /* '1' */
    VD_FW_KEY_2 = 50,  /* '2' */ VD_FW_KEY_3 = 51,  /* '3' */
    VD_FW_KEY_4 = 52,  /* '4' */ VD_FW_KEY_5 = 53,  /* '5' */
    VD_FW_KEY_6 = 54,  /* '6' */ VD_FW_KEY_7 = 55,  /* '7' */
    VD_FW_KEY_8 = 56,  /* '8' */ VD_FW_KEY_9 = 57,  /* '9' */
    VD_FW_KEY_ENTER         = 58,
    VD_FW_KEY_SEMICOLON     = 59,  /* ';' */
    VD_FW_KEY_TAB           = 60,
    VD_FW_KEY_EQUALS        = 61,  /* '=' */
    VD_FW_KEY_CAPITAL       = 62,
    VD_FW_KEY_ESCAPE        = 63,
    VD_FW_KEY_RESERVED1     = 64,  /* '@' */
    VD_FW_KEY_A = 65, VD_FW_KEY_B = 66, VD_FW_KEY_C = 67, VD_FW_KEY_D = 68,
    VD_FW_KEY_E = 69, VD_FW_KEY_F = 70, VD_FW_KEY_G = 71, VD_FW_KEY_H = 72,
    VD_FW_KEY_I = 73, VD_FW_KEY_J = 74, VD_FW_KEY_K = 75, VD_FW_KEY_L = 76,
    VD_FW_KEY_M = 77, VD_FW_KEY_N = 78, VD_FW_KEY_O = 79, VD_FW_KEY_P = 80,
    VD_FW_KEY_Q = 81, VD_FW_KEY_R = 82, VD_FW_KEY_S = 83, VD_FW_KEY_T = 84,
    VD_FW_KEY_U = 85, VD_FW_KEY_V = 86, VD_FW_KEY_W = 87, VD_FW_KEY_X = 88,
    VD_FW_KEY_Y = 89,
    VD_FW_KEY_Z = 90,
    VD_FW_KEY_BRACKET_OPEN  = 91,  /* '[' */
    VD_FW_KEY_SLASH_BACK    = 92,  /* '\\' */
    VD_FW_KEY_BRACKET_CLOSE = 93,  /* ']' */
    VD_FW_KEY_MEDIA_NEXT    = 94,  /* Media Next Track */
    VD_FW_KEY_MEDIA_PREV    = 95,  /* Media Prev Track */
    VD_FW_KEY_BACKTICK      = 96,  /* '`' */
    VD_FW_KEY_MEDIA_PLAY    = 97,  /* Media Play/Pause */
    VD_FW_KEY_NUMPAD_0 = 98,  /* Numpad 0 */ VD_FW_KEY_NUMPAD_1 = 99,  /* Numpad 1 */
    VD_FW_KEY_NUMPAD_2 = 100, /* Numpad 2 */ VD_FW_KEY_NUMPAD_3 = 101, /* Numpad 3 */
    VD_FW_KEY_NUMPAD_4 = 102, /* Numpad 4 */ VD_FW_KEY_NUMPAD_5 = 103, /* Numpad 5 */
    VD_FW_KEY_NUMPAD_6 = 104, /* Numpad 6 */ VD_FW_KEY_NUMPAD_7 = 105, /* Numpad 7 */
    VD_FW_KEY_NUMPAD_8 = 106, /* Numpad 8 */ VD_FW_KEY_NUMPAD_9 = 107, /* Numpad 9 */
    VD_FW_KEY_MAX,
};
typedef int VdFwKey;

/**
 * @brief Get whether a key was just pressed this frame
 * @param  key The key to check
 * @return     Whether this key was pressed this frame
 */
VD_FW_API int                vd_fw_get_key_pressed(int key);

/**
 * @brief Get the last known state of this key
 * @param  key The key to check
 * @return     Whether this key is down currently
 */
VD_FW_API int                vd_fw_get_key_down(int key);

VD_FW_API int                vd_fw_get_last_key_pressed(void);

/**
 * @brief Convert key to string.
 * @param  k The key
 * @return   The key's name
 */
VD_FW_INL const char*        vd_fw_get_key_name(VdFwKey k);

/* ----GAMEPADS------------------------------------------------------------------------------------------------------ */
enum {
    // XBox Style Buttons
    VD_FW_GAMEPAD_UNKNOWN = 0,
    VD_FW_GAMEPAD_A,
    VD_FW_GAMEPAD_B,
    VD_FW_GAMEPAD_X,
    VD_FW_GAMEPAD_Y,
    VD_FW_GAMEPAD_DUP,
    VD_FW_GAMEPAD_DDOWN,
    VD_FW_GAMEPAD_DLEFT,
    VD_FW_GAMEPAD_DRIGHT,
    VD_FW_GAMEPAD_START,
    VD_FW_GAMEPAD_BACK,
    VD_FW_GAMEPAD_LEFT_SHOULDER,
    VD_FW_GAMEPAD_RIGHT_SHOULDER,
    VD_FW_GAMEPAD_LEFT_STICK,
    VD_FW_GAMEPAD_RIGHT_STICK,
    VD_FW_GAMEPAD_LEFT_PAD0,
    VD_FW_GAMEPAD_RIGHT_PAD0,
    VD_FW_GAMEPAD_LEFT_PAD1,
    VD_FW_GAMEPAD_RIGHT_PAD1,
    VD_FW_GAMEPAD_LEFT_PAD2,
    VD_FW_GAMEPAD_RIGHT_PAD2,
    VD_FW_GAMEPAD_AUX0,
    VD_FW_GAMEPAD_AUX1,
    VD_FW_GAMEPAD_AUX2,
    VD_FW_GAMEPAD_AUX3,
    VD_FW_GAMEPAD_AUX4,
    VD_FW_GAMEPAD_AUX5,
    VD_FW_GAMEPAD_AUX6,
    VD_FW_GAMEPAD_AUX7,
    VD_FW_GAMEPAD_AUX8,
    VD_FW_GAMEPAD_AUX9,
    VD_FW_GAMEPAD_BUTTON_MAX,

    // Playstation Style Buttons
    VD_FW_GAMEPAD_CROSS    = VD_FW_GAMEPAD_A,
    VD_FW_GAMEPAD_CIRCLE   = VD_FW_GAMEPAD_B,
    VD_FW_GAMEPAD_SQUARE   = VD_FW_GAMEPAD_X,
    VD_FW_GAMEPAD_TRIANGLE = VD_FW_GAMEPAD_Y,
    VD_FW_GAMEPAD_SELECT   = VD_FW_GAMEPAD_BACK,
    VD_FW_GAMEPAD_L1       = VD_FW_GAMEPAD_LEFT_SHOULDER,
    VD_FW_GAMEPAD_R1       = VD_FW_GAMEPAD_RIGHT_SHOULDER,
    VD_FW_GAMEPAD_L3       = VD_FW_GAMEPAD_LEFT_STICK,
    VD_FW_GAMEPAD_R3       = VD_FW_GAMEPAD_RIGHT_STICK,

    VD_FW_GAMEPAD_H = 0 >> 1,
    VD_FW_GAMEPAD_V = 2 >> 1,
    VD_FW_GAMEPAD_L = 0 << 1,
    VD_FW_GAMEPAD_R = 1 << 1,
    VD_FW_GAMEPAD_LH = VD_FW_GAMEPAD_L | VD_FW_GAMEPAD_H,
    VD_FW_GAMEPAD_LV = VD_FW_GAMEPAD_L | VD_FW_GAMEPAD_V,
    VD_FW_GAMEPAD_RH = VD_FW_GAMEPAD_R | VD_FW_GAMEPAD_H,
    VD_FW_GAMEPAD_RV = VD_FW_GAMEPAD_R | VD_FW_GAMEPAD_V,
    VD_FW_GAMEPAD_L2 = 4,
    VD_FW_GAMEPAD_R2 = 5,
    VD_FW_GAMEPAD_LT = VD_FW_GAMEPAD_L2,
    VD_FW_GAMEPAD_RT = VD_FW_GAMEPAD_R2,
    VD_FW_GAMEPAD_AXIS_MAX,
};
typedef int VdFwGamepadInput;

enum {
    VD_FW_GAMEPAD_FACE_UNKNOWN = 0,
    VD_FW_GAMEPAD_FACE_NUMBERED,    /* face:numbered */
    VD_FW_GAMEPAD_FACE_XBOX,        /* face:xbox */
    VD_FW_GAMEPAD_FACE_PLAYSTATION, /* face:playstation */
    VD_FW_GAMEPAD_FACE_NINTENDO,    /* face:nintendo */
    VD_FW_GAMEPAD_FACE_MAX,
};
typedef VdFwU8 VdFwGamepadFace;

// Gamepads are ranked based weighted-importance input capability
// Generally, higher value -> more important inputs
// 
// For Gamepads that have the same amount of buttons but not with the same locality/affordance, Gameplay/Control buttons
// are deemed more significant.
// 
// The names somewhat map to the system/controller, but since those systems may or may not support more/less capable
// controllers or controller features, it should only be considered a mnemonic, and the GUID should be used instead.
//
// Some controllers, for example official controllers for the Playstation 2 system have even more capabilities, like
// pressure sensitive face buttons. For the purpose of this cross-platform library, they are not considered if they
// were relatively unpopular in games shipped with that system.
// 
// Additionally, controller inputs that do not explicitly indicate analog usage are also ignored (again, like the PS2
// pressure-sensitive shoulders/triggers).
enum {
    VD_FW_GAMEPAD_CLASS_INVALID = 0,
    // class:nes          | 1 PoV, 2 Control, 2 System
    VD_FW_GAMEPAD_CLASS_NES,
    // class:megadrive    | 1 PoV, 3 Control, 1 System
    VD_FW_GAMEPAD_CLASS_MEGADRIVE,
    // class:genesis      | 1 PoV, 6 Control, 2 System
    VD_FW_GAMEPAD_CLASS_GENESIS,
    // class:snes         | 1 PoV, 4 Control, 2 System, 2 Symmetrical
    VD_FW_GAMEPAD_CLASS_SNES,
    // class:ps1          | 1 PoV, 4 Control, 2 System, 4 Symmetrical
    VD_FW_GAMEPAD_CLASS_PS1,
    // class:joycon       |        4 Control, 2 System, 2 Symmetrical, 1 Clickable Stick
    VD_FW_GAMEPAD_CLASS_JOYCON,
    // class:n64          | 1 PoV, 6 Control, 2 System, 2 Symmetrical, 1 Stick
    VD_FW_GAMEPAD_CLASS_N64,
    // class:ps2          | 1 PoV, 4 Control, 2 System, 4 Symmetrical, 2 Clickable Sticks
    VD_FW_GAMEPAD_CLASS_PS2,
    // class:xbox         | 1 PoV, 4 Control, 2 System, 2 Symmetrical, 2 Clickable Sticks, 2 Symmetrical Axes
    VD_FW_GAMEPAD_CLASS_XBOX,
    // class:ps4          | 1 PoV, 4 Control, 2 System, 2 Symmetrical, 2 Clickable Sticks, 2 Symmetrical Axes, 1 Touchpad
    VD_FW_GAMEPAD_CLASS_PS4,
    // class:steamdeck    | 1 PoV, 4 Control, 2 System, 6 Symmetrical, 2 Clickable Sticks, 2 Symmetrical Axes, 2 Touchpads 
    VD_FW_GAMEPAD_CLASS_STEAMDECK,
    VD_FW_GAMEPAD_CLASS_MAX,
};
typedef VdFwU8 VdFwGamepadClass;

// ATTENTION
// Most of the enums regarding gamepad mapping are intended for internal usage
// But they are present here for future usages/features and to allow you to stack-allocate
// Gamepad entries, get debugging info and so on.
enum {
    VD_FW_GAMEPAD_INPUT_TYPE_DIGITAL,
    VD_FW_GAMEPAD_INPUT_TYPE_AXIAL,
    VD_FW_GAMEPAD_INPUT_TYPE_HAT_SWITCH,
};

enum {
    // No source kind. Used for the terminating entry.
    VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_NONE = 0,
    // Digital state input in report.
    VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON = 1,
    // Directional PoV input in report. Usually 0-7 or 1-8 to indicate NESW direction coming from d-pad
    VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_HAT = 2,
    // Axial input in report.
    VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS = 3,
    // Use this to mask a VdFwGamepadMappingSourceKind variable to get the aforementioned source kinds.
    VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_MASK = 0b00000011,

    // Reinterpret button as axis (On/Off) -> (1.0/0.0).
    VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_BUTTON_TO_AXIS = (1 << 2),
    // Reinterpret axis as button (+0.1/0.0) -> (On/Off).
    VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_AXIS_TO_BUTTON = (1 << 3),
    // Reinterpret input axis value as a 2 part vd_fw axis (rarely used).
    VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_PARTWISE       = (1 << 4),
    // Reserved.
    VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_SPLIT          = (1 << 5),
    // Invert button or axis values ([0.0, 1.0] -> [1.0, 0.0])
    VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_INVERTED       = (1 << 6),
    // Combined with other source flags to handle usage value reports that express multiple controller analog inputs.
    VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_ZERO_TO_MAX    = (1 << 7),

    VD_FW_GAMEPAD_MAX_MAPPINGS = 48,

    // Some Gamepads, while shipping with rumble motors may not support rumble (or the state packets that should be sent
    // to manipulate the actuators is unknown).
    // 
    // Additionally, certain gamepads, like 8BitDo may support rumble based on the mode they're in (e.g.
    // XInput-Compatible vs. RawInput/DirectInput-Compatible)
    VD_FW_GAMEPAD_RUMBLE_TYPE_NOT_AVAILABLE = 0,

    // rumble:w<prefix>llhh: Writes instantly to file
    VD_FW_GAMEPAD_RUMBLE_TYPE_RAW           = 1,

    // Used Internally when a gamepad has been correlated to an xinput dwUserIndex
    VD_FW_GAMEPAD_RUMBLE_TYPE_XINPUT        = 2,

    VD_FW_GAMEPAD_RUMBLE_MAX_PREFIX_BYTES   = 14,
};
typedef VdFwU8 VdFwGamepadMappingSourceKind;

typedef struct {
    VdFwGamepadMappingSourceKind kind;  
    unsigned char                target;
    unsigned short               index;
} VdFwGamepadMapEntry;

typedef union {
    VdFwU32 whole;
    struct {
        VdFwU16 offset;
        VdFwU16 byte_length;
    } parts;
} VdFwGamepadSignificantPacketPosition;

typedef struct {
    VdFwU8 type;
    VdFwU8 prefix_len;
    VdFwU8 prefix[VD_FW_GAMEPAD_RUMBLE_MAX_PREFIX_BYTES];
    union {
        struct {
            VdFwGamepadSignificantPacketPosition    rumble_lo;
            VdFwGamepadSignificantPacketPosition    rumble_hi;
        } raw;
    } dat;
} VdFwGamepadRumbleConfig;

typedef struct {
    VdFwGamepadMapEntry     mappings[VD_FW_GAMEPAD_MAX_MAPPINGS];
    VdFwGamepadRumbleConfig rumble_config;
    VdFwGamepadFace         face;
    VdFwGamepadClass        klass;
} VdFwGamepadMap;

typedef struct {
    float rumble_lo;
    float rumble_hi;
} VdFwGamepadRumbleState;

typedef union {
    VdFwU8 dat[16];
    struct {
        VdFwU16 bus;
        VdFwU16 crc;
        VdFwU16 vendor_id;
        VdFwU16 reserved0;
        VdFwU16 product_id;
        VdFwU16 reserved1;
        VdFwU16 version;
        VdFwU8  driver_signature;
        VdFwU8  driver_data;
    } parts;
} VdFwGuid;

typedef struct {
    VdFwGuid       guid;
    VdFwGamepadMap map;
} VdFwGamepadDBEntry;

/**
 * @brief Gets the number of gamepads currently connected
 * @return  The number of currently connected gamepads
 */
VD_FW_API int                vd_fw_get_gamepad_count(void);

/**
 * @brief Gets the state of all (digital) buttons on the gamepad
 * @param  index The gamepad index
 * @return       The button state bitfield (use (1 << VD_FW_GAMEPAD_A/B/AUX0...) to test)
 */
VD_FW_API VdFwU64            vd_fw_get_gamepad_button_state(int index);

/**
 * @brief Gets the state for a Gamepad button (digital)
 * @param  index  The gamepad index
 * @param  button The gamepad button to check
 * @return 1 for On, 0 for Off
 */
VD_FW_API int                vd_fw_get_gamepad_down(int index, int button);

/**
 * @brief Gets whether the Gamepad button was just pressed this frame
 * @param  index  The gamepad index
 * @param  button The gamepad button to check
 * @return 1 if the button was just pressed this frame, 0 otherwise
 */
VD_FW_API int                vd_fw_get_gamepad_pressed(int index, int button);

/** 
 * @brief Gets the gamepad's axis value 
 * @param  index The gamepad index
 * @param  axis  The axis to check
 * @param  out   The axis value [-1, 1] for directional axes (VD_FW_GAMEPAD_LH, etc..), [0, 1] for triggers
 * @return (Reserved)
 */
VD_FW_API int                vd_fw_get_gamepad_axis(int index, int axis, float *out);

/**
 * @brief Set the state of a gamepad's force-feedback motors
 * @param  index     The gamepad index
 * @param  rumble_lo The value of the small/left motor [0, 1]
 * @param  rumble_hi The value of the big/right motor [0, 1]
 */
VD_FW_API void               vd_fw_set_gamepad_rumble(int index, float rumble_lo, float rumble_hi);

/**
 * @brief Get the gamepad's GUID
 * @param  index The gamepad index
 * @return The guid
 */
VD_FW_API VdFwGuid           vd_fw_get_gamepad_guid(int index);

/**
 * @brief Get the detected gamepad's face type (i.e. the symbols shown on the physical controller)
 * @param  index The gamepad index
 * @return       The face type
 */
VD_FW_API VdFwGamepadFace    vd_fw_get_gamepad_face(int index);

/**
 * @brief Convert gamepad face type to string
 * @param  face The face type
 * @return      The face type as a string
 */
VD_FW_API const char*        vd_fw_get_gamepad_face_name(VdFwGamepadFace face);

/**
 * @brief Get the detected gamepad's classification (i.e. a rough ordered value of the gamepad's capabilities)
 * @param  index The gamepad index
 * @return       The class type
 */
VD_FW_API VdFwGamepadClass   vd_fw_get_gamepad_class(int index);

/**
 * @brief Convert gamepad class type to string
 * @param  klass The class type
 * @return       The class type as a string
 */
VD_FW_API const char*        vd_fw_get_gamepad_class_name(VdFwGamepadClass klass);

/**
 * @brief Get whether this gamepad supports rumble
 * @param  index The gamepad index
 * @return       1 if the gamepad supports rumble, 0 otherwise
 */
VD_FW_API int                vd_fw_get_gamepad_rumble_support(int index);

/**
 * @brief Parse and register gamepad entries from a RGCDB file
 * @param  text     The text file
 * @param  text_len The text file length in bytes
 */
VD_FW_API void               vd_fw_add_gamepad_rgcdb(const char *text, int text_len);

/**
 * @brief Parse an RGCDB entry ascii string, can be called without initializing this library
 * @param  s              The string
 * @param  s_len          The string's length, in bytes
 * @param  out            The db entry info
 * @param  out_platform   The platform for which this entry is valid
 * @param  out_begin_name The start of the name part of this gamepad (unused in db entries)
 * @return                1 for Success, 0 otherwise
 */
VD_FW_API int                vd_fw_parse_gamepad_db_entry(const char *s, int s_len, VdFwGamepadDBEntry *out, VdFwPlatform *out_platform, const char **out_begin_name);

/**
 * @brief Check if a map entry is a terminating entry. Use to iterate over vd_fw_parse_gamepad_db_entry results
 * @param  entry The entry
 * @return       1 if the entry is a terminating entry
 */
VD_FW_API int                vd_fw_gamepad_map_entry_is_none(VdFwGamepadMapEntry *entry);

/**
 * @brief Add an entry to the runtime gamepad db
 * @param  entry The entry to add
 * @return (Reserved)
 */
VD_FW_API int                vd_fw_add_gamepad_db_entry(VdFwGamepadDBEntry *entry);

/**
 * @brief Turn On/Off raw button/axis/hat reports
 * @param  on Whether to enable this behavior
 */
VD_FW_API void               vd_fw_set_gamepad_raw_reports(int on);

/**
 * @brief Get the raw state of at most 64 buttons on this gamepad
 * @param  index The gamepad index
 * @return       A bitmask of states (LSB -> MSB) --> (b0 -> b63)
 */
VD_FW_API VdFwU64            vd_fw_get_gamepad_raw_buttons(int index);

/**
 * @brief Get the raw state of all axes on this gamepad (scaled to [0,1])
 * @param  index      The gamepad index
 * @param  count_axes The number of axes
 * @return            A callee-allocated float array of axis values
 */
VD_FW_API float*             vd_fw_get_gamepad_raw_axes(int index, int *count_axes);

/* ----TEXT INPUT---------------------------------------------------------------------------------------------------- */
/**
 * @brief Get the number of characters sent by the user
 * @return  The count of characters. At most VD_FW_CODEPOINT_BUFFER_COUNT
 */
VD_FW_API unsigned short     vd_fw_get_num_codepoints(void);

/**
 * @brief Get the i'th character as a UTF-32 codepoint
 * @param  index The character index
 * @return       The Unicode codepoint
 */
VD_FW_API unsigned int       vd_fw_get_codepoint(unsigned short index);

/* ----PLATFORM SPECIFIC--------------------------------------------------------------------------------------------- */

/**
 * @brief Returns a pointer to the window handle allocated by the library
 * @return Win32(HWND*), MacOS(NSWindow*), X11(Window*)
 */
VD_FW_API void*              vd_fw_get_internal_window_handle(void);

/* ----OPENGL SPECIFIC----------------------------------------------------------------------------------------------- */
/**
 * @brief Compile a GLSL shader and check for errors
 * @param  type   The shader type
 * @param  source The shader source code
 * @return        The shader handle
 */
VD_FW_API unsigned int       vd_fw_compile_shader(unsigned int type, const char *source);

/**
 * @brief Link a GL program and check for errors
 * @param  program The program to link
 * @return         1 on success, 0 otherwise
 */
VD_FW_API int                vd_fw_link_program(unsigned int program);

/**
 * @brief Compiles a program, if any of the shader sources have been modified. You should call this every frame
 * @param  program            Pointer to program (GLuint), initialize it to zero before rendering loop
 * @param  last_compile       Pointer to last compilation time, initialize it to zero before rendering loop, and don't use it in any other way
 * @param  vertex_file_path   The relative (or absolute) path to the vertex shader source
 * @param  fragment_file_path The relative (or absolute) path to the fragment shader source
 * @return                    1 if successful, 0 if encountered any breaking error. You don't really need to check this.
 */
VD_FW_API int                vd_fw_compile_or_hotload_program(unsigned int *program, unsigned long long *last_compile, const char *vertex_file_path, const char *fragment_file_path);

/**
 * @brief Construct an orthographic projection matrix
 * @param left   The left side
 * @param right  The right side
 * @param top    The top side
 * @param bottom The bottom side
 * @param near   The near plane
 * @param far    The far plane
 * @param out    The output matrix
 */
VD_FW_INL void            vd_fw_u_ortho(float left, float right, float bottom, float top, float near, float far, float out[16]);

/**
 * @brief Construct a perspective projection matrix
 * @param fov    The vertical fov, in degrees
 * @param aspect The aspect ratio
 * @param far    The far plane
 * @param near   The near plane
 * @param out    The output matrix
 */
VD_FW_INL void            vd_fw_u_perspective(float fov, float aspect, float near, float far, float out[16]);

/**
 * @brief Construct a view matrix
 * @param eye Position of the camera
 * @param target Look target position
 * @param updir The up direction
 * @param out The output matrix
 */
VD_FW_INL void            vd_fw_u_lookat(float eye[3], float target[3], float updir[3], float out[16]);

VD_FW_INL float vd_fw_delta_s(void)
{
    unsigned long long ns  = vd_fw_delta_ns();
    double ms              = (double)ns / 1000000.0;
    double sec64           = ms         / 1000.0;
    float s                = (float)sec64;
    return s;
}

VD_FW_INL int vd_fw_get_mouse_statef(float *x, float *y)
{
    int xi, yi;
    int result = vd_fw_get_mouse_state(&xi, &yi);

    if (x) *x = (float)xi;
    if (y) *y = (float)yi;

    return result;
}

VD_FW_INL int vd_fw_get_mouse_down(int button)
{
    return (vd_fw_get_mouse_state(NULL, NULL) & button) ? 1 : 0;
}

VD_FW_INL float vd_fw__fabs(float x)
{
    if (x < 0.f) return x;
    return x;
}

VD_FW_INL int vd_fw__cmp_float(float x, float y)
{
    float precision = 1.1920929e-07f;

    if ((x - precision) < y) {
        return -1;
    } else if ((x + precision) > y) {
        return 1;
    } else {
        return 0;
    }
}

VD_FW_INL float vd_fw_cos(float x)
{
    if (x < 0.f) {
        x = -x;
    }

    if (vd_fw__cmp_float(x, VD_FW_FPI2) >= 0) {
        do {
            x -= VD_FW_FPI2;
        } while(vd_fw__cmp_float(x, VD_FW_FPI2) >= 0);
    }

    if ((vd_fw__cmp_float(x, VD_FW_FPI) >= 0) && (vd_fw__cmp_float(x, VD_FW_FPI2) == -1)) {
        x -= VD_FW_FPI;

        return -1.f * (1.0f - (x*x/2.0f)*( 1.0f - (x*x/12.0f) * ( 1.0f - (x*x/30.0f) * (1.0f - (x*x/56.0f )*(1.0f - (x*x/90.0f)*(1.0f - (x*x/132.0f)*(1.0f - (x*x/182.0f))))))));
    } else {
        return 1.0f - (x*x/2.0f)*( 1.0f - (x*x/12.0f) * ( 1.0f - (x*x/30.0f) * (1.0f - (x*x/56.0f )*(1.0f - (x*x/90.0f)*(1.0f - (x*x/132.0f)*(1.0f - (x*x/182.0f)))))));
    }
}

VD_FW_INL float vd_fw_sin(float x)
{
    return vd_fw_cos(x - VD_FW_FPIH);
}

VD_FW_INL float vd_fw_tan(float x)
{

    float x2 = x * x;
    return x * (1.0f
               + x2 * ( 1.0f/3.0f
               + x2 * ( 2.0f/15.0f
               + x2 * ( 17.0f/315.0f
               + x2 * ( 62.0f/2835.0f
               + x2 * ( 1382.0f/155925.0f
               + x2 * ( 21844.0f/6081075.0f
               )))))) );
}

VD_FW_INL float vd_fw_sqrt(float x)
{
    if (x <= 0.0f) return 0.0f; // Handle non-positive inputs safely

    // Initial guess using bit manipulation
    union { float f; unsigned int i; } u = { x };
    u.i = (u.i >> 1) + 0x1fc00000; // Approximate initial guess for sqrt

    // One Newton–Raphson refinement step
    // g = u.f is our initial guess
    float g = u.f;
    g = 0.5f * (g + x / g);

    return g; 
}

VD_FW_INL void vd_fw_u_ortho(float left, float right, float bottom, float top, float near, float far, float out[16])
{
    out[0]  = 2.0f / (right - left);               out[1]  = 0.0f;                              out[2]  = 0.0f;                          out[3]  = 0.0f;
    out[4]  = 0.0f;                                out[5]  = 2.0f / (top - bottom);             out[6]  = 0.0f;                          out[7]  = 0.0f;
    out[8]  = 0.0f;                                out[9]  = 0.0f;                              out[10] = -2.0f / (far - near);          out[11] = 0.0f;
    out[12] = - (right + left) / (right - left);   out[13] = - (top + bottom) / (top - bottom); out[14] = - (far + near) / (far - near); out[15] = 1.0f;
}

VD_FW_INL void vd_fw_u_perspective(float fov, float aspect, float near, float far, float out[16])
{
    float fovrad = (fov / 2.f) * (VD_FW_FPI / 180.f);
    float tangent = VD_FW_TAN(fovrad);
    float top   = near * tangent;
    float right = top * aspect;

    out[0]  = near / right;
    out[5]  = near / top;
    out[10] = -(far + near) / (far - near);
    out[11] = -1.f;
    out[14] = -(2.f * far * near) / (far - near);
    out[15] = 0.f;
}

VD_FW_INL void vd_fw_u_lookat(float eye[3], float target[3], float updir[3], float out[16])
{
    // Forward vector (camera looks along -Z)
    float forward[3] = { target[0] - eye[0], target[1] - eye[1], target[2] - eye[2] };
    float f_len = VD_FW_SQRT(forward[0]*forward[0] + forward[1]*forward[1] + forward[2]*forward[2]);
    forward[0]/=f_len; forward[1]/=f_len; forward[2]/=f_len;

    // Left vector
    float left[3] = {
        updir[1]*forward[2] - updir[2]*forward[1],
        updir[2]*forward[0] - updir[0]*forward[2],
        updir[0]*forward[1] - updir[1]*forward[0]
    };
    float l_len = VD_FW_SQRT(left[0]*left[0] + left[1]*left[1] + left[2]*left[2]);
    left[0]/=l_len; left[1]/=l_len; left[2]/=l_len;

    // Recompute up vector
    float up[3] = {
        forward[1]*left[2] - forward[2]*left[1],
        forward[2]*left[0] - forward[0]*left[2],
        forward[0]*left[1] - forward[1]*left[0]
    };

    // Column-major matrix
    out[0] = left[0];      out[4] = left[1];     out[8]  = left[2];     out[12] = - (left[0]    * eye[0] + left[1]    * eye[1] + left[2]    * eye[2]);
    out[1] = up[0];        out[5] = up[1];       out[9]  = up[2];       out[13] = - (up[0]      * eye[0] + up[1]      * eye[1] + up[2]      * eye[2]);
    out[2] = -forward[0];  out[6] = -forward[1]; out[10] = -forward[2]; out[14] =    forward[0] * eye[0] + forward[1] * eye[1] + forward[2] * eye[2];
    out[3] = 0.0f;         out[7] = 0.0f;        out[11] = 0.0f;        out[15] = 1.0f;
}

VD_FW_INL void *vd_fw_memcpy(void *dst, void *src, size_t count)
{
    for (size_t i = 0; i < count; ++i) ((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
    return dst;
}

VD_FW_INL void *vd_fw_memset(void *dst, unsigned char val, size_t num)
{
    for (size_t i = 0; i < num; ++i) ((unsigned char *)dst)[i] = val;
    return dst;
}

/* ----INTERNAL API-------------------------------------------------------------------------------------------------- */
VD_FW_API int      vd_fw__any_time_higher(int num_files, const char **files, unsigned long long *check_against);
VD_FW_API char*    vd_fw__debug_dump_file_text(const char *path);
VD_FW_API void*    vd_fw__realloc_mem(void *prev_ptr, size_t size);
VD_FW_API void     vd_fw__free_mem(void *memory);
VD_FW_API void*    vd_fw__resize_buffer(void *buffer, size_t element_size, int required_capacity, int *cap);
VD_FW_API void     vd_fw__def_gamepad(VdFwGamepadMap *map);
VD_FW_API int      vd_fw__map_gamepad(VdFwGuid guid, VdFwGamepadMap *map);
VD_FW_API VdFwU16  vd_fw__crc16(unsigned short crc, void *data, VdFwSz len);
VD_FW_API VdFwGuid vd_fw__make_gamepad_guid(VdFwU16 bus, VdFwU16 vendor, VdFwU16 product, VdFwU16 version,
                                            char *vendor_name, char *product_name,
                                            VdFwU8 driver_signature, VdFwU8 driver_data);
VD_FW_API void     vd_fw__lock_gamepaddb(void);
VD_FW_API void     vd_fw__unlock_gamepaddb(void);
VD_FW_API void     vd_fw__notify_gamepaddb_changed(void);
VD_FW_INL int      vd_fw__strlen(const char *s);
VD_FW_INL size_t   vd_fw__strlcpy(char *dst, const char *src, size_t maxlen);

VD_FW_INL int vd_fw__strlen(const char *s)
{
    int r = 0;
    while (*s++) r++;
    return r;
}

VD_FW_INL size_t vd_fw__strlcpy(char *dst, const char *src, size_t maxlen)
{
    size_t srclen = vd_fw__strlen(src);
    if (maxlen > 0) {
        size_t len = srclen < (maxlen - 1) ? srclen : (maxlen - 1);
        VD_FW_MEMCPY(dst, (void*)src, len);
        dst[len] = '\0';
    }
    return srclen;
}

#if _WIN32
#   define VD_FW_WIN32_SUBSYSTEM_CONSOLE 1
#   define VD_FW_WIN32_SUBSYSTEM_WINDOWS 2
#   ifndef VD_FW_WIN32_SUBSYSTEM
#       define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#   endif // !VD_FW_WIN32_SUBSYSTEM
#endif // _WIN32

#ifdef VD_FW_WIN32_NO_LINKER_COMMENTS
#   define VD_FW_WIN32_LINKER_COMMENTS 0
#else
#   define VD_FW_WIN32_LINKER_COMMENTS 1
#endif // !VD_FW_WIN32_NO_LINKER_COMMENTS

#if defined(__APPLE__)

#ifndef VD_FW_MACOS_NO_MAIN_OVERRIDE
#   define main vd_fw__macos_main
#endif // !VD_FW_MACOS_NO_MAIN_OVERRIDE

#ifndef GL_SILENCE_DEPRECATION
#   define GL_SILENCE_DEPRECATION
#endif // !GL_SILENCE_DEPRECATION
#import <OpenGL/gl3.h>
#else
/* ----GL TYPEDEFS--------------------------------------------------------------------------------------------------- */
typedef void               GLvoid;
typedef unsigned int       GLenum;
typedef float              GLfloat;
typedef int                GLint;
typedef int                GLsizei;
typedef unsigned int       GLbitfield;
typedef double             GLdouble;
typedef unsigned int       GLuint;
typedef unsigned char      GLboolean;
typedef unsigned char      GLubyte;
typedef float              GLclampf;
typedef double             GLclampd;
typedef char               GLbyte;
typedef signed short       GLshort;
typedef unsigned short     GLushort;
typedef signed long int    GLsizeiptr;
typedef signed long int    GLintptr;
typedef char               GLchar;
typedef unsigned short int GLhalf;
typedef struct __GLsync *  GLsync;
typedef int                GLfixed;
typedef int                GLclampx;
#ifdef _WIN32
typedef unsigned __int64   GLuint64;
typedef __int64            GLint64;
#else
#include <stdint.h>
typedef uint64_t GLuint64;
typedef int64_t  GLint64;
#endif // _WIN32
typedef void (*GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);

/* ----GL CONSTANTS-------------------------------------------------------------------------------------------------- */
#define GL_DEPTH_BUFFER_BIT                                           0x00000100
#define GL_STENCIL_BUFFER_BIT                                         0x00000400
#define GL_COLOR_BUFFER_BIT                                           0x00004000
#define GL_FALSE                                                      0
#define GL_TRUE                                                       1
#define GL_POINTS                                                     0x0000
#define GL_LINES                                                      0x0001
#define GL_LINE_LOOP                                                  0x0002
#define GL_LINE_STRIP                                                 0x0003
#define GL_TRIANGLES                                                  0x0004
#define GL_TRIANGLE_STRIP                                             0x0005
#define GL_TRIANGLE_FAN                                               0x0006
#define GL_QUADS                                                      0x0007
#define GL_NEVER                                                      0x0200
#define GL_LESS                                                       0x0201
#define GL_EQUAL                                                      0x0202
#define GL_LEQUAL                                                     0x0203
#define GL_GREATER                                                    0x0204
#define GL_NOTEQUAL                                                   0x0205
#define GL_GEQUAL                                                     0x0206
#define GL_ALWAYS                                                     0x0207
#define GL_ZERO                                                       0
#define GL_ONE                                                        1
#define GL_SRC_COLOR                                                  0x0300
#define GL_ONE_MINUS_SRC_COLOR                                        0x0301
#define GL_SRC_ALPHA                                                  0x0302
#define GL_ONE_MINUS_SRC_ALPHA                                        0x0303
#define GL_DST_ALPHA                                                  0x0304
#define GL_ONE_MINUS_DST_ALPHA                                        0x0305
#define GL_DST_COLOR                                                  0x0306
#define GL_ONE_MINUS_DST_COLOR                                        0x0307
#define GL_SRC_ALPHA_SATURATE                                         0x0308
#define GL_NONE                                                       0
#define GL_FRONT_LEFT                                                 0x0400
#define GL_FRONT_RIGHT                                                0x0401
#define GL_BACK_LEFT                                                  0x0402
#define GL_BACK_RIGHT                                                 0x0403
#define GL_FRONT                                                      0x0404
#define GL_BACK                                                       0x0405
#define GL_LEFT                                                       0x0406
#define GL_RIGHT                                                      0x0407
#define GL_FRONT_AND_BACK                                             0x0408
#define GL_NO_ERROR                                                   0
#define GL_INVALID_ENUM                                               0x0500
#define GL_INVALID_VALUE                                              0x0501
#define GL_INVALID_OPERATION                                          0x0502
#define GL_OUT_OF_MEMORY                                              0x0505
#define GL_CW                                                         0x0900
#define GL_CCW                                                        0x0901
#define GL_POINT_SIZE                                                 0x0B11
#define GL_POINT_SIZE_RANGE                                           0x0B12
#define GL_POINT_SIZE_GRANULARITY                                     0x0B13
#define GL_LINE_SMOOTH                                                0x0B20
#define GL_LINE_WIDTH                                                 0x0B21
#define GL_LINE_WIDTH_RANGE                                           0x0B22
#define GL_LINE_WIDTH_GRANULARITY                                     0x0B23
#define GL_POLYGON_MODE                                               0x0B40
#define GL_POLYGON_SMOOTH                                             0x0B41
#define GL_CULL_FACE                                                  0x0B44
#define GL_CULL_FACE_MODE                                             0x0B45
#define GL_FRONT_FACE                                                 0x0B46
#define GL_DEPTH_RANGE                                                0x0B70
#define GL_DEPTH_TEST                                                 0x0B71
#define GL_DEPTH_WRITEMASK                                            0x0B72
#define GL_DEPTH_CLEAR_VALUE                                          0x0B73
#define GL_DEPTH_FUNC                                                 0x0B74
#define GL_STENCIL_TEST                                               0x0B90
#define GL_STENCIL_CLEAR_VALUE                                        0x0B91
#define GL_STENCIL_FUNC                                               0x0B92
#define GL_STENCIL_VALUE_MASK                                         0x0B93
#define GL_STENCIL_FAIL                                               0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL                                    0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS                                    0x0B96
#define GL_STENCIL_REF                                                0x0B97
#define GL_STENCIL_WRITEMASK                                          0x0B98
#define GL_VIEWPORT                                                   0x0BA2
#define GL_DITHER                                                     0x0BD0
#define GL_BLEND_DST                                                  0x0BE0
#define GL_BLEND_SRC                                                  0x0BE1
#define GL_BLEND                                                      0x0BE2
#define GL_LOGIC_OP_MODE                                              0x0BF0
#define GL_DRAW_BUFFER                                                0x0C01
#define GL_READ_BUFFER                                                0x0C02
#define GL_SCISSOR_BOX                                                0x0C10
#define GL_SCISSOR_TEST                                               0x0C11
#define GL_COLOR_CLEAR_VALUE                                          0x0C22
#define GL_COLOR_WRITEMASK                                            0x0C23
#define GL_DOUBLEBUFFER                                               0x0C32
#define GL_STEREO                                                     0x0C33
#define GL_LINE_SMOOTH_HINT                                           0x0C52
#define GL_POLYGON_SMOOTH_HINT                                        0x0C53
#define GL_UNPACK_SWAP_BYTES                                          0x0CF0
#define GL_UNPACK_LSB_FIRST                                           0x0CF1
#define GL_UNPACK_ROW_LENGTH                                          0x0CF2
#define GL_UNPACK_SKIP_ROWS                                           0x0CF3
#define GL_UNPACK_SKIP_PIXELS                                         0x0CF4
#define GL_UNPACK_ALIGNMENT                                           0x0CF5
#define GL_PACK_SWAP_BYTES                                            0x0D00
#define GL_PACK_LSB_FIRST                                             0x0D01
#define GL_PACK_ROW_LENGTH                                            0x0D02
#define GL_PACK_SKIP_ROWS                                             0x0D03
#define GL_PACK_SKIP_PIXELS                                           0x0D04
#define GL_PACK_ALIGNMENT                                             0x0D05
#define GL_MAX_TEXTURE_SIZE                                           0x0D33
#define GL_MAX_VIEWPORT_DIMS                                          0x0D3A
#define GL_SUBPIXEL_BITS                                              0x0D50
#define GL_TEXTURE_1D                                                 0x0DE0
#define GL_TEXTURE_2D                                                 0x0DE1
#define GL_TEXTURE_WIDTH                                              0x1000
#define GL_TEXTURE_HEIGHT                                             0x1001
#define GL_TEXTURE_BORDER_COLOR                                       0x1004
#define GL_DONT_CARE                                                  0x1100
#define GL_FASTEST                                                    0x1101
#define GL_NICEST                                                     0x1102
#define GL_BYTE                                                       0x1400
#define GL_UNSIGNED_BYTE                                              0x1401
#define GL_SHORT                                                      0x1402
#define GL_UNSIGNED_SHORT                                             0x1403
#define GL_INT                                                        0x1404
#define GL_UNSIGNED_INT                                               0x1405
#define GL_FLOAT                                                      0x1406
#define GL_STACK_OVERFLOW                                             0x0503
#define GL_STACK_UNDERFLOW                                            0x0504
#define GL_CLEAR                                                      0x1500
#define GL_AND                                                        0x1501
#define GL_AND_REVERSE                                                0x1502
#define GL_COPY                                                       0x1503
#define GL_AND_INVERTED                                               0x1504
#define GL_NOOP                                                       0x1505
#define GL_XOR                                                        0x1506
#define GL_OR                                                         0x1507
#define GL_NOR                                                        0x1508
#define GL_EQUIV                                                      0x1509
#define GL_INVERT                                                     0x150A
#define GL_OR_REVERSE                                                 0x150B
#define GL_COPY_INVERTED                                              0x150C
#define GL_OR_INVERTED                                                0x150D
#define GL_NAND                                                       0x150E
#define GL_SET                                                        0x150F
#define GL_TEXTURE                                                    0x1702
#define GL_COLOR                                                      0x1800
#define GL_DEPTH                                                      0x1801
#define GL_STENCIL                                                    0x1802
#define GL_STENCIL_INDEX                                              0x1901
#define GL_DEPTH_COMPONENT                                            0x1902
#define GL_RED                                                        0x1903
#define GL_GREEN                                                      0x1904
#define GL_BLUE                                                       0x1905
#define GL_ALPHA                                                      0x1906
#define GL_RGB                                                        0x1907
#define GL_RGBA                                                       0x1908
#define GL_POINT                                                      0x1B00
#define GL_LINE                                                       0x1B01
#define GL_FILL                                                       0x1B02
#define GL_KEEP                                                       0x1E00
#define GL_REPLACE                                                    0x1E01
#define GL_INCR                                                       0x1E02
#define GL_DECR                                                       0x1E03
#define GL_VENDOR                                                     0x1F00
#define GL_RENDERER                                                   0x1F01
#define GL_VERSION                                                    0x1F02
#define GL_EXTENSIONS                                                 0x1F03
#define GL_NEAREST                                                    0x2600
#define GL_LINEAR                                                     0x2601
#define GL_NEAREST_MIPMAP_NEAREST                                     0x2700
#define GL_LINEAR_MIPMAP_NEAREST                                      0x2701
#define GL_NEAREST_MIPMAP_LINEAR                                      0x2702
#define GL_LINEAR_MIPMAP_LINEAR                                       0x2703
#define GL_TEXTURE_MAG_FILTER                                         0x2800
#define GL_TEXTURE_MIN_FILTER                                         0x2801
#define GL_TEXTURE_WRAP_S                                             0x2802
#define GL_TEXTURE_WRAP_T                                             0x2803
#define GL_REPEAT                                                     0x2901
#define GL_COLOR_LOGIC_OP                                             0x0BF2
#define GL_POLYGON_OFFSET_UNITS                                       0x2A00
#define GL_POLYGON_OFFSET_POINT                                       0x2A01
#define GL_POLYGON_OFFSET_LINE                                        0x2A02
#define GL_POLYGON_OFFSET_FILL                                        0x8037
#define GL_POLYGON_OFFSET_FACTOR                                      0x8038
#define GL_TEXTURE_BINDING_1D                                         0x8068
#define GL_TEXTURE_BINDING_2D                                         0x8069
#define GL_TEXTURE_INTERNAL_FORMAT                                    0x1003
#define GL_TEXTURE_RED_SIZE                                           0x805C
#define GL_TEXTURE_GREEN_SIZE                                         0x805D
#define GL_TEXTURE_BLUE_SIZE                                          0x805E
#define GL_TEXTURE_ALPHA_SIZE                                         0x805F
#define GL_DOUBLE                                                     0x140A
#define GL_PROXY_TEXTURE_1D                                           0x8063
#define GL_PROXY_TEXTURE_2D                                           0x8064
#define GL_R3_G3_B2                                                   0x2A10
#define GL_RGB4                                                       0x804F
#define GL_RGB5                                                       0x8050
#define GL_RGB8                                                       0x8051
#define GL_RGB10                                                      0x8052
#define GL_RGB12                                                      0x8053
#define GL_RGB16                                                      0x8054
#define GL_RGBA2                                                      0x8055
#define GL_RGBA4                                                      0x8056
#define GL_RGB5_A1                                                    0x8057
#define GL_RGBA8                                                      0x8058
#define GL_RGB10_A2                                                   0x8059
#define GL_RGBA12                                                     0x805A
#define GL_RGBA16                                                     0x805B
#define GL_VERTEX_ARRAY                                               0x8074
#define GL_UNSIGNED_BYTE_3_3_2                                        0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4                                     0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1                                     0x8034
#define GL_UNSIGNED_INT_8_8_8_8                                       0x8035
#define GL_UNSIGNED_INT_10_10_10_2                                    0x8036
#define GL_TEXTURE_BINDING_3D                                         0x806A
#define GL_PACK_SKIP_IMAGES                                           0x806B
#define GL_PACK_IMAGE_HEIGHT                                          0x806C
#define GL_UNPACK_SKIP_IMAGES                                         0x806D
#define GL_UNPACK_IMAGE_HEIGHT                                        0x806E
#define GL_TEXTURE_3D                                                 0x806F
#define GL_PROXY_TEXTURE_3D                                           0x8070
#define GL_TEXTURE_DEPTH                                              0x8071
#define GL_TEXTURE_WRAP_R                                             0x8072
#define GL_MAX_3D_TEXTURE_SIZE                                        0x8073
#define GL_UNSIGNED_BYTE_2_3_3_REV                                    0x8362
#define GL_UNSIGNED_SHORT_5_6_5                                       0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV                                   0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV                                 0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV                                 0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV                                   0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV                                0x8368
#define GL_BGR                                                        0x80E0
#define GL_BGRA                                                       0x80E1
#define GL_MAX_ELEMENTS_VERTICES                                      0x80E8
#define GL_MAX_ELEMENTS_INDICES                                       0x80E9
#define GL_CLAMP_TO_EDGE                                              0x812F
#define GL_TEXTURE_MIN_LOD                                            0x813A
#define GL_TEXTURE_MAX_LOD                                            0x813B
#define GL_TEXTURE_BASE_LEVEL                                         0x813C
#define GL_TEXTURE_MAX_LEVEL                                          0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE                                    0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY                              0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE                                    0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY                              0x0B23
#define GL_ALIASED_LINE_WIDTH_RANGE                                   0x846E
#define GL_TEXTURE0                                                   0x84C0
#define GL_TEXTURE1                                                   0x84C1
#define GL_TEXTURE2                                                   0x84C2
#define GL_TEXTURE3                                                   0x84C3
#define GL_TEXTURE4                                                   0x84C4
#define GL_TEXTURE5                                                   0x84C5
#define GL_TEXTURE6                                                   0x84C6
#define GL_TEXTURE7                                                   0x84C7
#define GL_TEXTURE8                                                   0x84C8
#define GL_TEXTURE9                                                   0x84C9
#define GL_TEXTURE10                                                  0x84CA
#define GL_TEXTURE11                                                  0x84CB
#define GL_TEXTURE12                                                  0x84CC
#define GL_TEXTURE13                                                  0x84CD
#define GL_TEXTURE14                                                  0x84CE
#define GL_TEXTURE15                                                  0x84CF
#define GL_TEXTURE16                                                  0x84D0
#define GL_TEXTURE17                                                  0x84D1
#define GL_TEXTURE18                                                  0x84D2
#define GL_TEXTURE19                                                  0x84D3
#define GL_TEXTURE20                                                  0x84D4
#define GL_TEXTURE21                                                  0x84D5
#define GL_TEXTURE22                                                  0x84D6
#define GL_TEXTURE23                                                  0x84D7
#define GL_TEXTURE24                                                  0x84D8
#define GL_TEXTURE25                                                  0x84D9
#define GL_TEXTURE26                                                  0x84DA
#define GL_TEXTURE27                                                  0x84DB
#define GL_TEXTURE28                                                  0x84DC
#define GL_TEXTURE29                                                  0x84DD
#define GL_TEXTURE30                                                  0x84DE
#define GL_TEXTURE31                                                  0x84DF
#define GL_ACTIVE_TEXTURE                                             0x84E0
#define GL_MULTISAMPLE                                                0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE                                   0x809E
#define GL_SAMPLE_ALPHA_TO_ONE                                        0x809F
#define GL_SAMPLE_COVERAGE                                            0x80A0
#define GL_SAMPLE_BUFFERS                                             0x80A8
#define GL_SAMPLES                                                    0x80A9
#define GL_SAMPLE_COVERAGE_VALUE                                      0x80AA
#define GL_SAMPLE_COVERAGE_INVERT                                     0x80AB
#define GL_TEXTURE_CUBE_MAP                                           0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP                                   0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X                                0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X                                0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y                                0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y                                0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z                                0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z                                0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP                                     0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE                                  0x851C
#define GL_COMPRESSED_RGB                                             0x84ED
#define GL_COMPRESSED_RGBA                                            0x84EE
#define GL_TEXTURE_COMPRESSION_HINT                                   0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE                              0x86A0
#define GL_TEXTURE_COMPRESSED                                         0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS                             0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS                                 0x86A3
#define GL_CLAMP_TO_BORDER                                            0x812D
#define GL_BLEND_DST_RGB                                              0x80C8
#define GL_BLEND_SRC_RGB                                              0x80C9
#define GL_BLEND_DST_ALPHA                                            0x80CA
#define GL_BLEND_SRC_ALPHA                                            0x80CB
#define GL_POINT_FADE_THRESHOLD_SIZE                                  0x8128
#define GL_DEPTH_COMPONENT16                                          0x81A5
#define GL_DEPTH_COMPONENT24                                          0x81A6
#define GL_DEPTH_COMPONENT32                                          0x81A7
#define GL_MIRRORED_REPEAT                                            0x8370
#define GL_MAX_TEXTURE_LOD_BIAS                                       0x84FD
#define GL_TEXTURE_LOD_BIAS                                           0x8501
#define GL_INCR_WRAP                                                  0x8507
#define GL_DECR_WRAP                                                  0x8508
#define GL_TEXTURE_DEPTH_SIZE                                         0x884A
#define GL_TEXTURE_COMPARE_MODE                                       0x884C
#define GL_TEXTURE_COMPARE_FUNC                                       0x884D
#define GL_BLEND_COLOR                                                0x8005
#define GL_BLEND_EQUATION                                             0x8009
#define GL_CONSTANT_COLOR                                             0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR                                   0x8002
#define GL_CONSTANT_ALPHA                                             0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA                                   0x8004
#define GL_FUNC_ADD                                                   0x8006
#define GL_FUNC_REVERSE_SUBTRACT                                      0x800B
#define GL_FUNC_SUBTRACT                                              0x800A
#define GL_MIN                                                        0x8007
#define GL_MAX                                                        0x8008
#define GL_BUFFER_SIZE                                                0x8764
#define GL_BUFFER_USAGE                                               0x8765
#define GL_QUERY_COUNTER_BITS                                         0x8864
#define GL_CURRENT_QUERY                                              0x8865
#define GL_QUERY_RESULT                                               0x8866
#define GL_QUERY_RESULT_AVAILABLE                                     0x8867
#define GL_ARRAY_BUFFER                                               0x8892
#define GL_ELEMENT_ARRAY_BUFFER                                       0x8893
#define GL_ARRAY_BUFFER_BINDING                                       0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING                               0x8895
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING                         0x889F
#define GL_READ_ONLY                                                  0x88B8
#define GL_WRITE_ONLY                                                 0x88B9
#define GL_READ_WRITE                                                 0x88BA
#define GL_BUFFER_ACCESS                                              0x88BB
#define GL_BUFFER_MAPPED                                              0x88BC
#define GL_BUFFER_MAP_POINTER                                         0x88BD
#define GL_STREAM_DRAW                                                0x88E0
#define GL_STREAM_READ                                                0x88E1
#define GL_STREAM_COPY                                                0x88E2
#define GL_STATIC_DRAW                                                0x88E4
#define GL_STATIC_READ                                                0x88E5
#define GL_STATIC_COPY                                                0x88E6
#define GL_DYNAMIC_DRAW                                               0x88E8
#define GL_DYNAMIC_READ                                               0x88E9
#define GL_DYNAMIC_COPY                                               0x88EA
#define GL_SAMPLES_PASSED                                             0x8914
#define GL_SRC1_ALPHA                                                 0x8589
#define GL_BLEND_EQUATION_RGB                                         0x8009
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED                                0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE                                   0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE                                 0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE                                   0x8625
#define GL_CURRENT_VERTEX_ATTRIB                                      0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE                                  0x8642
#define GL_VERTEX_ATTRIB_ARRAY_POINTER                                0x8645
#define GL_STENCIL_BACK_FUNC                                          0x8800
#define GL_STENCIL_BACK_FAIL                                          0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL                               0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS                               0x8803
#define GL_MAX_DRAW_BUFFERS                                           0x8824
#define GL_DRAW_BUFFER0                                               0x8825
#define GL_DRAW_BUFFER1                                               0x8826
#define GL_DRAW_BUFFER2                                               0x8827
#define GL_DRAW_BUFFER3                                               0x8828
#define GL_DRAW_BUFFER4                                               0x8829
#define GL_DRAW_BUFFER5                                               0x882A
#define GL_DRAW_BUFFER6                                               0x882B
#define GL_DRAW_BUFFER7                                               0x882C
#define GL_DRAW_BUFFER8                                               0x882D
#define GL_DRAW_BUFFER9                                               0x882E
#define GL_DRAW_BUFFER10                                              0x882F
#define GL_DRAW_BUFFER11                                              0x8830
#define GL_DRAW_BUFFER12                                              0x8831
#define GL_DRAW_BUFFER13                                              0x8832
#define GL_DRAW_BUFFER14                                              0x8833
#define GL_DRAW_BUFFER15                                              0x8834
#define GL_BLEND_EQUATION_ALPHA                                       0x883D
#define GL_MAX_VERTEX_ATTRIBS                                         0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED                             0x886A
#define GL_MAX_TEXTURE_IMAGE_UNITS                                    0x8872
#define GL_FRAGMENT_SHADER                                            0x8B30
#define GL_VERTEX_SHADER                                              0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS                            0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS                              0x8B4A
#define GL_MAX_VARYING_FLOATS                                         0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS                             0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS                           0x8B4D
#define GL_SHADER_TYPE                                                0x8B4F
#define GL_FLOAT_VEC2                                                 0x8B50
#define GL_FLOAT_VEC3                                                 0x8B51
#define GL_FLOAT_VEC4                                                 0x8B52
#define GL_INT_VEC2                                                   0x8B53
#define GL_INT_VEC3                                                   0x8B54
#define GL_INT_VEC4                                                   0x8B55
#define GL_BOOL                                                       0x8B56
#define GL_BOOL_VEC2                                                  0x8B57
#define GL_BOOL_VEC3                                                  0x8B58
#define GL_BOOL_VEC4                                                  0x8B59
#define GL_FLOAT_MAT2                                                 0x8B5A
#define GL_FLOAT_MAT3                                                 0x8B5B
#define GL_FLOAT_MAT4                                                 0x8B5C
#define GL_SAMPLER_1D                                                 0x8B5D
#define GL_SAMPLER_2D                                                 0x8B5E
#define GL_SAMPLER_3D                                                 0x8B5F
#define GL_SAMPLER_CUBE                                               0x8B60
#define GL_SAMPLER_1D_SHADOW                                          0x8B61
#define GL_SAMPLER_2D_SHADOW                                          0x8B62
#define GL_DELETE_STATUS                                              0x8B80
#define GL_COMPILE_STATUS                                             0x8B81
#define GL_LINK_STATUS                                                0x8B82
#define GL_VALIDATE_STATUS                                            0x8B83
#define GL_INFO_LOG_LENGTH                                            0x8B84
#define GL_ATTACHED_SHADERS                                           0x8B85
#define GL_ACTIVE_UNIFORMS                                            0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH                                  0x8B87
#define GL_SHADER_SOURCE_LENGTH                                       0x8B88
#define GL_ACTIVE_ATTRIBUTES                                          0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH                                0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT                            0x8B8B
#define GL_SHADING_LANGUAGE_VERSION                                   0x8B8C
#define GL_CURRENT_PROGRAM                                            0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN                                  0x8CA0
#define GL_LOWER_LEFT                                                 0x8CA1
#define GL_UPPER_LEFT                                                 0x8CA2
#define GL_STENCIL_BACK_REF                                           0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK                                    0x8CA4
#define GL_STENCIL_BACK_WRITEMASK                                     0x8CA5
#define GL_PIXEL_PACK_BUFFER                                          0x88EB
#define GL_PIXEL_UNPACK_BUFFER                                        0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING                                  0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING                                0x88EF
#define GL_FLOAT_MAT2x3                                               0x8B65
#define GL_FLOAT_MAT2x4                                               0x8B66
#define GL_FLOAT_MAT3x2                                               0x8B67
#define GL_FLOAT_MAT3x4                                               0x8B68
#define GL_FLOAT_MAT4x2                                               0x8B69
#define GL_FLOAT_MAT4x3                                               0x8B6A
#define GL_SRGB                                                       0x8C40
#define GL_SRGB8                                                      0x8C41
#define GL_SRGB_ALPHA                                                 0x8C42
#define GL_SRGB8_ALPHA8                                               0x8C43
#define GL_COMPRESSED_SRGB                                            0x8C48
#define GL_COMPRESSED_SRGB_ALPHA                                      0x8C49
#define GL_COMPARE_REF_TO_TEXTURE                                     0x884E
#define GL_CLIP_DISTANCE0                                             0x3000
#define GL_CLIP_DISTANCE1                                             0x3001
#define GL_CLIP_DISTANCE2                                             0x3002
#define GL_CLIP_DISTANCE3                                             0x3003
#define GL_CLIP_DISTANCE4                                             0x3004
#define GL_CLIP_DISTANCE5                                             0x3005
#define GL_CLIP_DISTANCE6                                             0x3006
#define GL_CLIP_DISTANCE7                                             0x3007
#define GL_MAX_CLIP_DISTANCES                                         0x0D32
#define GL_MAJOR_VERSION                                              0x821B
#define GL_MINOR_VERSION                                              0x821C
#define GL_NUM_EXTENSIONS                                             0x821D
#define GL_CONTEXT_FLAGS                                              0x821E
#define GL_COMPRESSED_RED                                             0x8225
#define GL_COMPRESSED_RG                                              0x8226
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT                        0x00000001
#define GL_RGBA32F                                                    0x8814
#define GL_RGB32F                                                     0x8815
#define GL_RGBA16F                                                    0x881A
#define GL_RGB16F                                                     0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER                                0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS                                   0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET                                   0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET                                   0x8905
#define GL_CLAMP_READ_COLOR                                           0x891C
#define GL_FIXED_ONLY                                                 0x891D
#define GL_MAX_VARYING_COMPONENTS                                     0x8B4B
#define GL_TEXTURE_1D_ARRAY                                           0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY                                     0x8C19
#define GL_TEXTURE_2D_ARRAY                                           0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY                                     0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY                                   0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY                                   0x8C1D
#define GL_R11F_G11F_B10F                                             0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV                               0x8C3B
#define GL_RGB9_E5                                                    0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV                                   0x8C3E
#define GL_TEXTURE_SHARED_SIZE                                        0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH                      0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE                             0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS                 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS                                0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START                            0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE                             0x8C85
#define GL_PRIMITIVES_GENERATED                                       0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN                      0x8C88
#define GL_RASTERIZER_DISCARD                                         0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS              0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS                    0x8C8B
#define GL_INTERLEAVED_ATTRIBS                                        0x8C8C
#define GL_SEPARATE_ATTRIBS                                           0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER                                  0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING                          0x8C8F
#define GL_RGBA32UI                                                   0x8D70
#define GL_RGB32UI                                                    0x8D71
#define GL_RGBA16UI                                                   0x8D76
#define GL_RGB16UI                                                    0x8D77
#define GL_RGBA8UI                                                    0x8D7C
#define GL_RGB8UI                                                     0x8D7D
#define GL_RGBA32I                                                    0x8D82
#define GL_RGB32I                                                     0x8D83
#define GL_RGBA16I                                                    0x8D88
#define GL_RGB16I                                                     0x8D89
#define GL_RGBA8I                                                     0x8D8E
#define GL_RGB8I                                                      0x8D8F
#define GL_RED_INTEGER                                                0x8D94
#define GL_GREEN_INTEGER                                              0x8D95
#define GL_BLUE_INTEGER                                               0x8D96
#define GL_RGB_INTEGER                                                0x8D98
#define GL_RGBA_INTEGER                                               0x8D99
#define GL_BGR_INTEGER                                                0x8D9A
#define GL_BGRA_INTEGER                                               0x8D9B
#define GL_SAMPLER_1D_ARRAY                                           0x8DC0
#define GL_SAMPLER_2D_ARRAY                                           0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW                                    0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW                                    0x8DC4
#define GL_SAMPLER_CUBE_SHADOW                                        0x8DC5
#define GL_UNSIGNED_INT_VEC2                                          0x8DC6
#define GL_UNSIGNED_INT_VEC3                                          0x8DC7
#define GL_UNSIGNED_INT_VEC4                                          0x8DC8
#define GL_INT_SAMPLER_1D                                             0x8DC9
#define GL_INT_SAMPLER_2D                                             0x8DCA
#define GL_INT_SAMPLER_3D                                             0x8DCB
#define GL_INT_SAMPLER_CUBE                                           0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY                                       0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY                                       0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D                                    0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D                                    0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D                                    0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE                                  0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY                              0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY                              0x8DD7
#define GL_QUERY_WAIT                                                 0x8E13
#define GL_QUERY_NO_WAIT                                              0x8E14
#define GL_QUERY_BY_REGION_WAIT                                       0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT                                    0x8E16
#define GL_BUFFER_ACCESS_FLAGS                                        0x911F
#define GL_BUFFER_MAP_LENGTH                                          0x9120
#define GL_BUFFER_MAP_OFFSET                                          0x9121
#define GL_DEPTH_COMPONENT32F                                         0x8CAC
#define GL_DEPTH32F_STENCIL8                                          0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV                             0x8DAD
#define GL_INVALID_FRAMEBUFFER_OPERATION                              0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING                      0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE                      0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE                            0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE                          0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE                           0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE                          0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE                          0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE                        0x8217
#define GL_FRAMEBUFFER_DEFAULT                                        0x8218
#define GL_FRAMEBUFFER_UNDEFINED                                      0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT                                   0x821A
#define GL_MAX_RENDERBUFFER_SIZE                                      0x84E8
#define GL_DEPTH_STENCIL                                              0x84F9
#define GL_UNSIGNED_INT_24_8                                          0x84FA
#define GL_DEPTH24_STENCIL8                                           0x88F0
#define GL_TEXTURE_STENCIL_SIZE                                       0x88F1
#define GL_TEXTURE_RED_TYPE                                           0x8C10
#define GL_TEXTURE_GREEN_TYPE                                         0x8C11
#define GL_TEXTURE_BLUE_TYPE                                          0x8C12
#define GL_TEXTURE_ALPHA_TYPE                                         0x8C13
#define GL_TEXTURE_DEPTH_TYPE                                         0x8C16
#define GL_UNSIGNED_NORMALIZED                                        0x8C17
#define GL_FRAMEBUFFER_BINDING                                        0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING                                   0x8CA6
#define GL_RENDERBUFFER_BINDING                                       0x8CA7
#define GL_READ_FRAMEBUFFER                                           0x8CA8
#define GL_DRAW_FRAMEBUFFER                                           0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING                                   0x8CAA
#define GL_RENDERBUFFER_SAMPLES                                       0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE                         0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME                         0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL                       0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE               0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER                       0x8CD4
#define GL_FRAMEBUFFER_COMPLETE                                       0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT                          0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT                  0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER                         0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER                         0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED                                    0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS                                      0x8CDF
#define GL_COLOR_ATTACHMENT0                                          0x8CE0
#define GL_COLOR_ATTACHMENT1                                          0x8CE1
#define GL_COLOR_ATTACHMENT2                                          0x8CE2
#define GL_COLOR_ATTACHMENT3                                          0x8CE3
#define GL_COLOR_ATTACHMENT4                                          0x8CE4
#define GL_COLOR_ATTACHMENT5                                          0x8CE5
#define GL_COLOR_ATTACHMENT6                                          0x8CE6
#define GL_COLOR_ATTACHMENT7                                          0x8CE7
#define GL_COLOR_ATTACHMENT8                                          0x8CE8
#define GL_COLOR_ATTACHMENT9                                          0x8CE9
#define GL_COLOR_ATTACHMENT10                                         0x8CEA
#define GL_COLOR_ATTACHMENT11                                         0x8CEB
#define GL_COLOR_ATTACHMENT12                                         0x8CEC
#define GL_COLOR_ATTACHMENT13                                         0x8CED
#define GL_COLOR_ATTACHMENT14                                         0x8CEE
#define GL_COLOR_ATTACHMENT15                                         0x8CEF
#define GL_COLOR_ATTACHMENT16                                         0x8CF0
#define GL_COLOR_ATTACHMENT17                                         0x8CF1
#define GL_COLOR_ATTACHMENT18                                         0x8CF2
#define GL_COLOR_ATTACHMENT19                                         0x8CF3
#define GL_COLOR_ATTACHMENT20                                         0x8CF4
#define GL_COLOR_ATTACHMENT21                                         0x8CF5
#define GL_COLOR_ATTACHMENT22                                         0x8CF6
#define GL_COLOR_ATTACHMENT23                                         0x8CF7
#define GL_COLOR_ATTACHMENT24                                         0x8CF8
#define GL_COLOR_ATTACHMENT25                                         0x8CF9
#define GL_COLOR_ATTACHMENT26                                         0x8CFA
#define GL_COLOR_ATTACHMENT27                                         0x8CFB
#define GL_COLOR_ATTACHMENT28                                         0x8CFC
#define GL_COLOR_ATTACHMENT29                                         0x8CFD
#define GL_COLOR_ATTACHMENT30                                         0x8CFE
#define GL_COLOR_ATTACHMENT31                                         0x8CFF
#define GL_DEPTH_ATTACHMENT                                           0x8D00
#define GL_STENCIL_ATTACHMENT                                         0x8D20
#define GL_FRAMEBUFFER                                                0x8D40
#define GL_RENDERBUFFER                                               0x8D41
#define GL_RENDERBUFFER_WIDTH                                         0x8D42
#define GL_RENDERBUFFER_HEIGHT                                        0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT                               0x8D44
#define GL_STENCIL_INDEX1                                             0x8D46
#define GL_STENCIL_INDEX4                                             0x8D47
#define GL_STENCIL_INDEX8                                             0x8D48
#define GL_STENCIL_INDEX16                                            0x8D49
#define GL_RENDERBUFFER_RED_SIZE                                      0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE                                    0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE                                     0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE                                    0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE                                    0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE                                  0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE                         0x8D56
#define GL_MAX_SAMPLES                                                0x8D57
#define GL_FRAMEBUFFER_SRGB                                           0x8DB9
#define GL_HALF_FLOAT                                                 0x140B
#define GL_MAP_READ_BIT                                               0x0001
#define GL_MAP_WRITE_BIT                                              0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT                                   0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT                                  0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT                                     0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT                                     0x0020
#define GL_COMPRESSED_RED_RGTC1                                       0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1                                0x8DBC
#define GL_COMPRESSED_RG_RGTC2                                        0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2                                 0x8DBE
#define GL_RG                                                         0x8227
#define GL_RG_INTEGER                                                 0x8228
#define GL_R8                                                         0x8229
#define GL_R16                                                        0x822A
#define GL_RG8                                                        0x822B
#define GL_RG16                                                       0x822C
#define GL_R16F                                                       0x822D
#define GL_R32F                                                       0x822E
#define GL_RG16F                                                      0x822F
#define GL_RG32F                                                      0x8230
#define GL_R8I                                                        0x8231
#define GL_R8UI                                                       0x8232
#define GL_R16I                                                       0x8233
#define GL_R16UI                                                      0x8234
#define GL_R32I                                                       0x8235
#define GL_R32UI                                                      0x8236
#define GL_RG8I                                                       0x8237
#define GL_RG8UI                                                      0x8238
#define GL_RG16I                                                      0x8239
#define GL_RG16UI                                                     0x823A
#define GL_RG32I                                                      0x823B
#define GL_RG32UI                                                     0x823C
#define GL_VERTEX_ARRAY_BINDING                                       0x85B5
#define GL_SAMPLER_2D_RECT                                            0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW                                     0x8B64
#define GL_SAMPLER_BUFFER                                             0x8DC2
#define GL_INT_SAMPLER_2D_RECT                                        0x8DCD
#define GL_INT_SAMPLER_BUFFER                                         0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT                               0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER                                0x8DD8
#define GL_TEXTURE_BUFFER                                             0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE                                    0x8C2B
#define GL_TEXTURE_BINDING_BUFFER                                     0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING                          0x8C2D
#define GL_TEXTURE_RECTANGLE                                          0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE                                  0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE                                    0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE                                 0x84F8
#define GL_R8_SNORM                                                   0x8F94
#define GL_RG8_SNORM                                                  0x8F95
#define GL_RGB8_SNORM                                                 0x8F96
#define GL_RGBA8_SNORM                                                0x8F97
#define GL_R16_SNORM                                                  0x8F98
#define GL_RG16_SNORM                                                 0x8F99
#define GL_RGB16_SNORM                                                0x8F9A
#define GL_RGBA16_SNORM                                               0x8F9B
#define GL_SIGNED_NORMALIZED                                          0x8F9C
#define GL_PRIMITIVE_RESTART                                          0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX                                    0x8F9E
#define GL_COPY_READ_BUFFER                                           0x8F36
#define GL_COPY_WRITE_BUFFER                                          0x8F37
#define GL_UNIFORM_BUFFER                                             0x8A11
#define GL_UNIFORM_BUFFER_BINDING                                     0x8A28
#define GL_UNIFORM_BUFFER_START                                       0x8A29
#define GL_UNIFORM_BUFFER_SIZE                                        0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS                                  0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS                                0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS                                0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS                                0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS                                0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE                                     0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS                     0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS                   0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS                   0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT                            0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH                       0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS                                      0x8A36
#define GL_UNIFORM_TYPE                                               0x8A37
#define GL_UNIFORM_SIZE                                               0x8A38
#define GL_UNIFORM_NAME_LENGTH                                        0x8A39
#define GL_UNIFORM_BLOCK_INDEX                                        0x8A3A
#define GL_UNIFORM_OFFSET                                             0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE                                       0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE                                      0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR                                       0x8A3E
#define GL_UNIFORM_BLOCK_BINDING                                      0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE                                    0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH                                  0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS                              0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES                       0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER                  0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER                0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER                0x8A46
#define GL_INVALID_INDEX                                              0xFFFFFFFFu
#define GL_CONTEXT_CORE_PROFILE_BIT                                   0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT                          0x00000002
#define GL_LINES_ADJACENCY                                            0x000A
#define GL_LINE_STRIP_ADJACENCY                                       0x000B
#define GL_TRIANGLES_ADJACENCY                                        0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY                                   0x000D
#define GL_PROGRAM_POINT_SIZE                                         0x8642
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS                           0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED                             0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS                       0x8DA8
#define GL_GEOMETRY_SHADER                                            0x8DD9
#define GL_GEOMETRY_VERTICES_OUT                                      0x8916
#define GL_GEOMETRY_INPUT_TYPE                                        0x8917
#define GL_GEOMETRY_OUTPUT_TYPE                                       0x8918
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS                            0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES                               0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS                       0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS                               0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS                              0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS                             0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS                              0x9125
#define GL_CONTEXT_PROFILE_MASK                                       0x9126
#define GL_DEPTH_CLAMP                                                0x864F
#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION                   0x8E4C
#define GL_FIRST_VERTEX_CONVENTION                                    0x8E4D
#define GL_LAST_VERTEX_CONVENTION                                     0x8E4E
#define GL_PROVOKING_VERTEX                                           0x8E4F
#define GL_TEXTURE_CUBE_MAP_SEAMLESS                                  0x884F
#define GL_MAX_SERVER_WAIT_TIMEOUT                                    0x9111
#define GL_OBJECT_TYPE                                                0x9112
#define GL_SYNC_CONDITION                                             0x9113
#define GL_SYNC_STATUS                                                0x9114
#define GL_SYNC_FLAGS                                                 0x9115
#define GL_SYNC_FENCE                                                 0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE                                 0x9117
#define GL_UNSIGNALED                                                 0x9118
#define GL_SIGNALED                                                   0x9119
#define GL_ALREADY_SIGNALED                                           0x911A
#define GL_TIMEOUT_EXPIRED                                            0x911B
#define GL_CONDITION_SATISFIED                                        0x911C
#define GL_WAIT_FAILED                                                0x911D
#define GL_TIMEOUT_IGNORED                                            0xFFFFFFFFFFFFFFFFull
#define GL_SYNC_FLUSH_COMMANDS_BIT                                    0x00000001
#define GL_SAMPLE_POSITION                                            0x8E50
#define GL_SAMPLE_MASK                                                0x8E51
#define GL_SAMPLE_MASK_VALUE                                          0x8E52
#define GL_MAX_SAMPLE_MASK_WORDS                                      0x8E59
#define GL_TEXTURE_2D_MULTISAMPLE                                     0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE                               0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY                               0x9102
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY                         0x9103
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE                             0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY                       0x9105
#define GL_TEXTURE_SAMPLES                                            0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS                             0x9107
#define GL_SAMPLER_2D_MULTISAMPLE                                     0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE                                 0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE                        0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY                               0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY                           0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY                  0x910D
#define GL_MAX_COLOR_TEXTURE_SAMPLES                                  0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES                                  0x910F
#define GL_MAX_INTEGER_SAMPLES                                        0x9110
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR                                0x88FE
#define GL_SRC1_COLOR                                                 0x88F9
#define GL_ONE_MINUS_SRC1_COLOR                                       0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA                                       0x88FB
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS                               0x88FC
#define GL_ANY_SAMPLES_PASSED                                         0x8C2F
#define GL_SAMPLER_BINDING                                            0x8919
#define GL_RGB10_A2UI                                                 0x906F
#define GL_TEXTURE_SWIZZLE_R                                          0x8E42
#define GL_TEXTURE_SWIZZLE_G                                          0x8E43
#define GL_TEXTURE_SWIZZLE_B                                          0x8E44
#define GL_TEXTURE_SWIZZLE_A                                          0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA                                       0x8E46
#define GL_TIME_ELAPSED                                               0x88BF
#define GL_TIMESTAMP                                                  0x8E28
#define GL_INT_2_10_10_10_REV                                         0x8D9F
#define GL_SAMPLE_SHADING                                             0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE                                   0x8C37
#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET                          0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET                          0x8E5F
#define GL_TEXTURE_CUBE_MAP_ARRAY                                     0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY                             0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY                               0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY                                     0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW                              0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY                                 0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY                        0x900F
#define GL_DRAW_INDIRECT_BUFFER                                       0x8F3F
#define GL_DRAW_INDIRECT_BUFFER_BINDING                               0x8F43
#define GL_GEOMETRY_SHADER_INVOCATIONS                                0x887F
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS                            0x8E5A
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET                          0x8E5B
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET                          0x8E5C
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS                         0x8E5D
#define GL_MAX_VERTEX_STREAMS                                         0x8E71
#define GL_DOUBLE_VEC2                                                0x8FFC
#define GL_DOUBLE_VEC3                                                0x8FFD
#define GL_DOUBLE_VEC4                                                0x8FFE
#define GL_DOUBLE_MAT2                                                0x8F46
#define GL_DOUBLE_MAT3                                                0x8F47
#define GL_DOUBLE_MAT4                                                0x8F48
#define GL_DOUBLE_MAT2x3                                              0x8F49
#define GL_DOUBLE_MAT2x4                                              0x8F4A
#define GL_DOUBLE_MAT3x2                                              0x8F4B
#define GL_DOUBLE_MAT3x4                                              0x8F4C
#define GL_DOUBLE_MAT4x2                                              0x8F4D
#define GL_DOUBLE_MAT4x3                                              0x8F4E
#define GL_ACTIVE_SUBROUTINES                                         0x8DE5
#define GL_ACTIVE_SUBROUTINE_UNIFORMS                                 0x8DE6
#define GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS                        0x8E47
#define GL_ACTIVE_SUBROUTINE_MAX_LENGTH                               0x8E48
#define GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH                       0x8E49
#define GL_MAX_SUBROUTINES                                            0x8DE7
#define GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS                           0x8DE8
#define GL_NUM_COMPATIBLE_SUBROUTINES                                 0x8E4A
#define GL_COMPATIBLE_SUBROUTINES                                     0x8E4B
#define GL_PATCHES                                                    0x000E
#define GL_PATCH_VERTICES                                             0x8E72
#define GL_PATCH_DEFAULT_INNER_LEVEL                                  0x8E73
#define GL_PATCH_DEFAULT_OUTER_LEVEL                                  0x8E74
#define GL_TESS_CONTROL_OUTPUT_VERTICES                               0x8E75
#define GL_TESS_GEN_MODE                                              0x8E76
#define GL_TESS_GEN_SPACING                                           0x8E77
#define GL_TESS_GEN_VERTEX_ORDER                                      0x8E78
#define GL_TESS_GEN_POINT_MODE                                        0x8E79
#define GL_ISOLINES                                                   0x8E7A
#define GL_FRACTIONAL_ODD                                             0x8E7B
#define GL_FRACTIONAL_EVEN                                            0x8E7C
#define GL_MAX_PATCH_VERTICES                                         0x8E7D
#define GL_MAX_TESS_GEN_LEVEL                                         0x8E7E
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS                        0x8E7F
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS                     0x8E80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS                       0x8E81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS                    0x8E82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS                         0x8E83
#define GL_MAX_TESS_PATCH_COMPONENTS                                  0x8E84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS                   0x8E85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS                      0x8E86
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS                            0x8E89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS                         0x8E8A
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS                          0x886C
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS                       0x886D
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS               0x8E1E
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS            0x8E1F
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER            0x84F0
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER         0x84F1
#define GL_TESS_EVALUATION_SHADER                                     0x8E87
#define GL_TESS_CONTROL_SHADER                                        0x8E88
#define GL_TRANSFORM_FEEDBACK                                         0x8E22
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED                           0x8E23
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE                           0x8E24
#define GL_TRANSFORM_FEEDBACK_BINDING                                 0x8E25
#define GL_MAX_TRANSFORM_FEEDBACK_BUFFERS                             0x8E70
#define GL_FIXED                                                      0x140C
#define GL_IMPLEMENTATION_COLOR_READ_TYPE                             0x8B9A
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT                           0x8B9B
#define GL_LOW_FLOAT                                                  0x8DF0
#define GL_MEDIUM_FLOAT                                               0x8DF1
#define GL_HIGH_FLOAT                                                 0x8DF2
#define GL_LOW_INT                                                    0x8DF3
#define GL_MEDIUM_INT                                                 0x8DF4
#define GL_HIGH_INT                                                   0x8DF5
#define GL_SHADER_COMPILER                                            0x8DFA
#define GL_SHADER_BINARY_FORMATS                                      0x8DF8
#define GL_NUM_SHADER_BINARY_FORMATS                                  0x8DF9
#define GL_MAX_VERTEX_UNIFORM_VECTORS                                 0x8DFB
#define GL_MAX_VARYING_VECTORS                                        0x8DFC
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS                               0x8DFD
#define GL_RGB565                                                     0x8D62
#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT                            0x8257
#define GL_PROGRAM_BINARY_LENGTH                                      0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS                                 0x87FE
#define GL_PROGRAM_BINARY_FORMATS                                     0x87FF
#define GL_VERTEX_SHADER_BIT                                          0x00000001
#define GL_FRAGMENT_SHADER_BIT                                        0x00000002
#define GL_GEOMETRY_SHADER_BIT                                        0x00000004
#define GL_TESS_CONTROL_SHADER_BIT                                    0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT                                 0x00000010
#define GL_ALL_SHADER_BITS                                            0xFFFFFFFF
#define GL_PROGRAM_SEPARABLE                                          0x8258
#define GL_ACTIVE_PROGRAM                                             0x8259
#define GL_PROGRAM_PIPELINE_BINDING                                   0x825A
#define GL_MAX_VIEWPORTS                                              0x825B
#define GL_VIEWPORT_SUBPIXEL_BITS                                     0x825C
#define GL_VIEWPORT_BOUNDS_RANGE                                      0x825D
#define GL_LAYER_PROVOKING_VERTEX                                     0x825E
#define GL_VIEWPORT_INDEX_PROVOKING_VERTEX                            0x825F
#define GL_UNDEFINED_VERTEX                                           0x8260
#define GL_COPY_READ_BUFFER_BINDING                                   0x8F36
#define GL_COPY_WRITE_BUFFER_BINDING                                  0x8F37
#define GL_TRANSFORM_FEEDBACK_ACTIVE                                  0x8E24
#define GL_TRANSFORM_FEEDBACK_PAUSED                                  0x8E23
#define GL_UNPACK_COMPRESSED_BLOCK_WIDTH                              0x9127
#define GL_UNPACK_COMPRESSED_BLOCK_HEIGHT                             0x9128
#define GL_UNPACK_COMPRESSED_BLOCK_DEPTH                              0x9129
#define GL_UNPACK_COMPRESSED_BLOCK_SIZE                               0x912A
#define GL_PACK_COMPRESSED_BLOCK_WIDTH                                0x912B
#define GL_PACK_COMPRESSED_BLOCK_HEIGHT                               0x912C
#define GL_PACK_COMPRESSED_BLOCK_DEPTH                                0x912D
#define GL_PACK_COMPRESSED_BLOCK_SIZE                                 0x912E
#define GL_NUM_SAMPLE_COUNTS                                          0x9380
#define GL_MIN_MAP_BUFFER_ALIGNMENT                                   0x90BC
#define GL_ATOMIC_COUNTER_BUFFER                                      0x92C0
#define GL_ATOMIC_COUNTER_BUFFER_BINDING                              0x92C1
#define GL_ATOMIC_COUNTER_BUFFER_START                                0x92C2
#define GL_ATOMIC_COUNTER_BUFFER_SIZE                                 0x92C3
#define GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE                            0x92C4
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS               0x92C5
#define GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES        0x92C6
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER          0x92C7
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER    0x92C8
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER 0x92C9
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER        0x92CA
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER        0x92CB
#define GL_MAX_VERTEX_ATOMIC_COUNTER_BUFFERS                          0x92CC
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTER_BUFFERS                    0x92CD
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTER_BUFFERS                 0x92CE
#define GL_MAX_GEOMETRY_ATOMIC_COUNTER_BUFFERS                        0x92CF
#define GL_MAX_FRAGMENT_ATOMIC_COUNTER_BUFFERS                        0x92D0
#define GL_MAX_COMBINED_ATOMIC_COUNTER_BUFFERS                        0x92D1
#define GL_MAX_VERTEX_ATOMIC_COUNTERS                                 0x92D2
#define GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS                           0x92D3
#define GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS                        0x92D4
#define GL_MAX_GEOMETRY_ATOMIC_COUNTERS                               0x92D5
#define GL_MAX_FRAGMENT_ATOMIC_COUNTERS                               0x92D6
#define GL_MAX_COMBINED_ATOMIC_COUNTERS                               0x92D7
#define GL_MAX_ATOMIC_COUNTER_BUFFER_SIZE                             0x92D8
#define GL_MAX_ATOMIC_COUNTER_BUFFER_BINDINGS                         0x92DC
#define GL_ACTIVE_ATOMIC_COUNTER_BUFFERS                              0x92D9
#define GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX                        0x92DA
#define GL_UNSIGNED_INT_ATOMIC_COUNTER                                0x92DB
#define GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT                            0x00000001
#define GL_ELEMENT_ARRAY_BARRIER_BIT                                  0x00000002
#define GL_UNIFORM_BARRIER_BIT                                        0x00000004
#define GL_TEXTURE_FETCH_BARRIER_BIT                                  0x00000008
#define GL_SHADER_IMAGE_ACCESS_BARRIER_BIT                            0x00000020
#define GL_COMMAND_BARRIER_BIT                                        0x00000040
#define GL_PIXEL_BUFFER_BARRIER_BIT                                   0x00000080
#define GL_TEXTURE_UPDATE_BARRIER_BIT                                 0x00000100
#define GL_BUFFER_UPDATE_BARRIER_BIT                                  0x00000200
#define GL_FRAMEBUFFER_BARRIER_BIT                                    0x00000400
#define GL_TRANSFORM_FEEDBACK_BARRIER_BIT                             0x00000800
#define GL_ATOMIC_COUNTER_BARRIER_BIT                                 0x00001000
#define GL_ALL_BARRIER_BITS                                           0xFFFFFFFF
#define GL_MAX_IMAGE_UNITS                                            0x8F38
#define GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS              0x8F39
#define GL_IMAGE_BINDING_NAME                                         0x8F3A
#define GL_IMAGE_BINDING_LEVEL                                        0x8F3B
#define GL_IMAGE_BINDING_LAYERED                                      0x8F3C
#define GL_IMAGE_BINDING_LAYER                                        0x8F3D
#define GL_IMAGE_BINDING_ACCESS                                       0x8F3E
#define GL_IMAGE_1D                                                   0x904C
#define GL_IMAGE_2D                                                   0x904D
#define GL_IMAGE_3D                                                   0x904E
#define GL_IMAGE_2D_RECT                                              0x904F
#define GL_IMAGE_CUBE                                                 0x9050
#define GL_IMAGE_BUFFER                                               0x9051
#define GL_IMAGE_1D_ARRAY                                             0x9052
#define GL_IMAGE_2D_ARRAY                                             0x9053
#define GL_IMAGE_CUBE_MAP_ARRAY                                       0x9054
#define GL_IMAGE_2D_MULTISAMPLE                                       0x9055
#define GL_IMAGE_2D_MULTISAMPLE_ARRAY                                 0x9056
#define GL_INT_IMAGE_1D                                               0x9057
#define GL_INT_IMAGE_2D                                               0x9058
#define GL_INT_IMAGE_3D                                               0x9059
#define GL_INT_IMAGE_2D_RECT                                          0x905A
#define GL_INT_IMAGE_CUBE                                             0x905B
#define GL_INT_IMAGE_BUFFER                                           0x905C
#define GL_INT_IMAGE_1D_ARRAY                                         0x905D
#define GL_INT_IMAGE_2D_ARRAY                                         0x905E
#define GL_INT_IMAGE_CUBE_MAP_ARRAY                                   0x905F
#define GL_INT_IMAGE_2D_MULTISAMPLE                                   0x9060
#define GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY                             0x9061
#define GL_UNSIGNED_INT_IMAGE_1D                                      0x9062
#define GL_UNSIGNED_INT_IMAGE_2D                                      0x9063
#define GL_UNSIGNED_INT_IMAGE_3D                                      0x9064
#define GL_UNSIGNED_INT_IMAGE_2D_RECT                                 0x9065
#define GL_UNSIGNED_INT_IMAGE_CUBE                                    0x9066
#define GL_UNSIGNED_INT_IMAGE_BUFFER                                  0x9067
#define GL_UNSIGNED_INT_IMAGE_1D_ARRAY                                0x9068
#define GL_UNSIGNED_INT_IMAGE_2D_ARRAY                                0x9069
#define GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY                          0x906A
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE                          0x906B
#define GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY                    0x906C
#define GL_MAX_IMAGE_SAMPLES                                          0x906D
#define GL_IMAGE_BINDING_FORMAT                                       0x906E
#define GL_IMAGE_FORMAT_COMPATIBILITY_TYPE                            0x90C7
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_SIZE                         0x90C8
#define GL_IMAGE_FORMAT_COMPATIBILITY_BY_CLASS                        0x90C9
#define GL_MAX_VERTEX_IMAGE_UNIFORMS                                  0x90CA
#define GL_MAX_TESS_CONTROL_IMAGE_UNIFORMS                            0x90CB
#define GL_MAX_TESS_EVALUATION_IMAGE_UNIFORMS                         0x90CC
#define GL_MAX_GEOMETRY_IMAGE_UNIFORMS                                0x90CD
#define GL_MAX_FRAGMENT_IMAGE_UNIFORMS                                0x90CE
#define GL_MAX_COMBINED_IMAGE_UNIFORMS                                0x90CF
#define GL_COMPRESSED_RGBA_BPTC_UNORM                                 0x8E8C
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM                           0x8E8D
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT                           0x8E8E
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT                         0x8E8F
#define GL_TEXTURE_IMMUTABLE_FORMAT                                   0x912F
#define GL_NUM_SHADING_LANGUAGE_VERSIONS                              0x82E9
#define GL_VERTEX_ATTRIB_ARRAY_LONG                                   0x874E
#define GL_COMPRESSED_RGB8_ETC2                                       0x9274
#define GL_COMPRESSED_SRGB8_ETC2                                      0x9275
#define GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2                   0x9276
#define GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2                  0x9277
#define GL_COMPRESSED_RGBA8_ETC2_EAC                                  0x9278
#define GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC                           0x9279
#define GL_COMPRESSED_R11_EAC                                         0x9270
#define GL_COMPRESSED_SIGNED_R11_EAC                                  0x9271
#define GL_COMPRESSED_RG11_EAC                                        0x9272
#define GL_COMPRESSED_SIGNED_RG11_EAC                                 0x9273
#define GL_PRIMITIVE_RESTART_FIXED_INDEX                              0x8D69
#define GL_ANY_SAMPLES_PASSED_CONSERVATIVE                            0x8D6A
#define GL_MAX_ELEMENT_INDEX                                          0x8D6B
#define GL_COMPUTE_SHADER                                             0x91B9
#define GL_MAX_COMPUTE_UNIFORM_BLOCKS                                 0x91BB
#define GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS                            0x91BC
#define GL_MAX_COMPUTE_IMAGE_UNIFORMS                                 0x91BD
#define GL_MAX_COMPUTE_SHARED_MEMORY_SIZE                             0x8262
#define GL_MAX_COMPUTE_UNIFORM_COMPONENTS                             0x8263
#define GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS                         0x8264
#define GL_MAX_COMPUTE_ATOMIC_COUNTERS                                0x8265
#define GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS                    0x8266
#define GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS                         0x90EB
#define GL_MAX_COMPUTE_WORK_GROUP_COUNT                               0x91BE
#define GL_MAX_COMPUTE_WORK_GROUP_SIZE                                0x91BF
#define GL_COMPUTE_WORK_GROUP_SIZE                                    0x8267
#define GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER                 0x90EC
#define GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER         0x90ED
#define GL_DISPATCH_INDIRECT_BUFFER                                   0x90EE
#define GL_DISPATCH_INDIRECT_BUFFER_BINDING                           0x90EF
#define GL_COMPUTE_SHADER_BIT                                         0x00000020
#define GL_DEBUG_OUTPUT_SYNCHRONOUS                                   0x8242
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH                           0x8243
#define GL_DEBUG_CALLBACK_FUNCTION                                    0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM                                  0x8245
#define GL_DEBUG_SOURCE_API                                           0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM                                 0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER                               0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY                                   0x8249
#define GL_DEBUG_SOURCE_APPLICATION                                   0x824A
#define GL_DEBUG_SOURCE_OTHER                                         0x824B
#define GL_DEBUG_TYPE_ERROR                                           0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR                             0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR                              0x824E
#define GL_DEBUG_TYPE_PORTABILITY                                     0x824F
#define GL_DEBUG_TYPE_PERFORMANCE                                     0x8250
#define GL_DEBUG_TYPE_OTHER                                           0x8251
#define GL_MAX_DEBUG_MESSAGE_LENGTH                                   0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES                                  0x9144
#define GL_DEBUG_LOGGED_MESSAGES                                      0x9145
#define GL_DEBUG_SEVERITY_HIGH                                        0x9146
#define GL_DEBUG_SEVERITY_MEDIUM                                      0x9147
#define GL_DEBUG_SEVERITY_LOW                                         0x9148
#define GL_DEBUG_TYPE_MARKER                                          0x8268
#define GL_DEBUG_TYPE_PUSH_GROUP                                      0x8269
#define GL_DEBUG_TYPE_POP_GROUP                                       0x826A
#define GL_DEBUG_SEVERITY_NOTIFICATION                                0x826B
#define GL_MAX_DEBUG_GROUP_STACK_DEPTH                                0x826C
#define GL_DEBUG_GROUP_STACK_DEPTH                                    0x826D
#define GL_BUFFER                                                     0x82E0
#define GL_SHADER                                                     0x82E1
#define GL_PROGRAM                                                    0x82E2
#define GL_QUERY                                                      0x82E3
#define GL_PROGRAM_PIPELINE                                           0x82E4
#define GL_SAMPLER                                                    0x82E6
#define GL_MAX_LABEL_LENGTH                                           0x82E8
#define GL_DEBUG_OUTPUT                                               0x92E0
#define GL_CONTEXT_FLAG_DEBUG_BIT                                     0x00000002
#define GL_MAX_UNIFORM_LOCATIONS                                      0x826E
#define GL_FRAMEBUFFER_DEFAULT_WIDTH                                  0x9310
#define GL_FRAMEBUFFER_DEFAULT_HEIGHT                                 0x9311
#define GL_FRAMEBUFFER_DEFAULT_LAYERS                                 0x9312
#define GL_FRAMEBUFFER_DEFAULT_SAMPLES                                0x9313
#define GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS                 0x9314
#define GL_MAX_FRAMEBUFFER_WIDTH                                      0x9315
#define GL_MAX_FRAMEBUFFER_HEIGHT                                     0x9316
#define GL_MAX_FRAMEBUFFER_LAYERS                                     0x9317
#define GL_MAX_FRAMEBUFFER_SAMPLES                                    0x9318
#define GL_INTERNALFORMAT_SUPPORTED                                   0x826F
#define GL_INTERNALFORMAT_PREFERRED                                   0x8270
#define GL_INTERNALFORMAT_RED_SIZE                                    0x8271
#define GL_INTERNALFORMAT_GREEN_SIZE                                  0x8272
#define GL_INTERNALFORMAT_BLUE_SIZE                                   0x8273
#define GL_INTERNALFORMAT_ALPHA_SIZE                                  0x8274
#define GL_INTERNALFORMAT_DEPTH_SIZE                                  0x8275
#define GL_INTERNALFORMAT_STENCIL_SIZE                                0x8276
#define GL_INTERNALFORMAT_SHARED_SIZE                                 0x8277
#define GL_INTERNALFORMAT_RED_TYPE                                    0x8278
#define GL_INTERNALFORMAT_GREEN_TYPE                                  0x8279
#define GL_INTERNALFORMAT_BLUE_TYPE                                   0x827A
#define GL_INTERNALFORMAT_ALPHA_TYPE                                  0x827B
#define GL_INTERNALFORMAT_DEPTH_TYPE                                  0x827C
#define GL_INTERNALFORMAT_STENCIL_TYPE                                0x827D
#define GL_MAX_WIDTH                                                  0x827E
#define GL_MAX_HEIGHT                                                 0x827F
#define GL_MAX_DEPTH                                                  0x8280
#define GL_MAX_LAYERS                                                 0x8281
#define GL_MAX_COMBINED_DIMENSIONS                                    0x8282
#define GL_COLOR_COMPONENTS                                           0x8283
#define GL_DEPTH_COMPONENTS                                           0x8284
#define GL_STENCIL_COMPONENTS                                         0x8285
#define GL_COLOR_RENDERABLE                                           0x8286
#define GL_DEPTH_RENDERABLE                                           0x8287
#define GL_STENCIL_RENDERABLE                                         0x8288
#define GL_FRAMEBUFFER_RENDERABLE                                     0x8289
#define GL_FRAMEBUFFER_RENDERABLE_LAYERED                             0x828A
#define GL_FRAMEBUFFER_BLEND                                          0x828B
#define GL_READ_PIXELS                                                0x828C
#define GL_READ_PIXELS_FORMAT                                         0x828D
#define GL_READ_PIXELS_TYPE                                           0x828E
#define GL_TEXTURE_IMAGE_FORMAT                                       0x828F
#define GL_TEXTURE_IMAGE_TYPE                                         0x8290
#define GL_GET_TEXTURE_IMAGE_FORMAT                                   0x8291
#define GL_GET_TEXTURE_IMAGE_TYPE                                     0x8292
#define GL_MIPMAP                                                     0x8293
#define GL_MANUAL_GENERATE_MIPMAP                                     0x8294
#define GL_AUTO_GENERATE_MIPMAP                                       0x8295
#define GL_COLOR_ENCODING                                             0x8296
#define GL_SRGB_READ                                                  0x8297
#define GL_SRGB_WRITE                                                 0x8298
#define GL_FILTER                                                     0x829A
#define GL_VERTEX_TEXTURE                                             0x829B
#define GL_TESS_CONTROL_TEXTURE                                       0x829C
#define GL_TESS_EVALUATION_TEXTURE                                    0x829D
#define GL_GEOMETRY_TEXTURE                                           0x829E
#define GL_FRAGMENT_TEXTURE                                           0x829F
#define GL_COMPUTE_TEXTURE                                            0x82A0
#define GL_TEXTURE_SHADOW                                             0x82A1
#define GL_TEXTURE_GATHER                                             0x82A2
#define GL_TEXTURE_GATHER_SHADOW                                      0x82A3
#define GL_SHADER_IMAGE_LOAD                                          0x82A4
#define GL_SHADER_IMAGE_STORE                                         0x82A5
#define GL_SHADER_IMAGE_ATOMIC                                        0x82A6
#define GL_IMAGE_TEXEL_SIZE                                           0x82A7
#define GL_IMAGE_COMPATIBILITY_CLASS                                  0x82A8
#define GL_IMAGE_PIXEL_FORMAT                                         0x82A9
#define GL_IMAGE_PIXEL_TYPE                                           0x82AA
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST                        0x82AC
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST                      0x82AD
#define GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE                       0x82AE
#define GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE                     0x82AF
#define GL_TEXTURE_COMPRESSED_BLOCK_WIDTH                             0x82B1
#define GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT                            0x82B2
#define GL_TEXTURE_COMPRESSED_BLOCK_SIZE                              0x82B3
#define GL_CLEAR_BUFFER                                               0x82B4
#define GL_TEXTURE_VIEW                                               0x82B5
#define GL_VIEW_COMPATIBILITY_CLASS                                   0x82B6
#define GL_FULL_SUPPORT                                               0x82B7
#define GL_CAVEAT_SUPPORT                                             0x82B8
#define GL_IMAGE_CLASS_4_X_32                                         0x82B9
#define GL_IMAGE_CLASS_2_X_32                                         0x82BA
#define GL_IMAGE_CLASS_1_X_32                                         0x82BB
#define GL_IMAGE_CLASS_4_X_16                                         0x82BC
#define GL_IMAGE_CLASS_2_X_16                                         0x82BD
#define GL_IMAGE_CLASS_1_X_16                                         0x82BE
#define GL_IMAGE_CLASS_4_X_8                                          0x82BF
#define GL_IMAGE_CLASS_2_X_8                                          0x82C0
#define GL_IMAGE_CLASS_1_X_8                                          0x82C1
#define GL_IMAGE_CLASS_11_11_10                                       0x82C2
#define GL_IMAGE_CLASS_10_10_10_2                                     0x82C3
#define GL_VIEW_CLASS_128_BITS                                        0x82C4
#define GL_VIEW_CLASS_96_BITS                                         0x82C5
#define GL_VIEW_CLASS_64_BITS                                         0x82C6
#define GL_VIEW_CLASS_48_BITS                                         0x82C7
#define GL_VIEW_CLASS_32_BITS                                         0x82C8
#define GL_VIEW_CLASS_24_BITS                                         0x82C9
#define GL_VIEW_CLASS_16_BITS                                         0x82CA
#define GL_VIEW_CLASS_8_BITS                                          0x82CB
#define GL_VIEW_CLASS_S3TC_DXT1_RGB                                   0x82CC
#define GL_VIEW_CLASS_S3TC_DXT1_RGBA                                  0x82CD
#define GL_VIEW_CLASS_S3TC_DXT3_RGBA                                  0x82CE
#define GL_VIEW_CLASS_S3TC_DXT5_RGBA                                  0x82CF
#define GL_VIEW_CLASS_RGTC1_RED                                       0x82D0
#define GL_VIEW_CLASS_RGTC2_RG                                        0x82D1
#define GL_VIEW_CLASS_BPTC_UNORM                                      0x82D2
#define GL_VIEW_CLASS_BPTC_FLOAT                                      0x82D3
#define GL_UNIFORM                                                    0x92E1
#define GL_UNIFORM_BLOCK                                              0x92E2
#define GL_PROGRAM_INPUT                                              0x92E3
#define GL_PROGRAM_OUTPUT                                             0x92E4
#define GL_BUFFER_VARIABLE                                            0x92E5
#define GL_SHADER_STORAGE_BLOCK                                       0x92E6
#define GL_VERTEX_SUBROUTINE                                          0x92E8
#define GL_TESS_CONTROL_SUBROUTINE                                    0x92E9
#define GL_TESS_EVALUATION_SUBROUTINE                                 0x92EA
#define GL_GEOMETRY_SUBROUTINE                                        0x92EB
#define GL_FRAGMENT_SUBROUTINE                                        0x92EC
#define GL_COMPUTE_SUBROUTINE                                         0x92ED
#define GL_VERTEX_SUBROUTINE_UNIFORM                                  0x92EE
#define GL_TESS_CONTROL_SUBROUTINE_UNIFORM                            0x92EF
#define GL_TESS_EVALUATION_SUBROUTINE_UNIFORM                         0x92F0
#define GL_GEOMETRY_SUBROUTINE_UNIFORM                                0x92F1
#define GL_FRAGMENT_SUBROUTINE_UNIFORM                                0x92F2
#define GL_COMPUTE_SUBROUTINE_UNIFORM                                 0x92F3
#define GL_TRANSFORM_FEEDBACK_VARYING                                 0x92F4
#define GL_ACTIVE_RESOURCES                                           0x92F5
#define GL_MAX_NAME_LENGTH                                            0x92F6
#define GL_MAX_NUM_ACTIVE_VARIABLES                                   0x92F7
#define GL_MAX_NUM_COMPATIBLE_SUBROUTINES                             0x92F8
#define GL_NAME_LENGTH                                                0x92F9
#define GL_TYPE                                                       0x92FA
#define GL_ARRAY_SIZE                                                 0x92FB
#define GL_OFFSET                                                     0x92FC
#define GL_BLOCK_INDEX                                                0x92FD
#define GL_ARRAY_STRIDE                                               0x92FE
#define GL_MATRIX_STRIDE                                              0x92FF
#define GL_IS_ROW_MAJOR                                               0x9300
#define GL_ATOMIC_COUNTER_BUFFER_INDEX                                0x9301
#define GL_BUFFER_BINDING                                             0x9302
#define GL_BUFFER_DATA_SIZE                                           0x9303
#define GL_NUM_ACTIVE_VARIABLES                                       0x9304
#define GL_ACTIVE_VARIABLES                                           0x9305
#define GL_REFERENCED_BY_VERTEX_SHADER                                0x9306
#define GL_REFERENCED_BY_TESS_CONTROL_SHADER                          0x9307
#define GL_REFERENCED_BY_TESS_EVALUATION_SHADER                       0x9308
#define GL_REFERENCED_BY_GEOMETRY_SHADER                              0x9309
#define GL_REFERENCED_BY_FRAGMENT_SHADER                              0x930A
#define GL_REFERENCED_BY_COMPUTE_SHADER                               0x930B
#define GL_TOP_LEVEL_ARRAY_SIZE                                       0x930C
#define GL_TOP_LEVEL_ARRAY_STRIDE                                     0x930D
#define GL_LOCATION                                                   0x930E
#define GL_LOCATION_INDEX                                             0x930F
#define GL_IS_PER_PATCH                                               0x92E7
#define GL_SHADER_STORAGE_BUFFER                                      0x90D2
#define GL_SHADER_STORAGE_BUFFER_BINDING                              0x90D3
#define GL_SHADER_STORAGE_BUFFER_START                                0x90D4
#define GL_SHADER_STORAGE_BUFFER_SIZE                                 0x90D5
#define GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS                           0x90D6
#define GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS                         0x90D7
#define GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS                     0x90D8
#define GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS                  0x90D9
#define GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS                         0x90DA
#define GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS                          0x90DB
#define GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS                         0x90DC
#define GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS                         0x90DD
#define GL_MAX_SHADER_STORAGE_BLOCK_SIZE                              0x90DE
#define GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT                     0x90DF
#define GL_SHADER_STORAGE_BARRIER_BIT                                 0x00002000
#define GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES                       0x8F39
#define GL_DEPTH_STENCIL_TEXTURE_MODE                                 0x90EA
#define GL_TEXTURE_BUFFER_OFFSET                                      0x919D
#define GL_TEXTURE_BUFFER_SIZE                                        0x919E
#define GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT                            0x919F
#define GL_TEXTURE_VIEW_MIN_LEVEL                                     0x82DB
#define GL_TEXTURE_VIEW_NUM_LEVELS                                    0x82DC
#define GL_TEXTURE_VIEW_MIN_LAYER                                     0x82DD
#define GL_TEXTURE_VIEW_NUM_LAYERS                                    0x82DE
#define GL_TEXTURE_IMMUTABLE_LEVELS                                   0x82DF
#define GL_VERTEX_ATTRIB_BINDING                                      0x82D4
#define GL_VERTEX_ATTRIB_RELATIVE_OFFSET                              0x82D5
#define GL_VERTEX_BINDING_DIVISOR                                     0x82D6
#define GL_VERTEX_BINDING_OFFSET                                      0x82D7
#define GL_VERTEX_BINDING_STRIDE                                      0x82D8
#define GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET                          0x82D9
#define GL_MAX_VERTEX_ATTRIB_BINDINGS                                 0x82DA
#define GL_VERTEX_BINDING_BUFFER                                      0x8F4F
#define GL_MAX_VERTEX_ATTRIB_STRIDE                                   0x82E5
#define GL_PRIMITIVE_RESTART_FOR_PATCHES_SUPPORTED                    0x8221
#define GL_TEXTURE_BUFFER_BINDING                                     0x8C2A
#define GL_MAP_PERSISTENT_BIT                                         0x0040
#define GL_MAP_COHERENT_BIT                                           0x0080
#define GL_DYNAMIC_STORAGE_BIT                                        0x0100
#define GL_CLIENT_STORAGE_BIT                                         0x0200
#define GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT                           0x00004000
#define GL_BUFFER_IMMUTABLE_STORAGE                                   0x821F
#define GL_BUFFER_STORAGE_FLAGS                                       0x8220
#define GL_CLEAR_TEXTURE                                              0x9365
#define GL_LOCATION_COMPONENT                                         0x934A
#define GL_TRANSFORM_FEEDBACK_BUFFER_INDEX                            0x934B
#define GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE                           0x934C
#define GL_QUERY_BUFFER                                               0x9192
#define GL_QUERY_BUFFER_BARRIER_BIT                                   0x00008000
#define GL_QUERY_BUFFER_BINDING                                       0x9193
#define GL_QUERY_RESULT_NO_WAIT                                       0x9194
#define GL_MIRROR_CLAMP_TO_EDGE                                       0x8743
#define GL_CONTEXT_LOST                                               0x0507
#define GL_NEGATIVE_ONE_TO_ONE                                        0x935E
#define GL_ZERO_TO_ONE                                                0x935F
#define GL_CLIP_ORIGIN                                                0x935C
#define GL_CLIP_DEPTH_MODE                                            0x935D
#define GL_QUERY_WAIT_INVERTED                                        0x8E17
#define GL_QUERY_NO_WAIT_INVERTED                                     0x8E18
#define GL_QUERY_BY_REGION_WAIT_INVERTED                              0x8E19
#define GL_QUERY_BY_REGION_NO_WAIT_INVERTED                           0x8E1A
#define GL_MAX_CULL_DISTANCES                                         0x82F9
#define GL_MAX_COMBINED_CLIP_AND_CULL_DISTANCES                       0x82FA
#define GL_TEXTURE_TARGET                                             0x1006
#define GL_QUERY_TARGET                                               0x82EA
#define GL_GUILTY_CONTEXT_RESET                                       0x8253
#define GL_INNOCENT_CONTEXT_RESET                                     0x8254
#define GL_UNKNOWN_CONTEXT_RESET                                      0x8255
#define GL_RESET_NOTIFICATION_STRATEGY                                0x8256
#define GL_LOSE_CONTEXT_ON_RESET                                      0x8252
#define GL_NO_RESET_NOTIFICATION                                      0x8261
#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT                             0x00000004
#define GL_CONTEXT_RELEASE_BEHAVIOR                                   0x82FB
#define GL_CONTEXT_RELEASE_BEHAVIOR_FLUSH                             0x82FC
#define GL_SHADER_BINARY_FORMAT_SPIR_V                                0x9551
#define GL_SPIR_V_BINARY                                              0x9552
#define GL_PARAMETER_BUFFER                                           0x80EE
#define GL_PARAMETER_BUFFER_BINDING                                   0x80EF
#define GL_CONTEXT_FLAG_NO_ERROR_BIT                                  0x00000008
#define GL_VERTICES_SUBMITTED                                         0x82EE
#define GL_PRIMITIVES_SUBMITTED                                       0x82EF
#define GL_VERTEX_SHADER_INVOCATIONS                                  0x82F0
#define GL_TESS_CONTROL_SHADER_PATCHES                                0x82F1
#define GL_TESS_EVALUATION_SHADER_INVOCATIONS                         0x82F2
#define GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED                         0x82F3
#define GL_FRAGMENT_SHADER_INVOCATIONS                                0x82F4
#define GL_COMPUTE_SHADER_INVOCATIONS                                 0x82F5
#define GL_CLIPPING_INPUT_PRIMITIVES                                  0x82F6
#define GL_CLIPPING_OUTPUT_PRIMITIVES                                 0x82F7
#define GL_POLYGON_OFFSET_CLAMP                                       0x8E1B
#define GL_SPIR_V_EXTENSIONS                                          0x9553
#define GL_NUM_SPIR_V_EXTENSIONS                                      0x9554
#define GL_TEXTURE_MAX_ANISOTROPY                                     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY                                 0x84FF
#define GL_TRANSFORM_FEEDBACK_OVERFLOW                                0x82EC
#define GL_TRANSFORM_FEEDBACK_STREAM_OVERFLOW                         0x82ED
#endif // defined(__APPLE__)

#define VD_FW_OPENGL_CORE_FUNCTIONS \
V(1_0) \
X(void, glAccum, (GLenum op, GLfloat value)) \
X(void, glActiveTexture, (GLenum texture)) \
X(void, glAlphaFunc, (GLenum func, GLfloat ref)) \
X(void, glAlphaFuncx, (GLenum func, GLfixed ref)) \
X(void, glBegin, (GLenum mode)) \
X(void, glBindBuffer, (GLenum target, GLuint buffer)) \
X(void, glBindTexture, (GLenum target, GLuint texture)) \
X(void, glBitmap, (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte * bitmap)) \
X(void, glBlendFunc, (GLenum sfactor, GLenum dfactor)) \
X(void, glBufferData, (GLenum target, GLsizeiptr size, const void * data, GLenum usage)) \
X(void, glBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, const void * data)) \
X(void, glCallList, (GLuint list)) \
X(void, glCallLists, (GLsizei n, GLenum type, const void * lists)) \
X(void, glClear, (GLbitfield mask)) \
X(void, glClearAccum, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)) \
X(void, glClearColor, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)) \
X(void, glClearColorx, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)) \
X(void, glClearDepth, (GLdouble depth)) \
X(void, glClearDepthf, (GLfloat d)) \
X(void, glClearDepthx, (GLfixed depth)) \
X(void, glClearIndex, (GLfloat c)) \
X(void, glClearStencil, (GLint s)) \
X(void, glClientActiveTexture, (GLenum texture)) \
X(void, glClipPlane, (GLenum plane, const GLdouble * equation)) \
X(void, glClipPlanef, (GLenum p, const GLfloat * eqn)) \
X(void, glClipPlanex, (GLenum plane, const GLfixed * equation)) \
X(void, glColor3b, (GLbyte red, GLbyte green, GLbyte blue)) \
X(void, glColor3bv, (const GLbyte * v)) \
X(void, glColor3d, (GLdouble red, GLdouble green, GLdouble blue)) \
X(void, glColor3dv, (const GLdouble * v)) \
X(void, glColor3f, (GLfloat red, GLfloat green, GLfloat blue)) \
X(void, glColor3fv, (const GLfloat * v)) \
X(void, glColor3i, (GLint red, GLint green, GLint blue)) \
X(void, glColor3iv, (const GLint * v)) \
X(void, glColor3s, (GLshort red, GLshort green, GLshort blue)) \
X(void, glColor3sv, (const GLshort * v)) \
X(void, glColor3ub, (GLubyte red, GLubyte green, GLubyte blue)) \
X(void, glColor3ubv, (const GLubyte * v)) \
X(void, glColor3ui, (GLuint red, GLuint green, GLuint blue)) \
X(void, glColor3uiv, (const GLuint * v)) \
X(void, glColor3us, (GLushort red, GLushort green, GLushort blue)) \
X(void, glColor3usv, (const GLushort * v)) \
X(void, glColor4b, (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)) \
X(void, glColor4bv, (const GLbyte * v)) \
X(void, glColor4d, (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)) \
X(void, glColor4dv, (const GLdouble * v)) \
X(void, glColor4f, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)) \
X(void, glColor4fv, (const GLfloat * v)) \
X(void, glColor4i, (GLint red, GLint green, GLint blue, GLint alpha)) \
X(void, glColor4iv, (const GLint * v)) \
X(void, glColor4s, (GLshort red, GLshort green, GLshort blue, GLshort alpha)) \
X(void, glColor4sv, (const GLshort * v)) \
X(void, glColor4ub, (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)) \
X(void, glColor4ubv, (const GLubyte * v)) \
X(void, glColor4ui, (GLuint red, GLuint green, GLuint blue, GLuint alpha)) \
X(void, glColor4uiv, (const GLuint * v)) \
X(void, glColor4us, (GLushort red, GLushort green, GLushort blue, GLushort alpha)) \
X(void, glColor4usv, (const GLushort * v)) \
X(void, glColor4x, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)) \
X(void, glColorMask, (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)) \
X(void, glColorMaterial, (GLenum face, GLenum mode)) \
X(void, glColorPointer, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, glCompressedTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data)) \
X(void, glCompressedTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data)) \
X(void, glCopyPixels, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)) \
X(void, glCopyTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)) \
X(void, glCopyTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, glCullFace, (GLenum mode)) \
X(void, glDeleteBuffers, (GLsizei n, const GLuint * buffers)) \
X(void, glDeleteLists, (GLuint list, GLsizei range)) \
X(void, glDeleteTextures, (GLsizei n, const GLuint * textures)) \
X(void, glDepthFunc, (GLenum func)) \
X(void, glDepthMask, (GLboolean flag)) \
X(void, glDepthRange, (GLdouble n, GLdouble f)) \
X(void, glDepthRangef, (GLfloat n, GLfloat f)) \
X(void, glDepthRangex, (GLfixed n, GLfixed f)) \
X(void, glDisable, (GLenum cap)) \
X(void, glDisableClientState, (GLenum array)) \
X(void, glDrawArrays, (GLenum mode, GLint first, GLsizei count)) \
X(void, glDrawBuffer, (GLenum buf)) \
X(void, glDrawElements, (GLenum mode, GLsizei count, GLenum type, const void * indices)) \
X(void, glDrawPixels, (GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels)) \
X(void, glEdgeFlag, (GLboolean flag)) \
X(void, glEdgeFlagv, (const GLboolean * flag)) \
X(void, glEnable, (GLenum cap)) \
X(void, glEnableClientState, (GLenum array)) \
X(void, glEnd, ()) \
X(void, glEndList, ()) \
X(void, glEvalCoord1d, (GLdouble u)) \
X(void, glEvalCoord1dv, (const GLdouble * u)) \
X(void, glEvalCoord1f, (GLfloat u)) \
X(void, glEvalCoord1fv, (const GLfloat * u)) \
X(void, glEvalCoord2d, (GLdouble u, GLdouble v)) \
X(void, glEvalCoord2dv, (const GLdouble * u)) \
X(void, glEvalCoord2f, (GLfloat u, GLfloat v)) \
X(void, glEvalCoord2fv, (const GLfloat * u)) \
X(void, glEvalMesh1, (GLenum mode, GLint i1, GLint i2)) \
X(void, glEvalMesh2, (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)) \
X(void, glEvalPoint1, (GLint i)) \
X(void, glEvalPoint2, (GLint i, GLint j)) \
X(void, glFeedbackBuffer, (GLsizei size, GLenum type, GLfloat * buffer)) \
X(void, glFinish, ()) \
X(void, glFlush, ()) \
X(void, glFogf, (GLenum pname, GLfloat param)) \
X(void, glFogfv, (GLenum pname, const GLfloat * params)) \
X(void, glFogi, (GLenum pname, GLint param)) \
X(void, glFogiv, (GLenum pname, const GLint * params)) \
X(void, glFogx, (GLenum pname, GLfixed param)) \
X(void, glFogxv, (GLenum pname, const GLfixed * param)) \
X(void, glFrontFace, (GLenum mode)) \
X(void, glFrustum, (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)) \
X(void, glFrustumf, (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)) \
X(void, glFrustumx, (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f)) \
X(void, glGenBuffers, (GLsizei n, GLuint * buffers)) \
X(GLuint, glGenLists, (GLsizei range)) \
X(void, glGenTextures, (GLsizei n, GLuint * textures)) \
X(void, glGetBooleanv, (GLenum pname, GLboolean * data)) \
X(void, glGetBufferParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(void, glGetClipPlane, (GLenum plane, GLdouble * equation)) \
X(void, glGetClipPlanef, (GLenum plane, GLfloat * equation)) \
X(void, glGetClipPlanex, (GLenum plane, GLfixed * equation)) \
X(void, glGetDoublev, (GLenum pname, GLdouble * data)) \
X(GLenum, glGetError, ()) \
X(void, glGetFixedv, (GLenum pname, GLfixed * params)) \
X(void, glGetFloatv, (GLenum pname, GLfloat * data)) \
X(void, glGetIntegerv, (GLenum pname, GLint * data)) \
X(void, glGetLightfv, (GLenum light, GLenum pname, GLfloat * params)) \
X(void, glGetLightiv, (GLenum light, GLenum pname, GLint * params)) \
X(void, glGetLightxv, (GLenum light, GLenum pname, GLfixed * params)) \
X(void, glGetMapdv, (GLenum target, GLenum query, GLdouble * v)) \
X(void, glGetMapfv, (GLenum target, GLenum query, GLfloat * v)) \
X(void, glGetMapiv, (GLenum target, GLenum query, GLint * v)) \
X(void, glGetMaterialfv, (GLenum face, GLenum pname, GLfloat * params)) \
X(void, glGetMaterialiv, (GLenum face, GLenum pname, GLint * params)) \
X(void, glGetMaterialxv, (GLenum face, GLenum pname, GLfixed * params)) \
X(void, glGetPixelMapfv, (GLenum map, GLfloat * values)) \
X(void, glGetPixelMapuiv, (GLenum map, GLuint * values)) \
X(void, glGetPixelMapusv, (GLenum map, GLushort * values)) \
X(void, glGetPointerv, (GLenum pname, void ** params)) \
X(void, glGetPolygonStipple, (GLubyte * mask)) \
X(const GLubyte *, glGetString, (GLenum name)) \
X(void, glGetTexEnvfv, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, glGetTexEnviv, (GLenum target, GLenum pname, GLint * params)) \
X(void, glGetTexEnvxv, (GLenum target, GLenum pname, GLfixed * params)) \
X(void, glGetTexGendv, (GLenum coord, GLenum pname, GLdouble * params)) \
X(void, glGetTexGenfv, (GLenum coord, GLenum pname, GLfloat * params)) \
X(void, glGetTexGeniv, (GLenum coord, GLenum pname, GLint * params)) \
X(void, glGetTexImage, (GLenum target, GLint level, GLenum format, GLenum type, void * pixels)) \
X(void, glGetTexLevelParameterfv, (GLenum target, GLint level, GLenum pname, GLfloat * params)) \
X(void, glGetTexLevelParameteriv, (GLenum target, GLint level, GLenum pname, GLint * params)) \
X(void, glGetTexParameterfv, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, glGetTexParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(void, glGetTexParameterxv, (GLenum target, GLenum pname, GLfixed * params)) \
X(void, glHint, (GLenum target, GLenum mode)) \
X(void, glIndexMask, (GLuint mask)) \
X(void, glIndexd, (GLdouble c)) \
X(void, glIndexdv, (const GLdouble * c)) \
X(void, glIndexf, (GLfloat c)) \
X(void, glIndexfv, (const GLfloat * c)) \
X(void, glIndexi, (GLint c)) \
X(void, glIndexiv, (const GLint * c)) \
X(void, glIndexs, (GLshort c)) \
X(void, glIndexsv, (const GLshort * c)) \
X(void, glInitNames, ()) \
X(GLboolean, glIsBuffer, (GLuint buffer)) \
X(GLboolean, glIsEnabled, (GLenum cap)) \
X(GLboolean, glIsList, (GLuint list)) \
X(GLboolean, glIsTexture, (GLuint texture)) \
X(void, glLightModelf, (GLenum pname, GLfloat param)) \
X(void, glLightModelfv, (GLenum pname, const GLfloat * params)) \
X(void, glLightModeli, (GLenum pname, GLint param)) \
X(void, glLightModeliv, (GLenum pname, const GLint * params)) \
X(void, glLightModelx, (GLenum pname, GLfixed param)) \
X(void, glLightModelxv, (GLenum pname, const GLfixed * param)) \
X(void, glLightf, (GLenum light, GLenum pname, GLfloat param)) \
X(void, glLightfv, (GLenum light, GLenum pname, const GLfloat * params)) \
X(void, glLighti, (GLenum light, GLenum pname, GLint param)) \
X(void, glLightiv, (GLenum light, GLenum pname, const GLint * params)) \
X(void, glLightx, (GLenum light, GLenum pname, GLfixed param)) \
X(void, glLightxv, (GLenum light, GLenum pname, const GLfixed * params)) \
X(void, glLineStipple, (GLint factor, GLushort pattern)) \
X(void, glLineWidth, (GLfloat width)) \
X(void, glLineWidthx, (GLfixed width)) \
X(void, glListBase, (GLuint base)) \
X(void, glLoadIdentity, ()) \
X(void, glLoadMatrixd, (const GLdouble * m)) \
X(void, glLoadMatrixf, (const GLfloat * m)) \
X(void, glLoadMatrixx, (const GLfixed * m)) \
X(void, glLoadName, (GLuint name)) \
X(void, glLogicOp, (GLenum opcode)) \
X(void, glMap1d, (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble * points)) \
X(void, glMap1f, (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat * points)) \
X(void, glMap2d, (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble * points)) \
X(void, glMap2f, (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat * points)) \
X(void, glMapGrid1d, (GLint un, GLdouble u1, GLdouble u2)) \
X(void, glMapGrid1f, (GLint un, GLfloat u1, GLfloat u2)) \
X(void, glMapGrid2d, (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2)) \
X(void, glMapGrid2f, (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2)) \
X(void, glMaterialf, (GLenum face, GLenum pname, GLfloat param)) \
X(void, glMaterialfv, (GLenum face, GLenum pname, const GLfloat * params)) \
X(void, glMateriali, (GLenum face, GLenum pname, GLint param)) \
X(void, glMaterialiv, (GLenum face, GLenum pname, const GLint * params)) \
X(void, glMaterialx, (GLenum face, GLenum pname, GLfixed param)) \
X(void, glMaterialxv, (GLenum face, GLenum pname, const GLfixed * param)) \
X(void, glMatrixMode, (GLenum mode)) \
X(void, glMultMatrixd, (const GLdouble * m)) \
X(void, glMultMatrixf, (const GLfloat * m)) \
X(void, glMultMatrixx, (const GLfixed * m)) \
X(void, glMultiTexCoord4f, (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)) \
X(void, glMultiTexCoord4x, (GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q)) \
X(void, glNewList, (GLuint list, GLenum mode)) \
X(void, glNormal3b, (GLbyte nx, GLbyte ny, GLbyte nz)) \
X(void, glNormal3bv, (const GLbyte * v)) \
X(void, glNormal3d, (GLdouble nx, GLdouble ny, GLdouble nz)) \
X(void, glNormal3dv, (const GLdouble * v)) \
X(void, glNormal3f, (GLfloat nx, GLfloat ny, GLfloat nz)) \
X(void, glNormal3fv, (const GLfloat * v)) \
X(void, glNormal3i, (GLint nx, GLint ny, GLint nz)) \
X(void, glNormal3iv, (const GLint * v)) \
X(void, glNormal3s, (GLshort nx, GLshort ny, GLshort nz)) \
X(void, glNormal3sv, (const GLshort * v)) \
X(void, glNormal3x, (GLfixed nx, GLfixed ny, GLfixed nz)) \
X(void, glNormalPointer, (GLenum type, GLsizei stride, const void * pointer)) \
X(void, glOrtho, (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)) \
X(void, glOrthof, (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)) \
X(void, glOrthox, (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f)) \
X(void, glPassThrough, (GLfloat token)) \
X(void, glPixelMapfv, (GLenum map, GLsizei mapsize, const GLfloat * values)) \
X(void, glPixelMapuiv, (GLenum map, GLsizei mapsize, const GLuint * values)) \
X(void, glPixelMapusv, (GLenum map, GLsizei mapsize, const GLushort * values)) \
X(void, glPixelStoref, (GLenum pname, GLfloat param)) \
X(void, glPixelStorei, (GLenum pname, GLint param)) \
X(void, glPixelTransferf, (GLenum pname, GLfloat param)) \
X(void, glPixelTransferi, (GLenum pname, GLint param)) \
X(void, glPixelZoom, (GLfloat xfactor, GLfloat yfactor)) \
X(void, glPointParameterf, (GLenum pname, GLfloat param)) \
X(void, glPointParameterfv, (GLenum pname, const GLfloat * params)) \
X(void, glPointParameterx, (GLenum pname, GLfixed param)) \
X(void, glPointParameterxv, (GLenum pname, const GLfixed * params)) \
X(void, glPointSize, (GLfloat size)) \
X(void, glPointSizex, (GLfixed size)) \
X(void, glPolygonMode, (GLenum face, GLenum mode)) \
X(void, glPolygonOffset, (GLfloat factor, GLfloat units)) \
X(void, glPolygonOffsetx, (GLfixed factor, GLfixed units)) \
X(void, glPolygonStipple, (const GLubyte * mask)) \
X(void, glPopAttrib, ()) \
X(void, glPopMatrix, ()) \
X(void, glPopName, ()) \
X(void, glPushAttrib, (GLbitfield mask)) \
X(void, glPushMatrix, ()) \
X(void, glPushName, (GLuint name)) \
X(void, glRasterPos2d, (GLdouble x, GLdouble y)) \
X(void, glRasterPos2dv, (const GLdouble * v)) \
X(void, glRasterPos2f, (GLfloat x, GLfloat y)) \
X(void, glRasterPos2fv, (const GLfloat * v)) \
X(void, glRasterPos2i, (GLint x, GLint y)) \
X(void, glRasterPos2iv, (const GLint * v)) \
X(void, glRasterPos2s, (GLshort x, GLshort y)) \
X(void, glRasterPos2sv, (const GLshort * v)) \
X(void, glRasterPos3d, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, glRasterPos3dv, (const GLdouble * v)) \
X(void, glRasterPos3f, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, glRasterPos3fv, (const GLfloat * v)) \
X(void, glRasterPos3i, (GLint x, GLint y, GLint z)) \
X(void, glRasterPos3iv, (const GLint * v)) \
X(void, glRasterPos3s, (GLshort x, GLshort y, GLshort z)) \
X(void, glRasterPos3sv, (const GLshort * v)) \
X(void, glRasterPos4d, (GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, glRasterPos4dv, (const GLdouble * v)) \
X(void, glRasterPos4f, (GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, glRasterPos4fv, (const GLfloat * v)) \
X(void, glRasterPos4i, (GLint x, GLint y, GLint z, GLint w)) \
X(void, glRasterPos4iv, (const GLint * v)) \
X(void, glRasterPos4s, (GLshort x, GLshort y, GLshort z, GLshort w)) \
X(void, glRasterPos4sv, (const GLshort * v)) \
X(void, glReadBuffer, (GLenum src)) \
X(void, glReadPixels, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * pixels)) \
X(void, glRectd, (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)) \
X(void, glRectdv, (const GLdouble * v1, const GLdouble * v2)) \
X(void, glRectf, (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)) \
X(void, glRectfv, (const GLfloat * v1, const GLfloat * v2)) \
X(void, glRecti, (GLint x1, GLint y1, GLint x2, GLint y2)) \
X(void, glRectiv, (const GLint * v1, const GLint * v2)) \
X(void, glRects, (GLshort x1, GLshort y1, GLshort x2, GLshort y2)) \
X(void, glRectsv, (const GLshort * v1, const GLshort * v2)) \
X(GLint, glRenderMode, (GLenum mode)) \
X(void, glRotated, (GLdouble angle, GLdouble x, GLdouble y, GLdouble z)) \
X(void, glRotatef, (GLfloat angle, GLfloat x, GLfloat y, GLfloat z)) \
X(void, glRotatex, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z)) \
X(void, glSampleCoverage, (GLfloat value, GLboolean invert)) \
X(void, glSampleCoveragex, (GLclampx value, GLboolean invert)) \
X(void, glScaled, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, glScalef, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, glScalex, (GLfixed x, GLfixed y, GLfixed z)) \
X(void, glScissor, (GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, glSelectBuffer, (GLsizei size, GLuint * buffer)) \
X(void, glShadeModel, (GLenum mode)) \
X(void, glStencilFunc, (GLenum func, GLint ref, GLuint mask)) \
X(void, glStencilMask, (GLuint mask)) \
X(void, glStencilOp, (GLenum fail, GLenum zfail, GLenum zpass)) \
X(void, glTexCoord1d, (GLdouble s)) \
X(void, glTexCoord1dv, (const GLdouble * v)) \
X(void, glTexCoord1f, (GLfloat s)) \
X(void, glTexCoord1fv, (const GLfloat * v)) \
X(void, glTexCoord1i, (GLint s)) \
X(void, glTexCoord1iv, (const GLint * v)) \
X(void, glTexCoord1s, (GLshort s)) \
X(void, glTexCoord1sv, (const GLshort * v)) \
X(void, glTexCoord2d, (GLdouble s, GLdouble t)) \
X(void, glTexCoord2dv, (const GLdouble * v)) \
X(void, glTexCoord2f, (GLfloat s, GLfloat t)) \
X(void, glTexCoord2fv, (const GLfloat * v)) \
X(void, glTexCoord2i, (GLint s, GLint t)) \
X(void, glTexCoord2iv, (const GLint * v)) \
X(void, glTexCoord2s, (GLshort s, GLshort t)) \
X(void, glTexCoord2sv, (const GLshort * v)) \
X(void, glTexCoord3d, (GLdouble s, GLdouble t, GLdouble r)) \
X(void, glTexCoord3dv, (const GLdouble * v)) \
X(void, glTexCoord3f, (GLfloat s, GLfloat t, GLfloat r)) \
X(void, glTexCoord3fv, (const GLfloat * v)) \
X(void, glTexCoord3i, (GLint s, GLint t, GLint r)) \
X(void, glTexCoord3iv, (const GLint * v)) \
X(void, glTexCoord3s, (GLshort s, GLshort t, GLshort r)) \
X(void, glTexCoord3sv, (const GLshort * v)) \
X(void, glTexCoord4d, (GLdouble s, GLdouble t, GLdouble r, GLdouble q)) \
X(void, glTexCoord4dv, (const GLdouble * v)) \
X(void, glTexCoord4f, (GLfloat s, GLfloat t, GLfloat r, GLfloat q)) \
X(void, glTexCoord4fv, (const GLfloat * v)) \
X(void, glTexCoord4i, (GLint s, GLint t, GLint r, GLint q)) \
X(void, glTexCoord4iv, (const GLint * v)) \
X(void, glTexCoord4s, (GLshort s, GLshort t, GLshort r, GLshort q)) \
X(void, glTexCoord4sv, (const GLshort * v)) \
X(void, glTexCoordPointer, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, glTexEnvf, (GLenum target, GLenum pname, GLfloat param)) \
X(void, glTexEnvfv, (GLenum target, GLenum pname, const GLfloat * params)) \
X(void, glTexEnvi, (GLenum target, GLenum pname, GLint param)) \
X(void, glTexEnviv, (GLenum target, GLenum pname, const GLint * params)) \
X(void, glTexEnvx, (GLenum target, GLenum pname, GLfixed param)) \
X(void, glTexEnvxv, (GLenum target, GLenum pname, const GLfixed * params)) \
X(void, glTexGend, (GLenum coord, GLenum pname, GLdouble param)) \
X(void, glTexGendv, (GLenum coord, GLenum pname, const GLdouble * params)) \
X(void, glTexGenf, (GLenum coord, GLenum pname, GLfloat param)) \
X(void, glTexGenfv, (GLenum coord, GLenum pname, const GLfloat * params)) \
X(void, glTexGeni, (GLenum coord, GLenum pname, GLint param)) \
X(void, glTexGeniv, (GLenum coord, GLenum pname, const GLint * params)) \
X(void, glTexImage1D, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, glTexImage2D, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, glTexParameterf, (GLenum target, GLenum pname, GLfloat param)) \
X(void, glTexParameterfv, (GLenum target, GLenum pname, const GLfloat * params)) \
X(void, glTexParameteri, (GLenum target, GLenum pname, GLint param)) \
X(void, glTexParameteriv, (GLenum target, GLenum pname, const GLint * params)) \
X(void, glTexParameterx, (GLenum target, GLenum pname, GLfixed param)) \
X(void, glTexParameterxv, (GLenum target, GLenum pname, const GLfixed * params)) \
X(void, glTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels)) \
X(void, glTranslated, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, glTranslatef, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, glTranslatex, (GLfixed x, GLfixed y, GLfixed z)) \
X(void, glVertex2d, (GLdouble x, GLdouble y)) \
X(void, glVertex2dv, (const GLdouble * v)) \
X(void, glVertex2f, (GLfloat x, GLfloat y)) \
X(void, glVertex2fv, (const GLfloat * v)) \
X(void, glVertex2i, (GLint x, GLint y)) \
X(void, glVertex2iv, (const GLint * v)) \
X(void, glVertex2s, (GLshort x, GLshort y)) \
X(void, glVertex2sv, (const GLshort * v)) \
X(void, glVertex3d, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, glVertex3dv, (const GLdouble * v)) \
X(void, glVertex3f, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, glVertex3fv, (const GLfloat * v)) \
X(void, glVertex3i, (GLint x, GLint y, GLint z)) \
X(void, glVertex3iv, (const GLint * v)) \
X(void, glVertex3s, (GLshort x, GLshort y, GLshort z)) \
X(void, glVertex3sv, (const GLshort * v)) \
X(void, glVertex4d, (GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, glVertex4dv, (const GLdouble * v)) \
X(void, glVertex4f, (GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, glVertex4fv, (const GLfloat * v)) \
X(void, glVertex4i, (GLint x, GLint y, GLint z, GLint w)) \
X(void, glVertex4iv, (const GLint * v)) \
X(void, glVertex4s, (GLshort x, GLshort y, GLshort z, GLshort w)) \
X(void, glVertex4sv, (const GLshort * v)) \
X(void, glVertexPointer, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, glViewport, (GLint x, GLint y, GLsizei width, GLsizei height)) \
VE() \
V(1_1) \
X(GLboolean, glAreTexturesResident, (GLsizei n, const GLuint * textures, GLboolean * residences)) \
X(void, glArrayElement, (GLint i)) \
X(void, glCopyTexImage1D, (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)) \
X(void, glCopyTexSubImage1D, (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)) \
X(void, glEdgeFlagPointer, (GLsizei stride, const void * pointer)) \
X(void, glIndexPointer, (GLenum type, GLsizei stride, const void * pointer)) \
X(void, glIndexub, (GLubyte c)) \
X(void, glIndexubv, (const GLubyte * c)) \
X(void, glInterleavedArrays, (GLenum format, GLsizei stride, const void * pointer)) \
X(void, glPopClientAttrib, ()) \
X(void, glPrioritizeTextures, (GLsizei n, const GLuint * textures, const GLfloat * priorities)) \
X(void, glPushClientAttrib, (GLbitfield mask)) \
X(void, glTexSubImage1D, (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels)) \
VE() \
V(1_2) \
X(void, glCopyTexSubImage3D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, glDrawRangeElements, (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices)) \
X(void, glTexImage3D, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, glTexSubImage3D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels)) \
VE() \
V(1_3) \
X(void, glCompressedTexImage1D, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * data)) \
X(void, glCompressedTexImage3D, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * data)) \
X(void, glCompressedTexSubImage1D, (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data)) \
X(void, glCompressedTexSubImage3D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data)) \
X(void, glGetCompressedTexImage, (GLenum target, GLint level, void * img)) \
X(void, glLoadTransposeMatrixd, (const GLdouble * m)) \
X(void, glLoadTransposeMatrixf, (const GLfloat * m)) \
X(void, glMultTransposeMatrixd, (const GLdouble * m)) \
X(void, glMultTransposeMatrixf, (const GLfloat * m)) \
X(void, glMultiTexCoord1d, (GLenum target, GLdouble s)) \
X(void, glMultiTexCoord1dv, (GLenum target, const GLdouble * v)) \
X(void, glMultiTexCoord1f, (GLenum target, GLfloat s)) \
X(void, glMultiTexCoord1fv, (GLenum target, const GLfloat * v)) \
X(void, glMultiTexCoord1i, (GLenum target, GLint s)) \
X(void, glMultiTexCoord1iv, (GLenum target, const GLint * v)) \
X(void, glMultiTexCoord1s, (GLenum target, GLshort s)) \
X(void, glMultiTexCoord1sv, (GLenum target, const GLshort * v)) \
X(void, glMultiTexCoord2d, (GLenum target, GLdouble s, GLdouble t)) \
X(void, glMultiTexCoord2dv, (GLenum target, const GLdouble * v)) \
X(void, glMultiTexCoord2f, (GLenum target, GLfloat s, GLfloat t)) \
X(void, glMultiTexCoord2fv, (GLenum target, const GLfloat * v)) \
X(void, glMultiTexCoord2i, (GLenum target, GLint s, GLint t)) \
X(void, glMultiTexCoord2iv, (GLenum target, const GLint * v)) \
X(void, glMultiTexCoord2s, (GLenum target, GLshort s, GLshort t)) \
X(void, glMultiTexCoord2sv, (GLenum target, const GLshort * v)) \
X(void, glMultiTexCoord3d, (GLenum target, GLdouble s, GLdouble t, GLdouble r)) \
X(void, glMultiTexCoord3dv, (GLenum target, const GLdouble * v)) \
X(void, glMultiTexCoord3f, (GLenum target, GLfloat s, GLfloat t, GLfloat r)) \
X(void, glMultiTexCoord3fv, (GLenum target, const GLfloat * v)) \
X(void, glMultiTexCoord3i, (GLenum target, GLint s, GLint t, GLint r)) \
X(void, glMultiTexCoord3iv, (GLenum target, const GLint * v)) \
X(void, glMultiTexCoord3s, (GLenum target, GLshort s, GLshort t, GLshort r)) \
X(void, glMultiTexCoord3sv, (GLenum target, const GLshort * v)) \
X(void, glMultiTexCoord4d, (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)) \
X(void, glMultiTexCoord4dv, (GLenum target, const GLdouble * v)) \
X(void, glMultiTexCoord4fv, (GLenum target, const GLfloat * v)) \
X(void, glMultiTexCoord4i, (GLenum target, GLint s, GLint t, GLint r, GLint q)) \
X(void, glMultiTexCoord4iv, (GLenum target, const GLint * v)) \
X(void, glMultiTexCoord4s, (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)) \
X(void, glMultiTexCoord4sv, (GLenum target, const GLshort * v)) \
VE() \
V(1_4) \
X(void, glBlendColor, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)) \
X(void, glBlendEquation, (GLenum mode)) \
X(void, glBlendFuncSeparate, (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)) \
X(void, glFogCoordPointer, (GLenum type, GLsizei stride, const void * pointer)) \
X(void, glFogCoordd, (GLdouble coord)) \
X(void, glFogCoorddv, (const GLdouble * coord)) \
X(void, glFogCoordf, (GLfloat coord)) \
X(void, glFogCoordfv, (const GLfloat * coord)) \
X(void, glMultiDrawArrays, (GLenum mode, const GLint * first, const GLsizei * count, GLsizei drawcount)) \
X(void, glMultiDrawElements, (GLenum mode, const GLsizei * count, GLenum type, const void *const * indices, GLsizei drawcount)) \
X(void, glPointParameteri, (GLenum pname, GLint param)) \
X(void, glPointParameteriv, (GLenum pname, const GLint * params)) \
X(void, glSecondaryColor3b, (GLbyte red, GLbyte green, GLbyte blue)) \
X(void, glSecondaryColor3bv, (const GLbyte * v)) \
X(void, glSecondaryColor3d, (GLdouble red, GLdouble green, GLdouble blue)) \
X(void, glSecondaryColor3dv, (const GLdouble * v)) \
X(void, glSecondaryColor3f, (GLfloat red, GLfloat green, GLfloat blue)) \
X(void, glSecondaryColor3fv, (const GLfloat * v)) \
X(void, glSecondaryColor3i, (GLint red, GLint green, GLint blue)) \
X(void, glSecondaryColor3iv, (const GLint * v)) \
X(void, glSecondaryColor3s, (GLshort red, GLshort green, GLshort blue)) \
X(void, glSecondaryColor3sv, (const GLshort * v)) \
X(void, glSecondaryColor3ub, (GLubyte red, GLubyte green, GLubyte blue)) \
X(void, glSecondaryColor3ubv, (const GLubyte * v)) \
X(void, glSecondaryColor3ui, (GLuint red, GLuint green, GLuint blue)) \
X(void, glSecondaryColor3uiv, (const GLuint * v)) \
X(void, glSecondaryColor3us, (GLushort red, GLushort green, GLushort blue)) \
X(void, glSecondaryColor3usv, (const GLushort * v)) \
X(void, glSecondaryColorPointer, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, glWindowPos2d, (GLdouble x, GLdouble y)) \
X(void, glWindowPos2dv, (const GLdouble * v)) \
X(void, glWindowPos2f, (GLfloat x, GLfloat y)) \
X(void, glWindowPos2fv, (const GLfloat * v)) \
X(void, glWindowPos2i, (GLint x, GLint y)) \
X(void, glWindowPos2iv, (const GLint * v)) \
X(void, glWindowPos2s, (GLshort x, GLshort y)) \
X(void, glWindowPos2sv, (const GLshort * v)) \
X(void, glWindowPos3d, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, glWindowPos3dv, (const GLdouble * v)) \
X(void, glWindowPos3f, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, glWindowPos3fv, (const GLfloat * v)) \
X(void, glWindowPos3i, (GLint x, GLint y, GLint z)) \
X(void, glWindowPos3iv, (const GLint * v)) \
X(void, glWindowPos3s, (GLshort x, GLshort y, GLshort z)) \
X(void, glWindowPos3sv, (const GLshort * v)) \
VE() \
V(1_5) \
X(void, glBeginQuery, (GLenum target, GLuint id)) \
X(void, glDeleteQueries, (GLsizei n, const GLuint * ids)) \
X(void, glEndQuery, (GLenum target)) \
X(void, glGenQueries, (GLsizei n, GLuint * ids)) \
X(void, glGetBufferPointerv, (GLenum target, GLenum pname, void ** params)) \
X(void, glGetBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, void * data)) \
X(void, glGetQueryObjectiv, (GLuint id, GLenum pname, GLint * params)) \
X(void, glGetQueryObjectuiv, (GLuint id, GLenum pname, GLuint * params)) \
X(void, glGetQueryiv, (GLenum target, GLenum pname, GLint * params)) \
X(GLboolean, glIsQuery, (GLuint id)) \
X(void *, glMapBuffer, (GLenum target, GLenum access)) \
X(GLboolean, glUnmapBuffer, (GLenum target)) \
VE() \
V(2_0) \
X(void, glAttachShader, (GLuint program, GLuint shader)) \
X(void, glBindAttribLocation, (GLuint program, GLuint index, const GLchar * name)) \
X(void, glBindFramebuffer, (GLenum target, GLuint framebuffer)) \
X(void, glBindRenderbuffer, (GLenum target, GLuint renderbuffer)) \
X(void, glBlendEquationSeparate, (GLenum modeRGB, GLenum modeAlpha)) \
X(GLenum, glCheckFramebufferStatus, (GLenum target)) \
X(void, glCompileShader, (GLuint shader)) \
X(GLuint, glCreateProgram, ()) \
X(GLuint, glCreateShader, (GLenum type)) \
X(void, glDeleteFramebuffers, (GLsizei n, const GLuint * framebuffers)) \
X(void, glDeleteProgram, (GLuint program)) \
X(void, glDeleteRenderbuffers, (GLsizei n, const GLuint * renderbuffers)) \
X(void, glDeleteShader, (GLuint shader)) \
X(void, glDetachShader, (GLuint program, GLuint shader)) \
X(void, glDisableVertexAttribArray, (GLuint index)) \
X(void, glDrawBuffers, (GLsizei n, const GLenum * bufs)) \
X(void, glEnableVertexAttribArray, (GLuint index)) \
X(void, glFramebufferRenderbuffer, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)) \
X(void, glFramebufferTexture2D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)) \
X(void, glGenFramebuffers, (GLsizei n, GLuint * framebuffers)) \
X(void, glGenRenderbuffers, (GLsizei n, GLuint * renderbuffers)) \
X(void, glGenerateMipmap, (GLenum target)) \
X(void, glGetActiveAttrib, (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name)) \
X(void, glGetActiveUniform, (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name)) \
X(void, glGetAttachedShaders, (GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders)) \
X(GLint, glGetAttribLocation, (GLuint program, const GLchar * name)) \
X(void, glGetFramebufferAttachmentParameteriv, (GLenum target, GLenum attachment, GLenum pname, GLint * params)) \
X(GLenum, glGetGraphicsResetStatus, ()) \
X(void, glGetProgramInfoLog, (GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog)) \
X(void, glGetProgramiv, (GLuint program, GLenum pname, GLint * params)) \
X(void, glGetRenderbufferParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(void, glGetShaderInfoLog, (GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog)) \
X(void, glGetShaderPrecisionFormat, (GLenum shadertype, GLenum precisiontype, GLint * range, GLint * precision)) \
X(void, glGetShaderSource, (GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source)) \
X(void, glGetShaderiv, (GLuint shader, GLenum pname, GLint * params)) \
X(GLint, glGetUniformLocation, (GLuint program, const GLchar * name)) \
X(void, glGetUniformfv, (GLuint program, GLint location, GLfloat * params)) \
X(void, glGetUniformiv, (GLuint program, GLint location, GLint * params)) \
X(void, glGetVertexAttribPointerv, (GLuint index, GLenum pname, void ** pointer)) \
X(void, glGetVertexAttribdv, (GLuint index, GLenum pname, GLdouble * params)) \
X(void, glGetVertexAttribfv, (GLuint index, GLenum pname, GLfloat * params)) \
X(void, glGetVertexAttribiv, (GLuint index, GLenum pname, GLint * params)) \
X(void, glGetnUniformfv, (GLuint program, GLint location, GLsizei bufSize, GLfloat * params)) \
X(void, glGetnUniformiv, (GLuint program, GLint location, GLsizei bufSize, GLint * params)) \
X(GLboolean, glIsFramebuffer, (GLuint framebuffer)) \
X(GLboolean, glIsProgram, (GLuint program)) \
X(GLboolean, glIsRenderbuffer, (GLuint renderbuffer)) \
X(GLboolean, glIsShader, (GLuint shader)) \
X(void, glLinkProgram, (GLuint program)) \
X(void, glProgramBinary, (GLuint program, GLenum binaryFormat, const void * binary, GLsizei length)) \
X(void, glReadnPixels, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void * data)) \
X(void, glReleaseShaderCompiler, ()) \
X(void, glRenderbufferStorage, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, glShaderBinary, (GLsizei count, const GLuint * shaders, GLenum binaryFormat, const void * binary, GLsizei length)) \
X(void, glShaderSource, (GLuint shader, GLsizei count, const GLchar *const * string, const GLint * length)) \
X(void, glStencilFuncSeparate, (GLenum face, GLenum func, GLint ref, GLuint mask)) \
X(void, glStencilMaskSeparate, (GLenum face, GLuint mask)) \
X(void, glStencilOpSeparate, (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)) \
X(void, glTexStorage2D, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, glUniform1f, (GLint location, GLfloat v0)) \
X(void, glUniform1fv, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, glUniform1i, (GLint location, GLint v0)) \
X(void, glUniform1iv, (GLint location, GLsizei count, const GLint * value)) \
X(void, glUniform2f, (GLint location, GLfloat v0, GLfloat v1)) \
X(void, glUniform2fv, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, glUniform2i, (GLint location, GLint v0, GLint v1)) \
X(void, glUniform2iv, (GLint location, GLsizei count, const GLint * value)) \
X(void, glUniform3f, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2)) \
X(void, glUniform3fv, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, glUniform3i, (GLint location, GLint v0, GLint v1, GLint v2)) \
X(void, glUniform3iv, (GLint location, GLsizei count, const GLint * value)) \
X(void, glUniform4f, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)) \
X(void, glUniform4fv, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, glUniform4i, (GLint location, GLint v0, GLint v1, GLint v2, GLint v3)) \
X(void, glUniform4iv, (GLint location, GLsizei count, const GLint * value)) \
X(void, glUniformMatrix2fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glUniformMatrix3fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glUniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glUseProgram, (GLuint program)) \
X(void, glValidateProgram, (GLuint program)) \
X(void, glVertexAttrib1d, (GLuint index, GLdouble x)) \
X(void, glVertexAttrib1dv, (GLuint index, const GLdouble * v)) \
X(void, glVertexAttrib1f, (GLuint index, GLfloat x)) \
X(void, glVertexAttrib1fv, (GLuint index, const GLfloat * v)) \
X(void, glVertexAttrib1s, (GLuint index, GLshort x)) \
X(void, glVertexAttrib1sv, (GLuint index, const GLshort * v)) \
X(void, glVertexAttrib2d, (GLuint index, GLdouble x, GLdouble y)) \
X(void, glVertexAttrib2dv, (GLuint index, const GLdouble * v)) \
X(void, glVertexAttrib2f, (GLuint index, GLfloat x, GLfloat y)) \
X(void, glVertexAttrib2fv, (GLuint index, const GLfloat * v)) \
X(void, glVertexAttrib2s, (GLuint index, GLshort x, GLshort y)) \
X(void, glVertexAttrib2sv, (GLuint index, const GLshort * v)) \
X(void, glVertexAttrib3d, (GLuint index, GLdouble x, GLdouble y, GLdouble z)) \
X(void, glVertexAttrib3dv, (GLuint index, const GLdouble * v)) \
X(void, glVertexAttrib3f, (GLuint index, GLfloat x, GLfloat y, GLfloat z)) \
X(void, glVertexAttrib3fv, (GLuint index, const GLfloat * v)) \
X(void, glVertexAttrib3s, (GLuint index, GLshort x, GLshort y, GLshort z)) \
X(void, glVertexAttrib3sv, (GLuint index, const GLshort * v)) \
X(void, glVertexAttrib4Nbv, (GLuint index, const GLbyte * v)) \
X(void, glVertexAttrib4Niv, (GLuint index, const GLint * v)) \
X(void, glVertexAttrib4Nsv, (GLuint index, const GLshort * v)) \
X(void, glVertexAttrib4Nub, (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w)) \
X(void, glVertexAttrib4Nubv, (GLuint index, const GLubyte * v)) \
X(void, glVertexAttrib4Nuiv, (GLuint index, const GLuint * v)) \
X(void, glVertexAttrib4Nusv, (GLuint index, const GLushort * v)) \
X(void, glVertexAttrib4bv, (GLuint index, const GLbyte * v)) \
X(void, glVertexAttrib4d, (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, glVertexAttrib4dv, (GLuint index, const GLdouble * v)) \
X(void, glVertexAttrib4f, (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, glVertexAttrib4fv, (GLuint index, const GLfloat * v)) \
X(void, glVertexAttrib4iv, (GLuint index, const GLint * v)) \
X(void, glVertexAttrib4s, (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w)) \
X(void, glVertexAttrib4sv, (GLuint index, const GLshort * v)) \
X(void, glVertexAttrib4ubv, (GLuint index, const GLubyte * v)) \
X(void, glVertexAttrib4uiv, (GLuint index, const GLuint * v)) \
X(void, glVertexAttrib4usv, (GLuint index, const GLushort * v)) \
X(void, glVertexAttribPointer, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer)) \
VE() \
V(2_1) \
X(void, glUniformMatrix2x3fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glUniformMatrix2x4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glUniformMatrix3x2fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glUniformMatrix3x4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glUniformMatrix4x2fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glUniformMatrix4x3fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
VE() \
V(3_0) \
X(void, glBeginConditionalRender, (GLuint id, GLenum mode)) \
X(void, glBeginTransformFeedback, (GLenum primitiveMode)) \
X(void, glBindBufferBase, (GLenum target, GLuint index, GLuint buffer)) \
X(void, glBindBufferRange, (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
X(void, glBindFragDataLocation, (GLuint program, GLuint color, const GLchar * name)) \
X(void, glBindSampler, (GLuint unit, GLuint sampler)) \
X(void, glBindTransformFeedback, (GLenum target, GLuint id)) \
X(void, glBindVertexArray, (GLuint array)) \
X(void, glBlitFramebuffer, (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)) \
X(void, glClampColor, (GLenum target, GLenum clamp)) \
X(void, glClearBufferfi, (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)) \
X(void, glClearBufferfv, (GLenum buffer, GLint drawbuffer, const GLfloat * value)) \
X(void, glClearBufferiv, (GLenum buffer, GLint drawbuffer, const GLint * value)) \
X(void, glClearBufferuiv, (GLenum buffer, GLint drawbuffer, const GLuint * value)) \
X(GLenum, glClientWaitSync, (GLsync sync, GLbitfield flags, GLuint64 timeout)) \
X(void, glColorMaski, (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a)) \
X(void, glCopyBufferSubData, (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)) \
X(void, glDeleteSamplers, (GLsizei count, const GLuint * samplers)) \
X(void, glDeleteSync, (GLsync sync)) \
X(void, glDeleteTransformFeedbacks, (GLsizei n, const GLuint * ids)) \
X(void, glDeleteVertexArrays, (GLsizei n, const GLuint * arrays)) \
X(void, glDisablei, (GLenum target, GLuint index)) \
X(void, glDrawArraysInstanced, (GLenum mode, GLint first, GLsizei count, GLsizei instancecount)) \
X(void, glDrawElementsInstanced, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount)) \
X(void, glEnablei, (GLenum target, GLuint index)) \
X(void, glEndConditionalRender, ()) \
X(void, glEndTransformFeedback, ()) \
X(GLsync, glFenceSync, (GLenum condition, GLbitfield flags)) \
X(void, glFlushMappedBufferRange, (GLenum target, GLintptr offset, GLsizeiptr length)) \
X(void, glFramebufferTexture1D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)) \
X(void, glFramebufferTexture3D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)) \
X(void, glFramebufferTextureLayer, (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)) \
X(void, glGenSamplers, (GLsizei count, GLuint * samplers)) \
X(void, glGenTransformFeedbacks, (GLsizei n, GLuint * ids)) \
X(void, glGenVertexArrays, (GLsizei n, GLuint * arrays)) \
X(void, glGetActiveUniformBlockName, (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformBlockName)) \
X(void, glGetActiveUniformBlockiv, (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint * params)) \
X(void, glGetActiveUniformsiv, (GLuint program, GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params)) \
X(void, glGetBooleani_v, (GLenum target, GLuint index, GLboolean * data)) \
X(void, glGetBufferParameteri64v, (GLenum target, GLenum pname, GLint64 * params)) \
X(GLint, glGetFragDataLocation, (GLuint program, const GLchar * name)) \
X(void, glGetInteger64i_v, (GLenum target, GLuint index, GLint64 * data)) \
X(void, glGetInteger64v, (GLenum pname, GLint64 * data)) \
X(void, glGetIntegeri_v, (GLenum target, GLuint index, GLint * data)) \
X(void, glGetInternalformativ, (GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint * params)) \
X(void, glGetProgramBinary, (GLuint program, GLsizei bufSize, GLsizei * length, GLenum * binaryFormat, void * binary)) \
X(void, glGetSamplerParameterfv, (GLuint sampler, GLenum pname, GLfloat * params)) \
X(void, glGetSamplerParameteriv, (GLuint sampler, GLenum pname, GLint * params)) \
X(const GLubyte *, glGetStringi, (GLenum name, GLuint index)) \
X(void, glGetSynciv, (GLsync sync, GLenum pname, GLsizei count, GLsizei * length, GLint * values)) \
X(void, glGetTexParameterIiv, (GLenum target, GLenum pname, GLint * params)) \
X(void, glGetTexParameterIuiv, (GLenum target, GLenum pname, GLuint * params)) \
X(void, glGetTransformFeedbackVarying, (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name)) \
X(GLuint, glGetUniformBlockIndex, (GLuint program, const GLchar * uniformBlockName)) \
X(void, glGetUniformIndices, (GLuint program, GLsizei uniformCount, const GLchar *const * uniformNames, GLuint * uniformIndices)) \
X(void, glGetUniformuiv, (GLuint program, GLint location, GLuint * params)) \
X(void, glGetVertexAttribIiv, (GLuint index, GLenum pname, GLint * params)) \
X(void, glGetVertexAttribIuiv, (GLuint index, GLenum pname, GLuint * params)) \
X(void, glInvalidateFramebuffer, (GLenum target, GLsizei numAttachments, const GLenum * attachments)) \
X(void, glInvalidateSubFramebuffer, (GLenum target, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(GLboolean, glIsEnabledi, (GLenum target, GLuint index)) \
X(GLboolean, glIsSampler, (GLuint sampler)) \
X(GLboolean, glIsSync, (GLsync sync)) \
X(GLboolean, glIsTransformFeedback, (GLuint id)) \
X(GLboolean, glIsVertexArray, (GLuint array)) \
X(void *, glMapBufferRange, (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)) \
X(void, glPauseTransformFeedback, ()) \
X(void, glProgramParameteri, (GLuint program, GLenum pname, GLint value)) \
X(void, glRenderbufferStorageMultisample, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, glResumeTransformFeedback, ()) \
X(void, glSamplerParameterf, (GLuint sampler, GLenum pname, GLfloat param)) \
X(void, glSamplerParameterfv, (GLuint sampler, GLenum pname, const GLfloat * param)) \
X(void, glSamplerParameteri, (GLuint sampler, GLenum pname, GLint param)) \
X(void, glSamplerParameteriv, (GLuint sampler, GLenum pname, const GLint * param)) \
X(void, glTexParameterIiv, (GLenum target, GLenum pname, const GLint * params)) \
X(void, glTexParameterIuiv, (GLenum target, GLenum pname, const GLuint * params)) \
X(void, glTexStorage3D, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)) \
X(void, glTransformFeedbackVaryings, (GLuint program, GLsizei count, const GLchar *const * varyings, GLenum bufferMode)) \
X(void, glUniform1ui, (GLint location, GLuint v0)) \
X(void, glUniform1uiv, (GLint location, GLsizei count, const GLuint * value)) \
X(void, glUniform2ui, (GLint location, GLuint v0, GLuint v1)) \
X(void, glUniform2uiv, (GLint location, GLsizei count, const GLuint * value)) \
X(void, glUniform3ui, (GLint location, GLuint v0, GLuint v1, GLuint v2)) \
X(void, glUniform3uiv, (GLint location, GLsizei count, const GLuint * value)) \
X(void, glUniform4ui, (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)) \
X(void, glUniform4uiv, (GLint location, GLsizei count, const GLuint * value)) \
X(void, glUniformBlockBinding, (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)) \
X(void, glVertexAttribDivisor, (GLuint index, GLuint divisor)) \
X(void, glVertexAttribI1i, (GLuint index, GLint x)) \
X(void, glVertexAttribI1iv, (GLuint index, const GLint * v)) \
X(void, glVertexAttribI1ui, (GLuint index, GLuint x)) \
X(void, glVertexAttribI1uiv, (GLuint index, const GLuint * v)) \
X(void, glVertexAttribI2i, (GLuint index, GLint x, GLint y)) \
X(void, glVertexAttribI2iv, (GLuint index, const GLint * v)) \
X(void, glVertexAttribI2ui, (GLuint index, GLuint x, GLuint y)) \
X(void, glVertexAttribI2uiv, (GLuint index, const GLuint * v)) \
X(void, glVertexAttribI3i, (GLuint index, GLint x, GLint y, GLint z)) \
X(void, glVertexAttribI3iv, (GLuint index, const GLint * v)) \
X(void, glVertexAttribI3ui, (GLuint index, GLuint x, GLuint y, GLuint z)) \
X(void, glVertexAttribI3uiv, (GLuint index, const GLuint * v)) \
X(void, glVertexAttribI4bv, (GLuint index, const GLbyte * v)) \
X(void, glVertexAttribI4i, (GLuint index, GLint x, GLint y, GLint z, GLint w)) \
X(void, glVertexAttribI4iv, (GLuint index, const GLint * v)) \
X(void, glVertexAttribI4sv, (GLuint index, const GLshort * v)) \
X(void, glVertexAttribI4ubv, (GLuint index, const GLubyte * v)) \
X(void, glVertexAttribI4ui, (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)) \
X(void, glVertexAttribI4uiv, (GLuint index, const GLuint * v)) \
X(void, glVertexAttribI4usv, (GLuint index, const GLushort * v)) \
X(void, glVertexAttribIPointer, (GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, glWaitSync, (GLsync sync, GLbitfield flags, GLuint64 timeout)) \
VE() \
V(3_1) \
X(void, glActiveShaderProgram, (GLuint pipeline, GLuint program)) \
X(void, glBindImageTexture, (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)) \
X(void, glBindProgramPipeline, (GLuint pipeline)) \
X(void, glBindVertexBuffer, (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)) \
X(GLuint, glCreateShaderProgramv, (GLenum type, GLsizei count, const GLchar *const * strings)) \
X(void, glDeleteProgramPipelines, (GLsizei n, const GLuint * pipelines)) \
X(void, glDispatchCompute, (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)) \
X(void, glDispatchComputeIndirect, (GLintptr indirect)) \
X(void, glDrawArraysIndirect, (GLenum mode, const void * indirect)) \
X(void, glDrawElementsIndirect, (GLenum mode, GLenum type, const void * indirect)) \
X(void, glFramebufferParameteri, (GLenum target, GLenum pname, GLint param)) \
X(void, glGenProgramPipelines, (GLsizei n, GLuint * pipelines)) \
X(void, glGetActiveUniformName, (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformName)) \
X(void, glGetFramebufferParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(void, glGetMultisamplefv, (GLenum pname, GLuint index, GLfloat * val)) \
X(void, glGetProgramInterfaceiv, (GLuint program, GLenum programInterface, GLenum pname, GLint * params)) \
X(void, glGetProgramPipelineInfoLog, (GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog)) \
X(void, glGetProgramPipelineiv, (GLuint pipeline, GLenum pname, GLint * params)) \
X(GLuint, glGetProgramResourceIndex, (GLuint program, GLenum programInterface, const GLchar * name)) \
X(GLint, glGetProgramResourceLocation, (GLuint program, GLenum programInterface, const GLchar * name)) \
X(void, glGetProgramResourceName, (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name)) \
X(void, glGetProgramResourceiv, (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum * props, GLsizei count, GLsizei * length, GLint * params)) \
X(GLboolean, glIsProgramPipeline, (GLuint pipeline)) \
X(void, glMemoryBarrier, (GLbitfield barriers)) \
X(void, glMemoryBarrierByRegion, (GLbitfield barriers)) \
X(void, glPrimitiveRestartIndex, (GLuint index)) \
X(void, glProgramUniform1f, (GLuint program, GLint location, GLfloat v0)) \
X(void, glProgramUniform1fv, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, glProgramUniform1i, (GLuint program, GLint location, GLint v0)) \
X(void, glProgramUniform1iv, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, glProgramUniform1ui, (GLuint program, GLint location, GLuint v0)) \
X(void, glProgramUniform1uiv, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, glProgramUniform2f, (GLuint program, GLint location, GLfloat v0, GLfloat v1)) \
X(void, glProgramUniform2fv, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, glProgramUniform2i, (GLuint program, GLint location, GLint v0, GLint v1)) \
X(void, glProgramUniform2iv, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, glProgramUniform2ui, (GLuint program, GLint location, GLuint v0, GLuint v1)) \
X(void, glProgramUniform2uiv, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, glProgramUniform3f, (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)) \
X(void, glProgramUniform3fv, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, glProgramUniform3i, (GLuint program, GLint location, GLint v0, GLint v1, GLint v2)) \
X(void, glProgramUniform3iv, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, glProgramUniform3ui, (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2)) \
X(void, glProgramUniform3uiv, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, glProgramUniform4f, (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)) \
X(void, glProgramUniform4fv, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, glProgramUniform4i, (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)) \
X(void, glProgramUniform4iv, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, glProgramUniform4ui, (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)) \
X(void, glProgramUniform4uiv, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, glProgramUniformMatrix2fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glProgramUniformMatrix2x3fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glProgramUniformMatrix2x4fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glProgramUniformMatrix3fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glProgramUniformMatrix3x2fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glProgramUniformMatrix3x4fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glProgramUniformMatrix4fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glProgramUniformMatrix4x2fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glProgramUniformMatrix4x3fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, glSampleMaski, (GLuint maskNumber, GLbitfield mask)) \
X(void, glTexBuffer, (GLenum target, GLenum internalformat, GLuint buffer)) \
X(void, glTexStorage2DMultisample, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)) \
X(void, glUseProgramStages, (GLuint pipeline, GLbitfield stages, GLuint program)) \
X(void, glValidateProgramPipeline, (GLuint pipeline)) \
X(void, glVertexAttribBinding, (GLuint attribindex, GLuint bindingindex)) \
X(void, glVertexAttribFormat, (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)) \
X(void, glVertexAttribIFormat, (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)) \
X(void, glVertexBindingDivisor, (GLuint bindingindex, GLuint divisor)) \
VE() \
V(3_2) \
X(void, glBlendBarrier, ()) \
X(void, glBlendEquationSeparatei, (GLuint buf, GLenum modeRGB, GLenum modeAlpha)) \
X(void, glBlendEquationi, (GLuint buf, GLenum mode)) \
X(void, glBlendFuncSeparatei, (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)) \
X(void, glBlendFunci, (GLuint buf, GLenum src, GLenum dst)) \
X(void, glCopyImageSubData, (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth)) \
X(void, glDebugMessageCallback, (GLDEBUGPROC callback, const void * userParam)) \
X(void, glDebugMessageControl, (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)) \
X(void, glDebugMessageInsert, (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf)) \
X(void, glDrawElementsBaseVertex, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLint basevertex)) \
X(void, glDrawElementsInstancedBaseVertex, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex)) \
X(void, glDrawRangeElementsBaseVertex, (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices, GLint basevertex)) \
X(void, glFramebufferTexture, (GLenum target, GLenum attachment, GLuint texture, GLint level)) \
X(GLuint, glGetDebugMessageLog, (GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog)) \
X(void, glGetObjectLabel, (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length, GLchar * label)) \
X(void, glGetObjectPtrLabel, (const void * ptr, GLsizei bufSize, GLsizei * length, GLchar * label)) \
X(void, glGetSamplerParameterIiv, (GLuint sampler, GLenum pname, GLint * params)) \
X(void, glGetSamplerParameterIuiv, (GLuint sampler, GLenum pname, GLuint * params)) \
X(void, glGetnUniformuiv, (GLuint program, GLint location, GLsizei bufSize, GLuint * params)) \
X(void, glMinSampleShading, (GLfloat value)) \
X(void, glMultiDrawElementsBaseVertex, (GLenum mode, const GLsizei * count, GLenum type, const void *const * indices, GLsizei drawcount, const GLint * basevertex)) \
X(void, glObjectLabel, (GLenum identifier, GLuint name, GLsizei length, const GLchar * label)) \
X(void, glObjectPtrLabel, (const void * ptr, GLsizei length, const GLchar * label)) \
X(void, glPatchParameteri, (GLenum pname, GLint value)) \
X(void, glPopDebugGroup, ()) \
X(void, glPrimitiveBoundingBox, (GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW)) \
X(void, glProvokingVertex, (GLenum mode)) \
X(void, glPushDebugGroup, (GLenum source, GLuint id, GLsizei length, const GLchar * message)) \
X(void, glSamplerParameterIiv, (GLuint sampler, GLenum pname, const GLint * param)) \
X(void, glSamplerParameterIuiv, (GLuint sampler, GLenum pname, const GLuint * param)) \
X(void, glTexBufferRange, (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
X(void, glTexImage2DMultisample, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)) \
X(void, glTexImage3DMultisample, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)) \
X(void, glTexStorage3DMultisample, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)) \
VE() \
V(3_3) \
X(void, glBindFragDataLocationIndexed, (GLuint program, GLuint colorNumber, GLuint index, const GLchar * name)) \
X(void, glColorP3ui, (GLenum type, GLuint color)) \
X(void, glColorP3uiv, (GLenum type, const GLuint * color)) \
X(void, glColorP4ui, (GLenum type, GLuint color)) \
X(void, glColorP4uiv, (GLenum type, const GLuint * color)) \
X(GLint, glGetFragDataIndex, (GLuint program, const GLchar * name)) \
X(void, glGetQueryObjecti64v, (GLuint id, GLenum pname, GLint64 * params)) \
X(void, glGetQueryObjectui64v, (GLuint id, GLenum pname, GLuint64 * params)) \
X(void, glMultiTexCoordP1ui, (GLenum texture, GLenum type, GLuint coords)) \
X(void, glMultiTexCoordP1uiv, (GLenum texture, GLenum type, const GLuint * coords)) \
X(void, glMultiTexCoordP2ui, (GLenum texture, GLenum type, GLuint coords)) \
X(void, glMultiTexCoordP2uiv, (GLenum texture, GLenum type, const GLuint * coords)) \
X(void, glMultiTexCoordP3ui, (GLenum texture, GLenum type, GLuint coords)) \
X(void, glMultiTexCoordP3uiv, (GLenum texture, GLenum type, const GLuint * coords)) \
X(void, glMultiTexCoordP4ui, (GLenum texture, GLenum type, GLuint coords)) \
X(void, glMultiTexCoordP4uiv, (GLenum texture, GLenum type, const GLuint * coords)) \
X(void, glNormalP3ui, (GLenum type, GLuint coords)) \
X(void, glNormalP3uiv, (GLenum type, const GLuint * coords)) \
X(void, glQueryCounter, (GLuint id, GLenum target)) \
X(void, glSecondaryColorP3ui, (GLenum type, GLuint color)) \
X(void, glSecondaryColorP3uiv, (GLenum type, const GLuint * color)) \
X(void, glTexCoordP1ui, (GLenum type, GLuint coords)) \
X(void, glTexCoordP1uiv, (GLenum type, const GLuint * coords)) \
X(void, glTexCoordP2ui, (GLenum type, GLuint coords)) \
X(void, glTexCoordP2uiv, (GLenum type, const GLuint * coords)) \
X(void, glTexCoordP3ui, (GLenum type, GLuint coords)) \
X(void, glTexCoordP3uiv, (GLenum type, const GLuint * coords)) \
X(void, glTexCoordP4ui, (GLenum type, GLuint coords)) \
X(void, glTexCoordP4uiv, (GLenum type, const GLuint * coords)) \
X(void, glVertexAttribP1ui, (GLuint index, GLenum type, GLboolean normalized, GLuint value)) \
X(void, glVertexAttribP1uiv, (GLuint index, GLenum type, GLboolean normalized, const GLuint * value)) \
X(void, glVertexAttribP2ui, (GLuint index, GLenum type, GLboolean normalized, GLuint value)) \
X(void, glVertexAttribP2uiv, (GLuint index, GLenum type, GLboolean normalized, const GLuint * value)) \
X(void, glVertexAttribP3ui, (GLuint index, GLenum type, GLboolean normalized, GLuint value)) \
X(void, glVertexAttribP3uiv, (GLuint index, GLenum type, GLboolean normalized, const GLuint * value)) \
X(void, glVertexAttribP4ui, (GLuint index, GLenum type, GLboolean normalized, GLuint value)) \
X(void, glVertexAttribP4uiv, (GLuint index, GLenum type, GLboolean normalized, const GLuint * value)) \
X(void, glVertexP2ui, (GLenum type, GLuint value)) \
X(void, glVertexP2uiv, (GLenum type, const GLuint * value)) \
X(void, glVertexP3ui, (GLenum type, GLuint value)) \
X(void, glVertexP3uiv, (GLenum type, const GLuint * value)) \
X(void, glVertexP4ui, (GLenum type, GLuint value)) \
X(void, glVertexP4uiv, (GLenum type, const GLuint * value)) \
VE() \
V(4_0) \
X(void, glBeginQueryIndexed, (GLenum target, GLuint index, GLuint id)) \
X(void, glDrawTransformFeedback, (GLenum mode, GLuint id)) \
X(void, glDrawTransformFeedbackStream, (GLenum mode, GLuint id, GLuint stream)) \
X(void, glEndQueryIndexed, (GLenum target, GLuint index)) \
X(void, glGetActiveSubroutineName, (GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name)) \
X(void, glGetActiveSubroutineUniformName, (GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name)) \
X(void, glGetActiveSubroutineUniformiv, (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint * values)) \
X(void, glGetProgramStageiv, (GLuint program, GLenum shadertype, GLenum pname, GLint * values)) \
X(void, glGetQueryIndexediv, (GLenum target, GLuint index, GLenum pname, GLint * params)) \
X(GLuint, glGetSubroutineIndex, (GLuint program, GLenum shadertype, const GLchar * name)) \
X(GLint, glGetSubroutineUniformLocation, (GLuint program, GLenum shadertype, const GLchar * name)) \
X(void, glGetUniformSubroutineuiv, (GLenum shadertype, GLint location, GLuint * params)) \
X(void, glGetUniformdv, (GLuint program, GLint location, GLdouble * params)) \
X(void, glPatchParameterfv, (GLenum pname, const GLfloat * values)) \
X(void, glUniform1d, (GLint location, GLdouble x)) \
X(void, glUniform1dv, (GLint location, GLsizei count, const GLdouble * value)) \
X(void, glUniform2d, (GLint location, GLdouble x, GLdouble y)) \
X(void, glUniform2dv, (GLint location, GLsizei count, const GLdouble * value)) \
X(void, glUniform3d, (GLint location, GLdouble x, GLdouble y, GLdouble z)) \
X(void, glUniform3dv, (GLint location, GLsizei count, const GLdouble * value)) \
X(void, glUniform4d, (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, glUniform4dv, (GLint location, GLsizei count, const GLdouble * value)) \
X(void, glUniformMatrix2dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glUniformMatrix2x3dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glUniformMatrix2x4dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glUniformMatrix3dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glUniformMatrix3x2dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glUniformMatrix3x4dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glUniformMatrix4dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glUniformMatrix4x2dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glUniformMatrix4x3dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glUniformSubroutinesuiv, (GLenum shadertype, GLsizei count, const GLuint * indices)) \
VE() \
V(4_1) \
X(void, glDepthRangeArrayv, (GLuint first, GLsizei count, const GLdouble * v)) \
X(void, glDepthRangeIndexed, (GLuint index, GLdouble n, GLdouble f)) \
X(void, glGetDoublei_v, (GLenum target, GLuint index, GLdouble * data)) \
X(void, glGetFloati_v, (GLenum target, GLuint index, GLfloat * data)) \
X(void, glGetVertexAttribLdv, (GLuint index, GLenum pname, GLdouble * params)) \
X(void, glProgramUniform1d, (GLuint program, GLint location, GLdouble v0)) \
X(void, glProgramUniform1dv, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, glProgramUniform2d, (GLuint program, GLint location, GLdouble v0, GLdouble v1)) \
X(void, glProgramUniform2dv, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, glProgramUniform3d, (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2)) \
X(void, glProgramUniform3dv, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, glProgramUniform4d, (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)) \
X(void, glProgramUniform4dv, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, glProgramUniformMatrix2dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glProgramUniformMatrix2x3dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glProgramUniformMatrix2x4dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glProgramUniformMatrix3dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glProgramUniformMatrix3x2dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glProgramUniformMatrix3x4dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glProgramUniformMatrix4dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glProgramUniformMatrix4x2dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glProgramUniformMatrix4x3dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, glScissorArrayv, (GLuint first, GLsizei count, const GLint * v)) \
X(void, glScissorIndexed, (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height)) \
X(void, glScissorIndexedv, (GLuint index, const GLint * v)) \
X(void, glVertexAttribL1d, (GLuint index, GLdouble x)) \
X(void, glVertexAttribL1dv, (GLuint index, const GLdouble * v)) \
X(void, glVertexAttribL2d, (GLuint index, GLdouble x, GLdouble y)) \
X(void, glVertexAttribL2dv, (GLuint index, const GLdouble * v)) \
X(void, glVertexAttribL3d, (GLuint index, GLdouble x, GLdouble y, GLdouble z)) \
X(void, glVertexAttribL3dv, (GLuint index, const GLdouble * v)) \
X(void, glVertexAttribL4d, (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, glVertexAttribL4dv, (GLuint index, const GLdouble * v)) \
X(void, glVertexAttribLPointer, (GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, glViewportArrayv, (GLuint first, GLsizei count, const GLfloat * v)) \
X(void, glViewportIndexedf, (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h)) \
X(void, glViewportIndexedfv, (GLuint index, const GLfloat * v)) \
VE() \
V(4_2) \
X(void, glDrawArraysInstancedBaseInstance, (GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance)) \
X(void, glDrawElementsInstancedBaseInstance, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLuint baseinstance)) \
X(void, glDrawElementsInstancedBaseVertexBaseInstance, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance)) \
X(void, glDrawTransformFeedbackInstanced, (GLenum mode, GLuint id, GLsizei instancecount)) \
X(void, glDrawTransformFeedbackStreamInstanced, (GLenum mode, GLuint id, GLuint stream, GLsizei instancecount)) \
X(void, glGetActiveAtomicCounterBufferiv, (GLuint program, GLuint bufferIndex, GLenum pname, GLint * params)) \
X(void, glTexStorage1D, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)) \
VE() \
V(4_3) \
X(void, glClearBufferData, (GLenum target, GLenum internalformat, GLenum format, GLenum type, const void * data)) \
X(void, glClearBufferSubData, (GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data)) \
X(void, glGetInternalformati64v, (GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint64 * params)) \
X(GLint, glGetProgramResourceLocationIndex, (GLuint program, GLenum programInterface, const GLchar * name)) \
X(void, glInvalidateBufferData, (GLuint buffer)) \
X(void, glInvalidateBufferSubData, (GLuint buffer, GLintptr offset, GLsizeiptr length)) \
X(void, glInvalidateTexImage, (GLuint texture, GLint level)) \
X(void, glInvalidateTexSubImage, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth)) \
X(void, glMultiDrawArraysIndirect, (GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride)) \
X(void, glMultiDrawElementsIndirect, (GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride)) \
X(void, glShaderStorageBlockBinding, (GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding)) \
X(void, glTextureView, (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers)) \
X(void, glVertexAttribLFormat, (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)) \
VE() \
V(4_4) \
X(void, glBindBuffersBase, (GLenum target, GLuint first, GLsizei count, const GLuint * buffers)) \
X(void, glBindBuffersRange, (GLenum target, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizeiptr * sizes)) \
X(void, glBindImageTextures, (GLuint first, GLsizei count, const GLuint * textures)) \
X(void, glBindSamplers, (GLuint first, GLsizei count, const GLuint * samplers)) \
X(void, glBindTextures, (GLuint first, GLsizei count, const GLuint * textures)) \
X(void, glBindVertexBuffers, (GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides)) \
X(void, glBufferStorage, (GLenum target, GLsizeiptr size, const void * data, GLbitfield flags)) \
X(void, glClearTexImage, (GLuint texture, GLint level, GLenum format, GLenum type, const void * data)) \
X(void, glClearTexSubImage, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * data)) \
VE() \
V(4_5) \
X(void, glBindTextureUnit, (GLuint unit, GLuint texture)) \
X(void, glBlitNamedFramebuffer, (GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)) \
X(GLenum, glCheckNamedFramebufferStatus, (GLuint framebuffer, GLenum target)) \
X(void, glClearNamedBufferData, (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void * data)) \
X(void, glClearNamedBufferSubData, (GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data)) \
X(void, glClearNamedFramebufferfi, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)) \
X(void, glClearNamedFramebufferfv, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat * value)) \
X(void, glClearNamedFramebufferiv, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint * value)) \
X(void, glClearNamedFramebufferuiv, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint * value)) \
X(void, glClipControl, (GLenum origin, GLenum depth)) \
X(void, glCompressedTextureSubImage1D, (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data)) \
X(void, glCompressedTextureSubImage2D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data)) \
X(void, glCompressedTextureSubImage3D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data)) \
X(void, glCopyNamedBufferSubData, (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)) \
X(void, glCopyTextureSubImage1D, (GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)) \
X(void, glCopyTextureSubImage2D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, glCopyTextureSubImage3D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, glCreateBuffers, (GLsizei n, GLuint * buffers)) \
X(void, glCreateFramebuffers, (GLsizei n, GLuint * framebuffers)) \
X(void, glCreateProgramPipelines, (GLsizei n, GLuint * pipelines)) \
X(void, glCreateQueries, (GLenum target, GLsizei n, GLuint * ids)) \
X(void, glCreateRenderbuffers, (GLsizei n, GLuint * renderbuffers)) \
X(void, glCreateSamplers, (GLsizei n, GLuint * samplers)) \
X(void, glCreateTextures, (GLenum target, GLsizei n, GLuint * textures)) \
X(void, glCreateTransformFeedbacks, (GLsizei n, GLuint * ids)) \
X(void, glCreateVertexArrays, (GLsizei n, GLuint * arrays)) \
X(void, glDisableVertexArrayAttrib, (GLuint vaobj, GLuint index)) \
X(void, glEnableVertexArrayAttrib, (GLuint vaobj, GLuint index)) \
X(void, glFlushMappedNamedBufferRange, (GLuint buffer, GLintptr offset, GLsizeiptr length)) \
X(void, glGenerateTextureMipmap, (GLuint texture)) \
X(void, glGetCompressedTextureImage, (GLuint texture, GLint level, GLsizei bufSize, void * pixels)) \
X(void, glGetCompressedTextureSubImage, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void * pixels)) \
X(void, glGetNamedBufferParameteri64v, (GLuint buffer, GLenum pname, GLint64 * params)) \
X(void, glGetNamedBufferParameteriv, (GLuint buffer, GLenum pname, GLint * params)) \
X(void, glGetNamedBufferPointerv, (GLuint buffer, GLenum pname, void ** params)) \
X(void, glGetNamedBufferSubData, (GLuint buffer, GLintptr offset, GLsizeiptr size, void * data)) \
X(void, glGetNamedFramebufferAttachmentParameteriv, (GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params)) \
X(void, glGetNamedFramebufferParameteriv, (GLuint framebuffer, GLenum pname, GLint * param)) \
X(void, glGetNamedRenderbufferParameteriv, (GLuint renderbuffer, GLenum pname, GLint * params)) \
X(void, glGetQueryBufferObjecti64v, (GLuint id, GLuint buffer, GLenum pname, GLintptr offset)) \
X(void, glGetQueryBufferObjectiv, (GLuint id, GLuint buffer, GLenum pname, GLintptr offset)) \
X(void, glGetQueryBufferObjectui64v, (GLuint id, GLuint buffer, GLenum pname, GLintptr offset)) \
X(void, glGetQueryBufferObjectuiv, (GLuint id, GLuint buffer, GLenum pname, GLintptr offset)) \
X(void, glGetTextureImage, (GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void * pixels)) \
X(void, glGetTextureLevelParameterfv, (GLuint texture, GLint level, GLenum pname, GLfloat * params)) \
X(void, glGetTextureLevelParameteriv, (GLuint texture, GLint level, GLenum pname, GLint * params)) \
X(void, glGetTextureParameterIiv, (GLuint texture, GLenum pname, GLint * params)) \
X(void, glGetTextureParameterIuiv, (GLuint texture, GLenum pname, GLuint * params)) \
X(void, glGetTextureParameterfv, (GLuint texture, GLenum pname, GLfloat * params)) \
X(void, glGetTextureParameteriv, (GLuint texture, GLenum pname, GLint * params)) \
X(void, glGetTextureSubImage, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void * pixels)) \
X(void, glGetTransformFeedbacki64_v, (GLuint xfb, GLenum pname, GLuint index, GLint64 * param)) \
X(void, glGetTransformFeedbacki_v, (GLuint xfb, GLenum pname, GLuint index, GLint * param)) \
X(void, glGetTransformFeedbackiv, (GLuint xfb, GLenum pname, GLint * param)) \
X(void, glGetVertexArrayIndexed64iv, (GLuint vaobj, GLuint index, GLenum pname, GLint64 * param)) \
X(void, glGetVertexArrayIndexediv, (GLuint vaobj, GLuint index, GLenum pname, GLint * param)) \
X(void, glGetVertexArrayiv, (GLuint vaobj, GLenum pname, GLint * param)) \
X(void, glGetnColorTable, (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void * table)) \
X(void, glGetnCompressedTexImage, (GLenum target, GLint lod, GLsizei bufSize, void * pixels)) \
X(void, glGetnConvolutionFilter, (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void * image)) \
X(void, glGetnHistogram, (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void * values)) \
X(void, glGetnMapdv, (GLenum target, GLenum query, GLsizei bufSize, GLdouble * v)) \
X(void, glGetnMapfv, (GLenum target, GLenum query, GLsizei bufSize, GLfloat * v)) \
X(void, glGetnMapiv, (GLenum target, GLenum query, GLsizei bufSize, GLint * v)) \
X(void, glGetnMinmax, (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void * values)) \
X(void, glGetnPixelMapfv, (GLenum map, GLsizei bufSize, GLfloat * values)) \
X(void, glGetnPixelMapuiv, (GLenum map, GLsizei bufSize, GLuint * values)) \
X(void, glGetnPixelMapusv, (GLenum map, GLsizei bufSize, GLushort * values)) \
X(void, glGetnPolygonStipple, (GLsizei bufSize, GLubyte * pattern)) \
X(void, glGetnSeparableFilter, (GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void * row, GLsizei columnBufSize, void * column, void * span)) \
X(void, glGetnTexImage, (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void * pixels)) \
X(void, glGetnUniformdv, (GLuint program, GLint location, GLsizei bufSize, GLdouble * params)) \
X(void, glInvalidateNamedFramebufferData, (GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments)) \
X(void, glInvalidateNamedFramebufferSubData, (GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void *, glMapNamedBuffer, (GLuint buffer, GLenum access)) \
X(void *, glMapNamedBufferRange, (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)) \
X(void, glNamedBufferData, (GLuint buffer, GLsizeiptr size, const void * data, GLenum usage)) \
X(void, glNamedBufferStorage, (GLuint buffer, GLsizeiptr size, const void * data, GLbitfield flags)) \
X(void, glNamedBufferSubData, (GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data)) \
X(void, glNamedFramebufferDrawBuffer, (GLuint framebuffer, GLenum buf)) \
X(void, glNamedFramebufferDrawBuffers, (GLuint framebuffer, GLsizei n, const GLenum * bufs)) \
X(void, glNamedFramebufferParameteri, (GLuint framebuffer, GLenum pname, GLint param)) \
X(void, glNamedFramebufferReadBuffer, (GLuint framebuffer, GLenum src)) \
X(void, glNamedFramebufferRenderbuffer, (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)) \
X(void, glNamedFramebufferTexture, (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)) \
X(void, glNamedFramebufferTextureLayer, (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer)) \
X(void, glNamedRenderbufferStorage, (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, glNamedRenderbufferStorageMultisample, (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, glTextureBarrier, ()) \
X(void, glTextureBuffer, (GLuint texture, GLenum internalformat, GLuint buffer)) \
X(void, glTextureBufferRange, (GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
X(void, glTextureParameterIiv, (GLuint texture, GLenum pname, const GLint * params)) \
X(void, glTextureParameterIuiv, (GLuint texture, GLenum pname, const GLuint * params)) \
X(void, glTextureParameterf, (GLuint texture, GLenum pname, GLfloat param)) \
X(void, glTextureParameterfv, (GLuint texture, GLenum pname, const GLfloat * param)) \
X(void, glTextureParameteri, (GLuint texture, GLenum pname, GLint param)) \
X(void, glTextureParameteriv, (GLuint texture, GLenum pname, const GLint * param)) \
X(void, glTextureStorage1D, (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width)) \
X(void, glTextureStorage2D, (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, glTextureStorage2DMultisample, (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)) \
X(void, glTextureStorage3D, (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)) \
X(void, glTextureStorage3DMultisample, (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)) \
X(void, glTextureSubImage1D, (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels)) \
X(void, glTextureSubImage2D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels)) \
X(void, glTextureSubImage3D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels)) \
X(void, glTransformFeedbackBufferBase, (GLuint xfb, GLuint index, GLuint buffer)) \
X(void, glTransformFeedbackBufferRange, (GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
X(GLboolean, glUnmapNamedBuffer, (GLuint buffer)) \
X(void, glVertexArrayAttribBinding, (GLuint vaobj, GLuint attribindex, GLuint bindingindex)) \
X(void, glVertexArrayAttribFormat, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)) \
X(void, glVertexArrayAttribIFormat, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)) \
X(void, glVertexArrayAttribLFormat, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)) \
X(void, glVertexArrayBindingDivisor, (GLuint vaobj, GLuint bindingindex, GLuint divisor)) \
X(void, glVertexArrayElementBuffer, (GLuint vaobj, GLuint buffer)) \
X(void, glVertexArrayVertexBuffer, (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)) \
X(void, glVertexArrayVertexBuffers, (GLuint vaobj, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides)) \
VE() \
V(4_6) \
X(void, glMultiDrawArraysIndirectCount, (GLenum mode, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride)) \
X(void, glMultiDrawElementsIndirectCount, (GLenum mode, GLenum type, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride)) \
X(void, glPolygonOffsetClamp, (GLfloat factor, GLfloat units, GLfloat clamp)) \
X(void, glSpecializeShader, (GLuint shader, const GLchar * pEntryPoint, GLuint numSpecializationConstants, const GLuint * pConstantIndex, const GLuint * pConstantValue)) \
VE() \

#if !defined(__APPLE__)
/* ----TYPEDEFS------------------------------------------------------------------------------------------------------ */
#define X(retval, name, params) typedef retval (*VdFwProcGL_##name)params;
#define V(v)
#define VE()
VD_FW_OPENGL_CORE_FUNCTIONS
#undef X
#undef V
#undef VE

/* ----EXTERNS------------------------------------------------------------------------------------------------------- */
#define X(retval, name, params) extern VdFwProcGL_##name name;
#define V(v)
#define VE()
VD_FW_OPENGL_CORE_FUNCTIONS
#undef X
#undef V
#undef VE
#endif // !defined(__APPLE__)

#if defined(__APPLE__)
#endif
#endif // !VD_FW_H

#if defined(__INTELLISENSE__) && !defined(MYLIB_IMPL)
#define VD_FW_IMPL
#endif

#ifdef VD_FW_IMPL

typedef unsigned char VdFw__GamepadButtonState;
typedef VdFwU64 VdFw__GamepadButtonBits;

typedef struct VdFw__GamepadState {
    VdFwGuid                 guid;
    VdFw__GamepadButtonBits  bits;
    VdFw__GamepadButtonBits  raw_bits;
    float                    axes[6];
    float                    raw_axes[16];
    VdFwGamepadFace          face;
    VdFwGamepadClass         klass;
    int                      has_rumble;
} VdFw__GamepadState;

#if defined(__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

static void vd_fw__load_opengl(VdFwGlVersion version);

#ifdef _WIN32

#if VD_FW_WIN32_LINKER_COMMENTS
#pragma comment(lib, "kernel32.lib")
#if VD_FW_WIN32_SUBSYSTEM == VD_FW_WIN32_SUBSYSTEM_CONSOLE
#pragma comment(linker, "/subsystem:console")
#else
#pragma comment(linker, "/subsystem:windows")
#endif // VD_FW_WIN32_SUBSYSTEM == VD_FW_WIN32_SUBSYSTEM_CONSOLE
#if VD_FW_NO_CRT
#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:msvcrt.lib")
#pragma comment(linker, "/NODEFAULTLIB:msvcrtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:oldnames.lib")
#pragma execution_character_set("utf-8")
#endif // VD_FW_NO_CRT
#endif // VD_FW_WIN32_LINKER_COMMENTS

#pragma pack(push, 1)
/* ----WIN32 BASE---------------------------------------------------------------------------------------------------- */
#define VD_FW_DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

typedef unsigned long       VdFwDWORD;
typedef int                 VdFwBOOL;
typedef unsigned char       VdFwBYTE;
typedef unsigned short      VdFwWORD;
typedef float               VdFwFLOAT;
typedef VdFwFLOAT*          VdFwPFLOAT;
typedef VdFwBOOL*           VdFwPBOOL;
typedef VdFwBOOL*           VdFwLPBOOL;
typedef VdFwBYTE*           VdFwPBYTE;
typedef VdFwBYTE*           VdFwLPBYTE;
typedef int*                VdFwPINT;
typedef int*                VdFwLPINT;
typedef VdFwWORD*           VdFwPWORD;
typedef VdFwWORD*           VdFwLPWORD;
typedef long*               VdFwLPLONG;
typedef VdFwDWORD*          VdFwPDWORD;
typedef VdFwDWORD*          VdFwLPDWORD;
typedef void*               VdFwLPVOID;
typedef const void*         VdFwLPCVOID;
typedef int                 VdFwINT;
typedef unsigned int        VdFwUINT;
typedef unsigned int*       VdFwPUINT;
typedef void*               VdFwHANDLE;
typedef VdFwHANDLE*         VdFwPHANDLE;
typedef char                VdFwCHAR;
typedef const VdFwCHAR*     VdFwLPCSTR, *VdFwPCSTR;
typedef long                VdFwLONG;
typedef unsigned long       VdFwULONG;
typedef VdFwULONG*          VdFwPULONG;
typedef unsigned short      VdFwUSHORT;
typedef VdFwUSHORT*         VdFwPUSHORT;
typedef unsigned char       VdFwUCHAR;
typedef VdFwUCHAR*          VdFwPUCHAR;
typedef VdFwBYTE            VdFwBOOLEAN;
typedef VdFwCHAR*           VdFwPCHAR, * VdFwLPCH, * VdFwPCH;
typedef long                VdFwHRESULT;
typedef __int64             VdFwLONGLONG;
typedef unsigned __int64    VdFwULONGLONG;
typedef VdFwULONGLONG       VdFwDWORDLONG;
typedef VdFwDWORDLONG*      VdFwPDWORDLONG;
typedef __int64             VdFwINT_PTR, * VdFwPINT_PTR;
typedef unsigned __int64    VdFwUINT_PTR, * VdFwPUINT_PTR;
typedef __int64             VdFwLONG_PTR, * VdFwPLONG_PTR;
typedef unsigned __int64    VdFwULONG_PTR, * VdFwPULONG_PTR;
typedef VdFwUINT_PTR        VdFwWPARAM;
typedef VdFwLONG_PTR        VdFwLPARAM;
typedef VdFwLONG_PTR        VdFwLRESULT;
typedef wchar_t             VdFwWCHAR;
typedef const VdFwWCHAR* VdFwLPCWSTR, * VdFwPCWSTR;
typedef VdFwWORD            VdFwATOM;
typedef VdFwULONG_PTR       VdFwDWORD_PTR, * VdFwPDWORD_PTR;
typedef short               VdFwSHORT;

VD_FW_DECLARE_HANDLE(VdFwHWND);
VD_FW_DECLARE_HANDLE(VdFwHDC);
VD_FW_DECLARE_HANDLE(VdFwHINSTANCE);
typedef VdFwHINSTANCE VdFwHMODULE;

typedef struct VdFwtagRECT
{
    VdFwLONG    left;
    VdFwLONG    top;
    VdFwLONG    right;
    VdFwLONG    bottom;
} VdFwRECT, * VdFwPRECT, * VdFwNPRECT, *VdFwLPRECT;

/* ----User32.dll---------------------------------------------------------------------------------------------------- */
#ifdef UNICODE
#define VdFwDispatchMessage         VdFwDispatchMessageW
#define VdFwPostMessage             VdFwPostMessageW
#define VdFwGetMessage              VdFwGetMessageW
#define VdFwDefWindowProc           VdFwDefWindowProcW
#define VdFwRegisterClassEx         VdFwRegisterClassExW
#define VdFwCreateWindowEx          VdFwCreateWindowExW
#define VdFwMapVirtualKey           VdFwMapVirtualKeyW
#define VdFwSetWindowText           VdFwSetWindowTextW
#define VdFwSetWindowLong           VdFwSetWindowLongW
#define VdFwGetWindowLongPtr        VdFwGetWindowLongPtrW
#define VdFwSetWindowLongPtr        VdFwSetWindowLongPtrW
#define VdFwLoadCursor              VdFwLoadCursorW
#define VdFwGetMonitorInfo          VdFwGetMonitorInfoW
#define VdFwGetRawInputDeviceInfo   VdFwGetRawInputDeviceInfoW
#define VdFwWNDCLASSEX              VdFwWNDCLASSEXW
#else
#define VdFwDispatchMessage         VdFwDispatchMessageA
#define VdFwPostMessage             VdFwPostMessageA
#define VdFwGetMessage              VdFwGetMessageA
#define VdFwDefWindowProc           VdFwDefWindowProcA
#define VdFwRegisterClassEx         VdFwRegisterClassExA
#define VdFwCreateWindowEx          VdFwCreateWindowExA
#define VdFwMapVirtualKey           VdFwMapVirtualKeyA
#define VdFwSetWindowText           VdFwSetWindowTextA
#define VdFwSetWindowLong           VdFwSetWindowLongA
#define VdFwGetWindowLongPtr        VdFwGetWindowLongPtrA
#define VdFwSetWindowLongPtr        VdFwSetWindowLongPtrA
#define VdFwLoadCursor              VdFwLoadCursorA
#define VdFwGetMonitorInfo          VdFwGetMonitorInfoA
#define VdFwGetRawInputDeviceInfo   VdFwGetRawInputDeviceInfoA
#define VdFwWNDCLASSEX              VdFwWNDCLASSEXA
#endif // !UNICODE

#define VD_FW_GET_X_LPARAM(lp)  ((int)(short)LOWORD(lp))
#define VD_FW_GET_Y_LPARAM(lp)  ((int)(short)HIWORD(lp))

#define VD_FW_WM_USER 0x0400

typedef VdFwLRESULT(*VdFwWNDPROC)(VdFwHWND, VdFwUINT, VdFwWPARAM, VdFwLPARAM);
typedef void (*VdFwTIMERPROC)(VdFwHWND, VdFwUINT, VdFwUINT_PTR, VdFwDWORD);

VD_FW_DECLARE_HANDLE(VdFwHICON);
VD_FW_DECLARE_HANDLE(VdFwHBRUSH);
VD_FW_DECLARE_HANDLE(VdFwHCURSOR);
VD_FW_DECLARE_HANDLE(VdFwHMENU);
VD_FW_DECLARE_HANDLE(VdFwHBITMAP);
VD_FW_DECLARE_HANDLE(VdFwHRGN);
VD_FW_DECLARE_HANDLE(VdFwHMONITOR);
VD_FW_DECLARE_HANDLE(VdFwDPI_AWARENESS_CONTEXT);
VD_FW_DECLARE_HANDLE(VdFwHRAWINPUT);
VD_FW_DECLARE_HANDLE(VdFwHKL);

typedef enum VdFwDPI_AWARENESS {
    VD_FW_DPI_AWARENESS_INVALID = -1,
    VD_FW_DPI_AWARENESS_UNAWARE = 0,
    VD_FW_DPI_AWARENESS_SYSTEM_AWARE = 1,
    VD_FW_DPI_AWARENESS_PER_MONITOR_AWARE = 2
} VdFwDPI_AWARENESS;

#define VD_FW_DPI_AWARENESS_CONTEXT_UNAWARE               ((VdFwDPI_AWARENESS_CONTEXT)-1)
#define VD_FW_DPI_AWARENESS_CONTEXT_SYSTEM_AWARE          ((VdFwDPI_AWARENESS_CONTEXT)-2)
#define VD_FW_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE     ((VdFwDPI_AWARENESS_CONTEXT)-3)
#define VD_FW_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2  ((VdFwDPI_AWARENESS_CONTEXT)-4)
#define VD_FW_DPI_AWARENESS_CONTEXT_UNAWARE_GDISCALED     ((VdFwDPI_AWARENESS_CONTEXT)-5)

#define VD_FW_WM_NCUAHDRAWCAPTION 0x00AE
#define VD_FW_WM_NCUAHDRAWFRAME   0x00AF

#define VD_FW_HWND_TOP        ((VdFwHWND)0)
#define VD_FW_HWND_BOTTOM     ((VdFwHWND)1)
#define VD_FW_HWND_TOPMOST    ((VdFwHWND)-1)
#define VD_FW_HWND_NOTOPMOST  ((VdFwHWND)-2)

typedef struct VdFwtagPOINT
{
    VdFwLONG  x;
    VdFwLONG  y;
} VdFwPOINT, * VdFwPPOINT, * VdFwNPPOINT, * VdFwLPPOINT;

typedef struct VdFwtagMSG {
    VdFwHWND        hwnd;
    VdFwUINT        message;
    VdFwWPARAM      wParam;
    VdFwLPARAM      lParam;
    VdFwDWORD       time;
    VdFwPOINT       pt;
#ifdef _MAC
    DWORD       lPrivate;
#endif
} VdFwMSG, * VdFwPMSG, * VdFwNPMSG, * VdFwLPMSG;

typedef struct VdFwtagWNDCLASSEXA {
    VdFwUINT        cbSize;
    VdFwUINT        style;
    VdFwWNDPROC     lpfnWndProc;
    int             cbClsExtra;
    int             cbWndExtra;
    VdFwHINSTANCE   hInstance;
    VdFwHICON       hIcon;
    VdFwHCURSOR     hCursor;
    VdFwHBRUSH      hbrBackground;
    VdFwLPCSTR      lpszMenuName;
    VdFwLPCSTR      lpszClassName;
    VdFwHICON       hIconSm;
} VdFwWNDCLASSEXA, * VdFwPWNDCLASSEXA, * VdFwNPWNDCLASSEXA, * VdFwLPWNDCLASSEXA;
typedef struct VdFwtagWNDCLASSEXW {
    VdFwUINT        cbSize;
    VdFwUINT        style;
    VdFwWNDPROC     lpfnWndProc;
    int             cbClsExtra;
    int             cbWndExtra;
    VdFwHINSTANCE   hInstance;
    VdFwHICON       hIcon;
    VdFwHCURSOR     hCursor;
    VdFwHBRUSH      hbrBackground;
    VdFwLPCWSTR     lpszMenuName;
    VdFwLPCWSTR     lpszClassName;
    VdFwHICON       hIconSm;
} VdFwWNDCLASSEXW, * VdFwPWNDCLASSEXW, * VdFwNPWNDCLASSEXW, * VdFwLPWNDCLASSEXW;

typedef struct VdFwtagWINDOWPLACEMENT {
    VdFwUINT  length;
    VdFwUINT  flags;
    VdFwUINT  showCmd;
    VdFwPOINT ptMinPosition;
    VdFwPOINT ptMaxPosition;
    VdFwRECT  rcNormalPosition;
#ifdef _MAC
    VdFwRECT  rcDevice;
#endif
} VdFwWINDOWPLACEMENT;
typedef VdFwWINDOWPLACEMENT* VdFwPWINDOWPLACEMENT, * VdFwLPWINDOWPLACEMENT;

typedef struct VdFwtagPAINTSTRUCT {
    VdFwHDC         hdc;
    VdFwBOOL        fErase;
    VdFwRECT        rcPaint;
    VdFwBOOL        fRestore;
    VdFwBOOL        fIncUpdate;
    VdFwBYTE        rgbReserved[32];
} VdFwPAINTSTRUCT, * VdFwPPAINTSTRUCT, * VdFwNPPAINTSTRUCT, * VdFwLPPAINTSTRUCT;

#pragma pack(push, 8)
typedef struct VdFw_ICONINFO {
    VdFwBOOL    fIcon;
    VdFwDWORD   xHotspot;
    VdFwDWORD   yHotspot;
    VdFwHBITMAP hbmMask;
    VdFwHBITMAP hbmColor;
} VdFwICONINFO;
typedef VdFwICONINFO* VdFwPICONINFO;
#pragma pack(pop)

typedef struct VdFwtagMONITORINFO
{
    VdFwDWORD   cbSize;
    VdFwRECT    rcMonitor;
    VdFwRECT    rcWork;
    VdFwDWORD   dwFlags;
} VdFwMONITORINFO, * VdFwLPMONITORINFO;

typedef struct VdFwtagWINDOWPOS {
    VdFwHWND    hwnd;
    VdFwHWND    hwndInsertAfter;
    int         x;
    int         y;
    int         cx;
    int         cy;
    VdFwUINT    flags;
} VdFwWINDOWPOS, * VdFwLPWINDOWPOS, * VdFwPWINDOWPOS;

typedef struct VdFwtagNCCALCSIZE_PARAMS {
    VdFwRECT       rgrc[3];
    VdFwPWINDOWPOS lppos;
} VdFwNCCALCSIZE_PARAMS, * VdFwLPNCCALCSIZE_PARAMS;

typedef struct VdFwtagWINDOWINFO
{
    VdFwDWORD cbSize;
    VdFwRECT rcWindow;
    VdFwRECT rcClient;
    VdFwDWORD dwStyle;
    VdFwDWORD dwExStyle;
    VdFwDWORD dwWindowStatus;
    VdFwUINT cxWindowBorders;
    VdFwUINT cyWindowBorders;
    VdFwATOM atomWindowType;
    VdFwWORD wCreatorVersion;
} VdFwWINDOWINFO, * VdFwPWINDOWINFO, * VdFwLPWINDOWINFO;

typedef struct VdFwtagRAWINPUTDEVICE {
    VdFwUSHORT usUsagePage; // Toplevel collection UsagePage
    VdFwUSHORT usUsage;     // Toplevel collection Usage
    VdFwDWORD dwFlags;
    VdFwHWND hwndTarget;    // Target hwnd. NULL = follows keyboard focus
} VdFwRAWINPUTDEVICE, * VdFwPRAWINPUTDEVICE, * VdFwLPRAWINPUTDEVICE;
typedef const VdFwRAWINPUTDEVICE* VdFwPCRAWINPUTDEVICE;

typedef struct VdFwtagRAWINPUTHEADER {
    VdFwDWORD dwType;
    VdFwDWORD dwSize;
    VdFwHANDLE hDevice;
    VdFwWPARAM wParam;
} VdFwRAWINPUTHEADER, * VdFwPRAWINPUTHEADER, * VdFwLPRAWINPUTHEADER;

#pragma pack(push, 8)
typedef struct VdFwtagRAWMOUSE {
    VdFwUSHORT usFlags;
    union {
        VdFwULONG ulButtons;
        struct {
            VdFwUSHORT  usButtonFlags;
            VdFwUSHORT  usButtonData;
        } fd;
    } v;
    VdFwULONG ulRawButtons;
    VdFwLONG lLastX;
    VdFwLONG lLastY;
    VdFwULONG ulExtraInformation;

} VdFwRAWMOUSE, * VdFwPRAWMOUSE, * VdFwLPRAWMOUSE;
#pragma pack(pop)

typedef struct VdFwtagRAWKEYBOARD {
    VdFwUSHORT MakeCode;
    VdFwUSHORT Flags;
    VdFwUSHORT Reserved;
    VdFwUSHORT VKey;
    VdFwUINT   Message;
    VdFwULONG ExtraInformation;
} VdFwRAWKEYBOARD, * VdFwPRAWKEYBOARD, * VdFwLPRAWKEYBOARD;

typedef struct VdFwtagRAWHID {
    VdFwDWORD dwSizeHid;
    VdFwDWORD dwCount;
    VdFwBYTE bRawData[1];
} VdFwRAWHID, * VdFwPRAWHID, * VdFwLPRAWHID;

typedef struct VdFwtagRAWINPUT {
    VdFwRAWINPUTHEADER header;
    union {
        VdFwRAWMOUSE    mouse;
        VdFwRAWKEYBOARD keyboard;
        VdFwRAWHID      hid;
    } data;
} VdFwRAWINPUT, * VdFwPRAWINPUT, * VdFwLPRAWINPUT;

typedef struct VdFwtagRID_DEVICE_INFO_MOUSE {
    VdFwDWORD dwId;
    VdFwDWORD dwNumberOfButtons;
    VdFwDWORD dwSampleRate;
    VdFwBOOL  fHasHorizontalWheel;
} VdFwRID_DEVICE_INFO_MOUSE, * VdFwPRID_DEVICE_INFO_MOUSE;

typedef struct VdFwtagRID_DEVICE_INFO_KEYBOARD {
    VdFwDWORD dwType;
    VdFwDWORD dwSubType;
    VdFwDWORD dwKeyboardMode;
    VdFwDWORD dwNumberOfFunctionKeys;
    VdFwDWORD dwNumberOfIndicators;
    VdFwDWORD dwNumberOfKeysTotal;
} VdFwRID_DEVICE_INFO_KEYBOARD, * VdFwPRID_DEVICE_INFO_KEYBOARD;

typedef struct VdFwtagRID_DEVICE_INFO_HID {
    VdFwDWORD dwVendorId;
    VdFwDWORD dwProductId;
    VdFwDWORD dwVersionNumber;
    VdFwUSHORT usUsagePage;
    VdFwUSHORT usUsage;
} VdFwRID_DEVICE_INFO_HID, * VdFwPRID_DEVICE_INFO_HID;

typedef struct VdFwtagRID_DEVICE_INFO {
    VdFwDWORD cbSize;
    VdFwDWORD dwType;
    union {
        VdFwRID_DEVICE_INFO_MOUSE mouse;
        VdFwRID_DEVICE_INFO_KEYBOARD keyboard;
        VdFwRID_DEVICE_INFO_HID hid;
    } v;
} VdFwRID_DEVICE_INFO, * VdFwPRID_DEVICE_INFO, * VdFwLPRID_DEVICE_INFO;

typedef struct VdFw_AppBarData
{
    VdFwDWORD cbSize;
    VdFwHWND hWnd;
    VdFwUINT uCallbackMessage;
    VdFwUINT uEdge;
    VdFwRECT rc;
    VdFwLPARAM lParam;
} VdFwAPPBARDATA, * VdFwPAPPBARDATA;

#define VD_FW_PROC_GetMessageA(name) VdFwBOOL name(VdFwLPMSG lpMsg, VdFwHWND hWnd, VdFwUINT wMsgFilterMin, VdFwUINT wMsgFilterMax)
typedef VD_FW_PROC_GetMessageA(VdFwProcGetMessageA);
static VdFwProcGetMessageA *VdFwGetMessageA;

#define VD_FW_PROC_GetMessageW(name) VdFwBOOL name(VdFwLPMSG lpMsg, VdFwHWND hWnd, VdFwUINT wMsgFilterMin, VdFwUINT wMsgFilterMax)
typedef VD_FW_PROC_GetMessageW(VdFwProcGetMessageW);
static VdFwProcGetMessageW *VdFwGetMessageW;

#define VD_FW_PROC_TranslateMessage(name) VdFwBOOL name(const VdFwMSG* lpMsg)
typedef VD_FW_PROC_TranslateMessage(VdFwProcTranslateMessage);
static VdFwProcTranslateMessage *VdFwTranslateMessage;

#define VD_FW_PROC_DispatchMessageA(name) VdFwLRESULT name(const VdFwMSG* lpMsg)
typedef VD_FW_PROC_DispatchMessageA(VdFwProcDispatchMessageA);
static VdFwProcDispatchMessageA *VdFwDispatchMessageA;

#define VD_FW_PROC_DispatchMessageW(name) VdFwLRESULT name(const VdFwMSG* lpMsg)
typedef VD_FW_PROC_DispatchMessageW(VdFwProcDispatchMessageW);
static VdFwProcDispatchMessageW *VdFwDispatchMessageW;

#define VD_FW_PROC_PostMessageA(name) VdFwBOOL name(VdFwHWND hWnd, VdFwUINT Msg, VdFwWPARAM wParam, VdFwLPARAM lParam)
typedef VD_FW_PROC_PostMessageA(VdFwProcPostMessageA);
static VdFwProcPostMessageA *VdFwPostMessageA;

#define VD_FW_PROC_PostMessageW(name) VdFwBOOL name(VdFwHWND hWnd, VdFwUINT Msg, VdFwWPARAM wParam, VdFwLPARAM lParam)
typedef VD_FW_PROC_PostMessageW(VdFwProcPostMessageW);
static VdFwProcPostMessageW *VdFwPostMessageW;

#define VD_FW_PROC_DefWindowProcA(name) VdFwLRESULT name(VdFwHWND hWnd, VdFwUINT Msg, VdFwWPARAM wParam, VdFwLPARAM lParam)
typedef VD_FW_PROC_DefWindowProcA(VdFwProcDefWindowProcA);
static VdFwProcDefWindowProcA *VdFwDefWindowProcA;

#define VD_FW_PROC_DefWindowProcW(name) VdFwLRESULT name(VdFwHWND hWnd, VdFwUINT Msg, VdFwWPARAM wParam, VdFwLPARAM lParam)
typedef VD_FW_PROC_DefWindowProcW(VdFwProcDefWindowProcW);
static VdFwProcDefWindowProcW *VdFwDefWindowProcW;

#define VD_FW_PROC_PostQuitMessage(name) void name(int nExitCode)
typedef VD_FW_PROC_PostQuitMessage(VdFwProcPostQuitMessage);
static VdFwProcPostQuitMessage *VdFwPostQuitMessage;

#define VD_FW_PROC_RegisterClassExA(name) VdFwATOM name(const VdFwWNDCLASSEXA* unnamedParam1)
typedef VD_FW_PROC_RegisterClassExA(VdFwProcRegisterClassExA);
static VdFwProcRegisterClassExA *VdFwRegisterClassExA;

#define VD_FW_PROC_RegisterClassExW(name) VdFwATOM name(const VdFwWNDCLASSEXW* unnamedParam1)
typedef VD_FW_PROC_RegisterClassExW(VdFwProcRegisterClassExW);
static VdFwProcRegisterClassExW *VdFwRegisterClassExW;

#define VD_FW_PROC_CreateWindowExA(name) VdFwHWND name(VdFwDWORD dwExStyle, VdFwLPCSTR lpClassName, VdFwLPCSTR lpWindowName, VdFwDWORD dwStyle, int X, int Y, int nWidth, int nHeight, VdFwHWND hWndParent, VdFwHMENU hMenu, VdFwHINSTANCE hInstance, VdFwLPVOID lpParam)
typedef VD_FW_PROC_CreateWindowExA(VdFwProcCreateWindowExA);
static VdFwProcCreateWindowExA *VdFwCreateWindowExA;

#define VD_FW_PROC_CreateWindowExW(name) VdFwHWND name(VdFwDWORD dwExStyle, VdFwLPCWSTR lpClassName, VdFwLPCWSTR lpWindowName, VdFwDWORD dwStyle, int X, int Y, int nWidth, int nHeight, VdFwHWND hWndParent, VdFwHMENU hMenu, VdFwHINSTANCE hInstance, VdFwLPVOID lpParam)
typedef VD_FW_PROC_CreateWindowExW(VdFwProcCreateWindowExW);
static VdFwProcCreateWindowExW *VdFwCreateWindowExW;

#define VD_FW_PROC_DestroyWindow(name) VdFwBOOL name(VdFwHWND hWnd)
typedef VD_FW_PROC_DestroyWindow(VdFwProcDestroyWindow);
static VdFwProcDestroyWindow *VdFwDestroyWindow;

#define VD_FW_PROC_ShowWindow(name) VdFwBOOL name(VdFwHWND hWnd, int nCmdShow)
typedef VD_FW_PROC_ShowWindow(VdFwProcShowWindow);
static VdFwProcShowWindow *VdFwShowWindow;

#define VD_FW_PROC_SetWindowPos(name) VdFwBOOL name(VdFwHWND hWnd, VdFwHWND hWndInsertAfter, int X, int Y, int cx, int cy, VdFwUINT uFlags)
typedef VD_FW_PROC_SetWindowPos(VdFwProcSetWindowPos);
static VdFwProcSetWindowPos *VdFwSetWindowPos;

#define VD_FW_PROC_GetWindowPlacement(name) VdFwBOOL name(VdFwHWND hWnd, VdFwWINDOWPLACEMENT* lpwndpl)
typedef VD_FW_PROC_GetWindowPlacement(VdFwProcGetWindowPlacement);
static VdFwProcGetWindowPlacement *VdFwGetWindowPlacement;

#define VD_FW_PROC_SetWindowPlacement(name) VdFwBOOL name(VdFwHWND hWnd, const VdFwWINDOWPLACEMENT* lpwndpl)
typedef VD_FW_PROC_SetWindowPlacement(VdFwProcSetWindowPlacement);
static VdFwProcSetWindowPlacement *VdFwSetWindowPlacement;

#define VD_FW_PROC_IsZoomed(name) VdFwBOOL name(VdFwHWND hWnd)
typedef VD_FW_PROC_IsZoomed(VdFwProcIsZoomed);
static VdFwProcIsZoomed *VdFwIsZoomed;

#define VD_FW_PROC_MapVirtualKeyA(name) VdFwUINT name(VdFwUINT uCode, VdFwUINT uMapType)
typedef VD_FW_PROC_MapVirtualKeyA(VdFwProcMapVirtualKeyA);
static VdFwProcMapVirtualKeyA *VdFwMapVirtualKeyA;

#define VD_FW_PROC_MapVirtualKeyW(name) VdFwUINT name(VdFwUINT uCode, VdFwUINT uMapType)
typedef VD_FW_PROC_MapVirtualKeyW(VdFwProcMapVirtualKeyW);
static VdFwProcMapVirtualKeyW *VdFwMapVirtualKeyW;

#define VD_FW_PROC_SetCapture(name) VdFwHWND name(VdFwHWND hWnd)
typedef VD_FW_PROC_SetCapture(VdFwProcSetCapture);
static VdFwProcSetCapture* VdFwSetCapture;

#define VD_FW_PROC_ReleaseCapture(name) VdFwBOOL name(void)
typedef VD_FW_PROC_ReleaseCapture(VdFwProcReleaseCapture);
static VdFwProcReleaseCapture *VdFwReleaseCapture;

#define VD_FW_PROC_GetSystemMetrics(name) int name(int nIndex)
typedef VD_FW_PROC_GetSystemMetrics(VdFwProcGetSystemMetrics);
static VdFwProcGetSystemMetrics *VdFwGetSystemMetrics;

#define VD_FW_PROC_UpdateWindow(name) VdFwBOOL name(VdFwHWND hWnd)
typedef VD_FW_PROC_UpdateWindow(VdFwProcUpdateWindow);
static VdFwProcUpdateWindow *VdFwUpdateWindow;

#define VD_FW_PROC_GetDC(name) VdFwHDC name(VdFwHWND hWnd)
typedef VD_FW_PROC_GetDC(VdFwProcGetDC);
static VdFwProcGetDC *VdFwGetDC;

#define VD_FW_PROC_ReleaseDC(name) int name(VdFwHWND hWnd, VdFwHDC hDC)
typedef VD_FW_PROC_ReleaseDC(VdFwProcReleaseDC);
static VdFwProcReleaseDC *VdFwReleaseDC;

#define VD_FW_PROC_BeginPaint(name) VdFwHDC name(VdFwHWND hWnd, VdFwLPPAINTSTRUCT lpPaint)
typedef VD_FW_PROC_BeginPaint(VdFwProcBeginPaint);
static VdFwProcBeginPaint *VdFwBeginPaint;

#define VD_FW_PROC_EndPaint(name) VdFwBOOL name(VdFwHWND hWnd, const VdFwPAINTSTRUCT* lpPaint)
typedef VD_FW_PROC_EndPaint(VdFwProcEndPaint);
static VdFwProcEndPaint *VdFwEndPaint;

#define VD_FW_PROC_SetWindowRgn(name) int name(VdFwHWND hWnd, VdFwHRGN hRgn, VdFwBOOL bRedraw)
typedef VD_FW_PROC_SetWindowRgn(VdFwProcSetWindowRgn);
static VdFwProcSetWindowRgn *VdFwSetWindowRgn;

#define VD_FW_PROC_SetWindowTextA(name) VdFwBOOL name(VdFwHWND hWnd, VdFwLPCSTR lpString)
typedef VD_FW_PROC_SetWindowTextA(VdFwProcSetWindowTextA);
static VdFwProcSetWindowTextA *VdFwSetWindowTextA;

#define VD_FW_PROC_SetWindowTextW(name) VdFwBOOL name(VdFwHWND hWnd, VdFwLPCWSTR lpString)
typedef VD_FW_PROC_SetWindowTextW(VdFwProcSetWindowTextW);
static VdFwProcSetWindowTextW *VdFwSetWindowTextW;

#define VD_FW_PROC_GetClientRect(name) VdFwBOOL name(VdFwHWND hWnd, VdFwLPRECT lpRect)
typedef VD_FW_PROC_GetClientRect(VdFwProcGetClientRect);
static VdFwProcGetClientRect *VdFwGetClientRect;

#define VD_FW_PROC_GetWindowRect(name) VdFwBOOL name(VdFwHWND hWnd, VdFwLPRECT lpRect)
typedef VD_FW_PROC_GetWindowRect(VdFwProcGetWindowRect);
static VdFwProcGetWindowRect* VdFwGetWindowRect;

#define VD_FW_PROC_ShowCursor(name) int name(VdFwBOOL bShow)
typedef VD_FW_PROC_ShowCursor(VdFwProcShowCursor);
static VdFwProcShowCursor *VdFwShowCursor;

#define VD_FW_PROC_SetCursorPos(name) VdFwBOOL name(int X, int Y)
typedef VD_FW_PROC_SetCursorPos(VdFwProcSetCursorPos);
static VdFwProcSetCursorPos *VdFwSetCursorPos;

#define VD_FW_PROC_ScreenToClient(name) VdFwBOOL name(VdFwHWND hWnd, VdFwLPPOINT lpPoint)
typedef VD_FW_PROC_ScreenToClient(VdFwProcScreenToClient);
static VdFwProcScreenToClient *VdFwScreenToClient;

#define VD_FW_PROC_EqualRect(name) VdFwBOOL name(const VdFwRECT* lprc1, const VdFwRECT* lprc2)
typedef VD_FW_PROC_EqualRect(VdFwProcEqualRect);
static VdFwProcEqualRect *VdFwEqualRect;

#define VD_FW_PROC_PtInRect(name) VdFwBOOL name(const VdFwRECT* lprc, VdFwPOINT pt)
typedef VD_FW_PROC_PtInRect(VdFwProcPtInRect);
static VdFwProcPtInRect *VdFwPtInRect;

#define VD_FW_PROC_SetWindowLongA(name) VdFwLONG name(VdFwHWND hWnd, int nIndex, VdFwLONG dwNewLong)
typedef VD_FW_PROC_SetWindowLongA(VdFwProcSetWindowLongA);
static VdFwProcSetWindowLongA *VdFwSetWindowLongA;

#define VD_FW_PROC_SetWindowLongW(name) VdFwLONG name(VdFwHWND hWnd, int nIndex, VdFwLONG dwNewLong)
typedef VD_FW_PROC_SetWindowLongW(VdFwProcSetWindowLongW);
static VdFwProcSetWindowLongW *VdFwSetWindowLongW;

#define VD_FW_PROC_GetWindowLongPtrA(name) VdFwLONG_PTR name(VdFwHWND hWnd, int nIndex)
typedef VD_FW_PROC_GetWindowLongPtrA(VdFwProcGetWindowLongPtrA);
static VdFwProcGetWindowLongPtrA *VdFwGetWindowLongPtrA;

#define VD_FW_PROC_GetWindowLongPtrW(name) VdFwLONG_PTR name(VdFwHWND hWnd, int nIndex)
typedef VD_FW_PROC_GetWindowLongPtrW(VdFwProcGetWindowLongPtrW);
static VdFwProcGetWindowLongPtrW *VdFwGetWindowLongPtrW;

#define VD_FW_PROC_SetWindowLongPtrA(name) VdFwLONG_PTR name(VdFwHWND hWnd, int nIndex, VdFwLONG_PTR dwNewLong)
typedef VD_FW_PROC_SetWindowLongPtrA(VdFwProcSetWindowLongPtrA);
static VdFwProcSetWindowLongPtrA *VdFwSetWindowLongPtrA;

#define VD_FW_PROC_SetWindowLongPtrW(name) VdFwLONG_PTR name(VdFwHWND hWnd, int nIndex, VdFwLONG_PTR dwNewLong)
typedef VD_FW_PROC_SetWindowLongPtrW(VdFwProcSetWindowLongPtrW);
static VdFwProcSetWindowLongPtrW *VdFwSetWindowLongPtrW;

#define VD_FW_PROC_LoadCursorA(name) VdFwHCURSOR name(VdFwHINSTANCE hInstance, VdFwLPCSTR lpCursorName)
typedef VD_FW_PROC_LoadCursorA(VdFwProcLoadCursorA);
static VdFwProcLoadCursorA *VdFwLoadCursorA;

#define VD_FW_PROC_LoadCursorW(name) VdFwHCURSOR name(VdFwHINSTANCE hInstance, VdFwLPCWSTR lpCursorName)
typedef VD_FW_PROC_LoadCursorW(VdFwProcLoadCursorW);
static VdFwProcLoadCursorW *VdFwLoadCursorW;

#define VD_FW_PROC_CreateIconIndirect(name) VdFwHICON name(VdFwPICONINFO piconinfo)
typedef VD_FW_PROC_CreateIconIndirect(VdFwProcCreateIconIndirect);
static VdFwProcCreateIconIndirect *VdFwCreateIconIndirect;

#define VD_FW_PROC_MonitorFromWindow(name) VdFwHMONITOR name(VdFwHWND hwnd, VdFwDWORD dwFlags)
typedef VD_FW_PROC_MonitorFromWindow(VdFwProcMonitorFromWindow);
static VdFwProcMonitorFromWindow *VdFwMonitorFromWindow;

#define VD_FW_PROC_GetMonitorInfoA(name) VdFwBOOL name(VdFwHMONITOR hMonitor, VdFwLPMONITORINFO lpmi)
typedef VD_FW_PROC_GetMonitorInfoA(VdFwProcGetMonitorInfoA);
static VdFwProcGetMonitorInfoA *VdFwGetMonitorInfoA;

#define VD_FW_PROC_GetMonitorInfoW(name) VdFwBOOL name(VdFwHMONITOR hMonitor, VdFwLPMONITORINFO lpmi)
typedef VD_FW_PROC_GetMonitorInfoW(VdFwProcGetMonitorInfoW);
static VdFwProcGetMonitorInfoW *VdFwGetMonitorInfoW;

#define VD_FW_PROC_GetDpiForWindow(name) VdFwUINT name(VdFwHWND hwnd)
typedef VD_FW_PROC_GetDpiForWindow(VdFwProcGetDpiForWindow);
static VdFwProcGetDpiForWindow *VdFwGetDpiForWindow;

#define VD_FW_PROC_SetProcessDpiAwarenessContext(name) VdFwBOOL name(VdFwDPI_AWARENESS_CONTEXT value)
typedef VD_FW_PROC_SetProcessDpiAwarenessContext(VdFwProcSetProcessDpiAwarenessContext);
static VdFwProcSetProcessDpiAwarenessContext *VdFwSetProcessDpiAwarenessContext;

#define VD_FW_PROC_GetWindowInfo(name) VdFwBOOL name(VdFwHWND hwnd, VdFwPWINDOWINFO pwi)
typedef VD_FW_PROC_GetWindowInfo(VdFwProcGetWindowInfo);
static VdFwProcGetWindowInfo *VdFwGetWindowInfo;

#define VD_FW_PROC_RegisterRawInputDevices(name) VdFwBOOL name(VdFwPCRAWINPUTDEVICE pRawInputDevices, VdFwUINT uiNumDevices, VdFwUINT cbSize)
typedef VD_FW_PROC_RegisterRawInputDevices(VdFwProcRegisterRawInputDevices);
static VdFwProcRegisterRawInputDevices *VdFwRegisterRawInputDevices;

#define VD_FW_PROC_GetRawInputData(name) VdFwUINT name(VdFwHRAWINPUT hRawInput, VdFwUINT uiCommand, VdFwLPVOID pData, VdFwPUINT pcbSize, VdFwUINT cbSizeHeader)
typedef VD_FW_PROC_GetRawInputData(VdFwProcGetRawInputData);
static VdFwProcGetRawInputData *VdFwGetRawInputData;

#define VD_FW_PROC_GetRawInputDeviceInfoA(name) VdFwUINT name(VdFwHANDLE hDevice, VdFwUINT uiCommand, VdFwLPVOID pData, VdFwPUINT pcbSize)
typedef VD_FW_PROC_GetRawInputDeviceInfoA(VdFwProcGetRawInputDeviceInfoA);
static VdFwProcGetRawInputDeviceInfoA *VdFwGetRawInputDeviceInfoA;

#define VD_FW_PROC_GetRawInputDeviceInfoW(name) VdFwUINT name(VdFwHANDLE hDevice, VdFwUINT uiCommand, VdFwLPVOID pData, VdFwPUINT pcbSize)
typedef VD_FW_PROC_GetRawInputDeviceInfoW(VdFwProcGetRawInputDeviceInfoW);
static VdFwProcGetRawInputDeviceInfoW *VdFwGetRawInputDeviceInfoW;

#define VD_FW_PROC_SetTimer(name) VdFwUINT_PTR name(VdFwHWND hWnd, VdFwUINT_PTR nIDEvent, VdFwUINT uElapse, VdFwTIMERPROC lpTimerFunc)
typedef VD_FW_PROC_SetTimer(VdFwProcSetTimer);
static VdFwProcSetTimer *VdFwSetTimer;

#define VD_FW_PROC_KillTimer(name) VdFwBOOL name(VdFwHWND hWnd, VdFwUINT_PTR uIDEvent)
typedef VD_FW_PROC_KillTimer(VdFwProcKillTimer);
static VdFwProcKillTimer *VdFwKillTimer;

#define VD_FW_PROC_GetKeyboardLayout(name) VdFwHKL name(VdFwDWORD idThread)
typedef VD_FW_PROC_GetKeyboardLayout(VdFwProcGetKeyboardLayout);
static VdFwProcGetKeyboardLayout *VdFwGetKeyboardLayout;

#define VD_FW_PROC_SetFocus(name) VdFwHWND name(VdFwHWND hWnd)
typedef VD_FW_PROC_SetFocus(VdFwProcSetFocus);
static VdFwProcSetFocus *VdFwSetFocus;

#define VD_FW_PROC_SetForegroundWindow(name) VdFwBOOL name(VdFwHWND hWnd)
typedef VD_FW_PROC_SetForegroundWindow(VdFwProcSetForegroundWindow);
static VdFwProcSetForegroundWindow *VdFwSetForegroundWindow;

#define VD_FW_PROC_GetKeyState(name) VdFwSHORT name(int nVirtKey)
typedef VD_FW_PROC_GetKeyState(VdFwProcGetKeyState);
static VdFwProcGetKeyState *VdFwGetKeyState;

/* ----Shell32.dll--------------------------------------------------------------------------------------------------- */
#define VD_FW_ABM_GETAUTOHIDEBAREX    0x0000000b
#define VD_FW_ABM_SETAUTOHIDEBAREX    0x0000000c
#define VD_FW_ABM_GETAUTOHIDEBAR      0x00000007

#define VD_FW_PROC_SHAppBarMessage(name) VdFwUINT_PTR name(VdFwDWORD dwMessage, VdFwPAPPBARDATA pData)
typedef VD_FW_PROC_SHAppBarMessage(VdFwProcSHAppBarMessage);
static VdFwProcSHAppBarMessage *VdFwSHAppBarMessage;

/* ----ntdll.dll----------------------------------------------------------------------------------------------------- */
typedef struct VdFw_OSVERSIONINFOW {
  VdFwULONG dwOSVersionInfoSize;
  VdFwULONG dwMajorVersion;
  VdFwULONG dwMinorVersion;
  VdFwULONG dwBuildNumber;
  VdFwULONG dwPlatformId;
  VdFwWCHAR szCSDVersion[128];
} VdFwOSVERSIONINFOW, *VdFwPOSVERSIONINFOW, *VdFwLPOSVERSIONINFOW, VdFwRTL_OSVERSIONINFOW, *VdFwPRTL_OSVERSIONINFOW;

#define VD_FW_PROC_RtlGetVersion(name) void name(VdFwPRTL_OSVERSIONINFOW info)
typedef VD_FW_PROC_RtlGetVersion(VdFwProcRtlGetVersion);
static VdFwProcRtlGetVersion *VdFwRtlGetVersion;

/* ----Winmm.dll----------------------------------------------------------------------------------------------------- */
typedef VdFwUINT VdFwMMRESULT;

#define VD_FW_PROC_timeBeginPeriod(name) VdFwMMRESULT name(VdFwUINT uPeriod)
typedef VD_FW_PROC_timeBeginPeriod(VdFwProctimeBeginPeriod);
static VdFwProctimeBeginPeriod* VdFwtimeBeginPeriod;

/* ----UxTheme.dll--------------------------------------------------------------------------------------------------- */
typedef struct VdFw_MARGINS
{
    int cxLeftWidth;
    int cxRightWidth;
    int cyTopHeight;
    int cyBottomHeight;
} VdFwMARGINS, * VdFwPMARGINS;

#define VD_FW_PROC_IsThemeActive(name) VdFwBOOL name();
typedef VD_FW_PROC_IsThemeActive(VdFwProcIsThemeActive);
static VdFwProcIsThemeActive *VdFwIsThemeActive;

/* ----Dwmapi.dll---------------------------------------------------------------------------------------------------- */
enum VdFwDWMWINDOWATTRIBUTE {
    VD_FW_DWMWA_NCRENDERING_ENABLED = 1,
    VD_FW_DWMWA_NCRENDERING_POLICY,
    VD_FW_DWMWA_TRANSITIONS_FORCEDISABLED,
    VD_FW_DWMWA_ALLOW_NCPAINT,
    VD_FW_DWMWA_CAPTION_BUTTON_BOUNDS,
    VD_FW_DWMWA_NONCLIENT_RTL_LAYOUT,
    VD_FW_DWMWA_FORCE_ICONIC_REPRESENTATION,
    VD_FW_DWMWA_FLIP3D_POLICY,
    VD_FW_DWMWA_EXTENDED_FRAME_BOUNDS,
    VD_FW_DWMWA_HAS_ICONIC_BITMAP,
    VD_FW_DWMWA_DISALLOW_PEEK,
    VD_FW_DWMWA_EXCLUDED_FROM_PEEK,
    VD_FW_DWMWA_CLOAK,
    VD_FW_DWMWA_CLOAKED,
    VD_FW_DWMWA_FREEZE_REPRESENTATION,
    VD_FW_DWMWA_PASSIVE_UPDATE_MODE,
    VD_FW_DWMWA_USE_HOSTBACKDROPBRUSH,
    VD_FW_DWMWA_USE_IMMERSIVE_DARK_MODE = 20,
    VD_FW_DWMWA_WINDOW_CORNER_PREFERENCE = 33,
    VD_FW_DWMWA_BORDER_COLOR,
    VD_FW_DWMWA_CAPTION_COLOR,
    VD_FW_DWMWA_TEXT_COLOR,
    VD_FW_DWMWA_VISIBLE_FRAME_BORDER_THICKNESS,
    VD_FW_DWMWA_SYSTEMBACKDROP_TYPE,
    VD_FW_DWMWA_REDIRECTIONBITMAP_ALPHA,
    VD_FW_DWMWA_BORDER_MARGINS,
    VD_FW_DWMWA_LAST
};

enum VdFwDWM_SYSTEMBACKDROP_TYPE {
    VD_FW_DWMSBT_AUTO,
    VD_FW_DWMSBT_NONE,
    VD_FW_DWMSBT_MAINWINDOW,
    VD_FW_DWMSBT_TRANSIENTWINDOW,
    VD_FW_DWMSBT_TABBEDWINDOW,
};

enum VdFwDWMNCRENDERINGPOLICY {
    VD_FW_DWMNCRP_USEWINDOWSTYLE,
    VD_FW_DWMNCRP_DISABLED,
    VD_FW_DWMNCRP_ENABLED,
    VD_FW_DWMNCRP_LAST
};

#define VD_FW_PROC_DwmExtendFrameIntoClientArea(name) VdFwHRESULT name(VdFwHWND hWnd, const VdFwMARGINS* pMarInset)
typedef VD_FW_PROC_DwmExtendFrameIntoClientArea(VdFwProcDwmExtendFrameIntoClientArea);
static VdFwProcDwmExtendFrameIntoClientArea *VdFwDwmExtendFrameIntoClientArea;

#define VD_FW_PROC_DwmIsCompositionEnabled(name) VdFwHRESULT name(VdFwBOOL *pfEnabled)
typedef VD_FW_PROC_DwmIsCompositionEnabled(VdFwProcDwmIsCompositionEnabled);
static VdFwProcDwmIsCompositionEnabled *VdFwDwmIsCompositionEnabled;

#define VD_FW_PROC_DwmSetWindowAttribute(name) VdFwHRESULT name(VdFwHWND hwnd, VdFwDWORD dwAttribute, VdFwLPCVOID pvAttribute, VdFwDWORD cbAttribute)
typedef VD_FW_PROC_DwmSetWindowAttribute(VdFwProcDwmSetWindowAttribute);
static VdFwProcDwmSetWindowAttribute *VdFwDwmSetWindowAttribute;

#define VD_FW_PROC_DwmFlush(name) VdFwHRESULT name()
typedef VD_FW_PROC_DwmFlush(VdFwProcDwmFlush);
static VdFwProcDwmFlush *VdFwDwmFlush;

/* ----Gdi32.dll----------------------------------------------------------------------------------------------------- */
typedef void* VdFwHGDIOBJ;

typedef struct VdFwtagPIXELFORMATDESCRIPTOR
{
    VdFwWORD  nSize;
    VdFwWORD  nVersion;
    VdFwDWORD dwFlags;
    VdFwBYTE  iPixelType;
    VdFwBYTE  cColorBits;
    VdFwBYTE  cRedBits;
    VdFwBYTE  cRedShift;
    VdFwBYTE  cGreenBits;
    VdFwBYTE  cGreenShift;
    VdFwBYTE  cBlueBits;
    VdFwBYTE  cBlueShift;
    VdFwBYTE  cAlphaBits;
    VdFwBYTE  cAlphaShift;
    VdFwBYTE  cAccumBits;
    VdFwBYTE  cAccumRedBits;
    VdFwBYTE  cAccumGreenBits;
    VdFwBYTE  cAccumBlueBits;
    VdFwBYTE  cAccumAlphaBits;
    VdFwBYTE  cDepthBits;
    VdFwBYTE  cStencilBits;
    VdFwBYTE  cAuxBuffers;
    VdFwBYTE  iLayerType;
    VdFwBYTE  bReserved;
    VdFwDWORD dwLayerMask;
    VdFwDWORD dwVisibleMask;
    VdFwDWORD dwDamageMask;
} VdFwPIXELFORMATDESCRIPTOR, * VdFwPPIXELFORMATDESCRIPTOR, * VdFwLPPIXELFORMATDESCRIPTOR;

typedef struct VdFwtagBITMAPINFOHEADER {
    VdFwDWORD      biSize;
    VdFwLONG       biWidth;
    VdFwLONG       biHeight;
    VdFwWORD       biPlanes;
    VdFwWORD       biBitCount;
    VdFwDWORD      biCompression;
    VdFwDWORD      biSizeImage;
    VdFwLONG       biXPelsPerMeter;
    VdFwLONG       biYPelsPerMeter;
    VdFwDWORD      biClrUsed;
    VdFwDWORD      biClrImportant;
} VdFwBITMAPINFOHEADER, * VdFwLPBITMAPINFOHEADER, * VdFwPBITMAPINFOHEADER;

typedef struct VdFwtagRGBQUAD {
    VdFwBYTE    rgbBlue;
    VdFwBYTE    rgbGreen;
    VdFwBYTE    rgbRed;
    VdFwBYTE    rgbReserved;
} VdFwRGBQUAD;

typedef struct VdFwtagBITMAPINFO {
    VdFwBITMAPINFOHEADER    bmiHeader;
    VdFwRGBQUAD             bmiColors[1];
} VdFwBITMAPINFO, * VdFwLPBITMAPINFO, * VdFwPBITMAPINFO;

#define VD_FW_PROC_ChoosePixelFormat(name) int name(VdFwHDC hdc, const VdFwPIXELFORMATDESCRIPTOR *ppfd)
typedef VD_FW_PROC_ChoosePixelFormat(VdFwProcChoosePixelFormat);
static VdFwProcChoosePixelFormat *VdFwChoosePixelFormat;

#define VD_FW_PROC_CreateBitmap(name) VdFwHBITMAP name(int nWidth, int nHeight, VdFwUINT nPlanes, VdFwUINT nBitCount, const void* lpBits)
typedef VD_FW_PROC_CreateBitmap(VdFwProcCreateBitmap);
static VdFwProcCreateBitmap *VdFwCreateBitmap;

#define VD_FW_PROC_CreateRectRgnIndirect(name) VdFwHRGN name(const VdFwRECT *lprect)
typedef VD_FW_PROC_CreateRectRgnIndirect(VdFwProcCreateRectRgnIndirect);
static VdFwProcCreateRectRgnIndirect *VdFwCreateRectRgnIndirect;

#define VD_FW_PROC_DescribePixelFormat(name) int name(VdFwHDC hdc, int iPixelFormat, VdFwUINT nBytes, VdFwLPPIXELFORMATDESCRIPTOR ppfd)
typedef VD_FW_PROC_DescribePixelFormat(VdFwProcDescribePixelFormat);
static VdFwProcDescribePixelFormat *VdFwDescribePixelFormat;

#define VD_FW_PROC_GetStockObject(name) VdFwHGDIOBJ name(int i)
typedef VD_FW_PROC_GetStockObject(VdFwProcGetStockObject);
static VdFwProcGetStockObject *VdFwGetStockObject;

#define VD_FW_PROC_SetPixelFormat(name) VdFwBOOL name(VdFwHDC hdc, int format, const VdFwPIXELFORMATDESCRIPTOR* ppfd)
typedef VD_FW_PROC_SetPixelFormat(VdFwProcSetPixelFormat);
static VdFwProcSetPixelFormat *VdFwSetPixelFormat;

#define VD_FW_PROC_CreateDIBSection(name) VdFwHBITMAP name(VdFwHDC hdc, const VdFwBITMAPINFO* pbmi, VdFwUINT usage, void** ppvBits, VdFwHANDLE hSection, VdFwDWORD offset)
typedef VD_FW_PROC_CreateDIBSection(VdFwProcCreateDIBSection);
static VdFwProcCreateDIBSection *VdFwCreateDIBSection;

#define VD_FW_PROC_SwapBuffers(name) VdFwBOOL name(VdFwHDC unnamedParam1)
typedef VD_FW_PROC_SwapBuffers(VdFwProcSwapBuffers);
static VdFwProcSwapBuffers *VdFwSwapBuffers;

/* ----OpenGL32.dll-------------------------------------------------------------------------------------------------- */
VD_FW_DECLARE_HANDLE(VdFwHGLRC);

typedef VdFwINT_PTR(* VdFwPROC)();

#define VD_FW_PROC_wglCreateContext(name) VdFwHGLRC name(VdFwHDC hDC)
typedef VD_FW_PROC_wglCreateContext(VdFwProcwglCreateContext);
static VdFwProcwglCreateContext *VdFwwglCreateContext;

#define VD_FW_PROC_wglMakeCurrent(name) VdFwBOOL name(VdFwHDC hDC, VdFwHGLRC hGLRC)
typedef VD_FW_PROC_wglMakeCurrent(VdFwProcwglMakeCurrent);
static VdFwProcwglMakeCurrent *VdFwwglMakeCurrent;

#define VD_FW_PROC_wglDeleteContext(name) VdFwBOOL name(VdFwHGLRC hGLRC)
typedef VD_FW_PROC_wglDeleteContext(VdFwProcwglDeleteContext);
static VdFwProcwglDeleteContext *VdFwwglDeleteContext;

#define VD_FW_PROC_wglGetProcAddress(name) VdFwPROC name(VdFwLPCSTR pName)
typedef VD_FW_PROC_wglGetProcAddress(VdFwProcwglGetProcAddress);
static VdFwProcwglGetProcAddress *VdFwwglGetProcAddress;

#define VD_FW_PROC_wglSwapIntervalExt(name) VdFwBOOL name(int interval)
typedef VD_FW_PROC_wglSwapIntervalExt(VdFwProcwglSwapIntervalExt);

#define VD_FW_PROC_wglCreateContextAttribsARB(name) VdFwHGLRC name(VdFwHDC hDC, VdFwHGLRC hShareContext, const int* attribList)
typedef VD_FW_PROC_wglCreateContextAttribsARB(VdFwProcwglCreateContextAttribsARB);

#define VD_FW_PROC_wglChoosePixelFormatARB(name) VdFwBOOL name(VdFwHDC hdc, const int* piAttribIList, const float* pfAttribFList, VdFwUINT nMaxFormats, int* piFormats, VdFwUINT* nNumFormats)
typedef VD_FW_PROC_wglChoosePixelFormatARB(VdFwProcwglChoosePixelFormatARB);

/* ----Hid.dll------------------------------------------------------------------------------------------------------- */
typedef VdFwLONG                         VdFwNTSTATUS;
typedef struct VdFw_HIDP_PREPARSED_DATA* VdFwPHIDP_PREPARSED_DATA;
typedef VdFwUSHORT                       VdFwUSAGE, * VdFwPUSAGE;

#define VD_FW_FACILITY_HID_ERROR_CODE 0x11
#define VD_FW_HIDP_ERROR_CODES(SEV, CODE) \
        ((VdFwNTSTATUS) (((SEV) << 28) | (VD_FW_FACILITY_HID_ERROR_CODE << 16) | (CODE)))

#define VD_FW_HIDP_STATUS_SUCCESS                  (VD_FW_HIDP_ERROR_CODES(0x0,0))
#define VD_FW_HIDP_STATUS_NULL                     (VD_FW_HIDP_ERROR_CODES(0x8,1))
#define VD_FW_HIDP_STATUS_INVALID_PREPARSED_DATA   (VD_FW_HIDP_ERROR_CODES(0xC,1))
#define VD_FW_HIDP_STATUS_INVALID_REPORT_TYPE      (VD_FW_HIDP_ERROR_CODES(0xC,2))
#define VD_FW_HIDP_STATUS_INVALID_REPORT_LENGTH    (VD_FW_HIDP_ERROR_CODES(0xC,3))
#define VD_FW_HIDP_STATUS_USAGE_NOT_FOUND          (VD_FW_HIDP_ERROR_CODES(0xC,4))
#define VD_FW_HIDP_STATUS_VALUE_OUT_OF_RANGE       (VD_FW_HIDP_ERROR_CODES(0xC,5))
#define VD_FW_HIDP_STATUS_BAD_LOG_PHY_VALUES       (VD_FW_HIDP_ERROR_CODES(0xC,6))
#define VD_FW_HIDP_STATUS_BUFFER_TOO_SMALL         (VD_FW_HIDP_ERROR_CODES(0xC,7))
#define VD_FW_HIDP_STATUS_INTERNAL_ERROR           (VD_FW_HIDP_ERROR_CODES(0xC,8))
#define VD_FW_HIDP_STATUS_I8042_TRANS_UNKNOWN      (VD_FW_HIDP_ERROR_CODES(0xC,9))
#define VD_FW_HIDP_STATUS_INCOMPATIBLE_REPORT_ID   (VD_FW_HIDP_ERROR_CODES(0xC,0xA))
#define VD_FW_HIDP_STATUS_NOT_VALUE_ARRAY          (VD_FW_HIDP_ERROR_CODES(0xC,0xB))
#define VD_FW_HIDP_STATUS_IS_VALUE_ARRAY           (VD_FW_HIDP_ERROR_CODES(0xC,0xC))
#define VD_FW_HIDP_STATUS_DATA_INDEX_NOT_FOUND     (VD_FW_HIDP_ERROR_CODES(0xC,0xD))
#define VD_FW_HIDP_STATUS_DATA_INDEX_OUT_OF_RANGE  (VD_FW_HIDP_ERROR_CODES(0xC,0xE))
#define VD_FW_HIDP_STATUS_BUTTON_NOT_PRESSED       (VD_FW_HIDP_ERROR_CODES(0xC,0xF))
#define VD_FW_HIDP_STATUS_REPORT_DOES_NOT_EXIST    (VD_FW_HIDP_ERROR_CODES(0xC,0x10))
#define VD_FW_HIDP_STATUS_NOT_IMPLEMENTED          (VD_FW_HIDP_ERROR_CODES(0xC,0x20))
#define VD_FW_HIDP_STATUS_NOT_BUTTON_ARRAY         (VD_FW_HIDP_ERROR_CODES(0xC,0x21))
#define VD_FW_RIDI_PREPARSEDDATA                    0x20000005
#define VD_FW_RIDI_DEVICENAME                       0x20000007
#define VD_FW_RIDI_DEVICEINFO                       0x2000000b

typedef enum VdFw_HIDP_REPORT_TYPE
{
    VdFwHidP_Input,
    VdFwHidP_Output,
    VdFwHidP_Feature
} VdFwHIDP_REPORT_TYPE;

typedef struct VdFw_HIDP_CAPS
{
    VdFwUSAGE    Usage;
    VdFwUSAGE    UsagePage;
    VdFwUSHORT   InputReportByteLength;
    VdFwUSHORT   OutputReportByteLength;
    VdFwUSHORT   FeatureReportByteLength;
    VdFwUSHORT   Reserved[17];
    VdFwUSHORT   NumberLinkCollectionNodes;
    VdFwUSHORT   NumberInputButtonCaps;
    VdFwUSHORT   NumberInputValueCaps;
    VdFwUSHORT   NumberInputDataIndices;
    VdFwUSHORT   NumberOutputButtonCaps;
    VdFwUSHORT   NumberOutputValueCaps;
    VdFwUSHORT   NumberOutputDataIndices;
    VdFwUSHORT   NumberFeatureButtonCaps;
    VdFwUSHORT   NumberFeatureValueCaps;
    VdFwUSHORT   NumberFeatureDataIndices;
} VdFwHIDP_CAPS, * VdFwPHIDP_CAPS;

typedef struct VdFw_HIDP_BUTTON_CAPS
{
    VdFwUSAGE    UsagePage;
    VdFwUCHAR    ReportID;
    VdFwBOOLEAN  IsAlias;
    VdFwUSHORT   BitField;
    VdFwUSHORT   LinkCollection;
    VdFwUSAGE    LinkUsage;
    VdFwUSAGE    LinkUsagePage;
    VdFwBOOLEAN  IsRange;
    VdFwBOOLEAN  IsStringRange;
    VdFwBOOLEAN  IsDesignatorRange;
    VdFwBOOLEAN  IsAbsolute;
    VdFwUSHORT   ReportCount;
    VdFwUSHORT   Reserved2;
    VdFwULONG    Reserved[9];
    union {
        struct {
            VdFwUSAGE    UsageMin, UsageMax;
            VdFwUSHORT   StringMin, StringMax;
            VdFwUSHORT   DesignatorMin, DesignatorMax;
            VdFwUSHORT   DataIndexMin, DataIndexMax;
        } Range;
        struct {
            VdFwUSAGE    Usage, Reserved1;
            VdFwUSHORT   StringIndex, Reserved2;
            VdFwUSHORT   DesignatorIndex, Reserved3;
            VdFwUSHORT   DataIndex, Reserved4;
        } NotRange;
    } v;
} VdFwHIDP_BUTTON_CAPS, * VdFwPHIDP_BUTTON_CAPS;

typedef struct VdFw_HIDP_VALUE_CAPS
{
    VdFwUSAGE    UsagePage;
    VdFwUCHAR    ReportID;
    VdFwBOOLEAN  IsAlias;
    VdFwUSHORT   BitField;
    VdFwUSHORT   LinkCollection;
    VdFwUSAGE    LinkUsage;
    VdFwUSAGE    LinkUsagePage;
    VdFwBOOLEAN  IsRange;
    VdFwBOOLEAN  IsStringRange;
    VdFwBOOLEAN  IsDesignatorRange;
    VdFwBOOLEAN  IsAbsolute;
    VdFwBOOLEAN  HasNull;
    VdFwUCHAR    Reserved;
    VdFwUSHORT   BitSize;
    VdFwUSHORT   ReportCount;
    VdFwUSHORT   Reserved2[5];
    VdFwULONG    UnitsExp;
    VdFwULONG    Units;
    VdFwLONG     LogicalMin, LogicalMax;
    VdFwLONG     PhysicalMin, PhysicalMax;

    union {
        struct {
            VdFwUSAGE    UsageMin, UsageMax;
            VdFwUSHORT   StringMin, StringMax;
            VdFwUSHORT   DesignatorMin, DesignatorMax;
            VdFwUSHORT   DataIndexMin, DataIndexMax;
        } Range;

        struct {
            VdFwUSAGE    Usage, Reserved1;
            VdFwUSHORT   StringIndex, Reserved2;
            VdFwUSHORT   DesignatorIndex, Reserved3;
            VdFwUSHORT   DataIndex, Reserved4;
        } NotRange;
    } v;
} VdFwHIDP_VALUE_CAPS, * VdFwPHIDP_VALUE_CAPS;

typedef struct VdFw_HIDP_DATA {
  VdFwUSHORT DataIndex;
  VdFwUSHORT Reserved;
  union {
    VdFwULONG   RawValue;
    VdFwBOOLEAN On;
  } dat;
} VdFwHIDP_DATA, *VdFwPHIDP_DATA;

#define VD_FW_PROC_HidP_GetCaps(name) VdFwNTSTATUS name(VdFwPHIDP_PREPARSED_DATA PreparsedData, VdFwPHIDP_CAPS Capabilities)
typedef VD_FW_PROC_HidP_GetCaps(VdFwProcHidP_GetCaps);
static VdFwProcHidP_GetCaps *VdFwHidP_GetCaps;

#define VD_FW_PROC_HidP_GetButtonCaps(name) VdFwNTSTATUS name(VdFwHIDP_REPORT_TYPE ReportType, VdFwPHIDP_BUTTON_CAPS ButtonCaps, VdFwPUSHORT ButtonCapsLength, VdFwPHIDP_PREPARSED_DATA PreparsedData)
typedef VD_FW_PROC_HidP_GetButtonCaps(VdFwProcHidP_GetButtonCaps);
static VdFwProcHidP_GetButtonCaps *VdFwHidP_GetButtonCaps;

#define VD_FW_PROC_HidP_GetValueCaps(name) VdFwNTSTATUS name(VdFwHIDP_REPORT_TYPE ReportType, VdFwPHIDP_VALUE_CAPS ValueCaps, VdFwPUSHORT ValueCapsLength, VdFwPHIDP_PREPARSED_DATA PreparsedData)
typedef VD_FW_PROC_HidP_GetValueCaps(VdFwProcHidP_GetValueCaps);
static VdFwProcHidP_GetValueCaps *VdFwHidP_GetValueCaps;

#define VD_FW_PROC_HidP_GetUsages(name) VdFwNTSTATUS name(VdFwHIDP_REPORT_TYPE ReportType, VdFwUSAGE UsagePage, VdFwUSHORT LinkCollection, VdFwPUSAGE UsageList, VdFwPULONG UsageLength, VdFwPHIDP_PREPARSED_DATA PreparsedData, VdFwPCHAR Report, VdFwULONG ReportLength)
typedef VD_FW_PROC_HidP_GetUsages(VdFwProcHidP_GetUsages);
static VdFwProcHidP_GetUsages *VdFwHidP_GetUsages;

#define VD_FW_PROC_HidP_GetData(name) VdFwNTSTATUS name(VdFwHIDP_REPORT_TYPE ReportType, VdFwPHIDP_DATA DataList, VdFwPULONG DataLength, VdFwPHIDP_PREPARSED_DATA PreparsedData, VdFwPCHAR Report, VdFwULONG ReportLength)
typedef VD_FW_PROC_HidP_GetData(VdFwProcHidP_GetData);
static VdFwProcHidP_GetData *VdFwHidP_GetData;

#define VD_FW_PROC_HidP_MaxUsageListLength(name) VdFwULONG name(VdFwHIDP_REPORT_TYPE ReportType, VdFwUSAGE UsagePage, VdFwPHIDP_PREPARSED_DATA PreparsedData)
typedef VD_FW_PROC_HidP_MaxUsageListLength(VdFwProcHidP_MaxUsageListLength);
VdFwProcHidP_MaxUsageListLength *VdFwHidP_MaxUsageListLength;

#define VD_FW_PROC_HidP_MaxDataListLength(name) VdFwULONG name(VdFwHIDP_REPORT_TYPE ReportType, VdFwPHIDP_PREPARSED_DATA PreparsedData)
typedef VD_FW_PROC_HidP_MaxDataListLength(VdFwProcHidP_MaxDataListLength);
static VdFwProcHidP_MaxDataListLength *VdFwHidP_MaxDataListLength;

#define VD_FW_PROC_HidP_GetUsageValue(name) VdFwNTSTATUS name(VdFwHIDP_REPORT_TYPE ReportType, VdFwUSAGE UsagePage, VdFwUSHORT LinkCollection, VdFwUSAGE Usage, VdFwPULONG UsageValue, VdFwPHIDP_PREPARSED_DATA PreparsedData, VdFwPCHAR Report, VdFwULONG ReportLength)
typedef VD_FW_PROC_HidP_GetUsageValue(VdFwProcHidP_GetUsageValue);
static VdFwProcHidP_GetUsageValue *VdFwHidP_GetUsageValue;

#define VD_FW_PROC_HidD_GetManufacturerString(name) VdFwBOOLEAN name(VdFwHANDLE HidDeviceObject, void *Buffer, VdFwULONG BufferLength)
typedef VD_FW_PROC_HidD_GetManufacturerString(VdFwProcHidD_GetManufacturerString);
static VdFwProcHidD_GetManufacturerString *VdFwHidD_GetManufacturerString;

#define VD_FW_PROC_HidD_GetProductString(name) VdFwBOOLEAN name(VdFwHANDLE HidDeviceObject, void *Buffer, VdFwULONG BufferLength)
typedef VD_FW_PROC_HidD_GetProductString(VdFwProcHidD_GetProductString);
static VdFwProcHidD_GetProductString *VdFwHidD_GetProductString;

#define VD_FW_PROC_HidD_SetFeature(name) VdFwBOOLEAN name(VdFwHANDLE HidDeviceObject, void *ReportBuffer, VdFwULONG ReportBufferLength)
typedef VD_FW_PROC_HidD_SetFeature(VdFwProcHidD_SetFeature);
static VdFwProcHidD_SetFeature *VdFwHidD_SetFeature;

#define VD_FW_PROC_HidD_SetOutputReport(name) VdFwBOOLEAN name(VdFwHANDLE HidDeviceObject, void *ReportBuffer, VdFwULONG ReportBufferLength)
typedef VD_FW_PROC_HidD_SetOutputReport(VdFwProcHidD_SetOutputReport);
static VdFwProcHidD_SetOutputReport *VdFwHidD_SetOutputReport;

/* ----XInput.dll---------------------------------------------------------------------------------------------------- */
#define VD_FW_XINPUT_GAMEPAD_DPAD_UP          0x0001
#define VD_FW_XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define VD_FW_XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define VD_FW_XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define VD_FW_XINPUT_GAMEPAD_START            0x0010
#define VD_FW_XINPUT_GAMEPAD_BACK             0x0020
#define VD_FW_XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define VD_FW_XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define VD_FW_XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define VD_FW_XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define VD_FW_XINPUT_GAMEPAD_A                0x1000
#define VD_FW_XINPUT_GAMEPAD_B                0x2000
#define VD_FW_XINPUT_GAMEPAD_X                0x4000
#define VD_FW_XINPUT_GAMEPAD_Y                0x8000
#define VD_FW_XINPUT_MAX_GAMEPADS             4

typedef struct VdFw_XINPUT_GAMEPAD {
    VdFwWORD  wButtons;
    VdFwBYTE  bLeftTrigger;
    VdFwBYTE  bRightTrigger;
    VdFwSHORT sThumbLX;
    VdFwSHORT sThumbLY;
    VdFwSHORT sThumbRX;
    VdFwSHORT sThumbRY;
} VdFwXINPUT_GAMEPAD, * VdFwPXINPUT_GAMEPAD;

typedef struct VdFw_XINPUT_STATE {
    VdFwDWORD          dwPacketNumber;
    VdFwXINPUT_GAMEPAD Gamepad;
} VdFwXINPUT_STATE, * VdFwPXINPUT_STATE;

typedef struct VdFw_XINPUT_VIBRATION {
    VdFwWORD wLeftMotorSpeed;
    VdFwWORD wRightMotorSpeed;
} VdFwXINPUT_VIBRATION, *VdFwPXINPUT_VIBRATION;

#define VD_FW_PROC_XInputGetState(name) VdFwDWORD name(VdFwDWORD dwUserIndex, VdFwXINPUT_STATE* pState)
typedef VD_FW_PROC_XInputGetState(VdFwProcXInputGetState);
static VdFwProcXInputGetState *VdFwXInputGetState;

#define VD_FW_PROC_XInputSetState(name) VdFwDWORD name(VdFwDWORD dwUserIndex, VdFwXINPUT_VIBRATION* pVibration)
typedef VD_FW_PROC_XInputSetState(VdFwProcXInputSetState);
static VdFwProcXInputSetState *VdFwXInputSetState;

/* ----Winnt.h------------------------------------------------------------------------------------------------------- */
#define VD_FW_LOWORD(l)           ((VdFwWORD)(((VdFwDWORD_PTR)(l)) & 0xffff))
#define VD_FW_HIWORD(l)           ((VdFwWORD)((((VdFwDWORD_PTR)(l)) >> 16) & 0xffff))
#define VD_FW_LOBYTE(w)           ((VdFwBYTE)(((VdFwDWORD_PTR)(w)) & 0xff))
#define VD_FW_HIBYTE(w)           ((VdFwBYTE)((((VdFwDWORD_PTR)(w)) >> 8) & 0xff))
#pragma pack(pop)

#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NODRAWTEXT
#define NOKERNEL
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#include <windows.h>
#include <versionhelpers.h>
#undef NOGDICAPMASKS
#undef NOMENUS
#undef NOICONS
#undef NOSYSCOMMANDS
#undef NORASTEROPS
#undef OEMRESOURCE
#undef NOATOM
#undef NOCLIPBOARD
#undef NOCOLOR
#undef NODRAWTEXT
#undef NOKERNEL
#undef NOMEMMGR
#undef NOMETAFILE
#undef NOOPENFILE
#undef NOSCROLL
#undef NOSERVICE
#undef NOSOUND
#undef NOTEXTMETRIC
#undef NOWH
#undef NOCOMM
#undef NOKANJI
#undef NOHELP
#undef NOPROFILER
#undef NODEFERWINDOWPOS
#undef NOMCX
#undef NORPC
#undef NOPROXYSTUB
#undef NOIMAGE
#undef NOTAPE
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#define min min
#define max max
#ifdef FAILED
#undef FAILED
#endif
// #ifdef RGB
// #undef RGB
// #endif
#ifdef FIXED
#undef FIXED
#endif

#ifdef VD_FW_WIN32_ADDITIONAL_WNDPROC
#   define VD_FW_WIN32_INVOKE_WNDPROC(hwnd, msg, wparam, lparam) do {                                        \
        LRESULT _r_ = VD_FW_WIN32_ADDITIONAL_WNDPROC((HWND)hwnd, (UINT)msg, (WPARAM)wparam, (LPARAM)lparam); \
        if (_r_ == 0) return _r_;                                                                            \
    } while (0)
#else
#   define VD_FW_WIN32_INVOKE_WNDPROC(hwnd, msg, wparam, lparam) do { } while(0)
#endif // VD_FW_WIN32_ADDITIONAL_WNDPROC

#define WGL_CONTEXT_DEBUG_BIT_ARB                   0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB      0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB               0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB               0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB                0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB            0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB   0x00000002
#define WGL_SAMPLE_BUFFERS_ARB                      0x2041
#define WGL_SAMPLES_ARB                             0x2042
#define WGL_DRAW_TO_WINDOW_ARB                      0x2001
#define WGL_SUPPORT_OPENGL_ARB                      0x2010
#define WGL_DOUBLE_BUFFER_ARB                       0x2011
#define WGL_ACCELERATION_ARB                        0x2003
#define WGL_FULL_ACCELERATION_ARB                   0x2027
#define WGL_TYPE_RGBA_ARB                           0x202B
#define WGL_PIXEL_TYPE_ARB                          0x2013
#define WGL_COLOR_BITS_ARB                          0x2014
#define WGL_ALPHA_BITS_ARB                          0x201B
#define WGL_DEPTH_BITS_ARB                          0x2022
#define WGL_STENCIL_BITS_ARB                        0x2023
#define WGL_CONTEXT_FLAGS_ARB                       0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB                   0x00000001

#define VD_FW_DISPLAY_PREFERENCE_DGPU 1
#define VD_FW_DISPLAY_PREFERENCE_IGPU 2

#if defined(VD_FW_PREFER_DISCRETE_GPU) && defined(VD_FW_PREFER_INTEGRATED_GPU)
#warning "You cannot define VD_FW_PREFER_DISCRETE_GPU and VD_FW_PREFER_INTEGRATED_GPU at the same time."
#endif

#ifdef VD_FW_PREFER_DISCRETE_GPU
#define VD_FW_DISPLAY_PREFERENCE VD_FW_DISPLAY_PREFERENCE_DGPU
#elif defined(VD_FW_PREFER_INTEGRATED_GPU)
#define VD_FW_DISPLAY_PREFERENCE VD_FW_DISPLAY_PREFERENCE_IGPU
#endif // VD_FW_PREFER_DISCRETE_GPU

#if VD_FW_DISPLAY_PREFERENCE == VD_FW_DISPLAY_PREFERENCE_DGPU
#if defined(__cplusplus)
extern "C" {
#endif
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#if defined(__cplusplus)
}
#endif
#elif VD_FW_DISPLAY_PREFERENCE == VD_FW_DISPLAY_PREFERENCE_IGPU
#if defined(__cplusplus)
extern "C" {
#endif
    __declspec(dllexport) DWORD NvOptimusEnablement = 0x00000000;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 0;
#if defined(__cplusplus)
}
#endif
#endif // VD_FW_DISPLAY_PREFERENCE

enum {
    VD_FW_WIN32_FLAGS_WAKE_COND_VAR = 1 << 0,
    VD_FW_WIN32_FLAGS_SIZE_CHANGED  = 1 << 1,

    VD_FW_WIN32_MESSAGE_BUFFER_SIZE = 256,
    VD_FW_WIN32_RAW_INPUT_BUFFER_COUNT = 1024,

    VD_FW_WIN32_MESSAGE_TYPE_SCROLL      = 10,
    VD_FW_WIN32_MESSAGE_TYPE_MOUSEMOVE   = 11,
    VD_FW_WIN32_MESSAGE_TYPE_MOUSEBTN    = 12,
    VD_FW_WIN32_MESSAGE_TYPE_CHANGEFOCUS = 13,
    VD_FW_WIN32_MESSAGE_TYPE_KEYSTATE    = 14,
    VD_FW_WIN32_MESSAGE_TYPE_STATECHANGE = 15,
    VD_FW_WIN32_MESSAGE_TYPE_CLOSERQ     = 16,
    VD_FW_WIN32_MESSAGE_TYPE_CHAR        = 17,

    VD_FW_WIN32_SHOW_CURSOR     = VD_FW_WM_USER + 1,
    VD_FW_WIN32_UPDATE_TITLE    = VD_FW_WM_USER + 2,
    VD_FW_WIN32_FULLSCREEN      = VD_FW_WM_USER + 3,
    VD_FW_WIN32_SIZE            = VD_FW_WM_USER + 4,
    VD_FW_WIN32_SIZEMIN         = VD_FW_WM_USER + 5,
    VD_FW_WIN32_SIZEMAX         = VD_FW_WM_USER + 6,
    VD_FW_WIN32_GAMEPADRMBREQ   = VD_FW_WM_USER + 7,
    VD_FW_WIN32_GAMEPADDBCH     = VD_FW_WM_USER + 8,
    VD_FW_WIN32_GAMEPADRAWRQ    = VD_FW_WM_USER + 9,
    VD_FW_WIN32_KILL            = VD_FW_WM_USER + 10,

    VD_FW_WIN32_WINDOW_STATE_MINIMIZED = 1 << 0,
    VD_FW_WIN32_WINDOW_STATE_MAXIMIZED = 1 << 1,
};

typedef struct {
    int w, h;
    int flags;
} VdFw__Win32Frame;

typedef struct {
    VdFwUINT msg;
    union {
        struct {
            float dx, dy;
        } scroll;

        struct {
            int   mx, my;
        } mousemove;

        struct {
            VdFwDWORD vkbutton;
            int       down;
        } mousebtn;

        struct {
            int   got_focus;
        } changefocus;

        struct {
            VdFwWORD  vkcode;
            int       down;
        } keystate;

        struct {
            int window_state_flag_that_changed;
            int window_state_flag_value;
        } statechange;

        struct {
            VdFwU32 codepoint;
        } character;
    } dat;
} VdFw__Win32Message;

enum {
    VD_FW__WIN32_GAMEPAD_FLAG_XINPUT = 1 << 0,
    VD_FW__WIN32_GAMEPAD_FLAG_SPLITZ = 1 << 1,
};

typedef struct {
    VdFwI32 data_index;
    VdFwI32 min_value;
    VdFwI32 max_value;
} VdFw__Win32Axis;

typedef struct VdFw__Win32GamepadInfo {
    VdFwHANDLE               handle;
    VdFwHANDLE               write_handle;
    VdFwGamepadRumbleState   rumble_state;
    VdFwGuid                 guid;
    int                      connected;
    int                      xinput_index;
    int                      flags;
    VdFwPHIDP_PREPARSED_DATA ppd;
    VdFwULONG                data_count;
    int                      output_report_size;

    VdFwGamepadMap           map;

    int                      button_data_indices_cap;
    int                      button_data_indices_len;
    int                      *button_data_indices;

    int                      axis_data_indices_cap;
    int                      axis_data_indices_len;
    VdFw__Win32Axis          *axis_data_indices;

    // @todo(mdodis): Replace this with VdFw__Win32Axis
    int                      hat_data_indices_cap;
    int                      hat_data_indices_len;
    VdFw__Win32Axis          *hat_data_indices;

    int                      z_data_index;
    int                      z_split;
    int                      z_split_min;
    int                      z_split_max;

    // @todo(mdodis): remove this
    int                      hidp_data_len;
    int                      hidp_data_cap;
    VdFwHIDP_DATA            *hidp_data;
} VdFw__Win32GamepadInfo;

typedef struct {
/* ----WINDOW THREAD ONLY-------------------------------------------------------------------------------------------- */
    VdFwGraphicsApi             graphics_api;           // Currently selected graphics api
    VdFwHWND                    hwnd;                   // Window handle
    int                         w, h;                   // Current window dimensions
    VdFwBOOL                    t_paint_ready;          // One time signal that window thread is paint-ready
                                                        // (to respond properly to events sent before we enter the
                                                        // message loop)

    VdFwBOOL                    draw_decorations;       // Draw window frame, or be frame-less
    VdFwRECT                    rgn;                    // Cached Window Region
    VdFwBOOL                    theme_enabled;          // Whether theming is enabled
    VdFwBOOL                    composition_enabled;    // Whether Compositor is enabled
    VdFwRAWINPUT                raw_input_buffer[VD_FW_WIN32_RAW_INPUT_BUFFER_COUNT];
    VdFwLONG                    last_window_style;      // Keeps last window style to switch back from fullscreen
    VdFwWINDOWPLACEMENT         last_window_placement;  // Keeps last window placement to switch back from fullscreen
    VdFw__Win32GamepadInfo      gamepad_infos[VD_FW_GAMEPAD_COUNT_MAX];
    int                         xinput;                 // Whether XInput is available
    int                         window_min[2], window_max[2];
    int                         def_window_min[2];
    int                         cap_gamepad_db_entries;
    int                         num_gamepad_db_entries;
    VdFwGamepadDBEntry          *gamepad_db_entries;
    VdFwUINT_PTR                rumble_timer_handle;    // Handle to the timer proc that lets us rumble gamepads
    VdFwU8                      *report_buffer;         // Dynamically sized report buffer, for writing to HIDs
    int                         report_buffer_len;
    int                         gamepad_raw_reports_on;
    VdFwWCHAR                   char_surrogate_hi;
    VdFwU32                     kb_codepage;


/* ----RENDER THREAD ONLY-------------------------------------------------------------------------------------------- */
    // Internal
    HMODULE                     opengl32;               // Handle to OpenGL32.dll, used when wglGetProcAddress fails.
    VdFwHANDLE                  win_thread;             // Handle to the window-thread
    VdFwDWORD                   win_thread_id;          // Window-thread ID
    VdFwHDC                     hdc;                    // Device Context
    VdFwHGLRC                   hglrc;
    LARGE_INTEGER               frequency;
    LARGE_INTEGER               performance_counter;
    VdFwProcwglSwapIntervalExt  *proc_swapInterval;     // Used for vd_fw_set_vsync
    unsigned long long          last_ns;                // Cached delta time
    // Mouse
    int                         mouse[2];
    int                         prev_mouse_state;
    int                         mouse_state;
    float                       mouse_delta[2];
    VdFwBOOL                    mouse_is_locked;
    int                         wheel_moved;
    float                       wheel[2];
    // Window
    int                         close_request;
    VdFwBOOL                    focus_changed;
    VdFwBOOL                    focused;
    VdFwBOOL                    is_fullscreen;
    int                         window_state;
    int                         window_state_changed;
    char                        *exedir;
    int                         exedir_len;
    int                         exedir_cap;
    // Gamepad
    VdFw__GamepadState          gamepad_curr_states[VD_FW_GAMEPAD_COUNT_MAX];
    VdFw__GamepadState          gamepad_prev_states[VD_FW_GAMEPAD_COUNT_MAX];
    int                         num_gamepads_present;
    unsigned char               curr_key_states[VD_FW_KEY_MAX];
    unsigned char               prev_key_states[VD_FW_KEY_MAX];
    // Character Input
    VdFwU16                     num_codepoints;
    VdFwU16                     first_codepoint_index;
    VdFwU32                     codepoints[VD_FW_CODEPOINT_BUFFER_COUNT];
    VdFwKey                     last_key;

/* ----RENDER THREAD - WINDOW THREAD DATA---------------------------------------------------------------------------- */
    VdFw__Win32Message          msgbuf[VD_FW_WIN32_MESSAGE_BUFFER_SIZE];
    volatile VdFwLONG           msgbuf_r;
    volatile VdFwLONG           msgbuf_w;
    int                         ncrect_count;
    int                         ncrects[VD_FW_NCRECTS_MAX][4];
    int                         nccaption[4];
    int                         nccaption_set;
    int                         receive_ncmouse_on;
    float                       winthread_mouse_delta[2];
    VdFw__GamepadState          winthread_gamepad_curr_states[VD_FW_GAMEPAD_COUNT_MAX];
    int                         winthread_num_gamepads_present;
    int                         has_initialized;

    char                        title[128];
    int                         title_len;
    VdFwRTL_OSVERSIONINFOW      os_version;
    int                         next_width, next_height;
    int                         next_pos_x, next_pos_y;

/* ----RENDER THREAD - WINDOW THREAD SYNC---------------------------------------------------------------------------- */
    VdFwHANDLE                  sem_window_ready;
    VdFwHANDLE                  sem_closed;
    volatile VdFwBOOL           t_running;
    CRITICAL_SECTION            critical_section;
    CRITICAL_SECTION            input_critical_section;
    CRITICAL_SECTION            db_section;
    CONDITION_VARIABLE          cond_var;
    VdFw__Win32Frame            next_frame;
    VdFw__Win32Frame            curr_frame;
} VdFw__Win32InternalData;

#define VD_FW_RAW_INPUT_ALIGN(x)        (((x) + sizeof(unsigned __int64) - 1) & ~(sizeof(unsigned __int64) - 1))
#define VD_FW_NEXT_RAW_INPUT_BLOCK(ptr) ((PRAWINPUT)VD_FW_RAW_INPUT_ALIGN((ULONG_PTR)((PBYTE)(ptr) + (ptr)->header.dwSize)))

VdFwKey vd_fw___vkcode_to_key(WORD vkcode)
{
    static VdFwKey translation_table[0xFF] = {
        VD_FW_KEY_UNKNOWN,       //                                     0x00    Invalid Key
        VD_FW_KEY_UNKNOWN,       // VK_LBUTTON                          0x01    Left mouse button
        VD_FW_KEY_UNKNOWN,       // VK_RBUTTON                          0x02    Right mouse button
        VD_FW_KEY_UNKNOWN,       // VK_CANCEL                           0x03    Control-break processing
        VD_FW_KEY_UNKNOWN,       // VK_MBUTTON                          0x04    Middle mouse button
        VD_FW_KEY_UNKNOWN,       // VK_XBUTTON1                         0x05    X1 mouse button
        VD_FW_KEY_UNKNOWN,       // VK_XBUTTON2                         0x06    X2 mouse button
        VD_FW_KEY_UNKNOWN,       //                                     0x07    Reserved
        VD_FW_KEY_BACKSPACE,     // VK_BACK                             0x08    Backspace key
        VD_FW_KEY_TAB,           // VK_TAB                              0x09    Tab key
        VD_FW_KEY_UNKNOWN,       //                                     0x0A    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x0B    Reserved
        VD_FW_KEY_UNKNOWN,       // VK_CLEAR                            0x0C    Clear key
        VD_FW_KEY_ENTER,         // VK_RETURN                           0x0D    Enter key
        VD_FW_KEY_UNKNOWN,       //                                     0x0E    Unassigned
        VD_FW_KEY_UNKNOWN,       //                                     0x0F    Unassigned
        VD_FW_KEY_UNKNOWN,       // VK_SHIFT                            0x10    Shift key
        VD_FW_KEY_UNKNOWN,       // VK_CONTROL                          0x11    Ctrl key
        VD_FW_KEY_UNKNOWN,       // VK_MENU                             0x12    Alt key
        VD_FW_KEY_UNKNOWN,       // VK_PAUSE                            0x13    Pause key
        VD_FW_KEY_CAPITAL,       // VK_CAPITAL                          0x14    Caps lock key
        VD_FW_KEY_UNKNOWN,       // VK_KANA                             0x15    IME Kana mode / IME Hangul mode
        VD_FW_KEY_UNKNOWN,       // VK_IME_ON                           0x16    IME On
        VD_FW_KEY_UNKNOWN,       // VK_JUNJA                            0x17    IME Junja mode
        VD_FW_KEY_UNKNOWN,       // VK_FINAL                            0x18    IME final mode
        VD_FW_KEY_UNKNOWN,       // VK_HANJA                            0x19    IME Hanja mode / IME Kanji mode
        VD_FW_KEY_UNKNOWN,       // VK_IME_OFF                          0x1A    IME Off
        VD_FW_KEY_ESCAPE,        // VK_ESCAPE                           0x1B    Esc key
        VD_FW_KEY_UNKNOWN,       // VK_CONVERT                          0x1C    IME convert
        VD_FW_KEY_UNKNOWN,       // VK_NONCONVERT                       0x1D    IME nonconvert
        VD_FW_KEY_UNKNOWN,       // VK_ACCEPT                           0x1E    IME accept
        VD_FW_KEY_UNKNOWN,       // VK_MODECHANGE                       0x1F    IME mode change request
        VD_FW_KEY_SPACE,         // VK_SPACE                            0x20    Spacebar key
        VD_FW_KEY_PGUP,          // VK_PRIOR                            0x21    Page up key
        VD_FW_KEY_PGDN,          // VK_NEXT                             0x22    Page down key
        VD_FW_KEY_END,           // VK_END                              0x23    End key
        VD_FW_KEY_HOME,          // VK_HOME                             0x24    Home key
        VD_FW_KEY_ARROW_LEFT,    // VK_LEFT                             0x25    Left arrow key
        VD_FW_KEY_ARROW_UP,      // VK_UP                               0x26    Up arrow key
        VD_FW_KEY_ARROW_RIGHT,   // VK_RIGHT                            0x27    Right arrow key
        VD_FW_KEY_ARROW_DOWN,    // VK_DOWN                             0x28    Down arrow key
        VD_FW_KEY_UNKNOWN,       // VK_SELECT                           0x29    Select key
        VD_FW_KEY_UNKNOWN,       // VK_PRINT                            0x2A    Print key
        VD_FW_KEY_UNKNOWN,       // VK_EXECUTE                          0x2B    Execute key
        VD_FW_KEY_UNKNOWN,       // VK_SNAPSHOT                         0x2C    Print screen key
        VD_FW_KEY_INS,           // VK_INSERT                           0x2D    Insert key
        VD_FW_KEY_DEL,           // VK_DELETE                           0x2E    Delete key
        VD_FW_KEY_UNKNOWN,       // VK_HELP                             0x2F    Help key
        VD_FW_KEY_0,             // `0`                                 0x30    0 key
        VD_FW_KEY_1,             // `1`                                 0x31    1 key
        VD_FW_KEY_2,             // `2`                                 0x32    2 key
        VD_FW_KEY_3,             // `3`                                 0x33    3 key
        VD_FW_KEY_4,             // `4`                                 0x34    4 key
        VD_FW_KEY_5,             // `5`                                 0x35    5 key
        VD_FW_KEY_6,             // `6`                                 0x36    6 key
        VD_FW_KEY_7,             // `7`                                 0x37    7 key
        VD_FW_KEY_8,             // `8`                                 0x38    8 key
        VD_FW_KEY_9,             // `9`                                 0x39    9 key
        VD_FW_KEY_UNKNOWN,       //                                     0x3A    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x3B    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x3C    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x3D    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x3E    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x3F    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x40    Undefined
        VD_FW_KEY_A,             // 'A'                                 0x41    A key
        VD_FW_KEY_B,             // 'B'                                 0x42    B key
        VD_FW_KEY_C,             // 'C'                                 0x43    C key
        VD_FW_KEY_D,             // 'D'                                 0x44    D key
        VD_FW_KEY_E,             // 'E'                                 0x45    E key
        VD_FW_KEY_F,             // 'F'                                 0x46    F key
        VD_FW_KEY_G,             // 'G'                                 0x47    G key
        VD_FW_KEY_H,             // 'H'                                 0x48    H key
        VD_FW_KEY_I,             // 'I'                                 0x49    I key
        VD_FW_KEY_J,             // 'J'                                 0x4A    J key
        VD_FW_KEY_K,             // 'K'                                 0x4B    K key
        VD_FW_KEY_L,             // 'L'                                 0x4C    L key
        VD_FW_KEY_M,             // 'M'                                 0x4D    M key
        VD_FW_KEY_N,             // 'N'                                 0x4E    N key
        VD_FW_KEY_O,             // 'O'                                 0x4F    O key
        VD_FW_KEY_P,             // 'P'                                 0x50    P key
        VD_FW_KEY_Q,             // 'Q'                                 0x51    Q key
        VD_FW_KEY_R,             // 'R'                                 0x52    R key
        VD_FW_KEY_S,             // 'S'                                 0x53    S key
        VD_FW_KEY_T,             // 'T'                                 0x54    T key
        VD_FW_KEY_U,             // 'U'                                 0x55    U key
        VD_FW_KEY_V,             // 'V'                                 0x56    V key
        VD_FW_KEY_W,             // 'W'                                 0x57    W key
        VD_FW_KEY_X,             // 'X'                                 0x58    X key
        VD_FW_KEY_Y,             // 'Y'                                 0x59    Y key
        VD_FW_KEY_Z,             // 'Z'                                 0x5A    Z key
        VD_FW_KEY_UNKNOWN,       // VK_LWIN                             0x5B    Left Windows logo key
        VD_FW_KEY_UNKNOWN,       // VK_RWIN                             0x5C    Right Windows logo key
        VD_FW_KEY_UNKNOWN,       // VK_APPS                             0x5D    Application key
        VD_FW_KEY_UNKNOWN,       //                                     0x5E    Reserved
        VD_FW_KEY_UNKNOWN,       // VK_SLEEP                            0x5F    Computer Sleep key
        VD_FW_KEY_NUMPAD_0,      // VK_NUMPAD0                          0x60    Numeric keypad 0 key
        VD_FW_KEY_NUMPAD_1,      // VK_NUMPAD1                          0x61    Numeric keypad 1 key
        VD_FW_KEY_NUMPAD_2,      // VK_NUMPAD2                          0x62    Numeric keypad 2 key
        VD_FW_KEY_NUMPAD_3,      // VK_NUMPAD3                          0x63    Numeric keypad 3 key
        VD_FW_KEY_NUMPAD_4,      // VK_NUMPAD4                          0x64    Numeric keypad 4 key
        VD_FW_KEY_NUMPAD_5,      // VK_NUMPAD5                          0x65    Numeric keypad 5 key
        VD_FW_KEY_NUMPAD_6,      // VK_NUMPAD6                          0x66    Numeric keypad 6 key
        VD_FW_KEY_NUMPAD_7,      // VK_NUMPAD7                          0x67    Numeric keypad 7 key
        VD_FW_KEY_NUMPAD_8,      // VK_NUMPAD8                          0x68    Numeric keypad 8 key
        VD_FW_KEY_NUMPAD_9,      // VK_NUMPAD9                          0x69    Numeric keypad 9 key
        VD_FW_KEY_UNKNOWN,       // VK_MULTIPLY                         0x6A    Multiply key
        VD_FW_KEY_UNKNOWN,       // VK_ADD                              0x6B    Add key
        VD_FW_KEY_UNKNOWN,       // VK_SEPARATOR                        0x6C    Separator key
        VD_FW_KEY_UNKNOWN,       // VK_SUBTRACT                         0x6D    Subtract key
        VD_FW_KEY_UNKNOWN,       // VK_DECIMAL                          0x6E    Decimal key
        VD_FW_KEY_UNKNOWN,       // VK_DIVIDE                           0x6F    Divide key
        VD_FW_KEY_F1,            // VK_F1                               0x70    F1 key
        VD_FW_KEY_F2,            // VK_F2                               0x71    F2 key
        VD_FW_KEY_F3,            // VK_F3                               0x72    F3 key
        VD_FW_KEY_F4,            // VK_F4                               0x73    F4 key
        VD_FW_KEY_F5,            // VK_F5                               0x74    F5 key
        VD_FW_KEY_F6,            // VK_F6                               0x75    F6 key
        VD_FW_KEY_F7,            // VK_F7                               0x76    F7 key
        VD_FW_KEY_F8,            // VK_F8                               0x77    F8 key
        VD_FW_KEY_F9,            // VK_F9                               0x78    F9 key
        VD_FW_KEY_F10,           // VK_F10                              0x79    F10 key
        VD_FW_KEY_F11,           // VK_F11                              0x7A    F11 key
        VD_FW_KEY_F12,           // VK_F12                              0x7B    F12 key
        VD_FW_KEY_F13,           // VK_F13                              0x7C    F13 key
        VD_FW_KEY_F14,           // VK_F14                              0x7D    F14 key
        VD_FW_KEY_F15,           // VK_F15                              0x7E    F15 key
        VD_FW_KEY_F16,           // VK_F16                              0x7F    F16 key
        VD_FW_KEY_F17,           // VK_F17                              0x80    F17 key
        VD_FW_KEY_F18,           // VK_F18                              0x81    F18 key
        VD_FW_KEY_F19,           // VK_F19                              0x82    F19 key
        VD_FW_KEY_F20,           // VK_F20                              0x83    F20 key
        VD_FW_KEY_F21,           // VK_F21                              0x84    F21 key
        VD_FW_KEY_F22,           // VK_F22                              0x85    F22 key
        VD_FW_KEY_F23,           // VK_F23                              0x86    F23 key
        VD_FW_KEY_F24,           // VK_F24                              0x87    F24 key
        VD_FW_KEY_UNKNOWN,       //                                     0x88    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x89    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8A    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8B    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8C    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8D    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8E    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8F    Reserved
        VD_FW_KEY_UNKNOWN,       // VK_NUMLOCK                          0x90    Num lock key
        VD_FW_KEY_UNKNOWN,       // VK_SCROLL                           0x91    Scroll lock key
        VD_FW_KEY_UNKNOWN,       //                                     0x92    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0x93    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0x94    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0x95    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0x96    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0x97    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x98    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x99    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9A    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9B    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9C    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9D    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9E    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9F    Unassigned 
        VD_FW_KEY_LSHIFT,        // VK_LSHIFT                           0xA0    Left Shift key
        VD_FW_KEY_RSHIFT,        // VK_RSHIFT                           0xA1    Right Shift key
        VD_FW_KEY_LCONTROL,      // VK_LCONTROL                         0xA2    Left Ctrl key
        VD_FW_KEY_RCONTROL,      // VK_RCONTROL                         0xA3    Right Ctrl key
        VD_FW_KEY_LALT,          // VK_LMENU                            0xA4    Left Alt key
        VD_FW_KEY_RALT,          // VK_RMENU                            0xA5    Right Alt key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_BACK                     0xA6    Browser Back key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_FORWARD                  0xA7    Browser Forward key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_REFRESH                  0xA8    Browser Refresh key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_STOP                     0xA9    Browser Stop key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_SEARCH                   0xAA    Browser Search key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_FAVORITES                0xAB    Browser Favorites key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_HOME                     0xAC    Browser Start and Home key
        VD_FW_KEY_UNKNOWN,       // VK_VOLUME_MUTE                      0xAD    Volume Mute key
        VD_FW_KEY_UNKNOWN,       // VK_VOLUME_DOWN                      0xAE    Volume Down key
        VD_FW_KEY_UNKNOWN,       // VK_VOLUME_UP                        0xAF    Volume Up key
        VD_FW_KEY_MEDIA_NEXT,    // VK_MEDIA_NEXT_TRACK                 0xB0    Next Track key
        VD_FW_KEY_MEDIA_PREV,    // VK_MEDIA_PREV_TRACK                 0xB1    Previous Track key
        VD_FW_KEY_UNKNOWN,       // VK_MEDIA_STOP                       0xB2    Stop Media key
        VD_FW_KEY_MEDIA_PLAY,    // VK_MEDIA_PLAY_PAUSE                 0xB3    Play/Pause Media key
        VD_FW_KEY_UNKNOWN,       // VK_LAUNCH_MAIL                      0xB4    Start Mail key
        VD_FW_KEY_UNKNOWN,       // VK_LAUNCH_MEDIA_SELECT              0xB5    Select Media key
        VD_FW_KEY_UNKNOWN,       // VK_LAUNCH_APP1                      0xB6    Start Application 1 key
        VD_FW_KEY_UNKNOWN,       // VK_LAUNCH_APP2                      0xB7    Start Application 2 key
        VD_FW_KEY_UNKNOWN,       //                                     0xB8    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0xB9    Reserved
        VD_FW_KEY_SEMICOLON,     // VK_OEM_1                            0xBA    It can vary by keyboard. For the US ANSI keyboard , the Semiсolon and Colon key
        VD_FW_KEY_EQUALS,        // VK_OEM_PLUS                         0xBB    For any country/region, the Equals and Plus key
        VD_FW_KEY_COMMA,         // VK_OEM_COMMA                        0xBC    For any country/region, the Comma and Less Than key
        VD_FW_KEY_MINUS,         // VK_OEM_MINUS                        0xBD    For any country/region, the Dash and Underscore key
        VD_FW_KEY_DOT,           // VK_OEM_PERIOD                       0xBE    For any country/region, the Period and Greater Than key
        VD_FW_KEY_SLASH_FORWARD, // VK_OEM_2                            0xBF    It can vary by keyboard. For the US ANSI keyboard, the Forward Slash and Question Mark key
        VD_FW_KEY_BACKTICK,      // VK_OEM_3                            0xC0    It can vary by keyboard. For the US ANSI keyboard, the Grave Accent and Tilde key
        VD_FW_KEY_UNKNOWN,       //                                     0xC1    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0xC2    Reserved
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_A                        0xC3    Gamepad A button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_B                        0xC4    Gamepad B button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_X                        0xC5    Gamepad X button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_Y                        0xC6    Gamepad Y button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_SHOULDER           0xC7    Gamepad Right Shoulder button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_SHOULDER            0xC8    Gamepad Left Shoulder button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_TRIGGER             0xC9    Gamepad Left Trigger button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_TRIGGER            0xCA    Gamepad Right Trigger button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_DPAD_UP                  0xCB    Gamepad D-pad Up button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_DPAD_DOWN                0xCC    Gamepad D-pad Down button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_DPAD_LEFT                0xCD    Gamepad D-pad Left button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_DPAD_RIGHT               0xCE    Gamepad D-pad Right button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_MENU                     0xCF    Gamepad Menu/Start button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_VIEW                     0xD0    Gamepad View/Back button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON   0xD1    Gamepad Left Thumbstick button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON  0xD2    Gamepad Right Thumbstick button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_THUMBSTICK_UP       0xD3    Gamepad Left Thumbstick up
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_THUMBSTICK_DOWN     0xD4    Gamepad Left Thumbstick down
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT    0xD5    Gamepad Left Thumbstick right
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_THUMBSTICK_LEFT     0xD6    Gamepad Left Thumbstick left
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_THUMBSTICK_UP      0xD7    Gamepad Right Thumbstick up
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN    0xD8    Gamepad Right Thumbstick down
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT   0xD9    Gamepad Right Thumbstick right
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT    0xDA    Gamepad Right Thumbstick left
        VD_FW_KEY_BRACKET_OPEN,  // VK_OEM_4                            0xDB    It can vary by keyboard. For the US ANSI keyboard, the Left Brace key
        VD_FW_KEY_SLASH_BACK,    // VK_OEM_5                            0xDC    It can vary by keyboard. For the US ANSI keyboard, the Backslash and Pipe key
        VD_FW_KEY_BRACKET_CLOSE, // VK_OEM_6                            0xDD    It can vary by keyboard. For the US ANSI keyboard, the Right Brace key
        VD_FW_KEY_QUOTE,         // VK_OEM_7                            0xDE    It can vary by keyboard. For the US ANSI keyboard, the Apostrophe and Double Quotation Mark key
        VD_FW_KEY_UNKNOWN,       // VK_OEM_8                            0xDF    It can vary by keyboard. For the Canadian CSA keyboard, the Right Ctrl key
        VD_FW_KEY_UNKNOWN,       //                                     0xE0    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0xE1    OEM specific
        VD_FW_KEY_UNKNOWN,       // VK_OEM_102                          0xE2    It can vary by keyboard. For the European ISO keyboard, the Backslash and Pipe key
        VD_FW_KEY_UNKNOWN,       //                                     0xE3    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xE4    OEM specific
        VD_FW_KEY_UNKNOWN,       // VK_PROCESSKEY                       0xE5    IME PROCESS key
        VD_FW_KEY_UNKNOWN,       //                                     0xE6    OEM specific
        VD_FW_KEY_UNKNOWN,       // VK_PACKET                           0xE7    Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
        VD_FW_KEY_UNKNOWN,       //                                     0xE8    Unassigned
        VD_FW_KEY_UNKNOWN,       //                                     0xE9    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xEA    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xEB    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xEC    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xED    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xEE    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xEF    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF0    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF1    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF2    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF3    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF4    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF5    OEM specific
        VD_FW_KEY_UNKNOWN,       // VK_ATTN                             0xF6    Attn key
        VD_FW_KEY_UNKNOWN,       // VK_CRSEL                            0xF7    CrSel key
        VD_FW_KEY_UNKNOWN,       // VK_EXSEL                            0xF8    ExSel key
        VD_FW_KEY_UNKNOWN,       // VK_EREOF                            0xF9    Erase EOF key
        VD_FW_KEY_UNKNOWN,       // VK_PLAY                             0xFA    Play key
        VD_FW_KEY_UNKNOWN,       // VK_ZOOM                             0xFB    Zoom key
        VD_FW_KEY_UNKNOWN,       // VK_NONAME                           0xFC    Reserved
        VD_FW_KEY_UNKNOWN,       // VK_PA1                              0xFD    PA1 key
        VD_FW_KEY_UNKNOWN,       // VK_OEM_CLEAR                        0xFE    Clear key
    };

    return translation_table[vkcode];
}

static VdFw__Win32InternalData Vd_Fw_Globals = {0};
static VdFwLRESULT  vd_fw__wndproc(VdFwHWND hwnd, VdFwUINT msg, VdFwWPARAM wparam, VdFwLPARAM lparam);
static void         vd_fw__composition_changed(void);
static void         vd_fw__update_region(void);
static void         vd_fw__theme_changed(void);
static VdFwLRESULT  vd_fw__nccalcsize(VdFwWPARAM wparam, VdFwLPARAM lparam);
static VdFwBOOL     vd_fw__has_autohide_taskbar(VdFwUINT edge, VdFwRECT monitor);
static void         vd_fw__window_pos_changed(VdFwWINDOWPOS *pos);
static VdFwLRESULT  vd_fw__handle_invisible(VdFwHWND hwnd, VdFwUINT msg, VdFwWPARAM wparam, VdFwLPARAM lparam);
static VdFwDWORD    vd_fw__win_thread_proc(LPVOID param);
static void         vd_fw__gl_debug_message_callback(GLenum source, GLenum type, GLuint id,
                                                     GLenum severity, GLsizei length, const GLchar *message,
                                                     const void *userParam);
static int          vd_fw__msgbuf_r(VdFw__Win32Message *message);
static int          vd_fw__msgbuf_w(VdFw__Win32Message *message);
static char*        vd_fw__utf16_to_utf8(const wchar_t *ws);
static void         vd_fw__update_kb_codepage(void);

#if VD_FW_WIN32_PROFILE
#define VD_FW_JOIN_(a,b) a##b
#define VD_FW_JOIN(a,b) VD_FW_JOIN_(a,b)
#define VD_FW_WIN32_PROFILE_BEGIN(name) LARGE_INTEGER name; QueryPerformanceCounter(&name)
#define VD_FW_WIN32_PROFILE_END(name)   do { \
        LARGE_INTEGER VD_FW_JOIN(name,end); QueryPerformanceCounter(&VD_FW_JOIN(name,end)); \
        LARGE_INTEGER delta; \
        delta.QuadPart = VD_FW_JOIN(name,end).QuadPart - (name).QuadPart; \
        unsigned long long q  =  delta.QuadPart / VD_FW_G.frequency.QuadPart; \
        unsigned long long r  =  delta.QuadPart % VD_FW_G.frequency.QuadPart; \
        unsigned long long ns =  q * 1000000000ULL; \
        ns                    += (r * 1000000000ULL) / VD_FW_G.frequency.QuadPart; \
        double ms              = (double)ns / 1000000.0; \
        printf("%20s took %30zuns %3.3fms\n", #name, ns, ms); \
    } while (0)
#else
#define VD_FW_WIN32_PROFILE_BEGIN(name)
#define VD_FW_WIN32_PROFILE_END(name)
#endif // VD_FW_WIN32_PROFILE

#if VD_FW_NO_CRT
#define VD_FW__CHECK_HRESULT(expr) expr
#define VD_FW__CHECK_INT(expr) expr
#define VD_FW__CHECK_NONZERO(expr) expr
#define VD_FW_SANITY_CHECK()
#define VD_FW__CHECK_NULL(expr) expr
#define VD_FW__CHECK_TRUE(expr) expr
#else
#include <stdio.h>
#define VD_FW__CHECK_HRESULT(expr) do {\
    if ((expr) != S_OK) { printf("Failed at: %s\n", #expr); DebugBreak(); } \
} while (0)

#define VD_FW__CHECK_INT(expr) do {\
    if ((expr) != 0) { printf("Failed at: %s\n", #expr); DebugBreak(); } \
} while (0)

#define VD_FW__CHECK_NONZERO(expr) do {\
    if ((expr) == 0) { printf("Failed at: %s\nGetLastError: %d", #expr, GetLastError()); DebugBreak(); } \
} while (0)

#define VD_FW_SANITY_CHECK() do { \
    DWORD error = GetLastError(); \
    if (error != ERROR_SUCCESS) { printf("GetLastError: %d\n", error); DebugBreak(); } \
} while (0)

#define VD_FW__CHECK_NULL(expr) do {\
    if ((expr) == 0) { printf("Failed at: %s\n GetLastError: %d", #expr, GetLastError()); DebugBreak(); } \
} while (0)

#define VD_FW__CHECK_TRUE(expr) do {\
    if ((expr) != TRUE) { printf("Failed at: %s\n GetLastError: %d", #expr, GetLastError()); DebugBreak(); } \
} while (0)
#endif // VD_FW_NO_CRT

#define VD_FW_G Vd_Fw_Globals

static void *vd_fw__gl_get_proc_address(const char *name)
{
    void *result = (void*)VdFwwglGetProcAddress(name);
    if (result == 0) {
        result = (void*)GetProcAddress(VD_FW_G.opengl32, name);
    }
    return result;
}

#if !VD_FW_NO_CRT
#include <io.h>
#include <fcntl.h>
#endif // !VD_FW_NO_CRT

static SIZE_T vd_fw__tcslen(LPCTSTR s)
{
    if (s == NULL) return 0;
    LPCTSTR p = s;
    while (*p) ++p;
    return (SIZE_T)(p - s);
}

VD_FW_API int vd_fw_init(VdFwInitInfo *info)
{
    VD_FW_G.graphics_api = VD_FW_GRAPHICS_API_INVALID;
    VD_FW_G.next_width = 640;
    VD_FW_G.next_height = 480;
    VD_FW_G.next_pos_x  = CW_USEDEFAULT;
    VD_FW_G.next_pos_y  = CW_USEDEFAULT;

    // Load Win32 Libraries
    {
        // User32.dll
        {
            HMODULE m                         = LoadLibraryA("User32.dll");
            VdFwGetMessageA                   =                   (VdFwProcGetMessageA*)GetProcAddress(m, "GetMessageA");
            VdFwGetMessageW                   =                   (VdFwProcGetMessageW*)GetProcAddress(m, "GetMessageW");
            VdFwTranslateMessage              =              (VdFwProcTranslateMessage*)GetProcAddress(m, "TranslateMessage");
            VdFwDispatchMessageA              =              (VdFwProcDispatchMessageA*)GetProcAddress(m, "DispatchMessageA");
            VdFwDispatchMessageW              =              (VdFwProcDispatchMessageW*)GetProcAddress(m, "DispatchMessageW");
            VdFwPostMessageA                  =                  (VdFwProcPostMessageA*)GetProcAddress(m, "PostMessageA");
            VdFwPostMessageW                  =                  (VdFwProcPostMessageW*)GetProcAddress(m, "PostMessageW");
            VdFwDefWindowProcA                =                (VdFwProcDefWindowProcA*)GetProcAddress(m, "DefWindowProcA");
            VdFwDefWindowProcW                =                (VdFwProcDefWindowProcW*)GetProcAddress(m, "DefWindowProcW");
            VdFwPostQuitMessage               =               (VdFwProcPostQuitMessage*)GetProcAddress(m, "PostQuitMessage");
            VdFwRegisterClassExA              =              (VdFwProcRegisterClassExA*)GetProcAddress(m, "RegisterClassExA");
            VdFwRegisterClassExW              =              (VdFwProcRegisterClassExW*)GetProcAddress(m, "RegisterClassExW");
            VdFwCreateWindowExA               =               (VdFwProcCreateWindowExA*)GetProcAddress(m, "CreateWindowExA");
            VdFwCreateWindowExW               =               (VdFwProcCreateWindowExW*)GetProcAddress(m, "CreateWindowExW");
            VdFwDestroyWindow                 =                 (VdFwProcDestroyWindow*)GetProcAddress(m, "DestroyWindow");
            VdFwShowWindow                    =                    (VdFwProcShowWindow*)GetProcAddress(m, "ShowWindow");
            VdFwSetWindowPos                  =                  (VdFwProcSetWindowPos*)GetProcAddress(m, "SetWindowPos");
            VdFwGetWindowPlacement            =            (VdFwProcGetWindowPlacement*)GetProcAddress(m, "GetWindowPlacement");
            VdFwSetWindowPlacement            =            (VdFwProcSetWindowPlacement*)GetProcAddress(m, "SetWindowPlacement");
            VdFwIsZoomed                      =                      (VdFwProcIsZoomed*)GetProcAddress(m, "IsZoomed");
            VdFwMapVirtualKeyA                =                (VdFwProcMapVirtualKeyA*)GetProcAddress(m, "MapVirtualKeyA");
            VdFwMapVirtualKeyW                =                (VdFwProcMapVirtualKeyW*)GetProcAddress(m, "MapVirtualKeyW");
            VdFwSetCapture                    =                    (VdFwProcSetCapture*)GetProcAddress(m, "SetCapture");
            VdFwReleaseCapture                =                (VdFwProcReleaseCapture*)GetProcAddress(m, "ReleaseCapture");
            VdFwGetSystemMetrics              =              (VdFwProcGetSystemMetrics*)GetProcAddress(m, "GetSystemMetrics");
            VdFwUpdateWindow                  =                  (VdFwProcUpdateWindow*)GetProcAddress(m, "UpdateWindow");
            VdFwGetDC                         =                         (VdFwProcGetDC*)GetProcAddress(m, "GetDC");
            VdFwReleaseDC                     =                     (VdFwProcReleaseDC*)GetProcAddress(m, "ReleaseDC");
            VdFwBeginPaint                    =                    (VdFwProcBeginPaint*)GetProcAddress(m, "BeginPaint");
            VdFwEndPaint                      =                      (VdFwProcEndPaint*)GetProcAddress(m, "EndPaint");
            VdFwSetWindowRgn                  =                  (VdFwProcSetWindowRgn*)GetProcAddress(m, "SetWindowRgn");
            VdFwSetWindowTextA                =                (VdFwProcSetWindowTextA*)GetProcAddress(m, "SetWindowTextA");
            VdFwSetWindowTextW                =                (VdFwProcSetWindowTextW*)GetProcAddress(m, "SetWindowTextW");
            VdFwGetClientRect                 =                 (VdFwProcGetClientRect*)GetProcAddress(m, "GetClientRect");
            VdFwGetWindowRect                 =                 (VdFwProcGetWindowRect*)GetProcAddress(m, "GetWindowRect");
            VdFwShowCursor                    =                    (VdFwProcShowCursor*)GetProcAddress(m, "ShowCursor");
            VdFwSetCursorPos                  =                  (VdFwProcSetCursorPos*)GetProcAddress(m, "SetCursorPos");
            VdFwScreenToClient                =                (VdFwProcScreenToClient*)GetProcAddress(m, "ScreenToClient");
            VdFwEqualRect                     =                     (VdFwProcEqualRect*)GetProcAddress(m, "EqualRect");
            VdFwPtInRect                      =                      (VdFwProcPtInRect*)GetProcAddress(m, "PtInRect");
            VdFwSetWindowLongA                =                (VdFwProcSetWindowLongA*)GetProcAddress(m, "SetWindowLongA");
            VdFwSetWindowLongW                =                (VdFwProcSetWindowLongW*)GetProcAddress(m, "SetWindowLongW");
            VdFwGetWindowLongPtrA             =             (VdFwProcGetWindowLongPtrA*)GetProcAddress(m, "GetWindowLongPtrA");
            VdFwGetWindowLongPtrW             =             (VdFwProcGetWindowLongPtrW*)GetProcAddress(m, "GetWindowLongPtrW");
            VdFwSetWindowLongPtrA             =             (VdFwProcSetWindowLongPtrA*)GetProcAddress(m, "SetWindowLongPtrA");
            VdFwSetWindowLongPtrW             =             (VdFwProcSetWindowLongPtrW*)GetProcAddress(m, "SetWindowLongPtrW");
            VdFwLoadCursorA                   =                   (VdFwProcLoadCursorA*)GetProcAddress(m, "LoadCursorA");
            VdFwLoadCursorW                   =                   (VdFwProcLoadCursorW*)GetProcAddress(m, "LoadCursorW");
            VdFwCreateIconIndirect            =            (VdFwProcCreateIconIndirect*)GetProcAddress(m, "CreateIconIndirect");
            VdFwMonitorFromWindow             =             (VdFwProcMonitorFromWindow*)GetProcAddress(m, "MonitorFromWindow");
            VdFwGetMonitorInfoA               =               (VdFwProcGetMonitorInfoA*)GetProcAddress(m, "GetMonitorInfoA");
            VdFwGetMonitorInfoW               =               (VdFwProcGetMonitorInfoW*)GetProcAddress(m, "GetMonitorInfoW");
            VdFwGetDpiForWindow               =               (VdFwProcGetDpiForWindow*)GetProcAddress(m, "GetDpiForWindow");
            VdFwSetProcessDpiAwarenessContext = (VdFwProcSetProcessDpiAwarenessContext*)GetProcAddress(m, "SetProcessDpiAwarenessContext");
            VdFwGetWindowInfo                 =                 (VdFwProcGetWindowInfo*)GetProcAddress(m, "GetWindowInfo");
            VdFwRegisterRawInputDevices       =       (VdFwProcRegisterRawInputDevices*)GetProcAddress(m, "RegisterRawInputDevices");
            VdFwGetRawInputData               =               (VdFwProcGetRawInputData*)GetProcAddress(m, "GetRawInputData");
            VdFwGetRawInputDeviceInfoA        =        (VdFwProcGetRawInputDeviceInfoA*)GetProcAddress(m, "GetRawInputDeviceInfoA");
            VdFwGetRawInputDeviceInfoW        =        (VdFwProcGetRawInputDeviceInfoW*)GetProcAddress(m, "GetRawInputDeviceInfoW");
            VdFwSetTimer                      =                      (VdFwProcSetTimer*)GetProcAddress(m, "SetTimer");
            VdFwKillTimer                     =                     (VdFwProcKillTimer*)GetProcAddress(m, "KillTimer");
            VdFwGetKeyboardLayout             =             (VdFwProcGetKeyboardLayout*)GetProcAddress(m, "GetKeyboardLayout");
            VdFwSetFocus                      =                      (VdFwProcSetFocus*)GetProcAddress(m, "SetFocus");
            VdFwSetForegroundWindow           =           (VdFwProcSetForegroundWindow*)GetProcAddress(m, "SetForegroundWindow");
            VdFwGetKeyState                   =                   (VdFwProcGetKeyState*)GetProcAddress(m, "GetKeyState");
        }

        // Shell32.dll
        {
            HMODULE m       = LoadLibraryA("Shell32.dll");
            VdFwSHAppBarMessage = (VdFwProcSHAppBarMessage*)GetProcAddress(m, "SHAppBarMessage");
        }

        // Winmm.dll
        {
            HMODULE m       = LoadLibraryA("Winmm.dll");
            VdFwtimeBeginPeriod = (VdFwProctimeBeginPeriod*)GetProcAddress(m, "timeBeginPeriod");
        }

        // ntdll.dll
        {
            HMODULE m = LoadLibraryA("ntdll.dll");
            VdFwRtlGetVersion = (VdFwProcRtlGetVersion*)GetProcAddress(m, "RtlGetVersion");

            if (VdFwRtlGetVersion) {
                VdFwRtlGetVersion((VdFwPRTL_OSVERSIONINFOW)&VD_FW_G.os_version);
            }

            FreeLibrary(m);
        }

        // UxTheme.dll
        {
            HMODULE m     = LoadLibraryA("UxTheme.dll");
            VdFwIsThemeActive = (VdFwProcIsThemeActive*)GetProcAddress(m, "IsThemeActive");
        }

        // Dwmapi.dll
        {
            HMODULE m                    = LoadLibraryA("Dwmapi.dll");
            VdFwDwmExtendFrameIntoClientArea = (VdFwProcDwmExtendFrameIntoClientArea*)GetProcAddress(m, "DwmExtendFrameIntoClientArea");
            VdFwDwmIsCompositionEnabled      =      (VdFwProcDwmIsCompositionEnabled*)GetProcAddress(m, "DwmIsCompositionEnabled");
            VdFwDwmSetWindowAttribute        =        (VdFwProcDwmSetWindowAttribute*)GetProcAddress(m, "DwmSetWindowAttribute");
            VdFwDwmFlush                     =                     (VdFwProcDwmFlush*)GetProcAddress(m, "DwmFlush");
        }

        // Gdi32.dll
        {
            HMODULE m             = LoadLibraryA("Gdi32.dll");
            VdFwChoosePixelFormat     =     (VdFwProcChoosePixelFormat*)GetProcAddress(m, "ChoosePixelFormat");
            VdFwCreateBitmap          =          (VdFwProcCreateBitmap*)GetProcAddress(m, "CreateBitmap");
            VdFwCreateRectRgnIndirect = (VdFwProcCreateRectRgnIndirect*)GetProcAddress(m, "CreateRectRgnIndirect");
            VdFwDescribePixelFormat   =   (VdFwProcDescribePixelFormat*)GetProcAddress(m, "DescribePixelFormat");
            VdFwGetStockObject        =        (VdFwProcGetStockObject*)GetProcAddress(m, "GetStockObject");
            VdFwSetPixelFormat        =        (VdFwProcSetPixelFormat*)GetProcAddress(m, "SetPixelFormat");
            VdFwCreateDIBSection      =      (VdFwProcCreateDIBSection*)GetProcAddress(m, "CreateDIBSection");
            VdFwSwapBuffers           =           (VdFwProcSwapBuffers*)GetProcAddress(m, "SwapBuffers");
        }

        // Hid.dll
        {
            HMODULE m          = LoadLibraryA("Hid.dll");
            VdFwHidP_GetCaps               =               (VdFwProcHidP_GetCaps*)GetProcAddress(m, "HidP_GetCaps");
            VdFwHidP_GetButtonCaps         =         (VdFwProcHidP_GetButtonCaps*)GetProcAddress(m, "HidP_GetButtonCaps");
            VdFwHidP_GetValueCaps          =          (VdFwProcHidP_GetValueCaps*)GetProcAddress(m, "HidP_GetValueCaps");
            VdFwHidP_GetUsages             =             (VdFwProcHidP_GetUsages*)GetProcAddress(m, "HidP_GetUsages");
            VdFwHidP_GetData               =               (VdFwProcHidP_GetData*)GetProcAddress(m, "HidP_GetData");
            VdFwHidP_MaxUsageListLength    =    (VdFwProcHidP_MaxUsageListLength*)GetProcAddress(m, "HidP_MaxUsageListLength");
            VdFwHidP_MaxDataListLength     =     (VdFwProcHidP_MaxDataListLength*)GetProcAddress(m, "HidP_MaxDataListLength");
            VdFwHidP_GetUsageValue         =         (VdFwProcHidP_GetUsageValue*)GetProcAddress(m, "HidP_GetUsageValue");
            VdFwHidD_GetManufacturerString = (VdFwProcHidD_GetManufacturerString*)GetProcAddress(m, "HidD_GetManufacturerString");
            VdFwHidD_GetProductString      =      (VdFwProcHidD_GetProductString*)GetProcAddress(m, "HidD_GetProductString");
            VdFwHidD_SetFeature            =            (VdFwProcHidD_SetFeature*)GetProcAddress(m, "HidD_SetFeature");
            VdFwHidD_SetOutputReport       =       (VdFwProcHidD_SetOutputReport*)GetProcAddress(m, "HidD_SetOutputReport");
        }

        // XInput.dll
        {
            const char* xinput_dll_name[] = {
                "xinput1_4.dll",   // Windows 8+
                "xinput1_3.dll",   // DirectX SDK, Windows XP...
                "xinput9_1_0.dll", // Windows Vista, 7...
            };

            HMODULE m;
            for (int i = 0; i < 3; ++i) {
                m = LoadLibraryA(xinput_dll_name[i]);

                if (m != NULL) {
                    break;
                }
            }

            VD_FW_G.xinput = m != NULL;
            if (VD_FW_G.xinput) {
                VdFwXInputGetState = (VdFwProcXInputGetState*)GetProcAddress(m, "XInputGetState");
                VdFwXInputSetState = (VdFwProcXInputSetState*)GetProcAddress(m, "XInputSetState");
            }
        }

        // OpenGL32.dll
        {
            VD_FW_G.opengl32 = LoadLibraryA("OpenGL32.dll");
            VdFwwglCreateContext  =  (VdFwProcwglCreateContext*)GetProcAddress(VD_FW_G.opengl32, "wglCreateContext");
            VdFwwglMakeCurrent    =    (VdFwProcwglMakeCurrent*)GetProcAddress(VD_FW_G.opengl32, "wglMakeCurrent");
            VdFwwglDeleteContext  =  (VdFwProcwglDeleteContext*)GetProcAddress(VD_FW_G.opengl32,"wglDeleteContext");
            VdFwwglGetProcAddress = (VdFwProcwglGetProcAddress*)GetProcAddress(VD_FW_G.opengl32, "wglGetProcAddress");
        }
    }
    // SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    VdFwSetProcessDpiAwarenessContext(VD_FW_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    VdFwtimeBeginPeriod(1);
    QueryPerformanceFrequency(&VD_FW_G.frequency);

    VD_FW_G.focused = 1;
    VD_FW_G.def_window_min[0] = VdFwGetSystemMetrics(SM_CXMINTRACK);
    VD_FW_G.def_window_min[1] = VdFwGetSystemMetrics(SM_CYMINTRACK);

    VD_FW_G.draw_decorations = 1;
    if (info != NULL) {
        VD_FW_G.draw_decorations = !info->window_options.borderless;
    }

    if (info != NULL) {
#if (VD_FW_WIN32_SUBSYSTEM == VD_FW_WIN32_SUBSYSTEM_WINDOWS)
        if (info->gl.debug_on) {
            AllocConsole();
            AttachConsole(GetCurrentProcessId());
            DWORD written;
            SetConsoleTitle(TEXT("DEBUG CONSOLE"));
            TCHAR *msg = TEXT("Console allocated for debugging\n");
            SIZE_T len = vd_fw__tcslen(msg);
            WriteConsole(
                GetStdHandle(STD_OUTPUT_HANDLE),
                msg,
                (DWORD)len,
                &written,
                0);
#if !VD_FW_NO_CRT
            freopen("conin$","r",stdin);
            freopen("conout$","w",stdout);
            freopen("conout$","w",stderr);
#endif // !VD_FW_NO_CRT

        }
#endif
    }

    SetConsoleOutputCP(CP_UTF8);

    {

        TCHAR *buf = (TCHAR*)vd_fw__realloc_mem(NULL, sizeof(TCHAR) * MAX_PATH);
        DWORD nsize = GetModuleFileName(NULL, buf, MAX_PATH);
#ifdef UNICODE
        buf[nsize] = 0;
        VD_FW_G.exedir = vd_fw__utf16_to_utf8(buf);
        VD_FW_G.exedir_cap = vd_fw__strlen(VD_FW_G.exedir);
#else
        VD_FW_G.exedir = buf;
        VD_FW_G.exedir_cap = nsize;
#endif 

        const char *c = VD_FW_G.exedir;
        for (DWORD i = (VD_FW_G.exedir_cap - 1); i > 0; --i) {
            if (c[i] == '\\') {
                VD_FW_G.exedir_len = i;
                break;
            }
        }
    }

    InitializeCriticalSection(&VD_FW_G.critical_section);
    InitializeCriticalSection(&VD_FW_G.db_section);
    InitializeCriticalSectionAndSpinCount(&VD_FW_G.input_critical_section, 3000);
    InitializeConditionVariable(&VD_FW_G.cond_var);

    VD_FW_G.sem_window_ready = CreateSemaphoreA(
        NULL,
        0,
        1,
        NULL);

    VD_FW_G.sem_closed = CreateSemaphoreA(
        NULL,
        0,
        1,
        NULL);

    VD_FW_G.win_thread = CreateThread(
        NULL,
        0,
        vd_fw__win_thread_proc,
        0,
        0,
        &VD_FW_G.win_thread_id);
    SetThreadDescription(VD_FW_G.win_thread, L"Window Thread");

    vd_fw__update_kb_codepage();

    WaitForSingleObject(VD_FW_G.sem_window_ready, INFINITE);


    VdFwGraphicsApi api = VD_FW_GRAPHICS_API_OPENGL;
    if (info) {
        api = info->api;
    }

    VdFwOpenGLOptions *poptions = NULL;
    if (info) {
        poptions = &info->gl; 
    }
    vd_fw_set_graphics_api(api, poptions);

    QueryPerformanceCounter(&VD_FW_G.performance_counter);
    VD_FW_G.has_initialized = 1;
    return 1;
}

VD_FW_API int vd_fw_running(void)
{
    DWORD result = WaitForSingleObject(VD_FW_G.sem_closed, 0);
    if (result != WAIT_TIMEOUT) {
        return 0;
    }

    VD_FW_G.wheel_moved = 0;
    VD_FW_G.wheel[0] = 0.f;
    VD_FW_G.wheel[1] = 0.f;
    VD_FW_G.focus_changed = 0;
    VD_FW_G.window_state_changed = 0;
    VD_FW_G.prev_mouse_state = VD_FW_G.mouse_state;
    VD_FW_G.close_request = 0;

    VD_FW_G.num_codepoints = 0;
    VD_FW_G.last_key = VD_FW_KEY_UNKNOWN;
    VdFwU16 num_codepoints = 0;

    for (int i = 0; i < VD_FW_KEY_MAX; ++i) {
        VD_FW_G.prev_key_states[i] = VD_FW_G.curr_key_states[i];
    }
    VdFw__Win32Message mm;
    while (vd_fw__msgbuf_r(&mm)) {
        switch (mm.msg) {

            case VD_FW_WIN32_MESSAGE_TYPE_CHAR: {
                VD_FW_G.codepoints[(num_codepoints++) % VD_FW_CODEPOINT_BUFFER_COUNT] = mm.dat.character.codepoint;
            } break;

            case VD_FW_WIN32_MESSAGE_TYPE_CLOSERQ: {
                VD_FW_G.close_request = 1;
            } break;

            case VD_FW_WIN32_MESSAGE_TYPE_MOUSEMOVE: {
                VD_FW_G.mouse[0] = mm.dat.mousemove.mx;
                VD_FW_G.mouse[1] = mm.dat.mousemove.my;
            } break;

            case VD_FW_WIN32_MESSAGE_TYPE_SCROLL: {
                VD_FW_G.wheel[0] += mm.dat.scroll.dx;
                VD_FW_G.wheel[1] += mm.dat.scroll.dy;
            } break;

            case VD_FW_WIN32_MESSAGE_TYPE_MOUSEBTN: {
                int state_mask = 0;

                switch (mm.dat.mousebtn.vkbutton) {
                    case VK_LBUTTON:  state_mask = VD_FW_MOUSE_BUTTON_LEFT; break;
                    case VK_RBUTTON:  state_mask = VD_FW_MOUSE_BUTTON_RIGHT; break;
                    case VK_MBUTTON:  state_mask = VD_FW_MOUSE_BUTTON_MIDDLE; break;
                    case VK_XBUTTON1: state_mask = VD_FW_MOUSE_BUTTON_M1; break;
                    case VK_XBUTTON2: state_mask = VD_FW_MOUSE_BUTTON_M2; break;
                    default: break;
                }

                if (state_mask) {
                    if (mm.dat.mousebtn.down) {
                        VD_FW_G.mouse_state |= state_mask;
                    } else {
                        VD_FW_G.mouse_state &= ~state_mask;
                    }
                }
            } break;

            case VD_FW_WIN32_MESSAGE_TYPE_CHANGEFOCUS: {
                VD_FW_G.focus_changed = 1;
                VD_FW_G.focused = mm.dat.changefocus.got_focus;
            } break;

            case VD_FW_WIN32_MESSAGE_TYPE_KEYSTATE: {

                int is_down = mm.dat.keystate.down;
                VdFwKey key = vd_fw___vkcode_to_key(mm.dat.keystate.vkcode);

                if (is_down) {
                    VD_FW_G.last_key = key;
                }

                // VD_FW_G.prev_key_states[key] = VD_FW_G.curr_key_states[key];
                VD_FW_G.curr_key_states[key] = (unsigned char)is_down;

            } break;

            case VD_FW_WIN32_MESSAGE_TYPE_STATECHANGE: {
                int prev_state = VD_FW_G.window_state;
                int change_flag = 0;

                if (mm.dat.statechange.window_state_flag_that_changed & VD_FW_WIN32_WINDOW_STATE_MINIMIZED) {
                    change_flag = VD_FW_WIN32_WINDOW_STATE_MINIMIZED;

                    if (mm.dat.statechange.window_state_flag_value) {
                        VD_FW_G.window_state |= VD_FW_WIN32_WINDOW_STATE_MINIMIZED;
                    } else {
                        VD_FW_G.window_state &= ~VD_FW_WIN32_WINDOW_STATE_MINIMIZED;
                    }
                } else if (mm.dat.statechange.window_state_flag_that_changed & VD_FW_WIN32_WINDOW_STATE_MAXIMIZED) {
                    change_flag = VD_FW_WIN32_WINDOW_STATE_MAXIMIZED;

                    if (mm.dat.statechange.window_state_flag_value) {
                        VD_FW_G.window_state |= VD_FW_WIN32_WINDOW_STATE_MAXIMIZED;
                    } else {
                        VD_FW_G.window_state &= ~VD_FW_WIN32_WINDOW_STATE_MAXIMIZED;
                    }
                }

                if (prev_state != VD_FW_G.window_state) {
                    VD_FW_G.window_state_changed |= change_flag;
                }

            } break;

            default: break;
        }
    }

    VD_FW_G.num_codepoints = (num_codepoints < VD_FW_CODEPOINT_BUFFER_COUNT) 
                             ? num_codepoints
                             : VD_FW_CODEPOINT_BUFFER_COUNT;
    if (num_codepoints > 0) {
        VD_FW_G.first_codepoint_index = (num_codepoints - 1) % VD_FW_CODEPOINT_BUFFER_COUNT;
    } else {
        VD_FW_G.first_codepoint_index = 0;
    }

    // @note(mdodis): For Raw Input mouse handling, instead of using the message queue
    // We use two sinks with an atomic write index.
    {
        VD_FW_WIN32_PROFILE_BEGIN(read_all_input);
        VD_FW_G.mouse_delta[0] = VD_FW_G.winthread_mouse_delta[0];
        VD_FW_G.mouse_delta[1] = VD_FW_G.winthread_mouse_delta[1];
        VD_FW_G.winthread_mouse_delta[0] = 0.f;
        VD_FW_G.winthread_mouse_delta[1] = 0.f;
        EnterCriticalSection(&VD_FW_G.input_critical_section);
        VD_FW_G.num_gamepads_present = VD_FW_G.winthread_num_gamepads_present;
        for (int i = 0; i < VD_FW_G.num_gamepads_present; ++i) {
            VD_FW_G.gamepad_prev_states[i] = VD_FW_G.gamepad_curr_states[i];
            VD_FW_G.gamepad_curr_states[i] = VD_FW_G.winthread_gamepad_curr_states[i];
        }
        LeaveCriticalSection(&VD_FW_G.input_critical_section);
        VD_FW_WIN32_PROFILE_END(read_all_input);
    }

    EnterCriticalSection(&VD_FW_G.critical_section);
    VD_FW_G.curr_frame = VD_FW_G.next_frame;
    VD_FW_G.next_frame.flags = 0;
    LeaveCriticalSection(&VD_FW_G.critical_section);

    LARGE_INTEGER now_performance_counter;
    QueryPerformanceCounter(&now_performance_counter);
    LARGE_INTEGER delta;
    delta.QuadPart = now_performance_counter.QuadPart - VD_FW_G.performance_counter.QuadPart;
    unsigned long long q  =  delta.QuadPart / VD_FW_G.frequency.QuadPart;
    unsigned long long r  =  delta.QuadPart % VD_FW_G.frequency.QuadPart;
    unsigned long long ns =  q * 1000000000ULL;
    ns                    += (r * 1000000000ULL) / VD_FW_G.frequency.QuadPart;
    VD_FW_G.last_ns = ns;

    VD_FW_G.performance_counter = now_performance_counter;

    if (VD_FW_G.mouse_is_locked && VD_FW_G.focused) {
        VdFwRECT rect;
        VdFwGetWindowRect(VD_FW_G.hwnd, &rect);

        VdFwINT center_x = (rect.left + rect.right)  / 2;
        VdFwINT center_y = (rect.top  + rect.bottom) / 2;

        VdFwSetCursorPos(center_x, center_y);
    }

    return 1;
}

VD_FW_API int vd_fw_swap_buffers(void)
{
    if (VD_FW_G.graphics_api != VD_FW_GRAPHICS_API_CUSTOM) {
        VdFwSwapBuffers(VD_FW_G.hdc);
    }

    // @note(mdodis): This needs to happen, otherwise the window animations and taskbar don't get redrawn if the window
    // is maximized to either section of the screen or the whole screen
    VdFwDwmFlush();

    if (VD_FW_G.graphics_api == VD_FW_GRAPHICS_API_OPENGL) {
        if (glFenceSync && glClientWaitSync && glDeleteSync) {
            GLsync fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
            if (fence) {
                glClientWaitSync(fence, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000ULL);
                glDeleteSync(fence);
            }
        }
    }

    if (VD_FW_G.curr_frame.flags & VD_FW_WIN32_FLAGS_WAKE_COND_VAR) {
        WakeConditionVariable(&VD_FW_G.cond_var);
    }
    return 1;
}

VD_FW_API int vd_fw_close_requested(void)
{
    return VD_FW_G.close_request;
}

VD_FW_API void vd_fw_quit(void)
{
    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_KILL,
        0, /* WPARAM */
        0  /* LPARAM */));
}

VD_FW_API VdFwPlatform vd_fw_get_platform(void)
{
    return VD_FW_PLATFORM_WINDOWS;
}

VD_FW_API void vd_fw_set_graphics_api(VdFwGraphicsApi api, VdFwOpenGLOptions *gl_options)
{

    WakeConditionVariable(&VD_FW_G.cond_var);

    if (VD_FW_G.graphics_api == VD_FW_GRAPHICS_API_OPENGL) {
        // Destroy OpenGL Context
        VD_FW__CHECK_TRUE(VdFwwglMakeCurrent(NULL, NULL));
        VD_FW__CHECK_TRUE(VdFwwglDeleteContext(VD_FW_G.hglrc));
        VD_FW_G.hglrc = 0;
        VD_FW__CHECK_NONZERO(VdFwReleaseDC(VD_FW_G.hwnd, VD_FW_G.hdc));
        VD_FW_G.hdc = 0;
    }

    if (VD_FW_G.graphics_api != VD_FW_GRAPHICS_API_INVALID) {

        VdFwRECT rect;
        VdFwGetWindowRect(VD_FW_G.hwnd, &rect);

        VD_FW_G.next_width = rect.right - rect.left;
        VD_FW_G.next_height = rect.bottom - rect.top;
        VD_FW_G.next_pos_x = rect.left;
        VD_FW_G.next_pos_y = rect.top;

        // Wait for thread to close
        {
            VD_FW__CHECK_TRUE(VdFwPostMessage(
                VD_FW_G.hwnd,
                VD_FW_WIN32_KILL,
                0, /* WPARAM */
                0  /* LPARAM */));
            WaitForSingleObject(VD_FW_G.win_thread, INFINITE);
        }

        // Reset Semaphores
        {
            CloseHandle(VD_FW_G.sem_window_ready);
            VD_FW_G.sem_window_ready = CreateSemaphoreA(
                NULL,
                0,
                1,
                NULL);

            CloseHandle(VD_FW_G.sem_closed);
            VD_FW_G.sem_closed = CreateSemaphoreA(
                NULL,
                0,
                1,
                NULL);
        }


        // Restart the thread to create the window
        {
            VD_FW_G.t_paint_ready = FALSE;

            VD_FW_G.win_thread = CreateThread(
                NULL,
                0,
                vd_fw__win_thread_proc,
                0,
                0,
                &VD_FW_G.win_thread_id);
            SetThreadDescription(VD_FW_G.win_thread, L"Window Thread");
            WaitForSingleObject(VD_FW_G.sem_window_ready, INFINITE);
        }
    }

    switch (api) {
        case VD_FW_GRAPHICS_API_OPENGL: {

            VD_FW_G.hdc = VdFwGetDC(VD_FW_G.hwnd);

            // Temp context flags
            VdFwPIXELFORMATDESCRIPTOR pfd = {
              sizeof(VdFwPIXELFORMATDESCRIPTOR),
              1,                                // Version Number
              PFD_DRAW_TO_WINDOW |              // Format Must Support Window
              PFD_SUPPORT_OPENGL |              // Format Must Support OpenGL
              PFD_DOUBLEBUFFER,                 // Must Support Double Buffering
              PFD_TYPE_RGBA,                    // Request An RGBA Format
              32,                               // Select Our Color Depth
              0, 0, 0, 0, 0, 0,                 // Color Bits Ignored
              0,                                // An Alpha Buffer
              0,                                // Shift Bit Ignored
              0,                                // No Accumulation Buffer
              0, 0, 0, 0,                       // Accumulation Bits Ignored
              24,                               // 16Bit Z-Buffer (Depth Buffer)
              8,                                // Some Stencil Buffer
              0,                                // No Auxiliary Buffer
              PFD_MAIN_PLANE,                   // Main Drawing Layer
              0,                                // Reserved
              0, 0, 0                           // Layer Masks Ignored
            };
            int pf = VdFwChoosePixelFormat(VD_FW_G.hdc, &pfd);
            VD_FW__CHECK_NONZERO(pf);
            VD_FW__CHECK_TRUE(VdFwSetPixelFormat(VD_FW_G.hdc, pf, &pfd));

            VdFwHGLRC temp_context = VdFwwglCreateContext(VD_FW_G.hdc);
            VD_FW__CHECK_NULL(temp_context);
            VD_FW__CHECK_TRUE(VdFwwglMakeCurrent(VD_FW_G.hdc, temp_context));

            int major = 3;
            int minor = 3;

            if (gl_options && gl_options->version != VD_FW_GL_VERSION_BASIC) {
                switch (gl_options->version) {
                    case VD_FW_GL_VERSION_1_0: major = 1; minor = 0; break;
                    case VD_FW_GL_VERSION_1_2: major = 1; minor = 2; break;
                    case VD_FW_GL_VERSION_1_3: major = 1; minor = 3; break;
                    case VD_FW_GL_VERSION_1_4: major = 1; minor = 4; break;
                    case VD_FW_GL_VERSION_1_5: major = 1; minor = 5; break;
                    case VD_FW_GL_VERSION_2_0: major = 2; minor = 0; break;
                    case VD_FW_GL_VERSION_2_1: major = 2; minor = 1; break;
                    case VD_FW_GL_VERSION_3_0: major = 3; minor = 0; break;
                    case VD_FW_GL_VERSION_3_1: major = 3; minor = 1; break;
                    case VD_FW_GL_VERSION_3_2: major = 3; minor = 2; break;
                    case VD_FW_GL_VERSION_3_3: major = 3; minor = 3; break;
                    case VD_FW_GL_VERSION_4_0: major = 4; minor = 0; break;
                    case VD_FW_GL_VERSION_4_1: major = 4; minor = 1; break;
                    case VD_FW_GL_VERSION_4_2: major = 4; minor = 2; break;
                    case VD_FW_GL_VERSION_4_3: major = 4; minor = 3; break;
                    case VD_FW_GL_VERSION_4_4: major = 4; minor = 4; break;
                    case VD_FW_GL_VERSION_4_5: major = 4; minor = 5; break;
                    case VD_FW_GL_VERSION_4_6: major = 4; minor = 6; break;
                    default: break;
                }
            }

            // Context attributes
            int attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, major,
                WGL_CONTEXT_MINOR_VERSION_ARB, minor,
                WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                WGL_CONTEXT_FLAGS_ARB,         WGL_CONTEXT_DEBUG_BIT_ARB,
                0
            };

            VdFwProcwglCreateContextAttribsARB *wglCreateContextAttribsARB =
                (VdFwProcwglCreateContextAttribsARB*) VdFwwglGetProcAddress("wglCreateContextAttribsARB");

            VdFwProcwglChoosePixelFormatARB *wglChoosePixelFormatARB =
                (VdFwProcwglChoosePixelFormatARB*)VdFwwglGetProcAddress("wglChoosePixelFormatARB");

            int pixel_attribs[] = {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
                WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,

                WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB,     24,
                WGL_ALPHA_BITS_ARB,     8,
                WGL_DEPTH_BITS_ARB,     24,
                WGL_STENCIL_BITS_ARB,   8,

                WGL_SAMPLE_BUFFERS_ARB, 1,
                WGL_SAMPLES_ARB,        4,
                0,
            };

            int pixel_format;
            VdFwUINT num_formats;

            VD_FW__CHECK_TRUE(wglChoosePixelFormatARB(VD_FW_G.hdc, pixel_attribs, NULL, 1, &pixel_format, &num_formats));

            VdFwPIXELFORMATDESCRIPTOR pfdchosen;
            VD_FW__CHECK_NONZERO(VdFwDescribePixelFormat(VD_FW_G.hdc, pixel_format, sizeof(pfdchosen), &pfdchosen));
            VD_FW__CHECK_TRUE(VdFwSetPixelFormat(VD_FW_G.hdc, pf, &pfdchosen));

            VD_FW_G.hglrc = wglCreateContextAttribsARB(VD_FW_G.hdc, 0, attribs);

            VD_FW__CHECK_TRUE(VdFwwglMakeCurrent(NULL, NULL));
            VD_FW__CHECK_TRUE(VdFwwglDeleteContext(temp_context));

            VD_FW__CHECK_NULL(VD_FW_G.hglrc);
            VD_FW__CHECK_TRUE(VdFwwglMakeCurrent(VD_FW_G.hdc, VD_FW_G.hglrc));

            VD_FW_G.proc_swapInterval = (VdFwProcwglSwapIntervalExt*)VdFwwglGetProcAddress("wglSwapIntervalEXT");

            VdFwGlVersion version = VD_FW_GL_VERSION_3_3;
            if (gl_options && gl_options->version != VD_FW_GL_VERSION_BASIC) {
                version = gl_options->version;
            }
            vd_fw__load_opengl(version);

            if (gl_options && gl_options->debug_on && version > VD_FW_GL_VERSION_3_0) {
                VdFwProcGL_glDebugMessageCallback glDebugMessageCallback_proc = (VdFwProcGL_glDebugMessageCallback)VdFwwglGetProcAddress("glDebugMessageCallback");

                if (glDebugMessageCallback_proc == 0) {
                    DWORD written;

                    TCHAR *msg = TEXT("ERROR: Failed to load glDebugMessageCallback!\n");
                    SIZE_T len = vd_fw__tcslen(msg);
                    WriteConsole(
                        GetStdHandle(STD_OUTPUT_HANDLE),
                        msg,
                        (DWORD)len,
                        &written,
                        0);
                } else {
                    glEnable(0x92E0 /* GL_DEBUG_OUTPUT */);
                    glDebugMessageCallback_proc(vd_fw__gl_debug_message_callback, 0);
                }
            }
        } break;

        default: break;
    }

    VD_FW_G.graphics_api = api;
}

VD_FW_API int vd_fw_get_size(int *w, int *h)
{
    *w = VD_FW_G.curr_frame.w;
    *h = VD_FW_G.curr_frame.h;
    return VD_FW_G.curr_frame.flags & VD_FW_WIN32_FLAGS_SIZE_CHANGED;
}

VD_FW_API void vd_fw_set_size(int w, int h)
{
    WORD ww = (WORD)w;
    WORD wh = (WORD)h;
    LPARAM lparam = MAKELPARAM(ww, wh);

    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_SIZE,
        0, /* WPARAM */
        lparam));
}

VD_FW_API void vd_fw_set_size_min(int w, int h)
{
    WORD ww = (WORD)w;
    WORD wh = (WORD)h;
    LPARAM lparam = MAKELPARAM(ww, wh);

    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_SIZEMIN,
        0, /* WPARAM */
        lparam));
}

VD_FW_API void vd_fw_set_size_max(int w, int h)
{
    WORD ww = (WORD)w;
    WORD wh = (WORD)h;
    LPARAM lparam = MAKELPARAM(ww, wh);

    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_SIZEMAX,
        0, /* WPARAM */
        lparam));
}

VD_FW_API int vd_fw_get_minimized(int *minimized)
{
    *minimized = VD_FW_G.window_state & VD_FW_WIN32_WINDOW_STATE_MINIMIZED;
    return VD_FW_G.window_state_changed & VD_FW_WIN32_WINDOW_STATE_MINIMIZED;
}

VD_FW_API void vd_fw_minimize(void)
{
    VdFwShowWindow(VD_FW_G.hwnd, SW_MINIMIZE);
}

VD_FW_API int vd_fw_get_maximized(int *maximized)
{
    *maximized = VD_FW_G.window_state & VD_FW_WIN32_WINDOW_STATE_MAXIMIZED;
    return VD_FW_G.window_state_changed & VD_FW_WIN32_WINDOW_STATE_MAXIMIZED;
}

VD_FW_API void vd_fw_maximize(void)
{
    VdFwShowWindow(VD_FW_G.hwnd, SW_MAXIMIZE);
    VD_FW_G.window_state |= VD_FW_WIN32_WINDOW_STATE_MAXIMIZED;
}

VD_FW_API void vd_fw_normalize(void)
{
    VdFwShowWindow(VD_FW_G.hwnd, SW_NORMAL);
}

VD_FW_API void vd_fw_set_fullscreen(int on)
{
    if (VD_FW_G.is_fullscreen == on) {
        return;
    }

    VD_FW_G.is_fullscreen = on;

    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_FULLSCREEN,
        0, /* WPARAM */
        on /* LPARAM */));
}

VD_FW_API int vd_fw_get_fullscreen(void)
{
    return VD_FW_G.is_fullscreen;
}

VD_FW_API int vd_fw_get_focused(int *focused)
{
    *focused = VD_FW_G.focused;
    return VD_FW_G.focus_changed;
}

VD_FW_API void vd_fw_set_ncrects(int caption[4], int count, int (*rects)[4])
{
    VD_FW_G.nccaption_set = 1;
    VD_FW_G.nccaption[0] = caption[0];
    VD_FW_G.nccaption[1] = caption[1];
    VD_FW_G.nccaption[2] = caption[2];
    VD_FW_G.nccaption[3] = caption[3];

    VD_FW_G.ncrect_count = count;
    int c = count;
    if (c > VD_FW_NCRECTS_MAX) {
        c = VD_FW_NCRECTS_MAX;
    }
    for (int i = 0; i < c; ++i) {
        VD_FW_G.ncrects[i][0] = rects[i][0];
        VD_FW_G.ncrects[i][1] = rects[i][1];
        VD_FW_G.ncrects[i][2] = rects[i][2];
        VD_FW_G.ncrects[i][3] = rects[i][3];
    }
}

VD_FW_API void vd_fw_set_receive_ncmouse(int on)
{
    VD_FW_G.receive_ncmouse_on = on;
}

VD_FW_API int vd_fw_set_vsync_on(int on)
{
    if (VD_FW_G.graphics_api == VD_FW_GRAPHICS_API_OPENGL) {
        BOOL result = VD_FW_G.proc_swapInterval(on);
        return result == TRUE ? on : 0;
    }

    return 0;
}

VD_FW_API const char *vd_fw_get_executable_dir(int *len)
{
    *len = VD_FW_G.exedir_len; 
    return VD_FW_G.exedir; 
}

VD_FW_API int vd_fw_get_mouse_state(int *x, int *y)
{
    if (x) *x = VD_FW_G.mouse[0];
    if (y) *y = VD_FW_G.mouse[1];

    return VD_FW_G.mouse_state;
}

VD_FW_API int vd_fw_get_mouse_clicked(int button)
{
    return !(VD_FW_G.prev_mouse_state & button) && (VD_FW_G.mouse_state & button);
}

VD_FW_API int vd_fw_get_mouse_released(int button)
{
    return (VD_FW_G.prev_mouse_state & button) && !(VD_FW_G.mouse_state & button);
}

VD_FW_API int vd_fw_get_key_pressed(int key)
{
    return !VD_FW_G.prev_key_states[key] && VD_FW_G.curr_key_states[key];
}

VD_FW_API int vd_fw_get_key_down(int key)
{
    return VD_FW_G.curr_key_states[key];
}

VD_FW_API int vd_fw_get_last_key_pressed(void)
{
    return VD_FW_G.last_key;
}

VD_FW_API int vd_fw_get_gamepad_count(void)
{
    return VD_FW_G.num_gamepads_present;
}

VD_FW_API VdFwU64 vd_fw_get_gamepad_button_state(int index)
{
    return VD_FW_G.gamepad_curr_states[index].bits;
}

VD_FW_API int vd_fw_get_gamepad_down(int index, int button)
{
    return (VD_FW_G.gamepad_curr_states[index].bits >> button) & 1;
}

VD_FW_API int vd_fw_get_gamepad_pressed(int index, int button)
{

    return ((VD_FW_G.gamepad_curr_states[index].bits >> button) & 1) && 
          !((VD_FW_G.gamepad_prev_states[index].bits >> button) & 1);
}

VD_FW_API int vd_fw_get_gamepad_axis(int index, int axis, float *out)
{
    *out = VD_FW_G.gamepad_curr_states[index].axes[axis];
    return 1;
}

VD_FW_API void vd_fw_set_gamepad_rumble(int index, float rumble_lo, float rumble_hi)
{
    WORD rl = (WORD)(rumble_lo * 65535.f);
    WORD rh = (WORD)(rumble_hi * 65535.f);
    LPARAM lparam = MAKELPARAM(rl, rh);

    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_GAMEPADRMBREQ,
        index, /* WPARAM */
        lparam));
}

VD_FW_API VdFwGuid vd_fw_get_gamepad_guid(int index)
{
    return VD_FW_G.gamepad_curr_states[index].guid;
}

VD_FW_API VdFwGamepadFace vd_fw_get_gamepad_face(int index)
{
    return VD_FW_G.gamepad_curr_states[index].face;
}

VD_FW_API VdFwGamepadClass vd_fw_get_gamepad_class(int index)
{
    return VD_FW_G.gamepad_curr_states[index].klass;
}


VD_FW_API int vd_fw_get_gamepad_rumble_support(int index)
{
    return VD_FW_G.gamepad_curr_states[index].has_rumble;
}

VD_FW_API void vd_fw_set_gamepad_raw_reports(int on)
{
    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_GAMEPADRAWRQ,
        on,
        0));
}

VD_FW_API VdFwU64 vd_fw_get_gamepad_raw_buttons(int index)
{
    return VD_FW_G.gamepad_curr_states[index].raw_bits;
}

VD_FW_API float *vd_fw_get_gamepad_raw_axes(int index, int *count_axes)
{
    *count_axes = 16;
    return VD_FW_G.gamepad_curr_states[index].raw_axes;
}

VD_FW_API unsigned short vd_fw_get_num_codepoints(void)
{
    return VD_FW_G.num_codepoints;    
}

VD_FW_API unsigned int vd_fw_get_codepoint(unsigned short index)
{
    return VD_FW_G.codepoints[(VD_FW_G.first_codepoint_index + index) % VD_FW_CODEPOINT_BUFFER_COUNT];
}

VD_FW_API void vd_fw_set_mouse_capture(int on)
{
    if (on) {
        VdFwSetCapture(VD_FW_G.hwnd);
    } else {
        VdFwReleaseCapture();
    }
}

VD_FW_API void vd_fw_get_mouse_delta(float *dx, float *dy)
{
    if (dx) *dx = VD_FW_G.mouse_delta[0];
    if (dy) *dy = VD_FW_G.mouse_delta[1];
}

VD_FW_API void vd_fw_set_mouse_locked(int locked)
{
    if (VD_FW_G.mouse_is_locked == locked) {
        return;
    }

    VD_FW_G.mouse_is_locked = locked;
    if (locked) {
        VD_FW__CHECK_TRUE(VdFwPostMessage(
            VD_FW_G.hwnd,
            VD_FW_WIN32_SHOW_CURSOR,
            0, /* WPARAM */
            0  /* LPARAM */));
    } else {
        VD_FW__CHECK_TRUE(VdFwPostMessage(
            VD_FW_G.hwnd,
            VD_FW_WIN32_SHOW_CURSOR,
            1, /* WPARAM */
            0  /* LPARAM */));
    }
}

VD_FW_API int vd_fw_get_mouse_locked(void)
{
    return VD_FW_G.mouse_is_locked;
}

VD_FW_API int vd_fw_get_mouse_wheel(float *dx, float *dy)
{
    if (dx) *dx = VD_FW_G.wheel[0];
    if (dy) *dy = VD_FW_G.wheel[1];
    return VD_FW_G.wheel_moved;
}

VD_FW_API float vd_fw_get_scale(void)
{
    return (float)VdFwGetDpiForWindow(VD_FW_G.hwnd) / 90.f;
}

VD_FW_API void vd_fw_set_title(const char *title)
{
    int len = 0;
    const char *t = title;
    while (*t++) len++;

    if (len > 127) {
        len = 127;
    }

    for (int i = 0; i < len; ++i) {
        VD_FW_G.title[i] = title[i];
    }
    VD_FW_G.title_len = len;

    VD_FW_G.title[len] = 0;

    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_UPDATE_TITLE,
        0, /* WPARAM */
        0  /* LPARAM */));
}

VD_FW_API void vd_fw_set_app_icon(void *pixels, int width, int height)
{
    VdFwBITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize        = sizeof(VdFwBITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = width;
    bmi.bmiHeader.biHeight      = height;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB; 

    void *bits = 0;
    VdFwHDC hdc = VdFwGetDC(NULL);
    VdFwHBITMAP hbitmap = VdFwCreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
    VD_FW__CHECK_NULL(hbitmap);
    VdFwHBITMAP hmaskbitmap = VdFwCreateBitmap(width, height, 1, 1, NULL);
    VD_FW__CHECK_NULL(hmaskbitmap);
    VdFwReleaseDC(NULL, hdc);

    VD_FW__CHECK_NULL(hbitmap);

    VD_FW_MEMCPY(bits, pixels, width * height * 4);

    VdFwICONINFO ii = {0};
    ii.fIcon    = TRUE;
    ii.hbmColor = hbitmap;
    ii.hbmMask  = hmaskbitmap;
    VdFwHICON icon = VdFwCreateIconIndirect(&ii);
    VD_FW__CHECK_NULL(icon);

    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        WM_SETICON,
        ICON_SMALL, /* WPARAM */
        (LPARAM)icon));
    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        WM_SETICON,
        ICON_BIG, /* WPARAM */
        (LPARAM)icon));
}

VD_FW_API void *vd_fw_get_internal_window_handle(void)
{
    return &VD_FW_G.hwnd;
}

VD_FW_API unsigned long long vd_fw_delta_ns(void)
{
    return VD_FW_G.last_ns;
}

static DWORD vd_fw__win_thread_proc(LPVOID param)
{
    (void)param;
    VD_FW_G.t_running = TRUE;
    // VD_FW_SANITY_CHECK();

    static int has_registered_class = 0;

    if (!has_registered_class) {
        VdFwWNDCLASSEX wcx;
        ZeroMemory(&wcx, sizeof(wcx));
        wcx.cbSize         = sizeof(wcx);
        wcx.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcx.hInstance      = NULL;
        wcx.lpfnWndProc    = vd_fw__wndproc;
        wcx.lpszClassName  = TEXT("FWCLASS");
        wcx.hbrBackground  = (VdFwHBRUSH)VdFwGetStockObject(BLACK_BRUSH);
        wcx.hCursor        = VdFwLoadCursor(NULL, IDC_ARROW);
        if (!VdFwRegisterClassEx(&wcx)) {
            return 0;
        }

        has_registered_class = 1;
    }

    LONG window_style;
    if (VD_FW_G.draw_decorations) {
        window_style = WS_OVERLAPPEDWINDOW | WS_SIZEBOX;
    } else {
        window_style = WS_OVERLAPPED | WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
    }

    // @note(mdodis): Some tests with windows
    // - WS_EX_OVERLAPPEDWINDOW:                      Doesn't do anything at all except paint a thick frame border when window gets focus
    //                                                So we don't use it
    //                                                
    // - WS_EX_APPWINDOW:                             Doesn't do anything useful.
    // - WS_EX_LAYERED:                               Doesn't do anything useful for non-transparent windows except make resizing reaaally slow
    //                                                
    // - WS_OVERLAPPEDWINDOW:                         Works even for DwmExtended windows, but DWM will draw the buttons and captions
    //                                                of the window and mess up alpha composition
    // 
    // - WS_OVERLAPPED | WS_SIZEBOX | WS_MAXIMIZEBOX: Works perfectly.
    // 

    VD_FW_G.hwnd = VdFwCreateWindowEx(
        0,
        TEXT("FWCLASS"),
        TEXT("FW Window"),
        window_style,
        VD_FW_G.next_pos_x,
        VD_FW_G.next_pos_y,
        VD_FW_G.next_width,
        VD_FW_G.next_height,
        0 /* hwndParent */,
        0 /* hMenu */,
        0 /* hInstance */,
        0 /* lpParam */);

    // @note(mdodis): This is an old thing from Windows 7 (I think... era). W
    // SetLayeredWindowAttributes(VD_FW_G.hwnd, RGB(255, 0, 255), 255, LWA_COLORKEY);

    if (VD_FW_G.draw_decorations) {
        if (VD_FW_G.os_version.dwBuildNumber >= 22000) {
            // @note(mdodis): Undocumented mica values: 0x02: 0x04
            VdFwDWORD t = TRUE;
            VdFwDwmSetWindowAttribute(VD_FW_G.hwnd, 20, &t, sizeof(t));
            int mica_value = 0x04;
            VdFwDwmSetWindowAttribute(VD_FW_G.hwnd, 38, &mica_value, sizeof(mica_value));
        } else if (VD_FW_G.os_version.dwMajorVersion >= 10) {
            // @note(mdodis): Dark mode
            VdFwDWORD t = TRUE;
            VdFwDwmSetWindowAttribute(VD_FW_G.hwnd, 20, &t, sizeof(t));
        }
    }
    vd_fw__composition_changed();
    VD_FW_SANITY_CHECK();
    vd_fw__theme_changed();
    VD_FW_SANITY_CHECK();

    VdFwRECT rect;
    VdFwGetClientRect(VD_FW_G.hwnd, &rect);
    VD_FW_G.w = rect.right - rect.left;
    VD_FW_G.h = rect.bottom - rect.top;
    VD_FW_G.next_frame.w = VD_FW_G.w;
    VD_FW_G.next_frame.h = VD_FW_G.h;


    // SetWindowPos(VD_FW_G.hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_SHOWWINDOW);
    VdFwShowWindow(VD_FW_G.hwnd, SW_SHOW);
    VD_FW__CHECK_NONZERO(VdFwUpdateWindow(VD_FW_G.hwnd));
    VD_FW__CHECK_NONZERO(VdFwSetFocus(VD_FW_G.hwnd));
    VD_FW__CHECK_NONZERO(VdFwSetForegroundWindow(VD_FW_G.hwnd));

    VD_FW__CHECK_TRUE(ReleaseSemaphore(VD_FW_G.sem_window_ready, 1, NULL));

    // Register raw input mouse
    VdFwRAWINPUTDEVICE rids[2];
    rids[0].usUsagePage = 0x01; // Generic desktop controls
    rids[0].usUsage     = 0x02; // Mouse
    rids[0].dwFlags     = 0x00; // None (NO RIDEV_INPUTSINK)
    rids[0].hwndTarget  = VD_FW_G.hwnd;

    rids[1].usUsagePage = 0x01; // Generic desktop controls
    rids[1].usUsage     = 0x05; // Gamepad
    rids[1].dwFlags     = RIDEV_DEVNOTIFY | RIDEV_INPUTSINK;
    rids[1].hwndTarget  = VD_FW_G.hwnd;
    VD_FW__CHECK_TRUE(VdFwRegisterRawInputDevices(rids, 2, sizeof(rids[0])));

    VD_FW_G.last_window_style = window_style;
    VD_FW_G.t_paint_ready = 1;

    while (VD_FW_G.t_running) {
        VdFwMSG message;
        // @note(mdodis): https://devblogs.microsoft.com/oldnewthing/20050209-00/?p=36493
        // We don't filter by window since WM_INPUTLANGCHANGE won't fire instantly
        while (VdFwGetMessage(&message, NULL, 0, 0)) {
            VdFwTranslateMessage(&message);
            VdFwDispatchMessage(&message);
        }
    }

    VD_FW_G.num_gamepads_present = 0;
    VD_FW_G.winthread_num_gamepads_present = 0;
    for (int i = 0; i < VD_FW_GAMEPAD_COUNT_MAX; ++i) {
        VD_FW_G.gamepad_infos[i].connected = 0;
        VD_FW_G.gamepad_infos[i].handle = NULL;
    }

    return 0;
}

VD_FW_API int vd_fw__any_time_higher(int num_files, const char **files, unsigned long long *check_against)
{
    int result = 0;

    // Reinterpret check_against as FILETIME
    FILETIME *against = (FILETIME*)check_against;
    for (int i = 0; i < num_files; ++i) {

        HANDLE hfile = CreateFileA(files[i],
                                   GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                   0, // lpSecurityAttributes
                                   OPEN_EXISTING,
                                   FILE_ATTRIBUTE_NORMAL,
                                   NULL);
        if (hfile == INVALID_HANDLE_VALUE) {
            continue;
        }

        FILETIME creation_time, last_access_time, last_write_time;
        if (!GetFileTime(hfile, &creation_time, &last_access_time, &last_write_time)) {
            CloseHandle(hfile);
            continue;
        }

        if (CompareFileTime(&last_write_time, against) > 0) {
            result = 1;
            *against = last_write_time;
            CloseHandle(hfile);
            break;
        }

        CloseHandle(hfile);
    }

    return result;
}

VD_FW_API char *vd_fw__debug_dump_file_text(const char *path)
{
    HANDLE hfile = CreateFileA(path,
                               GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                               0, // lpSecurityAttributes
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               NULL);
    if (hfile == INVALID_HANDLE_VALUE) {
        return 0;
    }

    LARGE_INTEGER sz;
    if (!GetFileSizeEx(hfile, &sz)) {
        return 0;
    }

    char *memory = (char*)HeapAlloc(GetProcessHeap(), 0, sz.QuadPart + 1);

    DWORD bytes_read;
    if (!ReadFile(hfile, memory, (DWORD)sz.QuadPart, &bytes_read, 0)) {
        HeapFree(GetProcessHeap(), 0, memory);
        return 0;
    }

    memory[sz.QuadPart] = 0;
    return memory;
}

VD_FW_API void *vd_fw__realloc_mem(void *prev_ptr, size_t size)
{
    if (prev_ptr == 0) {
        return HeapAlloc(GetProcessHeap(), 0, size);
    } else {
        return HeapReAlloc(GetProcessHeap(), 0, prev_ptr, size);
    }
}

VD_FW_API void vd_fw__free_mem(void *memory)
{
    HeapFree(GetProcessHeap(), 0, memory);
}

VD_FW_API void vd_fw__lock_gamepaddb(void)
{
    EnterCriticalSection(&VD_FW_G.db_section);
}

VD_FW_API void vd_fw__unlock_gamepaddb(void)
{
    LeaveCriticalSection(&VD_FW_G.db_section);
}

VD_FW_API void vd_fw__notify_gamepaddb_changed(void)
{
    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_GAMEPADDBCH,
        0, /* WPARAM */
        0  /* LPARAM */));
}

static void vd_fw__gl_debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    (void)userParam;
    (void)severity;
    (void)id;
    (void)type;
    (void)source;

    DWORD written;
    WriteConsoleA(
        GetStdHandle(STD_OUTPUT_HANDLE),
        message,
        length,
        &written,
        0);

    WriteConsoleA(
        GetStdHandle(STD_OUTPUT_HANDLE),
        "\n",
        1,
        &written,
        0);
}

static int vd_fw__hit_test(int x, int y)
{
    VdFwPOINT mouse;
    mouse.x = x;
    mouse.y = y;
    VdFwScreenToClient(VD_FW_G.hwnd, &mouse);

    VdFwRECT client;
    VdFwGetClientRect(VD_FW_G.hwnd, &client);
    int width  = client.right - client.left;
    int height = client.bottom - client.top;

    /* The horizontal frame should be the same size as the vertical frame,
       since the NONCLIENTMETRICS structure does not distinguish between them */
    int frame_size = VdFwGetSystemMetrics(SM_CXFRAME) +
                     VdFwGetSystemMetrics(SM_CXPADDEDBORDER);
    /* The diagonal size handles are wider than the frame */
    int diagonal_width = frame_size * 2 + VdFwGetSystemMetrics(SM_CXBORDER);

    if (!VD_FW_G.draw_decorations && VdFwIsZoomed(VD_FW_G.hwnd)) {
        mouse.y += frame_size;
    }

    if (!VdFwPtInRect(&client, mouse)) {
        return HTNOWHERE;
    }

    if (!VD_FW_G.is_fullscreen) {
        if (mouse.y < frame_size) {
            if (mouse.x < diagonal_width) {
                return HTTOPLEFT;
            }

            if (mouse.x >= width - diagonal_width) {
                return HTTOPRIGHT;
            }

            return HTTOP;
        }

        if (mouse.y >= height - frame_size) {
            if (mouse.x < diagonal_width) {
                return HTBOTTOMLEFT;
            }

            if (mouse.x >= width - diagonal_width) {
                return HTBOTTOMRIGHT;
            }

            return HTBOTTOM;
        }

        if (mouse.x < frame_size) {
            return HTLEFT;
        }

        if (mouse.x >= width - frame_size) {
            return HTRIGHT;
        }

        if (!VD_FW_G.nccaption_set) {
            return HTCAPTION;
        }

        int inside_caption = 
            ((mouse.x >= VD_FW_G.nccaption[0]) && (mouse.x <= VD_FW_G.nccaption[2])) &&
            ((mouse.y >= VD_FW_G.nccaption[1]) && (mouse.y <= VD_FW_G.nccaption[3]));

        if (inside_caption) {
            for (int ri = 0; ri < VD_FW_G.ncrect_count; ++ri) {
                int rect[4] = {
                    VD_FW_G.ncrects[ri][0],
                    VD_FW_G.ncrects[ri][1],
                    VD_FW_G.ncrects[ri][2],
                    VD_FW_G.ncrects[ri][3],
                };

                int inside =
                    ((mouse.x >= rect[0]) && (mouse.x <= rect[2])) &&
                    ((mouse.y >= rect[1]) && (mouse.y <= rect[3]));

                if (inside) {
                    return HTCLIENT;
                }
            }

            return HTCAPTION;
        }
    }

    return HTCLIENT;
}

static void vd_fw__composition_changed(void)
{
    if (VD_FW_G.draw_decorations) return;
    VD_FW_WIN32_PROFILE_BEGIN(composition_changed);

    BOOL enabled = FALSE;
    VD_FW__CHECK_HRESULT(VdFwDwmIsCompositionEnabled(&enabled));
    VD_FW_G.composition_enabled = enabled;

    if (enabled) {
        VdFwMARGINS m = {-1};
        VD_FW__CHECK_HRESULT(VdFwDwmExtendFrameIntoClientArea(VD_FW_G.hwnd, &m));

        // @note(mdodis): If we set this to disabled, then every time we resize the Windows 7 frame gets drawn behind
        // Additionally alpha compositing is done on fragments that haven't received a full alpha.
        // Also see vd_fw__wndproc, WM_NCPAINT
        {
            VdFwDWORD value = VD_FW_DWMNCRP_USEWINDOWSTYLE;
            VD_FW__CHECK_HRESULT(VdFwDwmSetWindowAttribute(VD_FW_G.hwnd, VD_FW_DWMWA_NCRENDERING_POLICY, &value, sizeof(value)));
        }
        // {
        //     BOOL value = TRUE;
        //     VD_FW__CHECK_HRESULT(DwmSetWindowAttribute(VD_FW_G.hwnd, DWMWA_ALLOW_NCPAINT, &value, sizeof(value)));
        // }

    }

    vd_fw__update_region();
    VD_FW_WIN32_PROFILE_END(composition_changed);
}

static void vd_fw__update_region(void)
{
    VdFwRECT old_rgn = VD_FW_G.rgn;

    if (VdFwIsZoomed(VD_FW_G.hwnd)) {
        // @note(mdodis): If the window is maximized when get the client and window rects and set the region subtracted
        // by the overall window's top left coordinates
        VdFwWINDOWINFO window_info = {};
        window_info.cbSize = sizeof(window_info);
        VdFwGetWindowInfo(VD_FW_G.hwnd, &window_info);
        VD_FW_G.rgn.left   = window_info.rcClient.left   - window_info.rcWindow.left;
        VD_FW_G.rgn.top    = window_info.rcClient.top    - window_info.rcWindow.top;
        VD_FW_G.rgn.right  = window_info.rcClient.right  - window_info.rcWindow.left;
        VD_FW_G.rgn.bottom = window_info.rcClient.bottom - window_info.rcWindow.top;
    } else if (!VD_FW_G.composition_enabled) {
        // @note(mdodis): If composition is enabled, set the window's region to something really high so that shadows of
        // the window are still drawn
        VD_FW_G.rgn.left   = 0;
        VD_FW_G.rgn.top    = 0;
        VD_FW_G.rgn.right  = 32767;
        VD_FW_G.rgn.bottom = 32767;
    } else {
        // @note(mdodis): Otherwise, the window's region is left unchanged
        VD_FW_G.rgn.left   = 0;
        VD_FW_G.rgn.top    = 0;
        VD_FW_G.rgn.right  = 0;
        VD_FW_G.rgn.bottom = 0;
    }

    if (VdFwEqualRect(&VD_FW_G.rgn, &old_rgn)) {
        return;
    }

    VdFwRECT zero_rect = {};
    if (VdFwEqualRect(&VD_FW_G.rgn, &zero_rect)) {
        VdFwSetWindowRgn(VD_FW_G.hwnd, NULL, TRUE);
    } else {
        VdFwSetWindowRgn(VD_FW_G.hwnd, VdFwCreateRectRgnIndirect(&VD_FW_G.rgn), TRUE);
    }
}

static void vd_fw__theme_changed(void)
{
    VD_FW_G.theme_enabled = VdFwIsThemeActive();
}

static LRESULT vd_fw__nccalcsize(WPARAM wparam, LPARAM lparam)
{
    int borderless = !VD_FW_G.draw_decorations;
    if (wparam && borderless) {

        VdFwNCCALCSIZE_PARAMS *params = (VdFwNCCALCSIZE_PARAMS*)lparam;
        if (VdFwIsZoomed(VD_FW_G.hwnd)) {
            VdFwHMONITOR monitor = VdFwMonitorFromWindow(VD_FW_G.hwnd, MONITOR_DEFAULTTONULL);
            if (!monitor) {
                return 0;
            }

            VdFwMONITORINFO monitor_info = {0};
            monitor_info.cbSize = sizeof(monitor_info);
            if (!VdFwGetMonitorInfo(monitor, &monitor_info)) {
                return 0;
            }

            params->rgrc[0] = monitor_info.rcWork;
            return 0;
        } else {
            params->rgrc[0].bottom += 1;
            return WVR_VALIDRECTS;
        }

    } else {
        return VdFwDefWindowProc(VD_FW_G.hwnd, WM_NCCALCSIZE, wparam, lparam);
    }
}

static BOOL vd_fw__has_autohide_taskbar(VdFwUINT edge, VdFwRECT monitor)
{
    if (IsWindows8Point1OrGreater()) {
        VdFwAPPBARDATA appbar_data = {0};
        appbar_data.cbSize = sizeof(appbar_data);
        appbar_data.uEdge  = edge;
        appbar_data.rc     = monitor;
        return VdFwSHAppBarMessage(VD_FW_ABM_GETAUTOHIDEBAREX, &appbar_data) != 0;
    }

    if (monitor.left != 0 || monitor.top != 0) {
        return FALSE;
    }

    VdFwAPPBARDATA appbar_data = {0};
    appbar_data.cbSize = sizeof(appbar_data);
    appbar_data.uEdge  = edge;
    return VdFwSHAppBarMessage(VD_FW_ABM_GETAUTOHIDEBAR, &appbar_data) != 0;
}

static void vd_fw__window_pos_changed(VdFwWINDOWPOS *pos)
{
#if 0
    VD_FW_WIN32_PROFILE_BEGIN(window_pos_changed);
#endif

    if (VD_FW_G.draw_decorations) {
        VdFwRECT client;
        VdFwGetClientRect(VD_FW_G.hwnd, &client);
        VD_FW_G.w = client.right - client.left;
        VD_FW_G.h = client.bottom - client.top;

    } else {
        VD_FW_G.w = pos->cx;
        VD_FW_G.h = pos->cy;

        // @note(mdodis): When window is maximized, pos->x and pos->y become -8, -8
        // So, subtract them from the overall width and height 2 times each so that
        // the maximized viewport is fully shown instead of clipped at the top and right
        if (VdFwIsZoomed(VD_FW_G.hwnd)) {
            VD_FW_G.w += 2 * pos->x;
            VD_FW_G.h += 2 * pos->y;
        }
    }

    if (pos->flags & SWP_FRAMECHANGED) {
        vd_fw__update_region();
    }

#if 0
    VD_FW_WIN32_PROFILE_END(window_pos_changed);
#endif
}

static VdFwLRESULT vd_fw__handle_invisible(VdFwHWND hwnd, VdFwUINT msg, VdFwWPARAM wparam, VdFwLPARAM lparam)
{
    // @note(mdodis): Prevent windows from drawing the default (Windows 7) titlebar and frames by toggling it temporarily
    VD_FW_WIN32_PROFILE_BEGIN(handle_invisible);

    VdFwLONG_PTR old_style = VdFwGetWindowLongPtr(hwnd, GWL_STYLE);

    VdFwSetWindowLongPtr(hwnd, GWL_STYLE, old_style & ~WS_VISIBLE);    
    VdFwLRESULT result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
    VdFwSetWindowLongPtr(hwnd, GWL_STYLE, old_style);

    VD_FW_WIN32_PROFILE_END(handle_invisible);

    return result;
}

static struct {
    VdFwXINPUT_STATE state;
    int              connected;
} Vd_Fw__XInput_States[4];

// See: https://learn.microsoft.com/en-us/windows/win32/xinput/directinput-and-xusb-devices
// 
// In essence, for legacy reasons some controllers report a single "Generic Z (0x0032)" as a partwise mapping to
// [LT, 0, RT].
// By using RAWINPUT/DirectInput it is impossible to distinguish between the left and right triggers being fully
// depressed or released simultaneously.
// 
// However, it is possible to tell if such an HID behaves this way, heuristically. Typically, these are all XInput
// devices.
// 
// So, to handle this issue, SDL correlates mapped gamepad inputs to XInput states to first determine if a controller
// reported by RAWINPUT/DirectInput actually maps to its assigned dwUserIndex by XInput. This is because XInput 'tries'
// to bring the same 4-player couch co-op experience supported by the xbox consoles, and as such:
// - There is a way to determine if a controller is handled by XInput (By checking its path)
// - But there is no simple way to know which dwUserIndex was assigned to said controller.
// 
// As far as I can understand, once a controller is correlated in XInput, SDL will switch to using that API to get
// buttons, axes and triggers. My tests, however, indicate that there is at least _one_ XInput controller that can
// report more buttons than those exposed by XInput (including the guide button). So in this function, only the reported
// trigger values are written if the controller was correlated to an xinput controller.
// 
// So... to summarize: Here I check each dwUserIndex controller state and match:
// - Each button to the raw input mapped ones
// - The combined Z value to the triggers
// - (In the future possibly the analog sticks as well)
// 
// After that we can safely assume that this dwUserIndex will stay mapped to the corresponding controller.
// 
// When any controller is connected/disconnected, the .xinput_index value is reset to -1, and then correlation has to
// happen again...
// 
// This essentially means that for xinput controllers, we'll have to use mmozeiko's 
// xbox DeviceIoControl (https://gist.github.com/mmozeiko/b8ccc54037a5eaf35432396feabbe435) call to send rumble
// to the controller, and then once (or if) the controller is correlated use XInputSetState.
// 
// https://learn.microsoft.com/en-us/windows/win32/xinput/getting-started-with-xinput#getting-controller-state
// Microsoft states here:
// | For performance reasons, don't call XInputGetState for an 'empty' user slot every frame. We recommend that
// | you space out checks for new controllers every few seconds instead.
// 
// This function is only called when a WM_INPUT message arrives, and when that gamepad is determined to be an xinput
// gamepad. Since XInputGetState is only called on a per-packet basis, this function's performance implications should
// be relatively small. However, should you get _any_ bottleneck from this function please report it!
static void vd_fw__win32_correlate_xinput_triggers(VdFw__Win32GamepadInfo *gamepad_info,
                                                   VdFw__GamepadButtonBits button_states, float *axes,
                                                   VdFwULONG z_value)
{
    int total_xinput_devices_connected = 0;
    for (int i = 0; i < 4; ++i) {
        Vd_Fw__XInput_States[i].connected = VdFwXInputGetState(i, &Vd_Fw__XInput_States[i].state) == ERROR_SUCCESS;
        total_xinput_devices_connected += Vd_Fw__XInput_States[i].connected;
    }

    if (gamepad_info->xinput_index == -1) {
        for (int i = 0; i < 4; ++i) {
            if (!Vd_Fw__XInput_States[i].connected) {
                continue;
            }

            VdFwXINPUT_STATE *state = &Vd_Fw__XInput_States[i].state;
            int matched = 0;

            {
                // 0x0400: GUIDE Button
                int any_xinput_buttons_pressed = (state->Gamepad.wButtons & ~0x0400);
                if (any_xinput_buttons_pressed) {
                    matched = matched || 
                        (
                            (((button_states >> VD_FW_GAMEPAD_A) & 1)      == ((state->Gamepad.wButtons &               VD_FW_XINPUT_GAMEPAD_A) ? 1 : 0)) &&
                            (((button_states >> VD_FW_GAMEPAD_B) & 1)      == ((state->Gamepad.wButtons &               VD_FW_XINPUT_GAMEPAD_B) ? 1 : 0)) &&
                            (((button_states >> VD_FW_GAMEPAD_X) & 1)      == ((state->Gamepad.wButtons &               VD_FW_XINPUT_GAMEPAD_X) ? 1 : 0)) &&
                            (((button_states >> VD_FW_GAMEPAD_Y) & 1)      == ((state->Gamepad.wButtons &               VD_FW_XINPUT_GAMEPAD_Y) ? 1 : 0)) &&
                            (((button_states >> VD_FW_GAMEPAD_DUP) & 1)    == ((state->Gamepad.wButtons &         VD_FW_XINPUT_GAMEPAD_DPAD_UP) ? 1 : 0)) &&
                            (((button_states >> VD_FW_GAMEPAD_DDOWN) & 1)  == ((state->Gamepad.wButtons &       VD_FW_XINPUT_GAMEPAD_DPAD_DOWN) ? 1 : 0)) &&
                            (((button_states >> VD_FW_GAMEPAD_DRIGHT) & 1) == ((state->Gamepad.wButtons &      VD_FW_XINPUT_GAMEPAD_DPAD_RIGHT) ? 1 : 0)) &&
                            (((button_states >> VD_FW_GAMEPAD_DLEFT) & 1)  == ((state->Gamepad.wButtons &       VD_FW_XINPUT_GAMEPAD_DPAD_LEFT) ? 1 : 0)) &&
                            (((button_states >> VD_FW_GAMEPAD_L1) & 1)     == ((state->Gamepad.wButtons &   VD_FW_XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1 : 0)) &&
                            (((button_states >> VD_FW_GAMEPAD_R1) & 1)     == ((state->Gamepad.wButtons &  VD_FW_XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1 : 0)) &&
                            (((button_states >> VD_FW_GAMEPAD_L3) & 1)     == ((state->Gamepad.wButtons &      VD_FW_XINPUT_GAMEPAD_LEFT_THUMB) ? 1 : 0)) &&
                            (((button_states >> VD_FW_GAMEPAD_R3) & 1)     == ((state->Gamepad.wButtons &     VD_FW_XINPUT_GAMEPAD_RIGHT_THUMB) ? 1 : 0))
                        );
                }
            }

            if (!matched) {
                int l_trigger_match = ((unsigned int)(((int)state->Gamepad.bLeftTrigger  * 257) - 32768) - z_value <= 0x2fff);
                int r_trigger_match = ((unsigned int)(((int)state->Gamepad.bRightTrigger * 257) - 32768) - z_value <= 0x2fff);
                matched = matched || l_trigger_match || r_trigger_match;
            }

            if (matched) {
                gamepad_info->xinput_index = i;
                VD_FW_LOG("Gamepad correlated to XInput dwUserIndex: %d", i);
                break;
            }
        }

        if (gamepad_info->xinput_index == -1) {
            VD_FW_LOG("Failed to correlate game pad to xinput devices %d/4", total_xinput_devices_connected);
        }
    }

    if (gamepad_info->xinput_index != -1) {
        axes[VD_FW_GAMEPAD_LT] = (float)Vd_Fw__XInput_States[gamepad_info->xinput_index].state.Gamepad.bLeftTrigger / 255.f;
        axes[VD_FW_GAMEPAD_RT] = (float)Vd_Fw__XInput_States[gamepad_info->xinput_index].state.Gamepad.bRightTrigger / 255.f;
    }

}

static VdFwLRESULT vd_fw__wndproc(VdFwHWND hwnd, VdFwUINT msg, VdFwWPARAM wparam, VdFwLPARAM lparam)
{
    VD_FW_WIN32_INVOKE_WNDPROC(hwnd, msg, wparam, lparam);

    VdFwLRESULT result = 0;
    switch (msg) {

        case WM_CLOSE: {
            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_CLOSERQ;
            vd_fw__msgbuf_w(&m);
        } break;

        case WM_DESTROY: {
            ReleaseSemaphore(VD_FW_G.sem_closed, 1, NULL);
            VdFwPostQuitMessage(0);
            VD_FW_G.t_running = FALSE;
        } break;

        case WM_DWMCOMPOSITIONCHANGED: {
            if (VD_FW_G.draw_decorations) {
                result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);

                VdFwBOOL enabled = FALSE;
                VD_FW__CHECK_HRESULT(VdFwDwmIsCompositionEnabled(&enabled));
                VD_FW_G.composition_enabled = enabled;
            } else {
                vd_fw__composition_changed();
            }
        } break;

        case WM_PAINT: {
            if (!VD_FW_G.t_paint_ready) break;

            VD_FW_WIN32_PROFILE_BEGIN(wm_paint);

            VdFwPAINTSTRUCT ps;
            VdFwBeginPaint(hwnd, &ps);
            EnterCriticalSection(&VD_FW_G.critical_section);

            if (VD_FW_G.w != VD_FW_G.next_frame.w || VD_FW_G.h != VD_FW_G.next_frame.h) {
                VD_FW_G.next_frame.w = VD_FW_G.w;
                VD_FW_G.next_frame.h = VD_FW_G.h;
                VD_FW_G.next_frame.flags |= VD_FW_WIN32_FLAGS_SIZE_CHANGED;
            }

            VD_FW_G.next_frame.flags |= VD_FW_WIN32_FLAGS_WAKE_COND_VAR;

            WakeConditionVariable(&VD_FW_G.cond_var);
            SleepConditionVariableCS(&VD_FW_G.cond_var, &VD_FW_G.critical_section, INFINITE);
            LeaveCriticalSection(&VD_FW_G.critical_section);
            VdFwEndPaint(hwnd, &ps);

            VD_FW_WIN32_PROFILE_END(wm_paint);
        } break;

        case WM_NCPAINT: {
            if (VD_FW_G.draw_decorations) {
                result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
                break;
            }
            // @note(mdodis): Theoretically, you should be able to DwmSetWindowAttribute(hwnd,
            //                                                                           DWMWA_NCRENDERING_POLICY,
            //                                                                           &DWMNCRP_DISABLED,
            //                                                                           sizeof(DWMNCRP_DISABLED));
            //                                                                           
            // And disable the code below entirely, but in reality this makes the window very laggy when resizing
            // You really only need to do this if you don't want the Window to be alpha-composited.
            // 
            // Anyhow... setting DWM Rendering Policy to enabled and redrawing on WM_PAINT produces the best possible
            // _visual_ result the way we do it right now.
            VD_FW_WIN32_PROFILE_BEGIN(wm_ncpaint);
                // HDC hdc = GetWindowDC(hwnd);

                // if (hdc) {
                //     RECT rect;
                //     GetWindowRect(hwnd, &rect);
                //     OffsetRect(&rect, -rect.left, -rect.top); // Normalize to (0,0)

                //     // Fill the entire non-client area with black
                //     FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

                //     ReleaseDC(hwnd, hdc);
                // }
            if (VD_FW_G.composition_enabled) {
                // HDC hdc;
                // hdc = GetDCEx(hwnd, (HRGN)wparam, DCX_WINDOW|DCX_INTERSECTRGN);

                // RECT rect;
                // GetWindowRect(hwnd, &rect);

                // // Normalize to window coordinates (0,0)
                // OffsetRect(&rect, -rect.left, -rect.top);

                // // Fill the entire window area black
                // FillRect(hdc, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));

                // ReleaseDC(hwnd, hdc);
                // if (wparam == 1) {
                //     result = 0;
                // } else {
                // }
                // PAINTSTRUCT ps;
                // HDC hdc = BeginPaint(hwnd, &ps);
                // FillRect(hdc, &ps.rcPaint, (HBRUSH)GetStockObject(BLACK_BRUSH));
                result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);

                // EndPaint(hwnd, &ps);
            }
            VD_FW_WIN32_PROFILE_END(wm_ncpaint);
        } break;


        // @note(mdodis): The two messages below are handled just to send a left mouse button up message
        // When a custom chrome window is defined, DefWindowProc will consume several non client area messages regarding
        // the mouse.
        // 
        // This results in WM_NCLBUTTONUP not being sent when the left mouse button is left on a window sub-rectangle
        // for which WM_NCHITTEST returns HTCAPTION. 
        // 
        // The only reliable way I've found to handle this is by emitting left mouse button up during the following
        // messages:
        // - WM_EXITSIZEMOVE 
        // - WM_SYSCOMMAND, with wParam == 0x0000F012.
        //
        // Obviously the second message is technically part of the internal Windows API that's used to handle some part
        // of the SIZEMOVE op... but I'm not really sure if there's a better way to do it and keep the mouse interface
        // relatively simple.
        case WM_EXITSIZEMOVE: {
            if (!VD_FW_G.draw_decorations) {
                VdFw__Win32Message m;
                m.msg = VD_FW_WIN32_MESSAGE_TYPE_MOUSEBTN;
                m.dat.mousebtn.down = 0;
                m.dat.mousebtn.vkbutton = VK_LBUTTON;
                vd_fw__msgbuf_w(&m);
            }
        } break;

        case WM_SYSCOMMAND: {
            if (!VD_FW_G.draw_decorations) {
                if (wparam == 0x0000F012) {
                    VdFw__Win32Message m;
                    m.msg = VD_FW_WIN32_MESSAGE_TYPE_MOUSEBTN;
                    m.dat.mousebtn.down = 0;
                    m.dat.mousebtn.vkbutton = VK_LBUTTON;
                    vd_fw__msgbuf_w(&m);
                }
            }
            result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
        } break;

        case WM_DPICHANGED: {
            VdFwRECT *rect = (VdFwRECT*)lparam;
            VdFwSetWindowPos(hwnd, 0, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        } break;

        case WM_ERASEBKGND: {
            result = 1;
        } break;

        case WM_NCACTIVATE: {
            if (!VD_FW_G.draw_decorations) {
                // @note(mdodis): DefWindowProc doesn't repaint border if lparam == -1
                // See: https://blogs.msdn.microsoft.com/wpfsdk/2008/09/08/custom-window-chrome-in-wpf/
                result = VdFwDefWindowProc(hwnd, msg, wparam, -1);
            } else {
                result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_NCCALCSIZE: {
            if (!VD_FW_G.draw_decorations) {
                vd_fw__nccalcsize(wparam, lparam);
            } else {
                result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_NCHITTEST: {
            if (!VD_FW_G.draw_decorations) {
                result = vd_fw__hit_test(VD_FW_GET_X_LPARAM(lparam), VD_FW_GET_Y_LPARAM(lparam));
            } else {
                result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case VD_FW_WM_NCUAHDRAWCAPTION:
        case VD_FW_WM_NCUAHDRAWFRAME: {
            if (!VD_FW_G.draw_decorations) {
                result = 0;
            } else {
                result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_SETICON:
        case WM_SETTEXT: {
            if (!VD_FW_G.draw_decorations) {
                if (!VD_FW_G.composition_enabled && !VD_FW_G.theme_enabled) {
                    result = vd_fw__handle_invisible(hwnd, msg, wparam, lparam);
                } else {
                    result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
                }
            } else {
                result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_THEMECHANGED: {
            if (!VD_FW_G.draw_decorations) {
                vd_fw__theme_changed();
            }

            result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
        } break;

        // @note(mdodis): Theoretically, we _should_ handle WM_WINDOWPOSCHANGED, but we don't really need to since
        // DefWindowProc does it well enough for the window type.
        // case WM_WINDOWPOSCHANGED: {
        //     if (VD_FW_G.draw_decorations) {
        //         result = DefWindowProc(hwnd, msg, wparam, lparam);
        //     } else {
        //         vd_fw__window_pos_changed((WINDOWPOS*)lparam);
        //     }
        // } break;

        case WM_SIZE: {
            // @note(mdodis): WM_SIZE will only get called if the WM_WINDOWPOSCHANGED isn't handled by our wndproc.
            // In this case and in this case only, we already know that we're drawing default borders
            VD_FW_G.w = LOWORD(lparam);
            VD_FW_G.h = HIWORD(lparam);

            switch (wparam) {
                case SIZE_MINIMIZED: {
                    VdFw__Win32Message mm;
                    mm.msg = VD_FW_WIN32_MESSAGE_TYPE_STATECHANGE;
                    mm.dat.statechange.window_state_flag_that_changed = VD_FW_WIN32_WINDOW_STATE_MINIMIZED;
                    mm.dat.statechange.window_state_flag_value = 1;
                    vd_fw__msgbuf_w(&mm);
                } break;

                case SIZE_RESTORED: {
                    VdFw__Win32Message mm;
                    mm.msg = VD_FW_WIN32_MESSAGE_TYPE_STATECHANGE;
                    mm.dat.statechange.window_state_flag_that_changed = VD_FW_WIN32_WINDOW_STATE_MINIMIZED;
                    mm.dat.statechange.window_state_flag_value = 0;
                    vd_fw__msgbuf_w(&mm);

                    mm.msg = VD_FW_WIN32_MESSAGE_TYPE_STATECHANGE;
                    mm.dat.statechange.window_state_flag_that_changed = VD_FW_WIN32_WINDOW_STATE_MAXIMIZED;
                    mm.dat.statechange.window_state_flag_value = 0;
                    vd_fw__msgbuf_w(&mm);
                } break;

                case SIZE_MAXIMIZED: {
                    VdFw__Win32Message mm;
                    mm.msg = VD_FW_WIN32_MESSAGE_TYPE_STATECHANGE;
                    mm.dat.statechange.window_state_flag_that_changed = VD_FW_WIN32_WINDOW_STATE_MAXIMIZED;
                    mm.dat.statechange.window_state_flag_value = 1;
                    vd_fw__msgbuf_w(&mm);
                } break;

                default: break;
            }

            if ((wparam == SIZE_MAXIMIZED) || (wparam == SIZE_MINIMIZED) || (wparam == SIZE_RESTORED)) {
                // @note(mdodis): Send a mouse release event right as we go into minimized or out of maximized/minimized
                // state. This is because we'll miss the mouse release otherwise.
                VdFw__Win32Message m;
                m.msg = VD_FW_WIN32_MESSAGE_TYPE_MOUSEBTN;
                m.dat.mousebtn.down = 0;
                m.dat.mousebtn.vkbutton = VK_LBUTTON;
                vd_fw__msgbuf_w(&m);
            }
        } break;

        case WM_INPUT: {
            VdFwUINT data_size = sizeof(VdFwRAWINPUT) * VD_FW_WIN32_RAW_INPUT_BUFFER_COUNT;
            VdFwUINT num_bytes_copied = VdFwGetRawInputData(
                (VdFwHRAWINPUT)lparam,
                RID_INPUT,
                VD_FW_G.raw_input_buffer,
                &data_size,
                sizeof(VdFwRAWINPUTHEADER));

            if (num_bytes_copied == ((VdFwUINT)-1)) {
                break;
            }

            VdFwRAWINPUT *raw = VD_FW_G.raw_input_buffer;
            if (raw->header.dwType == RIM_TYPEMOUSE) {
                VdFwLONG dx = raw->data.mouse.lLastX;
                VdFwLONG dy = raw->data.mouse.lLastY;

                EnterCriticalSection(&VD_FW_G.input_critical_section);
                VD_FW_G.winthread_mouse_delta[0] = VD_FW_G.winthread_mouse_delta[0] * 0.8f + dx * 0.2f;
                VD_FW_G.winthread_mouse_delta[1] = VD_FW_G.winthread_mouse_delta[1] * 0.8f + dy * 0.2f;
                LeaveCriticalSection(&VD_FW_G.input_critical_section);
            } else if (raw->header.dwType == RIM_TYPEHID) {

                VdFw__Win32GamepadInfo *gamepad_info = 0;
                int                     gamepad_info_index = -1;
                for (int i = 0; i < VD_FW_GAMEPAD_COUNT_MAX; ++i) {
                    if (VD_FW_G.gamepad_infos[i].handle == raw->header.hDevice) {
                        gamepad_info = &VD_FW_G.gamepad_infos[i];
                        gamepad_info_index = i;
                        break;
                    }
                }

                if (gamepad_info == 0) {
                    break;
                }

                VdFw__GamepadButtonBits button_states = 0;
                VdFw__GamepadButtonBits raw_button_states = 0;
                float axes[6] = {0.f};
                float axes_raw[16] = {0};

                for (VdFwDWORD ri = 0; ri < raw->data.hid.dwCount; ++ri) {
                    VdFwBYTE *bytes = &raw->data.hid.bRawData[0] + ri * (raw->data.hid.dwSizeHid);

                    VdFwHIDP_DATA *hidp_data = gamepad_info->hidp_data;
                    VdFwULONG data_count = gamepad_info->hidp_data_cap;
                    VdFwNTSTATUS status = VdFwHidP_GetData(VdFwHidP_Input,
                                                           gamepad_info->hidp_data, &data_count,
                                                           gamepad_info->ppd,
                                                           (VdFwPCHAR)bytes, raw->data.hid.dwSizeHid);
                    if (status != VD_FW_HIDP_STATUS_SUCCESS) {
                        continue;
                    }

                    if (VD_FW_G.gamepad_raw_reports_on) {
                        for (int bdii = 0; bdii < gamepad_info->button_data_indices_len; ++bdii) {
                            int button_data_index = gamepad_info->button_data_indices[bdii];
                            VdFwHIDP_DATA *data = 0;
                            for (VdFwULONG data_index = 0; data_index < data_count; ++data_index) {
                                if (hidp_data[data_index].DataIndex == button_data_index) {
                                    data = &hidp_data[data_index];
                                    break;
                                }
                            }

                            if (!data) {
                                continue;
                            }

                            if (data->dat.On) {
                                raw_button_states |= (1ull << bdii);
                            }
                        }

                        for (int axii = 0; axii < gamepad_info->axis_data_indices_len; ++axii) {
                            int axis_data_index = gamepad_info->axis_data_indices[axii].data_index;
                            int axis_min = gamepad_info->axis_data_indices[axii].min_value;
                            int axis_max = gamepad_info->axis_data_indices[axii].max_value;

                            VdFwHIDP_DATA *data = 0;
                            for (VdFwULONG data_index = 0; data_index < data_count; ++data_index) {
                                if (hidp_data[data_index].DataIndex == axis_data_index) {
                                    data = &hidp_data[data_index];
                                    break;
                                }
                            }

                            if (!data) {
                                continue;
                            }

                            float v01 = ((float)((data->dat.RawValue - axis_min))) / 
                                        ((float)(axis_max - axis_min));
                            axes_raw[axii] = v01;
                        }
                    }

                    VdFwULONG z_value = 0;

                    // Iterate over gamepad button entries
                    for (int entry_index = 0;
                             ((entry_index < VD_FW_GAMEPAD_MAX_MAPPINGS) && 
                             !vd_fw_gamepad_map_entry_is_none(&gamepad_info->map.mappings[entry_index]));
                         ++entry_index)
                    {

                        VdFwGamepadMapEntry *entry = &gamepad_info->map.mappings[entry_index];
                        VdFwGamepadMappingSourceKind actual_kind = entry->kind & VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_MASK;

                        switch (actual_kind) {
                            case VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON: {
                                int button_data_index = gamepad_info->button_data_indices[entry->index];

                                VdFwHIDP_DATA *data = 0;
                                for (VdFwULONG data_index = 0; data_index < data_count; ++data_index) {
                                    if (hidp_data[data_index].DataIndex == button_data_index) {
                                        data = &hidp_data[data_index];
                                        break;
                                    }
                                }

                                if (!data) {
                                    continue;
                                }

                                if (data->dat.On) {
                                    if (entry->kind & VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_BUTTON_TO_AXIS) {
                                        axes[entry->target] = 1.f;
                                    } else {
                                        button_states |= (1ull << ((VdFwU64)entry->target));
                                    }
                                }
                            } break;

                            case VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS: {

                                int axis_data_index = gamepad_info->axis_data_indices[entry->index].data_index;
                                int axis_min = gamepad_info->axis_data_indices[entry->index].min_value;
                                int axis_max = gamepad_info->axis_data_indices[entry->index].max_value;

                                VdFwHIDP_DATA *data = 0;
                                for (VdFwULONG data_index = 0; data_index < data_count; ++data_index) {
                                    if (hidp_data[data_index].DataIndex == axis_data_index) {
                                        data = &hidp_data[data_index];
                                        break;
                                    }
                                }

                                if (!data) {
                                    continue;
                                }

                                if (axis_data_index != gamepad_info->z_split) {
                                    float v01 = ((float)((data->dat.RawValue - axis_min))) / 
                                                ((float)(axis_max - axis_min));

                                    if (entry->target < VD_FW_GAMEPAD_LT) {
                                        axes[entry->target] = v01 * 2.f - 1.f;
                                    } else {
                                        axes[entry->target] = v01;
                                    }
                                } else {
                                    int total_range = gamepad_info->z_split_max - gamepad_info->z_split_min;
                                    z_value = data->dat.RawValue;
                                    float value = (float)data->dat.RawValue / (float)total_range;
                                    value -= 0.5f;
                                    value *= 2.f;

                                    float lt_value = value > 0.f ? +value : 0.f;
                                    float rt_value = value < 0.f ? -value : 0.f;

                                    axes[VD_FW_GAMEPAD_LT] = lt_value;
                                    axes[VD_FW_GAMEPAD_RT] = rt_value;
                                }
                            } break;

                            case VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_HAT: {
                                VdFwU8 hat_index = (VdFwU8)(entry->index >> 8);
                                VdFwU8 hat_mask  = (VdFwU8)(entry->index & 0xFF);

                                int hat_data_index = gamepad_info->hat_data_indices[hat_index].data_index;
                                int min_value = gamepad_info->hat_data_indices[hat_index].min_value;
                                int max_value = gamepad_info->hat_data_indices[hat_index].max_value;

                                VdFwHIDP_DATA *data = 0;
                                for (VdFwULONG data_index = 0; data_index < data_count; ++data_index) {
                                    if (hidp_data[data_index].DataIndex == hat_data_index) {
                                        data = &hidp_data[data_index];
                                        break;
                                    }
                                }

                                if (!data) {
                                    continue;
                                }

                                VdFwULONG state = data->dat.RawValue;

                                // @note(mdodis): I have found no logical way (as specified by SDL's usage of gamecontrollerdb
                                // hat mappings) to _really_ use the hat mask. I guess, ranging it to 1-8 would make sense?
                                if ((min_value == 1) && (max_value == 8)) {
                                    state -= 1;
                                }

                                static const int hat_to_mask[] = {
                                    0x01,
                                    0x01 | 0x02,
                                    0x02,
                                    0x02 | 0x04,
                                    0x04,
                                    0x04 | 0x08,
                                    0x08,
                                    0x08 | 0x01,
                                    0x00,
                                };

                                if (state < (sizeof(hat_to_mask)/sizeof(hat_to_mask[0]))) {
                                    int mask = hat_to_mask[state];
                                    if (mask & hat_mask) {
                                        button_states |= (1ull << (VdFwU64)entry->target);
                                    }
                                }
                            } break;
                        }
                    }

                    // Handle XInput correlation
                    if (gamepad_info->flags & VD_FW__WIN32_GAMEPAD_FLAG_XINPUT) {
                        vd_fw__win32_correlate_xinput_triggers(gamepad_info, button_states, axes, z_value);
                    }
                }

                int index_to_write_to = gamepad_info_index;
                EnterCriticalSection(&VD_FW_G.input_critical_section);
                VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].guid       = gamepad_info->guid;
                VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].bits       = button_states;
                VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].face       = gamepad_info->map.face;
                VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].klass      = gamepad_info->map.klass;
                VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].has_rumble = 
                    (gamepad_info->map.rumble_config.type != VD_FW_GAMEPAD_RUMBLE_TYPE_NOT_AVAILABLE) ||
                    ((gamepad_info->flags >> VD_FW__WIN32_GAMEPAD_FLAG_XINPUT) & 1);

                if (VD_FW_G.gamepad_raw_reports_on) {
                    VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].raw_bits = raw_button_states;
                    VD_FW_MEMCPY(VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].raw_axes,
                                 axes_raw,
                                 16 * sizeof(float));
                }

                for (int i = 0; i < 6; ++i) {
                    VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].axes[i] = axes[i];
                }
                LeaveCriticalSection(&VD_FW_G.input_critical_section);
            }

        } break;

        case WM_INPUT_DEVICE_CHANGE: {
            VdFwHANDLE device_handle = (VdFwHANDLE)lparam;

            if (wparam == GIDC_ARRIVAL) {
                RID_DEVICE_INFO device_info;
                device_info.cbSize = sizeof(device_info);
                UINT cb_size = sizeof(device_info);
                UINT device_info_result = VdFwGetRawInputDeviceInfoA(
                    device_handle,
                    VD_FW_RIDI_DEVICEINFO,
                    &device_info,
                    &cb_size);

                if ((device_info_result) == ((UINT)-1)) {
                    // Get device info failed
                    break;
                }

                if (device_info.dwType != RIM_TYPEHID) {
                    // Device is not an HID device
                    break;
                }

                // @note(mdodis): Check if this device is an XInput compatible device
                // We do this by inspecting RIDI_DEVICENAME, and checking for TEXT("IG_")
                // See: https://learn.microsoft.com/en-us/windows/win32/xinput/xinput-and-directinput
                int is_xinput_device = 0;
                char device_instance_path[256];
                if (VD_FW_G.xinput) {
                    VdFwUINT size = sizeof(device_instance_path);
                    device_info_result = VdFwGetRawInputDeviceInfoA(
                        device_handle,
                        VD_FW_RIDI_DEVICENAME,
                        device_instance_path,
                        &size);

                    // If we can't even get the device name, then there's obviously something more wrong
                    // than just not knowing if the device is an XInput device... I think.
                    if (device_info_result == ((UINT)-1)) {
                        break;
                    }

                    for (VdFwUINT i = 0; i < size; ++i) {
                        if ((i + 2) > size) {
                            continue;
                        }

                        if (device_instance_path[i + 0] == ('I') &&
                            device_instance_path[i + 1] == ('G') &&
                            device_instance_path[i + 2] == ('_'))
                        {
                            is_xinput_device = 1;
                            break;
                        }
                    }
                }

                if (is_xinput_device) {
                    VD_FW_LOG("Discovered XInput Device");
                } else {
                    VD_FW_LOG("Discovered RAWINPUT Device");
                }

                VdFw__Win32GamepadInfo *new_gamepad = NULL;
                int new_gamepad_index = 0;
                for (int i = 0; i < VD_FW_GAMEPAD_COUNT_MAX; ++i) {
                    if (!VD_FW_G.gamepad_infos[i].connected) {
                        new_gamepad = &VD_FW_G.gamepad_infos[i];
                        new_gamepad_index = i;
                        break;
                    }
                }

                if (new_gamepad == NULL) {
                    break;
                }

                // @todo(mdodis): Cache allocated ppd data and realloc only if required size is bigger
                UINT ppd_req_size = 0;
                if (VdFwGetRawInputDeviceInfoA(
                    device_handle,
                    RIDI_PREPARSEDDATA,
                    NULL,
                    &ppd_req_size) == ((UINT)-1))
                {
                    break;
                }

                new_gamepad->ppd = (VdFwPHIDP_PREPARSED_DATA)HeapAlloc(GetProcessHeap(), 0, ppd_req_size);
                if (VdFwGetRawInputDeviceInfoA(
                    device_handle,
                    RIDI_PREPARSEDDATA,
                    new_gamepad->ppd,
                    &ppd_req_size) == ((UINT)-1))
                {
                    break;
                }

                // Construct GUID form gamepad info
                VdFwGuid guid;
                char *manufacturer_string = 0;
                char *product_string = 0;
                VdFwU16 vendor_id    = (VdFwU16)device_info.hid.dwVendorId;
                VdFwU16 product_id   = (VdFwU16)device_info.hid.dwProductId;
                VdFwU16 version      = (VdFwU16)device_info.hid.dwVersionNumber;

                // Get Manufacturer String & Product String
                {

                    HANDLE device_file = CreateFileA(device_instance_path,
                                                     GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                                     NULL, OPEN_EXISTING, 0, NULL);
                    wchar_t temp_wstring[128];
                    temp_wstring[0] = 0;

                    if (VdFwHidD_GetManufacturerString(device_file, temp_wstring, sizeof(temp_wstring))) {
                        manufacturer_string = vd_fw__utf16_to_utf8(temp_wstring);
                    }

                    temp_wstring[0] = 0;
                    if (VdFwHidD_GetProductString(device_file, temp_wstring, sizeof(temp_wstring))) {
                        product_string = vd_fw__utf16_to_utf8(temp_wstring);
                    }

                    CloseHandle(device_file);

                    new_gamepad->write_handle = CreateFileA(device_instance_path,
                                                            GENERIC_READ | GENERIC_WRITE,
                                                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                                                            NULL, OPEN_EXISTING, 0, NULL);
                }
                // Compute GUID
                guid = vd_fw__make_gamepad_guid(0x03 /* USB Bus */,
                                         vendor_id, product_id, version,
                                         manufacturer_string, product_string,
                                         'r', 0);


                // Free strings if needed
                {
                    if (manufacturer_string) {
                        vd_fw__free_mem(manufacturer_string);
                    }

                    if (product_string) {
                        vd_fw__free_mem(product_string);
                    }
                }

                new_gamepad->flags = 0;
                if (is_xinput_device) {
                    new_gamepad->flags |= VD_FW__WIN32_GAMEPAD_FLAG_XINPUT;
                }

                // Reset all gamepad specific data
                new_gamepad->handle = device_handle;
                new_gamepad->guid = guid;
                new_gamepad->connected = TRUE;
                new_gamepad->xinput_index = -1;
                new_gamepad->z_split = -1;
                VdFwULONG data_count = VdFwHidP_MaxDataListLength(VdFwHidP_Input, new_gamepad->ppd);


                // Resize HIDP_DATA buffer if needed
                new_gamepad->hidp_data = (VdFwHIDP_DATA*)vd_fw__resize_buffer(new_gamepad->hidp_data,
                                                                              sizeof(VdFwHIDP_DATA),
                                                                              data_count,
                                                                              &new_gamepad->hidp_data_cap);
                new_gamepad->hidp_data_len = data_count;


                // Map gamepad
                VdFwGamepadMap gamepad_map = {0};
                int mapped_gamepad = vd_fw__map_gamepad(guid, &gamepad_map);
                if (!mapped_gamepad) {
                    vd_fw__def_gamepad(&gamepad_map);
                }
                new_gamepad->map = gamepad_map;


                VdFwHIDP_CAPS caps;
                if (VdFwHidP_GetCaps(new_gamepad->ppd, &caps) != VD_FW_HIDP_STATUS_SUCCESS) {
                    break;
                }

                new_gamepad->output_report_size = caps.OutputReportByteLength;

                // For each button, compute the data indices. These will map to:
                // b0, b1, b2, and so on...
                // 
                // HID Button Caps:
                // - Either are specified individually, (i.e 10 buttons on controller -> 10 button caps)
                // - Or, they are specified in a range (10 buttons on controller -> 1 button cap with range 10..1)
                // So, to handle this, we iterate on the button caps, and then iterate on the range, if that caps
                // entry happens to be a range
                static VdFwHIDP_BUTTON_CAPS button_caps[32];
                VdFwUSHORT num_button_caps = caps.NumberInputButtonCaps;
                if (VdFwHidP_GetButtonCaps(VdFwHidP_Input, button_caps, &num_button_caps, new_gamepad->ppd) == VD_FW_HIDP_STATUS_SUCCESS) {
                    int count = 0;
                    // Count the total button caps
                    for (int i = 0; i < num_button_caps; ++i) {
                        if (button_caps[i].IsRange) {
                            count += 1 + button_caps[i].v.Range.DataIndexMax - button_caps[i].v.Range.DataIndexMin;
                        } else {
                            count++;
                        }
                    }

                    new_gamepad->button_data_indices = (int*)vd_fw__resize_buffer(new_gamepad->button_data_indices,
                                                                                  sizeof(new_gamepad->button_data_indices[0]),
                                                                                  count,
                                                                                  &new_gamepad->button_data_indices_cap);
                    new_gamepad->button_data_indices_len = count;

                    // Write the button data indices
                    count = 0;
                    for (int i = 0; i < num_button_caps; ++i) {
                        if (button_caps[i].IsRange) {

                            VdFwUSHORT usage_count = 1 + button_caps[i].v.Range.DataIndexMax - button_caps[i].v.Range.DataIndexMin;
                            for (VdFwUSHORT j = 0; j < usage_count; ++j) {
                                VdFwUSHORT button_data_index = button_caps[i].v.Range.DataIndexMin + j;
                                new_gamepad->button_data_indices[count++] = button_data_index;
                            }
                        } else {
                            VdFwUSHORT data_index = button_caps[i].v.NotRange.DataIndex;
                            new_gamepad->button_data_indices[count++] = data_index;
                        }
                    }
                }

                // HID Value Caps:
                // - Are only useful afaik when they're not ranges
                static VdFwHIDP_VALUE_CAPS value_caps[32];
                VdFwUSHORT num_value_caps = caps.NumberInputValueCaps;
                int num_hats = 0;
                if (VdFwHidP_GetValueCaps(VdFwHidP_Input, value_caps, &num_value_caps, new_gamepad->ppd) == VD_FW_HIDP_STATUS_SUCCESS) {

                    // Sort value caps by v.NotRange.Usage
                    for (int i = 0; i < num_value_caps - 1; ++i) {
                        for (int j = i + 1; j < num_value_caps; ++j) {
                            if (value_caps[i].v.NotRange.Usage > value_caps[j].v.NotRange.Usage) {
                                VdFwHIDP_VALUE_CAPS temp = value_caps[i];
                                value_caps[i] = value_caps[j];
                                value_caps[j] = temp;
                            }
                        }
                    }

                    for (int i = 0; i < num_value_caps; ++i) {
                        VD_FW_LOG("a%2d -> %2d, Usage: %04x Page: %04x LMin: %5d LMax:%5d", i, 
                            value_caps[i].v.NotRange.DataIndex,
                            value_caps[i].v.NotRange.Usage,
                            value_caps[i].UsagePage,
                            value_caps[i].LogicalMin,
                            value_caps[i].LogicalMax);
                    }

                    int count = 0;
                    for (int i = 0; i < num_value_caps; ++i) {
                        if (value_caps[i].IsRange) {
                            continue;
                        }

                        if (value_caps[i].v.NotRange.Usage == 0x0039 /* Hat */) {
                            num_hats++;
                            continue; 
                        }

                        if (is_xinput_device && value_caps[i].v.NotRange.Usage == 0x0032 /* Generic Z*/) {
                            new_gamepad->flags |= VD_FW__WIN32_GAMEPAD_FLAG_SPLITZ;
                            new_gamepad->z_split = value_caps[i].v.NotRange.DataIndex; 
                            new_gamepad->z_split_min = value_caps[i].LogicalMin;
                            new_gamepad->z_split_max = value_caps[i].LogicalMax;
                            if (new_gamepad->z_split_max == -1) {
                                new_gamepad->z_split_max = (1u << value_caps[i].BitSize) - 1;
                            }
                            continue;
                        }

                        count++;
                    }

                    new_gamepad->axis_data_indices = (VdFw__Win32Axis*)
                        vd_fw__resize_buffer(new_gamepad->axis_data_indices,
                                             sizeof(new_gamepad->axis_data_indices[0]),
                                             count,
                                             &new_gamepad->axis_data_indices_cap);
                    new_gamepad->axis_data_indices_len = count;

                    if (num_hats > 0) {
                        new_gamepad->hat_data_indices = (VdFw__Win32Axis*)vd_fw__resize_buffer(new_gamepad->hat_data_indices,
                                                                                               sizeof(new_gamepad->hat_data_indices[0]),
                                                                                               num_hats,
                                                                                               &new_gamepad->hat_data_indices_cap);
                        new_gamepad->hat_data_indices_len = num_hats;
                    }

                    int count_hats = 0;
                    count = 0;
                    for (int i = 0; i < num_value_caps; ++i) {
                        if (value_caps[i].IsRange) {
                            continue;
                        }

                        if (value_caps[i].v.NotRange.Usage == 0x0039 /* Hat */) {
                            int new_index = count_hats++;
                            new_gamepad->hat_data_indices[new_index].data_index = value_caps[i].v.NotRange.DataIndex;
                            new_gamepad->hat_data_indices[new_index].min_value  = value_caps[i].LogicalMin;
                            new_gamepad->hat_data_indices[new_index].max_value  = value_caps[i].LogicalMax;
                            continue; 
                        }

                        if (is_xinput_device && value_caps[i].v.NotRange.Usage == 0x0032 /* Generic Z*/) {
                            continue;
                        }

                        VdFw__Win32Axis *axis = &new_gamepad->axis_data_indices[count++];
                        axis->data_index = value_caps[i].v.NotRange.DataIndex;
                        axis->min_value  = value_caps[i].LogicalMin;
                        axis->max_value  = value_caps[i].LogicalMax;

                        if (axis->max_value == -1) {
                            axis->max_value = (1u << value_caps[i].BitSize) - 1;
                        }
                    }

                }
                VD_FW_LOG("Buttons: %d, Axes: %d, ZSplit: %d", new_gamepad->button_data_indices_len, new_gamepad->axis_data_indices_len, new_gamepad->z_split);
                for (int i = 0; i < new_gamepad->axis_data_indices_len; ++i) {
                    VD_FW_LOG("a%d -> %d", i, new_gamepad->axis_data_indices[i].data_index);
                }

                for (int entry_index = 0;
                         (entry_index < VD_FW_GAMEPAD_MAX_MAPPINGS) && 
                         !vd_fw_gamepad_map_entry_is_none(&new_gamepad->map.mappings[entry_index]);
                     ++entry_index)
                {
                    VdFwGamepadMapEntry *entry = &new_gamepad->map.mappings[entry_index];
                    VdFwGamepadMappingSourceKind actual_kind = entry->kind & VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_MASK; 
                    if (actual_kind == VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS) {
                        VD_FW_LOG("Axis[%d] (=%d): vd_fw_axis_%d [%d %d]",
                            entry->index,
                            new_gamepad->axis_data_indices[entry->index].data_index,
                            entry->target,
                            new_gamepad->axis_data_indices[entry->index].min_value,
                            new_gamepad->axis_data_indices[entry->index].max_value);
                    }
                }


                int index_to_write_to = new_gamepad_index;
                EnterCriticalSection(&VD_FW_G.input_critical_section);
                VD_FW_G.winthread_num_gamepads_present++;
                VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].guid  = new_gamepad->guid;
                VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].bits  = 0;
                VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].face  = new_gamepad->map.face;
                VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].klass = new_gamepad->map.klass;
                VD_FW_G.winthread_gamepad_curr_states[index_to_write_to].has_rumble = 
                    (new_gamepad->map.rumble_config.type != VD_FW_GAMEPAD_RUMBLE_TYPE_NOT_AVAILABLE) ||
                    ((new_gamepad->flags >> VD_FW__WIN32_GAMEPAD_FLAG_XINPUT) & 1);

                LeaveCriticalSection(&VD_FW_G.input_critical_section);

            } else {

                int disconnected_gamepad_index = -1;
                VdFw__Win32GamepadInfo *disconnected_gamepad = NULL;
                for (int i = 0; i < VD_FW_GAMEPAD_COUNT_MAX; ++i) {
                    if (VD_FW_G.gamepad_infos[i].handle == device_handle) {
                        disconnected_gamepad = &VD_FW_G.gamepad_infos[i];
                        disconnected_gamepad_index = i;
                        break;
                    }
                }

                if (disconnected_gamepad == NULL) {
                    break;
                }

                CloseHandle(disconnected_gamepad->write_handle);

                // Move all gamepad infos after disconnected_gamepad_index to index -1
                {
                    for (int i = disconnected_gamepad_index; i < (VD_FW_G.winthread_num_gamepads_present - 1); ++i) {
                        VD_FW_G.gamepad_infos[i] = VD_FW_G.gamepad_infos[i + 1];
                    }
                }

                VD_FW_MEMSET(&VD_FW_G.gamepad_infos[VD_FW_G.winthread_num_gamepads_present - 1], 0, sizeof(VD_FW_G.gamepad_infos[0]));

                VD_FW_LOG("Gamepad Disconnected");
                VD_FW_G.winthread_num_gamepads_present--;

                for (int i = 0; i < VD_FW_GAMEPAD_COUNT_MAX; ++i) {
                    // Reset correlation from xinput
                    // This is done because we can't know if XInput decided to map the dwUserIndex to other devices
                    // which usually happens if, for example, player 2/3 disconnects and reconnects.
                    VD_FW_G.gamepad_infos[i].xinput_index = -1;
                }
            }

        } break;

        case WM_SYSKEYDOWN: {
            if (!VD_FW_G.draw_decorations) {

                if (wparam == VK_F4) {
                    OutputDebugStringA("SYSKEY\n");
                    VdFwPostMessage(hwnd, WM_CLOSE, 0, 0);
                }
            } else {
                result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
            }

        } break;

        case WM_KEYUP:
        case WM_KEYDOWN: {

            WORD vkcode = LOWORD(wparam);
            
            WORD keyflags = HIWORD(lparam);

            WORD scancode = LOBYTE(keyflags);                             // scan code
            BOOL isextendedkey = (keyflags & KF_EXTENDED) == KF_EXTENDED; // extended-key flag, 1 if scancode has 0xE0 prefix
            
            if (isextendedkey)
                scancode = MAKEWORD(scancode, 0xE0);

            // if we want to distinguish these keys:
            switch (vkcode)
            {
                case VK_SHIFT:   // converts to VK_LSHIFT or VK_RSHIFT
                case VK_CONTROL: // converts to VK_LCONTROL or VK_RCONTROL
                case VK_MENU:    // converts to VK_LMENU or VK_RMENU
                    vkcode = LOWORD(VdFwMapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX));
                    break;
            }

            int is_down = msg == WM_KEYDOWN;

            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_KEYSTATE;
            m.dat.mousebtn.down   = is_down;
            m.dat.keystate.vkcode = vkcode;
            vd_fw__msgbuf_w(&m);

            result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
        } break;

        case WM_UNICHAR: {
            result = 1;
            if (wparam == UNICODE_NOCHAR) {
                break;
            }

            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_CHAR;
            m.dat.character.codepoint = (VdFwU32)wparam;
            vd_fw__msgbuf_w(&m);
        } break;

        case WM_CHAR: {
            VdFwU32 codepoint = 0;
            int send_message = 1;
            if (IS_HIGH_SURROGATE(wparam)) {
                VD_FW_G.char_surrogate_hi = (VdFwWCHAR)wparam;
                send_message = 0;
            } else if (IS_SURROGATE_PAIR(VD_FW_G.char_surrogate_hi, wparam)) {
                VdFwU32 lo = (VdFwU32)wparam;
                VdFwU32 hi = (VdFwU32)VD_FW_G.char_surrogate_hi;
                codepoint = 0x10000 + (((hi - 0xD800) << 10) | (lo - 0xDC00));
                send_message = 2;
                VD_FW_G.char_surrogate_hi = 0;
            } else {
                codepoint = (VdFwU32)wparam;
            }

            if (!send_message) {
                break;
            }

            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_CHAR;
            m.dat.character.codepoint = codepoint;
            vd_fw__msgbuf_w(&m);
        } break;

        case WM_INPUTLANGCHANGE: {
            vd_fw__update_kb_codepage();
        } break;

        case WM_XBUTTONUP:
        case WM_XBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDOWN: {
            int down = 0;
            DWORD code = 0;
            DWORD hiword = HIWORD(wparam);

            switch (msg) {

                case WM_MBUTTONUP:    down = 0; code = VK_MBUTTON; break;
                case WM_MBUTTONDOWN:  down = 1; code = VK_MBUTTON; break;
                case WM_RBUTTONUP:    down = 0; code = VK_RBUTTON; break;
                case WM_RBUTTONDOWN:  down = 1; code = VK_RBUTTON; break;
                case WM_LBUTTONUP:    down = 0; code = VK_LBUTTON; break;
                case WM_LBUTTONDOWN:  down = 1; code = VK_LBUTTON; break;
                case WM_XBUTTONUP:    down = 0; code = VK_XBUTTON1 + hiword - 1; break;
                case WM_XBUTTONDOWN:  down = 1; code = VK_XBUTTON1 + hiword - 1; break;
                default: break;
            }

            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_MOUSEBTN;
            m.dat.mousebtn.down = down;
            m.dat.mousebtn.vkbutton = code;
            vd_fw__msgbuf_w(&m);
        } break;

        case WM_NCMBUTTONUP:
        case WM_NCMBUTTONDOWN:
        case WM_NCRBUTTONUP:
        case WM_NCRBUTTONDOWN:
        case WM_NCLBUTTONUP:
        case WM_NCLBUTTONDOWN: {

            if (!VD_FW_G.draw_decorations) {
                int down = 0;
                VdFwDWORD code = 0;

                switch (msg) {

                    case WM_NCMBUTTONUP:    down = 0; code = VK_MBUTTON; break;
                    case WM_NCMBUTTONDOWN:  down = 1; code = VK_MBUTTON; break;
                    case WM_NCRBUTTONUP:    down = 0; code = VK_RBUTTON; break;
                    case WM_NCRBUTTONDOWN:  down = 1; code = VK_RBUTTON; break;
                    case WM_NCLBUTTONUP:    down = 0; code = VK_LBUTTON; break;
                    case WM_NCLBUTTONDOWN:  down = 1; code = VK_LBUTTON; break;
                    default: break;
                }

                VdFw__Win32Message m;
                m.msg = VD_FW_WIN32_MESSAGE_TYPE_MOUSEBTN;
                m.dat.mousebtn.down = down;
                m.dat.mousebtn.vkbutton = code;
                vd_fw__msgbuf_w(&m);
            }

            result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
        } break;

        case WM_SETFOCUS:
        case WM_KILLFOCUS: {
            int got_focus = msg == WM_SETFOCUS;
            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_CHANGEFOCUS;
            m.dat.changefocus.got_focus = got_focus;
            vd_fw__msgbuf_w(&m);
        } break;

        // case WM_NCMOUSEMOVE: {
        //     if (!VD_FW_G.draw_decorations) {
        //         int x = GET_X_LPARAM(lparam);
        //         int y = GET_Y_LPARAM(lparam);

        //         POINT p = {x, y};
        //         ScreenToClient(VD_FW_G.hwnd, &p);

        //         VdFw__Win32Message m;
        //         m.msg = VD_FW_WIN32_MESSAGE_TYPE_MOUSEMOVE;
        //         m.dat.mousemove.mx = p.x;
        //         m.dat.mousemove.my = p.y;
        //         vd_fw__msgbuf_w(&m);
        //     }

        //     // result = DefWindowProc(hwnd, msg, wparam, lparam);
        // } break;

        case WM_NCMOUSEMOVE:
        case WM_MOUSEMOVE: {

            int x = VD_FW_GET_X_LPARAM(lparam);
            int y = VD_FW_GET_Y_LPARAM(lparam);

            if (msg == WM_NCMOUSEMOVE) {
                if (!VD_FW_G.receive_ncmouse_on && VD_FW_G.nccaption_set) {
                    break;
                }

                VdFwRECT rect;
                VdFwGetWindowRect(hwnd, &rect);

                x -= rect.left;
                y -= rect.top;
            }

            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_MOUSEMOVE;
            m.dat.mousemove.mx = x;
            m.dat.mousemove.my = y;
            vd_fw__msgbuf_w(&m);
        } break;

        case VD_FW_WIN32_SHOW_CURSOR: {
            VdFwBOOL should_show = (VdFwBOOL)wparam;
            VdFwShowCursor(should_show);
        } break;

        case VD_FW_WIN32_FULLSCREEN: {
            VD_FW_WIN32_PROFILE_BEGIN(fw_fullscreen);
            VdFwBOOL should_be_fullscreen = (VdFwBOOL)lparam;

            if (should_be_fullscreen) {

                VdFwGetWindowPlacement(VD_FW_G.hwnd, &VD_FW_G.last_window_placement);

                VdFwHMONITOR monitor = VdFwMonitorFromWindow(VD_FW_G.hwnd, MONITOR_DEFAULTTOPRIMARY);
                VdFwMONITORINFO monitor_info = {0};
                monitor_info.cbSize = sizeof(monitor_info);
                VD_FW__CHECK_NONZERO(VdFwGetMonitorInfo(monitor, &monitor_info));

                VdFwLONG style = VD_FW_G.last_window_style & ~(WS_OVERLAPPEDWINDOW);
                style |= WS_VISIBLE;

                VdFwSetWindowLong(VD_FW_G.hwnd, GWL_STYLE, style);
                VdFwSetWindowPos(VD_FW_G.hwnd, VD_FW_HWND_TOP,
                             monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
                             monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
                             monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
                             SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            } else {
                VdFwSetWindowLong(VD_FW_G.hwnd, GWL_STYLE, VD_FW_G.last_window_style);
                VdFwSetWindowPlacement(VD_FW_G.hwnd, &VD_FW_G.last_window_placement);
                VdFwSetWindowPos(VD_FW_G.hwnd, NULL,
                             0,
                             0,
                             0,
                             0,
                             SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
                             SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            }

            VD_FW_WIN32_PROFILE_END(fw_fullscreen);
        } break;

        case VD_FW_WIN32_SIZE: {
            VdFwWORD width  = LOWORD(lparam);
            VdFwWORD height = HIWORD(lparam);

            VdFwRECT rect;
            VdFwGetWindowRect(VD_FW_G.hwnd, &rect);

            VdFwRECT newrect;
            newrect.left = rect.left;
            newrect.right = rect.left + width;
            newrect.top = rect.top;
            newrect.bottom = rect.top + height;

            VdFwSetWindowPos(VD_FW_G.hwnd, VD_FW_HWND_TOP,
                         newrect.left, newrect.top,
                         newrect.right - newrect.left,
                         newrect.bottom - newrect.top,
                         SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

        } break;

        case VD_FW_WIN32_SIZEMIN:
        case VD_FW_WIN32_SIZEMAX: {
            VdFwDWORD width  = LOWORD(lparam);
            VdFwDWORD height = HIWORD(lparam);
            if (msg == VD_FW_WIN32_SIZEMIN) {
                VD_FW_G.window_min[0] = width;
                VD_FW_G.window_min[1] = height;
            } else {
                VD_FW_G.window_max[0] = width;
                VD_FW_G.window_max[1] = height;
            }
        } break;

        case VD_FW_WIN32_GAMEPADRMBREQ: {
            VdFwWORD lo = LOWORD(lparam);
            VdFwWORD hi = HIWORD(lparam);
            VdFw__Win32GamepadInfo *gamepad_info = &VD_FW_G.gamepad_infos[wparam];
            gamepad_info->rumble_state.rumble_lo = (float)lo / 65535.f;
            gamepad_info->rumble_state.rumble_hi = (float)hi / 65535.f;

            if (VD_FW_G.rumble_timer_handle == 0) {
                VD_FW_G.rumble_timer_handle = VdFwSetTimer(VD_FW_G.hwnd, 1, 100, NULL);
            }

        } break;

        case VD_FW_WIN32_GAMEPADDBCH: {
            for (int i = 0; i < VD_FW_G.num_gamepads_present; ++i) {
                VdFw__Win32GamepadInfo *gamepad_info = &VD_FW_G.gamepad_infos[i];
                vd_fw__map_gamepad(gamepad_info->guid, &gamepad_info->map);
            }
        } break;

        case VD_FW_WIN32_GAMEPADRAWRQ: {
            VD_FW_G.gamepad_raw_reports_on = (int)wparam;
        } break;

        case VD_FW_WIN32_KILL: {
            VdFwDestroyWindow(VD_FW_G.hwnd);
        } break;

        case WM_TIMER: {
            int timer_should_stop = 1;

            for (int i = 0; i < VD_FW_G.winthread_num_gamepads_present; ++i) {
                VdFw__Win32GamepadInfo *gamepad_info = &VD_FW_G.gamepad_infos[i];
                VdFwGamepadRumbleConfig *rumble_config = &gamepad_info->map.rumble_config;
                VdFwU8 rumble_type = rumble_config->type;

                if (gamepad_info->xinput_index != -1) {
                    // Gamepad is correlated. We switch to XInput
                    rumble_type = VD_FW_GAMEPAD_RUMBLE_TYPE_XINPUT;
                }

                switch (rumble_type) {
                    case VD_FW_GAMEPAD_RUMBLE_TYPE_RAW: {
                        VD_FW_G.report_buffer = (VdFwU8*)vd_fw__resize_buffer(VD_FW_G.report_buffer,
                                                                                sizeof(VD_FW_G.report_buffer[0]),
                                                                                gamepad_info->output_report_size,
                                                                                &VD_FW_G.report_buffer_len);
                        VD_FW_MEMSET(VD_FW_G.report_buffer, 0, sizeof(VD_FW_G.report_buffer[0]) * gamepad_info->output_report_size);
                        for (int j = 0; j < gamepad_info->map.rumble_config.prefix_len; ++j) {
                            VD_FW_G.report_buffer[j] = gamepad_info->map.rumble_config.prefix[j];
                        }

                        VD_FW_G.report_buffer[gamepad_info->map.rumble_config.dat.raw.rumble_lo.parts.offset] = (VdFwU8)(gamepad_info->rumble_state.rumble_lo * 255.f);
                        VD_FW_G.report_buffer[gamepad_info->map.rumble_config.dat.raw.rumble_hi.parts.offset] = (VdFwU8)(gamepad_info->rumble_state.rumble_hi * 255.f);
                        DWORD num_written = 0;
                        if (!WriteFile(gamepad_info->write_handle,
                                       VD_FW_G.report_buffer,
                                       gamepad_info->output_report_size,
                                       &num_written,
                                       NULL))
                        {
                            VD_FW_LOG("Failed to send report: %d", GetLastError());
                        }

                    } break;

                    case VD_FW_GAMEPAD_RUMBLE_TYPE_XINPUT: {

                        VdFwXINPUT_VIBRATION vib;
                        vib.wLeftMotorSpeed  = (VdFwWORD)(gamepad_info->rumble_state.rumble_lo * 65535.f);
                        vib.wRightMotorSpeed = (VdFwWORD)(gamepad_info->rumble_state.rumble_hi * 65535.f);
                        VdFwXInputSetState(gamepad_info->xinput_index, &vib);

                    } break;

                    default: continue;
                }

                if ((gamepad_info->rumble_state.rumble_lo != 0) || (gamepad_info->rumble_state.rumble_hi != 0)) {
                    timer_should_stop = 0;
                }
            }

            if (timer_should_stop) {
                VdFwKillTimer(VD_FW_G.hwnd, VD_FW_G.rumble_timer_handle);
                VD_FW_LOG("Killing rumble timer");
                VD_FW_G.rumble_timer_handle = 0;
            }
        } break;

        case WM_GETMINMAXINFO: {
            MINMAXINFO *min_max_info = (MINMAXINFO*)lparam;
            min_max_info->ptMinTrackSize.x = VD_FW_G.window_min[0] <= 0 ? VD_FW_G.def_window_min[0] : VD_FW_G.window_min[0];
            min_max_info->ptMinTrackSize.y = VD_FW_G.window_min[1] <= 0 ? VD_FW_G.def_window_min[1] : VD_FW_G.window_min[1];
            min_max_info->ptMaxTrackSize.x = VD_FW_G.window_max[0] <= 0 ? 0x7FFFFFFF : VD_FW_G.window_max[0];
            min_max_info->ptMaxTrackSize.y = VD_FW_G.window_max[1] <= 0 ? 0x7FFFFFFF : VD_FW_G.window_max[1];
        } break;

        case VD_FW_WIN32_UPDATE_TITLE: {
            VdFwSetWindowTextA(VD_FW_G.hwnd, VD_FW_G.title);
        } break;

        case WM_MOUSEHWHEEL: {
            if (!VD_FW_G.t_paint_ready) { result = VdFwDefWindowProc(hwnd, msg, wparam, lparam); break; }
            int delta = GET_WHEEL_DELTA_WPARAM(wparam);
            float dx = (float)delta / (float)WHEEL_DELTA;
            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_SCROLL;
            m.dat.scroll.dx = dx;
            m.dat.scroll.dy = 0.f;
            vd_fw__msgbuf_w(&m);
        } break;

        case WM_MOUSEWHEEL: {
            if (!VD_FW_G.t_paint_ready) { result = VdFwDefWindowProc(hwnd, msg, wparam, lparam); break; }
            int delta = GET_WHEEL_DELTA_WPARAM(wparam);
            float dy = (float)delta / (float)WHEEL_DELTA;
            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_SCROLL;
            m.dat.scroll.dx = 0.f;
            m.dat.scroll.dy = dy;
            vd_fw__msgbuf_w(&m);
        } break;

        default: {
            result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
        } break;
    }    
    return result;
}

static int vd_fw__msgbuf_r(VdFw__Win32Message *message)
{
    LONG r = VD_FW_G.msgbuf_r;
    LONG w = InterlockedCompareExchange(&VD_FW_G.msgbuf_w, VD_FW_G.msgbuf_w, VD_FW_G.msgbuf_w);

    MemoryBarrier();

    if (r == w) {
        return 0;
    }

    *message = VD_FW_G.msgbuf[r];

    LONG nr = (r + 1) % VD_FW_WIN32_MESSAGE_BUFFER_SIZE;
    InterlockedExchange(&VD_FW_G.msgbuf_r, nr);

    return 1;
}

static int vd_fw__msgbuf_w(VdFw__Win32Message *message)
{
    LONG w = VD_FW_G.msgbuf_w;
    LONG r = InterlockedCompareExchange(&VD_FW_G.msgbuf_r, VD_FW_G.msgbuf_r, VD_FW_G.msgbuf_r);

    MemoryBarrier();

    if ((w + 1) % VD_FW_WIN32_MESSAGE_BUFFER_SIZE == r) {
        return 0;
    }

    VD_FW_G.msgbuf[w] = *message;
    LONG nw = (w + 1) % VD_FW_WIN32_MESSAGE_BUFFER_SIZE;
    InterlockedExchange(&VD_FW_G.msgbuf_w, nw);

    return 1;
}

static char *vd_fw__utf16_to_utf8(const wchar_t *ws)
{
    int req = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                                  ws, -1,
                                  0, 0,
                                  NULL, NULL);
    if (req <= 0) {
        return 0;
    }

    char *data = (char*)vd_fw__realloc_mem(0, req + 1);

    int wrt = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS,
                                  ws, -1,
                                  data, req,
                                  NULL, NULL);

    if (wrt == 0) {
        vd_fw__free_mem(data);
        return 0;
    }

    data[req] = 0;
    return data;
}

static void vd_fw__update_kb_codepage(void)
{
    VdFwHKL keyboard_layout = VdFwGetKeyboardLayout(0);

    LCID keyboard_lcid = MAKELCID(HIWORD(keyboard_layout), SORT_DEFAULT);
    if (GetLocaleInfoA(keyboard_lcid, (LOCALE_RETURN_NUMBER | LOCALE_IDEFAULTANSICODEPAGE), (LPSTR)&VD_FW_G.kb_codepage, sizeof(VD_FW_G.kb_codepage)) == 0) {
        VD_FW_G.kb_codepage = CP_ACP;
    }
}

#if VD_FW_NO_CRT
#pragma function(memset)
void *__cdecl memset(void *dest, int value, size_t num)
{
    __stosb((unsigned char *)dest, (unsigned char)value, num);
    return dest;
}

#pragma function(memcpy)
void *__cdecl memcpy(void *dest, void *src, size_t num)
{
    __movsb((unsigned char *)dest, (unsigned const char*)src, num);
    return dest;
}
int _fltused;
#ifdef _M_IX86

    // float to int64 cast
    // on /arch:IA32
    // on /arch:SSE
    // on /arch:SSE2 with /d2noftol3 compiler switch
    __declspec(naked) void _ftol2()
    {
        __asm
        {
            fistp qword ptr[esp - 8]
            mov   edx, [esp - 4]
            mov   eax, [esp - 8]
            ret
        }
    }

    // float to int64 cast on /arch:IA32
    __declspec(naked) void _ftol2_sse()
    {
        __asm
        {
            fistp dword ptr[esp - 4]
            mov   eax, [esp - 4]
            ret
        }
    }

    // float to uint32 cast on / arch:SSE2
    __declspec(naked) void _ftoui3()
    {

    }

    // float to int64 cast on / arch:SSE2
    __declspec(naked) void _ftol3()
    {
        
    }

    // float to uint64 cast on / arch:SSE2
    __declspec(naked) void _ftoul3()
    {

    }

    // int64 to float cast on / arch:SSE2
    __declspec(naked) void _ltod3()
    {

    }

    // uint64 to float cast on / arch:SSE2
    __declspec(naked) void _ultod3()
    {

    }

#endif

#if VD_FW_WIN32_SUBSYSTEM == VD_FW_WIN32_SUBSYSTEM_CONSOLE
int mainCRTStartup(void)
{
    int result = main(0, 0);
    ExitProcess(result);
}
#else
LRESULT WinMainCRTStartup(void)
{
    int result = main(0, 0);
    ExitProcess(result);
}
#endif // VD_FW_WIN32_SUBSYSTEM == VD_FW_WIN32_SUBSYSTEM_CONSOLE
#else
int wWinMain(HINSTANCE hinstance, HINSTANCE prev_instance, LPWSTR cmdline, int nshowcmd)
{
    (void)(hinstance);
    (void)(prev_instance);
    (void)(cmdline);
    (void)(nshowcmd);
    int result = main(0, 0);
    ExitProcess(result);
}
#endif // VD_FW_NO_CRT

#elif defined(__APPLE__)
#import <AppKit/AppKit.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <QuartzCore/CVDisplayLink.h>
#import <os/log.h>
#import <mach/mach_time.h>
#import <sys/types.h>
#import <sys/stat.h>
#import <fcntl.h>
#import <unistd.h>
#import <stdlib.h>
#import <stdio.h>
#import <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>

#define VD_FW_G Vd_Fw_Globals

@interface VdFwWindowDelegate : NSObject<NSApplicationDelegate, NSWindowDelegate>
- (void)updateGLContext;
@end
@interface VdFwContentView : NSView
@end
@interface VdFwWindow : NSWindow
@end

typedef enum {
    VD_FW__MAC_FLAGS_WAKE_COND_VAR = 1 << 0,
    VD_FW__MAC_FLAGS_SIZE_CHANGED  = 1 << 1,
    VD_FW__MAC_FLAGS_REACQUIRE_CONTEXT = 1 << 2,

    VD_FW_MAC_MESSAGE_BUFFER_SIZE = 256,

    VD_FW__MAC_WINDOW_STATE_MINIMIZED = 1 << 0,
    VD_FW__MAC_WINDOW_STATE_ZOOMED = 1 << 1,

    VD_FW__MAC_MESSAGE_INVALID = 0,
    VD_FW__MAC_MESSAGE_MOUSEMOVE,
    VD_FW__MAC_MESSAGE_MOUSEBTN,
    VD_FW__MAC_MESSAGE_SCROLL,
    VD_FW__MAC_MESSAGE_KEY,
    VD_FW__MAC_MESSAGE_MINIMIZED,
    VD_FW__MAC_MESSAGE_ZOOMED,
    VD_FW__MAC_MESSAGE_CLOSE_REQUEST,
} VdFw__MacMessageType;

typedef struct {
    VdFw__MacMessageType type;
    union {
        struct {
            VdFwI32 mx, my;
            float   dx, dy;
        } mousemove;

        struct {
            int down;
            int mask; 
        } mousebtn;

        struct {
            float sx, sy;
        } scroll;

        struct {
            int down;
            int key;
        } key;

        struct {
            int on;
        } minimized;

        struct {
            int on;
        } zoomed;
    } dat;
} VdFw__MacMessage;

typedef struct {
    int w, h;
    int flags;
} VdFw__MacFrame;

typedef struct {
    NSOpenGLContext             *gl_context;
    BOOL                        should_close;
    mach_timebase_info_data_t   time_base;
    VdFwContentView             *content_view;
    uint64_t                    last_time;
    CGFloat                     scale;
    int                         wheel_moved;
    float                       wheel[2];
    NSPoint                     drag_start_location;
    NSPoint                     drag_start_pos_window_coords;
    BOOL                        dragging;
    NSPoint                     last_mouse;
    float                       mouse_delta[2];
    unsigned char               curr_key_states[VD_FW_KEY_MAX];
    unsigned char               prev_key_states[VD_FW_KEY_MAX];
    int                         focus_changed;
    int                         focused;
    int                         argc;
    const char                  **argv;

/* ----RENDER THREAD ONLY-------------------------------------------------------------------------------------------- */
    VdFwI32                     mouse[2];
    VdFwI32                     prev_mouse_state;
    VdFwI32                     mouse_state;
    BOOL                        mouse_is_locked;
    int                         window_state;
    int                         window_state_changed;
    int                         close_request;

/* ----WINDOW THREAD ONLY-------------------------------------------------------------------------------------------- */
    BOOL                        draw_decorations;
    VdFwInitInfo                c_init_info;
    VdFwWindow                  *window;
    int                         context_needs_update;
    BOOL                        is_zoomed;
    int                         cap_gamepad_db_entries;
    int                         num_gamepad_db_entries;
    VdFwGamepadDBEntry          *gamepad_db_entries;


/* ----MAIN - RENDER THREAD DATA------------------------------------------------------------------------------------- */
    int                         w, h;
    VdFw__MacFrame              next_frame;
    VdFw__MacFrame              curr_frame;

    VdFw__MacMessage            msgbuf[VD_FW_MAC_MESSAGE_BUFFER_SIZE];
    volatile VdFwI32            msgbuf_r;
    volatile VdFwI32            msgbuf_w;
    NSRect                      nccaption;
    int                         nccaption_set;
    int                         ncrect_count;
    NSRect                      ncrects[VD_FW_NCRECTS_MAX];
    NSImage                     *app_image;
    int                         has_initialized;

/* ----MAIN - RENDER THREAD SYNC------------------------------------------------------------------------------------- */
    pthread_t                   main_thread;
    pthread_mutex_t             m_paint;
    pthread_mutex_t             m_input;
    pthread_cond_t              n_paint;
    sem_t                       *s_main_thread_opened_me;
    sem_t                       *s_main_thread_window_ready;
    sem_t                       *s_main_thread_window_closed;
    sem_t                       *s_main_thread_context_needs_update;
    int                         context_update_requested;
} VdFw__MacOsInternalData;

static int vd_fw__msgbuf_r(VdFw__MacMessage *message);
static int vd_fw__msgbuf_w(VdFw__MacMessage *message);

static VdFw__MacOsInternalData Vd_Fw_Globals;

static VdFwKey vd_fw__translate_mac_keycode(unsigned short keycode)
{
                    // Escape: F17: 53
                    // Up: 126
                    // Down: 125
                    // Right: 124
                    // Left: 123
                    // 
    static VdFwKey translation_table[280] = {
        VD_FW_KEY_A,                 // 0,     KEY_A
        VD_FW_KEY_S,                 // 1,     KEY_S
        VD_FW_KEY_D,                 // 2,     KEY_D
        VD_FW_KEY_F,                 // 3,     KEY_F
        VD_FW_KEY_H,                 // 4,     KEY_H
        VD_FW_KEY_G,                 // 5,     KEY_G
        VD_FW_KEY_Z,                 // 6,     KEY_Z
        VD_FW_KEY_X,                 // 7,     KEY_X
        VD_FW_KEY_C,                 // 8,     KEY_C
        VD_FW_KEY_V,                 // 9,     KEY_V
        VD_FW_KEY_UNKNOWN,           // 10,    -----
        VD_FW_KEY_B,                 // 11,    KEY_B
        VD_FW_KEY_Q,                 // 12,    KEY_Q
        VD_FW_KEY_W,                 // 13,    KEY_W
        VD_FW_KEY_E,                 // 14,    KEY_E
        VD_FW_KEY_R,                 // 15,    KEY_R
        VD_FW_KEY_Y,                 // 16,    KEY_Y
        VD_FW_KEY_T,                 // 17,    KEY_T
        VD_FW_KEY_1,                 // 18,    KEY_1
        VD_FW_KEY_2,                 // 19,    KEY_2
        VD_FW_KEY_3,                 // 20,    KEY_3
        VD_FW_KEY_4,                 // 21,    KEY_4
        VD_FW_KEY_6,                 // 22,    KEY_6
        VD_FW_KEY_5,                 // 23,    KEY_5
        VD_FW_KEY_EQUALS,            // 24,    KEY_EQUAL
        VD_FW_KEY_9,                 // 25,    KEY_9
        VD_FW_KEY_7,                 // 26,    KEY_7
        VD_FW_KEY_MINUS,             // 27,    KEY_MINUS
        VD_FW_KEY_8,                 // 28,    KEY_8
        VD_FW_KEY_0,                 // 29,    KEY_0
        VD_FW_KEY_BRACKET_CLOSE,     // 30,    KEY_CLOSE_BRACE/KEY_CLOSE_BRACKET
        VD_FW_KEY_O,                 // 31,    KEY_O
        VD_FW_KEY_U,                 // 32,    KEY_U
        VD_FW_KEY_BRACKET_OPEN,      // 33,    KEY_OPEN_BRACE/KEY_OPEN_BRACKET
        VD_FW_KEY_I,                 // 34,    KEY_I
        VD_FW_KEY_P,                 // 35,    KEY_P
        VD_FW_KEY_ENTER,             // 36,    KEY_ENTER
        VD_FW_KEY_L,                 // 37,    KEY_L
        VD_FW_KEY_J,                 // 38,    KEY_J
        VD_FW_KEY_QUOTE,             // 39,    KEY_DOUBLE_QUOTES/KEY_SIMPLE_QUOTE
        VD_FW_KEY_K,                 // 40,    KEY_K
        VD_FW_KEY_SEMICOLON,         // 41,    KEY_COLON/KEY_SEMI_COLON
        VD_FW_KEY_SLASH_BACK,        // 42,    KEY_BACKSLASH/KEY_PIPE
        VD_FW_KEY_COMMA,             // 43,    KEY_COMMA/KEY_LESS_THAN
        VD_FW_KEY_SLASH_FORWARD,     // 44,    KEY_QUESTION_MARK/KEY_SLASH
        VD_FW_KEY_N,                 // 45,    KEY_N
        VD_FW_KEY_M,                 // 46,    KEY_M
        VD_FW_KEY_DOT,               // 47,    KEY_DOT/KEY_GREATER_THAN
        VD_FW_KEY_TAB,               // 48,    KEY_TAB
        VD_FW_KEY_SPACE,             // 49,    KEY_SPACEBAR
        VD_FW_KEY_BACKTICK,          // 50,    KEY_TILDE
        VD_FW_KEY_BACKSPACE,         // 51,    KEY_BACKSPACE
        VD_FW_KEY_UNKNOWN,           // 52,    ----
        VD_FW_KEY_ESCAPE,            // 53,    KEY_ESCAPE
        VD_FW_KEY_UNKNOWN,           // 54,    ----
        VD_FW_KEY_UNKNOWN,           // 55,    ----
        VD_FW_KEY_UNKNOWN,           // 56,    ----
        VD_FW_KEY_UNKNOWN,           // 57,    ----
        VD_FW_KEY_UNKNOWN,           // 58,    ----
        VD_FW_KEY_UNKNOWN,           // 59,    ----
        VD_FW_KEY_UNKNOWN,           // 60,    ----
        VD_FW_KEY_UNKNOWN,           // 61,    ----
        VD_FW_KEY_UNKNOWN,           // 62,    ----
        VD_FW_KEY_UNKNOWN,           // 63,    ----
        VD_FW_KEY_F17,               // 64,    KEY_F17
        VD_FW_KEY_UNKNOWN,           // 65,    KEY_PAD_DOT
        VD_FW_KEY_UNKNOWN,           // 66,    ----
        VD_FW_KEY_UNKNOWN,           // 67,    KEY_PAD_MULTIPLY
        VD_FW_KEY_UNKNOWN,           // 68,    ----
        VD_FW_KEY_UNKNOWN,           // 69,    KEY_PAD_ADD
        VD_FW_KEY_UNKNOWN,           // 70,    ----
        VD_FW_KEY_UNKNOWN,           // 71,    KEY_CLEAR
        VD_FW_KEY_UNKNOWN,           // 72,    ----
        VD_FW_KEY_UNKNOWN,           // 73,    ----
        VD_FW_KEY_UNKNOWN,           // 74,    ----
        VD_FW_KEY_UNKNOWN,           // 75,    KEY_PAD_DIVIDE
        VD_FW_KEY_UNKNOWN,           // 76,    KEY_PAD_ENTER
        VD_FW_KEY_UNKNOWN,           // 77,    ----
        VD_FW_KEY_UNKNOWN,           // 78,    KEY_PAD_SUB
        VD_FW_KEY_F18,               // 79,    KEY_F18
        VD_FW_KEY_F19,               // 80,    KEY_F19
        VD_FW_KEY_UNKNOWN,           // 81,    KEY_PAD_EQUAL
        VD_FW_KEY_NUMPAD_0,          // 82,    KEY_PAD_0
        VD_FW_KEY_NUMPAD_1,          // 83,    KEY_PAD_1
        VD_FW_KEY_NUMPAD_2,          // 84,    KEY_PAD_2
        VD_FW_KEY_NUMPAD_3,          // 85,    KEY_PAD_3
        VD_FW_KEY_NUMPAD_4,          // 86,    KEY_PAD_4
        VD_FW_KEY_NUMPAD_5,          // 87,    KEY_PAD_5
        VD_FW_KEY_NUMPAD_6,          // 88,    KEY_PAD_6
        VD_FW_KEY_NUMPAD_7,          // 89,    KEY_PAD_7
        VD_FW_KEY_UNKNOWN,           // 90,    ----
        VD_FW_KEY_NUMPAD_8,          // 91,    KEY_PAD_8
        VD_FW_KEY_NUMPAD_9,          // 92,    KEY_PAD_9
        VD_FW_KEY_UNKNOWN,           // 93,    ----
        VD_FW_KEY_UNKNOWN,           // 94,    ----
        VD_FW_KEY_UNKNOWN,           // 95,    ----
        VD_FW_KEY_F5,                // 96,    KEY_F5
        VD_FW_KEY_F6,                // 97,    KEY_F6
        VD_FW_KEY_F7,                // 98,    KEY_F7
        VD_FW_KEY_F3,                // 99,    KEY_F3
        VD_FW_KEY_F8,                // 100,   KEY_F8
        VD_FW_KEY_F9,                // 101,   KEY_F9
        VD_FW_KEY_UNKNOWN,           // 102,    ----
        VD_FW_KEY_F11,               // 103,   KEY_F11
        VD_FW_KEY_UNKNOWN,           // 104,    ----
        VD_FW_KEY_F13,               // 105,   KEY_F13
        VD_FW_KEY_F16,               // 106,   KEY_F16
        VD_FW_KEY_F14,               // 107,   KEY_F14
        VD_FW_KEY_UNKNOWN,           // 108,    ----
        VD_FW_KEY_F10,               // 109,   KEY_F10
        VD_FW_KEY_UNKNOWN,           // 110,    ----
        VD_FW_KEY_F12,               // 111,   KEY_F12
        VD_FW_KEY_UNKNOWN,           // 112,    ----
        VD_FW_KEY_F15,               // 113,   KEY_F15
        VD_FW_KEY_UNKNOWN,           // 114,    ----
        VD_FW_KEY_HOME,              // 115,   KEY_HOME
        VD_FW_KEY_PGUP,              // 116,   KEY_PAGE_UP
        VD_FW_KEY_DEL,               // 117,   KEY_DEL
        VD_FW_KEY_F4,                // 118,   KEY_F4
        VD_FW_KEY_END,               // 119,   KEY_END
        VD_FW_KEY_F2,                // 120,   KEY_F2
        VD_FW_KEY_PGDN,              // 121,   KEY_PAGE_DOWN
        VD_FW_KEY_F1,                // 122,   KEY_F1
        VD_FW_KEY_ARROW_LEFT,        // 123,   KEY_LEFT
        VD_FW_KEY_ARROW_RIGHT,       // 124,   KEY_RIGHT
        VD_FW_KEY_ARROW_DOWN,        // 125,   KEY_DOWN
        VD_FW_KEY_ARROW_UP,          // 126,   KEY_UP
        VD_FW_KEY_UNKNOWN,           // 127,    ----
        VD_FW_KEY_UNKNOWN,           // 128,    ----
        VD_FW_KEY_UNKNOWN,           // 129,    ----
        VD_FW_KEY_UNKNOWN,           // 130,    ----
        VD_FW_KEY_UNKNOWN,           // 131,    ----
        VD_FW_KEY_UNKNOWN,           // 132,    ----
        VD_FW_KEY_UNKNOWN,           // 133,    ----
        VD_FW_KEY_UNKNOWN,           // 134,    ----
        VD_FW_KEY_UNKNOWN,           // 135,    ----
        VD_FW_KEY_UNKNOWN,           // 136,    ----
        VD_FW_KEY_UNKNOWN,           // 137,    ----
        VD_FW_KEY_UNKNOWN,           // 138,    ----
        VD_FW_KEY_UNKNOWN,           // 139,    ----
        VD_FW_KEY_UNKNOWN,           // 140,    ----
        VD_FW_KEY_UNKNOWN,           // 141,    ----
        VD_FW_KEY_UNKNOWN,           // 142,    ----
        VD_FW_KEY_UNKNOWN,           // 143,    ----
        VD_FW_KEY_UNKNOWN,           // 144,    ----
        VD_FW_KEY_UNKNOWN,           // 145,    ----
        VD_FW_KEY_UNKNOWN,           // 146,    ----
        VD_FW_KEY_UNKNOWN,           // 147,    ----
        VD_FW_KEY_UNKNOWN,           // 148,    ----
        VD_FW_KEY_UNKNOWN,           // 149,    ----
        VD_FW_KEY_UNKNOWN,           // 150,    ----
        VD_FW_KEY_UNKNOWN,           // 151,    ----
        VD_FW_KEY_UNKNOWN,           // 152,    ----
        VD_FW_KEY_UNKNOWN,           // 153,    ----
        VD_FW_KEY_UNKNOWN,           // 154,    ----
        VD_FW_KEY_UNKNOWN,           // 155,    ----
        VD_FW_KEY_UNKNOWN,           // 156,    ----
        VD_FW_KEY_UNKNOWN,           // 157,    ----
        VD_FW_KEY_UNKNOWN,           // 158,    ----
        VD_FW_KEY_UNKNOWN,           // 159,    ----
        VD_FW_KEY_UNKNOWN,           // 160,    ----
        VD_FW_KEY_UNKNOWN,           // 161,    ----
        VD_FW_KEY_UNKNOWN,           // 162,    ----
        VD_FW_KEY_UNKNOWN,           // 163,    ----
        VD_FW_KEY_UNKNOWN,           // 164,    ----
        VD_FW_KEY_UNKNOWN,           // 165,    ----
        VD_FW_KEY_UNKNOWN,           // 166,    ----
        VD_FW_KEY_UNKNOWN,           // 167,    ----
        VD_FW_KEY_UNKNOWN,           // 168,    ----
        VD_FW_KEY_UNKNOWN,           // 169,    ----
        VD_FW_KEY_UNKNOWN,           // 170,    ----
        VD_FW_KEY_UNKNOWN,           // 171,    ----
        VD_FW_KEY_UNKNOWN,           // 172,    ----
        VD_FW_KEY_UNKNOWN,           // 173,    ----
        VD_FW_KEY_UNKNOWN,           // 174,    ----
        VD_FW_KEY_UNKNOWN,           // 175,    ----
        VD_FW_KEY_UNKNOWN,           // 176,    ----
        VD_FW_KEY_UNKNOWN,           // 177,    ----
        VD_FW_KEY_UNKNOWN,           // 178,    ----
        VD_FW_KEY_UNKNOWN,           // 179,    ----
        VD_FW_KEY_UNKNOWN,           // 180,    ----
        VD_FW_KEY_UNKNOWN,           // 181,    ----
        VD_FW_KEY_UNKNOWN,           // 182,    ----
        VD_FW_KEY_UNKNOWN,           // 183,    ----
        VD_FW_KEY_UNKNOWN,           // 184,    ----
        VD_FW_KEY_UNKNOWN,           // 185,    ----
        VD_FW_KEY_UNKNOWN,           // 186,    ----
        VD_FW_KEY_UNKNOWN,           // 187,    ----
        VD_FW_KEY_UNKNOWN,           // 188,    ----
        VD_FW_KEY_UNKNOWN,           // 189,    ----
        VD_FW_KEY_UNKNOWN,           // 190,    ----
        VD_FW_KEY_UNKNOWN,           // 191,    ----
        VD_FW_KEY_UNKNOWN,           // 192,    ----
        VD_FW_KEY_UNKNOWN,           // 193,    ----
        VD_FW_KEY_UNKNOWN,           // 194,    ----
        VD_FW_KEY_UNKNOWN,           // 195,    ----
        VD_FW_KEY_UNKNOWN,           // 196,    ----
        VD_FW_KEY_UNKNOWN,           // 197,    ----
        VD_FW_KEY_UNKNOWN,           // 198,    ----
        VD_FW_KEY_UNKNOWN,           // 199,    ----
        VD_FW_KEY_UNKNOWN,           // 200,    ----
        VD_FW_KEY_UNKNOWN,           // 201,    ----
        VD_FW_KEY_UNKNOWN,           // 202,    ----
        VD_FW_KEY_UNKNOWN,           // 203,    ----
        VD_FW_KEY_UNKNOWN,           // 204,    ----
        VD_FW_KEY_UNKNOWN,           // 205,    ----
        VD_FW_KEY_UNKNOWN,           // 206,    ----
        VD_FW_KEY_UNKNOWN,           // 207,    ----
        VD_FW_KEY_UNKNOWN,           // 208,    ----
        VD_FW_KEY_UNKNOWN,           // 209,    ----
        VD_FW_KEY_UNKNOWN,           // 210,    ----
        VD_FW_KEY_UNKNOWN,           // 211,    ----
        VD_FW_KEY_UNKNOWN,           // 212,    ----
        VD_FW_KEY_UNKNOWN,           // 213,    ----
        VD_FW_KEY_UNKNOWN,           // 214,    ----
        VD_FW_KEY_UNKNOWN,           // 215,    ----
        VD_FW_KEY_UNKNOWN,           // 216,    ----
        VD_FW_KEY_UNKNOWN,           // 217,    ----
        VD_FW_KEY_UNKNOWN,           // 218,    ----
        VD_FW_KEY_UNKNOWN,           // 219,    ----
        VD_FW_KEY_UNKNOWN,           // 220,    ----
        VD_FW_KEY_UNKNOWN,           // 221,    ----
        VD_FW_KEY_UNKNOWN,           // 222,    ----
        VD_FW_KEY_UNKNOWN,           // 223,    ----
        VD_FW_KEY_UNKNOWN,           // 224,    ----
        VD_FW_KEY_UNKNOWN,           // 225,    ----
        VD_FW_KEY_UNKNOWN,           // 226,    ----
        VD_FW_KEY_UNKNOWN,           // 227,    ----
        VD_FW_KEY_UNKNOWN,           // 228,    ----
        VD_FW_KEY_UNKNOWN,           // 229,    ----
        VD_FW_KEY_UNKNOWN,           // 230,    ----
        VD_FW_KEY_UNKNOWN,           // 231,    ----
        VD_FW_KEY_UNKNOWN,           // 232,    ----
        VD_FW_KEY_UNKNOWN,           // 233,    ----
        VD_FW_KEY_UNKNOWN,           // 234,    ----
        VD_FW_KEY_UNKNOWN,           // 235,    ----
        VD_FW_KEY_UNKNOWN,           // 236,    ----
        VD_FW_KEY_UNKNOWN,           // 237,    ----
        VD_FW_KEY_UNKNOWN,           // 238,    ----
        VD_FW_KEY_UNKNOWN,           // 239,    ----
        VD_FW_KEY_UNKNOWN,           // 240,    ----
        VD_FW_KEY_UNKNOWN,           // 241,    ----
        VD_FW_KEY_UNKNOWN,           // 242,    ----
        VD_FW_KEY_UNKNOWN,           // 243,    ----
        VD_FW_KEY_UNKNOWN,           // 244,    ----
        VD_FW_KEY_UNKNOWN,           // 245,    ----
        VD_FW_KEY_UNKNOWN,           // 246,    ----
        VD_FW_KEY_UNKNOWN,           // 247,    ----
        VD_FW_KEY_UNKNOWN,           // 248,    ----
        VD_FW_KEY_UNKNOWN,           // 249,    ----
        VD_FW_KEY_UNKNOWN,           // 250,    ----
        VD_FW_KEY_UNKNOWN,           // 251,    ----
        VD_FW_KEY_UNKNOWN,           // 252,    ----
        VD_FW_KEY_UNKNOWN,           // 253,    ----
        VD_FW_KEY_UNKNOWN,           // 254,    ----
        VD_FW_KEY_UNKNOWN,           // 255,    ----
        VD_FW_KEY_LCONTROL,          // 256,   KEY_CTRL_LEFT
        VD_FW_KEY_LSHIFT,            // 257,   KEY_SHIFT_LEFT
        VD_FW_KEY_RSHIFT,            // 258,   KEY_SHIFT_RIGHT
        VD_FW_KEY_LALT,              // 259,   KEY_COMMAND_LEFT
        VD_FW_KEY_RALT,              // 260,   KEY_COMMAND_RIGHT
        VD_FW_KEY_UNKNOWN,           // 261,   KEY_OPTION_LEFT
        VD_FW_KEY_UNKNOWN,           // 262,   KEY_ALT_GR
        VD_FW_KEY_RCONTROL,          // 269,   KEY_CTRL_RIGHT
        VD_FW_KEY_CAPITAL,           // 272,   KEY_CAPSLOCK
        VD_FW_KEY_UNKNOWN,           // 279,   KEY_FN
    };

    if (keycode < 280) {
        return translation_table[keycode];
    } else {
        return VD_FW_KEY_UNKNOWN;
    }
}

static NSPoint vd_fw__mac_mouse_cocoa_to_conventional(NSPoint loc)
{
    NSView *cv = [VD_FW_G.window contentView];
    NSRect cvf = [cv frame];
    NSPoint loc_top_left_origin = NSMakePoint(loc.x, cvf.size.height - loc.y);

    NSPoint result = NSMakePoint(
        loc_top_left_origin.x * VD_FW_G.scale,
        loc_top_left_origin.y * VD_FW_G.scale);

    return result;
}

static VdFwWindowDelegate *Vd_Fw_Delegate;
static int Update_Context = 0;

@implementation VdFwWindowDelegate
- (void)updateGLContext {
    // [VD_FW_G.gl_context update];
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
    VD_FW_G.focus_changed = 1;
    VD_FW_G.focused = 1;
}

- (void)windowDidResignKey:(NSNotification *)notification {
    VD_FW_G.focus_changed = 1;
    VD_FW_G.focused = 0;
}

- (void)windowDidMiniaturize:(NSNotification *)notification
{
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_MINIMIZED;
    msg.dat.minimized.on = 1;
    vd_fw__msgbuf_w(&msg); 
}

- (void)windowDidDeminiaturize:(NSNotification *)notification
{

    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_MINIMIZED;
    msg.dat.minimized.on = 0;
    vd_fw__msgbuf_w(&msg); 
}

- (void)windowWillClose:(NSNotification*)notification {
    VD_FW_G.should_close = YES;
    sem_post(VD_FW_G.s_main_thread_window_closed);
    [NSApp stop:nil];
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification {

}

- (NSApplicationTerminateReply) applicationShouldTerminate:(NSApplication *) sender {
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_CLOSE_REQUEST;
    vd_fw__msgbuf_w(&msg);
    return NSTerminateCancel;
}

- (BOOL)windowShouldClose:(NSWindow *)notification {
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_CLOSE_REQUEST;
    vd_fw__msgbuf_w(&msg);
    return FALSE;
}

- (void)windowDidResize:(NSNotification *)notification {
    NSRect rect = [[VD_FW_G.window contentView] frame];
    VD_FW_G.w = (int)rect.size.width * VD_FW_G.scale;
    VD_FW_G.h = (int)rect.size.height * VD_FW_G.scale;

    if (!VD_FW_G.context_update_requested) {
        sem_post(VD_FW_G.s_main_thread_context_needs_update);
        VD_FW_G.context_update_requested = 1;
    }

    if (VD_FW_G.is_zoomed != [VD_FW_G.window isZoomed]) {
        VD_FW_G.is_zoomed = [VD_FW_G.window isZoomed];
        VdFw__MacMessage msg;
        msg.type = VD_FW__MAC_MESSAGE_ZOOMED;
        msg.dat.zoomed.on = VD_FW_G.is_zoomed;
        vd_fw__msgbuf_w(&msg); 
    }


    // CGLContextObj ctx = CGLGetCurrentContext();
    // CGLLockContext(ctx);
    // [VD_FW_G.gl_context update];
    // CGLUnlockContext(ctx);

    // @note(mdodis): Apple States:
    // > Call this method whenever the receiver’s drawable object changes
    // > size or location. A multithreaded application must synchronize 
    // > all threads that access the same drawable object and call update 
    // > for each thread’s context serially.
    //
    // So we wait to acquire paint lock here.
    // pthread_mutex_lock(&VD_FW_G.m_paint);
    // VD_FW_G.next_frame.flags |= VD_FW__MAC_FLAGS_REACQUIRE_CONTEXT;
    // [VD_FW_G.gl_context makeCurrentContext];
    // [VD_FW_G.gl_context update];
    // pthread_mutex_unlock(&VD_FW_G.m_paint);
}
- (BOOL)acceptsFirstResponder {
    return YES;
}

- (BOOL)wantsUpdateLayer {
    return YES;
}

- (BOOL)canDrawConcurrently {
    return YES;
}

- (BOOL)preservesContentDuringLiveResize {
    return NO;
}

- (void)applicationDidBecomeActive:(NSNotification *)notification {

    // NSWindow *window = [NSApp mainWindow];
    // if (window && !window.isKeyWindow) {
    //     printf("made window key\n");
    //     [window makeKeyAndOrderFront:nil];
    // } 

    // Restore if minimized
    if ([VD_FW_G.window isMiniaturized]) {
        [VD_FW_G.window deminiaturize:nil];
    }
}


@end

@implementation VdFwWindow
- (BOOL)_usesCustomDrawing {
    return YES;
}

@end

@implementation VdFwContentView

- (void)drawRect:(NSRect)dirtyRect {

    NSRect rect = [[VD_FW_G.window contentView] frame];
    VD_FW_G.w = (int)rect.size.width * VD_FW_G.scale;
    VD_FW_G.h = (int)rect.size.height * VD_FW_G.scale;

    pthread_mutex_lock(&VD_FW_G.m_paint);

    if ((VD_FW_G.w != VD_FW_G.next_frame.w) || (VD_FW_G.h != VD_FW_G.next_frame.h)) {
        VD_FW_G.next_frame.w = VD_FW_G.w;
        VD_FW_G.next_frame.h = VD_FW_G.h;
        VD_FW_G.next_frame.flags |= VD_FW__MAC_FLAGS_SIZE_CHANGED;
    }

    VD_FW_G.next_frame.flags |= VD_FW__MAC_FLAGS_WAKE_COND_VAR;

    // pthread_cond_signal(&VD_FW_G.n_paint);
    // pthread_cond_wait(&VD_FW_G.n_paint, &VD_FW_G.m_paint);
    pthread_mutex_unlock(&VD_FW_G.m_paint);
}

- (void)keyUp:(NSEvent*)evt
{
    unsigned short keycode = [evt keyCode];
    VdFwKey key = vd_fw__translate_mac_keycode(keycode);
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_KEY;
    msg.dat.key.down = 0;
    msg.dat.key.key = key;
    vd_fw__msgbuf_w(&msg); 
}

- (void)flagsChanged:(NSEvent*)evt
{
    NSEventModifierFlags flags = [evt modifierFlags];
    unsigned short keycode = [evt keyCode];

    unsigned char shift_down = (flags & NSEventModifierFlagShift) ? 1 : 0;
    unsigned char option_down = (flags & NSEventModifierFlagOption) ? 1 : 0;
    unsigned char control_down = (flags & NSEventModifierFlagControl) ? 1 : 0;

    switch (keycode) {
        case 60:
        case 56: {
            VdFw__MacMessage msg;
            msg.type = VD_FW__MAC_MESSAGE_KEY;
            msg.dat.key.down = shift_down;
            msg.dat.key.key = VD_FW_KEY_LSHIFT;
            vd_fw__msgbuf_w(&msg); 

            msg.type = VD_FW__MAC_MESSAGE_KEY;
            msg.dat.key.down = shift_down;
            msg.dat.key.key = VD_FW_KEY_RSHIFT;
            vd_fw__msgbuf_w(&msg); 
        } break;
        case 59: {
            VdFw__MacMessage msg;
            msg.type = VD_FW__MAC_MESSAGE_KEY;
            msg.dat.key.down = control_down;
            msg.dat.key.key = VD_FW_KEY_LCONTROL;
            vd_fw__msgbuf_w(&msg); 
        } break;
        case 61: {
            VdFw__MacMessage msg;
            msg.type = VD_FW__MAC_MESSAGE_KEY;
            msg.dat.key.down = option_down;
            msg.dat.key.key = VD_FW_KEY_RCONTROL;
            vd_fw__msgbuf_w(&msg); 
        } break;
        default: break;
    }
}

- (void)keyDown:(NSEvent*)evt
{
    unsigned short keycode = [evt keyCode];
    VdFwKey key = vd_fw__translate_mac_keycode(keycode);
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_KEY;
    msg.dat.key.down = 1;
    msg.dat.key.key = key;
    vd_fw__msgbuf_w(&msg); 
}

- (void)mouseDown:(NSEvent *)evt
{
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_MOUSEBTN;
    msg.dat.mousebtn.mask = VD_FW_MOUSE_STATE_LEFT_BUTTON_DOWN;
    msg.dat.mousebtn.down = 1;
    vd_fw__msgbuf_w(&msg); 

    NSPoint view_point = [evt locationInWindow];
    NSPoint p = NSMakePoint(view_point.x, view_point.y);

    p.x *= VD_FW_G.scale;
    p.y *= VD_FW_G.scale;

    int hit_drag_area = 0;
    if (VD_FW_G.draw_decorations) {
        return;
    }

    if (!VD_FW_G.nccaption_set) {
        hit_drag_area = 1;
    } else if (NSPointInRect(p, VD_FW_G.nccaption)) {
        hit_drag_area = 1;
        for (int ri = 0; ri < VD_FW_G.ncrect_count; ++ri) {
            if (NSPointInRect(p, VD_FW_G.ncrects[ri])) {
                hit_drag_area = 0;
                break;
            }
        }
    }

    if (hit_drag_area) {
        NSPoint loc = [VD_FW_G.window convertPointToScreen:view_point];
        NSRect window_frame = [VD_FW_G.window frame];

        loc.x -= window_frame.origin.x;
        loc.y -= window_frame.origin.y;
        VD_FW_G.drag_start_location = loc;
        VD_FW_G.drag_start_pos_window_coords = view_point;
        VD_FW_G.dragging = TRUE;
    }
}

- (void)rightMouseDown:(NSEvent *)evt
{
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_MOUSEBTN;
    msg.dat.mousebtn.mask = VD_FW_MOUSE_STATE_RIGHT_BUTTON_DOWN;
    msg.dat.mousebtn.down = 1;
    vd_fw__msgbuf_w(&msg); 
}

- (void)otherMouseDown:(NSEvent *)evt
{
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_MOUSEBTN;
    msg.dat.mousebtn.down = 1;

    int send_message = 0;
    switch ([evt buttonNumber]) {
        case 2: {
            send_message = 1;
            msg.dat.mousebtn.mask = VD_FW_MOUSE_STATE_MIDDLE_BUTTON_DOWN;
        } break;

        case 3:
        case 4: {
            send_message = 1;
            msg.dat.mousebtn.mask = 1 << [evt buttonNumber];
        } break;
    }

    if (send_message) {
        vd_fw__msgbuf_w(&msg); 
    }
}

- (void)scrollWheel:(NSEvent *)evt
{

    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_SCROLL;

    if ([evt hasPreciseScrollingDeltas]) {
        msg.dat.scroll.sx = [evt scrollingDeltaX] * 0.05f;
        msg.dat.scroll.sy = [evt scrollingDeltaY] * 0.05f;
    } else {
        msg.dat.scroll.sx = [evt deltaX];
        msg.dat.scroll.sy = [evt deltaY];
    }
    vd_fw__msgbuf_w(&msg); 
}

- (void)mouseUp:(NSEvent *)evt
{
    VD_FW_G.dragging = FALSE;
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_MOUSEBTN;
    msg.dat.mousebtn.mask = VD_FW_MOUSE_STATE_LEFT_BUTTON_DOWN;
    msg.dat.mousebtn.down = 0;
    vd_fw__msgbuf_w(&msg); 
}

- (void)rightMouseUp:(NSEvent *)evt
{
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_MOUSEBTN;
    msg.dat.mousebtn.mask = VD_FW_MOUSE_STATE_RIGHT_BUTTON_DOWN;
    msg.dat.mousebtn.down = 0;
    vd_fw__msgbuf_w(&msg); 
}

- (void)otherMouseUp:(NSEvent *)evt
{

    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_MOUSEBTN;
    msg.dat.mousebtn.down = 0;

    int send_message = 0;
    switch ([evt buttonNumber]) {
        case 2: {
            send_message = 1;
            msg.dat.mousebtn.mask = VD_FW_MOUSE_STATE_MIDDLE_BUTTON_DOWN;
        } break;

        case 3:
        case 4: {
            send_message = 1;
            msg.dat.mousebtn.mask = 1 << [evt buttonNumber];
        } break;
    }

    if (send_message) {
        vd_fw__msgbuf_w(&msg); 
    }
}

- (void)mouseMoved:(NSEvent *)evt
{
    NSPoint loc = [evt locationInWindow];

    NSPoint pixel_point = vd_fw__mac_mouse_cocoa_to_conventional(loc);

    float delta[2] = {
        [evt deltaX],
        [evt deltaY],
       // pixel_point.x - VD_FW_G.last_mouse.x,
       // pixel_point.y - VD_FW_G.last_mouse.y,
    };

    VD_FW_G.last_mouse = pixel_point;
    // if (VD_FW_G.mouse_is_locked) {
    //     NSRect cvf = [VD_FW_G.window frame];

    //     NSPoint screen_loc = [NSEvent mouseLocation];

    //     CGFloat w = NSMaxX(cvf) - NSMinX(cvf);
    //     CGFloat h = NSMaxY(cvf) - NSMinY(cvf);

    //     if (!NSPointInRect(screen_loc, cvf)) {
    //         CGWarpMouseCursorPosition(CGPointMake(NSMinX(cvf) + w * .5f, NSMinY(cvf) + h * .5f));
    //         VD_FW_G.last_mouse.x = w * .5f;
    //         VD_FW_G.last_mouse.y = h * .5f;
    //     }
    // }

    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_MOUSEMOVE;
    msg.dat.mousemove.mx = pixel_point.x;
    msg.dat.mousemove.my = pixel_point.y;
    msg.dat.mousemove.dx = delta[0];
    msg.dat.mousemove.dy = delta[1];
    vd_fw__msgbuf_w(&msg); 

}

- (void)mouseDragged:(NSEvent *)evt
{

    NSPoint view_point = [evt locationInWindow];

    NSPoint scaled_pos = vd_fw__mac_mouse_cocoa_to_conventional(view_point);

    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_MOUSEMOVE;
    msg.dat.mousemove.mx = scaled_pos.x;
    msg.dat.mousemove.my = scaled_pos.y;
    vd_fw__msgbuf_w(&msg); 

    NSPoint p = [VD_FW_G.window convertPointToScreen: view_point];

    if (VD_FW_G.dragging) {

        NSPoint new_pos = NSMakePoint(p.x - VD_FW_G.drag_start_location.x,
                                      p.y - VD_FW_G.drag_start_location.y);

        [VD_FW_G.window setFrameOrigin: new_pos];
    }
}

- (void)rightMouseDragged:(NSEvent *)evt
{

}

- (void)otherMouseDragged:(NSEvent *)evt
{

}



//     if (event.type == NSEventTypeKeyDown) {
//         // swallow key events you don't want to beep
//         return YES;
//     }
//     return [super performKeyEquivalent:event];
// }
@end

static void vd_fw__mac_init(VdFwInitInfo *info);
static void vd_fw__mac_init_gl(VdFwInitInfo *info);
static void vd_fw__mac_runloop(int wait);

VD_FW_API int vd_fw_init(VdFwInitInfo *info)
{
    VD_FW_G.c_init_info = *info;
    sem_post(VD_FW_G.s_main_thread_opened_me);
    sem_wait(VD_FW_G.s_main_thread_window_ready);
    vd_fw__mac_init_gl(info);

    return 1;
}

VD_FW_API unsigned long long vd_fw_delta_ns(void)
{
    uint64_t now = mach_absolute_time();
    uint64_t ns = (now - VD_FW_G.last_time) * VD_FW_G.time_base.numer / VD_FW_G.time_base.denom;
    return ns;
}

VD_FW_API void vd_fw_get_mouse_delta(float *dx, float *dy)
{
    if (dx) *dx = VD_FW_G.mouse_delta[0]; 
    if (dy) *dy = VD_FW_G.mouse_delta[1];
}

VD_FW_API int vd_fw_get_mouse_state(int *x, int *y)
{
    int result = VD_FW_G.mouse_state;
    if (x) *x = VD_FW_G.mouse[0];
    if (y) *y = VD_FW_G.mouse[1];
    return result;
}

VD_FW_API int vd_fw_get_mouse_clicked(int button)
{
    return !(VD_FW_G.prev_mouse_state & button) && (VD_FW_G.mouse_state & button);
}

VD_FW_API void vd_fw_set_mouse_locked(int locked)
{
    if (VD_FW_G.mouse_is_locked == locked) {
        return;
    }

    VD_FW_G.mouse_is_locked = locked;

    if (locked) {
        CGDisplayHideCursor(kCGDirectMainDisplay);
        CGAssociateMouseAndMouseCursorPosition(false);

    } else {
        CGDisplayShowCursor(kCGDirectMainDisplay);
        CGAssociateMouseAndMouseCursorPosition(true);
    }
}

VD_FW_API int vd_fw_get_mouse_locked(void)
{
    return VD_FW_G.mouse_is_locked;
}

VD_FW_API int vd_fw_get_mouse_wheel(float *dx, float *dy)
{
    if (dx) *dx = VD_FW_G.wheel[0];
    if (dy) *dy = VD_FW_G.wheel[1];
    return VD_FW_G.wheel_moved;
}

VD_FW_API int vd_fw_get_key_pressed(int key)
{
    return !VD_FW_G.prev_key_states[key] && VD_FW_G.curr_key_states[key];
}

VD_FW_API int vd_fw_get_key_down(int key)
{
    return VD_FW_G.curr_key_states[key];    
}

VD_FW_API int vd_fw_running(void)
{
    if (sem_trywait(VD_FW_G.s_main_thread_window_closed) == 0) {
        return 0;
    }

    VD_FW_G.wheel_moved = 0;
    VD_FW_G.wheel[0] = 0.f;
    VD_FW_G.wheel[1] = 0.f;

    VD_FW_G.mouse_delta[0] = 0.f;
    VD_FW_G.mouse_delta[1] = 0.f;

    VD_FW_G.focus_changed = 0;
    VD_FW_G.window_state_changed = 0;
    VD_FW_G.close_request = 0;

    VD_FW_G.prev_mouse_state = VD_FW_G.mouse_state;
    for (int i = 0; i < VD_FW_KEY_MAX; ++i) {
        VD_FW_G.prev_key_states[i] = VD_FW_G.curr_key_states[i];
    }

    VdFw__MacMessage msg;
    while (vd_fw__msgbuf_r(&msg)) {
        switch (msg.type) {
            case VD_FW__MAC_MESSAGE_MOUSEMOVE: {

                VD_FW_G.mouse_delta[0] += msg.dat.mousemove.dx * 0.2f;
                VD_FW_G.mouse_delta[1] += msg.dat.mousemove.dy * 0.2f;

                VD_FW_G.mouse[0] = msg.dat.mousemove.mx;
                VD_FW_G.mouse[1] = msg.dat.mousemove.my;
            } break;

            case VD_FW__MAC_MESSAGE_MOUSEBTN: {
                int state_mask = msg.dat.mousebtn.mask;
                if (msg.dat.mousebtn.down) {
                    VD_FW_G.mouse_state |= state_mask;
                } else {
                    VD_FW_G.mouse_state &= ~state_mask;
                }
            } break;

            case VD_FW__MAC_MESSAGE_SCROLL: {
                VD_FW_G.wheel[0] += msg.dat.scroll.sx;
                VD_FW_G.wheel[1] += msg.dat.scroll.sy;
            } break;

            case VD_FW__MAC_MESSAGE_KEY: {
                VD_FW_G.curr_key_states[msg.dat.key.key] = msg.dat.key.down;
            } break;

            case VD_FW__MAC_MESSAGE_MINIMIZED: {
                if (msg.dat.minimized.on) {
                    VD_FW_G.window_state |= VD_FW__MAC_WINDOW_STATE_MINIMIZED;
                } else {
                    VD_FW_G.window_state &= ~VD_FW__MAC_WINDOW_STATE_MINIMIZED;
                }
                VD_FW_G.window_state_changed = 1;
            } break;

            case VD_FW__MAC_MESSAGE_ZOOMED: {
                if (msg.dat.zoomed.on) {
                    VD_FW_G.window_state |= VD_FW__MAC_WINDOW_STATE_ZOOMED;
                } else {
                    VD_FW_G.window_state &= ~VD_FW__MAC_WINDOW_STATE_ZOOMED;
                }
                VD_FW_G.window_state_changed = 1;
            } break;

            case VD_FW__MAC_MESSAGE_CLOSE_REQUEST: {
                VD_FW_G.close_request = 1;
            } break;

            default: break;
        }
    }

    pthread_mutex_lock(&VD_FW_G.m_paint);
    VD_FW_G.curr_frame = VD_FW_G.next_frame;
    VD_FW_G.next_frame.flags = 0;
    pthread_mutex_unlock(&VD_FW_G.m_paint);

    return !VD_FW_G.should_close;
}

VD_FW_API int vd_fw_swap_buffers(void)
{
    VD_FW_G.last_time = mach_absolute_time();
    [VD_FW_G.gl_context flushBuffer];

    // if (VD_FW_G.curr_frame.flags & VD_FW__MAC_FLAGS_WAKE_COND_VAR) {
    //     pthread_cond_signal(&VD_FW_G.n_paint);
    // }

    if (sem_trywait(VD_FW_G.s_main_thread_context_needs_update) == 0) {
        @autoreleasepool {
            // [Vd_Fw_Delegate performSelectorOnMainThread:@selector(updateGLContext)
            //                                      withObject:nil
            //                                   waitUntilDone:YES];
            dispatch_sync(dispatch_get_main_queue(), ^(void){
                [VD_FW_G.gl_context update];
                VD_FW_G.context_update_requested = 0;
            });
        }
    }

    return 1;
}

VD_FW_API int vd_fw_close_requested(void)
{
    return VD_FW_G.close_request;
}

VD_FW_API void vd_fw_quit(void)
{
    dispatch_sync(dispatch_get_main_queue(), ^(void){
        [VD_FW_G.window close];
    });
    return;
}

VD_FW_API VdFwPlatform vd_fw_get_platform(void)
{
    return VD_FW_PLATFORM_MACOS;
}

VD_FW_API int vd_fw_get_focused(int *focused)
{
    *focused = VD_FW_G.focused;
    return VD_FW_G.focus_changed;
}

VD_FW_API int vd_fw_get_size(int *w, int *h)
{
    *w = VD_FW_G.curr_frame.w;
    *h = VD_FW_G.curr_frame.h;
    return VD_FW_G.curr_frame.flags & VD_FW__MAC_FLAGS_SIZE_CHANGED;
}

VD_FW_API int vd_fw_get_minimized(int *minimized)
{
    if (minimized) {
        *minimized = VD_FW_G.window_state & VD_FW__MAC_WINDOW_STATE_MINIMIZED;
    }
    return VD_FW_G.window_state_changed;
}

VD_FW_API void vd_fw_minimize(void)
{
    NSWindow *window = VD_FW_G.window;
    dispatch_async(dispatch_get_main_queue(), ^{
        if (![window isMiniaturized]) {
            [window miniaturize:nil];
        }
    });
}

VD_FW_API int vd_fw_get_maximized(int *maximized)
{
    if (maximized) {
        *maximized = VD_FW_G.window_state & VD_FW__MAC_WINDOW_STATE_ZOOMED;
    }
    return VD_FW_G.window_state_changed;
}

VD_FW_API void vd_fw_maximize(void)
{
    NSWindow *window = VD_FW_G.window;
    dispatch_async(dispatch_get_main_queue(), ^{
        if (![window isZoomed]) {
            [window zoom:nil];
        }
    });
}

VD_FW_API void vd_fw_normalize(void)
{
    NSWindow *window = VD_FW_G.window;
    dispatch_async(dispatch_get_main_queue(), ^{
        // If the window is minimized, restore it from the Dock.
        if ([window isMiniaturized]) {
            [window deminiaturize:nil];
            return;
        }

        // If the window is zoomed (maximized), unzoom it.
        if ([window isZoomed]) {
            [window zoom:nil];
            return;
        }

        // Otherwise, just bring it to the front in case it’s hidden.
        [window makeKeyAndOrderFront:nil];
    });
}

VD_FW_API void vd_fw_set_ncrects(int caption[4], int count, int (*rects)[4])
{
    NSView *cv = [VD_FW_G.window contentView];
    NSRect cvf = [cv frame];

    cvf.size.width  *= VD_FW_G.scale;
    cvf.size.height *= VD_FW_G.scale;

    VD_FW_G.nccaption_set = 1;
    VD_FW_G.nccaption.origin.x    = caption[0];
    VD_FW_G.nccaption.origin.y    = cvf.size.height - (caption[3] - caption[1]);
    VD_FW_G.nccaption.size.width  = caption[2] - caption[0];
    VD_FW_G.nccaption.size.height = (caption[3] - caption[1]);

    VD_FW_G.ncrect_count = count;
    int c = count;
    if (c > VD_FW_NCRECTS_MAX) {
        c = VD_FW_NCRECTS_MAX;
    }
    for (int i = 0; i < c; ++i) {

        VD_FW_G.ncrects[i].origin.x    = rects[i][0];
        VD_FW_G.ncrects[i].origin.y    = cvf.size.height - (rects[i][3] - rects[i][1]);
        VD_FW_G.ncrects[i].size.width  = rects[i][2] - rects[i][0];
        VD_FW_G.ncrects[i].size.height = (rects[i][3] - rects[i][1]);
    }
}

VD_FW_API float vd_fw_get_scale(void)
{
    return VD_FW_G.scale;
}

VD_FW_API void vd_fw_set_title(const char *title)
{
    [VD_FW_G.window setTitle: [NSString stringWithUTF8String: (title)]];
}

VD_FW_API void vd_fw_set_app_icon(void *pixels, int width, int height)
{
    VdFwU32 *app_image_data = (VdFwU32*)malloc(sizeof(unsigned int) * width * height);
    VD_FW_MEMCPY(app_image_data, pixels, sizeof(unsigned int) * width * height);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            VdFwU32 pixel = app_image_data[y * width + x];
            VdFwU8  alpha = (pixel >> 24) & 0xFF;
            VdFwU8  red   = (pixel >> 16) & 0xFF;
            VdFwU8  green = (pixel >>  8) & 0xFF;
            VdFwU8  blue  = (pixel >>  0) & 0xFF;

            VdFwU32 new_pixel = (red << 24) | (green << 16) | (blue << 8) | alpha;
            app_image_data[y * width + x] = new_pixel;
        }
    }

    NSBitmapImageRep *rep = [[NSBitmapImageRep alloc]
        initWithBitmapDataPlanes:(unsigned char**)&app_image_data
                      pixelsWide:width
                      pixelsHigh:height
                   bitsPerSample:8
                 samplesPerPixel:4
                        hasAlpha:YES
                        isPlanar:NO
                  colorSpaceName:NSDeviceRGBColorSpace
                     bytesPerRow:width * 4
                    bitsPerPixel:32];
    NSImage *img = [[NSImage alloc] initWithSize:NSMakeSize(width, height)];
    [img addRepresentation:rep];
    dispatch_async(dispatch_get_main_queue(), ^ {
        NSImage *prev_img = VD_FW_G.app_image;
        VD_FW_G.app_image = img;
        [NSApp setApplicationIconImage:img];
        free(app_image_data);

        if (prev_img) {
            [prev_img release];
        }
    });
}

VD_FW_API int vd_fw_set_vsync_on(int on)
{
    @autoreleasepool {
        if (VD_FW_G.gl_context) {
            GLint sync = on;
            [VD_FW_G.gl_context setValues:&sync forParameter:NSOpenGLCPSwapInterval];
        }
    }
    return 1;
}

VD_FW_API int vd_fw__any_time_higher(int num_files, const char **files, unsigned long long *check_against)
{
    int result = 0;
    for (int i = 0; i < num_files; ++i) {
        int fd = open(files[i], O_RDONLY);

        if (fd < 0) {
            return 0;
        }

        struct stat st;
        if (fstat(fd, &st) != 0) {
            close(fd);
            return 0;
        }

        close(fd);

        unsigned long long file_secs = st.st_mtimespec.tv_sec;

        if (file_secs > *check_against) {
            *check_against = file_secs;
            close(fd);
            result = 1;
            break;
        }

    }

    return result;
}

VD_FW_API char *vd_fw__debug_dump_file_text(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        return 0;
    }

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *result = (char*)malloc(size +1);
    fread(result, size, 1, f);

    result[size] = 0;
    return result;
}

VD_FW_API void vd_fw__free_mem(void *memory)
{
    free(memory);
}

VD_FW_API void vd_fw__lock_gamepaddb(void)
{
    return;
}

VD_FW_API void vd_fw__unlock_gamepaddb(void)
{
    return;
}

VD_FW_API void vd_fw__notify_gamepaddb_changed(void)
{
    return;
}

VD_FW_API void *vd_fw__realloc_mem(void *prev_ptr, size_t size)
{
    return realloc(prev_ptr, size);
}


#ifndef VD_FW_MACOS_NO_MAIN_OVERRIDE
#   undef main
static void *vd_fw__macos__main(void *arg)
{
    (void)arg;
    vd_fw__macos_main(VD_FW_G.argc, VD_FW_G.argv);

    return NULL;
}


int main(int argc, char const *argv[])
{
    VD_FW_G.argc = argc;
    VD_FW_G.argv = argv;

    sem_unlink("/sm-fw-cmtom");
    VD_FW_G.s_main_thread_opened_me = sem_open("/sm-fw-cmtom",
                                               O_CREAT,
                                               0644,
                                               0);
    sem_unlink("/sm-fw-cmtwr");
    VD_FW_G.s_main_thread_window_ready = sem_open("/sm-fw-cmtwr",
                                                  O_CREAT,
                                                  0644,
                                                  0);

    sem_unlink("/sm-fw-cmtwc");
    VD_FW_G.s_main_thread_window_closed = sem_open("/sm-fw-cmtwc",
                                                   O_CREAT,
                                                   0644,
                                                   0);

    sem_unlink("/sm-fw-cmtcnu");
    VD_FW_G.s_main_thread_context_needs_update = sem_open("/sm-fw-cmtcnu",
                                                          O_CREAT,
                                                          0644,
                                                          0);
    pthread_mutex_init(&VD_FW_G.m_paint, NULL);
    pthread_mutex_init(&VD_FW_G.m_input, NULL);
    pthread_cond_init(&VD_FW_G.n_paint, NULL);
    pthread_create(&VD_FW_G.main_thread, NULL, vd_fw__macos__main, NULL);

    while (1) {

        sem_wait(VD_FW_G.s_main_thread_opened_me);
        vd_fw__mac_init(&VD_FW_G.c_init_info);

        sem_post(VD_FW_G.s_main_thread_window_ready);

        vd_fw__mac_runloop(1);

        if (VD_FW_G.should_close) {
            break;
        }
    }
    return 0;
}
#endif // !VD_FW_MACOS_NO_MAIN_OVERRIDE

static void vd_fw__mac_init(VdFwInitInfo *info)
{
    VD_FW_G.draw_decorations = 1;
    if (info) {
        VD_FW_G.draw_decorations = !info->window_options.borderless;
    }

    [NSApplication sharedApplication];
    [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
    [NSEvent setMouseCoalescingEnabled:NO];

    VdFwGlVersion version = VD_FW_GL_VERSION_3_3;
    if (info && info->gl.version != 0) {
        version = info->gl.version;
    }

    // @autoreleasepool
    {
        NSScreen *main_screen = [NSScreen mainScreen];
        VD_FW_G.scale = [main_screen backingScaleFactor];

        NSRect screen_rect = [main_screen frame];

        VdFwWindowDelegate *delegate = [[VdFwWindowDelegate alloc] init];
        [NSApp setDelegate: delegate];
        Vd_Fw_Delegate = delegate;

        // Create a simple menu bar so the app shows in the Dock
        NSMenu *menubar = [[NSMenu new] autorelease];
        NSMenuItem *appMenuItem = [[NSMenuItem new] autorelease];
        [menubar addItem:appMenuItem];
        [NSApp setMainMenu:menubar];
        NSMenu *appMenu = [[NSMenu new] initWithTitle:@"Application"];
        NSMenuItem *quitMenuItem = [[NSMenuItem alloc]
            initWithTitle:@"Quit"
                   action:@selector(terminate:)
            keyEquivalent:@"q"];
        [appMenu addItem:quitMenuItem];
        [appMenuItem setSubmenu:appMenu];
        int w = 640;
        int h = 480;
        int x = screen_rect.size.width  * 0.5f - w * 0.5f;
        int y = screen_rect.size.height * 0.5f - h * 0.5f;
        NSRect frame = NSMakeRect(x, y, w, h);
        NSWindowStyleMask window_style_mask = NSWindowStyleMaskClosable |
                                              NSWindowStyleMaskMiniaturizable |
                                              NSWindowStyleMaskTitled |
                                              NSWindowStyleMaskResizable;
        VD_FW_G.window = [[VdFwWindow alloc] initWithContentRect: frame
                                                     styleMask: window_style_mask
                                                       backing: NSBackingStoreBuffered
                                                         defer: NO];

        if (!VD_FW_G.draw_decorations) {
            VD_FW_G.window.titleVisibility = NSWindowTitleHidden;
            VD_FW_G.window.titlebarAppearsTransparent = YES;
            [[VD_FW_G.window standardWindowButton:NSWindowCloseButton] setHidden:YES];
            [[VD_FW_G.window standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
            [[VD_FW_G.window standardWindowButton:NSWindowZoomButton] setHidden:YES];
            VD_FW_G.window.styleMask |= NSWindowStyleMaskFullSizeContentView;
            [VD_FW_G.window setMovable:NO];
            [VD_FW_G.window setMovableByWindowBackground:NO];
        }

        [VD_FW_G.window                       setTitle: [NSString stringWithUTF8String: "FW Window"]];
        [VD_FW_G.window                   setHasShadow: YES];
        [VD_FW_G.window           makeKeyAndOrderFront: nil];
        // [VD_FW_G.window setAllowsConcurrentViewDrawing: YES];


        NSOpenGLPixelFormatAttribute nsversion = 0;

        switch (version) {
            default:
            case VD_FW_GL_VERSION_BASIC: nsversion = NSOpenGLProfileVersionLegacy; break;

            case VD_FW_GL_VERSION_1_0: nsversion = NSOpenGLProfileVersionLegacy; break;
            case VD_FW_GL_VERSION_1_1: nsversion = NSOpenGLProfileVersionLegacy; break;
            case VD_FW_GL_VERSION_1_2: nsversion = NSOpenGLProfileVersionLegacy; break;
            case VD_FW_GL_VERSION_1_3: nsversion = NSOpenGLProfileVersionLegacy; break;
            case VD_FW_GL_VERSION_1_4: nsversion = NSOpenGLProfileVersionLegacy; break;
            case VD_FW_GL_VERSION_1_5: nsversion = NSOpenGLProfileVersionLegacy; break;
            case VD_FW_GL_VERSION_2_0: nsversion = NSOpenGLProfileVersionLegacy; break;
            case VD_FW_GL_VERSION_2_1: nsversion = NSOpenGLProfileVersionLegacy; break;
            case VD_FW_GL_VERSION_3_0: nsversion = NSOpenGLProfileVersionLegacy; break;
            case VD_FW_GL_VERSION_3_1: nsversion = NSOpenGLProfileVersionLegacy; break;
            case VD_FW_GL_VERSION_3_2: nsversion = NSOpenGLProfileVersion3_2Core; break;
            case VD_FW_GL_VERSION_3_3: nsversion = NSOpenGLProfileVersion3_2Core; break;
            case VD_FW_GL_VERSION_4_0: nsversion = NSOpenGLProfileVersion4_1Core; break;
            case VD_FW_GL_VERSION_4_1: nsversion = NSOpenGLProfileVersion4_1Core; break;
            case VD_FW_GL_VERSION_4_2: nsversion = NSOpenGLProfileVersion4_1Core; break;
            case VD_FW_GL_VERSION_4_3: nsversion = NSOpenGLProfileVersion4_1Core; break;
            case VD_FW_GL_VERSION_4_4: nsversion = NSOpenGLProfileVersion4_1Core; break;
            case VD_FW_GL_VERSION_4_5: nsversion = NSOpenGLProfileVersion4_1Core; break;
            case VD_FW_GL_VERSION_4_6: nsversion = NSOpenGLProfileVersion4_1Core; break;
        } 

        NSOpenGLPixelFormatAttribute attrs[] = {
            NSOpenGLPFAOpenGLProfile, nsversion,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFAAlphaSize, 0,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFAAccelerated,
            0
        };

        NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes: attrs];
        VD_FW_G.gl_context = [[NSOpenGLContext alloc]              initWithFormat: pf
                                                                     shareContext: nil];
        NSRect wframe = [[VD_FW_G.window contentView] bounds];
        NSView *fw_view = [[VdFwContentView alloc] initWithFrame:wframe];
        VD_FW_G.content_view = (VdFwContentView*)fw_view;
        [VD_FW_G.window setContentView: fw_view];
        [fw_view setWantsLayer: YES];
        // [[fw_view layer] setDrawsAsynchronously: YES];

        [VD_FW_G.gl_context setView: fw_view];


        [VD_FW_G.window setDelegate:delegate];
        [VD_FW_G.window makeFirstResponder: fw_view];
        [VD_FW_G.window setAcceptsMouseMovedEvents: YES];
        [NSApp activateIgnoringOtherApps:YES];
    }
    VD_FW_G.w = 640 * VD_FW_G.scale;
    VD_FW_G.h = 480 * VD_FW_G.scale;
    vd_fw__load_opengl(version);

    mach_timebase_info(&VD_FW_G.time_base);
    VD_FW_G.last_time = mach_absolute_time();

    dispatch_async(dispatch_get_main_queue(), ^{
        [VD_FW_G.window makeKeyAndOrderFront:nil];
    });
}

static void vd_fw__mac_init_gl(VdFwInitInfo *info)
{
    [VD_FW_G.gl_context makeCurrentContext];
}


static void vd_fw__mac_runloop(int wait)
{
    [NSApp run];
}

static int vd_fw__msgbuf_r(VdFw__MacMessage *message)
{
    VdFwI32 r = VD_FW_G.msgbuf_r;
    VdFwI32 w;
    __atomic_load(&VD_FW_G.msgbuf_w, &w, __ATOMIC_SEQ_CST);

    if (r == w) {
        return 0;
    }

    *message = VD_FW_G.msgbuf[r];

    VdFwI32 nr = (r + 1) % VD_FW_MAC_MESSAGE_BUFFER_SIZE;
    __atomic_exchange_n(&VD_FW_G.msgbuf_r, nr, __ATOMIC_SEQ_CST);

    return 1;
}

static int vd_fw__msgbuf_w(VdFw__MacMessage *message)
{
    VdFwI32 w = VD_FW_G.msgbuf_w;
    VdFwI32 r;
    __atomic_load(&VD_FW_G.msgbuf_r, &r, __ATOMIC_SEQ_CST);

    if ((w + 1) % VD_FW_MAC_MESSAGE_BUFFER_SIZE == r) {
        return 0;
    }

    VD_FW_G.msgbuf[w] = *message;
    VdFwI32 nw = (w + 1) % VD_FW_MAC_MESSAGE_BUFFER_SIZE;
    __atomic_exchange_n(&VD_FW_G.msgbuf_w, nw, __ATOMIC_SEQ_CST);

    return 1;
}

#elif defined(__linux__)
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

extern Window XCreateWindow(
    Display*            /* display */,
    Window              /* parent */,
    int                 /* x */,
    int                 /* y */,
    unsigned int        /* width */,
    unsigned int        /* height */,
    unsigned int        /* border_width */,
    int                 /* depth */,
    unsigned int        /* class */,
    Visual*             /* visual */,
    unsigned long       /* valuemask */,
    XSetWindowAttributes*       /* attributes */
);

#define VD_FW_X11_FUNCTIONS \
    XBEGIN_MODULE(xlib) \
    XSYM(xlib, Display*, XOpenDisplay, (const char *name)) \
    XEND_MODULE()

#define XBEGIN_MODULE(name)
#define XSYM(module, retval, name, args) typedef retval (*VdFw__Proc##name)args; static VdFw__Proc##name VdFw__##name;
#define XEND_MODULE()

VD_FW_X11_FUNCTIONS

#undef XBEGIN_MODULE
#undef XSYM
#undef XEND_MODULE

#define VD_FW_LOG(fmt, ...) printf("fw: " fmt "\n", ##__VA_ARGS__)

#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    void         *handle_xlib;
    int          has_xlib;
    Display      *display;
} VdFw__LinuxInternalData;

VdFw__LinuxInternalData VdFw__Globals = {0};

#define VD_FW_G VdFw__Globals

void *vd_fw__gl_get_proc_address(const char *name)
{
    return NULL;
}

VD_FW_API int vd_fw_init(VdFwInitInfo *info)
{
    {
        VD_FW_G.handle_xlib = dlopen("libX11.so.6", RTLD_NOW | RTLD_GLOBAL);
        VD_FW_G.has_xlib    = VD_FW_G.handle_xlib != NULL;

#define XBEGIN_MODULE(name) if (VD_FW_G.has_##name) {
#define XSYM(module, retval, name, args) VdFw__##name = (VdFw__Proc##name)dlsym(VD_FW_G.handle_##module, #name);
#define XEND_MODULE() }
        VD_FW_X11_FUNCTIONS
#undef XBEGIN_MODULE
#undef XSYM
#undef XEND_MODULE
    }

    VD_FW_G.display = VdFw__XOpenDisplay(NULL);

    if (VD_FW_G.display) {
        VD_FW_LOG("Opened default display");
    }

    Window root = DefaultRootWindow(VD_FW_G.display);
    int default_screen = DefaultScreen(VD_FW_G.display);

    int screen_bits = 24;
    XVisualInfo visual_info = {};
    if (!XMatchVisualInfo(VD_FW_G.display, default_screen, screen_bits, TrueColor, &visual_info)) {
        return 0;
    }

    XSetWindowAttributes window_attributes = {0};
    window_attributes.background_pixel = 0;
    window_attributes.colormap = XCreateColormap(VD_FW_G.display, root, visual_info.visual, AllocNone);
    unsigned long attribute_mask = CWBackPixel | CWColormap;

    return 1;
}

VD_FW_API int vd_fw_running(void)
{
    return 0;
}

VD_FW_API int vd_fw_swap_buffers(void)
{
    return 0;
}

VD_FW_API int vd_fw_set_vsync_on(int on)
{
    return 0;
}

VD_FW_API int vd_fw_get_size(int *w, int *h)
{
    return 0;
}

VD_FW_API int vd_fw__any_time_higher(int num_files, const char **files, unsigned long long *check_against)
{
    return 0;
}

VD_FW_API char *vd_fw__debug_dump_file_text(const char *path)
{
    return NULL;
}

VD_FW_API void vd_fw__free_mem(void *memory)
{
    free(memory);
}
#endif // _WIN32, __APPLE__, __linux__

#if !defined(__APPLE__)
/* ----EXTERNS DEFINITIONS------------------------------------------------------------------------------------------- */
#define X(retval, name, params) VdFwProcGL_##name name;
#define V(v)
#define VE()
VD_FW_OPENGL_CORE_FUNCTIONS
#undef X
#undef V
#undef VE

#endif // !defined(__APPLE__)

static void vd_fw__load_opengl(VdFwGlVersion version)
{
#if defined(__APPLE__)
    // @todo(mdodis): This check
    // if (version > VD_FW_GL_VERION_4_1) {

    // }
#else
#define LOAD(p, s) s = (p)vd_fw__gl_get_proc_address(#s)
/* ----LOADING------------------------------------------------------------------------------------------------------- */
#define X(retval, name, params) LOAD(VdFwProcGL_##name, name);
#define V(v) if (version >= VD_FW_GL_VERSION_##v) {
#define VE() }
VD_FW_OPENGL_CORE_FUNCTIONS
#undef X
#undef V
#undef VE

#undef LOAD
#endif  // defined(__APPLE__)
}

VD_FW_API unsigned int vd_fw_compile_shader(unsigned int type, const char *source)
{
    int success;
    GLuint shd = glCreateShader(type);
    glShaderSource(shd, 1, &source, 0);
    glCompileShader(shd);
    glGetShaderiv(shd, GL_COMPILE_STATUS, &success);

    if (success) {
        return shd;
    }

    static char buf[1024];
    GLsizei len;
    glGetShaderInfoLog(shd, sizeof(buf), &len, buf);
    buf[len] = 0;

    VD_FW_LOG("Shader compilation failed: %s\n", buf);
    return 0;
}

VD_FW_API int vd_fw_link_program(unsigned int program)
{
    int success;
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success) {
        return 1;
    }


    static char buf[1024];
    GLsizei len;
    glGetProgramInfoLog(program, sizeof(buf), &len, buf);
    return 0;
}

VD_FW_API int vd_fw_compile_or_hotload_program(unsigned int *program, unsigned long long *last_compile,
                                                              const char *vertex_file_path,
                                                              const char *fragment_file_path)
{
    int needs_recompile = 0;
    if (*last_compile == 0) {
        needs_recompile = 1;
    }

    const char *files[2] = {
        vertex_file_path,
        fragment_file_path,
    };

    if (vd_fw__any_time_higher(2, files, last_compile)) {
        needs_recompile = 1;
    }

    if (!needs_recompile) {
        return 1;
    }

    int result = 1;

    char *srv = vd_fw__debug_dump_file_text(vertex_file_path);
    char *srf = vd_fw__debug_dump_file_text(fragment_file_path);

    unsigned int vshd = vd_fw_compile_shader(GL_VERTEX_SHADER, srv);
    unsigned int fshd = vd_fw_compile_shader(GL_FRAGMENT_SHADER, srf);

    unsigned int new_program;
    if (vshd == 0 || fshd == 0) {
        result = 0;
    } else {
        vd_fw__free_mem(srv);
        vd_fw__free_mem(srf);

        new_program = glCreateProgram();
        glAttachShader(new_program, vshd);
        glAttachShader(new_program, fshd);

        if (vd_fw_link_program(new_program)) {
            glDeleteShader(vshd);
            glDeleteShader(fshd);
            if (*program != 0) {
                glDeleteProgram(*program);
            }

            *program = new_program;
        } else {
            result = 0;
        }
    }

    return result;
}

VD_FW_API void vd_fw__def_gamepad(VdFwGamepadMap *map)
{
    int c = 0;
    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;
    map->mappings[c].index  = 0x00;
    map->mappings[c].target = VD_FW_GAMEPAD_A;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;
    map->mappings[c].index  = 0x01;
    map->mappings[c].target = VD_FW_GAMEPAD_B;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;
    map->mappings[c].index  = 0x02;
    map->mappings[c].target = VD_FW_GAMEPAD_X;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;
    map->mappings[c].index  = 0x03;
    map->mappings[c].target = VD_FW_GAMEPAD_Y;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;
    map->mappings[c].index  = 0x04;
    map->mappings[c].target = VD_FW_GAMEPAD_L1;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;
    map->mappings[c].index  = 0x05;
    map->mappings[c].target = VD_FW_GAMEPAD_R1;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;
    map->mappings[c].index  = 0x06;
    map->mappings[c].target = VD_FW_GAMEPAD_SELECT;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;
    map->mappings[c].index  = 0x07;
    map->mappings[c].target = VD_FW_GAMEPAD_START;
    c++;
    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;
    map->mappings[c].index  = 0x08;
    map->mappings[c].target = VD_FW_GAMEPAD_L3;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;
    map->mappings[c].index  = 0x09;
    map->mappings[c].target = VD_FW_GAMEPAD_R3;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS;
    map->mappings[c].index  = 0x00;
    map->mappings[c].target = VD_FW_GAMEPAD_LH;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS;
    map->mappings[c].index  = 0x01;
    map->mappings[c].target = VD_FW_GAMEPAD_LV;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS;
    map->mappings[c].index  = 0x02;
    map->mappings[c].target = VD_FW_GAMEPAD_RH;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS;
    map->mappings[c].index  = 0x03;
    map->mappings[c].target = VD_FW_GAMEPAD_RV;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS;
    map->mappings[c].index  = 0x04;
    map->mappings[c].target = VD_FW_GAMEPAD_LT;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS;
    map->mappings[c].index  = 0x04;
    map->mappings[c].target = VD_FW_GAMEPAD_RT;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_HAT;
    map->mappings[c].index  = 0x01;
    map->mappings[c].target = VD_FW_GAMEPAD_DUP;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_HAT;
    map->mappings[c].index  = 0x02;
    map->mappings[c].target = VD_FW_GAMEPAD_DRIGHT;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_HAT;
    map->mappings[c].index  = 0x04;
    map->mappings[c].target = VD_FW_GAMEPAD_DDOWN;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_HAT;
    map->mappings[c].index  = 0x08;
    map->mappings[c].target = VD_FW_GAMEPAD_DLEFT;
    c++;

    map->mappings[c].kind   = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_NONE;
}

VD_FW_INL VdFwU16 vd_fw__crc16_byte(VdFwU8 r)
{
    VdFwU16 result = 0;
    int i;

    for (i = 0; i < 8; ++i) {
        result = ((result ^ r) & 1 ? 0xa001 : 0) ^ result >> 1;
        r >>= 1;
    }

    return result;
} 

VD_FW_API VdFwU16 vd_fw__crc16(VdFwU16 crc, void *data, VdFwSz len)
{
    size_t i;
    for (i = 0; i < len; ++i) {
        crc = vd_fw__crc16_byte((VdFwU8)crc ^ ((VdFwU8*)data)[i]) ^ crc >> 8;
    }
    return crc;
}

VD_FW_API VdFwGuid vd_fw__make_gamepad_guid(VdFwU16 bus, VdFwU16 vendor, VdFwU16 product, VdFwU16 version,
                                            char *vendor_name, char *product_name,
                                            VdFwU8 driver_signature, VdFwU8 driver_data)
{
    VdFwGuid result;
    VdFwU16 *guid16 = (VdFwU16*)result.dat;
    VdFwU16 crc = 0;

    VD_FW_MEMSET(&result, 0, sizeof(result));

    if (vendor_name && *vendor_name && product_name && *product_name) {
        crc = vd_fw__crc16(crc, vendor_name,  vd_fw__strlen(vendor_name));
        crc = vd_fw__crc16(crc, " ", 1);
        crc = vd_fw__crc16(crc, product_name, vd_fw__strlen(product_name));
    } else if (product_name) {
        crc = vd_fw__crc16(crc, product_name, vd_fw__strlen(product_name));
    }

    *guid16++ = VD_FW_SWAP16LE(bus);
    *guid16++ = VD_FW_SWAP16LE(crc);

    if (vendor) {
        *guid16++ = VD_FW_SWAP16LE(vendor);
        *guid16++ = 0;
        *guid16++ = VD_FW_SWAP16LE(product);
        *guid16++ = 0;
        *guid16++ = VD_FW_SWAP16LE(version);
        result.dat[14] = driver_signature;
        result.dat[15] = driver_data;
    } else {
        size_t avail = sizeof(result.dat) - 4;

        if (driver_signature) {
            avail -= 2;
            result.dat[14] = driver_signature;
            result.dat[15] = driver_data;
        }

        if (product_name) {
            vd_fw__strlcpy((char*)guid16, product_name, avail);
        }
    }

    return result;    
}

VD_FW_API void *vd_fw__resize_buffer(void *buffer, size_t element_size, int required_capacity, int *cap)
{
    if (required_capacity <= *cap) {
        return buffer;
    }

    int resize_capacity = required_capacity;
    buffer = vd_fw__realloc_mem(buffer, element_size * resize_capacity);
    *cap = resize_capacity;
    return buffer;
}

VD_FW_INL const char *vd_fw_get_key_name(VdFwKey k)
{
    static const char *translation_table[VD_FW_KEY_MAX] = {
        "Unknown","F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","F13","F14","F15",
        "F16","F17","F18","F19","F20","F21","F22","F23","F24","Backspace","Ins","Home","Pgup","Del","End","Pgdn",
        "Space","Lcontrol","Rcontrol","Lalt","Ralt","Lshift","Rshift","Quote","ArrowUp","ArrowLeft","ArrowDown","ArrowRight","Comma","Minus","Dot","SlashForward",
        "0","1","2","3","4","5","6","7","8","9","Enter","Semicolon","Tab","Equals","Capital","Escape",
        "Reserved1","A","B","C","D","E","F","G","H","I","J","K","L","M","N","O",
        "P","Q","R","S","T","U","V","W","X","Y","Z","BracketOpen","SlashBack","BracketClose","MediaNext","MediaPrev",
        "Backtick","MediaPlay","Numpad0","Numpad1",
        "Numpad2","Numpad3","Numpad4","Numpad5",
        "Numpad6","Numpad7","Numpad8","Numpad9",
    };

    return translation_table[k];
}

VD_FW_API const char *vd_fw_get_gamepad_face_name(VdFwGamepadFace face)
{
    if (face >= VD_FW_GAMEPAD_FACE_MAX) {
        face = 0;
    }

    static const char *translation_table[VD_FW_GAMEPAD_FACE_MAX] = {
        "unknown",
        "numbered",
        "xbox",
        "playstation",
        "nintendo",
    };

    return translation_table[face];
}

VD_FW_API const char *vd_fw_get_gamepad_class_name(VdFwGamepadClass klass)
{
    if (klass >= VD_FW_GAMEPAD_CLASS_MAX) {
        klass = 0;
    }

    static const char *translation_table[VD_FW_GAMEPAD_CLASS_MAX] = {
        "invalid",
        "nes",
        "megadrive",
        "genesis",
        "snes",
        "ps1",
        "joycon",
        "n64",
        "ps2",
        "xbox",
        "ps4",
        "steamdeck",
    };

    return translation_table[klass];
}

VD_FW_INL int vd_fw__compare_string(const char *s, int s_len, int i,
                                    const char *t, int t_len)
{
    if ((s_len - i) < t_len) {
        return 0;
    }

    int x;
    for (x = 0; x < t_len; ++x) {
        if (s[i + x] != t[x]) {
            return 0;
        }
    }

    return x;
}

typedef struct {
    const char          *sym;
    int                 len;
    VdFwGamepadFace face;
} VdFw__GamepadSymbolToFace;

static VdFw__GamepadSymbolToFace Vd_Fw__Gamepad_Symbols_To_Faces[] = {
#define VD_FW__SYM_LEN(s) s, sizeof(s) - 1
    {VD_FW__SYM_LEN("numbered"),    VD_FW_GAMEPAD_FACE_NUMBERED},
    {VD_FW__SYM_LEN("xbox"),        VD_FW_GAMEPAD_FACE_XBOX},
    {VD_FW__SYM_LEN("playstation"), VD_FW_GAMEPAD_FACE_PLAYSTATION},
    {VD_FW__SYM_LEN("nintendo"),    VD_FW_GAMEPAD_FACE_NINTENDO},
#undef VD_FW__SYM_LEN
};

static int vd_fw__parse_map_face(const char *s, int s_len, VdFwGamepadFace *out)
{
    *out = VD_FW_GAMEPAD_FACE_UNKNOWN;

    int map_count = VD_FW_ARRAY_COUNT(Vd_Fw__Gamepad_Symbols_To_Faces);

    for (int map_index = 0; map_index < map_count; ++map_index) {
        VdFw__GamepadSymbolToFace *check = &Vd_Fw__Gamepad_Symbols_To_Faces[map_index];
        if (check->len != s_len) {
            continue;
        }

        int found = 1;
        for (int i = 0; i < check->len; ++i) {
            if (check->sym[i] != s[i]) {
                found = 0;
                break;
            }
        }

        if (found) {
            *out = check->face;
            return check->len;
        }
    }
    return 0;
}

typedef struct {
    const char       *sym;
    int              len;
    VdFwGamepadClass klass;
} VdFw__GamepadSymbolToClass;
static VdFw__GamepadSymbolToClass Vd_Fw__Gamepad_Symbols_To_Classes[] = {
#define VD_FW__SYM_LEN(s) s, sizeof(s) - 1
    {VD_FW__SYM_LEN("nes"),       VD_FW_GAMEPAD_CLASS_NES},
    {VD_FW__SYM_LEN("megadrive"), VD_FW_GAMEPAD_CLASS_MEGADRIVE},
    {VD_FW__SYM_LEN("genesis"),   VD_FW_GAMEPAD_CLASS_GENESIS},
    {VD_FW__SYM_LEN("snes"),      VD_FW_GAMEPAD_CLASS_SNES},
    {VD_FW__SYM_LEN("ps1"),       VD_FW_GAMEPAD_CLASS_PS1},
    {VD_FW__SYM_LEN("joycon"),    VD_FW_GAMEPAD_CLASS_JOYCON},
    {VD_FW__SYM_LEN("n64"),       VD_FW_GAMEPAD_CLASS_N64},
    {VD_FW__SYM_LEN("ps2"),       VD_FW_GAMEPAD_CLASS_PS2},
    {VD_FW__SYM_LEN("xbox"),      VD_FW_GAMEPAD_CLASS_XBOX},
    {VD_FW__SYM_LEN("ps4"),       VD_FW_GAMEPAD_CLASS_PS4},
    {VD_FW__SYM_LEN("steamdeck"), VD_FW_GAMEPAD_CLASS_STEAMDECK},
#undef VD_FW__SYM_LEN
};

static int vd_fw__parse_map_class(const char *s, int s_len, VdFwGamepadClass *out)
{
    *out = VD_FW_GAMEPAD_CLASS_INVALID;

    int map_count = VD_FW_ARRAY_COUNT(Vd_Fw__Gamepad_Symbols_To_Classes);

    for (int map_index = 0; map_index < map_count; ++map_index) {
        VdFw__GamepadSymbolToClass *check = &Vd_Fw__Gamepad_Symbols_To_Classes[map_index];
        if (check->len != s_len) {
            continue;
        }

        int found = 1;
        for (int i = 0; i < check->len; ++i) {
            if (check->sym[i] != s[i]) {
                found = 0;
                break;
            }
        }

        if (found) {
            *out = check->klass;
            return check->len;
        }
    }
    return 0;
}


static int vd_fw__parse_map_entry(const char *s, int s_len, VdFwGamepadMapEntry *out)
{
    int sign_invert = 0;
    int range_zero_to_max = 0;
    int range_invert = 0;

    int i = 0;

    // Handle sign
    switch (s[i]) {
        case '+': range_zero_to_max = 1; i++; break;
        case '-': range_zero_to_max = 1; range_invert = 1; i++; break;
        case '~': sign_invert = 1; i++; break;
        default: break;
    }

    switch (s[i]) {
        case 'b': {
            i++;
            unsigned short number = 0;
            while ((i < s_len) && (s[i] >= '0' && s[i] <= '9')) {
                number *= 10;
                number += s[i++] - '0';
            }

            if (i >= s_len) {
                return 0;
            }

            out->kind = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;
            out->index = number;
        } break;

        case 'h': {
            i++;
            unsigned char hat_index = 0;

            while ((i < s_len) && (s[i] >= '0' && s[i] <= '9')) {
                hat_index *= 10;
                hat_index += s[i++] - '0';
            }

            if (i >= s_len) {
                return 0;
            }

            if (s[i++] != '.') {
                return 0;
            }

            unsigned char hat_value = 0;
            while ((i < s_len) && (s[i] >= '0' && s[i] <= '9')) {
                hat_value *= 10;
                hat_value += s[i++] - '0';
            }

            if (i >= s_len) {
                return 0;
            }

            out->kind = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_HAT;
            out->index = (((unsigned short)hat_index) << 8) | (hat_value);
        } break;

        case 'a': {
            i++;
            unsigned short number = 0;
            while ((i < s_len) && (s[i] >= '0' && s[i] <= '9')) {
                number *= 10;
                number += s[i++] - '0';
            }

            if (i >= s_len) {
                return 0;
            }

            out->kind = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS;
            out->index = number;
        } break;

        default: {
        } break;
    }

    if (range_zero_to_max) {
        out->kind |= VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_ZERO_TO_MAX;

        if (range_invert) {
            out->kind |= VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_INVERTED;
        }
    } else if (sign_invert) {
        out->kind |= VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_INVERTED;
    }

    return i;
}

static int vd_fw__is_db_symbol(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           (c >= '0' && c <= '0');
}

typedef struct {
    const char                   *sym;
    size_t                       len;
    uint8_t                      tgt;
    uint8_t                      is_axis;
} VdFw__GamepadSymbolToTarget;

VdFw__GamepadSymbolToTarget Vd_Fw__Gamepad_Symbols_To_Targets[] = {
#define VD_FW__SYM_LEN(s) s, sizeof(s) - 1
    {VD_FW__SYM_LEN("a"),                   VD_FW_GAMEPAD_A,              0},
    {VD_FW__SYM_LEN("b"),                   VD_FW_GAMEPAD_B,              0},
    {VD_FW__SYM_LEN("x"),                   VD_FW_GAMEPAD_X,              0},
    {VD_FW__SYM_LEN("y"),                   VD_FW_GAMEPAD_Y,              0},
    {VD_FW__SYM_LEN("start"),               VD_FW_GAMEPAD_START,          0},
    {VD_FW__SYM_LEN("back"),                VD_FW_GAMEPAD_BACK,           0},
    {VD_FW__SYM_LEN("dpup"),                VD_FW_GAMEPAD_DUP,            0},
    {VD_FW__SYM_LEN("dpdown"),              VD_FW_GAMEPAD_DDOWN,          0},
    {VD_FW__SYM_LEN("dpleft"),              VD_FW_GAMEPAD_DLEFT,          0},
    {VD_FW__SYM_LEN("dpright"),             VD_FW_GAMEPAD_DRIGHT,         0},
    {VD_FW__SYM_LEN("leftshoulder"),        VD_FW_GAMEPAD_LEFT_SHOULDER,  0},
    {VD_FW__SYM_LEN("leftstick"),           VD_FW_GAMEPAD_LEFT_STICK,     0},
    {VD_FW__SYM_LEN("lefttrigger"),         VD_FW_GAMEPAD_LT,             1},
    {VD_FW__SYM_LEN("rightshoulder"),       VD_FW_GAMEPAD_RIGHT_SHOULDER, 0},
    {VD_FW__SYM_LEN("rightstick"),          VD_FW_GAMEPAD_RIGHT_STICK,    0},
    {VD_FW__SYM_LEN("righttrigger"),        VD_FW_GAMEPAD_RT,             1},
    {VD_FW__SYM_LEN("leftx"),               VD_FW_GAMEPAD_LH,             1},
    {VD_FW__SYM_LEN("lefty"),               VD_FW_GAMEPAD_LV,             1},
    {VD_FW__SYM_LEN("rightx"),              VD_FW_GAMEPAD_RH,             1},
    {VD_FW__SYM_LEN("righty"),              VD_FW_GAMEPAD_RV,             1},
    {VD_FW__SYM_LEN("lpad0"),               VD_FW_GAMEPAD_LEFT_PAD0,      0},
    {VD_FW__SYM_LEN("rpad0"),               VD_FW_GAMEPAD_RIGHT_PAD0,     0},
    {VD_FW__SYM_LEN("lpad1"),               VD_FW_GAMEPAD_LEFT_PAD1,      0},
    {VD_FW__SYM_LEN("rpad1"),               VD_FW_GAMEPAD_RIGHT_PAD1,     0},
    {VD_FW__SYM_LEN("lpad2"),               VD_FW_GAMEPAD_LEFT_PAD2,      0},
    {VD_FW__SYM_LEN("rpad2"),               VD_FW_GAMEPAD_RIGHT_PAD2,     0},
    {VD_FW__SYM_LEN("aux0"),                VD_FW_GAMEPAD_AUX0,           0},
    {VD_FW__SYM_LEN("aux1"),                VD_FW_GAMEPAD_AUX1,           0},
    {VD_FW__SYM_LEN("aux2"),                VD_FW_GAMEPAD_AUX2,           0},
    {VD_FW__SYM_LEN("aux3"),                VD_FW_GAMEPAD_AUX3,           0},
    {VD_FW__SYM_LEN("aux4"),                VD_FW_GAMEPAD_AUX4,           0},
    {VD_FW__SYM_LEN("aux5"),                VD_FW_GAMEPAD_AUX5,           0},
    {VD_FW__SYM_LEN("aux6"),                VD_FW_GAMEPAD_AUX6,           0},
    {VD_FW__SYM_LEN("aux7"),                VD_FW_GAMEPAD_AUX7,           0},
    {VD_FW__SYM_LEN("aux8"),                VD_FW_GAMEPAD_AUX8,           0},
    {VD_FW__SYM_LEN("aux9"),                VD_FW_GAMEPAD_AUX9,           0},
#undef VD_FW__SYM_LEN
};

static VdFw__GamepadSymbolToTarget *vd_fw__get_map_from_symbol(const char *s, int s_len)
{
    int map_count = sizeof(Vd_Fw__Gamepad_Symbols_To_Targets) / sizeof(Vd_Fw__Gamepad_Symbols_To_Targets[0]);

    for (int map_index = 0; map_index < map_count; ++map_index) {
        VdFw__GamepadSymbolToTarget *check = &Vd_Fw__Gamepad_Symbols_To_Targets[map_index];
        if (check->len != s_len) {
            continue;
        }

        int found = 1;
        for (int i = 0; i < check->len; ++i) {
            if (check->sym[i] != s[i]) {
                found = 0;
                break;
            }
        }

        if (found) {
            return check;
        }
    }

    return 0;
}

static int vd_fw__parse_hex_byte(const char *s, int i, VdFwU8 *out)
{
    VdFwU8 hi_nibble;
    VdFwU8 lo_nibble;

    if (s[i] >= '0' && s[i] <= '9') {
        hi_nibble = s[i] - '0';
    } else if (s[i] >= 'a' && s[i] <= 'f') {
        hi_nibble = s[i] - 'a' + 0xa;
    } else if (s[i] >= 'A' && s[i] <= 'F') {
        hi_nibble = s[i] - 'A' + 0xa;
    } else {
        return 0;
    }

    if (s[i+1] >= '0' && s[i+1] <= '9') {
        lo_nibble = s[i+1] - '0';
    } else if (s[i+1] >= 'a' && s[i+1] <= 'f') {
        lo_nibble = s[i+1] - 'a' + 0xa;
    } else if (s[i+1] >= 'A' && s[i+1] <= 'F') {
        lo_nibble = s[i+1] - 'A' + 0xa;
    } else {
        return 0;
    }

    *out = hi_nibble * 16 + lo_nibble;
    return 1;
}

VD_FW_API int vd_fw_gamepad_map_entry_is_none(VdFwGamepadMapEntry *entry)
{
    return (entry->kind & VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_MASK) == VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_NONE;    
}

VD_FW_API int vd_fw_add_gamepad_db_entry(VdFwGamepadDBEntry *entry)
{
    int will_resize = 0;
    int cap = VD_FW_G.cap_gamepad_db_entries;
    if (cap == 0) {
        will_resize = 1;
        cap = 32;
    }

    if ((VD_FW_G.num_gamepad_db_entries + 1) >= cap) {
        cap *= 2;
        will_resize = 1;
    }

    if (will_resize) {
        VD_FW_G.gamepad_db_entries = (VdFwGamepadDBEntry*)vd_fw__realloc_mem(VD_FW_G.gamepad_db_entries, 
                                                                             sizeof(VD_FW_G.gamepad_db_entries[0]) * cap);
        VD_FW_G.cap_gamepad_db_entries = cap;
    }

    if (VD_FW_G.has_initialized) vd_fw__lock_gamepaddb();
    VD_FW_G.gamepad_db_entries[VD_FW_G.num_gamepad_db_entries++] = *entry;
    if (VD_FW_G.has_initialized) vd_fw__unlock_gamepaddb();
    vd_fw__notify_gamepaddb_changed();
    return 1;
}

VD_FW_API void vd_fw_add_gamepad_rgcdb(const char *text, int text_len)
{
    int i = 0;
    while (i < text_len) {

        int line_start = i;

        // Read Line
        while ((i < text_len) && (text[i] != '\r' && text[i] != '\n')) {
            i++;
        }
        int line_end = i;

        const char *entry_to_parse = text + line_start;
        int entry_to_parse_len = line_end - line_start;
        VdFwGamepadDBEntry entry;
        VdFwPlatform platform;
        const char *name;
        if (!vd_fw_parse_gamepad_db_entry(entry_to_parse, entry_to_parse_len,
                                         &entry, &platform, &name))
        {
            continue;
        }

        if (platform != vd_fw_get_platform()) {
            continue;
        }

        vd_fw_add_gamepad_db_entry(&entry);

        while ((i < text_len) && (text[i] == '\r' || text[i] == '\n')) {
            i++;
        }
    }
}

VD_FW_API int vd_fw_parse_gamepad_db_entry(const char *s, int s_len, VdFwGamepadDBEntry *out,
                                           VdFwPlatform *out_platform, const char **out_begin_name)
{
    int i = 0;
    // @todo(mdodis): somehow, we skip some values. Fix this.
    if (s_len < 32) {
        return 0;
    }

    for (i = 0; i < 32; i += 2) {
        unsigned char byte;
        if (!vd_fw__parse_hex_byte(s, i, &byte)) {
            return 0;
        }

        out->guid.dat[i / 2] = byte;
    }

    if (i > s_len || s[i] != ',') {
        return 0;
    }
    i++;

    // Skip device name
    *out_begin_name = &s[i];
    while ((i < s_len) && (s[i] != ',')) i++;
    if (i++ > s_len) return 0;

#define VD_FW_STR_AND_LEN(cs) cs, (sizeof(cs) - 1) 
#define VD_FW_EXPECT_COLON() do { if (s[i] != ':') return 0; if (++i >= s_len) return 0; } while(0)

    int mapping_count = 0;

    // Parse Entries
    while (i < s_len) {
        VdFwGamepadMapEntry map_entry = {0};

        // Handle sign before target assignment
        // This happens
        char partwise_sign = 0;
        if ((s[i] == '+') || (s[i] == '-')) {
            partwise_sign = s[i];
            i++;
        }

        // Read word
        int word_start = i;
        while ((i < s_len) && vd_fw__is_db_symbol(s[i])) {
            i++;
        }
        int word_end = i;

        VdFw__GamepadSymbolToTarget *sym = vd_fw__get_map_from_symbol(s + word_start, word_end - word_start);
        int did_map_input = 0;

        if (sym != 0) {
            VD_FW_EXPECT_COLON();
            map_entry.target = sym->tgt;
            did_map_input = 1;
        } else if (vd_fw__compare_string(s, s_len, word_start, VD_FW_STR_AND_LEN("platform"))) {
            VD_FW_EXPECT_COLON();
            int platform_begin = i;
            {
                while ((i < s_len) && (((s[i] >= 'a') && (s[i] <= 'z')) ||
                                       ((s[i] >= 'A') && (s[i] <= 'Z')) ||
                                       (s[i] == ' ')))
                {
                    i++;
                }

                if (vd_fw__compare_string(s, s_len, platform_begin, VD_FW_STR_AND_LEN("Windows"))) {
                    *out_platform = VD_FW_PLATFORM_WINDOWS;
                } else if (vd_fw__compare_string(s, s_len, platform_begin, VD_FW_STR_AND_LEN("Linux"))) {
                    *out_platform = VD_FW_PLATFORM_LINUX;
                } else if (vd_fw__compare_string(s, s_len, platform_begin, VD_FW_STR_AND_LEN("Mac OS X"))) {
                    *out_platform = VD_FW_PLATFORM_MACOS;
                } else if (vd_fw__compare_string(s, s_len, platform_begin, VD_FW_STR_AND_LEN("Android"))) {
                    *out_platform = VD_FW_PLATFORM_ANDROID;
                } else if (vd_fw__compare_string(s, s_len, platform_begin, VD_FW_STR_AND_LEN("iOS"))) {
                    *out_platform = VD_FW_PLATFORM_IOS;
                } else {
                    *out_platform = VD_FW_PLATFORM_UNKNOWN;
                }
            }
        } else if (vd_fw__compare_string(s, s_len, word_start, VD_FW_STR_AND_LEN("rumble"))) {
            VD_FW_EXPECT_COLON();
            // Format
            // <mode char><prefix as hex><ll><mm>

            if (s[i] == 'w') {
                out->map.rumble_config.type = VD_FW_GAMEPAD_RUMBLE_TYPE_RAW;
                i++;
            }

            VdFwU8 byte_count = 0;
            while ((i + 1) < s_len) {

                VdFwU8 byte;
                if (!vd_fw__parse_hex_byte(s, i, &byte)) {
                    break;
                }

                out->map.rumble_config.prefix[byte_count++] = byte;
                i += 2;
            }
            out->map.rumble_config.prefix_len = byte_count;

            while ((i + 1) < s_len) {

                if ((s[i] == 'l') && (s[i + 1] == 'l')) {
                    out->map.rumble_config.dat.raw.rumble_lo.parts.offset      = byte_count++;
                    out->map.rumble_config.dat.raw.rumble_lo.parts.byte_length = 1;
                    i += 2;
                    continue;
                }

                if ((s[i] == 'h') && (s[i + 1] == 'h')) {
                    out->map.rumble_config.dat.raw.rumble_hi.parts.offset      = byte_count++;
                    out->map.rumble_config.dat.raw.rumble_hi.parts.byte_length = 1;
                    i += 2;
                    continue;
                }

                break;
            }
        } else if (vd_fw__compare_string(s, s_len, word_start, VD_FW_STR_AND_LEN("face"))) {
            VD_FW_EXPECT_COLON();
            int face_start = i;
            while ((i < s_len) && vd_fw__is_db_symbol(s[i])) {
                i++;
            }
            int face_end = i;

            int c = vd_fw__parse_map_face(s + face_start, face_end - face_start, &out->map.face);

            if (c == 0) {
                return 0;
            }
        } else if (vd_fw__compare_string(s, s_len, word_start, VD_FW_STR_AND_LEN("class"))) {
            VD_FW_EXPECT_COLON();
            int class_start = i;
            while ((i < s_len) && vd_fw__is_db_symbol(s[i])) {
                i++;
            }
            int class_end = i;

            int c = vd_fw__parse_map_class(s + class_start, class_end - class_start, &out->map.klass);

            if (c == 0) {
                return 0;
            }
        } else {
            i++;
        }

        if (did_map_input) {
            int c = vd_fw__parse_map_entry(s + i, s_len - i, &map_entry);
            if (c == 0) {
                return 0;
            }

            if (((map_entry.kind & VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_MASK) == VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS) &&
                (!sym->is_axis))
            {
                map_entry.kind |= VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_AXIS_TO_BUTTON;
            }

            if (((map_entry.kind & VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_MASK) == VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON) &&
                (sym->is_axis))
            {
                map_entry.kind |= VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_BUTTON_TO_AXIS;
            }

            if (partwise_sign == '+') {
                map_entry.kind |= VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_PARTWISE;
            } else if (partwise_sign == '-') {
                map_entry.kind |= VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_PARTWISE | VD_FW_GAMEPAD_MAPPING_SOURCE_FLAG_INVERTED;
            }

            out->map.mappings[mapping_count++] = map_entry;

            i += c;
        }

        while ((i < s_len) && (s[i] != ',')) i++;
        i++;
    }

#undef VD_FW_EXPECT_COLON
#undef VD_FW_STR_AND_LEN
    return 1;
}

#ifdef __clang__
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wmissing-braces"
#endif
#if VD_FW_GAMEPAD_DB_DEFAULT
#if defined(VD_FW_GAMEPAD_DB_DEFAULT_EXTERNAL)
#include "builtin.rgcdb.c"
#else
VdFwGamepadDBEntry Vd_Fw__Gamepad_Db_Entries[3] = {
    /*Sony DualShock 4*/
    {{0x03,0x00,0xd0,0x42,0x4c,0x05,0x00,0x00,0xcc,0x09,0x00,0x00,0x00,0x01,0x72,0x00},
    {
        {{0x1,VD_FW_GAMEPAD_A,1},{0x1,VD_FW_GAMEPAD_B,2},{0x1,VD_FW_GAMEPAD_X,0},{0x1,VD_FW_GAMEPAD_Y,3},{0x3,VD_FW_GAMEPAD_LH,2},{0x3,VD_FW_GAMEPAD_LV,3},{0x3,VD_FW_GAMEPAD_RH,4},{0x3,VD_FW_GAMEPAD_RV,7},{0x3,VD_FW_GAMEPAD_LT,5},{0x3,VD_FW_GAMEPAD_RT,6},{0x1,VD_FW_GAMEPAD_L1,4},{0x1,VD_FW_GAMEPAD_R1,5},{0x1,VD_FW_GAMEPAD_SELECT,8},{0x1,VD_FW_GAMEPAD_START,9},{0x2,VD_FW_GAMEPAD_DUP,1},{0x2,VD_FW_GAMEPAD_DRIGHT,2},{0x2,VD_FW_GAMEPAD_DDOWN,4},{0x2,VD_FW_GAMEPAD_DLEFT,8},{0x1,VD_FW_GAMEPAD_L3,10},{0x1,VD_FW_GAMEPAD_R3,11},{0,0,0},},
        {
            VD_FW_GAMEPAD_RUMBLE_TYPE_RAW,
            4,
            {0x05,0xff,0x00,0x00,},
            {0x00010004,0x00010005},
        },
        VD_FW_GAMEPAD_FACE_PLAYSTATION,
        VD_FW_GAMEPAD_CLASS_XBOX,
    }},
    /*Xbox One Controller for Windows*/
    {{0x03,0x00,0x93,0x8d,0x5e,0x04,0x00,0x00,0xff,0x02,0x00,0x00,0x00,0x00,0x72,0x00},
    {
        {{0x1,VD_FW_GAMEPAD_A,0},{0x1,VD_FW_GAMEPAD_B,1},{0x1,VD_FW_GAMEPAD_X,2},{0x1,VD_FW_GAMEPAD_Y,3},{0x3,VD_FW_GAMEPAD_LH,0},{0x3,VD_FW_GAMEPAD_LV,1},{0x3,VD_FW_GAMEPAD_RH,2},{0x3,VD_FW_GAMEPAD_RV,3},{0x3,VD_FW_GAMEPAD_LT,4},{0x3,VD_FW_GAMEPAD_RT,4},{0x1,VD_FW_GAMEPAD_L1,4},{0x1,VD_FW_GAMEPAD_R1,5},{0x2,VD_FW_GAMEPAD_DUP,1},{0x2,VD_FW_GAMEPAD_DRIGHT,2},{0x2,VD_FW_GAMEPAD_DDOWN,4},{0x2,VD_FW_GAMEPAD_DLEFT,8},{0x1,VD_FW_GAMEPAD_SELECT,6},{0x1,VD_FW_GAMEPAD_START,7},{0x1,VD_FW_GAMEPAD_L3,8},{0x1,VD_FW_GAMEPAD_R3,9},{0,0,0},},
        {
            VD_FW_GAMEPAD_RUMBLE_TYPE_NOT_AVAILABLE,
        },
        VD_FW_GAMEPAD_FACE_XBOX,
        VD_FW_GAMEPAD_CLASS_XBOX,
    }},
    /*8BitDo Ultimate 2C (Wired) (RawInput)*/
    {{0x03,0x00,0x39,0x19,0xc8,0x2d,0x00,0x00,0x1d,0x30,0x00,0x00,0x01,0x00,0x72,0x00},
    {
        {{0x1,VD_FW_GAMEPAD_A,0},{0x1,VD_FW_GAMEPAD_B,1},{0x1,VD_FW_GAMEPAD_X,3},{0x1,VD_FW_GAMEPAD_Y,4},{0x3,VD_FW_GAMEPAD_LH,1},{0x3,VD_FW_GAMEPAD_LV,2},{0x3,VD_FW_GAMEPAD_RH,3},{0x3,VD_FW_GAMEPAD_RV,4},{0x1,VD_FW_GAMEPAD_L1,6},{0x1,VD_FW_GAMEPAD_R1,7},{0x1,VD_FW_GAMEPAD_SELECT,10},{0x1,VD_FW_GAMEPAD_START,11},{0x5,VD_FW_GAMEPAD_LT,8},{0x5,VD_FW_GAMEPAD_RT,9},{0x2,VD_FW_GAMEPAD_DUP,1},{0x2,VD_FW_GAMEPAD_DRIGHT,2},{0x2,VD_FW_GAMEPAD_DDOWN,4},{0x2,VD_FW_GAMEPAD_DLEFT,8},{0x1,VD_FW_GAMEPAD_L3,13},{0x1,VD_FW_GAMEPAD_R3,14},{0,0,0},},
        {
            VD_FW_GAMEPAD_RUMBLE_TYPE_NOT_AVAILABLE,
        },
        VD_FW_GAMEPAD_FACE_XBOX,
        VD_FW_GAMEPAD_CLASS_XBOX,
    }},
};
#endif // VD_FW_GAMEPAD_DB_DEFAULT_EXTERNAL
#endif // VD_FW_GAMEPAD_DB_DEFAULT
#ifdef __clang__
#   pragma clang diagnostic pop
#endif

static int vd_fw__guid_matches(VdFwGuid *detected_guid, VdFwGuid *candidate_guid)
{
    return (detected_guid->parts.bus == candidate_guid->parts.bus) &&
           (detected_guid->parts.vendor_id == candidate_guid->parts.vendor_id) &&
           (detected_guid->parts.product_id == candidate_guid->parts.product_id);
}

VD_FW_API int vd_fw__map_gamepad(VdFwGuid guid, VdFwGamepadMap *map)
{
    VdFwGamepadDBEntry *db_entry = 0;
    vd_fw__lock_gamepaddb();
    for (int i = 0; i < VD_FW_G.num_gamepad_db_entries; ++i) {
        if (vd_fw__guid_matches(&guid, &VD_FW_G.gamepad_db_entries[i].guid)) {
            db_entry = &VD_FW_G.gamepad_db_entries[i];
            break;
        }
    }
    vd_fw__unlock_gamepaddb();

    if (db_entry) {
        *map = db_entry->map;
        return 1;
    }

#if VD_FW_GAMEPAD_DB_DEFAULT
    size_t default_db_count = VD_FW_ARRAY_COUNT(Vd_Fw__Gamepad_Db_Entries);
    for (size_t i = 0; i < default_db_count; ++i) {
        if (vd_fw__guid_matches(&guid, &Vd_Fw__Gamepad_Db_Entries[i].guid)) {
            db_entry = &Vd_Fw__Gamepad_Db_Entries[i];
            break;
        }
    }
#endif

    if (!db_entry) {
        return 0;
    }

    // @todo(mdodis): Only copy the actual count of maps we use
    *map = db_entry->map;
    return 1;
}



#if defined(__APPLE__)
#pragma clang diagnostic pop
#endif
#endif // VD_FW_IMPL