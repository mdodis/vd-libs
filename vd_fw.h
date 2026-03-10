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
 * FEATURE TRACKING
 * ╔═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╗
 * ║ Feature                                 ║ Win32   ║ X11     ║ Mac     ║                                           ║
 * ║═════════════════════════════════════════║═════════║═════════║═════════║═══════════════════════════════════════════║
 * ║ Normal Sizable Window                   ║ YES     ║ YES     ║ YES     ║                                           ║
 * ║ Borderless Sizable Window               ║ YES     ║ YES     ║ YES     ║                                           ║
 * ║ vd_fw_set_ncrects                       ║ YES     ║ YES     ║ YES     ║                                           ║
 * ║                                         ║         ║         ║         ║                                           ║
 * ║ OpenGL                                  ║         ║         ║         ║                                           ║
 * ║═════════════════════════════════════════║═════════║═════════║═════════║═══════════════════════════════════════════║
 * ║ Extensions                              ║ YES     ║ YES     ║ NO      ║                                           ║
 * ║ Config API                              ║ YES     ║ YES     ║ NO      ║                                           ║
 * ║                                         ║         ║         ║         ║                                           ║
 * ║ Vulkan                                  ║         ║         ║         ║                                           ║
 * ║═════════════════════════════════════════║═════════║═════════║═════════║═══════════════════════════════════════════║
 * ║ Basic Support                           ║ YES     ║ YES     ║ NO      ║                                           ║
 * ║                                         ║         ║         ║         ║                                           ║
 * ╚═══════════════════════════════════════════════════════════════════════════════════════════════════════════════════╝
 * 
 * TODO
 * - Gamepads
 *     - Face Heuristics
 *     - Class Heuristics
 * - Remove all weird CRT stuff, custom memcpy impls, allocations with HeapAlloc for example, move everything into
 *   re-definable macros like VD_FW_REALLOC  
 * - Win32: Use DeviceIoControl for XBOX controllers until they're correlated to XINPUT
 * - vd_fw_get_last_mouse_button_pressed
 * - Win32: Allow gamepad input even when window isn't focused?
 *     - Option for that
 * - Win32:
 *     - Filter monitor orientation and other settings in display modes
 *     - Sort the display modes if not sorted already
 * - Win32:
 *     - dpi change
 * - MacOS: Event Queue
 * - MacOS: vd_fw_get_key_released: Gets if the key was released this frame
 * - raw hat states
 * - OBS Studio breaks ChoosePixelFormat
 * - Make sure we can export functions properly for C++
 * - Expose customizable function pointer if the user needs to do something platform-specific before/after winthread has initialized or before vd_fw_init returns anyways.
 * - Clipboard
 * - Properly handle vd_fw_set_receive_ncmouse for clicks and scrolls
 * - Set mouse cursor to constants (resize, I, etc...)
 * - Have a way to store and load the window placement state (size, position, maximization state)
 * - Should vd_fw_set_receive_ncmouse be default 0 or 1?
 *   - Actually, consider removing it entirely
 * - set window unresizable
 * - X11: Fix moveresize leaving window unfocused
 * - X11: Fix dbl-click on borderless with ncrects initiating move instead of maximizing
 * - MacOS: simple flag to disable cocoa main thread requirement workaround
 * - MacOS: vd_fw_get_last_key_pressed
 * - MacOS: vd_fw_get_executable_dir()
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

#define VD_FW_FPI  3.14159265359f
#define VD_FW_FPI2 (2 * VD_FW_FPI)
#define VD_FW_FPIH (0.5f * VD_FW_FPI)

#ifndef VD_FW_SIN
#   include <math.h>
#   define VD_FW_SIN(x) sinf(x)
#endif // !VD_FW_SIN

#ifndef VD_FW_COS
#   include <math.h>
#   define VD_FW_COS(x) cosf(x)
#endif // !VD_FW_COS

#ifndef VD_FW_TAN
#   include <math.h>
#   define VD_FW_TAN(x) tanf(x)
#endif // !VD_FW_TAN

#ifndef VD_FW_SQRT
#   include <math.h>
#   define VD_FW_SQRT(x) sqrtf(x)
#endif // !VD_FW_SQRT

#ifndef VD_FW_MEMCPY
#   include <string.h>
#   define VD_FW_MEMCPY(dst, src, count) memcpy(dst, src, count)
#endif // !VD_FW_MEMCPY

#ifndef VD_FW_MEMSET
#   include <string.h>
#   define VD_FW_MEMSET(dst, val, num) memset(dst, val, num)
#endif // !VD_FW_MEMSET

#ifndef VD_FW_REALLOC
#   include <stdlib.h>
#   define VD_FW_REALLOC(pprev, psize, nsize) realloc(pprev, nsize)
#endif // !VD_FW_REALLOC

#ifndef VD_FW_FREE
#   include <stdlib.h>
#   define VD_FW_FREE(pprev, psize) free(pprev)
#endif // !VD_FW_FREE

#ifndef VD_FW_LOG
#   define VD_FW_LOG(fmt, ...)
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
#   elif defined(__APPLE__)
#       define VD_FW_ENDIANNESS VD_FW_ENDIANNESS_LE
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
#   define VdFwI64  int64_t
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

#ifndef VD_FW_EVENT_COUNT_MAX
#   define VD_FW_EVENT_COUNT_MAX 32
#endif // !VD_FW_EVENT_COUNT_MAX

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
    const char *name;
    int        available;
} VdFwGlExtension;

enum {
    VD_FW_GL_DONT_CARE             = 0,
    VD_FW_GL_PIXEL_FORMAT_R8G8B8A8 = 1,
    VD_FW_GL_PIXEL_FORMAT_R8G8B8   = 2,

    VD_FW_GL_DEPTH_FORMAT_D32      = 1,
    VD_FW_GL_DEPTH_FORMAT_D24S8    = 2,

    VD_FW_GL_MSAA_DISABLED         = 0,
    VD_FW_GL_MSAA_ENABLED_2X       = 2,
    VD_FW_GL_MSAA_ENABLED_4X       = 4,
    VD_FW_GL_MSAA_ENABLED_8X       = 8,
};

typedef struct {
    VdFwGlVersion   version;
    int             pixel_format;
    int             depth_format;
    int             msaa;
    int             debug;
    int             compat;
    VdFwGlExtension *req_extensions;
    VdFwGlExtension *opt_extensions;
} VdFwGlConfig;

typedef struct {
    int           selected_config;
    VdFwGlConfig  *configs;
} VdFwOpenGLOptions;

typedef struct {
    /* The graphics API you're planning to use. Defaulted to OpenGL. */
    VdFwGraphicsApi     api;

    VdFwOpenGLOptions   gl;

    struct {
        /* Set to 1 to disable window frame. */
        int             borderless;
        /* Set to 1 to block while sizing. */
        int             block_while_sizing;
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
 * @brief Acquire lock to the window buffer for drawing
 */
VD_FW_API void               vd_fw_lock(void);

/**
 * @brief Swap buffers and release lock to the window buffer after drawing
 */
VD_FW_API void               vd_fw_unlock(void);

/**
 * @brief Get whether the system will block while sizing the window
 * @return  Boolean value
 */
VD_FW_API int                vd_fw_get_block_while_sizing(void);

/**
 * @brief Set whether the system will block while sizing the window
 * @param  on Whether to block while sizing
 */
VD_FW_API void               vd_fw_set_block_while_sizing(int on);

/**
 * @brief Close the window and end the rendering loop
 */
VD_FW_API void               vd_fw_quit(void);

/**
 * @brief Deinitialize library
 */
VD_FW_API void               vd_fw_exit(void);

/* ----EVENTS-------------------------------------------------------------------------------------------------------- */
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

    VD_FW_MOD_SHIFT   = 1 << 0,
    VD_FW_MOD_CONTROL = 1 << 1,
    VD_FW_MOD_ALT     = 1 << 1,
};
typedef int VdFwKey;

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

enum VD_FW_WINDOW_STATE_ {
    VD_FW_WINDOW_STATE_MINIMIZED = 1 << 0,
    VD_FW_WINDOW_STATE_MAXIMIZED = 1 << 1,
};
typedef int VdFwWindowState;

typedef enum {
    VD_FW_EVENT_TYPE_NONE = 0,
    VD_FW_EVENT_TYPE_CLOSE_REQUEST,
    VD_FW_EVENT_TYPE_FOCUS_CHANGE,
    VD_FW_EVENT_TYPE_KEY_DOWN,
    VD_FW_EVENT_TYPE_KEY_UP,
    VD_FW_EVENT_TYPE_CHARACTER,
    VD_FW_EVENT_TYPE_MOUSE_MOVE,
    VD_FW_EVENT_TYPE_MOUSE_DELTA,
    VD_FW_EVENT_TYPE_MOUSE_BUTTON_DOWN,
    VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP,
    VD_FW_EVENT_TYPE_MOUSE_SCROLL,
    VD_FW_EVENT_TYPE_WINDOW_STATE_CHANGE,
    VD_FW_EVENT_TYPE_SCALE_CHANGE,
} VdFwEventType;

typedef struct {
    int _temp;
} VdFwEventCloseRequestData;

typedef struct {
    int got_focus;
} VdFwEventFocusChangeData;

typedef struct {
    VdFwKey key;
    int     repeat;
    int     modifiers;
} VdFwEventKeyDownData;

typedef struct {
    VdFwKey key;
} VdFwEventKeyUpData;

typedef struct {
    unsigned int codepoint;
} VdFwEventCharacter;

typedef struct {
    int x, y;
} VdFwEventMouseMoveData;

typedef struct {
    float dx, dy;
} VdFwEventMouseDeltaData;

typedef struct {
    int button;
} VdFwEventMouseButtonDownData;

typedef struct {
    int button;
} VdFwEventMouseButtonUpData;

typedef struct {
    float dx, dy;
} VdFwEventMouseScrollData;

typedef struct {
    int flag;
    int value;
} VdFwEventWindowStateChangeData;

typedef struct {
    int connected;
} VdFwEventGamepadStatusData;

typedef struct {
    float new_scale;
} VdFwEventScaleChangeData;

typedef union {
    VdFwEventCloseRequestData      close_request;
    VdFwEventFocusChangeData       focus_change;
    VdFwEventKeyDownData           key_down;
    VdFwEventKeyUpData             key_up;
    VdFwEventCharacter             character;
    VdFwEventMouseMoveData         mouse_move;
    VdFwEventMouseDeltaData        mouse_delta;
    VdFwEventMouseButtonDownData   mouse_button_down;
    VdFwEventMouseButtonUpData     mouse_button_up;
    VdFwEventMouseScrollData       mouse_scroll;
    VdFwEventWindowStateChangeData window_state_change;
    VdFwEventScaleChangeData       scale_change;
} VdFwEventData;

typedef struct {
    VdFwEventType type;
    VdFwEventData data;
} VdFwEvent;

/**
 * @brief Poll for events. Call this every frame, even if you don't use them.
 * @param  count The count of events available
 * @return The event buffer
 */
VD_FW_API VdFwEvent*         vd_fw_poll(int *count);

/**
 * @brief Get if the user requested to close the window
 * @return  Whether the user tried to close the window this frame
 */
VD_FW_API int                vd_fw_close_requested(void);

/**
 * @brief Get the current platform.
 * @return  The current platform.
 */
VD_FW_API VdFwPlatform       vd_fw_get_platform(void);

/**
 * @brief Switch the current graphics API (must not be called between vd_fw_lock and vd_fw_unlock)
 * @param  api        The new API to use
 * @param  gl_options If api is VD_FW_GRAPHICS_API_OPENGL, the options for OpenGL
 * @return  Whether changing API was successful. For OpenGL: No = no config could be selected
 */
VD_FW_API int                vd_fw_set_graphics_api(VdFwGraphicsApi api, VdFwOpenGLOptions *gl_options);

/* ----WINDOW-------------------------------------------------------------------------------------------------------- */
/**
 * @brief Get the size of the window, in pixels
 * @param  w The width of the window, in pixels
 * @param  h The height of the window, in pixels
 * @return   1 if the size changed this frame, 0 otherwise
 */
VD_FW_API int                vd_fw_get_size(int *w, int *h);

/**
 * @brief Set the window size, in pixels
 * @param  w The width of the window, in pixels
 * @param  h The height of the window, in pixels
 */
VD_FW_API void               vd_fw_set_size(int w, int h);

/**
 * @brief Get whether to allow the user to change the window size or maximize it.
 * @return  Whether the user is allowed to change the window size or maximize it.
 */
VD_FW_API int                vd_fw_get_resizable(void);

/**
 * @brief Set whether to allow the user to change the window size or maximize it.
 * @return  Whether the user is allowed to change the window size or maximize it.
 */
VD_FW_API void               vd_fw_set_resizable(int on);

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
VD_FW_API void               vd_fw_set_minimized(void);

/**
 * @brief Get if the window is maximized
 * @param  maximized Whether the window is maximized
 * @return 1 if the maximization state of the window was changed
 */
VD_FW_API int                vd_fw_get_maximized(int *maximized);

/**
 * @brief Maximize the window
 */
VD_FW_API void               vd_fw_set_maximized(void);

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
 * @param  scale Pointer to the scale factor (1.0f: 1:1 scale, 2.0f, 2:1 scale, etc...)
 * @return  Whether the scale has changed this frame
 */
VD_FW_API int                vd_fw_get_scale(float *scale);

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
 * @brief Get the time (in nanoseconds) since the last call to vd_fw_poll
 * @return  The delta time (in nanoseconds)
 */
VD_FW_API unsigned long long vd_fw_delta_ns(void);

/**
 * @brief Get the time (in seconds) since the last call to vd_fw_poll
 * @return  The delta time (in seconds)
 */
VD_FW_INL float              vd_fw_delta_s(void);

/**
 * @brief Set the number of frames to sync on
 * @param  on Use: 0 for no sync, 1 for sync every frame and 2 for sync every other frame
 * @return 1 if the change was applied successfully
 */
VD_FW_API int                vd_fw_set_vsync_on(int on);
/* ----MONITORS------------------------------------------------------------------------------------------------------ */
typedef struct {
    int     width;
    int     height;
    int     frequency;
    struct {
        int numerator;
        int denominator;
    } aspect;
} VdFwDisplayMode;

/**
 * @brief Get the total count of monitors. Monitor 0 is the primary monitor of the display
 * @return  The count of all the monitors
 */
VD_FW_API int                vd_fw_get_monitor_count(void);

/**
 * @brief Get the monitor's friendly name via EDID
 * @param  index The monitor index
 * @return       The monitor name
 */
VD_FW_API const char*        vd_fw_get_monitor_name(int index);

/**
 * @brief (Warning: slow) Get the available display modes of the monitor
 * @param  index The monitor index
 * @param  count The total count of display modes
 * @return       Sorted display modes
 */
VD_FW_API VdFwDisplayMode*   vd_fw_get_monitor_display_modes(int index, int *count);

/* ----MOUSE--------------------------------------------------------------------------------------------------------- */

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

/**
 * @brief Get whether a key was just pressed this frame
 * @param  key The key to check
 * @return     Whether this key was pressed this frame
 */
VD_FW_API int                vd_fw_get_key_pressed(int key);

/**
 * @brief Get whether a key was just released this frame
 * @param  key The key to check
 * @return     Whether this key was released this frame
 */
VD_FW_API int                vd_fw_get_key_released(int key);

/**
 * @brief Get the last known state of this key
 * @param  key The key to check
 * @return     Whether this key is down currently
 */
VD_FW_API int                vd_fw_get_key_down(int key);

/**
 * @brief Get the last key pressed
 * @return  The last key pressed
 */
VD_FW_API int                vd_fw_get_last_key_pressed(void);

/**
 * @brief Convert key to string.
 * @param  k The key
 * @return   The key's name
 */
VD_FW_INL const char*        vd_fw_get_key_name(VdFwKey k);


/* ----VULKAN INTEGRATION-------------------------------------------------------------------------------------------- */
typedef void (*VdFwVkVoidFunction)(void);
typedef VdFwVkVoidFunction (*VdFwVkGetInstanceProcAddrProc)(void* instance, const char *name);

/**
 * @brief Set the vkGetInstanceProcAddr pointer. Call this before any function below
 * @param  proc The function
 */
VD_FW_API void               vd_fw_vk_set_get_instance_proc_addr(VdFwVkGetInstanceProcAddrProc proc);

/**
 * @brief Get the required instance extensions for WSI 
 * @param  count The count of the extensions
 * @return       The extension names to pass into vkCreateInstance
 */
VD_FW_API const char**       vd_fw_vk_wsi_instance_extensions(int *count);

/**
 * @brief Create WSI surface
 * @param  p_instance             Pointer to VkInstance
 * @param  p_allocation_callbacks Pointer to VkAllocationCallbacks
 * @param  p_surface              Pointer to VkSurfaceKHR
 * @return                        VkResult
 */
VD_FW_API int                vd_fw_vk_wsi_surface_create(void *p_instance, void *p_allocation_callbacks, void *p_surface);

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
 * @brief Convert a gamepad GUID to a C String
 * @param  guid The GUID
 * @param  out  The output buffer. Must be at least 32 bytes long
 */
VD_FW_API void               vd_fw_gamepad_guid_to_cstr(VdFwGuid *guid, char *out);

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
    float forward[3];
    forward[0] = target[0] - eye[0];
    forward[1] = target[1] - eye[1];
    forward[2] = target[2] - eye[2];

    float f_len = VD_FW_SQRT(forward[0]*forward[0] + forward[1]*forward[1] + forward[2]*forward[2]);
    forward[0]/=f_len; forward[1]/=f_len; forward[2]/=f_len;

    // Left vector
    float left[3];
    left[0] = updir[1]*forward[2] - updir[2]*forward[1];
    left[1] = updir[2]*forward[0] - updir[0]*forward[2];
    left[2] = updir[0]*forward[1] - updir[1]*forward[0];

    float l_len = VD_FW_SQRT(left[0]*left[0] + left[1]*left[1] + left[2]*left[2]);
    left[0]/=l_len; left[1]/=l_len; left[2]/=l_len;

    // Recompute up vector
    float up[3];
    up[0] = forward[1]*left[2] - forward[2]*left[1];
    up[1] = forward[2]*left[0] - forward[0]*left[2];
    up[2] = forward[0]*left[1] - forward[1]*left[0];

    // Column-major matrix
    out[0] = left[0];      out[4] = left[1];     out[8]  = left[2];     out[12] = - (left[0]    * eye[0] + left[1]    * eye[1] + left[2]    * eye[2]);
    out[1] = up[0];        out[5] = up[1];       out[9]  = up[2];       out[13] = - (up[0]      * eye[0] + up[1]      * eye[1] + up[2]      * eye[2]);
    out[2] = -forward[0];  out[6] = -forward[1]; out[10] = -forward[2]; out[14] =    forward[0] * eye[0] + forward[1] * eye[1] + forward[2] * eye[2];
    out[3] = 0.0f;         out[7] = 0.0f;        out[11] = 0.0f;        out[15] = 1.0f;
}

/* ----INTERNAL API-------------------------------------------------------------------------------------------------- */
#pragma pack(push, 1)
typedef struct {
    VdFwU8                                         horizontal_addressable_pixels;                      // (horizontal_addressable_pixels + 31) * 8 
    VdFwU8                                         field_refresh_rate : 6;
    VdFwU8                                         image_aspect_ratio : 2;
} VdFwEdid1_4StandardTiming;

typedef struct {
    // Detailed Timing Definitions - Part 1
    VdFwU16                                        pixel_clock;                                        // pixel_clock * 10000, 0x0000 --> This is a Display Descriptor
    VdFwU8                                         horizontal_addressable_pixels_lo;
    VdFwU8                                         horizontal_blanking_pixels_lo;
    VdFwU8                                         horizontal_addressable_pixels_hi : 4;
    VdFwU8                                         horizontal_blanking_pixels_hi : 4;
    VdFwU8                                         vertical_addressable_lines_lo;
    VdFwU8                                         vertical_blanking_lines_lo;
    VdFwU8                                         vertical_addressable_lines_hi : 4;
    VdFwU8                                         vertical_blanking_lines_hi : 4;
    VdFwU8                                         horizontal_front_porch_pixels_lo;
    VdFwU8                                         horizontal_sync_pulse_width_pixels_lo;
    VdFwU8                                         vertical_front_porch_lines_lo : 4;
    VdFwU8                                         vertical_sync_pulse_width_lines_lo : 4;

    // Bit Definitions
    VdFwU8                                         horizontal_front_porch_pixels_hi : 2;
    VdFwU8                                         horizontal_sync_pulse_width_pixels_hi : 2;
    VdFwU8                                         vertical_front_porch_lines_hi : 2;
    VdFwU8                                         vertical_sync_pulse_width_lines_hi : 2;

    // Video Image Size & Border Definitions
    VdFwU8                                         horizontal_addressable_video_image_size_mm_lo;
    VdFwU8                                         vertical_addressable_video_image_size_mm_lo;
    VdFwU8                                         horizontal_addressable_video_image_size_mm_hi : 4;
    VdFwU8                                         vertical_addressable_video_image_size_mm_hi : 4;
    VdFwU8                                         right_left_horizontal_border_pixels;
    VdFwU8                                         top_bottom_vertical_border_lines;

    // Detailed Timing Definitions - Part 2
    VdFwU8                                         signal_interface_type : 1;
    VdFwU8                                         stereo_viewing_support_hi : 2;
    VdFwU8                                         a_d_sync_signal_definitions : 4;
    VdFwU8                                         stereo_viewing_support_lo : 1;
} VdFwEdid1_4DetailedTiming;

typedef struct {
    VdFwU8                                         bytes[13];
} VdFwEdid1_4DisplayDescriptorDataOpaque;

typedef struct {
    char                                           serial_number[13];
} VdFwEdid1_4DisplayProductSerialNumber;

typedef struct {
    char                                           name[13];
} VdFwEdid1_4DisplayProductName;

typedef union {
    VdFwEdid1_4DisplayDescriptorDataOpaque         opaque;         // 13 bytes
    VdFwEdid1_4DisplayProductSerialNumber          serial_number;  // 0xFF
    VdFwEdid1_4DisplayProductName                  product_name;   // 0xFC
} VdFwEdid1_4DisplayDescriptorDataBlock;

typedef struct {
    VdFwU16                                        zeroes;
    VdFwU8                                         zero;
    VdFwU8                                         tag;
    VdFwU8                                         zero_or_display_range_limits_descriptor; 
    VdFwEdid1_4DisplayDescriptorDataBlock          data_block;
} VdFwEdid1_4DisplayDescriptor;

typedef union {
    VdFwEdid1_4DetailedTiming                      detailed_timing;
    VdFwEdid1_4DisplayDescriptor                   display_descriptor;
} VdFwEdid1_4DataBlock;

typedef struct {
    VdFwU8                                         digital_signal_bit : 1;
    VdFwU8                                         signal_level_standard : 2;
    VdFwU8                                         video_setup : 1;
    VdFwU8                                         sync_types : 3;
    VdFwU8                                         serrations : 1;
} VdFwEdid1_4AnalogVideoSignalDefinition;

typedef struct {
    VdFwU8                                         digital_signal_bit : 1;
    VdFwU8                                         color_bit_depth : 3;
    VdFwU8                                         video_interface_standard_supported : 4;
} VdFwEdid1_4DigitalVideoSignalDefinition;

typedef struct {
    VdFwU8                                         digital_signal_bit : 1;
    VdFwU8                                         rest : 7;
} VdFwEdid1_4UndeterminedVideoSignalDefinition;

typedef union {
    VdFwEdid1_4UndeterminedVideoSignalDefinition    general;
    VdFwEdid1_4AnalogVideoSignalDefinition          analog;
    VdFwEdid1_4DigitalVideoSignalDefinition         digital;
} VdFwEdid1_4VideoInputDefinition;

typedef struct {
    VdFwU8                                         header[8];                                      // 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00

    // Vendor & Product Identification
    VdFwU16                                        id_manufacturer_name;                           // Section 3.4.1
    VdFwU16                                        id_product_code;                                // Section 3.4.2
    VdFwU32                                        id_serial_number;                               // Section 3.4.3
    VdFwU8                                         week_of_manufacture;                            // Section 3.4.4
    VdFwU8                                         year_of_manufacture;                            // Section 3.4.4

    // EDID Structure Version & Revision
    VdFwU8                                         version;
    VdFwU8                                         revision;

    // Basic Display Parameters
    VdFwEdid1_4VideoInputDefinition                video_input_definition;                         // Section 3.6.1
    VdFwU8                                         horizontal_image_size_or_aspect;                // cm.
    VdFwU8                                         vertical_image_size_or_aspect;                  // cm.
    VdFwU8                                         display_transfer_characteristic;                // Gamma
    VdFwU8                                         feature_support;                                // Section 3.6.4

    // Color Characteristics
    VdFwU8                                         red_green_low_order_bits;
    VdFwU8                                         blue_white_low_order_bits;
    VdFwU8                                         red_x_high_order_bits;
    VdFwU8                                         red_y_high_order_bits;
    VdFwU8                                         green_x_high_order_bits;
    VdFwU8                                         green_y_high_order_bits;
    VdFwU8                                         blue_x_high_order_bits;
    VdFwU8                                         blue_y_high_order_bits;
    VdFwU8                                         white_x_high_order_bits;
    VdFwU8                                         white_y_high_order_bits;

    // Established Timings
    VdFwU8                                         established_timings_1;
    VdFwU8                                         established_timings_2;
    VdFwU8                                         manufacturer_reserved_timings;

    // Standard Timings: Identification 1 -> 8
    VdFwEdid1_4StandardTiming                      standard_timings[8];
    
    // 18 Byte Data Blocks
    VdFwEdid1_4DataBlock                           data_blocks[4];

    VdFwU8                                         extension_block_count;
    VdFwU8                                         checksum;

} VdFwEdid1_4;
#pragma pack(pop)

VD_FW_API int      vd_fw__any_time_higher(int num_files, const char **files, unsigned long long *check_against);
VD_FW_API char*    vd_fw__debug_dump_file_text(const char *path, size_t *size);
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
VD_FW_INL VdFwU32  vd_fw__gcd(VdFwU32 a, VdFwU32 b);
VD_FW_INL int      vd_fw__strcmp(const char *a, const char *b);
VD_FW_INL int      vd_fw__utf8_to_utf32(unsigned char *buf, size_t len, VdFwU32 *codepoint);

VD_FW_INL int vd_fw__compare_string_wide_nullsep_case_insensitive(const wchar_t *str1, const wchar_t *str2);

VD_FW_INL void vd_fw__sort_display_modes(VdFwDisplayMode *modes, int count);

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

VD_FW_INL int vd_fw__compare_string_wide_nullsep_case_insensitive(const wchar_t *str1, const wchar_t *str2)
{
    // @todo(mdodis): LCMapStringEx for proper case insensitivity

    while ((*str1) && (*str2)) {

        wchar_t str1_lower, str2_lower;
        str1_lower = *str1;
        str2_lower = *str2;

        if ((str1_lower >= L'A') && (str1_lower <= 'Z')) {
            str1_lower = str1_lower - L'A' + L'a';
        }

        if ((str2_lower >= L'A') && (str2_lower <= 'Z')) {
            str2_lower = str2_lower - L'A' + L'a';
        }


        if (str1_lower != str2_lower) {
            return 0;
        }

        str1++;
        str2++;
    }

    if ((*str1) != (*str2)) {
        return 0;
    }

    return 1;
}

VD_FW_INL VdFwU32 vd_fw__gcd(VdFwU32 a, VdFwU32 b)
{
    while (a != b) {
        if (a > b) {
            a -= b;
        } else {
            b -= a;
        }
    }

    return a;
}

VD_FW_INL int vd_fw__utf8_to_utf32(unsigned char *buf, size_t len, VdFwU32 *codepoint)
{
    if (len == 0) return -1;

    unsigned char b0 = buf[0];

    // 1-byte sequence
    if (b0 <= 0x7F) {
        *codepoint = b0;
        return 1;
    }

    // 2-byte sequence
    if ((b0 & 0xE0) == 0xC0) {
        if (len < 2) return -1;
        if ((buf[1] & 0xC0) != 0x80) return -1;

        *codepoint = ((b0 & 0x1F) << 6) |
                     (buf[1] & 0x3F);
        return 2;
    }

    // 3-byte sequence
    if ((b0 & 0xF0) == 0xE0) {
        if (len < 3) return -1;
        if ((buf[1] & 0xC0) != 0x80 ||
            (buf[2] & 0xC0) != 0x80) return -1;

        *codepoint = ((b0 & 0x0F) << 12) |
                     ((buf[1] & 0x3F) << 6) |
                     (buf[2] & 0x3F);
        return 3;
    }

    // 4-byte sequence
    if ((b0 & 0xF8) == 0xF0) {
        if (len < 4) return -1;
        if ((buf[1] & 0xC0) != 0x80 ||
            (buf[2] & 0xC0) != 0x80 ||
            (buf[3] & 0xC0) != 0x80) return -1;

        *codepoint = ((b0 & 0x07) << 18) |
                     ((buf[1] & 0x3F) << 12) |
                     ((buf[2] & 0x3F) << 6) |
                     (buf[3] & 0x3F);
        return 4;
    }

    return -1;
}

VD_FW_INL int vd_fw__strcmp(const char *a, const char *b)
{
    const unsigned char *p1 = (const unsigned char *)a;
    const unsigned char *p2 = (const unsigned char *)b;

    while (*p1 && *p1 == *p2 ) ++p1, ++p2;

    return (*p1 > *p2 ) - (*p2  > *p1);
}

VD_FW_INL int vd_fw__compare_display_mode(VdFwDisplayMode *a, VdFwDisplayMode *b)
{
    int a_area = a->width * a->height;
    int b_area = b->width * b->height;
    if (a_area > b_area) {
        return 1;
    } else if (a_area < b_area) {
        return -1;
    }

    if (a->frequency > b->frequency) {
        return 1;
    } else if (a->frequency < b->frequency) {
        return -1;
    }

    return 0;
}

VD_FW_INL void vd_fw__sort_display_modes_impl(VdFwDisplayMode *modes, int lo, int hi)
{
    if (lo < hi) {
        int p;

        {
            VdFwDisplayMode *pivot = &modes[hi];
            int i = lo - 1;

            for (int j = lo; j <= hi - 1; ++j) {
                if (vd_fw__compare_display_mode(&modes[j], pivot) < 0) {
                    i++;
                    VdFwDisplayMode temp = modes[i];
                    modes[i] = modes[j];
                    modes[j] = temp;
                }
            }

            VdFwDisplayMode temp = modes[i + 1];
            modes[i + 1] = modes[hi];
            modes[hi] = temp;
            p = i + 1;
        }

        vd_fw__sort_display_modes_impl(modes, lo, p - 1);
        vd_fw__sort_display_modes_impl(modes, p + 1, hi);
    }
} 

VD_FW_INL void vd_fw__sort_display_modes(VdFwDisplayMode *modes, int count)
{
    vd_fw__sort_display_modes_impl(modes, 0, count - 1);
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
typedef void *GLhandleARB;
#else
/* ----GL TYPEDEFS--------------------------------------------------------------------------------------------------- */
typedef unsigned int       GLhandleARB;
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
typedef void (*GLVULKANPROCNV)(void);
typedef GLintptr GLvdpauSurfaceNV;
typedef void (*GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef GLDEBUGPROC GLDEBUGPROCKHR;
typedef GLDEBUGPROC GLDEBUGPROCARB;
typedef GLDEBUGPROC GLDEBUGPROCAMD;
typedef GLint64 GLint64EXT;
typedef GLuint64 GLuint64EXT;
struct _cl_context;
struct _cl_event;
typedef GLchar GLcharARB;
typedef GLsizeiptr GLsizeiptrARB;
typedef GLintptr GLintptrARB;
typedef void *GLeglImageOES;
typedef void *GLeglClientBufferEXT;
typedef unsigned short GLhalfNV;

/* ----GL CONSTANTS-------------------------------------------------------------------------------------------------- */
#define GL_ALL_BARRIER_BITS 0xFFFFFFFF
#define GL_ALL_BARRIER_BITS_EXT 0xFFFFFFFF
#define GL_MULTISAMPLE_BUFFER_BIT7_QCOM 0x80000000
#define GL_ALL_ATTRIB_BITS 0xFFFFFFFF
#define GL_ALL_SHADER_BITS 0xFFFFFFFF
#define GL_ALL_SHADER_BITS_EXT 0xFFFFFFFF
#define GL_TEXCOORD4_BIT_PGI 0x80000000
#define GL_INVALID_INDEX 0xFFFFFFFF
#define GL_ALL_PIXELS_AMD 0xFFFFFFFF
#define GL_QUERY_ALL_EVENT_BITS_AMD 0xFFFFFFFF
#define GL_CLIENT_ALL_ATTRIB_BITS 0xFFFFFFFF

enum {
    /* GetPName */                                                                        /* InternalFormat */                                        /* AttributeType */
    GL_CURRENT_COLOR                                                           = 0x0B00,  GL_STENCIL_INDEX_OES                             = 0x1901,  GL_FLOAT_VEC2                                = 0x8B50,
    GL_CURRENT_INDEX                                                           = 0x0B01,  GL_R3_G3_B2                                      = 0x2A10,  GL_FLOAT_VEC2_ARB                            = 0x8B50,
    GL_CURRENT_NORMAL                                                          = 0x0B02,  GL_ALPHA4                                        = 0x803B,  GL_FLOAT_VEC3                                = 0x8B51,
    GL_CURRENT_TEXTURE_COORDS                                                  = 0x0B03,  GL_ALPHA4_EXT                                    = 0x803B,  GL_FLOAT_VEC3_ARB                            = 0x8B51,
    GL_CURRENT_RASTER_COLOR                                                    = 0x0B04,  GL_ALPHA8                                        = 0x803C,  GL_FLOAT_VEC4                                = 0x8B52,
    GL_CURRENT_RASTER_INDEX                                                    = 0x0B05,  GL_ALPHA8_EXT                                    = 0x803C,  GL_FLOAT_VEC4_ARB                            = 0x8B52,
    GL_CURRENT_RASTER_TEXTURE_COORDS                                           = 0x0B06,  GL_ALPHA8_OES                                    = 0x803C,  GL_INT_VEC2                                  = 0x8B53,
    GL_CURRENT_RASTER_POSITION                                                 = 0x0B07,  GL_ALPHA12                                       = 0x803D,  GL_INT_VEC2_ARB                              = 0x8B53,
    GL_CURRENT_RASTER_POSITION_VALID                                           = 0x0B08,  GL_ALPHA12_EXT                                   = 0x803D,  GL_INT_VEC3                                  = 0x8B54,
    GL_CURRENT_RASTER_DISTANCE                                                 = 0x0B09,  GL_ALPHA16                                       = 0x803E,  GL_INT_VEC3_ARB                              = 0x8B54,
    GL_POINT_SMOOTH                                                            = 0x0B10,  GL_ALPHA16_EXT                                   = 0x803E,  GL_INT_VEC4                                  = 0x8B55,
    GL_POINT_SIZE                                                              = 0x0B11,  GL_LUMINANCE4                                    = 0x803F,  GL_INT_VEC4_ARB                              = 0x8B55,
    GL_POINT_SIZE_RANGE                                                        = 0x0B12,  GL_LUMINANCE4_EXT                                = 0x803F,  GL_BOOL                                      = 0x8B56,
    GL_SMOOTH_POINT_SIZE_RANGE                                                 = 0x0B12,  GL_LUMINANCE8                                    = 0x8040,  GL_BOOL_ARB                                  = 0x8B56,
    GL_POINT_SIZE_GRANULARITY                                                  = 0x0B13,  GL_LUMINANCE8_EXT                                = 0x8040,  GL_BOOL_VEC2                                 = 0x8B57,
    GL_SMOOTH_POINT_SIZE_GRANULARITY                                           = 0x0B13,  GL_LUMINANCE8_OES                                = 0x8040,  GL_BOOL_VEC2_ARB                             = 0x8B57,
    GL_LINE_SMOOTH                                                             = 0x0B20,  GL_LUMINANCE12                                   = 0x8041,  GL_BOOL_VEC3                                 = 0x8B58,
    GL_LINE_WIDTH                                                              = 0x0B21,  GL_LUMINANCE12_EXT                               = 0x8041,  GL_BOOL_VEC3_ARB                             = 0x8B58,
    GL_LINE_WIDTH_RANGE                                                        = 0x0B22,  GL_LUMINANCE16                                   = 0x8042,  GL_BOOL_VEC4                                 = 0x8B59,
    GL_SMOOTH_LINE_WIDTH_RANGE                                                 = 0x0B22,  GL_LUMINANCE16_EXT                               = 0x8042,  GL_BOOL_VEC4_ARB                             = 0x8B59,
    GL_LINE_WIDTH_GRANULARITY                                                  = 0x0B23,  GL_LUMINANCE4_ALPHA4                             = 0x8043,  GL_FLOAT_MAT2                                = 0x8B5A,
    GL_SMOOTH_LINE_WIDTH_GRANULARITY                                           = 0x0B23,  GL_LUMINANCE4_ALPHA4_EXT                         = 0x8043,  GL_FLOAT_MAT2_ARB                            = 0x8B5A,
    GL_LINE_STIPPLE                                                            = 0x0B24,  GL_LUMINANCE4_ALPHA4_OES                         = 0x8043,  GL_FLOAT_MAT3                                = 0x8B5B,
    GL_LINE_STIPPLE_PATTERN                                                    = 0x0B25,  GL_LUMINANCE6_ALPHA2                             = 0x8044,  GL_FLOAT_MAT3_ARB                            = 0x8B5B,
    GL_LINE_STIPPLE_REPEAT                                                     = 0x0B26,  GL_LUMINANCE6_ALPHA2_EXT                         = 0x8044,  GL_FLOAT_MAT4                                = 0x8B5C,
    GL_LIST_MODE                                                               = 0x0B30,  GL_LUMINANCE8_ALPHA8                             = 0x8045,  GL_FLOAT_MAT4_ARB                            = 0x8B5C,
    GL_MAX_LIST_NESTING                                                        = 0x0B31,  GL_LUMINANCE8_ALPHA8_EXT                         = 0x8045,  GL_SAMPLER_1D                                = 0x8B5D,
    GL_LIST_BASE                                                               = 0x0B32,  GL_LUMINANCE8_ALPHA8_OES                         = 0x8045,  GL_SAMPLER_1D_ARB                            = 0x8B5D,
    GL_LIST_INDEX                                                              = 0x0B33,  GL_LUMINANCE12_ALPHA4                            = 0x8046,  GL_SAMPLER_2D                                = 0x8B5E,
    GL_POLYGON_MODE                                                            = 0x0B40,  GL_LUMINANCE12_ALPHA4_EXT                        = 0x8046,  GL_SAMPLER_2D_ARB                            = 0x8B5E,
    GL_POLYGON_SMOOTH                                                          = 0x0B41,  GL_LUMINANCE12_ALPHA12                           = 0x8047,  GL_SAMPLER_3D                                = 0x8B5F,
    GL_POLYGON_STIPPLE                                                         = 0x0B42,  GL_LUMINANCE12_ALPHA12_EXT                       = 0x8047,  GL_SAMPLER_3D_ARB                            = 0x8B5F,
    GL_EDGE_FLAG                                                               = 0x0B43,  GL_LUMINANCE16_ALPHA16                           = 0x8048,  GL_SAMPLER_3D_OES                            = 0x8B5F,
    GL_CULL_FACE                                                               = 0x0B44,  GL_LUMINANCE16_ALPHA16_EXT                       = 0x8048,  GL_SAMPLER_CUBE                              = 0x8B60,
    GL_CULL_FACE_MODE                                                          = 0x0B45,  GL_INTENSITY4                                    = 0x804A,  GL_SAMPLER_CUBE_ARB                          = 0x8B60,
    GL_FRONT_FACE                                                              = 0x0B46,  GL_INTENSITY4_EXT                                = 0x804A,  GL_SAMPLER_1D_SHADOW                         = 0x8B61,
    GL_LIGHTING                                                                = 0x0B50,  GL_INTENSITY8                                    = 0x804B,  GL_SAMPLER_1D_SHADOW_ARB                     = 0x8B61,
    GL_LIGHT_MODEL_LOCAL_VIEWER                                                = 0x0B51,  GL_INTENSITY8_EXT                                = 0x804B,  GL_SAMPLER_2D_SHADOW                         = 0x8B62,
    GL_LIGHT_MODEL_TWO_SIDE                                                    = 0x0B52,  GL_INTENSITY12                                   = 0x804C,  GL_SAMPLER_2D_SHADOW_ARB                     = 0x8B62,
    GL_LIGHT_MODEL_AMBIENT                                                     = 0x0B53,  GL_INTENSITY12_EXT                               = 0x804C,  GL_SAMPLER_2D_SHADOW_EXT                     = 0x8B62,
    GL_SHADE_MODEL                                                             = 0x0B54,  GL_INTENSITY16                                   = 0x804D,  GL_SAMPLER_2D_RECT                           = 0x8B63,
    GL_COLOR_MATERIAL_FACE                                                     = 0x0B55,  GL_INTENSITY16_EXT                               = 0x804D,  GL_SAMPLER_2D_RECT_ARB                       = 0x8B63,
    GL_COLOR_MATERIAL_PARAMETER                                                = 0x0B56,  GL_RGB2_EXT                                      = 0x804E,  GL_SAMPLER_2D_RECT_SHADOW                    = 0x8B64,
    GL_COLOR_MATERIAL                                                          = 0x0B57,  GL_RGB4                                          = 0x804F,  GL_SAMPLER_2D_RECT_SHADOW_ARB                = 0x8B64,
    GL_FOG                                                                     = 0x0B60,  GL_RGB4_EXT                                      = 0x804F,  GL_FLOAT_MAT2x3                              = 0x8B65,
    GL_FOG_INDEX                                                               = 0x0B61,  GL_RGB5                                          = 0x8050,  GL_FLOAT_MAT2x3_NV                           = 0x8B65,
    GL_FOG_DENSITY                                                             = 0x0B62,  GL_RGB5_EXT                                      = 0x8050,  GL_FLOAT_MAT2x4                              = 0x8B66,
    GL_FOG_START                                                               = 0x0B63,  GL_RGB8                                          = 0x8051,  GL_FLOAT_MAT2x4_NV                           = 0x8B66,
    GL_FOG_END                                                                 = 0x0B64,  GL_RGB8_EXT                                      = 0x8051,  GL_FLOAT_MAT3x2                              = 0x8B67,
    GL_FOG_MODE                                                                = 0x0B65,  GL_RGB8_OES                                      = 0x8051,  GL_FLOAT_MAT3x2_NV                           = 0x8B67,
    GL_FOG_COLOR                                                               = 0x0B66,  GL_RGB10                                         = 0x8052,  GL_FLOAT_MAT3x4                              = 0x8B68,
    GL_DEPTH_RANGE                                                             = 0x0B70,  GL_RGB10_EXT                                     = 0x8052,  GL_FLOAT_MAT3x4_NV                           = 0x8B68,
    GL_DEPTH_TEST                                                              = 0x0B71,  GL_RGB12                                         = 0x8053,  GL_FLOAT_MAT4x2                              = 0x8B69,
    GL_DEPTH_WRITEMASK                                                         = 0x0B72,  GL_RGB12_EXT                                     = 0x8053,  GL_FLOAT_MAT4x2_NV                           = 0x8B69,
    GL_DEPTH_CLEAR_VALUE                                                       = 0x0B73,  GL_RGB16                                         = 0x8054,  GL_FLOAT_MAT4x3                              = 0x8B6A,
    GL_DEPTH_FUNC                                                              = 0x0B74,  GL_RGB16_EXT                                     = 0x8054,  GL_FLOAT_MAT4x3_NV                           = 0x8B6A,
    GL_ACCUM_CLEAR_VALUE                                                       = 0x0B80,  GL_RGBA2                                         = 0x8055,  GL_SAMPLER_BUFFER                            = 0x8DC2,
    GL_STENCIL_TEST                                                            = 0x0B90,  GL_RGBA2_EXT                                     = 0x8055,  GL_SAMPLER_1D_ARRAY_SHADOW                   = 0x8DC3,
    GL_STENCIL_CLEAR_VALUE                                                     = 0x0B91,  GL_RGBA4                                         = 0x8056,  GL_SAMPLER_2D_ARRAY_SHADOW                   = 0x8DC4,
    GL_STENCIL_FUNC                                                            = 0x0B92,  GL_RGBA4_EXT                                     = 0x8056,  GL_SAMPLER_CUBE_SHADOW                       = 0x8DC5,
    GL_STENCIL_VALUE_MASK                                                      = 0x0B93,  GL_RGBA4_OES                                     = 0x8056,  GL_UNSIGNED_INT_VEC2                         = 0x8DC6,
    GL_STENCIL_FAIL                                                            = 0x0B94,  GL_RGB5_A1                                       = 0x8057,  GL_UNSIGNED_INT_VEC3                         = 0x8DC7,
    GL_STENCIL_PASS_DEPTH_FAIL                                                 = 0x0B95,  GL_RGB5_A1_EXT                                   = 0x8057,  GL_UNSIGNED_INT_VEC4                         = 0x8DC8,
    GL_STENCIL_PASS_DEPTH_PASS                                                 = 0x0B96,  GL_RGB5_A1_OES                                   = 0x8057,  GL_INT_SAMPLER_1D                            = 0x8DC9,
    GL_STENCIL_REF                                                             = 0x0B97,  GL_RGBA8                                         = 0x8058,  GL_INT_SAMPLER_2D                            = 0x8DCA,
    GL_STENCIL_WRITEMASK                                                       = 0x0B98,  GL_RGBA8_EXT                                     = 0x8058,  GL_INT_SAMPLER_3D                            = 0x8DCB,
    GL_MATRIX_MODE                                                             = 0x0BA0,  GL_RGBA8_OES                                     = 0x8058,  GL_INT_SAMPLER_CUBE                          = 0x8DCC,
    GL_NORMALIZE                                                               = 0x0BA1,  GL_RGB10_A2                                      = 0x8059,  GL_INT_SAMPLER_2D_RECT                       = 0x8DCD,
    GL_VIEWPORT                                                                = 0x0BA2,  GL_RGB10_A2_EXT                                  = 0x8059,  GL_INT_SAMPLER_1D_ARRAY                      = 0x8DCE,
    GL_MODELVIEW_STACK_DEPTH                                                   = 0x0BA3,  GL_RGBA12                                        = 0x805A,  GL_INT_SAMPLER_2D_ARRAY                      = 0x8DCF,
    GL_MODELVIEW0_STACK_DEPTH_EXT                                              = 0x0BA3,  GL_RGBA12_EXT                                    = 0x805A,  GL_INT_SAMPLER_BUFFER                        = 0x8DD0,
    GL_PROJECTION_STACK_DEPTH                                                  = 0x0BA4,  GL_RGBA16                                        = 0x805B,  GL_UNSIGNED_INT_SAMPLER_1D                   = 0x8DD1,
    GL_TEXTURE_STACK_DEPTH                                                     = 0x0BA5,  GL_RGBA16_EXT                                    = 0x805B,  GL_UNSIGNED_INT_SAMPLER_2D                   = 0x8DD2,
    GL_MODELVIEW_MATRIX                                                        = 0x0BA6,  GL_DUAL_ALPHA4_SGIS                              = 0x8110,  GL_UNSIGNED_INT_SAMPLER_3D                   = 0x8DD3,
    GL_MODELVIEW0_MATRIX_EXT                                                   = 0x0BA6,  GL_DUAL_ALPHA8_SGIS                              = 0x8111,  GL_UNSIGNED_INT_SAMPLER_CUBE                 = 0x8DD4,
    GL_PROJECTION_MATRIX                                                       = 0x0BA7,  GL_DUAL_ALPHA12_SGIS                             = 0x8112,  GL_UNSIGNED_INT_SAMPLER_2D_RECT              = 0x8DD5,
    GL_TEXTURE_MATRIX                                                          = 0x0BA8,  GL_DUAL_ALPHA16_SGIS                             = 0x8113,  GL_UNSIGNED_INT_SAMPLER_1D_ARRAY             = 0x8DD6,
    GL_ATTRIB_STACK_DEPTH                                                      = 0x0BB0,  GL_DUAL_LUMINANCE4_SGIS                          = 0x8114,  GL_UNSIGNED_INT_SAMPLER_2D_ARRAY             = 0x8DD7,
    GL_CLIENT_ATTRIB_STACK_DEPTH                                               = 0x0BB1,  GL_DUAL_LUMINANCE8_SGIS                          = 0x8115,  GL_UNSIGNED_INT_SAMPLER_BUFFER               = 0x8DD8,
    GL_ALPHA_TEST                                                              = 0x0BC0,  GL_DUAL_LUMINANCE12_SGIS                         = 0x8116,  GL_DOUBLE_MAT2                               = 0x8F46,
    GL_ALPHA_TEST_QCOM                                                         = 0x0BC0,  GL_DUAL_LUMINANCE16_SGIS                         = 0x8117,  GL_DOUBLE_MAT3                               = 0x8F47,
    GL_ALPHA_TEST_FUNC                                                         = 0x0BC1,  GL_DUAL_INTENSITY4_SGIS                          = 0x8118,  GL_DOUBLE_MAT4                               = 0x8F48,
    GL_ALPHA_TEST_FUNC_QCOM                                                    = 0x0BC1,  GL_DUAL_INTENSITY8_SGIS                          = 0x8119,  GL_DOUBLE_MAT2x3                             = 0x8F49,
    GL_ALPHA_TEST_REF                                                          = 0x0BC2,  GL_DUAL_INTENSITY12_SGIS                         = 0x811A,  GL_DOUBLE_MAT2x4                             = 0x8F4A,
    GL_ALPHA_TEST_REF_QCOM                                                     = 0x0BC2,  GL_DUAL_INTENSITY16_SGIS                         = 0x811B,  GL_DOUBLE_MAT3x2                             = 0x8F4B,
    GL_DITHER                                                                  = 0x0BD0,  GL_DUAL_LUMINANCE_ALPHA4_SGIS                    = 0x811C,  GL_DOUBLE_MAT3x4                             = 0x8F4C,
    GL_BLEND_DST                                                               = 0x0BE0,  GL_DUAL_LUMINANCE_ALPHA8_SGIS                    = 0x811D,  GL_DOUBLE_MAT4x2                             = 0x8F4D,
    GL_BLEND_SRC                                                               = 0x0BE1,  GL_QUAD_ALPHA4_SGIS                              = 0x811E,  GL_DOUBLE_MAT4x3                             = 0x8F4E,
    GL_LOGIC_OP_MODE                                                           = 0x0BF0,  GL_QUAD_ALPHA8_SGIS                              = 0x811F,  GL_INT64_VEC2_ARB                            = 0x8FE9,
    GL_INDEX_LOGIC_OP                                                          = 0x0BF1,  GL_QUAD_LUMINANCE4_SGIS                          = 0x8120,  GL_INT64_VEC3_ARB                            = 0x8FEA,
    GL_LOGIC_OP                                                                = 0x0BF1,  GL_QUAD_LUMINANCE8_SGIS                          = 0x8121,  GL_INT64_VEC4_ARB                            = 0x8FEB,
    GL_COLOR_LOGIC_OP                                                          = 0x0BF2,  GL_QUAD_INTENSITY4_SGIS                          = 0x8122,  GL_UNSIGNED_INT64_VEC2_ARB                   = 0x8FF5,
    GL_AUX_BUFFERS                                                             = 0x0C00,  GL_QUAD_INTENSITY8_SGIS                          = 0x8123,  GL_UNSIGNED_INT64_VEC3_ARB                   = 0x8FF6,
    GL_DRAW_BUFFER                                                             = 0x0C01,  GL_DEPTH_COMPONENT16                             = 0x81A5,  GL_UNSIGNED_INT64_VEC4_ARB                   = 0x8FF7,
    GL_DRAW_BUFFER_EXT                                                         = 0x0C01,  GL_DEPTH_COMPONENT16_ARB                         = 0x81A5,  GL_DOUBLE_VEC2                               = 0x8FFC,
    GL_READ_BUFFER                                                             = 0x0C02,  GL_DEPTH_COMPONENT16_OES                         = 0x81A5,  GL_DOUBLE_VEC3                               = 0x8FFD,
    GL_READ_BUFFER_EXT                                                         = 0x0C02,  GL_DEPTH_COMPONENT16_SGIX                        = 0x81A5,  GL_DOUBLE_VEC4                               = 0x8FFE,
    GL_READ_BUFFER_NV                                                          = 0x0C02,  GL_DEPTH_COMPONENT24                             = 0x81A6,  GL_SAMPLER_CUBE_MAP_ARRAY                    = 0x900C,
    GL_SCISSOR_BOX                                                             = 0x0C10,  GL_DEPTH_COMPONENT24_ARB                         = 0x81A6,  GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW             = 0x900D,
    GL_SCISSOR_TEST                                                            = 0x0C11,  GL_DEPTH_COMPONENT24_OES                         = 0x81A6,  GL_INT_SAMPLER_CUBE_MAP_ARRAY                = 0x900E,
    GL_INDEX_CLEAR_VALUE                                                       = 0x0C20,  GL_DEPTH_COMPONENT24_SGIX                        = 0x81A6,  GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY       = 0x900F,
    GL_INDEX_WRITEMASK                                                         = 0x0C21,  GL_DEPTH_COMPONENT32                             = 0x81A7,  GL_IMAGE_1D                                  = 0x904C,
    GL_COLOR_CLEAR_VALUE                                                       = 0x0C22,  GL_DEPTH_COMPONENT32_ARB                         = 0x81A7,  GL_IMAGE_2D                                  = 0x904D,
    GL_COLOR_WRITEMASK                                                         = 0x0C23,  GL_DEPTH_COMPONENT32_OES                         = 0x81A7,  GL_IMAGE_3D                                  = 0x904E,
    GL_INDEX_MODE                                                              = 0x0C30,  GL_DEPTH_COMPONENT32_SGIX                        = 0x81A7,  GL_IMAGE_2D_RECT                             = 0x904F,
    GL_RGBA_MODE                                                               = 0x0C31,  GL_COMPRESSED_RED                                = 0x8225,  GL_IMAGE_CUBE                                = 0x9050,
    GL_DOUBLEBUFFER                                                            = 0x0C32,  GL_COMPRESSED_RG                                 = 0x8226,  GL_IMAGE_BUFFER                              = 0x9051,
    GL_STEREO                                                                  = 0x0C33,  GL_R8                                            = 0x8229,  GL_IMAGE_1D_ARRAY                            = 0x9052,
    GL_RENDER_MODE                                                             = 0x0C40,  GL_R8_EXT                                        = 0x8229,  GL_IMAGE_2D_ARRAY                            = 0x9053,
    GL_PERSPECTIVE_CORRECTION_HINT                                             = 0x0C50,  GL_R16                                           = 0x822A,  GL_IMAGE_CUBE_MAP_ARRAY                      = 0x9054,
    GL_POINT_SMOOTH_HINT                                                       = 0x0C51,  GL_R16_EXT                                       = 0x822A,  GL_IMAGE_2D_MULTISAMPLE                      = 0x9055,
    GL_LINE_SMOOTH_HINT                                                        = 0x0C52,  GL_RG8                                           = 0x822B,  GL_IMAGE_2D_MULTISAMPLE_ARRAY                = 0x9056,
    GL_POLYGON_SMOOTH_HINT                                                     = 0x0C53,  GL_RG8_EXT                                       = 0x822B,  GL_INT_IMAGE_1D                              = 0x9057,
    GL_FOG_HINT                                                                = 0x0C54,  GL_RG16                                          = 0x822C,  GL_INT_IMAGE_2D                              = 0x9058,
    GL_TEXTURE_GEN_S                                                           = 0x0C60,  GL_RG16_EXT                                      = 0x822C,  GL_INT_IMAGE_3D                              = 0x9059,
    GL_TEXTURE_GEN_T                                                           = 0x0C61,  GL_R16F                                          = 0x822D,  GL_INT_IMAGE_2D_RECT                         = 0x905A,
    GL_TEXTURE_GEN_R                                                           = 0x0C62,  GL_R16F_EXT                                      = 0x822D,  GL_INT_IMAGE_CUBE                            = 0x905B,
    GL_TEXTURE_GEN_Q                                                           = 0x0C63,  GL_R32F                                          = 0x822E,  GL_INT_IMAGE_BUFFER                          = 0x905C,
    GL_PIXEL_MAP_I_TO_I_SIZE                                                   = 0x0CB0,  GL_R32F_EXT                                      = 0x822E,  GL_INT_IMAGE_1D_ARRAY                        = 0x905D,
    GL_PIXEL_MAP_S_TO_S_SIZE                                                   = 0x0CB1,  GL_RG16F                                         = 0x822F,  GL_INT_IMAGE_2D_ARRAY                        = 0x905E,
    GL_PIXEL_MAP_I_TO_R_SIZE                                                   = 0x0CB2,  GL_RG16F_EXT                                     = 0x822F,  GL_INT_IMAGE_CUBE_MAP_ARRAY                  = 0x905F,
    GL_PIXEL_MAP_I_TO_G_SIZE                                                   = 0x0CB3,  GL_RG32F                                         = 0x8230,  GL_INT_IMAGE_2D_MULTISAMPLE                  = 0x9060,
    GL_PIXEL_MAP_I_TO_B_SIZE                                                   = 0x0CB4,  GL_RG32F_EXT                                     = 0x8230,  GL_INT_IMAGE_2D_MULTISAMPLE_ARRAY            = 0x9061,
    GL_PIXEL_MAP_I_TO_A_SIZE                                                   = 0x0CB5,  GL_R8I                                           = 0x8231,  GL_UNSIGNED_INT_IMAGE_1D                     = 0x9062,
    GL_PIXEL_MAP_R_TO_R_SIZE                                                   = 0x0CB6,  GL_R8UI                                          = 0x8232,  GL_UNSIGNED_INT_IMAGE_2D                     = 0x9063,
    GL_PIXEL_MAP_G_TO_G_SIZE                                                   = 0x0CB7,  GL_R16I                                          = 0x8233,  GL_UNSIGNED_INT_IMAGE_3D                     = 0x9064,
    GL_PIXEL_MAP_B_TO_B_SIZE                                                   = 0x0CB8,  GL_R16UI                                         = 0x8234,  GL_UNSIGNED_INT_IMAGE_2D_RECT                = 0x9065,
    GL_PIXEL_MAP_A_TO_A_SIZE                                                   = 0x0CB9,  GL_R32I                                          = 0x8235,  GL_UNSIGNED_INT_IMAGE_CUBE                   = 0x9066,
    GL_UNPACK_SWAP_BYTES                                                       = 0x0CF0,  GL_R32UI                                         = 0x8236,  GL_UNSIGNED_INT_IMAGE_BUFFER                 = 0x9067,
    GL_UNPACK_LSB_FIRST                                                        = 0x0CF1,  GL_RG8I                                          = 0x8237,  GL_UNSIGNED_INT_IMAGE_1D_ARRAY               = 0x9068,
    GL_UNPACK_ROW_LENGTH                                                       = 0x0CF2,  GL_RG8UI                                         = 0x8238,  GL_UNSIGNED_INT_IMAGE_2D_ARRAY               = 0x9069,
    GL_UNPACK_SKIP_ROWS                                                        = 0x0CF3,  GL_RG16I                                         = 0x8239,  GL_UNSIGNED_INT_IMAGE_CUBE_MAP_ARRAY         = 0x906A,
    GL_UNPACK_SKIP_PIXELS                                                      = 0x0CF4,  GL_RG16UI                                        = 0x823A,  GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE         = 0x906B,
    GL_UNPACK_ALIGNMENT                                                        = 0x0CF5,  GL_RG32I                                         = 0x823B,  GL_UNSIGNED_INT_IMAGE_2D_MULTISAMPLE_ARRAY   = 0x906C,
    GL_PACK_SWAP_BYTES                                                         = 0x0D00,  GL_RG32UI                                        = 0x823C,  GL_SAMPLER_2D_MULTISAMPLE                    = 0x9108,
    GL_PACK_LSB_FIRST                                                          = 0x0D01,  GL_COMPRESSED_RGB_S3TC_DXT1_EXT                  = 0x83F0,  GL_INT_SAMPLER_2D_MULTISAMPLE                = 0x9109,
    GL_PACK_ROW_LENGTH                                                         = 0x0D02,  GL_COMPRESSED_RGBA_S3TC_DXT1_EXT                 = 0x83F1,  GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE       = 0x910A,
    GL_PACK_ROW_LENGTH_NV                                                      = 0x0D02,  GL_COMPRESSED_RGBA_S3TC_DXT3_ANGLE               = 0x83F2,  GL_SAMPLER_2D_MULTISAMPLE_ARRAY              = 0x910B,
    GL_PACK_SKIP_ROWS                                                          = 0x0D03,  GL_COMPRESSED_RGBA_S3TC_DXT3_EXT                 = 0x83F2,  GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY          = 0x910C,
    GL_PACK_SKIP_ROWS_NV                                                       = 0x0D03,  GL_COMPRESSED_RGBA_S3TC_DXT5_ANGLE               = 0x83F3,  GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY = 0x910D,
    GL_PACK_SKIP_PIXELS                                                        = 0x0D04,  GL_COMPRESSED_RGBA_S3TC_DXT5_EXT                 = 0x83F3,
    GL_PACK_SKIP_PIXELS_NV                                                     = 0x0D04,  GL_COMPRESSED_RGB                                = 0x84ED,
    GL_PACK_ALIGNMENT                                                          = 0x0D05,  GL_COMPRESSED_RGBA                               = 0x84EE,
    GL_MAP_COLOR                                                               = 0x0D10,  GL_DEPTH_STENCIL_EXT                             = 0x84F9,
    GL_MAP_STENCIL                                                             = 0x0D11,  GL_DEPTH_STENCIL_NV                              = 0x84F9,
    GL_INDEX_SHIFT                                                             = 0x0D12,  GL_DEPTH_STENCIL_OES                             = 0x84F9,
    GL_INDEX_OFFSET                                                            = 0x0D13,  GL_DEPTH_STENCIL_MESA                            = 0x8750,
    GL_RED_SCALE                                                               = 0x0D14,  GL_RGBA32F                                       = 0x8814,
    GL_RED_BIAS                                                                = 0x0D15,  GL_RGBA32F_ARB                                   = 0x8814,
    GL_ZOOM_X                                                                  = 0x0D16,  GL_RGBA32F_EXT                                   = 0x8814,
    GL_ZOOM_Y                                                                  = 0x0D17,  GL_RGB32F                                        = 0x8815,
    GL_GREEN_SCALE                                                             = 0x0D18,  GL_RGB32F_ARB                                    = 0x8815,
    GL_GREEN_BIAS                                                              = 0x0D19,  GL_RGB32F_EXT                                    = 0x8815,
    GL_BLUE_SCALE                                                              = 0x0D1A,  GL_RGBA16F                                       = 0x881A,
    GL_BLUE_BIAS                                                               = 0x0D1B,  GL_RGBA16F_ARB                                   = 0x881A,
    GL_ALPHA_SCALE                                                             = 0x0D1C,  GL_RGBA16F_EXT                                   = 0x881A,
    GL_ALPHA_BIAS                                                              = 0x0D1D,  GL_RGB16F                                        = 0x881B,
    GL_DEPTH_SCALE                                                             = 0x0D1E,  GL_RGB16F_ARB                                    = 0x881B,
    GL_DEPTH_BIAS                                                              = 0x0D1F,  GL_RGB16F_EXT                                    = 0x881B,
    GL_MAX_EVAL_ORDER                                                          = 0x0D30,  GL_DEPTH24_STENCIL8                              = 0x88F0,
    GL_MAX_LIGHTS                                                              = 0x0D31,  GL_DEPTH24_STENCIL8_EXT                          = 0x88F0,
    GL_MAX_CLIP_PLANES                                                         = 0x0D32,  GL_DEPTH24_STENCIL8_OES                          = 0x88F0,
    GL_MAX_CLIP_DISTANCES                                                      = 0x0D32,  GL_R11F_G11F_B10F                                = 0x8C3A,
    GL_MAX_TEXTURE_SIZE                                                        = 0x0D33,  GL_R11F_G11F_B10F_APPLE                          = 0x8C3A,
    GL_MAX_PIXEL_MAP_TABLE                                                     = 0x0D34,  GL_R11F_G11F_B10F_EXT                            = 0x8C3A,
    GL_MAX_ATTRIB_STACK_DEPTH                                                  = 0x0D35,  GL_RGB9_E5                                       = 0x8C3D,
    GL_MAX_MODELVIEW_STACK_DEPTH                                               = 0x0D36,  GL_RGB9_E5_APPLE                                 = 0x8C3D,
    GL_MAX_NAME_STACK_DEPTH                                                    = 0x0D37,  GL_RGB9_E5_EXT                                   = 0x8C3D,
    GL_MAX_PROJECTION_STACK_DEPTH                                              = 0x0D38,  GL_SRGB                                          = 0x8C40,
    GL_MAX_TEXTURE_STACK_DEPTH                                                 = 0x0D39,  GL_SRGB_EXT                                      = 0x8C40,
    GL_MAX_VIEWPORT_DIMS                                                       = 0x0D3A,  GL_SRGB8                                         = 0x8C41,
    GL_MAX_CLIENT_ATTRIB_STACK_DEPTH                                           = 0x0D3B,  GL_SRGB8_EXT                                     = 0x8C41,
    GL_SUBPIXEL_BITS                                                           = 0x0D50,  GL_SRGB8_NV                                      = 0x8C41,
    GL_INDEX_BITS                                                              = 0x0D51,  GL_SRGB_ALPHA                                    = 0x8C42,
    GL_RED_BITS                                                                = 0x0D52,  GL_SRGB_ALPHA_EXT                                = 0x8C42,
    GL_GREEN_BITS                                                              = 0x0D53,  GL_SRGB8_ALPHA8                                  = 0x8C43,
    GL_BLUE_BITS                                                               = 0x0D54,  GL_SRGB8_ALPHA8_EXT                              = 0x8C43,
    GL_ALPHA_BITS                                                              = 0x0D55,  GL_COMPRESSED_SRGB                               = 0x8C48,
    GL_DEPTH_BITS                                                              = 0x0D56,  GL_COMPRESSED_SRGB_ALPHA                         = 0x8C49,
    GL_STENCIL_BITS                                                            = 0x0D57,  GL_COMPRESSED_SRGB_S3TC_DXT1_EXT                 = 0x8C4C,
    GL_ACCUM_RED_BITS                                                          = 0x0D58,  GL_COMPRESSED_SRGB_S3TC_DXT1_NV                  = 0x8C4C,
    GL_ACCUM_GREEN_BITS                                                        = 0x0D59,  GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_EXT           = 0x8C4D,
    GL_ACCUM_BLUE_BITS                                                         = 0x0D5A,  GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT1_NV            = 0x8C4D,
    GL_ACCUM_ALPHA_BITS                                                        = 0x0D5B,  GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_EXT           = 0x8C4E,
    GL_NAME_STACK_DEPTH                                                        = 0x0D70,  GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT3_NV            = 0x8C4E,
    GL_AUTO_NORMAL                                                             = 0x0D80,  GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_EXT           = 0x8C4F,
    GL_MAP1_COLOR_4                                                            = 0x0D90,  GL_COMPRESSED_SRGB_ALPHA_S3TC_DXT5_NV            = 0x8C4F,
    GL_MAP1_INDEX                                                              = 0x0D91,  GL_DEPTH_COMPONENT32F                            = 0x8CAC,
    GL_MAP1_NORMAL                                                             = 0x0D92,  GL_DEPTH32F_STENCIL8                             = 0x8CAD,
    GL_MAP1_TEXTURE_COORD_1                                                    = 0x0D93,  GL_STENCIL_INDEX1                                = 0x8D46,
    GL_MAP1_TEXTURE_COORD_2                                                    = 0x0D94,  GL_STENCIL_INDEX1_EXT                            = 0x8D46,
    GL_MAP1_TEXTURE_COORD_3                                                    = 0x0D95,  GL_STENCIL_INDEX1_OES                            = 0x8D46,
    GL_MAP1_TEXTURE_COORD_4                                                    = 0x0D96,  GL_STENCIL_INDEX4                                = 0x8D47,
    GL_MAP1_VERTEX_3                                                           = 0x0D97,  GL_STENCIL_INDEX4_EXT                            = 0x8D47,
    GL_MAP1_VERTEX_4                                                           = 0x0D98,  GL_STENCIL_INDEX4_OES                            = 0x8D47,
    GL_MAP2_COLOR_4                                                            = 0x0DB0,  GL_STENCIL_INDEX8                                = 0x8D48,
    GL_MAP2_INDEX                                                              = 0x0DB1,  GL_STENCIL_INDEX8_EXT                            = 0x8D48,
    GL_MAP2_NORMAL                                                             = 0x0DB2,  GL_STENCIL_INDEX8_OES                            = 0x8D48,
    GL_MAP2_TEXTURE_COORD_1                                                    = 0x0DB3,  GL_STENCIL_INDEX16                               = 0x8D49,
    GL_MAP2_TEXTURE_COORD_2                                                    = 0x0DB4,  GL_STENCIL_INDEX16_EXT                           = 0x8D49,
    GL_MAP2_TEXTURE_COORD_3                                                    = 0x0DB5,  GL_RGB565_OES                                    = 0x8D62,
    GL_MAP2_TEXTURE_COORD_4                                                    = 0x0DB6,  GL_RGB565                                        = 0x8D62,
    GL_MAP2_VERTEX_3                                                           = 0x0DB7,  GL_ETC1_RGB8_OES                                 = 0x8D64,
    GL_MAP2_VERTEX_4                                                           = 0x0DB8,  GL_RGBA32UI                                      = 0x8D70,
    GL_MAP1_GRID_DOMAIN                                                        = 0x0DD0,  GL_RGBA32UI_EXT                                  = 0x8D70,
    GL_MAP1_GRID_SEGMENTS                                                      = 0x0DD1,  GL_RGB32UI                                       = 0x8D71,
    GL_MAP2_GRID_DOMAIN                                                        = 0x0DD2,  GL_RGB32UI_EXT                                   = 0x8D71,
    GL_MAP2_GRID_SEGMENTS                                                      = 0x0DD3,  GL_ALPHA32UI_EXT                                 = 0x8D72,
    GL_TEXTURE_1D                                                              = 0x0DE0,  GL_INTENSITY32UI_EXT                             = 0x8D73,
    GL_TEXTURE_2D                                                              = 0x0DE1,  GL_LUMINANCE32UI_EXT                             = 0x8D74,
    GL_FEEDBACK_BUFFER_SIZE                                                    = 0x0DF1,  GL_LUMINANCE_ALPHA32UI_EXT                       = 0x8D75,
    GL_FEEDBACK_BUFFER_TYPE                                                    = 0x0DF2,  GL_RGBA16UI                                      = 0x8D76,
    GL_SELECTION_BUFFER_SIZE                                                   = 0x0DF4,  GL_RGBA16UI_EXT                                  = 0x8D76,
    GL_POLYGON_OFFSET_UNITS                                                    = 0x2A00,  GL_RGB16UI                                       = 0x8D77,
    GL_POLYGON_OFFSET_POINT                                                    = 0x2A01,  GL_RGB16UI_EXT                                   = 0x8D77,
    GL_POLYGON_OFFSET_LINE                                                     = 0x2A02,  GL_ALPHA16UI_EXT                                 = 0x8D78,
    GL_CLIP_PLANE0                                                             = 0x3000,  GL_INTENSITY16UI_EXT                             = 0x8D79,
    GL_CLIP_PLANE1                                                             = 0x3001,  GL_LUMINANCE16UI_EXT                             = 0x8D7A,
    GL_CLIP_PLANE2                                                             = 0x3002,  GL_LUMINANCE_ALPHA16UI_EXT                       = 0x8D7B,
    GL_CLIP_PLANE3                                                             = 0x3003,  GL_RGBA8UI                                       = 0x8D7C,
    GL_CLIP_PLANE4                                                             = 0x3004,  GL_RGBA8UI_EXT                                   = 0x8D7C,
    GL_CLIP_PLANE5                                                             = 0x3005,  GL_RGB8UI                                        = 0x8D7D,
    GL_LIGHT0                                                                  = 0x4000,  GL_RGB8UI_EXT                                    = 0x8D7D,
    GL_LIGHT1                                                                  = 0x4001,  GL_ALPHA8UI_EXT                                  = 0x8D7E,
    GL_LIGHT2                                                                  = 0x4002,  GL_INTENSITY8UI_EXT                              = 0x8D7F,
    GL_LIGHT3                                                                  = 0x4003,  GL_LUMINANCE8UI_EXT                              = 0x8D80,
    GL_LIGHT4                                                                  = 0x4004,  GL_LUMINANCE_ALPHA8UI_EXT                        = 0x8D81,
    GL_LIGHT5                                                                  = 0x4005,  GL_RGBA32I                                       = 0x8D82,
    GL_LIGHT6                                                                  = 0x4006,  GL_RGBA32I_EXT                                   = 0x8D82,
    GL_LIGHT7                                                                  = 0x4007,  GL_RGB32I                                        = 0x8D83,
    GL_BLEND_COLOR                                                             = 0x8005,  GL_RGB32I_EXT                                    = 0x8D83,
    GL_BLEND_COLOR_EXT                                                         = 0x8005,  GL_ALPHA32I_EXT                                  = 0x8D84,
    GL_BLEND_EQUATION                                                          = 0x8009,  GL_INTENSITY32I_EXT                              = 0x8D85,
    GL_BLEND_EQUATION_EXT                                                      = 0x8009,  GL_LUMINANCE32I_EXT                              = 0x8D86,
    GL_BLEND_EQUATION_OES                                                      = 0x8009,  GL_LUMINANCE_ALPHA32I_EXT                        = 0x8D87,
    GL_BLEND_EQUATION_RGB                                                      = 0x8009,  GL_RGBA16I                                       = 0x8D88,
    GL_PACK_CMYK_HINT_EXT                                                      = 0x800E,  GL_RGBA16I_EXT                                   = 0x8D88,
    GL_UNPACK_CMYK_HINT_EXT                                                    = 0x800F,  GL_RGB16I                                        = 0x8D89,
    GL_CONVOLUTION_1D_EXT                                                      = 0x8010,  GL_RGB16I_EXT                                    = 0x8D89,
    GL_CONVOLUTION_2D_EXT                                                      = 0x8011,  GL_ALPHA16I_EXT                                  = 0x8D8A,
    GL_SEPARABLE_2D_EXT                                                        = 0x8012,  GL_INTENSITY16I_EXT                              = 0x8D8B,
    GL_POST_CONVOLUTION_RED_SCALE_EXT                                          = 0x801C,  GL_LUMINANCE16I_EXT                              = 0x8D8C,
    GL_POST_CONVOLUTION_GREEN_SCALE_EXT                                        = 0x801D,  GL_LUMINANCE_ALPHA16I_EXT                        = 0x8D8D,
    GL_POST_CONVOLUTION_BLUE_SCALE_EXT                                         = 0x801E,  GL_RGBA8I                                        = 0x8D8E,
    GL_POST_CONVOLUTION_ALPHA_SCALE_EXT                                        = 0x801F,  GL_RGBA8I_EXT                                    = 0x8D8E,
    GL_POST_CONVOLUTION_RED_BIAS_EXT                                           = 0x8020,  GL_RGB8I                                         = 0x8D8F,
    GL_POST_CONVOLUTION_GREEN_BIAS_EXT                                         = 0x8021,  GL_RGB8I_EXT                                     = 0x8D8F,
    GL_POST_CONVOLUTION_BLUE_BIAS_EXT                                          = 0x8022,  GL_ALPHA8I_EXT                                   = 0x8D90,
    GL_POST_CONVOLUTION_ALPHA_BIAS_EXT                                         = 0x8023,  GL_INTENSITY8I_EXT                               = 0x8D91,
    GL_HISTOGRAM_EXT                                                           = 0x8024,  GL_LUMINANCE8I_EXT                               = 0x8D92,
    GL_MINMAX_EXT                                                              = 0x802E,  GL_LUMINANCE_ALPHA8I_EXT                         = 0x8D93,
    GL_POLYGON_OFFSET_FILL                                                     = 0x8037,  GL_DEPTH_COMPONENT32F_NV                         = 0x8DAB,
    GL_POLYGON_OFFSET_FACTOR                                                   = 0x8038,  GL_DEPTH32F_STENCIL8_NV                          = 0x8DAC,
    GL_POLYGON_OFFSET_BIAS_EXT                                                 = 0x8039,  GL_COMPRESSED_RED_RGTC1                          = 0x8DBB,
    GL_RESCALE_NORMAL_EXT                                                      = 0x803A,  GL_COMPRESSED_RED_RGTC1_EXT                      = 0x8DBB,
    GL_TEXTURE_BINDING_1D                                                      = 0x8068,  GL_COMPRESSED_SIGNED_RED_RGTC1                   = 0x8DBC,
    GL_TEXTURE_BINDING_2D                                                      = 0x8069,  GL_COMPRESSED_SIGNED_RED_RGTC1_EXT               = 0x8DBC,
    GL_TEXTURE_3D_BINDING_EXT                                                  = 0x806A,  GL_COMPRESSED_RED_GREEN_RGTC2_EXT                = 0x8DBD,
    GL_TEXTURE_BINDING_3D                                                      = 0x806A,  GL_COMPRESSED_RG_RGTC2                           = 0x8DBD,
    GL_PACK_SKIP_IMAGES                                                        = 0x806B,  GL_COMPRESSED_SIGNED_RED_GREEN_RGTC2_EXT         = 0x8DBE,
    GL_PACK_SKIP_IMAGES_EXT                                                    = 0x806B,  GL_COMPRESSED_SIGNED_RG_RGTC2                    = 0x8DBE,
    GL_PACK_IMAGE_HEIGHT                                                       = 0x806C,  GL_COMPRESSED_RGBA_BPTC_UNORM                    = 0x8E8C,
    GL_PACK_IMAGE_HEIGHT_EXT                                                   = 0x806C,  GL_COMPRESSED_RGBA_BPTC_UNORM_ARB                = 0x8E8C,
    GL_UNPACK_SKIP_IMAGES                                                      = 0x806D,  GL_COMPRESSED_RGBA_BPTC_UNORM_EXT                = 0x8E8C,
    GL_UNPACK_SKIP_IMAGES_EXT                                                  = 0x806D,  GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM              = 0x8E8D,
    GL_UNPACK_IMAGE_HEIGHT                                                     = 0x806E,  GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB          = 0x8E8D,
    GL_UNPACK_IMAGE_HEIGHT_EXT                                                 = 0x806E,  GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_EXT          = 0x8E8D,
    GL_TEXTURE_3D_EXT                                                          = 0x806F,  GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT              = 0x8E8E,
    GL_MAX_3D_TEXTURE_SIZE                                                     = 0x8073,  GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB          = 0x8E8E,
    GL_MAX_3D_TEXTURE_SIZE_EXT                                                 = 0x8073,  GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_EXT          = 0x8E8E,
    GL_VERTEX_ARRAY                                                            = 0x8074,  GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT            = 0x8E8F,
    GL_NORMAL_ARRAY                                                            = 0x8075,  GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB        = 0x8E8F,
    GL_COLOR_ARRAY                                                             = 0x8076,  GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_EXT        = 0x8E8F,
    GL_INDEX_ARRAY                                                             = 0x8077,  GL_R8_SNORM                                      = 0x8F94,
    GL_TEXTURE_COORD_ARRAY                                                     = 0x8078,  GL_RG8_SNORM                                     = 0x8F95,
    GL_EDGE_FLAG_ARRAY                                                         = 0x8079,  GL_RGB8_SNORM                                    = 0x8F96,
    GL_VERTEX_ARRAY_SIZE                                                       = 0x807A,  GL_RGBA8_SNORM                                   = 0x8F97,
    GL_VERTEX_ARRAY_TYPE                                                       = 0x807B,  GL_R16_SNORM                                     = 0x8F98,
    GL_VERTEX_ARRAY_STRIDE                                                     = 0x807C,  GL_R16_SNORM_EXT                                 = 0x8F98,
    GL_VERTEX_ARRAY_COUNT_EXT                                                  = 0x807D,  GL_RG16_SNORM                                    = 0x8F99,
    GL_NORMAL_ARRAY_TYPE                                                       = 0x807E,  GL_RG16_SNORM_EXT                                = 0x8F99,
    GL_NORMAL_ARRAY_STRIDE                                                     = 0x807F,  GL_RGB16_SNORM                                   = 0x8F9A,
    GL_NORMAL_ARRAY_COUNT_EXT                                                  = 0x8080,  GL_RGB16_SNORM_EXT                               = 0x8F9A,
    GL_COLOR_ARRAY_SIZE                                                        = 0x8081,  GL_RGBA16_SNORM                                  = 0x8F9B,
    GL_COLOR_ARRAY_TYPE                                                        = 0x8082,  GL_RGBA16_SNORM_EXT                              = 0x8F9B,
    GL_COLOR_ARRAY_STRIDE                                                      = 0x8083,  GL_SR8_EXT                                       = 0x8FBD,
    GL_COLOR_ARRAY_COUNT_EXT                                                   = 0x8084,  GL_SRG8_EXT                                      = 0x8FBE,
    GL_INDEX_ARRAY_TYPE                                                        = 0x8085,  GL_RGB10_A2UI                                    = 0x906F,
    GL_INDEX_ARRAY_STRIDE                                                      = 0x8086,  GL_COMPRESSED_R11_EAC                            = 0x9270,
    GL_INDEX_ARRAY_COUNT_EXT                                                   = 0x8087,  GL_COMPRESSED_R11_EAC_OES                        = 0x9270,
    GL_TEXTURE_COORD_ARRAY_SIZE                                                = 0x8088,  GL_COMPRESSED_SIGNED_R11_EAC                     = 0x9271,
    GL_TEXTURE_COORD_ARRAY_TYPE                                                = 0x8089,  GL_COMPRESSED_SIGNED_R11_EAC_OES                 = 0x9271,
    GL_TEXTURE_COORD_ARRAY_STRIDE                                              = 0x808A,  GL_COMPRESSED_RG11_EAC                           = 0x9272,
    GL_TEXTURE_COORD_ARRAY_COUNT_EXT                                           = 0x808B,  GL_COMPRESSED_RG11_EAC_OES                       = 0x9272,
    GL_EDGE_FLAG_ARRAY_STRIDE                                                  = 0x808C,  GL_COMPRESSED_SIGNED_RG11_EAC                    = 0x9273,
    GL_EDGE_FLAG_ARRAY_COUNT_EXT                                               = 0x808D,  GL_COMPRESSED_SIGNED_RG11_EAC_OES                = 0x9273,
    GL_INTERLACE_SGIX                                                          = 0x8094,  GL_COMPRESSED_RGB8_ETC2                          = 0x9274,
    GL_DETAIL_TEXTURE_2D_BINDING_SGIS                                          = 0x8096,  GL_COMPRESSED_RGB8_ETC2_OES                      = 0x9274,
    GL_MULTISAMPLE_SGIS                                                        = 0x809D,  GL_COMPRESSED_SRGB8_ETC2                         = 0x9275,
    GL_SAMPLE_ALPHA_TO_MASK_SGIS                                               = 0x809E,  GL_COMPRESSED_SRGB8_ETC2_OES                     = 0x9275,
    GL_SAMPLE_ALPHA_TO_ONE_SGIS                                                = 0x809F,  GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2      = 0x9276,
    GL_SAMPLE_MASK_SGIS                                                        = 0x80A0,  GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2_OES  = 0x9276,
    GL_SAMPLE_BUFFERS                                                          = 0x80A8,  GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2     = 0x9277,
    GL_SAMPLE_BUFFERS_SGIS                                                     = 0x80A8,  GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2_OES = 0x9277,
    GL_SAMPLES                                                                 = 0x80A9,  GL_COMPRESSED_RGBA8_ETC2_EAC                     = 0x9278,
    GL_SAMPLES_SGIS                                                            = 0x80A9,  GL_COMPRESSED_RGBA8_ETC2_EAC_OES                 = 0x9278,
    GL_SAMPLE_COVERAGE_VALUE                                                   = 0x80AA,  GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC              = 0x9279,
    GL_SAMPLE_MASK_VALUE_SGIS                                                  = 0x80AA,  GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC_OES          = 0x9279,
    GL_SAMPLE_COVERAGE_INVERT                                                  = 0x80AB,  GL_COMPRESSED_RGBA_ASTC_4x4                      = 0x93B0,
    GL_SAMPLE_MASK_INVERT_SGIS                                                 = 0x80AB,  GL_COMPRESSED_RGBA_ASTC_4x4_KHR                  = 0x93B0,
    GL_SAMPLE_PATTERN_SGIS                                                     = 0x80AC,  GL_COMPRESSED_RGBA_ASTC_5x4                      = 0x93B1,
    GL_COLOR_MATRIX_SGI                                                        = 0x80B1,  GL_COMPRESSED_RGBA_ASTC_5x4_KHR                  = 0x93B1,
    GL_COLOR_MATRIX_STACK_DEPTH_SGI                                            = 0x80B2,  GL_COMPRESSED_RGBA_ASTC_5x5                      = 0x93B2,
    GL_MAX_COLOR_MATRIX_STACK_DEPTH_SGI                                        = 0x80B3,  GL_COMPRESSED_RGBA_ASTC_5x5_KHR                  = 0x93B2,
    GL_POST_COLOR_MATRIX_RED_SCALE_SGI                                         = 0x80B4,  GL_COMPRESSED_RGBA_ASTC_6x5                      = 0x93B3,
    GL_POST_COLOR_MATRIX_GREEN_SCALE_SGI                                       = 0x80B5,  GL_COMPRESSED_RGBA_ASTC_6x5_KHR                  = 0x93B3,
    GL_POST_COLOR_MATRIX_BLUE_SCALE_SGI                                        = 0x80B6,  GL_COMPRESSED_RGBA_ASTC_6x6                      = 0x93B4,
    GL_POST_COLOR_MATRIX_ALPHA_SCALE_SGI                                       = 0x80B7,  GL_COMPRESSED_RGBA_ASTC_6x6_KHR                  = 0x93B4,
    GL_POST_COLOR_MATRIX_RED_BIAS_SGI                                          = 0x80B8,  GL_COMPRESSED_RGBA_ASTC_8x5                      = 0x93B5,
    GL_POST_COLOR_MATRIX_GREEN_BIAS_SGI                                        = 0x80B9,  GL_COMPRESSED_RGBA_ASTC_8x5_KHR                  = 0x93B5,
    GL_POST_COLOR_MATRIX_BLUE_BIAS_SGI                                         = 0x80BA,  GL_COMPRESSED_RGBA_ASTC_8x6                      = 0x93B6,
    GL_POST_COLOR_MATRIX_ALPHA_BIAS_SGI                                        = 0x80BB,  GL_COMPRESSED_RGBA_ASTC_8x6_KHR                  = 0x93B6,
    GL_TEXTURE_COLOR_TABLE_SGI                                                 = 0x80BC,  GL_COMPRESSED_RGBA_ASTC_8x8                      = 0x93B7,
    GL_BLEND_DST_RGB                                                           = 0x80C8,  GL_COMPRESSED_RGBA_ASTC_8x8_KHR                  = 0x93B7,
    GL_BLEND_SRC_RGB                                                           = 0x80C9,  GL_COMPRESSED_RGBA_ASTC_10x5                     = 0x93B8,
    GL_BLEND_DST_ALPHA                                                         = 0x80CA,  GL_COMPRESSED_RGBA_ASTC_10x5_KHR                 = 0x93B8,
    GL_BLEND_SRC_ALPHA                                                         = 0x80CB,  GL_COMPRESSED_RGBA_ASTC_10x6                     = 0x93B9,
    GL_COLOR_TABLE_SGI                                                         = 0x80D0,  GL_COMPRESSED_RGBA_ASTC_10x6_KHR                 = 0x93B9,
    GL_POST_CONVOLUTION_COLOR_TABLE_SGI                                        = 0x80D1,  GL_COMPRESSED_RGBA_ASTC_10x8                     = 0x93BA,
    GL_POST_COLOR_MATRIX_COLOR_TABLE_SGI                                       = 0x80D2,  GL_COMPRESSED_RGBA_ASTC_10x8_KHR                 = 0x93BA,
    GL_MAX_ELEMENTS_VERTICES                                                   = 0x80E8,  GL_COMPRESSED_RGBA_ASTC_10x10                    = 0x93BB,
    GL_MAX_ELEMENTS_INDICES                                                    = 0x80E9,  GL_COMPRESSED_RGBA_ASTC_10x10_KHR                = 0x93BB,
    GL_POINT_SIZE_MIN                                                          = 0x8126,  GL_COMPRESSED_RGBA_ASTC_12x10                    = 0x93BC,
    GL_POINT_SIZE_MIN_ARB                                                      = 0x8126,  GL_COMPRESSED_RGBA_ASTC_12x10_KHR                = 0x93BC,
    GL_POINT_SIZE_MIN_EXT                                                      = 0x8126,  GL_COMPRESSED_RGBA_ASTC_12x12                    = 0x93BD,
    GL_POINT_SIZE_MIN_SGIS                                                     = 0x8126,  GL_COMPRESSED_RGBA_ASTC_12x12_KHR                = 0x93BD,
    GL_POINT_SIZE_MAX                                                          = 0x8127,  GL_COMPRESSED_RGBA_ASTC_3x3x3_OES                = 0x93C0,
    GL_POINT_SIZE_MAX_ARB                                                      = 0x8127,  GL_COMPRESSED_RGBA_ASTC_4x3x3_OES                = 0x93C1,
    GL_POINT_SIZE_MAX_EXT                                                      = 0x8127,  GL_COMPRESSED_RGBA_ASTC_4x4x3_OES                = 0x93C2,
    GL_POINT_SIZE_MAX_SGIS                                                     = 0x8127,  GL_COMPRESSED_RGBA_ASTC_4x4x4_OES                = 0x93C3,
    GL_POINT_FADE_THRESHOLD_SIZE                                               = 0x8128,  GL_COMPRESSED_RGBA_ASTC_5x4x4_OES                = 0x93C4,
    GL_POINT_FADE_THRESHOLD_SIZE_ARB                                           = 0x8128,  GL_COMPRESSED_RGBA_ASTC_5x5x4_OES                = 0x93C5,
    GL_POINT_FADE_THRESHOLD_SIZE_EXT                                           = 0x8128,  GL_COMPRESSED_RGBA_ASTC_5x5x5_OES                = 0x93C6,
    GL_POINT_FADE_THRESHOLD_SIZE_SGIS                                          = 0x8128,  GL_COMPRESSED_RGBA_ASTC_6x5x5_OES                = 0x93C7,
    GL_DISTANCE_ATTENUATION_EXT                                                = 0x8129,  GL_COMPRESSED_RGBA_ASTC_6x6x5_OES                = 0x93C8,
    GL_DISTANCE_ATTENUATION_SGIS                                               = 0x8129,  GL_COMPRESSED_RGBA_ASTC_6x6x6_OES                = 0x93C9,
    GL_POINT_DISTANCE_ATTENUATION                                              = 0x8129,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4              = 0x93D0,
    GL_POINT_DISTANCE_ATTENUATION_ARB                                          = 0x8129,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4_KHR          = 0x93D0,
    GL_FOG_FUNC_POINTS_SGIS                                                    = 0x812B,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4              = 0x93D1,
    GL_MAX_FOG_FUNC_POINTS_SGIS                                                = 0x812C,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4_KHR          = 0x93D1,
    GL_PACK_SKIP_VOLUMES_SGIS                                                  = 0x8130,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5              = 0x93D2,
    GL_PACK_IMAGE_DEPTH_SGIS                                                   = 0x8131,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5_KHR          = 0x93D2,
    GL_UNPACK_SKIP_VOLUMES_SGIS                                                = 0x8132,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5              = 0x93D3,
    GL_UNPACK_IMAGE_DEPTH_SGIS                                                 = 0x8133,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5_KHR          = 0x93D3,
    GL_TEXTURE_4D_SGIS                                                         = 0x8134,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6              = 0x93D4,
    GL_MAX_4D_TEXTURE_SIZE_SGIS                                                = 0x8138,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6_KHR          = 0x93D4,
    GL_PIXEL_TEX_GEN_SGIX                                                      = 0x8139,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5              = 0x93D5,
    GL_PIXEL_TILE_BEST_ALIGNMENT_SGIX                                          = 0x813E,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5_KHR          = 0x93D5,
    GL_PIXEL_TILE_CACHE_INCREMENT_SGIX                                         = 0x813F,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6              = 0x93D6,
    GL_PIXEL_TILE_WIDTH_SGIX                                                   = 0x8140,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6_KHR          = 0x93D6,
    GL_PIXEL_TILE_HEIGHT_SGIX                                                  = 0x8141,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8              = 0x93D7,
    GL_PIXEL_TILE_GRID_WIDTH_SGIX                                              = 0x8142,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8_KHR          = 0x93D7,
    GL_PIXEL_TILE_GRID_HEIGHT_SGIX                                             = 0x8143,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5             = 0x93D8,
    GL_PIXEL_TILE_GRID_DEPTH_SGIX                                              = 0x8144,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5_KHR         = 0x93D8,
    GL_PIXEL_TILE_CACHE_SIZE_SGIX                                              = 0x8145,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6             = 0x93D9,
    GL_SPRITE_SGIX                                                             = 0x8148,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6_KHR         = 0x93D9,
    GL_SPRITE_MODE_SGIX                                                        = 0x8149,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8             = 0x93DA,
    GL_SPRITE_AXIS_SGIX                                                        = 0x814A,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8_KHR         = 0x93DA,
    GL_SPRITE_TRANSLATION_SGIX                                                 = 0x814B,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10            = 0x93DB,
    GL_TEXTURE_4D_BINDING_SGIS                                                 = 0x814F,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10_KHR        = 0x93DB,
    GL_MAX_CLIPMAP_DEPTH_SGIX                                                  = 0x8177,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10            = 0x93DC,
    GL_MAX_CLIPMAP_VIRTUAL_DEPTH_SGIX                                          = 0x8178,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10_KHR        = 0x93DC,
    GL_POST_TEXTURE_FILTER_BIAS_RANGE_SGIX                                     = 0x817B,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12            = 0x93DD,
    GL_POST_TEXTURE_FILTER_SCALE_RANGE_SGIX                                    = 0x817C,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12_KHR        = 0x93DD,
    GL_REFERENCE_PLANE_SGIX                                                    = 0x817D,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_3x3x3_OES        = 0x93E0,
    GL_REFERENCE_PLANE_EQUATION_SGIX                                           = 0x817E,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x3x3_OES        = 0x93E1,
    GL_IR_INSTRUMENT1_SGIX                                                     = 0x817F,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4x3_OES        = 0x93E2,
    GL_INSTRUMENT_MEASUREMENTS_SGIX                                            = 0x8181,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4x4_OES        = 0x93E3,
    GL_CALLIGRAPHIC_FRAGMENT_SGIX                                              = 0x8183,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4x4_OES        = 0x93E4,
    GL_FRAMEZOOM_SGIX                                                          = 0x818B,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5x4_OES        = 0x93E5,
    GL_FRAMEZOOM_FACTOR_SGIX                                                   = 0x818C,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5x5_OES        = 0x93E6,
    GL_MAX_FRAMEZOOM_FACTOR_SGIX                                               = 0x818D,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5x5_OES        = 0x93E7,
    GL_GENERATE_MIPMAP_HINT_SGIS                                               = 0x8192,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6x5_OES        = 0x93E8,
    GL_DEFORMATIONS_MASK_SGIX                                                  = 0x8196,  GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6x6_OES        = 0x93E9,
    GL_FOG_OFFSET_SGIX                                                         = 0x8198,
    GL_FOG_OFFSET_VALUE_SGIX                                                   = 0x8199,
    GL_LIGHT_MODEL_COLOR_CONTROL                                               = 0x81F8,
    GL_SHARED_TEXTURE_PALETTE_EXT                                              = 0x81FB,
    GL_MAJOR_VERSION                                                           = 0x821B,
    GL_MINOR_VERSION                                                           = 0x821C,
    GL_NUM_EXTENSIONS                                                          = 0x821D,
    GL_CONTEXT_FLAGS                                                           = 0x821E,
    GL_PROGRAM_PIPELINE_BINDING                                                = 0x825A,
    GL_MAX_VIEWPORTS                                                           = 0x825B,
    GL_VIEWPORT_SUBPIXEL_BITS                                                  = 0x825C,
    GL_VIEWPORT_BOUNDS_RANGE                                                   = 0x825D,
    GL_LAYER_PROVOKING_VERTEX                                                  = 0x825E,
    GL_VIEWPORT_INDEX_PROVOKING_VERTEX                                         = 0x825F,
    GL_MAX_COMPUTE_UNIFORM_COMPONENTS                                          = 0x8263,
    GL_MAX_COMPUTE_ATOMIC_COUNTER_BUFFERS                                      = 0x8264,
    GL_MAX_COMPUTE_ATOMIC_COUNTERS                                             = 0x8265,
    GL_MAX_COMBINED_COMPUTE_UNIFORM_COMPONENTS                                 = 0x8266,
    GL_MAX_DEBUG_GROUP_STACK_DEPTH                                             = 0x826C,
    GL_DEBUG_GROUP_STACK_DEPTH                                                 = 0x826D,
    GL_MAX_UNIFORM_LOCATIONS                                                   = 0x826E,
    GL_VERTEX_BINDING_DIVISOR                                                  = 0x82D6,
    GL_VERTEX_BINDING_OFFSET                                                   = 0x82D7,
    GL_VERTEX_BINDING_STRIDE                                                   = 0x82D8,
    GL_MAX_VERTEX_ATTRIB_RELATIVE_OFFSET                                       = 0x82D9,
    GL_MAX_VERTEX_ATTRIB_BINDINGS                                              = 0x82DA,
    GL_MAX_LABEL_LENGTH                                                        = 0x82E8,
    GL_CONVOLUTION_HINT_SGIX                                                   = 0x8316,
    GL_ASYNC_MARKER_SGIX                                                       = 0x8329,
    GL_PIXEL_TEX_GEN_MODE_SGIX                                                 = 0x832B,
    GL_ASYNC_HISTOGRAM_SGIX                                                    = 0x832C,
    GL_MAX_ASYNC_HISTOGRAM_SGIX                                                = 0x832D,
    GL_PIXEL_TEXTURE_SGIS                                                      = 0x8353,
    GL_ASYNC_TEX_IMAGE_SGIX                                                    = 0x835C,
    GL_ASYNC_DRAW_PIXELS_SGIX                                                  = 0x835D,
    GL_ASYNC_READ_PIXELS_SGIX                                                  = 0x835E,
    GL_MAX_ASYNC_TEX_IMAGE_SGIX                                                = 0x835F,
    GL_MAX_ASYNC_DRAW_PIXELS_SGIX                                              = 0x8360,
    GL_MAX_ASYNC_READ_PIXELS_SGIX                                              = 0x8361,
    GL_VERTEX_PRECLIP_SGIX                                                     = 0x83EE,
    GL_VERTEX_PRECLIP_HINT_SGIX                                                = 0x83EF,
    GL_FRAGMENT_LIGHTING_SGIX                                                  = 0x8400,
    GL_FRAGMENT_COLOR_MATERIAL_SGIX                                            = 0x8401,
    GL_FRAGMENT_COLOR_MATERIAL_FACE_SGIX                                       = 0x8402,
    GL_FRAGMENT_COLOR_MATERIAL_PARAMETER_SGIX                                  = 0x8403,
    GL_MAX_FRAGMENT_LIGHTS_SGIX                                                = 0x8404,
    GL_MAX_ACTIVE_LIGHTS_SGIX                                                  = 0x8405,
    GL_LIGHT_ENV_MODE_SGIX                                                     = 0x8407,
    GL_FRAGMENT_LIGHT_MODEL_LOCAL_VIEWER_SGIX                                  = 0x8408,
    GL_FRAGMENT_LIGHT_MODEL_TWO_SIDE_SGIX                                      = 0x8409,
    GL_FRAGMENT_LIGHT_MODEL_AMBIENT_SGIX                                       = 0x840A,
    GL_FRAGMENT_LIGHT_MODEL_NORMAL_INTERPOLATION_SGIX                          = 0x840B,
    GL_FRAGMENT_LIGHT0_SGIX                                                    = 0x840C,
    GL_PACK_RESAMPLE_SGIX                                                      = 0x842E,
    GL_UNPACK_RESAMPLE_SGIX                                                    = 0x842F,
    GL_ALIASED_POINT_SIZE_RANGE                                                = 0x846D,
    GL_ALIASED_LINE_WIDTH_RANGE                                                = 0x846E,
    GL_ACTIVE_TEXTURE                                                          = 0x84E0,
    GL_MAX_RENDERBUFFER_SIZE                                                   = 0x84E8,
    GL_TEXTURE_COMPRESSION_HINT                                                = 0x84EF,
    GL_TEXTURE_BINDING_RECTANGLE                                               = 0x84F6,
    GL_TEXTURE_BINDING_RECTANGLE_ARB                                           = 0x84F6,
    GL_TEXTURE_BINDING_RECTANGLE_NV                                            = 0x84F6,
    GL_MAX_RECTANGLE_TEXTURE_SIZE                                              = 0x84F8,
    GL_MAX_TEXTURE_LOD_BIAS                                                    = 0x84FD,
    GL_TEXTURE_BINDING_CUBE_MAP                                                = 0x8514,
    GL_TEXTURE_BINDING_CUBE_MAP_ARB                                            = 0x8514,
    GL_TEXTURE_BINDING_CUBE_MAP_EXT                                            = 0x8514,
    GL_TEXTURE_BINDING_CUBE_MAP_OES                                            = 0x8514,
    GL_MAX_CUBE_MAP_TEXTURE_SIZE                                               = 0x851C,
    GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB                                           = 0x851C,
    GL_MAX_CUBE_MAP_TEXTURE_SIZE_EXT                                           = 0x851C,
    GL_MAX_CUBE_MAP_TEXTURE_SIZE_OES                                           = 0x851C,
    GL_PACK_SUBSAMPLE_RATE_SGIX                                                = 0x85A0,
    GL_UNPACK_SUBSAMPLE_RATE_SGIX                                              = 0x85A1,
    GL_VERTEX_ARRAY_BINDING                                                    = 0x85B5,
    GL_PROGRAM_POINT_SIZE                                                      = 0x8642,
    GL_NUM_COMPRESSED_TEXTURE_FORMATS                                          = 0x86A2,
    GL_COMPRESSED_TEXTURE_FORMATS                                              = 0x86A3,
    GL_NUM_PROGRAM_BINARY_FORMATS                                              = 0x87FE,
    GL_PROGRAM_BINARY_FORMATS                                                  = 0x87FF,
    GL_STENCIL_BACK_FUNC                                                       = 0x8800,
    GL_STENCIL_BACK_FAIL                                                       = 0x8801,
    GL_STENCIL_BACK_PASS_DEPTH_FAIL                                            = 0x8802,
    GL_STENCIL_BACK_PASS_DEPTH_PASS                                            = 0x8803,
    GL_MAX_DRAW_BUFFERS                                                        = 0x8824,
    GL_BLEND_EQUATION_ALPHA                                                    = 0x883D,
    GL_MAX_VERTEX_ATTRIBS                                                      = 0x8869,
    GL_MAX_TEXTURE_IMAGE_UNITS                                                 = 0x8872,
    GL_ARRAY_BUFFER_BINDING                                                    = 0x8894,
    GL_ELEMENT_ARRAY_BUFFER_BINDING                                            = 0x8895,
    GL_PIXEL_PACK_BUFFER_BINDING                                               = 0x88ED,
    GL_PIXEL_UNPACK_BUFFER_BINDING                                             = 0x88EF,
    GL_MAX_DUAL_SOURCE_DRAW_BUFFERS                                            = 0x88FC,
    GL_MAX_ARRAY_TEXTURE_LAYERS                                                = 0x88FF,
    GL_MIN_PROGRAM_TEXEL_OFFSET                                                = 0x8904,
    GL_MAX_PROGRAM_TEXEL_OFFSET                                                = 0x8905,
    GL_SAMPLER_BINDING                                                         = 0x8919,
    GL_FRAGMENT_SHADER_ATI                                                     = 0x8920,
    GL_UNIFORM_BUFFER_BINDING                                                  = 0x8A28,
    GL_UNIFORM_BUFFER_START                                                    = 0x8A29,
    GL_UNIFORM_BUFFER_SIZE                                                     = 0x8A2A,
    GL_MAX_VERTEX_UNIFORM_BLOCKS                                               = 0x8A2B,
    GL_MAX_GEOMETRY_UNIFORM_BLOCKS                                             = 0x8A2C,
    GL_MAX_FRAGMENT_UNIFORM_BLOCKS                                             = 0x8A2D,
    GL_MAX_COMBINED_UNIFORM_BLOCKS                                             = 0x8A2E,
    GL_MAX_UNIFORM_BUFFER_BINDINGS                                             = 0x8A2F,
    GL_MAX_UNIFORM_BLOCK_SIZE                                                  = 0x8A30,
    GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS                                  = 0x8A31,
    GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS                                = 0x8A32,
    GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS                                = 0x8A33,
    GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT                                         = 0x8A34,
    GL_MAX_FRAGMENT_UNIFORM_COMPONENTS                                         = 0x8B49,
    GL_MAX_VERTEX_UNIFORM_COMPONENTS                                           = 0x8B4A,
    GL_MAX_VARYING_FLOATS                                                      = 0x8B4B,
    GL_MAX_VARYING_COMPONENTS                                                  = 0x8B4B,
    GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS                                          = 0x8B4C,
    GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS                                        = 0x8B4D,
    GL_FRAGMENT_SHADER_DERIVATIVE_HINT                                         = 0x8B8B,
    GL_CURRENT_PROGRAM                                                         = 0x8B8D,
    GL_IMPLEMENTATION_COLOR_READ_TYPE                                          = 0x8B9A,
    GL_IMPLEMENTATION_COLOR_READ_FORMAT                                        = 0x8B9B,
    GL_TEXTURE_BINDING_1D_ARRAY                                                = 0x8C1C,
    GL_TEXTURE_BINDING_2D_ARRAY                                                = 0x8C1D,
    GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS                                        = 0x8C29,
    GL_MAX_TEXTURE_BUFFER_SIZE                                                 = 0x8C2B,
    GL_TEXTURE_BINDING_BUFFER                                                  = 0x8C2C,
    GL_TRANSFORM_FEEDBACK_BUFFER_START                                         = 0x8C84,
    GL_TRANSFORM_FEEDBACK_BUFFER_SIZE                                          = 0x8C85,
    GL_TRANSFORM_FEEDBACK_BUFFER_BINDING                                       = 0x8C8F,
    GL_MOTION_ESTIMATION_SEARCH_BLOCK_X_QCOM                                   = 0x8C90,
    GL_MOTION_ESTIMATION_SEARCH_BLOCK_Y_QCOM                                   = 0x8C91,
    GL_STENCIL_BACK_REF                                                        = 0x8CA3,
    GL_STENCIL_BACK_VALUE_MASK                                                 = 0x8CA4,
    GL_STENCIL_BACK_WRITEMASK                                                  = 0x8CA5,
    GL_DRAW_FRAMEBUFFER_BINDING                                                = 0x8CA6,
    GL_RENDERBUFFER_BINDING                                                    = 0x8CA7,
    GL_READ_FRAMEBUFFER_BINDING                                                = 0x8CAA,
    GL_MAX_COLOR_ATTACHMENTS                                                   = 0x8CDF,
    GL_MAX_COLOR_ATTACHMENTS_EXT                                               = 0x8CDF,
    GL_MAX_COLOR_ATTACHMENTS_NV                                                = 0x8CDF,
    GL_TEXTURE_GEN_STR_OES                                                     = 0x8D60,
    GL_MAX_ELEMENT_INDEX                                                       = 0x8D6B,
    GL_MAX_GEOMETRY_UNIFORM_COMPONENTS                                         = 0x8DDF,
    GL_SHADER_BINARY_FORMATS                                                   = 0x8DF8,
    GL_NUM_SHADER_BINARY_FORMATS                                               = 0x8DF9,
    GL_SHADER_COMPILER                                                         = 0x8DFA,
    GL_MAX_VERTEX_UNIFORM_VECTORS                                              = 0x8DFB,
    GL_MAX_VARYING_VECTORS                                                     = 0x8DFC,
    GL_MAX_FRAGMENT_UNIFORM_VECTORS                                            = 0x8DFD,
    GL_TIMESTAMP                                                               = 0x8E28,
    GL_TIMESTAMP_EXT                                                           = 0x8E28,
    GL_PROVOKING_VERTEX                                                        = 0x8E4F,
    GL_MAX_SAMPLE_MASK_WORDS                                                   = 0x8E59,
    GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS                                         = 0x8E89,
    GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS                                      = 0x8E8A,
    GL_FETCH_PER_SAMPLE_ARM                                                    = 0x8F65,
    GL_FRAGMENT_SHADER_FRAMEBUFFER_FETCH_MRT_ARM                               = 0x8F66,
    GL_FRAGMENT_SHADING_RATE_NON_TRIVIAL_COMBINERS_SUPPORTED_EXT               = 0x8F6F,
    GL_PRIMITIVE_RESTART_INDEX                                                 = 0x8F9E,
    GL_MIN_MAP_BUFFER_ALIGNMENT                                                = 0x90BC,
    GL_SHADER_STORAGE_BUFFER_BINDING                                           = 0x90D3,
    GL_SHADER_STORAGE_BUFFER_START                                             = 0x90D4,
    GL_SHADER_STORAGE_BUFFER_SIZE                                              = 0x90D5,
    GL_MAX_VERTEX_SHADER_STORAGE_BLOCKS                                        = 0x90D6,
    GL_MAX_GEOMETRY_SHADER_STORAGE_BLOCKS                                      = 0x90D7,
    GL_MAX_TESS_CONTROL_SHADER_STORAGE_BLOCKS                                  = 0x90D8,
    GL_MAX_TESS_EVALUATION_SHADER_STORAGE_BLOCKS                               = 0x90D9,
    GL_MAX_FRAGMENT_SHADER_STORAGE_BLOCKS                                      = 0x90DA,
    GL_MAX_COMPUTE_SHADER_STORAGE_BLOCKS                                       = 0x90DB,
    GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS                                      = 0x90DC,
    GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS                                      = 0x90DD,
    GL_SHADER_STORAGE_BUFFER_OFFSET_ALIGNMENT                                  = 0x90DF,
    GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS                                      = 0x90EB,
    GL_DISPATCH_INDIRECT_BUFFER_BINDING                                        = 0x90EF,
    GL_TEXTURE_BINDING_2D_MULTISAMPLE                                          = 0x9104,
    GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY                                    = 0x9105,
    GL_MAX_COLOR_TEXTURE_SAMPLES                                               = 0x910E,
    GL_MAX_DEPTH_TEXTURE_SAMPLES                                               = 0x910F,
    GL_MAX_INTEGER_SAMPLES                                                     = 0x9110,
    GL_MAX_SERVER_WAIT_TIMEOUT                                                 = 0x9111,
    GL_MAX_VERTEX_OUTPUT_COMPONENTS                                            = 0x9122,
    GL_MAX_GEOMETRY_INPUT_COMPONENTS                                           = 0x9123,
    GL_MAX_GEOMETRY_OUTPUT_COMPONENTS                                          = 0x9124,
    GL_MAX_FRAGMENT_INPUT_COMPONENTS                                           = 0x9125,
    GL_CONTEXT_PROFILE_MASK                                                    = 0x9126,
    GL_TEXTURE_BUFFER_OFFSET_ALIGNMENT                                         = 0x919F,
    GL_MAX_COMPUTE_UNIFORM_BLOCKS                                              = 0x91BB,
    GL_MAX_COMPUTE_TEXTURE_IMAGE_UNITS                                         = 0x91BC,
    GL_MAX_COMPUTE_WORK_GROUP_COUNT                                            = 0x91BE,
    GL_MAX_COMPUTE_WORK_GROUP_SIZE                                             = 0x91BF,
    GL_MAX_VERTEX_ATOMIC_COUNTERS                                              = 0x92D2,
    GL_MAX_TESS_CONTROL_ATOMIC_COUNTERS                                        = 0x92D3,
    GL_MAX_TESS_EVALUATION_ATOMIC_COUNTERS                                     = 0x92D4,
    GL_MAX_GEOMETRY_ATOMIC_COUNTERS                                            = 0x92D5,
    GL_MAX_FRAGMENT_ATOMIC_COUNTERS                                            = 0x92D6,
    GL_MAX_COMBINED_ATOMIC_COUNTERS                                            = 0x92D7,
    GL_MAX_FRAMEBUFFER_WIDTH                                                   = 0x9315,
    GL_MAX_FRAMEBUFFER_HEIGHT                                                  = 0x9316,
    GL_MAX_FRAMEBUFFER_LAYERS                                                  = 0x9317,
    GL_MAX_FRAMEBUFFER_SAMPLES                                                 = 0x9318,
    GL_NUM_DEVICE_UUIDS_EXT                                                    = 0x9596,
    GL_DEVICE_UUID_EXT                                                         = 0x9597,
    GL_DRIVER_UUID_EXT                                                         = 0x9598,
    GL_DEVICE_LUID_EXT                                                         = 0x9599,
    GL_DEVICE_NODE_MASK_EXT                                                    = 0x959A,
    GL_SHADING_RATE_IMAGE_PER_PRIMITIVE_NV                                     = 0x95B1,
    GL_SHADING_RATE_IMAGE_PALETTE_COUNT_NV                                     = 0x95B2,
    GL_MAX_TIMELINE_SEMAPHORE_VALUE_DIFFERENCE_NV                              = 0x95B6,
    GL_FRAMEBUFFER_FETCH_NONCOHERENT_QCOM                                      = 0x96A2,
    GL_SHADING_RATE_QCOM                                                       = 0x96A4,
    GL_SHADING_RATE_EXT                                                        = 0x96D0,
    GL_MIN_FRAGMENT_SHADING_RATE_ATTACHMENT_TEXEL_WIDTH_EXT                    = 0x96D7,
    GL_MAX_FRAGMENT_SHADING_RATE_ATTACHMENT_TEXEL_WIDTH_EXT                    = 0x96D8,
    GL_MIN_FRAGMENT_SHADING_RATE_ATTACHMENT_TEXEL_HEIGHT_EXT                   = 0x96D9,
    GL_MAX_FRAGMENT_SHADING_RATE_ATTACHMENT_TEXEL_HEIGHT_EXT                   = 0x96DA,
    GL_MAX_FRAGMENT_SHADING_RATE_ATTACHMENT_TEXEL_ASPECT_RATIO_EXT             = 0x96DB,
    GL_MAX_FRAGMENT_SHADING_RATE_ATTACHMENT_LAYERS_EXT                         = 0x96DC,
    GL_FRAGMENT_SHADING_RATE_WITH_SHADER_DEPTH_STENCIL_WRITES_SUPPORTED_EXT    = 0x96DD,
    GL_FRAGMENT_SHADING_RATE_WITH_SAMPLE_MASK_SUPPORTED_EXT                    = 0x96DE,
    GL_FRAGMENT_SHADING_RATE_ATTACHMENT_WITH_DEFAULT_FRAMEBUFFER_SUPPORTED_EXT = 0x96DF,
    GL_SHADER_CORE_COUNT_ARM                                                   = 0x96F0,
    GL_SHADER_CORE_ACTIVE_COUNT_ARM                                            = 0x96F1,
    GL_SHADER_CORE_PRESENT_MASK_ARM                                            = 0x96F2,
    GL_SHADER_CORE_MAX_WARP_COUNT_ARM                                          = 0x96F3,
    GL_SHADER_CORE_PIXEL_RATE_ARM                                              = 0x96F4,
    GL_SHADER_CORE_TEXEL_RATE_ARM                                              = 0x96F5,
    GL_SHADER_CORE_FMA_RATE_ARM                                                = 0x96F6,
    GL_MAX_TASK_WORK_GROUP_TOTAL_COUNT_EXT                                     = 0x9740,
    GL_MAX_MESH_WORK_GROUP_TOTAL_COUNT_EXT                                     = 0x9741,
    GL_MAX_TASK_PAYLOAD_SIZE_EXT                                               = 0x9742,
    GL_MAX_TASK_SHARED_MEMORY_SIZE_EXT                                         = 0x9743,
    GL_MAX_MESH_SHARED_MEMORY_SIZE_EXT                                         = 0x9744,
    GL_MAX_TASK_PAYLOAD_AND_SHARED_MEMORY_SIZE_EXT                             = 0x9745,
    GL_MAX_MESH_PAYLOAD_AND_SHARED_MEMORY_SIZE_EXT                             = 0x9746,
    GL_MAX_MESH_OUTPUT_MEMORY_SIZE_EXT                                         = 0x9747,
    GL_MAX_MESH_PAYLOAD_AND_OUTPUT_MEMORY_SIZE_EXT                             = 0x9748,
    GL_MAX_MESH_OUTPUT_COMPONENTS_EXT                                          = 0x9749,
    GL_MAX_MESH_OUTPUT_LAYERS_EXT                                              = 0x974A,
    GL_MAX_PREFERRED_TASK_WORK_GROUP_INVOCATIONS_EXT                           = 0x974B,
    GL_MAX_PREFERRED_MESH_WORK_GROUP_INVOCATIONS_EXT                           = 0x974C,
    GL_MESH_PREFERS_LOCAL_INVOCATION_VERTEX_OUTPUT_EXT                         = 0x974D,
    GL_MESH_PREFERS_LOCAL_INVOCATION_PRIMITIVE_OUTPUT_EXT                      = 0x974E,
    GL_MESH_PREFERS_COMPACT_VERTEX_OUTPUT_EXT                                  = 0x974F,
    GL_MESH_PREFERS_COMPACT_PRIMITIVE_OUTPUT_EXT                               = 0x9750,
    GL_MAX_TASK_WORK_GROUP_COUNT_EXT                                           = 0x9751,
    GL_MAX_MESH_WORK_GROUP_COUNT_EXT                                           = 0x9752,
    GL_MAX_MESH_OUTPUT_PRIMITIVES_EXT                                          = 0x9756,
    GL_MAX_MESH_WORK_GROUP_INVOCATIONS_EXT                                     = 0x9757,
    GL_MAX_MESH_WORK_GROUP_SIZE_EXT                                            = 0x9758,
    GL_MAX_TASK_WORK_GROUP_INVOCATIONS_EXT                                     = 0x9759,
    GL_MAX_TASK_WORK_GROUP_SIZE_EXT                                            = 0x975A,
    /* TextureParameterName */                         /* TextureEnvParameter */        /* InternalFormatPName */
    GL_TEXTURE_WIDTH                        = 0x1000,  GL_TEXTURE_ENV_MODE   = 0x2200,  GL_INTERNALFORMAT_SUPPORTED                = 0x826F,
    GL_TEXTURE_HEIGHT                       = 0x1001,  GL_TEXTURE_ENV_COLOR  = 0x2201,  GL_INTERNALFORMAT_PREFERRED                = 0x8270,
    GL_TEXTURE_INTERNAL_FORMAT              = 0x1003,  GL_TEXTURE_LOD_BIAS   = 0x8501,  GL_INTERNALFORMAT_RED_SIZE                 = 0x8271,
    GL_TEXTURE_COMPONENTS                   = 0x1003,  GL_COMBINE_ARB        = 0x8570,  GL_INTERNALFORMAT_GREEN_SIZE               = 0x8272,
    GL_TEXTURE_BORDER_COLOR                 = 0x1004,  GL_COMBINE_EXT        = 0x8570,  GL_INTERNALFORMAT_BLUE_SIZE                = 0x8273,
    GL_TEXTURE_BORDER_COLOR_NV              = 0x1004,  GL_COMBINE_RGB        = 0x8571,  GL_INTERNALFORMAT_ALPHA_SIZE               = 0x8274,
    GL_TEXTURE_BORDER                       = 0x1005,  GL_COMBINE_RGB_ARB    = 0x8571,  GL_INTERNALFORMAT_DEPTH_SIZE               = 0x8275,
    GL_TEXTURE_MAG_FILTER                   = 0x2800,  GL_COMBINE_RGB_EXT    = 0x8571,  GL_INTERNALFORMAT_STENCIL_SIZE             = 0x8276,
    GL_TEXTURE_MIN_FILTER                   = 0x2801,  GL_COMBINE_ALPHA      = 0x8572,  GL_INTERNALFORMAT_SHARED_SIZE              = 0x8277,
    GL_TEXTURE_WRAP_S                       = 0x2802,  GL_COMBINE_ALPHA_ARB  = 0x8572,  GL_INTERNALFORMAT_RED_TYPE                 = 0x8278,
    GL_TEXTURE_WRAP_T                       = 0x2803,  GL_COMBINE_ALPHA_EXT  = 0x8572,  GL_INTERNALFORMAT_GREEN_TYPE               = 0x8279,
    GL_TEXTURE_RED_SIZE                     = 0x805C,  GL_RGB_SCALE          = 0x8573,  GL_INTERNALFORMAT_BLUE_TYPE                = 0x827A,
    GL_TEXTURE_GREEN_SIZE                   = 0x805D,  GL_RGB_SCALE_ARB      = 0x8573,  GL_INTERNALFORMAT_ALPHA_TYPE               = 0x827B,
    GL_TEXTURE_BLUE_SIZE                    = 0x805E,  GL_RGB_SCALE_EXT      = 0x8573,  GL_INTERNALFORMAT_DEPTH_TYPE               = 0x827C,
    GL_TEXTURE_ALPHA_SIZE                   = 0x805F,  GL_ADD_SIGNED         = 0x8574,  GL_INTERNALFORMAT_STENCIL_TYPE             = 0x827D,
    GL_TEXTURE_LUMINANCE_SIZE               = 0x8060,  GL_ADD_SIGNED_ARB     = 0x8574,  GL_MAX_WIDTH                               = 0x827E,
    GL_TEXTURE_INTENSITY_SIZE               = 0x8061,  GL_ADD_SIGNED_EXT     = 0x8574,  GL_MAX_HEIGHT                              = 0x827F,
    GL_TEXTURE_PRIORITY                     = 0x8066,  GL_INTERPOLATE        = 0x8575,  GL_MAX_DEPTH                               = 0x8280,
    GL_TEXTURE_PRIORITY_EXT                 = 0x8066,  GL_INTERPOLATE_ARB    = 0x8575,  GL_MAX_LAYERS                              = 0x8281,
    GL_TEXTURE_RESIDENT                     = 0x8067,  GL_INTERPOLATE_EXT    = 0x8575,  GL_COLOR_COMPONENTS                        = 0x8283,
    GL_TEXTURE_DEPTH_EXT                    = 0x8071,  GL_CONSTANT_ARB       = 0x8576,  GL_COLOR_RENDERABLE                        = 0x8286,
    GL_TEXTURE_WRAP_R                       = 0x8072,  GL_CONSTANT_EXT       = 0x8576,  GL_DEPTH_RENDERABLE                        = 0x8287,
    GL_TEXTURE_WRAP_R_EXT                   = 0x8072,  GL_CONSTANT_NV        = 0x8576,  GL_STENCIL_RENDERABLE                      = 0x8288,
    GL_TEXTURE_WRAP_R_OES                   = 0x8072,  GL_PREVIOUS           = 0x8578,  GL_FRAMEBUFFER_RENDERABLE                  = 0x8289,
    GL_DETAIL_TEXTURE_LEVEL_SGIS            = 0x809A,  GL_PREVIOUS_ARB       = 0x8578,  GL_FRAMEBUFFER_RENDERABLE_LAYERED          = 0x828A,
    GL_DETAIL_TEXTURE_MODE_SGIS             = 0x809B,  GL_PREVIOUS_EXT       = 0x8578,  GL_FRAMEBUFFER_BLEND                       = 0x828B,
    GL_DETAIL_TEXTURE_FUNC_POINTS_SGIS      = 0x809C,  GL_SOURCE0_RGB        = 0x8580,  GL_READ_PIXELS                             = 0x828C,
    GL_SHARPEN_TEXTURE_FUNC_POINTS_SGIS     = 0x80B0,  GL_SOURCE0_RGB_ARB    = 0x8580,  GL_READ_PIXELS_FORMAT                      = 0x828D,
    GL_SHADOW_AMBIENT_SGIX                  = 0x80BF,  GL_SOURCE0_RGB_EXT    = 0x8580,  GL_READ_PIXELS_TYPE                        = 0x828E,
    GL_DUAL_TEXTURE_SELECT_SGIS             = 0x8124,  GL_SRC0_RGB           = 0x8580,  GL_TEXTURE_IMAGE_FORMAT                    = 0x828F,
    GL_QUAD_TEXTURE_SELECT_SGIS             = 0x8125,  GL_SOURCE1_RGB        = 0x8581,  GL_TEXTURE_IMAGE_TYPE                      = 0x8290,
    GL_TEXTURE_4DSIZE_SGIS                  = 0x8136,  GL_SOURCE1_RGB_ARB    = 0x8581,  GL_GET_TEXTURE_IMAGE_FORMAT                = 0x8291,
    GL_TEXTURE_WRAP_Q_SGIS                  = 0x8137,  GL_SOURCE1_RGB_EXT    = 0x8581,  GL_GET_TEXTURE_IMAGE_TYPE                  = 0x8292,
    GL_TEXTURE_MIN_LOD                      = 0x813A,  GL_SRC1_RGB           = 0x8581,  GL_MIPMAP                                  = 0x8293,
    GL_TEXTURE_MIN_LOD_SGIS                 = 0x813A,  GL_SOURCE2_RGB        = 0x8582,  GL_AUTO_GENERATE_MIPMAP                    = 0x8295,
    GL_TEXTURE_MAX_LOD                      = 0x813B,  GL_SOURCE2_RGB_ARB    = 0x8582,  GL_COLOR_ENCODING                          = 0x8296,
    GL_TEXTURE_MAX_LOD_SGIS                 = 0x813B,  GL_SOURCE2_RGB_EXT    = 0x8582,  GL_SRGB_READ                               = 0x8297,
    GL_TEXTURE_BASE_LEVEL                   = 0x813C,  GL_SRC2_RGB           = 0x8582,  GL_SRGB_WRITE                              = 0x8298,
    GL_TEXTURE_BASE_LEVEL_SGIS              = 0x813C,  GL_SOURCE3_RGB_NV     = 0x8583,  GL_FILTER                                  = 0x829A,
    GL_TEXTURE_MAX_LEVEL                    = 0x813D,  GL_SOURCE0_ALPHA      = 0x8588,  GL_VERTEX_TEXTURE                          = 0x829B,
    GL_TEXTURE_MAX_LEVEL_SGIS               = 0x813D,  GL_SOURCE0_ALPHA_ARB  = 0x8588,  GL_TESS_CONTROL_TEXTURE                    = 0x829C,
    GL_TEXTURE_FILTER4_SIZE_SGIS            = 0x8147,  GL_SOURCE0_ALPHA_EXT  = 0x8588,  GL_TESS_EVALUATION_TEXTURE                 = 0x829D,
    GL_TEXTURE_CLIPMAP_CENTER_SGIX          = 0x8171,  GL_SRC0_ALPHA         = 0x8588,  GL_GEOMETRY_TEXTURE                        = 0x829E,
    GL_TEXTURE_CLIPMAP_FRAME_SGIX           = 0x8172,  GL_SOURCE1_ALPHA      = 0x8589,  GL_FRAGMENT_TEXTURE                        = 0x829F,
    GL_TEXTURE_CLIPMAP_OFFSET_SGIX          = 0x8173,  GL_SOURCE1_ALPHA_ARB  = 0x8589,  GL_COMPUTE_TEXTURE                         = 0x82A0,
    GL_TEXTURE_CLIPMAP_VIRTUAL_DEPTH_SGIX   = 0x8174,  GL_SOURCE1_ALPHA_EXT  = 0x8589,  GL_TEXTURE_SHADOW                          = 0x82A1,
    GL_TEXTURE_CLIPMAP_LOD_OFFSET_SGIX      = 0x8175,  GL_SRC1_ALPHA_EXT     = 0x8589,  GL_TEXTURE_GATHER                          = 0x82A2,
    GL_TEXTURE_CLIPMAP_DEPTH_SGIX           = 0x8176,  GL_SOURCE2_ALPHA      = 0x858A,  GL_TEXTURE_GATHER_SHADOW                   = 0x82A3,
    GL_POST_TEXTURE_FILTER_BIAS_SGIX        = 0x8179,  GL_SOURCE2_ALPHA_ARB  = 0x858A,  GL_SHADER_IMAGE_LOAD                       = 0x82A4,
    GL_POST_TEXTURE_FILTER_SCALE_SGIX       = 0x817A,  GL_SOURCE2_ALPHA_EXT  = 0x858A,  GL_SHADER_IMAGE_STORE                      = 0x82A5,
    GL_TEXTURE_LOD_BIAS_S_SGIX              = 0x818E,  GL_SRC2_ALPHA         = 0x858A,  GL_SHADER_IMAGE_ATOMIC                     = 0x82A6,
    GL_TEXTURE_LOD_BIAS_T_SGIX              = 0x818F,  GL_SOURCE3_ALPHA_NV   = 0x858B,  GL_IMAGE_TEXEL_SIZE                        = 0x82A7,
    GL_TEXTURE_LOD_BIAS_R_SGIX              = 0x8190,  GL_OPERAND0_RGB       = 0x8590,  GL_IMAGE_COMPATIBILITY_CLASS               = 0x82A8,
    GL_GENERATE_MIPMAP                      = 0x8191,  GL_OPERAND0_RGB_ARB   = 0x8590,  GL_IMAGE_PIXEL_FORMAT                      = 0x82A9,
    GL_GENERATE_MIPMAP_SGIS                 = 0x8191,  GL_OPERAND0_RGB_EXT   = 0x8590,  GL_IMAGE_PIXEL_TYPE                        = 0x82AA,
    GL_TEXTURE_COMPARE_SGIX                 = 0x819A,  GL_OPERAND1_RGB       = 0x8591,  GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_TEST     = 0x82AC,
    GL_TEXTURE_COMPARE_OPERATOR_SGIX        = 0x819B,  GL_OPERAND1_RGB_ARB   = 0x8591,  GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_TEST   = 0x82AD,
    GL_TEXTURE_LEQUAL_R_SGIX                = 0x819C,  GL_OPERAND1_RGB_EXT   = 0x8591,  GL_SIMULTANEOUS_TEXTURE_AND_DEPTH_WRITE    = 0x82AE,
    GL_TEXTURE_GEQUAL_R_SGIX                = 0x819D,  GL_OPERAND2_RGB       = 0x8592,  GL_SIMULTANEOUS_TEXTURE_AND_STENCIL_WRITE  = 0x82AF,
    GL_TEXTURE_MAX_CLAMP_S_SGIX             = 0x8369,  GL_OPERAND2_RGB_ARB   = 0x8592,  GL_TEXTURE_COMPRESSED_BLOCK_WIDTH          = 0x82B1,
    GL_TEXTURE_MAX_CLAMP_T_SGIX             = 0x836A,  GL_OPERAND2_RGB_EXT   = 0x8592,  GL_TEXTURE_COMPRESSED_BLOCK_HEIGHT         = 0x82B2,
    GL_TEXTURE_MAX_CLAMP_R_SGIX             = 0x836B,  GL_OPERAND3_RGB_NV    = 0x8593,  GL_TEXTURE_COMPRESSED_BLOCK_SIZE           = 0x82B3,
    GL_TEXTURE_MEMORY_LAYOUT_INTEL          = 0x83FF,  GL_OPERAND0_ALPHA     = 0x8598,  GL_CLEAR_BUFFER                            = 0x82B4,
    GL_TEXTURE_MAX_ANISOTROPY               = 0x84FE,  GL_OPERAND0_ALPHA_ARB = 0x8598,  GL_TEXTURE_VIEW                            = 0x82B5,
    GL_TEXTURE_COMPARE_MODE                 = 0x884C,  GL_OPERAND0_ALPHA_EXT = 0x8598,  GL_VIEW_COMPATIBILITY_CLASS                = 0x82B6,
    GL_TEXTURE_COMPARE_FUNC                 = 0x884D,  GL_OPERAND1_ALPHA     = 0x8599,  GL_TEXTURE_COMPRESSED                      = 0x86A1,
    GL_TEXTURE_SWIZZLE_R                    = 0x8E42,  GL_OPERAND1_ALPHA_ARB = 0x8599,  GL_NUM_SURFACE_COMPRESSION_FIXED_RATES_EXT = 0x8F6E,
    GL_TEXTURE_SWIZZLE_G                    = 0x8E43,  GL_OPERAND1_ALPHA_EXT = 0x8599,  GL_IMAGE_FORMAT_COMPATIBILITY_TYPE         = 0x90C7,
    GL_TEXTURE_SWIZZLE_B                    = 0x8E44,  GL_OPERAND2_ALPHA     = 0x859A,  GL_CLEAR_TEXTURE                           = 0x9365,
    GL_TEXTURE_SWIZZLE_A                    = 0x8E45,  GL_OPERAND2_ALPHA_ARB = 0x859A,  GL_NUM_SAMPLE_COUNTS                       = 0x9380,
    GL_TEXTURE_SWIZZLE_RGBA                 = 0x8E46,  GL_OPERAND2_ALPHA_EXT = 0x859A,
    GL_TEXTURE_UNNORMALIZED_COORDINATES_ARM = 0x8F6A,  GL_OPERAND3_ALPHA_NV  = 0x859B,
    GL_DEPTH_STENCIL_TEXTURE_MODE           = 0x90EA,  GL_COORD_REPLACE      = 0x8862,
    GL_TEXTURE_TILING_EXT                   = 0x9580,
    GL_TEXTURE_FOVEATED_CUTOFF_DENSITY_QCOM = 0x96A0,
    GL_TEXTURE_Y_DEGAMMA_QCOM               = 0x9710,
    GL_TEXTURE_CBCR_DEGAMMA_QCOM            = 0x9711,
    /* FragmentShaderGenericSourceATI */     /* DrawBufferMode */                /* TextureTarget */
    GL_PRIMARY_COLOR              = 0x8577,  GL_FRONT_LEFT            = 0x0400,  GL_PROXY_TEXTURE_1D                   = 0x8063,
    GL_PRIMARY_COLOR_ARB          = 0x8577,  GL_FRONT_RIGHT           = 0x0401,  GL_PROXY_TEXTURE_1D_EXT               = 0x8063,
    GL_PRIMARY_COLOR_EXT          = 0x8577,  GL_BACK_LEFT             = 0x0402,  GL_PROXY_TEXTURE_2D                   = 0x8064,
    GL_REG_0_ATI                  = 0x8921,  GL_BACK_RIGHT            = 0x0403,  GL_PROXY_TEXTURE_2D_EXT               = 0x8064,
    GL_REG_1_ATI                  = 0x8922,  GL_FRONT                 = 0x0404,  GL_TEXTURE_3D_OES                     = 0x806F,
    GL_REG_2_ATI                  = 0x8923,  GL_BACK                  = 0x0405,  GL_PROXY_TEXTURE_3D                   = 0x8070,
    GL_REG_3_ATI                  = 0x8924,  GL_LEFT                  = 0x0406,  GL_PROXY_TEXTURE_3D_EXT               = 0x8070,
    GL_REG_4_ATI                  = 0x8925,  GL_RIGHT                 = 0x0407,  GL_DETAIL_TEXTURE_2D_SGIS             = 0x8095,
    GL_REG_5_ATI                  = 0x8926,  GL_FRONT_AND_BACK        = 0x0408,  GL_PROXY_TEXTURE_4D_SGIS              = 0x8135,
    GL_REG_6_ATI                  = 0x8927,  GL_AUX0                  = 0x0409,  GL_PROXY_TEXTURE_RECTANGLE            = 0x84F7,
    GL_REG_7_ATI                  = 0x8928,  GL_AUX1                  = 0x040A,  GL_PROXY_TEXTURE_RECTANGLE_ARB        = 0x84F7,
    GL_REG_8_ATI                  = 0x8929,  GL_AUX2                  = 0x040B,  GL_PROXY_TEXTURE_RECTANGLE_NV         = 0x84F7,
    GL_REG_9_ATI                  = 0x892A,  GL_AUX3                  = 0x040C,  GL_TEXTURE_CUBE_MAP_POSITIVE_X        = 0x8515,
    GL_REG_10_ATI                 = 0x892B,  GL_COLOR_ATTACHMENT0     = 0x8CE0,  GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB    = 0x8515,
    GL_REG_11_ATI                 = 0x892C,  GL_COLOR_ATTACHMENT0_NV  = 0x8CE0,  GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT    = 0x8515,
    GL_REG_12_ATI                 = 0x892D,  GL_COLOR_ATTACHMENT1     = 0x8CE1,  GL_TEXTURE_CUBE_MAP_POSITIVE_X_OES    = 0x8515,
    GL_REG_13_ATI                 = 0x892E,  GL_COLOR_ATTACHMENT1_NV  = 0x8CE1,  GL_TEXTURE_CUBE_MAP_NEGATIVE_X        = 0x8516,
    GL_REG_14_ATI                 = 0x892F,  GL_COLOR_ATTACHMENT2     = 0x8CE2,  GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB    = 0x8516,
    GL_REG_15_ATI                 = 0x8930,  GL_COLOR_ATTACHMENT2_NV  = 0x8CE2,  GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT    = 0x8516,
    GL_REG_16_ATI                 = 0x8931,  GL_COLOR_ATTACHMENT3     = 0x8CE3,  GL_TEXTURE_CUBE_MAP_NEGATIVE_X_OES    = 0x8516,
    GL_REG_17_ATI                 = 0x8932,  GL_COLOR_ATTACHMENT3_NV  = 0x8CE3,  GL_TEXTURE_CUBE_MAP_POSITIVE_Y        = 0x8517,
    GL_REG_18_ATI                 = 0x8933,  GL_COLOR_ATTACHMENT4     = 0x8CE4,  GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB    = 0x8517,
    GL_REG_19_ATI                 = 0x8934,  GL_COLOR_ATTACHMENT4_NV  = 0x8CE4,  GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT    = 0x8517,
    GL_REG_20_ATI                 = 0x8935,  GL_COLOR_ATTACHMENT5     = 0x8CE5,  GL_TEXTURE_CUBE_MAP_POSITIVE_Y_OES    = 0x8517,
    GL_REG_21_ATI                 = 0x8936,  GL_COLOR_ATTACHMENT5_NV  = 0x8CE5,  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y        = 0x8518,
    GL_REG_22_ATI                 = 0x8937,  GL_COLOR_ATTACHMENT6     = 0x8CE6,  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB    = 0x8518,
    GL_REG_23_ATI                 = 0x8938,  GL_COLOR_ATTACHMENT6_NV  = 0x8CE6,  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT    = 0x8518,
    GL_REG_24_ATI                 = 0x8939,  GL_COLOR_ATTACHMENT7     = 0x8CE7,  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_OES    = 0x8518,
    GL_REG_25_ATI                 = 0x893A,  GL_COLOR_ATTACHMENT7_NV  = 0x8CE7,  GL_TEXTURE_CUBE_MAP_POSITIVE_Z        = 0x8519,
    GL_REG_26_ATI                 = 0x893B,  GL_COLOR_ATTACHMENT8     = 0x8CE8,  GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB    = 0x8519,
    GL_REG_27_ATI                 = 0x893C,  GL_COLOR_ATTACHMENT8_NV  = 0x8CE8,  GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT    = 0x8519,
    GL_REG_28_ATI                 = 0x893D,  GL_COLOR_ATTACHMENT9     = 0x8CE9,  GL_TEXTURE_CUBE_MAP_POSITIVE_Z_OES    = 0x8519,
    GL_REG_29_ATI                 = 0x893E,  GL_COLOR_ATTACHMENT9_NV  = 0x8CE9,  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z        = 0x851A,
    GL_REG_30_ATI                 = 0x893F,  GL_COLOR_ATTACHMENT10    = 0x8CEA,  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB    = 0x851A,
    GL_REG_31_ATI                 = 0x8940,  GL_COLOR_ATTACHMENT10_NV = 0x8CEA,  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT    = 0x851A,
    GL_CON_0_ATI                  = 0x8941,  GL_COLOR_ATTACHMENT11    = 0x8CEB,  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_OES    = 0x851A,
    GL_CON_1_ATI                  = 0x8942,  GL_COLOR_ATTACHMENT11_NV = 0x8CEB,  GL_PROXY_TEXTURE_CUBE_MAP             = 0x851B,
    GL_CON_2_ATI                  = 0x8943,  GL_COLOR_ATTACHMENT12    = 0x8CEC,  GL_PROXY_TEXTURE_CUBE_MAP_ARB         = 0x851B,
    GL_CON_3_ATI                  = 0x8944,  GL_COLOR_ATTACHMENT12_NV = 0x8CEC,  GL_PROXY_TEXTURE_CUBE_MAP_EXT         = 0x851B,
    GL_CON_4_ATI                  = 0x8945,  GL_COLOR_ATTACHMENT13    = 0x8CED,  GL_PROXY_TEXTURE_1D_ARRAY             = 0x8C19,
    GL_CON_5_ATI                  = 0x8946,  GL_COLOR_ATTACHMENT13_NV = 0x8CED,  GL_PROXY_TEXTURE_1D_ARRAY_EXT         = 0x8C19,
    GL_CON_6_ATI                  = 0x8947,  GL_COLOR_ATTACHMENT14    = 0x8CEE,  GL_PROXY_TEXTURE_2D_ARRAY             = 0x8C1B,
    GL_CON_7_ATI                  = 0x8948,  GL_COLOR_ATTACHMENT14_NV = 0x8CEE,  GL_PROXY_TEXTURE_2D_ARRAY_EXT         = 0x8C1B,
    GL_CON_8_ATI                  = 0x8949,  GL_COLOR_ATTACHMENT15    = 0x8CEF,  GL_TEXTURE_BUFFER                     = 0x8C2A,
    GL_CON_9_ATI                  = 0x894A,  GL_COLOR_ATTACHMENT15_NV = 0x8CEF,  GL_TEXTURE_CUBE_MAP_ARRAY_ARB         = 0x9009,
    GL_CON_10_ATI                 = 0x894B,  GL_COLOR_ATTACHMENT16    = 0x8CF0,  GL_TEXTURE_CUBE_MAP_ARRAY_EXT         = 0x9009,
    GL_CON_11_ATI                 = 0x894C,  GL_COLOR_ATTACHMENT17    = 0x8CF1,  GL_TEXTURE_CUBE_MAP_ARRAY_OES         = 0x9009,
    GL_CON_12_ATI                 = 0x894D,  GL_COLOR_ATTACHMENT18    = 0x8CF2,  GL_PROXY_TEXTURE_CUBE_MAP_ARRAY       = 0x900B,
    GL_CON_13_ATI                 = 0x894E,  GL_COLOR_ATTACHMENT19    = 0x8CF3,  GL_PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB   = 0x900B,
    GL_CON_14_ATI                 = 0x894F,  GL_COLOR_ATTACHMENT20    = 0x8CF4,  GL_PROXY_TEXTURE_2D_MULTISAMPLE       = 0x9101,
    GL_CON_15_ATI                 = 0x8950,  GL_COLOR_ATTACHMENT21    = 0x8CF5,  GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY = 0x9103,
    GL_CON_16_ATI                 = 0x8951,  GL_COLOR_ATTACHMENT22    = 0x8CF6,
    GL_CON_17_ATI                 = 0x8952,  GL_COLOR_ATTACHMENT23    = 0x8CF7,
    GL_CON_18_ATI                 = 0x8953,  GL_COLOR_ATTACHMENT24    = 0x8CF8,
    GL_CON_19_ATI                 = 0x8954,  GL_COLOR_ATTACHMENT25    = 0x8CF9,
    GL_CON_20_ATI                 = 0x8955,  GL_COLOR_ATTACHMENT26    = 0x8CFA,
    GL_CON_21_ATI                 = 0x8956,  GL_COLOR_ATTACHMENT27    = 0x8CFB,
    GL_CON_22_ATI                 = 0x8957,  GL_COLOR_ATTACHMENT28    = 0x8CFC,
    GL_CON_23_ATI                 = 0x8958,  GL_COLOR_ATTACHMENT29    = 0x8CFD,
    GL_CON_24_ATI                 = 0x8959,  GL_COLOR_ATTACHMENT30    = 0x8CFE,
    GL_CON_25_ATI                 = 0x895A,  GL_COLOR_ATTACHMENT31    = 0x8CFF,
    GL_CON_26_ATI                 = 0x895B,
    GL_CON_27_ATI                 = 0x895C,
    GL_CON_28_ATI                 = 0x895D,
    GL_CON_29_ATI                 = 0x895E,
    GL_CON_30_ATI                 = 0x895F,
    GL_CON_31_ATI                 = 0x8960,
    GL_SECONDARY_INTERPOLATOR_ATI = 0x896D,
    /* PathCoordType */                               /* PixelType */                                  /* EnableCap */
    GL_CLOSE_PATH_NV                         = 0x00,  GL_HALF_FLOAT                         = 0x140B,  GL_CLIP_DISTANCE0                          = 0x3000,
    GL_MOVE_TO_NV                            = 0x02,  GL_HALF_FLOAT_ARB                     = 0x140B,  GL_CLIP_DISTANCE1                          = 0x3001,
    GL_RELATIVE_MOVE_TO_NV                   = 0x03,  GL_HALF_FLOAT_NV                      = 0x140B,  GL_CLIP_DISTANCE2                          = 0x3002,
    GL_LINE_TO_NV                            = 0x04,  GL_HALF_APPLE                         = 0x140B,  GL_CLIP_DISTANCE3                          = 0x3003,
    GL_RELATIVE_LINE_TO_NV                   = 0x05,  GL_BITMAP                             = 0x1A00,  GL_CLIP_DISTANCE4                          = 0x3004,
    GL_HORIZONTAL_LINE_TO_NV                 = 0x06,  GL_UNSIGNED_BYTE_3_3_2                = 0x8032,  GL_CLIP_DISTANCE5                          = 0x3005,
    GL_RELATIVE_HORIZONTAL_LINE_TO_NV        = 0x07,  GL_UNSIGNED_BYTE_3_3_2_EXT            = 0x8032,  GL_CLIP_DISTANCE6                          = 0x3006,
    GL_VERTICAL_LINE_TO_NV                   = 0x08,  GL_UNSIGNED_SHORT_4_4_4_4             = 0x8033,  GL_CLIP_DISTANCE7                          = 0x3007,
    GL_RELATIVE_VERTICAL_LINE_TO_NV          = 0x09,  GL_UNSIGNED_SHORT_4_4_4_4_EXT         = 0x8033,  GL_MULTISAMPLE                             = 0x809D,
    GL_QUADRATIC_CURVE_TO_NV                 = 0x0A,  GL_UNSIGNED_SHORT_5_5_5_1             = 0x8034,  GL_SAMPLE_ALPHA_TO_COVERAGE                = 0x809E,
    GL_RELATIVE_QUADRATIC_CURVE_TO_NV        = 0x0B,  GL_UNSIGNED_SHORT_5_5_5_1_EXT         = 0x8034,  GL_SAMPLE_ALPHA_TO_ONE                     = 0x809F,
    GL_CUBIC_CURVE_TO_NV                     = 0x0C,  GL_UNSIGNED_INT_8_8_8_8               = 0x8035,  GL_SAMPLE_COVERAGE                         = 0x80A0,
    GL_RELATIVE_CUBIC_CURVE_TO_NV            = 0x0D,  GL_UNSIGNED_INT_8_8_8_8_EXT           = 0x8035,  GL_COLOR_TABLE                             = 0x80D0,
    GL_SMOOTH_QUADRATIC_CURVE_TO_NV          = 0x0E,  GL_UNSIGNED_INT_10_10_10_2            = 0x8036,  GL_POST_CONVOLUTION_COLOR_TABLE            = 0x80D1,
    GL_RELATIVE_SMOOTH_QUADRATIC_CURVE_TO_NV = 0x0F,  GL_UNSIGNED_INT_10_10_10_2_EXT        = 0x8036,  GL_POST_COLOR_MATRIX_COLOR_TABLE           = 0x80D2,
    GL_SMOOTH_CUBIC_CURVE_TO_NV              = 0x10,  GL_UNSIGNED_BYTE_2_3_3_REV            = 0x8362,  GL_DEBUG_OUTPUT_SYNCHRONOUS                = 0x8242,
    GL_RELATIVE_SMOOTH_CUBIC_CURVE_TO_NV     = 0x11,  GL_UNSIGNED_BYTE_2_3_3_REV_EXT        = 0x8362,  GL_FRAGMENT_LIGHT1_SGIX                    = 0x840D,
    GL_SMALL_CCW_ARC_TO_NV                   = 0x12,  GL_UNSIGNED_SHORT_5_6_5               = 0x8363,  GL_FRAGMENT_LIGHT2_SGIX                    = 0x840E,
    GL_RELATIVE_SMALL_CCW_ARC_TO_NV          = 0x13,  GL_UNSIGNED_SHORT_5_6_5_EXT           = 0x8363,  GL_FRAGMENT_LIGHT3_SGIX                    = 0x840F,
    GL_SMALL_CW_ARC_TO_NV                    = 0x14,  GL_UNSIGNED_SHORT_5_6_5_REV           = 0x8364,  GL_FRAGMENT_LIGHT4_SGIX                    = 0x8410,
    GL_RELATIVE_SMALL_CW_ARC_TO_NV           = 0x15,  GL_UNSIGNED_SHORT_5_6_5_REV_EXT       = 0x8364,  GL_FRAGMENT_LIGHT5_SGIX                    = 0x8411,
    GL_LARGE_CCW_ARC_TO_NV                   = 0x16,  GL_UNSIGNED_SHORT_4_4_4_4_REV         = 0x8365,  GL_FRAGMENT_LIGHT6_SGIX                    = 0x8412,
    GL_RELATIVE_LARGE_CCW_ARC_TO_NV          = 0x17,  GL_UNSIGNED_SHORT_4_4_4_4_REV_EXT     = 0x8365,  GL_FRAGMENT_LIGHT7_SGIX                    = 0x8413,
    GL_LARGE_CW_ARC_TO_NV                    = 0x18,  GL_UNSIGNED_SHORT_4_4_4_4_REV_IMG     = 0x8365,  GL_TEXTURE_RECTANGLE                       = 0x84F5,
    GL_RELATIVE_LARGE_CW_ARC_TO_NV           = 0x19,  GL_UNSIGNED_SHORT_1_5_5_5_REV         = 0x8366,  GL_TEXTURE_RECTANGLE_ARB                   = 0x84F5,
    GL_CONIC_CURVE_TO_NV                     = 0x1A,  GL_UNSIGNED_SHORT_1_5_5_5_REV_EXT     = 0x8366,  GL_TEXTURE_RECTANGLE_NV                    = 0x84F5,
    GL_RELATIVE_CONIC_CURVE_TO_NV            = 0x1B,  GL_UNSIGNED_INT_8_8_8_8_REV           = 0x8367,  GL_TEXTURE_CUBE_MAP                        = 0x8513,
    GL_ROUNDED_RECT_NV                       = 0xE8,  GL_UNSIGNED_INT_8_8_8_8_REV_EXT       = 0x8367,  GL_TEXTURE_CUBE_MAP_ARB                    = 0x8513,
    GL_RELATIVE_ROUNDED_RECT_NV              = 0xE9,  GL_UNSIGNED_INT_2_10_10_10_REV        = 0x8368,  GL_TEXTURE_CUBE_MAP_EXT                    = 0x8513,
    GL_ROUNDED_RECT2_NV                      = 0xEA,  GL_UNSIGNED_INT_2_10_10_10_REV_EXT    = 0x8368,  GL_TEXTURE_CUBE_MAP_OES                    = 0x8513,
    GL_RELATIVE_ROUNDED_RECT2_NV             = 0xEB,  GL_UNSIGNED_INT_24_8                  = 0x84FA,  GL_DEPTH_CLAMP                             = 0x864F,
    GL_ROUNDED_RECT4_NV                      = 0xEC,  GL_UNSIGNED_INT_24_8_EXT              = 0x84FA,  GL_TEXTURE_CUBE_MAP_SEAMLESS               = 0x884F,
    GL_RELATIVE_ROUNDED_RECT4_NV             = 0xED,  GL_UNSIGNED_INT_24_8_NV               = 0x84FA,  GL_SAMPLE_SHADING                          = 0x8C36,
    GL_ROUNDED_RECT8_NV                      = 0xEE,  GL_UNSIGNED_INT_24_8_OES              = 0x84FA,  GL_RASTERIZER_DISCARD                      = 0x8C89,
    GL_RELATIVE_ROUNDED_RECT8_NV             = 0xEF,  GL_UNSIGNED_INT_10F_11F_11F_REV       = 0x8C3B,  GL_PRIMITIVE_RESTART_FIXED_INDEX           = 0x8D69,
    GL_RESTART_PATH_NV                       = 0xF0,  GL_UNSIGNED_INT_10F_11F_11F_REV_APPLE = 0x8C3B,  GL_FRAMEBUFFER_SRGB                        = 0x8DB9,
    GL_DUP_FIRST_CUBIC_CURVE_TO_NV           = 0xF2,  GL_UNSIGNED_INT_10F_11F_11F_REV_EXT   = 0x8C3B,  GL_SAMPLE_MASK                             = 0x8E51,
    GL_DUP_LAST_CUBIC_CURVE_TO_NV            = 0xF4,  GL_UNSIGNED_INT_5_9_9_9_REV           = 0x8C3E,  GL_PRIMITIVE_RESTART                       = 0x8F9D,
    GL_RECT_NV                               = 0xF6,  GL_UNSIGNED_INT_5_9_9_9_REV_APPLE     = 0x8C3E,  GL_DEBUG_OUTPUT                            = 0x92E0,
    GL_RELATIVE_RECT_NV                      = 0xF7,  GL_UNSIGNED_INT_5_9_9_9_REV_EXT       = 0x8C3E,  GL_SHADING_RATE_PRESERVE_ASPECT_RATIO_QCOM = 0x96A5,
    GL_CIRCULAR_CCW_ARC_TO_NV                = 0xF8,  GL_FLOAT_32_UNSIGNED_INT_24_8_REV     = 0x8DAD,
    GL_CIRCULAR_CW_ARC_TO_NV                 = 0xFA,  GL_FLOAT_32_UNSIGNED_INT_24_8_REV_NV  = 0x8DAD,
    GL_CIRCULAR_TANGENT_ARC_TO_NV            = 0xFC,
    GL_ARC_TO_NV                             = 0xFE,
    GL_RELATIVE_ARC_TO_NV                    = 0xFF,
    /* HintTarget */                                   /* RenderbufferParameterName */                /* FramebufferAttachmentParameterName */
    GL_PHONG_HINT_WIN                      = 0x80EB,   GL_RENDERBUFFER_COVERAGE_SAMPLES_NV = 0x8CAB,  GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING              = 0x8210,
    GL_CLIP_VOLUME_CLIPPING_HINT_EXT       = 0x80F0,   GL_RENDERBUFFER_SAMPLES             = 0x8CAB,  GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING_EXT          = 0x8210,
    GL_TEXTURE_MULTI_BUFFER_HINT_SGIX      = 0x812E,   GL_RENDERBUFFER_SAMPLES_ANGLE       = 0x8CAB,  GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE              = 0x8211,
    GL_GENERATE_MIPMAP_HINT                = 0x8192,   GL_RENDERBUFFER_SAMPLES_APPLE       = 0x8CAB,  GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE_EXT          = 0x8211,
    GL_PROGRAM_BINARY_RETRIEVABLE_HINT     = 0x8257,   GL_RENDERBUFFER_SAMPLES_EXT         = 0x8CAB,  GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE                    = 0x8212,
    GL_SCALEBIAS_HINT_SGIX                 = 0x8322,   GL_RENDERBUFFER_SAMPLES_NV          = 0x8CAB,  GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE                  = 0x8213,
    GL_LINE_QUALITY_HINT_SGIX              = 0x835B,   GL_RENDERBUFFER_WIDTH               = 0x8D42,  GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE                   = 0x8214,
    GL_TEXTURE_COMPRESSION_HINT_ARB        = 0x84EF,   GL_RENDERBUFFER_WIDTH_EXT           = 0x8D42,  GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE                  = 0x8215,
    GL_VERTEX_ARRAY_STORAGE_HINT_APPLE     = 0x851F,   GL_RENDERBUFFER_WIDTH_OES           = 0x8D42,  GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE                  = 0x8216,
    GL_MULTISAMPLE_FILTER_HINT_NV          = 0x8534,   GL_RENDERBUFFER_HEIGHT              = 0x8D43,  GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE                = 0x8217,
    GL_TRANSFORM_HINT_APPLE                = 0x85B1,   GL_RENDERBUFFER_HEIGHT_EXT          = 0x8D43,  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE                 = 0x8CD0,
    GL_TEXTURE_STORAGE_HINT_APPLE          = 0x85BC,   GL_RENDERBUFFER_HEIGHT_OES          = 0x8D43,  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_EXT             = 0x8CD0,
    GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB = 0x8B8B,   GL_RENDERBUFFER_INTERNAL_FORMAT     = 0x8D44,  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE_OES             = 0x8CD0,
    GL_FRAGMENT_SHADER_DERIVATIVE_HINT_OES = 0x8B8B,   GL_RENDERBUFFER_INTERNAL_FORMAT_EXT = 0x8D44,  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME                 = 0x8CD1,
    GL_BINNING_CONTROL_HINT_QCOM           = 0x8FB0,   GL_RENDERBUFFER_INTERNAL_FORMAT_OES = 0x8D44,  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_EXT             = 0x8CD1,
    GL_PREFER_DOUBLEBUFFER_HINT_PGI        = 0x1A1F8,  GL_RENDERBUFFER_RED_SIZE            = 0x8D50,  GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME_OES             = 0x8CD1,
    GL_CONSERVE_MEMORY_HINT_PGI            = 0x1A1FD,  GL_RENDERBUFFER_RED_SIZE_EXT        = 0x8D50,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL               = 0x8CD2,
    GL_RECLAIM_MEMORY_HINT_PGI             = 0x1A1FE,  GL_RENDERBUFFER_RED_SIZE_OES        = 0x8D50,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_EXT           = 0x8CD2,
    GL_NATIVE_GRAPHICS_BEGIN_HINT_PGI      = 0x1A203,  GL_RENDERBUFFER_GREEN_SIZE          = 0x8D51,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL_OES           = 0x8CD2,
    GL_NATIVE_GRAPHICS_END_HINT_PGI        = 0x1A204,  GL_RENDERBUFFER_GREEN_SIZE_EXT      = 0x8D51,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE       = 0x8CD3,
    GL_ALWAYS_FAST_HINT_PGI                = 0x1A20C,  GL_RENDERBUFFER_GREEN_SIZE_OES      = 0x8D51,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_EXT   = 0x8CD3,
    GL_ALWAYS_SOFT_HINT_PGI                = 0x1A20D,  GL_RENDERBUFFER_BLUE_SIZE           = 0x8D52,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE_OES   = 0x8CD3,
    GL_ALLOW_DRAW_OBJ_HINT_PGI             = 0x1A20E,  GL_RENDERBUFFER_BLUE_SIZE_EXT       = 0x8D52,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_EXT      = 0x8CD4,
    GL_ALLOW_DRAW_WIN_HINT_PGI             = 0x1A20F,  GL_RENDERBUFFER_BLUE_SIZE_OES       = 0x8D52,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_3D_ZOFFSET_OES      = 0x8CD4,
    GL_ALLOW_DRAW_FRG_HINT_PGI             = 0x1A210,  GL_RENDERBUFFER_ALPHA_SIZE          = 0x8D53,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER               = 0x8CD4,
    GL_ALLOW_DRAW_MEM_HINT_PGI             = 0x1A211,  GL_RENDERBUFFER_ALPHA_SIZE_EXT      = 0x8D53,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER_EXT           = 0x8CD4,
    GL_STRICT_DEPTHFUNC_HINT_PGI           = 0x1A216,  GL_RENDERBUFFER_ALPHA_SIZE_OES      = 0x8D53,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_SAMPLES_EXT         = 0x8D6C,
    GL_STRICT_LIGHTING_HINT_PGI            = 0x1A217,  GL_RENDERBUFFER_DEPTH_SIZE          = 0x8D54,  GL_FRAMEBUFFER_ATTACHMENT_LAYERED                     = 0x8DA7,
    GL_STRICT_SCISSOR_HINT_PGI             = 0x1A218,  GL_RENDERBUFFER_DEPTH_SIZE_EXT      = 0x8D54,  GL_FRAMEBUFFER_ATTACHMENT_LAYERED_ARB                 = 0x8DA7,
    GL_FULL_STIPPLE_HINT_PGI               = 0x1A219,  GL_RENDERBUFFER_DEPTH_SIZE_OES      = 0x8D54,  GL_FRAMEBUFFER_ATTACHMENT_LAYERED_EXT                 = 0x8DA7,
    GL_CLIP_NEAR_HINT_PGI                  = 0x1A220,  GL_RENDERBUFFER_STENCIL_SIZE        = 0x8D55,  GL_FRAMEBUFFER_ATTACHMENT_LAYERED_OES                 = 0x8DA7,
    GL_CLIP_FAR_HINT_PGI                   = 0x1A221,  GL_RENDERBUFFER_STENCIL_SIZE_EXT    = 0x8D55,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_SCALE_IMG           = 0x913F,
    GL_WIDE_LINE_HINT_PGI                  = 0x1A222,  GL_RENDERBUFFER_STENCIL_SIZE_OES    = 0x8D55,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_NUM_VIEWS_OVR       = 0x9630,
    GL_BACK_NORMALS_HINT_PGI               = 0x1A223,  GL_RENDERBUFFER_COLOR_SAMPLES_NV    = 0x8E10,  GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_BASE_VIEW_INDEX_OVR = 0x9632,
    GL_VERTEX_DATA_HINT_PGI                = 0x1A22A,  GL_RENDERBUFFER_SAMPLES_IMG         = 0x9133,
    GL_VERTEX_CONSISTENT_HINT_PGI          = 0x1A22B,  GL_RENDERBUFFER_STORAGE_SAMPLES_AMD = 0x91B2,
    GL_MATERIAL_SIDE_HINT_PGI              = 0x1A22C,
    GL_MAX_VERTEX_HINT_PGI                 = 0x1A22D,
    /* BufferBitQCOM */                            /* TextureUnit */       /* MemoryBarrierMask */
    GL_COLOR_BUFFER_BIT0_QCOM       = 0x00000001,  GL_TEXTURE0  = 0x84C0,  GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT      = 0x00000001,
    GL_COLOR_BUFFER_BIT1_QCOM       = 0x00000002,  GL_TEXTURE1  = 0x84C1,  GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT_EXT  = 0x00000001,
    GL_COLOR_BUFFER_BIT2_QCOM       = 0x00000004,  GL_TEXTURE2  = 0x84C2,  GL_ELEMENT_ARRAY_BARRIER_BIT            = 0x00000002,
    GL_COLOR_BUFFER_BIT3_QCOM       = 0x00000008,  GL_TEXTURE3  = 0x84C3,  GL_ELEMENT_ARRAY_BARRIER_BIT_EXT        = 0x00000002,
    GL_COLOR_BUFFER_BIT4_QCOM       = 0x00000010,  GL_TEXTURE4  = 0x84C4,  GL_UNIFORM_BARRIER_BIT                  = 0x00000004,
    GL_COLOR_BUFFER_BIT5_QCOM       = 0x00000020,  GL_TEXTURE5  = 0x84C5,  GL_UNIFORM_BARRIER_BIT_EXT              = 0x00000004,
    GL_COLOR_BUFFER_BIT6_QCOM       = 0x00000040,  GL_TEXTURE6  = 0x84C6,  GL_TEXTURE_FETCH_BARRIER_BIT            = 0x00000008,
    GL_COLOR_BUFFER_BIT7_QCOM       = 0x00000080,  GL_TEXTURE7  = 0x84C7,  GL_TEXTURE_FETCH_BARRIER_BIT_EXT        = 0x00000008,
    GL_DEPTH_BUFFER_BIT0_QCOM       = 0x00000100,  GL_TEXTURE8  = 0x84C8,  GL_SHADER_GLOBAL_ACCESS_BARRIER_BIT_NV  = 0x00000010,
    GL_DEPTH_BUFFER_BIT1_QCOM       = 0x00000200,  GL_TEXTURE9  = 0x84C9,  GL_SHADER_IMAGE_ACCESS_BARRIER_BIT      = 0x00000020,
    GL_DEPTH_BUFFER_BIT2_QCOM       = 0x00000400,  GL_TEXTURE10 = 0x84CA,  GL_SHADER_IMAGE_ACCESS_BARRIER_BIT_EXT  = 0x00000020,
    GL_DEPTH_BUFFER_BIT3_QCOM       = 0x00000800,  GL_TEXTURE11 = 0x84CB,  GL_COMMAND_BARRIER_BIT                  = 0x00000040,
    GL_DEPTH_BUFFER_BIT4_QCOM       = 0x00001000,  GL_TEXTURE12 = 0x84CC,  GL_COMMAND_BARRIER_BIT_EXT              = 0x00000040,
    GL_DEPTH_BUFFER_BIT5_QCOM       = 0x00002000,  GL_TEXTURE13 = 0x84CD,  GL_PIXEL_BUFFER_BARRIER_BIT             = 0x00000080,
    GL_DEPTH_BUFFER_BIT6_QCOM       = 0x00004000,  GL_TEXTURE14 = 0x84CE,  GL_PIXEL_BUFFER_BARRIER_BIT_EXT         = 0x00000080,
    GL_DEPTH_BUFFER_BIT7_QCOM       = 0x00008000,  GL_TEXTURE15 = 0x84CF,  GL_TEXTURE_UPDATE_BARRIER_BIT           = 0x00000100,
    GL_STENCIL_BUFFER_BIT0_QCOM     = 0x00010000,  GL_TEXTURE16 = 0x84D0,  GL_TEXTURE_UPDATE_BARRIER_BIT_EXT       = 0x00000100,
    GL_STENCIL_BUFFER_BIT1_QCOM     = 0x00020000,  GL_TEXTURE17 = 0x84D1,  GL_BUFFER_UPDATE_BARRIER_BIT            = 0x00000200,
    GL_STENCIL_BUFFER_BIT2_QCOM     = 0x00040000,  GL_TEXTURE18 = 0x84D2,  GL_BUFFER_UPDATE_BARRIER_BIT_EXT        = 0x00000200,
    GL_STENCIL_BUFFER_BIT3_QCOM     = 0x00080000,  GL_TEXTURE19 = 0x84D3,  GL_FRAMEBUFFER_BARRIER_BIT              = 0x00000400,
    GL_STENCIL_BUFFER_BIT4_QCOM     = 0x00100000,  GL_TEXTURE20 = 0x84D4,  GL_FRAMEBUFFER_BARRIER_BIT_EXT          = 0x00000400,
    GL_STENCIL_BUFFER_BIT5_QCOM     = 0x00200000,  GL_TEXTURE21 = 0x84D5,  GL_TRANSFORM_FEEDBACK_BARRIER_BIT       = 0x00000800,
    GL_STENCIL_BUFFER_BIT6_QCOM     = 0x00400000,  GL_TEXTURE22 = 0x84D6,  GL_TRANSFORM_FEEDBACK_BARRIER_BIT_EXT   = 0x00000800,
    GL_STENCIL_BUFFER_BIT7_QCOM     = 0x00800000,  GL_TEXTURE23 = 0x84D7,  GL_ATOMIC_COUNTER_BARRIER_BIT           = 0x00001000,
    GL_MULTISAMPLE_BUFFER_BIT0_QCOM = 0x01000000,  GL_TEXTURE24 = 0x84D8,  GL_ATOMIC_COUNTER_BARRIER_BIT_EXT       = 0x00001000,
    GL_MULTISAMPLE_BUFFER_BIT1_QCOM = 0x02000000,  GL_TEXTURE25 = 0x84D9,  GL_SHADER_STORAGE_BARRIER_BIT           = 0x00002000,
    GL_MULTISAMPLE_BUFFER_BIT2_QCOM = 0x04000000,  GL_TEXTURE26 = 0x84DA,  GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT     = 0x00004000,
    GL_MULTISAMPLE_BUFFER_BIT3_QCOM = 0x08000000,  GL_TEXTURE27 = 0x84DB,  GL_CLIENT_MAPPED_BUFFER_BARRIER_BIT_EXT = 0x00004000,
    GL_MULTISAMPLE_BUFFER_BIT4_QCOM = 0x10000000,  GL_TEXTURE28 = 0x84DC,  GL_QUERY_BUFFER_BARRIER_BIT             = 0x00008000,
    GL_MULTISAMPLE_BUFFER_BIT5_QCOM = 0x20000000,  GL_TEXTURE29 = 0x84DD,  
    GL_MULTISAMPLE_BUFFER_BIT6_QCOM = 0x40000000,  GL_TEXTURE30 = 0x84DE,  
                                                   GL_TEXTURE31 = 0x84DF,
    /* ProgramResourceProperty */                      /* AttribMask */                       /* PrimitiveType */
    GL_IS_PER_PATCH                         = 0x92E7,  GL_CURRENT_BIT          = 0x00000001,  GL_POINTS                       = 0x0000,
    GL_NAME_LENGTH                          = 0x92F9,  GL_POINT_BIT            = 0x00000002,  GL_LINES                        = 0x0001,
    GL_TYPE                                 = 0x92FA,  GL_LINE_BIT             = 0x00000004,  GL_LINE_LOOP                    = 0x0002,
    GL_ARRAY_SIZE                           = 0x92FB,  GL_POLYGON_BIT          = 0x00000008,  GL_LINE_STRIP                   = 0x0003,
    GL_OFFSET                               = 0x92FC,  GL_POLYGON_STIPPLE_BIT  = 0x00000010,  GL_TRIANGLES                    = 0x0004,
    GL_BLOCK_INDEX                          = 0x92FD,  GL_PIXEL_MODE_BIT       = 0x00000020,  GL_TRIANGLE_STRIP               = 0x0005,
    GL_ARRAY_STRIDE                         = 0x92FE,  GL_LIGHTING_BIT         = 0x00000040,  GL_TRIANGLE_FAN                 = 0x0006,
    GL_MATRIX_STRIDE                        = 0x92FF,  GL_FOG_BIT              = 0x00000080,  GL_QUADS                        = 0x0007,
    GL_IS_ROW_MAJOR                         = 0x9300,  GL_DEPTH_BUFFER_BIT     = 0x00000100,  GL_QUADS_EXT                    = 0x0007,
    GL_ATOMIC_COUNTER_BUFFER_INDEX          = 0x9301,  GL_ACCUM_BUFFER_BIT     = 0x00000200,  GL_QUAD_STRIP                   = 0x0008,
    GL_BUFFER_BINDING                       = 0x9302,  GL_STENCIL_BUFFER_BIT   = 0x00000400,  GL_POLYGON                      = 0x0009,
    GL_BUFFER_DATA_SIZE                     = 0x9303,  GL_VIEWPORT_BIT         = 0x00000800,  GL_LINES_ADJACENCY              = 0x000A,
    GL_NUM_ACTIVE_VARIABLES                 = 0x9304,  GL_TRANSFORM_BIT        = 0x00001000,  GL_LINES_ADJACENCY_ARB          = 0x000A,
    GL_ACTIVE_VARIABLES                     = 0x9305,  GL_ENABLE_BIT           = 0x00002000,  GL_LINES_ADJACENCY_EXT          = 0x000A,
    GL_REFERENCED_BY_VERTEX_SHADER          = 0x9306,  GL_COLOR_BUFFER_BIT     = 0x00004000,  GL_LINE_STRIP_ADJACENCY         = 0x000B,
    GL_REFERENCED_BY_TESS_CONTROL_SHADER    = 0x9307,  GL_HINT_BIT             = 0x00008000,  GL_LINE_STRIP_ADJACENCY_ARB     = 0x000B,
    GL_REFERENCED_BY_TESS_EVALUATION_SHADER = 0x9308,  GL_EVAL_BIT             = 0x00010000,  GL_LINE_STRIP_ADJACENCY_EXT     = 0x000B,
    GL_REFERENCED_BY_GEOMETRY_SHADER        = 0x9309,  GL_LIST_BIT             = 0x00020000,  GL_TRIANGLES_ADJACENCY          = 0x000C,
    GL_REFERENCED_BY_FRAGMENT_SHADER        = 0x930A,  GL_TEXTURE_BIT          = 0x00040000,  GL_TRIANGLES_ADJACENCY_ARB      = 0x000C,
    GL_REFERENCED_BY_COMPUTE_SHADER         = 0x930B,  GL_SCISSOR_BIT          = 0x00080000,  GL_TRIANGLES_ADJACENCY_EXT      = 0x000C,
    GL_TOP_LEVEL_ARRAY_SIZE                 = 0x930C,  GL_MULTISAMPLE_BIT      = 0x20000000,  GL_TRIANGLE_STRIP_ADJACENCY     = 0x000D,
    GL_TOP_LEVEL_ARRAY_STRIDE               = 0x930D,  GL_MULTISAMPLE_BIT_ARB  = 0x20000000,  GL_TRIANGLE_STRIP_ADJACENCY_ARB = 0x000D,
    GL_LOCATION                             = 0x930E,  GL_MULTISAMPLE_BIT_EXT  = 0x20000000,  GL_TRIANGLE_STRIP_ADJACENCY_EXT = 0x000D,
    GL_LOCATION_INDEX                       = 0x930F,  GL_MULTISAMPLE_BIT_3DFX = 0x20000000,  GL_PATCHES                      = 0x000E,
    GL_LOCATION_COMPONENT                   = 0x934A,                                         GL_PATCHES_EXT                  = 0x000E,
    GL_TRANSFORM_FEEDBACK_BUFFER_INDEX      = 0x934B,
    GL_TRANSFORM_FEEDBACK_BUFFER_STRIDE     = 0x934C,
    /* PixelFormat */             /* VertexShaderOpEXT */              /* PathMetricMask */
    GL_COLOR_INDEX     = 0x1900,  GL_OP_INDEX_EXT           = 0x8782,  GL_GLYPH_WIDTH_BIT_NV                      = 0x01,
    GL_STENCIL_INDEX   = 0x1901,  GL_OP_NEGATE_EXT          = 0x8783,  GL_GLYPH_HEIGHT_BIT_NV                     = 0x02,
    GL_DEPTH_COMPONENT = 0x1902,  GL_OP_DOT3_EXT            = 0x8784,  GL_GLYPH_HORIZONTAL_BEARING_X_BIT_NV       = 0x04,
    GL_RED_EXT         = 0x1903,  GL_OP_DOT4_EXT            = 0x8785,  GL_GLYPH_HORIZONTAL_BEARING_Y_BIT_NV       = 0x08,
    GL_ABGR_EXT        = 0x8000,  GL_OP_MUL_EXT             = 0x8786,  GL_GLYPH_HORIZONTAL_BEARING_ADVANCE_BIT_NV = 0x10,
    GL_CMYK_EXT        = 0x800C,  GL_OP_ADD_EXT             = 0x8787,  GL_GLYPH_VERTICAL_BEARING_X_BIT_NV         = 0x20,
    GL_CMYKA_EXT       = 0x800D,  GL_OP_MADD_EXT            = 0x8788,  GL_GLYPH_VERTICAL_BEARING_Y_BIT_NV         = 0x40,
    GL_BGR             = 0x80E0,  GL_OP_FRAC_EXT            = 0x8789,  GL_GLYPH_VERTICAL_BEARING_ADVANCE_BIT_NV   = 0x80,
    GL_BGR_EXT         = 0x80E0,  GL_OP_MAX_EXT             = 0x878A,  GL_GLYPH_HAS_KERNING_BIT_NV                = 0x100,
    GL_BGRA            = 0x80E1,  GL_OP_MIN_EXT             = 0x878B,  GL_FONT_X_MIN_BOUNDS_BIT_NV                = 0x00010000,
    GL_BGRA_EXT        = 0x80E1,  GL_OP_SET_GE_EXT          = 0x878C,  GL_FONT_Y_MIN_BOUNDS_BIT_NV                = 0x00020000,
    GL_BGRA_IMG        = 0x80E1,  GL_OP_SET_LT_EXT          = 0x878D,  GL_FONT_X_MAX_BOUNDS_BIT_NV                = 0x00040000,
    GL_YCRCB_422_SGIX  = 0x81BB,  GL_OP_CLAMP_EXT           = 0x878E,  GL_FONT_Y_MAX_BOUNDS_BIT_NV                = 0x00080000,
    GL_YCRCB_444_SGIX  = 0x81BC,  GL_OP_FLOOR_EXT           = 0x878F,  GL_FONT_UNITS_PER_EM_BIT_NV                = 0x00100000,
    GL_RG              = 0x8227,  GL_OP_ROUND_EXT           = 0x8790,  GL_FONT_ASCENDER_BIT_NV                    = 0x00200000,
    GL_RG_INTEGER      = 0x8228,  GL_OP_EXP_BASE_2_EXT      = 0x8791,  GL_FONT_DESCENDER_BIT_NV                   = 0x00400000,
    GL_DEPTH_STENCIL   = 0x84F9,  GL_OP_LOG_BASE_2_EXT      = 0x8792,  GL_FONT_HEIGHT_BIT_NV                      = 0x00800000,
    GL_RED_INTEGER     = 0x8D94,  GL_OP_POWER_EXT           = 0x8793,  GL_FONT_MAX_ADVANCE_WIDTH_BIT_NV           = 0x01000000,
    GL_GREEN_INTEGER   = 0x8D95,  GL_OP_RECIP_EXT           = 0x8794,  GL_FONT_MAX_ADVANCE_HEIGHT_BIT_NV          = 0x02000000,
    GL_BLUE_INTEGER    = 0x8D96,  GL_OP_RECIP_SQRT_EXT      = 0x8795,  GL_FONT_UNDERLINE_POSITION_BIT_NV          = 0x04000000,
    GL_RGB_INTEGER     = 0x8D98,  GL_OP_SUB_EXT             = 0x8796,  GL_FONT_UNDERLINE_THICKNESS_BIT_NV         = 0x08000000,
    GL_RGBA_INTEGER    = 0x8D99,  GL_OP_CROSS_PRODUCT_EXT   = 0x8797,  GL_FONT_HAS_KERNING_BIT_NV                 = 0x10000000,
    GL_BGR_INTEGER     = 0x8D9A,  GL_OP_MULTIPLY_MATRIX_EXT = 0x8798,  GL_FONT_NUM_GLYPH_INDICES_BIT_NV           = 0x20000000,
    GL_BGRA_INTEGER    = 0x8D9B,  GL_OP_MOV_EXT             = 0x8799,
    /* InvalidateFramebufferAttachment */  /* PathParameter */                       /* UseProgramStageMask */
    GL_DEPTH_STENCIL_ATTACHMENT = 0x821A,  GL_PATH_STROKE_WIDTH_NV        = 0x9075,  GL_VERTEX_SHADER_BIT              = 0x00000001,
    GL_COLOR_ATTACHMENT0_EXT    = 0x8CE0,  GL_PATH_END_CAPS_NV            = 0x9076,  GL_VERTEX_SHADER_BIT_EXT          = 0x00000001,
    GL_COLOR_ATTACHMENT0_OES    = 0x8CE0,  GL_PATH_INITIAL_END_CAP_NV     = 0x9077,  GL_FRAGMENT_SHADER_BIT            = 0x00000002,
    GL_COLOR_ATTACHMENT1_EXT    = 0x8CE1,  GL_PATH_TERMINAL_END_CAP_NV    = 0x9078,  GL_FRAGMENT_SHADER_BIT_EXT        = 0x00000002,
    GL_COLOR_ATTACHMENT2_EXT    = 0x8CE2,  GL_PATH_JOIN_STYLE_NV          = 0x9079,  GL_GEOMETRY_SHADER_BIT            = 0x00000004,
    GL_COLOR_ATTACHMENT3_EXT    = 0x8CE3,  GL_PATH_MITER_LIMIT_NV         = 0x907A,  GL_GEOMETRY_SHADER_BIT_EXT        = 0x00000004,
    GL_COLOR_ATTACHMENT4_EXT    = 0x8CE4,  GL_PATH_DASH_CAPS_NV           = 0x907B,  GL_GEOMETRY_SHADER_BIT_OES        = 0x00000004,
    GL_COLOR_ATTACHMENT5_EXT    = 0x8CE5,  GL_PATH_INITIAL_DASH_CAP_NV    = 0x907C,  GL_TESS_CONTROL_SHADER_BIT        = 0x00000008,
    GL_COLOR_ATTACHMENT6_EXT    = 0x8CE6,  GL_PATH_TERMINAL_DASH_CAP_NV   = 0x907D,  GL_TESS_CONTROL_SHADER_BIT_EXT    = 0x00000008,
    GL_COLOR_ATTACHMENT7_EXT    = 0x8CE7,  GL_PATH_DASH_OFFSET_NV         = 0x907E,  GL_TESS_CONTROL_SHADER_BIT_OES    = 0x00000008,
    GL_COLOR_ATTACHMENT8_EXT    = 0x8CE8,  GL_PATH_CLIENT_LENGTH_NV       = 0x907F,  GL_TESS_EVALUATION_SHADER_BIT     = 0x00000010,
    GL_COLOR_ATTACHMENT9_EXT    = 0x8CE9,  GL_PATH_FILL_MASK_NV           = 0x9081,  GL_TESS_EVALUATION_SHADER_BIT_EXT = 0x00000010,
    GL_COLOR_ATTACHMENT10_EXT   = 0x8CEA,  GL_PATH_FILL_COVER_MODE_NV     = 0x9082,  GL_TESS_EVALUATION_SHADER_BIT_OES = 0x00000010,
    GL_COLOR_ATTACHMENT11_EXT   = 0x8CEB,  GL_PATH_STROKE_COVER_MODE_NV   = 0x9083,  GL_COMPUTE_SHADER_BIT             = 0x00000020,
    GL_COLOR_ATTACHMENT12_EXT   = 0x8CEC,  GL_PATH_STROKE_MASK_NV         = 0x9084,  GL_MESH_SHADER_BIT_NV             = 0x00000040,
    GL_COLOR_ATTACHMENT13_EXT   = 0x8CED,  GL_PATH_COMMAND_COUNT_NV       = 0x909D,  GL_MESH_SHADER_BIT_EXT            = 0x00000040,
    GL_COLOR_ATTACHMENT14_EXT   = 0x8CEE,  GL_PATH_COORD_COUNT_NV         = 0x909E,  GL_TASK_SHADER_BIT_NV             = 0x00000080,
    GL_COLOR_ATTACHMENT15_EXT   = 0x8CEF,  GL_PATH_DASH_ARRAY_COUNT_NV    = 0x909F,  GL_TASK_SHADER_BIT_EXT            = 0x00000080,
    GL_DEPTH_ATTACHMENT         = 0x8D00,  GL_PATH_COMPUTED_LENGTH_NV     = 0x90A0,  
    GL_DEPTH_ATTACHMENT_EXT     = 0x8D00,  GL_PATH_FILL_BOUNDING_BOX_NV   = 0x90A1,  
    GL_DEPTH_ATTACHMENT_OES     = 0x8D00,  GL_PATH_STROKE_BOUNDING_BOX_NV = 0x90A2,
    GL_STENCIL_ATTACHMENT_EXT   = 0x8D20,  GL_PATH_DASH_OFFSET_RESET_NV   = 0x90B4,
    GL_STENCIL_ATTACHMENT_OES   = 0x8D20,
    /* ColorTableParameterPName */               /* CommandOpcodesNV */                           /* ProgramPropertyARB */
    GL_COLOR_TABLE_SCALE              = 0x80D6,  GL_TERMINATE_SEQUENCE_COMMAND_NV      = 0x0000,  GL_COMPUTE_WORK_GROUP_SIZE               = 0x8267,
    GL_COLOR_TABLE_SCALE_SGI          = 0x80D6,  GL_NOP_COMMAND_NV                     = 0x0001,  GL_PROGRAM_BINARY_LENGTH                 = 0x8741,
    GL_COLOR_TABLE_BIAS               = 0x80D7,  GL_DRAW_ELEMENTS_COMMAND_NV           = 0x0002,  GL_GEOMETRY_VERTICES_OUT                 = 0x8916,
    GL_COLOR_TABLE_BIAS_SGI           = 0x80D7,  GL_DRAW_ARRAYS_COMMAND_NV             = 0x0003,  GL_GEOMETRY_INPUT_TYPE                   = 0x8917,
    GL_COLOR_TABLE_FORMAT             = 0x80D8,  GL_DRAW_ELEMENTS_STRIP_COMMAND_NV     = 0x0004,  GL_GEOMETRY_OUTPUT_TYPE                  = 0x8918,
    GL_COLOR_TABLE_FORMAT_SGI         = 0x80D8,  GL_DRAW_ARRAYS_STRIP_COMMAND_NV       = 0x0005,  GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH  = 0x8A35,
    GL_COLOR_TABLE_WIDTH              = 0x80D9,  GL_DRAW_ELEMENTS_INSTANCED_COMMAND_NV = 0x0006,  GL_ACTIVE_UNIFORM_BLOCKS                 = 0x8A36,
    GL_COLOR_TABLE_WIDTH_SGI          = 0x80D9,  GL_DRAW_ARRAYS_INSTANCED_COMMAND_NV   = 0x0007,  GL_DELETE_STATUS                         = 0x8B80,
    GL_COLOR_TABLE_RED_SIZE           = 0x80DA,  GL_ELEMENT_ADDRESS_COMMAND_NV         = 0x0008,  GL_LINK_STATUS                           = 0x8B82,
    GL_COLOR_TABLE_RED_SIZE_SGI       = 0x80DA,  GL_ATTRIBUTE_ADDRESS_COMMAND_NV       = 0x0009,  GL_VALIDATE_STATUS                       = 0x8B83,
    GL_COLOR_TABLE_GREEN_SIZE         = 0x80DB,  GL_UNIFORM_ADDRESS_COMMAND_NV         = 0x000A,  GL_ATTACHED_SHADERS                      = 0x8B85,
    GL_COLOR_TABLE_GREEN_SIZE_SGI     = 0x80DB,  GL_BLEND_COLOR_COMMAND_NV             = 0x000B,  GL_ACTIVE_UNIFORMS                       = 0x8B86,
    GL_COLOR_TABLE_BLUE_SIZE          = 0x80DC,  GL_STENCIL_REF_COMMAND_NV             = 0x000C,  GL_ACTIVE_UNIFORM_MAX_LENGTH             = 0x8B87,
    GL_COLOR_TABLE_BLUE_SIZE_SGI      = 0x80DC,  GL_LINE_WIDTH_COMMAND_NV              = 0x000D,  GL_ACTIVE_ATTRIBUTES                     = 0x8B89,
    GL_COLOR_TABLE_ALPHA_SIZE         = 0x80DD,  GL_POLYGON_OFFSET_COMMAND_NV          = 0x000E,  GL_ACTIVE_ATTRIBUTE_MAX_LENGTH           = 0x8B8A,
    GL_COLOR_TABLE_ALPHA_SIZE_SGI     = 0x80DD,  GL_ALPHA_REF_COMMAND_NV               = 0x000F,  GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH = 0x8C76,
    GL_COLOR_TABLE_LUMINANCE_SIZE     = 0x80DE,  GL_VIEWPORT_COMMAND_NV                = 0x0010,  GL_TRANSFORM_FEEDBACK_BUFFER_MODE        = 0x8C7F,
    GL_COLOR_TABLE_LUMINANCE_SIZE_SGI = 0x80DE,  GL_SCISSOR_COMMAND_NV                 = 0x0011,  GL_TRANSFORM_FEEDBACK_VARYINGS           = 0x8C83,
    GL_COLOR_TABLE_INTENSITY_SIZE     = 0x80DF,  GL_FRONT_FACE_COMMAND_NV              = 0x0012,  GL_ACTIVE_ATOMIC_COUNTER_BUFFERS         = 0x92D9,
    GL_COLOR_TABLE_INTENSITY_SIZE_SGI = 0x80DF,
    /* ProgramInterface */                           /* VertexHintsMaskPGI */                          /* BlendingFactor */
    GL_UNIFORM                            = 0x92E1,  GL_VERTEX23_BIT_PGI                = 0x00000004,  GL_SRC_COLOR                = 0x0300,
    GL_UNIFORM_BLOCK                      = 0x92E2,  GL_VERTEX4_BIT_PGI                 = 0x00000008,  GL_ONE_MINUS_SRC_COLOR      = 0x0301,
    GL_PROGRAM_INPUT                      = 0x92E3,  GL_COLOR3_BIT_PGI                  = 0x00010000,  GL_SRC_ALPHA                = 0x0302,
    GL_PROGRAM_OUTPUT                     = 0x92E4,  GL_COLOR4_BIT_PGI                  = 0x00020000,  GL_ONE_MINUS_SRC_ALPHA      = 0x0303,
    GL_BUFFER_VARIABLE                    = 0x92E5,  GL_EDGEFLAG_BIT_PGI                = 0x00040000,  GL_DST_ALPHA                = 0x0304,
    GL_SHADER_STORAGE_BLOCK               = 0x92E6,  GL_INDEX_BIT_PGI                   = 0x00080000,  GL_ONE_MINUS_DST_ALPHA      = 0x0305,
    GL_VERTEX_SUBROUTINE                  = 0x92E8,  GL_MAT_AMBIENT_BIT_PGI             = 0x00100000,  GL_DST_COLOR                = 0x0306,
    GL_TESS_CONTROL_SUBROUTINE            = 0x92E9,  GL_MAT_AMBIENT_AND_DIFFUSE_BIT_PGI = 0x00200000,  GL_ONE_MINUS_DST_COLOR      = 0x0307,
    GL_TESS_EVALUATION_SUBROUTINE         = 0x92EA,  GL_MAT_DIFFUSE_BIT_PGI             = 0x00400000,  GL_SRC_ALPHA_SATURATE       = 0x0308,
    GL_GEOMETRY_SUBROUTINE                = 0x92EB,  GL_MAT_EMISSION_BIT_PGI            = 0x00800000,  GL_CONSTANT_COLOR           = 0x8001,
    GL_FRAGMENT_SUBROUTINE                = 0x92EC,  GL_MAT_COLOR_INDEXES_BIT_PGI       = 0x01000000,  GL_ONE_MINUS_CONSTANT_COLOR = 0x8002,
    GL_COMPUTE_SUBROUTINE                 = 0x92ED,  GL_MAT_SHININESS_BIT_PGI           = 0x02000000,  GL_CONSTANT_ALPHA           = 0x8003,
    GL_VERTEX_SUBROUTINE_UNIFORM          = 0x92EE,  GL_MAT_SPECULAR_BIT_PGI            = 0x04000000,  GL_ONE_MINUS_CONSTANT_ALPHA = 0x8004,
    GL_TESS_CONTROL_SUBROUTINE_UNIFORM    = 0x92EF,  GL_NORMAL_BIT_PGI                  = 0x08000000,  GL_SRC1_ALPHA               = 0x8589,
    GL_TESS_EVALUATION_SUBROUTINE_UNIFORM = 0x92F0,  GL_TEXCOORD1_BIT_PGI               = 0x10000000,  GL_SRC1_COLOR               = 0x88F9,
    GL_GEOMETRY_SUBROUTINE_UNIFORM        = 0x92F1,  GL_TEXCOORD2_BIT_PGI               = 0x20000000,  GL_ONE_MINUS_SRC1_COLOR     = 0x88FA,
    GL_FRAGMENT_SUBROUTINE_UNIFORM        = 0x92F2,  GL_TEXCOORD3_BIT_PGI               = 0x40000000,  GL_ONE_MINUS_SRC1_ALPHA     = 0x88FB,
    GL_COMPUTE_SUBROUTINE_UNIFORM         = 0x92F3,
    GL_TRANSFORM_FEEDBACK_VARYING         = 0x92F4,
    /* GetPointervPName */                        /* ConvolutionParameter */                 /* BufferStorageMask */
    GL_FEEDBACK_BUFFER_POINTER         = 0x0DF0,  GL_CONVOLUTION_BORDER_MODE      = 0x8013,  GL_DYNAMIC_STORAGE_BIT           = 0x0100,
    GL_SELECTION_BUFFER_POINTER        = 0x0DF3,  GL_CONVOLUTION_BORDER_MODE_EXT  = 0x8013,  GL_DYNAMIC_STORAGE_BIT_EXT       = 0x0100,
    GL_VERTEX_ARRAY_POINTER            = 0x808E,  GL_CONVOLUTION_FILTER_SCALE     = 0x8014,  GL_CLIENT_STORAGE_BIT            = 0x0200,
    GL_VERTEX_ARRAY_POINTER_EXT        = 0x808E,  GL_CONVOLUTION_FILTER_SCALE_EXT = 0x8014,  GL_CLIENT_STORAGE_BIT_EXT        = 0x0200,
    GL_NORMAL_ARRAY_POINTER            = 0x808F,  GL_CONVOLUTION_FILTER_BIAS      = 0x8015,  GL_SPARSE_STORAGE_BIT_ARB        = 0x0400,
    GL_NORMAL_ARRAY_POINTER_EXT        = 0x808F,  GL_CONVOLUTION_FILTER_BIAS_EXT  = 0x8015,  GL_LGPU_SEPARATE_STORAGE_BIT_NVX = 0x0800,
    GL_COLOR_ARRAY_POINTER             = 0x8090,  GL_CONVOLUTION_FORMAT           = 0x8017,  GL_PER_GPU_STORAGE_BIT_NV        = 0x0800,
    GL_COLOR_ARRAY_POINTER_EXT         = 0x8090,  GL_CONVOLUTION_FORMAT_EXT       = 0x8017,  GL_EXTERNAL_STORAGE_BIT_NVX      = 0x2000,
    GL_INDEX_ARRAY_POINTER             = 0x8091,  GL_CONVOLUTION_WIDTH            = 0x8018,  GL_MAP_READ_BIT                  = 0x0001,
    GL_INDEX_ARRAY_POINTER_EXT         = 0x8091,  GL_CONVOLUTION_WIDTH_EXT        = 0x8018,  GL_MAP_READ_BIT_EXT              = 0x0001,
    GL_TEXTURE_COORD_ARRAY_POINTER     = 0x8092,  GL_CONVOLUTION_HEIGHT           = 0x8019,  GL_MAP_WRITE_BIT                 = 0x0002,
    GL_TEXTURE_COORD_ARRAY_POINTER_EXT = 0x8092,  GL_CONVOLUTION_HEIGHT_EXT       = 0x8019,  GL_MAP_WRITE_BIT_EXT             = 0x0002,
    GL_EDGE_FLAG_ARRAY_POINTER         = 0x8093,  GL_MAX_CONVOLUTION_WIDTH        = 0x801A,  GL_MAP_PERSISTENT_BIT            = 0x0040,
    GL_EDGE_FLAG_ARRAY_POINTER_EXT     = 0x8093,  GL_MAX_CONVOLUTION_WIDTH_EXT    = 0x801A,  GL_MAP_PERSISTENT_BIT_EXT        = 0x0040,
    GL_INSTRUMENT_BUFFER_POINTER_SGIX  = 0x8180,  GL_MAX_CONVOLUTION_HEIGHT       = 0x801B,  GL_MAP_COHERENT_BIT              = 0x0080,
    GL_DEBUG_CALLBACK_FUNCTION         = 0x8244,  GL_MAX_CONVOLUTION_HEIGHT_EXT   = 0x801B,  GL_MAP_COHERENT_BIT_EXT          = 0x0080,
    GL_DEBUG_CALLBACK_USER_PARAM       = 0x8245,  GL_CONVOLUTION_BORDER_COLOR     = 0x8154,
    /* PixelTransferParameter */                /* GetHistogramParameterPNameEXT */        /* SpecialNumbers */
    GL_POST_CONVOLUTION_RED_SCALE    = 0x801C,  GL_HISTOGRAM_WIDTH              = 0x8026,  GL_FALSE                 = 0,
    GL_POST_CONVOLUTION_GREEN_SCALE  = 0x801D,  GL_HISTOGRAM_WIDTH_EXT          = 0x8026,  GL_NO_ERROR              = 0,
    GL_POST_CONVOLUTION_BLUE_SCALE   = 0x801E,  GL_HISTOGRAM_FORMAT             = 0x8027,  GL_ZERO                  = 0,
    GL_POST_CONVOLUTION_ALPHA_SCALE  = 0x801F,  GL_HISTOGRAM_FORMAT_EXT         = 0x8027,  GL_NONE_OES              = 0,
    GL_POST_CONVOLUTION_RED_BIAS     = 0x8020,  GL_HISTOGRAM_RED_SIZE           = 0x8028,  GL_TRUE                  = 1,
    GL_POST_CONVOLUTION_GREEN_BIAS   = 0x8021,  GL_HISTOGRAM_RED_SIZE_EXT       = 0x8028,  GL_ONE                   = 1,
    GL_POST_CONVOLUTION_BLUE_BIAS    = 0x8022,  GL_HISTOGRAM_GREEN_SIZE         = 0x8029,  
    GL_POST_CONVOLUTION_ALPHA_BIAS   = 0x8023,  GL_HISTOGRAM_GREEN_SIZE_EXT     = 0x8029,  
    GL_POST_COLOR_MATRIX_RED_SCALE   = 0x80B4,  GL_HISTOGRAM_BLUE_SIZE          = 0x802A,  GL_TIMEOUT_IGNORED       = 0xFFFFFFFFFFFFFFFF,
    GL_POST_COLOR_MATRIX_GREEN_SCALE = 0x80B5,  GL_HISTOGRAM_BLUE_SIZE_EXT      = 0x802A,  GL_TIMEOUT_IGNORED_APPLE = 0xFFFFFFFFFFFFFFFF,
    GL_POST_COLOR_MATRIX_BLUE_SCALE  = 0x80B6,  GL_HISTOGRAM_ALPHA_SIZE         = 0x802B,  GL_VERSION_ES_CL_1_0     = 1,
    GL_POST_COLOR_MATRIX_ALPHA_SCALE = 0x80B7,  GL_HISTOGRAM_ALPHA_SIZE_EXT     = 0x802B,  GL_VERSION_ES_CM_1_1     = 1,
    GL_POST_COLOR_MATRIX_RED_BIAS    = 0x80B8,  GL_HISTOGRAM_LUMINANCE_SIZE     = 0x802C,  GL_VERSION_ES_CL_1_1     = 1,
    GL_POST_COLOR_MATRIX_GREEN_BIAS  = 0x80B9,  GL_HISTOGRAM_LUMINANCE_SIZE_EXT = 0x802C,  GL_UUID_SIZE_EXT         = 16,
    GL_POST_COLOR_MATRIX_BLUE_BIAS   = 0x80BA,  GL_HISTOGRAM_SINK               = 0x802D,  GL_LUID_SIZE_EXT         = 8,
    GL_POST_COLOR_MATRIX_ALPHA_BIAS  = 0x80BB,  GL_HISTOGRAM_SINK_EXT           = 0x802D,
    /* LogicOp */               /* TextureWrapMode */              /* InterleavedArrayFormat */  /* SamplePatternSGIS */
    GL_CLEAR         = 0x1500,  GL_CLAMP                = 0x2900,  GL_V2F             = 0x2A20,  GL_1PASS_EXT    = 0x80A1,
    GL_AND           = 0x1501,  GL_REPEAT               = 0x2901,  GL_V3F             = 0x2A21,  GL_1PASS_SGIS   = 0x80A1,
    GL_AND_REVERSE   = 0x1502,  GL_CLAMP_TO_BORDER      = 0x812D,  GL_C4UB_V2F        = 0x2A22,  GL_2PASS_0_EXT  = 0x80A2,
    GL_COPY          = 0x1503,  GL_CLAMP_TO_BORDER_ARB  = 0x812D,  GL_C4UB_V3F        = 0x2A23,  GL_2PASS_0_SGIS = 0x80A2,
    GL_AND_INVERTED  = 0x1504,  GL_CLAMP_TO_BORDER_EXT  = 0x812D,  GL_C3F_V3F         = 0x2A24,  GL_2PASS_1_EXT  = 0x80A3,
    GL_NOOP          = 0x1505,  GL_CLAMP_TO_BORDER_NV   = 0x812D,  GL_N3F_V3F         = 0x2A25,  GL_2PASS_1_SGIS = 0x80A3,
    GL_XOR           = 0x1506,  GL_CLAMP_TO_BORDER_SGIS = 0x812D,  GL_C4F_N3F_V3F     = 0x2A26,  GL_4PASS_0_EXT  = 0x80A4,
    GL_OR            = 0x1507,  GL_CLAMP_TO_BORDER_OES  = 0x812D,  GL_T2F_V3F         = 0x2A27,  GL_4PASS_0_SGIS = 0x80A4,
    GL_NOR           = 0x1508,  GL_CLAMP_TO_EDGE        = 0x812F,  GL_T4F_V4F         = 0x2A28,  GL_4PASS_1_EXT  = 0x80A5,
    GL_EQUIV         = 0x1509,  GL_CLAMP_TO_EDGE_SGIS   = 0x812F,  GL_T2F_C4UB_V3F    = 0x2A29,  GL_4PASS_1_SGIS = 0x80A5,
    GL_OR_REVERSE    = 0x150B,  GL_MIRRORED_REPEAT      = 0x8370,  GL_T2F_C3F_V3F     = 0x2A2A,  GL_4PASS_2_EXT  = 0x80A6,
    GL_COPY_INVERTED = 0x150C,  GL_MIRRORED_REPEAT_ARB  = 0x8370,  GL_T2F_N3F_V3F     = 0x2A2B,  GL_4PASS_2_SGIS = 0x80A6,
    GL_OR_INVERTED   = 0x150D,  GL_MIRRORED_REPEAT_IBM  = 0x8370,  GL_T2F_C4F_N3F_V3F = 0x2A2C,  GL_4PASS_3_EXT  = 0x80A7,
    GL_NAND          = 0x150E,  GL_MIRRORED_REPEAT_OES  = 0x8370,  GL_T4F_C4F_N3F_V4F = 0x2A2D,  GL_4PASS_3_SGIS = 0x80A7,
    GL_SET           = 0x150F,
    /* BufferTargetARB */                   /* TexStorageAttribs */                                  /* BufferPNameARB */
    GL_PARAMETER_BUFFER          = 0x80EE,  GL_SURFACE_COMPRESSION_FIXED_RATE_NONE_EXT    = 0x96C1,  GL_BUFFER_IMMUTABLE_STORAGE = 0x821F,
    GL_ARRAY_BUFFER              = 0x8892,  GL_SURFACE_COMPRESSION_FIXED_RATE_DEFAULT_EXT = 0x96C2,  GL_BUFFER_STORAGE_FLAGS     = 0x8220,
    GL_ELEMENT_ARRAY_BUFFER      = 0x8893,  GL_SURFACE_COMPRESSION_FIXED_RATE_1BPC_EXT    = 0x96C4,  GL_BUFFER_SIZE              = 0x8764,
    GL_PIXEL_PACK_BUFFER         = 0x88EB,  GL_SURFACE_COMPRESSION_FIXED_RATE_2BPC_EXT    = 0x96C5,  GL_BUFFER_SIZE_ARB          = 0x8764,
    GL_PIXEL_UNPACK_BUFFER       = 0x88EC,  GL_SURFACE_COMPRESSION_FIXED_RATE_3BPC_EXT    = 0x96C6,  GL_BUFFER_USAGE             = 0x8765,
    GL_UNIFORM_BUFFER            = 0x8A11,  GL_SURFACE_COMPRESSION_FIXED_RATE_4BPC_EXT    = 0x96C7,  GL_BUFFER_USAGE_ARB         = 0x8765,
    GL_TRANSFORM_FEEDBACK_BUFFER = 0x8C8E,  GL_SURFACE_COMPRESSION_FIXED_RATE_5BPC_EXT    = 0x96C8,  GL_BUFFER_ACCESS            = 0x88BB,
    GL_COPY_READ_BUFFER          = 0x8F36,  GL_SURFACE_COMPRESSION_FIXED_RATE_6BPC_EXT    = 0x96C9,  GL_BUFFER_ACCESS_ARB        = 0x88BB,
    GL_COPY_WRITE_BUFFER         = 0x8F37,  GL_SURFACE_COMPRESSION_FIXED_RATE_7BPC_EXT    = 0x96CA,  GL_BUFFER_MAPPED            = 0x88BC,
    GL_DRAW_INDIRECT_BUFFER      = 0x8F3F,  GL_SURFACE_COMPRESSION_FIXED_RATE_8BPC_EXT    = 0x96CB,  GL_BUFFER_MAPPED_ARB        = 0x88BC,
    GL_SHADER_STORAGE_BUFFER     = 0x90D2,  GL_SURFACE_COMPRESSION_FIXED_RATE_9BPC_EXT    = 0x96CC,  GL_BUFFER_ACCESS_FLAGS      = 0x911F,
    GL_DISPATCH_INDIRECT_BUFFER  = 0x90EE,  GL_SURFACE_COMPRESSION_FIXED_RATE_10BPC_EXT   = 0x96CD,  GL_BUFFER_MAP_LENGTH        = 0x9120,
    GL_QUERY_BUFFER              = 0x9192,  GL_SURFACE_COMPRESSION_FIXED_RATE_11BPC_EXT   = 0x96CE,  GL_BUFFER_MAP_OFFSET        = 0x9121,
    GL_ATOMIC_COUNTER_BUFFER     = 0x92C0,  GL_SURFACE_COMPRESSION_FIXED_RATE_12BPC_EXT   = 0x96CF,
    /* VertexAttribPropertyARB */                    /* QueryTarget */                                   /* ErrorCode */
    GL_VERTEX_ATTRIB_BINDING              = 0x82D4,  GL_TRANSFORM_FEEDBACK_OVERFLOW           = 0x82EC,  GL_INVALID_ENUM                      = 0x0500,
    GL_VERTEX_ATTRIB_RELATIVE_OFFSET      = 0x82D5,  GL_VERTICES_SUBMITTED                    = 0x82EE,  GL_INVALID_VALUE                     = 0x0501,
    GL_VERTEX_ATTRIB_ARRAY_ENABLED        = 0x8622,  GL_PRIMITIVES_SUBMITTED                  = 0x82EF,  GL_INVALID_OPERATION                 = 0x0502,
    GL_VERTEX_ATTRIB_ARRAY_SIZE           = 0x8623,  GL_VERTEX_SHADER_INVOCATIONS             = 0x82F0,  GL_STACK_OVERFLOW                    = 0x0503,
    GL_VERTEX_ATTRIB_ARRAY_STRIDE         = 0x8624,  GL_TIME_ELAPSED                          = 0x88BF,  GL_STACK_UNDERFLOW                   = 0x0504,
    GL_VERTEX_ATTRIB_ARRAY_TYPE           = 0x8625,  GL_SAMPLES_PASSED                        = 0x8914,  GL_OUT_OF_MEMORY                     = 0x0505,
    GL_CURRENT_VERTEX_ATTRIB              = 0x8626,  GL_ANY_SAMPLES_PASSED                    = 0x8C2F,  GL_INVALID_FRAMEBUFFER_OPERATION     = 0x0506,
    GL_VERTEX_ATTRIB_ARRAY_LONG           = 0x874E,  GL_PRIMITIVES_GENERATED                  = 0x8C87,  GL_INVALID_FRAMEBUFFER_OPERATION_EXT = 0x0506,
    GL_VERTEX_ATTRIB_ARRAY_NORMALIZED     = 0x886A,  GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN = 0x8C88,  GL_INVALID_FRAMEBUFFER_OPERATION_OES = 0x0506,
    GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F,  GL_ANY_SAMPLES_PASSED_CONSERVATIVE       = 0x8D6A,  GL_TABLE_TOO_LARGE_EXT               = 0x8031,
    GL_VERTEX_ATTRIB_ARRAY_INTEGER        = 0x88FD,  GL_TASK_SHADER_INVOCATIONS_EXT           = 0x9753,  GL_TABLE_TOO_LARGE                   = 0x8031,
    GL_VERTEX_ATTRIB_ARRAY_INTEGER_EXT    = 0x88FD,  GL_MESH_SHADER_INVOCATIONS_EXT           = 0x9754,  GL_TEXTURE_TOO_LARGE_EXT             = 0x8065,
    GL_VERTEX_ATTRIB_ARRAY_DIVISOR        = 0x88FE,  GL_MESH_PRIMITIVES_GENERATED_EXT         = 0x9755,
    /* BlendEquationModeEXT */              /* GetTextureParameter */             /* UniformBlockPName */
    GL_FUNC_ADD                  = 0x8006,  GL_NORMAL_MAP              = 0x8511,  GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER    = 0x84F0,
    GL_FUNC_ADD_EXT              = 0x8006,  GL_NORMAL_MAP_ARB          = 0x8511,  GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER = 0x84F1,
    GL_MIN                       = 0x8007,  GL_NORMAL_MAP_EXT          = 0x8511,  GL_UNIFORM_BLOCK_BINDING                              = 0x8A3F,
    GL_MIN_EXT                   = 0x8007,  GL_NORMAL_MAP_NV           = 0x8511,  GL_UNIFORM_BLOCK_DATA_SIZE                            = 0x8A40,
    GL_MAX                       = 0x8008,  GL_NORMAL_MAP_OES          = 0x8511,  GL_UNIFORM_BLOCK_NAME_LENGTH                          = 0x8A41,
    GL_MAX_EXT                   = 0x8008,  GL_REFLECTION_MAP          = 0x8512,  GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS                      = 0x8A42,
    GL_FUNC_SUBTRACT             = 0x800A,  GL_REFLECTION_MAP_ARB      = 0x8512,  GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES               = 0x8A43,
    GL_FUNC_SUBTRACT_EXT         = 0x800A,  GL_REFLECTION_MAP_EXT      = 0x8512,  GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER          = 0x8A44,
    GL_FUNC_REVERSE_SUBTRACT     = 0x800B,  GL_REFLECTION_MAP_NV       = 0x8512,  GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER        = 0x8A45,
    GL_FUNC_REVERSE_SUBTRACT_EXT = 0x800B,  GL_REFLECTION_MAP_OES      = 0x8512,  GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER        = 0x8A46,
    GL_ALPHA_MIN_SGIX            = 0x8320,  GL_SURFACE_COMPRESSION_EXT = 0x96C0,  GL_UNIFORM_BLOCK_REFERENCED_BY_COMPUTE_SHADER         = 0x90EC,
    GL_ALPHA_MAX_SGIX            = 0x8321,
    /* VertexShaderCoordOutEXT */  /* PixelMap */                 /* LightParameter */                /* ImageTransformPNameHP */
    GL_X_EXT            = 0x87D5,  GL_PIXEL_MAP_I_TO_I = 0x0C70,  GL_AMBIENT               = 0x1200,  GL_IMAGE_SCALE_X_HP         = 0x8155,
    GL_Y_EXT            = 0x87D6,  GL_PIXEL_MAP_S_TO_S = 0x0C71,  GL_DIFFUSE               = 0x1201,  GL_IMAGE_SCALE_Y_HP         = 0x8156,
    GL_Z_EXT            = 0x87D7,  GL_PIXEL_MAP_I_TO_R = 0x0C72,  GL_SPECULAR              = 0x1202,  GL_IMAGE_TRANSLATE_X_HP     = 0x8157,
    GL_W_EXT            = 0x87D8,  GL_PIXEL_MAP_I_TO_G = 0x0C73,  GL_POSITION              = 0x1203,  GL_IMAGE_TRANSLATE_Y_HP     = 0x8158,
    GL_NEGATIVE_X_EXT   = 0x87D9,  GL_PIXEL_MAP_I_TO_B = 0x0C74,  GL_SPOT_DIRECTION        = 0x1204,  GL_IMAGE_ROTATE_ANGLE_HP    = 0x8159,
    GL_NEGATIVE_Y_EXT   = 0x87DA,  GL_PIXEL_MAP_I_TO_A = 0x0C75,  GL_SPOT_EXPONENT         = 0x1205,  GL_IMAGE_ROTATE_ORIGIN_X_HP = 0x815A,
    GL_NEGATIVE_Z_EXT   = 0x87DB,  GL_PIXEL_MAP_R_TO_R = 0x0C76,  GL_SPOT_CUTOFF           = 0x1206,  GL_IMAGE_ROTATE_ORIGIN_Y_HP = 0x815B,
    GL_NEGATIVE_W_EXT   = 0x87DC,  GL_PIXEL_MAP_G_TO_G = 0x0C77,  GL_CONSTANT_ATTENUATION  = 0x1207,  GL_IMAGE_MAG_FILTER_HP      = 0x815C,
    GL_ZERO_EXT         = 0x87DD,  GL_PIXEL_MAP_B_TO_B = 0x0C78,  GL_LINEAR_ATTENUATION    = 0x1208,  GL_IMAGE_MIN_FILTER_HP      = 0x815D,
    GL_ONE_EXT          = 0x87DE,  GL_PIXEL_MAP_A_TO_A = 0x0C79,  GL_QUADRATIC_ATTENUATION = 0x1209,  GL_IMAGE_CUBIC_WEIGHT_HP    = 0x815E,
    GL_NEGATIVE_ONE_EXT = 0x87DF,
    /* AtomicCounterBufferPName */                                           /* SubgroupSupportedFeatures */                             /* DebugType */
    GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_COMPUTE_SHADER         = 0x90ED,  GL_SUBGROUP_FEATURE_BASIC_BIT_KHR            = 0x00000001,  GL_DEBUG_TYPE_ERROR               = 0x824C,
    GL_ATOMIC_COUNTER_BUFFER_BINDING                              = 0x92C1,  GL_SUBGROUP_FEATURE_VOTE_BIT_KHR             = 0x00000002,  GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR = 0x824D,
    GL_ATOMIC_COUNTER_BUFFER_DATA_SIZE                            = 0x92C4,  GL_SUBGROUP_FEATURE_ARITHMETIC_BIT_KHR       = 0x00000004,  GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  = 0x824E,
    GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTERS               = 0x92C5,  GL_SUBGROUP_FEATURE_BALLOT_BIT_KHR           = 0x00000008,  GL_DEBUG_TYPE_PORTABILITY         = 0x824F,
    GL_ATOMIC_COUNTER_BUFFER_ACTIVE_ATOMIC_COUNTER_INDICES        = 0x92C6,  GL_SUBGROUP_FEATURE_SHUFFLE_BIT_KHR          = 0x00000010,  GL_DEBUG_TYPE_PERFORMANCE         = 0x8250,
    GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_VERTEX_SHADER          = 0x92C7,  GL_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT_KHR = 0x00000020,  GL_DEBUG_TYPE_OTHER               = 0x8251,
    GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_CONTROL_SHADER    = 0x92C8,  GL_SUBGROUP_FEATURE_CLUSTERED_BIT_KHR        = 0x00000040,  GL_DEBUG_TYPE_MARKER              = 0x8268,
    GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_TESS_EVALUATION_SHADER = 0x92C9,  GL_SUBGROUP_FEATURE_QUAD_BIT_KHR             = 0x00000080,  GL_DEBUG_TYPE_PUSH_GROUP          = 0x8269,
    GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_GEOMETRY_SHADER        = 0x92CA,  GL_SUBGROUP_FEATURE_PARTITIONED_BIT_NV       = 0x00000100,  GL_DEBUG_TYPE_POP_GROUP           = 0x826A,
    GL_ATOMIC_COUNTER_BUFFER_REFERENCED_BY_FRAGMENT_SHADER        = 0x92CB,
    /* FramebufferStatus */                                 /* BufferUsageARB */       /* UniformPName */
    GL_FRAMEBUFFER_UNDEFINED                     = 0x8219,  GL_STREAM_DRAW  = 0x88E0,  GL_UNIFORM_TYPE                        = 0x8A37,
    GL_FRAMEBUFFER_COMPLETE                      = 0x8CD5,  GL_STREAM_READ  = 0x88E1,  GL_UNIFORM_SIZE                        = 0x8A38,
    GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT         = 0x8CD6,  GL_STREAM_COPY  = 0x88E2,  GL_UNIFORM_NAME_LENGTH                 = 0x8A39,
    GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7,  GL_STATIC_DRAW  = 0x88E4,  GL_UNIFORM_BLOCK_INDEX                 = 0x8A3A,
    GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER        = 0x8CDB,  GL_STATIC_READ  = 0x88E5,  GL_UNIFORM_OFFSET                      = 0x8A3B,
    GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER        = 0x8CDC,  GL_STATIC_COPY  = 0x88E6,  GL_UNIFORM_ARRAY_STRIDE                = 0x8A3C,
    GL_FRAMEBUFFER_UNSUPPORTED                   = 0x8CDD,  GL_DYNAMIC_DRAW = 0x88E8,  GL_UNIFORM_MATRIX_STRIDE               = 0x8A3D,
    GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE        = 0x8D56,  GL_DYNAMIC_READ = 0x88E9,  GL_UNIFORM_IS_ROW_MAJOR                = 0x8A3E,
    GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS      = 0x8DA8,  GL_DYNAMIC_COPY = 0x88EA,  GL_UNIFORM_ATOMIC_COUNTER_BUFFER_INDEX = 0x92DA,
    /* TextureLayout */                                         /* ShadingRateQCOM */                      /* ShadingRate */
    GL_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_EXT = 0x9530,  GL_SHADING_RATE_1X1_PIXELS_QCOM = 0x96A6,  GL_SHADING_RATE_1X1_PIXELS_EXT = 0x96A6,
    GL_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_EXT = 0x9531,  GL_SHADING_RATE_1X2_PIXELS_QCOM = 0x96A7,  GL_SHADING_RATE_1X2_PIXELS_EXT = 0x96A7,
    GL_LAYOUT_GENERAL_EXT                            = 0x958D,  GL_SHADING_RATE_2X1_PIXELS_QCOM = 0x96A8,  GL_SHADING_RATE_2X1_PIXELS_EXT = 0x96A8,
    GL_LAYOUT_COLOR_ATTACHMENT_EXT                   = 0x958E,  GL_SHADING_RATE_2X2_PIXELS_QCOM = 0x96A9,  GL_SHADING_RATE_2X2_PIXELS_EXT = 0x96A9,
    GL_LAYOUT_DEPTH_STENCIL_ATTACHMENT_EXT           = 0x958F,  GL_SHADING_RATE_1X4_PIXELS_QCOM = 0x96AA,  GL_SHADING_RATE_1X4_PIXELS_EXT = 0x96AA,
    GL_LAYOUT_DEPTH_STENCIL_READ_ONLY_EXT            = 0x9590,  GL_SHADING_RATE_4X1_PIXELS_QCOM = 0x96AB,  GL_SHADING_RATE_4X1_PIXELS_EXT = 0x96AB,
    GL_LAYOUT_SHADER_READ_ONLY_EXT                   = 0x9591,  GL_SHADING_RATE_4X2_PIXELS_QCOM = 0x96AC,  GL_SHADING_RATE_4X2_PIXELS_EXT = 0x96AC,
    GL_LAYOUT_TRANSFER_SRC_EXT                       = 0x9592,  GL_SHADING_RATE_2X4_PIXELS_QCOM = 0x96AD,  GL_SHADING_RATE_2X4_PIXELS_EXT = 0x96AD,
    GL_LAYOUT_TRANSFER_DST_EXT                       = 0x9593,  GL_SHADING_RATE_4X4_PIXELS_QCOM = 0x96AE,  GL_SHADING_RATE_4X4_PIXELS_EXT = 0x96AE,
    /* ContextFlagMask */                                    /* MapBufferAccessMask */                   /* PathTransformType */
    GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT    = 0x00000001,  GL_MAP_INVALIDATE_RANGE_BIT      = 0x0004,  GL_TRANSLATE_X_NV         = 0x908E,
    GL_CONTEXT_FLAG_DEBUG_BIT                 = 0x00000002,  GL_MAP_INVALIDATE_RANGE_BIT_EXT  = 0x0004,  GL_TRANSLATE_Y_NV         = 0x908F,
    GL_CONTEXT_FLAG_DEBUG_BIT_KHR             = 0x00000002,  GL_MAP_INVALIDATE_BUFFER_BIT     = 0x0008,  GL_TRANSLATE_2D_NV        = 0x9090,
    GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT         = 0x00000004,  GL_MAP_INVALIDATE_BUFFER_BIT_EXT = 0x0008,  GL_TRANSLATE_3D_NV        = 0x9091,
    GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB     = 0x00000004,  GL_MAP_FLUSH_EXPLICIT_BIT        = 0x0010,  GL_AFFINE_2D_NV           = 0x9092,
    GL_CONTEXT_FLAG_NO_ERROR_BIT              = 0x00000008,  GL_MAP_FLUSH_EXPLICIT_BIT_EXT    = 0x0010,  GL_AFFINE_3D_NV           = 0x9094,
    GL_CONTEXT_FLAG_NO_ERROR_BIT_KHR          = 0x00000008,  GL_MAP_UNSYNCHRONIZED_BIT        = 0x0020,  GL_TRANSPOSE_AFFINE_2D_NV = 0x9096,
    GL_CONTEXT_FLAG_PROTECTED_CONTENT_BIT_EXT = 0x00000010,  GL_MAP_UNSYNCHRONIZED_BIT_EXT    = 0x0020,  GL_TRANSPOSE_AFFINE_3D_NV = 0x9098,
    /* StencilFunction */  /* FeedBackToken */              /* ObjectIdentifier */           /* TextureGenParameter */
    GL_NEVER    = 0x0200,  GL_PASS_THROUGH_TOKEN = 0x0700,  GL_BUFFER             = 0x82E0,  GL_TEXTURE_GEN_MODE     = 0x2500,
    GL_LESS     = 0x0201,  GL_POINT_TOKEN        = 0x0701,  GL_SHADER             = 0x82E1,  GL_TEXTURE_GEN_MODE_OES = 0x2500,
    GL_EQUAL    = 0x0202,  GL_LINE_TOKEN         = 0x0702,  GL_PROGRAM            = 0x82E2,  GL_OBJECT_PLANE         = 0x2501,
    GL_LEQUAL   = 0x0203,  GL_POLYGON_TOKEN      = 0x0703,  GL_QUERY              = 0x82E3,  GL_EYE_PLANE            = 0x2502,
    GL_GREATER  = 0x0204,  GL_BITMAP_TOKEN       = 0x0704,  GL_PROGRAM_PIPELINE   = 0x82E4,  GL_EYE_POINT_SGIS       = 0x81F4,
    GL_NOTEQUAL = 0x0205,  GL_DRAW_PIXEL_TOKEN   = 0x0705,  GL_SAMPLER            = 0x82E6,  GL_OBJECT_POINT_SGIS    = 0x81F5,
    GL_GEQUAL   = 0x0206,  GL_COPY_PIXEL_TOKEN   = 0x0706,  GL_FRAMEBUFFER        = 0x8D40,  GL_EYE_LINE_SGIS        = 0x81F6,
    GL_ALWAYS   = 0x0207,  GL_LINE_RESET_TOKEN   = 0x0707,  GL_TRANSFORM_FEEDBACK = 0x8E22,  GL_OBJECT_LINE_SGIS     = 0x81F7,
    /* TextureMinFilter */                     /* CombinerMappingNV */            /* CombinerStageNV */      /* VertexStreamATI */
    GL_NEAREST_MIPMAP_NEAREST       = 0x2700,  GL_UNSIGNED_IDENTITY_NV = 0x8536,  GL_COMBINER0_NV = 0x8550,  GL_VERTEX_STREAM0_ATI = 0x876C,
    GL_LINEAR_MIPMAP_NEAREST        = 0x2701,  GL_UNSIGNED_INVERT_NV   = 0x8537,  GL_COMBINER1_NV = 0x8551,  GL_VERTEX_STREAM1_ATI = 0x876D,
    GL_NEAREST_MIPMAP_LINEAR        = 0x2702,  GL_EXPAND_NORMAL_NV     = 0x8538,  GL_COMBINER2_NV = 0x8552,  GL_VERTEX_STREAM2_ATI = 0x876E,
    GL_LINEAR_MIPMAP_LINEAR         = 0x2703,  GL_EXPAND_NEGATE_NV     = 0x8539,  GL_COMBINER3_NV = 0x8553,  GL_VERTEX_STREAM3_ATI = 0x876F,
    GL_LINEAR_CLIPMAP_LINEAR_SGIX   = 0x8170,  GL_HALF_BIAS_NORMAL_NV  = 0x853A,  GL_COMBINER4_NV = 0x8554,  GL_VERTEX_STREAM4_ATI = 0x8770,
    GL_NEAREST_CLIPMAP_NEAREST_SGIX = 0x844D,  GL_HALF_BIAS_NEGATE_NV  = 0x853B,  GL_COMBINER5_NV = 0x8555,  GL_VERTEX_STREAM5_ATI = 0x8771,
    GL_NEAREST_CLIPMAP_LINEAR_SGIX  = 0x844E,  GL_SIGNED_IDENTITY_NV   = 0x853C,  GL_COMBINER6_NV = 0x8556,  GL_VERTEX_STREAM6_ATI = 0x8772,
    GL_LINEAR_CLIPMAP_NEAREST_SGIX  = 0x844F,  GL_SIGNED_NEGATE_NV     = 0x853D,  GL_COMBINER7_NV = 0x8557,  GL_VERTEX_STREAM7_ATI = 0x8773,
    /* ConditionalRenderMode */                    /* ExternalHandleType */                       /* FragmentShaderDestModMaskATI */
    GL_QUERY_WAIT                       = 0x8E13,  GL_HANDLE_TYPE_OPAQUE_FD_EXT        = 0x9586,  GL_2X_BIT_ATI       = 0x00000001,
    GL_QUERY_NO_WAIT                    = 0x8E14,  GL_HANDLE_TYPE_OPAQUE_WIN32_EXT     = 0x9587,  GL_4X_BIT_ATI       = 0x00000002,
    GL_QUERY_BY_REGION_WAIT             = 0x8E15,  GL_HANDLE_TYPE_OPAQUE_WIN32_KMT_EXT = 0x9588,  GL_8X_BIT_ATI       = 0x00000004,
    GL_QUERY_BY_REGION_NO_WAIT          = 0x8E16,  GL_HANDLE_TYPE_D3D12_TILEPOOL_EXT   = 0x9589,  GL_HALF_BIT_ATI     = 0x00000008,
    GL_QUERY_WAIT_INVERTED              = 0x8E17,  GL_HANDLE_TYPE_D3D12_RESOURCE_EXT   = 0x958A,  GL_QUARTER_BIT_ATI  = 0x00000010,
    GL_QUERY_NO_WAIT_INVERTED           = 0x8E18,  GL_HANDLE_TYPE_D3D11_IMAGE_EXT      = 0x958B,  GL_EIGHTH_BIT_ATI   = 0x00000020,
    GL_QUERY_BY_REGION_WAIT_INVERTED    = 0x8E19,  GL_HANDLE_TYPE_D3D11_IMAGE_KMT_EXT  = 0x958C,  GL_SATURATE_BIT_ATI = 0x00000040,
    GL_QUERY_BY_REGION_NO_WAIT_INVERTED = 0x8E1A,  GL_HANDLE_TYPE_D3D12_FENCE_EXT      = 0x9594,
    /* TraceMaskMESA */                     /* StencilOp */         /* CopyImageSubDataTarget */               /* TextureMagFilter */
    GL_TRACE_OPERATIONS_BIT_MESA = 0x0001,  GL_INVERT    = 0x150A,  GL_TEXTURE_3D                   = 0x806F,  GL_LINEAR_DETAIL_SGIS        = 0x8097,
    GL_TRACE_PRIMITIVES_BIT_MESA = 0x0002,  GL_KEEP      = 0x1E00,  GL_TEXTURE_1D_ARRAY             = 0x8C18,  GL_LINEAR_DETAIL_ALPHA_SGIS  = 0x8098,
    GL_TRACE_ARRAYS_BIT_MESA     = 0x0004,  GL_REPLACE   = 0x1E01,  GL_TEXTURE_2D_ARRAY             = 0x8C1A,  GL_LINEAR_DETAIL_COLOR_SGIS  = 0x8099,
    GL_TRACE_TEXTURES_BIT_MESA   = 0x0008,  GL_INCR      = 0x1E02,  GL_RENDERBUFFER                 = 0x8D41,  GL_LINEAR_SHARPEN_SGIS       = 0x80AD,
    GL_TRACE_PIXELS_BIT_MESA     = 0x0010,  GL_DECR      = 0x1E03,  GL_TEXTURE_CUBE_MAP_ARRAY       = 0x9009,  GL_LINEAR_SHARPEN_ALPHA_SGIS = 0x80AE,
    GL_TRACE_ERRORS_BIT_MESA     = 0x0020,  GL_INCR_WRAP = 0x8507,  GL_TEXTURE_2D_MULTISAMPLE       = 0x9100,  GL_LINEAR_SHARPEN_COLOR_SGIS = 0x80AF,
    GL_TRACE_ALL_BITS_MESA       = 0xFFFF,  GL_DECR_WRAP = 0x8508,  GL_TEXTURE_2D_MULTISAMPLE_ARRAY = 0x9102,  GL_FILTER4_SGIS              = 0x8146,
    /* ColorTableTargetSGI */                             /* ProgramTarget */                      /* PipelineParameterName */
    GL_PROXY_TEXTURE_COLOR_TABLE_SGI           = 0x80BD,  GL_TEXT_FRAGMENT_SHADER_ATI   = 0x8200,  GL_ACTIVE_PROGRAM         = 0x8259,
    GL_PROXY_COLOR_TABLE                       = 0x80D3,  GL_VERTEX_PROGRAM_ARB         = 0x8620,  GL_FRAGMENT_SHADER        = 0x8B30,
    GL_PROXY_COLOR_TABLE_SGI                   = 0x80D3,  GL_FRAGMENT_PROGRAM_ARB       = 0x8804,  GL_VERTEX_SHADER          = 0x8B31,
    GL_PROXY_POST_CONVOLUTION_COLOR_TABLE      = 0x80D4,  GL_TESS_CONTROL_PROGRAM_NV    = 0x891E,  GL_INFO_LOG_LENGTH        = 0x8B84,
    GL_PROXY_POST_CONVOLUTION_COLOR_TABLE_SGI  = 0x80D4,  GL_TESS_EVALUATION_PROGRAM_NV = 0x891F,  GL_GEOMETRY_SHADER        = 0x8DD9,
    GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE     = 0x80D5,  GL_GEOMETRY_PROGRAM_NV        = 0x8C26,  GL_TESS_EVALUATION_SHADER = 0x8E87,
    GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE_SGI = 0x80D5,  GL_COMPUTE_PROGRAM_NV         = 0x90FB,  GL_TESS_CONTROL_SHADER    = 0x8E88,
    /* CombinerRegisterNV */         /* CombinerVariableNV */    /* ShaderBinaryFormat */                  /* TextureEnvMode */
    GL_TEXTURE0_ARB       = 0x84C0,  GL_VARIABLE_A_NV = 0x8523,  GL_SGX_BINARY_IMG              = 0x8C0A,  GL_BLEND                 = 0x0BE2,
    GL_TEXTURE1_ARB       = 0x84C1,  GL_VARIABLE_B_NV = 0x8524,  GL_MALI_SHADER_BINARY_ARM      = 0x8F60,  GL_MODULATE              = 0x2100,
    GL_PRIMARY_COLOR_NV   = 0x852C,  GL_VARIABLE_C_NV = 0x8525,  GL_SHADER_BINARY_VIV           = 0x8FC4,  GL_DECAL                 = 0x2101,
    GL_SECONDARY_COLOR_NV = 0x852D,  GL_VARIABLE_D_NV = 0x8526,  GL_SHADER_BINARY_DMP           = 0x9250,  GL_REPLACE_EXT           = 0x8062,
    GL_SPARE0_NV          = 0x852E,  GL_VARIABLE_E_NV = 0x8527,  GL_GCCSO_SHADER_BINARY_FJ      = 0x9260,  GL_TEXTURE_ENV_BIAS_SGIX = 0x80BE,
    GL_SPARE1_NV          = 0x852F,  GL_VARIABLE_F_NV = 0x8528,  GL_SHADER_BINARY_FORMAT_SPIR_V = 0x9551,  GL_COMBINE               = 0x8570,
    GL_DISCARD_NV         = 0x8530,  GL_VARIABLE_G_NV = 0x8529,  GL_HUAWEI_SHADER_BINARY        = 0x9770,
    /* DebugSource */                          /* VertexAttribIType */      /* PrecisionType */        /* OcclusionQueryEventMaskAMD */
    GL_DEBUG_SOURCE_API             = 0x8246,  GL_BYTE           = 0x1400,  GL_LOW_FLOAT    = 0x8DF0,  GL_QUERY_DEPTH_PASS_EVENT_BIT_AMD        = 0x00000001,
    GL_DEBUG_SOURCE_WINDOW_SYSTEM   = 0x8247,  GL_UNSIGNED_BYTE  = 0x1401,  GL_MEDIUM_FLOAT = 0x8DF1,  GL_QUERY_DEPTH_FAIL_EVENT_BIT_AMD        = 0x00000002,
    GL_DEBUG_SOURCE_SHADER_COMPILER = 0x8248,  GL_SHORT          = 0x1402,  GL_HIGH_FLOAT   = 0x8DF2,  GL_QUERY_STENCIL_FAIL_EVENT_BIT_AMD      = 0x00000004,
    GL_DEBUG_SOURCE_THIRD_PARTY     = 0x8249,  GL_UNSIGNED_SHORT = 0x1403,  GL_LOW_INT      = 0x8DF3,  GL_QUERY_DEPTH_BOUNDS_FAIL_EVENT_BIT_AMD = 0x00000008,
    GL_DEBUG_SOURCE_APPLICATION     = 0x824A,  GL_INT            = 0x1404,  GL_MEDIUM_INT   = 0x8DF4,  
    GL_DEBUG_SOURCE_OTHER           = 0x824B,  GL_UNSIGNED_INT   = 0x1405,  GL_HIGH_INT     = 0x8DF5,
    /* TransformFeedbackTokenNV */  /* PathColorFormat */         /* PixelTexGenModeSGIX */                  /* AccumOp */
    GL_NEXT_BUFFER_NV      = -2,    GL_RGB             = 0x1907,  GL_PIXEL_TEX_GEN_Q_CEILING_SGIX = 0x8184,  GL_ACCUM  = 0x0100,
    GL_SKIP_COMPONENTS4_NV = -3,    GL_RGBA            = 0x1908,  GL_PIXEL_TEX_GEN_Q_ROUND_SGIX   = 0x8185,  GL_LOAD   = 0x0101,
    GL_SKIP_COMPONENTS3_NV = -4,    GL_LUMINANCE       = 0x1909,  GL_PIXEL_TEX_GEN_Q_FLOOR_SGIX   = 0x8186,  GL_RETURN = 0x0102,
    GL_SKIP_COMPONENTS2_NV = -5,    GL_LUMINANCE_ALPHA = 0x190A,  GL_PIXEL_TEX_GEN_ALPHA_LS_SGIX  = 0x8189,  GL_MULT   = 0x0103,
    GL_SKIP_COMPONENTS1_NV = -6,    GL_INTENSITY       = 0x8049,  GL_PIXEL_TEX_GEN_ALPHA_MS_SGIX  = 0x818A,  GL_ADD    = 0x0104,
    /* FeedbackType */             /* GetFramebufferParameter */                            /* PixelStoreParameter */
    GL_2D               = 0x0600,  GL_FRAMEBUFFER_DEFAULT_WIDTH                  = 0x9310,  GL_UNPACK_ROW_LENGTH_EXT  = 0x0CF2,
    GL_3D               = 0x0601,  GL_FRAMEBUFFER_DEFAULT_HEIGHT                 = 0x9311,  GL_UNPACK_SKIP_ROWS_EXT   = 0x0CF3,
    GL_3D_COLOR         = 0x0602,  GL_FRAMEBUFFER_DEFAULT_LAYERS                 = 0x9312,  GL_UNPACK_SKIP_PIXELS_EXT = 0x0CF4,
    GL_3D_COLOR_TEXTURE = 0x0603,  GL_FRAMEBUFFER_DEFAULT_SAMPLES                = 0x9313,  GL_PACK_RESAMPLE_OML      = 0x8984,
    GL_4D_COLOR_TEXTURE = 0x0604,  GL_FRAMEBUFFER_DEFAULT_FIXED_SAMPLE_LOCATIONS = 0x9314,  GL_UNPACK_RESAMPLE_OML    = 0x8985,
    /* DebugSeverity */                       /* StringName */                       /* TextureGenMode */
    GL_DONT_CARE                   = 0x1100,  GL_VENDOR                   = 0x1F00,  GL_SPHERE_MAP                    = 0x2402,
    GL_DEBUG_SEVERITY_NOTIFICATION = 0x826B,  GL_RENDERER                 = 0x1F01,  GL_EYE_DISTANCE_TO_POINT_SGIS    = 0x81F0,
    GL_DEBUG_SEVERITY_HIGH         = 0x9146,  GL_VERSION                  = 0x1F02,  GL_OBJECT_DISTANCE_TO_POINT_SGIS = 0x81F1,
    GL_DEBUG_SEVERITY_MEDIUM       = 0x9147,  GL_EXTENSIONS               = 0x1F03,  GL_EYE_DISTANCE_TO_LINE_SGIS     = 0x81F2,
    GL_DEBUG_SEVERITY_LOW          = 0x9148,  GL_SHADING_LANGUAGE_VERSION = 0x8B8C,  GL_OBJECT_DISTANCE_TO_LINE_SGIS  = 0x81F3,
    /* LightTextureModeEXT */           /* FragmentOp2ATI */   /* FragmentOp3ATI */       /* ProgramStagePName */
    GL_FRAGMENT_MATERIAL_EXT = 0x8349,  GL_ADD_ATI  = 0x8963,  GL_MAD_ATI      = 0x8968,  GL_ACTIVE_SUBROUTINES                   = 0x8DE5,
    GL_FRAGMENT_NORMAL_EXT   = 0x834A,  GL_MUL_ATI  = 0x8964,  GL_LERP_ATI     = 0x8969,  GL_ACTIVE_SUBROUTINE_UNIFORMS           = 0x8DE6,
    GL_FRAGMENT_COLOR_EXT    = 0x834C,  GL_SUB_ATI  = 0x8965,  GL_CND_ATI      = 0x896A,  GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS  = 0x8E47,
    GL_FRAGMENT_DEPTH        = 0x8452,  GL_DOT3_ATI = 0x8966,  GL_CND0_ATI     = 0x896B,  GL_ACTIVE_SUBROUTINE_MAX_LENGTH         = 0x8E48,
    GL_FRAGMENT_DEPTH_EXT    = 0x8452,  GL_DOT4_ATI = 0x8967,  GL_DOT2_ADD_ATI = 0x896C,  GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH = 0x8E49,
    /* SemaphoreParameterName */              /* ShadingRateCombinerOp */                                 /* FragmentShaderDestMaskATI */  /* TextureSwizzle */
    GL_D3D12_FENCE_VALUE_EXT       = 0x9595,  GL_FRAGMENT_SHADING_RATE_COMBINER_OP_KEEP_EXT    = 0x96D2,  GL_RED_BIT_ATI   = 0x00000001,   GL_RED   = 0x1903,
    GL_TIMELINE_SEMAPHORE_VALUE_NV = 0x9595,  GL_FRAGMENT_SHADING_RATE_COMBINER_OP_REPLACE_EXT = 0x96D3,  GL_GREEN_BIT_ATI = 0x00000002,   GL_GREEN = 0x1904,
    GL_SEMAPHORE_TYPE_NV           = 0x95B3,  GL_FRAGMENT_SHADING_RATE_COMBINER_OP_MIN_EXT     = 0x96D4,  GL_BLUE_BIT_ATI  = 0x00000004,   GL_BLUE  = 0x1905,
    GL_SEMAPHORE_TYPE_BINARY_NV    = 0x95B4,  GL_FRAGMENT_SHADING_RATE_COMBINER_OP_MAX_EXT     = 0x96D5,  GL_NONE          = 0,            GL_ALPHA = 0x1906,
    GL_SEMAPHORE_TYPE_TIMELINE_NV  = 0x95B5,  GL_FRAGMENT_SHADING_RATE_COMBINER_OP_MUL_EXT     = 0x96D6,
    /* PathGenMode */                         /* FogMode */               /* MaterialParameter */           /* VertexAttribPointerType */
    GL_EYE_LINEAR                  = 0x2400,  GL_EXP           = 0x0800,  GL_EMISSION            = 0x1600,  GL_INT64_ARB          = 0x140E,
    GL_OBJECT_LINEAR               = 0x2401,  GL_EXP2          = 0x0801,  GL_SHININESS           = 0x1601,  GL_INT64_NV           = 0x140E,
    GL_CONSTANT                    = 0x8576,  GL_LINEAR        = 0x2601,  GL_AMBIENT_AND_DIFFUSE = 0x1602,  GL_UNSIGNED_INT64_ARB = 0x140F,
    GL_PATH_OBJECT_BOUNDING_BOX_NV = 0x908A,  GL_FOG_FUNC_SGIS = 0x812A,  GL_COLOR_INDEXES       = 0x1603,  GL_UNSIGNED_INT64_NV  = 0x140F,
    /* MatrixMode */             /* TextureCoordName */  /* GetMinmaxParameterPNameEXT */  /* LightModelColorControl */
    GL_MODELVIEW      = 0x1700,  GL_S = 0x2000,          GL_MINMAX_FORMAT     = 0x802F,    GL_SINGLE_COLOR                = 0x81F9,
    GL_MODELVIEW0_EXT = 0x1700,  GL_T = 0x2001,          GL_MINMAX_FORMAT_EXT = 0x802F,    GL_SINGLE_COLOR_EXT            = 0x81F9,
    GL_PROJECTION     = 0x1701,  GL_R = 0x2002,          GL_MINMAX_SINK       = 0x8030,    GL_SEPARATE_SPECULAR_COLOR     = 0x81FA,
    GL_TEXTURE        = 0x1702,  GL_Q = 0x2003,          GL_MINMAX_SINK_EXT   = 0x8030,    GL_SEPARATE_SPECULAR_COLOR_EXT = 0x81FA,
    /* QueryObjectParameterName */       /* GetTexBumpParameterATI */           /* VertexShaderStorageTypeEXT */  /* GetVariantValueEXT */
    GL_QUERY_TARGET           = 0x82EA,  GL_BUMP_ROT_MATRIX_ATI      = 0x8775,  GL_VARIANT_EXT        = 0x87C1,   GL_VARIANT_VALUE_EXT        = 0x87E4,
    GL_QUERY_RESULT           = 0x8866,  GL_BUMP_ROT_MATRIX_SIZE_ATI = 0x8776,  GL_INVARIANT_EXT      = 0x87C2,   GL_VARIANT_DATATYPE_EXT     = 0x87E5,
    GL_QUERY_RESULT_AVAILABLE = 0x8867,  GL_BUMP_NUM_TEX_UNITS_ATI   = 0x8777,  GL_LOCAL_CONSTANT_EXT = 0x87C3,   GL_VARIANT_ARRAY_STRIDE_EXT = 0x87E6,
    GL_QUERY_RESULT_NO_WAIT   = 0x9194,  GL_BUMP_TEX_UNITS_ATI       = 0x8778,  GL_LOCAL_EXT          = 0x87C4,   GL_VARIANT_ARRAY_TYPE_EXT   = 0x87E7,
    /* ClampColorTargetARB */              /* SwizzleOpATI */               /* SyncParameterName */      /* SyncStatus */
    GL_CLAMP_VERTEX_COLOR_ARB   = 0x891A,  GL_SWIZZLE_STR_ATI    = 0x8976,  GL_OBJECT_TYPE    = 0x9112,  GL_ALREADY_SIGNALED    = 0x911A,
    GL_CLAMP_FRAGMENT_COLOR_ARB = 0x891B,  GL_SWIZZLE_STQ_ATI    = 0x8977,  GL_SYNC_CONDITION = 0x9113,  GL_TIMEOUT_EXPIRED     = 0x911B,
    GL_CLAMP_READ_COLOR         = 0x891C,  GL_SWIZZLE_STR_DR_ATI = 0x8978,  GL_SYNC_STATUS    = 0x9114,  GL_CONDITION_SATISFIED = 0x911C,
    GL_CLAMP_READ_COLOR_ARB     = 0x891C,  GL_SWIZZLE_STQ_DQ_ATI = 0x8979,  GL_SYNC_FLAGS     = 0x9115,  GL_WAIT_FAILED         = 0x911D,
    /* ProgramInterfacePName */                  /* ClientAttribMask */                    /* FragmentShaderColorModMaskATI */
    GL_ACTIVE_RESOURCES               = 0x92F5,  GL_CLIENT_PIXEL_STORE_BIT  = 0x00000001,  GL_COMP_BIT_ATI   = 0x00000002,
    GL_MAX_NAME_LENGTH                = 0x92F6,  GL_CLIENT_VERTEX_ARRAY_BIT = 0x00000002,  GL_NEGATE_BIT_ATI = 0x00000004,
    GL_MAX_NUM_ACTIVE_VARIABLES       = 0x92F7,                                            GL_BIAS_BIT_ATI   = 0x00000008,
    GL_MAX_NUM_COMPATIBLE_SUBROUTINES = 0x92F8,
    /* FoveationConfigBitQCOM */                                  /* MapTextureFormatINTEL */             /* TriangleListSUN */
    GL_FOVEATION_ENABLE_BIT_QCOM                   = 0x00000001,  GL_LAYOUT_DEFAULT_INTEL           = 0,  GL_RESTART_SUN        = 0x0001,
    GL_FOVEATION_SCALED_BIN_METHOD_BIT_QCOM        = 0x00000002,  GL_LAYOUT_LINEAR_INTEL            = 1,  GL_REPLACE_MIDDLE_SUN = 0x0002,
    GL_FOVEATION_SUBSAMPLED_LAYOUT_METHOD_BIT_QCOM = 0x00000004,  GL_LAYOUT_LINEAR_CPU_CACHED_INTEL = 2,  GL_REPLACE_OLDEST_SUN = 0x0003,
    /* GraphicsResetStatus */            /* CombinerScaleNV */              /* MapQuery */       /* ListNameType */
    GL_GUILTY_CONTEXT_RESET   = 0x8253,  GL_SCALE_BY_TWO_NV      = 0x853E,  GL_COEFF  = 0x0A00,  GL_2_BYTES = 0x1407,
    GL_INNOCENT_CONTEXT_RESET = 0x8254,  GL_SCALE_BY_FOUR_NV     = 0x853F,  GL_ORDER  = 0x0A01,  GL_3_BYTES = 0x1408,
    GL_UNKNOWN_CONTEXT_RESET  = 0x8255,  GL_SCALE_BY_ONE_HALF_NV = 0x8540,  GL_DOMAIN = 0x0A02,  GL_4_BYTES = 0x1409,
    /* PathFillMode */              /* Buffer */          /* PixelCopyType */       /* PolygonMode */
    GL_PATH_FILL_MODE_NV = 0x9080,  GL_COLOR   = 0x1800,  GL_COLOR_EXT   = 0x1800,  GL_POINT = 0x1B00,
    GL_COUNT_UP_NV       = 0x9088,  GL_DEPTH   = 0x1801,  GL_DEPTH_EXT   = 0x1801,  GL_LINE  = 0x1B01,
    GL_COUNT_DOWN_NV     = 0x9089,  GL_STENCIL = 0x1802,  GL_STENCIL_EXT = 0x1802,  GL_FILL  = 0x1B02,
    /* RenderingMode */    /* TextureEnvTarget */               /* SpriteModeSGIX */                     /* PixelTransformPNameEXT */
    GL_RENDER   = 0x1C00,  GL_TEXTURE_ENV            = 0x2300,  GL_SPRITE_AXIAL_SGIX          = 0x814C,  GL_PIXEL_MAG_FILTER_EXT   = 0x8331,
    GL_FEEDBACK = 0x1C01,  GL_TEXTURE_FILTER_CONTROL = 0x8500,  GL_SPRITE_OBJECT_ALIGNED_SGIX = 0x814D,  GL_PIXEL_MIN_FILTER_EXT   = 0x8332,
    GL_SELECT   = 0x1C02,  GL_POINT_SPRITE           = 0x8861,  GL_SPRITE_EYE_ALIGNED_SGIX    = 0x814E,  GL_PIXEL_CUBIC_WEIGHT_EXT = 0x8333,
    /* PerfQueryDataFlags */                  /* PixelStoreResampleMode */          /* FogCoordSrc */                /* CombinerParameterNV */
    GL_PERFQUERY_DONOT_FLUSH_INTEL = 0x83F9,  GL_RESAMPLE_DECIMATE_SGIX  = 0x8430,  GL_FOG_COORDINATE     = 0x8451,  GL_COMBINER_INPUT_NV           = 0x8542,
    GL_PERFQUERY_FLUSH_INTEL       = 0x83FA,  GL_RESAMPLE_REPLICATE_SGIX = 0x8433,  GL_FOG_COORDINATE_EXT = 0x8451,  GL_COMBINER_MAPPING_NV         = 0x8543,
    GL_PERFQUERY_WAIT_INTEL        = 0x83FB,  GL_RESAMPLE_ZERO_FILL_SGIX = 0x8434,  GL_FOG_COORD          = 0x8451,  GL_COMBINER_COMPONENT_USAGE_NV = 0x8544,
    /* PixelStoreSubsampleRate */           /* VertexArrayPNameAPPLE */        /* DataTypeEXT */        /* PNTrianglesPNameATI */
    GL_PIXEL_SUBSAMPLE_4444_SGIX = 0x85A2,  GL_STORAGE_CLIENT_APPLE = 0x85B4,  GL_SCALAR_EXT = 0x87BE,  GL_PN_TRIANGLES_POINT_MODE_ATI        = 0x87F2,
    GL_PIXEL_SUBSAMPLE_2424_SGIX = 0x85A3,  GL_STORAGE_CACHED_APPLE = 0x85BE,  GL_VECTOR_EXT = 0x87BF,  GL_PN_TRIANGLES_NORMAL_MODE_ATI       = 0x87F3,
    GL_PIXEL_SUBSAMPLE_4242_SGIX = 0x85A4,  GL_STORAGE_SHARED_APPLE = 0x85BF,  GL_MATRIX_EXT = 0x87C0,  GL_PN_TRIANGLES_TESSELATION_LEVEL_ATI = 0x87F4,
    /* BufferAccessARB */    /* ShaderType */                  /* ShaderParameterName */          /* FramebufferTarget */
    GL_READ_ONLY  = 0x88B8,  GL_FRAGMENT_SHADER_ARB = 0x8B30,  GL_SHADER_TYPE          = 0x8B4F,  GL_READ_FRAMEBUFFER = 0x8CA8,
    GL_WRITE_ONLY = 0x88B9,  GL_VERTEX_SHADER_ARB   = 0x8B31,  GL_COMPILE_STATUS       = 0x8B81,  GL_DRAW_FRAMEBUFFER = 0x8CA9,
    GL_READ_WRITE = 0x88BA,  GL_COMPUTE_SHADER      = 0x91B9,  GL_SHADER_SOURCE_LENGTH = 0x8B88,  GL_FRAMEBUFFER_OES  = 0x8D40,
    /* GetMultisamplePNameNV */                    /* PatchParameterName */                /* PathFontTarget */
    GL_SAMPLE_POSITION                  = 0x8E50,  GL_PATCH_VERTICES            = 0x8E72,  GL_STANDARD_FONT_NAME_NV = 0x9072,
    GL_SAMPLE_LOCATION_ARB              = 0x8E50,  GL_PATCH_DEFAULT_INNER_LEVEL = 0x8E73,  GL_SYSTEM_FONT_NAME_NV   = 0x9073,
    GL_PROGRAMMABLE_SAMPLE_LOCATION_ARB = 0x9341,  GL_PATCH_DEFAULT_OUTER_LEVEL = 0x8E74,  GL_FILE_NAME_NV          = 0x9074,
    /* PathListMode */                    /* ContextProfileMask */                            /* SyncObjectMask */
    GL_ACCUM_ADJACENT_PAIRS_NV = 0x90AD,  GL_CONTEXT_CORE_PROFILE_BIT          = 0x00000001,  GL_SYNC_FLUSH_COMMANDS_BIT       = 0x00000001,
    GL_ADJACENT_PAIRS_NV       = 0x90AE,  GL_CONTEXT_COMPATIBILITY_PROFILE_BIT = 0x00000002,  GL_SYNC_FLUSH_COMMANDS_BIT_APPLE = 0x00000001,
    GL_FIRST_TO_REST_NV        = 0x90AF,
    /* PathFontStyle */       /* PerformanceQueryCapsMaskINTEL */              /* FfdMaskSGIX */                               /* ClampColorModeARB */
    GL_BOLD_BIT_NV   = 0x01,  GL_PERFQUERY_SINGLE_CONTEXT_INTEL = 0x00000000,  GL_TEXTURE_DEFORMATION_BIT_SGIX  = 0x00000001,  GL_FIXED_ONLY     = 0x891D,
    GL_ITALIC_BIT_NV = 0x02,  GL_PERFQUERY_GLOBAL_CONTEXT_INTEL = 0x00000001,  GL_GEOMETRY_DEFORMATION_BIT_SGIX = 0x00000002,  GL_FIXED_ONLY_ARB = 0x891D,
    /* TextureCompareMode */             /* FrontFaceDirection */  /* MapTarget */                         /* HintMode */
    GL_COMPARE_R_TO_TEXTURE   = 0x884E,  GL_CW  = 0x0900,          GL_GEOMETRY_DEFORMATION_SGIX = 0x8194,  GL_FASTEST = 0x1101,
    GL_COMPARE_REF_TO_TEXTURE = 0x884E,  GL_CCW = 0x0901,          GL_TEXTURE_DEFORMATION_SGIX  = 0x8195,  GL_NICEST  = 0x1102,
    /* ListMode */                    /* WeightPointerTypeARB */  /* VertexAttribType */           /* UniformType */
    GL_COMPILE             = 0x1300,  GL_FLOAT  = 0x1406,         GL_FIXED              = 0x140C,  GL_SAMPLER_1D_ARRAY = 0x8DC0,
    GL_COMPILE_AND_EXECUTE = 0x1301,  GL_DOUBLE = 0x140A,         GL_INT_2_10_10_10_REV = 0x8D9F,  GL_SAMPLER_2D_ARRAY = 0x8DC1,
    /* ShadingModel */   /* ConvolutionTarget */      /* ConvolutionBorderModeEXT */  /* HistogramTarget */
    GL_FLAT   = 0x1D00,  GL_CONVOLUTION_1D = 0x8010,  GL_REDUCE     = 0x8016,         GL_HISTOGRAM       = 0x8024,
    GL_SMOOTH = 0x1D01,  GL_CONVOLUTION_2D = 0x8011,  GL_REDUCE_EXT = 0x8016,         GL_PROXY_HISTOGRAM = 0x8025,
    /* CullParameterEXT */                        /* FramebufferAttachment */               /* LightTexturePNameEXT */
    GL_CULL_VERTEX_EYE_POSITION_EXT    = 0x81AB,  GL_STENCIL_ATTACHMENT          = 0x8D20,  GL_ATTENUATION_EXT        = 0x834D,
    GL_CULL_VERTEX_OBJECT_POSITION_EXT = 0x81AC,  GL_SHADING_RATE_ATTACHMENT_EXT = 0x96D1,  GL_SHADOW_ATTENUATION_EXT = 0x834E,
    /* PixelTexGenParameterNameSGIS */             /* FenceParameterNameNV */       /* VertexAttribPointerPropertyARB */
    GL_PIXEL_FRAGMENT_RGB_SOURCE_SGIS   = 0x8354,  GL_FENCE_STATUS_NV    = 0x84F3,  GL_VERTEX_ATTRIB_ARRAY_POINTER     = 0x8645,
    GL_PIXEL_FRAGMENT_ALPHA_SOURCE_SGIS = 0x8355,  GL_FENCE_CONDITION_NV = 0x84F4,  GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB = 0x8645,
    /* EvalTargetNV */                  /* MapAttribParameterNV */          /* ArrayObjectUsageATI */  /* PreserveModeATI */
    GL_EVAL_2D_NV            = 0x86C0,  GL_MAP_ATTRIB_U_ORDER_NV = 0x86C3,  GL_STATIC_ATI  = 0x8760,   GL_PRESERVE_ATI = 0x8762,
    GL_EVAL_TRIANGULAR_2D_NV = 0x86C1,  GL_MAP_ATTRIB_V_ORDER_NV = 0x86C4,  GL_DYNAMIC_ATI = 0x8761,   GL_DISCARD_ATI  = 0x8763,
    /* ArrayObjectPNameATI */             /* ParameterRangeEXT */            /* VertexShaderParameterEXT */   /* QueryParameterName */
    GL_OBJECT_BUFFER_SIZE_ATI  = 0x8764,  GL_NORMALIZED_RANGE_EXT = 0x87E0,  GL_CURRENT_VERTEX_EXT = 0x87E2,  GL_QUERY_COUNTER_BITS = 0x8864,
    GL_OBJECT_BUFFER_USAGE_ATI = 0x8765,  GL_FULL_RANGE_EXT       = 0x87E1,  GL_MVP_MATRIX_EXT     = 0x87E3,  GL_CURRENT_QUERY      = 0x8865,
    /* OcclusionQueryParameterNameNV */    /* PixelDataRangeTargetNV */            /* BufferPointerNameARB */           /* ObjectTypeAPPLE */
    GL_PIXEL_COUNT_NV           = 0x8866,  GL_WRITE_PIXEL_DATA_RANGE_NV = 0x8878,  GL_BUFFER_MAP_POINTER     = 0x88BD,  GL_DRAW_PIXELS_APPLE = 0x8A0A,
    GL_PIXEL_COUNT_AVAILABLE_NV = 0x8867,  GL_READ_PIXEL_DATA_RANGE_NV  = 0x8879,  GL_BUFFER_MAP_POINTER_ARB = 0x88BD,  GL_FENCE_APPLE       = 0x8A0B,
    /* SubroutineParameterName */            /* ContainerType */              /* TransformFeedbackPName */            /* TransformFeedbackBufferMode */
    GL_NUM_COMPATIBLE_SUBROUTINES = 0x8E4A,  GL_PROGRAM_OBJECT_ARB = 0x8B40,  GL_TRANSFORM_FEEDBACK_PAUSED = 0x8E23,  GL_INTERLEAVED_ATTRIBS = 0x8C8C,
    GL_COMPATIBLE_SUBROUTINES     = 0x8E4B,  GL_PROGRAM_OBJECT_EXT = 0x8B40,  GL_TRANSFORM_FEEDBACK_ACTIVE = 0x8E24,  GL_SEPARATE_ATTRIBS    = 0x8C8D,
    /* ClipControlOrigin */  /* VertexProvokingMode */             /* PathStringFormat */           /* PathCoverMode */
    GL_LOWER_LEFT = 0x8CA1,  GL_FIRST_VERTEX_CONVENTION = 0x8E4D,  GL_PATH_FORMAT_SVG_NV = 0x9070,  GL_CONVEX_HULL_NV  = 0x908B,
    GL_UPPER_LEFT = 0x8CA2,  GL_LAST_VERTEX_CONVENTION  = 0x8E4E,  GL_PATH_FORMAT_PS_NV  = 0x9071,  GL_BOUNDING_BOX_NV = 0x908D,
    /* PathElementType */  /* PathHandleMissingGlyphs */       /* ClipControlDepth */            /* MemoryObjectParameterName */
    GL_UTF8_NV  = 0x909A,  GL_SKIP_MISSING_GLYPH_NV = 0x90A9,  GL_NEGATIVE_ONE_TO_ONE = 0x935E,  GL_DEDICATED_MEMORY_OBJECT_EXT = 0x9581,
    GL_UTF16_NV = 0x909B,  GL_USE_MISSING_GLYPH_NV  = 0x90AA,  GL_ZERO_TO_ONE         = 0x935F,  GL_PROTECTED_MEMORY_OBJECT_EXT = 0x959B,
    /* ClearBufferMask */                    /* TextureStorageMaskAMD */                      /* CombinerBiasNV */
    GL_COVERAGE_BUFFER_BIT_NV = 0x00008000,  GL_TEXTURE_STORAGE_SPARSE_BIT_AMD = 0x00000001,  GL_BIAS_BY_NEGATIVE_ONE_HALF_NV = 0x8541,
    /* LightModelParameter */                   /* FogPName */              /* TangentPointerTypeEXT */  /* EvalMapsModeNV */
    GL_LIGHT_MODEL_COLOR_CONTROL_EXT = 0x81F8,  GL_FOG_COORD_SRC = 0x8450,  GL_DOUBLE_EXT = 0x140A,      GL_FILL_NV = 0x1B02,
    /* BlitFramebufferFilter */  /* SeparableTarget */      /* HistogramTargetEXT */          /* MinmaxTarget */
    GL_NEAREST = 0x2600,         GL_SEPARABLE_2D = 0x8012,  GL_PROXY_HISTOGRAM_EXT = 0x8025,  GL_MINMAX = 0x802E,
    /* ImageTransformTargetHP */        /* ListParameterName */          /* ProgramParameterPName */     /* PixelTransformTargetEXT */
    GL_IMAGE_TRANSFORM_2D_HP = 0x8161,  GL_LIST_PRIORITY_SGIX = 0x8182,  GL_PROGRAM_SEPARABLE = 0x8258,  GL_PIXEL_TRANSFORM_2D_EXT = 0x8330,
    /* FenceConditionNV */         /* TextureNormalModeEXT */  /* ProgramStringProperty */      /* VertexAttribEnumNV */
    GL_ALL_COMPLETED_NV = 0x84F2,  GL_PERTURB_EXT = 0x85AE,    GL_PROGRAM_STRING_ARB = 0x8628,  GL_PROGRAM_PARAMETER_NV = 0x8644,
    /* MapParameterNV */              /* VariantCapEXT */             /* ProgramFormat */                    /* FragmentOp1ATI */
    GL_MAP_TESSELLATION_NV = 0x86C2,  GL_VARIANT_ARRAY_EXT = 0x87E8,  GL_PROGRAM_FORMAT_ASCII_ARB = 0x8875,  GL_MOV_ATI = 0x8961,
    /* RenderbufferTarget */       /* InstancedPathCoverMode */                    /* SyncCondition */
    GL_RENDERBUFFER_OES = 0x8D41,  GL_BOUNDING_BOX_OF_BOUNDING_BOXES_NV = 0x909C,  GL_SYNC_GPU_COMMANDS_COMPLETE = 0x9117,
};
#endif // defined(__APPLE__)

#define VD_FW_OPENGL_CORE_FUNCTIONS \
VER_START(1_0) \
X(void, Accum, (GLenum op, GLfloat value)) \
X(void, AlphaFunc, (GLenum func, GLfloat ref)) \
X(void, AlphaFuncx, (GLenum func, GLfixed ref)) \
X(void, Begin, (GLenum mode)) \
X(void, Bitmap, (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte * bitmap)) \
X(void, CallList, (GLuint list)) \
X(void, CallLists, (GLsizei n, GLenum type, const void * lists)) \
X(void, ClearAccum, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)) \
X(void, ClearColorx, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)) \
X(void, ClearDepth, (GLdouble depth)) \
X(void, ClearDepthx, (GLfixed depth)) \
X(void, ClearIndex, (GLfloat c)) \
X(void, ClientActiveTexture, (GLenum texture)) \
X(void, ClipPlane, (GLenum plane, const GLdouble * equation)) \
X(void, ClipPlanef, (GLenum p, const GLfloat * eqn)) \
X(void, ClipPlanex, (GLenum plane, const GLfixed * equation)) \
X(void, Color3b, (GLbyte red, GLbyte green, GLbyte blue)) \
X(void, Color3bv, (const GLbyte * v)) \
X(void, Color3d, (GLdouble red, GLdouble green, GLdouble blue)) \
X(void, Color3dv, (const GLdouble * v)) \
X(void, Color3f, (GLfloat red, GLfloat green, GLfloat blue)) \
X(void, Color3fv, (const GLfloat * v)) \
X(void, Color3i, (GLint red, GLint green, GLint blue)) \
X(void, Color3iv, (const GLint * v)) \
X(void, Color3s, (GLshort red, GLshort green, GLshort blue)) \
X(void, Color3sv, (const GLshort * v)) \
X(void, Color3ub, (GLubyte red, GLubyte green, GLubyte blue)) \
X(void, Color3ubv, (const GLubyte * v)) \
X(void, Color3ui, (GLuint red, GLuint green, GLuint blue)) \
X(void, Color3uiv, (const GLuint * v)) \
X(void, Color3us, (GLushort red, GLushort green, GLushort blue)) \
X(void, Color3usv, (const GLushort * v)) \
X(void, Color4b, (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha)) \
X(void, Color4bv, (const GLbyte * v)) \
X(void, Color4d, (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha)) \
X(void, Color4dv, (const GLdouble * v)) \
X(void, Color4f, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)) \
X(void, Color4fv, (const GLfloat * v)) \
X(void, Color4i, (GLint red, GLint green, GLint blue, GLint alpha)) \
X(void, Color4iv, (const GLint * v)) \
X(void, Color4s, (GLshort red, GLshort green, GLshort blue, GLshort alpha)) \
X(void, Color4sv, (const GLshort * v)) \
X(void, Color4ub, (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha)) \
X(void, Color4ubv, (const GLubyte * v)) \
X(void, Color4ui, (GLuint red, GLuint green, GLuint blue, GLuint alpha)) \
X(void, Color4uiv, (const GLuint * v)) \
X(void, Color4us, (GLushort red, GLushort green, GLushort blue, GLushort alpha)) \
X(void, Color4usv, (const GLushort * v)) \
X(void, Color4x, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)) \
X(void, ColorMaterial, (GLenum face, GLenum mode)) \
X(void, ColorPointer, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, CopyPixels, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type)) \
X(void, DeleteLists, (GLuint list, GLsizei range)) \
X(void, DepthRange, (GLdouble n, GLdouble f)) \
X(void, DepthRangex, (GLfixed n, GLfixed f)) \
X(void, DisableClientState, (GLenum array)) \
X(void, DrawBuffer, (GLenum buf)) \
X(void, DrawPixels, (GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels)) \
X(void, EdgeFlag, (GLboolean flag)) \
X(void, EdgeFlagv, (const GLboolean * flag)) \
X(void, EnableClientState, (GLenum array)) \
X(void, End, ()) \
X(void, EndList, ()) \
X(void, EvalCoord1d, (GLdouble u)) \
X(void, EvalCoord1dv, (const GLdouble * u)) \
X(void, EvalCoord1f, (GLfloat u)) \
X(void, EvalCoord1fv, (const GLfloat * u)) \
X(void, EvalCoord2d, (GLdouble u, GLdouble v)) \
X(void, EvalCoord2dv, (const GLdouble * u)) \
X(void, EvalCoord2f, (GLfloat u, GLfloat v)) \
X(void, EvalCoord2fv, (const GLfloat * u)) \
X(void, EvalMesh1, (GLenum mode, GLint i1, GLint i2)) \
X(void, EvalMesh2, (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2)) \
X(void, EvalPoint1, (GLint i)) \
X(void, EvalPoint2, (GLint i, GLint j)) \
X(void, FeedbackBuffer, (GLsizei size, GLenum type, GLfloat * buffer)) \
X(void, Fogf, (GLenum pname, GLfloat param)) \
X(void, Fogfv, (GLenum pname, const GLfloat * params)) \
X(void, Fogi, (GLenum pname, GLint param)) \
X(void, Fogiv, (GLenum pname, const GLint * params)) \
X(void, Fogx, (GLenum pname, GLfixed param)) \
X(void, Fogxv, (GLenum pname, const GLfixed * param)) \
X(void, Frustum, (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)) \
X(void, Frustumf, (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)) \
X(void, Frustumx, (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f)) \
X(GLuint, GenLists, (GLsizei range)) \
X(void, GetClipPlane, (GLenum plane, GLdouble * equation)) \
X(void, GetClipPlanef, (GLenum plane, GLfloat * equation)) \
X(void, GetClipPlanex, (GLenum plane, GLfixed * equation)) \
X(void, GetDoublev, (GLenum pname, GLdouble * data)) \
X(void, GetFixedv, (GLenum pname, GLfixed * params)) \
X(void, GetLightfv, (GLenum light, GLenum pname, GLfloat * params)) \
X(void, GetLightiv, (GLenum light, GLenum pname, GLint * params)) \
X(void, GetLightxv, (GLenum light, GLenum pname, GLfixed * params)) \
X(void, GetMapdv, (GLenum target, GLenum query, GLdouble * v)) \
X(void, GetMapfv, (GLenum target, GLenum query, GLfloat * v)) \
X(void, GetMapiv, (GLenum target, GLenum query, GLint * v)) \
X(void, GetMaterialfv, (GLenum face, GLenum pname, GLfloat * params)) \
X(void, GetMaterialiv, (GLenum face, GLenum pname, GLint * params)) \
X(void, GetMaterialxv, (GLenum face, GLenum pname, GLfixed * params)) \
X(void, GetPixelMapfv, (GLenum map, GLfloat * values)) \
X(void, GetPixelMapuiv, (GLenum map, GLuint * values)) \
X(void, GetPixelMapusv, (GLenum map, GLushort * values)) \
X(void, GetPolygonStipple, (GLubyte * mask)) \
X(void, GetTexEnvfv, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetTexEnviv, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetTexEnvxv, (GLenum target, GLenum pname, GLfixed * params)) \
X(void, GetTexGendv, (GLenum coord, GLenum pname, GLdouble * params)) \
X(void, GetTexGenfv, (GLenum coord, GLenum pname, GLfloat * params)) \
X(void, GetTexGeniv, (GLenum coord, GLenum pname, GLint * params)) \
X(void, GetTexImage, (GLenum target, GLint level, GLenum format, GLenum type, void * pixels)) \
X(void, GetTexParameterxv, (GLenum target, GLenum pname, GLfixed * params)) \
X(void, IndexMask, (GLuint mask)) \
X(void, Indexd, (GLdouble c)) \
X(void, Indexdv, (const GLdouble * c)) \
X(void, Indexf, (GLfloat c)) \
X(void, Indexfv, (const GLfloat * c)) \
X(void, Indexi, (GLint c)) \
X(void, Indexiv, (const GLint * c)) \
X(void, Indexs, (GLshort c)) \
X(void, Indexsv, (const GLshort * c)) \
X(void, InitNames, ()) \
X(GLboolean, IsList, (GLuint list)) \
X(void, LightModelf, (GLenum pname, GLfloat param)) \
X(void, LightModelfv, (GLenum pname, const GLfloat * params)) \
X(void, LightModeli, (GLenum pname, GLint param)) \
X(void, LightModeliv, (GLenum pname, const GLint * params)) \
X(void, LightModelx, (GLenum pname, GLfixed param)) \
X(void, LightModelxv, (GLenum pname, const GLfixed * param)) \
X(void, Lightf, (GLenum light, GLenum pname, GLfloat param)) \
X(void, Lightfv, (GLenum light, GLenum pname, const GLfloat * params)) \
X(void, Lighti, (GLenum light, GLenum pname, GLint param)) \
X(void, Lightiv, (GLenum light, GLenum pname, const GLint * params)) \
X(void, Lightx, (GLenum light, GLenum pname, GLfixed param)) \
X(void, Lightxv, (GLenum light, GLenum pname, const GLfixed * params)) \
X(void, LineStipple, (GLint factor, GLushort pattern)) \
X(void, LineWidthx, (GLfixed width)) \
X(void, ListBase, (GLuint base)) \
X(void, LoadIdentity, ()) \
X(void, LoadMatrixd, (const GLdouble * m)) \
X(void, LoadMatrixf, (const GLfloat * m)) \
X(void, LoadMatrixx, (const GLfixed * m)) \
X(void, LoadName, (GLuint name)) \
X(void, LogicOp, (GLenum opcode)) \
X(void, Map1d, (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble * points)) \
X(void, Map1f, (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat * points)) \
X(void, Map2d, (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble * points)) \
X(void, Map2f, (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat * points)) \
X(void, MapGrid1d, (GLint un, GLdouble u1, GLdouble u2)) \
X(void, MapGrid1f, (GLint un, GLfloat u1, GLfloat u2)) \
X(void, MapGrid2d, (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2)) \
X(void, MapGrid2f, (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2)) \
X(void, Materialf, (GLenum face, GLenum pname, GLfloat param)) \
X(void, Materialfv, (GLenum face, GLenum pname, const GLfloat * params)) \
X(void, Materiali, (GLenum face, GLenum pname, GLint param)) \
X(void, Materialiv, (GLenum face, GLenum pname, const GLint * params)) \
X(void, Materialx, (GLenum face, GLenum pname, GLfixed param)) \
X(void, Materialxv, (GLenum face, GLenum pname, const GLfixed * param)) \
X(void, MatrixMode, (GLenum mode)) \
X(void, MultMatrixd, (const GLdouble * m)) \
X(void, MultMatrixf, (const GLfloat * m)) \
X(void, MultMatrixx, (const GLfixed * m)) \
X(void, MultiTexCoord4f, (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)) \
X(void, MultiTexCoord4x, (GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q)) \
X(void, NewList, (GLuint list, GLenum mode)) \
X(void, Normal3b, (GLbyte nx, GLbyte ny, GLbyte nz)) \
X(void, Normal3bv, (const GLbyte * v)) \
X(void, Normal3d, (GLdouble nx, GLdouble ny, GLdouble nz)) \
X(void, Normal3dv, (const GLdouble * v)) \
X(void, Normal3f, (GLfloat nx, GLfloat ny, GLfloat nz)) \
X(void, Normal3fv, (const GLfloat * v)) \
X(void, Normal3i, (GLint nx, GLint ny, GLint nz)) \
X(void, Normal3iv, (const GLint * v)) \
X(void, Normal3s, (GLshort nx, GLshort ny, GLshort nz)) \
X(void, Normal3sv, (const GLshort * v)) \
X(void, Normal3x, (GLfixed nx, GLfixed ny, GLfixed nz)) \
X(void, NormalPointer, (GLenum type, GLsizei stride, const void * pointer)) \
X(void, Ortho, (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)) \
X(void, Orthof, (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)) \
X(void, Orthox, (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f)) \
X(void, PassThrough, (GLfloat token)) \
X(void, PixelMapfv, (GLenum map, GLsizei mapsize, const GLfloat * values)) \
X(void, PixelMapuiv, (GLenum map, GLsizei mapsize, const GLuint * values)) \
X(void, PixelMapusv, (GLenum map, GLsizei mapsize, const GLushort * values)) \
X(void, PixelStoref, (GLenum pname, GLfloat param)) \
X(void, PixelTransferf, (GLenum pname, GLfloat param)) \
X(void, PixelTransferi, (GLenum pname, GLint param)) \
X(void, PixelZoom, (GLfloat xfactor, GLfloat yfactor)) \
X(void, PointParameterf, (GLenum pname, GLfloat param)) \
X(void, PointParameterfv, (GLenum pname, const GLfloat * params)) \
X(void, PointParameterx, (GLenum pname, GLfixed param)) \
X(void, PointParameterxv, (GLenum pname, const GLfixed * params)) \
X(void, PointSize, (GLfloat size)) \
X(void, PointSizex, (GLfixed size)) \
X(void, PolygonMode, (GLenum face, GLenum mode)) \
X(void, PolygonOffsetx, (GLfixed factor, GLfixed units)) \
X(void, PolygonStipple, (const GLubyte * mask)) \
X(void, PopAttrib, ()) \
X(void, PopMatrix, ()) \
X(void, PopName, ()) \
X(void, PushAttrib, (GLbitfield mask)) \
X(void, PushMatrix, ()) \
X(void, PushName, (GLuint name)) \
X(void, RasterPos2d, (GLdouble x, GLdouble y)) \
X(void, RasterPos2dv, (const GLdouble * v)) \
X(void, RasterPos2f, (GLfloat x, GLfloat y)) \
X(void, RasterPos2fv, (const GLfloat * v)) \
X(void, RasterPos2i, (GLint x, GLint y)) \
X(void, RasterPos2iv, (const GLint * v)) \
X(void, RasterPos2s, (GLshort x, GLshort y)) \
X(void, RasterPos2sv, (const GLshort * v)) \
X(void, RasterPos3d, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, RasterPos3dv, (const GLdouble * v)) \
X(void, RasterPos3f, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, RasterPos3fv, (const GLfloat * v)) \
X(void, RasterPos3i, (GLint x, GLint y, GLint z)) \
X(void, RasterPos3iv, (const GLint * v)) \
X(void, RasterPos3s, (GLshort x, GLshort y, GLshort z)) \
X(void, RasterPos3sv, (const GLshort * v)) \
X(void, RasterPos4d, (GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, RasterPos4dv, (const GLdouble * v)) \
X(void, RasterPos4f, (GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, RasterPos4fv, (const GLfloat * v)) \
X(void, RasterPos4i, (GLint x, GLint y, GLint z, GLint w)) \
X(void, RasterPos4iv, (const GLint * v)) \
X(void, RasterPos4s, (GLshort x, GLshort y, GLshort z, GLshort w)) \
X(void, RasterPos4sv, (const GLshort * v)) \
X(void, Rectd, (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2)) \
X(void, Rectdv, (const GLdouble * v1, const GLdouble * v2)) \
X(void, Rectf, (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)) \
X(void, Rectfv, (const GLfloat * v1, const GLfloat * v2)) \
X(void, Recti, (GLint x1, GLint y1, GLint x2, GLint y2)) \
X(void, Rectiv, (const GLint * v1, const GLint * v2)) \
X(void, Rects, (GLshort x1, GLshort y1, GLshort x2, GLshort y2)) \
X(void, Rectsv, (const GLshort * v1, const GLshort * v2)) \
X(GLint, RenderMode, (GLenum mode)) \
X(void, Rotated, (GLdouble angle, GLdouble x, GLdouble y, GLdouble z)) \
X(void, Rotatef, (GLfloat angle, GLfloat x, GLfloat y, GLfloat z)) \
X(void, Rotatex, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z)) \
X(void, SampleCoveragex, (GLclampx value, GLboolean invert)) \
X(void, Scaled, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, Scalef, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, Scalex, (GLfixed x, GLfixed y, GLfixed z)) \
X(void, SelectBuffer, (GLsizei size, GLuint * buffer)) \
X(void, ShadeModel, (GLenum mode)) \
X(void, TexCoord1d, (GLdouble s)) \
X(void, TexCoord1dv, (const GLdouble * v)) \
X(void, TexCoord1f, (GLfloat s)) \
X(void, TexCoord1fv, (const GLfloat * v)) \
X(void, TexCoord1i, (GLint s)) \
X(void, TexCoord1iv, (const GLint * v)) \
X(void, TexCoord1s, (GLshort s)) \
X(void, TexCoord1sv, (const GLshort * v)) \
X(void, TexCoord2d, (GLdouble s, GLdouble t)) \
X(void, TexCoord2dv, (const GLdouble * v)) \
X(void, TexCoord2f, (GLfloat s, GLfloat t)) \
X(void, TexCoord2fv, (const GLfloat * v)) \
X(void, TexCoord2i, (GLint s, GLint t)) \
X(void, TexCoord2iv, (const GLint * v)) \
X(void, TexCoord2s, (GLshort s, GLshort t)) \
X(void, TexCoord2sv, (const GLshort * v)) \
X(void, TexCoord3d, (GLdouble s, GLdouble t, GLdouble r)) \
X(void, TexCoord3dv, (const GLdouble * v)) \
X(void, TexCoord3f, (GLfloat s, GLfloat t, GLfloat r)) \
X(void, TexCoord3fv, (const GLfloat * v)) \
X(void, TexCoord3i, (GLint s, GLint t, GLint r)) \
X(void, TexCoord3iv, (const GLint * v)) \
X(void, TexCoord3s, (GLshort s, GLshort t, GLshort r)) \
X(void, TexCoord3sv, (const GLshort * v)) \
X(void, TexCoord4d, (GLdouble s, GLdouble t, GLdouble r, GLdouble q)) \
X(void, TexCoord4dv, (const GLdouble * v)) \
X(void, TexCoord4f, (GLfloat s, GLfloat t, GLfloat r, GLfloat q)) \
X(void, TexCoord4fv, (const GLfloat * v)) \
X(void, TexCoord4i, (GLint s, GLint t, GLint r, GLint q)) \
X(void, TexCoord4iv, (const GLint * v)) \
X(void, TexCoord4s, (GLshort s, GLshort t, GLshort r, GLshort q)) \
X(void, TexCoord4sv, (const GLshort * v)) \
X(void, TexCoordPointer, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, TexEnvf, (GLenum target, GLenum pname, GLfloat param)) \
X(void, TexEnvfv, (GLenum target, GLenum pname, const GLfloat * params)) \
X(void, TexEnvi, (GLenum target, GLenum pname, GLint param)) \
X(void, TexEnviv, (GLenum target, GLenum pname, const GLint * params)) \
X(void, TexEnvx, (GLenum target, GLenum pname, GLfixed param)) \
X(void, TexEnvxv, (GLenum target, GLenum pname, const GLfixed * params)) \
X(void, TexGend, (GLenum coord, GLenum pname, GLdouble param)) \
X(void, TexGendv, (GLenum coord, GLenum pname, const GLdouble * params)) \
X(void, TexGenf, (GLenum coord, GLenum pname, GLfloat param)) \
X(void, TexGenfv, (GLenum coord, GLenum pname, const GLfloat * params)) \
X(void, TexGeni, (GLenum coord, GLenum pname, GLint param)) \
X(void, TexGeniv, (GLenum coord, GLenum pname, const GLint * params)) \
X(void, TexImage1D, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, TexParameterx, (GLenum target, GLenum pname, GLfixed param)) \
X(void, TexParameterxv, (GLenum target, GLenum pname, const GLfixed * params)) \
X(void, Translated, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, Translatef, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, Translatex, (GLfixed x, GLfixed y, GLfixed z)) \
X(void, Vertex2d, (GLdouble x, GLdouble y)) \
X(void, Vertex2dv, (const GLdouble * v)) \
X(void, Vertex2f, (GLfloat x, GLfloat y)) \
X(void, Vertex2fv, (const GLfloat * v)) \
X(void, Vertex2i, (GLint x, GLint y)) \
X(void, Vertex2iv, (const GLint * v)) \
X(void, Vertex2s, (GLshort x, GLshort y)) \
X(void, Vertex2sv, (const GLshort * v)) \
X(void, Vertex3d, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, Vertex3dv, (const GLdouble * v)) \
X(void, Vertex3f, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, Vertex3fv, (const GLfloat * v)) \
X(void, Vertex3i, (GLint x, GLint y, GLint z)) \
X(void, Vertex3iv, (const GLint * v)) \
X(void, Vertex3s, (GLshort x, GLshort y, GLshort z)) \
X(void, Vertex3sv, (const GLshort * v)) \
X(void, Vertex4d, (GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, Vertex4dv, (const GLdouble * v)) \
X(void, Vertex4f, (GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, Vertex4fv, (const GLfloat * v)) \
X(void, Vertex4i, (GLint x, GLint y, GLint z, GLint w)) \
X(void, Vertex4iv, (const GLint * v)) \
X(void, Vertex4s, (GLshort x, GLshort y, GLshort z, GLshort w)) \
X(void, Vertex4sv, (const GLshort * v)) \
X(void, VertexPointer, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
VER_END(1_0) \
VER_START(1_1) \
X(GLboolean, AreTexturesResident, (GLsizei n, const GLuint * textures, GLboolean * residences)) \
X(void, ArrayElement, (GLint i)) \
X(void, CopyTexImage1D, (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)) \
X(void, CopyTexSubImage1D, (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)) \
X(void, EdgeFlagPointer, (GLsizei stride, const void * pointer)) \
X(void, IndexPointer, (GLenum type, GLsizei stride, const void * pointer)) \
X(void, Indexub, (GLubyte c)) \
X(void, Indexubv, (const GLubyte * c)) \
X(void, InterleavedArrays, (GLenum format, GLsizei stride, const void * pointer)) \
X(void, PopClientAttrib, ()) \
X(void, PrioritizeTextures, (GLsizei n, const GLuint * textures, const GLfloat * priorities)) \
X(void, PushClientAttrib, (GLbitfield mask)) \
X(void, TexSubImage1D, (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels)) \
VER_END(1_1) \
VER_START(1_3) \
X(void, CompressedTexImage1D, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * data)) \
X(void, CompressedTexSubImage1D, (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data)) \
X(void, GetCompressedTexImage, (GLenum target, GLint level, void * img)) \
X(void, LoadTransposeMatrixd, (const GLdouble * m)) \
X(void, LoadTransposeMatrixf, (const GLfloat * m)) \
X(void, MultTransposeMatrixd, (const GLdouble * m)) \
X(void, MultTransposeMatrixf, (const GLfloat * m)) \
X(void, MultiTexCoord1d, (GLenum target, GLdouble s)) \
X(void, MultiTexCoord1dv, (GLenum target, const GLdouble * v)) \
X(void, MultiTexCoord1f, (GLenum target, GLfloat s)) \
X(void, MultiTexCoord1fv, (GLenum target, const GLfloat * v)) \
X(void, MultiTexCoord1i, (GLenum target, GLint s)) \
X(void, MultiTexCoord1iv, (GLenum target, const GLint * v)) \
X(void, MultiTexCoord1s, (GLenum target, GLshort s)) \
X(void, MultiTexCoord1sv, (GLenum target, const GLshort * v)) \
X(void, MultiTexCoord2d, (GLenum target, GLdouble s, GLdouble t)) \
X(void, MultiTexCoord2dv, (GLenum target, const GLdouble * v)) \
X(void, MultiTexCoord2f, (GLenum target, GLfloat s, GLfloat t)) \
X(void, MultiTexCoord2fv, (GLenum target, const GLfloat * v)) \
X(void, MultiTexCoord2i, (GLenum target, GLint s, GLint t)) \
X(void, MultiTexCoord2iv, (GLenum target, const GLint * v)) \
X(void, MultiTexCoord2s, (GLenum target, GLshort s, GLshort t)) \
X(void, MultiTexCoord2sv, (GLenum target, const GLshort * v)) \
X(void, MultiTexCoord3d, (GLenum target, GLdouble s, GLdouble t, GLdouble r)) \
X(void, MultiTexCoord3dv, (GLenum target, const GLdouble * v)) \
X(void, MultiTexCoord3f, (GLenum target, GLfloat s, GLfloat t, GLfloat r)) \
X(void, MultiTexCoord3fv, (GLenum target, const GLfloat * v)) \
X(void, MultiTexCoord3i, (GLenum target, GLint s, GLint t, GLint r)) \
X(void, MultiTexCoord3iv, (GLenum target, const GLint * v)) \
X(void, MultiTexCoord3s, (GLenum target, GLshort s, GLshort t, GLshort r)) \
X(void, MultiTexCoord3sv, (GLenum target, const GLshort * v)) \
X(void, MultiTexCoord4d, (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)) \
X(void, MultiTexCoord4dv, (GLenum target, const GLdouble * v)) \
X(void, MultiTexCoord4fv, (GLenum target, const GLfloat * v)) \
X(void, MultiTexCoord4i, (GLenum target, GLint s, GLint t, GLint r, GLint q)) \
X(void, MultiTexCoord4iv, (GLenum target, const GLint * v)) \
X(void, MultiTexCoord4s, (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)) \
X(void, MultiTexCoord4sv, (GLenum target, const GLshort * v)) \
VER_END(1_3) \
VER_START(1_4) \
X(void, FogCoordPointer, (GLenum type, GLsizei stride, const void * pointer)) \
X(void, FogCoordd, (GLdouble coord)) \
X(void, FogCoorddv, (const GLdouble * coord)) \
X(void, FogCoordf, (GLfloat coord)) \
X(void, FogCoordfv, (const GLfloat * coord)) \
X(void, MultiDrawArrays, (GLenum mode, const GLint * first, const GLsizei * count, GLsizei drawcount)) \
X(void, MultiDrawElements, (GLenum mode, const GLsizei * count, GLenum type, const void *const* indices, GLsizei drawcount)) \
X(void, PointParameteri, (GLenum pname, GLint param)) \
X(void, PointParameteriv, (GLenum pname, const GLint * params)) \
X(void, SecondaryColor3b, (GLbyte red, GLbyte green, GLbyte blue)) \
X(void, SecondaryColor3bv, (const GLbyte * v)) \
X(void, SecondaryColor3d, (GLdouble red, GLdouble green, GLdouble blue)) \
X(void, SecondaryColor3dv, (const GLdouble * v)) \
X(void, SecondaryColor3f, (GLfloat red, GLfloat green, GLfloat blue)) \
X(void, SecondaryColor3fv, (const GLfloat * v)) \
X(void, SecondaryColor3i, (GLint red, GLint green, GLint blue)) \
X(void, SecondaryColor3iv, (const GLint * v)) \
X(void, SecondaryColor3s, (GLshort red, GLshort green, GLshort blue)) \
X(void, SecondaryColor3sv, (const GLshort * v)) \
X(void, SecondaryColor3ub, (GLubyte red, GLubyte green, GLubyte blue)) \
X(void, SecondaryColor3ubv, (const GLubyte * v)) \
X(void, SecondaryColor3ui, (GLuint red, GLuint green, GLuint blue)) \
X(void, SecondaryColor3uiv, (const GLuint * v)) \
X(void, SecondaryColor3us, (GLushort red, GLushort green, GLushort blue)) \
X(void, SecondaryColor3usv, (const GLushort * v)) \
X(void, SecondaryColorPointer, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, WindowPos2d, (GLdouble x, GLdouble y)) \
X(void, WindowPos2dv, (const GLdouble * v)) \
X(void, WindowPos2f, (GLfloat x, GLfloat y)) \
X(void, WindowPos2fv, (const GLfloat * v)) \
X(void, WindowPos2i, (GLint x, GLint y)) \
X(void, WindowPos2iv, (const GLint * v)) \
X(void, WindowPos2s, (GLshort x, GLshort y)) \
X(void, WindowPos2sv, (const GLshort * v)) \
X(void, WindowPos3d, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, WindowPos3dv, (const GLdouble * v)) \
X(void, WindowPos3f, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, WindowPos3fv, (const GLfloat * v)) \
X(void, WindowPos3i, (GLint x, GLint y, GLint z)) \
X(void, WindowPos3iv, (const GLint * v)) \
X(void, WindowPos3s, (GLshort x, GLshort y, GLshort z)) \
X(void, WindowPos3sv, (const GLshort * v)) \
VER_END(1_4) \
VER_START(1_5) \
X(void, GetBufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, void * data)) \
X(void, GetQueryObjectiv, (GLuint id, GLenum pname, GLint * params)) \
X(void *, MapBuffer, (GLenum target, GLenum access)) \
VER_END(1_5) \
VER_START(2_0) \
X(void, ActiveTexture, (GLenum texture)) \
X(void, AttachShader, (GLuint program, GLuint shader)) \
X(void, BindAttribLocation, (GLuint program, GLuint index, const GLchar * name)) \
X(void, BindBuffer, (GLenum target, GLuint buffer)) \
X(void, BindFramebuffer, (GLenum target, GLuint framebuffer)) \
X(void, BindRenderbuffer, (GLenum target, GLuint renderbuffer)) \
X(void, BindTexture, (GLenum target, GLuint texture)) \
X(void, BlendColor, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)) \
X(void, BlendEquation, (GLenum mode)) \
X(void, BlendEquationSeparate, (GLenum modeRGB, GLenum modeAlpha)) \
X(void, BlendFunc, (GLenum sfactor, GLenum dfactor)) \
X(void, BlendFuncSeparate, (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)) \
X(void, BufferData, (GLenum target, GLsizeiptr size, const void * data, GLenum usage)) \
X(void, BufferSubData, (GLenum target, GLintptr offset, GLsizeiptr size, const void * data)) \
X(GLenum, CheckFramebufferStatus, (GLenum target)) \
X(void, Clear, (GLbitfield mask)) \
X(void, ClearColor, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)) \
X(void, ClearDepthf, (GLfloat d)) \
X(void, ClearStencil, (GLint s)) \
X(void, ColorMask, (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)) \
X(void, CompileShader, (GLuint shader)) \
X(void, CompressedTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data)) \
X(void, CompressedTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data)) \
X(void, CopyTexImage2D, (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)) \
X(void, CopyTexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(GLuint, CreateProgram, ()) \
X(GLuint, CreateShader, (GLenum type)) \
X(void, CullFace, (GLenum mode)) \
X(void, DeleteBuffers, (GLsizei n, const GLuint * buffers)) \
X(void, DeleteFramebuffers, (GLsizei n, const GLuint * framebuffers)) \
X(void, DeleteProgram, (GLuint program)) \
X(void, DeleteRenderbuffers, (GLsizei n, const GLuint * renderbuffers)) \
X(void, DeleteShader, (GLuint shader)) \
X(void, DeleteTextures, (GLsizei n, const GLuint * textures)) \
X(void, DepthFunc, (GLenum func)) \
X(void, DepthMask, (GLboolean flag)) \
X(void, DepthRangef, (GLfloat n, GLfloat f)) \
X(void, DetachShader, (GLuint program, GLuint shader)) \
X(void, Disable, (GLenum cap)) \
X(void, DisableVertexAttribArray, (GLuint index)) \
X(void, DrawArrays, (GLenum mode, GLint first, GLsizei count)) \
X(void, DrawElements, (GLenum mode, GLsizei count, GLenum type, const void * indices)) \
X(void, DrawRangeElements, (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices)) \
X(void, Enable, (GLenum cap)) \
X(void, EnableVertexAttribArray, (GLuint index)) \
X(void, Finish, ()) \
X(void, Flush, ()) \
X(void, FramebufferRenderbuffer, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)) \
X(void, FramebufferTexture2D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)) \
X(void, FrontFace, (GLenum mode)) \
X(void, GenBuffers, (GLsizei n, GLuint * buffers)) \
X(void, GenFramebuffers, (GLsizei n, GLuint * framebuffers)) \
X(void, GenRenderbuffers, (GLsizei n, GLuint * renderbuffers)) \
X(void, GenTextures, (GLsizei n, GLuint * textures)) \
X(void, GenerateMipmap, (GLenum target)) \
X(void, GetActiveAttrib, (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name)) \
X(void, GetActiveUniform, (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLint * size, GLenum * type, GLchar * name)) \
X(void, GetAttachedShaders, (GLuint program, GLsizei maxCount, GLsizei * count, GLuint * shaders)) \
X(GLint, GetAttribLocation, (GLuint program, const GLchar * name)) \
X(void, GetBooleanv, (GLenum pname, GLboolean * data)) \
X(void, GetBufferParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(GLenum, GetError, ()) \
X(void, GetFloatv, (GLenum pname, GLfloat * data)) \
X(void, GetFramebufferAttachmentParameteriv, (GLenum target, GLenum attachment, GLenum pname, GLint * params)) \
X(GLenum, GetGraphicsResetStatus, ()) \
X(void, GetIntegerv, (GLenum pname, GLint * data)) \
X(void, GetProgramInfoLog, (GLuint program, GLsizei bufSize, GLsizei * length, GLchar * infoLog)) \
X(void, GetProgramiv, (GLuint program, GLenum pname, GLint * params)) \
X(void, GetRenderbufferParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetShaderInfoLog, (GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * infoLog)) \
X(void, GetShaderPrecisionFormat, (GLenum shadertype, GLenum precisiontype, GLint * range, GLint * precision)) \
X(void, GetShaderSource, (GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source)) \
X(void, GetShaderiv, (GLuint shader, GLenum pname, GLint * params)) \
X(const GLubyte *, GetString, (GLenum name)) \
X(void, GetTexParameterfv, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetTexParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(GLint, GetUniformLocation, (GLuint program, const GLchar * name)) \
X(void, GetUniformfv, (GLuint program, GLint location, GLfloat * params)) \
X(void, GetUniformiv, (GLuint program, GLint location, GLint * params)) \
X(void, GetVertexAttribPointerv, (GLuint index, GLenum pname, void ** pointer)) \
X(void, GetVertexAttribdv, (GLuint index, GLenum pname, GLdouble * params)) \
X(void, GetVertexAttribfv, (GLuint index, GLenum pname, GLfloat * params)) \
X(void, GetVertexAttribiv, (GLuint index, GLenum pname, GLint * params)) \
X(void, GetnUniformfv, (GLuint program, GLint location, GLsizei bufSize, GLfloat * params)) \
X(void, GetnUniformiv, (GLuint program, GLint location, GLsizei bufSize, GLint * params)) \
X(void, Hint, (GLenum target, GLenum mode)) \
X(GLboolean, IsBuffer, (GLuint buffer)) \
X(GLboolean, IsEnabled, (GLenum cap)) \
X(GLboolean, IsFramebuffer, (GLuint framebuffer)) \
X(GLboolean, IsProgram, (GLuint program)) \
X(GLboolean, IsRenderbuffer, (GLuint renderbuffer)) \
X(GLboolean, IsShader, (GLuint shader)) \
X(GLboolean, IsTexture, (GLuint texture)) \
X(void, LineWidth, (GLfloat width)) \
X(void, LinkProgram, (GLuint program)) \
X(void, PixelStorei, (GLenum pname, GLint param)) \
X(void, PolygonOffset, (GLfloat factor, GLfloat units)) \
X(void, ProgramBinary, (GLuint program, GLenum binaryFormat, const void * binary, GLsizei length)) \
X(void, ReadPixels, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void * pixels)) \
X(void, ReadnPixels, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void * data)) \
X(void, ReleaseShaderCompiler, ()) \
X(void, RenderbufferStorage, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, SampleCoverage, (GLfloat value, GLboolean invert)) \
X(void, Scissor, (GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, ShaderBinary, (GLsizei count, const GLuint * shaders, GLenum binaryFormat, const void * binary, GLsizei length)) \
X(void, ShaderSource, (GLuint shader, GLsizei count, const GLchar *const* string, const GLint * length)) \
X(void, StencilFunc, (GLenum func, GLint ref, GLuint mask)) \
X(void, StencilFuncSeparate, (GLenum face, GLenum func, GLint ref, GLuint mask)) \
X(void, StencilMask, (GLuint mask)) \
X(void, StencilMaskSeparate, (GLenum face, GLuint mask)) \
X(void, StencilOp, (GLenum fail, GLenum zfail, GLenum zpass)) \
X(void, StencilOpSeparate, (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)) \
X(void, TexImage2D, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, TexParameterf, (GLenum target, GLenum pname, GLfloat param)) \
X(void, TexParameterfv, (GLenum target, GLenum pname, const GLfloat * params)) \
X(void, TexParameteri, (GLenum target, GLenum pname, GLint param)) \
X(void, TexParameteriv, (GLenum target, GLenum pname, const GLint * params)) \
X(void, TexStorage2D, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, TexSubImage2D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels)) \
X(void, Uniform1f, (GLint location, GLfloat v0)) \
X(void, Uniform1fv, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, Uniform1i, (GLint location, GLint v0)) \
X(void, Uniform1iv, (GLint location, GLsizei count, const GLint * value)) \
X(void, Uniform2f, (GLint location, GLfloat v0, GLfloat v1)) \
X(void, Uniform2fv, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, Uniform2i, (GLint location, GLint v0, GLint v1)) \
X(void, Uniform2iv, (GLint location, GLsizei count, const GLint * value)) \
X(void, Uniform3f, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2)) \
X(void, Uniform3fv, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, Uniform3i, (GLint location, GLint v0, GLint v1, GLint v2)) \
X(void, Uniform3iv, (GLint location, GLsizei count, const GLint * value)) \
X(void, Uniform4f, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)) \
X(void, Uniform4fv, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, Uniform4i, (GLint location, GLint v0, GLint v1, GLint v2, GLint v3)) \
X(void, Uniform4iv, (GLint location, GLsizei count, const GLint * value)) \
X(void, UniformMatrix2fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix3fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UseProgram, (GLuint program)) \
X(void, ValidateProgram, (GLuint program)) \
X(void, VertexAttrib1d, (GLuint index, GLdouble x)) \
X(void, VertexAttrib1dv, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib1f, (GLuint index, GLfloat x)) \
X(void, VertexAttrib1fv, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib1s, (GLuint index, GLshort x)) \
X(void, VertexAttrib1sv, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib2d, (GLuint index, GLdouble x, GLdouble y)) \
X(void, VertexAttrib2dv, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib2f, (GLuint index, GLfloat x, GLfloat y)) \
X(void, VertexAttrib2fv, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib2s, (GLuint index, GLshort x, GLshort y)) \
X(void, VertexAttrib2sv, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib3d, (GLuint index, GLdouble x, GLdouble y, GLdouble z)) \
X(void, VertexAttrib3dv, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib3f, (GLuint index, GLfloat x, GLfloat y, GLfloat z)) \
X(void, VertexAttrib3fv, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib3s, (GLuint index, GLshort x, GLshort y, GLshort z)) \
X(void, VertexAttrib3sv, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib4Nbv, (GLuint index, const GLbyte * v)) \
X(void, VertexAttrib4Niv, (GLuint index, const GLint * v)) \
X(void, VertexAttrib4Nsv, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib4Nub, (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w)) \
X(void, VertexAttrib4Nubv, (GLuint index, const GLubyte * v)) \
X(void, VertexAttrib4Nuiv, (GLuint index, const GLuint * v)) \
X(void, VertexAttrib4Nusv, (GLuint index, const GLushort * v)) \
X(void, VertexAttrib4bv, (GLuint index, const GLbyte * v)) \
X(void, VertexAttrib4d, (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, VertexAttrib4dv, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib4f, (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, VertexAttrib4fv, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib4iv, (GLuint index, const GLint * v)) \
X(void, VertexAttrib4s, (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w)) \
X(void, VertexAttrib4sv, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib4ubv, (GLuint index, const GLubyte * v)) \
X(void, VertexAttrib4uiv, (GLuint index, const GLuint * v)) \
X(void, VertexAttrib4usv, (GLuint index, const GLushort * v)) \
X(void, VertexAttribPointer, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer)) \
X(void, Viewport, (GLint x, GLint y, GLsizei width, GLsizei height)) \
VER_END(2_0) \
VER_START(3_0) \
X(void, BeginConditionalRender, (GLuint id, GLenum mode)) \
X(void, BeginQuery, (GLenum target, GLuint id)) \
X(void, BeginTransformFeedback, (GLenum primitiveMode)) \
X(void, BindBufferBase, (GLenum target, GLuint index, GLuint buffer)) \
X(void, BindBufferRange, (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
X(void, BindFragDataLocation, (GLuint program, GLuint color, const GLchar * name)) \
X(void, BindSampler, (GLuint unit, GLuint sampler)) \
X(void, BindTransformFeedback, (GLenum target, GLuint id)) \
X(void, BindVertexArray, (GLuint array)) \
X(void, BlitFramebuffer, (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)) \
X(void, ClampColor, (GLenum target, GLenum clamp)) \
X(void, ClearBufferfi, (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)) \
X(void, ClearBufferfv, (GLenum buffer, GLint drawbuffer, const GLfloat * value)) \
X(void, ClearBufferiv, (GLenum buffer, GLint drawbuffer, const GLint * value)) \
X(void, ClearBufferuiv, (GLenum buffer, GLint drawbuffer, const GLuint * value)) \
X(GLenum, ClientWaitSync, (GLsync sync, GLbitfield flags, GLuint64 timeout)) \
X(void, CompressedTexImage3D, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * data)) \
X(void, CompressedTexSubImage3D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data)) \
X(void, CopyBufferSubData, (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)) \
X(void, CopyTexSubImage3D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, DeleteQueries, (GLsizei n, const GLuint * ids)) \
X(void, DeleteSamplers, (GLsizei count, const GLuint * samplers)) \
X(void, DeleteSync, (GLsync sync)) \
X(void, DeleteTransformFeedbacks, (GLsizei n, const GLuint * ids)) \
X(void, DeleteVertexArrays, (GLsizei n, const GLuint * arrays)) \
X(void, DrawArraysInstanced, (GLenum mode, GLint first, GLsizei count, GLsizei instancecount)) \
X(void, DrawBuffers, (GLsizei n, const GLenum * bufs)) \
X(void, DrawElementsInstanced, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount)) \
X(void, EndConditionalRender, ()) \
X(void, EndQuery, (GLenum target)) \
X(void, EndTransformFeedback, ()) \
X(GLsync, FenceSync, (GLenum condition, GLbitfield flags)) \
X(void, FlushMappedBufferRange, (GLenum target, GLintptr offset, GLsizeiptr length)) \
X(void, FramebufferTexture1D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)) \
X(void, FramebufferTexture3D, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)) \
X(void, FramebufferTextureLayer, (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)) \
X(void, GenQueries, (GLsizei n, GLuint * ids)) \
X(void, GenSamplers, (GLsizei count, GLuint * samplers)) \
X(void, GenTransformFeedbacks, (GLsizei n, GLuint * ids)) \
X(void, GenVertexArrays, (GLsizei n, GLuint * arrays)) \
X(void, GetActiveUniformBlockName, (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformBlockName)) \
X(void, GetActiveUniformBlockiv, (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint * params)) \
X(void, GetActiveUniformsiv, (GLuint program, GLsizei uniformCount, const GLuint * uniformIndices, GLenum pname, GLint * params)) \
X(void, GetBufferParameteri64v, (GLenum target, GLenum pname, GLint64 * params)) \
X(void, GetBufferPointerv, (GLenum target, GLenum pname, void ** params)) \
X(GLint, GetFragDataLocation, (GLuint program, const GLchar * name)) \
X(void, GetInteger64i_v, (GLenum target, GLuint index, GLint64 * data)) \
X(void, GetInteger64v, (GLenum pname, GLint64 * data)) \
X(void, GetIntegeri_v, (GLenum target, GLuint index, GLint * data)) \
X(void, GetInternalformativ, (GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint * params)) \
X(void, GetProgramBinary, (GLuint program, GLsizei bufSize, GLsizei * length, GLenum * binaryFormat, void * binary)) \
X(void, GetQueryObjectuiv, (GLuint id, GLenum pname, GLuint * params)) \
X(void, GetQueryiv, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetSamplerParameterfv, (GLuint sampler, GLenum pname, GLfloat * params)) \
X(void, GetSamplerParameteriv, (GLuint sampler, GLenum pname, GLint * params)) \
X(const GLubyte *, GetStringi, (GLenum name, GLuint index)) \
X(void, GetSynciv, (GLsync sync, GLenum pname, GLsizei count, GLsizei * length, GLint * values)) \
X(void, GetTransformFeedbackVarying, (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name)) \
X(GLuint, GetUniformBlockIndex, (GLuint program, const GLchar * uniformBlockName)) \
X(void, GetUniformIndices, (GLuint program, GLsizei uniformCount, const GLchar *const* uniformNames, GLuint * uniformIndices)) \
X(void, GetUniformuiv, (GLuint program, GLint location, GLuint * params)) \
X(void, GetVertexAttribIiv, (GLuint index, GLenum pname, GLint * params)) \
X(void, GetVertexAttribIuiv, (GLuint index, GLenum pname, GLuint * params)) \
X(void, InvalidateFramebuffer, (GLenum target, GLsizei numAttachments, const GLenum * attachments)) \
X(void, InvalidateSubFramebuffer, (GLenum target, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(GLboolean, IsQuery, (GLuint id)) \
X(GLboolean, IsSampler, (GLuint sampler)) \
X(GLboolean, IsSync, (GLsync sync)) \
X(GLboolean, IsTransformFeedback, (GLuint id)) \
X(GLboolean, IsVertexArray, (GLuint array)) \
X(void *, MapBufferRange, (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)) \
X(void, PauseTransformFeedback, ()) \
X(void, ProgramParameteri, (GLuint program, GLenum pname, GLint value)) \
X(void, ReadBuffer, (GLenum src)) \
X(void, RenderbufferStorageMultisample, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, ResumeTransformFeedback, ()) \
X(void, SamplerParameterf, (GLuint sampler, GLenum pname, GLfloat param)) \
X(void, SamplerParameterfv, (GLuint sampler, GLenum pname, const GLfloat * param)) \
X(void, SamplerParameteri, (GLuint sampler, GLenum pname, GLint param)) \
X(void, SamplerParameteriv, (GLuint sampler, GLenum pname, const GLint * param)) \
X(void, TexImage3D, (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, TexStorage3D, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)) \
X(void, TexSubImage3D, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels)) \
X(void, TransformFeedbackVaryings, (GLuint program, GLsizei count, const GLchar *const* varyings, GLenum bufferMode)) \
X(void, Uniform1ui, (GLint location, GLuint v0)) \
X(void, Uniform1uiv, (GLint location, GLsizei count, const GLuint * value)) \
X(void, Uniform2ui, (GLint location, GLuint v0, GLuint v1)) \
X(void, Uniform2uiv, (GLint location, GLsizei count, const GLuint * value)) \
X(void, Uniform3ui, (GLint location, GLuint v0, GLuint v1, GLuint v2)) \
X(void, Uniform3uiv, (GLint location, GLsizei count, const GLuint * value)) \
X(void, Uniform4ui, (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)) \
X(void, Uniform4uiv, (GLint location, GLsizei count, const GLuint * value)) \
X(void, UniformBlockBinding, (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding)) \
X(void, UniformMatrix2x3fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix2x4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix3x2fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix3x4fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix4x2fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix4x3fv, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(GLboolean, UnmapBuffer, (GLenum target)) \
X(void, VertexAttribDivisor, (GLuint index, GLuint divisor)) \
X(void, VertexAttribI1i, (GLuint index, GLint x)) \
X(void, VertexAttribI1iv, (GLuint index, const GLint * v)) \
X(void, VertexAttribI1ui, (GLuint index, GLuint x)) \
X(void, VertexAttribI1uiv, (GLuint index, const GLuint * v)) \
X(void, VertexAttribI2i, (GLuint index, GLint x, GLint y)) \
X(void, VertexAttribI2iv, (GLuint index, const GLint * v)) \
X(void, VertexAttribI2ui, (GLuint index, GLuint x, GLuint y)) \
X(void, VertexAttribI2uiv, (GLuint index, const GLuint * v)) \
X(void, VertexAttribI3i, (GLuint index, GLint x, GLint y, GLint z)) \
X(void, VertexAttribI3iv, (GLuint index, const GLint * v)) \
X(void, VertexAttribI3ui, (GLuint index, GLuint x, GLuint y, GLuint z)) \
X(void, VertexAttribI3uiv, (GLuint index, const GLuint * v)) \
X(void, VertexAttribI4bv, (GLuint index, const GLbyte * v)) \
X(void, VertexAttribI4i, (GLuint index, GLint x, GLint y, GLint z, GLint w)) \
X(void, VertexAttribI4iv, (GLuint index, const GLint * v)) \
X(void, VertexAttribI4sv, (GLuint index, const GLshort * v)) \
X(void, VertexAttribI4ubv, (GLuint index, const GLubyte * v)) \
X(void, VertexAttribI4ui, (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)) \
X(void, VertexAttribI4uiv, (GLuint index, const GLuint * v)) \
X(void, VertexAttribI4usv, (GLuint index, const GLushort * v)) \
X(void, VertexAttribIPointer, (GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, WaitSync, (GLsync sync, GLbitfield flags, GLuint64 timeout)) \
VER_END(3_0) \
VER_START(3_1) \
X(void, ActiveShaderProgram, (GLuint pipeline, GLuint program)) \
X(void, BindImageTexture, (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format)) \
X(void, BindProgramPipeline, (GLuint pipeline)) \
X(void, BindVertexBuffer, (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)) \
X(GLuint, CreateShaderProgramv, (GLenum type, GLsizei count, const GLchar *const* strings)) \
X(void, DeleteProgramPipelines, (GLsizei n, const GLuint * pipelines)) \
X(void, DispatchCompute, (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)) \
X(void, DispatchComputeIndirect, (GLintptr indirect)) \
X(void, DrawArraysIndirect, (GLenum mode, const void * indirect)) \
X(void, DrawElementsIndirect, (GLenum mode, GLenum type, const void * indirect)) \
X(void, FramebufferParameteri, (GLenum target, GLenum pname, GLint param)) \
X(void, GenProgramPipelines, (GLsizei n, GLuint * pipelines)) \
X(void, GetActiveUniformName, (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei * length, GLchar * uniformName)) \
X(void, GetBooleani_v, (GLenum target, GLuint index, GLboolean * data)) \
X(void, GetFramebufferParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetMultisamplefv, (GLenum pname, GLuint index, GLfloat * val)) \
X(void, GetProgramInterfaceiv, (GLuint program, GLenum programInterface, GLenum pname, GLint * params)) \
X(void, GetProgramPipelineInfoLog, (GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog)) \
X(void, GetProgramPipelineiv, (GLuint pipeline, GLenum pname, GLint * params)) \
X(GLuint, GetProgramResourceIndex, (GLuint program, GLenum programInterface, const GLchar * name)) \
X(GLint, GetProgramResourceLocation, (GLuint program, GLenum programInterface, const GLchar * name)) \
X(void, GetProgramResourceName, (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name)) \
X(void, GetProgramResourceiv, (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum * props, GLsizei count, GLsizei * length, GLint * params)) \
X(void, GetTexLevelParameterfv, (GLenum target, GLint level, GLenum pname, GLfloat * params)) \
X(void, GetTexLevelParameteriv, (GLenum target, GLint level, GLenum pname, GLint * params)) \
X(GLboolean, IsProgramPipeline, (GLuint pipeline)) \
X(void, MemoryBarrier, (GLbitfield barriers)) \
X(void, MemoryBarrierByRegion, (GLbitfield barriers)) \
X(void, PrimitiveRestartIndex, (GLuint index)) \
X(void, ProgramUniform1f, (GLuint program, GLint location, GLfloat v0)) \
X(void, ProgramUniform1fv, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, ProgramUniform1i, (GLuint program, GLint location, GLint v0)) \
X(void, ProgramUniform1iv, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, ProgramUniform1ui, (GLuint program, GLint location, GLuint v0)) \
X(void, ProgramUniform1uiv, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, ProgramUniform2f, (GLuint program, GLint location, GLfloat v0, GLfloat v1)) \
X(void, ProgramUniform2fv, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, ProgramUniform2i, (GLuint program, GLint location, GLint v0, GLint v1)) \
X(void, ProgramUniform2iv, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, ProgramUniform2ui, (GLuint program, GLint location, GLuint v0, GLuint v1)) \
X(void, ProgramUniform2uiv, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, ProgramUniform3f, (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)) \
X(void, ProgramUniform3fv, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, ProgramUniform3i, (GLuint program, GLint location, GLint v0, GLint v1, GLint v2)) \
X(void, ProgramUniform3iv, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, ProgramUniform3ui, (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2)) \
X(void, ProgramUniform3uiv, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, ProgramUniform4f, (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)) \
X(void, ProgramUniform4fv, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, ProgramUniform4i, (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)) \
X(void, ProgramUniform4iv, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, ProgramUniform4ui, (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)) \
X(void, ProgramUniform4uiv, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, ProgramUniformMatrix2fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix2x3fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix2x4fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix3fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix3x2fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix3x4fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix4fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix4x2fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix4x3fv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, SampleMaski, (GLuint maskNumber, GLbitfield mask)) \
X(void, TexStorage2DMultisample, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)) \
X(void, UseProgramStages, (GLuint pipeline, GLbitfield stages, GLuint program)) \
X(void, ValidateProgramPipeline, (GLuint pipeline)) \
X(void, VertexAttribBinding, (GLuint attribindex, GLuint bindingindex)) \
X(void, VertexAttribFormat, (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)) \
X(void, VertexAttribIFormat, (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)) \
X(void, VertexBindingDivisor, (GLuint bindingindex, GLuint divisor)) \
VER_END(3_1) \
VER_START(3_2) \
X(void, BlendBarrier, ()) \
X(void, BlendEquationSeparatei, (GLuint buf, GLenum modeRGB, GLenum modeAlpha)) \
X(void, BlendEquationi, (GLuint buf, GLenum mode)) \
X(void, BlendFuncSeparatei, (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)) \
X(void, BlendFunci, (GLuint buf, GLenum src, GLenum dst)) \
X(void, ColorMaski, (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a)) \
X(void, CopyImageSubData, (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth)) \
X(void, DebugMessageCallback, (GLDEBUGPROC callback, const void * userParam)) \
X(void, DebugMessageControl, (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)) \
X(void, DebugMessageInsert, (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf)) \
X(void, Disablei, (GLenum target, GLuint index)) \
X(void, DrawElementsBaseVertex, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLint basevertex)) \
X(void, DrawElementsInstancedBaseVertex, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex)) \
X(void, DrawRangeElementsBaseVertex, (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices, GLint basevertex)) \
X(void, Enablei, (GLenum target, GLuint index)) \
X(void, FramebufferTexture, (GLenum target, GLenum attachment, GLuint texture, GLint level)) \
X(GLuint, GetDebugMessageLog, (GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog)) \
X(void, GetObjectLabel, (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length, GLchar * label)) \
X(void, GetObjectPtrLabel, (const void * ptr, GLsizei bufSize, GLsizei * length, GLchar * label)) \
X(void, GetPointerv, (GLenum pname, void ** params)) \
X(void, GetSamplerParameterIiv, (GLuint sampler, GLenum pname, GLint * params)) \
X(void, GetSamplerParameterIuiv, (GLuint sampler, GLenum pname, GLuint * params)) \
X(void, GetTexParameterIiv, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetTexParameterIuiv, (GLenum target, GLenum pname, GLuint * params)) \
X(void, GetnUniformuiv, (GLuint program, GLint location, GLsizei bufSize, GLuint * params)) \
X(GLboolean, IsEnabledi, (GLenum target, GLuint index)) \
X(void, MinSampleShading, (GLfloat value)) \
X(void, MultiDrawElementsBaseVertex, (GLenum mode, const GLsizei * count, GLenum type, const void *const* indices, GLsizei drawcount, const GLint * basevertex)) \
X(void, ObjectLabel, (GLenum identifier, GLuint name, GLsizei length, const GLchar * label)) \
X(void, ObjectPtrLabel, (const void * ptr, GLsizei length, const GLchar * label)) \
X(void, PatchParameteri, (GLenum pname, GLint value)) \
X(void, PopDebugGroup, ()) \
X(void, PrimitiveBoundingBox, (GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW)) \
X(void, ProvokingVertex, (GLenum mode)) \
X(void, PushDebugGroup, (GLenum source, GLuint id, GLsizei length, const GLchar * message)) \
X(void, SamplerParameterIiv, (GLuint sampler, GLenum pname, const GLint * param)) \
X(void, SamplerParameterIuiv, (GLuint sampler, GLenum pname, const GLuint * param)) \
X(void, TexBuffer, (GLenum target, GLenum internalformat, GLuint buffer)) \
X(void, TexBufferRange, (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
X(void, TexImage2DMultisample, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)) \
X(void, TexImage3DMultisample, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)) \
X(void, TexParameterIiv, (GLenum target, GLenum pname, const GLint * params)) \
X(void, TexParameterIuiv, (GLenum target, GLenum pname, const GLuint * params)) \
X(void, TexStorage3DMultisample, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)) \
VER_END(3_2) \
VER_START(3_3) \
X(void, BindFragDataLocationIndexed, (GLuint program, GLuint colorNumber, GLuint index, const GLchar * name)) \
X(void, ColorP3ui, (GLenum type, GLuint color)) \
X(void, ColorP3uiv, (GLenum type, const GLuint * color)) \
X(void, ColorP4ui, (GLenum type, GLuint color)) \
X(void, ColorP4uiv, (GLenum type, const GLuint * color)) \
X(GLint, GetFragDataIndex, (GLuint program, const GLchar * name)) \
X(void, GetQueryObjecti64v, (GLuint id, GLenum pname, GLint64 * params)) \
X(void, GetQueryObjectui64v, (GLuint id, GLenum pname, GLuint64 * params)) \
X(void, MultiTexCoordP1ui, (GLenum texture, GLenum type, GLuint coords)) \
X(void, MultiTexCoordP1uiv, (GLenum texture, GLenum type, const GLuint * coords)) \
X(void, MultiTexCoordP2ui, (GLenum texture, GLenum type, GLuint coords)) \
X(void, MultiTexCoordP2uiv, (GLenum texture, GLenum type, const GLuint * coords)) \
X(void, MultiTexCoordP3ui, (GLenum texture, GLenum type, GLuint coords)) \
X(void, MultiTexCoordP3uiv, (GLenum texture, GLenum type, const GLuint * coords)) \
X(void, MultiTexCoordP4ui, (GLenum texture, GLenum type, GLuint coords)) \
X(void, MultiTexCoordP4uiv, (GLenum texture, GLenum type, const GLuint * coords)) \
X(void, NormalP3ui, (GLenum type, GLuint coords)) \
X(void, NormalP3uiv, (GLenum type, const GLuint * coords)) \
X(void, QueryCounter, (GLuint id, GLenum target)) \
X(void, SecondaryColorP3ui, (GLenum type, GLuint color)) \
X(void, SecondaryColorP3uiv, (GLenum type, const GLuint * color)) \
X(void, TexCoordP1ui, (GLenum type, GLuint coords)) \
X(void, TexCoordP1uiv, (GLenum type, const GLuint * coords)) \
X(void, TexCoordP2ui, (GLenum type, GLuint coords)) \
X(void, TexCoordP2uiv, (GLenum type, const GLuint * coords)) \
X(void, TexCoordP3ui, (GLenum type, GLuint coords)) \
X(void, TexCoordP3uiv, (GLenum type, const GLuint * coords)) \
X(void, TexCoordP4ui, (GLenum type, GLuint coords)) \
X(void, TexCoordP4uiv, (GLenum type, const GLuint * coords)) \
X(void, VertexAttribP1ui, (GLuint index, GLenum type, GLboolean normalized, GLuint value)) \
X(void, VertexAttribP1uiv, (GLuint index, GLenum type, GLboolean normalized, const GLuint * value)) \
X(void, VertexAttribP2ui, (GLuint index, GLenum type, GLboolean normalized, GLuint value)) \
X(void, VertexAttribP2uiv, (GLuint index, GLenum type, GLboolean normalized, const GLuint * value)) \
X(void, VertexAttribP3ui, (GLuint index, GLenum type, GLboolean normalized, GLuint value)) \
X(void, VertexAttribP3uiv, (GLuint index, GLenum type, GLboolean normalized, const GLuint * value)) \
X(void, VertexAttribP4ui, (GLuint index, GLenum type, GLboolean normalized, GLuint value)) \
X(void, VertexAttribP4uiv, (GLuint index, GLenum type, GLboolean normalized, const GLuint * value)) \
X(void, VertexP2ui, (GLenum type, GLuint value)) \
X(void, VertexP2uiv, (GLenum type, const GLuint * value)) \
X(void, VertexP3ui, (GLenum type, GLuint value)) \
X(void, VertexP3uiv, (GLenum type, const GLuint * value)) \
X(void, VertexP4ui, (GLenum type, GLuint value)) \
X(void, VertexP4uiv, (GLenum type, const GLuint * value)) \
VER_END(3_3) \
VER_START(4_0) \
X(void, BeginQueryIndexed, (GLenum target, GLuint index, GLuint id)) \
X(void, DrawTransformFeedback, (GLenum mode, GLuint id)) \
X(void, DrawTransformFeedbackStream, (GLenum mode, GLuint id, GLuint stream)) \
X(void, EndQueryIndexed, (GLenum target, GLuint index)) \
X(void, GetActiveSubroutineName, (GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name)) \
X(void, GetActiveSubroutineUniformName, (GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei * length, GLchar * name)) \
X(void, GetActiveSubroutineUniformiv, (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint * values)) \
X(void, GetProgramStageiv, (GLuint program, GLenum shadertype, GLenum pname, GLint * values)) \
X(void, GetQueryIndexediv, (GLenum target, GLuint index, GLenum pname, GLint * params)) \
X(GLuint, GetSubroutineIndex, (GLuint program, GLenum shadertype, const GLchar * name)) \
X(GLint, GetSubroutineUniformLocation, (GLuint program, GLenum shadertype, const GLchar * name)) \
X(void, GetUniformSubroutineuiv, (GLenum shadertype, GLint location, GLuint * params)) \
X(void, GetUniformdv, (GLuint program, GLint location, GLdouble * params)) \
X(void, PatchParameterfv, (GLenum pname, const GLfloat * values)) \
X(void, Uniform1d, (GLint location, GLdouble x)) \
X(void, Uniform1dv, (GLint location, GLsizei count, const GLdouble * value)) \
X(void, Uniform2d, (GLint location, GLdouble x, GLdouble y)) \
X(void, Uniform2dv, (GLint location, GLsizei count, const GLdouble * value)) \
X(void, Uniform3d, (GLint location, GLdouble x, GLdouble y, GLdouble z)) \
X(void, Uniform3dv, (GLint location, GLsizei count, const GLdouble * value)) \
X(void, Uniform4d, (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, Uniform4dv, (GLint location, GLsizei count, const GLdouble * value)) \
X(void, UniformMatrix2dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, UniformMatrix2x3dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, UniformMatrix2x4dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, UniformMatrix3dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, UniformMatrix3x2dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, UniformMatrix3x4dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, UniformMatrix4dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, UniformMatrix4x2dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, UniformMatrix4x3dv, (GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, UniformSubroutinesuiv, (GLenum shadertype, GLsizei count, const GLuint * indices)) \
VER_END(4_0) \
VER_START(4_1) \
X(void, DepthRangeArrayv, (GLuint first, GLsizei count, const GLdouble * v)) \
X(void, DepthRangeIndexed, (GLuint index, GLdouble n, GLdouble f)) \
X(void, GetDoublei_v, (GLenum target, GLuint index, GLdouble * data)) \
X(void, GetFloati_v, (GLenum target, GLuint index, GLfloat * data)) \
X(void, GetVertexAttribLdv, (GLuint index, GLenum pname, GLdouble * params)) \
X(void, ProgramUniform1d, (GLuint program, GLint location, GLdouble v0)) \
X(void, ProgramUniform1dv, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, ProgramUniform2d, (GLuint program, GLint location, GLdouble v0, GLdouble v1)) \
X(void, ProgramUniform2dv, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, ProgramUniform3d, (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2)) \
X(void, ProgramUniform3dv, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, ProgramUniform4d, (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)) \
X(void, ProgramUniform4dv, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, ProgramUniformMatrix2dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix2x3dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix2x4dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix3dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix3x2dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix3x4dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix4dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix4x2dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix4x3dv, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ScissorArrayv, (GLuint first, GLsizei count, const GLint * v)) \
X(void, ScissorIndexed, (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height)) \
X(void, ScissorIndexedv, (GLuint index, const GLint * v)) \
X(void, VertexAttribL1d, (GLuint index, GLdouble x)) \
X(void, VertexAttribL1dv, (GLuint index, const GLdouble * v)) \
X(void, VertexAttribL2d, (GLuint index, GLdouble x, GLdouble y)) \
X(void, VertexAttribL2dv, (GLuint index, const GLdouble * v)) \
X(void, VertexAttribL3d, (GLuint index, GLdouble x, GLdouble y, GLdouble z)) \
X(void, VertexAttribL3dv, (GLuint index, const GLdouble * v)) \
X(void, VertexAttribL4d, (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, VertexAttribL4dv, (GLuint index, const GLdouble * v)) \
X(void, VertexAttribLPointer, (GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, ViewportArrayv, (GLuint first, GLsizei count, const GLfloat * v)) \
X(void, ViewportIndexedf, (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h)) \
X(void, ViewportIndexedfv, (GLuint index, const GLfloat * v)) \
VER_END(4_1) \
VER_START(4_2) \
X(void, DrawArraysInstancedBaseInstance, (GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance)) \
X(void, DrawElementsInstancedBaseInstance, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLuint baseinstance)) \
X(void, DrawElementsInstancedBaseVertexBaseInstance, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance)) \
X(void, DrawTransformFeedbackInstanced, (GLenum mode, GLuint id, GLsizei instancecount)) \
X(void, DrawTransformFeedbackStreamInstanced, (GLenum mode, GLuint id, GLuint stream, GLsizei instancecount)) \
X(void, GetActiveAtomicCounterBufferiv, (GLuint program, GLuint bufferIndex, GLenum pname, GLint * params)) \
X(void, TexStorage1D, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)) \
VER_END(4_2) \
VER_START(4_3) \
X(void, ClearBufferData, (GLenum target, GLenum internalformat, GLenum format, GLenum type, const void * data)) \
X(void, ClearBufferSubData, (GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data)) \
X(void, GetInternalformati64v, (GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint64 * params)) \
X(GLint, GetProgramResourceLocationIndex, (GLuint program, GLenum programInterface, const GLchar * name)) \
X(void, InvalidateBufferData, (GLuint buffer)) \
X(void, InvalidateBufferSubData, (GLuint buffer, GLintptr offset, GLsizeiptr length)) \
X(void, InvalidateTexImage, (GLuint texture, GLint level)) \
X(void, InvalidateTexSubImage, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth)) \
X(void, MultiDrawArraysIndirect, (GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride)) \
X(void, MultiDrawElementsIndirect, (GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride)) \
X(void, ShaderStorageBlockBinding, (GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding)) \
X(void, TextureView, (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers)) \
X(void, VertexAttribLFormat, (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)) \
VER_END(4_3) \
VER_START(4_4) \
X(void, BindBuffersBase, (GLenum target, GLuint first, GLsizei count, const GLuint * buffers)) \
X(void, BindBuffersRange, (GLenum target, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizeiptr * sizes)) \
X(void, BindImageTextures, (GLuint first, GLsizei count, const GLuint * textures)) \
X(void, BindSamplers, (GLuint first, GLsizei count, const GLuint * samplers)) \
X(void, BindTextures, (GLuint first, GLsizei count, const GLuint * textures)) \
X(void, BindVertexBuffers, (GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides)) \
X(void, BufferStorage, (GLenum target, GLsizeiptr size, const void * data, GLbitfield flags)) \
X(void, ClearTexImage, (GLuint texture, GLint level, GLenum format, GLenum type, const void * data)) \
X(void, ClearTexSubImage, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * data)) \
VER_END(4_4) \
VER_START(4_5) \
X(void, BindTextureUnit, (GLuint unit, GLuint texture)) \
X(void, BlitNamedFramebuffer, (GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)) \
X(GLenum, CheckNamedFramebufferStatus, (GLuint framebuffer, GLenum target)) \
X(void, ClearNamedBufferData, (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void * data)) \
X(void, ClearNamedBufferSubData, (GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data)) \
X(void, ClearNamedFramebufferfi, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil)) \
X(void, ClearNamedFramebufferfv, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat * value)) \
X(void, ClearNamedFramebufferiv, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint * value)) \
X(void, ClearNamedFramebufferuiv, (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint * value)) \
X(void, ClipControl, (GLenum origin, GLenum depth)) \
X(void, CompressedTextureSubImage1D, (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data)) \
X(void, CompressedTextureSubImage2D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data)) \
X(void, CompressedTextureSubImage3D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data)) \
X(void, CopyNamedBufferSubData, (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)) \
X(void, CopyTextureSubImage1D, (GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)) \
X(void, CopyTextureSubImage2D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, CopyTextureSubImage3D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, CreateBuffers, (GLsizei n, GLuint * buffers)) \
X(void, CreateFramebuffers, (GLsizei n, GLuint * framebuffers)) \
X(void, CreateProgramPipelines, (GLsizei n, GLuint * pipelines)) \
X(void, CreateQueries, (GLenum target, GLsizei n, GLuint * ids)) \
X(void, CreateRenderbuffers, (GLsizei n, GLuint * renderbuffers)) \
X(void, CreateSamplers, (GLsizei n, GLuint * samplers)) \
X(void, CreateTextures, (GLenum target, GLsizei n, GLuint * textures)) \
X(void, CreateTransformFeedbacks, (GLsizei n, GLuint * ids)) \
X(void, CreateVertexArrays, (GLsizei n, GLuint * arrays)) \
X(void, DisableVertexArrayAttrib, (GLuint vaobj, GLuint index)) \
X(void, EnableVertexArrayAttrib, (GLuint vaobj, GLuint index)) \
X(void, FlushMappedNamedBufferRange, (GLuint buffer, GLintptr offset, GLsizeiptr length)) \
X(void, GenerateTextureMipmap, (GLuint texture)) \
X(void, GetCompressedTextureImage, (GLuint texture, GLint level, GLsizei bufSize, void * pixels)) \
X(void, GetCompressedTextureSubImage, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void * pixels)) \
X(void, GetNamedBufferParameteri64v, (GLuint buffer, GLenum pname, GLint64 * params)) \
X(void, GetNamedBufferParameteriv, (GLuint buffer, GLenum pname, GLint * params)) \
X(void, GetNamedBufferPointerv, (GLuint buffer, GLenum pname, void ** params)) \
X(void, GetNamedBufferSubData, (GLuint buffer, GLintptr offset, GLsizeiptr size, void * data)) \
X(void, GetNamedFramebufferAttachmentParameteriv, (GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params)) \
X(void, GetNamedFramebufferParameteriv, (GLuint framebuffer, GLenum pname, GLint * param)) \
X(void, GetNamedRenderbufferParameteriv, (GLuint renderbuffer, GLenum pname, GLint * params)) \
X(void, GetQueryBufferObjecti64v, (GLuint id, GLuint buffer, GLenum pname, GLintptr offset)) \
X(void, GetQueryBufferObjectiv, (GLuint id, GLuint buffer, GLenum pname, GLintptr offset)) \
X(void, GetQueryBufferObjectui64v, (GLuint id, GLuint buffer, GLenum pname, GLintptr offset)) \
X(void, GetQueryBufferObjectuiv, (GLuint id, GLuint buffer, GLenum pname, GLintptr offset)) \
X(void, GetTextureImage, (GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void * pixels)) \
X(void, GetTextureLevelParameterfv, (GLuint texture, GLint level, GLenum pname, GLfloat * params)) \
X(void, GetTextureLevelParameteriv, (GLuint texture, GLint level, GLenum pname, GLint * params)) \
X(void, GetTextureParameterIiv, (GLuint texture, GLenum pname, GLint * params)) \
X(void, GetTextureParameterIuiv, (GLuint texture, GLenum pname, GLuint * params)) \
X(void, GetTextureParameterfv, (GLuint texture, GLenum pname, GLfloat * params)) \
X(void, GetTextureParameteriv, (GLuint texture, GLenum pname, GLint * params)) \
X(void, GetTextureSubImage, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void * pixels)) \
X(void, GetTransformFeedbacki64_v, (GLuint xfb, GLenum pname, GLuint index, GLint64 * param)) \
X(void, GetTransformFeedbacki_v, (GLuint xfb, GLenum pname, GLuint index, GLint * param)) \
X(void, GetTransformFeedbackiv, (GLuint xfb, GLenum pname, GLint * param)) \
X(void, GetVertexArrayIndexed64iv, (GLuint vaobj, GLuint index, GLenum pname, GLint64 * param)) \
X(void, GetVertexArrayIndexediv, (GLuint vaobj, GLuint index, GLenum pname, GLint * param)) \
X(void, GetVertexArrayiv, (GLuint vaobj, GLenum pname, GLint * param)) \
X(void, GetnColorTable, (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void * table)) \
X(void, GetnCompressedTexImage, (GLenum target, GLint lod, GLsizei bufSize, void * pixels)) \
X(void, GetnConvolutionFilter, (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void * image)) \
X(void, GetnHistogram, (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void * values)) \
X(void, GetnMapdv, (GLenum target, GLenum query, GLsizei bufSize, GLdouble * v)) \
X(void, GetnMapfv, (GLenum target, GLenum query, GLsizei bufSize, GLfloat * v)) \
X(void, GetnMapiv, (GLenum target, GLenum query, GLsizei bufSize, GLint * v)) \
X(void, GetnMinmax, (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void * values)) \
X(void, GetnPixelMapfv, (GLenum map, GLsizei bufSize, GLfloat * values)) \
X(void, GetnPixelMapuiv, (GLenum map, GLsizei bufSize, GLuint * values)) \
X(void, GetnPixelMapusv, (GLenum map, GLsizei bufSize, GLushort * values)) \
X(void, GetnPolygonStipple, (GLsizei bufSize, GLubyte * pattern)) \
X(void, GetnSeparableFilter, (GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void * row, GLsizei columnBufSize, void * column, void * span)) \
X(void, GetnTexImage, (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void * pixels)) \
X(void, GetnUniformdv, (GLuint program, GLint location, GLsizei bufSize, GLdouble * params)) \
X(void, InvalidateNamedFramebufferData, (GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments)) \
X(void, InvalidateNamedFramebufferSubData, (GLuint framebuffer, GLsizei numAttachments, const GLenum * attachments, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void *, MapNamedBuffer, (GLuint buffer, GLenum access)) \
X(void *, MapNamedBufferRange, (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)) \
X(void, NamedBufferData, (GLuint buffer, GLsizeiptr size, const void * data, GLenum usage)) \
X(void, NamedBufferStorage, (GLuint buffer, GLsizeiptr size, const void * data, GLbitfield flags)) \
X(void, NamedBufferSubData, (GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data)) \
X(void, NamedFramebufferDrawBuffer, (GLuint framebuffer, GLenum buf)) \
X(void, NamedFramebufferDrawBuffers, (GLuint framebuffer, GLsizei n, const GLenum * bufs)) \
X(void, NamedFramebufferParameteri, (GLuint framebuffer, GLenum pname, GLint param)) \
X(void, NamedFramebufferReadBuffer, (GLuint framebuffer, GLenum src)) \
X(void, NamedFramebufferRenderbuffer, (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)) \
X(void, NamedFramebufferTexture, (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)) \
X(void, NamedFramebufferTextureLayer, (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer)) \
X(void, NamedRenderbufferStorage, (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, NamedRenderbufferStorageMultisample, (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, TextureBarrier, ()) \
X(void, TextureBuffer, (GLuint texture, GLenum internalformat, GLuint buffer)) \
X(void, TextureBufferRange, (GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
X(void, TextureParameterIiv, (GLuint texture, GLenum pname, const GLint * params)) \
X(void, TextureParameterIuiv, (GLuint texture, GLenum pname, const GLuint * params)) \
X(void, TextureParameterf, (GLuint texture, GLenum pname, GLfloat param)) \
X(void, TextureParameterfv, (GLuint texture, GLenum pname, const GLfloat * param)) \
X(void, TextureParameteri, (GLuint texture, GLenum pname, GLint param)) \
X(void, TextureParameteriv, (GLuint texture, GLenum pname, const GLint * param)) \
X(void, TextureStorage1D, (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width)) \
X(void, TextureStorage2D, (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, TextureStorage2DMultisample, (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)) \
X(void, TextureStorage3D, (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)) \
X(void, TextureStorage3DMultisample, (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)) \
X(void, TextureSubImage1D, (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels)) \
X(void, TextureSubImage2D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels)) \
X(void, TextureSubImage3D, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels)) \
X(void, TransformFeedbackBufferBase, (GLuint xfb, GLuint index, GLuint buffer)) \
X(void, TransformFeedbackBufferRange, (GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
X(GLboolean, UnmapNamedBuffer, (GLuint buffer)) \
X(void, VertexArrayAttribBinding, (GLuint vaobj, GLuint attribindex, GLuint bindingindex)) \
X(void, VertexArrayAttribFormat, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)) \
X(void, VertexArrayAttribIFormat, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)) \
X(void, VertexArrayAttribLFormat, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)) \
X(void, VertexArrayBindingDivisor, (GLuint vaobj, GLuint bindingindex, GLuint divisor)) \
X(void, VertexArrayElementBuffer, (GLuint vaobj, GLuint buffer)) \
X(void, VertexArrayVertexBuffer, (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)) \
X(void, VertexArrayVertexBuffers, (GLuint vaobj, GLuint first, GLsizei count, const GLuint * buffers, const GLintptr * offsets, const GLsizei * strides)) \
VER_END(4_5) \
VER_START(4_6) \
X(void, MultiDrawArraysIndirectCount, (GLenum mode, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride)) \
X(void, MultiDrawElementsIndirectCount, (GLenum mode, GLenum type, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride)) \
X(void, PolygonOffsetClamp, (GLfloat factor, GLfloat units, GLfloat clamp)) \
X(void, SpecializeShader, (GLuint shader, const GLchar * pEntryPoint, GLuint numSpecializationConstants, const GLuint * pConstantIndex, const GLuint * pConstantValue)) \
VER_END(4_6) \
EXT_START("GL_3DFX_tbuffer") \
X(void, TbufferMask3DFX, (GLuint mask)) \
EXT_END() \
EXT_START("GL_AMD_debug_output") \
X(void, DebugMessageCallbackAMD, (GLDEBUGPROCAMD callback, void * userParam)) \
X(void, DebugMessageEnableAMD, (GLenum category, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)) \
X(void, DebugMessageInsertAMD, (GLenum category, GLenum severity, GLuint id, GLsizei length, const GLchar * buf)) \
X(GLuint, GetDebugMessageLogAMD, (GLuint count, GLsizei bufSize, GLenum * categories, GLenum * severities, GLuint * ids, GLsizei * lengths, GLchar * message)) \
EXT_END() \
EXT_START("GL_AMD_draw_buffers_blend") \
X(void, BlendEquationIndexedAMD, (GLuint buf, GLenum mode)) \
X(void, BlendEquationSeparateIndexedAMD, (GLuint buf, GLenum modeRGB, GLenum modeAlpha)) \
X(void, BlendFuncIndexedAMD, (GLuint buf, GLenum src, GLenum dst)) \
X(void, BlendFuncSeparateIndexedAMD, (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)) \
EXT_END() \
EXT_START("GL_AMD_framebuffer_multisample_advanced") \
X(void, NamedRenderbufferStorageMultisampleAdvancedAMD, (GLuint renderbuffer, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, RenderbufferStorageMultisampleAdvancedAMD, (GLenum target, GLsizei samples, GLsizei storageSamples, GLenum internalformat, GLsizei width, GLsizei height)) \
EXT_END() \
EXT_START("GL_AMD_framebuffer_sample_positions") \
X(void, FramebufferSamplePositionsfvAMD, (GLenum target, GLuint numsamples, GLuint pixelindex, const GLfloat * values)) \
X(void, GetFramebufferParameterfvAMD, (GLenum target, GLenum pname, GLuint numsamples, GLuint pixelindex, GLsizei size, GLfloat * values)) \
X(void, GetNamedFramebufferParameterfvAMD, (GLuint framebuffer, GLenum pname, GLuint numsamples, GLuint pixelindex, GLsizei size, GLfloat * values)) \
X(void, NamedFramebufferSamplePositionsfvAMD, (GLuint framebuffer, GLuint numsamples, GLuint pixelindex, const GLfloat * values)) \
EXT_END() \
EXT_START("GL_AMD_gpu_shader_int64") \
X(void, GetUniformi64vNV, (GLuint program, GLint location, GLint64EXT * params)) \
X(void, GetUniformui64vNV, (GLuint program, GLint location, GLuint64EXT * params)) \
X(void, ProgramUniform1i64NV, (GLuint program, GLint location, GLint64EXT x)) \
X(void, ProgramUniform1i64vNV, (GLuint program, GLint location, GLsizei count, const GLint64EXT * value)) \
X(void, ProgramUniform1ui64NV, (GLuint program, GLint location, GLuint64EXT x)) \
X(void, ProgramUniform1ui64vNV, (GLuint program, GLint location, GLsizei count, const GLuint64EXT * value)) \
X(void, ProgramUniform2i64NV, (GLuint program, GLint location, GLint64EXT x, GLint64EXT y)) \
X(void, ProgramUniform2i64vNV, (GLuint program, GLint location, GLsizei count, const GLint64EXT * value)) \
X(void, ProgramUniform2ui64NV, (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y)) \
X(void, ProgramUniform2ui64vNV, (GLuint program, GLint location, GLsizei count, const GLuint64EXT * value)) \
X(void, ProgramUniform3i64NV, (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z)) \
X(void, ProgramUniform3i64vNV, (GLuint program, GLint location, GLsizei count, const GLint64EXT * value)) \
X(void, ProgramUniform3ui64NV, (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z)) \
X(void, ProgramUniform3ui64vNV, (GLuint program, GLint location, GLsizei count, const GLuint64EXT * value)) \
X(void, ProgramUniform4i64NV, (GLuint program, GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w)) \
X(void, ProgramUniform4i64vNV, (GLuint program, GLint location, GLsizei count, const GLint64EXT * value)) \
X(void, ProgramUniform4ui64NV, (GLuint program, GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w)) \
X(void, ProgramUniform4ui64vNV, (GLuint program, GLint location, GLsizei count, const GLuint64EXT * value)) \
X(void, Uniform1i64NV, (GLint location, GLint64EXT x)) \
X(void, Uniform1i64vNV, (GLint location, GLsizei count, const GLint64EXT * value)) \
X(void, Uniform1ui64NV, (GLint location, GLuint64EXT x)) \
X(void, Uniform1ui64vNV, (GLint location, GLsizei count, const GLuint64EXT * value)) \
X(void, Uniform2i64NV, (GLint location, GLint64EXT x, GLint64EXT y)) \
X(void, Uniform2i64vNV, (GLint location, GLsizei count, const GLint64EXT * value)) \
X(void, Uniform2ui64NV, (GLint location, GLuint64EXT x, GLuint64EXT y)) \
X(void, Uniform2ui64vNV, (GLint location, GLsizei count, const GLuint64EXT * value)) \
X(void, Uniform3i64NV, (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z)) \
X(void, Uniform3i64vNV, (GLint location, GLsizei count, const GLint64EXT * value)) \
X(void, Uniform3ui64NV, (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z)) \
X(void, Uniform3ui64vNV, (GLint location, GLsizei count, const GLuint64EXT * value)) \
X(void, Uniform4i64NV, (GLint location, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w)) \
X(void, Uniform4i64vNV, (GLint location, GLsizei count, const GLint64EXT * value)) \
X(void, Uniform4ui64NV, (GLint location, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w)) \
X(void, Uniform4ui64vNV, (GLint location, GLsizei count, const GLuint64EXT * value)) \
EXT_END() \
EXT_START("GL_AMD_interleaved_elements") \
X(void, VertexAttribParameteriAMD, (GLuint index, GLenum pname, GLint param)) \
EXT_END() \
EXT_START("GL_AMD_multi_draw_indirect") \
X(void, MultiDrawArraysIndirectAMD, (GLenum mode, const void * indirect, GLsizei primcount, GLsizei stride)) \
X(void, MultiDrawElementsIndirectAMD, (GLenum mode, GLenum type, const void * indirect, GLsizei primcount, GLsizei stride)) \
EXT_END() \
EXT_START("GL_AMD_name_gen_delete") \
X(void, DeleteNamesAMD, (GLenum identifier, GLuint num, const GLuint * names)) \
X(void, GenNamesAMD, (GLenum identifier, GLuint num, GLuint * names)) \
X(GLboolean, IsNameAMD, (GLenum identifier, GLuint name)) \
EXT_END() \
EXT_START("GL_AMD_occlusion_query_event") \
X(void, QueryObjectParameteruiAMD, (GLenum target, GLuint id, GLenum pname, GLuint param)) \
EXT_END() \
EXT_START("GL_AMD_performance_monitor") \
X(void, BeginPerfMonitorAMD, (GLuint monitor)) \
X(void, DeletePerfMonitorsAMD, (GLsizei n, GLuint * monitors)) \
X(void, EndPerfMonitorAMD, (GLuint monitor)) \
X(void, GenPerfMonitorsAMD, (GLsizei n, GLuint * monitors)) \
X(void, GetPerfMonitorCounterDataAMD, (GLuint monitor, GLenum pname, GLsizei dataSize, GLuint * data, GLint * bytesWritten)) \
X(void, GetPerfMonitorCounterInfoAMD, (GLuint group, GLuint counter, GLenum pname, void * data)) \
X(void, GetPerfMonitorCounterStringAMD, (GLuint group, GLuint counter, GLsizei bufSize, GLsizei * length, GLchar * counterString)) \
X(void, GetPerfMonitorCountersAMD, (GLuint group, GLint * numCounters, GLint * maxActiveCounters, GLsizei counterSize, GLuint * counters)) \
X(void, GetPerfMonitorGroupStringAMD, (GLuint group, GLsizei bufSize, GLsizei * length, GLchar * groupString)) \
X(void, GetPerfMonitorGroupsAMD, (GLint * numGroups, GLsizei groupsSize, GLuint * groups)) \
X(void, SelectPerfMonitorCountersAMD, (GLuint monitor, GLboolean enable, GLuint group, GLint numCounters, GLuint * counterList)) \
EXT_END() \
EXT_START("GL_AMD_sample_positions") \
X(void, SetMultisamplefvAMD, (GLenum pname, GLuint index, const GLfloat * val)) \
EXT_END() \
EXT_START("GL_AMD_sparse_texture") \
X(void, TexStorageSparseAMD, (GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags)) \
X(void, TextureStorageSparseAMD, (GLuint texture, GLenum target, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLsizei layers, GLbitfield flags)) \
EXT_END() \
EXT_START("GL_AMD_stencil_operation_extended") \
X(void, StencilOpValueAMD, (GLenum face, GLuint value)) \
EXT_END() \
EXT_START("GL_AMD_vertex_shader_tessellator") \
X(void, TessellationFactorAMD, (GLfloat factor)) \
X(void, TessellationModeAMD, (GLenum mode)) \
EXT_END() \
EXT_START("GL_ANGLE_framebuffer_blit") \
X(void, BlitFramebufferANGLE, (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)) \
EXT_END() \
EXT_START("GL_ANGLE_framebuffer_multisample") \
X(void, RenderbufferStorageMultisampleANGLE, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)) \
EXT_END() \
EXT_START("GL_ANGLE_instanced_arrays") \
X(void, DrawArraysInstancedANGLE, (GLenum mode, GLint first, GLsizei count, GLsizei primcount)) \
X(void, DrawElementsInstancedANGLE, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount)) \
X(void, VertexAttribDivisorANGLE, (GLuint index, GLuint divisor)) \
EXT_END() \
EXT_START("GL_ANGLE_translated_shader_source") \
X(void, GetTranslatedShaderSourceANGLE, (GLuint shader, GLsizei bufSize, GLsizei * length, GLchar * source)) \
EXT_END() \
EXT_START("GL_APPLE_copy_texture_levels") \
X(void, CopyTextureLevelsAPPLE, (GLuint destinationTexture, GLuint sourceTexture, GLint sourceBaseLevel, GLsizei sourceLevelCount)) \
EXT_END() \
EXT_START("GL_APPLE_element_array") \
X(void, DrawElementArrayAPPLE, (GLenum mode, GLint first, GLsizei count)) \
X(void, DrawRangeElementArrayAPPLE, (GLenum mode, GLuint start, GLuint end, GLint first, GLsizei count)) \
X(void, ElementPointerAPPLE, (GLenum type, const void * pointer)) \
X(void, MultiDrawElementArrayAPPLE, (GLenum mode, const GLint * first, const GLsizei * count, GLsizei primcount)) \
X(void, MultiDrawRangeElementArrayAPPLE, (GLenum mode, GLuint start, GLuint end, const GLint * first, const GLsizei * count, GLsizei primcount)) \
EXT_END() \
EXT_START("GL_APPLE_fence") \
X(void, DeleteFencesAPPLE, (GLsizei n, const GLuint * fences)) \
X(void, FinishFenceAPPLE, (GLuint fence)) \
X(void, FinishObjectAPPLE, (GLenum object, GLint name)) \
X(void, GenFencesAPPLE, (GLsizei n, GLuint * fences)) \
X(GLboolean, IsFenceAPPLE, (GLuint fence)) \
X(void, SetFenceAPPLE, (GLuint fence)) \
X(GLboolean, TestFenceAPPLE, (GLuint fence)) \
X(GLboolean, TestObjectAPPLE, (GLenum object, GLuint name)) \
EXT_END() \
EXT_START("GL_APPLE_flush_buffer_range") \
X(void, BufferParameteriAPPLE, (GLenum target, GLenum pname, GLint param)) \
X(void, FlushMappedBufferRangeAPPLE, (GLenum target, GLintptr offset, GLsizeiptr size)) \
EXT_END() \
EXT_START("GL_APPLE_framebuffer_multisample") \
X(void, RenderbufferStorageMultisampleAPPLE, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, ResolveMultisampleFramebufferAPPLE, ()) \
EXT_END() \
EXT_START("GL_APPLE_object_purgeable") \
X(void, GetObjectParameterivAPPLE, (GLenum objectType, GLuint name, GLenum pname, GLint * params)) \
X(GLenum, ObjectPurgeableAPPLE, (GLenum objectType, GLuint name, GLenum option)) \
X(GLenum, ObjectUnpurgeableAPPLE, (GLenum objectType, GLuint name, GLenum option)) \
EXT_END() \
EXT_START("GL_APPLE_sync") \
X(GLenum, ClientWaitSyncAPPLE, (GLsync sync, GLbitfield flags, GLuint64 timeout)) \
X(void, DeleteSyncAPPLE, (GLsync sync)) \
X(GLsync, FenceSyncAPPLE, (GLenum condition, GLbitfield flags)) \
X(void, GetInteger64vAPPLE, (GLenum pname, GLint64 * params)) \
X(void, GetSyncivAPPLE, (GLsync sync, GLenum pname, GLsizei count, GLsizei * length, GLint * values)) \
X(GLboolean, IsSyncAPPLE, (GLsync sync)) \
X(void, WaitSyncAPPLE, (GLsync sync, GLbitfield flags, GLuint64 timeout)) \
EXT_END() \
EXT_START("GL_APPLE_texture_range") \
X(void, GetTexParameterPointervAPPLE, (GLenum target, GLenum pname, void ** params)) \
X(void, TextureRangeAPPLE, (GLenum target, GLsizei length, const void * pointer)) \
EXT_END() \
EXT_START("GL_APPLE_vertex_array_object") \
X(void, BindVertexArrayAPPLE, (GLuint array)) \
X(void, DeleteVertexArraysAPPLE, (GLsizei n, const GLuint * arrays)) \
X(void, GenVertexArraysAPPLE, (GLsizei n, GLuint * arrays)) \
X(GLboolean, IsVertexArrayAPPLE, (GLuint array)) \
EXT_END() \
EXT_START("GL_APPLE_vertex_array_range") \
X(void, FlushVertexArrayRangeAPPLE, (GLsizei length, void * pointer)) \
X(void, VertexArrayParameteriAPPLE, (GLenum pname, GLint param)) \
X(void, VertexArrayRangeAPPLE, (GLsizei length, void * pointer)) \
EXT_END() \
EXT_START("GL_APPLE_vertex_program_evaluators") \
X(void, DisableVertexAttribAPPLE, (GLuint index, GLenum pname)) \
X(void, EnableVertexAttribAPPLE, (GLuint index, GLenum pname)) \
X(GLboolean, IsVertexAttribEnabledAPPLE, (GLuint index, GLenum pname)) \
X(void, MapVertexAttrib1dAPPLE, (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble * points)) \
X(void, MapVertexAttrib1fAPPLE, (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat * points)) \
X(void, MapVertexAttrib2dAPPLE, (GLuint index, GLuint size, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble * points)) \
X(void, MapVertexAttrib2fAPPLE, (GLuint index, GLuint size, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat * points)) \
EXT_END() \
EXT_START("GL_ARB_ES3_2_compatibility") \
X(void, PrimitiveBoundingBoxARB, (GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW)) \
EXT_END() \
EXT_START("GL_ARB_bindless_texture") \
X(GLuint64, GetImageHandleARB, (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format)) \
X(GLuint64, GetTextureHandleARB, (GLuint texture)) \
X(GLuint64, GetTextureSamplerHandleARB, (GLuint texture, GLuint sampler)) \
X(void, GetVertexAttribLui64vARB, (GLuint index, GLenum pname, GLuint64EXT * params)) \
X(GLboolean, IsImageHandleResidentARB, (GLuint64 handle)) \
X(GLboolean, IsTextureHandleResidentARB, (GLuint64 handle)) \
X(void, MakeImageHandleNonResidentARB, (GLuint64 handle)) \
X(void, MakeImageHandleResidentARB, (GLuint64 handle, GLenum access)) \
X(void, MakeTextureHandleNonResidentARB, (GLuint64 handle)) \
X(void, MakeTextureHandleResidentARB, (GLuint64 handle)) \
X(void, ProgramUniformHandleui64ARB, (GLuint program, GLint location, GLuint64 value)) \
X(void, ProgramUniformHandleui64vARB, (GLuint program, GLint location, GLsizei count, const GLuint64 * values)) \
X(void, UniformHandleui64ARB, (GLint location, GLuint64 value)) \
X(void, UniformHandleui64vARB, (GLint location, GLsizei count, const GLuint64 * value)) \
X(void, VertexAttribL1ui64ARB, (GLuint index, GLuint64EXT x)) \
X(void, VertexAttribL1ui64vARB, (GLuint index, const GLuint64EXT * v)) \
EXT_END() \
EXT_START("GL_ARB_cl_event") \
X(GLsync, CreateSyncFromCLeventARB, (struct _cl_context * context, struct _cl_event * event, GLbitfield flags)) \
EXT_END() \
EXT_START("GL_ARB_color_buffer_float") \
X(void, ClampColorARB, (GLenum target, GLenum clamp)) \
EXT_END() \
EXT_START("GL_ARB_compute_variable_group_size") \
X(void, DispatchComputeGroupSizeARB, (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, GLuint group_size_x, GLuint group_size_y, GLuint group_size_z)) \
EXT_END() \
EXT_START("GL_ARB_debug_output") \
X(void, DebugMessageCallbackARB, (GLDEBUGPROCARB callback, const void * userParam)) \
X(void, DebugMessageControlARB, (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)) \
X(void, DebugMessageInsertARB, (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf)) \
X(GLuint, GetDebugMessageLogARB, (GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog)) \
EXT_END() \
EXT_START("GL_ARB_draw_buffers") \
X(void, DrawBuffersARB, (GLsizei n, const GLenum * bufs)) \
EXT_END() \
EXT_START("GL_ARB_draw_buffers_blend") \
X(void, BlendEquationSeparateiARB, (GLuint buf, GLenum modeRGB, GLenum modeAlpha)) \
X(void, BlendEquationiARB, (GLuint buf, GLenum mode)) \
X(void, BlendFuncSeparateiARB, (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)) \
X(void, BlendFunciARB, (GLuint buf, GLenum src, GLenum dst)) \
EXT_END() \
EXT_START("GL_ARB_draw_instanced") \
X(void, DrawArraysInstancedARB, (GLenum mode, GLint first, GLsizei count, GLsizei primcount)) \
X(void, DrawElementsInstancedARB, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount)) \
EXT_END() \
EXT_START("GL_ARB_fragment_program") \
X(void, BindProgramARB, (GLenum target, GLuint program)) \
X(void, DeleteProgramsARB, (GLsizei n, const GLuint * programs)) \
X(void, GenProgramsARB, (GLsizei n, GLuint * programs)) \
X(void, GetProgramEnvParameterdvARB, (GLenum target, GLuint index, GLdouble * params)) \
X(void, GetProgramEnvParameterfvARB, (GLenum target, GLuint index, GLfloat * params)) \
X(void, GetProgramLocalParameterdvARB, (GLenum target, GLuint index, GLdouble * params)) \
X(void, GetProgramLocalParameterfvARB, (GLenum target, GLuint index, GLfloat * params)) \
X(void, GetProgramStringARB, (GLenum target, GLenum pname, void * string)) \
X(void, GetProgramivARB, (GLenum target, GLenum pname, GLint * params)) \
X(GLboolean, IsProgramARB, (GLuint program)) \
X(void, ProgramEnvParameter4dARB, (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, ProgramEnvParameter4dvARB, (GLenum target, GLuint index, const GLdouble * params)) \
X(void, ProgramEnvParameter4fARB, (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, ProgramEnvParameter4fvARB, (GLenum target, GLuint index, const GLfloat * params)) \
X(void, ProgramLocalParameter4dARB, (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, ProgramLocalParameter4dvARB, (GLenum target, GLuint index, const GLdouble * params)) \
X(void, ProgramLocalParameter4fARB, (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, ProgramLocalParameter4fvARB, (GLenum target, GLuint index, const GLfloat * params)) \
X(void, ProgramStringARB, (GLenum target, GLenum format, GLsizei len, const void * string)) \
EXT_END() \
EXT_START("GL_ARB_geometry_shader4") \
X(void, FramebufferTextureARB, (GLenum target, GLenum attachment, GLuint texture, GLint level)) \
X(void, FramebufferTextureFaceARB, (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face)) \
X(void, FramebufferTextureLayerARB, (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)) \
X(void, ProgramParameteriARB, (GLuint program, GLenum pname, GLint value)) \
EXT_END() \
EXT_START("GL_ARB_gl_spirv") \
X(void, SpecializeShaderARB, (GLuint shader, const GLchar * pEntryPoint, GLuint numSpecializationConstants, const GLuint * pConstantIndex, const GLuint * pConstantValue)) \
EXT_END() \
EXT_START("GL_ARB_gpu_shader_int64") \
X(void, GetUniformi64vARB, (GLuint program, GLint location, GLint64 * params)) \
X(void, GetUniformui64vARB, (GLuint program, GLint location, GLuint64 * params)) \
X(void, GetnUniformi64vARB, (GLuint program, GLint location, GLsizei bufSize, GLint64 * params)) \
X(void, GetnUniformui64vARB, (GLuint program, GLint location, GLsizei bufSize, GLuint64 * params)) \
X(void, ProgramUniform1i64ARB, (GLuint program, GLint location, GLint64 x)) \
X(void, ProgramUniform1i64vARB, (GLuint program, GLint location, GLsizei count, const GLint64 * value)) \
X(void, ProgramUniform1ui64ARB, (GLuint program, GLint location, GLuint64 x)) \
X(void, ProgramUniform1ui64vARB, (GLuint program, GLint location, GLsizei count, const GLuint64 * value)) \
X(void, ProgramUniform2i64ARB, (GLuint program, GLint location, GLint64 x, GLint64 y)) \
X(void, ProgramUniform2i64vARB, (GLuint program, GLint location, GLsizei count, const GLint64 * value)) \
X(void, ProgramUniform2ui64ARB, (GLuint program, GLint location, GLuint64 x, GLuint64 y)) \
X(void, ProgramUniform2ui64vARB, (GLuint program, GLint location, GLsizei count, const GLuint64 * value)) \
X(void, ProgramUniform3i64ARB, (GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z)) \
X(void, ProgramUniform3i64vARB, (GLuint program, GLint location, GLsizei count, const GLint64 * value)) \
X(void, ProgramUniform3ui64ARB, (GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z)) \
X(void, ProgramUniform3ui64vARB, (GLuint program, GLint location, GLsizei count, const GLuint64 * value)) \
X(void, ProgramUniform4i64ARB, (GLuint program, GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w)) \
X(void, ProgramUniform4i64vARB, (GLuint program, GLint location, GLsizei count, const GLint64 * value)) \
X(void, ProgramUniform4ui64ARB, (GLuint program, GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w)) \
X(void, ProgramUniform4ui64vARB, (GLuint program, GLint location, GLsizei count, const GLuint64 * value)) \
X(void, Uniform1i64ARB, (GLint location, GLint64 x)) \
X(void, Uniform1i64vARB, (GLint location, GLsizei count, const GLint64 * value)) \
X(void, Uniform1ui64ARB, (GLint location, GLuint64 x)) \
X(void, Uniform1ui64vARB, (GLint location, GLsizei count, const GLuint64 * value)) \
X(void, Uniform2i64ARB, (GLint location, GLint64 x, GLint64 y)) \
X(void, Uniform2i64vARB, (GLint location, GLsizei count, const GLint64 * value)) \
X(void, Uniform2ui64ARB, (GLint location, GLuint64 x, GLuint64 y)) \
X(void, Uniform2ui64vARB, (GLint location, GLsizei count, const GLuint64 * value)) \
X(void, Uniform3i64ARB, (GLint location, GLint64 x, GLint64 y, GLint64 z)) \
X(void, Uniform3i64vARB, (GLint location, GLsizei count, const GLint64 * value)) \
X(void, Uniform3ui64ARB, (GLint location, GLuint64 x, GLuint64 y, GLuint64 z)) \
X(void, Uniform3ui64vARB, (GLint location, GLsizei count, const GLuint64 * value)) \
X(void, Uniform4i64ARB, (GLint location, GLint64 x, GLint64 y, GLint64 z, GLint64 w)) \
X(void, Uniform4i64vARB, (GLint location, GLsizei count, const GLint64 * value)) \
X(void, Uniform4ui64ARB, (GLint location, GLuint64 x, GLuint64 y, GLuint64 z, GLuint64 w)) \
X(void, Uniform4ui64vARB, (GLint location, GLsizei count, const GLuint64 * value)) \
EXT_END() \
EXT_START("GL_ARB_imaging") \
X(void, ColorSubTable, (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void * data)) \
X(void, ColorTable, (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void * table)) \
X(void, ColorTableParameterfv, (GLenum target, GLenum pname, const GLfloat * params)) \
X(void, ColorTableParameteriv, (GLenum target, GLenum pname, const GLint * params)) \
X(void, ConvolutionFilter1D, (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void * image)) \
X(void, ConvolutionFilter2D, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * image)) \
X(void, ConvolutionParameterf, (GLenum target, GLenum pname, GLfloat params)) \
X(void, ConvolutionParameterfv, (GLenum target, GLenum pname, const GLfloat * params)) \
X(void, ConvolutionParameteri, (GLenum target, GLenum pname, GLint params)) \
X(void, ConvolutionParameteriv, (GLenum target, GLenum pname, const GLint * params)) \
X(void, CopyColorSubTable, (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width)) \
X(void, CopyColorTable, (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width)) \
X(void, CopyConvolutionFilter1D, (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width)) \
X(void, CopyConvolutionFilter2D, (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, GetColorTable, (GLenum target, GLenum format, GLenum type, void * table)) \
X(void, GetColorTableParameterfv, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetColorTableParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetConvolutionFilter, (GLenum target, GLenum format, GLenum type, void * image)) \
X(void, GetConvolutionParameterfv, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetConvolutionParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetHistogram, (GLenum target, GLboolean reset, GLenum format, GLenum type, void * values)) \
X(void, GetHistogramParameterfv, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetHistogramParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetMinmax, (GLenum target, GLboolean reset, GLenum format, GLenum type, void * values)) \
X(void, GetMinmaxParameterfv, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetMinmaxParameteriv, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetSeparableFilter, (GLenum target, GLenum format, GLenum type, void * row, void * column, void * span)) \
X(void, Histogram, (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink)) \
X(void, Minmax, (GLenum target, GLenum internalformat, GLboolean sink)) \
X(void, ResetHistogram, (GLenum target)) \
X(void, ResetMinmax, (GLenum target)) \
X(void, SeparableFilter2D, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * row, const void * column)) \
EXT_END() \
EXT_START("GL_ARB_indirect_parameters") \
X(void, MultiDrawArraysIndirectCountARB, (GLenum mode, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride)) \
X(void, MultiDrawElementsIndirectCountARB, (GLenum mode, GLenum type, const void * indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride)) \
EXT_END() \
EXT_START("GL_ARB_instanced_arrays") \
X(void, VertexAttribDivisorARB, (GLuint index, GLuint divisor)) \
EXT_END() \
EXT_START("GL_ARB_matrix_palette") \
X(void, CurrentPaletteMatrixARB, (GLint index)) \
X(void, MatrixIndexPointerARB, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, MatrixIndexubvARB, (GLint size, const GLubyte * indices)) \
X(void, MatrixIndexuivARB, (GLint size, const GLuint * indices)) \
X(void, MatrixIndexusvARB, (GLint size, const GLushort * indices)) \
EXT_END() \
EXT_START("GL_ARB_multisample") \
X(void, SampleCoverageARB, (GLfloat value, GLboolean invert)) \
EXT_END() \
EXT_START("GL_ARB_multitexture") \
X(void, ActiveTextureARB, (GLenum texture)) \
X(void, ClientActiveTextureARB, (GLenum texture)) \
X(void, MultiTexCoord1dARB, (GLenum target, GLdouble s)) \
X(void, MultiTexCoord1dvARB, (GLenum target, const GLdouble * v)) \
X(void, MultiTexCoord1fARB, (GLenum target, GLfloat s)) \
X(void, MultiTexCoord1fvARB, (GLenum target, const GLfloat * v)) \
X(void, MultiTexCoord1iARB, (GLenum target, GLint s)) \
X(void, MultiTexCoord1ivARB, (GLenum target, const GLint * v)) \
X(void, MultiTexCoord1sARB, (GLenum target, GLshort s)) \
X(void, MultiTexCoord1svARB, (GLenum target, const GLshort * v)) \
X(void, MultiTexCoord2dARB, (GLenum target, GLdouble s, GLdouble t)) \
X(void, MultiTexCoord2dvARB, (GLenum target, const GLdouble * v)) \
X(void, MultiTexCoord2fARB, (GLenum target, GLfloat s, GLfloat t)) \
X(void, MultiTexCoord2fvARB, (GLenum target, const GLfloat * v)) \
X(void, MultiTexCoord2iARB, (GLenum target, GLint s, GLint t)) \
X(void, MultiTexCoord2ivARB, (GLenum target, const GLint * v)) \
X(void, MultiTexCoord2sARB, (GLenum target, GLshort s, GLshort t)) \
X(void, MultiTexCoord2svARB, (GLenum target, const GLshort * v)) \
X(void, MultiTexCoord3dARB, (GLenum target, GLdouble s, GLdouble t, GLdouble r)) \
X(void, MultiTexCoord3dvARB, (GLenum target, const GLdouble * v)) \
X(void, MultiTexCoord3fARB, (GLenum target, GLfloat s, GLfloat t, GLfloat r)) \
X(void, MultiTexCoord3fvARB, (GLenum target, const GLfloat * v)) \
X(void, MultiTexCoord3iARB, (GLenum target, GLint s, GLint t, GLint r)) \
X(void, MultiTexCoord3ivARB, (GLenum target, const GLint * v)) \
X(void, MultiTexCoord3sARB, (GLenum target, GLshort s, GLshort t, GLshort r)) \
X(void, MultiTexCoord3svARB, (GLenum target, const GLshort * v)) \
X(void, MultiTexCoord4dARB, (GLenum target, GLdouble s, GLdouble t, GLdouble r, GLdouble q)) \
X(void, MultiTexCoord4dvARB, (GLenum target, const GLdouble * v)) \
X(void, MultiTexCoord4fARB, (GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q)) \
X(void, MultiTexCoord4fvARB, (GLenum target, const GLfloat * v)) \
X(void, MultiTexCoord4iARB, (GLenum target, GLint s, GLint t, GLint r, GLint q)) \
X(void, MultiTexCoord4ivARB, (GLenum target, const GLint * v)) \
X(void, MultiTexCoord4sARB, (GLenum target, GLshort s, GLshort t, GLshort r, GLshort q)) \
X(void, MultiTexCoord4svARB, (GLenum target, const GLshort * v)) \
EXT_END() \
EXT_START("GL_ARB_occlusion_query") \
X(void, BeginQueryARB, (GLenum target, GLuint id)) \
X(void, DeleteQueriesARB, (GLsizei n, const GLuint * ids)) \
X(void, EndQueryARB, (GLenum target)) \
X(void, GenQueriesARB, (GLsizei n, GLuint * ids)) \
X(void, GetQueryObjectivARB, (GLuint id, GLenum pname, GLint * params)) \
X(void, GetQueryObjectuivARB, (GLuint id, GLenum pname, GLuint * params)) \
X(void, GetQueryivARB, (GLenum target, GLenum pname, GLint * params)) \
X(GLboolean, IsQueryARB, (GLuint id)) \
EXT_END() \
EXT_START("GL_ARB_parallel_shader_compile") \
X(void, MaxShaderCompilerThreadsARB, (GLuint count)) \
EXT_END() \
EXT_START("GL_ARB_point_parameters") \
X(void, PointParameterfARB, (GLenum pname, GLfloat param)) \
X(void, PointParameterfvARB, (GLenum pname, const GLfloat * params)) \
EXT_END() \
EXT_START("GL_ARB_robustness") \
X(GLenum, GetGraphicsResetStatusARB, ()) \
X(void, GetnColorTableARB, (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void * table)) \
X(void, GetnCompressedTexImageARB, (GLenum target, GLint lod, GLsizei bufSize, void * img)) \
X(void, GetnConvolutionFilterARB, (GLenum target, GLenum format, GLenum type, GLsizei bufSize, void * image)) \
X(void, GetnHistogramARB, (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void * values)) \
X(void, GetnMapdvARB, (GLenum target, GLenum query, GLsizei bufSize, GLdouble * v)) \
X(void, GetnMapfvARB, (GLenum target, GLenum query, GLsizei bufSize, GLfloat * v)) \
X(void, GetnMapivARB, (GLenum target, GLenum query, GLsizei bufSize, GLint * v)) \
X(void, GetnMinmaxARB, (GLenum target, GLboolean reset, GLenum format, GLenum type, GLsizei bufSize, void * values)) \
X(void, GetnPixelMapfvARB, (GLenum map, GLsizei bufSize, GLfloat * values)) \
X(void, GetnPixelMapuivARB, (GLenum map, GLsizei bufSize, GLuint * values)) \
X(void, GetnPixelMapusvARB, (GLenum map, GLsizei bufSize, GLushort * values)) \
X(void, GetnPolygonStippleARB, (GLsizei bufSize, GLubyte * pattern)) \
X(void, GetnSeparableFilterARB, (GLenum target, GLenum format, GLenum type, GLsizei rowBufSize, void * row, GLsizei columnBufSize, void * column, void * span)) \
X(void, GetnTexImageARB, (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void * img)) \
X(void, GetnUniformdvARB, (GLuint program, GLint location, GLsizei bufSize, GLdouble * params)) \
X(void, GetnUniformfvARB, (GLuint program, GLint location, GLsizei bufSize, GLfloat * params)) \
X(void, GetnUniformivARB, (GLuint program, GLint location, GLsizei bufSize, GLint * params)) \
X(void, GetnUniformuivARB, (GLuint program, GLint location, GLsizei bufSize, GLuint * params)) \
X(void, ReadnPixelsARB, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void * data)) \
EXT_END() \
EXT_START("GL_ARB_sample_locations") \
X(void, EvaluateDepthValuesARB, ()) \
X(void, FramebufferSampleLocationsfvARB, (GLenum target, GLuint start, GLsizei count, const GLfloat * v)) \
X(void, NamedFramebufferSampleLocationsfvARB, (GLuint framebuffer, GLuint start, GLsizei count, const GLfloat * v)) \
EXT_END() \
EXT_START("GL_ARB_sample_shading") \
X(void, MinSampleShadingARB, (GLfloat value)) \
EXT_END() \
EXT_START("GL_ARB_shader_objects") \
X(void, AttachObjectARB, (GLhandleARB containerObj, GLhandleARB obj)) \
X(void, CompileShaderARB, (GLhandleARB shaderObj)) \
X(GLhandleARB, CreateProgramObjectARB, ()) \
X(GLhandleARB, CreateShaderObjectARB, (GLenum shaderType)) \
X(void, DeleteObjectARB, (GLhandleARB obj)) \
X(void, DetachObjectARB, (GLhandleARB containerObj, GLhandleARB attachedObj)) \
X(void, GetActiveUniformARB, (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei * length, GLint * size, GLenum * type, GLcharARB * name)) \
X(void, GetAttachedObjectsARB, (GLhandleARB containerObj, GLsizei maxCount, GLsizei * count, GLhandleARB * obj)) \
X(GLhandleARB, GetHandleARB, (GLenum pname)) \
X(void, GetInfoLogARB, (GLhandleARB obj, GLsizei maxLength, GLsizei * length, GLcharARB * infoLog)) \
X(void, GetObjectParameterfvARB, (GLhandleARB obj, GLenum pname, GLfloat * params)) \
X(void, GetObjectParameterivARB, (GLhandleARB obj, GLenum pname, GLint * params)) \
X(void, GetShaderSourceARB, (GLhandleARB obj, GLsizei maxLength, GLsizei * length, GLcharARB * source)) \
X(GLint, GetUniformLocationARB, (GLhandleARB programObj, const GLcharARB * name)) \
X(void, GetUniformfvARB, (GLhandleARB programObj, GLint location, GLfloat * params)) \
X(void, GetUniformivARB, (GLhandleARB programObj, GLint location, GLint * params)) \
X(void, LinkProgramARB, (GLhandleARB programObj)) \
X(void, ShaderSourceARB, (GLhandleARB shaderObj, GLsizei count, const GLcharARB ** string, const GLint * length)) \
X(void, Uniform1fARB, (GLint location, GLfloat v0)) \
X(void, Uniform1fvARB, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, Uniform1iARB, (GLint location, GLint v0)) \
X(void, Uniform1ivARB, (GLint location, GLsizei count, const GLint * value)) \
X(void, Uniform2fARB, (GLint location, GLfloat v0, GLfloat v1)) \
X(void, Uniform2fvARB, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, Uniform2iARB, (GLint location, GLint v0, GLint v1)) \
X(void, Uniform2ivARB, (GLint location, GLsizei count, const GLint * value)) \
X(void, Uniform3fARB, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2)) \
X(void, Uniform3fvARB, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, Uniform3iARB, (GLint location, GLint v0, GLint v1, GLint v2)) \
X(void, Uniform3ivARB, (GLint location, GLsizei count, const GLint * value)) \
X(void, Uniform4fARB, (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)) \
X(void, Uniform4fvARB, (GLint location, GLsizei count, const GLfloat * value)) \
X(void, Uniform4iARB, (GLint location, GLint v0, GLint v1, GLint v2, GLint v3)) \
X(void, Uniform4ivARB, (GLint location, GLsizei count, const GLint * value)) \
X(void, UniformMatrix2fvARB, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix3fvARB, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix4fvARB, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UseProgramObjectARB, (GLhandleARB programObj)) \
X(void, ValidateProgramARB, (GLhandleARB programObj)) \
EXT_END() \
EXT_START("GL_ARB_shading_language_include") \
X(void, CompileShaderIncludeARB, (GLuint shader, GLsizei count, const GLchar *const* path, const GLint * length)) \
X(void, DeleteNamedStringARB, (GLint namelen, const GLchar * name)) \
X(void, GetNamedStringARB, (GLint namelen, const GLchar * name, GLsizei bufSize, GLint * stringlen, GLchar * string)) \
X(void, GetNamedStringivARB, (GLint namelen, const GLchar * name, GLenum pname, GLint * params)) \
X(GLboolean, IsNamedStringARB, (GLint namelen, const GLchar * name)) \
X(void, NamedStringARB, (GLenum type, GLint namelen, const GLchar * name, GLint stringlen, const GLchar * string)) \
EXT_END() \
EXT_START("GL_ARB_sparse_buffer") \
X(void, BufferPageCommitmentARB, (GLenum target, GLintptr offset, GLsizeiptr size, GLboolean commit)) \
X(void, NamedBufferPageCommitmentARB, (GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit)) \
X(void, NamedBufferPageCommitmentEXT, (GLuint buffer, GLintptr offset, GLsizeiptr size, GLboolean commit)) \
EXT_END() \
EXT_START("GL_ARB_sparse_texture") \
X(void, TexPageCommitmentARB, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit)) \
EXT_END() \
EXT_START("GL_ARB_texture_buffer_object") \
X(void, TexBufferARB, (GLenum target, GLenum internalformat, GLuint buffer)) \
EXT_END() \
EXT_START("GL_ARB_texture_compression") \
X(void, CompressedTexImage1DARB, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * data)) \
X(void, CompressedTexImage2DARB, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * data)) \
X(void, CompressedTexImage3DARB, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * data)) \
X(void, CompressedTexSubImage1DARB, (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * data)) \
X(void, CompressedTexSubImage2DARB, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * data)) \
X(void, CompressedTexSubImage3DARB, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data)) \
X(void, GetCompressedTexImageARB, (GLenum target, GLint level, void * img)) \
EXT_END() \
EXT_START("GL_ARB_transpose_matrix") \
X(void, LoadTransposeMatrixdARB, (const GLdouble * m)) \
X(void, LoadTransposeMatrixfARB, (const GLfloat * m)) \
X(void, MultTransposeMatrixdARB, (const GLdouble * m)) \
X(void, MultTransposeMatrixfARB, (const GLfloat * m)) \
EXT_END() \
EXT_START("GL_ARB_vertex_blend") \
X(void, VertexBlendARB, (GLint count)) \
X(void, WeightPointerARB, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, WeightbvARB, (GLint size, const GLbyte * weights)) \
X(void, WeightdvARB, (GLint size, const GLdouble * weights)) \
X(void, WeightfvARB, (GLint size, const GLfloat * weights)) \
X(void, WeightivARB, (GLint size, const GLint * weights)) \
X(void, WeightsvARB, (GLint size, const GLshort * weights)) \
X(void, WeightubvARB, (GLint size, const GLubyte * weights)) \
X(void, WeightuivARB, (GLint size, const GLuint * weights)) \
X(void, WeightusvARB, (GLint size, const GLushort * weights)) \
EXT_END() \
EXT_START("GL_ARB_vertex_buffer_object") \
X(void, BindBufferARB, (GLenum target, GLuint buffer)) \
X(void, BufferDataARB, (GLenum target, GLsizeiptrARB size, const void * data, GLenum usage)) \
X(void, BufferSubDataARB, (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const void * data)) \
X(void, DeleteBuffersARB, (GLsizei n, const GLuint * buffers)) \
X(void, GenBuffersARB, (GLsizei n, GLuint * buffers)) \
X(void, GetBufferParameterivARB, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetBufferPointervARB, (GLenum target, GLenum pname, void ** params)) \
X(void, GetBufferSubDataARB, (GLenum target, GLintptrARB offset, GLsizeiptrARB size, void * data)) \
X(GLboolean, IsBufferARB, (GLuint buffer)) \
X(void *, MapBufferARB, (GLenum target, GLenum access)) \
X(GLboolean, UnmapBufferARB, (GLenum target)) \
EXT_END() \
EXT_START("GL_ARB_vertex_program") \
X(void, DisableVertexAttribArrayARB, (GLuint index)) \
X(void, EnableVertexAttribArrayARB, (GLuint index)) \
X(void, GetVertexAttribPointervARB, (GLuint index, GLenum pname, void ** pointer)) \
X(void, GetVertexAttribdvARB, (GLuint index, GLenum pname, GLdouble * params)) \
X(void, GetVertexAttribfvARB, (GLuint index, GLenum pname, GLfloat * params)) \
X(void, GetVertexAttribivARB, (GLuint index, GLenum pname, GLint * params)) \
X(void, VertexAttrib1dARB, (GLuint index, GLdouble x)) \
X(void, VertexAttrib1dvARB, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib1fARB, (GLuint index, GLfloat x)) \
X(void, VertexAttrib1fvARB, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib1sARB, (GLuint index, GLshort x)) \
X(void, VertexAttrib1svARB, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib2dARB, (GLuint index, GLdouble x, GLdouble y)) \
X(void, VertexAttrib2dvARB, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib2fARB, (GLuint index, GLfloat x, GLfloat y)) \
X(void, VertexAttrib2fvARB, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib2sARB, (GLuint index, GLshort x, GLshort y)) \
X(void, VertexAttrib2svARB, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib3dARB, (GLuint index, GLdouble x, GLdouble y, GLdouble z)) \
X(void, VertexAttrib3dvARB, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib3fARB, (GLuint index, GLfloat x, GLfloat y, GLfloat z)) \
X(void, VertexAttrib3fvARB, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib3sARB, (GLuint index, GLshort x, GLshort y, GLshort z)) \
X(void, VertexAttrib3svARB, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib4NbvARB, (GLuint index, const GLbyte * v)) \
X(void, VertexAttrib4NivARB, (GLuint index, const GLint * v)) \
X(void, VertexAttrib4NsvARB, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib4NubARB, (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w)) \
X(void, VertexAttrib4NubvARB, (GLuint index, const GLubyte * v)) \
X(void, VertexAttrib4NuivARB, (GLuint index, const GLuint * v)) \
X(void, VertexAttrib4NusvARB, (GLuint index, const GLushort * v)) \
X(void, VertexAttrib4bvARB, (GLuint index, const GLbyte * v)) \
X(void, VertexAttrib4dARB, (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, VertexAttrib4dvARB, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib4fARB, (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, VertexAttrib4fvARB, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib4ivARB, (GLuint index, const GLint * v)) \
X(void, VertexAttrib4sARB, (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w)) \
X(void, VertexAttrib4svARB, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib4ubvARB, (GLuint index, const GLubyte * v)) \
X(void, VertexAttrib4uivARB, (GLuint index, const GLuint * v)) \
X(void, VertexAttrib4usvARB, (GLuint index, const GLushort * v)) \
X(void, VertexAttribPointerARB, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer)) \
EXT_END() \
EXT_START("GL_ARB_vertex_shader") \
X(void, BindAttribLocationARB, (GLhandleARB programObj, GLuint index, const GLcharARB * name)) \
X(void, GetActiveAttribARB, (GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei * length, GLint * size, GLenum * type, GLcharARB * name)) \
X(GLint, GetAttribLocationARB, (GLhandleARB programObj, const GLcharARB * name)) \
EXT_END() \
EXT_START("GL_ARB_viewport_array") \
X(void, DepthRangeArraydvNV, (GLuint first, GLsizei count, const GLdouble * v)) \
X(void, DepthRangeIndexeddNV, (GLuint index, GLdouble n, GLdouble f)) \
EXT_END() \
EXT_START("GL_ARB_window_pos") \
X(void, WindowPos2dARB, (GLdouble x, GLdouble y)) \
X(void, WindowPos2dvARB, (const GLdouble * v)) \
X(void, WindowPos2fARB, (GLfloat x, GLfloat y)) \
X(void, WindowPos2fvARB, (const GLfloat * v)) \
X(void, WindowPos2iARB, (GLint x, GLint y)) \
X(void, WindowPos2ivARB, (const GLint * v)) \
X(void, WindowPos2sARB, (GLshort x, GLshort y)) \
X(void, WindowPos2svARB, (const GLshort * v)) \
X(void, WindowPos3dARB, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, WindowPos3dvARB, (const GLdouble * v)) \
X(void, WindowPos3fARB, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, WindowPos3fvARB, (const GLfloat * v)) \
X(void, WindowPos3iARB, (GLint x, GLint y, GLint z)) \
X(void, WindowPos3ivARB, (const GLint * v)) \
X(void, WindowPos3sARB, (GLshort x, GLshort y, GLshort z)) \
X(void, WindowPos3svARB, (const GLshort * v)) \
EXT_END() \
EXT_START("GL_ARM_shader_core_properties") \
X(void, MaxActiveShaderCoresARM, (GLuint count)) \
EXT_END() \
EXT_START("GL_ATI_draw_buffers") \
X(void, DrawBuffersATI, (GLsizei n, const GLenum * bufs)) \
EXT_END() \
EXT_START("GL_ATI_element_array") \
X(void, DrawElementArrayATI, (GLenum mode, GLsizei count)) \
X(void, DrawRangeElementArrayATI, (GLenum mode, GLuint start, GLuint end, GLsizei count)) \
X(void, ElementPointerATI, (GLenum type, const void * pointer)) \
EXT_END() \
EXT_START("GL_ATI_envmap_bumpmap") \
X(void, GetTexBumpParameterfvATI, (GLenum pname, GLfloat * param)) \
X(void, GetTexBumpParameterivATI, (GLenum pname, GLint * param)) \
X(void, TexBumpParameterfvATI, (GLenum pname, const GLfloat * param)) \
X(void, TexBumpParameterivATI, (GLenum pname, const GLint * param)) \
EXT_END() \
EXT_START("GL_ATI_fragment_shader") \
X(void, AlphaFragmentOp1ATI, (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod)) \
X(void, AlphaFragmentOp2ATI, (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod)) \
X(void, AlphaFragmentOp3ATI, (GLenum op, GLuint dst, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod)) \
X(void, BeginFragmentShaderATI, ()) \
X(void, BindFragmentShaderATI, (GLuint id)) \
X(void, ColorFragmentOp1ATI, (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod)) \
X(void, ColorFragmentOp2ATI, (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod)) \
X(void, ColorFragmentOp3ATI, (GLenum op, GLuint dst, GLuint dstMask, GLuint dstMod, GLuint arg1, GLuint arg1Rep, GLuint arg1Mod, GLuint arg2, GLuint arg2Rep, GLuint arg2Mod, GLuint arg3, GLuint arg3Rep, GLuint arg3Mod)) \
X(void, DeleteFragmentShaderATI, (GLuint id)) \
X(void, EndFragmentShaderATI, ()) \
X(GLuint, GenFragmentShadersATI, (GLuint range)) \
X(void, PassTexCoordATI, (GLuint dst, GLuint coord, GLenum swizzle)) \
X(void, SampleMapATI, (GLuint dst, GLuint interp, GLenum swizzle)) \
X(void, SetFragmentShaderConstantATI, (GLuint dst, const GLfloat * value)) \
EXT_END() \
EXT_START("GL_ATI_map_object_buffer") \
X(void *, MapObjectBufferATI, (GLuint buffer)) \
X(void, UnmapObjectBufferATI, (GLuint buffer)) \
EXT_END() \
EXT_START("GL_ATI_pn_triangles") \
X(void, PNTrianglesfATI, (GLenum pname, GLfloat param)) \
X(void, PNTrianglesiATI, (GLenum pname, GLint param)) \
EXT_END() \
EXT_START("GL_ATI_separate_stencil") \
X(void, StencilFuncSeparateATI, (GLenum frontfunc, GLenum backfunc, GLint ref, GLuint mask)) \
X(void, StencilOpSeparateATI, (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass)) \
EXT_END() \
EXT_START("GL_ATI_vertex_array_object") \
X(void, ArrayObjectATI, (GLenum array, GLint size, GLenum type, GLsizei stride, GLuint buffer, GLuint offset)) \
X(void, FreeObjectBufferATI, (GLuint buffer)) \
X(void, GetArrayObjectfvATI, (GLenum array, GLenum pname, GLfloat * params)) \
X(void, GetArrayObjectivATI, (GLenum array, GLenum pname, GLint * params)) \
X(void, GetObjectBufferfvATI, (GLuint buffer, GLenum pname, GLfloat * params)) \
X(void, GetObjectBufferivATI, (GLuint buffer, GLenum pname, GLint * params)) \
X(void, GetVariantArrayObjectfvATI, (GLuint id, GLenum pname, GLfloat * params)) \
X(void, GetVariantArrayObjectivATI, (GLuint id, GLenum pname, GLint * params)) \
X(GLboolean, IsObjectBufferATI, (GLuint buffer)) \
X(GLuint, NewObjectBufferATI, (GLsizei size, const void * pointer, GLenum usage)) \
X(void, UpdateObjectBufferATI, (GLuint buffer, GLuint offset, GLsizei size, const void * pointer, GLenum preserve)) \
X(void, VariantArrayObjectATI, (GLuint id, GLenum type, GLsizei stride, GLuint buffer, GLuint offset)) \
EXT_END() \
EXT_START("GL_ATI_vertex_attrib_array_object") \
X(void, GetVertexAttribArrayObjectfvATI, (GLuint index, GLenum pname, GLfloat * params)) \
X(void, GetVertexAttribArrayObjectivATI, (GLuint index, GLenum pname, GLint * params)) \
X(void, VertexAttribArrayObjectATI, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLuint buffer, GLuint offset)) \
EXT_END() \
EXT_START("GL_ATI_vertex_streams") \
X(void, ClientActiveVertexStreamATI, (GLenum stream)) \
X(void, NormalStream3bATI, (GLenum stream, GLbyte nx, GLbyte ny, GLbyte nz)) \
X(void, NormalStream3bvATI, (GLenum stream, const GLbyte * coords)) \
X(void, NormalStream3dATI, (GLenum stream, GLdouble nx, GLdouble ny, GLdouble nz)) \
X(void, NormalStream3dvATI, (GLenum stream, const GLdouble * coords)) \
X(void, NormalStream3fATI, (GLenum stream, GLfloat nx, GLfloat ny, GLfloat nz)) \
X(void, NormalStream3fvATI, (GLenum stream, const GLfloat * coords)) \
X(void, NormalStream3iATI, (GLenum stream, GLint nx, GLint ny, GLint nz)) \
X(void, NormalStream3ivATI, (GLenum stream, const GLint * coords)) \
X(void, NormalStream3sATI, (GLenum stream, GLshort nx, GLshort ny, GLshort nz)) \
X(void, NormalStream3svATI, (GLenum stream, const GLshort * coords)) \
X(void, VertexBlendEnvfATI, (GLenum pname, GLfloat param)) \
X(void, VertexBlendEnviATI, (GLenum pname, GLint param)) \
X(void, VertexStream1dATI, (GLenum stream, GLdouble x)) \
X(void, VertexStream1dvATI, (GLenum stream, const GLdouble * coords)) \
X(void, VertexStream1fATI, (GLenum stream, GLfloat x)) \
X(void, VertexStream1fvATI, (GLenum stream, const GLfloat * coords)) \
X(void, VertexStream1iATI, (GLenum stream, GLint x)) \
X(void, VertexStream1ivATI, (GLenum stream, const GLint * coords)) \
X(void, VertexStream1sATI, (GLenum stream, GLshort x)) \
X(void, VertexStream1svATI, (GLenum stream, const GLshort * coords)) \
X(void, VertexStream2dATI, (GLenum stream, GLdouble x, GLdouble y)) \
X(void, VertexStream2dvATI, (GLenum stream, const GLdouble * coords)) \
X(void, VertexStream2fATI, (GLenum stream, GLfloat x, GLfloat y)) \
X(void, VertexStream2fvATI, (GLenum stream, const GLfloat * coords)) \
X(void, VertexStream2iATI, (GLenum stream, GLint x, GLint y)) \
X(void, VertexStream2ivATI, (GLenum stream, const GLint * coords)) \
X(void, VertexStream2sATI, (GLenum stream, GLshort x, GLshort y)) \
X(void, VertexStream2svATI, (GLenum stream, const GLshort * coords)) \
X(void, VertexStream3dATI, (GLenum stream, GLdouble x, GLdouble y, GLdouble z)) \
X(void, VertexStream3dvATI, (GLenum stream, const GLdouble * coords)) \
X(void, VertexStream3fATI, (GLenum stream, GLfloat x, GLfloat y, GLfloat z)) \
X(void, VertexStream3fvATI, (GLenum stream, const GLfloat * coords)) \
X(void, VertexStream3iATI, (GLenum stream, GLint x, GLint y, GLint z)) \
X(void, VertexStream3ivATI, (GLenum stream, const GLint * coords)) \
X(void, VertexStream3sATI, (GLenum stream, GLshort x, GLshort y, GLshort z)) \
X(void, VertexStream3svATI, (GLenum stream, const GLshort * coords)) \
X(void, VertexStream4dATI, (GLenum stream, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, VertexStream4dvATI, (GLenum stream, const GLdouble * coords)) \
X(void, VertexStream4fATI, (GLenum stream, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, VertexStream4fvATI, (GLenum stream, const GLfloat * coords)) \
X(void, VertexStream4iATI, (GLenum stream, GLint x, GLint y, GLint z, GLint w)) \
X(void, VertexStream4ivATI, (GLenum stream, const GLint * coords)) \
X(void, VertexStream4sATI, (GLenum stream, GLshort x, GLshort y, GLshort z, GLshort w)) \
X(void, VertexStream4svATI, (GLenum stream, const GLshort * coords)) \
EXT_END() \
EXT_START("GL_EXT_EGL_image_storage") \
X(void, EGLImageTargetTexStorageEXT, (GLenum target, GLeglImageOES image, const GLint * attrib_list)) \
X(void, EGLImageTargetTextureStorageEXT, (GLuint texture, GLeglImageOES image, const GLint * attrib_list)) \
EXT_END() \
EXT_START("GL_EXT_base_instance") \
X(void, DrawArraysInstancedBaseInstanceEXT, (GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance)) \
X(void, DrawElementsInstancedBaseInstanceEXT, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLuint baseinstance)) \
X(void, DrawElementsInstancedBaseVertexBaseInstanceEXT, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance)) \
EXT_END() \
EXT_START("GL_EXT_bindable_uniform") \
X(GLint, GetUniformBufferSizeEXT, (GLuint program, GLint location)) \
X(GLintptr, GetUniformOffsetEXT, (GLuint program, GLint location)) \
X(void, UniformBufferEXT, (GLuint program, GLint location, GLuint buffer)) \
EXT_END() \
EXT_START("GL_EXT_blend_color") \
X(void, BlendColorEXT, (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)) \
EXT_END() \
EXT_START("GL_EXT_blend_equation_separate") \
X(void, BlendEquationSeparateEXT, (GLenum modeRGB, GLenum modeAlpha)) \
EXT_END() \
EXT_START("GL_EXT_blend_func_extended") \
X(void, BindFragDataLocationEXT, (GLuint program, GLuint color, const GLchar * name)) \
X(void, BindFragDataLocationIndexedEXT, (GLuint program, GLuint colorNumber, GLuint index, const GLchar * name)) \
X(GLint, GetFragDataIndexEXT, (GLuint program, const GLchar * name)) \
X(GLint, GetProgramResourceLocationIndexEXT, (GLuint program, GLenum programInterface, const GLchar * name)) \
EXT_END() \
EXT_START("GL_EXT_blend_func_separate") \
X(void, BlendFuncSeparateEXT, (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)) \
EXT_END() \
EXT_START("GL_EXT_blend_minmax") \
X(void, BlendEquationEXT, (GLenum mode)) \
EXT_END() \
EXT_START("GL_EXT_buffer_storage") \
X(void, BufferStorageEXT, (GLenum target, GLsizeiptr size, const void * data, GLbitfield flags)) \
EXT_END() \
EXT_START("GL_EXT_clear_texture") \
X(void, ClearTexImageEXT, (GLuint texture, GLint level, GLenum format, GLenum type, const void * data)) \
X(void, ClearTexSubImageEXT, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * data)) \
EXT_END() \
EXT_START("GL_EXT_clip_control") \
X(void, ClipControlEXT, (GLenum origin, GLenum depth)) \
EXT_END() \
EXT_START("GL_EXT_color_subtable") \
X(void, ColorSubTableEXT, (GLenum target, GLsizei start, GLsizei count, GLenum format, GLenum type, const void * data)) \
X(void, CopyColorSubTableEXT, (GLenum target, GLsizei start, GLint x, GLint y, GLsizei width)) \
EXT_END() \
EXT_START("GL_EXT_compiled_vertex_array") \
X(void, LockArraysEXT, (GLint first, GLsizei count)) \
X(void, UnlockArraysEXT, ()) \
EXT_END() \
EXT_START("GL_EXT_convolution") \
X(void, ConvolutionFilter1DEXT, (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void * image)) \
X(void, ConvolutionFilter2DEXT, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * image)) \
X(void, ConvolutionParameterfEXT, (GLenum target, GLenum pname, GLfloat params)) \
X(void, ConvolutionParameterfvEXT, (GLenum target, GLenum pname, const GLfloat * params)) \
X(void, ConvolutionParameteriEXT, (GLenum target, GLenum pname, GLint params)) \
X(void, ConvolutionParameterivEXT, (GLenum target, GLenum pname, const GLint * params)) \
X(void, CopyConvolutionFilter1DEXT, (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width)) \
X(void, CopyConvolutionFilter2DEXT, (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, GetConvolutionFilterEXT, (GLenum target, GLenum format, GLenum type, void * image)) \
X(void, GetConvolutionParameterfvEXT, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetConvolutionParameterivEXT, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetSeparableFilterEXT, (GLenum target, GLenum format, GLenum type, void * row, void * column, void * span)) \
X(void, SeparableFilter2DEXT, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * row, const void * column)) \
EXT_END() \
EXT_START("GL_EXT_coordinate_frame") \
X(void, Binormal3bEXT, (GLbyte bx, GLbyte by, GLbyte bz)) \
X(void, Binormal3bvEXT, (const GLbyte * v)) \
X(void, Binormal3dEXT, (GLdouble bx, GLdouble by, GLdouble bz)) \
X(void, Binormal3dvEXT, (const GLdouble * v)) \
X(void, Binormal3fEXT, (GLfloat bx, GLfloat by, GLfloat bz)) \
X(void, Binormal3fvEXT, (const GLfloat * v)) \
X(void, Binormal3iEXT, (GLint bx, GLint by, GLint bz)) \
X(void, Binormal3ivEXT, (const GLint * v)) \
X(void, Binormal3sEXT, (GLshort bx, GLshort by, GLshort bz)) \
X(void, Binormal3svEXT, (const GLshort * v)) \
X(void, BinormalPointerEXT, (GLenum type, GLsizei stride, const void * pointer)) \
X(void, Tangent3bEXT, (GLbyte tx, GLbyte ty, GLbyte tz)) \
X(void, Tangent3bvEXT, (const GLbyte * v)) \
X(void, Tangent3dEXT, (GLdouble tx, GLdouble ty, GLdouble tz)) \
X(void, Tangent3dvEXT, (const GLdouble * v)) \
X(void, Tangent3fEXT, (GLfloat tx, GLfloat ty, GLfloat tz)) \
X(void, Tangent3fvEXT, (const GLfloat * v)) \
X(void, Tangent3iEXT, (GLint tx, GLint ty, GLint tz)) \
X(void, Tangent3ivEXT, (const GLint * v)) \
X(void, Tangent3sEXT, (GLshort tx, GLshort ty, GLshort tz)) \
X(void, Tangent3svEXT, (const GLshort * v)) \
X(void, TangentPointerEXT, (GLenum type, GLsizei stride, const void * pointer)) \
EXT_END() \
EXT_START("GL_EXT_copy_image") \
X(void, CopyImageSubDataEXT, (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth)) \
EXT_END() \
EXT_START("GL_EXT_copy_texture") \
X(void, CopyTexImage1DEXT, (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)) \
X(void, CopyTexImage2DEXT, (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)) \
X(void, CopyTexSubImage1DEXT, (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)) \
X(void, CopyTexSubImage2DEXT, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, CopyTexSubImage3DEXT, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
EXT_END() \
EXT_START("GL_EXT_cull_vertex") \
X(void, CullParameterdvEXT, (GLenum pname, GLdouble * params)) \
X(void, CullParameterfvEXT, (GLenum pname, GLfloat * params)) \
EXT_END() \
EXT_START("GL_EXT_debug_label") \
X(void, GetObjectLabelEXT, (GLenum type, GLuint object, GLsizei bufSize, GLsizei * length, GLchar * label)) \
X(void, LabelObjectEXT, (GLenum type, GLuint object, GLsizei length, const GLchar * label)) \
EXT_END() \
EXT_START("GL_EXT_debug_marker") \
X(void, InsertEventMarkerEXT, (GLsizei length, const GLchar * marker)) \
X(void, PopGroupMarkerEXT, ()) \
X(void, PushGroupMarkerEXT, (GLsizei length, const GLchar * marker)) \
EXT_END() \
EXT_START("GL_EXT_depth_bounds_test") \
X(void, DepthBoundsEXT, (GLclampd zmin, GLclampd zmax)) \
EXT_END() \
EXT_START("GL_EXT_direct_state_access") \
X(void, BindMultiTextureEXT, (GLenum texunit, GLenum target, GLuint texture)) \
X(GLenum, CheckNamedFramebufferStatusEXT, (GLuint framebuffer, GLenum target)) \
X(void, ClearNamedBufferDataEXT, (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void * data)) \
X(void, ClearNamedBufferSubDataEXT, (GLuint buffer, GLenum internalformat, GLsizeiptr offset, GLsizeiptr size, GLenum format, GLenum type, const void * data)) \
X(void, ClientAttribDefaultEXT, (GLbitfield mask)) \
X(void, CompressedMultiTexImage1DEXT, (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * bits)) \
X(void, CompressedMultiTexImage2DEXT, (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * bits)) \
X(void, CompressedMultiTexImage3DEXT, (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * bits)) \
X(void, CompressedMultiTexSubImage1DEXT, (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * bits)) \
X(void, CompressedMultiTexSubImage2DEXT, (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * bits)) \
X(void, CompressedMultiTexSubImage3DEXT, (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * bits)) \
X(void, CompressedTextureImage1DEXT, (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void * bits)) \
X(void, CompressedTextureImage2DEXT, (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void * bits)) \
X(void, CompressedTextureImage3DEXT, (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * bits)) \
X(void, CompressedTextureSubImage1DEXT, (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void * bits)) \
X(void, CompressedTextureSubImage2DEXT, (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void * bits)) \
X(void, CompressedTextureSubImage3DEXT, (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * bits)) \
X(void, CopyMultiTexImage1DEXT, (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)) \
X(void, CopyMultiTexImage2DEXT, (GLenum texunit, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)) \
X(void, CopyMultiTexSubImage1DEXT, (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)) \
X(void, CopyMultiTexSubImage2DEXT, (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, CopyMultiTexSubImage3DEXT, (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, CopyTextureImage1DEXT, (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border)) \
X(void, CopyTextureImage2DEXT, (GLuint texture, GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border)) \
X(void, CopyTextureSubImage1DEXT, (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width)) \
X(void, CopyTextureSubImage2DEXT, (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, CopyTextureSubImage3DEXT, (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, DisableClientStateIndexedEXT, (GLenum array, GLuint index)) \
X(void, DisableClientStateiEXT, (GLenum array, GLuint index)) \
X(void, DisableIndexedEXT, (GLenum target, GLuint index)) \
X(void, DisableVertexArrayAttribEXT, (GLuint vaobj, GLuint index)) \
X(void, DisableVertexArrayEXT, (GLuint vaobj, GLenum array)) \
X(void, EnableClientStateIndexedEXT, (GLenum array, GLuint index)) \
X(void, EnableClientStateiEXT, (GLenum array, GLuint index)) \
X(void, EnableIndexedEXT, (GLenum target, GLuint index)) \
X(void, EnableVertexArrayAttribEXT, (GLuint vaobj, GLuint index)) \
X(void, EnableVertexArrayEXT, (GLuint vaobj, GLenum array)) \
X(void, FlushMappedNamedBufferRangeEXT, (GLuint buffer, GLintptr offset, GLsizeiptr length)) \
X(void, FramebufferDrawBufferEXT, (GLuint framebuffer, GLenum mode)) \
X(void, FramebufferDrawBuffersEXT, (GLuint framebuffer, GLsizei n, const GLenum * bufs)) \
X(void, FramebufferReadBufferEXT, (GLuint framebuffer, GLenum mode)) \
X(void, GenerateMultiTexMipmapEXT, (GLenum texunit, GLenum target)) \
X(void, GenerateTextureMipmapEXT, (GLuint texture, GLenum target)) \
X(void, GetBooleanIndexedvEXT, (GLenum target, GLuint index, GLboolean * data)) \
X(void, GetCompressedMultiTexImageEXT, (GLenum texunit, GLenum target, GLint lod, void * img)) \
X(void, GetCompressedTextureImageEXT, (GLuint texture, GLenum target, GLint lod, void * img)) \
X(void, GetDoubleIndexedvEXT, (GLenum target, GLuint index, GLdouble * data)) \
X(void, GetDoublei_vEXT, (GLenum pname, GLuint index, GLdouble * params)) \
X(void, GetFloatIndexedvEXT, (GLenum target, GLuint index, GLfloat * data)) \
X(void, GetFloati_vEXT, (GLenum pname, GLuint index, GLfloat * params)) \
X(void, GetFramebufferParameterivEXT, (GLuint framebuffer, GLenum pname, GLint * params)) \
X(void, GetIntegerIndexedvEXT, (GLenum target, GLuint index, GLint * data)) \
X(void, GetMultiTexEnvfvEXT, (GLenum texunit, GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetMultiTexEnvivEXT, (GLenum texunit, GLenum target, GLenum pname, GLint * params)) \
X(void, GetMultiTexGendvEXT, (GLenum texunit, GLenum coord, GLenum pname, GLdouble * params)) \
X(void, GetMultiTexGenfvEXT, (GLenum texunit, GLenum coord, GLenum pname, GLfloat * params)) \
X(void, GetMultiTexGenivEXT, (GLenum texunit, GLenum coord, GLenum pname, GLint * params)) \
X(void, GetMultiTexImageEXT, (GLenum texunit, GLenum target, GLint level, GLenum format, GLenum type, void * pixels)) \
X(void, GetMultiTexLevelParameterfvEXT, (GLenum texunit, GLenum target, GLint level, GLenum pname, GLfloat * params)) \
X(void, GetMultiTexLevelParameterivEXT, (GLenum texunit, GLenum target, GLint level, GLenum pname, GLint * params)) \
X(void, GetMultiTexParameterIivEXT, (GLenum texunit, GLenum target, GLenum pname, GLint * params)) \
X(void, GetMultiTexParameterIuivEXT, (GLenum texunit, GLenum target, GLenum pname, GLuint * params)) \
X(void, GetMultiTexParameterfvEXT, (GLenum texunit, GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetMultiTexParameterivEXT, (GLenum texunit, GLenum target, GLenum pname, GLint * params)) \
X(void, GetNamedBufferParameterivEXT, (GLuint buffer, GLenum pname, GLint * params)) \
X(void, GetNamedBufferPointervEXT, (GLuint buffer, GLenum pname, void ** params)) \
X(void, GetNamedBufferSubDataEXT, (GLuint buffer, GLintptr offset, GLsizeiptr size, void * data)) \
X(void, GetNamedFramebufferAttachmentParameterivEXT, (GLuint framebuffer, GLenum attachment, GLenum pname, GLint * params)) \
X(void, GetNamedFramebufferParameterivEXT, (GLuint framebuffer, GLenum pname, GLint * params)) \
X(void, GetNamedProgramLocalParameterIivEXT, (GLuint program, GLenum target, GLuint index, GLint * params)) \
X(void, GetNamedProgramLocalParameterIuivEXT, (GLuint program, GLenum target, GLuint index, GLuint * params)) \
X(void, GetNamedProgramLocalParameterdvEXT, (GLuint program, GLenum target, GLuint index, GLdouble * params)) \
X(void, GetNamedProgramLocalParameterfvEXT, (GLuint program, GLenum target, GLuint index, GLfloat * params)) \
X(void, GetNamedProgramStringEXT, (GLuint program, GLenum target, GLenum pname, void * string)) \
X(void, GetNamedProgramivEXT, (GLuint program, GLenum target, GLenum pname, GLint * params)) \
X(void, GetNamedRenderbufferParameterivEXT, (GLuint renderbuffer, GLenum pname, GLint * params)) \
X(void, GetPointerIndexedvEXT, (GLenum target, GLuint index, void ** data)) \
X(void, GetPointeri_vEXT, (GLenum pname, GLuint index, void ** params)) \
X(void, GetTextureImageEXT, (GLuint texture, GLenum target, GLint level, GLenum format, GLenum type, void * pixels)) \
X(void, GetTextureLevelParameterfvEXT, (GLuint texture, GLenum target, GLint level, GLenum pname, GLfloat * params)) \
X(void, GetTextureLevelParameterivEXT, (GLuint texture, GLenum target, GLint level, GLenum pname, GLint * params)) \
X(void, GetTextureParameterIivEXT, (GLuint texture, GLenum target, GLenum pname, GLint * params)) \
X(void, GetTextureParameterIuivEXT, (GLuint texture, GLenum target, GLenum pname, GLuint * params)) \
X(void, GetTextureParameterfvEXT, (GLuint texture, GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetTextureParameterivEXT, (GLuint texture, GLenum target, GLenum pname, GLint * params)) \
X(void, GetVertexArrayIntegeri_vEXT, (GLuint vaobj, GLuint index, GLenum pname, GLint * param)) \
X(void, GetVertexArrayIntegervEXT, (GLuint vaobj, GLenum pname, GLint * param)) \
X(void, GetVertexArrayPointeri_vEXT, (GLuint vaobj, GLuint index, GLenum pname, void ** param)) \
X(void, GetVertexArrayPointervEXT, (GLuint vaobj, GLenum pname, void ** param)) \
X(GLboolean, IsEnabledIndexedEXT, (GLenum target, GLuint index)) \
X(void *, MapNamedBufferEXT, (GLuint buffer, GLenum access)) \
X(void *, MapNamedBufferRangeEXT, (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access)) \
X(void, MatrixFrustumEXT, (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)) \
X(void, MatrixLoadIdentityEXT, (GLenum mode)) \
X(void, MatrixLoadTransposedEXT, (GLenum mode, const GLdouble * m)) \
X(void, MatrixLoadTransposefEXT, (GLenum mode, const GLfloat * m)) \
X(void, MatrixLoaddEXT, (GLenum mode, const GLdouble * m)) \
X(void, MatrixLoadfEXT, (GLenum mode, const GLfloat * m)) \
X(void, MatrixMultTransposedEXT, (GLenum mode, const GLdouble * m)) \
X(void, MatrixMultTransposefEXT, (GLenum mode, const GLfloat * m)) \
X(void, MatrixMultdEXT, (GLenum mode, const GLdouble * m)) \
X(void, MatrixMultfEXT, (GLenum mode, const GLfloat * m)) \
X(void, MatrixOrthoEXT, (GLenum mode, GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)) \
X(void, MatrixPopEXT, (GLenum mode)) \
X(void, MatrixPushEXT, (GLenum mode)) \
X(void, MatrixRotatedEXT, (GLenum mode, GLdouble angle, GLdouble x, GLdouble y, GLdouble z)) \
X(void, MatrixRotatefEXT, (GLenum mode, GLfloat angle, GLfloat x, GLfloat y, GLfloat z)) \
X(void, MatrixScaledEXT, (GLenum mode, GLdouble x, GLdouble y, GLdouble z)) \
X(void, MatrixScalefEXT, (GLenum mode, GLfloat x, GLfloat y, GLfloat z)) \
X(void, MatrixTranslatedEXT, (GLenum mode, GLdouble x, GLdouble y, GLdouble z)) \
X(void, MatrixTranslatefEXT, (GLenum mode, GLfloat x, GLfloat y, GLfloat z)) \
X(void, MultiTexBufferEXT, (GLenum texunit, GLenum target, GLenum internalformat, GLuint buffer)) \
X(void, MultiTexCoordPointerEXT, (GLenum texunit, GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, MultiTexEnvfEXT, (GLenum texunit, GLenum target, GLenum pname, GLfloat param)) \
X(void, MultiTexEnvfvEXT, (GLenum texunit, GLenum target, GLenum pname, const GLfloat * params)) \
X(void, MultiTexEnviEXT, (GLenum texunit, GLenum target, GLenum pname, GLint param)) \
X(void, MultiTexEnvivEXT, (GLenum texunit, GLenum target, GLenum pname, const GLint * params)) \
X(void, MultiTexGendEXT, (GLenum texunit, GLenum coord, GLenum pname, GLdouble param)) \
X(void, MultiTexGendvEXT, (GLenum texunit, GLenum coord, GLenum pname, const GLdouble * params)) \
X(void, MultiTexGenfEXT, (GLenum texunit, GLenum coord, GLenum pname, GLfloat param)) \
X(void, MultiTexGenfvEXT, (GLenum texunit, GLenum coord, GLenum pname, const GLfloat * params)) \
X(void, MultiTexGeniEXT, (GLenum texunit, GLenum coord, GLenum pname, GLint param)) \
X(void, MultiTexGenivEXT, (GLenum texunit, GLenum coord, GLenum pname, const GLint * params)) \
X(void, MultiTexImage1DEXT, (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, MultiTexImage2DEXT, (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, MultiTexImage3DEXT, (GLenum texunit, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, MultiTexParameterIivEXT, (GLenum texunit, GLenum target, GLenum pname, const GLint * params)) \
X(void, MultiTexParameterIuivEXT, (GLenum texunit, GLenum target, GLenum pname, const GLuint * params)) \
X(void, MultiTexParameterfEXT, (GLenum texunit, GLenum target, GLenum pname, GLfloat param)) \
X(void, MultiTexParameterfvEXT, (GLenum texunit, GLenum target, GLenum pname, const GLfloat * params)) \
X(void, MultiTexParameteriEXT, (GLenum texunit, GLenum target, GLenum pname, GLint param)) \
X(void, MultiTexParameterivEXT, (GLenum texunit, GLenum target, GLenum pname, const GLint * params)) \
X(void, MultiTexRenderbufferEXT, (GLenum texunit, GLenum target, GLuint renderbuffer)) \
X(void, MultiTexSubImage1DEXT, (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels)) \
X(void, MultiTexSubImage2DEXT, (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels)) \
X(void, MultiTexSubImage3DEXT, (GLenum texunit, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels)) \
X(void, NamedBufferDataEXT, (GLuint buffer, GLsizeiptr size, const void * data, GLenum usage)) \
X(void, NamedBufferStorageEXT, (GLuint buffer, GLsizeiptr size, const void * data, GLbitfield flags)) \
X(void, NamedBufferSubDataEXT, (GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data)) \
X(void, NamedCopyBufferSubDataEXT, (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)) \
X(void, NamedFramebufferParameteriEXT, (GLuint framebuffer, GLenum pname, GLint param)) \
X(void, NamedFramebufferRenderbufferEXT, (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)) \
X(void, NamedFramebufferTexture1DEXT, (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level)) \
X(void, NamedFramebufferTexture2DEXT, (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level)) \
X(void, NamedFramebufferTexture3DEXT, (GLuint framebuffer, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)) \
X(void, NamedFramebufferTextureEXT, (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level)) \
X(void, NamedFramebufferTextureFaceEXT, (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLenum face)) \
X(void, NamedFramebufferTextureLayerEXT, (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer)) \
X(void, NamedProgramLocalParameter4dEXT, (GLuint program, GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, NamedProgramLocalParameter4dvEXT, (GLuint program, GLenum target, GLuint index, const GLdouble * params)) \
X(void, NamedProgramLocalParameter4fEXT, (GLuint program, GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, NamedProgramLocalParameter4fvEXT, (GLuint program, GLenum target, GLuint index, const GLfloat * params)) \
X(void, NamedProgramLocalParameterI4iEXT, (GLuint program, GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w)) \
X(void, NamedProgramLocalParameterI4ivEXT, (GLuint program, GLenum target, GLuint index, const GLint * params)) \
X(void, NamedProgramLocalParameterI4uiEXT, (GLuint program, GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)) \
X(void, NamedProgramLocalParameterI4uivEXT, (GLuint program, GLenum target, GLuint index, const GLuint * params)) \
X(void, NamedProgramLocalParameters4fvEXT, (GLuint program, GLenum target, GLuint index, GLsizei count, const GLfloat * params)) \
X(void, NamedProgramLocalParametersI4ivEXT, (GLuint program, GLenum target, GLuint index, GLsizei count, const GLint * params)) \
X(void, NamedProgramLocalParametersI4uivEXT, (GLuint program, GLenum target, GLuint index, GLsizei count, const GLuint * params)) \
X(void, NamedProgramStringEXT, (GLuint program, GLenum target, GLenum format, GLsizei len, const void * string)) \
X(void, NamedRenderbufferStorageEXT, (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, NamedRenderbufferStorageMultisampleCoverageEXT, (GLuint renderbuffer, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, NamedRenderbufferStorageMultisampleEXT, (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, ProgramUniform1dEXT, (GLuint program, GLint location, GLdouble x)) \
X(void, ProgramUniform1dvEXT, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, ProgramUniform1fEXT, (GLuint program, GLint location, GLfloat v0)) \
X(void, ProgramUniform1fvEXT, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, ProgramUniform1iEXT, (GLuint program, GLint location, GLint v0)) \
X(void, ProgramUniform1ivEXT, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, ProgramUniform1uiEXT, (GLuint program, GLint location, GLuint v0)) \
X(void, ProgramUniform1uivEXT, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, ProgramUniform2dEXT, (GLuint program, GLint location, GLdouble x, GLdouble y)) \
X(void, ProgramUniform2dvEXT, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, ProgramUniform2fEXT, (GLuint program, GLint location, GLfloat v0, GLfloat v1)) \
X(void, ProgramUniform2fvEXT, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, ProgramUniform2iEXT, (GLuint program, GLint location, GLint v0, GLint v1)) \
X(void, ProgramUniform2ivEXT, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, ProgramUniform2uiEXT, (GLuint program, GLint location, GLuint v0, GLuint v1)) \
X(void, ProgramUniform2uivEXT, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, ProgramUniform3dEXT, (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z)) \
X(void, ProgramUniform3dvEXT, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, ProgramUniform3fEXT, (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2)) \
X(void, ProgramUniform3fvEXT, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, ProgramUniform3iEXT, (GLuint program, GLint location, GLint v0, GLint v1, GLint v2)) \
X(void, ProgramUniform3ivEXT, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, ProgramUniform3uiEXT, (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2)) \
X(void, ProgramUniform3uivEXT, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, ProgramUniform4dEXT, (GLuint program, GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, ProgramUniform4dvEXT, (GLuint program, GLint location, GLsizei count, const GLdouble * value)) \
X(void, ProgramUniform4fEXT, (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3)) \
X(void, ProgramUniform4fvEXT, (GLuint program, GLint location, GLsizei count, const GLfloat * value)) \
X(void, ProgramUniform4iEXT, (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3)) \
X(void, ProgramUniform4ivEXT, (GLuint program, GLint location, GLsizei count, const GLint * value)) \
X(void, ProgramUniform4uiEXT, (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)) \
X(void, ProgramUniform4uivEXT, (GLuint program, GLint location, GLsizei count, const GLuint * value)) \
X(void, ProgramUniformMatrix2dvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix2fvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix2x3dvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix2x3fvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix2x4dvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix2x4fvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix3dvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix3fvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix3x2dvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix3x2fvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix3x4dvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix3x4fvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix4dvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix4fvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix4x2dvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix4x2fvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, ProgramUniformMatrix4x3dvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble * value)) \
X(void, ProgramUniformMatrix4x3fvEXT, (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, PushClientAttribDefaultEXT, (GLbitfield mask)) \
X(void, TextureBufferEXT, (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer)) \
X(void, TextureBufferRangeEXT, (GLuint texture, GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
X(void, TextureImage1DEXT, (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, TextureImage2DEXT, (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, TextureImage3DEXT, (GLuint texture, GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, TexturePageCommitmentEXT, (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit)) \
X(void, TextureParameterIivEXT, (GLuint texture, GLenum target, GLenum pname, const GLint * params)) \
X(void, TextureParameterIuivEXT, (GLuint texture, GLenum target, GLenum pname, const GLuint * params)) \
X(void, TextureParameterfEXT, (GLuint texture, GLenum target, GLenum pname, GLfloat param)) \
X(void, TextureParameterfvEXT, (GLuint texture, GLenum target, GLenum pname, const GLfloat * params)) \
X(void, TextureParameteriEXT, (GLuint texture, GLenum target, GLenum pname, GLint param)) \
X(void, TextureParameterivEXT, (GLuint texture, GLenum target, GLenum pname, const GLint * params)) \
X(void, TextureRenderbufferEXT, (GLuint texture, GLenum target, GLuint renderbuffer)) \
X(void, TextureStorage1DEXT, (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)) \
X(void, TextureStorage2DEXT, (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, TextureStorage2DMultisampleEXT, (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations)) \
X(void, TextureStorage3DEXT, (GLuint texture, GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)) \
X(void, TextureStorage3DMultisampleEXT, (GLuint texture, GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)) \
X(void, TextureSubImage1DEXT, (GLuint texture, GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels)) \
X(void, TextureSubImage2DEXT, (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels)) \
X(void, TextureSubImage3DEXT, (GLuint texture, GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels)) \
X(GLboolean, UnmapNamedBufferEXT, (GLuint buffer)) \
X(void, VertexArrayBindVertexBufferEXT, (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride)) \
X(void, VertexArrayColorOffsetEXT, (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)) \
X(void, VertexArrayEdgeFlagOffsetEXT, (GLuint vaobj, GLuint buffer, GLsizei stride, GLintptr offset)) \
X(void, VertexArrayFogCoordOffsetEXT, (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)) \
X(void, VertexArrayIndexOffsetEXT, (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)) \
X(void, VertexArrayMultiTexCoordOffsetEXT, (GLuint vaobj, GLuint buffer, GLenum texunit, GLint size, GLenum type, GLsizei stride, GLintptr offset)) \
X(void, VertexArrayNormalOffsetEXT, (GLuint vaobj, GLuint buffer, GLenum type, GLsizei stride, GLintptr offset)) \
X(void, VertexArraySecondaryColorOffsetEXT, (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)) \
X(void, VertexArrayTexCoordOffsetEXT, (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)) \
X(void, VertexArrayVertexAttribBindingEXT, (GLuint vaobj, GLuint attribindex, GLuint bindingindex)) \
X(void, VertexArrayVertexAttribDivisorEXT, (GLuint vaobj, GLuint index, GLuint divisor)) \
X(void, VertexArrayVertexAttribFormatEXT, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset)) \
X(void, VertexArrayVertexAttribIFormatEXT, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)) \
X(void, VertexArrayVertexAttribIOffsetEXT, (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset)) \
X(void, VertexArrayVertexAttribLFormatEXT, (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset)) \
X(void, VertexArrayVertexAttribLOffsetEXT, (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLsizei stride, GLintptr offset)) \
X(void, VertexArrayVertexAttribOffsetEXT, (GLuint vaobj, GLuint buffer, GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLintptr offset)) \
X(void, VertexArrayVertexBindingDivisorEXT, (GLuint vaobj, GLuint bindingindex, GLuint divisor)) \
X(void, VertexArrayVertexOffsetEXT, (GLuint vaobj, GLuint buffer, GLint size, GLenum type, GLsizei stride, GLintptr offset)) \
EXT_END() \
EXT_START("GL_EXT_discard_framebuffer") \
X(void, DiscardFramebufferEXT, (GLenum target, GLsizei numAttachments, const GLenum * attachments)) \
EXT_END() \
EXT_START("GL_EXT_disjoint_timer_query") \
X(void, BeginQueryEXT, (GLenum target, GLuint id)) \
X(void, DeleteQueriesEXT, (GLsizei n, const GLuint * ids)) \
X(void, EndQueryEXT, (GLenum target)) \
X(void, GenQueriesEXT, (GLsizei n, GLuint * ids)) \
X(void, GetInteger64vEXT, (GLenum pname, GLint64 * data)) \
X(void, GetQueryObjecti64vEXT, (GLuint id, GLenum pname, GLint64 * params)) \
X(void, GetQueryObjectivEXT, (GLuint id, GLenum pname, GLint * params)) \
X(void, GetQueryObjectui64vEXT, (GLuint id, GLenum pname, GLuint64 * params)) \
X(void, GetQueryObjectuivEXT, (GLuint id, GLenum pname, GLuint * params)) \
X(void, GetQueryivEXT, (GLenum target, GLenum pname, GLint * params)) \
X(GLboolean, IsQueryEXT, (GLuint id)) \
X(void, QueryCounterEXT, (GLuint id, GLenum target)) \
EXT_END() \
EXT_START("GL_EXT_draw_buffers") \
X(void, DrawBuffersEXT, (GLsizei n, const GLenum * bufs)) \
EXT_END() \
EXT_START("GL_EXT_draw_buffers2") \
X(void, ColorMaskIndexedEXT, (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a)) \
EXT_END() \
EXT_START("GL_EXT_draw_buffers_indexed") \
X(void, BlendEquationSeparateiEXT, (GLuint buf, GLenum modeRGB, GLenum modeAlpha)) \
X(void, BlendEquationiEXT, (GLuint buf, GLenum mode)) \
X(void, BlendFuncSeparateiEXT, (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)) \
X(void, BlendFunciEXT, (GLuint buf, GLenum src, GLenum dst)) \
X(void, ColorMaskiEXT, (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a)) \
X(void, DisableiEXT, (GLenum target, GLuint index)) \
X(void, EnableiEXT, (GLenum target, GLuint index)) \
X(GLboolean, IsEnablediEXT, (GLenum target, GLuint index)) \
EXT_END() \
EXT_START("GL_EXT_draw_elements_base_vertex") \
X(void, DrawElementsBaseVertexEXT, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLint basevertex)) \
X(void, DrawElementsInstancedBaseVertexEXT, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex)) \
X(void, DrawRangeElementsBaseVertexEXT, (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices, GLint basevertex)) \
X(void, MultiDrawElementsBaseVertexEXT, (GLenum mode, const GLsizei * count, GLenum type, const void *const* indices, GLsizei drawcount, const GLint * basevertex)) \
EXT_END() \
EXT_START("GL_EXT_draw_instanced") \
X(void, DrawArraysInstancedEXT, (GLenum mode, GLint start, GLsizei count, GLsizei primcount)) \
X(void, DrawElementsInstancedEXT, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount)) \
EXT_END() \
EXT_START("GL_EXT_draw_range_elements") \
X(void, DrawRangeElementsEXT, (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices)) \
EXT_END() \
EXT_START("GL_EXT_draw_transform_feedback") \
X(void, DrawTransformFeedbackEXT, (GLenum mode, GLuint id)) \
X(void, DrawTransformFeedbackInstancedEXT, (GLenum mode, GLuint id, GLsizei instancecount)) \
EXT_END() \
EXT_START("GL_EXT_external_buffer") \
X(void, BufferStorageExternalEXT, (GLenum target, GLintptr offset, GLsizeiptr size, GLeglClientBufferEXT clientBuffer, GLbitfield flags)) \
X(void, NamedBufferStorageExternalEXT, (GLuint buffer, GLintptr offset, GLsizeiptr size, GLeglClientBufferEXT clientBuffer, GLbitfield flags)) \
EXT_END() \
EXT_START("GL_EXT_fog_coord") \
X(void, FogCoordPointerEXT, (GLenum type, GLsizei stride, const void * pointer)) \
X(void, FogCoorddEXT, (GLdouble coord)) \
X(void, FogCoorddvEXT, (const GLdouble * coord)) \
X(void, FogCoordfEXT, (GLfloat coord)) \
X(void, FogCoordfvEXT, (const GLfloat * coord)) \
EXT_END() \
EXT_START("GL_EXT_fragment_shading_rate") \
X(void, FramebufferShadingRateEXT, (GLenum target, GLenum attachment, GLuint texture, GLint baseLayer, GLsizei numLayers, GLsizei texelWidth, GLsizei texelHeight)) \
X(void, GetFragmentShadingRatesEXT, (GLsizei samples, GLsizei maxCount, GLsizei * count, GLenum * shadingRates)) \
X(void, ShadingRateCombinerOpsEXT, (GLenum combinerOp0, GLenum combinerOp1)) \
X(void, ShadingRateEXT, (GLenum rate)) \
EXT_END() \
EXT_START("GL_EXT_framebuffer_blit") \
X(void, BlitFramebufferEXT, (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)) \
EXT_END() \
EXT_START("GL_EXT_framebuffer_blit_layers") \
X(void, BlitFramebufferLayerEXT, (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint srcLayer, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLint dstLayer, GLbitfield mask, GLenum filter)) \
X(void, BlitFramebufferLayersEXT, (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)) \
EXT_END() \
EXT_START("GL_EXT_framebuffer_multisample") \
X(void, RenderbufferStorageMultisampleEXT, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)) \
EXT_END() \
EXT_START("GL_EXT_framebuffer_object") \
X(void, BindFramebufferEXT, (GLenum target, GLuint framebuffer)) \
X(void, BindRenderbufferEXT, (GLenum target, GLuint renderbuffer)) \
X(GLenum, CheckFramebufferStatusEXT, (GLenum target)) \
X(void, DeleteFramebuffersEXT, (GLsizei n, const GLuint * framebuffers)) \
X(void, DeleteRenderbuffersEXT, (GLsizei n, const GLuint * renderbuffers)) \
X(void, FramebufferRenderbufferEXT, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)) \
X(void, FramebufferTexture1DEXT, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)) \
X(void, FramebufferTexture2DEXT, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)) \
X(void, FramebufferTexture3DEXT, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)) \
X(void, GenFramebuffersEXT, (GLsizei n, GLuint * framebuffers)) \
X(void, GenRenderbuffersEXT, (GLsizei n, GLuint * renderbuffers)) \
X(void, GenerateMipmapEXT, (GLenum target)) \
X(void, GetFramebufferAttachmentParameterivEXT, (GLenum target, GLenum attachment, GLenum pname, GLint * params)) \
X(void, GetRenderbufferParameterivEXT, (GLenum target, GLenum pname, GLint * params)) \
X(GLboolean, IsFramebufferEXT, (GLuint framebuffer)) \
X(GLboolean, IsRenderbufferEXT, (GLuint renderbuffer)) \
X(void, RenderbufferStorageEXT, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)) \
EXT_END() \
EXT_START("GL_EXT_geometry_shader") \
X(void, FramebufferTextureEXT, (GLenum target, GLenum attachment, GLuint texture, GLint level)) \
EXT_END() \
EXT_START("GL_EXT_geometry_shader4") \
X(void, ProgramParameteriEXT, (GLuint program, GLenum pname, GLint value)) \
EXT_END() \
EXT_START("GL_EXT_gpu_program_parameters") \
X(void, ProgramEnvParameters4fvEXT, (GLenum target, GLuint index, GLsizei count, const GLfloat * params)) \
X(void, ProgramLocalParameters4fvEXT, (GLenum target, GLuint index, GLsizei count, const GLfloat * params)) \
EXT_END() \
EXT_START("GL_EXT_gpu_shader4") \
X(GLint, GetFragDataLocationEXT, (GLuint program, const GLchar * name)) \
X(void, GetUniformuivEXT, (GLuint program, GLint location, GLuint * params)) \
X(void, GetVertexAttribIivEXT, (GLuint index, GLenum pname, GLint * params)) \
X(void, GetVertexAttribIuivEXT, (GLuint index, GLenum pname, GLuint * params)) \
X(void, Uniform1uiEXT, (GLint location, GLuint v0)) \
X(void, Uniform1uivEXT, (GLint location, GLsizei count, const GLuint * value)) \
X(void, Uniform2uiEXT, (GLint location, GLuint v0, GLuint v1)) \
X(void, Uniform2uivEXT, (GLint location, GLsizei count, const GLuint * value)) \
X(void, Uniform3uiEXT, (GLint location, GLuint v0, GLuint v1, GLuint v2)) \
X(void, Uniform3uivEXT, (GLint location, GLsizei count, const GLuint * value)) \
X(void, Uniform4uiEXT, (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3)) \
X(void, Uniform4uivEXT, (GLint location, GLsizei count, const GLuint * value)) \
X(void, VertexAttribI1iEXT, (GLuint index, GLint x)) \
X(void, VertexAttribI1ivEXT, (GLuint index, const GLint * v)) \
X(void, VertexAttribI1uiEXT, (GLuint index, GLuint x)) \
X(void, VertexAttribI1uivEXT, (GLuint index, const GLuint * v)) \
X(void, VertexAttribI2iEXT, (GLuint index, GLint x, GLint y)) \
X(void, VertexAttribI2ivEXT, (GLuint index, const GLint * v)) \
X(void, VertexAttribI2uiEXT, (GLuint index, GLuint x, GLuint y)) \
X(void, VertexAttribI2uivEXT, (GLuint index, const GLuint * v)) \
X(void, VertexAttribI3iEXT, (GLuint index, GLint x, GLint y, GLint z)) \
X(void, VertexAttribI3ivEXT, (GLuint index, const GLint * v)) \
X(void, VertexAttribI3uiEXT, (GLuint index, GLuint x, GLuint y, GLuint z)) \
X(void, VertexAttribI3uivEXT, (GLuint index, const GLuint * v)) \
X(void, VertexAttribI4bvEXT, (GLuint index, const GLbyte * v)) \
X(void, VertexAttribI4iEXT, (GLuint index, GLint x, GLint y, GLint z, GLint w)) \
X(void, VertexAttribI4ivEXT, (GLuint index, const GLint * v)) \
X(void, VertexAttribI4svEXT, (GLuint index, const GLshort * v)) \
X(void, VertexAttribI4ubvEXT, (GLuint index, const GLubyte * v)) \
X(void, VertexAttribI4uiEXT, (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)) \
X(void, VertexAttribI4uivEXT, (GLuint index, const GLuint * v)) \
X(void, VertexAttribI4usvEXT, (GLuint index, const GLushort * v)) \
X(void, VertexAttribIPointerEXT, (GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer)) \
EXT_END() \
EXT_START("GL_EXT_histogram") \
X(void, GetHistogramEXT, (GLenum target, GLboolean reset, GLenum format, GLenum type, void * values)) \
X(void, GetHistogramParameterfvEXT, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetHistogramParameterivEXT, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetMinmaxEXT, (GLenum target, GLboolean reset, GLenum format, GLenum type, void * values)) \
X(void, GetMinmaxParameterfvEXT, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetMinmaxParameterivEXT, (GLenum target, GLenum pname, GLint * params)) \
X(void, HistogramEXT, (GLenum target, GLsizei width, GLenum internalformat, GLboolean sink)) \
X(void, MinmaxEXT, (GLenum target, GLenum internalformat, GLboolean sink)) \
X(void, ResetHistogramEXT, (GLenum target)) \
X(void, ResetMinmaxEXT, (GLenum target)) \
EXT_END() \
EXT_START("GL_EXT_index_func") \
X(void, IndexFuncEXT, (GLenum func, GLclampf ref)) \
EXT_END() \
EXT_START("GL_EXT_index_material") \
X(void, IndexMaterialEXT, (GLenum face, GLenum mode)) \
EXT_END() \
EXT_START("GL_EXT_instanced_arrays") \
X(void, VertexAttribDivisorEXT, (GLuint index, GLuint divisor)) \
EXT_END() \
EXT_START("GL_EXT_light_texture") \
X(void, ApplyTextureEXT, (GLenum mode)) \
X(void, TextureLightEXT, (GLenum pname)) \
X(void, TextureMaterialEXT, (GLenum face, GLenum mode)) \
EXT_END() \
EXT_START("GL_EXT_map_buffer_range") \
X(void, FlushMappedBufferRangeEXT, (GLenum target, GLintptr offset, GLsizeiptr length)) \
X(void *, MapBufferRangeEXT, (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access)) \
EXT_END() \
EXT_START("GL_EXT_memory_object") \
X(void, BufferStorageMemEXT, (GLenum target, GLsizeiptr size, GLuint memory, GLuint64 offset)) \
X(void, CreateMemoryObjectsEXT, (GLsizei n, GLuint * memoryObjects)) \
X(void, DeleteMemoryObjectsEXT, (GLsizei n, const GLuint * memoryObjects)) \
X(void, GetMemoryObjectParameterivEXT, (GLuint memoryObject, GLenum pname, GLint * params)) \
X(void, GetUnsignedBytei_vEXT, (GLenum target, GLuint index, GLubyte * data)) \
X(void, GetUnsignedBytevEXT, (GLenum pname, GLubyte * data)) \
X(GLboolean, IsMemoryObjectEXT, (GLuint memoryObject)) \
X(void, MemoryObjectParameterivEXT, (GLuint memoryObject, GLenum pname, const GLint * params)) \
X(void, NamedBufferStorageMemEXT, (GLuint buffer, GLsizeiptr size, GLuint memory, GLuint64 offset)) \
X(void, TexStorageMem1DEXT, (GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset)) \
X(void, TexStorageMem2DEXT, (GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset)) \
X(void, TexStorageMem2DMultisampleEXT, (GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)) \
X(void, TexStorageMem3DEXT, (GLenum target, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset)) \
X(void, TexStorageMem3DMultisampleEXT, (GLenum target, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)) \
X(void, TextureStorageMem1DEXT, (GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLuint memory, GLuint64 offset)) \
X(void, TextureStorageMem2DEXT, (GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLuint memory, GLuint64 offset)) \
X(void, TextureStorageMem2DMultisampleEXT, (GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)) \
X(void, TextureStorageMem3DEXT, (GLuint texture, GLsizei levels, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset)) \
X(void, TextureStorageMem3DMultisampleEXT, (GLuint texture, GLsizei samples, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations, GLuint memory, GLuint64 offset)) \
EXT_END() \
EXT_START("GL_EXT_memory_object_fd") \
X(void, ImportMemoryFdEXT, (GLuint memory, GLuint64 size, GLenum handleType, GLint fd)) \
EXT_END() \
EXT_START("GL_EXT_memory_object_win32") \
X(void, ImportMemoryWin32HandleEXT, (GLuint memory, GLuint64 size, GLenum handleType, void * handle)) \
X(void, ImportMemoryWin32NameEXT, (GLuint memory, GLuint64 size, GLenum handleType, const void * name)) \
EXT_END() \
EXT_START("GL_EXT_mesh_shader") \
X(void, DrawMeshTasksEXT, (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)) \
X(void, DrawMeshTasksIndirectEXT, (GLintptr indirect)) \
X(void, MultiDrawMeshTasksIndirectCountEXT, (GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride)) \
X(void, MultiDrawMeshTasksIndirectEXT, (GLintptr indirect, GLsizei drawcount, GLsizei stride)) \
EXT_END() \
EXT_START("GL_EXT_multi_draw_arrays") \
X(void, MultiDrawArraysEXT, (GLenum mode, const GLint * first, const GLsizei * count, GLsizei primcount)) \
X(void, MultiDrawElementsEXT, (GLenum mode, const GLsizei * count, GLenum type, const void *const* indices, GLsizei primcount)) \
EXT_END() \
EXT_START("GL_EXT_multi_draw_indirect") \
X(void, MultiDrawArraysIndirectEXT, (GLenum mode, const void * indirect, GLsizei drawcount, GLsizei stride)) \
X(void, MultiDrawElementsIndirectEXT, (GLenum mode, GLenum type, const void * indirect, GLsizei drawcount, GLsizei stride)) \
EXT_END() \
EXT_START("GL_EXT_multisample") \
X(void, SampleMaskEXT, (GLclampf value, GLboolean invert)) \
X(void, SamplePatternEXT, (GLenum pattern)) \
EXT_END() \
EXT_START("GL_EXT_multisampled_render_to_texture") \
X(void, FramebufferTexture2DMultisampleEXT, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples)) \
EXT_END() \
EXT_START("GL_EXT_multiview_draw_buffers") \
X(void, DrawBuffersIndexedEXT, (GLint n, const GLenum * location, const GLint * indices)) \
X(void, GetIntegeri_vEXT, (GLenum target, GLuint index, GLint * data)) \
X(void, ReadBufferIndexedEXT, (GLenum src, GLint index)) \
EXT_END() \
EXT_START("GL_EXT_paletted_texture") \
X(void, ColorTableEXT, (GLenum target, GLenum internalFormat, GLsizei width, GLenum format, GLenum type, const void * table)) \
X(void, GetColorTableEXT, (GLenum target, GLenum format, GLenum type, void * data)) \
X(void, GetColorTableParameterfvEXT, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetColorTableParameterivEXT, (GLenum target, GLenum pname, GLint * params)) \
EXT_END() \
EXT_START("GL_EXT_pixel_transform") \
X(void, GetPixelTransformParameterfvEXT, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetPixelTransformParameterivEXT, (GLenum target, GLenum pname, GLint * params)) \
X(void, PixelTransformParameterfEXT, (GLenum target, GLenum pname, GLfloat param)) \
X(void, PixelTransformParameterfvEXT, (GLenum target, GLenum pname, const GLfloat * params)) \
X(void, PixelTransformParameteriEXT, (GLenum target, GLenum pname, GLint param)) \
X(void, PixelTransformParameterivEXT, (GLenum target, GLenum pname, const GLint * params)) \
EXT_END() \
EXT_START("GL_EXT_point_parameters") \
X(void, PointParameterfEXT, (GLenum pname, GLfloat param)) \
X(void, PointParameterfvEXT, (GLenum pname, const GLfloat * params)) \
EXT_END() \
EXT_START("GL_EXT_polygon_offset") \
X(void, PolygonOffsetEXT, (GLfloat factor, GLfloat bias)) \
EXT_END() \
EXT_START("GL_EXT_polygon_offset_clamp") \
X(void, PolygonOffsetClampEXT, (GLfloat factor, GLfloat units, GLfloat clamp)) \
EXT_END() \
EXT_START("GL_EXT_primitive_bounding_box") \
X(void, PrimitiveBoundingBoxEXT, (GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW)) \
EXT_END() \
EXT_START("GL_EXT_provoking_vertex") \
X(void, ProvokingVertexEXT, (GLenum mode)) \
EXT_END() \
EXT_START("GL_EXT_raster_multisample") \
X(void, RasterSamplesEXT, (GLuint samples, GLboolean fixedsamplelocations)) \
EXT_END() \
EXT_START("GL_EXT_robustness") \
X(GLenum, GetGraphicsResetStatusEXT, ()) \
X(void, GetnUniformfvEXT, (GLuint program, GLint location, GLsizei bufSize, GLfloat * params)) \
X(void, GetnUniformivEXT, (GLuint program, GLint location, GLsizei bufSize, GLint * params)) \
X(void, ReadnPixelsEXT, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void * data)) \
EXT_END() \
EXT_START("GL_EXT_secondary_color") \
X(void, SecondaryColor3bEXT, (GLbyte red, GLbyte green, GLbyte blue)) \
X(void, SecondaryColor3bvEXT, (const GLbyte * v)) \
X(void, SecondaryColor3dEXT, (GLdouble red, GLdouble green, GLdouble blue)) \
X(void, SecondaryColor3dvEXT, (const GLdouble * v)) \
X(void, SecondaryColor3fEXT, (GLfloat red, GLfloat green, GLfloat blue)) \
X(void, SecondaryColor3fvEXT, (const GLfloat * v)) \
X(void, SecondaryColor3iEXT, (GLint red, GLint green, GLint blue)) \
X(void, SecondaryColor3ivEXT, (const GLint * v)) \
X(void, SecondaryColor3sEXT, (GLshort red, GLshort green, GLshort blue)) \
X(void, SecondaryColor3svEXT, (const GLshort * v)) \
X(void, SecondaryColor3ubEXT, (GLubyte red, GLubyte green, GLubyte blue)) \
X(void, SecondaryColor3ubvEXT, (const GLubyte * v)) \
X(void, SecondaryColor3uiEXT, (GLuint red, GLuint green, GLuint blue)) \
X(void, SecondaryColor3uivEXT, (const GLuint * v)) \
X(void, SecondaryColor3usEXT, (GLushort red, GLushort green, GLushort blue)) \
X(void, SecondaryColor3usvEXT, (const GLushort * v)) \
X(void, SecondaryColorPointerEXT, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
EXT_END() \
EXT_START("GL_EXT_semaphore") \
X(void, DeleteSemaphoresEXT, (GLsizei n, const GLuint * semaphores)) \
X(void, GenSemaphoresEXT, (GLsizei n, GLuint * semaphores)) \
X(void, GetSemaphoreParameterui64vEXT, (GLuint semaphore, GLenum pname, GLuint64 * params)) \
X(GLboolean, IsSemaphoreEXT, (GLuint semaphore)) \
X(void, SemaphoreParameterui64vEXT, (GLuint semaphore, GLenum pname, const GLuint64 * params)) \
X(void, SignalSemaphoreEXT, (GLuint semaphore, GLuint numBufferBarriers, const GLuint * buffers, GLuint numTextureBarriers, const GLuint * textures, const GLenum * dstLayouts)) \
X(void, WaitSemaphoreEXT, (GLuint semaphore, GLuint numBufferBarriers, const GLuint * buffers, GLuint numTextureBarriers, const GLuint * textures, const GLenum * srcLayouts)) \
EXT_END() \
EXT_START("GL_EXT_semaphore_fd") \
X(void, ImportSemaphoreFdEXT, (GLuint semaphore, GLenum handleType, GLint fd)) \
EXT_END() \
EXT_START("GL_EXT_semaphore_win32") \
X(void, ImportSemaphoreWin32HandleEXT, (GLuint semaphore, GLenum handleType, void * handle)) \
X(void, ImportSemaphoreWin32NameEXT, (GLuint semaphore, GLenum handleType, const void * name)) \
EXT_END() \
EXT_START("GL_EXT_separate_shader_objects") \
X(void, ActiveProgramEXT, (GLuint program)) \
X(void, ActiveShaderProgramEXT, (GLuint pipeline, GLuint program)) \
X(void, BindProgramPipelineEXT, (GLuint pipeline)) \
X(GLuint, CreateShaderProgramEXT, (GLenum type, const GLchar * string)) \
X(GLuint, CreateShaderProgramvEXT, (GLenum type, GLsizei count, const GLchar *const* strings)) \
X(void, DeleteProgramPipelinesEXT, (GLsizei n, const GLuint * pipelines)) \
X(void, GenProgramPipelinesEXT, (GLsizei n, GLuint * pipelines)) \
X(void, GetProgramPipelineInfoLogEXT, (GLuint pipeline, GLsizei bufSize, GLsizei * length, GLchar * infoLog)) \
X(void, GetProgramPipelineivEXT, (GLuint pipeline, GLenum pname, GLint * params)) \
X(GLboolean, IsProgramPipelineEXT, (GLuint pipeline)) \
X(void, UseProgramStagesEXT, (GLuint pipeline, GLbitfield stages, GLuint program)) \
X(void, UseShaderProgramEXT, (GLenum type, GLuint program)) \
X(void, ValidateProgramPipelineEXT, (GLuint pipeline)) \
EXT_END() \
EXT_START("GL_EXT_shader_framebuffer_fetch_non_coherent") \
X(void, FramebufferFetchBarrierEXT, ()) \
EXT_END() \
EXT_START("GL_EXT_shader_image_load_store") \
X(void, BindImageTextureEXT, (GLuint index, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLint format)) \
X(void, MemoryBarrierEXT, (GLbitfield barriers)) \
EXT_END() \
EXT_START("GL_EXT_shader_pixel_local_storage2") \
X(void, ClearPixelLocalStorageuiEXT, (GLsizei offset, GLsizei n, const GLuint * values)) \
X(void, FramebufferPixelLocalStorageSizeEXT, (GLuint target, GLsizei size)) \
X(GLsizei, GetFramebufferPixelLocalStorageSizeEXT, (GLuint target)) \
EXT_END() \
EXT_START("GL_EXT_sparse_texture") \
X(void, TexPageCommitmentEXT, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLboolean commit)) \
EXT_END() \
EXT_START("GL_EXT_stencil_clear_tag") \
X(void, StencilClearTagEXT, (GLsizei stencilTagBits, GLuint stencilClearTag)) \
EXT_END() \
EXT_START("GL_EXT_stencil_two_side") \
X(void, ActiveStencilFaceEXT, (GLenum face)) \
EXT_END() \
EXT_START("GL_EXT_subtexture") \
X(void, TexSubImage1DEXT, (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void * pixels)) \
X(void, TexSubImage2DEXT, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void * pixels)) \
EXT_END() \
EXT_START("GL_EXT_tessellation_shader") \
X(void, PatchParameteriEXT, (GLenum pname, GLint value)) \
EXT_END() \
EXT_START("GL_EXT_texture3D") \
X(void, TexImage3DEXT, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, TexSubImage3DEXT, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels)) \
EXT_END() \
EXT_START("GL_EXT_texture_array") \
X(void, FramebufferTextureLayerEXT, (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer)) \
EXT_END() \
EXT_START("GL_EXT_texture_border_clamp") \
X(void, GetSamplerParameterIivEXT, (GLuint sampler, GLenum pname, GLint * params)) \
X(void, GetSamplerParameterIuivEXT, (GLuint sampler, GLenum pname, GLuint * params)) \
X(void, GetTexParameterIivEXT, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetTexParameterIuivEXT, (GLenum target, GLenum pname, GLuint * params)) \
X(void, SamplerParameterIivEXT, (GLuint sampler, GLenum pname, const GLint * param)) \
X(void, SamplerParameterIuivEXT, (GLuint sampler, GLenum pname, const GLuint * param)) \
X(void, TexParameterIivEXT, (GLenum target, GLenum pname, const GLint * params)) \
X(void, TexParameterIuivEXT, (GLenum target, GLenum pname, const GLuint * params)) \
EXT_END() \
EXT_START("GL_EXT_texture_buffer") \
X(void, TexBufferEXT, (GLenum target, GLenum internalformat, GLuint buffer)) \
X(void, TexBufferRangeEXT, (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
EXT_END() \
EXT_START("GL_EXT_texture_integer") \
X(void, ClearColorIiEXT, (GLint red, GLint green, GLint blue, GLint alpha)) \
X(void, ClearColorIuiEXT, (GLuint red, GLuint green, GLuint blue, GLuint alpha)) \
EXT_END() \
EXT_START("GL_EXT_texture_object") \
X(GLboolean, AreTexturesResidentEXT, (GLsizei n, const GLuint * textures, GLboolean * residences)) \
X(void, BindTextureEXT, (GLenum target, GLuint texture)) \
X(void, DeleteTexturesEXT, (GLsizei n, const GLuint * textures)) \
X(void, GenTexturesEXT, (GLsizei n, GLuint * textures)) \
X(GLboolean, IsTextureEXT, (GLuint texture)) \
X(void, PrioritizeTexturesEXT, (GLsizei n, const GLuint * textures, const GLclampf * priorities)) \
EXT_END() \
EXT_START("GL_EXT_texture_perturb_normal") \
X(void, TextureNormalEXT, (GLenum mode)) \
EXT_END() \
EXT_START("GL_EXT_texture_storage") \
X(void, TexStorage1DEXT, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width)) \
X(void, TexStorage2DEXT, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height)) \
X(void, TexStorage3DEXT, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth)) \
EXT_END() \
EXT_START("GL_EXT_texture_storage_compression") \
X(void, TexStorageAttribs2DEXT, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, const GLint * attrib_list)) \
X(void, TexStorageAttribs3DEXT, (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, const GLint * attrib_list)) \
EXT_END() \
EXT_START("GL_EXT_texture_view") \
X(void, TextureViewEXT, (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers)) \
EXT_END() \
EXT_START("GL_EXT_transform_feedback") \
X(void, BeginTransformFeedbackEXT, (GLenum primitiveMode)) \
X(void, BindBufferBaseEXT, (GLenum target, GLuint index, GLuint buffer)) \
X(void, BindBufferOffsetEXT, (GLenum target, GLuint index, GLuint buffer, GLintptr offset)) \
X(void, BindBufferRangeEXT, (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
X(void, EndTransformFeedbackEXT, ()) \
X(void, GetTransformFeedbackVaryingEXT, (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name)) \
X(void, TransformFeedbackVaryingsEXT, (GLuint program, GLsizei count, const GLchar *const* varyings, GLenum bufferMode)) \
EXT_END() \
EXT_START("GL_EXT_vertex_array") \
X(void, ArrayElementEXT, (GLint i)) \
X(void, ColorPointerEXT, (GLint size, GLenum type, GLsizei stride, GLsizei count, const void * pointer)) \
X(void, DrawArraysEXT, (GLenum mode, GLint first, GLsizei count)) \
X(void, EdgeFlagPointerEXT, (GLsizei stride, GLsizei count, const GLboolean * pointer)) \
X(void, GetPointervEXT, (GLenum pname, void ** params)) \
X(void, IndexPointerEXT, (GLenum type, GLsizei stride, GLsizei count, const void * pointer)) \
X(void, NormalPointerEXT, (GLenum type, GLsizei stride, GLsizei count, const void * pointer)) \
X(void, TexCoordPointerEXT, (GLint size, GLenum type, GLsizei stride, GLsizei count, const void * pointer)) \
X(void, VertexPointerEXT, (GLint size, GLenum type, GLsizei stride, GLsizei count, const void * pointer)) \
EXT_END() \
EXT_START("GL_EXT_vertex_attrib_64bit") \
X(void, GetVertexAttribLdvEXT, (GLuint index, GLenum pname, GLdouble * params)) \
X(void, VertexAttribL1dEXT, (GLuint index, GLdouble x)) \
X(void, VertexAttribL1dvEXT, (GLuint index, const GLdouble * v)) \
X(void, VertexAttribL2dEXT, (GLuint index, GLdouble x, GLdouble y)) \
X(void, VertexAttribL2dvEXT, (GLuint index, const GLdouble * v)) \
X(void, VertexAttribL3dEXT, (GLuint index, GLdouble x, GLdouble y, GLdouble z)) \
X(void, VertexAttribL3dvEXT, (GLuint index, const GLdouble * v)) \
X(void, VertexAttribL4dEXT, (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, VertexAttribL4dvEXT, (GLuint index, const GLdouble * v)) \
X(void, VertexAttribLPointerEXT, (GLuint index, GLint size, GLenum type, GLsizei stride, const void * pointer)) \
EXT_END() \
EXT_START("GL_EXT_vertex_shader") \
X(void, BeginVertexShaderEXT, ()) \
X(GLuint, BindLightParameterEXT, (GLenum light, GLenum value)) \
X(GLuint, BindMaterialParameterEXT, (GLenum face, GLenum value)) \
X(GLuint, BindParameterEXT, (GLenum value)) \
X(GLuint, BindTexGenParameterEXT, (GLenum unit, GLenum coord, GLenum value)) \
X(GLuint, BindTextureUnitParameterEXT, (GLenum unit, GLenum value)) \
X(void, BindVertexShaderEXT, (GLuint id)) \
X(void, DeleteVertexShaderEXT, (GLuint id)) \
X(void, DisableVariantClientStateEXT, (GLuint id)) \
X(void, EnableVariantClientStateEXT, (GLuint id)) \
X(void, EndVertexShaderEXT, ()) \
X(void, ExtractComponentEXT, (GLuint res, GLuint src, GLuint num)) \
X(GLuint, GenSymbolsEXT, (GLenum datatype, GLenum storagetype, GLenum range, GLuint components)) \
X(GLuint, GenVertexShadersEXT, (GLuint range)) \
X(void, GetInvariantBooleanvEXT, (GLuint id, GLenum value, GLboolean * data)) \
X(void, GetInvariantFloatvEXT, (GLuint id, GLenum value, GLfloat * data)) \
X(void, GetInvariantIntegervEXT, (GLuint id, GLenum value, GLint * data)) \
X(void, GetLocalConstantBooleanvEXT, (GLuint id, GLenum value, GLboolean * data)) \
X(void, GetLocalConstantFloatvEXT, (GLuint id, GLenum value, GLfloat * data)) \
X(void, GetLocalConstantIntegervEXT, (GLuint id, GLenum value, GLint * data)) \
X(void, GetVariantBooleanvEXT, (GLuint id, GLenum value, GLboolean * data)) \
X(void, GetVariantFloatvEXT, (GLuint id, GLenum value, GLfloat * data)) \
X(void, GetVariantIntegervEXT, (GLuint id, GLenum value, GLint * data)) \
X(void, GetVariantPointervEXT, (GLuint id, GLenum value, void ** data)) \
X(void, InsertComponentEXT, (GLuint res, GLuint src, GLuint num)) \
X(GLboolean, IsVariantEnabledEXT, (GLuint id, GLenum cap)) \
X(void, SetInvariantEXT, (GLuint id, GLenum type, const void * addr)) \
X(void, SetLocalConstantEXT, (GLuint id, GLenum type, const void * addr)) \
X(void, ShaderOp1EXT, (GLenum op, GLuint res, GLuint arg1)) \
X(void, ShaderOp2EXT, (GLenum op, GLuint res, GLuint arg1, GLuint arg2)) \
X(void, ShaderOp3EXT, (GLenum op, GLuint res, GLuint arg1, GLuint arg2, GLuint arg3)) \
X(void, SwizzleEXT, (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW)) \
X(void, VariantPointerEXT, (GLuint id, GLenum type, GLuint stride, const void * addr)) \
X(void, VariantbvEXT, (GLuint id, const GLbyte * addr)) \
X(void, VariantdvEXT, (GLuint id, const GLdouble * addr)) \
X(void, VariantfvEXT, (GLuint id, const GLfloat * addr)) \
X(void, VariantivEXT, (GLuint id, const GLint * addr)) \
X(void, VariantsvEXT, (GLuint id, const GLshort * addr)) \
X(void, VariantubvEXT, (GLuint id, const GLubyte * addr)) \
X(void, VariantuivEXT, (GLuint id, const GLuint * addr)) \
X(void, VariantusvEXT, (GLuint id, const GLushort * addr)) \
X(void, WriteMaskEXT, (GLuint res, GLuint in, GLenum outX, GLenum outY, GLenum outZ, GLenum outW)) \
EXT_END() \
EXT_START("GL_EXT_vertex_weighting") \
X(void, VertexWeightPointerEXT, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, VertexWeightfEXT, (GLfloat weight)) \
X(void, VertexWeightfvEXT, (const GLfloat * weight)) \
EXT_END() \
EXT_START("GL_EXT_win32_keyed_mutex") \
X(GLboolean, AcquireKeyedMutexWin32EXT, (GLuint memory, GLuint64 key, GLuint timeout)) \
X(GLboolean, ReleaseKeyedMutexWin32EXT, (GLuint memory, GLuint64 key)) \
EXT_END() \
EXT_START("GL_EXT_window_rectangles") \
X(void, WindowRectanglesEXT, (GLenum mode, GLsizei count, const GLint * box)) \
EXT_END() \
EXT_START("GL_EXT_x11_sync_object") \
X(GLsync, ImportSyncEXT, (GLenum external_sync_type, GLintptr external_sync, GLbitfield flags)) \
EXT_END() \
EXT_START("GL_GREMEDY_frame_terminator") \
X(void, FrameTerminatorGREMEDY, ()) \
EXT_END() \
EXT_START("GL_GREMEDY_string_marker") \
X(void, StringMarkerGREMEDY, (GLsizei len, const void * string)) \
EXT_END() \
EXT_START("GL_HP_image_transform") \
X(void, GetImageTransformParameterfvHP, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetImageTransformParameterivHP, (GLenum target, GLenum pname, GLint * params)) \
X(void, ImageTransformParameterfHP, (GLenum target, GLenum pname, GLfloat param)) \
X(void, ImageTransformParameterfvHP, (GLenum target, GLenum pname, const GLfloat * params)) \
X(void, ImageTransformParameteriHP, (GLenum target, GLenum pname, GLint param)) \
X(void, ImageTransformParameterivHP, (GLenum target, GLenum pname, const GLint * params)) \
EXT_END() \
EXT_START("GL_IBM_multimode_draw_arrays") \
X(void, MultiModeDrawArraysIBM, (const GLenum * mode, const GLint * first, const GLsizei * count, GLsizei primcount, GLint modestride)) \
X(void, MultiModeDrawElementsIBM, (const GLenum * mode, const GLsizei * count, GLenum type, const void *const* indices, GLsizei primcount, GLint modestride)) \
EXT_END() \
EXT_START("GL_IBM_static_data") \
X(void, FlushStaticDataIBM, (GLenum target)) \
EXT_END() \
EXT_START("GL_IBM_vertex_array_lists") \
X(void, ColorPointerListIBM, (GLint size, GLenum type, GLint stride, const void ** pointer, GLint ptrstride)) \
X(void, EdgeFlagPointerListIBM, (GLint stride, const GLboolean ** pointer, GLint ptrstride)) \
X(void, FogCoordPointerListIBM, (GLenum type, GLint stride, const void ** pointer, GLint ptrstride)) \
X(void, IndexPointerListIBM, (GLenum type, GLint stride, const void ** pointer, GLint ptrstride)) \
X(void, NormalPointerListIBM, (GLenum type, GLint stride, const void ** pointer, GLint ptrstride)) \
X(void, SecondaryColorPointerListIBM, (GLint size, GLenum type, GLint stride, const void ** pointer, GLint ptrstride)) \
X(void, TexCoordPointerListIBM, (GLint size, GLenum type, GLint stride, const void ** pointer, GLint ptrstride)) \
X(void, VertexPointerListIBM, (GLint size, GLenum type, GLint stride, const void ** pointer, GLint ptrstride)) \
EXT_END() \
EXT_START("GL_IMG_bindless_texture") \
X(GLuint64, GetTextureHandleIMG, (GLuint texture)) \
X(GLuint64, GetTextureSamplerHandleIMG, (GLuint texture, GLuint sampler)) \
X(void, ProgramUniformHandleui64IMG, (GLuint program, GLint location, GLuint64 value)) \
X(void, ProgramUniformHandleui64vIMG, (GLuint program, GLint location, GLsizei count, const GLuint64 * values)) \
X(void, UniformHandleui64IMG, (GLint location, GLuint64 value)) \
X(void, UniformHandleui64vIMG, (GLint location, GLsizei count, const GLuint64 * value)) \
EXT_END() \
EXT_START("GL_IMG_framebuffer_downsample") \
X(void, FramebufferTexture2DDownsampleIMG, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint xscale, GLint yscale)) \
X(void, FramebufferTextureLayerDownsampleIMG, (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer, GLint xscale, GLint yscale)) \
EXT_END() \
EXT_START("GL_IMG_multisampled_render_to_texture") \
X(void, FramebufferTexture2DMultisampleIMG, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLsizei samples)) \
X(void, RenderbufferStorageMultisampleIMG, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)) \
EXT_END() \
EXT_START("GL_IMG_user_clip_plane") \
X(void, ClipPlanefIMG, (GLenum p, const GLfloat * eqn)) \
X(void, ClipPlanexIMG, (GLenum p, const GLfixed * eqn)) \
EXT_END() \
EXT_START("GL_INGR_blend_func_separate") \
X(void, BlendFuncSeparateINGR, (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha)) \
EXT_END() \
EXT_START("GL_INTEL_framebuffer_CMAA") \
X(void, ApplyFramebufferAttachmentCMAAINTEL, ()) \
EXT_END() \
EXT_START("GL_INTEL_map_texture") \
X(void *, MapTexture2DINTEL, (GLuint texture, GLint level, GLbitfield access, GLint * stride, GLenum * layout)) \
X(void, SyncTextureINTEL, (GLuint texture)) \
X(void, UnmapTexture2DINTEL, (GLuint texture, GLint level)) \
EXT_END() \
EXT_START("GL_INTEL_parallel_arrays") \
X(void, ColorPointervINTEL, (GLint size, GLenum type, const void ** pointer)) \
X(void, NormalPointervINTEL, (GLenum type, const void ** pointer)) \
X(void, TexCoordPointervINTEL, (GLint size, GLenum type, const void ** pointer)) \
X(void, VertexPointervINTEL, (GLint size, GLenum type, const void ** pointer)) \
EXT_END() \
EXT_START("GL_INTEL_performance_query") \
X(void, BeginPerfQueryINTEL, (GLuint queryHandle)) \
X(void, CreatePerfQueryINTEL, (GLuint queryId, GLuint * queryHandle)) \
X(void, DeletePerfQueryINTEL, (GLuint queryHandle)) \
X(void, EndPerfQueryINTEL, (GLuint queryHandle)) \
X(void, GetFirstPerfQueryIdINTEL, (GLuint * queryId)) \
X(void, GetNextPerfQueryIdINTEL, (GLuint queryId, GLuint * nextQueryId)) \
X(void, GetPerfCounterInfoINTEL, (GLuint queryId, GLuint counterId, GLuint counterNameLength, GLchar * counterName, GLuint counterDescLength, GLchar * counterDesc, GLuint * counterOffset, GLuint * counterDataSize, GLuint * counterTypeEnum, GLuint * counterDataTypeEnum, GLuint64 * rawCounterMaxValue)) \
X(void, GetPerfQueryDataINTEL, (GLuint queryHandle, GLuint flags, GLsizei dataSize, void * data, GLuint * bytesWritten)) \
X(void, GetPerfQueryIdByNameINTEL, (GLchar * queryName, GLuint * queryId)) \
X(void, GetPerfQueryInfoINTEL, (GLuint queryId, GLuint queryNameLength, GLchar * queryName, GLuint * dataSize, GLuint * noCounters, GLuint * noInstances, GLuint * capsMask)) \
EXT_END() \
EXT_START("GL_KHR_blend_equation_advanced") \
X(void, BlendBarrierKHR, ()) \
EXT_END() \
EXT_START("GL_KHR_debug") \
X(void, DebugMessageCallbackKHR, (GLDEBUGPROCKHR callback, const void * userParam)) \
X(void, DebugMessageControlKHR, (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint * ids, GLboolean enabled)) \
X(void, DebugMessageInsertKHR, (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * buf)) \
X(GLuint, GetDebugMessageLogKHR, (GLuint count, GLsizei bufSize, GLenum * sources, GLenum * types, GLuint * ids, GLenum * severities, GLsizei * lengths, GLchar * messageLog)) \
X(void, GetObjectLabelKHR, (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei * length, GLchar * label)) \
X(void, GetObjectPtrLabelKHR, (const void * ptr, GLsizei bufSize, GLsizei * length, GLchar * label)) \
X(void, GetPointervKHR, (GLenum pname, void ** params)) \
X(void, ObjectLabelKHR, (GLenum identifier, GLuint name, GLsizei length, const GLchar * label)) \
X(void, ObjectPtrLabelKHR, (const void * ptr, GLsizei length, const GLchar * label)) \
X(void, PopDebugGroupKHR, ()) \
X(void, PushDebugGroupKHR, (GLenum source, GLuint id, GLsizei length, const GLchar * message)) \
EXT_END() \
EXT_START("GL_KHR_parallel_shader_compile") \
X(void, MaxShaderCompilerThreadsKHR, (GLuint count)) \
EXT_END() \
EXT_START("GL_KHR_robustness") \
X(GLenum, GetGraphicsResetStatusKHR, ()) \
X(void, GetnUniformfvKHR, (GLuint program, GLint location, GLsizei bufSize, GLfloat * params)) \
X(void, GetnUniformivKHR, (GLuint program, GLint location, GLsizei bufSize, GLint * params)) \
X(void, GetnUniformuivKHR, (GLuint program, GLint location, GLsizei bufSize, GLuint * params)) \
X(void, ReadnPixelsKHR, (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void * data)) \
EXT_END() \
EXT_START("GL_MESA_framebuffer_flip_y") \
X(void, FramebufferParameteriMESA, (GLenum target, GLenum pname, GLint param)) \
X(void, GetFramebufferParameterivMESA, (GLenum target, GLenum pname, GLint * params)) \
EXT_END() \
EXT_START("GL_MESA_resize_buffers") \
X(void, ResizeBuffersMESA, ()) \
EXT_END() \
EXT_START("GL_MESA_window_pos") \
X(void, WindowPos2dMESA, (GLdouble x, GLdouble y)) \
X(void, WindowPos2dvMESA, (const GLdouble * v)) \
X(void, WindowPos2fMESA, (GLfloat x, GLfloat y)) \
X(void, WindowPos2fvMESA, (const GLfloat * v)) \
X(void, WindowPos2iMESA, (GLint x, GLint y)) \
X(void, WindowPos2ivMESA, (const GLint * v)) \
X(void, WindowPos2sMESA, (GLshort x, GLshort y)) \
X(void, WindowPos2svMESA, (const GLshort * v)) \
X(void, WindowPos3dMESA, (GLdouble x, GLdouble y, GLdouble z)) \
X(void, WindowPos3dvMESA, (const GLdouble * v)) \
X(void, WindowPos3fMESA, (GLfloat x, GLfloat y, GLfloat z)) \
X(void, WindowPos3fvMESA, (const GLfloat * v)) \
X(void, WindowPos3iMESA, (GLint x, GLint y, GLint z)) \
X(void, WindowPos3ivMESA, (const GLint * v)) \
X(void, WindowPos3sMESA, (GLshort x, GLshort y, GLshort z)) \
X(void, WindowPos3svMESA, (const GLshort * v)) \
X(void, WindowPos4dMESA, (GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, WindowPos4dvMESA, (const GLdouble * v)) \
X(void, WindowPos4fMESA, (GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, WindowPos4fvMESA, (const GLfloat * v)) \
X(void, WindowPos4iMESA, (GLint x, GLint y, GLint z, GLint w)) \
X(void, WindowPos4ivMESA, (const GLint * v)) \
X(void, WindowPos4sMESA, (GLshort x, GLshort y, GLshort z, GLshort w)) \
X(void, WindowPos4svMESA, (const GLshort * v)) \
EXT_END() \
EXT_START("GL_NVX_conditional_render") \
X(void, BeginConditionalRenderNVX, (GLuint id)) \
X(void, EndConditionalRenderNVX, ()) \
EXT_END() \
EXT_START("GL_NVX_gpu_multicast2") \
X(GLuint, AsyncCopyBufferSubDataNVX, (GLsizei waitSemaphoreCount, const GLuint * waitSemaphoreArray, const GLuint64 * fenceValueArray, GLuint readGpu, GLbitfield writeGpuMask, GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size, GLsizei signalSemaphoreCount, const GLuint * signalSemaphoreArray, const GLuint64 * signalValueArray)) \
X(GLuint, AsyncCopyImageSubDataNVX, (GLsizei waitSemaphoreCount, const GLuint * waitSemaphoreArray, const GLuint64 * waitValueArray, GLuint srcGpu, GLbitfield dstGpuMask, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth, GLsizei signalSemaphoreCount, const GLuint * signalSemaphoreArray, const GLuint64 * signalValueArray)) \
X(void, MulticastScissorArrayvNVX, (GLuint gpu, GLuint first, GLsizei count, const GLint * v)) \
X(void, MulticastViewportArrayvNVX, (GLuint gpu, GLuint first, GLsizei count, const GLfloat * v)) \
X(void, MulticastViewportPositionWScaleNVX, (GLuint gpu, GLuint index, GLfloat xcoeff, GLfloat ycoeff)) \
X(void, UploadGpuMaskNVX, (GLbitfield mask)) \
EXT_END() \
EXT_START("GL_NVX_linked_gpu_multicast") \
X(void, LGPUCopyImageSubDataNVX, (GLuint sourceGpu, GLbitfield destinationGpuMask, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srxY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth)) \
X(void, LGPUInterlockNVX, ()) \
X(void, LGPUNamedBufferSubDataNVX, (GLbitfield gpuMask, GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data)) \
EXT_END() \
EXT_START("GL_NVX_progress_fence") \
X(void, ClientWaitSemaphoreui64NVX, (GLsizei fenceObjectCount, const GLuint * semaphoreArray, const GLuint64 * fenceValueArray)) \
X(GLuint, CreateProgressFenceNVX, ()) \
X(void, SignalSemaphoreui64NVX, (GLuint signalGpu, GLsizei fenceObjectCount, const GLuint * semaphoreArray, const GLuint64 * fenceValueArray)) \
X(void, WaitSemaphoreui64NVX, (GLuint waitGpu, GLsizei fenceObjectCount, const GLuint * semaphoreArray, const GLuint64 * fenceValueArray)) \
EXT_END() \
EXT_START("GL_NV_alpha_to_coverage_dither_control") \
X(void, AlphaToCoverageDitherControlNV, (GLenum mode)) \
EXT_END() \
EXT_START("GL_NV_bindless_multi_draw_indirect") \
X(void, MultiDrawArraysIndirectBindlessNV, (GLenum mode, const void * indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount)) \
X(void, MultiDrawElementsIndirectBindlessNV, (GLenum mode, GLenum type, const void * indirect, GLsizei drawCount, GLsizei stride, GLint vertexBufferCount)) \
EXT_END() \
EXT_START("GL_NV_bindless_multi_draw_indirect_count") \
X(void, MultiDrawArraysIndirectBindlessCountNV, (GLenum mode, const void * indirect, GLsizei drawCount, GLsizei maxDrawCount, GLsizei stride, GLint vertexBufferCount)) \
X(void, MultiDrawElementsIndirectBindlessCountNV, (GLenum mode, GLenum type, const void * indirect, GLsizei drawCount, GLsizei maxDrawCount, GLsizei stride, GLint vertexBufferCount)) \
EXT_END() \
EXT_START("GL_NV_bindless_texture") \
X(GLuint64, GetImageHandleNV, (GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum format)) \
X(GLuint64, GetTextureHandleNV, (GLuint texture)) \
X(GLuint64, GetTextureSamplerHandleNV, (GLuint texture, GLuint sampler)) \
X(GLboolean, IsImageHandleResidentNV, (GLuint64 handle)) \
X(GLboolean, IsTextureHandleResidentNV, (GLuint64 handle)) \
X(void, MakeImageHandleNonResidentNV, (GLuint64 handle)) \
X(void, MakeImageHandleResidentNV, (GLuint64 handle, GLenum access)) \
X(void, MakeTextureHandleNonResidentNV, (GLuint64 handle)) \
X(void, MakeTextureHandleResidentNV, (GLuint64 handle)) \
X(void, ProgramUniformHandleui64NV, (GLuint program, GLint location, GLuint64 value)) \
X(void, ProgramUniformHandleui64vNV, (GLuint program, GLint location, GLsizei count, const GLuint64 * values)) \
X(void, UniformHandleui64NV, (GLint location, GLuint64 value)) \
X(void, UniformHandleui64vNV, (GLint location, GLsizei count, const GLuint64 * value)) \
EXT_END() \
EXT_START("GL_NV_blend_equation_advanced") \
X(void, BlendBarrierNV, ()) \
X(void, BlendParameteriNV, (GLenum pname, GLint value)) \
EXT_END() \
EXT_START("GL_NV_clip_space_w_scaling") \
X(void, ViewportPositionWScaleNV, (GLuint index, GLfloat xcoeff, GLfloat ycoeff)) \
EXT_END() \
EXT_START("GL_NV_command_list") \
X(void, CallCommandListNV, (GLuint list)) \
X(void, CommandListSegmentsNV, (GLuint list, GLuint segments)) \
X(void, CompileCommandListNV, (GLuint list)) \
X(void, CreateCommandListsNV, (GLsizei n, GLuint * lists)) \
X(void, CreateStatesNV, (GLsizei n, GLuint * states)) \
X(void, DeleteCommandListsNV, (GLsizei n, const GLuint * lists)) \
X(void, DeleteStatesNV, (GLsizei n, const GLuint * states)) \
X(void, DrawCommandsAddressNV, (GLenum primitiveMode, const GLuint64 * indirects, const GLsizei * sizes, GLuint count)) \
X(void, DrawCommandsNV, (GLenum primitiveMode, GLuint buffer, const GLintptr * indirects, const GLsizei * sizes, GLuint count)) \
X(void, DrawCommandsStatesAddressNV, (const GLuint64 * indirects, const GLsizei * sizes, const GLuint * states, const GLuint * fbos, GLuint count)) \
X(void, DrawCommandsStatesNV, (GLuint buffer, const GLintptr * indirects, const GLsizei * sizes, const GLuint * states, const GLuint * fbos, GLuint count)) \
X(GLuint, GetCommandHeaderNV, (GLenum tokenID, GLuint size)) \
X(GLushort, GetStageIndexNV, (GLenum shadertype)) \
X(GLboolean, IsCommandListNV, (GLuint list)) \
X(GLboolean, IsStateNV, (GLuint state)) \
X(void, ListDrawCommandsStatesClientNV, (GLuint list, GLuint segment, const void ** indirects, const GLsizei * sizes, const GLuint * states, const GLuint * fbos, GLuint count)) \
X(void, StateCaptureNV, (GLuint state, GLenum mode)) \
EXT_END() \
EXT_START("GL_NV_conditional_render") \
X(void, BeginConditionalRenderNV, (GLuint id, GLenum mode)) \
X(void, EndConditionalRenderNV, ()) \
EXT_END() \
EXT_START("GL_NV_conservative_raster") \
X(void, SubpixelPrecisionBiasNV, (GLuint xbits, GLuint ybits)) \
EXT_END() \
EXT_START("GL_NV_conservative_raster_dilate") \
X(void, ConservativeRasterParameterfNV, (GLenum pname, GLfloat value)) \
EXT_END() \
EXT_START("GL_NV_conservative_raster_pre_snap_triangles") \
X(void, ConservativeRasterParameteriNV, (GLenum pname, GLint param)) \
EXT_END() \
EXT_START("GL_NV_copy_buffer") \
X(void, CopyBufferSubDataNV, (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)) \
EXT_END() \
EXT_START("GL_NV_copy_image") \
X(void, CopyImageSubDataNV, (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei width, GLsizei height, GLsizei depth)) \
EXT_END() \
EXT_START("GL_NV_coverage_sample") \
X(void, CoverageMaskNV, (GLboolean mask)) \
X(void, CoverageOperationNV, (GLenum operation)) \
EXT_END() \
EXT_START("GL_NV_depth_buffer_float") \
X(void, ClearDepthdNV, (GLdouble depth)) \
X(void, DepthBoundsdNV, (GLdouble zmin, GLdouble zmax)) \
X(void, DepthRangedNV, (GLdouble zNear, GLdouble zFar)) \
EXT_END() \
EXT_START("GL_NV_draw_buffers") \
X(void, DrawBuffersNV, (GLsizei n, const GLenum * bufs)) \
EXT_END() \
EXT_START("GL_NV_draw_instanced") \
X(void, DrawArraysInstancedNV, (GLenum mode, GLint first, GLsizei count, GLsizei primcount)) \
X(void, DrawElementsInstancedNV, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei primcount)) \
EXT_END() \
EXT_START("GL_NV_draw_texture") \
X(void, DrawTextureNV, (GLuint texture, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1)) \
EXT_END() \
EXT_START("GL_NV_draw_vulkan_image") \
X(void, DrawVkImageNV, (GLuint64 vkImage, GLuint sampler, GLfloat x0, GLfloat y0, GLfloat x1, GLfloat y1, GLfloat z, GLfloat s0, GLfloat t0, GLfloat s1, GLfloat t1)) \
X(GLVULKANPROCNV, GetVkProcAddrNV, (const GLchar * name)) \
X(void, SignalVkFenceNV, (GLuint64 vkFence)) \
X(void, SignalVkSemaphoreNV, (GLuint64 vkSemaphore)) \
X(void, WaitVkSemaphoreNV, (GLuint64 vkSemaphore)) \
EXT_END() \
EXT_START("GL_NV_evaluators") \
X(void, EvalMapsNV, (GLenum target, GLenum mode)) \
X(void, GetMapAttribParameterfvNV, (GLenum target, GLuint index, GLenum pname, GLfloat * params)) \
X(void, GetMapAttribParameterivNV, (GLenum target, GLuint index, GLenum pname, GLint * params)) \
X(void, GetMapControlPointsNV, (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLboolean packed, void * points)) \
X(void, GetMapParameterfvNV, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetMapParameterivNV, (GLenum target, GLenum pname, GLint * params)) \
X(void, MapControlPointsNV, (GLenum target, GLuint index, GLenum type, GLsizei ustride, GLsizei vstride, GLint uorder, GLint vorder, GLboolean packed, const void * points)) \
X(void, MapParameterfvNV, (GLenum target, GLenum pname, const GLfloat * params)) \
X(void, MapParameterivNV, (GLenum target, GLenum pname, const GLint * params)) \
EXT_END() \
EXT_START("GL_NV_explicit_multisample") \
X(void, GetMultisamplefvNV, (GLenum pname, GLuint index, GLfloat * val)) \
X(void, SampleMaskIndexedNV, (GLuint index, GLbitfield mask)) \
X(void, TexRenderbufferNV, (GLenum target, GLuint renderbuffer)) \
EXT_END() \
EXT_START("GL_NV_fence") \
X(void, DeleteFencesNV, (GLsizei n, const GLuint * fences)) \
X(void, FinishFenceNV, (GLuint fence)) \
X(void, GenFencesNV, (GLsizei n, GLuint * fences)) \
X(void, GetFenceivNV, (GLuint fence, GLenum pname, GLint * params)) \
X(GLboolean, IsFenceNV, (GLuint fence)) \
X(void, SetFenceNV, (GLuint fence, GLenum condition)) \
X(GLboolean, TestFenceNV, (GLuint fence)) \
EXT_END() \
EXT_START("GL_NV_fragment_coverage_to_color") \
X(void, FragmentCoverageColorNV, (GLuint color)) \
EXT_END() \
EXT_START("GL_NV_fragment_program") \
X(void, GetProgramNamedParameterdvNV, (GLuint id, GLsizei len, const GLubyte * name, GLdouble * params)) \
X(void, GetProgramNamedParameterfvNV, (GLuint id, GLsizei len, const GLubyte * name, GLfloat * params)) \
X(void, ProgramNamedParameter4dNV, (GLuint id, GLsizei len, const GLubyte * name, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, ProgramNamedParameter4dvNV, (GLuint id, GLsizei len, const GLubyte * name, const GLdouble * v)) \
X(void, ProgramNamedParameter4fNV, (GLuint id, GLsizei len, const GLubyte * name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, ProgramNamedParameter4fvNV, (GLuint id, GLsizei len, const GLubyte * name, const GLfloat * v)) \
EXT_END() \
EXT_START("GL_NV_framebuffer_blit") \
X(void, BlitFramebufferNV, (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)) \
EXT_END() \
EXT_START("GL_NV_framebuffer_mixed_samples") \
X(void, CoverageModulationNV, (GLenum components)) \
X(void, CoverageModulationTableNV, (GLsizei n, const GLfloat * v)) \
X(void, GetCoverageModulationTableNV, (GLsizei bufSize, GLfloat * v)) \
EXT_END() \
EXT_START("GL_NV_framebuffer_multisample") \
X(void, RenderbufferStorageMultisampleNV, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height)) \
EXT_END() \
EXT_START("GL_NV_framebuffer_multisample_coverage") \
X(void, RenderbufferStorageMultisampleCoverageNV, (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLenum internalformat, GLsizei width, GLsizei height)) \
EXT_END() \
EXT_START("GL_NV_geometry_program4") \
X(void, FramebufferTextureFaceEXT, (GLenum target, GLenum attachment, GLuint texture, GLint level, GLenum face)) \
X(void, ProgramVertexLimitNV, (GLenum target, GLint limit)) \
EXT_END() \
EXT_START("GL_NV_gpu_multicast") \
X(void, MulticastBarrierNV, ()) \
X(void, MulticastBlitFramebufferNV, (GLuint srcGpu, GLuint dstGpu, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter)) \
X(void, MulticastBufferSubDataNV, (GLbitfield gpuMask, GLuint buffer, GLintptr offset, GLsizeiptr size, const void * data)) \
X(void, MulticastCopyBufferSubDataNV, (GLuint readGpu, GLbitfield writeGpuMask, GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size)) \
X(void, MulticastCopyImageSubDataNV, (GLuint srcGpu, GLbitfield dstGpuMask, GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth)) \
X(void, MulticastFramebufferSampleLocationsfvNV, (GLuint gpu, GLuint framebuffer, GLuint start, GLsizei count, const GLfloat * v)) \
X(void, MulticastGetQueryObjecti64vNV, (GLuint gpu, GLuint id, GLenum pname, GLint64 * params)) \
X(void, MulticastGetQueryObjectivNV, (GLuint gpu, GLuint id, GLenum pname, GLint * params)) \
X(void, MulticastGetQueryObjectui64vNV, (GLuint gpu, GLuint id, GLenum pname, GLuint64 * params)) \
X(void, MulticastGetQueryObjectuivNV, (GLuint gpu, GLuint id, GLenum pname, GLuint * params)) \
X(void, MulticastWaitSyncNV, (GLuint signalGpu, GLbitfield waitGpuMask)) \
X(void, RenderGpuMaskNV, (GLbitfield mask)) \
EXT_END() \
EXT_START("GL_NV_gpu_program4") \
X(void, GetProgramEnvParameterIivNV, (GLenum target, GLuint index, GLint * params)) \
X(void, GetProgramEnvParameterIuivNV, (GLenum target, GLuint index, GLuint * params)) \
X(void, GetProgramLocalParameterIivNV, (GLenum target, GLuint index, GLint * params)) \
X(void, GetProgramLocalParameterIuivNV, (GLenum target, GLuint index, GLuint * params)) \
X(void, ProgramEnvParameterI4iNV, (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w)) \
X(void, ProgramEnvParameterI4ivNV, (GLenum target, GLuint index, const GLint * params)) \
X(void, ProgramEnvParameterI4uiNV, (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)) \
X(void, ProgramEnvParameterI4uivNV, (GLenum target, GLuint index, const GLuint * params)) \
X(void, ProgramEnvParametersI4ivNV, (GLenum target, GLuint index, GLsizei count, const GLint * params)) \
X(void, ProgramEnvParametersI4uivNV, (GLenum target, GLuint index, GLsizei count, const GLuint * params)) \
X(void, ProgramLocalParameterI4iNV, (GLenum target, GLuint index, GLint x, GLint y, GLint z, GLint w)) \
X(void, ProgramLocalParameterI4ivNV, (GLenum target, GLuint index, const GLint * params)) \
X(void, ProgramLocalParameterI4uiNV, (GLenum target, GLuint index, GLuint x, GLuint y, GLuint z, GLuint w)) \
X(void, ProgramLocalParameterI4uivNV, (GLenum target, GLuint index, const GLuint * params)) \
X(void, ProgramLocalParametersI4ivNV, (GLenum target, GLuint index, GLsizei count, const GLint * params)) \
X(void, ProgramLocalParametersI4uivNV, (GLenum target, GLuint index, GLsizei count, const GLuint * params)) \
EXT_END() \
EXT_START("GL_NV_gpu_program5") \
X(void, GetProgramSubroutineParameteruivNV, (GLenum target, GLuint index, GLuint * param)) \
X(void, ProgramSubroutineParametersuivNV, (GLenum target, GLsizei count, const GLuint * params)) \
EXT_END() \
EXT_START("GL_NV_half_float") \
X(void, Color3hNV, (GLhalfNV red, GLhalfNV green, GLhalfNV blue)) \
X(void, Color3hvNV, (const GLhalfNV * v)) \
X(void, Color4hNV, (GLhalfNV red, GLhalfNV green, GLhalfNV blue, GLhalfNV alpha)) \
X(void, Color4hvNV, (const GLhalfNV * v)) \
X(void, FogCoordhNV, (GLhalfNV fog)) \
X(void, FogCoordhvNV, (const GLhalfNV * fog)) \
X(void, MultiTexCoord1hNV, (GLenum target, GLhalfNV s)) \
X(void, MultiTexCoord1hvNV, (GLenum target, const GLhalfNV * v)) \
X(void, MultiTexCoord2hNV, (GLenum target, GLhalfNV s, GLhalfNV t)) \
X(void, MultiTexCoord2hvNV, (GLenum target, const GLhalfNV * v)) \
X(void, MultiTexCoord3hNV, (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r)) \
X(void, MultiTexCoord3hvNV, (GLenum target, const GLhalfNV * v)) \
X(void, MultiTexCoord4hNV, (GLenum target, GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q)) \
X(void, MultiTexCoord4hvNV, (GLenum target, const GLhalfNV * v)) \
X(void, Normal3hNV, (GLhalfNV nx, GLhalfNV ny, GLhalfNV nz)) \
X(void, Normal3hvNV, (const GLhalfNV * v)) \
X(void, SecondaryColor3hNV, (GLhalfNV red, GLhalfNV green, GLhalfNV blue)) \
X(void, SecondaryColor3hvNV, (const GLhalfNV * v)) \
X(void, TexCoord1hNV, (GLhalfNV s)) \
X(void, TexCoord1hvNV, (const GLhalfNV * v)) \
X(void, TexCoord2hNV, (GLhalfNV s, GLhalfNV t)) \
X(void, TexCoord2hvNV, (const GLhalfNV * v)) \
X(void, TexCoord3hNV, (GLhalfNV s, GLhalfNV t, GLhalfNV r)) \
X(void, TexCoord3hvNV, (const GLhalfNV * v)) \
X(void, TexCoord4hNV, (GLhalfNV s, GLhalfNV t, GLhalfNV r, GLhalfNV q)) \
X(void, TexCoord4hvNV, (const GLhalfNV * v)) \
X(void, Vertex2hNV, (GLhalfNV x, GLhalfNV y)) \
X(void, Vertex2hvNV, (const GLhalfNV * v)) \
X(void, Vertex3hNV, (GLhalfNV x, GLhalfNV y, GLhalfNV z)) \
X(void, Vertex3hvNV, (const GLhalfNV * v)) \
X(void, Vertex4hNV, (GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w)) \
X(void, Vertex4hvNV, (const GLhalfNV * v)) \
X(void, VertexAttrib1hNV, (GLuint index, GLhalfNV x)) \
X(void, VertexAttrib1hvNV, (GLuint index, const GLhalfNV * v)) \
X(void, VertexAttrib2hNV, (GLuint index, GLhalfNV x, GLhalfNV y)) \
X(void, VertexAttrib2hvNV, (GLuint index, const GLhalfNV * v)) \
X(void, VertexAttrib3hNV, (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z)) \
X(void, VertexAttrib3hvNV, (GLuint index, const GLhalfNV * v)) \
X(void, VertexAttrib4hNV, (GLuint index, GLhalfNV x, GLhalfNV y, GLhalfNV z, GLhalfNV w)) \
X(void, VertexAttrib4hvNV, (GLuint index, const GLhalfNV * v)) \
X(void, VertexAttribs1hvNV, (GLuint index, GLsizei n, const GLhalfNV * v)) \
X(void, VertexAttribs2hvNV, (GLuint index, GLsizei n, const GLhalfNV * v)) \
X(void, VertexAttribs3hvNV, (GLuint index, GLsizei n, const GLhalfNV * v)) \
X(void, VertexAttribs4hvNV, (GLuint index, GLsizei n, const GLhalfNV * v)) \
X(void, VertexWeighthNV, (GLhalfNV weight)) \
X(void, VertexWeighthvNV, (const GLhalfNV * weight)) \
EXT_END() \
EXT_START("GL_NV_instanced_arrays") \
X(void, VertexAttribDivisorNV, (GLuint index, GLuint divisor)) \
EXT_END() \
EXT_START("GL_NV_internalformat_sample_query") \
X(void, GetInternalformatSampleivNV, (GLenum target, GLenum internalformat, GLsizei samples, GLenum pname, GLsizei count, GLint * params)) \
EXT_END() \
EXT_START("GL_NV_memory_attachment") \
X(void, BufferAttachMemoryNV, (GLenum target, GLuint memory, GLuint64 offset)) \
X(void, GetMemoryObjectDetachedResourcesuivNV, (GLuint memory, GLenum pname, GLint first, GLsizei count, GLuint * params)) \
X(void, NamedBufferAttachMemoryNV, (GLuint buffer, GLuint memory, GLuint64 offset)) \
X(void, ResetMemoryObjectParameterNV, (GLuint memory, GLenum pname)) \
X(void, TexAttachMemoryNV, (GLenum target, GLuint memory, GLuint64 offset)) \
X(void, TextureAttachMemoryNV, (GLuint texture, GLuint memory, GLuint64 offset)) \
EXT_END() \
EXT_START("GL_NV_memory_object_sparse") \
X(void, BufferPageCommitmentMemNV, (GLenum target, GLintptr offset, GLsizeiptr size, GLuint memory, GLuint64 memOffset, GLboolean commit)) \
X(void, NamedBufferPageCommitmentMemNV, (GLuint buffer, GLintptr offset, GLsizeiptr size, GLuint memory, GLuint64 memOffset, GLboolean commit)) \
X(void, TexPageCommitmentMemNV, (GLenum target, GLint layer, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset, GLboolean commit)) \
X(void, TexturePageCommitmentMemNV, (GLuint texture, GLint layer, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLuint memory, GLuint64 offset, GLboolean commit)) \
EXT_END() \
EXT_START("GL_NV_mesh_shader") \
X(void, DrawMeshTasksIndirectNV, (GLintptr indirect)) \
X(void, DrawMeshTasksNV, (GLuint first, GLuint count)) \
X(void, MultiDrawMeshTasksIndirectCountNV, (GLintptr indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride)) \
X(void, MultiDrawMeshTasksIndirectNV, (GLintptr indirect, GLsizei drawcount, GLsizei stride)) \
EXT_END() \
EXT_START("GL_NV_non_square_matrices") \
X(void, UniformMatrix2x3fvNV, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix2x4fvNV, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix3x2fvNV, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix3x4fvNV, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix4x2fvNV, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
X(void, UniformMatrix4x3fvNV, (GLint location, GLsizei count, GLboolean transpose, const GLfloat * value)) \
EXT_END() \
EXT_START("GL_NV_occlusion_query") \
X(void, BeginOcclusionQueryNV, (GLuint id)) \
X(void, DeleteOcclusionQueriesNV, (GLsizei n, const GLuint * ids)) \
X(void, EndOcclusionQueryNV, ()) \
X(void, GenOcclusionQueriesNV, (GLsizei n, GLuint * ids)) \
X(void, GetOcclusionQueryivNV, (GLuint id, GLenum pname, GLint * params)) \
X(void, GetOcclusionQueryuivNV, (GLuint id, GLenum pname, GLuint * params)) \
X(GLboolean, IsOcclusionQueryNV, (GLuint id)) \
EXT_END() \
EXT_START("GL_NV_parameter_buffer_object") \
X(void, ProgramBufferParametersIivNV, (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLint * params)) \
X(void, ProgramBufferParametersIuivNV, (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLuint * params)) \
X(void, ProgramBufferParametersfvNV, (GLenum target, GLuint bindingIndex, GLuint wordIndex, GLsizei count, const GLfloat * params)) \
EXT_END() \
EXT_START("GL_NV_path_rendering") \
X(void, CopyPathNV, (GLuint resultPath, GLuint srcPath)) \
X(void, CoverFillPathInstancedNV, (GLsizei numPaths, GLenum pathNameType, const void * paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat * transformValues)) \
X(void, CoverFillPathNV, (GLuint path, GLenum coverMode)) \
X(void, CoverStrokePathInstancedNV, (GLsizei numPaths, GLenum pathNameType, const void * paths, GLuint pathBase, GLenum coverMode, GLenum transformType, const GLfloat * transformValues)) \
X(void, CoverStrokePathNV, (GLuint path, GLenum coverMode)) \
X(void, DeletePathsNV, (GLuint path, GLsizei range)) \
X(GLuint, GenPathsNV, (GLsizei range)) \
X(void, GetPathColorGenfvNV, (GLenum color, GLenum pname, GLfloat * value)) \
X(void, GetPathColorGenivNV, (GLenum color, GLenum pname, GLint * value)) \
X(void, GetPathCommandsNV, (GLuint path, GLubyte * commands)) \
X(void, GetPathCoordsNV, (GLuint path, GLfloat * coords)) \
X(void, GetPathDashArrayNV, (GLuint path, GLfloat * dashArray)) \
X(GLfloat, GetPathLengthNV, (GLuint path, GLsizei startSegment, GLsizei numSegments)) \
X(void, GetPathMetricRangeNV, (GLbitfield metricQueryMask, GLuint firstPathName, GLsizei numPaths, GLsizei stride, GLfloat * metrics)) \
X(void, GetPathMetricsNV, (GLbitfield metricQueryMask, GLsizei numPaths, GLenum pathNameType, const void * paths, GLuint pathBase, GLsizei stride, GLfloat * metrics)) \
X(void, GetPathParameterfvNV, (GLuint path, GLenum pname, GLfloat * value)) \
X(void, GetPathParameterivNV, (GLuint path, GLenum pname, GLint * value)) \
X(void, GetPathSpacingNV, (GLenum pathListMode, GLsizei numPaths, GLenum pathNameType, const void * paths, GLuint pathBase, GLfloat advanceScale, GLfloat kerningScale, GLenum transformType, GLfloat * returnedSpacing)) \
X(void, GetPathTexGenfvNV, (GLenum texCoordSet, GLenum pname, GLfloat * value)) \
X(void, GetPathTexGenivNV, (GLenum texCoordSet, GLenum pname, GLint * value)) \
X(void, GetProgramResourcefvNV, (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum * props, GLsizei count, GLsizei * length, GLfloat * params)) \
X(void, InterpolatePathsNV, (GLuint resultPath, GLuint pathA, GLuint pathB, GLfloat weight)) \
X(GLboolean, IsPathNV, (GLuint path)) \
X(GLboolean, IsPointInFillPathNV, (GLuint path, GLuint mask, GLfloat x, GLfloat y)) \
X(GLboolean, IsPointInStrokePathNV, (GLuint path, GLfloat x, GLfloat y)) \
X(void, MatrixLoad3x2fNV, (GLenum matrixMode, const GLfloat * m)) \
X(void, MatrixLoad3x3fNV, (GLenum matrixMode, const GLfloat * m)) \
X(void, MatrixLoadTranspose3x3fNV, (GLenum matrixMode, const GLfloat * m)) \
X(void, MatrixMult3x2fNV, (GLenum matrixMode, const GLfloat * m)) \
X(void, MatrixMult3x3fNV, (GLenum matrixMode, const GLfloat * m)) \
X(void, MatrixMultTranspose3x3fNV, (GLenum matrixMode, const GLfloat * m)) \
X(void, PathColorGenNV, (GLenum color, GLenum genMode, GLenum colorFormat, const GLfloat * coeffs)) \
X(void, PathCommandsNV, (GLuint path, GLsizei numCommands, const GLubyte * commands, GLsizei numCoords, GLenum coordType, const void * coords)) \
X(void, PathCoordsNV, (GLuint path, GLsizei numCoords, GLenum coordType, const void * coords)) \
X(void, PathCoverDepthFuncNV, (GLenum func)) \
X(void, PathDashArrayNV, (GLuint path, GLsizei dashCount, const GLfloat * dashArray)) \
X(void, PathFogGenNV, (GLenum genMode)) \
X(GLenum, PathGlyphIndexArrayNV, (GLuint firstPathName, GLenum fontTarget, const void * fontName, GLbitfield fontStyle, GLuint firstGlyphIndex, GLsizei numGlyphs, GLuint pathParameterTemplate, GLfloat emScale)) \
X(GLenum, PathGlyphIndexRangeNV, (GLenum fontTarget, const void * fontName, GLbitfield fontStyle, GLuint pathParameterTemplate, GLfloat emScale, GLuint * baseAndCount)) \
X(void, PathGlyphRangeNV, (GLuint firstPathName, GLenum fontTarget, const void * fontName, GLbitfield fontStyle, GLuint firstGlyph, GLsizei numGlyphs, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale)) \
X(void, PathGlyphsNV, (GLuint firstPathName, GLenum fontTarget, const void * fontName, GLbitfield fontStyle, GLsizei numGlyphs, GLenum type, const void * charcodes, GLenum handleMissingGlyphs, GLuint pathParameterTemplate, GLfloat emScale)) \
X(GLenum, PathMemoryGlyphIndexArrayNV, (GLuint firstPathName, GLenum fontTarget, GLsizeiptr fontSize, const void * fontData, GLsizei faceIndex, GLuint firstGlyphIndex, GLsizei numGlyphs, GLuint pathParameterTemplate, GLfloat emScale)) \
X(void, PathParameterfNV, (GLuint path, GLenum pname, GLfloat value)) \
X(void, PathParameterfvNV, (GLuint path, GLenum pname, const GLfloat * value)) \
X(void, PathParameteriNV, (GLuint path, GLenum pname, GLint value)) \
X(void, PathParameterivNV, (GLuint path, GLenum pname, const GLint * value)) \
X(void, PathStencilDepthOffsetNV, (GLfloat factor, GLfloat units)) \
X(void, PathStencilFuncNV, (GLenum func, GLint ref, GLuint mask)) \
X(void, PathStringNV, (GLuint path, GLenum format, GLsizei length, const void * pathString)) \
X(void, PathSubCommandsNV, (GLuint path, GLsizei commandStart, GLsizei commandsToDelete, GLsizei numCommands, const GLubyte * commands, GLsizei numCoords, GLenum coordType, const void * coords)) \
X(void, PathSubCoordsNV, (GLuint path, GLsizei coordStart, GLsizei numCoords, GLenum coordType, const void * coords)) \
X(void, PathTexGenNV, (GLenum texCoordSet, GLenum genMode, GLint components, const GLfloat * coeffs)) \
X(GLboolean, PointAlongPathNV, (GLuint path, GLsizei startSegment, GLsizei numSegments, GLfloat distance, GLfloat * x, GLfloat * y, GLfloat * tangentX, GLfloat * tangentY)) \
X(void, ProgramPathFragmentInputGenNV, (GLuint program, GLint location, GLenum genMode, GLint components, const GLfloat * coeffs)) \
X(void, StencilFillPathInstancedNV, (GLsizei numPaths, GLenum pathNameType, const void * paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum transformType, const GLfloat * transformValues)) \
X(void, StencilFillPathNV, (GLuint path, GLenum fillMode, GLuint mask)) \
X(void, StencilStrokePathInstancedNV, (GLsizei numPaths, GLenum pathNameType, const void * paths, GLuint pathBase, GLint reference, GLuint mask, GLenum transformType, const GLfloat * transformValues)) \
X(void, StencilStrokePathNV, (GLuint path, GLint reference, GLuint mask)) \
X(void, StencilThenCoverFillPathInstancedNV, (GLsizei numPaths, GLenum pathNameType, const void * paths, GLuint pathBase, GLenum fillMode, GLuint mask, GLenum coverMode, GLenum transformType, const GLfloat * transformValues)) \
X(void, StencilThenCoverFillPathNV, (GLuint path, GLenum fillMode, GLuint mask, GLenum coverMode)) \
X(void, StencilThenCoverStrokePathInstancedNV, (GLsizei numPaths, GLenum pathNameType, const void * paths, GLuint pathBase, GLint reference, GLuint mask, GLenum coverMode, GLenum transformType, const GLfloat * transformValues)) \
X(void, StencilThenCoverStrokePathNV, (GLuint path, GLint reference, GLuint mask, GLenum coverMode)) \
X(void, TransformPathNV, (GLuint resultPath, GLuint srcPath, GLenum transformType, const GLfloat * transformValues)) \
X(void, WeightPathsNV, (GLuint resultPath, GLsizei numPaths, const GLuint * paths, const GLfloat * weights)) \
EXT_END() \
EXT_START("GL_NV_pixel_data_range") \
X(void, FlushPixelDataRangeNV, (GLenum target)) \
X(void, PixelDataRangeNV, (GLenum target, GLsizei length, const void * pointer)) \
EXT_END() \
EXT_START("GL_NV_point_sprite") \
X(void, PointParameteriNV, (GLenum pname, GLint param)) \
X(void, PointParameterivNV, (GLenum pname, const GLint * params)) \
EXT_END() \
EXT_START("GL_NV_polygon_mode") \
X(void, PolygonModeNV, (GLenum face, GLenum mode)) \
EXT_END() \
EXT_START("GL_NV_present_video") \
X(void, GetVideoi64vNV, (GLuint video_slot, GLenum pname, GLint64EXT * params)) \
X(void, GetVideoivNV, (GLuint video_slot, GLenum pname, GLint * params)) \
X(void, GetVideoui64vNV, (GLuint video_slot, GLenum pname, GLuint64EXT * params)) \
X(void, GetVideouivNV, (GLuint video_slot, GLenum pname, GLuint * params)) \
X(void, PresentFrameDualFillNV, (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLenum target1, GLuint fill1, GLenum target2, GLuint fill2, GLenum target3, GLuint fill3)) \
X(void, PresentFrameKeyedNV, (GLuint video_slot, GLuint64EXT minPresentTime, GLuint beginPresentTimeId, GLuint presentDurationId, GLenum type, GLenum target0, GLuint fill0, GLuint key0, GLenum target1, GLuint fill1, GLuint key1)) \
EXT_END() \
EXT_START("GL_NV_primitive_restart") \
X(void, PrimitiveRestartIndexNV, (GLuint index)) \
X(void, PrimitiveRestartNV, ()) \
EXT_END() \
EXT_START("GL_NV_query_resource") \
X(GLint, QueryResourceNV, (GLenum queryType, GLint tagId, GLuint count, GLint * buffer)) \
EXT_END() \
EXT_START("GL_NV_query_resource_tag") \
X(void, DeleteQueryResourceTagNV, (GLsizei n, const GLint * tagIds)) \
X(void, GenQueryResourceTagNV, (GLsizei n, GLint * tagIds)) \
X(void, QueryResourceTagNV, (GLint tagId, const GLchar * tagString)) \
EXT_END() \
EXT_START("GL_NV_read_buffer") \
X(void, ReadBufferNV, (GLenum mode)) \
EXT_END() \
EXT_START("GL_NV_register_combiners") \
X(void, CombinerInputNV, (GLenum stage, GLenum portion, GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage)) \
X(void, CombinerOutputNV, (GLenum stage, GLenum portion, GLenum abOutput, GLenum cdOutput, GLenum sumOutput, GLenum scale, GLenum bias, GLboolean abDotProduct, GLboolean cdDotProduct, GLboolean muxSum)) \
X(void, CombinerParameterfNV, (GLenum pname, GLfloat param)) \
X(void, CombinerParameterfvNV, (GLenum pname, const GLfloat * params)) \
X(void, CombinerParameteriNV, (GLenum pname, GLint param)) \
X(void, CombinerParameterivNV, (GLenum pname, const GLint * params)) \
X(void, FinalCombinerInputNV, (GLenum variable, GLenum input, GLenum mapping, GLenum componentUsage)) \
X(void, GetCombinerInputParameterfvNV, (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLfloat * params)) \
X(void, GetCombinerInputParameterivNV, (GLenum stage, GLenum portion, GLenum variable, GLenum pname, GLint * params)) \
X(void, GetCombinerOutputParameterfvNV, (GLenum stage, GLenum portion, GLenum pname, GLfloat * params)) \
X(void, GetCombinerOutputParameterivNV, (GLenum stage, GLenum portion, GLenum pname, GLint * params)) \
X(void, GetFinalCombinerInputParameterfvNV, (GLenum variable, GLenum pname, GLfloat * params)) \
X(void, GetFinalCombinerInputParameterivNV, (GLenum variable, GLenum pname, GLint * params)) \
EXT_END() \
EXT_START("GL_NV_register_combiners2") \
X(void, CombinerStageParameterfvNV, (GLenum stage, GLenum pname, const GLfloat * params)) \
X(void, GetCombinerStageParameterfvNV, (GLenum stage, GLenum pname, GLfloat * params)) \
EXT_END() \
EXT_START("GL_NV_sample_locations") \
X(void, FramebufferSampleLocationsfvNV, (GLenum target, GLuint start, GLsizei count, const GLfloat * v)) \
X(void, NamedFramebufferSampleLocationsfvNV, (GLuint framebuffer, GLuint start, GLsizei count, const GLfloat * v)) \
X(void, ResolveDepthValuesNV, ()) \
EXT_END() \
EXT_START("GL_NV_scissor_exclusive") \
X(void, ScissorExclusiveArrayvNV, (GLuint first, GLsizei count, const GLint * v)) \
X(void, ScissorExclusiveNV, (GLint x, GLint y, GLsizei width, GLsizei height)) \
EXT_END() \
EXT_START("GL_NV_shader_buffer_load") \
X(void, GetBufferParameterui64vNV, (GLenum target, GLenum pname, GLuint64EXT * params)) \
X(void, GetIntegerui64vNV, (GLenum value, GLuint64EXT * result)) \
X(void, GetNamedBufferParameterui64vNV, (GLuint buffer, GLenum pname, GLuint64EXT * params)) \
X(GLboolean, IsBufferResidentNV, (GLenum target)) \
X(GLboolean, IsNamedBufferResidentNV, (GLuint buffer)) \
X(void, MakeBufferNonResidentNV, (GLenum target)) \
X(void, MakeBufferResidentNV, (GLenum target, GLenum access)) \
X(void, MakeNamedBufferNonResidentNV, (GLuint buffer)) \
X(void, MakeNamedBufferResidentNV, (GLuint buffer, GLenum access)) \
X(void, ProgramUniformui64NV, (GLuint program, GLint location, GLuint64EXT value)) \
X(void, ProgramUniformui64vNV, (GLuint program, GLint location, GLsizei count, const GLuint64EXT * value)) \
X(void, Uniformui64NV, (GLint location, GLuint64EXT value)) \
X(void, Uniformui64vNV, (GLint location, GLsizei count, const GLuint64EXT * value)) \
EXT_END() \
EXT_START("GL_NV_shading_rate_image") \
X(void, BindShadingRateImageNV, (GLuint texture)) \
X(void, GetShadingRateImagePaletteNV, (GLuint viewport, GLuint entry, GLenum * rate)) \
X(void, GetShadingRateSampleLocationivNV, (GLenum rate, GLuint samples, GLuint index, GLint * location)) \
X(void, ShadingRateImageBarrierNV, (GLboolean synchronize)) \
X(void, ShadingRateImagePaletteNV, (GLuint viewport, GLuint first, GLsizei count, const GLenum * rates)) \
X(void, ShadingRateSampleOrderCustomNV, (GLenum rate, GLuint samples, const GLint * locations)) \
X(void, ShadingRateSampleOrderNV, (GLenum order)) \
EXT_END() \
EXT_START("GL_NV_texture_barrier") \
X(void, TextureBarrierNV, ()) \
EXT_END() \
EXT_START("GL_NV_texture_multisample") \
X(void, TexImage2DMultisampleCoverageNV, (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations)) \
X(void, TexImage3DMultisampleCoverageNV, (GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations)) \
X(void, TextureImage2DMultisampleCoverageNV, (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations)) \
X(void, TextureImage2DMultisampleNV, (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLboolean fixedSampleLocations)) \
X(void, TextureImage3DMultisampleCoverageNV, (GLuint texture, GLenum target, GLsizei coverageSamples, GLsizei colorSamples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations)) \
X(void, TextureImage3DMultisampleNV, (GLuint texture, GLenum target, GLsizei samples, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedSampleLocations)) \
EXT_END() \
EXT_START("GL_NV_timeline_semaphore") \
X(void, CreateSemaphoresNV, (GLsizei n, GLuint * semaphores)) \
X(void, GetSemaphoreParameterivNV, (GLuint semaphore, GLenum pname, GLint * params)) \
X(void, SemaphoreParameterivNV, (GLuint semaphore, GLenum pname, const GLint * params)) \
EXT_END() \
EXT_START("GL_NV_transform_feedback") \
X(void, ActiveVaryingNV, (GLuint program, const GLchar * name)) \
X(void, BeginTransformFeedbackNV, (GLenum primitiveMode)) \
X(void, BindBufferBaseNV, (GLenum target, GLuint index, GLuint buffer)) \
X(void, BindBufferOffsetNV, (GLenum target, GLuint index, GLuint buffer, GLintptr offset)) \
X(void, BindBufferRangeNV, (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
X(void, EndTransformFeedbackNV, ()) \
X(void, GetActiveVaryingNV, (GLuint program, GLuint index, GLsizei bufSize, GLsizei * length, GLsizei * size, GLenum * type, GLchar * name)) \
X(void, GetTransformFeedbackVaryingNV, (GLuint program, GLuint index, GLint * location)) \
X(GLint, GetVaryingLocationNV, (GLuint program, const GLchar * name)) \
X(void, TransformFeedbackAttribsNV, (GLsizei count, const GLint * attribs, GLenum bufferMode)) \
X(void, TransformFeedbackStreamAttribsNV, (GLsizei count, const GLint * attribs, GLsizei nbuffers, const GLint * bufstreams, GLenum bufferMode)) \
X(void, TransformFeedbackVaryingsNV, (GLuint program, GLsizei count, const GLint * locations, GLenum bufferMode)) \
EXT_END() \
EXT_START("GL_NV_transform_feedback2") \
X(void, BindTransformFeedbackNV, (GLenum target, GLuint id)) \
X(void, DeleteTransformFeedbacksNV, (GLsizei n, const GLuint * ids)) \
X(void, DrawTransformFeedbackNV, (GLenum mode, GLuint id)) \
X(void, GenTransformFeedbacksNV, (GLsizei n, GLuint * ids)) \
X(GLboolean, IsTransformFeedbackNV, (GLuint id)) \
X(void, PauseTransformFeedbackNV, ()) \
X(void, ResumeTransformFeedbackNV, ()) \
EXT_END() \
EXT_START("GL_NV_vdpau_interop") \
X(void, VDPAUFiniNV, ()) \
X(void, VDPAUGetSurfaceivNV, (GLvdpauSurfaceNV surface, GLenum pname, GLsizei count, GLsizei * length, GLint * values)) \
X(void, VDPAUInitNV, (const void * vdpDevice, const void * getProcAddress)) \
X(GLboolean, VDPAUIsSurfaceNV, (GLvdpauSurfaceNV surface)) \
X(void, VDPAUMapSurfacesNV, (GLsizei numSurfaces, const GLvdpauSurfaceNV * surfaces)) \
X(GLvdpauSurfaceNV, VDPAURegisterOutputSurfaceNV, (const void * vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint * textureNames)) \
X(GLvdpauSurfaceNV, VDPAURegisterVideoSurfaceNV, (const void * vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint * textureNames)) \
X(void, VDPAUSurfaceAccessNV, (GLvdpauSurfaceNV surface, GLenum access)) \
X(void, VDPAUUnmapSurfacesNV, (GLsizei numSurface, const GLvdpauSurfaceNV * surfaces)) \
X(void, VDPAUUnregisterSurfaceNV, (GLvdpauSurfaceNV surface)) \
EXT_END() \
EXT_START("GL_NV_vdpau_interop2") \
X(GLvdpauSurfaceNV, VDPAURegisterVideoSurfaceWithPictureStructureNV, (const void * vdpSurface, GLenum target, GLsizei numTextureNames, const GLuint * textureNames, GLboolean isFrameStructure)) \
EXT_END() \
EXT_START("GL_NV_vertex_array_range") \
X(void, FlushVertexArrayRangeNV, ()) \
X(void, VertexArrayRangeNV, (GLsizei length, const void * pointer)) \
EXT_END() \
EXT_START("GL_NV_vertex_attrib_integer_64bit") \
X(void, GetVertexAttribLi64vNV, (GLuint index, GLenum pname, GLint64EXT * params)) \
X(void, GetVertexAttribLui64vNV, (GLuint index, GLenum pname, GLuint64EXT * params)) \
X(void, VertexAttribL1i64NV, (GLuint index, GLint64EXT x)) \
X(void, VertexAttribL1i64vNV, (GLuint index, const GLint64EXT * v)) \
X(void, VertexAttribL1ui64NV, (GLuint index, GLuint64EXT x)) \
X(void, VertexAttribL1ui64vNV, (GLuint index, const GLuint64EXT * v)) \
X(void, VertexAttribL2i64NV, (GLuint index, GLint64EXT x, GLint64EXT y)) \
X(void, VertexAttribL2i64vNV, (GLuint index, const GLint64EXT * v)) \
X(void, VertexAttribL2ui64NV, (GLuint index, GLuint64EXT x, GLuint64EXT y)) \
X(void, VertexAttribL2ui64vNV, (GLuint index, const GLuint64EXT * v)) \
X(void, VertexAttribL3i64NV, (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z)) \
X(void, VertexAttribL3i64vNV, (GLuint index, const GLint64EXT * v)) \
X(void, VertexAttribL3ui64NV, (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z)) \
X(void, VertexAttribL3ui64vNV, (GLuint index, const GLuint64EXT * v)) \
X(void, VertexAttribL4i64NV, (GLuint index, GLint64EXT x, GLint64EXT y, GLint64EXT z, GLint64EXT w)) \
X(void, VertexAttribL4i64vNV, (GLuint index, const GLint64EXT * v)) \
X(void, VertexAttribL4ui64NV, (GLuint index, GLuint64EXT x, GLuint64EXT y, GLuint64EXT z, GLuint64EXT w)) \
X(void, VertexAttribL4ui64vNV, (GLuint index, const GLuint64EXT * v)) \
X(void, VertexAttribLFormatNV, (GLuint index, GLint size, GLenum type, GLsizei stride)) \
EXT_END() \
EXT_START("GL_NV_vertex_buffer_unified_memory") \
X(void, BufferAddressRangeNV, (GLenum pname, GLuint index, GLuint64EXT address, GLsizeiptr length)) \
X(void, ColorFormatNV, (GLint size, GLenum type, GLsizei stride)) \
X(void, EdgeFlagFormatNV, (GLsizei stride)) \
X(void, FogCoordFormatNV, (GLenum type, GLsizei stride)) \
X(void, GetIntegerui64i_vNV, (GLenum value, GLuint index, GLuint64EXT * result)) \
X(void, IndexFormatNV, (GLenum type, GLsizei stride)) \
X(void, NormalFormatNV, (GLenum type, GLsizei stride)) \
X(void, SecondaryColorFormatNV, (GLint size, GLenum type, GLsizei stride)) \
X(void, TexCoordFormatNV, (GLint size, GLenum type, GLsizei stride)) \
X(void, VertexAttribFormatNV, (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride)) \
X(void, VertexAttribIFormatNV, (GLuint index, GLint size, GLenum type, GLsizei stride)) \
X(void, VertexFormatNV, (GLint size, GLenum type, GLsizei stride)) \
EXT_END() \
EXT_START("GL_NV_vertex_program") \
X(GLboolean, AreProgramsResidentNV, (GLsizei n, const GLuint * programs, GLboolean * residences)) \
X(void, BindProgramNV, (GLenum target, GLuint id)) \
X(void, DeleteProgramsNV, (GLsizei n, const GLuint * programs)) \
X(void, ExecuteProgramNV, (GLenum target, GLuint id, const GLfloat * params)) \
X(void, GenProgramsNV, (GLsizei n, GLuint * programs)) \
X(void, GetProgramParameterdvNV, (GLenum target, GLuint index, GLenum pname, GLdouble * params)) \
X(void, GetProgramParameterfvNV, (GLenum target, GLuint index, GLenum pname, GLfloat * params)) \
X(void, GetProgramStringNV, (GLuint id, GLenum pname, GLubyte * program)) \
X(void, GetProgramivNV, (GLuint id, GLenum pname, GLint * params)) \
X(void, GetTrackMatrixivNV, (GLenum target, GLuint address, GLenum pname, GLint * params)) \
X(void, GetVertexAttribPointervNV, (GLuint index, GLenum pname, void ** pointer)) \
X(void, GetVertexAttribdvNV, (GLuint index, GLenum pname, GLdouble * params)) \
X(void, GetVertexAttribfvNV, (GLuint index, GLenum pname, GLfloat * params)) \
X(void, GetVertexAttribivNV, (GLuint index, GLenum pname, GLint * params)) \
X(GLboolean, IsProgramNV, (GLuint id)) \
X(void, LoadProgramNV, (GLenum target, GLuint id, GLsizei len, const GLubyte * program)) \
X(void, ProgramParameter4dNV, (GLenum target, GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, ProgramParameter4dvNV, (GLenum target, GLuint index, const GLdouble * v)) \
X(void, ProgramParameter4fNV, (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, ProgramParameter4fvNV, (GLenum target, GLuint index, const GLfloat * v)) \
X(void, ProgramParameters4dvNV, (GLenum target, GLuint index, GLsizei count, const GLdouble * v)) \
X(void, ProgramParameters4fvNV, (GLenum target, GLuint index, GLsizei count, const GLfloat * v)) \
X(void, RequestResidentProgramsNV, (GLsizei n, const GLuint * programs)) \
X(void, TrackMatrixNV, (GLenum target, GLuint address, GLenum matrix, GLenum transform)) \
X(void, VertexAttrib1dNV, (GLuint index, GLdouble x)) \
X(void, VertexAttrib1dvNV, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib1fNV, (GLuint index, GLfloat x)) \
X(void, VertexAttrib1fvNV, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib1sNV, (GLuint index, GLshort x)) \
X(void, VertexAttrib1svNV, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib2dNV, (GLuint index, GLdouble x, GLdouble y)) \
X(void, VertexAttrib2dvNV, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib2fNV, (GLuint index, GLfloat x, GLfloat y)) \
X(void, VertexAttrib2fvNV, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib2sNV, (GLuint index, GLshort x, GLshort y)) \
X(void, VertexAttrib2svNV, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib3dNV, (GLuint index, GLdouble x, GLdouble y, GLdouble z)) \
X(void, VertexAttrib3dvNV, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib3fNV, (GLuint index, GLfloat x, GLfloat y, GLfloat z)) \
X(void, VertexAttrib3fvNV, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib3sNV, (GLuint index, GLshort x, GLshort y, GLshort z)) \
X(void, VertexAttrib3svNV, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib4dNV, (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w)) \
X(void, VertexAttrib4dvNV, (GLuint index, const GLdouble * v)) \
X(void, VertexAttrib4fNV, (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, VertexAttrib4fvNV, (GLuint index, const GLfloat * v)) \
X(void, VertexAttrib4sNV, (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w)) \
X(void, VertexAttrib4svNV, (GLuint index, const GLshort * v)) \
X(void, VertexAttrib4ubNV, (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w)) \
X(void, VertexAttrib4ubvNV, (GLuint index, const GLubyte * v)) \
X(void, VertexAttribPointerNV, (GLuint index, GLint fsize, GLenum type, GLsizei stride, const void * pointer)) \
X(void, VertexAttribs1dvNV, (GLuint index, GLsizei count, const GLdouble * v)) \
X(void, VertexAttribs1fvNV, (GLuint index, GLsizei count, const GLfloat * v)) \
X(void, VertexAttribs1svNV, (GLuint index, GLsizei count, const GLshort * v)) \
X(void, VertexAttribs2dvNV, (GLuint index, GLsizei count, const GLdouble * v)) \
X(void, VertexAttribs2fvNV, (GLuint index, GLsizei count, const GLfloat * v)) \
X(void, VertexAttribs2svNV, (GLuint index, GLsizei count, const GLshort * v)) \
X(void, VertexAttribs3dvNV, (GLuint index, GLsizei count, const GLdouble * v)) \
X(void, VertexAttribs3fvNV, (GLuint index, GLsizei count, const GLfloat * v)) \
X(void, VertexAttribs3svNV, (GLuint index, GLsizei count, const GLshort * v)) \
X(void, VertexAttribs4dvNV, (GLuint index, GLsizei count, const GLdouble * v)) \
X(void, VertexAttribs4fvNV, (GLuint index, GLsizei count, const GLfloat * v)) \
X(void, VertexAttribs4svNV, (GLuint index, GLsizei count, const GLshort * v)) \
X(void, VertexAttribs4ubvNV, (GLuint index, GLsizei count, const GLubyte * v)) \
EXT_END() \
EXT_START("GL_NV_video_capture") \
X(void, BeginVideoCaptureNV, (GLuint video_capture_slot)) \
X(void, BindVideoCaptureStreamBufferNV, (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLintptrARB offset)) \
X(void, BindVideoCaptureStreamTextureNV, (GLuint video_capture_slot, GLuint stream, GLenum frame_region, GLenum target, GLuint texture)) \
X(void, EndVideoCaptureNV, (GLuint video_capture_slot)) \
X(void, GetVideoCaptureStreamdvNV, (GLuint video_capture_slot, GLuint stream, GLenum pname, GLdouble * params)) \
X(void, GetVideoCaptureStreamfvNV, (GLuint video_capture_slot, GLuint stream, GLenum pname, GLfloat * params)) \
X(void, GetVideoCaptureStreamivNV, (GLuint video_capture_slot, GLuint stream, GLenum pname, GLint * params)) \
X(void, GetVideoCaptureivNV, (GLuint video_capture_slot, GLenum pname, GLint * params)) \
X(GLenum, VideoCaptureNV, (GLuint video_capture_slot, GLuint * sequence_num, GLuint64EXT * capture_time)) \
X(void, VideoCaptureStreamParameterdvNV, (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLdouble * params)) \
X(void, VideoCaptureStreamParameterfvNV, (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLfloat * params)) \
X(void, VideoCaptureStreamParameterivNV, (GLuint video_capture_slot, GLuint stream, GLenum pname, const GLint * params)) \
EXT_END() \
EXT_START("GL_NV_viewport_array") \
X(void, DepthRangeArrayfvNV, (GLuint first, GLsizei count, const GLfloat * v)) \
X(void, DepthRangeIndexedfNV, (GLuint index, GLfloat n, GLfloat f)) \
X(void, DisableiNV, (GLenum target, GLuint index)) \
X(void, EnableiNV, (GLenum target, GLuint index)) \
X(void, GetFloati_vNV, (GLenum target, GLuint index, GLfloat * data)) \
X(GLboolean, IsEnablediNV, (GLenum target, GLuint index)) \
X(void, ScissorArrayvNV, (GLuint first, GLsizei count, const GLint * v)) \
X(void, ScissorIndexedNV, (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height)) \
X(void, ScissorIndexedvNV, (GLuint index, const GLint * v)) \
X(void, ViewportArrayvNV, (GLuint first, GLsizei count, const GLfloat * v)) \
X(void, ViewportIndexedfNV, (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h)) \
X(void, ViewportIndexedfvNV, (GLuint index, const GLfloat * v)) \
EXT_END() \
EXT_START("GL_NV_viewport_swizzle") \
X(void, ViewportSwizzleNV, (GLuint index, GLenum swizzlex, GLenum swizzley, GLenum swizzlez, GLenum swizzlew)) \
EXT_END() \
EXT_START("GL_OES_EGL_image") \
X(void, EGLImageTargetRenderbufferStorageOES, (GLenum target, GLeglImageOES image)) \
X(void, EGLImageTargetTexture2DOES, (GLenum target, GLeglImageOES image)) \
EXT_END() \
EXT_START("GL_OES_blend_equation_separate") \
X(void, BlendEquationSeparateOES, (GLenum modeRGB, GLenum modeAlpha)) \
EXT_END() \
EXT_START("GL_OES_blend_func_separate") \
X(void, BlendFuncSeparateOES, (GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)) \
EXT_END() \
EXT_START("GL_OES_blend_subtract") \
X(void, BlendEquationOES, (GLenum mode)) \
EXT_END() \
EXT_START("GL_OES_byte_coordinates") \
X(void, MultiTexCoord1bOES, (GLenum texture, GLbyte s)) \
X(void, MultiTexCoord1bvOES, (GLenum texture, const GLbyte * coords)) \
X(void, MultiTexCoord2bOES, (GLenum texture, GLbyte s, GLbyte t)) \
X(void, MultiTexCoord2bvOES, (GLenum texture, const GLbyte * coords)) \
X(void, MultiTexCoord3bOES, (GLenum texture, GLbyte s, GLbyte t, GLbyte r)) \
X(void, MultiTexCoord3bvOES, (GLenum texture, const GLbyte * coords)) \
X(void, MultiTexCoord4bOES, (GLenum texture, GLbyte s, GLbyte t, GLbyte r, GLbyte q)) \
X(void, MultiTexCoord4bvOES, (GLenum texture, const GLbyte * coords)) \
X(void, TexCoord1bOES, (GLbyte s)) \
X(void, TexCoord1bvOES, (const GLbyte * coords)) \
X(void, TexCoord2bOES, (GLbyte s, GLbyte t)) \
X(void, TexCoord2bvOES, (const GLbyte * coords)) \
X(void, TexCoord3bOES, (GLbyte s, GLbyte t, GLbyte r)) \
X(void, TexCoord3bvOES, (const GLbyte * coords)) \
X(void, TexCoord4bOES, (GLbyte s, GLbyte t, GLbyte r, GLbyte q)) \
X(void, TexCoord4bvOES, (const GLbyte * coords)) \
X(void, Vertex2bOES, (GLbyte x, GLbyte y)) \
X(void, Vertex2bvOES, (const GLbyte * coords)) \
X(void, Vertex3bOES, (GLbyte x, GLbyte y, GLbyte z)) \
X(void, Vertex3bvOES, (const GLbyte * coords)) \
X(void, Vertex4bOES, (GLbyte x, GLbyte y, GLbyte z, GLbyte w)) \
X(void, Vertex4bvOES, (const GLbyte * coords)) \
EXT_END() \
EXT_START("GL_OES_copy_image") \
X(void, CopyImageSubDataOES, (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth)) \
EXT_END() \
EXT_START("GL_OES_draw_buffers_indexed") \
X(void, BlendEquationSeparateiOES, (GLuint buf, GLenum modeRGB, GLenum modeAlpha)) \
X(void, BlendEquationiOES, (GLuint buf, GLenum mode)) \
X(void, BlendFuncSeparateiOES, (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha)) \
X(void, BlendFunciOES, (GLuint buf, GLenum src, GLenum dst)) \
X(void, ColorMaskiOES, (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a)) \
X(void, DisableiOES, (GLenum target, GLuint index)) \
X(void, EnableiOES, (GLenum target, GLuint index)) \
X(GLboolean, IsEnablediOES, (GLenum target, GLuint index)) \
EXT_END() \
EXT_START("GL_OES_draw_elements_base_vertex") \
X(void, DrawElementsBaseVertexOES, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLint basevertex)) \
X(void, DrawElementsInstancedBaseVertexOES, (GLenum mode, GLsizei count, GLenum type, const void * indices, GLsizei instancecount, GLint basevertex)) \
X(void, DrawRangeElementsBaseVertexOES, (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void * indices, GLint basevertex)) \
EXT_END() \
EXT_START("GL_OES_draw_texture") \
X(void, DrawTexfOES, (GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)) \
X(void, DrawTexfvOES, (const GLfloat * coords)) \
X(void, DrawTexiOES, (GLint x, GLint y, GLint z, GLint width, GLint height)) \
X(void, DrawTexivOES, (const GLint * coords)) \
X(void, DrawTexsOES, (GLshort x, GLshort y, GLshort z, GLshort width, GLshort height)) \
X(void, DrawTexsvOES, (const GLshort * coords)) \
X(void, DrawTexxOES, (GLfixed x, GLfixed y, GLfixed z, GLfixed width, GLfixed height)) \
X(void, DrawTexxvOES, (const GLfixed * coords)) \
EXT_END() \
EXT_START("GL_OES_fixed_point") \
X(void, AccumxOES, (GLenum op, GLfixed value)) \
X(void, AlphaFuncxOES, (GLenum func, GLfixed ref)) \
X(void, BitmapxOES, (GLsizei width, GLsizei height, GLfixed xorig, GLfixed yorig, GLfixed xmove, GLfixed ymove, const GLubyte * bitmap)) \
X(void, BlendColorxOES, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)) \
X(void, ClearAccumxOES, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)) \
X(void, ClearColorxOES, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)) \
X(void, ClearDepthxOES, (GLfixed depth)) \
X(void, ClipPlanexOES, (GLenum plane, const GLfixed * equation)) \
X(void, Color3xOES, (GLfixed red, GLfixed green, GLfixed blue)) \
X(void, Color3xvOES, (const GLfixed * components)) \
X(void, Color4xOES, (GLfixed red, GLfixed green, GLfixed blue, GLfixed alpha)) \
X(void, Color4xvOES, (const GLfixed * components)) \
X(void, ConvolutionParameterxOES, (GLenum target, GLenum pname, GLfixed param)) \
X(void, ConvolutionParameterxvOES, (GLenum target, GLenum pname, const GLfixed * params)) \
X(void, DepthRangexOES, (GLfixed n, GLfixed f)) \
X(void, EvalCoord1xOES, (GLfixed u)) \
X(void, EvalCoord1xvOES, (const GLfixed * coords)) \
X(void, EvalCoord2xOES, (GLfixed u, GLfixed v)) \
X(void, EvalCoord2xvOES, (const GLfixed * coords)) \
X(void, FeedbackBufferxOES, (GLsizei n, GLenum type, const GLfixed * buffer)) \
X(void, FogxOES, (GLenum pname, GLfixed param)) \
X(void, FogxvOES, (GLenum pname, const GLfixed * param)) \
X(void, FrustumxOES, (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f)) \
X(void, GetClipPlanexOES, (GLenum plane, GLfixed * equation)) \
X(void, GetConvolutionParameterxvOES, (GLenum target, GLenum pname, GLfixed * params)) \
X(void, GetFixedvOES, (GLenum pname, GLfixed * params)) \
X(void, GetHistogramParameterxvOES, (GLenum target, GLenum pname, GLfixed * params)) \
X(void, GetLightxOES, (GLenum light, GLenum pname, GLfixed * params)) \
X(void, GetLightxvOES, (GLenum light, GLenum pname, GLfixed * params)) \
X(void, GetMapxvOES, (GLenum target, GLenum query, GLfixed * v)) \
X(void, GetMaterialxOES, (GLenum face, GLenum pname, GLfixed param)) \
X(void, GetMaterialxvOES, (GLenum face, GLenum pname, GLfixed * params)) \
X(void, GetPixelMapxv, (GLenum map, GLint size, GLfixed * values)) \
X(void, GetTexEnvxvOES, (GLenum target, GLenum pname, GLfixed * params)) \
X(void, GetTexGenxvOES, (GLenum coord, GLenum pname, GLfixed * params)) \
X(void, GetTexLevelParameterxvOES, (GLenum target, GLint level, GLenum pname, GLfixed * params)) \
X(void, GetTexParameterxvOES, (GLenum target, GLenum pname, GLfixed * params)) \
X(void, IndexxOES, (GLfixed component)) \
X(void, IndexxvOES, (const GLfixed * component)) \
X(void, LightModelxOES, (GLenum pname, GLfixed param)) \
X(void, LightModelxvOES, (GLenum pname, const GLfixed * param)) \
X(void, LightxOES, (GLenum light, GLenum pname, GLfixed param)) \
X(void, LightxvOES, (GLenum light, GLenum pname, const GLfixed * params)) \
X(void, LineWidthxOES, (GLfixed width)) \
X(void, LoadMatrixxOES, (const GLfixed * m)) \
X(void, LoadTransposeMatrixxOES, (const GLfixed * m)) \
X(void, Map1xOES, (GLenum target, GLfixed u1, GLfixed u2, GLint stride, GLint order, GLfixed points)) \
X(void, Map2xOES, (GLenum target, GLfixed u1, GLfixed u2, GLint ustride, GLint uorder, GLfixed v1, GLfixed v2, GLint vstride, GLint vorder, GLfixed points)) \
X(void, MapGrid1xOES, (GLint n, GLfixed u1, GLfixed u2)) \
X(void, MapGrid2xOES, (GLint n, GLfixed u1, GLfixed u2, GLfixed v1, GLfixed v2)) \
X(void, MaterialxOES, (GLenum face, GLenum pname, GLfixed param)) \
X(void, MaterialxvOES, (GLenum face, GLenum pname, const GLfixed * param)) \
X(void, MultMatrixxOES, (const GLfixed * m)) \
X(void, MultTransposeMatrixxOES, (const GLfixed * m)) \
X(void, MultiTexCoord1xOES, (GLenum texture, GLfixed s)) \
X(void, MultiTexCoord1xvOES, (GLenum texture, const GLfixed * coords)) \
X(void, MultiTexCoord2xOES, (GLenum texture, GLfixed s, GLfixed t)) \
X(void, MultiTexCoord2xvOES, (GLenum texture, const GLfixed * coords)) \
X(void, MultiTexCoord3xOES, (GLenum texture, GLfixed s, GLfixed t, GLfixed r)) \
X(void, MultiTexCoord3xvOES, (GLenum texture, const GLfixed * coords)) \
X(void, MultiTexCoord4xOES, (GLenum texture, GLfixed s, GLfixed t, GLfixed r, GLfixed q)) \
X(void, MultiTexCoord4xvOES, (GLenum texture, const GLfixed * coords)) \
X(void, Normal3xOES, (GLfixed nx, GLfixed ny, GLfixed nz)) \
X(void, Normal3xvOES, (const GLfixed * coords)) \
X(void, OrthoxOES, (GLfixed l, GLfixed r, GLfixed b, GLfixed t, GLfixed n, GLfixed f)) \
X(void, PassThroughxOES, (GLfixed token)) \
X(void, PixelMapx, (GLenum map, GLint size, const GLfixed * values)) \
X(void, PixelStorex, (GLenum pname, GLfixed param)) \
X(void, PixelTransferxOES, (GLenum pname, GLfixed param)) \
X(void, PixelZoomxOES, (GLfixed xfactor, GLfixed yfactor)) \
X(void, PointParameterxOES, (GLenum pname, GLfixed param)) \
X(void, PointParameterxvOES, (GLenum pname, const GLfixed * params)) \
X(void, PointSizexOES, (GLfixed size)) \
X(void, PolygonOffsetxOES, (GLfixed factor, GLfixed units)) \
X(void, PrioritizeTexturesxOES, (GLsizei n, const GLuint * textures, const GLfixed * priorities)) \
X(void, RasterPos2xOES, (GLfixed x, GLfixed y)) \
X(void, RasterPos2xvOES, (const GLfixed * coords)) \
X(void, RasterPos3xOES, (GLfixed x, GLfixed y, GLfixed z)) \
X(void, RasterPos3xvOES, (const GLfixed * coords)) \
X(void, RasterPos4xOES, (GLfixed x, GLfixed y, GLfixed z, GLfixed w)) \
X(void, RasterPos4xvOES, (const GLfixed * coords)) \
X(void, RectxOES, (GLfixed x1, GLfixed y1, GLfixed x2, GLfixed y2)) \
X(void, RectxvOES, (const GLfixed * v1, const GLfixed * v2)) \
X(void, RotatexOES, (GLfixed angle, GLfixed x, GLfixed y, GLfixed z)) \
X(void, SampleCoveragexOES, (GLclampx value, GLboolean invert)) \
X(void, ScalexOES, (GLfixed x, GLfixed y, GLfixed z)) \
X(void, TexCoord1xOES, (GLfixed s)) \
X(void, TexCoord1xvOES, (const GLfixed * coords)) \
X(void, TexCoord2xOES, (GLfixed s, GLfixed t)) \
X(void, TexCoord2xvOES, (const GLfixed * coords)) \
X(void, TexCoord3xOES, (GLfixed s, GLfixed t, GLfixed r)) \
X(void, TexCoord3xvOES, (const GLfixed * coords)) \
X(void, TexCoord4xOES, (GLfixed s, GLfixed t, GLfixed r, GLfixed q)) \
X(void, TexCoord4xvOES, (const GLfixed * coords)) \
X(void, TexEnvxOES, (GLenum target, GLenum pname, GLfixed param)) \
X(void, TexEnvxvOES, (GLenum target, GLenum pname, const GLfixed * params)) \
X(void, TexGenxOES, (GLenum coord, GLenum pname, GLfixed param)) \
X(void, TexGenxvOES, (GLenum coord, GLenum pname, const GLfixed * params)) \
X(void, TexParameterxOES, (GLenum target, GLenum pname, GLfixed param)) \
X(void, TexParameterxvOES, (GLenum target, GLenum pname, const GLfixed * params)) \
X(void, TranslatexOES, (GLfixed x, GLfixed y, GLfixed z)) \
X(void, Vertex2xOES, (GLfixed x)) \
X(void, Vertex2xvOES, (const GLfixed * coords)) \
X(void, Vertex3xOES, (GLfixed x, GLfixed y)) \
X(void, Vertex3xvOES, (const GLfixed * coords)) \
X(void, Vertex4xOES, (GLfixed x, GLfixed y, GLfixed z)) \
X(void, Vertex4xvOES, (const GLfixed * coords)) \
EXT_END() \
EXT_START("GL_OES_framebuffer_object") \
X(void, BindFramebufferOES, (GLenum target, GLuint framebuffer)) \
X(void, BindRenderbufferOES, (GLenum target, GLuint renderbuffer)) \
X(GLenum, CheckFramebufferStatusOES, (GLenum target)) \
X(void, DeleteFramebuffersOES, (GLsizei n, const GLuint * framebuffers)) \
X(void, DeleteRenderbuffersOES, (GLsizei n, const GLuint * renderbuffers)) \
X(void, FramebufferRenderbufferOES, (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer)) \
X(void, FramebufferTexture2DOES, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level)) \
X(void, GenFramebuffersOES, (GLsizei n, GLuint * framebuffers)) \
X(void, GenRenderbuffersOES, (GLsizei n, GLuint * renderbuffers)) \
X(void, GenerateMipmapOES, (GLenum target)) \
X(void, GetFramebufferAttachmentParameterivOES, (GLenum target, GLenum attachment, GLenum pname, GLint * params)) \
X(void, GetRenderbufferParameterivOES, (GLenum target, GLenum pname, GLint * params)) \
X(GLboolean, IsFramebufferOES, (GLuint framebuffer)) \
X(GLboolean, IsRenderbufferOES, (GLuint renderbuffer)) \
X(void, RenderbufferStorageOES, (GLenum target, GLenum internalformat, GLsizei width, GLsizei height)) \
EXT_END() \
EXT_START("GL_OES_geometry_shader") \
X(void, FramebufferTextureOES, (GLenum target, GLenum attachment, GLuint texture, GLint level)) \
EXT_END() \
EXT_START("GL_OES_get_program_binary") \
X(void, GetProgramBinaryOES, (GLuint program, GLsizei bufSize, GLsizei * length, GLenum * binaryFormat, void * binary)) \
X(void, ProgramBinaryOES, (GLuint program, GLenum binaryFormat, const void * binary, GLint length)) \
EXT_END() \
EXT_START("GL_OES_mapbuffer") \
X(void, GetBufferPointervOES, (GLenum target, GLenum pname, void ** params)) \
X(void *, MapBufferOES, (GLenum target, GLenum access)) \
X(GLboolean, UnmapBufferOES, (GLenum target)) \
EXT_END() \
EXT_START("GL_OES_matrix_palette") \
X(void, CurrentPaletteMatrixOES, (GLuint matrixpaletteindex)) \
X(void, LoadPaletteFromModelViewMatrixOES, ()) \
X(void, MatrixIndexPointerOES, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
X(void, WeightPointerOES, (GLint size, GLenum type, GLsizei stride, const void * pointer)) \
EXT_END() \
EXT_START("GL_OES_point_size_array") \
X(void, PointSizePointerOES, (GLenum type, GLsizei stride, const void * pointer)) \
EXT_END() \
EXT_START("GL_OES_primitive_bounding_box") \
X(void, PrimitiveBoundingBoxOES, (GLfloat minX, GLfloat minY, GLfloat minZ, GLfloat minW, GLfloat maxX, GLfloat maxY, GLfloat maxZ, GLfloat maxW)) \
EXT_END() \
EXT_START("GL_OES_query_matrix") \
X(GLbitfield, QueryMatrixxOES, (GLfixed * mantissa, GLint * exponent)) \
EXT_END() \
EXT_START("GL_OES_sample_shading") \
X(void, MinSampleShadingOES, (GLfloat value)) \
EXT_END() \
EXT_START("GL_OES_single_precision") \
X(void, ClearDepthfOES, (GLclampf depth)) \
X(void, ClipPlanefOES, (GLenum plane, const GLfloat * equation)) \
X(void, DepthRangefOES, (GLclampf n, GLclampf f)) \
X(void, FrustumfOES, (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)) \
X(void, GetClipPlanefOES, (GLenum plane, GLfloat * equation)) \
X(void, OrthofOES, (GLfloat l, GLfloat r, GLfloat b, GLfloat t, GLfloat n, GLfloat f)) \
EXT_END() \
EXT_START("GL_OES_tessellation_shader") \
X(void, PatchParameteriOES, (GLenum pname, GLint value)) \
EXT_END() \
EXT_START("GL_OES_texture_3D") \
X(void, CompressedTexImage3DOES, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void * data)) \
X(void, CompressedTexSubImage3DOES, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void * data)) \
X(void, CopyTexSubImage3DOES, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height)) \
X(void, FramebufferTexture3DOES, (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset)) \
X(void, TexImage3DOES, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, TexSubImage3DOES, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void * pixels)) \
EXT_END() \
EXT_START("GL_OES_texture_border_clamp") \
X(void, GetSamplerParameterIivOES, (GLuint sampler, GLenum pname, GLint * params)) \
X(void, GetSamplerParameterIuivOES, (GLuint sampler, GLenum pname, GLuint * params)) \
X(void, GetTexParameterIivOES, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetTexParameterIuivOES, (GLenum target, GLenum pname, GLuint * params)) \
X(void, SamplerParameterIivOES, (GLuint sampler, GLenum pname, const GLint * param)) \
X(void, SamplerParameterIuivOES, (GLuint sampler, GLenum pname, const GLuint * param)) \
X(void, TexParameterIivOES, (GLenum target, GLenum pname, const GLint * params)) \
X(void, TexParameterIuivOES, (GLenum target, GLenum pname, const GLuint * params)) \
EXT_END() \
EXT_START("GL_OES_texture_buffer") \
X(void, TexBufferOES, (GLenum target, GLenum internalformat, GLuint buffer)) \
X(void, TexBufferRangeOES, (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size)) \
EXT_END() \
EXT_START("GL_OES_texture_cube_map") \
X(void, GetTexGenfvOES, (GLenum coord, GLenum pname, GLfloat * params)) \
X(void, GetTexGenivOES, (GLenum coord, GLenum pname, GLint * params)) \
X(void, TexGenfOES, (GLenum coord, GLenum pname, GLfloat param)) \
X(void, TexGenfvOES, (GLenum coord, GLenum pname, const GLfloat * params)) \
X(void, TexGeniOES, (GLenum coord, GLenum pname, GLint param)) \
X(void, TexGenivOES, (GLenum coord, GLenum pname, const GLint * params)) \
EXT_END() \
EXT_START("GL_OES_texture_storage_multisample_2d_array") \
X(void, TexStorage3DMultisampleOES, (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations)) \
EXT_END() \
EXT_START("GL_OES_texture_view") \
X(void, TextureViewOES, (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers)) \
EXT_END() \
EXT_START("GL_OES_vertex_array_object") \
X(void, BindVertexArrayOES, (GLuint array)) \
X(void, DeleteVertexArraysOES, (GLsizei n, const GLuint * arrays)) \
X(void, GenVertexArraysOES, (GLsizei n, GLuint * arrays)) \
X(GLboolean, IsVertexArrayOES, (GLuint array)) \
EXT_END() \
EXT_START("GL_OES_viewport_array") \
X(void, DepthRangeArrayfvOES, (GLuint first, GLsizei count, const GLfloat * v)) \
X(void, DepthRangeIndexedfOES, (GLuint index, GLfloat n, GLfloat f)) \
X(void, GetFloati_vOES, (GLenum target, GLuint index, GLfloat * data)) \
X(void, ScissorArrayvOES, (GLuint first, GLsizei count, const GLint * v)) \
X(void, ScissorIndexedOES, (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height)) \
X(void, ScissorIndexedvOES, (GLuint index, const GLint * v)) \
X(void, ViewportArrayvOES, (GLuint first, GLsizei count, const GLfloat * v)) \
X(void, ViewportIndexedfOES, (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h)) \
X(void, ViewportIndexedfvOES, (GLuint index, const GLfloat * v)) \
EXT_END() \
EXT_START("GL_OVR_multiview") \
X(void, FramebufferTextureMultiviewOVR, (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint baseViewIndex, GLsizei numViews)) \
X(void, NamedFramebufferTextureMultiviewOVR, (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint baseViewIndex, GLsizei numViews)) \
EXT_END() \
EXT_START("GL_OVR_multiview_multisampled_render_to_texture") \
X(void, FramebufferTextureMultisampleMultiviewOVR, (GLenum target, GLenum attachment, GLuint texture, GLint level, GLsizei samples, GLint baseViewIndex, GLsizei numViews)) \
EXT_END() \
EXT_START("GL_PGI_misc_hints") \
X(void, HintPGI, (GLenum target, GLint mode)) \
EXT_END() \
EXT_START("GL_QCOM_alpha_test") \
X(void, AlphaFuncQCOM, (GLenum func, GLclampf ref)) \
EXT_END() \
EXT_START("GL_QCOM_driver_control") \
X(void, DisableDriverControlQCOM, (GLuint driverControl)) \
X(void, EnableDriverControlQCOM, (GLuint driverControl)) \
X(void, GetDriverControlStringQCOM, (GLuint driverControl, GLsizei bufSize, GLsizei * length, GLchar * driverControlString)) \
X(void, GetDriverControlsQCOM, (GLint * num, GLsizei size, GLuint * driverControls)) \
EXT_END() \
EXT_START("GL_QCOM_extended_get") \
X(void, ExtGetBufferPointervQCOM, (GLenum target, void ** params)) \
X(void, ExtGetBuffersQCOM, (GLuint * buffers, GLint maxBuffers, GLint * numBuffers)) \
X(void, ExtGetFramebuffersQCOM, (GLuint * framebuffers, GLint maxFramebuffers, GLint * numFramebuffers)) \
X(void, ExtGetRenderbuffersQCOM, (GLuint * renderbuffers, GLint maxRenderbuffers, GLint * numRenderbuffers)) \
X(void, ExtGetTexLevelParameterivQCOM, (GLuint texture, GLenum face, GLint level, GLenum pname, GLint * params)) \
X(void, ExtGetTexSubImageQCOM, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, void * texels)) \
X(void, ExtGetTexturesQCOM, (GLuint * textures, GLint maxTextures, GLint * numTextures)) \
X(void, ExtTexObjectStateOverrideiQCOM, (GLenum target, GLenum pname, GLint param)) \
EXT_END() \
EXT_START("GL_QCOM_extended_get2") \
X(void, ExtGetProgramBinarySourceQCOM, (GLuint program, GLenum shadertype, GLchar * source, GLint * length)) \
X(void, ExtGetProgramsQCOM, (GLuint * programs, GLint maxPrograms, GLint * numPrograms)) \
X(void, ExtGetShadersQCOM, (GLuint * shaders, GLint maxShaders, GLint * numShaders)) \
X(GLboolean, ExtIsProgramBinaryQCOM, (GLuint program)) \
EXT_END() \
EXT_START("GL_QCOM_frame_extrapolation") \
X(void, ExtrapolateTex2DQCOM, (GLuint src1, GLuint src2, GLuint output, GLfloat scaleFactor)) \
EXT_END() \
EXT_START("GL_QCOM_framebuffer_foveated") \
X(void, FramebufferFoveationConfigQCOM, (GLuint framebuffer, GLuint numLayers, GLuint focalPointsPerLayer, GLuint requestedFeatures, GLuint * providedFeatures)) \
X(void, FramebufferFoveationParametersQCOM, (GLuint framebuffer, GLuint layer, GLuint focalPoint, GLfloat focalX, GLfloat focalY, GLfloat gainX, GLfloat gainY, GLfloat foveaArea)) \
EXT_END() \
EXT_START("GL_QCOM_motion_estimation") \
X(void, TexEstimateMotionQCOM, (GLuint ref, GLuint target, GLuint output)) \
X(void, TexEstimateMotionRegionsQCOM, (GLuint ref, GLuint target, GLuint output, GLuint mask)) \
EXT_END() \
EXT_START("GL_QCOM_shader_framebuffer_fetch_noncoherent") \
X(void, FramebufferFetchBarrierQCOM, ()) \
EXT_END() \
EXT_START("GL_QCOM_shading_rate") \
X(void, ShadingRateQCOM, (GLenum rate)) \
EXT_END() \
EXT_START("GL_QCOM_texture_foveated") \
X(void, TextureFoveationParametersQCOM, (GLuint texture, GLuint layer, GLuint focalPoint, GLfloat focalX, GLfloat focalY, GLfloat gainX, GLfloat gainY, GLfloat foveaArea)) \
EXT_END() \
EXT_START("GL_QCOM_tiled_rendering") \
X(void, EndTilingQCOM, (GLbitfield preserveMask)) \
X(void, StartTilingQCOM, (GLuint x, GLuint y, GLuint width, GLuint height, GLbitfield preserveMask)) \
EXT_END() \
EXT_START("GL_SGIS_detail_texture") \
X(void, DetailTexFuncSGIS, (GLenum target, GLsizei n, const GLfloat * points)) \
X(void, GetDetailTexFuncSGIS, (GLenum target, GLfloat * points)) \
EXT_END() \
EXT_START("GL_SGIS_fog_function") \
X(void, FogFuncSGIS, (GLsizei n, const GLfloat * points)) \
X(void, GetFogFuncSGIS, (GLfloat * points)) \
EXT_END() \
EXT_START("GL_SGIS_multisample") \
X(void, SampleMaskSGIS, (GLclampf value, GLboolean invert)) \
X(void, SamplePatternSGIS, (GLenum pattern)) \
EXT_END() \
EXT_START("GL_SGIS_pixel_texture") \
X(void, GetPixelTexGenParameterfvSGIS, (GLenum pname, GLfloat * params)) \
X(void, GetPixelTexGenParameterivSGIS, (GLenum pname, GLint * params)) \
X(void, PixelTexGenParameterfSGIS, (GLenum pname, GLfloat param)) \
X(void, PixelTexGenParameterfvSGIS, (GLenum pname, const GLfloat * params)) \
X(void, PixelTexGenParameteriSGIS, (GLenum pname, GLint param)) \
X(void, PixelTexGenParameterivSGIS, (GLenum pname, const GLint * params)) \
EXT_END() \
EXT_START("GL_SGIS_point_parameters") \
X(void, PointParameterfSGIS, (GLenum pname, GLfloat param)) \
X(void, PointParameterfvSGIS, (GLenum pname, const GLfloat * params)) \
EXT_END() \
EXT_START("GL_SGIS_sharpen_texture") \
X(void, GetSharpenTexFuncSGIS, (GLenum target, GLfloat * points)) \
X(void, SharpenTexFuncSGIS, (GLenum target, GLsizei n, const GLfloat * points)) \
EXT_END() \
EXT_START("GL_SGIS_texture4D") \
X(void, TexImage4DSGIS, (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLint border, GLenum format, GLenum type, const void * pixels)) \
X(void, TexSubImage4DSGIS, (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint woffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei size4d, GLenum format, GLenum type, const void * pixels)) \
EXT_END() \
EXT_START("GL_SGIS_texture_color_mask") \
X(void, TextureColorMaskSGIS, (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha)) \
EXT_END() \
EXT_START("GL_SGIS_texture_filter4") \
X(void, GetTexFilterFuncSGIS, (GLenum target, GLenum filter, GLfloat * weights)) \
X(void, TexFilterFuncSGIS, (GLenum target, GLenum filter, GLsizei n, const GLfloat * weights)) \
EXT_END() \
EXT_START("GL_SGIX_async") \
X(void, AsyncMarkerSGIX, (GLuint marker)) \
X(void, DeleteAsyncMarkersSGIX, (GLuint marker, GLsizei range)) \
X(GLint, FinishAsyncSGIX, (GLuint * markerp)) \
X(GLuint, GenAsyncMarkersSGIX, (GLsizei range)) \
X(GLboolean, IsAsyncMarkerSGIX, (GLuint marker)) \
X(GLint, PollAsyncSGIX, (GLuint * markerp)) \
EXT_END() \
EXT_START("GL_SGIX_flush_raster") \
X(void, FlushRasterSGIX, ()) \
EXT_END() \
EXT_START("GL_SGIX_fragment_lighting") \
X(void, FragmentColorMaterialSGIX, (GLenum face, GLenum mode)) \
X(void, FragmentLightModelfSGIX, (GLenum pname, GLfloat param)) \
X(void, FragmentLightModelfvSGIX, (GLenum pname, const GLfloat * params)) \
X(void, FragmentLightModeliSGIX, (GLenum pname, GLint param)) \
X(void, FragmentLightModelivSGIX, (GLenum pname, const GLint * params)) \
X(void, FragmentLightfSGIX, (GLenum light, GLenum pname, GLfloat param)) \
X(void, FragmentLightfvSGIX, (GLenum light, GLenum pname, const GLfloat * params)) \
X(void, FragmentLightiSGIX, (GLenum light, GLenum pname, GLint param)) \
X(void, FragmentLightivSGIX, (GLenum light, GLenum pname, const GLint * params)) \
X(void, FragmentMaterialfSGIX, (GLenum face, GLenum pname, GLfloat param)) \
X(void, FragmentMaterialfvSGIX, (GLenum face, GLenum pname, const GLfloat * params)) \
X(void, FragmentMaterialiSGIX, (GLenum face, GLenum pname, GLint param)) \
X(void, FragmentMaterialivSGIX, (GLenum face, GLenum pname, const GLint * params)) \
X(void, GetFragmentLightfvSGIX, (GLenum light, GLenum pname, GLfloat * params)) \
X(void, GetFragmentLightivSGIX, (GLenum light, GLenum pname, GLint * params)) \
X(void, GetFragmentMaterialfvSGIX, (GLenum face, GLenum pname, GLfloat * params)) \
X(void, GetFragmentMaterialivSGIX, (GLenum face, GLenum pname, GLint * params)) \
X(void, LightEnviSGIX, (GLenum pname, GLint param)) \
EXT_END() \
EXT_START("GL_SGIX_framezoom") \
X(void, FrameZoomSGIX, (GLint factor)) \
EXT_END() \
EXT_START("GL_SGIX_igloo_interface") \
X(void, IglooInterfaceSGIX, (GLenum pname, const void * params)) \
EXT_END() \
EXT_START("GL_SGIX_instruments") \
X(GLint, GetInstrumentsSGIX, ()) \
X(void, InstrumentsBufferSGIX, (GLsizei size, GLint * buffer)) \
X(GLint, PollInstrumentsSGIX, (GLint * marker_p)) \
X(void, ReadInstrumentsSGIX, (GLint marker)) \
X(void, StartInstrumentsSGIX, ()) \
X(void, StopInstrumentsSGIX, (GLint marker)) \
EXT_END() \
EXT_START("GL_SGIX_list_priority") \
X(void, GetListParameterfvSGIX, (GLuint list, GLenum pname, GLfloat * params)) \
X(void, GetListParameterivSGIX, (GLuint list, GLenum pname, GLint * params)) \
X(void, ListParameterfSGIX, (GLuint list, GLenum pname, GLfloat param)) \
X(void, ListParameterfvSGIX, (GLuint list, GLenum pname, const GLfloat * params)) \
X(void, ListParameteriSGIX, (GLuint list, GLenum pname, GLint param)) \
X(void, ListParameterivSGIX, (GLuint list, GLenum pname, const GLint * params)) \
EXT_END() \
EXT_START("GL_SGIX_pixel_texture") \
X(void, PixelTexGenSGIX, (GLenum mode)) \
EXT_END() \
EXT_START("GL_SGIX_polynomial_ffd") \
X(void, DeformSGIX, (GLbitfield mask)) \
X(void, DeformationMap3dSGIX, (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, GLdouble w1, GLdouble w2, GLint wstride, GLint worder, const GLdouble * points)) \
X(void, DeformationMap3fSGIX, (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, GLfloat w1, GLfloat w2, GLint wstride, GLint worder, const GLfloat * points)) \
X(void, LoadIdentityDeformationMapSGIX, (GLbitfield mask)) \
EXT_END() \
EXT_START("GL_SGIX_reference_plane") \
X(void, ReferencePlaneSGIX, (const GLdouble * equation)) \
EXT_END() \
EXT_START("GL_SGIX_sprite") \
X(void, SpriteParameterfSGIX, (GLenum pname, GLfloat param)) \
X(void, SpriteParameterfvSGIX, (GLenum pname, const GLfloat * params)) \
X(void, SpriteParameteriSGIX, (GLenum pname, GLint param)) \
X(void, SpriteParameterivSGIX, (GLenum pname, const GLint * params)) \
EXT_END() \
EXT_START("GL_SGIX_tag_sample_buffer") \
X(void, TagSampleBufferSGIX, ()) \
EXT_END() \
EXT_START("GL_SGI_color_table") \
X(void, ColorTableParameterfvSGI, (GLenum target, GLenum pname, const GLfloat * params)) \
X(void, ColorTableParameterivSGI, (GLenum target, GLenum pname, const GLint * params)) \
X(void, ColorTableSGI, (GLenum target, GLenum internalformat, GLsizei width, GLenum format, GLenum type, const void * table)) \
X(void, CopyColorTableSGI, (GLenum target, GLenum internalformat, GLint x, GLint y, GLsizei width)) \
X(void, GetColorTableParameterfvSGI, (GLenum target, GLenum pname, GLfloat * params)) \
X(void, GetColorTableParameterivSGI, (GLenum target, GLenum pname, GLint * params)) \
X(void, GetColorTableSGI, (GLenum target, GLenum format, GLenum type, void * table)) \
EXT_END() \
EXT_START("GL_SUNX_constant_data") \
X(void, FinishTextureSUNX, ()) \
EXT_END() \
EXT_START("GL_SUN_global_alpha") \
X(void, GlobalAlphaFactorbSUN, (GLbyte factor)) \
X(void, GlobalAlphaFactordSUN, (GLdouble factor)) \
X(void, GlobalAlphaFactorfSUN, (GLfloat factor)) \
X(void, GlobalAlphaFactoriSUN, (GLint factor)) \
X(void, GlobalAlphaFactorsSUN, (GLshort factor)) \
X(void, GlobalAlphaFactorubSUN, (GLubyte factor)) \
X(void, GlobalAlphaFactoruiSUN, (GLuint factor)) \
X(void, GlobalAlphaFactorusSUN, (GLushort factor)) \
EXT_END() \
EXT_START("GL_SUN_mesh_array") \
X(void, DrawMeshArraysSUN, (GLenum mode, GLint first, GLsizei count, GLsizei width)) \
EXT_END() \
EXT_START("GL_SUN_triangle_list") \
X(void, ReplacementCodePointerSUN, (GLenum type, GLsizei stride, const void ** pointer)) \
X(void, ReplacementCodeubSUN, (GLubyte code)) \
X(void, ReplacementCodeubvSUN, (const GLubyte * code)) \
X(void, ReplacementCodeuiSUN, (GLuint code)) \
X(void, ReplacementCodeuivSUN, (const GLuint * code)) \
X(void, ReplacementCodeusSUN, (GLushort code)) \
X(void, ReplacementCodeusvSUN, (const GLushort * code)) \
EXT_END() \
EXT_START("GL_SUN_vertex") \
X(void, Color3fVertex3fSUN, (GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z)) \
X(void, Color3fVertex3fvSUN, (const GLfloat * c, const GLfloat * v)) \
X(void, Color4fNormal3fVertex3fSUN, (GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z)) \
X(void, Color4fNormal3fVertex3fvSUN, (const GLfloat * c, const GLfloat * n, const GLfloat * v)) \
X(void, Color4ubVertex2fSUN, (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y)) \
X(void, Color4ubVertex2fvSUN, (const GLubyte * c, const GLfloat * v)) \
X(void, Color4ubVertex3fSUN, (GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z)) \
X(void, Color4ubVertex3fvSUN, (const GLubyte * c, const GLfloat * v)) \
X(void, Normal3fVertex3fSUN, (GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z)) \
X(void, Normal3fVertex3fvSUN, (const GLfloat * n, const GLfloat * v)) \
X(void, ReplacementCodeuiColor3fVertex3fSUN, (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z)) \
X(void, ReplacementCodeuiColor3fVertex3fvSUN, (const GLuint * rc, const GLfloat * c, const GLfloat * v)) \
X(void, ReplacementCodeuiColor4fNormal3fVertex3fSUN, (GLuint rc, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z)) \
X(void, ReplacementCodeuiColor4fNormal3fVertex3fvSUN, (const GLuint * rc, const GLfloat * c, const GLfloat * n, const GLfloat * v)) \
X(void, ReplacementCodeuiColor4ubVertex3fSUN, (GLuint rc, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z)) \
X(void, ReplacementCodeuiColor4ubVertex3fvSUN, (const GLuint * rc, const GLubyte * c, const GLfloat * v)) \
X(void, ReplacementCodeuiNormal3fVertex3fSUN, (GLuint rc, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z)) \
X(void, ReplacementCodeuiNormal3fVertex3fvSUN, (const GLuint * rc, const GLfloat * n, const GLfloat * v)) \
X(void, ReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fSUN, (GLuint rc, GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z)) \
X(void, ReplacementCodeuiTexCoord2fColor4fNormal3fVertex3fvSUN, (const GLuint * rc, const GLfloat * tc, const GLfloat * c, const GLfloat * n, const GLfloat * v)) \
X(void, ReplacementCodeuiTexCoord2fNormal3fVertex3fSUN, (GLuint rc, GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z)) \
X(void, ReplacementCodeuiTexCoord2fNormal3fVertex3fvSUN, (const GLuint * rc, const GLfloat * tc, const GLfloat * n, const GLfloat * v)) \
X(void, ReplacementCodeuiTexCoord2fVertex3fSUN, (GLuint rc, GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z)) \
X(void, ReplacementCodeuiTexCoord2fVertex3fvSUN, (const GLuint * rc, const GLfloat * tc, const GLfloat * v)) \
X(void, ReplacementCodeuiVertex3fSUN, (GLuint rc, GLfloat x, GLfloat y, GLfloat z)) \
X(void, ReplacementCodeuiVertex3fvSUN, (const GLuint * rc, const GLfloat * v)) \
X(void, TexCoord2fColor3fVertex3fSUN, (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat x, GLfloat y, GLfloat z)) \
X(void, TexCoord2fColor3fVertex3fvSUN, (const GLfloat * tc, const GLfloat * c, const GLfloat * v)) \
X(void, TexCoord2fColor4fNormal3fVertex3fSUN, (GLfloat s, GLfloat t, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z)) \
X(void, TexCoord2fColor4fNormal3fVertex3fvSUN, (const GLfloat * tc, const GLfloat * c, const GLfloat * n, const GLfloat * v)) \
X(void, TexCoord2fColor4ubVertex3fSUN, (GLfloat s, GLfloat t, GLubyte r, GLubyte g, GLubyte b, GLubyte a, GLfloat x, GLfloat y, GLfloat z)) \
X(void, TexCoord2fColor4ubVertex3fvSUN, (const GLfloat * tc, const GLubyte * c, const GLfloat * v)) \
X(void, TexCoord2fNormal3fVertex3fSUN, (GLfloat s, GLfloat t, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z)) \
X(void, TexCoord2fNormal3fVertex3fvSUN, (const GLfloat * tc, const GLfloat * n, const GLfloat * v)) \
X(void, TexCoord2fVertex3fSUN, (GLfloat s, GLfloat t, GLfloat x, GLfloat y, GLfloat z)) \
X(void, TexCoord2fVertex3fvSUN, (const GLfloat * tc, const GLfloat * v)) \
X(void, TexCoord4fColor4fNormal3fVertex4fSUN, (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat r, GLfloat g, GLfloat b, GLfloat a, GLfloat nx, GLfloat ny, GLfloat nz, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, TexCoord4fColor4fNormal3fVertex4fvSUN, (const GLfloat * tc, const GLfloat * c, const GLfloat * n, const GLfloat * v)) \
X(void, TexCoord4fVertex4fSUN, (GLfloat s, GLfloat t, GLfloat p, GLfloat q, GLfloat x, GLfloat y, GLfloat z, GLfloat w)) \
X(void, TexCoord4fVertex4fvSUN, (const GLfloat * tc, const GLfloat * v)) \
EXT_END() \

#if !defined(__APPLE__)
/* ----TYPEDEFS------------------------------------------------------------------------------------------------------ */
#define X(retval, name, params) typedef retval (*VdFwProcGL_##name)params;
#define VER_START(v)
#define VER_END(v)
#define EXT_START(name)
#define EXT_END()
VD_FW_OPENGL_CORE_FUNCTIONS
#undef X
#undef VER_START
#undef VER_END
#undef EXT_START
#undef EXT_END

/* ----EXTERNS------------------------------------------------------------------------------------------------------- */
#define X(retval, name, params) extern VdFwProcGL_##name gl##name;
#define VER_START(v)
#define VER_END(v)
#define EXT_START(name)
#define EXT_END()
VD_FW_OPENGL_CORE_FUNCTIONS
#undef X
#undef VER_START
#undef VER_END
#undef EXT_START
#undef EXT_END
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

static int vd_fw__load_opengl(VdFwGlConfig *config);
static int vd_fw__lookup_gl_extension(const char *q, VdFwGlConfig *config);

#ifdef _WIN32

#if VD_FW_WIN32_LINKER_COMMENTS
#pragma comment(lib, "kernel32.lib")
#pragma execution_character_set("utf-8")
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

/* ----Winnt.h------------------------------------------------------------------------------------------------------- */
#define VD_FW_LOWORD(l)           ((VdFwWORD)(((VdFwDWORD_PTR)(l)) & 0xffff))
#define VD_FW_HIWORD(l)           ((VdFwWORD)((((VdFwDWORD_PTR)(l)) >> 16) & 0xffff))
#define VD_FW_LOBYTE(w)           ((VdFwBYTE)(((VdFwDWORD_PTR)(w)) & 0xff))
#define VD_FW_HIBYTE(w)           ((VdFwBYTE)((((VdFwDWORD_PTR)(w)) >> 8) & 0xff))

#define VD_FW__WIN32_DELETE                           (0x00010000L)
#define VD_FW__WIN32_READ_CONTROL                     (0x00020000L)
#define VD_FW__WIN32_WRITE_DAC                        (0x00040000L)
#define VD_FW__WIN32_WRITE_OWNER                      (0x00080000L)
#define VD_FW__WIN32_SYNCHRONIZE                      (0x00100000L)
#define VD_FW__WIN32_STANDARD_RIGHTS_REQUIRED         (0x000F0000L)
#define VD_FW__WIN32_STANDARD_RIGHTS_READ             (VD_FW__WIN32_READ_CONTROL)
#define VD_FW__WIN32_STANDARD_RIGHTS_WRITE            (VD_FW__WIN32_READ_CONTROL)
#define VD_FW__WIN32_STANDARD_RIGHTS_EXECUTE          (VD_FW__WIN32_READ_CONTROL)
#define VD_FW__WIN32_STANDARD_RIGHTS_ALL              (0x001F0000L)
#define VD_FW__WIN32_SPECIFIC_RIGHTS_ALL              (0x0000FFFFL)

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
VD_FW_DECLARE_HANDLE(VdFwHKEY);

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

typedef enum {
    VdFwMDT_EFFECTIVE_DPI = 0,
    VdFwMDT_ANGULAR_DPI = 1,
    VdFwMDT_RAW_DPI = 2,
    VdFwMDT_DEFAULT
} VdFwMONITOR_DPI_TYPE;

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

typedef struct VdFwtagMONITORINFOEXW {
    VdFwMONITORINFO monitor_info;
    VdFwWCHAR       szDevice[32];
} VdFwMONITORINFOEXW, *VdFwLPMONITORINFOEXW;

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

typedef VdFwBOOL (*VdFwMONITORENUMPROC)(VdFwHMONITOR monitor, VdFwHDC hdc, VdFwLPRECT rect, VdFwLPARAM lpparam);

/* ----Shell32.dll--------------------------------------------------------------------------------------------------- */
#define VD_FW_ABM_GETAUTOHIDEBAREX    0x0000000b
#define VD_FW_ABM_SETAUTOHIDEBAREX    0x0000000c
#define VD_FW_ABM_GETAUTOHIDEBAR      0x00000007

/* ----ntdll.dll----------------------------------------------------------------------------------------------------- */
typedef struct VdFw_OSVERSIONINFOW {
  VdFwULONG dwOSVersionInfoSize;
  VdFwULONG dwMajorVersion;
  VdFwULONG dwMinorVersion;
  VdFwULONG dwBuildNumber;
  VdFwULONG dwPlatformId;
  VdFwWCHAR szCSDVersion[128];
} VdFwOSVERSIONINFOW, *VdFwPOSVERSIONINFOW, *VdFwLPOSVERSIONINFOW, VdFwRTL_OSVERSIONINFOW, *VdFwPRTL_OSVERSIONINFOW;

/* ----Winmm.dll----------------------------------------------------------------------------------------------------- */
typedef VdFwUINT VdFwMMRESULT;

/* ----UxTheme.dll--------------------------------------------------------------------------------------------------- */
typedef struct VdFw_MARGINS
{
    int cxLeftWidth;
    int cxRightWidth;
    int cyTopHeight;
    int cyBottomHeight;
} VdFwMARGINS, * VdFwPMARGINS;

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

typedef struct VdFw_DISPLAY_DEVICEA {
  VdFwDWORD cb;
  VdFwCHAR  DeviceName[32];
  VdFwCHAR  DeviceString[128];
  VdFwDWORD StateFlags;
  VdFwCHAR  DeviceID[128];
  VdFwCHAR  DeviceKey[128];
} VdFwDISPLAY_DEVICEA, *VdFwPDISPLAY_DEVICEA, *VdFwLPDISPLAY_DEVICEA;

typedef struct VdFw_DISPLAY_DEVICEW {
  VdFwDWORD cb;
  VdFwWCHAR DeviceName[32];
  VdFwWCHAR DeviceString[128];
  VdFwDWORD StateFlags;
  VdFwWCHAR DeviceID[128];
  VdFwWCHAR DeviceKey[128];
} VdFwDISPLAY_DEVICEW, *VdFwPDISPLAY_DEVICEW, *VdFwLPDISPLAY_DEVICEW;

#define VD_FW__WIN32_EDD_GET_DEVICE_INTERFACE_NAME 0x00000001

typedef struct VdFw_POINTL
{
    VdFwLONG  x;
    VdFwLONG  y;
} VdFwPOINTL, *VdFwPPOINTL;

typedef struct VdFw_devicemodeW {
  VdFwWCHAR dmDeviceName[32];
  VdFwWORD  dmSpecVersion;
  VdFwWORD  dmDriverVersion;
  VdFwWORD  dmSize;
  VdFwWORD  dmDriverExtra;
  VdFwDWORD dmFields;
  union {
    struct {
      short dmOrientation;
      short dmPaperSize;
      short dmPaperLength;
      short dmPaperWidth;
      short dmScale;
      short dmCopies;
      short dmDefaultSource;
      short dmPrintQuality;
    } printer;
    VdFwPOINTL dmPosition;
    struct {
      VdFwPOINTL dmPosition;
      VdFwDWORD  dmDisplayOrientation;
      VdFwDWORD  dmDisplayFixedOutput;
    } displays;
  } extra;
  short dmColor;
  short dmDuplex;
  short dmYResolution;
  short dmTTOption;
  short dmCollate;
  VdFwWCHAR dmFormName[32];
  VdFwWORD  dmLogPixels;
  VdFwDWORD dmBitsPerPel;
  VdFwDWORD dmPelsWidth;
  VdFwDWORD dmPelsHeight;
  union {
    VdFwDWORD dmDisplayFlags;
    VdFwDWORD dmNup;
  } DUMMYUNIONNAME2;
  VdFwDWORD dmDisplayFrequency;
  VdFwDWORD dmICMMethod;
  VdFwDWORD dmICMIntent;
  VdFwDWORD dmMediaType;
  VdFwDWORD dmDitherType;
  VdFwDWORD dmReserved1;
  VdFwDWORD dmReserved2;
  VdFwDWORD dmPanningWidth;
  VdFwDWORD dmPanningHeight;
} VdFwDEVMODEW, *VdFwPDEVMODEW, *VdFwNPDEVMODEW, *VdFwLPDEVMODEW;

/* ----OpenGL32.dll-------------------------------------------------------------------------------------------------- */
VD_FW_DECLARE_HANDLE(VdFwHGLRC);
typedef VdFwINT_PTR(* VdFwPROC)();

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

/* ----winreg.h------------------------------------------------------------------------------------------------------ */
typedef VdFwDWORD VdFwACCESS_MASK;
typedef VdFwACCESS_MASK VdFwREGSAM;

#define VD_FW__WIN32_KEY_QUERY_VALUE         (0x0001)
#define VD_FW__WIN32_KEY_SET_VALUE           (0x0002)
#define VD_FW__WIN32_KEY_CREATE_SUB_KEY      (0x0004)
#define VD_FW__WIN32_KEY_ENUMERATE_SUB_KEYS  (0x0008)
#define VD_FW__WIN32_KEY_NOTIFY              (0x0010)
#define VD_FW__WIN32_KEY_CREATE_LINK         (0x0020)
#define VD_FW__WIN32_KEY_WOW64_32KEY         (0x0200)
#define VD_FW__WIN32_KEY_WOW64_64KEY         (0x0100)
#define VD_FW__WIN32_KEY_WOW64_RES           (0x0300)

#define VD_FW__WIN32_KEY_READ       ((VD_FW__WIN32_STANDARD_RIGHTS_READ       |\
                                     VD_FW__WIN32_KEY_QUERY_VALUE             |\
                                     VD_FW__WIN32_KEY_ENUMERATE_SUB_KEYS      |\
                                     VD_FW__WIN32_KEY_NOTIFY)                  \
                                     &                                         \
                                    (~VD_FW__WIN32_SYNCHRONIZE))

#define VD_FW__WIN32_KEY_WRITE      ((VD_FW__WIN32_STANDARD_RIGHTS_WRITE      |\
                                     VD_FW__WIN32_KEY_SET_VALUE               |\
                                     VD_FW__WIN32_KEY_CREATE_SUB_KEY)          \
                                     &                                         \
                                    (~VD_FW__WIN32_SYNCHRONIZE))

#define VD_FW__WIN32_KEY_EXECUTE    ((VD_FW__WIN32_KEY_READ)                   \
                                     &                                         \
                                    (~VD_FW__WIN32_SYNCHRONIZE))

#define VD_FW__WIN32_KEY_ALL_ACCESS      ((VD_FW__WIN32_STANDARD_RIGHTS_ALL  |\
                                    VD_FW__WIN32_KEY_QUERY_VALUE             |\
                                    VD_FW__WIN32_KEY_SET_VALUE               |\
                                    VD_FW__WIN32_KEY_CREATE_SUB_KEY          |\
                                    VD_FW__WIN32_KEY_ENUMERATE_SUB_KEYS      |\
                                    VD_FW__WIN32_KEY_NOTIFY                  |\
                                    VD_FW__WIN32_KEY_CREATE_LINK)             \
                                    &                                         \
                                    (~VD_FW__WIN32_SYNCHRONIZE))

/* ----SetupAPI.dll-------------------------------------------------------------------------------------------------- */
typedef VdFwLPVOID VdFwHDEVINFO;
typedef struct VdFw_GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[8];
} VdFw__Win32GUID;

#define VD_FW__WIN32_DIGCF_DEVICEINTERFACE 0x00000010

typedef struct VdFw_SP_DEVICE_INTERFACE_DATA {
    VdFwDWORD cbSize;
    VdFw__Win32GUID InterfaceClassGuid;
    VdFwDWORD Flags;
    VdFwULONG_PTR Reserved;
} VdFwSP_DEVICE_INTERFACE_DATA, *VdFwPSP_DEVICE_INTERFACE_DATA;

typedef struct VdFw_SP_DEVICE_INTERFACE_DETAIL_DATA_W {
    VdFwDWORD  cbSize;
    VdFwWCHAR  DevicePath[2];
} VdFwSP_DEVICE_INTERFACE_DETAIL_DATA_W, *VdFwPSP_DEVICE_INTERFACE_DETAIL_DATA_W;

typedef struct VdFw_SP_DEVINFO_DATA {
    VdFwDWORD cbSize;
    VdFw__Win32GUID ClassGuid;
    VdFwDWORD DevInst;
    VdFwULONG_PTR Reserved;
} VdFwSP_DEVINFO_DATA, *VdFwPSP_DEVINFO_DATA;

#define VD_FW__WIN32_DICS_FLAG_GLOBAL         0x00000001
#define VD_FW__WIN32_DICS_FLAG_CONFIGSPECIFIC 0x00000002
#define VD_FW__WIN32_DICS_FLAG_CONFIGGENERAL  0x00000004
#define VD_FW__WIN32_DIREG_DEV       0x00000001
#define VD_FW__WIN32_DIREG_DRV       0x00000002
#define VD_FW__WIN32_DIREG_BOTH      0x00000004

#define VD_FW__WIN32_CDS_UPDATEREGISTRY           0x00000001
#define VD_FW__WIN32_CDS_TEST                     0x00000002
#define VD_FW__WIN32_CDS_FULLSCREEN               0x00000004
#define VD_FW__WIN32_CDS_GLOBAL                   0x00000008
#define VD_FW__WIN32_CDS_SET_PRIMARY              0x00000010
#define VD_FW__WIN32_CDS_VIDEOPARAMETERS          0x00000020
#define VD_FW__WIN32_CDS_RESET                    0x40000000
#define VD_FW__WIN32_CDS_RESET_EX                 0x20000000
#define VD_FW__WIN32_CDS_NORESET                  0x10000000
#define VD_FW__WIN32_DISP_CHANGE_SUCCESSFUL       0

/* ----Advapi32.dll-------------------------------------------------------------------------------------------------- */
typedef VdFwLONG VdFwLSTATUS;

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
#pragma pack(pop)

/* ----Win32 Functions----------------------------------------------------------------------------------------------- */
#define VD_FW__WIN32_FUNCTIONS \
V("User32.dll") \
X(VdFwBOOL,     GetMessageA, (VdFwLPMSG lpMsg, VdFwHWND hWnd, VdFwUINT wMsgFilterMin, VdFwUINT wMsgFilterMax)) \
X(VdFwBOOL,     GetMessageW, (VdFwLPMSG lpMsg, VdFwHWND hWnd, VdFwUINT wMsgFilterMin, VdFwUINT wMsgFilterMax)) \
X(VdFwBOOL,     TranslateMessage, (const VdFwMSG* lpMsg)) \
X(VdFwLRESULT,  DispatchMessageA, (const VdFwMSG* lpMsg)) \
X(VdFwLRESULT,  DispatchMessageW, (const VdFwMSG* lpMsg)) \
X(VdFwBOOL,     PostMessageA, (VdFwHWND hWnd, VdFwUINT Msg, VdFwWPARAM wParam, VdFwLPARAM lParam)) \
X(VdFwBOOL,     PostMessageW, (VdFwHWND hWnd, VdFwUINT Msg, VdFwWPARAM wParam, VdFwLPARAM lParam)) \
X(VdFwLRESULT,  DefWindowProcA, (VdFwHWND hWnd, VdFwUINT Msg, VdFwWPARAM wParam, VdFwLPARAM lParam)) \
X(VdFwLRESULT,  DefWindowProcW, (VdFwHWND hWnd, VdFwUINT Msg, VdFwWPARAM wParam, VdFwLPARAM lParam)) \
X(void,         PostQuitMessage, (int nExitCode)) \
X(VdFwATOM,     RegisterClassExA, (const VdFwWNDCLASSEXA* unnamedParam1)) \
X(VdFwATOM,     RegisterClassExW, (const VdFwWNDCLASSEXW* unnamedParam1)) \
X(VdFwHWND,     CreateWindowExA, (VdFwDWORD dwExStyle, VdFwLPCSTR lpClassName, VdFwLPCSTR lpWindowName, VdFwDWORD dwStyle, int X, int Y, int nWidth, int nHeight, VdFwHWND hWndParent, VdFwHMENU hMenu, VdFwHINSTANCE hInstance, VdFwLPVOID lpParam)) \
X(VdFwHWND,     CreateWindowExW, (VdFwDWORD dwExStyle, VdFwLPCWSTR lpClassName, VdFwLPCWSTR lpWindowName, VdFwDWORD dwStyle, int X, int Y, int nWidth, int nHeight, VdFwHWND hWndParent, VdFwHMENU hMenu, VdFwHINSTANCE hInstance, VdFwLPVOID lpParam)) \
X(VdFwBOOL,     DestroyWindow, (VdFwHWND hWnd)) \
X(VdFwBOOL,     ShowWindow, (VdFwHWND hWnd, int nCmdShow)) \
X(VdFwBOOL,     SetWindowPos, (VdFwHWND hWnd, VdFwHWND hWndInsertAfter, int X, int Y, int cx, int cy, VdFwUINT uFlags)) \
X(VdFwBOOL,     GetWindowPlacement, (VdFwHWND hWnd, VdFwWINDOWPLACEMENT* lpwndpl)) \
X(VdFwBOOL,     SetWindowPlacement, (VdFwHWND hWnd, const VdFwWINDOWPLACEMENT* lpwndpl)) \
X(VdFwBOOL,     IsZoomed, (VdFwHWND hWnd)) \
X(VdFwUINT,     MapVirtualKeyA, (VdFwUINT uCode, VdFwUINT uMapType)) \
X(VdFwUINT,     MapVirtualKeyW, (VdFwUINT uCode, VdFwUINT uMapType)) \
X(VdFwHWND,     SetCapture, (VdFwHWND hWnd)) \
X(VdFwBOOL,     ReleaseCapture, (void)) \
X(int,          GetSystemMetrics, (int nIndex)) \
X(VdFwBOOL,     UpdateWindow, (VdFwHWND hWnd)) \
X(VdFwHDC,      GetDC, (VdFwHWND hWnd)) \
X(int,          ReleaseDC, (VdFwHWND hWnd, VdFwHDC hDC)) \
X(VdFwHDC,      BeginPaint, (VdFwHWND hWnd, VdFwLPPAINTSTRUCT lpPaint)) \
X(VdFwBOOL,     EndPaint, (VdFwHWND hWnd, const VdFwPAINTSTRUCT* lpPaint)) \
X(int,          SetWindowRgn, (VdFwHWND hWnd, VdFwHRGN hRgn, VdFwBOOL bRedraw)) \
X(VdFwBOOL,     SetWindowTextA, (VdFwHWND hWnd, VdFwLPCSTR lpString)) \
X(VdFwBOOL,     SetWindowTextW, (VdFwHWND hWnd, VdFwLPCWSTR lpString)) \
X(VdFwBOOL,     GetClientRect, (VdFwHWND hWnd, VdFwLPRECT lpRect)) \
X(VdFwBOOL,     GetWindowRect, (VdFwHWND hWnd, VdFwLPRECT lpRect)) \
X(int,          ShowCursor, (VdFwBOOL bShow)) \
X(VdFwBOOL,     SetCursorPos, (int X, int Y)) \
X(VdFwBOOL,     GetCursorPos, (VdFwLPPOINT lpPoint)) \
X(VdFwBOOL,     ScreenToClient, (VdFwHWND hWnd, VdFwLPPOINT lpPoint)) \
X(VdFwBOOL,     EqualRect, (const VdFwRECT* lprc1, const VdFwRECT* lprc2)) \
X(VdFwBOOL,     PtInRect, (const VdFwRECT* lprc, VdFwPOINT pt)) \
X(VdFwLONG,     GetWindowLongA, (VdFwHWND, int nIndex)) \
X(VdFwLONG,     SetWindowLongA, (VdFwHWND hWnd, int nIndex, VdFwLONG dwNewLong)) \
X(VdFwLONG,     SetWindowLongW, (VdFwHWND hWnd, int nIndex, VdFwLONG dwNewLong)) \
X(VdFwLONG_PTR, GetWindowLongPtrA, (VdFwHWND hWnd, int nIndex)) \
X(VdFwLONG_PTR, GetWindowLongPtrW, (VdFwHWND hWnd, int nIndex)) \
X(VdFwLONG_PTR, SetWindowLongPtrA, (VdFwHWND hWnd, int nIndex, VdFwLONG_PTR dwNewLong)) \
X(VdFwLONG_PTR, SetWindowLongPtrW, (VdFwHWND hWnd, int nIndex, VdFwLONG_PTR dwNewLong)) \
X(VdFwHCURSOR,  LoadCursorA, (VdFwHINSTANCE hInstance, VdFwLPCSTR lpCursorName)) \
X(VdFwHCURSOR,  LoadCursorW, (VdFwHINSTANCE hInstance, VdFwLPCWSTR lpCursorName)) \
X(VdFwHICON,    CreateIconIndirect, (VdFwPICONINFO piconinfo)) \
X(VdFwHMONITOR, MonitorFromWindow, (VdFwHWND hwnd, VdFwDWORD dwFlags)) \
X(VdFwHMONITOR, MonitorFromPoint, (VdFwPOINT pt, VdFwDWORD dwFlags)) \
X(VdFwBOOL,     EnumDisplayMonitors, (VdFwHDC hdc, VdFwLPRECT lprcClip, VdFwMONITORENUMPROC lpfnEnum, VdFwLPARAM dwData)) \
X(VdFwBOOL,     EnumDisplayDevicesA, (VdFwLPCSTR lpDevice, VdFwDWORD iDevNum, VdFwPDISPLAY_DEVICEA lpDisplayDevice, VdFwDWORD dwFlags)) \
X(VdFwBOOL,     EnumDisplayDevicesW, (VdFwLPCWSTR lpDevice, VdFwDWORD iDevNum, VdFwPDISPLAY_DEVICEW lpDisplayDevice, VdFwDWORD dwFlags)) \
X(VdFwBOOL,     EnumDisplaySettingsW, (VdFwLPCSTR lpszDeviceName, VdFwDWORD iModeNum, VdFwDEVMODEW *lpDevMode)) \
X(VdFwLONG,     ChangeDisplaySettingsW, (VdFwDEVMODEW *lpDevMode, VdFwDWORD dwFlags)) \
X(VdFwBOOL,     GetMonitorInfoA, (VdFwHMONITOR hMonitor, VdFwLPMONITORINFO lpmi)) \
X(VdFwBOOL,     GetMonitorInfoW, (VdFwHMONITOR hMonitor, VdFwLPMONITORINFO lpmi)) \
X(VdFwUINT,     GetDpiForWindow, (VdFwHWND hwnd)) \
X(VdFwHRESULT,  GetDpiForMonitor, (VdFwHMONITOR hmonitor, VdFwMONITOR_DPI_TYPE dpiType, VdFwUINT *dpiX, VdFwUINT *dpiY)) \
X(VdFwBOOL,     SetProcessDpiAwarenessContext, (VdFwDPI_AWARENESS_CONTEXT value)) \
X(VdFwBOOL,     GetWindowInfo, (VdFwHWND hwnd, VdFwPWINDOWINFO pwi)) \
X(VdFwBOOL,     RegisterRawInputDevices, (VdFwPCRAWINPUTDEVICE pRawInputDevices, VdFwUINT uiNumDevices, VdFwUINT cbSize)) \
X(VdFwUINT,     GetRawInputData, (VdFwHRAWINPUT hRawInput, VdFwUINT uiCommand, VdFwLPVOID pData, VdFwPUINT pcbSize, VdFwUINT cbSizeHeader)) \
X(VdFwUINT,     GetRawInputDeviceInfoA, (VdFwHANDLE hDevice, VdFwUINT uiCommand, VdFwLPVOID pData, VdFwPUINT pcbSize)) \
X(VdFwUINT,     GetRawInputDeviceInfoW, (VdFwHANDLE hDevice, VdFwUINT uiCommand, VdFwLPVOID pData, VdFwPUINT pcbSize)) \
X(VdFwUINT_PTR, SetTimer, (VdFwHWND hWnd, VdFwUINT_PTR nIDEvent, VdFwUINT uElapse, VdFwTIMERPROC lpTimerFunc)) \
X(VdFwBOOL,     KillTimer, (VdFwHWND hWnd, VdFwUINT_PTR uIDEvent)) \
X(VdFwHKL,      GetKeyboardLayout, (VdFwDWORD idThread)) \
X(VdFwHWND,     SetFocus, (VdFwHWND hWnd)) \
X(VdFwBOOL,     SetForegroundWindow, (VdFwHWND hWnd)) \
X(VdFwSHORT,    GetKeyState, (int nVirtKey)) \
X(VdFwBOOL,     AdjustWindowRect, (VdFwLPRECT lpRect, VdFwDWORD dwStyle, VdFwBOOL bMenu)) \
VE() \
V("Shell32.dll") \
X(VdFwUINT_PTR, SHAppBarMessage, (VdFwDWORD dwMessage, VdFwPAPPBARDATA pData)) \
VE() \
V("ntdll.dll") \
X(void,         RtlGetVersion, (VdFwPRTL_OSVERSIONINFOW info)) \
VE() \
V("Winmm.dll") \
X(VdFwMMRESULT, timeBeginPeriod, (VdFwUINT uPeriod)) \
VE() \
V("UxTheme.dll") \
X(VdFwBOOL,     IsThemeActive, (void)) \
VE() \
V("Dwmapi.dll") \
X(VdFwHRESULT,  DwmExtendFrameIntoClientArea, (VdFwHWND hWnd, const VdFwMARGINS* pMarInset)) \
X(VdFwHRESULT,  DwmIsCompositionEnabled, (VdFwBOOL *pfEnabled)) \
X(VdFwHRESULT,  DwmSetWindowAttribute, (VdFwHWND hwnd, VdFwDWORD dwAttribute, VdFwLPCVOID pvAttribute, VdFwDWORD cbAttribute)) \
X(VdFwHRESULT,  DwmFlush, (void)) \
VE() \
V("Gdi32.dll") \
X(int,          ChoosePixelFormat, (VdFwHDC hdc, const VdFwPIXELFORMATDESCRIPTOR *ppfd)) \
X(VdFwHBITMAP,  CreateBitmap, (int nWidth, int nHeight, VdFwUINT nPlanes, VdFwUINT nBitCount, const void* lpBits)) \
X(VdFwHRGN,     CreateRectRgnIndirect, (const VdFwRECT *lprect)) \
X(int,          DescribePixelFormat, (VdFwHDC hdc, int iPixelFormat, VdFwUINT nBytes, VdFwLPPIXELFORMATDESCRIPTOR ppfd)) \
X(VdFwHGDIOBJ,  GetStockObject, (int i)) \
X(VdFwBOOL,     SetPixelFormat, (VdFwHDC hdc, int format, const VdFwPIXELFORMATDESCRIPTOR* ppfd)) \
X(VdFwHBITMAP,  CreateDIBSection, (VdFwHDC hdc, const VdFwBITMAPINFO* pbmi, VdFwUINT usage, void** ppvBits, VdFwHANDLE hSection, VdFwDWORD offset)) \
X(VdFwBOOL,     SwapBuffers, (VdFwHDC unnamedParam1)) \
VE() \
V("Hid.dll") \
X(VdFwNTSTATUS, HidP_GetCaps, (VdFwPHIDP_PREPARSED_DATA PreparsedData, VdFwPHIDP_CAPS Capabilities)) \
X(VdFwNTSTATUS, HidP_GetButtonCaps, (VdFwHIDP_REPORT_TYPE ReportType, VdFwPHIDP_BUTTON_CAPS ButtonCaps, VdFwPUSHORT ButtonCapsLength, VdFwPHIDP_PREPARSED_DATA PreparsedData)) \
X(VdFwNTSTATUS, HidP_GetValueCaps, (VdFwHIDP_REPORT_TYPE ReportType, VdFwPHIDP_VALUE_CAPS ValueCaps, VdFwPUSHORT ValueCapsLength, VdFwPHIDP_PREPARSED_DATA PreparsedData)) \
X(VdFwNTSTATUS, HidP_GetUsages, (VdFwHIDP_REPORT_TYPE ReportType, VdFwUSAGE UsagePage, VdFwUSHORT LinkCollection, VdFwPUSAGE UsageList, VdFwPULONG UsageLength, VdFwPHIDP_PREPARSED_DATA PreparsedData, VdFwPCHAR Report, VdFwULONG ReportLength)) \
X(VdFwNTSTATUS, HidP_GetData, (VdFwHIDP_REPORT_TYPE ReportType, VdFwPHIDP_DATA DataList, VdFwPULONG DataLength, VdFwPHIDP_PREPARSED_DATA PreparsedData, VdFwPCHAR Report, VdFwULONG ReportLength)) \
X(VdFwULONG,    HidP_MaxUsageListLength, (VdFwHIDP_REPORT_TYPE ReportType, VdFwUSAGE UsagePage, VdFwPHIDP_PREPARSED_DATA PreparsedData)) \
X(VdFwULONG,    HidP_MaxDataListLength, (VdFwHIDP_REPORT_TYPE ReportType, VdFwPHIDP_PREPARSED_DATA PreparsedData)) \
X(VdFwNTSTATUS, HidP_GetUsageValue, (VdFwHIDP_REPORT_TYPE ReportType, VdFwUSAGE UsagePage, VdFwUSHORT LinkCollection, VdFwUSAGE Usage, VdFwPULONG UsageValue, VdFwPHIDP_PREPARSED_DATA PreparsedData, VdFwPCHAR Report, VdFwULONG ReportLength)) \
X(VdFwBOOLEAN,  HidD_GetManufacturerString, (VdFwHANDLE HidDeviceObject, void *Buffer, VdFwULONG BufferLength)) \
X(VdFwBOOLEAN,  HidD_GetProductString, (VdFwHANDLE HidDeviceObject, void *Buffer, VdFwULONG BufferLength)) \
X(VdFwBOOLEAN,  HidD_SetFeature, (VdFwHANDLE HidDeviceObject, void *ReportBuffer, VdFwULONG ReportBufferLength)) \
X(VdFwBOOLEAN,  HidD_SetOutputReport, (VdFwHANDLE HidDeviceObject, void *ReportBuffer, VdFwULONG ReportBufferLength)) \
VE() \
V("SetupAPI.dll") \
X(VdFwHDEVINFO, SetupDiGetClassDevsW, (VdFw__Win32GUID *ClassGuid, VdFwPCWSTR Enumerator, VdFwHWND hwndParent, VdFwDWORD Flags)) \
X(VdFwBOOL,     SetupDiEnumDeviceInterfaces, (VdFwHDEVINFO DeviceInfoSet, VdFwPSP_DEVINFO_DATA DeviceInfoData, VdFw__Win32GUID *InterfaceClassGuid, VdFwDWORD MemberIndex, VdFwPSP_DEVICE_INTERFACE_DATA DeviceInterfaceData)) \
X(VdFwBOOL,     SetupDiGetDeviceInterfaceDetailW, (VdFwHDEVINFO DeviceInfoSet, VdFwPSP_DEVICE_INTERFACE_DATA DeviceInterfaceData, VdFwPSP_DEVICE_INTERFACE_DETAIL_DATA_W DeviceInterfaceDetailData, VdFwDWORD DeviceInterfaceDetailDataSize, VdFwPDWORD RequiredSize, VdFwPSP_DEVINFO_DATA DeviceInfoData)) \
X(VdFwBOOL,     SetupDiEnumDeviceInfo, (VdFwHDEVINFO DeviceInfoSet, VdFwDWORD MemberIndex, VdFwPSP_DEVINFO_DATA DeviceInfoData)) \
X(VdFwHKEY,     SetupDiOpenDevRegKey, (VdFwHDEVINFO DeviceInfoSet, VdFwPSP_DEVINFO_DATA DeviceInfoData, VdFwDWORD Scope, VdFwDWORD HwProfile, VdFwDWORD KeyType, VdFwREGSAM samDesired)) \
VE() \
V("Advapi32.dll") \
X(VdFwLSTATUS,  RegQueryValueExA, (VdFwHKEY hKey, VdFwLPCSTR lpValueName, VdFwLPDWORD lpReserved, VdFwLPDWORD lpType, VdFwLPBYTE lpData, VdFwLPDWORD lpcbData)) \
X(VdFwLSTATUS,  RegQueryValueExW, (VdFwHKEY hKey, VdFwLPCWSTR lpValueName, VdFwLPDWORD lpReserved, VdFwLPDWORD lpType, VdFwLPBYTE lpData, VdFwLPDWORD lpcbData)) \
X(VdFwLSTATUS,  RegCloseKey, (VdFwHKEY hKey)) \
VE() \

#define X(retval, name, params) typedef retval (*VdFwProc##name)params;
#define V(dlllpath)
#define VE()
VD_FW__WIN32_FUNCTIONS
#undef X
#undef V
#undef VE

#define _X2(s, e) s##e
#define X(retval, name, params) static _X2(VdFwProc,name) _X2(VdFw,name);
#define V(dlllpath)
#define VE()
VD_FW__WIN32_FUNCTIONS
#undef X
#undef _X2
#undef V
#undef VE

typedef VdFwHGLRC (*VdFwProcwglCreateContext)(VdFwHDC hDC);
typedef VdFwBOOL  (*VdFwProcwglMakeCurrent)(VdFwHDC hDC, VdFwHGLRC hGLRC);
typedef VdFwBOOL  (*VdFwProcwglDeleteContext)(VdFwHGLRC hGLRC);
typedef VdFwPROC  (*VdFwProcwglGetProcAddress)(VdFwLPCSTR pName);
static VdFwProcwglCreateContext     VdFwwglCreateContext;
static VdFwProcwglMakeCurrent       VdFwwglMakeCurrent;
static VdFwProcwglDeleteContext     VdFwwglDeleteContext;
static VdFwProcwglGetProcAddress    VdFwwglGetProcAddress;
typedef VdFwBOOL  (*VdFwProcwglSwapIntervalExt)(int interval);
typedef VdFwHGLRC (*VdFwProcwglCreateContextAttribsARB)(VdFwHDC hDC, VdFwHGLRC hShareContext, const int* attribList);
typedef VdFwBOOL  (*VdFwProcwglChoosePixelFormatARB)(VdFwHDC hdc, const int* piAttribIList, const float* pfAttribFList, VdFwUINT nMaxFormats, int* piFormats, VdFwUINT* nNumFormats);

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
#define WGL_RED_BITS_ARB                            0x2015
#define WGL_RED_SHIFT_ARB                           0x2016
#define WGL_GREEN_BITS_ARB                          0x2017
#define WGL_GREEN_SHIFT_ARB                         0x2018
#define WGL_BLUE_BITS_ARB                           0x2019
#define WGL_BLUE_SHIFT_ARB                          0x201A
#define WGL_ALPHA_BITS_ARB                          0x201B
#define WGL_ALPHA_SHIFT_ARB                         0x201C
#define WGL_ACCUM_BITS_ARB                          0x201D
#define WGL_ACCUM_RED_BITS_ARB                      0x201E
#define WGL_ACCUM_GREEN_BITS_ARB                    0x201F
#define WGL_ACCUM_BLUE_BITS_ARB                     0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB                    0x2021
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
    VD_FW_WIN32_RESIZABLE       = VD_FW_WM_USER + 11,
    VD_FW_WIN32_BLOCKMODE       = VD_FW_WM_USER + 12,

    VD_FW_WIN32_WINDOW_STATE_MINIMIZED = 1 << 0,
    VD_FW_WIN32_WINDOW_STATE_MAXIMIZED = 1 << 1,
};

typedef struct {
    int w, h;
    int flags;
} VdFw__Win32Frame;

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
    VdFwDWORD width;
    VdFwDWORD height;
    VdFwDWORD frequency;
    VdFwDWORD aspect_numerator;
    VdFwDWORD aspect_denominator;
} VdFw__Win32DisplayMode;

typedef struct {
    VdFwHMONITOR            hmonitor;
    char                    friendly_name[32];
    VdFwDisplayMode         *display_modes;
    int                     display_modes_len;
    int                     display_modes_cap;
} VdFw__Win32Monitor;

typedef int (*VdFw__vkCreateWin32SurfaceKHRProc)(void *instance, void *pCreateInfo, void *pAllocator, void *pSurface);

typedef struct {
/* ----WINDOW THREAD ONLY-------------------------------------------------------------------------------------------- */
    VdFwGraphicsApi             graphics_api;           // Currently selected graphics api
    VdFwHWND                    hwnd;                   // Window handle
    int                         w, h;                   // Current window dimensions
    VdFwBOOL                    t_paint_ready;          // One time signal that window thread is paint-ready
                                                        // (to respond properly to events sent before we enter the
                                                        // message loop)

    VdFwBOOL                    draw_decorations;       // Draw window frame, or be frame-less
    VdFwBOOL                    winthread_block_while_sizing; // Block while sizing
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

    VdFw__Win32Monitor          *monitor_buffer;
    int                         monitor_buffer_len;
    int                         monitor_buffer_cap;
    int                         winthread_resizable;

    VdFwU32                     monitor_count;


/* ----RENDER THREAD ONLY-------------------------------------------------------------------------------------------- */
    // Internal
    HMODULE                     opengl32;               // Handle to OpenGL32.dll, used when wglGetProcAddress fails.
    VdFwHANDLE                  win_thread;             // Handle to the window-thread
    VdFwDWORD                   win_thread_id;          // Window-thread ID
    VdFwHDC                     hdc;                    // Device Context
    VdFwHGLRC                   hglrc;
    LARGE_INTEGER               frequency;
    LARGE_INTEGER               performance_counter;
    VdFwProcwglSwapIntervalExt  proc_swapInterval;      // Used for vd_fw_set_vsync
    unsigned long long          last_ns;                // Cached delta time
    // Mouse
    int                         resizable;
    int                         block_while_sizing;
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
    VdFwU8                      *temp_buf;
    int                         temp_buf_cap;
    int                         last_mouse_before_lock[2];

    int                         num_evts;
    VdFwEvent                   evtbuf[VD_FW_EVENT_COUNT_MAX];

    VdFw__vkCreateWin32SurfaceKHRProc vk_create_win32_surface_khr_proc;
    VdFwVkGetInstanceProcAddrProc vk_get_instance_proc_addr;
    float                       scale;
    int                         scale_changed;

/* ----RENDER THREAD - WINDOW THREAD DATA---------------------------------------------------------------------------- */
    VdFwEvent                   msgbuf[VD_FW_WIN32_MESSAGE_BUFFER_SIZE];
    volatile VdFwLONG           msgbuf_r;
    volatile VdFwLONG           msgbuf_w;
    int                         ncrect_count;
    int                         ncrects[VD_FW_NCRECTS_MAX][4];
    int                         nccaption[4];
    int                         nccaption_set;
    int                         receive_ncmouse_on;
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

int vd_fw__win32_translate_button(WORD vkcode)
{
    int result = 0;
    switch (vkcode) {
        case VK_LBUTTON:  result = VD_FW_MOUSE_BUTTON_LEFT; break;
        case VK_RBUTTON:  result = VD_FW_MOUSE_BUTTON_RIGHT; break;
        case VK_MBUTTON:  result = VD_FW_MOUSE_BUTTON_MIDDLE; break;
        case VK_XBUTTON1: result = VD_FW_MOUSE_BUTTON_M1; break;
        case VK_XBUTTON2: result = VD_FW_MOUSE_BUTTON_M2; break;
        default: break;
    }
    return result;
}

VdFwKey vd_fw___vkcode_to_key(WORD vkcode)
{
    static VdFwKey translation_table[256] = {
        VD_FW_KEY_UNKNOWN,       //                                     0x00 0x00    Invalid Key
        VD_FW_KEY_UNKNOWN,       // VK_LBUTTON                          0x01 0x01    Left mouse button
        VD_FW_KEY_UNKNOWN,       // VK_RBUTTON                          0x02 0x02    Right mouse button
        VD_FW_KEY_UNKNOWN,       // VK_CANCEL                           0x03 0x03    Control-break processing
        VD_FW_KEY_UNKNOWN,       // VK_MBUTTON                          0x04 0x04    Middle mouse button
        VD_FW_KEY_UNKNOWN,       // VK_XBUTTON1                         0x05 0x05    X1 mouse button
        VD_FW_KEY_UNKNOWN,       // VK_XBUTTON2                         0x06 0x06    X2 mouse button
        VD_FW_KEY_UNKNOWN,       //                                     0x07 0x07    Reserved
        VD_FW_KEY_BACKSPACE,     // VK_BACK                             0x08 0x08    Backspace key
        VD_FW_KEY_TAB,           // VK_TAB                              0x09 0x09    Tab key
        VD_FW_KEY_UNKNOWN,       //                                     0x0A 0x0A    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x0B 0x0B    Reserved
        VD_FW_KEY_UNKNOWN,       // VK_CLEAR                            0x0C 0x0C    Clear key
        VD_FW_KEY_ENTER,         // VK_RETURN                           0x0D 0x0D    Enter key
        VD_FW_KEY_UNKNOWN,       //                                     0x0E 0x0E    Unassigned
        VD_FW_KEY_UNKNOWN,       //                                     0x0F 0x0F    Unassigned
        VD_FW_KEY_UNKNOWN,       // VK_SHIFT                            0x10 0x10    Shift key
        VD_FW_KEY_UNKNOWN,       // VK_CONTROL                          0x11 0x11    Ctrl key
        VD_FW_KEY_UNKNOWN,       // VK_MENU                             0x12 0x12    Alt key
        VD_FW_KEY_UNKNOWN,       // VK_PAUSE                            0x13 0x13    Pause key
        VD_FW_KEY_CAPITAL,       // VK_CAPITAL                          0x14 0x14    Caps lock key
        VD_FW_KEY_UNKNOWN,       // VK_KANA                             0x15 0x15    IME Kana mode / IME Hangul mode
        VD_FW_KEY_UNKNOWN,       // VK_IME_ON                           0x16 0x16    IME On
        VD_FW_KEY_UNKNOWN,       // VK_JUNJA                            0x17 0x17    IME Junja mode
        VD_FW_KEY_UNKNOWN,       // VK_FINAL                            0x18 0x18    IME final mode
        VD_FW_KEY_UNKNOWN,       // VK_HANJA                            0x19 0x19    IME Hanja mode / IME Kanji mode
        VD_FW_KEY_UNKNOWN,       // VK_IME_OFF                          0x1A 0x1A    IME Off
        VD_FW_KEY_ESCAPE,        // VK_ESCAPE                           0x1B 0x1B    Esc key
        VD_FW_KEY_UNKNOWN,       // VK_CONVERT                          0x1C 0x1C    IME convert
        VD_FW_KEY_UNKNOWN,       // VK_NONCONVERT                       0x1D 0x1D    IME nonconvert
        VD_FW_KEY_UNKNOWN,       // VK_ACCEPT                           0x1E 0x1E    IME accept
        VD_FW_KEY_UNKNOWN,       // VK_MODECHANGE                       0x1F 0x1F    IME mode change request
        VD_FW_KEY_SPACE,         // VK_SPACE                            0x20 0x20    Spacebar key
        VD_FW_KEY_PGUP,          // VK_PRIOR                            0x21 0x21    Page up key
        VD_FW_KEY_PGDN,          // VK_NEXT                             0x22 0x22    Page down key
        VD_FW_KEY_END,           // VK_END                              0x23 0x23    End key
        VD_FW_KEY_HOME,          // VK_HOME                             0x24 0x24    Home key
        VD_FW_KEY_ARROW_LEFT,    // VK_LEFT                             0x25 0x25    Left arrow key
        VD_FW_KEY_ARROW_UP,      // VK_UP                               0x26 0x26    Up arrow key
        VD_FW_KEY_ARROW_RIGHT,   // VK_RIGHT                            0x27 0x27    Right arrow key
        VD_FW_KEY_ARROW_DOWN,    // VK_DOWN                             0x28 0x28    Down arrow key
        VD_FW_KEY_UNKNOWN,       // VK_SELECT                           0x29 0x29    Select key
        VD_FW_KEY_UNKNOWN,       // VK_PRINT                            0x2A 0x2A    Print key
        VD_FW_KEY_UNKNOWN,       // VK_EXECUTE                          0x2B 0x2B    Execute key
        VD_FW_KEY_UNKNOWN,       // VK_SNAPSHOT                         0x2C 0x2C    Print screen key
        VD_FW_KEY_INS,           // VK_INSERT                           0x2D 0x2D    Insert key
        VD_FW_KEY_DEL,           // VK_DELETE                           0x2E 0x2E    Delete key
        VD_FW_KEY_UNKNOWN,       // VK_HELP                             0x2F 0x2F    Help key
        VD_FW_KEY_0,             // `0`                                 0x30 0x30    0 key
        VD_FW_KEY_1,             // `1`                                 0x31 0x31    1 key
        VD_FW_KEY_2,             // `2`                                 0x32 0x32    2 key
        VD_FW_KEY_3,             // `3`                                 0x33 0x33    3 key
        VD_FW_KEY_4,             // `4`                                 0x34 0x34    4 key
        VD_FW_KEY_5,             // `5`                                 0x35 0x35    5 key
        VD_FW_KEY_6,             // `6`                                 0x36 0x36    6 key
        VD_FW_KEY_7,             // `7`                                 0x37 0x37    7 key
        VD_FW_KEY_8,             // `8`                                 0x38 0x38    8 key
        VD_FW_KEY_9,             // `9`                                 0x39 0x39    9 key
        VD_FW_KEY_UNKNOWN,       //                                     0x3A 0x3A    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x3B 0x3B    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x3C 0x3C    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x3D 0x3D    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x3E 0x3E    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x3F 0x3F    Undefined
        VD_FW_KEY_UNKNOWN,       //                                     0x40 0x40    Undefined
        VD_FW_KEY_A,             // 'A'                                 0x41 0x41    A key
        VD_FW_KEY_B,             // 'B'                                 0x42 0x42    B key
        VD_FW_KEY_C,             // 'C'                                 0x43 0x43    C key
        VD_FW_KEY_D,             // 'D'                                 0x44 0x44    D key
        VD_FW_KEY_E,             // 'E'                                 0x45 0x45    E key
        VD_FW_KEY_F,             // 'F'                                 0x46 0x46    F key
        VD_FW_KEY_G,             // 'G'                                 0x47 0x47    G key
        VD_FW_KEY_H,             // 'H'                                 0x48 0x48    H key
        VD_FW_KEY_I,             // 'I'                                 0x49 0x49    I key
        VD_FW_KEY_J,             // 'J'                                 0x4A 0x4A    J key
        VD_FW_KEY_K,             // 'K'                                 0x4B 0x4B    K key
        VD_FW_KEY_L,             // 'L'                                 0x4C 0x4C    L key
        VD_FW_KEY_M,             // 'M'                                 0x4D 0x4D    M key
        VD_FW_KEY_N,             // 'N'                                 0x4E 0x4E    N key
        VD_FW_KEY_O,             // 'O'                                 0x4F 0x4F    O key
        VD_FW_KEY_P,             // 'P'                                 0x50 0x50    P key
        VD_FW_KEY_Q,             // 'Q'                                 0x51 0x51    Q key
        VD_FW_KEY_R,             // 'R'                                 0x52 0x52    R key
        VD_FW_KEY_S,             // 'S'                                 0x53 0x53    S key
        VD_FW_KEY_T,             // 'T'                                 0x54 0x54    T key
        VD_FW_KEY_U,             // 'U'                                 0x55 0x55    U key
        VD_FW_KEY_V,             // 'V'                                 0x56 0x56    V key
        VD_FW_KEY_W,             // 'W'                                 0x57 0x57    W key
        VD_FW_KEY_X,             // 'X'                                 0x58 0x58    X key
        VD_FW_KEY_Y,             // 'Y'                                 0x59 0x59    Y key
        VD_FW_KEY_Z,             // 'Z'                                 0x5A 0x5A    Z key
        VD_FW_KEY_UNKNOWN,       // VK_LWIN                             0x5B 0x5B    Left Windows logo key
        VD_FW_KEY_UNKNOWN,       // VK_RWIN                             0x5C 0x5C    Right Windows logo key
        VD_FW_KEY_UNKNOWN,       // VK_APPS                             0x5D 0x5D    Application key
        VD_FW_KEY_UNKNOWN,       //                                     0x5E 0x5E    Reserved
        VD_FW_KEY_UNKNOWN,       // VK_SLEEP                            0x5F 0x5F    Computer Sleep key
        VD_FW_KEY_NUMPAD_0,      // VK_NUMPAD0                          0x60 0x60    Numeric keypad 0 key
        VD_FW_KEY_NUMPAD_1,      // VK_NUMPAD1                          0x61 0x61    Numeric keypad 1 key
        VD_FW_KEY_NUMPAD_2,      // VK_NUMPAD2                          0x62 0x62    Numeric keypad 2 key
        VD_FW_KEY_NUMPAD_3,      // VK_NUMPAD3                          0x63 0x63    Numeric keypad 3 key
        VD_FW_KEY_NUMPAD_4,      // VK_NUMPAD4                          0x64 0x64    Numeric keypad 4 key
        VD_FW_KEY_NUMPAD_5,      // VK_NUMPAD5                          0x65 0x65    Numeric keypad 5 key
        VD_FW_KEY_NUMPAD_6,      // VK_NUMPAD6                          0x66 0x66    Numeric keypad 6 key
        VD_FW_KEY_NUMPAD_7,      // VK_NUMPAD7                          0x67 0x67    Numeric keypad 7 key
        VD_FW_KEY_NUMPAD_8,      // VK_NUMPAD8                          0x68 0x68    Numeric keypad 8 key
        VD_FW_KEY_NUMPAD_9,      // VK_NUMPAD9                          0x69 0x69    Numeric keypad 9 key
        VD_FW_KEY_UNKNOWN,       // VK_MULTIPLY                         0x6A 0x6A    Multiply key
        VD_FW_KEY_UNKNOWN,       // VK_ADD                              0x6B 0x6B    Add key
        VD_FW_KEY_UNKNOWN,       // VK_SEPARATOR                        0x6C 0x6C    Separator key
        VD_FW_KEY_UNKNOWN,       // VK_SUBTRACT                         0x6D 0x6D    Subtract key
        VD_FW_KEY_UNKNOWN,       // VK_DECIMAL                          0x6E 0x6E    Decimal key
        VD_FW_KEY_UNKNOWN,       // VK_DIVIDE                           0x6F 0x6F    Divide key
        VD_FW_KEY_F1,            // VK_F1                               0x70 0x70    F1 key
        VD_FW_KEY_F2,            // VK_F2                               0x71 0x71    F2 key
        VD_FW_KEY_F3,            // VK_F3                               0x72 0x72    F3 key
        VD_FW_KEY_F4,            // VK_F4                               0x73 0x73    F4 key
        VD_FW_KEY_F5,            // VK_F5                               0x74 0x74    F5 key
        VD_FW_KEY_F6,            // VK_F6                               0x75 0x75    F6 key
        VD_FW_KEY_F7,            // VK_F7                               0x76 0x76    F7 key
        VD_FW_KEY_F8,            // VK_F8                               0x77 0x77    F8 key
        VD_FW_KEY_F9,            // VK_F9                               0x78 0x78    F9 key
        VD_FW_KEY_F10,           // VK_F10                              0x79 0x79    F10 key
        VD_FW_KEY_F11,           // VK_F11                              0x7A 0x7A    F11 key
        VD_FW_KEY_F12,           // VK_F12                              0x7B 0x7B    F12 key
        VD_FW_KEY_F13,           // VK_F13                              0x7C 0x7C    F13 key
        VD_FW_KEY_F14,           // VK_F14                              0x7D 0x7D    F14 key
        VD_FW_KEY_F15,           // VK_F15                              0x7E 0x7E    F15 key
        VD_FW_KEY_F16,           // VK_F16                              0x7F 0x7F    F16 key
        VD_FW_KEY_F17,           // VK_F17                              0x80 0x80    F17 key
        VD_FW_KEY_F18,           // VK_F18                              0x81 0x81    F18 key
        VD_FW_KEY_F19,           // VK_F19                              0x82 0x82    F19 key
        VD_FW_KEY_F20,           // VK_F20                              0x83 0x83    F20 key
        VD_FW_KEY_F21,           // VK_F21                              0x84 0x84    F21 key
        VD_FW_KEY_F22,           // VK_F22                              0x85 0x85    F22 key
        VD_FW_KEY_F23,           // VK_F23                              0x86 0x86    F23 key
        VD_FW_KEY_F24,           // VK_F24                              0x87 0x87    F24 key
        VD_FW_KEY_UNKNOWN,       //                                     0x88 0x88    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x89 0x89    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8A 0x8A    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8B 0x8B    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8C 0x8C    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8D 0x8D    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8E 0x8E    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0x8F 0x8F    Reserved
        VD_FW_KEY_UNKNOWN,       // VK_NUMLOCK                          0x90 0x90    Num lock key
        VD_FW_KEY_UNKNOWN,       // VK_SCROLL                           0x91 0x91    Scroll lock key
        VD_FW_KEY_UNKNOWN,       //                                     0x92 0x92    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0x93 0x93    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0x94 0x94    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0x95 0x95    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0x96 0x96    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0x97 0x97    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x98 0x98    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x99 0x99    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9A 0x9A    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9B 0x9B    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9C 0x9C    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9D 0x9D    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9E 0x9E    Unassigned 
        VD_FW_KEY_UNKNOWN,       //                                     0x9F 0x9F    Unassigned 
        VD_FW_KEY_LSHIFT,        // VK_LSHIFT                           0xA0 0xA0    Left Shift key
        VD_FW_KEY_RSHIFT,        // VK_RSHIFT                           0xA1 0xA1    Right Shift key
        VD_FW_KEY_LCONTROL,      // VK_LCONTROL                         0xA2 0xA2    Left Ctrl key
        VD_FW_KEY_RCONTROL,      // VK_RCONTROL                         0xA3 0xA3    Right Ctrl key
        VD_FW_KEY_LALT,          // VK_LMENU                            0xA4 0xA4    Left Alt key
        VD_FW_KEY_RALT,          // VK_RMENU                            0xA5 0xA5    Right Alt key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_BACK                     0xA6 0xA6    Browser Back key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_FORWARD                  0xA7 0xA7    Browser Forward key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_REFRESH                  0xA8 0xA8    Browser Refresh key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_STOP                     0xA9 0xA9    Browser Stop key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_SEARCH                   0xAA 0xAA    Browser Search key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_FAVORITES                0xAB 0xAB    Browser Favorites key
        VD_FW_KEY_UNKNOWN,       // VK_BROWSER_HOME                     0xAC 0xAC    Browser Start and Home key
        VD_FW_KEY_UNKNOWN,       // VK_VOLUME_MUTE                      0xAD 0xAD    Volume Mute key
        VD_FW_KEY_UNKNOWN,       // VK_VOLUME_DOWN                      0xAE 0xAE    Volume Down key
        VD_FW_KEY_UNKNOWN,       // VK_VOLUME_UP                        0xAF 0xAF    Volume Up key
        VD_FW_KEY_MEDIA_NEXT,    // VK_MEDIA_NEXT_TRACK                 0xB0 0xB0    Next Track key
        VD_FW_KEY_MEDIA_PREV,    // VK_MEDIA_PREV_TRACK                 0xB1 0xB1    Previous Track key
        VD_FW_KEY_UNKNOWN,       // VK_MEDIA_STOP                       0xB2 0xB2    Stop Media key
        VD_FW_KEY_MEDIA_PLAY,    // VK_MEDIA_PLAY_PAUSE                 0xB3 0xB3    Play/Pause Media key
        VD_FW_KEY_UNKNOWN,       // VK_LAUNCH_MAIL                      0xB4 0xB4    Start Mail key
        VD_FW_KEY_UNKNOWN,       // VK_LAUNCH_MEDIA_SELECT              0xB5 0xB5    Select Media key
        VD_FW_KEY_UNKNOWN,       // VK_LAUNCH_APP1                      0xB6 0xB6    Start Application 1 key
        VD_FW_KEY_UNKNOWN,       // VK_LAUNCH_APP2                      0xB7 0xB7    Start Application 2 key
        VD_FW_KEY_UNKNOWN,       //                                     0xB8 0xB8    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0xB9 0xB9    Reserved
        VD_FW_KEY_SEMICOLON,     // VK_OEM_1                            0xBA 0xBA    It can vary by keyboard. For the US ANSI keyboard , the Semiсolon and Colon key
        VD_FW_KEY_EQUALS,        // VK_OEM_PLUS                         0xBB 0xBB    For any country/region, the Equals and Plus key
        VD_FW_KEY_COMMA,         // VK_OEM_COMMA                        0xBC 0xBC    For any country/region, the Comma and Less Than key
        VD_FW_KEY_MINUS,         // VK_OEM_MINUS                        0xBD 0xBD    For any country/region, the Dash and Underscore key
        VD_FW_KEY_DOT,           // VK_OEM_PERIOD                       0xBE 0xBE    For any country/region, the Period and Greater Than key
        VD_FW_KEY_SLASH_FORWARD, // VK_OEM_2                            0xBF 0xBF    It can vary by keyboard. For the US ANSI keyboard, the Forward Slash and Question Mark key
        VD_FW_KEY_BACKTICK,      // VK_OEM_3                            0xC0 0xC0    It can vary by keyboard. For the US ANSI keyboard, the Grave Accent and Tilde key
        VD_FW_KEY_UNKNOWN,       //                                     0xC1 0xC1    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0xC2 0xC2    Reserved
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_A                        0xC3 0xC3    Gamepad A button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_B                        0xC4 0xC4    Gamepad B button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_X                        0xC5 0xC5    Gamepad X button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_Y                        0xC6 0xC6    Gamepad Y button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_SHOULDER           0xC7 0xC7    Gamepad Right Shoulder button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_SHOULDER            0xC8 0xC8    Gamepad Left Shoulder button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_TRIGGER             0xC9 0xC9    Gamepad Left Trigger button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_TRIGGER            0xCA 0xCA    Gamepad Right Trigger button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_DPAD_UP                  0xCB 0xCB    Gamepad D-pad Up button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_DPAD_DOWN                0xCC 0xCC    Gamepad D-pad Down button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_DPAD_LEFT                0xCD 0xCD    Gamepad D-pad Left button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_DPAD_RIGHT               0xCE 0xCE    Gamepad D-pad Right button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_MENU                     0xCF 0xCF    Gamepad Menu/Start button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_VIEW                     0xD0 0xD0    Gamepad View/Back button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_THUMBSTICK_BUTTON   0xD1 0xD1    Gamepad Left Thumbstick button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_THUMBSTICK_BUTTON  0xD2 0xD2    Gamepad Right Thumbstick button
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_THUMBSTICK_UP       0xD3 0xD3    Gamepad Left Thumbstick up
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_THUMBSTICK_DOWN     0xD4 0xD4    Gamepad Left Thumbstick down
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_THUMBSTICK_RIGHT    0xD5 0xD5    Gamepad Left Thumbstick right
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_LEFT_THUMBSTICK_LEFT     0xD6 0xD6    Gamepad Left Thumbstick left
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_THUMBSTICK_UP      0xD7 0xD7    Gamepad Right Thumbstick up
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_THUMBSTICK_DOWN    0xD8 0xD8    Gamepad Right Thumbstick down
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_THUMBSTICK_RIGHT   0xD9 0xD9    Gamepad Right Thumbstick right
        VD_FW_KEY_UNKNOWN,       // VK_GAMEPAD_RIGHT_THUMBSTICK_LEFT    0xDA 0xDA    Gamepad Right Thumbstick left
        VD_FW_KEY_BRACKET_OPEN,  // VK_OEM_4                            0xDB 0xDB    It can vary by keyboard. For the US ANSI keyboard, the Left Brace key
        VD_FW_KEY_SLASH_BACK,    // VK_OEM_5                            0xDC 0xDC    It can vary by keyboard. For the US ANSI keyboard, the Backslash and Pipe key
        VD_FW_KEY_BRACKET_CLOSE, // VK_OEM_6                            0xDD 0xDD    It can vary by keyboard. For the US ANSI keyboard, the Right Brace key
        VD_FW_KEY_QUOTE,         // VK_OEM_7                            0xDE 0xDE    It can vary by keyboard. For the US ANSI keyboard, the Apostrophe and Double Quotation Mark key
        VD_FW_KEY_UNKNOWN,       // VK_OEM_8                            0xDF 0xDF    It can vary by keyboard. For the Canadian CSA keyboard, the Right Ctrl key
        VD_FW_KEY_UNKNOWN,       //                                     0xE0 0xE0    Reserved
        VD_FW_KEY_UNKNOWN,       //                                     0xE1 0xE1    OEM specific
        VD_FW_KEY_UNKNOWN,       // VK_OEM_102                          0xE2 0xE2    It can vary by keyboard. For the European ISO keyboard, the Backslash and Pipe key
        VD_FW_KEY_UNKNOWN,       //                                     0xE3 0xE3    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xE4 0xE4    OEM specific
        VD_FW_KEY_UNKNOWN,       // VK_PROCESSKEY                       0xE5 0xE5    IME PROCESS key
        VD_FW_KEY_UNKNOWN,       //                                     0xE6 0xE6    OEM specific
        VD_FW_KEY_UNKNOWN,       // VK_PACKET                           0xE7 0xE7    Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
        VD_FW_KEY_UNKNOWN,       //                                     0xE8 0xE8    Unassigned
        VD_FW_KEY_UNKNOWN,       //                                     0xE9 0xE9    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xEA 0xEA    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xEB 0xEB    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xEC 0xEC    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xED 0xED    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xEE 0xEE    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xEF 0xEF    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF0 0xF0    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF1 0xF1    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF2 0xF2    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF3 0xF3    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF4 0xF4    OEM specific
        VD_FW_KEY_UNKNOWN,       //                                     0xF5 0xF5    OEM specific
        VD_FW_KEY_UNKNOWN,       // VK_ATTN                             0xF6 0xF6    Attn key
        VD_FW_KEY_UNKNOWN,       // VK_CRSEL                            0xF7 0xF7    CrSel key
        VD_FW_KEY_UNKNOWN,       // VK_EXSEL                            0xF8 0xF8    ExSel key
        VD_FW_KEY_UNKNOWN,       // VK_EREOF                            0xF9 0xF9    Erase EOF key
        VD_FW_KEY_UNKNOWN,       // VK_PLAY                             0xFA 0xFA    Play key
        VD_FW_KEY_UNKNOWN,       // VK_ZOOM                             0xFB 0xFB    Zoom key
        VD_FW_KEY_UNKNOWN,       // VK_NONAME                           0xFC 0xFC    Reserved
        VD_FW_KEY_UNKNOWN,       // VK_PA1                              0xFD 0xFD    PA1 key
        VD_FW_KEY_UNKNOWN,       // VK_OEM_CLEAR                        0xFE 0xFE    Clear key
    };

    return translation_table[vkcode];
}

static VdFw__Win32InternalData Vd_Fw_Globals = {0};
static VdFwLRESULT  vd_fw__wndproc(VdFwHWND hwnd, VdFwUINT msg, VdFwWPARAM wparam, VdFwLPARAM lparam);
static void         vd_fw__composition_changed(void);
static void         vd_fw__update_region(void);
static void         vd_fw__theme_changed(void);
static void         vd_fw__update_monitors(void);
static VdFwLRESULT  vd_fw__nccalcsize(VdFwWPARAM wparam, VdFwLPARAM lparam);
static VdFwBOOL     vd_fw__has_autohide_taskbar(VdFwUINT edge, VdFwRECT monitor);
static void         vd_fw__window_pos_changed(VdFwWINDOWPOS *pos);
static VdFwLRESULT  vd_fw__handle_invisible(VdFwHWND hwnd, VdFwUINT msg, VdFwWPARAM wparam, VdFwLPARAM lparam);
static VdFwDWORD    vd_fw__win_thread_proc(LPVOID param);
static void         vd_fw__gl_debug_message_callback(GLenum source, GLenum type, GLuint id,
                                                     GLenum severity, GLsizei length, const GLchar *message,
                                                     const void *userParam);
static int          vd_fw__msgbuf_r(VdFwEvent *message);
static int          vd_fw__msgbuf_w(VdFwEvent *message);
static void         vd_fw__update_kb_codepage(void);
static VdFwBOOL     vd_fw__win32_enum_monitor(VdFwHMONITOR monitor, VdFwHDC hdc, VdFwLPRECT rect, VdFwLPARAM lpparam);
static VdFwBOOL     vd_fw__win32_enum_monitor_resize_count(VdFwHMONITOR monitor, VdFwHDC hdc, VdFwLPRECT rect, VdFwLPARAM lpparam);
static void         vd_fw__win32_update_monitor_display_modes(VdFw__Win32Monitor *monitor);
static void         vd_fw__win32_update_monitors(void);

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
    VD_FW_G.resizable = 1;
    VD_FW_G.winthread_resizable = 1;
    VD_FW_G.block_while_sizing = 0;
    VD_FW_G.winthread_block_while_sizing = 0;

    VD_FW_G.next_width = 640;
    VD_FW_G.next_height = 480;

    VD_FW_G.next_pos_x  = CW_USEDEFAULT;
    VD_FW_G.next_pos_y  = CW_USEDEFAULT;

    // Load Win32 Libraries
    {
#define V(dllpath) { HMODULE m = LoadLibraryA(dllpath);
#define X(retval, name, params) VdFw##name = (VdFwProc##name)GetProcAddress(m, #name);
#define VE()       }
        VD_FW__WIN32_FUNCTIONS
#undef V
#undef X
#undef VE

        if (VdFwRtlGetVersion) {
            VdFwRtlGetVersion((VdFwPRTL_OSVERSIONINFOW)&VD_FW_G.os_version);
        }
        // OpenGL32.dll
        {
            VD_FW_G.opengl32 = LoadLibraryA("OpenGL32.dll");
            VdFwwglCreateContext  =  (VdFwProcwglCreateContext)GetProcAddress(VD_FW_G.opengl32, "wglCreateContext");
            VdFwwglMakeCurrent    =    (VdFwProcwglMakeCurrent)GetProcAddress(VD_FW_G.opengl32, "wglMakeCurrent");
            VdFwwglDeleteContext  =  (VdFwProcwglDeleteContext)GetProcAddress(VD_FW_G.opengl32,"wglDeleteContext");
            VdFwwglGetProcAddress = (VdFwProcwglGetProcAddress)GetProcAddress(VD_FW_G.opengl32, "wglGetProcAddress");
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
    }

    VD_FW_G.curr_frame.w = VD_FW_G.next_width;
    VD_FW_G.curr_frame.h = VD_FW_G.next_height;
    VD_FW_G.curr_frame.flags = 0;

    // @todo(mdodis): Use different versions of SetProcessDpiAware if not supported
    // SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    VdFwSetProcessDpiAwarenessContext(VD_FW_DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    if (!info || !info->window_options.borderless) {
        VdFwRECT r;
        r.left = 0;
        r.right = VD_FW_G.next_width;
        r.top = 0;
        r.bottom = VD_FW_G.next_height;

        // @note(mdodis): Documentation says that this function in not DPI aware for a process
        // which is per-monitor aware. I've not seen this to be the case yet
        VD_FW__CHECK_TRUE(VdFwAdjustWindowRect(&r, WS_OVERLAPPEDWINDOW | WS_SIZEBOX, 0));

        VD_FW_G.next_width = r.right - r.left;
        VD_FW_G.next_height = r.bottom - r.top;
    }

    VdFwtimeBeginPeriod(1);
    QueryPerformanceFrequency(&VD_FW_G.frequency);

    // Monitors
    vd_fw__win32_update_monitors();

    VD_FW_G.focused = 1;
    VD_FW_G.def_window_min[0] = VdFwGetSystemMetrics(SM_CXMINTRACK);
    VD_FW_G.def_window_min[1] = VdFwGetSystemMetrics(SM_CYMINTRACK);

    VD_FW_G.draw_decorations = 1;
    if (info != NULL) {
        VD_FW_G.draw_decorations = !info->window_options.borderless;
        VD_FW_G.winthread_block_while_sizing = info->window_options.block_while_sizing;
        VD_FW_G.block_while_sizing = info->window_options.block_while_sizing;
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

    return 1;
}

VD_FW_API VdFwEvent *vd_fw_poll(int *count)
{
    VD_FW_G.scale_changed = 0;
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
    VD_FW_G.mouse_delta[0] = VD_FW_G.mouse_delta[1] = 0.f;

    for (int i = 0; i < VD_FW_KEY_MAX; ++i) {
        VD_FW_G.prev_key_states[i] = VD_FW_G.curr_key_states[i];
    }

    VD_FW_G.num_evts = 0;

    VdFwEvent mm;
    while (vd_fw__msgbuf_r(&mm) && (VD_FW_G.num_evts < VD_FW_EVENT_COUNT_MAX)) {
        VD_FW_G.evtbuf[VD_FW_G.num_evts++] = mm;

        switch (mm.type) {
            case VD_FW_EVENT_TYPE_CHARACTER: {
                VD_FW_G.codepoints[(num_codepoints++) % VD_FW_CODEPOINT_BUFFER_COUNT] = mm.data.character.codepoint;
            } break;

            case VD_FW_EVENT_TYPE_CLOSE_REQUEST: {
                VD_FW_G.close_request = 1;
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_MOVE: {
                VD_FW_G.mouse[0] = mm.data.mouse_move.x;
                VD_FW_G.mouse[1] = mm.data.mouse_move.y;
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_DELTA: {
                VD_FW_G.mouse_delta[0] = VD_FW_G.mouse_delta[0] * 0.8f + mm.data.mouse_delta.dx * 0.2f;
                VD_FW_G.mouse_delta[1] = VD_FW_G.mouse_delta[1] * 0.8f + mm.data.mouse_delta.dy * 0.2f;
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_SCROLL: {
                VD_FW_G.wheel[0] += mm.data.mouse_scroll.dx;
                VD_FW_G.wheel[1] += mm.data.mouse_scroll.dy;
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_BUTTON_DOWN: {
                VD_FW_G.mouse_state |= mm.data.mouse_button_down.button;
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP: {
                VD_FW_G.mouse_state &= ~mm.data.mouse_button_up.button;
            } break;

            case VD_FW_EVENT_TYPE_FOCUS_CHANGE: {
                VD_FW_G.focus_changed = 1;
                VD_FW_G.focused = mm.data.focus_change.got_focus;
            } break;

            case VD_FW_EVENT_TYPE_KEY_UP: {
                VD_FW_G.curr_key_states[mm.data.key_up.key] = 0;
            } break;

            case VD_FW_EVENT_TYPE_KEY_DOWN: {
                VD_FW_G.curr_key_states[mm.data.key_down.key] = 1;
            } break;

            case VD_FW_EVENT_TYPE_WINDOW_STATE_CHANGE: {
                int prev_state = VD_FW_G.window_state;
                int change_flag = mm.data.window_state_change.flag;
                if (mm.data.window_state_change.value) {
                    VD_FW_G.window_state |= change_flag;
                } else {
                    VD_FW_G.window_state &= ~change_flag;
                }

                if (prev_state != VD_FW_G.window_state) {
                    VD_FW_G.window_state_changed |= change_flag;
                }

            } break;

            case VD_FW_EVENT_TYPE_SCALE_CHANGE: {
                VD_FW_G.scale = mm.data.scale_change.new_scale;
                VD_FW_G.scale_changed = 1;
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

    if (count) {
        *count = VD_FW_G.num_evts;
    }


    // @note(mdodis): For Raw Input mouse handling, instead of using the message queue
    // We use two sinks with an atomic write index.
    {
        // VD_FW_WIN32_PROFILE_BEGIN(read_all_input);
        EnterCriticalSection(&VD_FW_G.input_critical_section);
        VD_FW_G.num_gamepads_present = VD_FW_G.winthread_num_gamepads_present;
        for (int i = 0; i < VD_FW_G.num_gamepads_present; ++i) {
            VD_FW_G.gamepad_prev_states[i] = VD_FW_G.gamepad_curr_states[i];
            VD_FW_G.gamepad_curr_states[i] = VD_FW_G.winthread_gamepad_curr_states[i];
        }
        LeaveCriticalSection(&VD_FW_G.input_critical_section);
        // VD_FW_WIN32_PROFILE_END(read_all_input);
    }

    if (VD_FW_G.mouse_is_locked && VD_FW_G.focused) {
        VdFwSetCursorPos(VD_FW_G.last_mouse_before_lock[0], VD_FW_G.last_mouse_before_lock[1]);
    }

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

    return VD_FW_G.evtbuf;
}

VD_FW_API void vd_fw_lock(void)
{
    EnterCriticalSection(&VD_FW_G.critical_section);
    VD_FW_G.curr_frame = VD_FW_G.next_frame;
    VD_FW_G.next_frame.flags = 0;
    LeaveCriticalSection(&VD_FW_G.critical_section);
}

VD_FW_API void vd_fw_unlock(void)
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
}

VD_FW_API int vd_fw_get_block_while_sizing(void)
{
    return VD_FW_G.block_while_sizing;    
}

VD_FW_API void vd_fw_set_block_while_sizing(int on)
{
    if (VD_FW_G.block_while_sizing == on) {
        return;
    }

    VD_FW_G.block_while_sizing = on;

    LPARAM lparam = on;

    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_BLOCKMODE,
        0, /* WPARAM */
        lparam));
}

VD_FW_API int vd_fw_close_requested(void)
{
    return VD_FW_G.close_request;
}

VD_FW_API void vd_fw_quit(void)
{
    VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_KILL,
        0, /* WPARAM */
        0  /* LPARAM */);
}

VD_FW_API void vd_fw_exit(void)
{
    vd_fw_quit();

    EnterCriticalSection(&VD_FW_G.critical_section);
    VD_FW_G.curr_frame = VD_FW_G.next_frame;
    VD_FW_G.next_frame.flags = 0;
    VD_FW_G.t_running = 0;
    VD_FW_G.t_paint_ready = 0;
    LeaveCriticalSection(&VD_FW_G.critical_section);

    if (VD_FW_G.curr_frame.flags & VD_FW_WIN32_FLAGS_WAKE_COND_VAR) {
        WakeConditionVariable(&VD_FW_G.cond_var);
    }

    WaitForSingleObject(VD_FW_G.win_thread, INFINITE);

    CloseHandle(VD_FW_G.sem_window_ready);
    CloseHandle(VD_FW_G.sem_closed);
    DeleteCriticalSection(&VD_FW_G.critical_section);
}

VD_FW_API VdFwPlatform vd_fw_get_platform(void)
{
    return VD_FW_PLATFORM_WINDOWS;
}

VD_FW_API int vd_fw_set_graphics_api(VdFwGraphicsApi api, VdFwOpenGLOptions *gl_options)
{
    VD_FW_WIN32_PROFILE_BEGIN(vd_fw_set_graphics_api);
    int result = 1;

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

            VD_FW_WIN32_PROFILE_BEGIN(create_temp_context);
            VdFwGlConfig      default_configs[2];
            VD_FW_MEMSET(default_configs, 0, sizeof(default_configs));

            default_configs[0].version = VD_FW_GL_VERSION_3_3;

            VdFwOpenGLOptions default_options = {0};
            default_options.configs = default_configs;

            if (!gl_options || (gl_options->configs == 0) || (gl_options->configs[0].version == VD_FW_GL_VERSION_BASIC)) {
                gl_options = &default_options;
            }

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
            VD_FW_WIN32_PROFILE_BEGIN(temp_pixel_format);
            int pf = VdFwChoosePixelFormat(VD_FW_G.hdc, &pfd);
            VD_FW__CHECK_NONZERO(pf);
            VD_FW__CHECK_TRUE(VdFwSetPixelFormat(VD_FW_G.hdc, pf, &pfd));
            VD_FW_WIN32_PROFILE_END(temp_pixel_format);

            VD_FW_WIN32_PROFILE_BEGIN(make_temp_current);
            VdFwHGLRC temp_context = VdFwwglCreateContext(VD_FW_G.hdc);
            VD_FW__CHECK_NULL(temp_context);
            VD_FW__CHECK_TRUE(VdFwwglMakeCurrent(VD_FW_G.hdc, temp_context));
            VD_FW_WIN32_PROFILE_END(make_temp_current);

            VdFwProcwglCreateContextAttribsARB wglCreateContextAttribsARB =
                (VdFwProcwglCreateContextAttribsARB) VdFwwglGetProcAddress("wglCreateContextAttribsARB");

            VdFwProcwglChoosePixelFormatARB wglChoosePixelFormatARB =
                (VdFwProcwglChoosePixelFormatARB)VdFwwglGetProcAddress("wglChoosePixelFormatARB");
            VD_FW_WIN32_PROFILE_END(create_temp_context);

            VD_FW_WIN32_PROFILE_BEGIN(attempt_configs);
            int index = 0;
            while (gl_options->configs && gl_options->configs[index].version != 0) {
                int minor        = gl_options->configs[index].version % 10;
                int major        = gl_options->configs[index].version / 10;
                int debug        = gl_options->configs[index].debug;
                int compat       = gl_options->configs[index].compat;
                int pixel_format = gl_options->configs[index].pixel_format;
                int depth_format = gl_options->configs[index].depth_format;
                int msaa         = gl_options->configs[index].msaa;

                // Context attributes
                int attribs[9];
                attribs[0] = WGL_CONTEXT_MAJOR_VERSION_ARB; attribs[1] = major;
                attribs[2] = WGL_CONTEXT_MINOR_VERSION_ARB; attribs[3] = minor;
                attribs[4] = WGL_CONTEXT_PROFILE_MASK_ARB;  attribs[5] = (compat) ? WGL_CONTEXT_CORE_PROFILE_BIT_ARB : WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
                attribs[6] = WGL_CONTEXT_FLAGS_ARB;         attribs[7] = (debug)  ? WGL_CONTEXT_DEBUG_BIT_ARB : 0;
                attribs[8] = 0;

                int running_attrib = 0;
                int pixel_attribs[32] = {0};
                pixel_attribs[running_attrib++] = WGL_PIXEL_TYPE_ARB;
                pixel_attribs[running_attrib++] = WGL_TYPE_RGBA_ARB;

                pixel_attribs[running_attrib++] = WGL_DRAW_TO_WINDOW_ARB;
                pixel_attribs[running_attrib++] = 1;

                pixel_attribs[running_attrib++] = WGL_SUPPORT_OPENGL_ARB;
                pixel_attribs[running_attrib++] = 1;

                pixel_attribs[running_attrib++] = WGL_DOUBLE_BUFFER_ARB;
                pixel_attribs[running_attrib++] = 1;

                pixel_attribs[running_attrib++] = WGL_ACCELERATION_ARB;
                pixel_attribs[running_attrib++] = WGL_FULL_ACCELERATION_ARB;

                switch (pixel_format) {
                    case VD_FW_GL_PIXEL_FORMAT_R8G8B8A8: {
                        pixel_attribs[running_attrib++] = WGL_COLOR_BITS_ARB;
                        pixel_attribs[running_attrib++] = 24;

                        pixel_attribs[running_attrib++] = WGL_RED_BITS_ARB;
                        pixel_attribs[running_attrib++] = 8;

                        pixel_attribs[running_attrib++] = WGL_GREEN_BITS_ARB;
                        pixel_attribs[running_attrib++] = 8;

                        pixel_attribs[running_attrib++] = WGL_BLUE_BITS_ARB;
                        pixel_attribs[running_attrib++] = 8;

                        pixel_attribs[running_attrib++] = WGL_ALPHA_BITS_ARB;
                        pixel_attribs[running_attrib++] = 8;
                    } break;

                    case VD_FW_GL_PIXEL_FORMAT_R8G8B8: {
                        pixel_attribs[running_attrib++] = WGL_COLOR_BITS_ARB;
                        pixel_attribs[running_attrib++] = 24;

                        pixel_attribs[running_attrib++] = WGL_RED_BITS_ARB;
                        pixel_attribs[running_attrib++] = 8;

                        pixel_attribs[running_attrib++] = WGL_GREEN_BITS_ARB;
                        pixel_attribs[running_attrib++] = 8;

                        pixel_attribs[running_attrib++] = WGL_BLUE_BITS_ARB;
                        pixel_attribs[running_attrib++] = 8;
                    } break;

                    default: break;
                }

                switch (depth_format) {
                    case VD_FW_GL_DEPTH_FORMAT_D32: {
                        pixel_attribs[running_attrib++] = WGL_DEPTH_BITS_ARB;
                        pixel_attribs[running_attrib++] = 32;
                    } break;

                    case VD_FW_GL_DEPTH_FORMAT_D24S8: {
                        pixel_attribs[running_attrib++] = WGL_DEPTH_BITS_ARB;
                        pixel_attribs[running_attrib++] = 24;

                        pixel_attribs[running_attrib++] = WGL_STENCIL_BITS_ARB;
                        pixel_attribs[running_attrib++] = 8;
                    } break;

                    default: break;
                }

                switch (msaa) {

                    case VD_FW_GL_MSAA_ENABLED_2X: {
                        pixel_attribs[running_attrib++] = WGL_SAMPLES_ARB;
                        pixel_attribs[running_attrib++] = 2;
                    } break;

                    case VD_FW_GL_MSAA_ENABLED_4X: {
                        pixel_attribs[running_attrib++] = WGL_SAMPLES_ARB;
                        pixel_attribs[running_attrib++] = 4;
                    } break;

                    case VD_FW_GL_MSAA_ENABLED_8X: {
                        pixel_attribs[running_attrib++] = WGL_SAMPLES_ARB;
                        pixel_attribs[running_attrib++] = 8;
                    } break;

                    default: break;
                }

                int wpixel_format;
                VdFwUINT num_formats;

                if (!wglChoosePixelFormatARB(VD_FW_G.hdc, pixel_attribs, NULL, 1, &wpixel_format, &num_formats)) {
                    goto LOOP_END;                   
                }

                VdFwPIXELFORMATDESCRIPTOR pfdchosen;
                VD_FW__CHECK_NONZERO(VdFwDescribePixelFormat(VD_FW_G.hdc, wpixel_format, sizeof(pfdchosen), &pfdchosen));
                VD_FW__CHECK_TRUE(VdFwSetPixelFormat(VD_FW_G.hdc, pf, &pfdchosen));

                VD_FW_G.hglrc = wglCreateContextAttribsARB(VD_FW_G.hdc, 0, attribs);
                if (VD_FW_G.hglrc == 0) {
                    goto LOOP_END;
                }

                VD_FW__CHECK_NULL(VD_FW_G.hglrc);
                VD_FW__CHECK_TRUE(VdFwwglMakeCurrent(VD_FW_G.hdc, VD_FW_G.hglrc));

                VD_FW_G.proc_swapInterval = (VdFwProcwglSwapIntervalExt)VdFwwglGetProcAddress("wglSwapIntervalEXT");

                if (vd_fw__load_opengl(&gl_options->configs[index])) {
                    break;
                } else {
                    VD_FW__CHECK_TRUE(VdFwwglMakeCurrent(NULL, NULL));
                    VD_FW__CHECK_TRUE(VdFwwglDeleteContext(VD_FW_G.hglrc));
                }
LOOP_END:
                index++;
            }
            VD_FW_WIN32_PROFILE_END(attempt_configs);

            if (!gl_options->configs[index].version) {
                result = 0;
                break;
            }

            VD_FW__CHECK_TRUE(VdFwwglDeleteContext(temp_context));
            gl_options->selected_config = index;

        } break;

        default: break;
    }

    VD_FW_G.graphics_api = api;
    VD_FW_WIN32_PROFILE_END(vd_fw_set_graphics_api);
    return result;
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

VD_FW_API int vd_fw_get_resizable(void)
{
    return VD_FW_G.resizable;
}

VD_FW_API void vd_fw_set_resizable(int on)
{
    if (VD_FW_G.resizable == on)
    {
        return;
    }

    LPARAM lparam = on;

    VD_FW_G.resizable = on;
    VD_FW__CHECK_TRUE(VdFwPostMessage(
        VD_FW_G.hwnd,
        VD_FW_WIN32_RESIZABLE,
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

VD_FW_API void vd_fw_set_minimized(void)
{
    VdFwShowWindow(VD_FW_G.hwnd, SW_MINIMIZE);
}

VD_FW_API int vd_fw_get_maximized(int *maximized)
{
    *maximized = VD_FW_G.window_state & VD_FW_WIN32_WINDOW_STATE_MAXIMIZED;
    return VD_FW_G.window_state_changed & VD_FW_WIN32_WINDOW_STATE_MAXIMIZED;
}

VD_FW_API void vd_fw_set_maximized(void)
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

VD_FW_API int vd_fw_get_monitor_count(void)
{
    return VD_FW_G.monitor_count;
}

VD_FW_API const char *vd_fw_get_monitor_name(int index)
{
    return VD_FW_G.monitor_buffer[index].friendly_name;
}

VD_FW_API VdFwDisplayMode *vd_fw_get_monitor_display_modes(int index, int *count)
{
    vd_fw__win32_update_monitor_display_modes(&VD_FW_G.monitor_buffer[index]);
    *count = VD_FW_G.monitor_buffer[index].display_modes_len;

    return VD_FW_G.monitor_buffer[index].display_modes;
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

VD_FW_API int vd_fw_get_key_released(int key)
{
    return VD_FW_G.prev_key_states[key] && !VD_FW_G.curr_key_states[key];
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
        VdFwPOINT point;
        VdFwGetCursorPos(&point);
        VD_FW_G.last_mouse_before_lock[0] = point.x;
        VD_FW_G.last_mouse_before_lock[1] = point.y;
        VD_FW__CHECK_TRUE(VdFwPostMessage(
            VD_FW_G.hwnd,
            VD_FW_WIN32_SHOW_CURSOR,
            0, /* WPARAM */
            0  /* LPARAM */));
    } else {
        VdFwSetCursorPos(VD_FW_G.last_mouse_before_lock[0], VD_FW_G.last_mouse_before_lock[1]);

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

VD_FW_API int vd_fw_get_scale(float *scale)
{
    if (scale) *scale = VD_FW_G.scale;
    return VD_FW_G.scale_changed;
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
    VdFwSetForegroundWindow(VD_FW_G.hwnd);

    {
        VdFwEvent evt;
        evt.type = VD_FW_EVENT_TYPE_SCALE_CHANGE;
        evt.data.scale_change.new_scale = VdFwGetDpiForWindow(VD_FW_G.hwnd) / 90.f;
        VD_FW_G.scale = evt.data.scale_change.new_scale;
        vd_fw__msgbuf_w(&evt);
    }

    VD_FW__CHECK_TRUE(ReleaseSemaphore(VD_FW_G.sem_window_ready, 1, NULL));

    // Register raw input mouse
    VdFwRAWINPUTDEVICE rids[2];
    rids[0].usUsagePage = 0x01; // Generic desktop controls
    rids[0].usUsage     = 0x02; // Mouse
    rids[0].dwFlags     = 0x00; // None (NO RIDEV_INPUTSINK)
    rids[0].hwndTarget  = VD_FW_G.hwnd;

    // rids[1].usUsagePage = 0x01; // Generic desktop controls
    // rids[1].usUsage     = 0x05; // Gamepad
    // rids[1].dwFlags     = RIDEV_DEVNOTIFY | RIDEV_INPUTSINK;
    // rids[1].hwndTarget  = VD_FW_G.hwnd;
    VD_FW__CHECK_TRUE(VdFwRegisterRawInputDevices(rids, 1, sizeof(rids[0])));

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

VD_FW_API char *vd_fw__debug_dump_file_text(const char *path, size_t *size)
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

    char *memory = (char*)VD_FW_REALLOC(0, 0, sz.QuadPart + 1);

    DWORD bytes_read;
    if (!ReadFile(hfile, memory, (DWORD)sz.QuadPart, &bytes_read, 0)) {
        VD_FW_FREE(memory, sz.QuadPart + 1);
        return 0;
    }

    memory[sz.QuadPart] = 0;
    *size = sz.QuadPart + 1;
    return memory;
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
        if (VD_FW_G.winthread_resizable) {
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
        }

        if (!VD_FW_G.nccaption_set) {
            return HTCAPTION;
        }

        int inside_caption = 
            ((mouse.x >= VD_FW_G.nccaption[0]) && (mouse.x <= VD_FW_G.nccaption[2])) &&
            ((mouse.y >= VD_FW_G.nccaption[1]) && (mouse.y <= VD_FW_G.nccaption[3]));

        if (inside_caption) {
            for (int ri = 0; ri < VD_FW_G.ncrect_count; ++ri) {
                int rect[4];
                rect[0] = VD_FW_G.ncrects[ri][0];
                rect[1] = VD_FW_G.ncrects[ri][1];
                rect[2] = VD_FW_G.ncrects[ri][2];
                rect[3] = VD_FW_G.ncrects[ri][3];

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

static void vd_fw__update_monitors(void)
{

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
            VdFwEvent evt;
            evt.type = VD_FW_EVENT_TYPE_CLOSE_REQUEST;
            vd_fw__msgbuf_w(&evt);
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
                if (!VD_FW_G.winthread_block_while_sizing) {
                    EnterCriticalSection(&VD_FW_G.critical_section);
                }

                if (VD_FW_G.w != VD_FW_G.next_frame.w || VD_FW_G.h != VD_FW_G.next_frame.h) {
                    VD_FW_G.next_frame.w = VD_FW_G.w;
                    VD_FW_G.next_frame.h = VD_FW_G.h;
                    VD_FW_G.next_frame.flags |= VD_FW_WIN32_FLAGS_SIZE_CHANGED;
                }

                VD_FW_G.next_frame.flags |= VD_FW_WIN32_FLAGS_WAKE_COND_VAR;

                if (!VD_FW_G.winthread_block_while_sizing) {
                    WakeConditionVariable(&VD_FW_G.cond_var);
                    SleepConditionVariableCS(&VD_FW_G.cond_var, &VD_FW_G.critical_section, INFINITE);
                    LeaveCriticalSection(&VD_FW_G.critical_section);
                }

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

        case WM_ENTERSIZEMOVE: {
            if (VD_FW_G.winthread_block_while_sizing) {
                EnterCriticalSection(&VD_FW_G.critical_section);
            }

        } break;

        case WM_EXITSIZEMOVE: {
            if (VD_FW_G.winthread_block_while_sizing) {
                LeaveCriticalSection(&VD_FW_G.critical_section);
            }

            if (!VD_FW_G.draw_decorations) {
                VdFwEvent evt;
                evt.type = VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP;
                evt.data.mouse_button_up.button = vd_fw__win32_translate_button(VK_LBUTTON);
                vd_fw__msgbuf_w(&evt);
            }

            result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
        } break;

        case WM_SYSCOMMAND: {
            if (!VD_FW_G.draw_decorations) {
                if (wparam == 0x0000F012) {
                    VdFwEvent evt;
                    evt.type = VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP;
                    evt.data.mouse_button_up.button = vd_fw__win32_translate_button(VK_LBUTTON);
                    vd_fw__msgbuf_w(&evt);
                }
            }
            result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
        } break;

        case WM_DPICHANGED: {
            VdFwUINT dpi = VD_FW_HIWORD(wparam);

            VdFwRECT *rect = (VdFwRECT*)lparam;
            VdFwSetWindowPos(hwnd, 0, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, SWP_NOZORDER | SWP_NOACTIVATE);

            VdFwEvent evt;
            evt.type = VD_FW_EVENT_TYPE_SCALE_CHANGE;
            evt.data.scale_change.new_scale = (float)dpi / 90.f;
            vd_fw__msgbuf_w(&evt);
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
                    VdFwEvent evt;
                    evt.type = VD_FW_EVENT_TYPE_WINDOW_STATE_CHANGE;
                    evt.data.window_state_change.flag = VD_FW_WINDOW_STATE_MINIMIZED;
                    evt.data.window_state_change.value = 1;
                    vd_fw__msgbuf_w(&evt);
                } break;

                case SIZE_RESTORED: {
                    VdFwEvent evt;
                    evt.type = VD_FW_EVENT_TYPE_WINDOW_STATE_CHANGE;
                    evt.data.window_state_change.flag = VD_FW_WINDOW_STATE_MINIMIZED;
                    evt.data.window_state_change.value = 0;
                    vd_fw__msgbuf_w(&evt);

                    evt.data.window_state_change.flag = VD_FW_WINDOW_STATE_MAXIMIZED;
                    evt.data.window_state_change.value = 0;
                    vd_fw__msgbuf_w(&evt);
                } break;

                case SIZE_MAXIMIZED: {
                    VdFwEvent evt;
                    evt.type = VD_FW_EVENT_TYPE_WINDOW_STATE_CHANGE;
                    evt.data.window_state_change.flag = VD_FW_WINDOW_STATE_MAXIMIZED;
                    evt.data.window_state_change.value = 1;
                    vd_fw__msgbuf_w(&evt);
                } break;

                default: break;
            }

            if ((wparam == SIZE_MAXIMIZED) || (wparam == SIZE_MINIMIZED) || (wparam == SIZE_RESTORED)) {
                // @note(mdodis): Send a mouse release event right as we go into minimized or out of maximized/minimized
                // state. This is because we'll miss the mouse release otherwise.
                VdFwEvent evt;
                evt.type = VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP;
                evt.data.mouse_button_up.button = vd_fw__win32_translate_button(VK_LBUTTON);
                vd_fw__msgbuf_w(&evt);
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

                VdFwEvent evt;
                evt.type = VD_FW_EVENT_TYPE_MOUSE_DELTA;
                evt.data.mouse_delta.dx = (float)dx;
                evt.data.mouse_delta.dy = (float)dy;
                vd_fw__msgbuf_w(&evt);
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

        // case WM_SYSKEYUP:
        // case WM_SYSKEYDOWN: {

        //     if (wparam == VK_F4) {
        //     } else if (wparam == VK_F10) {
        //         WORD vkcode = LOWORD(wparam);
        //         VdFwEvent evt;
        //         evt.type = VD_FW_EVENT_TYPE_KEY_DOWN;
        //         evt.data.key_down.modifiers = 0;
        //         evt.data.key_down.key = vd_fw___vkcode_to_key(vkcode);
        //         evt.data.key_down.repeat = 0;
        //         vd_fw__msgbuf_w(&evt);
        //     }
        //     // if (!VD_FW_G.draw_decorations) {


        //     //     result = 0;
        //     // } else {
        //     //     result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
        //     // }

        // } break;

        case WM_SYSKEYUP:
        case WM_SYSKEYDOWN:
        case WM_KEYUP:
        case WM_KEYDOWN: {

            WORD vkcode = LOWORD(wparam);
            
            WORD keyflags = HIWORD(lparam);

            WORD scancode = LOBYTE(keyflags);                             // scan code
            BOOL isextendedkey = (keyflags & KF_EXTENDED) == KF_EXTENDED; // extended-key flag, 1 if scancode has 0xE0 prefix

            BOOL shift   = (VdFwGetKeyState(VK_SHIFT)   & 0x8000) != 0 ? 1 : 0;
            BOOL ctrl    = (VdFwGetKeyState(VK_CONTROL) & 0x8000) != 0 ? 1 : 0;
            BOOL alt     = (VdFwGetKeyState(VK_MENU)    & 0x8000) != 0 ? 1 : 0;            

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

            int is_down = (msg == WM_KEYDOWN) || (msg == WM_SYSKEYDOWN);

            VdFwEvent evt;
            if (is_down) {

                if ((vkcode == VK_F4) && (alt && !shift && !ctrl)) {
                    VdFwPostMessage(hwnd, WM_CLOSE, 0, 0);
                }

                int repeat = (lparam & (1 << 30)) != 0;
                evt.type = VD_FW_EVENT_TYPE_KEY_DOWN;
                evt.data.key_down.modifiers = 0;
                if (shift) { evt.data.key_down.modifiers |= VD_FW_MOD_SHIFT; }
                if (ctrl)  { evt.data.key_down.modifiers |= VD_FW_MOD_CONTROL; }
                if (alt)   { evt.data.key_down.modifiers |= VD_FW_MOD_ALT; }
                evt.data.key_down.key = vd_fw___vkcode_to_key(vkcode);
                evt.data.key_down.repeat = repeat;
            } else {
                evt.type = VD_FW_EVENT_TYPE_KEY_UP;
                evt.data.key_up.key = vd_fw___vkcode_to_key(vkcode);
            }

            vd_fw__msgbuf_w(&evt);

            result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
        } break;

        case WM_UNICHAR: {
            result = 1;
            if (wparam == UNICODE_NOCHAR) {
                break;
            }


            VdFwEvent evt;
            evt.type = VD_FW_EVENT_TYPE_CHARACTER;
            evt.data.character.codepoint = (VdFwU32)wparam;
            vd_fw__msgbuf_w(&evt);
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

            VdFwEvent evt;
            evt.type = VD_FW_EVENT_TYPE_CHARACTER;
            evt.data.character.codepoint = codepoint;
            vd_fw__msgbuf_w(&evt);
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

            VdFwEvent evt;
            if (down) {
                evt.type = VD_FW_EVENT_TYPE_MOUSE_BUTTON_DOWN;
                evt.data.mouse_button_down.button = vd_fw__win32_translate_button((VdFwWORD)code);
            } else {
                evt.type = VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP;
                evt.data.mouse_button_up.button = vd_fw__win32_translate_button((VdFwWORD)code);
            }
            vd_fw__msgbuf_w(&evt);
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

                VdFwEvent evt;
                if (down) {
                    evt.type = VD_FW_EVENT_TYPE_MOUSE_BUTTON_DOWN;
                    evt.data.mouse_button_down.button = vd_fw__win32_translate_button((VdFwWORD)code);
                } else {
                    evt.type = VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP;
                    evt.data.mouse_button_up.button = vd_fw__win32_translate_button((VdFwWORD)code);
                }
                vd_fw__msgbuf_w(&evt);
            }

            result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
        } break;

        case WM_SETFOCUS:
        case WM_KILLFOCUS: {
            int got_focus = msg == WM_SETFOCUS;
            VdFwEvent evt;
            evt.type = VD_FW_EVENT_TYPE_FOCUS_CHANGE;
            evt.data.focus_change.got_focus = got_focus;
            vd_fw__msgbuf_w(&evt);
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
                VdFwPOINT p;
                p.x = x;
                p.y = y;
                VdFwScreenToClient(hwnd, &p);
                x = p.x;
                y = p.y;

                // x -= rect.left;
                // y -= rect.top;
            }

            VdFwEvent evt;
            evt.type = VD_FW_EVENT_TYPE_MOUSE_MOVE;
            evt.data.mouse_move.x = x;
            evt.data.mouse_move.y = y;
            vd_fw__msgbuf_w(&evt);
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
                VdFwLONG style;
                VdFwUINT flags;

                if (VD_FW_G.draw_decorations) {
                    style = WS_POPUP | WS_VISIBLE;
                    flags = SWP_FRAMECHANGED;
                } else {
                    style = WS_POPUP | WS_VISIBLE;
                    flags = SWP_FRAMECHANGED;
                }

                VdFwSetWindowLong(VD_FW_G.hwnd, GWL_STYLE, style);
                VdFwSetWindowPos(VD_FW_G.hwnd, VD_FW_HWND_TOP,
                             monitor_info.rcMonitor.left , monitor_info.rcMonitor.top,
                             monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
                             monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
                             flags);

                if (VD_FW_G.draw_decorations) {

                    VdFwMARGINS m = {-1,-1,-1,-1};
                    VD_FW__CHECK_HRESULT(VdFwDwmExtendFrameIntoClientArea(VD_FW_G.hwnd, &m));
                }
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

        case VD_FW_WIN32_RESIZABLE: {
            int on = (int)lparam;
            VD_FW_G.winthread_resizable = on;
            LONG style = VdFwGetWindowLongA(hwnd, GWL_STYLE);
            if (on) {
                if (VD_FW_G.draw_decorations) {
                    style |= WS_MAXIMIZEBOX;
                }

                style |= WS_SIZEBOX;
            } else {
                style &= ~WS_MAXIMIZEBOX;
                style &= ~WS_SIZEBOX;
            }

            VdFwSetWindowLong(hwnd, GWL_STYLE, style);
        } break;

        case VD_FW_WIN32_BLOCKMODE: {
            int on = (int)lparam;
            VD_FW_G.winthread_block_while_sizing = on;
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
            VdFwEvent evt;
            evt.type = VD_FW_EVENT_TYPE_MOUSE_SCROLL;
            evt.data.mouse_scroll.dx = dx;
            evt.data.mouse_scroll.dy = 0.f;
            vd_fw__msgbuf_w(&evt);
        } break;

        case WM_MOUSEWHEEL: {
            if (!VD_FW_G.t_paint_ready) { result = VdFwDefWindowProc(hwnd, msg, wparam, lparam); break; }
            int delta = GET_WHEEL_DELTA_WPARAM(wparam);
            float dy = (float)delta / (float)WHEEL_DELTA;

            VdFwEvent evt;
            evt.type = VD_FW_EVENT_TYPE_MOUSE_SCROLL;
            evt.data.mouse_scroll.dx = 0.f;
            evt.data.mouse_scroll.dy = dy;
            vd_fw__msgbuf_w(&evt);
        } break;

        default: {
            result = VdFwDefWindowProc(hwnd, msg, wparam, lparam);
        } break;
    }    
    return result;
}

static int vd_fw__msgbuf_r(VdFwEvent *message)
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

static int vd_fw__msgbuf_w(VdFwEvent *message)
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

static void vd_fw__update_kb_codepage(void)
{
    VdFwHKL keyboard_layout = VdFwGetKeyboardLayout(0);

    LCID keyboard_lcid = MAKELCID(HIWORD(keyboard_layout), SORT_DEFAULT);
    if (GetLocaleInfoA(keyboard_lcid, (LOCALE_RETURN_NUMBER | LOCALE_IDEFAULTANSICODEPAGE), (LPSTR)&VD_FW_G.kb_codepage, sizeof(VD_FW_G.kb_codepage)) == 0) {
        VD_FW_G.kb_codepage = CP_ACP;
    }
}

static VdFwBOOL vd_fw__win32_enum_monitor(VdFwHMONITOR monitor, VdFwHDC hdc, VdFwLPRECT rect, VdFwLPARAM lpparam)
{
    (void)lpparam;
    (void)hdc;
    (void)rect;

    VdFwMONITORINFOEXW monitor_info = {};
    monitor_info.monitor_info.cbSize = sizeof(monitor_info);
    VD_FW__CHECK_TRUE(VdFwGetMonitorInfoW(monitor, (VdFwLPMONITORINFO)&monitor_info));

    VdFwDISPLAY_DEVICEW display_monitor = {0};
    display_monitor.cb = sizeof(display_monitor);
    if (!VdFwEnumDisplayDevicesW(monitor_info.szDevice, 0, (VdFwPDISPLAY_DEVICEW)&display_monitor, VD_FW__WIN32_EDD_GET_DEVICE_INTERFACE_NAME)) {
        return TRUE;
    }

    static VdFw__Win32GUID guid_devinterface_monitor = { 0xe6f07b5f, 0xee97, 0x4a90, 0xb0, 0x76, 0x33, 0xf5, 0x7b, 0xf4, 0xea, 0xa7 };

    VdFwHDEVINFO devinfo = VdFwSetupDiGetClassDevsW(&guid_devinterface_monitor,
                                                    NULL, NULL, VD_FW__WIN32_DIGCF_DEVICEINTERFACE);
    if (devinfo == NULL) {
        return TRUE;
    }

    VdFwSP_DEVICE_INTERFACE_DATA di_data = {0};
    di_data.cbSize = sizeof(di_data);
    VdFwDWORD di_index = 0;
    static char di_detail_data_buffer[512] = {0}; 
    VdFwSP_DEVICE_INTERFACE_DETAIL_DATA_W *pdi_detail_data = (VdFwSP_DEVICE_INTERFACE_DETAIL_DATA_W*)di_detail_data_buffer;
    pdi_detail_data->cbSize = sizeof(*pdi_detail_data);

    int found = 0;

    while (VdFwSetupDiEnumDeviceInterfaces(devinfo, NULL, &guid_devinterface_monitor, di_index, &di_data)) {

        if (VdFwSetupDiGetDeviceInterfaceDetailW(devinfo, &di_data,
                                                 pdi_detail_data, 512, // DeviceInterfaceDetailDataSize
                                                 0, NULL))
        {

            if (vd_fw__compare_string_wide_nullsep_case_insensitive(pdi_detail_data->DevicePath,
                                                                    display_monitor.DeviceID))
            {
                found = 1;
                break;
            }
        }

        di_index++;
    }

    if (!found) {
        return TRUE;
    }


    VdFwSP_DEVINFO_DATA devinfo_data = {0};
    devinfo_data.cbSize = sizeof(devinfo_data);
    if (!VdFwSetupDiEnumDeviceInfo(devinfo, di_index, &devinfo_data)) {
        return TRUE;
    }

    VdFwHKEY hk = VdFwSetupDiOpenDevRegKey(devinfo, &devinfo_data, 
                                           VD_FW__WIN32_DICS_FLAG_GLOBAL, 0, VD_FW__WIN32_DIREG_DEV,
                                           VD_FW__WIN32_KEY_READ);
    if (hk == NULL) {
        return TRUE;
    }

    static VdFwBYTE edid_data[1024];
    VdFwDWORD edid_data_size = sizeof(edid_data);
    VdFwEdid1_4 *edid = 0;
    int display_name_len = 0;
    const char *display_name = 0;
    VdFw__Win32Monitor *mmonitor = 0;

    if (VdFwRegQueryValueExW(hk, L"EDID", NULL, NULL, edid_data, &edid_data_size) != 0) {
        goto WIN32_DISPLAY_MONITOR_EDID_FAIL;
    }

    if (edid_data_size < sizeof(VdFwEdid1_4)) {
        goto WIN32_DISPLAY_MONITOR_EDID_FAIL;
    }


    edid = (VdFwEdid1_4*)edid_data;
    if ((edid->header[0] != 0x00) || (edid->header[1] != 0xFF) ||
        (edid->header[2] != 0xFF) || (edid->header[3] != 0xFF) ||
        (edid->header[4] != 0xFF) || (edid->header[5] != 0xFF) ||
        (edid->header[6] != 0xFF) || (edid->header[7] != 0x00))
    {
        goto WIN32_DISPLAY_MONITOR_EDID_FAIL;
    }

    display_name_len = sizeof("Generic PnP Monitor") - 1;
    display_name = "Generic PnP Monitor";

    for (int i = 0; i < 4; ++i) {
        VdFwEdid1_4DataBlock *data_block = &edid->data_blocks[i];
        if (data_block->detailed_timing.pixel_clock != 0) {
            continue;
        }

        VdFwEdid1_4DisplayDescriptor *display_descriptor = &data_block->display_descriptor;

        if (display_descriptor->tag != 0xFC) {
            continue;
        }

        display_name = display_descriptor->data_block.product_name.name;

        int len = 0;
        const char *c = display_name;
        while ((len < 13) && (*c != '\r') && (*c != '\n')) {
            len++;
            c++;
        }

        display_name_len = len;
        break;
    }

    mmonitor = &VD_FW_G.monitor_buffer[VD_FW_G.monitor_buffer_len++];
    VD_FW_MEMCPY(mmonitor->friendly_name, display_name, display_name_len);
    mmonitor->friendly_name[display_name_len] = 0;
    mmonitor->hmonitor = monitor;
    mmonitor->display_modes_len = 0;

WIN32_DISPLAY_MONITOR_EDID_FAIL:
    VdFwRegCloseKey(hk);
    return TRUE;
}

static VdFwBOOL vd_fw__win32_enum_monitor_resize_count(VdFwHMONITOR monitor, VdFwHDC hdc, VdFwLPRECT rect, VdFwLPARAM lpparam)
{
    (void)monitor;
    (void)hdc;
    (void)rect;
    (void)lpparam;
    VD_FW_G.monitor_count++;
    return TRUE;
}

static void vd_fw__win32_update_monitor_display_modes(VdFw__Win32Monitor *monitor)
{
    if (monitor->display_modes_len != 0) {
        return;
    }

    VdFwMONITORINFOEXW monitor_info = {};
    monitor_info.monitor_info.cbSize = sizeof(monitor_info);
    VD_FW__CHECK_TRUE(VdFwGetMonitorInfoW(monitor->hmonitor, (VdFwLPMONITORINFO)&monitor_info));

    VdFwDEVMODEW devmode;
    devmode.dmSize = sizeof(devmode);
    int display_mode_count = 0;
    int graphics_mode_index = 0;
    while (VdFwEnumDisplaySettingsW((VdFwLPCSTR)monitor_info.szDevice, graphics_mode_index, &devmode)) {
        graphics_mode_index++;

        if (devmode.dmBitsPerPel < 15) {
            continue;
        }

        if (devmode.extra.displays.dmDisplayOrientation != 0) {
            continue;
        }

        display_mode_count++;
    }

    monitor->display_modes = (VdFwDisplayMode*)vd_fw__resize_buffer(monitor->display_modes, sizeof(*monitor->display_modes),
                                                                    display_mode_count, &monitor->display_modes_cap);

    graphics_mode_index = 0;
    while (VdFwEnumDisplaySettingsW((VdFwLPCSTR)monitor_info.szDevice, graphics_mode_index, &devmode)) {
        graphics_mode_index++;

        if (devmode.dmBitsPerPel < 15) {
            continue;
        }

        if (devmode.extra.displays.dmDisplayOrientation != 0) {
            continue;
        }

        VdFwDisplayMode candidate_mode;
        candidate_mode.width = devmode.dmPelsWidth;
        candidate_mode.height = devmode.dmPelsHeight;
        candidate_mode.frequency = devmode.dmDisplayFrequency;

        int skip_mode = 0;

        for (int i = 0; i < monitor->display_modes_len; ++i) {

            VdFwDisplayMode *display_mode = &monitor->display_modes[i];

            if ((display_mode->width == candidate_mode.width) &&
                (display_mode->height == candidate_mode.height) &&
                (display_mode->frequency == candidate_mode.frequency))
            {
                skip_mode = 1;
                break;
            }
        }

        if (skip_mode) {
            continue;
        }

        VdFwDisplayMode *display_mode = &monitor->display_modes[monitor->display_modes_len++];

        VdFwU32 gcd = vd_fw__gcd(devmode.dmPelsWidth, devmode.dmPelsHeight);

        display_mode->width = devmode.dmPelsWidth;
        display_mode->height = devmode.dmPelsHeight;
        display_mode->frequency = devmode.dmDisplayFrequency;
        display_mode->aspect.numerator = devmode.dmPelsWidth / gcd;
        display_mode->aspect.denominator = devmode.dmPelsHeight / gcd;
    }

    vd_fw__sort_display_modes(monitor->display_modes, monitor->display_modes_len);
}

static void vd_fw__win32_update_monitors(void)
{
    VD_FW_G.monitor_count = 0;
    VdFwEnumDisplayMonitors(NULL, NULL, vd_fw__win32_enum_monitor_resize_count, (VdFwLPARAM)NULL);

    VD_FW_G.monitor_buffer_len = 0;
    VD_FW_G.monitor_buffer = (VdFw__Win32Monitor*)vd_fw__resize_buffer(VD_FW_G.monitor_buffer, sizeof(*VD_FW_G.monitor_buffer),
                                                                       VD_FW_G.monitor_count, &VD_FW_G.monitor_buffer_cap);

    VdFwEnumDisplayMonitors(NULL, NULL, vd_fw__win32_enum_monitor, (VdFwLPARAM)NULL);
    VdFwPOINT p = {0, 0};
    VdFwHMONITOR primary_monitor = VdFwMonitorFromPoint(p, MONITOR_DEFAULTTOPRIMARY);

    int primary_monitor_index = -1;
    for (int i = 0; i < VD_FW_G.monitor_buffer_len; ++i) {
        if (VD_FW_G.monitor_buffer[i].hmonitor == primary_monitor) {
            primary_monitor_index = i;
            break;
        }
    }

    if ((primary_monitor_index != -1) && (primary_monitor_index != 0)) {
        VdFw__Win32Monitor temp = VD_FW_G.monitor_buffer[0];
        VD_FW_G.monitor_buffer[0] = VD_FW_G.monitor_buffer[primary_monitor_index];
        VD_FW_G.monitor_buffer[primary_monitor_index] = temp;
    }
}

const char *Vd_Fw_Vk_Instance_Extensions[] = {
    "VK_KHR_win32_surface",
};

VD_FW_API void vd_fw_vk_set_get_instance_proc_addr(VdFwVkGetInstanceProcAddrProc proc)
{
    VD_FW_G.vk_get_instance_proc_addr = proc;
}

VD_FW_API const char **vd_fw_vk_wsi_instance_extensions(int *count)
{
    if (count) {
        *count = sizeof(Vd_Fw_Vk_Instance_Extensions) / sizeof(Vd_Fw_Vk_Instance_Extensions[0]);
    }
    return Vd_Fw_Vk_Instance_Extensions;
}

typedef struct VdFwVkWin32SurfaceCreateInfoKHR {
    int                                 sType;
    const void*                         pNext;
    VdFwU32                             flags;
    VdFwHINSTANCE                       hinstance;
    VdFwHWND                            hwnd;
} VdFwVkWin32SurfaceCreateInfoKHR;

VD_FW_API int vd_fw_vk_wsi_surface_create(void *p_instance, void *p_allocation_callbacks, void *p_surface)
{
    VdFwVkWin32SurfaceCreateInfoKHR surface_create_info;
    surface_create_info.sType     = 1000009000 /*VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR*/;
    surface_create_info.pNext     = 0;
    surface_create_info.flags     = 0;
    surface_create_info.hinstance = (VdFwHINSTANCE)GetModuleHandleA(NULL);
    surface_create_info.hwnd      = VD_FW_G.hwnd;

    if (VD_FW_G.vk_create_win32_surface_khr_proc == 0) {
        VD_FW_G.vk_create_win32_surface_khr_proc = (VdFw__vkCreateWin32SurfaceKHRProc)
            VD_FW_G.vk_get_instance_proc_addr(p_instance, "vkCreateWin32SurfaceKHR");
    }

    if (VD_FW_G.vk_create_win32_surface_khr_proc) {
        return VD_FW_G.vk_create_win32_surface_khr_proc(p_instance, (void*)&surface_create_info, p_allocation_callbacks, p_surface);
    } else {
        return -13;
    }
}

#elif defined(__APPLE__)
#import <AppKit/AppKit.h>
#import <IOKit/IOKitLib.h>
#import <IOKit/hid/IOHIDManager.h>
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
    VD_FW__MAC_MESSAGE_FULLSCREEN,
    VD_FW__MAC_MESSAGE_GAMEPAD_CONNECTED,
    VD_FW__MAC_MESSAGE_GAMEPAD_DISCONNECTED,
    VD_FW__MAC_MESSAGE_GAMEPAD_INPUT,
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

        struct {
            int on;
        } fullscreen;

        struct {
            int gamepad_index;
        } gamepad_connected, gamepad_disconnected;

        struct {
            int                 gamepad_index;
            VdFwGamepadMapEntry entry;
            VdFwI32             value;
            float               float_value;
        } gamepad_input;
    } dat;
} VdFw__MacMessage;

typedef struct {
    int w, h;
    int flags;
} VdFw__MacFrame;

typedef struct {
    IOHIDDeviceRef device;
    VdFwGamepadMap map;
} VdFw__MacGamepadInfo;

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
    int                         is_fullscreen;
    int                         fullscreen_changed_this_frame;
    uint64_t                    delta_ns;
    int                         num_gamepads_present;
    VdFw__GamepadState          gamepad_curr_states[VD_FW_GAMEPAD_COUNT_MAX];
    VdFw__GamepadState          gamepad_prev_states[VD_FW_GAMEPAD_COUNT_MAX];

/* ----WINDOW THREAD ONLY-------------------------------------------------------------------------------------------- */
    BOOL                        draw_decorations;
    VdFwInitInfo                c_init_info;
    VdFwWindow                  *window;
    int                         context_needs_update;
    BOOL                        is_zoomed;
    int                         cap_gamepad_db_entries;
    int                         num_gamepad_db_entries;
    VdFwGamepadDBEntry          *gamepad_db_entries;
    int                         winthread_num_gamepads;
    VdFw__MacGamepadInfo        gamepad_infos[VD_FW_GAMEPAD_COUNT_MAX];


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
    int                         main_thread_exited;
} VdFw__MacOsInternalData;

static int vd_fw__msgbuf_r(VdFw__MacMessage *message);
static int vd_fw__msgbuf_w(VdFw__MacMessage *message);
static void vd_fw__mac_hid_device_added_callback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);
static void vd_fw__mac_hid_device_removed_callback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device);
static void vd_fw__mac_hid_value_callback(void *context, IOReturn result, void *sender, IOHIDValueRef value);


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

- (void)backingChanged:(NSNotification *)note {
    NSWindow *win = note.object;
    CGFloat scale = win.backingScaleFactor;
    VD_FW_G.scale = scale;

    NSRect rect = [[VD_FW_G.window contentView] frame];
    VD_FW_G.w = (int)rect.size.width * VD_FW_G.scale;
    VD_FW_G.h = (int)rect.size.height * VD_FW_G.scale;

    if (!VD_FW_G.context_update_requested) {
        sem_post(VD_FW_G.s_main_thread_context_needs_update);
        VD_FW_G.context_update_requested = 1;
    }
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

- (void) windowDidEnterFullScreen:(NSNotification *) notification
{
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_FULLSCREEN;
    msg.dat.fullscreen.on = 1;
    vd_fw__msgbuf_w(&msg); 
}

- (void) windowDidExitFullScreen:(NSNotification *) notification
{
    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_FULLSCREEN;
    msg.dat.fullscreen.on = 0;
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

    float dx = [evt deltaX];
    float dy = [evt deltaY];

    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_MOUSEMOVE;
    msg.dat.mousemove.mx = scaled_pos.x;
    msg.dat.mousemove.my = scaled_pos.y;
    msg.dat.mousemove.dx = dx;
    msg.dat.mousemove.dy = dy;
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
    return VD_FW_G.delta_ns;
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
    (void)index;
    (void)button;
    return 0;
}

VD_FW_API int vd_fw_get_gamepad_axis(int index, int axis, float *out)
{
    *out = VD_FW_G.gamepad_curr_states[index].axes[axis];
    return 0;
}

VD_FW_API void vd_fw_set_gamepad_rumble(int index, float rumble_lo, float rumble_hi)
{
    (void)index;
    (void)rumble_lo;
    (void)rumble_hi;
}

VD_FW_API VdFwGuid vd_fw_get_gamepad_guid(int index)
{
    (void)index;
    VdFwGuid result = {0};
    return result;
}

VD_FW_API VdFwGamepadFace vd_fw_get_gamepad_face(int index)
{
    (void)index;
    return VD_FW_GAMEPAD_FACE_UNKNOWN;
}

VD_FW_API VdFwGamepadClass vd_fw_get_gamepad_class(int index)
{
    (void)index;
    return VD_FW_GAMEPAD_CLASS_XBOX;
}

VD_FW_API int vd_fw_get_gamepad_rumble_support(int index)
{
    (void)index;
    return 0;
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

    for (int i = 0; i < VD_FW_GAMEPAD_COUNT_MAX; ++i) {
        VD_FW_G.gamepad_prev_states[i] = VD_FW_G.gamepad_curr_states[i];
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

            case VD_FW__MAC_MESSAGE_FULLSCREEN: {
                VD_FW_G.is_fullscreen = msg.dat.fullscreen.on;
            } break;

            case VD_FW__MAC_MESSAGE_CLOSE_REQUEST: {
                VD_FW_G.close_request = 1;
            } break;

            case VD_FW__MAC_MESSAGE_GAMEPAD_CONNECTED: {
                VdFw__GamepadState zero_state = {0};
                VD_FW_G.gamepad_prev_states[msg.dat.gamepad_connected.gamepad_index] = zero_state;
                VD_FW_G.gamepad_curr_states[msg.dat.gamepad_connected.gamepad_index] = zero_state;
                VD_FW_G.num_gamepads_present++;
            } break;

            case VD_FW__MAC_MESSAGE_GAMEPAD_DISCONNECTED: {
                VD_FW_G.num_gamepads_present--;
            } break;

            case VD_FW__MAC_MESSAGE_GAMEPAD_INPUT: {
                VdFw__GamepadState *state = &VD_FW_G.gamepad_curr_states[msg.dat.gamepad_input.gamepad_index];
                VdFwGamepadMapEntry *entry = &msg.dat.gamepad_input.entry;
                int value = msg.dat.gamepad_input.value;

                switch ((entry->kind & VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_MASK)) {

                    case VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON: {
                        if (value) {
                            state->bits |= (1 << entry->target);
                        } else {
                            state->bits &= ~(1 << entry->target);
                        }
                    } break;

                    case VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS: {
                        state->axes[entry->target] = msg.dat.gamepad_input.float_value;
                    } break;

                    default: break;
                }
            } break;

            default: break;
        }
    }

    pthread_mutex_lock(&VD_FW_G.m_paint);
    VD_FW_G.curr_frame = VD_FW_G.next_frame;
    VD_FW_G.next_frame.flags = 0;
    pthread_mutex_unlock(&VD_FW_G.m_paint);

    uint64_t now = mach_absolute_time();
    uint64_t ns = (now - VD_FW_G.last_time) * VD_FW_G.time_base.numer / VD_FW_G.time_base.denom;
    VD_FW_G.delta_ns = ns;
    VD_FW_G.last_time = now;

    return !VD_FW_G.should_close;
}

VD_FW_API int vd_fw_swap_buffers(void)
{
    if (VD_FW_G.fullscreen_changed_this_frame) {
        for (int i = 0; i < VD_FW_KEY_MAX; ++i) {
            VD_FW_G.prev_key_states[i] = VD_FW_G.curr_key_states[i];
            VD_FW_G.curr_key_states[i] = 0;
        }

        VD_FW_G.fullscreen_changed_this_frame = 0;
    }

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

VD_FW_API void vd_fw_set_size(int w, int h)
{
    dispatch_async(dispatch_get_main_queue(), ^(void) {
        NSRect frame = [VD_FW_G.window frame];
        frame.origin.y -= frame.size.height;
        frame.origin.y += h; 
        frame.size.width = w;
        frame.size.height = h;
        [VD_FW_G.window setFrame: frame display: YES animate: NO];
    });
}

VD_FW_API int vd_fw_get_minimized(int *minimized)
{
    if (minimized) {
        *minimized = VD_FW_G.window_state & VD_FW__MAC_WINDOW_STATE_MINIMIZED;
    }
    return VD_FW_G.window_state_changed;
}

VD_FW_API void vd_fw_set_minimized(void)
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

VD_FW_API void vd_fw_set_maximized(void)
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

VD_FW_API void vd_fw_set_fullscreen(int on)
{
    if (VD_FW_G.is_fullscreen == on) {
        return;
    }

    VD_FW_G.fullscreen_changed_this_frame = 1;

    dispatch_async(dispatch_get_main_queue(), ^{
        [VD_FW_G.window toggleFullScreen: nil];
    });

}

VD_FW_API int vd_fw_get_fullscreen(void)
{
    return VD_FW_G.is_fullscreen;
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

VD_FW_API int vd_fw_get_scale(float *scale)
{
    *scale = VD_FW_G.scale;
    return 1;
}

VD_FW_API void vd_fw_set_title(const char *title)
{

    NSString *s = [NSString stringWithUTF8String: title];

    dispatch_sync(dispatch_get_main_queue(), ^(void){
        [VD_FW_G.window setTitle: s];
        [s release];
    });
}

VD_FW_API void vd_fw_set_app_icon(void *pixels, int width, int height)
{
    VdFwU32 *app_image_data = (VdFwU32*)VD_FW_REALLOC(0, 0, sizeof(unsigned int) * width * height);
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

VD_FW_API void *vd_fw_get_internal_window_handle(void)
{
    return (void*)VD_FW_G.window;
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

    char *result = (char*)VD_FW_REALLOC(0, 0, size +1);
    fread(result, size, 1, f);

    result[size] = 0;
    return result;
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


#ifndef VD_FW_MACOS_NO_MAIN_OVERRIDE
#   undef main
static void *vd_fw__macos__main(void *arg)
{
    (void)arg;
    vd_fw__macos_main(VD_FW_G.argc, VD_FW_G.argv);
    VD_FW_G.main_thread_exited = 1;
    sem_post(VD_FW_G.s_main_thread_opened_me);
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

        if (VD_FW_G.main_thread_exited) {
            break;
        }

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

    // Window
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
                                                         defer: NO
                                                        screen: main_screen];

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
            // NSOpenGLPFAMultisample,
            // NSOpenGLPFASampleBuffers, (NSOpenGLPixelFormatAttribute)1,
            // NSOpenGLPFASamples, (NSOpenGLPixelFormatAttribute)4,
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
        [[NSNotificationCenter defaultCenter] addObserver: delegate
                                              selector: @selector(backingChanged:)
                                              name: NSWindowDidChangeBackingPropertiesNotification
                                              object: VD_FW_G.window];

    }
    VD_FW_G.w = 640 * VD_FW_G.scale;
    VD_FW_G.h = 480 * VD_FW_G.scale;
    vd_fw__load_opengl(version);

    mach_timebase_info(&VD_FW_G.time_base);
    VD_FW_G.last_time = mach_absolute_time();

    dispatch_async(dispatch_get_main_queue(), ^{
        [VD_FW_G.window makeKeyAndOrderFront:nil];
    });

    // IOKit
    int filter[] = {kHIDPage_GenericDesktop};
    IOHIDManagerRef hidman = IOHIDManagerCreate(kCFAllocatorDefault, kIOHIDManagerOptionNone);

    const void *keys[] = {
        CFSTR(kIOHIDDeviceUsagePageKey),
    };

    const void *values[] = {
        CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, filter)
    };

    CFDictionaryRef match_dictionary = CFDictionaryCreate(kCFAllocatorDefault,
                                                          keys,
                                                          values,
                                                          1,
                                                          NULL,
                                                          NULL);

    // CFMutableDictionaryRef match_dictionary = CFDictionaryCreateMutable(kCFAllocatorDefault,
    //                                                                     0,
    //                                                                     &kCFTypeDictionaryKeyCallBacks,
    //                                                                     &kCFTypeDictionaryValueCallBacks);
    // CFDictionarySetValue(match_dictionary,
    //                      CFSTR(kIOHIDDeviceUsagePageKey),
    //                      CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, filter));
    IOHIDManagerSetDeviceMatching(hidman, match_dictionary);
    IOHIDManagerRegisterDeviceMatchingCallback(hidman, vd_fw__mac_hid_device_added_callback, 0);
    IOHIDManagerRegisterDeviceRemovalCallback(hidman, vd_fw__mac_hid_device_removed_callback, 0);

    IOHIDManagerScheduleWithRunLoop(hidman, CFRunLoopGetCurrent(), kCFRunLoopDefaultMode);
    IOHIDManagerOpen(hidman, kIOHIDOptionsTypeNone);
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);

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

static void vd_fw__mac_hid_device_added_callback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device)
{
    CFTypeRef ref_usage = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDPrimaryUsageKey));
    if (!ref_usage) {
        return;
    }

    int usage = 0;
    CFNumberGetValue(ref_usage, kCFNumberIntType, (void*)&usage);

    if ((usage != kHIDUsage_GD_Joystick) &&
        (usage != kHIDUsage_GD_GamePad) &&
        (usage != kHIDUsage_GD_MultiAxisController))
    {
        return;
    }

    int new_device_index = VD_FW_G.winthread_num_gamepads++;
    VdFw__MacGamepadInfo *gamepad_info = &VD_FW_G.gamepad_infos[new_device_index];

    VdFwU32 product_id = 0;
    VdFwU32 vendor_id = 0;
    VdFwU32 version = 0;
    char product_name[128];

    {
        CFStringRef prop = (CFStringRef)IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductKey));
        if (prop) {
            CFStringGetCString(prop, product_name, sizeof(product_name), kCFStringEncodingUTF8);
        }
    }

    {
        CFTypeRef prop = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDVendorIDKey));
        if (prop) {
            CFNumberGetValue((CFNumberRef)prop, (CFNumberType)kCFNumberSInt32Type, (void*)&vendor_id);
        }
    }

    {
        CFTypeRef prop = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDProductIDKey));
        if (prop) {
            CFNumberGetValue((CFNumberRef)prop, (CFNumberType)kCFNumberSInt32Type, (void*)&product_id);
        }
    }

    {
        CFTypeRef prop = IOHIDDeviceGetProperty(device, CFSTR(kIOHIDVersionNumberKey));
        if (prop) {
            CFNumberGetValue((CFNumberRef)prop, (CFNumberType)kCFNumberSInt32Type, (void*)&version);
        }
    }

    VdFwGuid guid = vd_fw__make_gamepad_guid(0x03, (VdFwU16)vendor_id, (VdFwU16)product_id, (VdFwU16)version,
                                             NULL, NULL,
                                             0x00, 0x00);

    char guid_str[33] = {0};
    vd_fw_gamepad_guid_to_cstr(&guid, guid_str);

    gamepad_info->device = device;
    if (!vd_fw__map_gamepad(guid, &gamepad_info->map)) {
        vd_fw__def_gamepad(&gamepad_info->map);
    }

    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_GAMEPAD_CONNECTED;
    msg.dat.gamepad_connected.gamepad_index = new_device_index;
    vd_fw__msgbuf_w(&msg); 
    IOHIDDeviceRegisterInputValueCallback(device, vd_fw__mac_hid_value_callback, (void*)device);
}

static void vd_fw__mac_hid_device_removed_callback(void *context, IOReturn result, void *sender, IOHIDDeviceRef device)
{
    int disconnected_gamepad_index = -1;
    for (int i = 0; i < VD_FW_G.winthread_num_gamepads; ++i) {
        if (VD_FW_G.gamepad_infos[i].device == device) {
            disconnected_gamepad_index = i;
            break;
        }
    }

    if (disconnected_gamepad_index == -1) {
        return;
    }

    for (int i = disconnected_gamepad_index; i < (VD_FW_G.winthread_num_gamepads - 1); ++i) {
        VD_FW_G.gamepad_infos[i] = VD_FW_G.gamepad_infos[i + 1];
    }

    VD_FW_MEMSET(&VD_FW_G.gamepad_infos[VD_FW_G.winthread_num_gamepads - 1], 0, sizeof(VD_FW_G.gamepad_infos[0]));

    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_GAMEPAD_DISCONNECTED;
    msg.dat.gamepad_disconnected.gamepad_index = disconnected_gamepad_index;
    vd_fw__msgbuf_w(&msg); 
    VD_FW_G.winthread_num_gamepads--;
}

static void vd_fw__mac_hid_value_callback(void *context, IOReturn result, void *sender, IOHIDValueRef value)
{
    IOHIDDeviceRef device = (IOHIDDeviceRef)context;
    // @todo(mdodis): Find a way to deregister + re-register device callbacks upon device reassignment
    int gamepad_index = -1;
    for (int i = 0; i < VD_FW_G.winthread_num_gamepads; ++i) {
        if (VD_FW_G.gamepad_infos[i].device == device) {
            gamepad_index = i;
            break;
        }
    }

    if (gamepad_index == -1) {
        return;
    }

    IOHIDElementRef element = IOHIDValueGetElement(value);
    IOHIDDeviceRef device_from_element = IOHIDElementGetDevice(element);

    VdFwU32 usage_page = IOHIDElementGetUsagePage(element);
    VdFwU32 usage = IOHIDElementGetUsage(element);
    CFIndex int_value = IOHIDValueGetIntegerValue(value);
    float   float_value = (float)int_value;

    if (device_from_element != device) {
        return;
    }

    VdFwGamepadMappingSourceKind source_match = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON;

    // @todo(mdodis): Proper axis mapping (usages -> indices <-> targets)
    switch (usage_page) {
        case kHIDPage_GenericDesktop: {
            source_match = VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS;
            CFIndex logical_min = IOHIDElementGetLogicalMin(element);
            CFIndex logical_max = IOHIDElementGetLogicalMax(element);

            if (logical_min > int_value) {
                int_value = logical_min;
            }

            if (logical_max < int_value) {
                int_value = logical_max;
            }

            float_value = -1.f + ((int_value - logical_min) * 2.f) / ((float)(logical_max - logical_min));

        } break;

        default: break;
    }

    VdFw__MacGamepadInfo *gamepad_info = &VD_FW_G.gamepad_infos[gamepad_index];
    VdFwGamepadMapEntry *matched_entry = 0;

    int matched_entry_index = -1;
    for (int entry_index = 0;
             ((entry_index < VD_FW_GAMEPAD_MAX_MAPPINGS) && 
             !vd_fw_gamepad_map_entry_is_none(&gamepad_info->map.mappings[entry_index]));
         ++entry_index)
    {
        VdFwGamepadMapEntry *entry = &gamepad_info->map.mappings[entry_index];

        if ((entry->kind & VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_MASK) != source_match) {
            continue;
        }

        switch (source_match) {
            case VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON: {
                if (entry->index == (usage - 1)) {
                    matched_entry = entry;
                    matched_entry_index = entry_index;
                    break;
                }
            } break;

            case VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS: {

                if (entry->index == (usage - 0x30)) {
                    matched_entry = entry;
                    matched_entry_index = entry_index;
                    break;
                }
            } break;

            default: break;
        }

    }

    if (!matched_entry) {
        return;
    }

    VdFw__MacMessage msg;
    msg.type = VD_FW__MAC_MESSAGE_GAMEPAD_INPUT;
    msg.dat.gamepad_input.entry = *matched_entry;
    msg.dat.gamepad_input.gamepad_index = gamepad_index;
    msg.dat.gamepad_input.value = int_value;
    msg.dat.gamepad_input.float_value = float_value;
    vd_fw__msgbuf_w(&msg); 
}

#elif defined(__linux__)
#ifndef VD_FW_X11_MESSAGE_BUFFER_SIZE
#   define VD_FW_X11_MESSAGE_BUFFER_SIZE 256
#endif // !VD_FW_X11_MESSAGE_BUFFER_SIZE

#define GL_NO_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 0
#define GLX_GLXEXT_PROTOTYPES 0
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/extensions/syncconst.h>
#include <X11/extensions/XI.h>
#include <X11/extensions/Xrandr.h>
#include <time.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pthread.h>

enum {
    VD_FW_GLX_DRAWABLE_TYPE = 0x8010,
    VD_FW_GLX_X_RENDERABLE = 0x8012,
    VD_FW_GLX_WINDOW_BIT = 0x00000001,
    VD_FW_GLX_RENDER_TYPE = 0x8011,
    VD_FW_GLX_RGBA_BIT = 0x00000001,
    VD_FW_GLX_X_VISUAL_TYPE = 0x22,
    VD_FW_GLX_TRUE_COLOR = 0x8002,
    VD_FW_GLX_DOUBLEBUFFER = 5,
    VD_FW_GLX_RED_SIZE = 8,
    VD_FW_GLX_GREEN_SIZE = 9,
    VD_FW_GLX_BLUE_SIZE = 10,
    VD_FW_GLX_ALPHA_SIZE = 11,
    VD_FW_GLX_DEPTH_SIZE = 12,
    VD_FW_GLX_STENCIL_SIZE = 13,
    VD_FW_GLX_SAMPLE_BUFFERS = 0x186a0, /*100000*/
    VD_FW_GLX_SAMPLES = 0x186a1, /*100001*/
    VD_FW_GLX_CONTEXT_DEBUG_BIT_ARB = 0x00000001,
    VD_FW_GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB = 0x00000002,
    VD_FW_GLX_CONTEXT_MAJOR_VERSION_ARB = 0x2091,
    VD_FW_GLX_CONTEXT_MINOR_VERSION_ARB = 0x2092,
    VD_FW_GLX_CONTEXT_FLAGS_ARB = 0x2094,
    VD_FW_GLX_CONTEXT_CORE_PROFILE_BIT_ARB = 0x00000001,
    VD_FW_GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB = 0x00000002,
    VD_FW_GLX_CONTEXT_PROFILE_MASK_ARB = 0x9126,
    VD_FW_GLX_SWAP_INTERVAL_EXT = 0x20F1,
    VD_FW_XIAllDevices = 0,
    VD_FW_XIAllMasterDevices = 1,
    VD_FW_XI_RawMotion = 17,
    VD_FW_XI_RawButtonPress = 15,
    VD_FW_XI_RawButtonRelease = 16,

};

typedef struct __GlxContextInternal* __GlxContext;
typedef struct __GlxFbConfigInternal* __GlxFbConfig;
typedef __GlxContext (*VdFwProc__glXCreateContextAttribsARB)(Display *dpy, __GlxFbConfig config, __GlxContext share_context, Bool direct, const int *attrib_list);
typedef void (*VdFwProc__glXSwapIntervalEXT)(Display *dpy, XID drawable, int interval);

typedef struct {
    int             deviceid;       /**< Device id to select for        */
    int             mask_len;       /**< Length of mask in 4 byte units */
    unsigned char   *mask;
} VdFw__XIEventMask;

typedef struct {
    int           mask_len;
    unsigned char *mask;
    double        *values;
} VdFw__XIValuatorState;

typedef struct {
    int           type;         /* GenericEvent */
    unsigned long serial;       /* # of last request processed by server */
    Bool          send_event;   /* true if this came from a SendEvent request */
    Display       *display;     /* Display the event was read from */
    int           extension;    /* XI extension offset */
    int           evtype;       /* XI_RawKeyPress, XI_RawKeyRelease, etc. */
    Time          time;
    int           deviceid;
    int           sourceid;     /* Bug: Always 0. https://bugs.freedesktop.org//show_bug.cgi?id=34240 */
    int           detail;
    int           flags;
    VdFw__XIValuatorState valuators;
    double        *raw_values;
} VdFw__XIRawEvent;

typedef struct {
    unsigned long flags;
    unsigned long functions;
    unsigned long decorations;
    long input_mode;
    unsigned long status;
} VdFw__X11MotifWmHints;

#define VD_FW_X11_FUNCTIONS \
    XBEGIN_MODULE(xlib) \
    XSYM(xlib, Display*, XOpenDisplay, (const char *name)) \
    XSYM(xlib, int, XCloseDisplay, (Display*)) \
    XSYM(xlib, Status, XInitThreads, (void)) \
    XSYM(xlib, void, XLockDisplay, (Display*)) \
    XSYM(xlib, void, XUnlockDisplay, (Display*)) \
    XSYM(xlib, Status, XMatchVisualInfo, (Display *display, int screen, int depth, int klass, XVisualInfo *vinfo_return)) \
    XSYM(xlib, Colormap, XCreateColormap, (Display *display, Window w, Visual *visual, int alloc)) \
    XSYM(xlib, Window, XCreateWindow, (Display *display, Window parent, int x, int y, unsigned int width, unsigned int height, unsigned int border_width, int depth, unsigned int klass, Visual *visual, unsigned long valuemask, XSetWindowAttributes *attributes)) \
    XSYM(xlib, int, XMapWindow, (Display *display, Window w)) \
    XSYM(xlib, int, XUnmapWindow, (Display *display, Window w)) \
    XSYM(xlib, int, XFree, (void *data)) \
    XSYM(xlib, int, XStoreName, (Display *display, Window w, char *window_name)) \
    XSYM(xlib, int, XSync, (Display *display, Bool discard)) \
    XSYM(xlib, int, XFlush, (Display *display)) \
    XSYM(xlib, int, XPending, (Display* display)) \
    XSYM(xlib, int, XNextEvent, (Display* display, XEvent* evt)) \
    XSYM(xlib, int, XChangeWindowAttributes, (Display *display, Window w, unsigned long valuemask, XSetWindowAttributes *attributes)) \
    XSYM(xlib, Atom, XInternAtom, (Display *display, char *atom_name, Bool only_if_exists)) \
    XSYM(xlib, Status, XSetWMProtocols, (Display *display, Window w, Atom *protocols, int count)) \
    XSYM(xlib, int, XDestroyWindow, (Display *display, Window w)) \
    XSYM(xlib, int, XSetWindowColormap, (Display *display, Window w, Colormap colormap)) \
    XSYM(xlib, Bool, XQueryExtension, (Display *display, char *name, int *major_opcode_return, int *first_event_return, int *first_error_return)) \
    XSYM(xlib, int, XChangeProperty, (Display *display, Window w, Atom property, Atom type, int format, int mode, unsigned char *data, int nelements)) \
    XSYM(xlib, int, XSetClassHint, (Display *display, Window window, XClassHint *hint)) \
    XSYM(xlib, KeySym, XLookupKeysym, (XKeyEvent *key_event, int index))\
    XSYM(xlib, Window, XDefaultRootWindow, (Display *display)) \
    XSYM(xlib, int, XDefaultScreen, (Display *display)) \
    XSYM(xlib, void, XFreeEventData, (Display *display, XGenericEventCookie *cookie)) \
    XSYM(xlib, Bool, XGetEventData, (Display *dpy, XGenericEventCookie *cookie)) \
    XSYM(xlib, int, XWarpPointer, (Display *display, Window src_w, Window dest_w, int src_x, int src_y, unsigned int src_width, unsigned int src_height, int dest_x,  int dest_y)) \
    XSYM(xlib, int, XGrabPointer, (Display *display, Window grab_window, Bool owner_events, unsigned intevent_mask, int pointer_mode, int keyboard_mode, Window confine_to, Cursor cursor, Time time)) \
    XSYM(xlib, int, XUngrabPointer, (Display *display, Time time)) \
    XSYM(xlib, int, XGetWindowProperty, (Display *display, Window w, Atom property, long long_offset, long long_length, Bool del, Atom req_type, Atom *actual_type_return, int *actual_format_return, unsigned long *nitems_return, unsigned long *bytes_after_return, unsigned char **prop_return)) \
    XSYM(xlib, Status, XIconifyWindow, (Display *display, Window w, int screen_number)) \
    XSYM(xlib, Status, XSendEvent, (Display *display, Window w, Bool propagate, long event_mask, XEvent *event_send)) \
    XSYM(xlib, Status, XGetWindowAttributes, (Display *display, Window w, XWindowAttributes *attrs))\
    XSYM(xlib, int, XDefineCursor, (Display *display, Window w, XID cursor)) \
    XSYM(xlib, int, XUndefineCursor, (Display *display, Window w)) \
    XSYM(xlib, int, XResizeWindow, (Display *display, Window w, int width, int height)) \
    XSYM(xlib, void, XSetWMNormalHints, (Display *display, Window w, XSizeHints *hints)) \
    XSYM(xlib, void, XSetWMSizeHints, (Display *display, Window w, XSizeHints *hints, Atom property)) \
    XSYM(xlib, XIM, XOpenIM, (Display *display, struct _XrmHashBucketRec *rdb, char *res_name, char *res_class)) \
    XSYM(xlib, Status, XCloseIM, (XIM)) \
    XSYM(xlib, char*, XGetIMValues, (XIM im, ...)) \
    XSYM(xlib, XIC, XCreateIC, (XIM im, ...)) \
    XSYM(xlib, void, XDestroyIC, (XIC ic)) \
    XSYM(xlib, int, Xutf8LookupString, (XIC ic, XKeyPressedEvent *event, char *buffer_return, int bytes_buffer, KeySym *keysym_return, Status *status_return)) \
    XSYM(xlib, int, XSelectInput, (Display*, Window, long)) \
    XSYM(xlib, char*, XResourceManagerString, (Display*)) \
    XSYM(xlib, void, XrmInitialize, (void)) \
    XSYM(xlib, XrmDatabase, XrmGetStringDatabase, (const char*)) \
    XSYM(xlib, Bool, XrmGetResource, (XrmDatabase, const char*, const char*, char**, XrmValue*)) \
    XSYM(xlib, void, XrmDestroyDatabase, (XrmDatabase)) \
    XEND_MODULE() \
    XBEGIN_MODULE(xext) \
    XSYM(xext, Status, XSyncQueryExtension, (Display *display, int *event_base_return, int *error_base_return)) \
    XSYM(xext, Status, XSyncInitialize, (Display *dpy, int *major_version_return, int *minor_version_return)) \
    XSYM(xext, void, XSyncIntToValue, (XSyncValue *pv, int i)) \
    XSYM(xext, XSyncCounter, XSyncCreateCounter, (Display *dpy, XSyncValue initial_value)) \
    XSYM(xext, Status, XSyncSetCounter, (Display *dpy, XSyncCounter counter, XSyncValue value)) \
    XEND_MODULE() \
    XBEGIN_MODULE(xi) \
    XSYM(xi, Status, XIQueryVersion, (Display *display, int *major, int *minor)) \
    XSYM(xi, int, XISelectEvents, (Display *dpy, Window win, VdFw__XIEventMask *masks, int num_masks)) \
    XEND_MODULE() \
    XBEGIN_MODULE(xfixes) \
    XSYM(xfixes, void, XFixesHideCursor, (Display *display, Window w)) \
    XSYM(xfixes, void, XFixesShowCursor, (Display *display, Window w)) \
    XEND_MODULE() \
    XBEGIN_MODULE(xcursor) \
    XSYM(xcursor, XID, XcursorLibraryLoadCursor, (Display *dpy, const char *file)) \
    XSYM(xcursor, XID, XcursorShapeLoadCursor, (Display *dpy, unsigned int shape)) \
    XEND_MODULE() \
    XBEGIN_MODULE(xrandr) \
    XSYM(xrandr, Bool, XRRQueryExtension, (Display *dpy, int *event_base_return, int *error_base_return)) \
    XSYM(xrandr, void, XRRSelectInput, (Display *dpy, Window window, int mask)) \
    XEND_MODULE() \
    XBEGIN_MODULE(glx) \
    XSYM(glx, __GlxFbConfig*, glXChooseFBConfig, (Display *display, int screen, const int *attrib_list, int *nelements)) \
    XSYM(glx, XVisualInfo*, glXGetVisualFromFBConfig, (Display *display, __GlxFbConfig config)) \
    XSYM(glx, const char*, glXQueryExtensionsString, (Display *display, int screen)) \
    XSYM(glx, void*, glXGetProcAddress, (const GLubyte *procName))\
    XSYM(glx, Bool, glXMakeCurrent, (Display *display, XID drawable, __GlxContext ctx)) \
    XSYM(glx, void, glXDestroyContext, (Display *display, __GlxContext ctx)) \
    XSYM(glx, void, glXSwapBuffers, (Display *display, XID drawable)) \
    XSYM(glx, int,  glXQueryDrawable, (Display *display, XID draw, int attribute, unsigned int * value)) \
    XEND_MODULE() \

#define XBEGIN_MODULE(name)
#define XSYM(module, retval, name, args) typedef retval (*VdFw__Proc##name)args; static VdFw__Proc##name VdFw##name;
#define XEND_MODULE()

VD_FW_X11_FUNCTIONS

#undef XBEGIN_MODULE
#undef XSYM
#undef XEND_MODULE

#define VD_FW_XISetMask(ptr, event)   (((unsigned char*)(ptr))[(event)>>3] |=  (1 << ((event) & 7)))
#define VD_FW_XIClearMask(ptr, event) (((unsigned char*)(ptr))[(event)>>3] &= ~(1 << ((event) & 7)))
#define VD_FW_XIMaskIsSet(ptr, event) (((unsigned char*)(ptr))[(event)>>3] &   (1 << ((event) & 7)))
#define VD_FW_XIMaskLen(event)        (((event) >> 3) + 1)

enum {
    VD_FW_X11_FLAGS_WAKE_COND_VAR = 1 << 0,
    VD_FW_X11_FLAGS_SIZE_CHANGED  = 1 << 1,
};

typedef struct {
    int w, h;
    int flags;
} VdFw__X11Frame;

typedef struct {
    void                            *handle_xlib;
    int                             has_xlib;

    void                            *handle_xext;
    int                             has_xext;

    void                            *handle_xi;
    int                             has_xi;
    int                             xi_opcode;

    void                            *handle_xfixes;
    int                             has_xfixes;

    void                            *handle_xcursor;
    int                             has_xcursor;

    void                            *handle_xrandr;
    int                             has_xrandr;
    int                             event_base_xrandr;

    int                             xlib_supports_xsync;

    void                            *handle_glx;
    int                             has_glx;

    __GlxContext                    glx_context;
    VdFwProc__glXSwapIntervalEXT    glx_swap_interval_ext;

    int                             borderless;
    int                             caption_dragging;
    int                             caption_drag_mouse_start[2];
    int                             caption_drag_win_start[2];
    Display                         *display;
    Window                          root_window;
    Window                          window;
    XIMStyle                        input_style;
    XIC                             input_context;
    XIM                             input_method;
    int                             screen;
    Atom                            wm_delete_window;
    Atom                            wm_sync_request;
    Atom                            wm_sync_request_counter;
    Atom                            wm_protocols;
    Atom                            wm_motif;
    Atom                            wm_state;
    Atom                            wm_max_h;
    Atom                            wm_max_v;
    Atom                            wm_hidden;
    Atom                            wm_fullscreen;
    Atom                            wm_icon;
    Atom                            wm_usr_close;
    Atom                            wm_usr_block;
    Atom                            wm_xft_dpi;
    Atom                            wm_dpi_change;
    Atom                            wm_dpi_change_xsettings;
    XID                             sync_counter;
    VdFwU64                         sync_counter_value;
    int                             sync_redraw;
    int                             size_changed;
    int                             width, height;
    VdFwWindowState                 window_state;
    int                             window_state_changed;
    int                             is_fullscreen;
    int                             focus_changed;
    int                             window_min[2], window_max[2];
    int                             is_focused;

    int                             window_open;
    int                             close_request;
    int                             quit_request;
    VdFwGraphicsApi                 graphics_api;
    int                             block_while_sizing;
    int                             winthread_block_while_sizing;

    int                             ncrect_count;
    int                             ncrects[VD_FW_NCRECTS_MAX][4];
    int                             nccaption[4];
    int                             nccaption_set;
    XID                             cursor_left;
    XID                             cursor_right;
    XID                             cursor_top;
    XID                             cursor_bottom;
    XID                             cursor_tl;
    XID                             cursor_tr;
    XID                             cursor_bl;
    XID                             cursor_br;
    XID                             cursor_arrow;
    XID                             curr_cursor;

    int                             num_evts;
    VdFwEvent                       evtbuf[VD_FW_EVENT_COUNT_MAX];

    unsigned char                   curr_key_states[VD_FW_KEY_MAX];
    unsigned char                   prev_key_states[VD_FW_KEY_MAX];
    VdFwKey                         last_key;

    int                             prev_mouse_state;
    int                             mouse_state;
    int                             prev_mouse[2];
    int                             mouse[2];
    float                           wheel[2];
    int                             wheel_moved;
    int                             last_mouse_before_lock[2];
    float                           mouse_delta[2];
    int                             mouse_is_locked;
    float                           scale;
    int                             scale_changed;

    int                             has_initialized;
    int                             cap_gamepad_db_entries;
    int                             num_gamepad_db_entries;
    VdFwGamepadDBEntry              *gamepad_db_entries;
    struct timespec                 time_last;
    VdFwU64                         delta_ns;

    VdFwU16                         num_codepoints;
    VdFwU16                         first_codepoint_index;
    VdFwU32                         codepoints[VD_FW_CODEPOINT_BUFFER_COUNT];

    pthread_t                       win_thread;
    pthread_mutex_t                 mtx_paint;
    pthread_cond_t                  cnd_paint;
    volatile int                    t_running;

    VdFwEvent                       msgbuf[VD_FW_X11_MESSAGE_BUFFER_SIZE];
    int                             msgbuf_r;
    int                             msgbuf_w;
    VdFw__X11Frame                  next_frame;
    VdFw__X11Frame                  curr_frame;
} VdFw__LinuxInternalData;

VdFw__LinuxInternalData VdFw__Globals = {0};

#define VD_FW_G VdFw__Globals

static struct timespec vd_fw__linux_timespec_diff(struct timespec a, struct timespec b);
static int             vd_fw__x11_extension_supported(const char *extList, const char *extension);
static VdFwKey         vd_fw__x11_translate_keycode(XEvent *evt);
static int             vd_fw__x11_translate_mouse_button(unsigned int button);
static int             vd_fw__x11_recreate_window(Colormap colormap, int depth, Visual *visual);
static int             vd_fw__x11_test_orientation(int x, int y, int w, int h);
static void*           vd_fw__x11_thread_proc(void *arg);
static int             vd_fw__x11_msgbuf_r(VdFwEvent *message);
static int             vd_fw__x11_msgbuf_w(VdFwEvent *message);
static float           vd_fw__x11_xft_dpi(void);
static void            vd_fw__x11_thread_finish(void);

void *vd_fw__gl_get_proc_address(const char *name)
{
    void *result = VdFwglXGetProcAddress((const GLubyte*) name);
    return result;
}

VD_FW_API int vd_fw_init(VdFwInitInfo *info)
{
    VD_FW_G.graphics_api = VD_FW_GRAPHICS_API_INVALID;
    VD_FW_G.borderless = 0;
    VD_FW_G.window_max[0] = VD_FW_G.window_max[1] = 99999;

    if (info) {
        VD_FW_G.borderless = info->window_options.borderless;
    }

    {
        VD_FW_G.handle_xlib     = dlopen("libX11.so.6", RTLD_NOW | RTLD_GLOBAL);
        VD_FW_G.has_xlib        = VD_FW_G.handle_xlib != NULL;

        VD_FW_G.handle_xext     = dlopen("libXext.so", RTLD_NOW | RTLD_GLOBAL);
        VD_FW_G.has_xext        = VD_FW_G.handle_xext != NULL;

        VD_FW_G.handle_xfixes   = dlopen("libXfixes.so", RTLD_NOW | RTLD_GLOBAL);
        VD_FW_G.has_xfixes      = VD_FW_G.handle_xfixes != NULL;

        VD_FW_G.handle_xcursor   = dlopen("libXcursor.so", RTLD_NOW | RTLD_GLOBAL);
        VD_FW_G.has_xcursor      = VD_FW_G.handle_xcursor != NULL;

        VD_FW_G.handle_xrandr   = dlopen("libXrandr.so", RTLD_NOW | RTLD_GLOBAL);
        VD_FW_G.has_xrandr      = VD_FW_G.handle_xrandr != NULL;

        const char *xi_libs[] = {
            "libXi.so.6",
            "libXi.so",
        };
        for (unsigned i = 0; i < sizeof(xi_libs) / sizeof(xi_libs[0]); ++i) {
            void *dl = dlopen(xi_libs[i], RTLD_NOW | RTLD_GLOBAL);
            if (dl) {
                VD_FW_G.handle_xi   = dl;
                VD_FW_G.has_xi      = 1;
            }
        }

        const char *glx_libs[] = {
            "libGLX.so.0",
            "libGL.so.1",
            "libGL.so",
        };

        for (unsigned i = 0; i < sizeof(glx_libs) / sizeof(glx_libs[0]); ++i) {
            void *dl = dlopen(glx_libs[i], RTLD_NOW | RTLD_GLOBAL);
            if (dl) {
                VD_FW_G.handle_glx = dl;
                VD_FW_G.has_glx = 1;
                break;
            }
        }

#define XBEGIN_MODULE(name) if (VD_FW_G.has_##name) {
#define XSYM(module, retval, name, args) VdFw##name = (VdFw__Proc##name)dlsym(VD_FW_G.handle_##module, #name);
#define XEND_MODULE() }
        VD_FW_X11_FUNCTIONS
#undef XBEGIN_MODULE
#undef XSYM
#undef XEND_MODULE
    }
    VdFwGraphicsApi api = VD_FW_GRAPHICS_API_OPENGL;
    if (info) {
        api = info->api;
    }

    VdFwXInitThreads();

    VD_FW_G.display = VdFwXOpenDisplay(NULL);
    VD_FW_G.screen = VdFwXDefaultScreen(VD_FW_G.display);
    VD_FW_G.root_window = VdFwXDefaultRootWindow(VD_FW_G.display);

    if (VD_FW_G.has_xcursor) {
        VD_FW_G.cursor_left   = VdFwXcursorShapeLoadCursor(VD_FW_G.display, 108);
        VD_FW_G.cursor_right  = VdFwXcursorShapeLoadCursor(VD_FW_G.display, 108);
        VD_FW_G.cursor_top    = VdFwXcursorShapeLoadCursor(VD_FW_G.display, 116);
        VD_FW_G.cursor_bottom = VdFwXcursorShapeLoadCursor(VD_FW_G.display, 116);
        VD_FW_G.cursor_tl     = VdFwXcursorShapeLoadCursor(VD_FW_G.display, 134);
        VD_FW_G.cursor_tr     = VdFwXcursorShapeLoadCursor(VD_FW_G.display, 136);
        VD_FW_G.cursor_bl     = VdFwXcursorShapeLoadCursor(VD_FW_G.display, 12);
        VD_FW_G.cursor_br     = VdFwXcursorShapeLoadCursor(VD_FW_G.display, 14);
        VD_FW_G.cursor_arrow  = VdFwXcursorShapeLoadCursor(VD_FW_G.display, 68);
    }

    // Sync Extension
    {
        int major_opcode, first_event, first_error;
        if (VdFwXQueryExtension(VD_FW_G.display, (char*)"SYNC", &major_opcode, &first_event, &first_error)) {
            int version = 0;
            {
                int major, minor;
                VdFwXSyncInitialize(VD_FW_G.display, &major, &minor);
                version = major * 1000 + minor;
            }

            if (version >= 3000) {
                VD_FW_G.xlib_supports_xsync = 1;
            }
        }
    }

    // XInput 2 Extension
    {

        int major_opcode, first_event, first_error;
        if (!VdFwXQueryExtension(VD_FW_G.display, (char*)"XInputExtension", &major_opcode, &first_event, &first_error)) {
            VD_FW_G.has_xi = 0;
        }

        int major, minor;
        major = 2;
        minor = 4;
        if (VdFwXIQueryVersion(VD_FW_G.display, &major, &minor) != Success) {
            VD_FW_G.has_xi = 0;
        }

        VD_FW_G.xi_opcode = major_opcode;
    }

    // Xrandr Extension
    {
        int event_base_return, error_base_return;
        if (!VdFwXRRQueryExtension(VD_FW_G.display, &event_base_return, &error_base_return)) {
            VD_FW_G.has_xrandr = 0;
        }
        VD_FW_G.event_base_xrandr = event_base_return;
    }

    // Character Input
    {
        XIM input_method = VdFwXOpenIM(VD_FW_G.display, 0, 0, 0);
        if (input_method) {
            XIMStyles *styles = 0;
            if ((VdFwXGetIMValues(input_method, XNQueryInputStyle, &styles, NULL) == 0) && styles) {

                XIMStyle best_match_style = 0;
                for (int i = 0; i < styles->count_styles; ++i) {

                    XIMStyle style = styles->supported_styles[i];
                    if (style == (XIMPreeditNothing | XIMStatusNothing)) {
                        best_match_style = style;
                        break;
                    }
                }

                VdFwXFree(styles);
                VD_FW_G.input_style = best_match_style;
                VD_FW_G.input_method = input_method;
            }
        }
    }

    VD_FW_G.wm_motif = VdFwXInternAtom(VD_FW_G.display, (char*)"_MOTIF_WM_HINTS", 0);
    VD_FW_G.wm_protocols = VdFwXInternAtom(VD_FW_G.display, (char*)"WM_PROTOCOLS", 0);
    VD_FW_G.wm_delete_window = VdFwXInternAtom(VD_FW_G.display, (char*)"WM_DELETE_WINDOW", 0);
    VD_FW_G.wm_state = VdFwXInternAtom(VD_FW_G.display, (char*)"_NET_WM_STATE", 0);
    VD_FW_G.wm_max_h = VdFwXInternAtom(VD_FW_G.display, (char*)"_NET_WM_STATE_MAXIMIZED_HORZ", 0);
    VD_FW_G.wm_max_v = VdFwXInternAtom(VD_FW_G.display, (char*)"_NET_WM_STATE_MAXIMIZED_VERT", 0);
    VD_FW_G.wm_hidden = VdFwXInternAtom(VD_FW_G.display, (char*)"_NET_WM_STATE_HIDDEN", 0);
    VD_FW_G.wm_fullscreen = VdFwXInternAtom(VD_FW_G.display, (char*)"_NET_WM_STATE_FULLSCREEN", 0);
    VD_FW_G.wm_icon = VdFwXInternAtom(VD_FW_G.display, (char*)"_NET_WM_ICON", False);

    if (VD_FW_G.xlib_supports_xsync) {
        VD_FW_G.wm_sync_request = VdFwXInternAtom(VD_FW_G.display, (char*)"_NET_WM_SYNC_REQUEST", 0);
        VD_FW_G.wm_sync_request_counter = VdFwXInternAtom(VD_FW_G.display, (char*)"_NET_WM_SYNC_REQUEST_COUNTER", 0);
    }

    VD_FW_G.wm_usr_close = VdFwXInternAtom(VD_FW_G.display, (char*)"WM_USR_CLOSE", 0);
    VD_FW_G.wm_usr_block = VdFwXInternAtom(VD_FW_G.display, (char*)"WM_USR_BLOCK", 0);
    VD_FW_G.wm_xft_dpi   = VdFwXInternAtom(VD_FW_G.display, (char*)"Xft.dpi", 0);
    VD_FW_G.wm_dpi_change = VdFwXInternAtom(VD_FW_G.display, (char*)"RESOURCE_MANAGER", 0);
    VD_FW_G.wm_dpi_change_xsettings = VdFwXInternAtom(VD_FW_G.display, (char*)"_XSETTINGS_S0", 0);
    VD_FW_G.scale = vd_fw__x11_xft_dpi();

    int screen_bits = 24;
    XVisualInfo visual_info = {};
    if (!VdFwXMatchVisualInfo(VD_FW_G.display, VD_FW_G.screen, screen_bits, TrueColor, &visual_info)) {
        return 0;
    }

    VdFwOpenGLOptions *gl_options = 0;
    if (info) {
        gl_options = &info->gl;
    }

    if (!vd_fw_set_graphics_api(api, gl_options)) {
        return 0;
    }

    VD_FW_G.has_initialized = 1;
    return 1;
}

VD_FW_API unsigned long long vd_fw_delta_ns(void)
{
    return VD_FW_G.delta_ns;
}

VD_FW_API int vd_fw_set_graphics_api(VdFwGraphicsApi api, VdFwOpenGLOptions *gl_options)
{
    int result = 1;

    if (VD_FW_G.has_initialized) {
        vd_fw__x11_thread_finish();
    }

    Colormap window_colormap;
    XVisualInfo window_visual_info;
    int window_depth;
    Visual *window_visual;

    switch (api) {
        case VD_FW_GRAPHICS_API_OPENGL: {

            VdFwGlConfig      default_configs[2];
            VD_FW_MEMSET(default_configs, 0, sizeof(default_configs));

            default_configs[0].version = VD_FW_GL_VERSION_3_3;

            VdFwOpenGLOptions default_options = {0};
            default_options.configs = default_configs;

            if (!gl_options || (gl_options->configs == 0) || (gl_options->configs[0].version == VD_FW_GL_VERSION_BASIC)) {
                gl_options = &default_options;
            }

            const char *glx_exts = VdFwglXQueryExtensionsString(VD_FW_G.display, VD_FW_G.screen);
            VdFwProc__glXCreateContextAttribsARB glXCreateContextAttribsARB = (VdFwProc__glXCreateContextAttribsARB)
                VdFwglXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");
            VD_FW_G.glx_swap_interval_ext = (VdFwProc__glXSwapIntervalEXT)VdFwglXGetProcAddress((const GLubyte*)"glXSwapIntervalEXT");

            int use_old_style_context = 0;
            if (!vd_fw__x11_extension_supported(glx_exts, "GLX_ARB_create_context") || !glXCreateContextAttribsARB) {
                use_old_style_context = 1;
                printf("Old style context used\n");
            }

            int index = 0;
            while (gl_options->configs && gl_options->configs[index].version != 0) {
                int minor        = gl_options->configs[index].version % 10;
                int major        = gl_options->configs[index].version / 10;
                int debug        = gl_options->configs[index].debug;
                int compat       = gl_options->configs[index].compat;
                int pixel_format = gl_options->configs[index].pixel_format;
                int depth_format = gl_options->configs[index].depth_format;
                int msaa         = gl_options->configs[index].msaa;

                int running_attrib = 0;
                GLint pixel_attribs[32] = {0};
                pixel_attribs[running_attrib++] = VD_FW_GLX_X_RENDERABLE;
                pixel_attribs[running_attrib++] = 1;

                pixel_attribs[running_attrib++] = VD_FW_GLX_DRAWABLE_TYPE;
                pixel_attribs[running_attrib++] = VD_FW_GLX_WINDOW_BIT;

                pixel_attribs[running_attrib++] = VD_FW_GLX_RENDER_TYPE;
                pixel_attribs[running_attrib++] = VD_FW_GLX_RGBA_BIT;

                pixel_attribs[running_attrib++] = VD_FW_GLX_X_VISUAL_TYPE;
                pixel_attribs[running_attrib++] = VD_FW_GLX_TRUE_COLOR;

                pixel_attribs[running_attrib++] = VD_FW_GLX_DOUBLEBUFFER;
                pixel_attribs[running_attrib++] = 1;

                switch (pixel_format) {
                    case VD_FW_GL_PIXEL_FORMAT_R8G8B8A8: {
                        pixel_attribs[running_attrib++] = VD_FW_GLX_RED_SIZE;
                        pixel_attribs[running_attrib++] = 8;

                        pixel_attribs[running_attrib++] = VD_FW_GLX_GREEN_SIZE;
                        pixel_attribs[running_attrib++] = 8;

                        pixel_attribs[running_attrib++] = VD_FW_GLX_BLUE_SIZE;
                        pixel_attribs[running_attrib++] = 8;

                        pixel_attribs[running_attrib++] = VD_FW_GLX_ALPHA_SIZE;
                        pixel_attribs[running_attrib++] = 8;
                    } break;

                    case VD_FW_GL_PIXEL_FORMAT_R8G8B8: {
                        pixel_attribs[running_attrib++] = VD_FW_GLX_RED_SIZE;
                        pixel_attribs[running_attrib++] = 8;

                        pixel_attribs[running_attrib++] = VD_FW_GLX_GREEN_SIZE;
                        pixel_attribs[running_attrib++] = 8;

                        pixel_attribs[running_attrib++] = VD_FW_GLX_BLUE_SIZE;
                        pixel_attribs[running_attrib++] = 8;
                    } break;

                    default: break;
                }

                switch (depth_format) {
                    case VD_FW_GL_DEPTH_FORMAT_D32: {
                        pixel_attribs[running_attrib++] = VD_FW_GLX_DEPTH_SIZE;
                        pixel_attribs[running_attrib++] = 32;
                    } break;

                    case VD_FW_GL_DEPTH_FORMAT_D24S8: {
                        pixel_attribs[running_attrib++] = VD_FW_GLX_DEPTH_SIZE;
                        pixel_attribs[running_attrib++] = 24;

                        pixel_attribs[running_attrib++] = VD_FW_GLX_STENCIL_SIZE;
                        pixel_attribs[running_attrib++] = 8;
                    } break;

                    default: break;
                }

                switch (msaa) {
                    case VD_FW_GL_MSAA_ENABLED_2X: {
                        pixel_attribs[running_attrib++] = VD_FW_GLX_SAMPLES;
                        pixel_attribs[running_attrib++] = 2;
                    } break;

                    case VD_FW_GL_MSAA_ENABLED_4X: {
                        pixel_attribs[running_attrib++] = VD_FW_GLX_SAMPLES;
                        pixel_attribs[running_attrib++] = 4;
                    } break;

                    case VD_FW_GL_MSAA_ENABLED_8X: {
                        pixel_attribs[running_attrib++] = VD_FW_GLX_SAMPLES;
                        pixel_attribs[running_attrib++] = 8;
                    } break;

                    default: break;
                }

                int nelements;
                __GlxFbConfig *fbs = VdFwglXChooseFBConfig(VD_FW_G.display, VD_FW_G.screen, pixel_attribs, &nelements);
                __GlxFbConfig fb_cfg;
                XVisualInfo *vi_info;
                int context_attribs[9];
                int ctx_flags = 0;
                int prf_flags = 0;

                if (!fbs || nelements < 1) {
                    VdFwXFree(fbs);
                    goto LOOP_END;
                }

                fb_cfg = fbs[0];
                VdFwXFree(fbs);

                vi_info = VdFwglXGetVisualFromFBConfig(VD_FW_G.display, fb_cfg);
                window_visual_info = *vi_info;
                window_depth = vi_info->depth;
                window_visual = vi_info->visual;
                window_colormap = VdFwXCreateColormap(VD_FW_G.display, VD_FW_G.root_window, vi_info->visual, AllocNone);

                VdFwXSync(VD_FW_G.display, False);

                if (debug) {
                    ctx_flags |= VD_FW_GLX_CONTEXT_DEBUG_BIT_ARB;
                }

                if (compat) {
                    prf_flags |= VD_FW_GLX_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;
                } else {
                    prf_flags |= VD_FW_GLX_CONTEXT_CORE_PROFILE_BIT_ARB;
                }

                context_attribs[0] = VD_FW_GLX_CONTEXT_MAJOR_VERSION_ARB;
                context_attribs[1] = major;

                context_attribs[2] = VD_FW_GLX_CONTEXT_MINOR_VERSION_ARB;
                context_attribs[3] = minor;

                context_attribs[4] = VD_FW_GLX_CONTEXT_PROFILE_MASK_ARB;
                context_attribs[5] = prf_flags;

                context_attribs[6] = VD_FW_GLX_CONTEXT_FLAGS_ARB;
                context_attribs[7] = ctx_flags;

                context_attribs[8] = 0;

                VD_FW_G.glx_context = glXCreateContextAttribsARB(VD_FW_G.display, fb_cfg, 0, 1, context_attribs);
                if (!VD_FW_G.glx_context) {
                    goto LOOP_END;
                }

                if (vd_fw__load_opengl(&gl_options->configs[index])) {
                    break;
                } else {
                    VdFwglXDestroyContext(VD_FW_G.display, VD_FW_G.glx_context);
                    result = 0;
                    goto LOOP_END;
                }

                break;
LOOP_END:
                index++;
            }

            if (!gl_options->configs[index].version) {
                result = 0;
                break;
            }

            gl_options->selected_config = index;

        } break;

        case VD_FW_GRAPHICS_API_CUSTOM: {

            window_depth = 0;
            window_visual = DefaultVisual(VD_FW_G.display, VD_FW_G.screen);
            window_colormap = DefaultColormap(VD_FW_G.display, VD_FW_G.screen);
        } break;

        default: break;
    }

    if (result) {
        VD_FW_G.graphics_api = api;

        // @note(mdodis): We create the window after a graphics api is set
        // This is done because we need a Visual and a compatible colormap from glx first
        vd_fw__x11_recreate_window(window_colormap, window_depth, window_visual);

        if (api == VD_FW_GRAPHICS_API_OPENGL) {
            VdFwglXMakeCurrent(VD_FW_G.display, VD_FW_G.window, VD_FW_G.glx_context);
        }

        VdFwXSync(VD_FW_G.display, 0);

        pthread_mutex_init(&VD_FW_G.mtx_paint, NULL);
        pthread_cond_init(&VD_FW_G.cnd_paint, NULL);
        pthread_create(&VD_FW_G.win_thread, NULL, vd_fw__x11_thread_proc, NULL);
    }

    return result;
}

VD_FW_API int vd_fw_running(void)
{
    return VD_FW_G.window_open;
}

VD_FW_API VdFwEvent *vd_fw_poll(int *count)
{
    VD_FW_G.scale_changed = 0;
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
    VD_FW_G.mouse_delta[0] = VD_FW_G.mouse_delta[1] = 0.f;

    for (int i = 0; i < VD_FW_KEY_MAX; ++i) {
        VD_FW_G.prev_key_states[i] = VD_FW_G.curr_key_states[i];
    }

    VD_FW_G.num_evts = 0;

    VdFwEvent mm;
    while (vd_fw__x11_msgbuf_r(&mm) && (VD_FW_G.num_evts < VD_FW_EVENT_COUNT_MAX)) {
        VD_FW_G.evtbuf[VD_FW_G.num_evts++] = mm;

        switch (mm.type) {
            case VD_FW_EVENT_TYPE_CHARACTER: {
                VD_FW_G.codepoints[(num_codepoints++) % VD_FW_CODEPOINT_BUFFER_COUNT] = mm.data.character.codepoint;
            } break;

            case VD_FW_EVENT_TYPE_CLOSE_REQUEST: {
                VD_FW_G.close_request = 1;
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_MOVE: {
                VD_FW_G.mouse[0] = mm.data.mouse_move.x;
                VD_FW_G.mouse[1] = mm.data.mouse_move.y;
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_DELTA: {
                VD_FW_G.mouse_delta[0] = VD_FW_G.mouse_delta[0] * 0.8f + mm.data.mouse_delta.dx * 0.2f;
                VD_FW_G.mouse_delta[1] = VD_FW_G.mouse_delta[1] * 0.8f + mm.data.mouse_delta.dy * 0.2f;
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_SCROLL: {
                VD_FW_G.wheel[0] += mm.data.mouse_scroll.dx;
                VD_FW_G.wheel[1] += mm.data.mouse_scroll.dy;
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_BUTTON_DOWN: {
                VD_FW_G.mouse_state |= mm.data.mouse_button_down.button;
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP: {
                VD_FW_G.mouse_state &= ~mm.data.mouse_button_up.button;
            } break;

            case VD_FW_EVENT_TYPE_FOCUS_CHANGE: {
                VD_FW_G.focus_changed = 1;
                VD_FW_G.is_focused = mm.data.focus_change.got_focus;
            } break;

            case VD_FW_EVENT_TYPE_KEY_UP: {
                VD_FW_G.curr_key_states[mm.data.key_up.key] = 0;
            } break;

            case VD_FW_EVENT_TYPE_KEY_DOWN: {
                VD_FW_G.curr_key_states[mm.data.key_down.key] = 1;
            } break;

            case VD_FW_EVENT_TYPE_WINDOW_STATE_CHANGE: {
                VdFwWindowState prev_state = VD_FW_G.window_state;
                VdFwWindowState change_flag = (VdFwWindowState)mm.data.window_state_change.flag;
                if (mm.data.window_state_change.value) {
                    VD_FW_G.window_state |= change_flag;
                } else {
                    VD_FW_G.window_state &= ~change_flag;
                }

                if (prev_state != VD_FW_G.window_state) {
                    VD_FW_G.window_state_changed |= change_flag;
                }

            } break;

            case VD_FW_EVENT_TYPE_SCALE_CHANGE: {
                VD_FW_G.scale_changed = 1;
                VD_FW_G.scale = mm.data.scale_change.new_scale;
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

    if (count) {
        *count = VD_FW_G.num_evts;
    }


    if (VD_FW_G.mouse_is_locked) {
        VdFwXWarpPointer(VD_FW_G.display, None, VD_FW_G.window,
                         0, 0, 0, 0,
                         VD_FW_G.last_mouse_before_lock[0], VD_FW_G.last_mouse_before_lock[1]);
    }

    struct timespec now;
    struct timespec delta_timespec;
    clock_gettime(CLOCK_MONOTONIC, &now);
    delta_timespec = vd_fw__linux_timespec_diff(now, VD_FW_G.time_last);
    VD_FW_G.time_last = now;
    VD_FW_G.delta_ns = delta_timespec.tv_nsec;

    return VD_FW_G.evtbuf;
}

VD_FW_API int vd_fw_get_key_down(int key)
{
    return VD_FW_G.curr_key_states[key];
}

VD_FW_API int vd_fw_get_key_pressed(int key)
{
    return !VD_FW_G.prev_key_states[key] && VD_FW_G.curr_key_states[key];
}

VD_FW_API int vd_fw_get_last_key_pressed(void)
{
    return VD_FW_G.last_key;
}

VD_FW_API int vd_fw_get_key_released(int key)
{
    return VD_FW_G.prev_key_states[key] && !VD_FW_G.curr_key_states[key];
}

VD_FW_API int vd_fw_close_requested(void)
{
    return VD_FW_G.close_request;
}

VD_FW_API void vd_fw_quit(void)
{
    VdFwXLockDisplay(VD_FW_G.display);
    XEvent ev = {0};
    ev.xclient.type = ClientMessage;
    ev.xclient.display = VD_FW_G.display;
    ev.xclient.window = VD_FW_G.window;
    ev.xclient.message_type = VD_FW_G.wm_usr_close;
    ev.xclient.format = 32;
    VdFwXSendEvent(VD_FW_G.display, VD_FW_G.window, False, NoEventMask, &ev);
    VdFwXUnlockDisplay(VD_FW_G.display);
    VD_FW_G.window_open = 0;
}

VD_FW_API void vd_fw_exit(void)
{
    if (VD_FW_G.has_initialized) {
        vd_fw__x11_thread_finish();

        VD_FW_G.has_initialized = 0;
    }

    VdFwXCloseDisplay(VD_FW_G.display);

    if (VD_FW_G.input_method) {
        VdFwXCloseIM(VD_FW_G.input_method);
    }

    dlclose(VD_FW_G.handle_xlib);
    dlclose(VD_FW_G.handle_xext);
    dlclose(VD_FW_G.handle_xfixes);
    dlclose(VD_FW_G.handle_xcursor);
    dlclose(VD_FW_G.handle_xrandr);
    dlclose(VD_FW_G.handle_xi);
    dlclose(VD_FW_G.handle_glx);
}

VD_FW_API void vd_fw_lock(void)
{
    pthread_mutex_lock(&VD_FW_G.mtx_paint);
    VD_FW_G.curr_frame = VD_FW_G.next_frame;
    VD_FW_G.next_frame.flags = 0;
    pthread_mutex_unlock(&VD_FW_G.mtx_paint);
}

VD_FW_API void vd_fw_unlock(void)
{
    if (VD_FW_G.window_open) {
        if (VD_FW_G.graphics_api == VD_FW_GRAPHICS_API_OPENGL) {
            VdFwglXSwapBuffers(VD_FW_G.display, VD_FW_G.window);
        }

        if (VD_FW_G.graphics_api == VD_FW_GRAPHICS_API_OPENGL) {
            if (glFenceSync && glClientWaitSync && glDeleteSync) {
                GLsync fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
                if (fence) {
                    glClientWaitSync(fence, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000ULL);
                    glDeleteSync(fence);
                }
            }
        }
    }

    if (VD_FW_G.curr_frame.flags & VD_FW_X11_FLAGS_WAKE_COND_VAR) {
        pthread_cond_signal(&VD_FW_G.cnd_paint);
    }
}

VD_FW_API int vd_fw_get_block_while_sizing(void)
{
    return VD_FW_G.block_while_sizing;
}

VD_FW_API void vd_fw_set_block_while_sizing(int on)
{
    if (VD_FW_G.block_while_sizing == on) {
        return;
    }

    VD_FW_G.block_while_sizing = on;

    XEvent ev = {0};
    ev.xclient.type = ClientMessage;
    ev.xclient.display = VD_FW_G.display;
    ev.xclient.window = VD_FW_G.window;
    ev.xclient.message_type = VD_FW_G.wm_usr_block;
    ev.xclient.format = 32;
    ev.xclient.data.b[0] = on ? 1 : 0;
    VdFwXSendEvent(VD_FW_G.display, VD_FW_G.window, False, NoEventMask, &ev);
}

VD_FW_API int vd_fw_set_vsync_on(int on)
{
    if (VD_FW_G.glx_swap_interval_ext) {
        VD_FW_G.glx_swap_interval_ext(VD_FW_G.display, VD_FW_G.window, on);

        unsigned int value;
        VdFwglXQueryDrawable(VD_FW_G.display, VD_FW_G.window, VD_FW_GLX_SWAP_INTERVAL_EXT, &value);
        return on == (int)value;
    }

    return 0;
}

VD_FW_API int vd_fw_get_gamepad_count(void)
{
    return 0;
}

VD_FW_API VdFwU64 vd_fw_get_gamepad_button_state(int index)
{
    return 0;
}

VD_FW_API int vd_fw_get_gamepad_down(int index, int button)
{
    return 0;
}

VD_FW_API int vd_fw_get_gamepad_pressed(int index, int button)
{
    return 0;
}

VD_FW_API int vd_fw_get_gamepad_axis(int index, int axis, float *out)
{
    return 0;
}

VD_FW_API void vd_fw_set_app_icon(void *pixels, int width, int height)
{
    int count = 2 + width * height;
    unsigned long *data = (unsigned long*)vd_fw__realloc_mem((void*)0, count * sizeof(unsigned long));

    data[0] = width;
    data[1] = height;

    for (int i = 0; i < width * height; i++) {
        data[2 + i] = ((VdFwU32*)pixels)[i];
    }

    VdFwXChangeProperty(VD_FW_G.display, VD_FW_G.window,
                        VD_FW_G.wm_icon,
                        XA_CARDINAL,
                        32,
                        PropModeReplace,
                        (unsigned char*)data,
                        count);

    VD_FW_FREE(data, count * sizeof(unsigned long));
}

VD_FW_API int vd_fw_get_size(int *w, int *h)
{
    if (w) *w = VD_FW_G.curr_frame.w;
    if (h) *h = VD_FW_G.curr_frame.h;
    return VD_FW_G.curr_frame.flags & VD_FW_X11_FLAGS_SIZE_CHANGED;
}

VD_FW_API void vd_fw_set_size(int w, int h)
{
    VdFwXResizeWindow(VD_FW_G.display, VD_FW_G.window, w, h);    
}

VD_FW_API void vd_fw_set_size_min(int w, int h)
{
    if (w != 0) {
        VD_FW_G.window_min[0] = w;
    } else {
        VD_FW_G.window_min[0] = 0;
    }

    if (h != 0) {
        VD_FW_G.window_min[1] = h;
    } else {
        VD_FW_G.window_min[1] = 0;
    }

    XSizeHints hints = {};
    hints.flags |= PMinSize;
    hints.min_width = VD_FW_G.window_min[0];
    hints.min_height = VD_FW_G.window_min[1];
    hints.flags |= PMaxSize;
    hints.max_width = VD_FW_G.window_max[0];
    hints.max_height = VD_FW_G.window_max[1];

    VdFwXSetWMNormalHints(VD_FW_G.display, VD_FW_G.window, &hints);
}

VD_FW_API void vd_fw_set_size_max(int w, int h)
{
    if (w != 0) {
        VD_FW_G.window_max[0] = w;
    } else {
        VD_FW_G.window_max[0] = 999999;
    }

    if (h != 0) {
        VD_FW_G.window_max[1] = h;
    } else {
        VD_FW_G.window_max[1] = 999999;
    }

    XSizeHints hints = {};
    hints.flags |= PMinSize;
    hints.min_width = VD_FW_G.window_min[0];
    hints.min_height = VD_FW_G.window_min[1];
    hints.flags |= PMaxSize;
    hints.max_width = VD_FW_G.window_max[0];
    hints.max_height = VD_FW_G.window_max[1];

    VdFwXSetWMNormalHints(VD_FW_G.display, VD_FW_G.window, &hints);
}

VD_FW_API int vd_fw_get_focused(int *focused)
{
    if (focused) {
        *focused = VD_FW_G.is_focused;
    }

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
}

VD_FW_API int vd_fw_get_minimized(int *minimized)
{
    if (minimized) {
        *minimized = VD_FW_G.window_state & VD_FW_WINDOW_STATE_MINIMIZED;
    }
    return VD_FW_G.window_state_changed & VD_FW_WINDOW_STATE_MINIMIZED;
}

VD_FW_API void vd_fw_set_minimized(void)
{
    VdFwXIconifyWindow(VD_FW_G.display, VD_FW_G.window, VdFwXDefaultScreen(VD_FW_G.display));
}

VD_FW_API int vd_fw_get_maximized(int *maximized)
{
    if (maximized) {
        *maximized = VD_FW_G.window_state & VD_FW_WINDOW_STATE_MAXIMIZED;
    }
    return VD_FW_G.window_state_changed & VD_FW_WINDOW_STATE_MAXIMIZED;
}

VD_FW_API void vd_fw_set_maximized(void)
{
    XEvent e = {0};
    e.xclient.type = ClientMessage;
    e.xclient.window = VD_FW_G.window;
    e.xclient.message_type = VD_FW_G.wm_state;
    e.xclient.format = 32;
    e.xclient.data.l[0] = 1;
    e.xclient.data.l[1] = VD_FW_G.wm_max_h;
    e.xclient.data.l[2] = VD_FW_G.wm_max_v;
    e.xclient.data.l[3] = 1;

    VdFwXSendEvent(VD_FW_G.display,
                   VdFwXDefaultRootWindow(VD_FW_G.display),
                   False,
                   SubstructureRedirectMask | SubstructureNotifyMask,
                   &e);
}

VD_FW_API void vd_fw_normalize(void)
{
    XEvent e = {0};
    e.xclient.type = ClientMessage;
    e.xclient.window = VD_FW_G.window;
    e.xclient.message_type = VD_FW_G.wm_state;
    e.xclient.format = 32;
    e.xclient.data.l[0] = 0;
    e.xclient.data.l[1] = VD_FW_G.wm_max_h;
    e.xclient.data.l[2] = VD_FW_G.wm_max_v;
    e.xclient.data.l[3] = 1;

    VdFwXSendEvent(VD_FW_G.display,
                   VdFwXDefaultRootWindow(VD_FW_G.display),
                   False,
                   SubstructureRedirectMask | SubstructureNotifyMask,
                   &e);
}

VD_FW_API void vd_fw_set_fullscreen(int on)
{
    if (VD_FW_G.is_fullscreen == on) {
        return;
    }

    VD_FW_G.is_fullscreen = on;

    XEvent e;
    memset(&e, 0, sizeof(e));

    e.xclient.type = ClientMessage;
    e.xclient.window = VD_FW_G.window;
    e.xclient.message_type = VD_FW_G.wm_state;
    e.xclient.format = 32;

    e.xclient.data.l[1] = VD_FW_G.wm_fullscreen;
    e.xclient.data.l[2] = 0;
    e.xclient.data.l[3] = 1;
    e.xclient.data.l[4] = 0;

    if (on) {
        e.xclient.data.l[0] = 1;
    } else {
        e.xclient.data.l[0] = 0;
    }

    VdFwXSendEvent(VD_FW_G.display,
                   VdFwXDefaultRootWindow(VD_FW_G.display),
                   False,
                   SubstructureRedirectMask | SubstructureNotifyMask,
                   &e);
}

VD_FW_API int vd_fw_get_fullscreen(void)
{
    return VD_FW_G.is_fullscreen;
}

VD_FW_API int vd_fw_get_mouse_state(int *x, int *y)
{
    int result = VD_FW_G.mouse_state;
    if (x) *x = VD_FW_G.mouse[0];
    if (y) *y = VD_FW_G.mouse[1];
    return result;
}

VD_FW_API void vd_fw_get_mouse_delta(float *dx, float *dy)
{
    if (dx) *dx = VD_FW_G.mouse_delta[0];
    if (dy) *dy = VD_FW_G.mouse_delta[1];
}

VD_FW_API void vd_fw_set_mouse_locked(int locked)
{
    if (locked == VD_FW_G.mouse_is_locked) {
        return;
    }

    VD_FW_G.mouse_is_locked = locked;

    if (locked) {
        VdFwXGrabPointer(VD_FW_G.display, VD_FW_G.window, True, PointerMotionMask, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
        VD_FW_G.last_mouse_before_lock[0] = VD_FW_G.mouse[0];
        VD_FW_G.last_mouse_before_lock[1] = VD_FW_G.mouse[1];
        VdFwXFixesHideCursor(VD_FW_G.display, VD_FW_G.window);
    } else {
        VdFwXUngrabPointer(VD_FW_G.display, CurrentTime);
        VdFwXFixesShowCursor(VD_FW_G.display, VD_FW_G.window);
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

VD_FW_API int vd_fw_get_mouse_clicked(int button)
{
    return !(VD_FW_G.prev_mouse_state & button) && (VD_FW_G.mouse_state & button);
}

VD_FW_API int vd_fw_get_mouse_released(int button)
{
    return (VD_FW_G.prev_mouse_state & button) && !(VD_FW_G.mouse_state & button);
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

        unsigned long long file_secs = st.st_mtime;

        if (file_secs > *check_against) {
            *check_against = file_secs;
            close(fd);
            result = 1;
            break;
        }

    }

    return result;
}

VD_FW_API VdFwPlatform vd_fw_get_platform(void)
{
    return VD_FW_PLATFORM_LINUX;
}

VD_FW_API int vd_fw_get_scale(float *scale)
{
    if (scale) *scale = VD_FW_G.scale;
    return VD_FW_G.scale_changed;
}

VD_FW_API void vd_fw_set_title(const char *title)
{
    VdFwXStoreName(VD_FW_G.display, VD_FW_G.window, (char*)title);
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

    char *result = (char*)VD_FW_REALLOC(0, 0, size +1);
    fread(result, size, 1, f);

    result[size] = 0;
    return result;
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

static struct timespec vd_fw__linux_timespec_diff(struct timespec a, struct timespec b)
{
    struct timespec result;
    VdFwI64 nsec_diff = a.tv_nsec - b.tv_nsec;
    VdFwI64 sec_diff  = a.tv_sec  - b.tv_sec;
    if (nsec_diff < 0) {
        result.tv_sec = sec_diff - 1;
        result.tv_nsec = 1000000000 + nsec_diff;
    } else {
        result.tv_nsec = nsec_diff;
        result.tv_sec = sec_diff;
    }

    return result;
}

static int vd_fw__x11_extension_supported(const char *extList, const char *extension)
{
    const char *start;
    const char *where, *terminator;
    
    where = strchr(extension, ' ');
    if (where || *extension == '\0')
        return 0;
    
    for (start=extList;;) {
        where = strstr(start, extension);
        
        if (!where)
            break;
        
        terminator = where + strlen(extension);
        
        if ( where == start || *(where - 1) == ' ' )
            if ( *terminator == ' ' || *terminator == '\0' )
            return 1;
        
        start = terminator;
    }
    
    return 0;
}

static VdFwKey vd_fw__x11_translate_keycode(XEvent *evt)
{
    KeySym keysym = VdFwXLookupKeysym((XKeyEvent*)evt, 0);
    switch (keysym) {
        default: return VD_FW_KEY_UNKNOWN; break;

        case XK_F1:             return VD_FW_KEY_F1;
        case XK_F2:             return VD_FW_KEY_F2;
        case XK_F3:             return VD_FW_KEY_F3;
        case XK_F4:             return VD_FW_KEY_F4;
        case XK_F5:             return VD_FW_KEY_F5;
        case XK_F6:             return VD_FW_KEY_F6;
        case XK_F7:             return VD_FW_KEY_F7;
        case XK_F8:             return VD_FW_KEY_F8;
        case XK_F9:             return VD_FW_KEY_F9;
        case XK_F10:            return VD_FW_KEY_F10;
        case XK_F11:            return VD_FW_KEY_F11;
        case XK_F12:            return VD_FW_KEY_F12;
        case XK_F13:            return VD_FW_KEY_F13;
        case XK_F14:            return VD_FW_KEY_F14;
        case XK_F15:            return VD_FW_KEY_F15;
        case XK_F16:            return VD_FW_KEY_F16;
        case XK_F17:            return VD_FW_KEY_F17;
        case XK_F18:            return VD_FW_KEY_F18;
        case XK_F19:            return VD_FW_KEY_F19;
        case XK_F20:            return VD_FW_KEY_F20;
        case XK_F21:            return VD_FW_KEY_F21;
        case XK_F22:            return VD_FW_KEY_F22;
        case XK_F23:            return VD_FW_KEY_F23;
        case XK_F24:            return VD_FW_KEY_F24;
        case XK_BackSpace:      return VD_FW_KEY_BACKSPACE;
        case XK_Insert:         return VD_FW_KEY_INS;
        case XK_Home:           return VD_FW_KEY_HOME;
        case XK_Page_Up:        return VD_FW_KEY_PGUP;
        case XK_Delete:         return VD_FW_KEY_DEL;
        case XK_End:            return VD_FW_KEY_END;
        case XK_Page_Down:      return VD_FW_KEY_PGDN;
        case XK_space:          return VD_FW_KEY_SPACE;
        case XK_Control_L:      return VD_FW_KEY_LCONTROL;
        case XK_Control_R:      return VD_FW_KEY_RCONTROL;
        case XK_Alt_L:          return VD_FW_KEY_LALT;
        case XK_Alt_R:          return VD_FW_KEY_RALT;
        case XK_Shift_L:        return VD_FW_KEY_LSHIFT;
        case XK_Shift_R:        return VD_FW_KEY_RSHIFT;
        case XK_apostrophe:     return VD_FW_KEY_QUOTE;
        case XK_Up:             return VD_FW_KEY_ARROW_UP;
        case XK_Left:           return VD_FW_KEY_ARROW_LEFT;
        case XK_Down:           return VD_FW_KEY_ARROW_DOWN;
        case XK_Right:          return VD_FW_KEY_ARROW_RIGHT;
        case XK_comma:          return VD_FW_KEY_COMMA;
        case XK_minus:          return VD_FW_KEY_MINUS;
        case XK_period:         return VD_FW_KEY_DOT;
        case XK_slash:          return VD_FW_KEY_SLASH_FORWARD;
        case XK_0:              return VD_FW_KEY_0;
        case XK_1:              return VD_FW_KEY_1;
        case XK_2:              return VD_FW_KEY_2;
        case XK_3:              return VD_FW_KEY_3;
        case XK_4:              return VD_FW_KEY_4;
        case XK_5:              return VD_FW_KEY_5;
        case XK_6:              return VD_FW_KEY_6;
        case XK_7:              return VD_FW_KEY_7;
        case XK_8:              return VD_FW_KEY_8;
        case XK_9:              return VD_FW_KEY_9;
        case XK_Return:         return VD_FW_KEY_ENTER;
        case XK_semicolon:      return VD_FW_KEY_SEMICOLON;
        case XK_Tab:            return VD_FW_KEY_TAB;
        case XK_equal:          return VD_FW_KEY_EQUALS;
        case XK_Caps_Lock:      return VD_FW_KEY_CAPITAL;
        case XK_Escape:         return VD_FW_KEY_ESCAPE;
        case XK_a:              return VD_FW_KEY_A;
        case XK_b:              return VD_FW_KEY_B;
        case XK_c:              return VD_FW_KEY_C;
        case XK_d:              return VD_FW_KEY_D;
        case XK_e:              return VD_FW_KEY_E;
        case XK_f:              return VD_FW_KEY_F;
        case XK_g:              return VD_FW_KEY_G;
        case XK_h:              return VD_FW_KEY_H;
        case XK_i:              return VD_FW_KEY_I;
        case XK_j:              return VD_FW_KEY_J;
        case XK_k:              return VD_FW_KEY_K;
        case XK_l:              return VD_FW_KEY_L;
        case XK_m:              return VD_FW_KEY_M;
        case XK_n:              return VD_FW_KEY_N;
        case XK_o:              return VD_FW_KEY_O;
        case XK_p:              return VD_FW_KEY_P;
        case XK_q:              return VD_FW_KEY_Q;
        case XK_r:              return VD_FW_KEY_R;
        case XK_s:              return VD_FW_KEY_S;
        case XK_t:              return VD_FW_KEY_T;
        case XK_u:              return VD_FW_KEY_U;
        case XK_v:              return VD_FW_KEY_V;
        case XK_w:              return VD_FW_KEY_W;
        case XK_x:              return VD_FW_KEY_X;
        case XK_y:              return VD_FW_KEY_Y;
        case XK_z:              return VD_FW_KEY_Z;
        case XK_bracketleft:    return VD_FW_KEY_BRACKET_OPEN;
        case XK_backslash:      return VD_FW_KEY_SLASH_BACK;
        case XK_bracketright:   return VD_FW_KEY_BRACKET_CLOSE;
        case XK_asciitilde:     return VD_FW_KEY_BACKTICK;
        case XK_KP_0:           return VD_FW_KEY_0;
        case XK_KP_1:           return VD_FW_KEY_1;
        case XK_KP_2:           return VD_FW_KEY_2;
        case XK_KP_3:           return VD_FW_KEY_3;
        case XK_KP_4:           return VD_FW_KEY_4;
        case XK_KP_5:           return VD_FW_KEY_5;
        case XK_KP_6:           return VD_FW_KEY_6;
        case XK_KP_7:           return VD_FW_KEY_7;
        case XK_KP_8:           return VD_FW_KEY_8;
        case XK_KP_9:           return VD_FW_KEY_9;
    }
}

static int vd_fw__x11_translate_mouse_button(unsigned int button)
{
    switch (button) {
        case Button1: return VD_FW_MOUSE_BUTTON_LEFT;
        case Button2: return VD_FW_MOUSE_BUTTON_MIDDLE;
        case Button3: return VD_FW_MOUSE_BUTTON_RIGHT;
        case 8: return VD_FW_MOUSE_BUTTON_M1;
        case 9: return VD_FW_MOUSE_BUTTON_M2;
        default: return 0;
    }    
}

static int vd_fw__x11_recreate_window(Colormap colormap, int depth, Visual* visual)
{
    XSetWindowAttributes window_attributes = {0};
    // window_attributes.bit_gravity = ForgetGravity;
    window_attributes.bit_gravity = StaticGravity;
    window_attributes.background_pixel = 0;
    window_attributes.colormap = colormap;
    window_attributes.event_mask = StructureNotifyMask | ExposureMask | FocusChangeMask |
                                   KeyPressMask | KeyReleaseMask |
                                   ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
                                   PropertyChangeMask;


    unsigned long attribute_mask = CWBitGravity | CWBackPixel | CWColormap | CWEventMask;

    int width = 640;
    int height = 480;
    VD_FW_G.window = VdFwXCreateWindow(VD_FW_G.display, VD_FW_G.root_window,
                                       0, 0,
                                       width, height, 0,
                                       depth, InputOutput,
                                       visual, attribute_mask, &window_attributes);

    XClassHint class_hint = {(char*)"fw_window", (char*)"popup"};
    VdFwXSetClassHint(VD_FW_G.display, VD_FW_G.window, &class_hint);

    VdFwXSelectInput(VD_FW_G.display, VD_FW_G.root_window, PropertyChangeMask);

    // if (VD_FW_G.has_xrandr) {
    //     VdFwXRRSelectInput(VD_FW_G.display, VD_FW_G.root_window,
    //                        RRScreenChangeNotifyMask | RROutputChangeNotifyMask | RRCrtcChangeNotifyMask);
    // }

    if (!VD_FW_G.window) {
        return 0;
    }

    VdFwXStoreName(VD_FW_G.display, VD_FW_G.window, (char*)"FW Window");

    if (VD_FW_G.borderless) {
        VdFw__X11MotifWmHints hints = {0};
        hints.flags = 2;
        hints.decorations = 0;
        VdFwXChangeProperty(VD_FW_G.display, VD_FW_G.window,
                            VD_FW_G.wm_motif,
                            VD_FW_G.wm_motif, 32,
                            PropModeReplace,
                            (unsigned char*) &hints,
                            sizeof(hints) / sizeof(long));
    }

    VD_FW_G.window_open = 1;

    if (VD_FW_G.xlib_supports_xsync) {
        VdFwXSetWMProtocols(VD_FW_G.display, VD_FW_G.window, &VD_FW_G.wm_sync_request, 1);

        XSyncValue initial_value;
        VdFwXSyncIntToValue(&initial_value, 0);
        VD_FW_G.sync_counter = VdFwXSyncCreateCounter(VD_FW_G.display, initial_value);

        VdFwXChangeProperty(VD_FW_G.display, VD_FW_G.window, VD_FW_G.wm_sync_request_counter, XA_CARDINAL, 32, PropModeReplace, (uint8_t*)&VD_FW_G.sync_counter, 1);
    }

    VdFwXSetWMProtocols(VD_FW_G.display, VD_FW_G.window, &VD_FW_G.wm_delete_window, 1);

    VdFwXMapWindow(VD_FW_G.display, VD_FW_G.window);
    VdFwXSync(VD_FW_G.display, False);

    // XInput2
    {
        unsigned char mask[4] = { 0 };
        VD_FW_XISetMask(mask, VD_FW_XI_RawMotion);
        VdFw__XIEventMask em = {0};
        em.deviceid = VD_FW_XIAllMasterDevices;
        em.mask_len = sizeof(mask);
        em.mask = mask;
        VdFwXISelectEvents(VD_FW_G.display, DefaultRootWindow(VD_FW_G.display), &em, 1);
    }

    // Character Input
    {
        if (VD_FW_G.input_method && VD_FW_G.input_style) {
            VD_FW_G.input_context = VdFwXCreateIC(VD_FW_G.input_method, XNInputStyle, VD_FW_G.input_style,
                                                  XNClientWindow, VD_FW_G.window,
                                                  XNFocusWindow, VD_FW_G.window,
                                                  NULL);
        }
    }

    return 1;
}

static int vd_fw__x11_test_orientation(int x, int y, int w, int h)
{
    const int BORDER = 6;
    int left   = x < BORDER;
    int right  = x > w - BORDER;
    int top    = y < BORDER;
    int bottom = y > h - BORDER;

    int result = 8;
    // 0---1---2
    // |       |
    // 7   8   3
    // |       |
    // 6---5---4
    //

    if (top && left)         result = 0;
    else if (top && right)   result = 2;
    else if (bottom && left) result = 6;
    else if (bottom && right)result = 4;
    else if (left)           result = 7;
    else if (right)          result = 3;
    else if (top)            result = 1;
    else if (bottom)         result = 5;

    return result;
}

static int vd_fw__x11_msgbuf_r(VdFwEvent *message)
{
    int r = VD_FW_G.msgbuf_r;
    int w;
    __atomic_load(&VD_FW_G.msgbuf_w, &w, __ATOMIC_SEQ_CST);

    if (r == w) {
        return 0;
    }

    *message = VD_FW_G.msgbuf[r];

    int nr = (r + 1) % VD_FW_X11_MESSAGE_BUFFER_SIZE;
    __atomic_exchange_n(&VD_FW_G.msgbuf_r, nr, __ATOMIC_SEQ_CST);

    return 1;
}

static int vd_fw__x11_msgbuf_w(VdFwEvent *message)
{
    int w = VD_FW_G.msgbuf_w;
    int r;
    __atomic_load(&VD_FW_G.msgbuf_r, &r, __ATOMIC_SEQ_CST);

    if ((w + 1) % VD_FW_X11_MESSAGE_BUFFER_SIZE == r) {
        return 0;
    }

    VD_FW_G.msgbuf[w] = *message;
    int nw = (w + 1) % VD_FW_X11_MESSAGE_BUFFER_SIZE;
    __atomic_exchange_n(&VD_FW_G.msgbuf_w, nw, __ATOMIC_SEQ_CST);

    return 1;
}

static float vd_fw__x11_xft_dpi(void)
{
#if 1
    char *resource_str = VdFwXResourceManagerString(VD_FW_G.display);
    XrmDatabase db;
    XrmValue value;
    char *type = NULL;
    float dpi = 96.f;

    VdFwXrmInitialize();


    if (resource_str) {
        db = VdFwXrmGetStringDatabase(resource_str);
        if (VdFwXrmGetResource(db, "Xft.dpi", "String", &type, &value) == True) {
            if (value.addr) {
                dpi = atof(value.addr);
                printf("Found xft %f\n", dpi);
            }
        }
        VdFwXrmDestroyDatabase(db);
    }

    return dpi / 96.f;
#else

    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char *prop = NULL;

    Atom atom = VD_FW_G.wm_xft_dpi;
    if (atom == None) {
        return 1.f;
    }

    if (VdFwXGetWindowProperty(
            VD_FW_G.display,
            VD_FW_G.root_window,
            atom,
            0, 1024,
            False,
            AnyPropertyType,
            &actual_type,
            &actual_format,
            &nitems,
            &bytes_after,
            &prop) != Success)
    {
        return 1.f;
    }

    if (!prop) {
        return 1.f;
    }

    float dpi = atof((char*)prop);

    VdFwXFree(prop);
    return dpi;
#endif
}

static void vd_fw__x11_thread_finish(void)
{
    XEvent ev = {0};
    ev.xclient.type = ClientMessage;
    ev.xclient.display = VD_FW_G.display;
    ev.xclient.window = VD_FW_G.window;
    ev.xclient.message_type = VD_FW_G.wm_usr_close;
    ev.xclient.format = 32;
    VdFwXLockDisplay(VD_FW_G.display);
    VdFwXSendEvent(VD_FW_G.display, VD_FW_G.window, False, NoEventMask, &ev);
    VdFwXUnlockDisplay(VD_FW_G.display);

    pthread_mutex_lock(&VD_FW_G.mtx_paint);
    VD_FW_G.curr_frame = VD_FW_G.next_frame;
    VD_FW_G.next_frame.flags = 0;
    VD_FW_G.sync_redraw = 0;
    VD_FW_G.t_running = 0;
    pthread_cond_broadcast(&VD_FW_G.cnd_paint);
    pthread_mutex_unlock(&VD_FW_G.mtx_paint);

    pthread_join(VD_FW_G.win_thread, NULL);
    pthread_cond_destroy(&VD_FW_G.cnd_paint);
    pthread_mutex_destroy(&VD_FW_G.mtx_paint);

    if (VD_FW_G.graphics_api == VD_FW_GRAPHICS_API_OPENGL) {
        // Destroy OpenGL Context
        VdFwglXMakeCurrent(VD_FW_G.display, 0, NULL);
        VdFwglXDestroyContext(VD_FW_G.display, VD_FW_G.glx_context);
    }

    VdFwXDestroyWindow(VD_FW_G.display, VD_FW_G.window);
    VdFwXFlush(VD_FW_G.display);
}

static void *vd_fw__x11_thread_proc(void *arg)
{
    (void)arg;
    int w = 0;
    int h = 0;

    XWindowAttributes attr;
    VdFwXGetWindowAttributes(VD_FW_G.display, VD_FW_G.window, &attr);
    w = attr.width;
    h = attr.height;


    struct timespec resize_start;
    int resizing = 0;

    XEvent evt = {};
    // while ((VdFwXPending(VD_FW_G.display)) && (VD_FW_G.num_evts < VD_FW_EVENT_COUNT_MAX)) {
    VD_FW_G.t_running = 1;
    while (VD_FW_G.t_running) {
        // if (!VdFwXPending(VD_FW_G.display)) {
        //     continue;
        // }

        VdFwXNextEvent(VD_FW_G.display, &evt);

        switch (evt.type) {
            case DestroyNotify: {
                // VD_FW_G.window_open = 0;
            } break;

            case Expose: {
                // if (VD_FW_G.xlib_supports_xsync) {
                //     VD_FW_G.sync_redraw = 1;
                // }
            } break;

            case PropertyNotify: {

                if (evt.xproperty.atom == VD_FW_G.wm_state) {

                    Atom actual_type;
                    int actual_format;
                    unsigned long nitems, bytes_after;
                    Atom *states = NULL; 
                    if (VdFwXGetWindowProperty(VD_FW_G.display, VD_FW_G.window,
                                               VD_FW_G.wm_state,
                                               0, (~0L), False, XA_ATOM,
                                               &actual_type, &actual_format, &nitems, &bytes_after,
                                               (unsigned char**)&states) == Success)
                    {
                        int is_hidden = 0;
                        int is_max_h  = 0;
                        int is_max_v  = 0;

                        for (unsigned long i = 0; i < nitems; i++) {
                            if (states[i] == VD_FW_G.wm_hidden) {
                                is_hidden = 1;
                            }

                            if (states[i] == VD_FW_G.wm_max_h) {
                                is_max_h = 1;
                            }

                            if (states[i] == VD_FW_G.wm_max_v) {
                                is_max_v = 1;
                            }
                        }

                        int is_maximized = is_max_h && is_max_v;
                        int was_minimized = VD_FW_G.window_state & VD_FW_WINDOW_STATE_MINIMIZED ? 1 : 0;
                        int was_maximized = VD_FW_G.window_state & VD_FW_WINDOW_STATE_MAXIMIZED ? 1 : 0;

                        if (was_minimized != is_hidden) {
                            VD_FW_G.window_state_changed |= VD_FW_WINDOW_STATE_MINIMIZED;

                            if (is_hidden) {
                                VD_FW_G.window_state |= VD_FW_WINDOW_STATE_MINIMIZED;
                            } else {
                                VD_FW_G.window_state &= ~VD_FW_WINDOW_STATE_MINIMIZED;
                            }

                            VdFwEvent fw_event;
                            fw_event.type = VD_FW_EVENT_TYPE_WINDOW_STATE_CHANGE;
                            fw_event.data.window_state_change.flag = VD_FW_WINDOW_STATE_MINIMIZED;
                            fw_event.data.window_state_change.value = is_hidden;
                            vd_fw__x11_msgbuf_w(&fw_event);
                        }

                        if (was_maximized != is_maximized) {
                            if (is_maximized) {
                                VD_FW_G.window_state |= VD_FW_WINDOW_STATE_MAXIMIZED;
                            } else {
                                VD_FW_G.window_state &= ~VD_FW_WINDOW_STATE_MAXIMIZED;
                            }

                            VdFwEvent fw_event;
                            fw_event.type = VD_FW_EVENT_TYPE_WINDOW_STATE_CHANGE;
                            fw_event.data.window_state_change.flag = VD_FW_WINDOW_STATE_MAXIMIZED;
                            fw_event.data.window_state_change.value = is_maximized;
                            vd_fw__x11_msgbuf_w(&fw_event);
                        }
                    }
                } else if ((evt.xproperty.atom == VD_FW_G.wm_dpi_change) || (evt.xproperty.atom == VD_FW_G.wm_dpi_change_xsettings)) {
                    VdFwEvent fw_event;
                    fw_event.type = VD_FW_EVENT_TYPE_SCALE_CHANGE;
                    fw_event.data.scale_change.new_scale = vd_fw__x11_xft_dpi();
                    vd_fw__x11_msgbuf_w(&fw_event);
                }

            } break;

            case FocusIn: {
                VdFwEvent fw_event;
                fw_event.type = VD_FW_EVENT_TYPE_FOCUS_CHANGE;
                fw_event.data.focus_change.got_focus = 1;
                vd_fw__x11_msgbuf_w(&fw_event);
            } break;

            case FocusOut: {
                VdFwEvent fw_event;
                fw_event.type = VD_FW_EVENT_TYPE_FOCUS_CHANGE;
                fw_event.data.focus_change.got_focus = 0;
                vd_fw__x11_msgbuf_w(&fw_event);
            } break;

            case ClientMessage: {
                XClientMessageEvent *e = &evt.xclient;

                if(e->message_type == VD_FW_G.wm_protocols) {
                    if (e->data.l[0] == (long)VD_FW_G.wm_delete_window) {

                        VdFwEvent fw_event;
                        fw_event.type = VD_FW_EVENT_TYPE_CLOSE_REQUEST;
                        vd_fw__x11_msgbuf_w(&fw_event);
                    } else if (e->data.l[0] == (long)VD_FW_G.wm_sync_request) {
                        VD_FW_G.sync_counter_value = 0;
                        VD_FW_G.sync_counter_value |= e->data.l[2];
                        VD_FW_G.sync_counter_value |= e->data.l[3] << 32;
                        VD_FW_G.sync_redraw = 1;
                    }
                } else if (e->message_type == VD_FW_G.wm_usr_close) {
                    VD_FW_G.t_running = 0;
                } else if (e->message_type == VD_FW_G.wm_usr_block) {
                    VD_FW_G.winthread_block_while_sizing = e->data.b[0];
                }

            } break;

            case KeyPress: {
                XKeyPressedEvent *key_event = &evt.xkey;

                VdFwEvent fw_event;
                VD_FW_MEMSET(&fw_event, 0, sizeof(fw_event));
                fw_event.type = VD_FW_EVENT_TYPE_KEY_DOWN;
                fw_event.data.key_down.key = vd_fw__x11_translate_keycode(&evt);
                fw_event.data.key_down.modifiers = 0;

                if (key_event->state & ControlMask) {
                    fw_event.data.key_down.modifiers |= VD_FW_MOD_CONTROL;
                }

                if (key_event->state & ShiftMask) {
                    fw_event.data.key_down.modifiers |= VD_FW_MOD_SHIFT;
                }

                if (key_event->state & Mod1Mask) {
                    fw_event.data.key_down.modifiers |= VD_FW_MOD_ALT;
                }

                vd_fw__x11_msgbuf_w(&fw_event);

                // Character Input
                if (VD_FW_G.input_method && VD_FW_G.input_style) {
                    char buf[5] = {0};
                    Status status = 0;
                    VdFwXutf8LookupString(VD_FW_G.input_context, key_event, buf, 4, 0, &status);

                    if (status == XLookupChars) {
                        int len = 0;
                        char *b = buf;
                        while (*b) {
                            len++;
                            b++;
                        }

                        VdFwU32 codepoint = 0;
                        int l = vd_fw__utf8_to_utf32((unsigned char*)buf, len, &codepoint);

                        if (l != -1) {
                            fw_event.type = VD_FW_EVENT_TYPE_CHARACTER;
                            fw_event.data.character.codepoint = codepoint;
                            vd_fw__x11_msgbuf_w(&fw_event);
                        }
                    }
                }
            } break;

            case KeyRelease: {
                VdFwEvent fw_event;
                VD_FW_MEMSET(&fw_event, 0, sizeof(fw_event));
                fw_event.type = VD_FW_EVENT_TYPE_KEY_UP;
                fw_event.data.key_up.key = vd_fw__x11_translate_keycode(&evt);
                
                vd_fw__x11_msgbuf_w(&fw_event);
            } break;

            case MotionNotify: {
                float delta[2] = {0.f, 0.f};
                if (!VD_FW_G.has_xi) {
                    delta[0] = (evt.xmotion.x - VD_FW_G.prev_mouse[0]);
                    delta[1] = (evt.xmotion.y - VD_FW_G.prev_mouse[1]);
                }

                VD_FW_G.prev_mouse[0] = evt.xmotion.x;
                VD_FW_G.prev_mouse[1] = evt.xmotion.y;

                {
                    VdFwEvent fw_event;
                    VD_FW_MEMSET(&fw_event, 0, sizeof(fw_event));
                    fw_event.type = VD_FW_EVENT_TYPE_MOUSE_MOVE;
                    fw_event.data.mouse_move.x = evt.xmotion.x;
                    fw_event.data.mouse_move.y = evt.xmotion.y;
                    vd_fw__x11_msgbuf_w(&fw_event);
                }

                if (!VD_FW_G.has_xi) {
                    VdFwEvent fw_event;
                    VD_FW_MEMSET(&fw_event, 0, sizeof(fw_event));
                    fw_event.type = VD_FW_EVENT_TYPE_MOUSE_DELTA;
                    fw_event.data.mouse_delta.dx = delta[0];
                    fw_event.data.mouse_delta.dy = delta[1];
                    vd_fw__x11_msgbuf_w(&fw_event);
                }

                if (VD_FW_G.borderless) {
                    int x = evt.xmotion.x;
                    int y = evt.xmotion.y;

                    int orientation = vd_fw__x11_test_orientation(x, y, w, h);
                    XID c = VD_FW_G.cursor_arrow;
                    switch (orientation) {
                        case 0: c = VD_FW_G.cursor_tl; break;
                        case 1: c = VD_FW_G.cursor_top; break;
                        case 2: c = VD_FW_G.cursor_tr; break;
                        case 3: c = VD_FW_G.cursor_right; break;
                        case 4: c = VD_FW_G.cursor_br; break;
                        case 5: c = VD_FW_G.cursor_bottom; break;
                        case 6: c = VD_FW_G.cursor_bl; break;
                        case 7: c = VD_FW_G.cursor_left; break;
                        case 8: c = VD_FW_G.cursor_arrow; break;
                        default: break;
                    }

                    if (c != VD_FW_G.curr_cursor) {
                        if (c != VD_FW_G.cursor_arrow) {
                            VdFwXDefineCursor(VD_FW_G.display, VD_FW_G.window, c);
                        } else {
                            VdFwXUndefineCursor(VD_FW_G.display, VD_FW_G.window);
                        }
                    }
                }
            } break;

            case GenericEvent: {
                VdFwXGetEventData(VD_FW_G.display, &evt.xcookie);
                if (VD_FW_G.has_xi && (evt.xcookie.evtype == VD_FW_XI_RawMotion)) {
                    VdFw__XIRawEvent *raw = (VdFw__XIRawEvent*)evt.xcookie.data;
                    if (raw->valuators.mask_len == 0) {
                        VdFwXFreeEventData(VD_FW_G.display, &evt.xcookie);
                        break;
                    }

                    float dx = 0.f;
                    float dy = 0.f;

                    if (VD_FW_XIMaskIsSet(raw->valuators.mask, 0) != 0) {
                        dx += raw->raw_values[0];
                    }

                    if (VD_FW_XIMaskIsSet(raw->valuators.mask, 1) != 0) {
                        dy += raw->raw_values[1];
                    }

                    VdFwEvent fw_event;
                    VD_FW_MEMSET(&fw_event, 0, sizeof(fw_event));
                    fw_event.type = VD_FW_EVENT_TYPE_MOUSE_DELTA;
                    fw_event.data.mouse_delta.dx = dx;
                    fw_event.data.mouse_delta.dy = dy;
                    vd_fw__x11_msgbuf_w(&fw_event);
                }

                VdFwXFreeEventData(VD_FW_G.display, &evt.xcookie);

            } break;

            case ButtonPress: {

                int handled = 1;
                int btn = 0;
                {
                    const float wheel_delta = 1.f;

                    VdFwEvent fw_event;
                    VD_FW_MEMSET(&fw_event, 0, sizeof(fw_event));
                    fw_event.type = VD_FW_EVENT_TYPE_MOUSE_SCROLL;
                    fw_event.data.mouse_scroll.dx = 0.f;
                    fw_event.data.mouse_scroll.dy = 0.f;

                    switch (evt.xbutton.button) {
                        case 4: fw_event.data.mouse_scroll.dy += wheel_delta; break;
                        case 5: fw_event.data.mouse_scroll.dy -= wheel_delta; break;
                        case 6: fw_event.data.mouse_scroll.dx += wheel_delta; break;
                        case 7: fw_event.data.mouse_scroll.dx -= wheel_delta; break;
                        default: handled = 0; break;
                    }

                    if (handled) {
                        vd_fw__x11_msgbuf_w(&fw_event);
                    }
                }

                if (!handled) {
                    btn = vd_fw__x11_translate_mouse_button(evt.xbutton.button);
                }

                if (btn) {
                    VdFwEvent fw_event;
                    VD_FW_MEMSET(&fw_event, 0, sizeof(fw_event));
                    fw_event.type = VD_FW_EVENT_TYPE_MOUSE_BUTTON_DOWN;
                    fw_event.data.mouse_button_down.button = btn;
                    vd_fw__x11_msgbuf_w(&fw_event);

                    if (VD_FW_G.borderless) {
                        if (btn == VD_FW_MOUSE_BUTTON_LEFT) {

                            int mouse_x = evt.xbutton.x;
                            int mouse_y = evt.xbutton.y;

                            int orientation = vd_fw__x11_test_orientation(mouse_x, mouse_y, w, h);
                            if (orientation != 8) {
                                int move_resize_place = orientation;

                                // !!! FIXME: we need to regrab this if necessary when the drag is done.
                                VdFwXUngrabPointer(VD_FW_G.display, 0L);
                                VdFwXFlush(VD_FW_G.display);

                                XEvent ev = {0};
                                ev.xclient.type = ClientMessage;
                                ev.xclient.window = VD_FW_G.window;
                                ev.xclient.message_type = 
                                        VdFwXInternAtom(VD_FW_G.display, (char*)"_NET_WM_MOVERESIZE", False);
                                ev.xclient.format = 32;
                                ev.xclient.data.l[0] = evt.xbutton.x_root;
                                ev.xclient.data.l[1] = evt.xbutton.y_root;
                                ev.xclient.data.l[2] = move_resize_place;
                                ev.xclient.data.l[3] = Button1;
                                ev.xclient.data.l[4] = 0;
                                VdFwXSendEvent(VD_FW_G.display, VdFwXDefaultRootWindow(VD_FW_G.display), False, SubstructureRedirectMask | SubstructureNotifyMask, &ev);

                                VdFwXSync(VD_FW_G.display, 0);
                                break;
                            }

                            int inside_caption = 
                                ((mouse_x >= VD_FW_G.nccaption[0]) && (mouse_x <= VD_FW_G.nccaption[2])) &&
                                ((mouse_y >= VD_FW_G.nccaption[1]) && (mouse_y <= VD_FW_G.nccaption[3]));

                            if (!VD_FW_G.nccaption_set) {
                                inside_caption = 1;
                            }

                            if (inside_caption) {
                                int hit_ignore_rects = 0;
                                for (int ri = 0; ri < VD_FW_G.ncrect_count; ++ri) {
                                    int rect[4] = {
                                        VD_FW_G.ncrects[ri][0],
                                        VD_FW_G.ncrects[ri][1],
                                        VD_FW_G.ncrects[ri][2],
                                        VD_FW_G.ncrects[ri][3],
                                    };

                                    int inside =
                                        ((mouse_x >= rect[0]) && (mouse_x <= rect[2])) &&
                                        ((mouse_y >= rect[1]) && (mouse_y <= rect[3]));

                                    if (inside) {
                                        hit_ignore_rects = 1;
                                        break;
                                    }
                                }

                                if (!hit_ignore_rects) {
                                    VD_FW_G.caption_dragging = 1;
                                    XEvent ev = {0};
                                    ev.xclient.type = ClientMessage;
                                    ev.xclient.window = VD_FW_G.window;
                                    ev.xclient.message_type =
                                        VdFwXInternAtom(VD_FW_G.display, (char*)"_NET_WM_MOVERESIZE", False);
                                    ev.xclient.format = 32;
                                    ev.xclient.data.l[0] = evt.xmotion.x_root;
                                    ev.xclient.data.l[1] = evt.xmotion.y_root;
                                    ev.xclient.data.l[2] = 8; // _NET_WM_MOVERESIZE_MOVE
                                    ev.xclient.data.l[3] = Button1;
                                    ev.xclient.data.l[4] = 0;

                                    VdFwXUngrabPointer(VD_FW_G.display, 0L);
                                    VdFwXFlush(VD_FW_G.display);

                                    VdFwXSendEvent(VD_FW_G.display,
                                       VdFwXDefaultRootWindow(VD_FW_G.display),
                                       False,
                                       SubstructureRedirectMask | SubstructureNotifyMask,
                                       &ev);

                                    VdFwXSync(VD_FW_G.display, 0);
                                }
                            }
                        } 
                    }
                }
            } break;

            case ButtonRelease: {
                int btn = vd_fw__x11_translate_mouse_button(evt.xbutton.button);
                if (btn) {
                    VdFwEvent fw_event;
                    VD_FW_MEMSET(&fw_event, 0, sizeof(fw_event));
                    fw_event.type = VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP;
                    fw_event.data.mouse_button_down.button = btn;
                    vd_fw__x11_msgbuf_w(&fw_event);

                    VD_FW_G.mouse_state &= ~btn;

                    if (VD_FW_G.borderless) {
                        if (btn == VD_FW_MOUSE_BUTTON_LEFT) {
                            VD_FW_G.caption_dragging = 0;
                        } 
                    }
                }
            } break;

            case ConfigureNotify: {
                XConfigureEvent* e = (XConfigureEvent*)&evt;
                // VD_FW_G.width = e->width;
                // VD_FW_G.height = e->height;
                // VD_FW_G.size_changed = 1;
                if ((w != e->width) || (h != e->height)) {
                    w = e->width;
                    h = e->height;

                    if (VD_FW_G.winthread_block_while_sizing) {
                        if (!resizing) {
                            resizing = 1;
                            clock_gettime(CLOCK_MONOTONIC, &resize_start);
                            pthread_mutex_lock(&VD_FW_G.mtx_paint);
                        } else {
                            clock_gettime(CLOCK_MONOTONIC, &resize_start);
                        }
                    }
                }
            } break;

            default: break;
        }

        if (VD_FW_G.t_running == 0) {
            break;
        }

        if (VD_FW_G.winthread_block_while_sizing) {

            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);

            struct timespec diff = vd_fw__linux_timespec_diff(now, resize_start);
            unsigned long long ms_diff = diff.tv_nsec / 1000;

            if (resizing && (ms_diff > 500)) {
                resizing = 0;
                pthread_mutex_unlock(&VD_FW_G.mtx_paint);
            }
        }


        if (!VD_FW_G.winthread_block_while_sizing) {
            pthread_mutex_lock(&VD_FW_G.mtx_paint);
        }

        if (!VD_FW_G.t_running) {
            if (!VD_FW_G.winthread_block_while_sizing) {
                pthread_mutex_unlock(&VD_FW_G.mtx_paint);
            }
            break;
        }

        if (w != VD_FW_G.next_frame.w || h != VD_FW_G.next_frame.h) {
            VD_FW_G.next_frame.w = w;
            VD_FW_G.next_frame.h = h;
            VD_FW_G.next_frame.flags |= VD_FW_X11_FLAGS_SIZE_CHANGED;
        }

        if (!VD_FW_G.winthread_block_while_sizing && VD_FW_G.sync_redraw) {
            VD_FW_G.next_frame.flags |= VD_FW_X11_FLAGS_WAKE_COND_VAR;
            pthread_cond_signal(&VD_FW_G.cnd_paint);
            struct timespec bound_time;
            bound_time.tv_sec = 0;
            bound_time.tv_nsec = 1000 * 100;
            pthread_cond_timedwait(&VD_FW_G.cnd_paint, &VD_FW_G.mtx_paint, &bound_time);
            // pthread_cond_wait(&VD_FW_G.cnd_paint, &VD_FW_G.mtx_paint);
        }

        if (!VD_FW_G.winthread_block_while_sizing) {
            pthread_mutex_unlock(&VD_FW_G.mtx_paint);
        }

        if (VD_FW_G.sync_redraw) {

            XSyncValue value;
            VdFwXSyncIntToValue(&value, VD_FW_G.sync_counter_value);
            VdFwXSyncSetCounter(VD_FW_G.display, VD_FW_G.sync_counter, value);
            VD_FW_G.sync_redraw = 0;
        }
    }
    return NULL;
}

#endif // _WIN32, __APPLE__, __linux__

#if !defined(__APPLE__)
/* ----EXTERNS DEFINITIONS------------------------------------------------------------------------------------------- */
#define X(retval, name, params) VdFwProcGL_##name  gl##name;
#define VER_START(v)
#define VER_END(v)
#define EXT_START(name)
#define EXT_END()
VD_FW_OPENGL_CORE_FUNCTIONS
#undef X
#undef VER_START
#undef VER_END
#undef EXT_START
#undef EXT_END

#endif // !defined(__APPLE__)

static int vd_fw__lookup_gl_extension(const char *q, VdFwGlConfig *config)
{
    VdFwGlExtension *extensions = config->req_extensions;
    int i = 0;
    while (extensions && (extensions[i].name != 0)) {
        if (vd_fw__strcmp(extensions[i].name, q) == 0) {
            extensions[i].available = 1;
            return 1;
        }
        i++;
    }

    extensions = config->opt_extensions;
    i = 0;
    while (extensions && (extensions[i].name != 0)) {
        if (vd_fw__strcmp(extensions[i].name, q) == 0) {
            extensions[i].available = 1;
            return 1;
        }
        i++;
    }

    return 0;
}

static int vd_fw__load_opengl(VdFwGlConfig *config)
{
#if defined(__APPLE__)
    // @todo(mdodis): This check
    // if (version > VD_FW_GL_VERION_4_1) {

    // }
#else
#define LOAD(p, s) s = (p)vd_fw__gl_get_proc_address(#s)
/* ----LOADING------------------------------------------------------------------------------------------------------- */
#define X(retval, name, params) LOAD(VdFwProcGL_##name, gl##name);
#define VER_START(v) if (config->version >= VD_FW_GL_VERSION_##v) {
#define VER_END(v) }
#define EXT_START(name) if (vd_fw__lookup_gl_extension(name, config)) {
#define EXT_END() }
VD_FW_OPENGL_CORE_FUNCTIONS
#undef X
#undef VER_START
#undef VER_END
#undef EXT_START
#undef EXT_END

#undef LOAD
#endif  // defined(__APPLE__)
    
    int result = 1;
    // Check required extensions 
    int i = 0;
    while (config->req_extensions && (config->req_extensions[i].name != 0)) {
        if (!config->req_extensions[i].available) {
            result = 0;
            break;
        }
        i++;
    }
    return result;
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

    const char *files[2];
    files[0] = vertex_file_path;
    files[1] = fragment_file_path;

    if (vd_fw__any_time_higher(2, files, last_compile)) {
        needs_recompile = 1;
    }

    if (!needs_recompile) {
        return 1;
    }

    int result = 1;

    size_t srv_sz, srf_sz;
    char *srv = vd_fw__debug_dump_file_text(vertex_file_path, &srv_sz);
    char *srf = vd_fw__debug_dump_file_text(fragment_file_path, &srf_sz);

    unsigned int vshd = vd_fw_compile_shader(GL_VERTEX_SHADER, srv);
    unsigned int fshd = vd_fw_compile_shader(GL_FRAGMENT_SHADER, srf);

    unsigned int new_program;
    if (vshd == 0 || fshd == 0) {
        result = 0;
    } else {
        VD_FW_FREE(srv, srv_sz);
        VD_FW_FREE(srf, srf_sz);

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
        crc = vd_fw__crc16(crc, (void*)" ", 1);
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
    buffer = VD_FW_REALLOC(buffer, element_size * (*cap), element_size * resize_capacity);
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

VD_FW_INL void vd_fw__u8_to_hex(VdFwU8 n, char *out)
{
    static const char numbers[] = "0123456789abcdef";
    VdFwU8 hi_bits = (n & 0xF0) >> 4;
    VdFwU8 lo_bits = (n & 0x0F);
    out[0] = numbers[hi_bits];
    out[1] = numbers[lo_bits];
}

VD_FW_INL void vd_fw__u16_to_hex(VdFwU16 n, char *out)
{
    VdFwU8 msb = (n & 0xFF00) >> 8;
    VdFwU8 lsb = (n & 0x00FF);

    vd_fw__u8_to_hex(msb, out + 0);
    vd_fw__u8_to_hex(lsb, out + 2);
}

VD_FW_API void vd_fw_gamepad_guid_to_cstr(VdFwGuid *guid, char *out)
{
    vd_fw__u16_to_hex(guid->parts.bus,             out +  0);
    vd_fw__u16_to_hex(guid->parts.crc,             out +  4);
    vd_fw__u16_to_hex(guid->parts.vendor_id,       out +  8);
    vd_fw__u16_to_hex(guid->parts.reserved0,       out + 12);
    vd_fw__u16_to_hex(guid->parts.product_id,      out + 16);
    vd_fw__u16_to_hex(guid->parts.reserved1,       out + 20);
    vd_fw__u16_to_hex(guid->parts.version,         out + 24);
    vd_fw__u8_to_hex(guid->parts.driver_signature, out + 28);
    vd_fw__u8_to_hex(guid->parts.driver_data,      out + 30);
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
#define __SYM(s) s, sizeof(s) - 1
    {__SYM("nes"),       VD_FW_GAMEPAD_CLASS_NES},       {__SYM("megadrive"), VD_FW_GAMEPAD_CLASS_MEGADRIVE},
    {__SYM("genesis"),   VD_FW_GAMEPAD_CLASS_GENESIS},   {__SYM("snes"),      VD_FW_GAMEPAD_CLASS_SNES},
    {__SYM("ps1"),       VD_FW_GAMEPAD_CLASS_PS1},       {__SYM("joycon"),    VD_FW_GAMEPAD_CLASS_JOYCON},
    {__SYM("n64"),       VD_FW_GAMEPAD_CLASS_N64},       {__SYM("ps2"),       VD_FW_GAMEPAD_CLASS_PS2},
    {__SYM("xbox"),      VD_FW_GAMEPAD_CLASS_XBOX},      {__SYM("ps4"),       VD_FW_GAMEPAD_CLASS_PS4},
    {__SYM("steamdeck"), VD_FW_GAMEPAD_CLASS_STEAMDECK},
#undef __SYM
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
#define __SYM(s) s, sizeof(s) - 1
    {__SYM("a"),             VD_FW_GAMEPAD_A,             0}, {__SYM("b"),             VD_FW_GAMEPAD_B,              0},
    {__SYM("x"),             VD_FW_GAMEPAD_X,             0}, {__SYM("y"),             VD_FW_GAMEPAD_Y,              0},
    {__SYM("start"),         VD_FW_GAMEPAD_START,         0}, {__SYM("back"),          VD_FW_GAMEPAD_BACK,           0},
    {__SYM("dpup"),          VD_FW_GAMEPAD_DUP,           0}, {__SYM("dpdown"),        VD_FW_GAMEPAD_DDOWN,          0},
    {__SYM("dpleft"),        VD_FW_GAMEPAD_DLEFT,         0}, {__SYM("dpright"),       VD_FW_GAMEPAD_DRIGHT,         0},
    {__SYM("leftshoulder"),  VD_FW_GAMEPAD_LEFT_SHOULDER, 0}, {__SYM("leftstick"),     VD_FW_GAMEPAD_LEFT_STICK,     0},
    {__SYM("lefttrigger"),   VD_FW_GAMEPAD_LT,            1}, {__SYM("rightshoulder"), VD_FW_GAMEPAD_RIGHT_SHOULDER, 0},
    {__SYM("rightstick"),    VD_FW_GAMEPAD_RIGHT_STICK,   0}, {__SYM("righttrigger"),  VD_FW_GAMEPAD_RT,             1},
    {__SYM("leftx"),         VD_FW_GAMEPAD_LH,            1}, {__SYM("lefty"),         VD_FW_GAMEPAD_LV,             1},
    {__SYM("rightx"),        VD_FW_GAMEPAD_RH,            1}, {__SYM("righty"),        VD_FW_GAMEPAD_RV,             1},
    {__SYM("lpad0"),         VD_FW_GAMEPAD_LEFT_PAD0,     0}, {__SYM("rpad0"),         VD_FW_GAMEPAD_RIGHT_PAD0,     0},
    {__SYM("lpad1"),         VD_FW_GAMEPAD_LEFT_PAD1,     0}, {__SYM("rpad1"),         VD_FW_GAMEPAD_RIGHT_PAD1,     0},
    {__SYM("lpad2"),         VD_FW_GAMEPAD_LEFT_PAD2,     0}, {__SYM("rpad2"),         VD_FW_GAMEPAD_RIGHT_PAD2,     0},
    {__SYM("aux0"),          VD_FW_GAMEPAD_AUX0,          0}, {__SYM("aux1"),          VD_FW_GAMEPAD_AUX1,           0},
    {__SYM("aux2"),          VD_FW_GAMEPAD_AUX2,          0}, {__SYM("aux3"),          VD_FW_GAMEPAD_AUX3,           0},
    {__SYM("aux4"),          VD_FW_GAMEPAD_AUX4,          0}, {__SYM("aux5"),          VD_FW_GAMEPAD_AUX5,           0},
    {__SYM("aux6"),          VD_FW_GAMEPAD_AUX6,          0}, {__SYM("aux7"),          VD_FW_GAMEPAD_AUX7,           0},
    {__SYM("aux8"),          VD_FW_GAMEPAD_AUX8,          0}, {__SYM("aux9"),          VD_FW_GAMEPAD_AUX9,           0},
#undef __SYM
};

static VdFw__GamepadSymbolToTarget *vd_fw__get_map_from_symbol(const char *s, int s_len)
{
    int map_count = sizeof(Vd_Fw__Gamepad_Symbols_To_Targets) / sizeof(Vd_Fw__Gamepad_Symbols_To_Targets[0]);

    for (int map_index = 0; map_index < map_count; ++map_index) {
        VdFw__GamepadSymbolToTarget *check = &Vd_Fw__Gamepad_Symbols_To_Targets[map_index];
        if (check->len != (size_t)s_len) {
            continue;
        }

        int found = 1;
        for (int i = 0; i < (int)check->len; ++i) {
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
        // VD_FW_G.gamepad_db_entries = (VdFwGamepadDBEntry*)vd_fw__realloc_mem(VD_FW_G.gamepad_db_entries, 
        //                                                                      sizeof(VD_FW_G.gamepad_db_entries[0]) * cap);
        // VD_FW_G.cap_gamepad_db_entries = cap;
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
VdFwGamepadDBEntry Vd_Fw__Gamepad_Db_Entries[] = {
#if defined(_WIN32)
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
#elif defined(__APPLE__)
    /*8BitDo Ultimate 2C*/
    {{0x03,0x00,0x00,0x00,0xc8,0x2d,0x00,0x00,0x1b,0x30,0x00,0x00,0x01,0x00,0x00,0x00},
    {
        {{0x1,VD_FW_GAMEPAD_A,0},{0x1,VD_FW_GAMEPAD_B,1},{0x1,VD_FW_GAMEPAD_SELECT,10},{0x2,VD_FW_GAMEPAD_DDOWN,4},{0x2,VD_FW_GAMEPAD_DLEFT,8},{0x2,VD_FW_GAMEPAD_DRIGHT,2},{0x2,VD_FW_GAMEPAD_DUP,1},{0x1,VD_FW_GAMEPAD_L1,6},{0x1,VD_FW_GAMEPAD_L3,13},{0x3,VD_FW_GAMEPAD_LT,5},{0x3,VD_FW_GAMEPAD_LH,0},{0x3,VD_FW_GAMEPAD_LV,1},{0x1,VD_FW_GAMEPAD_R1,7},{0x1,VD_FW_GAMEPAD_R3,14},{0x3,VD_FW_GAMEPAD_RT,4},{0x3,VD_FW_GAMEPAD_RH,2},{0x3,VD_FW_GAMEPAD_RV,3},{0x1,VD_FW_GAMEPAD_START,11},{0x1,VD_FW_GAMEPAD_X,3},{0x1,VD_FW_GAMEPAD_Y,4},{0,0,0},},
        {
            VD_FW_GAMEPAD_RUMBLE_TYPE_NOT_AVAILABLE,
        },
        VD_FW_GAMEPAD_FACE_UNKNOWN,
        VD_FW_GAMEPAD_CLASS_INVALID,
    }},
    /*8BitDo Ultimate 2C*/
    {{0x03,0x00,0x00,0x00,0xc8,0x2d,0x00,0x00,0x1d,0x30,0x00,0x00,0x01,0x00,0x00,0x00},
    {
        {{0x1,VD_FW_GAMEPAD_A,0},{0x1,VD_FW_GAMEPAD_B,1},{0x1,VD_FW_GAMEPAD_SELECT,10},{0x2,VD_FW_GAMEPAD_DDOWN,4},{0x2,VD_FW_GAMEPAD_DLEFT,8},{0x2,VD_FW_GAMEPAD_DRIGHT,2},{0x2,VD_FW_GAMEPAD_DUP,1},{0x1,VD_FW_GAMEPAD_L1,6},{0x1,VD_FW_GAMEPAD_L3,13},{0x3,VD_FW_GAMEPAD_LT,5},{0x3,VD_FW_GAMEPAD_LH,0},{0x3,VD_FW_GAMEPAD_LV,1},{0x1,VD_FW_GAMEPAD_R1,7},{0x1,VD_FW_GAMEPAD_R3,14},{0x3,VD_FW_GAMEPAD_RT,4},{0x3,VD_FW_GAMEPAD_RH,2},{0x3,VD_FW_GAMEPAD_RV,3},{0x1,VD_FW_GAMEPAD_START,11},{0x1,VD_FW_GAMEPAD_X,3},{0x1,VD_FW_GAMEPAD_Y,4},{0,0,0},},
        {
            VD_FW_GAMEPAD_RUMBLE_TYPE_NOT_AVAILABLE,
        },
        VD_FW_GAMEPAD_FACE_UNKNOWN,
        VD_FW_GAMEPAD_CLASS_INVALID,
    }},
#endif
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