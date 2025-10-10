/*
 * vd_fw.h - Gets you a window with OpenGL running on platforms that support it
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
 * TODO
 * - Game Controller DB
 * - Use or don't use stdlib memcpy
 * - Properly handle vd_fw_set_receive_ncmouse for clicks and scrolls
 * - Set mouse cursor to constants (resize, I, etc...)
 * - Should vd_fw_set_receive_ncmouse be default 0 or 1?
 *   - Actually, consider removing it entirely
 * - set min/max window size
 * - set window unresizable
 * - vd_fw_get_executable_dir() <-- statically allocated char * of executable directory
 * - vd_fw_minimize()
 * - vd_fw_maximize()
 * - vd_fw_get_last_key_pressed
 * - vd_fw_get_last_mouse_button_pressed
 * - MacOS: vd_fw_set_app_icon
 * - MacOS: vd_fw_set_fullscreen
 * - When window not focused, or minimize, delay drawing?
 * - Allow to request specific framerate?
 * - MacOS: There's no way to draw while resizing without changing the api?
 * - MacOS APIs can't be used on another thread other than main thread :/
 *   so, just initialize display link and wait on condition variable + mutex when drawing while resizing
 * - On borderless, push mouse event right as we lose focus to a value outside of the window space
 */
#ifndef VD_FW_H
#define VD_FW_H
#define VD_FW_VERSION_MAJOR    0
#define VD_FW_VERSION_MINOR    0
#define VD_FW_VERSION_PATCH    1
#define VD_FW_VERSION          ((VD_FW_VERSION_MAJOR << 16) | (VD_FW_VERSION_MINOR << 8) | (VD_FW_VERSION_PATCH))

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#ifndef VD_FW_API
#ifdef VD_FW_STATIC
#define VD_FW_API static
#else
#define VD_FW_API extern
#endif // VD_FW_STATIC
#endif // !VD_FW_API

#ifndef VD_FW_INL
#define VD_FW_INL static inline
#endif // VD_FW_INL

#ifndef VD_FW_NO_CRT
#define VD_FW_NO_CRT 0
#endif
#define VD_FW_FPI  3.14159265359f
#define VD_FW_FPI2 (2 * VD_FW_FPI)
#define VD_FW_FPIH (0.5f * VD_FW_FPI)

#ifndef VD_FW_SIN
#if VD_FW_NO_CRT
#define VD_FW_SIN(x) vd_fw_sin(x)
#else
#include <math.h>
#define VD_FW_SIN(x) sinf(x)
#endif
#endif // !VD_FW_SIN

#ifndef VD_FW_COS
#if VD_FW_NO_CRT
#define VD_FW_COS(x) vd_fw_cos(x)
#else
#include <math.h>
#define VD_FW_COS(x) cosf(x)
#endif
#endif // !VD_FW_COS

#ifndef VD_FW_TAN
#if VD_FW_NO_CRT
#define VD_FW_TAN(x) vd_fw_tan(x)
#else
#include <math.h>
#define VD_FW_TAN(x) tanf(x)
#endif
#endif // !VD_FW_TAN

#ifndef VD_FW_SQRT
#if VD_FW_NO_CRT
#define VD_FW_SQRT(x) vd_fw_sqrt(x)
#else
#include <math.h>
#define VD_FW_SQRT(x) sqrtf(x)
#endif
#endif // !VD_FW_SQRT

#ifndef VD_FW_GAMEPAD_COUNT_MAX
#define VD_FW_GAMEPAD_COUNT_MAX 4
#endif // !VD_FW_GAMEPAD_COUNT_MAX

typedef enum {
    VD_FW_GL_VERSION_BASIC = 0,
    VD_FW_GL_VERSION_1_0   = 1,
    VD_FW_GL_VERSION_1_2   = 12,
    VD_FW_GL_VERSION_1_3   = 13,
    VD_FW_GL_VERSION_1_4   = 14,
    VD_FW_GL_VERSION_1_5   = 15,
    VD_FW_GL_VERSION_2_0   = 20,
    VD_FW_GL_VERSION_2_1   = 21,
    VD_FW_GL_VERSION_3_0   = 30,
    VD_FW_GL_VERSION_3_1   = 31,
    VD_FW_GL_VERSION_3_2   = 32,
    VD_FW_GL_VERSION_3_3   = 33,
    VD_FW_GL_VERSION_4_0   = 40,
    VD_FW_GL_VERSION_4_1   = 41,
    VD_FW_GL_VERSION_4_2   = 42,
    VD_FW_GL_VERSION_4_3   = 43,
    VD_FW_GL_VERSION_4_4   = 44,
    VD_FW_GL_VERSION_4_5   = 45,
    VD_FW_GL_VERSION_4_6   = 46,
} VdFwGlVersion;

enum {
    VD_FW_KEY_UNKNOWN       = 0,
    VD_FW_KEY_F1            = 1,
    VD_FW_KEY_F2            = 2,
    VD_FW_KEY_F3            = 3,
    VD_FW_KEY_F4            = 4,
    VD_FW_KEY_F5            = 5,
    VD_FW_KEY_F6            = 6,
    VD_FW_KEY_F7            = 7,
    VD_FW_KEY_F8            = 8,
    VD_FW_KEY_F9            = 9,
    VD_FW_KEY_F10           = 10,
    VD_FW_KEY_F11           = 11,
    VD_FW_KEY_F12           = 12,
    VD_FW_KEY_F13           = 13,
    VD_FW_KEY_F14           = 14,
    VD_FW_KEY_F15           = 15,
    VD_FW_KEY_F16           = 16,
    VD_FW_KEY_F17           = 17,
    VD_FW_KEY_F18           = 18,
    VD_FW_KEY_F19           = 19,
    VD_FW_KEY_F20           = 20,
    VD_FW_KEY_F21           = 21,
    VD_FW_KEY_F22           = 22,
    VD_FW_KEY_F23           = 23,
    VD_FW_KEY_F24           = 24,
    VD_FW_KEY_BACKSPACE     = 25,  
    VD_FW_KEY_INS           = 26,
    VD_FW_KEY_HOME          = 27,
    VD_FW_KEY_PGUP          = 28,
    VD_FW_KEY_DEL           = 29,
    VD_FW_KEY_END           = 30,
    VD_FW_KEY_PGDN          = 31,
    VD_FW_KEY_SPACE         = 32,  // ' ' 
    VD_FW_KEY_LCONTROL      = 33,
    VD_FW_KEY_RCONTROL      = 34,
    VD_FW_KEY_LALT          = 35,
    VD_FW_KEY_RALT          = 36,
    VD_FW_KEY_LSHIFT        = 37,
    VD_FW_KEY_RSHIFT        = 38,
    VD_FW_KEY_QUOTE         = 39,  // '\''
    VD_FW_KEY_ARROW_UP      = 40,
    VD_FW_KEY_ARROW_LEFT    = 41,
    VD_FW_KEY_ARROW_DOWN    = 42,
    VD_FW_KEY_ARROW_RIGHT   = 43,
    VD_FW_KEY_COMMA         = 44,  // ','
    VD_FW_KEY_MINUS         = 45,  // '-'
    VD_FW_KEY_DOT           = 46,  // '.'
    VD_FW_KEY_SLASH_FORWARD = 47,  // '/'
    VD_FW_KEY_0             = 48,  // '0'
    VD_FW_KEY_1             = 49,  // '1'
    VD_FW_KEY_2             = 50,  // '2'
    VD_FW_KEY_3             = 51,  // '3'
    VD_FW_KEY_4             = 52,  // '4'
    VD_FW_KEY_5             = 53,  // '5'
    VD_FW_KEY_6             = 54,  // '6'
    VD_FW_KEY_7             = 55,  // '7'
    VD_FW_KEY_8             = 56,  // '8'
    VD_FW_KEY_9             = 57,  // '9'
    VD_FW_KEY_ENTER         = 58,
    VD_FW_KEY_SEMICOLON     = 59,  // ';'
    VD_FW_KEY_TAB           = 60,
    VD_FW_KEY_EQUALS        = 61,  // '='
    VD_FW_KEY_CAPITAL       = 62,
    VD_FW_KEY_ESCAPE        = 63,
    VD_FW_KEY_RESERVED1     = 64,  // '@'
    VD_FW_KEY_A             = 65,  // 'A'
    VD_FW_KEY_B             = 66,  // 'B'
    VD_FW_KEY_C             = 67,  // 'C'
    VD_FW_KEY_D             = 68,  // 'D'
    VD_FW_KEY_E             = 69,  // 'E'
    VD_FW_KEY_F             = 70,  // 'F'
    VD_FW_KEY_G             = 71,  // 'G'
    VD_FW_KEY_H             = 72,  // 'H'
    VD_FW_KEY_I             = 73,  // 'I'
    VD_FW_KEY_J             = 74,  // 'J'
    VD_FW_KEY_K             = 75,  // 'K'
    VD_FW_KEY_L             = 76,  // 'L'
    VD_FW_KEY_M             = 77,  // 'M'
    VD_FW_KEY_N             = 78,  // 'N'
    VD_FW_KEY_O             = 79,  // 'O'
    VD_FW_KEY_P             = 80,  // 'P'
    VD_FW_KEY_Q             = 81,  // 'Q'
    VD_FW_KEY_R             = 82,  // 'R'
    VD_FW_KEY_S             = 83,  // 'S'
    VD_FW_KEY_T             = 84,  // 'T'
    VD_FW_KEY_U             = 85,  // 'U'
    VD_FW_KEY_V             = 86,  // 'V'
    VD_FW_KEY_W             = 87,  // 'W'
    VD_FW_KEY_X             = 88,  // 'X'
    VD_FW_KEY_Y             = 89,  // 'Y'
    VD_FW_KEY_Z             = 90,  // 'Z'
    VD_FW_KEY_BRACKET_OPEN  = 91,  // '['
    VD_FW_KEY_SLASH_BACK    = 92,  // '\\'
    VD_FW_KEY_BRACKET_CLOSE = 93,  // ']'
    VD_FW_KEY_MEDIA_NEXT    = 94,  // Media Next Track
    VD_FW_KEY_MEDIA_PREV    = 95,  // Media Prev Track
    VD_FW_KEY_BACKTICK      = 96,  // '`'
    VD_FW_KEY_MEDIA_PLAY    = 97,  // Media Play/Pause
    VD_FW_KEY_NUMPAD_0      = 98,  // Numpad 0
    VD_FW_KEY_NUMPAD_1      = 99,  // Numpad 1
    VD_FW_KEY_NUMPAD_2      = 100, // Numpad 2
    VD_FW_KEY_NUMPAD_3      = 101, // Numpad 3
    VD_FW_KEY_NUMPAD_4      = 102, // Numpad 4
    VD_FW_KEY_NUMPAD_5      = 103, // Numpad 5
    VD_FW_KEY_NUMPAD_6      = 104, // Numpad 6
    VD_FW_KEY_NUMPAD_7      = 105, // Numpad 7
    VD_FW_KEY_NUMPAD_8      = 106, // Numpad 8
    VD_FW_KEY_NUMPAD_9      = 107, // Numpad 9
    VD_FW_KEY_MAX,
};
typedef int VdFwKey;

enum {
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
    VD_FW_GAMEPAD_SELECT,
    VD_FW_GAMEPAD_L1,
    VD_FW_GAMEPAD_R1,
    VD_FW_GAMEPAD_BUTTON_MAX,
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
};
typedef int VdFwGamepadInput;

enum {
    VD_FW_GAMEPAD_INPUT_TYPE_DIGITAL,
    VD_FW_GAMEPAD_INPUT_TYPE_AXIAL,
    VD_FW_GAMEPAD_INPUT_TYPE_HAT_SWITCH,
};

typedef struct {
    unsigned short input;
    unsigned short id;
} VdFwGamepadInputMapping;

typedef struct {
    int min_value;
    int max_value;
    unsigned short input;
    unsigned short id;
} VdFwGamepadInputMappingWithRange;

typedef struct {
    int            logical_range;
    unsigned short ids[4]; // Up, Right, Down, Left
} VdFwGamepadInputMappingWithDir;

typedef struct {
    int                              num_digital_mappings;
    VdFwGamepadInputMapping          digital_mappings[16];

    int                              num_axial_mappings;
    VdFwGamepadInputMappingWithRange axial_mappings[8];

    int                              num_hat_switch_mappings;
    VdFwGamepadInputMappingWithDir   hat_switch_mappings[2];
} VdFwGamepadConfig;

enum {
    VD_FW_MOUSE_STATE_LEFT_BUTTON_DOWN   = 1 << 0,
    VD_FW_MOUSE_STATE_RIGHT_BUTTON_DOWN  = 1 << 1,
    VD_FW_MOUSE_STATE_MIDDLE_BUTTON_DOWN = 1 << 2,
};

typedef struct {
    struct {
        /* What version of OpenGL you'd like to use. 3.3 and upwards recommended. */
        VdFwGlVersion version;

        /* Whether to enable a debug console to show you errors produced by GL calls */
        int           debug_on;
    } gl;

    struct {
        /* Set to 1 to disable window frame. */
        int           borderless;
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

/**
 * @brief Convert key to string.
 * @param  k The key
 * @return   The key's name
 */
VD_FW_INL const char*        vd_fw_get_key_name(VdFwKey k);

VD_FW_API int                vd_fw_get_gamepad_down(int index, int button);
VD_FW_API int                vd_fw_get_gamepad_axis(int index, int axis, float *out);

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
 * @param  pixels A packed A8B8G8R8 pixel buffer
 * @param  width  The width of the icon, in pixels, must be at least 16px
 * @param  height The height of the icon, in pixels, must be at least 16px
 */
VD_FW_API void               vd_fw_set_app_icon(void *pixels, int width, int height);

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

/* ----INTERNAL API-------------------------------------------------------------------------------------------------- */
VD_FW_API int vd_fw__any_time_higher(int num_files, const char **files, unsigned long long *check_against);
VD_FW_API char *vd_fw__debug_dump_file_text(const char *path);
VD_FW_API void vd_fw__free_mem(void *memory);
#if _WIN32
#define VD_FW_WIN32_SUBSYSTEM_CONSOLE 1
#define VD_FW_WIN32_SUBSYSTEM_WINDOWS 2

#ifndef VD_FW_WIN32_SUBSYSTEM
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#endif // !VD_FW_WIN32_SUBSYSTEM
#endif // _WIN32

#ifdef VD_FW_WIN32_NO_LINKER_COMMENTS
#define VD_FW_WIN32_LINKER_COMMENTS 0
#else
#define VD_FW_WIN32_LINKER_COMMENTS 1
#endif // !VD_FW_WIN32_NO_LINKER_COMMENTS

#if defined(__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
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
#ifdef _WIN32
typedef unsigned __int64   GLuint64;
typedef __int64            GLint64;
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

#if !defined(__APPLE__)
/* ----GL VERSION 1.0------------------------------------------------------------------------------------------------ */
typedef void                           (*PFNGLCULLFACEPROC)               (GLenum mode);
typedef void                           (*PFNGLFRONTFACEPROC)              (GLenum mode);
typedef void                           (*PFNGLHINTPROC)                   (GLenum target, GLenum mode);
typedef void                           (*PFNGLLINEWIDTHPROC)              (GLfloat width);
typedef void                           (*PFNGLPOINTSIZEPROC)              (GLfloat size);
typedef void                           (*PFNGLPOLYGONMODEPROC)            (GLenum face, GLenum mode);
typedef void                           (*PFNGLSCISSORPROC)                (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void                           (*PFNGLTEXPARAMETERFPROC)          (GLenum target, GLenum pname, GLfloat param);
typedef void                           (*PFNGLTEXPARAMETERFVPROC)         (GLenum target, GLenum pname, const GLfloat *params);
typedef void                           (*PFNGLTEXPARAMETERIPROC)          (GLenum target, GLenum pname, GLint param);
typedef void                           (*PFNGLTEXPARAMETERIVPROC)         (GLenum target, GLenum pname, const GLint *params);
typedef void                           (*PFNGLTEXIMAGE1DPROC)             (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void                           (*PFNGLTEXIMAGE2DPROC)             (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void                           (*PFNGLDRAWBUFFERPROC)             (GLenum buf);
typedef void                           (*PFNGLCLEARPROC)                  (GLbitfield mask);
typedef void                           (*PFNGLCLEARCOLORPROC)             (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void                           (*PFNGLCLEARSTENCILPROC)           (GLint s);
typedef void                           (*PFNGLCLEARDEPTHPROC)             (GLdouble depth);
typedef void                           (*PFNGLSTENCILMASKPROC)            (GLuint mask);
typedef void                           (*PFNGLCOLORMASKPROC)              (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void                           (*PFNGLDEPTHMASKPROC)              (GLboolean flag);
typedef void                           (*PFNGLDISABLEPROC)                (GLenum cap);
typedef void                           (*PFNGLENABLEPROC)                 (GLenum cap);
typedef void                           (*PFNGLFINISHPROC)                 (void);
typedef void                           (*PFNGLFLUSHPROC)                  (void);
typedef void                           (*PFNGLBLENDFUNCPROC)              (GLenum sfactor, GLenum dfactor);
typedef void                           (*PFNGLLOGICOPPROC)                (GLenum opcode);
typedef void                           (*PFNGLSTENCILFUNCPROC)            (GLenum func, GLint ref, GLuint mask);
typedef void                           (*PFNGLSTENCILOPPROC)              (GLenum fail, GLenum zfail, GLenum zpass);
typedef void                           (*PFNGLDEPTHFUNCPROC)              (GLenum func);
typedef void                           (*PFNGLPIXELSTOREFPROC)            (GLenum pname, GLfloat param);
typedef void                           (*PFNGLPIXELSTOREIPROC)            (GLenum pname, GLint param);
typedef void                           (*PFNGLREADBUFFERPROC)             (GLenum src);
typedef void                           (*PFNGLREADPIXELSPROC)             (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
typedef void                           (*PFNGLGETBOOLEANVPROC)            (GLenum pname, GLboolean *data);
typedef void                           (*PFNGLGETDOUBLEVPROC)             (GLenum pname, GLdouble *data);
typedef GLenum                         (*PFNGLGETERRORPROC)               (void);
typedef void                           (*PFNGLGETFLOATVPROC)              (GLenum pname, GLfloat *data);
typedef void                           (*PFNGLGETINTEGERVPROC)            (GLenum pname, GLint *data);
typedef const GLubyte *                (*PFNGLGETSTRINGPROC)              (GLenum name);
typedef void                           (*PFNGLGETTEXIMAGEPROC)            (GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
typedef void                           (*PFNGLGETTEXPARAMETERFVPROC)      (GLenum target, GLenum pname, GLfloat *params);
typedef void                           (*PFNGLGETTEXPARAMETERIVPROC)      (GLenum target, GLenum pname, GLint *params);
typedef void                           (*PFNGLGETTEXLEVELPARAMETERFVPROC) (GLenum target, GLint level, GLenum pname, GLfloat *params);
typedef void                           (*PFNGLGETTEXLEVELPARAMETERIVPROC) (GLenum target, GLint level, GLenum pname, GLint *params);
typedef GLboolean                      (*PFNGLISENABLEDPROC)              (GLenum cap);
typedef void                           (*PFNGLDEPTHRANGEPROC)             (GLdouble n, GLdouble f);
typedef void                           (*PFNGLVIEWPORTPROC)               (GLint x, GLint y, GLsizei width, GLsizei height);
extern void glCullFace                 (GLenum mode);
extern void glFrontFace                (GLenum mode);
extern void glHint                     (GLenum target, GLenum mode);
extern void glLineWidth                (GLfloat width);
extern void glPointSize                (GLfloat size);
extern void glPolygonMode              (GLenum face, GLenum mode);
extern void glScissor                  (GLint x, GLint y, GLsizei width, GLsizei height);
extern void glTexParameterf            (GLenum target, GLenum pname, GLfloat param);
extern void glTexParameterfv           (GLenum target, GLenum pname, const GLfloat *params);
extern void glTexParameteri            (GLenum target, GLenum pname, GLint param);
extern void glTexParameteriv           (GLenum target, GLenum pname, const GLint *params);
extern void glTexImage1D               (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
extern void glTexImage2D               (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
extern void glDrawBuffer               (GLenum buf);
extern void glClear                    (GLbitfield mask);
extern void glClearColor               (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void glClearStencil             (GLint s);
extern void glStencilMask              (GLuint mask);
extern void glColorMask                (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern void glDepthMask                (GLboolean flag);
extern void glDisable                  (GLenum cap);
extern void glEnable                   (GLenum cap);
extern void glFinish                   (void);
extern void glFlush                    (void);
extern void glBlendFunc                (GLenum sfactor, GLenum dfactor);
extern void glLogicOp                  (GLenum opcode);
extern void glStencilFunc              (GLenum func, GLint ref, GLuint mask);
extern void glStencilOp                (GLenum fail, GLenum zfail, GLenum zpass);
extern void glDepthFunc                (GLenum func);
extern void glPixelStoref              (GLenum pname, GLfloat param);
extern void glPixelStorei              (GLenum pname, GLint param);
extern void glReadBuffer               (GLenum src);
extern void glReadPixels               (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
extern void glGetBooleanv              (GLenum pname, GLboolean *data);
extern void glGetDoublev               (GLenum pname, GLdouble *data);
extern GLenum glGetError               (void);
extern void glGetFloatv                (GLenum pname, GLfloat *data);
extern void glGetIntegerv              (GLenum pname, GLint *data);
extern const GLubyte *glGetString      (GLenum name);
extern void glGetTexImage              (GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
extern void glGetTexParameterfv        (GLenum target, GLenum pname, GLfloat *params);
extern void glGetTexParameteriv        (GLenum target, GLenum pname, GLint *params);
extern void glGetTexLevelParameterfv   (GLenum target, GLint level, GLenum pname, GLfloat *params);
extern void glGetTexLevelParameteriv   (GLenum target, GLint level, GLenum pname, GLint *params);
extern GLboolean glIsEnabled           (GLenum cap);
extern void glDepthRange               (GLdouble n, GLdouble f);
extern void glViewport                 (GLint x, GLint y, GLsizei width, GLsizei height);
extern void glAccum                    (GLenum op, GLfloat value);
extern void glAlphaFunc                (GLenum func, GLclampf ref);
extern GLboolean glAreTexturesResident (GLsizei n, const GLuint *textures, GLboolean *residences);
extern void glArrayElement             (GLint i);
extern void glBegin                    (GLenum mode);
extern void glBindTexture              (GLenum target, GLuint texture);
extern void glBitmap                   (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
extern void glBlendFunc                (GLenum sfactor, GLenum dfactor);
extern void glCallList                 (GLuint list);
extern void glCallLists                (GLsizei n, GLenum type, const GLvoid *lists);
extern void glClear                    (GLbitfield mask);
extern void glClearAccum               (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void glClearColor               (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern void glClearDepth               (GLclampd depth);
extern void glClearIndex               (GLfloat c);
extern void glClearStencil             (GLint s);
extern void glClipPlane                (GLenum plane, const GLdouble *equation);
extern void glColor3b                  (GLbyte red, GLbyte green, GLbyte blue);
extern void glColor3bv                 (const GLbyte *v);
extern void glColor3d                  (GLdouble red, GLdouble green, GLdouble blue);
extern void glColor3dv                 (const GLdouble *v);
extern void glColor3f                  (GLfloat red, GLfloat green, GLfloat blue);
extern void glColor3fv                 (const GLfloat *v);
extern void glColor3i                  (GLint red, GLint green, GLint blue);
extern void glColor3iv                 (const GLint *v);
extern void glColor3s                  (GLshort red, GLshort green, GLshort blue);
extern void glColor3sv                 (const GLshort *v);
extern void glColor3ub                 (GLubyte red, GLubyte green, GLubyte blue);
extern void glColor3ubv                (const GLubyte *v);
extern void glColor3ui                 (GLuint red, GLuint green, GLuint blue);
extern void glColor3uiv                (const GLuint *v);
extern void glColor3us                 (GLushort red, GLushort green, GLushort blue);
extern void glColor3usv                (const GLushort *v);
extern void glColor4b                  (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
extern void glColor4bv                 (const GLbyte *v);
extern void glColor4d                  (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
extern void glColor4dv                 (const GLdouble *v);
extern void glColor4f                  (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void glColor4fv                 (const GLfloat *v);
extern void glColor4i                  (GLint red, GLint green, GLint blue, GLint alpha);
extern void glColor4iv                 (const GLint *v);
extern void glColor4s                  (GLshort red, GLshort green, GLshort blue, GLshort alpha);
extern void glColor4sv                 (const GLshort *v);
extern void glColor4ub                 (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
extern void glColor4ubv                (const GLubyte *v);
extern void glColor4ui                 (GLuint red, GLuint green, GLuint blue, GLuint alpha);
extern void glColor4uiv                (const GLuint *v);
extern void glColor4us                 (GLushort red, GLushort green, GLushort blue, GLushort alpha);
extern void glColor4usv                (const GLushort *v);
extern void glColorMask                (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern void glColorMaterial            (GLenum face, GLenum mode);
extern void glColorPointer             (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glCopyPixels               (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
extern void glCopyTexImage1D           (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
extern void glCopyTexImage2D           (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void glCopyTexSubImage1D        (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern void glCopyTexSubImage2D        (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void glCullFace                 (GLenum mode);
extern void glDeleteLists              (GLuint list, GLsizei range);
extern void glDeleteTextures           (GLsizei n, const GLuint *textures);
extern void glDepthFunc                (GLenum func);
extern void glDepthMask                (GLboolean flag);
extern void glDepthRange               (GLclampd zNear, GLclampd zFar);
extern void glDisable                  (GLenum cap);
extern void glDisableClientState       (GLenum array);
extern void glDrawArrays               (GLenum mode, GLint first, GLsizei count);
extern void glDrawBuffer               (GLenum mode);
extern void glDrawElements             (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
extern void glDrawPixels               (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern void glEdgeFlag                 (GLboolean flag);
extern void glEdgeFlagPointer          (GLsizei stride, const GLvoid *pointer);
extern void glEdgeFlagv                (const GLboolean *flag);
extern void glEnable                   (GLenum cap);
extern void glEnableClientState        (GLenum array);
extern void glEnd                      (void);
extern void glEndList                  (void);
extern void glEvalCoord1d              (GLdouble u);
extern void glEvalCoord1dv             (const GLdouble *u);
extern void glEvalCoord1f              (GLfloat u);
extern void glEvalCoord1fv             (const GLfloat *u);
extern void glEvalCoord2d              (GLdouble u, GLdouble v);
extern void glEvalCoord2dv             (const GLdouble *u);
extern void glEvalCoord2f              (GLfloat u, GLfloat v);
extern void glEvalCoord2fv             (const GLfloat *u);
extern void glEvalMesh1                (GLenum mode, GLint i1, GLint i2);
extern void glEvalMesh2                (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
extern void glEvalPoint1               (GLint i);
extern void glEvalPoint2               (GLint i, GLint j);
extern void glFeedbackBuffer           (GLsizei size, GLenum type, GLfloat *buffer);
extern void glFinish                   (void);
extern void glFlush                    (void);
extern void glFogf                     (GLenum pname, GLfloat param);
extern void glFogfv                    (GLenum pname, const GLfloat *params);
extern void glFogi                     (GLenum pname, GLint param);
extern void glFogiv                    (GLenum pname, const GLint *params);
extern void glFrontFace                (GLenum mode);
extern void glFrustum                  (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern GLuint glGenLists               (GLsizei range);
extern void glGenTextures              (GLsizei n, GLuint *textures);
extern void glGetBooleanv              (GLenum pname, GLboolean *params);
extern void glGetClipPlane             (GLenum plane, GLdouble *equation);
extern void glGetDoublev               (GLenum pname, GLdouble *params);
extern GLenum glGetError               (void);
extern void glGetFloatv                (GLenum pname, GLfloat *params);
extern void glGetIntegerv              (GLenum pname, GLint *params);
extern void glGetLightfv               (GLenum light, GLenum pname, GLfloat *params);
extern void glGetLightiv               (GLenum light, GLenum pname, GLint *params);
extern void glGetMapdv                 (GLenum target, GLenum query, GLdouble *v);
extern void glGetMapfv                 (GLenum target, GLenum query, GLfloat *v);
extern void glGetMapiv                 (GLenum target, GLenum query, GLint *v);
extern void glGetMaterialfv            (GLenum face, GLenum pname, GLfloat *params);
extern void glGetMaterialiv            (GLenum face, GLenum pname, GLint *params);
extern void glGetPixelMapfv            (GLenum map, GLfloat *values);
extern void glGetPixelMapuiv           (GLenum map, GLuint *values);
extern void glGetPixelMapusv           (GLenum map, GLushort *values);
extern void glGetPointerv              (GLenum pname, GLvoid* *params);
extern void glGetPolygonStipple        (GLubyte *mask);
extern const GLubyte * glGetString     (GLenum name);
extern void glGetTexEnvfv              (GLenum target, GLenum pname, GLfloat *params);
extern void glGetTexEnviv              (GLenum target, GLenum pname, GLint *params);
extern void glGetTexGendv              (GLenum coord, GLenum pname, GLdouble *params);
extern void glGetTexGenfv              (GLenum coord, GLenum pname, GLfloat *params);
extern void glGetTexGeniv              (GLenum coord, GLenum pname, GLint *params);
extern void glGetTexImage              (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
extern void glGetTexLevelParameterfv   (GLenum target, GLint level, GLenum pname, GLfloat *params);
extern void glGetTexLevelParameteriv   (GLenum target, GLint level, GLenum pname, GLint *params);
extern void glGetTexParameterfv        (GLenum target, GLenum pname, GLfloat *params);
extern void glGetTexParameteriv        (GLenum target, GLenum pname, GLint *params);
extern void glHint                     (GLenum target, GLenum mode);
extern void glIndexMask                (GLuint mask);
extern void glIndexPointer             (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glIndexd                   (GLdouble c);
extern void glIndexdv                  (const GLdouble *c);
extern void glIndexf                   (GLfloat c);
extern void glIndexfv                  (const GLfloat *c);
extern void glIndexi                   (GLint c);
extern void glIndexiv                  (const GLint *c);
extern void glIndexs                   (GLshort c);
extern void glIndexsv                  (const GLshort *c);
extern void glIndexub                  (GLubyte c);
extern void glIndexubv                 (const GLubyte *c);
extern void glInitNames                (void);
extern void glInterleavedArrays        (GLenum format, GLsizei stride, const GLvoid *pointer);
extern GLboolean glIsEnabled           (GLenum cap);
extern GLboolean glIsList              (GLuint list);
extern GLboolean glIsTexture           (GLuint texture);
extern void glLightModelf              (GLenum pname, GLfloat param);
extern void glLightModelfv             (GLenum pname, const GLfloat *params);
extern void glLightModeli              (GLenum pname, GLint param);
extern void glLightModeliv             (GLenum pname, const GLint *params);
extern void glLightf                   (GLenum light, GLenum pname, GLfloat param);
extern void glLightfv                  (GLenum light, GLenum pname, const GLfloat *params);
extern void glLighti                   (GLenum light, GLenum pname, GLint param);
extern void glLightiv                  (GLenum light, GLenum pname, const GLint *params);
extern void glLineStipple              (GLint factor, GLushort pattern);
extern void glLineWidth                (GLfloat width);
extern void glListBase                 (GLuint base);
extern void glLoadIdentity             (void);
extern void glLoadMatrixd              (const GLdouble *m);
extern void glLoadMatrixf              (const GLfloat *m);
extern void glLoadName                 (GLuint name);
extern void glLogicOp                  (GLenum opcode);
extern void glMap1d                    (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
extern void glMap1f                    (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
extern void glMap2d                    (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
extern void glMap2f                    (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
extern void glMapGrid1d                (GLint un, GLdouble u1, GLdouble u2);
extern void glMapGrid1f                (GLint un, GLfloat u1, GLfloat u2);
extern void glMapGrid2d                (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
extern void glMapGrid2f                (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
extern void glMaterialf                (GLenum face, GLenum pname, GLfloat param);
extern void glMaterialfv               (GLenum face, GLenum pname, const GLfloat *params);
extern void glMateriali                (GLenum face, GLenum pname, GLint param);
extern void glMaterialiv               (GLenum face, GLenum pname, const GLint *params);
extern void glMatrixMode               (GLenum mode);
extern void glMultMatrixd              (const GLdouble *m);
extern void glMultMatrixf              (const GLfloat *m);
extern void glNewList                  (GLuint list, GLenum mode);
extern void glNormal3b                 (GLbyte nx, GLbyte ny, GLbyte nz);
extern void glNormal3bv                (const GLbyte *v);
extern void glNormal3d                 (GLdouble nx, GLdouble ny, GLdouble nz);
extern void glNormal3dv                (const GLdouble *v);
extern void glNormal3f                 (GLfloat nx, GLfloat ny, GLfloat nz);
extern void glNormal3fv                (const GLfloat *v);
extern void glNormal3i                 (GLint nx, GLint ny, GLint nz);
extern void glNormal3iv                (const GLint *v);
extern void glNormal3s                 (GLshort nx, GLshort ny, GLshort nz);
extern void glNormal3sv                (const GLshort *v);
extern void glNormalPointer            (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glOrtho                    (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern void glPassThrough              (GLfloat token);
extern void glPixelMapfv               (GLenum map, GLsizei mapsize, const GLfloat *values);
extern void glPixelMapuiv              (GLenum map, GLsizei mapsize, const GLuint *values);
extern void glPixelMapusv              (GLenum map, GLsizei mapsize, const GLushort *values);
extern void glPixelStoref              (GLenum pname, GLfloat param);
extern void glPixelStorei              (GLenum pname, GLint param);
extern void glPixelTransferf           (GLenum pname, GLfloat param);
extern void glPixelTransferi           (GLenum pname, GLint param);
extern void glPixelZoom                (GLfloat xfactor, GLfloat yfactor);
extern void glPointSize                (GLfloat size);
extern void glPolygonMode              (GLenum face, GLenum mode);
extern void glPolygonOffset            (GLfloat factor, GLfloat units);
extern void glPolygonStipple           (const GLubyte *mask);
extern void glPopAttrib                (void);
extern void glPopClientAttrib          (void);
extern void glPopMatrix                (void);
extern void glPopName                  (void);
extern void glPrioritizeTextures       (GLsizei n, const GLuint *textures, const GLclampf *priorities);
extern void glPushAttrib               (GLbitfield mask);
extern void glPushClientAttrib         (GLbitfield mask);
extern void glPushMatrix               (void);
extern void glPushName                 (GLuint name);
extern void glRasterPos2d              (GLdouble x, GLdouble y);
extern void glRasterPos2dv             (const GLdouble *v);
extern void glRasterPos2f              (GLfloat x, GLfloat y);
extern void glRasterPos2fv             (const GLfloat *v);
extern void glRasterPos2i              (GLint x, GLint y);
extern void glRasterPos2iv             (const GLint *v);
extern void glRasterPos2s              (GLshort x, GLshort y);
extern void glRasterPos2sv             (const GLshort *v);
extern void glRasterPos3d              (GLdouble x, GLdouble y, GLdouble z);
extern void glRasterPos3dv             (const GLdouble *v);
extern void glRasterPos3f              (GLfloat x, GLfloat y, GLfloat z);
extern void glRasterPos3fv             (const GLfloat *v);
extern void glRasterPos3i              (GLint x, GLint y, GLint z);
extern void glRasterPos3iv             (const GLint *v);
extern void glRasterPos3s              (GLshort x, GLshort y, GLshort z);
extern void glRasterPos3sv             (const GLshort *v);
extern void glRasterPos4d              (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glRasterPos4dv             (const GLdouble *v);
extern void glRasterPos4f              (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glRasterPos4fv             (const GLfloat *v);
extern void glRasterPos4i              (GLint x, GLint y, GLint z, GLint w);
extern void glRasterPos4iv             (const GLint *v);
extern void glRasterPos4s              (GLshort x, GLshort y, GLshort z, GLshort w);
extern void glRasterPos4sv             (const GLshort *v);
extern void glReadBuffer               (GLenum mode);
extern void glReadPixels               (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
extern void glRectd                    (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
extern void glRectdv                   (const GLdouble *v1, const GLdouble *v2);
extern void glRectf                    (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
extern void glRectfv                   (const GLfloat *v1, const GLfloat *v2);
extern void glRecti                    (GLint x1, GLint y1, GLint x2, GLint y2);
extern void glRectiv                   (const GLint *v1, const GLint *v2);
extern void glRects                    (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
extern void glRectsv                   (const GLshort *v1, const GLshort *v2);
extern GLint glRenderMode              (GLenum mode);
extern void glRotated                  (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
extern void glRotatef                  (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern void glScaled                   (GLdouble x, GLdouble y, GLdouble z);
extern void glScalef                   (GLfloat x, GLfloat y, GLfloat z);
extern void glScissor                  (GLint x, GLint y, GLsizei width, GLsizei height);
extern void glSelectBuffer             (GLsizei size, GLuint *buffer);
extern void glShadeModel               (GLenum mode);
extern void glStencilFunc              (GLenum func, GLint ref, GLuint mask);
extern void glStencilMask              (GLuint mask);
extern void glStencilOp                (GLenum fail, GLenum zfail, GLenum zpass);
extern void glTexCoord1d               (GLdouble s);
extern void glTexCoord1dv              (const GLdouble *v);
extern void glTexCoord1f               (GLfloat s);
extern void glTexCoord1fv              (const GLfloat *v);
extern void glTexCoord1i               (GLint s);
extern void glTexCoord1iv              (const GLint *v);
extern void glTexCoord1s               (GLshort s);
extern void glTexCoord1sv              (const GLshort *v);
extern void glTexCoord2d               (GLdouble s, GLdouble t);
extern void glTexCoord2dv              (const GLdouble *v);
extern void glTexCoord2f               (GLfloat s, GLfloat t);
extern void glTexCoord2fv              (const GLfloat *v);
extern void glTexCoord2i               (GLint s, GLint t);
extern void glTexCoord2iv              (const GLint *v);
extern void glTexCoord2s               (GLshort s, GLshort t);
extern void glTexCoord2sv              (const GLshort *v);
extern void glTexCoord3d               (GLdouble s, GLdouble t, GLdouble r);
extern void glTexCoord3dv              (const GLdouble *v);
extern void glTexCoord3f               (GLfloat s, GLfloat t, GLfloat r);
extern void glTexCoord3fv              (const GLfloat *v);
extern void glTexCoord3i               (GLint s, GLint t, GLint r);
extern void glTexCoord3iv              (const GLint *v);
extern void glTexCoord3s               (GLshort s, GLshort t, GLshort r);
extern void glTexCoord3sv              (const GLshort *v);
extern void glTexCoord4d               (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern void glTexCoord4dv              (const GLdouble *v);
extern void glTexCoord4f               (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern void glTexCoord4fv              (const GLfloat *v);
extern void glTexCoord4i               (GLint s, GLint t, GLint r, GLint q);
extern void glTexCoord4iv              (const GLint *v);
extern void glTexCoord4s               (GLshort s, GLshort t, GLshort r, GLshort q);
extern void glTexCoord4sv              (const GLshort *v);
extern void glTexCoordPointer          (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glTexEnvf                  (GLenum target, GLenum pname, GLfloat param);
extern void glTexEnvfv                 (GLenum target, GLenum pname, const GLfloat *params);
extern void glTexEnvi                  (GLenum target, GLenum pname, GLint param);
extern void glTexEnviv                 (GLenum target, GLenum pname, const GLint *params);
extern void glTexGend                  (GLenum coord, GLenum pname, GLdouble param);
extern void glTexGendv                 (GLenum coord, GLenum pname, const GLdouble *params);
extern void glTexGenf                  (GLenum coord, GLenum pname, GLfloat param);
extern void glTexGenfv                 (GLenum coord, GLenum pname, const GLfloat *params);
extern void glTexGeni                  (GLenum coord, GLenum pname, GLint param);
extern void glTexGeniv                 (GLenum coord, GLenum pname, const GLint *params);
extern void glTexImage1D               (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTexImage2D               (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTexParameterf            (GLenum target, GLenum pname, GLfloat param);
extern void glTexParameterfv           (GLenum target, GLenum pname, const GLfloat *params);
extern void glTexParameteri            (GLenum target, GLenum pname, GLint param);
extern void glTexParameteriv           (GLenum target, GLenum pname, const GLint *params);
extern void glTexSubImage1D            (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTexSubImage2D            (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTranslated               (GLdouble x, GLdouble y, GLdouble z);
extern void glTranslatef               (GLfloat x, GLfloat y, GLfloat z);
extern void glVertex2d                 (GLdouble x, GLdouble y);
extern void glVertex2dv                (const GLdouble *v);
extern void glVertex2f                 (GLfloat x, GLfloat y);
extern void glVertex2fv                (const GLfloat *v);
extern void glVertex2i                 (GLint x, GLint y);
extern void glVertex2iv                (const GLint *v);
extern void glVertex2s                 (GLshort x, GLshort y);
extern void glVertex2sv                (const GLshort *v);
extern void glVertex3d                 (GLdouble x, GLdouble y, GLdouble z);
extern void glVertex3dv                (const GLdouble *v);
extern void glVertex3f                 (GLfloat x, GLfloat y, GLfloat z);
extern void glVertex3fv                (const GLfloat *v);
extern void glVertex3i                 (GLint x, GLint y, GLint z);
extern void glVertex3iv                (const GLint *v);
extern void glVertex3s                 (GLshort x, GLshort y, GLshort z);
extern void glVertex3sv                (const GLshort *v);
extern void glVertex4d                 (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glVertex4dv                (const GLdouble *v);
extern void glVertex4f                 (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glVertex4fv                (const GLfloat *v);
extern void glVertex4i                 (GLint x, GLint y, GLint z, GLint w);
extern void glVertex4iv                (const GLint *v);
extern void glVertex4s                 (GLshort x, GLshort y, GLshort z, GLshort w);
extern void glVertex4sv                (const GLshort *v);
extern void glVertexPointer            (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glViewport                 (GLint x, GLint y, GLsizei width, GLsizei height);
/* ----GL VERSION 1.1------------------------------------------------------------------------------------------------ */
typedef void      (* PFNGLDRAWARRAYSPROC)        (GLenum mode, GLint first, GLsizei count);
typedef void      (* PFNGLDRAWELEMENTSPROC)      (GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void      (* PFNGLGETPOINTERVPROC)       (GLenum pname, void **params);
typedef void      (* PFNGLPOLYGONOFFSETPROC)     (GLfloat factor, GLfloat units);
typedef void      (* PFNGLCOPYTEXIMAGE1DPROC)    (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void      (* PFNGLCOPYTEXIMAGE2DPROC)    (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void      (* PFNGLCOPYTEXSUBIMAGE1DPROC) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void      (* PFNGLCOPYTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void      (* PFNGLTEXSUBIMAGE1DPROC)     (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void      (* PFNGLTEXSUBIMAGE2DPROC)     (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void      (* PFNGLBINDTEXTUREPROC)       (GLenum target, GLuint texture);
typedef void      (* PFNGLDELETETEXTURESPROC)    (GLsizei n, const GLuint *textures);
typedef void      (* PFNGLGENTEXTURESPROC)       (GLsizei n, GLuint *textures);
typedef GLboolean (* PFNGLISTEXTUREPROC)         (GLuint texture);
extern void      glDrawArrays        (GLenum mode, GLint first, GLsizei count);
extern void      glDrawElements      (GLenum mode, GLsizei count, GLenum type, const void *indices);
extern void      glGetPointerv       (GLenum pname, void **params);
extern void      glPolygonOffset     (GLfloat factor, GLfloat units);
extern void      glCopyTexImage1D    (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
extern void      glCopyTexImage2D    (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void      glCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern void      glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void      glTexSubImage1D     (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
extern void      glTexSubImage2D     (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
extern void      glBindTexture       (GLenum target, GLuint texture);
extern void      glDeleteTextures    (GLsizei n, const GLuint *textures);
extern void      glGenTextures       (GLsizei n, GLuint *textures);
extern GLboolean glIsTexture         (GLuint texture);
/* ----GL VERSION 1.2------------------------------------------------------------------------------------------------ */
typedef void (* PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
typedef void (* PFNGLTEXIMAGE3DPROC)        (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (* PFNGLTEXSUBIMAGE3DPROC)     (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
typedef void (* PFNGLCOPYTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
extern PFNGLTEXIMAGE3DPROC        glTexImage3D;
extern PFNGLTEXSUBIMAGE3DPROC     glTexSubImage3D;
extern PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
/* ----GL VERSION 1.3------------------------------------------------------------------------------------------------ */
typedef void (* PFNGLACTIVETEXTUREPROC)           (GLenum texture);
typedef void (* PFNGLSAMPLECOVERAGEPROC)          (GLfloat value, GLboolean invert);
typedef void (* PFNGLCOMPRESSEDTEXIMAGE3DPROC)    (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
typedef void (* PFNGLCOMPRESSEDTEXIMAGE2DPROC)    (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
typedef void (* PFNGLCOMPRESSEDTEXIMAGE1DPROC)    (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
typedef void (* PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void (* PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
typedef void (* PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
typedef void (* PFNGLGETCOMPRESSEDTEXIMAGEPROC)   (GLenum target, GLint level, void *img);
extern PFNGLACTIVETEXTUREPROC           glActiveTexture;
extern PFNGLSAMPLECOVERAGEPROC          glSampleCoverage;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC    glCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC    glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC    glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC   glGetCompressedTexImage;
/* ----GL VERSION 1.4------------------------------------------------------------------------------------------------ */
typedef void (*PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (*PFNGLMULTIDRAWARRAYSPROC) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
typedef void (*PFNGLMULTIDRAWELEMENTSPROC) (GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount);
typedef void (*PFNGLPOINTPARAMETERFPROC) (GLenum pname, GLfloat param);
typedef void (*PFNGLPOINTPARAMETERFVPROC) (GLenum pname, const GLfloat *params);
typedef void (*PFNGLPOINTPARAMETERIPROC) (GLenum pname, GLint param);
typedef void (*PFNGLPOINTPARAMETERIVPROC) (GLenum pname, const GLint *params);
typedef void (*PFNGLBLENDCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (*PFNGLBLENDEQUATIONPROC) (GLenum mode);
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
extern PFNGLMULTIDRAWARRAYSPROC   glMultiDrawArrays;
extern PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
extern PFNGLPOINTPARAMETERFPROC   glPointParameterf;
extern PFNGLPOINTPARAMETERFVPROC  glPointParameterfv;
extern PFNGLPOINTPARAMETERIPROC   glPointParameteri;
extern PFNGLPOINTPARAMETERIVPROC  glPointParameteriv;
extern PFNGLBLENDCOLORPROC        glBlendColor;
extern PFNGLBLENDEQUATIONPROC     glBlendEquation;
/* ----GL VERSION 1.5------------------------------------------------------------------------------------------------ */
typedef void      (*PFNGLGENQUERIESPROC)           (GLsizei n, GLuint *ids);
typedef void      (*PFNGLDELETEQUERIESPROC)        (GLsizei n, const GLuint *ids);
typedef GLboolean (*PFNGLISQUERYPROC)              (GLuint id);
typedef void      (*PFNGLBEGINQUERYPROC)           (GLenum target, GLuint id);
typedef void      (*PFNGLENDQUERYPROC)             (GLenum target);
typedef void      (*PFNGLGETQUERYIVPROC)           (GLenum target, GLenum pname, GLint *params);
typedef void      (*PFNGLGETQUERYOBJECTIVPROC)     (GLuint id, GLenum pname, GLint *params);
typedef void      (*PFNGLGETQUERYOBJECTUIVPROC)    (GLuint id, GLenum pname, GLuint *params);
typedef void      (*PFNGLBINDBUFFERPROC)           (GLenum target, GLuint buffer);
typedef void      (*PFNGLDELETEBUFFERSPROC)        (GLsizei n, const GLuint *buffers);
typedef void      (*PFNGLGENBUFFERSPROC)           (GLsizei n, GLuint *buffers);
typedef GLboolean (*PFNGLISBUFFERPROC)             (GLuint buffer);
typedef void      (*PFNGLBUFFERDATAPROC)           (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void      (*PFNGLBUFFERSUBDATAPROC)        (GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
typedef void      (*PFNGLGETBUFFERSUBDATAPROC)     (GLenum target, GLintptr offset, GLsizeiptr size, void *data);
typedef void *    (*PFNGLMAPBUFFERPROC)            (GLenum target, GLenum access);
typedef GLboolean (*PFNGLUNMAPBUFFERPROC)          (GLenum target);
typedef void      (*PFNGLGETBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void      (*PFNGLGETBUFFERPOINTERVPROC)    (GLenum target, GLenum pname, void **params);
extern PFNGLGENQUERIESPROC           glGenQueries;
extern PFNGLDELETEQUERIESPROC        glDeleteQueries;
extern PFNGLISQUERYPROC              glIsQuery;
extern PFNGLBEGINQUERYPROC           glBeginQuery;
extern PFNGLENDQUERYPROC             glEndQuery;
extern PFNGLGETQUERYIVPROC           glGetQueryiv;
extern PFNGLGETQUERYOBJECTIVPROC     glGetQueryObjectiv;
extern PFNGLGETQUERYOBJECTUIVPROC    glGetQueryObjectuiv;
extern PFNGLBINDBUFFERPROC           glBindBuffer;
extern PFNGLDELETEBUFFERSPROC        glDeleteBuffers;
extern PFNGLGENBUFFERSPROC           glGenBuffers;
extern PFNGLISBUFFERPROC             glIsBuffer;
extern PFNGLBUFFERDATAPROC           glBufferData;
extern PFNGLBUFFERSUBDATAPROC        glBufferSubData;
extern PFNGLGETBUFFERSUBDATAPROC     glGetBufferSubData;
extern PFNGLMAPBUFFERPROC            glMapBuffer;
extern PFNGLUNMAPBUFFERPROC          glUnmapBuffer;
extern PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
extern PFNGLGETBUFFERPOINTERVPROC    glGetBufferPointerv;
/* ----GL VERSION 2.0------------------------------------------------------------------------------------------------ */
typedef void (*PFNGLBLENDEQUATIONSEPARATEPROC)    (GLenum modeRGB, GLenum modeAlpha);
typedef void (*PFNGLDRAWBUFFERSPROC)              (GLsizei n, const GLenum *bufs);
typedef void (*PFNGLSTENCILOPSEPARATEPROC)        (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (*PFNGLSTENCILFUNCSEPARATEPROC)      (GLenum face, GLenum func, GLint ref, GLuint mask);
typedef void (*PFNGLSTENCILMASKSEPARATEPROC)      (GLenum face, GLuint mask);
typedef void (*PFNGLATTACHSHADERPROC)             (GLuint program, GLuint shader);
typedef void (*PFNGLBINDATTRIBLOCATIONPROC)       (GLuint program, GLuint index, const GLchar *name);
typedef void (*PFNGLCOMPILESHADERPROC)            (GLuint shader);
typedef GLuint (*PFNGLCREATEPROGRAMPROC)          (void);
typedef GLuint (*PFNGLCREATESHADERPROC)           (GLenum type);
typedef void (*PFNGLDELETEPROGRAMPROC)            (GLuint program);
typedef void (*PFNGLDELETESHADERPROC)             (GLuint shader);
typedef void (*PFNGLDETACHSHADERPROC)             (GLuint program, GLuint shader);
typedef void (*PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (*PFNGLENABLEVERTEXATTRIBARRAYPROC)  (GLuint index);
typedef void (*PFNGLGETACTIVEATTRIBPROC)          (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (*PFNGLGETACTIVEUNIFORMPROC)         (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (*PFNGLGETATTACHEDSHADERSPROC)       (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
typedef GLint (*PFNGLGETATTRIBLOCATIONPROC)       (GLuint program, const GLchar *name);
typedef void (*PFNGLGETPROGRAMIVPROC)             (GLuint program, GLenum pname, GLint *params);
typedef void (*PFNGLGETPROGRAMINFOLOGPROC)        (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (*PFNGLGETSHADERIVPROC)              (GLuint shader, GLenum pname, GLint *params);
typedef void (*PFNGLGETSHADERINFOLOGPROC)         (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (*PFNGLGETSHADERSOURCEPROC)          (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
typedef GLint (*PFNGLGETUNIFORMLOCATIONPROC)      (GLuint program, const GLchar *name);
typedef void (*PFNGLGETUNIFORMFVPROC)             (GLuint program, GLint location, GLfloat *params);
typedef void (*PFNGLGETUNIFORMIVPROC)             (GLuint program, GLint location, GLint *params);
typedef void (*PFNGLGETVERTEXATTRIBDVPROC)        (GLuint index, GLenum pname, GLdouble *params);
typedef void (*PFNGLGETVERTEXATTRIBFVPROC)        (GLuint index, GLenum pname, GLfloat *params);
typedef void (*PFNGLGETVERTEXATTRIBIVPROC)        (GLuint index, GLenum pname, GLint *params);
typedef void (*PFNGLGETVERTEXATTRIBPOINTERVPROC)  (GLuint index, GLenum pname, void **pointer);
typedef GLboolean (*PFNGLISPROGRAMPROC)           (GLuint program);
typedef GLboolean (*PFNGLISSHADERPROC)            (GLuint shader);
typedef void (*PFNGLLINKPROGRAMPROC)              (GLuint program);
typedef void (*PFNGLSHADERSOURCEPROC)             (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (*PFNGLUSEPROGRAMPROC)               (GLuint program);
typedef void (*PFNGLUNIFORM1FPROC)                (GLint location, GLfloat v0);
typedef void (*PFNGLUNIFORM2FPROC)                (GLint location, GLfloat v0, GLfloat v1);
typedef void (*PFNGLUNIFORM3FPROC)                (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (*PFNGLUNIFORM4FPROC)                (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (*PFNGLUNIFORM1IPROC)                (GLint location, GLint v0);
typedef void (*PFNGLUNIFORM2IPROC)                (GLint location, GLint v0, GLint v1);
typedef void (*PFNGLUNIFORM3IPROC)                (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (*PFNGLUNIFORM4IPROC)                (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (*PFNGLUNIFORM1FVPROC)               (GLint location, GLsizei count, const GLfloat *value);
typedef void (*PFNGLUNIFORM2FVPROC)               (GLint location, GLsizei count, const GLfloat *value);
typedef void (*PFNGLUNIFORM3FVPROC)               (GLint location, GLsizei count, const GLfloat *value);
typedef void (*PFNGLUNIFORM4FVPROC)               (GLint location, GLsizei count, const GLfloat *value);
typedef void (*PFNGLUNIFORM1IVPROC)               (GLint location, GLsizei count, const GLint *value);
typedef void (*PFNGLUNIFORM2IVPROC)               (GLint location, GLsizei count, const GLint *value);
typedef void (*PFNGLUNIFORM3IVPROC)               (GLint location, GLsizei count, const GLint *value);
typedef void (*PFNGLUNIFORM4IVPROC)               (GLint location, GLsizei count, const GLint *value);
typedef void (*PFNGLUNIFORMMATRIX2FVPROC)         (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX3FVPROC)         (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX4FVPROC)         (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLVALIDATEPROGRAMPROC)          (GLuint program);
typedef void (*PFNGLVERTEXATTRIB1DPROC)           (GLuint index, GLdouble x);
typedef void (*PFNGLVERTEXATTRIB1DVPROC)          (GLuint index, const GLdouble *v);
typedef void (*PFNGLVERTEXATTRIB1FPROC)           (GLuint index, GLfloat x);
typedef void (*PFNGLVERTEXATTRIB1FVPROC)          (GLuint index, const GLfloat *v);
typedef void (*PFNGLVERTEXATTRIB1SPROC)           (GLuint index, GLshort x);
typedef void (*PFNGLVERTEXATTRIB1SVPROC)          (GLuint index, const GLshort *v);
typedef void (*PFNGLVERTEXATTRIB2DPROC)           (GLuint index, GLdouble x, GLdouble y);
typedef void (*PFNGLVERTEXATTRIB2DVPROC)          (GLuint index, const GLdouble *v);
typedef void (*PFNGLVERTEXATTRIB2FPROC)           (GLuint index, GLfloat x, GLfloat y);
typedef void (*PFNGLVERTEXATTRIB2FVPROC)          (GLuint index, const GLfloat *v);
typedef void (*PFNGLVERTEXATTRIB2SPROC)           (GLuint index, GLshort x, GLshort y);
typedef void (*PFNGLVERTEXATTRIB2SVPROC)          (GLuint index, const GLshort *v);
typedef void (*PFNGLVERTEXATTRIB3DPROC)           (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (*PFNGLVERTEXATTRIB3DVPROC)          (GLuint index, const GLdouble *v);
typedef void (*PFNGLVERTEXATTRIB3FPROC)           (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (*PFNGLVERTEXATTRIB3FVPROC)          (GLuint index, const GLfloat *v);
typedef void (*PFNGLVERTEXATTRIB3SPROC)           (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (*PFNGLVERTEXATTRIB3SVPROC)          (GLuint index, const GLshort *v);
typedef void (*PFNGLVERTEXATTRIB4NBVPROC)         (GLuint index, const GLbyte *v);
typedef void (*PFNGLVERTEXATTRIB4NIVPROC)         (GLuint index, const GLint *v);
typedef void (*PFNGLVERTEXATTRIB4NSVPROC)         (GLuint index, const GLshort *v);
typedef void (*PFNGLVERTEXATTRIB4NUBPROC)         (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (*PFNGLVERTEXATTRIB4NUBVPROC)        (GLuint index, const GLubyte *v);
typedef void (*PFNGLVERTEXATTRIB4NUIVPROC)        (GLuint index, const GLuint *v);
typedef void (*PFNGLVERTEXATTRIB4NUSVPROC)        (GLuint index, const GLushort *v);
typedef void (*PFNGLVERTEXATTRIB4BVPROC)          (GLuint index, const GLbyte *v);
typedef void (*PFNGLVERTEXATTRIB4DPROC)           (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (*PFNGLVERTEXATTRIB4DVPROC)          (GLuint index, const GLdouble *v);
typedef void (*PFNGLVERTEXATTRIB4FPROC)           (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (*PFNGLVERTEXATTRIB4FVPROC)          (GLuint index, const GLfloat *v);
typedef void (*PFNGLVERTEXATTRIB4IVPROC)          (GLuint index, const GLint *v);
typedef void (*PFNGLVERTEXATTRIB4SPROC)           (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (*PFNGLVERTEXATTRIB4SVPROC)          (GLuint index, const GLshort *v);
typedef void (*PFNGLVERTEXATTRIB4UBVPROC)         (GLuint index, const GLubyte *v);
typedef void (*PFNGLVERTEXATTRIB4UIVPROC)         (GLuint index, const GLuint *v);
typedef void (*PFNGLVERTEXATTRIB4USVPROC)         (GLuint index, const GLushort *v);
typedef void (*PFNGLVERTEXATTRIBPOINTERPROC)      (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
extern PFNGLBLENDEQUATIONSEPARATEPROC    glBlendEquationSeparate;
extern PFNGLDRAWBUFFERSPROC              glDrawBuffers;
extern PFNGLSTENCILOPSEPARATEPROC        glStencilOpSeparate;
extern PFNGLSTENCILFUNCSEPARATEPROC      glStencilFuncSeparate;
extern PFNGLSTENCILMASKSEPARATEPROC      glStencilMaskSeparate;
extern PFNGLATTACHSHADERPROC             glAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation;
extern PFNGLCOMPILESHADERPROC            glCompileShader;
extern PFNGLCREATEPROGRAMPROC            glCreateProgram;
extern PFNGLCREATESHADERPROC             glCreateShader;
extern PFNGLDELETEPROGRAMPROC            glDeleteProgram;
extern PFNGLDELETESHADERPROC             glDeleteShader;
extern PFNGLDETACHSHADERPROC             glDetachShader;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
extern PFNGLGETACTIVEATTRIBPROC          glGetActiveAttrib;
extern PFNGLGETACTIVEUNIFORMPROC         glGetActiveUniform;
extern PFNGLGETATTACHEDSHADERSPROC       glGetAttachedShaders;
extern PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
extern PFNGLGETPROGRAMIVPROC             glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
extern PFNGLGETSHADERIVPROC              glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
extern PFNGLGETSHADERSOURCEPROC          glGetShaderSource;
extern PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation;
extern PFNGLGETUNIFORMFVPROC             glGetUniformfv;
extern PFNGLGETUNIFORMIVPROC             glGetUniformiv;
extern PFNGLGETVERTEXATTRIBDVPROC        glGetVertexAttribdv;
extern PFNGLGETVERTEXATTRIBFVPROC        glGetVertexAttribfv;
extern PFNGLGETVERTEXATTRIBIVPROC        glGetVertexAttribiv;
extern PFNGLGETVERTEXATTRIBPOINTERVPROC  glGetVertexAttribPointerv;
extern PFNGLISPROGRAMPROC                glIsProgram;
extern PFNGLISSHADERPROC                 glIsShader;
extern PFNGLLINKPROGRAMPROC              glLinkProgram;
extern PFNGLSHADERSOURCEPROC             glShaderSource;
extern PFNGLUSEPROGRAMPROC               glUseProgram;
extern PFNGLUNIFORM1FPROC                glUniform1f;
extern PFNGLUNIFORM2FPROC                glUniform2f;
extern PFNGLUNIFORM3FPROC                glUniform3f;
extern PFNGLUNIFORM4FPROC                glUniform4f;
extern PFNGLUNIFORM1IPROC                glUniform1i;
extern PFNGLUNIFORM2IPROC                glUniform2i;
extern PFNGLUNIFORM3IPROC                glUniform3i;
extern PFNGLUNIFORM4IPROC                glUniform4i;
extern PFNGLUNIFORM1FVPROC               glUniform1fv;
extern PFNGLUNIFORM2FVPROC               glUniform2fv;
extern PFNGLUNIFORM3FVPROC               glUniform3fv;
extern PFNGLUNIFORM4FVPROC               glUniform4fv;
extern PFNGLUNIFORM1IVPROC               glUniform1iv;
extern PFNGLUNIFORM2IVPROC               glUniform2iv;
extern PFNGLUNIFORM3IVPROC               glUniform3iv;
extern PFNGLUNIFORM4IVPROC               glUniform4iv;
extern PFNGLUNIFORMMATRIX2FVPROC         glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC         glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv;
extern PFNGLVALIDATEPROGRAMPROC          glValidateProgram;
extern PFNGLVERTEXATTRIB1DPROC           glVertexAttrib1d;
extern PFNGLVERTEXATTRIB1DVPROC          glVertexAttrib1dv;
extern PFNGLVERTEXATTRIB1FPROC           glVertexAttrib1f;
extern PFNGLVERTEXATTRIB1FVPROC          glVertexAttrib1fv;
extern PFNGLVERTEXATTRIB1SPROC           glVertexAttrib1s;
extern PFNGLVERTEXATTRIB1SVPROC          glVertexAttrib1sv;
extern PFNGLVERTEXATTRIB2DPROC           glVertexAttrib2d;
extern PFNGLVERTEXATTRIB2DVPROC          glVertexAttrib2dv;
extern PFNGLVERTEXATTRIB2FPROC           glVertexAttrib2f;
extern PFNGLVERTEXATTRIB2FVPROC          glVertexAttrib2fv;
extern PFNGLVERTEXATTRIB2SPROC           glVertexAttrib2s;
extern PFNGLVERTEXATTRIB2SVPROC          glVertexAttrib2sv;
extern PFNGLVERTEXATTRIB3DPROC           glVertexAttrib3d;
extern PFNGLVERTEXATTRIB3DVPROC          glVertexAttrib3dv;
extern PFNGLVERTEXATTRIB3FPROC           glVertexAttrib3f;
extern PFNGLVERTEXATTRIB3FVPROC          glVertexAttrib3fv;
extern PFNGLVERTEXATTRIB3SPROC           glVertexAttrib3s;
extern PFNGLVERTEXATTRIB3SVPROC          glVertexAttrib3sv;
extern PFNGLVERTEXATTRIB4NBVPROC         glVertexAttrib4Nbv;
extern PFNGLVERTEXATTRIB4NIVPROC         glVertexAttrib4Niv;
extern PFNGLVERTEXATTRIB4NSVPROC         glVertexAttrib4Nsv;
extern PFNGLVERTEXATTRIB4NUBPROC         glVertexAttrib4Nub;
extern PFNGLVERTEXATTRIB4NUBVPROC        glVertexAttrib4Nubv;
extern PFNGLVERTEXATTRIB4NUIVPROC        glVertexAttrib4Nuiv;
extern PFNGLVERTEXATTRIB4NUSVPROC        glVertexAttrib4Nusv;
extern PFNGLVERTEXATTRIB4BVPROC          glVertexAttrib4bv;
extern PFNGLVERTEXATTRIB4DPROC           glVertexAttrib4d;
extern PFNGLVERTEXATTRIB4DVPROC          glVertexAttrib4dv;
extern PFNGLVERTEXATTRIB4FPROC           glVertexAttrib4f;
extern PFNGLVERTEXATTRIB4FVPROC          glVertexAttrib4fv;
extern PFNGLVERTEXATTRIB4IVPROC          glVertexAttrib4iv;
extern PFNGLVERTEXATTRIB4SPROC           glVertexAttrib4s;
extern PFNGLVERTEXATTRIB4SVPROC          glVertexAttrib4sv;
extern PFNGLVERTEXATTRIB4UBVPROC         glVertexAttrib4ubv;
extern PFNGLVERTEXATTRIB4UIVPROC         glVertexAttrib4uiv;
extern PFNGLVERTEXATTRIB4USVPROC         glVertexAttrib4usv;
extern PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
/* ----GL VERSION 2.1------------------------------------------------------------------------------------------------ */
typedef void (*PFNGLUNIFORMMATRIX2X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX3X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX2X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX4X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX3X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX4X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv;
extern PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
extern PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv;
extern PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv;
extern PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
extern PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;
/* ----GL VERSION 3.0------------------------------------------------------------------------------------------------ */
typedef void            (*PFNGLCOLORMASKIPROC)                          (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void            (*PFNGLGETBOOLEANI_VPROC)                       (GLenum target, GLuint index, GLboolean *data);
typedef void            (*PFNGLGETINTEGERI_VPROC)                       (GLenum target, GLuint index, GLint *data);
typedef void            (*PFNGLENABLEIPROC)                             (GLenum target, GLuint index);
typedef void            (*PFNGLDISABLEIPROC)                            (GLenum target, GLuint index);
typedef GLboolean       (*PFNGLISENABLEDIPROC)                          (GLenum target, GLuint index);
typedef void            (*PFNGLBEGINTRANSFORMFEEDBACKPROC)              (GLenum primitiveMode);
typedef void            (*PFNGLENDTRANSFORMFEEDBACKPROC)                (void);
typedef void            (*PFNGLBINDBUFFERRANGEPROC)                     (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void            (*PFNGLBINDBUFFERBASEPROC)                      (GLenum target, GLuint index, GLuint buffer);
typedef void            (*PFNGLTRANSFORMFEEDBACKVARYINGSPROC)           (GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode);
typedef void            (*PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)         (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
typedef void            (*PFNGLCLAMPCOLORPROC)                          (GLenum target, GLenum clamp);
typedef void            (*PFNGLBEGINCONDITIONALRENDERPROC)              (GLuint id, GLenum mode);
typedef void            (*PFNGLENDCONDITIONALRENDERPROC)                (void);
typedef void            (*PFNGLVERTEXATTRIBIPOINTERPROC)                (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void            (*PFNGLGETVERTEXATTRIBIIVPROC)                  (GLuint index, GLenum pname, GLint *params);
typedef void            (*PFNGLGETVERTEXATTRIBIUIVPROC)                 (GLuint index, GLenum pname, GLuint *params);
typedef void            (*PFNGLVERTEXATTRIBI1IPROC)                     (GLuint index, GLint x);
typedef void            (*PFNGLVERTEXATTRIBI2IPROC)                     (GLuint index, GLint x, GLint y);
typedef void            (*PFNGLVERTEXATTRIBI3IPROC)                     (GLuint index, GLint x, GLint y, GLint z);
typedef void            (*PFNGLVERTEXATTRIBI4IPROC)                     (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void            (*PFNGLVERTEXATTRIBI1UIPROC)                    (GLuint index, GLuint x);
typedef void            (*PFNGLVERTEXATTRIBI2UIPROC)                    (GLuint index, GLuint x, GLuint y);
typedef void            (*PFNGLVERTEXATTRIBI3UIPROC)                    (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void            (*PFNGLVERTEXATTRIBI4UIPROC)                    (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void            (*PFNGLVERTEXATTRIBI1IVPROC)                    (GLuint index, const GLint *v);
typedef void            (*PFNGLVERTEXATTRIBI2IVPROC)                    (GLuint index, const GLint *v);
typedef void            (*PFNGLVERTEXATTRIBI3IVPROC)                    (GLuint index, const GLint *v);
typedef void            (*PFNGLVERTEXATTRIBI4IVPROC)                    (GLuint index, const GLint *v);
typedef void            (*PFNGLVERTEXATTRIBI1UIVPROC)                   (GLuint index, const GLuint *v);
typedef void            (*PFNGLVERTEXATTRIBI2UIVPROC)                   (GLuint index, const GLuint *v);
typedef void            (*PFNGLVERTEXATTRIBI3UIVPROC)                   (GLuint index, const GLuint *v);
typedef void            (*PFNGLVERTEXATTRIBI4UIVPROC)                   (GLuint index, const GLuint *v);
typedef void            (*PFNGLVERTEXATTRIBI4BVPROC)                    (GLuint index, const GLbyte *v);
typedef void            (*PFNGLVERTEXATTRIBI4SVPROC)                    (GLuint index, const GLshort *v);
typedef void            (*PFNGLVERTEXATTRIBI4UBVPROC)                   (GLuint index, const GLubyte *v);
typedef void            (*PFNGLVERTEXATTRIBI4USVPROC)                   (GLuint index, const GLushort *v);
typedef void            (*PFNGLGETUNIFORMUIVPROC)                       (GLuint program, GLint location, GLuint *params);
typedef void            (*PFNGLBINDFRAGDATALOCATIONPROC)                (GLuint program, GLuint color, const GLchar *name);
typedef GLint           (*PFNGLGETFRAGDATALOCATIONPROC)                 (GLuint program, const GLchar *name);
typedef void            (*PFNGLUNIFORM1UIPROC)                          (GLint location, GLuint v0);
typedef void            (*PFNGLUNIFORM2UIPROC)                          (GLint location, GLuint v0, GLuint v1);
typedef void            (*PFNGLUNIFORM3UIPROC)                          (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void            (*PFNGLUNIFORM4UIPROC)                          (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void            (*PFNGLUNIFORM1UIVPROC)                         (GLint location, GLsizei count, const GLuint *value);
typedef void            (*PFNGLUNIFORM2UIVPROC)                         (GLint location, GLsizei count, const GLuint *value);
typedef void            (*PFNGLUNIFORM3UIVPROC)                         (GLint location, GLsizei count, const GLuint *value);
typedef void            (*PFNGLUNIFORM4UIVPROC)                         (GLint location, GLsizei count, const GLuint *value);
typedef void            (*PFNGLTEXPARAMETERIIVPROC)                     (GLenum target, GLenum pname, const GLint *params);
typedef void            (*PFNGLTEXPARAMETERIUIVPROC)                    (GLenum target, GLenum pname, const GLuint *params);
typedef void            (*PFNGLGETTEXPARAMETERIIVPROC)                  (GLenum target, GLenum pname, GLint *params);
typedef void            (*PFNGLGETTEXPARAMETERIUIVPROC)                 (GLenum target, GLenum pname, GLuint *params);
typedef void            (*PFNGLCLEARBUFFERIVPROC)                       (GLenum buffer, GLint drawbuffer, const GLint *value);
typedef void            (*PFNGLCLEARBUFFERUIVPROC)                      (GLenum buffer, GLint drawbuffer, const GLuint *value);
typedef void            (*PFNGLCLEARBUFFERFVPROC)                       (GLenum buffer, GLint drawbuffer, const GLfloat *value);
typedef void            (*PFNGLCLEARBUFFERFIPROC)                       (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef const GLubyte * (*PFNGLGETSTRINGIPROC)                          (GLenum name, GLuint index);
typedef GLboolean       (*PFNGLISRENDERBUFFERPROC)                      (GLuint renderbuffer);
typedef void            (*PFNGLBINDRENDERBUFFERPROC)                    (GLenum target, GLuint renderbuffer);
typedef void            (*PFNGLDELETERENDERBUFFERSPROC)                 (GLsizei n, const GLuint *renderbuffers);
typedef void            (*PFNGLGENRENDERBUFFERSPROC)                    (GLsizei n, GLuint *renderbuffers);
typedef void            (*PFNGLRENDERBUFFERSTORAGEPROC)                 (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void            (*PFNGLGETRENDERBUFFERPARAMETERIVPROC)          (GLenum target, GLenum pname, GLint *params);
typedef GLboolean       (*PFNGLISFRAMEBUFFERPROC)                       (GLuint framebuffer);
typedef void            (*PFNGLBINDFRAMEBUFFERPROC)                     (GLenum target, GLuint framebuffer);
typedef void            (*PFNGLDELETEFRAMEBUFFERSPROC)                  (GLsizei n, const GLuint *framebuffers);
typedef void            (*PFNGLGENFRAMEBUFFERSPROC)                     (GLsizei n, GLuint *framebuffers);
typedef GLenum          (*PFNGLCHECKFRAMEBUFFERSTATUSPROC)              (GLenum target);
typedef void            (*PFNGLFRAMEBUFFERTEXTURE1DPROC)                (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void            (*PFNGLFRAMEBUFFERTEXTURE2DPROC)                (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void            (*PFNGLFRAMEBUFFERTEXTURE3DPROC)                (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void            (*PFNGLFRAMEBUFFERRENDERBUFFERPROC)             (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void            (*PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
typedef void            (*PFNGLGENERATEMIPMAPPROC)                      (GLenum target);
typedef void            (*PFNGLBLITFRAMEBUFFERPROC)                     (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void            (*PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void            (*PFNGLFRAMEBUFFERTEXTURELAYERPROC)             (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void *          (*PFNGLMAPBUFFERRANGEPROC)                      (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void            (*PFNGLFLUSHMAPPEDBUFFERRANGEPROC)              (GLenum target, GLintptr offset, GLsizeiptr length);
typedef void            (*PFNGLBINDVERTEXARRAYPROC)                     (GLuint array);
typedef void            (*PFNGLDELETEVERTEXARRAYSPROC)                  (GLsizei n, const GLuint *arrays);
typedef void            (*PFNGLGENVERTEXARRAYSPROC)                     (GLsizei n, GLuint *arrays);
typedef GLboolean       (*PFNGLISVERTEXARRAYPROC)                       (GLuint array);
extern PFNGLCOLORMASKIPROC                          glColorMaski;
extern PFNGLGETBOOLEANI_VPROC                       glGetBooleani_v;
extern PFNGLGETINTEGERI_VPROC                       glGetIntegeri_v;
extern PFNGLENABLEIPROC                             glEnablei;
extern PFNGLDISABLEIPROC                            glDisablei;
extern PFNGLISENABLEDIPROC                          glIsEnabledi;
extern PFNGLBEGINTRANSFORMFEEDBACKPROC              glBeginTransformFeedback;
extern PFNGLENDTRANSFORMFEEDBACKPROC                glEndTransformFeedback;
extern PFNGLBINDBUFFERRANGEPROC                     glBindBufferRange;
extern PFNGLBINDBUFFERBASEPROC                      glBindBufferBase;
extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC           glTransformFeedbackVaryings;
extern PFNGLGETTRANSFORMFEEDBACKVARYINGPROC         glGetTransformFeedbackVarying;
extern PFNGLCLAMPCOLORPROC                          glClampColor;
extern PFNGLBEGINCONDITIONALRENDERPROC              glBeginConditionalRender;
extern PFNGLENDCONDITIONALRENDERPROC                glEndConditionalRender;
extern PFNGLVERTEXATTRIBIPOINTERPROC                glVertexAttribIPointer;
extern PFNGLGETVERTEXATTRIBIIVPROC                  glGetVertexAttribIiv;
extern PFNGLGETVERTEXATTRIBIUIVPROC                 glGetVertexAttribIuiv;
extern PFNGLVERTEXATTRIBI1IPROC                     glVertexAttribI1i;
extern PFNGLVERTEXATTRIBI2IPROC                     glVertexAttribI2i;
extern PFNGLVERTEXATTRIBI3IPROC                     glVertexAttribI3i;
extern PFNGLVERTEXATTRIBI4IPROC                     glVertexAttribI4i;
extern PFNGLVERTEXATTRIBI1UIPROC                    glVertexAttribI1ui;
extern PFNGLVERTEXATTRIBI2UIPROC                    glVertexAttribI2ui;
extern PFNGLVERTEXATTRIBI3UIPROC                    glVertexAttribI3ui;
extern PFNGLVERTEXATTRIBI4UIPROC                    glVertexAttribI4ui;
extern PFNGLVERTEXATTRIBI1IVPROC                    glVertexAttribI1iv;
extern PFNGLVERTEXATTRIBI2IVPROC                    glVertexAttribI2iv;
extern PFNGLVERTEXATTRIBI3IVPROC                    glVertexAttribI3iv;
extern PFNGLVERTEXATTRIBI4IVPROC                    glVertexAttribI4iv;
extern PFNGLVERTEXATTRIBI1UIVPROC                   glVertexAttribI1uiv;
extern PFNGLVERTEXATTRIBI2UIVPROC                   glVertexAttribI2uiv;
extern PFNGLVERTEXATTRIBI3UIVPROC                   glVertexAttribI3uiv;
extern PFNGLVERTEXATTRIBI4UIVPROC                   glVertexAttribI4uiv;
extern PFNGLVERTEXATTRIBI4BVPROC                    glVertexAttribI4bv;
extern PFNGLVERTEXATTRIBI4SVPROC                    glVertexAttribI4sv;
extern PFNGLVERTEXATTRIBI4UBVPROC                   glVertexAttribI4ubv;
extern PFNGLVERTEXATTRIBI4USVPROC                   glVertexAttribI4usv;
extern PFNGLGETUNIFORMUIVPROC                       glGetUniformuiv;
extern PFNGLBINDFRAGDATALOCATIONPROC                glBindFragDataLocation;
extern PFNGLGETFRAGDATALOCATIONPROC                 glGetFragDataLocation;
extern PFNGLUNIFORM1UIPROC                          glUniform1ui;
extern PFNGLUNIFORM2UIPROC                          glUniform2ui;
extern PFNGLUNIFORM3UIPROC                          glUniform3ui;
extern PFNGLUNIFORM4UIPROC                          glUniform4ui;
extern PFNGLUNIFORM1UIVPROC                         glUniform1uiv;
extern PFNGLUNIFORM2UIVPROC                         glUniform2uiv;
extern PFNGLUNIFORM3UIVPROC                         glUniform3uiv;
extern PFNGLUNIFORM4UIVPROC                         glUniform4uiv;
extern PFNGLTEXPARAMETERIIVPROC                     glTexParameterIiv;
extern PFNGLTEXPARAMETERIUIVPROC                    glTexParameterIuiv;
extern PFNGLGETTEXPARAMETERIIVPROC                  glGetTexParameterIiv;
extern PFNGLGETTEXPARAMETERIUIVPROC                 glGetTexParameterIuiv;
extern PFNGLCLEARBUFFERIVPROC                       glClearBufferiv;
extern PFNGLCLEARBUFFERUIVPROC                      glClearBufferuiv;
extern PFNGLCLEARBUFFERFVPROC                       glClearBufferfv;
extern PFNGLCLEARBUFFERFIPROC                       glClearBufferfi;
extern PFNGLGETSTRINGIPROC                          glGetStringi;
extern PFNGLISRENDERBUFFERPROC                      glIsRenderbuffer;
extern PFNGLBINDRENDERBUFFERPROC                    glBindRenderbuffer;
extern PFNGLDELETERENDERBUFFERSPROC                 glDeleteRenderbuffers;
extern PFNGLGENRENDERBUFFERSPROC                    glGenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEPROC                 glRenderbufferStorage;
extern PFNGLGETRENDERBUFFERPARAMETERIVPROC          glGetRenderbufferParameteriv;
extern PFNGLISFRAMEBUFFERPROC                       glIsFramebuffer;
extern PFNGLBINDFRAMEBUFFERPROC                     glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC                  glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSPROC                     glGenFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC              glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE1DPROC                glFramebufferTexture1D;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC                glFramebufferTexture2D;
extern PFNGLFRAMEBUFFERTEXTURE3DPROC                glFramebufferTexture3D;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC             glFramebufferRenderbuffer;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
extern PFNGLGENERATEMIPMAPPROC                      glGenerateMipmap;
extern PFNGLBLITFRAMEBUFFERPROC                     glBlitFramebuffer;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC      glRenderbufferStorageMultisample;
extern PFNGLFRAMEBUFFERTEXTURELAYERPROC             glFramebufferTextureLayer;
extern PFNGLMAPBUFFERRANGEPROC                      glMapBufferRange;
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC              glFlushMappedBufferRange;
extern PFNGLBINDVERTEXARRAYPROC                     glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC                  glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSPROC                     glGenVertexArrays;
extern PFNGLISVERTEXARRAYPROC                       glIsVertexArray;
/* ----GL VERSION 3.1------------------------------------------------------------------------------------------------ */
typedef void   (*PFNGLDRAWARRAYSINSTANCEDPROC)       (GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
typedef void   (*PFNGLDRAWELEMENTSINSTANCEDPROC)     (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
typedef void   (*PFNGLTEXBUFFERPROC)                 (GLenum target, GLenum internalformat, GLuint buffer);
typedef void   (*PFNGLPRIMITIVERESTARTINDEXPROC)     (GLuint index);
typedef void   (*PFNGLCOPYBUFFERSUBDATAPROC)         (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void   (*PFNGLGETUNIFORMINDICESPROC)         (GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
typedef void   (*PFNGLGETACTIVEUNIFORMSIVPROC)       (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
typedef void   (*PFNGLGETACTIVEUNIFORMNAMEPROC)      (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
typedef GLuint (*PFNGLGETUNIFORMBLOCKINDEXPROC)      (GLuint program, const GLchar *uniformBlockName);
typedef void   (*PFNGLGETACTIVEUNIFORMBLOCKIVPROC)   (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
typedef void   (*PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
typedef void   (*PFNGLUNIFORMBLOCKBINDINGPROC)       (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
extern PFNGLDRAWARRAYSINSTANCEDPROC       glDrawArraysInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDPROC     glDrawElementsInstanced;
extern PFNGLTEXBUFFERPROC                 glTexBuffer;
extern PFNGLPRIMITIVERESTARTINDEXPROC     glPrimitiveRestartIndex;
extern PFNGLCOPYBUFFERSUBDATAPROC         glCopyBufferSubData;
extern PFNGLGETUNIFORMINDICESPROC         glGetUniformIndices;
extern PFNGLGETACTIVEUNIFORMSIVPROC       glGetActiveUniformsiv;
extern PFNGLGETACTIVEUNIFORMNAMEPROC      glGetActiveUniformName;
extern PFNGLGETUNIFORMBLOCKINDEXPROC      glGetUniformBlockIndex;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC   glGetActiveUniformBlockiv;
extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
extern PFNGLUNIFORMBLOCKBINDINGPROC       glUniformBlockBinding;
/* ----GL VERSION 3.2------------------------------------------------------------------------------------------------ */
typedef void      (*PFNGLDRAWELEMENTSBASEVERTEXPROC)          (GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void      (*PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)     (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void      (*PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
typedef void      (*PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)     (GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex);
typedef void      (*PFNGLPROVOKINGVERTEXPROC)                 (GLenum mode);
typedef GLsync    (*PFNGLFENCESYNCPROC)                       (GLenum condition, GLbitfield flags);
typedef GLboolean (*PFNGLISSYNCPROC)                          (GLsync sync);
typedef void      (*PFNGLDELETESYNCPROC)                      (GLsync sync);
typedef GLenum    (*PFNGLCLIENTWAITSYNCPROC)                  (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void      (*PFNGLWAITSYNCPROC)                        (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void      (*PFNGLGETINTEGER64VPROC)                   (GLenum pname, GLint64 *data);
typedef void      (*PFNGLGETSYNCIVPROC)                       (GLsync sync, GLenum pname, GLsizei count, GLsizei *length, GLint *values);
typedef void      (*PFNGLGETINTEGER64I_VPROC)                 (GLenum target, GLuint index, GLint64 *data);
typedef void      (*PFNGLGETBUFFERPARAMETERI64VPROC)          (GLenum target, GLenum pname, GLint64 *params);
typedef void      (*PFNGLFRAMEBUFFERTEXTUREPROC)              (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void      (*PFNGLTEXIMAGE2DMULTISAMPLEPROC)           (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void      (*PFNGLTEXIMAGE3DMULTISAMPLEPROC)           (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void      (*PFNGLGETMULTISAMPLEFVPROC)                (GLenum pname, GLuint index, GLfloat *val);
typedef void      (*PFNGLSAMPLEMASKIPROC)                     (GLuint maskNumber, GLbitfield mask);
extern PFNGLDRAWELEMENTSBASEVERTEXPROC          glDrawElementsBaseVertex;
extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC     glDrawRangeElementsBaseVertex;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex;
extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC     glMultiDrawElementsBaseVertex;
extern PFNGLPROVOKINGVERTEXPROC                 glProvokingVertex;
extern PFNGLFENCESYNCPROC                       glFenceSync;
extern PFNGLISSYNCPROC                          glIsSync;
extern PFNGLDELETESYNCPROC                      glDeleteSync;
extern PFNGLCLIENTWAITSYNCPROC                  glClientWaitSync;
extern PFNGLWAITSYNCPROC                        glWaitSync;
extern PFNGLGETINTEGER64VPROC                   glGetInteger64v;
extern PFNGLGETSYNCIVPROC                       glGetSynciv;
extern PFNGLGETINTEGER64I_VPROC                 glGetInteger64i_v;
extern PFNGLGETBUFFERPARAMETERI64VPROC          glGetBufferParameteri64v;
extern PFNGLFRAMEBUFFERTEXTUREPROC              glFramebufferTexture;
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC           glTexImage2DMultisample;
extern PFNGLTEXIMAGE3DMULTISAMPLEPROC           glTexImage3DMultisample;
extern PFNGLGETMULTISAMPLEFVPROC                glGetMultisamplefv;
extern PFNGLSAMPLEMASKIPROC                     glSampleMaski;
/* ----GL VERSION 3.3------------------------------------------------------------------------------------------------ */
typedef void      (*PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) (GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
typedef GLint     (*PFNGLGETFRAGDATAINDEXPROC)            (GLuint program, const GLchar *name);
typedef void      (*PFNGLGENSAMPLERSPROC)                 (GLsizei count, GLuint *samplers);
typedef void      (*PFNGLDELETESAMPLERSPROC)              (GLsizei count, const GLuint *samplers);
typedef GLboolean (*PFNGLISSAMPLERPROC)                   (GLuint sampler);
typedef void      (*PFNGLBINDSAMPLERPROC)                 (GLuint unit, GLuint sampler);
typedef void      (*PFNGLSAMPLERPARAMETERIPROC)           (GLuint sampler, GLenum pname, GLint param);
typedef void      (*PFNGLSAMPLERPARAMETERIVPROC)          (GLuint sampler, GLenum pname, const GLint *param);
typedef void      (*PFNGLSAMPLERPARAMETERFPROC)           (GLuint sampler, GLenum pname, GLfloat param);
typedef void      (*PFNGLSAMPLERPARAMETERFVPROC)          (GLuint sampler, GLenum pname, const GLfloat *param);
typedef void      (*PFNGLSAMPLERPARAMETERIIVPROC)         (GLuint sampler, GLenum pname, const GLint *param);
typedef void      (*PFNGLSAMPLERPARAMETERIUIVPROC)        (GLuint sampler, GLenum pname, const GLuint *param);
typedef void      (*PFNGLGETSAMPLERPARAMETERIVPROC)       (GLuint sampler, GLenum pname, GLint *params);
typedef void      (*PFNGLGETSAMPLERPARAMETERIIVPROC)      (GLuint sampler, GLenum pname, GLint *params);
typedef void      (*PFNGLGETSAMPLERPARAMETERFVPROC)       (GLuint sampler, GLenum pname, GLfloat *params);
typedef void      (*PFNGLGETSAMPLERPARAMETERIUIVPROC)     (GLuint sampler, GLenum pname, GLuint *params);
typedef void      (*PFNGLQUERYCOUNTERPROC)                (GLuint id, GLenum target);
typedef void      (*PFNGLGETQUERYOBJECTI64VPROC)          (GLuint id, GLenum pname, GLint64 *params);
typedef void      (*PFNGLGETQUERYOBJECTUI64VPROC)         (GLuint id, GLenum pname, GLuint64 *params);
typedef void      (*PFNGLVERTEXATTRIBDIVISORPROC)         (GLuint index, GLuint divisor);
typedef void      (*PFNGLVERTEXATTRIBP1UIPROC)            (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void      (*PFNGLVERTEXATTRIBP1UIVPROC)           (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
typedef void      (*PFNGLVERTEXATTRIBP2UIPROC)            (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void      (*PFNGLVERTEXATTRIBP2UIVPROC)           (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
typedef void      (*PFNGLVERTEXATTRIBP3UIPROC)            (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void      (*PFNGLVERTEXATTRIBP3UIVPROC)           (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
typedef void      (*PFNGLVERTEXATTRIBP4UIPROC)            (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void      (*PFNGLVERTEXATTRIBP4UIVPROC)           (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed;
extern PFNGLGETFRAGDATAINDEXPROC            glGetFragDataIndex;
extern PFNGLGENSAMPLERSPROC                 glGenSamplers;
extern PFNGLDELETESAMPLERSPROC              glDeleteSamplers;
extern PFNGLISSAMPLERPROC                   glIsSampler;
extern PFNGLBINDSAMPLERPROC                 glBindSampler;
extern PFNGLSAMPLERPARAMETERIPROC           glSamplerParameteri;
extern PFNGLSAMPLERPARAMETERIVPROC          glSamplerParameteriv;
extern PFNGLSAMPLERPARAMETERFPROC           glSamplerParameterf;
extern PFNGLSAMPLERPARAMETERFVPROC          glSamplerParameterfv;
extern PFNGLSAMPLERPARAMETERIIVPROC         glSamplerParameterIiv;
extern PFNGLSAMPLERPARAMETERIUIVPROC        glSamplerParameterIuiv;
extern PFNGLGETSAMPLERPARAMETERIVPROC       glGetSamplerParameteriv;
extern PFNGLGETSAMPLERPARAMETERIIVPROC      glGetSamplerParameterIiv;
extern PFNGLGETSAMPLERPARAMETERFVPROC       glGetSamplerParameterfv;
extern PFNGLGETSAMPLERPARAMETERIUIVPROC     glGetSamplerParameterIuiv;
extern PFNGLQUERYCOUNTERPROC                glQueryCounter;
extern PFNGLGETQUERYOBJECTI64VPROC          glGetQueryObjecti64v;
extern PFNGLGETQUERYOBJECTUI64VPROC         glGetQueryObjectui64v;
extern PFNGLVERTEXATTRIBDIVISORPROC         glVertexAttribDivisor;
extern PFNGLVERTEXATTRIBP1UIPROC            glVertexAttribP1ui;
extern PFNGLVERTEXATTRIBP1UIVPROC           glVertexAttribP1uiv;
extern PFNGLVERTEXATTRIBP2UIPROC            glVertexAttribP2ui;
extern PFNGLVERTEXATTRIBP2UIVPROC           glVertexAttribP2uiv;
extern PFNGLVERTEXATTRIBP3UIPROC            glVertexAttribP3ui;
extern PFNGLVERTEXATTRIBP3UIVPROC           glVertexAttribP3uiv;
extern PFNGLVERTEXATTRIBP4UIPROC            glVertexAttribP4ui;
extern PFNGLVERTEXATTRIBP4UIVPROC           glVertexAttribP4uiv;
/* ----GL VERSION 4.0------------------------------------------------------------------------------------------------ */
typedef void      (*PFNGLMINSAMPLESHADINGPROC) (GLfloat value);
typedef void      (*PFNGLBLENDEQUATIONIPROC) (GLuint buf, GLenum mode);
typedef void      (*PFNGLBLENDEQUATIONSEPARATEIPROC) (GLuint buf, GLenum modeRGB, GLenum modeAlpha);
typedef void      (*PFNGLBLENDFUNCIPROC) (GLuint buf, GLenum src, GLenum dst);
typedef void      (*PFNGLBLENDFUNCSEPARATEIPROC) (GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
typedef void      (*PFNGLDRAWARRAYSINDIRECTPROC) (GLenum mode, const void *indirect);
typedef void      (*PFNGLDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void *indirect);
typedef void      (*PFNGLUNIFORM1DPROC) (GLint location, GLdouble x);
typedef void      (*PFNGLUNIFORM2DPROC) (GLint location, GLdouble x, GLdouble y);
typedef void      (*PFNGLUNIFORM3DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z);
typedef void      (*PFNGLUNIFORM4DPROC) (GLint location, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void      (*PFNGLUNIFORM1DVPROC) (GLint location, GLsizei count, const GLdouble *value);
typedef void      (*PFNGLUNIFORM2DVPROC) (GLint location, GLsizei count, const GLdouble *value);
typedef void      (*PFNGLUNIFORM3DVPROC) (GLint location, GLsizei count, const GLdouble *value);
typedef void      (*PFNGLUNIFORM4DVPROC) (GLint location, GLsizei count, const GLdouble *value);
typedef void      (*PFNGLUNIFORMMATRIX2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLUNIFORMMATRIX3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLUNIFORMMATRIX4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLUNIFORMMATRIX2X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLUNIFORMMATRIX2X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLUNIFORMMATRIX3X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLUNIFORMMATRIX3X4DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLUNIFORMMATRIX4X2DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLUNIFORMMATRIX4X3DVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLGETUNIFORMDVPROC) (GLuint program, GLint location, GLdouble *params);
typedef GLint     (*PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC) (GLuint program, GLenum shadertype, const GLchar *name);
typedef GLuint    (*PFNGLGETSUBROUTINEINDEXPROC) (GLuint program, GLenum shadertype, const GLchar *name);
typedef void      (*PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC) (GLuint program, GLenum shadertype, GLuint index, GLenum pname, GLint *values);
typedef void      (*PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
typedef void      (*PFNGLGETACTIVESUBROUTINENAMEPROC) (GLuint program, GLenum shadertype, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
typedef void      (*PFNGLUNIFORMSUBROUTINESUIVPROC) (GLenum shadertype, GLsizei count, const GLuint *indices);
typedef void      (*PFNGLGETUNIFORMSUBROUTINEUIVPROC) (GLenum shadertype, GLint location, GLuint *params);
typedef void      (*PFNGLGETPROGRAMSTAGEIVPROC) (GLuint program, GLenum shadertype, GLenum pname, GLint *values);
typedef void      (*PFNGLPATCHPARAMETERIPROC) (GLenum pname, GLint value);
typedef void      (*PFNGLPATCHPARAMETERFVPROC) (GLenum pname, const GLfloat *values);
typedef void      (*PFNGLBINDTRANSFORMFEEDBACKPROC) (GLenum target, GLuint id);
typedef void      (*PFNGLDELETETRANSFORMFEEDBACKSPROC) (GLsizei n, const GLuint *ids);
typedef void      (*PFNGLGENTRANSFORMFEEDBACKSPROC) (GLsizei n, GLuint *ids);
typedef GLboolean (*PFNGLISTRANSFORMFEEDBACKPROC) (GLuint id);
typedef void      (*PFNGLPAUSETRANSFORMFEEDBACKPROC) (void);
typedef void      (*PFNGLRESUMETRANSFORMFEEDBACKPROC) (void);
typedef void      (*PFNGLDRAWTRANSFORMFEEDBACKPROC) (GLenum mode, GLuint id);
typedef void      (*PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC) (GLenum mode, GLuint id, GLuint stream);
typedef void      (*PFNGLBEGINQUERYINDEXEDPROC) (GLenum target, GLuint index, GLuint id);
typedef void      (*PFNGLENDQUERYINDEXEDPROC) (GLenum target, GLuint index);
typedef void      (*PFNGLGETQUERYINDEXEDIVPROC) (GLenum target, GLuint index, GLenum pname, GLint *params);
extern PFNGLMINSAMPLESHADINGPROC               glMinSampleShading;
extern PFNGLBLENDEQUATIONIPROC                 glBlendEquationi;
extern PFNGLBLENDEQUATIONSEPARATEIPROC         glBlendEquationSeparatei;
extern PFNGLBLENDFUNCIPROC                     glBlendFunci;
extern PFNGLBLENDFUNCSEPARATEIPROC             glBlendFuncSeparatei;
extern PFNGLDRAWARRAYSINDIRECTPROC             glDrawArraysIndirect;
extern PFNGLDRAWELEMENTSINDIRECTPROC           glDrawElementsIndirect;
extern PFNGLUNIFORM1DPROC                      glUniform1d;
extern PFNGLUNIFORM2DPROC                      glUniform2d;
extern PFNGLUNIFORM3DPROC                      glUniform3d;
extern PFNGLUNIFORM4DPROC                      glUniform4d;
extern PFNGLUNIFORM1DVPROC                     glUniform1dv;
extern PFNGLUNIFORM2DVPROC                     glUniform2dv;
extern PFNGLUNIFORM3DVPROC                     glUniform3dv;
extern PFNGLUNIFORM4DVPROC                     glUniform4dv;
extern PFNGLUNIFORMMATRIX2DVPROC               glUniformMatrix2dv;
extern PFNGLUNIFORMMATRIX3DVPROC               glUniformMatrix3dv;
extern PFNGLUNIFORMMATRIX4DVPROC               glUniformMatrix4dv;
extern PFNGLUNIFORMMATRIX2X3DVPROC             glUniformMatrix2x3dv;
extern PFNGLUNIFORMMATRIX2X4DVPROC             glUniformMatrix2x4dv;
extern PFNGLUNIFORMMATRIX3X2DVPROC             glUniformMatrix3x2dv;
extern PFNGLUNIFORMMATRIX3X4DVPROC             glUniformMatrix3x4dv;
extern PFNGLUNIFORMMATRIX4X2DVPROC             glUniformMatrix4x2dv;
extern PFNGLUNIFORMMATRIX4X3DVPROC             glUniformMatrix4x3dv;
extern PFNGLGETUNIFORMDVPROC                   glGetUniformdv;
extern PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC   glGetSubroutineUniformLocation;
extern PFNGLGETSUBROUTINEINDEXPROC             glGetSubroutineIndex;
extern PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC   glGetActiveSubroutineUniformiv;
extern PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName;
extern PFNGLGETACTIVESUBROUTINENAMEPROC        glGetActiveSubroutineName;
extern PFNGLUNIFORMSUBROUTINESUIVPROC          glUniformSubroutinesuiv;
extern PFNGLGETUNIFORMSUBROUTINEUIVPROC        glGetUniformSubroutineuiv;
extern PFNGLGETPROGRAMSTAGEIVPROC              glGetProgramStageiv;
extern PFNGLPATCHPARAMETERIPROC                glPatchParameteri;
extern PFNGLPATCHPARAMETERFVPROC               glPatchParameterfv;
extern PFNGLBINDTRANSFORMFEEDBACKPROC          glBindTransformFeedback;
extern PFNGLDELETETRANSFORMFEEDBACKSPROC       glDeleteTransformFeedbacks;
extern PFNGLGENTRANSFORMFEEDBACKSPROC          glGenTransformFeedbacks;
extern PFNGLISTRANSFORMFEEDBACKPROC            glIsTransformFeedback;
extern PFNGLPAUSETRANSFORMFEEDBACKPROC         glPauseTransformFeedback;
extern PFNGLRESUMETRANSFORMFEEDBACKPROC        glResumeTransformFeedback;
extern PFNGLDRAWTRANSFORMFEEDBACKPROC          glDrawTransformFeedback;
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC    glDrawTransformFeedbackStream;
extern PFNGLBEGINQUERYINDEXEDPROC              glBeginQueryIndexed;
extern PFNGLENDQUERYINDEXEDPROC                glEndQueryIndexed;
extern PFNGLGETQUERYINDEXEDIVPROC              glGetQueryIndexediv;
/* ----GL VERSION 4.1------------------------------------------------------------------------------------------------ */
typedef void      (*PFNGLRELEASESHADERCOMPILERPROC) (void);
typedef void      (*PFNGLSHADERBINARYPROC) (GLsizei count, const GLuint *shaders, GLenum binaryFormat, const void *binary, GLsizei length);
typedef void      (*PFNGLGETSHADERPRECISIONFORMATPROC) (GLenum shadertype, GLenum precisiontype, GLint *range, GLint *precision);
typedef void      (*PFNGLDEPTHRANGEFPROC) (GLfloat n, GLfloat f);
typedef void      (*PFNGLCLEARDEPTHFPROC) (GLfloat d);
typedef void      (*PFNGLGETPROGRAMBINARYPROC) (GLuint program, GLsizei bufSize, GLsizei *length, GLenum *binaryFormat, void *binary);
typedef void      (*PFNGLPROGRAMBINARYPROC) (GLuint program, GLenum binaryFormat, const void *binary, GLsizei length);
typedef void      (*PFNGLPROGRAMPARAMETERIPROC) (GLuint program, GLenum pname, GLint value);
typedef void      (*PFNGLUSEPROGRAMSTAGESPROC) (GLuint pipeline, GLbitfield stages, GLuint program);
typedef void      (*PFNGLACTIVESHADERPROGRAMPROC) (GLuint pipeline, GLuint program);
typedef GLuint    (*PFNGLCREATESHADERPROGRAMVPROC) (GLenum type, GLsizei count, const GLchar *const*strings);
typedef void      (*PFNGLBINDPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void      (*PFNGLDELETEPROGRAMPIPELINESPROC) (GLsizei n, const GLuint *pipelines);
typedef void      (*PFNGLGENPROGRAMPIPELINESPROC) (GLsizei n, GLuint *pipelines);
typedef GLboolean (*PFNGLISPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void      (*PFNGLGETPROGRAMPIPELINEIVPROC) (GLuint pipeline, GLenum pname, GLint *params);
typedef void      (*PFNGLPROGRAMUNIFORM1IPROC) (GLuint program, GLint location, GLint v0);
typedef void      (*PFNGLPROGRAMUNIFORM1IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void      (*PFNGLPROGRAMUNIFORM1FPROC) (GLuint program, GLint location, GLfloat v0);
typedef void      (*PFNGLPROGRAMUNIFORM1FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORM1DPROC) (GLuint program, GLint location, GLdouble v0);
typedef void      (*PFNGLPROGRAMUNIFORM1DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORM1UIPROC) (GLuint program, GLint location, GLuint v0);
typedef void      (*PFNGLPROGRAMUNIFORM1UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void      (*PFNGLPROGRAMUNIFORM2IPROC) (GLuint program, GLint location, GLint v0, GLint v1);
typedef void      (*PFNGLPROGRAMUNIFORM2IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void      (*PFNGLPROGRAMUNIFORM2FPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1);
typedef void      (*PFNGLPROGRAMUNIFORM2FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORM2DPROC) (GLuint program, GLint location, GLdouble v0, GLdouble v1);
typedef void      (*PFNGLPROGRAMUNIFORM2DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORM2UIPROC) (GLuint program, GLint location, GLuint v0, GLuint v1);
typedef void      (*PFNGLPROGRAMUNIFORM2UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void      (*PFNGLPROGRAMUNIFORM3IPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2);
typedef void      (*PFNGLPROGRAMUNIFORM3IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void      (*PFNGLPROGRAMUNIFORM3FPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void      (*PFNGLPROGRAMUNIFORM3FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORM3DPROC) (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2);
typedef void      (*PFNGLPROGRAMUNIFORM3DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORM3UIPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void      (*PFNGLPROGRAMUNIFORM3UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void      (*PFNGLPROGRAMUNIFORM4IPROC) (GLuint program, GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void      (*PFNGLPROGRAMUNIFORM4IVPROC) (GLuint program, GLint location, GLsizei count, const GLint *value);
typedef void      (*PFNGLPROGRAMUNIFORM4FPROC) (GLuint program, GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void      (*PFNGLPROGRAMUNIFORM4FVPROC) (GLuint program, GLint location, GLsizei count, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORM4DPROC) (GLuint program, GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
typedef void      (*PFNGLPROGRAMUNIFORM4DVPROC) (GLuint program, GLint location, GLsizei count, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORM4UIPROC) (GLuint program, GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void      (*PFNGLPROGRAMUNIFORM4UIVPROC) (GLuint program, GLint location, GLsizei count, const GLuint *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC) (GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLdouble *value);
typedef void      (*PFNGLVALIDATEPROGRAMPIPELINEPROC) (GLuint pipeline);
typedef void      (*PFNGLGETPROGRAMPIPELINEINFOLOGPROC) (GLuint pipeline, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void      (*PFNGLVERTEXATTRIBL1DPROC) (GLuint index, GLdouble x);
typedef void      (*PFNGLVERTEXATTRIBL2DPROC) (GLuint index, GLdouble x, GLdouble y);
typedef void      (*PFNGLVERTEXATTRIBL3DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void      (*PFNGLVERTEXATTRIBL4DPROC) (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void      (*PFNGLVERTEXATTRIBL1DVPROC) (GLuint index, const GLdouble *v);
typedef void      (*PFNGLVERTEXATTRIBL2DVPROC) (GLuint index, const GLdouble *v);
typedef void      (*PFNGLVERTEXATTRIBL3DVPROC) (GLuint index, const GLdouble *v);
typedef void      (*PFNGLVERTEXATTRIBL4DVPROC) (GLuint index, const GLdouble *v);
typedef void      (*PFNGLVERTEXATTRIBLPOINTERPROC) (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void      (*PFNGLGETVERTEXATTRIBLDVPROC) (GLuint index, GLenum pname, GLdouble *params);
typedef void      (*PFNGLVIEWPORTARRAYVPROC) (GLuint first, GLsizei count, const GLfloat *v);
typedef void      (*PFNGLVIEWPORTINDEXEDFPROC) (GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);
typedef void      (*PFNGLVIEWPORTINDEXEDFVPROC) (GLuint index, const GLfloat *v);
typedef void      (*PFNGLSCISSORARRAYVPROC) (GLuint first, GLsizei count, const GLint *v);
typedef void      (*PFNGLSCISSORINDEXEDPROC) (GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
typedef void      (*PFNGLSCISSORINDEXEDVPROC) (GLuint index, const GLint *v);
typedef void      (*PFNGLDEPTHRANGEARRAYVPROC) (GLuint first, GLsizei count, const GLdouble *v);
typedef void      (*PFNGLDEPTHRANGEINDEXEDPROC) (GLuint index, GLdouble n, GLdouble f);
typedef void      (*PFNGLGETFLOATI_VPROC) (GLenum target, GLuint index, GLfloat *data);
typedef void      (*PFNGLGETDOUBLEI_VPROC) (GLenum target, GLuint index, GLdouble *data);
extern PFNGLRELEASESHADERCOMPILERPROC     glReleaseShaderCompiler;
extern PFNGLSHADERBINARYPROC              glShaderBinary;
extern PFNGLGETSHADERPRECISIONFORMATPROC  glGetShaderPrecisionFormat;
extern PFNGLDEPTHRANGEFPROC               glDepthRangef;
extern PFNGLCLEARDEPTHFPROC               glClearDepthf;
extern PFNGLGETPROGRAMBINARYPROC          glGetProgramBinary;
extern PFNGLPROGRAMBINARYPROC             glProgramBinary;
extern PFNGLPROGRAMPARAMETERIPROC         glProgramParameteri;
extern PFNGLUSEPROGRAMSTAGESPROC          glUseProgramStages;
extern PFNGLACTIVESHADERPROGRAMPROC       glActiveShaderProgram;
extern PFNGLCREATESHADERPROGRAMVPROC      glCreateShaderProgramv;
extern PFNGLBINDPROGRAMPIPELINEPROC       glBindProgramPipeline;
extern PFNGLDELETEPROGRAMPIPELINESPROC    glDeleteProgramPipelines;
extern PFNGLGENPROGRAMPIPELINESPROC       glGenProgramPipelines;
extern PFNGLISPROGRAMPIPELINEPROC         glIsProgramPipeline;
extern PFNGLGETPROGRAMPIPELINEIVPROC      glGetProgramPipelineiv;
extern PFNGLPROGRAMUNIFORM1IPROC          glProgramUniform1i;
extern PFNGLPROGRAMUNIFORM1IVPROC         glProgramUniform1iv;
extern PFNGLPROGRAMUNIFORM1FPROC          glProgramUniform1f;
extern PFNGLPROGRAMUNIFORM1FVPROC         glProgramUniform1fv;
extern PFNGLPROGRAMUNIFORM1DPROC          glProgramUniform1d;
extern PFNGLPROGRAMUNIFORM1DVPROC         glProgramUniform1dv;
extern PFNGLPROGRAMUNIFORM1UIPROC         glProgramUniform1ui;
extern PFNGLPROGRAMUNIFORM1UIVPROC        glProgramUniform1uiv;
extern PFNGLPROGRAMUNIFORM2IPROC          glProgramUniform2i;
extern PFNGLPROGRAMUNIFORM2IVPROC         glProgramUniform2iv;
extern PFNGLPROGRAMUNIFORM2FPROC          glProgramUniform2f;
extern PFNGLPROGRAMUNIFORM2FVPROC         glProgramUniform2fv;
extern PFNGLPROGRAMUNIFORM2DPROC          glProgramUniform2d;
extern PFNGLPROGRAMUNIFORM2DVPROC         glProgramUniform2dv;
extern PFNGLPROGRAMUNIFORM2UIPROC         glProgramUniform2ui;
extern PFNGLPROGRAMUNIFORM2UIVPROC        glProgramUniform2uiv;
extern PFNGLPROGRAMUNIFORM3IPROC          glProgramUniform3i;
extern PFNGLPROGRAMUNIFORM3IVPROC         glProgramUniform3iv;
extern PFNGLPROGRAMUNIFORM3FPROC          glProgramUniform3f;
extern PFNGLPROGRAMUNIFORM3FVPROC         glProgramUniform3fv;
extern PFNGLPROGRAMUNIFORM3DPROC          glProgramUniform3d;
extern PFNGLPROGRAMUNIFORM3DVPROC         glProgramUniform3dv;
extern PFNGLPROGRAMUNIFORM3UIPROC         glProgramUniform3ui;
extern PFNGLPROGRAMUNIFORM3UIVPROC        glProgramUniform3uiv;
extern PFNGLPROGRAMUNIFORM4IPROC          glProgramUniform4i;
extern PFNGLPROGRAMUNIFORM4IVPROC         glProgramUniform4iv;
extern PFNGLPROGRAMUNIFORM4FPROC          glProgramUniform4f;
extern PFNGLPROGRAMUNIFORM4FVPROC         glProgramUniform4fv;
extern PFNGLPROGRAMUNIFORM4DPROC          glProgramUniform4d;
extern PFNGLPROGRAMUNIFORM4DVPROC         glProgramUniform4dv;
extern PFNGLPROGRAMUNIFORM4UIPROC         glProgramUniform4ui;
extern PFNGLPROGRAMUNIFORM4UIVPROC        glProgramUniform4uiv;
extern PFNGLPROGRAMUNIFORMMATRIX2FVPROC   glProgramUniformMatrix2fv;
extern PFNGLPROGRAMUNIFORMMATRIX3FVPROC   glProgramUniformMatrix3fv;
extern PFNGLPROGRAMUNIFORMMATRIX4FVPROC   glProgramUniformMatrix4fv;
extern PFNGLPROGRAMUNIFORMMATRIX2DVPROC   glProgramUniformMatrix2dv;
extern PFNGLPROGRAMUNIFORMMATRIX3DVPROC   glProgramUniformMatrix3dv;
extern PFNGLPROGRAMUNIFORMMATRIX4DVPROC   glProgramUniformMatrix4dv;
extern PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv;
extern PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv;
extern PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv;
extern PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv;
extern PFNGLVALIDATEPROGRAMPIPELINEPROC   glValidateProgramPipeline;
extern PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog;
extern PFNGLVERTEXATTRIBL1DPROC           glVertexAttribL1d;
extern PFNGLVERTEXATTRIBL2DPROC           glVertexAttribL2d;
extern PFNGLVERTEXATTRIBL3DPROC           glVertexAttribL3d;
extern PFNGLVERTEXATTRIBL4DPROC           glVertexAttribL4d;
extern PFNGLVERTEXATTRIBL1DVPROC          glVertexAttribL1dv;
extern PFNGLVERTEXATTRIBL2DVPROC          glVertexAttribL2dv;
extern PFNGLVERTEXATTRIBL3DVPROC          glVertexAttribL3dv;
extern PFNGLVERTEXATTRIBL4DVPROC          glVertexAttribL4dv;
extern PFNGLVERTEXATTRIBLPOINTERPROC      glVertexAttribLPointer;
extern PFNGLGETVERTEXATTRIBLDVPROC        glGetVertexAttribLdv;
extern PFNGLVIEWPORTARRAYVPROC            glViewportArrayv;
extern PFNGLVIEWPORTINDEXEDFPROC          glViewportIndexedf;
extern PFNGLVIEWPORTINDEXEDFVPROC         glViewportIndexedfv;
extern PFNGLSCISSORARRAYVPROC             glScissorArrayv;
extern PFNGLSCISSORINDEXEDPROC            glScissorIndexed;
extern PFNGLSCISSORINDEXEDVPROC           glScissorIndexedv;
extern PFNGLDEPTHRANGEARRAYVPROC          glDepthRangeArrayv;
extern PFNGLDEPTHRANGEINDEXEDPROC         glDepthRangeIndexed;
extern PFNGLGETFLOATI_VPROC               glGetFloati_v;
extern PFNGLGETDOUBLEI_VPROC              glGetDoublei_v;
/* ----GL VERSION 4.2------------------------------------------------------------------------------------------------ */
typedef void (*PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLint first, GLsizei count, GLsizei instancecount, GLuint baseinstance);
typedef void (*PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLuint baseinstance);
typedef void (*PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex, GLuint baseinstance);
typedef void (*PFNGLGETINTERNALFORMATIVPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint *params);
typedef void (*PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC) (GLuint program, GLuint bufferIndex, GLenum pname, GLint *params);
typedef void (*PFNGLBINDIMAGETEXTUREPROC) (GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
typedef void (*PFNGLMEMORYBARRIERPROC) (GLbitfield barriers);
typedef void (*PFNGLTEXSTORAGE1DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void (*PFNGLTEXSTORAGE2DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void (*PFNGLTEXSTORAGE3DPROC) (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void (*PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC) (GLenum mode, GLuint id, GLsizei instancecount);
typedef void (*PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC) (GLenum mode, GLuint id, GLuint stream, GLsizei instancecount);
extern PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC             glDrawArraysInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC           glDrawElementsInstancedBaseInstance;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance;
extern PFNGLGETINTERNALFORMATIVPROC                         glGetInternalformativ;
extern PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC              glGetActiveAtomicCounterBufferiv;
extern PFNGLBINDIMAGETEXTUREPROC                            glBindImageTexture;
extern PFNGLMEMORYBARRIERPROC                               glMemoryBarrier;
extern PFNGLTEXSTORAGE1DPROC                                glTexStorage1D;
extern PFNGLTEXSTORAGE2DPROC                                glTexStorage2D;
extern PFNGLTEXSTORAGE3DPROC                                glTexStorage3D;
extern PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC              glDrawTransformFeedbackInstanced;
extern PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC        glDrawTransformFeedbackStreamInstanced;
/* ----GL VERSION 4.3------------------------------------------------------------------------------------------------ */
typedef void   (*PFNGLCLEARBUFFERDATAPROC) (GLenum target, GLenum internalformat, GLenum format, GLenum type, const void *data);
typedef void   (*PFNGLCLEARBUFFERSUBDATAPROC) (GLenum target, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
typedef void   (*PFNGLDISPATCHCOMPUTEPROC) (GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z);
typedef void   (*PFNGLDISPATCHCOMPUTEINDIRECTPROC) (GLintptr indirect);
typedef void   (*PFNGLCOPYIMAGESUBDATAPROC) (GLuint srcName, GLenum srcTarget, GLint srcLevel, GLint srcX, GLint srcY, GLint srcZ, GLuint dstName, GLenum dstTarget, GLint dstLevel, GLint dstX, GLint dstY, GLint dstZ, GLsizei srcWidth, GLsizei srcHeight, GLsizei srcDepth);
typedef void   (*PFNGLFRAMEBUFFERPARAMETERIPROC) (GLenum target, GLenum pname, GLint param);
typedef void   (*PFNGLGETFRAMEBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void   (*PFNGLGETINTERNALFORMATI64VPROC) (GLenum target, GLenum internalformat, GLenum pname, GLsizei count, GLint64 *params);
typedef void   (*PFNGLINVALIDATETEXSUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth);
typedef void   (*PFNGLINVALIDATETEXIMAGEPROC) (GLuint texture, GLint level);
typedef void   (*PFNGLINVALIDATEBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void   (*PFNGLINVALIDATEBUFFERDATAPROC) (GLuint buffer);
typedef void   (*PFNGLINVALIDATEFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum *attachments);
typedef void   (*PFNGLINVALIDATESUBFRAMEBUFFERPROC) (GLenum target, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void   (*PFNGLMULTIDRAWARRAYSINDIRECTPROC) (GLenum mode, const void *indirect, GLsizei drawcount, GLsizei stride);
typedef void   (*PFNGLMULTIDRAWELEMENTSINDIRECTPROC) (GLenum mode, GLenum type, const void *indirect, GLsizei drawcount, GLsizei stride);
typedef void   (*PFNGLGETPROGRAMINTERFACEIVPROC) (GLuint program, GLenum programInterface, GLenum pname, GLint *params);
typedef GLuint (*PFNGLGETPROGRAMRESOURCEINDEXPROC) (GLuint program, GLenum programInterface, const GLchar *name);
typedef void   (*PFNGLGETPROGRAMRESOURCENAMEPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei bufSize, GLsizei *length, GLchar *name);
typedef void   (*PFNGLGETPROGRAMRESOURCEIVPROC) (GLuint program, GLenum programInterface, GLuint index, GLsizei propCount, const GLenum *props, GLsizei count, GLsizei *length, GLint *params);
typedef GLint  (*PFNGLGETPROGRAMRESOURCELOCATIONPROC) (GLuint program, GLenum programInterface, const GLchar *name);
typedef GLint  (*PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC) (GLuint program, GLenum programInterface, const GLchar *name);
typedef void   (*PFNGLSHADERSTORAGEBLOCKBINDINGPROC) (GLuint program, GLuint storageBlockIndex, GLuint storageBlockBinding);
typedef void   (*PFNGLTEXBUFFERRANGEPROC) (GLenum target, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void   (*PFNGLTEXSTORAGE2DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void   (*PFNGLTEXSTORAGE3DMULTISAMPLEPROC) (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void   (*PFNGLTEXTUREVIEWPROC) (GLuint texture, GLenum target, GLuint origtexture, GLenum internalformat, GLuint minlevel, GLuint numlevels, GLuint minlayer, GLuint numlayers);
typedef void   (*PFNGLBINDVERTEXBUFFERPROC) (GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void   (*PFNGLVERTEXATTRIBFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void   (*PFNGLVERTEXATTRIBIFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void   (*PFNGLVERTEXATTRIBLFORMATPROC) (GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void   (*PFNGLVERTEXATTRIBBINDINGPROC) (GLuint attribindex, GLuint bindingindex);
typedef void   (*PFNGLVERTEXBINDINGDIVISORPROC) (GLuint bindingindex, GLuint divisor);
typedef void   (*PFNGLDEBUGMESSAGECONTROLPROC) (GLenum source, GLenum type, GLenum severity, GLsizei count, const GLuint *ids, GLboolean enabled);
typedef void   (*PFNGLDEBUGMESSAGEINSERTPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *buf);
typedef void   (*PFNGLDEBUGMESSAGECALLBACKPROC) (GLDEBUGPROC callback, const void *userParam);
typedef GLuint (*PFNGLGETDEBUGMESSAGELOGPROC) (GLuint count, GLsizei bufSize, GLenum *sources, GLenum *types, GLuint *ids, GLenum *severities, GLsizei *lengths, GLchar *messageLog);
typedef void   (*PFNGLPUSHDEBUGGROUPPROC) (GLenum source, GLuint id, GLsizei length, const GLchar *message);
typedef void   (*PFNGLPOPDEBUGGROUPPROC) (void);
typedef void   (*PFNGLOBJECTLABELPROC) (GLenum identifier, GLuint name, GLsizei length, const GLchar *label);
typedef void   (*PFNGLGETOBJECTLABELPROC) (GLenum identifier, GLuint name, GLsizei bufSize, GLsizei *length, GLchar *label);
typedef void   (*PFNGLOBJECTPTRLABELPROC) (const void *ptr, GLsizei length, const GLchar *label);
typedef void   (*PFNGLGETOBJECTPTRLABELPROC) (const void *ptr, GLsizei bufSize, GLsizei *length, GLchar *label);
extern PFNGLCLEARBUFFERDATAPROC                 glClearBufferData;
extern PFNGLCLEARBUFFERSUBDATAPROC              glClearBufferSubData;
extern PFNGLDISPATCHCOMPUTEPROC                 glDispatchCompute;
extern PFNGLDISPATCHCOMPUTEINDIRECTPROC         glDispatchComputeIndirect;
extern PFNGLCOPYIMAGESUBDATAPROC                glCopyImageSubData;
extern PFNGLFRAMEBUFFERPARAMETERIPROC           glFramebufferParameteri;
extern PFNGLGETFRAMEBUFFERPARAMETERIVPROC       glGetFramebufferParameteriv;
extern PFNGLGETINTERNALFORMATI64VPROC           glGetInternalformati64v;
extern PFNGLINVALIDATETEXSUBIMAGEPROC           glInvalidateTexSubImage;
extern PFNGLINVALIDATETEXIMAGEPROC              glInvalidateTexImage;
extern PFNGLINVALIDATEBUFFERSUBDATAPROC         glInvalidateBufferSubData;
extern PFNGLINVALIDATEBUFFERDATAPROC            glInvalidateBufferData;
extern PFNGLINVALIDATEFRAMEBUFFERPROC           glInvalidateFramebuffer;
extern PFNGLINVALIDATESUBFRAMEBUFFERPROC        glInvalidateSubFramebuffer;
extern PFNGLMULTIDRAWARRAYSINDIRECTPROC         glMultiDrawArraysIndirect;
extern PFNGLMULTIDRAWELEMENTSINDIRECTPROC       glMultiDrawElementsIndirect;
extern PFNGLGETPROGRAMINTERFACEIVPROC           glGetProgramInterfaceiv;
extern PFNGLGETPROGRAMRESOURCEINDEXPROC         glGetProgramResourceIndex;
extern PFNGLGETPROGRAMRESOURCENAMEPROC          glGetProgramResourceName;
extern PFNGLGETPROGRAMRESOURCEIVPROC            glGetProgramResourceiv;
extern PFNGLGETPROGRAMRESOURCELOCATIONPROC      glGetProgramResourceLocation;
extern PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex;
extern PFNGLSHADERSTORAGEBLOCKBINDINGPROC       glShaderStorageBlockBinding;
extern PFNGLTEXBUFFERRANGEPROC                  glTexBufferRange;
extern PFNGLTEXSTORAGE2DMULTISAMPLEPROC         glTexStorage2DMultisample;
extern PFNGLTEXSTORAGE3DMULTISAMPLEPROC         glTexStorage3DMultisample;
extern PFNGLTEXTUREVIEWPROC                     glTextureView;
extern PFNGLBINDVERTEXBUFFERPROC                glBindVertexBuffer;
extern PFNGLVERTEXATTRIBFORMATPROC              glVertexAttribFormat;
extern PFNGLVERTEXATTRIBIFORMATPROC             glVertexAttribIFormat;
extern PFNGLVERTEXATTRIBLFORMATPROC             glVertexAttribLFormat;
extern PFNGLVERTEXATTRIBBINDINGPROC             glVertexAttribBinding;
extern PFNGLVERTEXBINDINGDIVISORPROC            glVertexBindingDivisor;
extern PFNGLDEBUGMESSAGECONTROLPROC             glDebugMessageControl;
extern PFNGLDEBUGMESSAGEINSERTPROC              glDebugMessageInsert;
extern PFNGLDEBUGMESSAGECALLBACKPROC            glDebugMessageCallback;
extern PFNGLGETDEBUGMESSAGELOGPROC              glGetDebugMessageLog;
extern PFNGLPUSHDEBUGGROUPPROC                  glPushDebugGroup;
extern PFNGLPOPDEBUGGROUPPROC                   glPopDebugGroup;
extern PFNGLOBJECTLABELPROC                     glObjectLabel;
extern PFNGLGETOBJECTLABELPROC                  glGetObjectLabel;
extern PFNGLOBJECTPTRLABELPROC                  glObjectPtrLabel;
extern PFNGLGETOBJECTPTRLABELPROC               glGetObjectPtrLabel;
/* ----GL VERSION 4.4------------------------------------------------------------------------------------------------ */
typedef void (*PFNGLBUFFERSTORAGEPROC) (GLenum target, GLsizeiptr size, const void *data, GLbitfield flags);
typedef void (*PFNGLCLEARTEXIMAGEPROC) (GLuint texture, GLint level, GLenum format, GLenum type, const void *data);
typedef void (*PFNGLCLEARTEXSUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *data);
typedef void (*PFNGLBINDBUFFERSBASEPROC) (GLenum target, GLuint first, GLsizei count, const GLuint *buffers);
typedef void (*PFNGLBINDBUFFERSRANGEPROC) (GLenum target, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizeiptr *sizes);
typedef void (*PFNGLBINDTEXTURESPROC) (GLuint first, GLsizei count, const GLuint *textures);
typedef void (*PFNGLBINDSAMPLERSPROC) (GLuint first, GLsizei count, const GLuint *samplers);
typedef void (*PFNGLBINDIMAGETEXTURESPROC) (GLuint first, GLsizei count, const GLuint *textures);
typedef void (*PFNGLBINDVERTEXBUFFERSPROC) (GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
extern PFNGLBUFFERSTORAGEPROC     glBufferStorage;
extern PFNGLCLEARTEXIMAGEPROC     glClearTexImage;
extern PFNGLCLEARTEXSUBIMAGEPROC  glClearTexSubImage;
extern PFNGLBINDBUFFERSBASEPROC   glBindBuffersBase;
extern PFNGLBINDBUFFERSRANGEPROC  glBindBuffersRange;
extern PFNGLBINDTEXTURESPROC      glBindTextures;
extern PFNGLBINDSAMPLERSPROC      glBindSamplers;
extern PFNGLBINDIMAGETEXTURESPROC glBindImageTextures;
extern PFNGLBINDVERTEXBUFFERSPROC glBindVertexBuffers;
/* ----GL VERSION 4.5------------------------------------------------------------------------------------------------ */
typedef void      (*PFNGLCLIPCONTROLPROC) (GLenum origin, GLenum depth);
typedef void      (*PFNGLCREATETRANSFORMFEEDBACKSPROC) (GLsizei n, GLuint *ids);
typedef void      (*PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC) (GLuint xfb, GLuint index, GLuint buffer);
typedef void      (*PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC) (GLuint xfb, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void      (*PFNGLGETTRANSFORMFEEDBACKIVPROC) (GLuint xfb, GLenum pname, GLint *param);
typedef void      (*PFNGLGETTRANSFORMFEEDBACKI_VPROC) (GLuint xfb, GLenum pname, GLuint index, GLint *param);
typedef void      (*PFNGLGETTRANSFORMFEEDBACKI64_VPROC) (GLuint xfb, GLenum pname, GLuint index, GLint64 *param);
typedef void      (*PFNGLCREATEBUFFERSPROC) (GLsizei n, GLuint *buffers);
typedef void      (*PFNGLNAMEDBUFFERSTORAGEPROC) (GLuint buffer, GLsizeiptr size, const void *data, GLbitfield flags);
typedef void      (*PFNGLNAMEDBUFFERDATAPROC) (GLuint buffer, GLsizeiptr size, const void *data, GLenum usage);
typedef void      (*PFNGLNAMEDBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, const void *data);
typedef void      (*PFNGLCOPYNAMEDBUFFERSUBDATAPROC) (GLuint readBuffer, GLuint writeBuffer, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void      (*PFNGLCLEARNAMEDBUFFERDATAPROC) (GLuint buffer, GLenum internalformat, GLenum format, GLenum type, const void *data);
typedef void      (*PFNGLCLEARNAMEDBUFFERSUBDATAPROC) (GLuint buffer, GLenum internalformat, GLintptr offset, GLsizeiptr size, GLenum format, GLenum type, const void *data);
typedef void     *(*PFNGLMAPNAMEDBUFFERPROC) (GLuint buffer, GLenum access);
typedef void     *(*PFNGLMAPNAMEDBUFFERRANGEPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef GLboolean (*PFNGLUNMAPNAMEDBUFFERPROC) (GLuint buffer);
typedef void      (*PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC) (GLuint buffer, GLintptr offset, GLsizeiptr length);
typedef void      (*PFNGLGETNAMEDBUFFERPARAMETERIVPROC) (GLuint buffer, GLenum pname, GLint *params);
typedef void      (*PFNGLGETNAMEDBUFFERPARAMETERI64VPROC) (GLuint buffer, GLenum pname, GLint64 *params);
typedef void      (*PFNGLGETNAMEDBUFFERPOINTERVPROC) (GLuint buffer, GLenum pname, void **params);
typedef void      (*PFNGLGETNAMEDBUFFERSUBDATAPROC) (GLuint buffer, GLintptr offset, GLsizeiptr size, void *data);
typedef void      (*PFNGLCREATEFRAMEBUFFERSPROC) (GLsizei n, GLuint *framebuffers);
typedef void      (*PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC) (GLuint framebuffer, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void      (*PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC) (GLuint framebuffer, GLenum pname, GLint param);
typedef void      (*PFNGLNAMEDFRAMEBUFFERTEXTUREPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level);
typedef void      (*PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC) (GLuint framebuffer, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void      (*PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC) (GLuint framebuffer, GLenum buf);
typedef void      (*PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC) (GLuint framebuffer, GLsizei n, const GLenum *bufs);
typedef void      (*PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC) (GLuint framebuffer, GLenum src);
typedef void      (*PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC) (GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments);
typedef void      (*PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC) (GLuint framebuffer, GLsizei numAttachments, const GLenum *attachments, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void      (*PFNGLCLEARNAMEDFRAMEBUFFERIVPROC) (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLint *value);
typedef void      (*PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC) (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLuint *value);
typedef void      (*PFNGLCLEARNAMEDFRAMEBUFFERFVPROC) (GLuint framebuffer, GLenum buffer, GLint drawbuffer, const GLfloat *value);
typedef void      (*PFNGLCLEARNAMEDFRAMEBUFFERFIPROC) (GLuint framebuffer, GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef void      (*PFNGLBLITNAMEDFRAMEBUFFERPROC) (GLuint readFramebuffer, GLuint drawFramebuffer, GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef GLenum    (*PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC) (GLuint framebuffer, GLenum target);
typedef void      (*PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC) (GLuint framebuffer, GLenum pname, GLint *param);
typedef void      (*PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLuint framebuffer, GLenum attachment, GLenum pname, GLint *params);
typedef void      (*PFNGLCREATERENDERBUFFERSPROC) (GLsizei n, GLuint *renderbuffers);
typedef void      (*PFNGLNAMEDRENDERBUFFERSTORAGEPROC) (GLuint renderbuffer, GLenum internalformat, GLsizei width, GLsizei height);
typedef void      (*PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC) (GLuint renderbuffer, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void      (*PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC) (GLuint renderbuffer, GLenum pname, GLint *params);
typedef void      (*PFNGLCREATETEXTURESPROC) (GLenum target, GLsizei n, GLuint *textures);
typedef void      (*PFNGLTEXTUREBUFFERPROC) (GLuint texture, GLenum internalformat, GLuint buffer);
typedef void      (*PFNGLTEXTUREBUFFERRANGEPROC) (GLuint texture, GLenum internalformat, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void      (*PFNGLTEXTURESTORAGE1DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void      (*PFNGLTEXTURESTORAGE2DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void      (*PFNGLTEXTURESTORAGE3DPROC) (GLuint texture, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void      (*PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC) (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void      (*PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC) (GLuint texture, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void      (*PFNGLTEXTURESUBIMAGE1DPROC) (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void      (*PFNGLTEXTURESUBIMAGE2DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void      (*PFNGLTEXTURESUBIMAGE3DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
typedef void      (*PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC) (GLuint texture, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
typedef void      (*PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
typedef void      (*PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void      (*PFNGLCOPYTEXTURESUBIMAGE1DPROC) (GLuint texture, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void      (*PFNGLCOPYTEXTURESUBIMAGE2DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void      (*PFNGLCOPYTEXTURESUBIMAGE3DPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void      (*PFNGLTEXTUREPARAMETERFPROC) (GLuint texture, GLenum pname, GLfloat param);
typedef void      (*PFNGLTEXTUREPARAMETERFVPROC) (GLuint texture, GLenum pname, const GLfloat *param);
typedef void      (*PFNGLTEXTUREPARAMETERIPROC) (GLuint texture, GLenum pname, GLint param);
typedef void      (*PFNGLTEXTUREPARAMETERIIVPROC) (GLuint texture, GLenum pname, const GLint *params);
typedef void      (*PFNGLTEXTUREPARAMETERIUIVPROC) (GLuint texture, GLenum pname, const GLuint *params);
typedef void      (*PFNGLTEXTUREPARAMETERIVPROC) (GLuint texture, GLenum pname, const GLint *param);
typedef void      (*PFNGLGENERATETEXTUREMIPMAPPROC) (GLuint texture);
typedef void      (*PFNGLBINDTEXTUREUNITPROC) (GLuint unit, GLuint texture);
typedef void      (*PFNGLGETTEXTUREIMAGEPROC) (GLuint texture, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
typedef void      (*PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC) (GLuint texture, GLint level, GLsizei bufSize, void *pixels);
typedef void      (*PFNGLGETTEXTURELEVELPARAMETERFVPROC) (GLuint texture, GLint level, GLenum pname, GLfloat *params);
typedef void      (*PFNGLGETTEXTURELEVELPARAMETERIVPROC) (GLuint texture, GLint level, GLenum pname, GLint *params);
typedef void      (*PFNGLGETTEXTUREPARAMETERFVPROC) (GLuint texture, GLenum pname, GLfloat *params);
typedef void      (*PFNGLGETTEXTUREPARAMETERIIVPROC) (GLuint texture, GLenum pname, GLint *params);
typedef void      (*PFNGLGETTEXTUREPARAMETERIUIVPROC) (GLuint texture, GLenum pname, GLuint *params);
typedef void      (*PFNGLGETTEXTUREPARAMETERIVPROC) (GLuint texture, GLenum pname, GLint *params);
typedef void      (*PFNGLCREATEVERTEXARRAYSPROC) (GLsizei n, GLuint *arrays);
typedef void      (*PFNGLDISABLEVERTEXARRAYATTRIBPROC) (GLuint vaobj, GLuint index);
typedef void      (*PFNGLENABLEVERTEXARRAYATTRIBPROC) (GLuint vaobj, GLuint index);
typedef void      (*PFNGLVERTEXARRAYELEMENTBUFFERPROC) (GLuint vaobj, GLuint buffer);
typedef void      (*PFNGLVERTEXARRAYVERTEXBUFFERPROC) (GLuint vaobj, GLuint bindingindex, GLuint buffer, GLintptr offset, GLsizei stride);
typedef void      (*PFNGLVERTEXARRAYVERTEXBUFFERSPROC) (GLuint vaobj, GLuint first, GLsizei count, const GLuint *buffers, const GLintptr *offsets, const GLsizei *strides);
typedef void      (*PFNGLVERTEXARRAYATTRIBBINDINGPROC) (GLuint vaobj, GLuint attribindex, GLuint bindingindex);
typedef void      (*PFNGLVERTEXARRAYATTRIBFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLboolean normalized, GLuint relativeoffset);
typedef void      (*PFNGLVERTEXARRAYATTRIBIFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void      (*PFNGLVERTEXARRAYATTRIBLFORMATPROC) (GLuint vaobj, GLuint attribindex, GLint size, GLenum type, GLuint relativeoffset);
typedef void      (*PFNGLVERTEXARRAYBINDINGDIVISORPROC) (GLuint vaobj, GLuint bindingindex, GLuint divisor);
typedef void      (*PFNGLGETVERTEXARRAYIVPROC) (GLuint vaobj, GLenum pname, GLint *param);
typedef void      (*PFNGLGETVERTEXARRAYINDEXEDIVPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint *param);
typedef void      (*PFNGLGETVERTEXARRAYINDEXED64IVPROC) (GLuint vaobj, GLuint index, GLenum pname, GLint64 *param);
typedef void      (*PFNGLCREATESAMPLERSPROC) (GLsizei n, GLuint *samplers);
typedef void      (*PFNGLCREATEPROGRAMPIPELINESPROC) (GLsizei n, GLuint *pipelines);
typedef void      (*PFNGLCREATEQUERIESPROC) (GLenum target, GLsizei n, GLuint *ids);
typedef void      (*PFNGLGETQUERYBUFFEROBJECTI64VPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void      (*PFNGLGETQUERYBUFFEROBJECTIVPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void      (*PFNGLGETQUERYBUFFEROBJECTUI64VPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void      (*PFNGLGETQUERYBUFFEROBJECTUIVPROC) (GLuint id, GLuint buffer, GLenum pname, GLintptr offset);
typedef void      (*PFNGLMEMORYBARRIERBYREGIONPROC) (GLbitfield barriers);
typedef void      (*PFNGLGETTEXTURESUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
typedef void      (*PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC) (GLuint texture, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLsizei bufSize, void *pixels);
typedef GLenum    (*PFNGLGETGRAPHICSRESETSTATUSPROC) (void);
typedef void      (*PFNGLGETNCOMPRESSEDTEXIMAGEPROC) (GLenum target, GLint lod, GLsizei bufSize, void *pixels);
typedef void      (*PFNGLGETNTEXIMAGEPROC) (GLenum target, GLint level, GLenum format, GLenum type, GLsizei bufSize, void *pixels);
typedef void      (*PFNGLGETNUNIFORMDVPROC) (GLuint program, GLint location, GLsizei bufSize, GLdouble *params);
typedef void      (*PFNGLGETNUNIFORMFVPROC) (GLuint program, GLint location, GLsizei bufSize, GLfloat *params);
typedef void      (*PFNGLGETNUNIFORMIVPROC) (GLuint program, GLint location, GLsizei bufSize, GLint *params);
typedef void      (*PFNGLGETNUNIFORMUIVPROC) (GLuint program, GLint location, GLsizei bufSize, GLuint *params);
typedef void      (*PFNGLREADNPIXELSPROC) (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLsizei bufSize, void *data);
typedef void      (*PFNGLTEXTUREBARRIERPROC) (void);
extern  PFNGLCLIPCONTROLPROC                              glClipControl;
extern  PFNGLCREATETRANSFORMFEEDBACKSPROC                 glCreateTransformFeedbacks;
extern  PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC              glTransformFeedbackBufferBase;
extern  PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC             glTransformFeedbackBufferRange;
extern  PFNGLGETTRANSFORMFEEDBACKIVPROC                   glGetTransformFeedbackiv;
extern  PFNGLGETTRANSFORMFEEDBACKI_VPROC                  glGetTransformFeedbacki_v;
extern  PFNGLGETTRANSFORMFEEDBACKI64_VPROC                glGetTransformFeedbacki64_v;
extern  PFNGLCREATEBUFFERSPROC                            glCreateBuffers;
extern  PFNGLNAMEDBUFFERSTORAGEPROC                       glNamedBufferStorage;
extern  PFNGLNAMEDBUFFERDATAPROC                          glNamedBufferData;
extern  PFNGLNAMEDBUFFERSUBDATAPROC                       glNamedBufferSubData;
extern  PFNGLCOPYNAMEDBUFFERSUBDATAPROC                   glCopyNamedBufferSubData;
extern  PFNGLCLEARNAMEDBUFFERDATAPROC                     glClearNamedBufferData;
extern  PFNGLCLEARNAMEDBUFFERSUBDATAPROC                  glClearNamedBufferSubData;
extern  PFNGLMAPNAMEDBUFFERPROC                           glMapNamedBuffer;
extern  PFNGLMAPNAMEDBUFFERRANGEPROC                      glMapNamedBufferRange;
extern  PFNGLUNMAPNAMEDBUFFERPROC                         glUnmapNamedBuffer;
extern  PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC              glFlushMappedNamedBufferRange;
extern  PFNGLGETNAMEDBUFFERPARAMETERIVPROC                glGetNamedBufferParameteriv;
extern  PFNGLGETNAMEDBUFFERPARAMETERI64VPROC              glGetNamedBufferParameteri64v;
extern  PFNGLGETNAMEDBUFFERPOINTERVPROC                   glGetNamedBufferPointerv;
extern  PFNGLGETNAMEDBUFFERSUBDATAPROC                    glGetNamedBufferSubData;
extern  PFNGLCREATEFRAMEBUFFERSPROC                       glCreateFramebuffers;
extern  PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC             glNamedFramebufferRenderbuffer;
extern  PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC               glNamedFramebufferParameteri;
extern  PFNGLNAMEDFRAMEBUFFERTEXTUREPROC                  glNamedFramebufferTexture;
extern  PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC             glNamedFramebufferTextureLayer;
extern  PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC               glNamedFramebufferDrawBuffer;
extern  PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC              glNamedFramebufferDrawBuffers;
extern  PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC               glNamedFramebufferReadBuffer;
extern  PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC           glInvalidateNamedFramebufferData;
extern  PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC        glInvalidateNamedFramebufferSubData;
extern  PFNGLCLEARNAMEDFRAMEBUFFERIVPROC                  glClearNamedFramebufferiv;
extern  PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC                 glClearNamedFramebufferuiv;
extern  PFNGLCLEARNAMEDFRAMEBUFFERFVPROC                  glClearNamedFramebufferfv;
extern  PFNGLCLEARNAMEDFRAMEBUFFERFIPROC                  glClearNamedFramebufferfi;
extern  PFNGLBLITNAMEDFRAMEBUFFERPROC                     glBlitNamedFramebuffer;
extern  PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC              glCheckNamedFramebufferStatus;
extern  PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC           glGetNamedFramebufferParameteriv;
extern  PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetNamedFramebufferAttachmentParameteriv;
extern  PFNGLCREATERENDERBUFFERSPROC                      glCreateRenderbuffers;
extern  PFNGLNAMEDRENDERBUFFERSTORAGEPROC                 glNamedRenderbufferStorage;
extern  PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC      glNamedRenderbufferStorageMultisample;
extern  PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC          glGetNamedRenderbufferParameteriv;
extern  PFNGLCREATETEXTURESPROC                           glCreateTextures;
extern  PFNGLTEXTUREBUFFERPROC                            glTextureBuffer;
extern  PFNGLTEXTUREBUFFERRANGEPROC                       glTextureBufferRange;
extern  PFNGLTEXTURESTORAGE1DPROC                         glTextureStorage1D;
extern  PFNGLTEXTURESTORAGE2DPROC                         glTextureStorage2D;
extern  PFNGLTEXTURESTORAGE3DPROC                         glTextureStorage3D;
extern  PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC              glTextureStorage2DMultisample;
extern  PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC              glTextureStorage3DMultisample;
extern  PFNGLTEXTURESUBIMAGE1DPROC                        glTextureSubImage1D;
extern  PFNGLTEXTURESUBIMAGE2DPROC                        glTextureSubImage2D;
extern  PFNGLTEXTURESUBIMAGE3DPROC                        glTextureSubImage3D;
extern  PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC              glCompressedTextureSubImage1D;
extern  PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC              glCompressedTextureSubImage2D;
extern  PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC              glCompressedTextureSubImage3D;
extern  PFNGLCOPYTEXTURESUBIMAGE1DPROC                    glCopyTextureSubImage1D;
extern  PFNGLCOPYTEXTURESUBIMAGE2DPROC                    glCopyTextureSubImage2D;
extern  PFNGLCOPYTEXTURESUBIMAGE3DPROC                    glCopyTextureSubImage3D;
extern  PFNGLTEXTUREPARAMETERFPROC                        glTextureParameterf;
extern  PFNGLTEXTUREPARAMETERFVPROC                       glTextureParameterfv;
extern  PFNGLTEXTUREPARAMETERIPROC                        glTextureParameteri;
extern  PFNGLTEXTUREPARAMETERIIVPROC                      glTextureParameterIiv;
extern  PFNGLTEXTUREPARAMETERIUIVPROC                     glTextureParameterIuiv;
extern  PFNGLTEXTUREPARAMETERIVPROC                       glTextureParameteriv;
extern  PFNGLGENERATETEXTUREMIPMAPPROC                    glGenerateTextureMipmap;
extern  PFNGLBINDTEXTUREUNITPROC                          glBindTextureUnit;
extern  PFNGLGETTEXTUREIMAGEPROC                          glGetTextureImage;
extern  PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC                glGetCompressedTextureImage;
extern  PFNGLGETTEXTURELEVELPARAMETERFVPROC               glGetTextureLevelParameterfv;
extern  PFNGLGETTEXTURELEVELPARAMETERIVPROC               glGetTextureLevelParameteriv;
extern  PFNGLGETTEXTUREPARAMETERFVPROC                    glGetTextureParameterfv;
extern  PFNGLGETTEXTUREPARAMETERIIVPROC                   glGetTextureParameterIiv;
extern  PFNGLGETTEXTUREPARAMETERIUIVPROC                  glGetTextureParameterIuiv;
extern  PFNGLGETTEXTUREPARAMETERIVPROC                    glGetTextureParameteriv;
extern  PFNGLCREATEVERTEXARRAYSPROC                       glCreateVertexArrays;
extern  PFNGLDISABLEVERTEXARRAYATTRIBPROC                 glDisableVertexArrayAttrib;
extern  PFNGLENABLEVERTEXARRAYATTRIBPROC                  glEnableVertexArrayAttrib;
extern  PFNGLVERTEXARRAYELEMENTBUFFERPROC                 glVertexArrayElementBuffer;
extern  PFNGLVERTEXARRAYVERTEXBUFFERPROC                  glVertexArrayVertexBuffer;
extern  PFNGLVERTEXARRAYVERTEXBUFFERSPROC                 glVertexArrayVertexBuffers;
extern  PFNGLVERTEXARRAYATTRIBBINDINGPROC                 glVertexArrayAttribBinding;
extern  PFNGLVERTEXARRAYATTRIBFORMATPROC                  glVertexArrayAttribFormat;
extern  PFNGLVERTEXARRAYATTRIBIFORMATPROC                 glVertexArrayAttribIFormat;
extern  PFNGLVERTEXARRAYATTRIBLFORMATPROC                 glVertexArrayAttribLFormat;
extern  PFNGLVERTEXARRAYBINDINGDIVISORPROC                glVertexArrayBindingDivisor;
extern  PFNGLGETVERTEXARRAYIVPROC                         glGetVertexArrayiv;
extern  PFNGLGETVERTEXARRAYINDEXEDIVPROC                  glGetVertexArrayIndexediv;
extern  PFNGLGETVERTEXARRAYINDEXED64IVPROC                glGetVertexArrayIndexed64iv;
extern  PFNGLCREATESAMPLERSPROC                           glCreateSamplers;
extern  PFNGLCREATEPROGRAMPIPELINESPROC                   glCreateProgramPipelines;
extern  PFNGLCREATEQUERIESPROC                            glCreateQueries;
extern  PFNGLGETQUERYBUFFEROBJECTI64VPROC                 glGetQueryBufferObjecti64v;
extern  PFNGLGETQUERYBUFFEROBJECTIVPROC                   glGetQueryBufferObjectiv;
extern  PFNGLGETQUERYBUFFEROBJECTUI64VPROC                glGetQueryBufferObjectui64v;
extern  PFNGLGETQUERYBUFFEROBJECTUIVPROC                  glGetQueryBufferObjectuiv;
extern  PFNGLMEMORYBARRIERBYREGIONPROC                    glMemoryBarrierByRegion;
extern  PFNGLGETTEXTURESUBIMAGEPROC                       glGetTextureSubImage;
extern  PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC             glGetCompressedTextureSubImage;
extern  PFNGLGETGRAPHICSRESETSTATUSPROC                   glGetGraphicsResetStatus;
extern  PFNGLGETNCOMPRESSEDTEXIMAGEPROC                   glGetnCompressedTexImage;
extern  PFNGLGETNTEXIMAGEPROC                             glGetnTexImage;
extern  PFNGLGETNUNIFORMDVPROC                            glGetnUniformdv;
extern  PFNGLGETNUNIFORMFVPROC                            glGetnUniformfv;
extern  PFNGLGETNUNIFORMIVPROC                            glGetnUniformiv;
extern  PFNGLGETNUNIFORMUIVPROC                           glGetnUniformuiv;
extern  PFNGLREADNPIXELSPROC                              glReadnPixels;
extern  PFNGLTEXTUREBARRIERPROC                           glTextureBarrier;
/* ----GL VERSION 4.6------------------------------------------------------------------------------------------------ */
typedef void (*PFNGLSPECIALIZESHADERPROC) (GLuint shader, const GLchar *pEntryPoint, GLuint numSpecializationConstants, const GLuint *pConstantIndex, const GLuint *pConstantValue);
typedef void (*PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC) (GLenum mode, const void *indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
typedef void (*PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC) (GLenum mode, GLenum type, const void *indirect, GLintptr drawcount, GLsizei maxdrawcount, GLsizei stride);
typedef void (*PFNGLPOLYGONOFFSETCLAMPPROC) (GLfloat factor, GLfloat units, GLfloat clamp);
extern PFNGLSPECIALIZESHADERPROC               glSpecializeShader;
extern PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC   glMultiDrawArraysIndirectCount;
extern PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC glMultiDrawElementsIndirectCount;
extern PFNGLPOLYGONOFFSETCLAMPPROC             glPolygonOffsetClamp;
#endif // !defined(__APPLE__)

#if defined(__APPLE__)
#pragma clang diagnostic pop
#endif
#endif // !VD_FW_H

#ifdef VD_FW_IMPL

typedef unsigned char VdFw__GamepadButtonState;

enum {
    VD_FW__BUTTON_COUNT_MASK = 0x7F,
    VD_FW__BUTTON_STATE_MASK = 0x80,
};

typedef struct VdFw__GamepadState {
    VdFw__GamepadButtonState buttons[VD_FW_GAMEPAD_BUTTON_MAX];
    float                    axes[6];
} VdFw__GamepadState;

#if defined(__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

static void vd_fw__load_opengl(VdFwGlVersion version);

#ifdef _WIN32

#if VD_FW_WIN32_LINKER_COMMENTS
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "Hid.lib")
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
// #define NOTEXTMETRIC
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
#include <windowsx.h>
#include <shellapi.h>
#include <uxtheme.h>
#include <dwmapi.h>
#include <shellscalingapi.h>
#include <versionhelpers.h>
#include <timeapi.h>
#include <setupapi.h>
#include <hidsdi.h>
#include <hidpi.h>
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

#ifndef WM_NCUAHDRAWCAPTION
#define WM_NCUAHDRAWCAPTION (0x00AE)
#endif
#ifndef WM_NCUAHDRAWFRAME
#define WM_NCUAHDRAWFRAME (0x00AF)
#endif

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
typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef HGLRC (WINAPI * PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int *attribList);
typedef BOOL (WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

#define VD_FW_DISPLAY_PREFERENCE_DGPU 1
#define VD_FW_DISPLAY_PREFERENCE_IGPU 2

#if defined(VD_FW_PREFER_DISCRETE_GPU) && defined(VD_FW_PREFER_INTEGRATED_GPU)
#warning "You cannot defined VD_FW_PREFER_DISCRETE_GPU and VD_FW_PREFER_INTEGRATED_GPU at the same time."
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

    VD_FW_WIN32_SHOW_CURSOR  = WM_USER + 1,
    VD_FW_WIN32_UPDATE_TITLE = WM_USER + 2, 
    VD_FW_WIN32_FULLSCREEN   = WM_USER + 3, 
    VD_FW_WIN32_SIZE         = WM_USER + 4, 
};

typedef struct {
    int w, h;
    int flags;
} VdFw__Win32Frame;

typedef struct {
    UINT msg;
    union {
        struct {
            float dx, dy;
        } scroll;

        struct {
            int   mx, my;
        } mousemove;

        struct {
            DWORD vkbutton;
            int   down;
        } mousebtn;

        struct {
            int   got_focus;
        } changefocus;

        struct {
            WORD  vkcode;
            int   down;
        } keystate;
    } dat;
} VdFw__Win32Message;

typedef struct {
    unsigned short us_id;
    unsigned short us_page;
} VdFw__Win32GamepadMapping;

typedef struct VdFw__Win32GamepadInfo {
    HANDLE               handle;
    unsigned char        guid[16];
    int                  connected;
    PHIDP_PREPARSED_DATA ppd;
    VdFwGamepadConfig    config;
    int                  splitz;
} VdFw__Win32GamepadInfo;

typedef struct {
/* ----WINDOW THREAD ONLY-------------------------------------------------------------------------------------------- */
    HWND                        hwnd;
    int                         w, h;
    volatile BOOL               t_paint_ready;
    BOOL                        draw_decorations;
    RECT                        rgn;
    BOOL                        theme_enabled;
    BOOL                        composition_enabled;
    DWORD                       main_thread_id;
    BOOL                        focus_changed;
    BOOL                        focused;
    RAWINPUT                    raw_input_buffer[VD_FW_WIN32_RAW_INPUT_BUFFER_COUNT];
    LONG                        last_window_style;
    RECT                        windowed_rect;
    WINDOWPLACEMENT             last_window_placement;
    DWORD                       num_gamepads_present;
    VdFw__Win32GamepadInfo      gamepad_infos[VD_FW_GAMEPAD_COUNT_MAX];
    VdFw__GamepadState          gamepad_curr_states[VD_FW_GAMEPAD_COUNT_MAX];
    VdFw__GamepadState          gamepad_prev_states[VD_FW_GAMEPAD_COUNT_MAX];

/* ----RENDER THREAD ONLY-------------------------------------------------------------------------------------------- */
    HANDLE                      win_thread;
    DWORD                       win_thread_id;
    HDC                         hdc;
    HGLRC                       hglrc;
    LARGE_INTEGER               frequency;
    LARGE_INTEGER               performance_counter;
    PFNWGLSWAPINTERVALEXTPROC   proc_swapInterval;
    int                         wheel_moved;
    float                       wheel[2];
    unsigned long long          last_ns;
    int                         mouse[2];
    DWORD                       lmousedown;
    DWORD                       rmousedown;
    DWORD                       mmousedown;
    float                       mouse_delta[2];
    BOOL                        mouse_is_locked;
    BOOL                        is_fullscreen;

/* ----RENDER THREAD - WINDOW THREAD DATA---------------------------------------------------------------------------- */
    VdFw__Win32Message          msgbuf[VD_FW_WIN32_MESSAGE_BUFFER_SIZE];
    volatile LONG               msgbuf_r;
    volatile LONG               msgbuf_w;
    int                         ncrect_count;
    int                         ncrects[16][4];
    int                         nccaption[4];
    int                         nccaption_set;
    int                         receive_ncmouse_on;
    volatile LONG               sink_index;
    float                       mouse_delta_sinks[2][2];
    unsigned char               curr_key_states[VD_FW_KEY_MAX];
    unsigned char               prev_key_states[VD_FW_KEY_MAX];
    char                        title[128];
    int                         title_len;

/* ----RENDER THREAD - WINDOW THREAD SYNC---------------------------------------------------------------------------- */
    HANDLE                      sem_window_ready;
    HANDLE                      sem_closed;
    volatile BOOL               t_running;
    CRITICAL_SECTION            critical_section;
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
static LRESULT vd_fw__wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
static void    vd_fw__composition_changed(void);
static void    vd_fw__update_region(void);
static void    vd_fw__theme_changed(void);
static LRESULT vd_fw__nccalcsize(WPARAM wparam, LPARAM lparam);
static BOOL    vd_fw__has_autohide_taskbar(UINT edge, RECT monitor);
static void    vd_fw__window_pos_changed(WINDOWPOS *pos);
static LRESULT vd_fw__handle_invisible(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
static DWORD   vd_fw__win_thread_proc(LPVOID param);
static void    vd_fw__gl_debug_message_callback(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
static int     vd_fw__msgbuf_r(VdFw__Win32Message *message);
static int     vd_fw__msgbuf_w(VdFw__Win32Message *message);

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
    return (void*)wglGetProcAddress(name);
}

#if !VD_FW_NO_CRT
#include <io.h>
#include <fcntl.h>
#endif // !VD_FW_NO_CRT

VD_FW_API int vd_fw_init(VdFwInitInfo *info)
{
    // SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
    timeBeginPeriod(1);
    QueryPerformanceFrequency(&VD_FW_G.frequency);

    VD_FW_G.focused = 1;
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
            SetConsoleTitleW(L"DEBUG CONSOLE");
            WriteConsole(
                GetStdHandle(STD_OUTPUT_HANDLE),
                TEXT("Console allocated for debugging\n"),
                sizeof(TEXT("Console allocated for debugging\n")) - 1,
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

    VD_FW_G.main_thread_id = GetCurrentThreadId();

    InitializeCriticalSection(&VD_FW_G.critical_section);
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
        &VD_FW_G.main_thread_id,
        0,
        &VD_FW_G.win_thread_id);
    SetThreadDescription(VD_FW_G.win_thread, L"Window Thread");

    WaitForSingleObject(VD_FW_G.sem_window_ready, INFINITE);


    VD_FW_G.hdc = GetDC(VD_FW_G.hwnd);

    // Create context
    {
        // Temp context flags
        PIXELFORMATDESCRIPTOR pfd = {
          sizeof(PIXELFORMATDESCRIPTOR),
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
        int pf = ChoosePixelFormat(VD_FW_G.hdc, &pfd);
        SetPixelFormat(VD_FW_G.hdc, pf, &pfd);

        HGLRC temp_context = wglCreateContext(VD_FW_G.hdc);
        VD_FW__CHECK_NULL(temp_context);
        VD_FW__CHECK_TRUE(wglMakeCurrent(VD_FW_G.hdc, temp_context));

        int major = 3;
        int minor = 3;


        if (info && info->gl.version != VD_FW_GL_VERSION_BASIC) {
            switch (info->gl.version) {
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

        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = 
            (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");

        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = 
            (PFNWGLCHOOSEPIXELFORMATARBPROC) wglGetProcAddress("wglChoosePixelFormatARB");

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
        UINT num_formats;

        VD_FW__CHECK_TRUE(wglChoosePixelFormatARB(VD_FW_G.hdc, pixel_attribs, NULL, 1, &pixel_format, &num_formats));

        PIXELFORMATDESCRIPTOR pfdchosen;
        DescribePixelFormat(VD_FW_G.hdc, pixel_format, sizeof(pfdchosen), &pfdchosen);
        SetPixelFormat(VD_FW_G.hdc, pf, &pfdchosen);

        VD_FW_G.hglrc = wglCreateContextAttribsARB(VD_FW_G.hdc, 0, attribs);

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(temp_context);

        VD_FW__CHECK_NULL(VD_FW_G.hglrc);
        VD_FW__CHECK_TRUE(wglMakeCurrent(VD_FW_G.hdc, VD_FW_G.hglrc));
    }

    VD_FW_G.proc_swapInterval = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

    VdFwGlVersion version = VD_FW_GL_VERSION_3_3;
    if (info && info->gl.version != VD_FW_GL_VERSION_BASIC) {
        version = info->gl.version;
    }
    vd_fw__load_opengl(version);

    if (info != 0 && info->gl.debug_on) {
        PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback_proc = (PFNGLDEBUGMESSAGECALLBACKPROC)wglGetProcAddress("glDebugMessageCallback");

        if (!glDebugMessageCallback_proc) {
            DWORD written;
            WriteConsole(
                GetStdHandle(STD_OUTPUT_HANDLE),
                TEXT("ERROR: Failed to load glDebugMessageCallback!"),
                sizeof(TEXT("ERROR: Failed to load glDebugMessageCallback!")) - 1,
                &written,
                0);
        } else {
            glEnable(0x92E0 /* GL_DEBUG_OUTPUT */);
            glDebugMessageCallback_proc(vd_fw__gl_debug_message_callback, 0);
        }
    }

    QueryPerformanceCounter(&VD_FW_G.performance_counter);
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

    for (int i = 0; i < VD_FW_KEY_MAX; ++i) {
        VD_FW_G.prev_key_states[i] = VD_FW_G.curr_key_states[i];
    }

    VdFw__Win32Message mm;
    while (vd_fw__msgbuf_r(&mm)) {
        switch (mm.msg) {
            case VD_FW_WIN32_MESSAGE_TYPE_MOUSEMOVE: {
                VD_FW_G.mouse[0] = mm.dat.mousemove.mx;
                VD_FW_G.mouse[1] = mm.dat.mousemove.my;
            } break;

            case VD_FW_WIN32_MESSAGE_TYPE_SCROLL: {
                VD_FW_G.wheel[0] += mm.dat.scroll.dx;
                VD_FW_G.wheel[1] += mm.dat.scroll.dy;
            } break;

            case VD_FW_WIN32_MESSAGE_TYPE_MOUSEBTN: {
                if (mm.dat.mousebtn.vkbutton == VK_LBUTTON) {
                    VD_FW_G.lmousedown = mm.dat.mousebtn.down;
                } else if (mm.dat.mousebtn.vkbutton == VK_RBUTTON) {
                    VD_FW_G.rmousedown = mm.dat.mousebtn.down;
                } else if (mm.dat.mousebtn.vkbutton == VK_MBUTTON) {
                    VD_FW_G.mmousedown = mm.dat.mousebtn.down;
                }
            } break;

            case VD_FW_WIN32_MESSAGE_TYPE_CHANGEFOCUS: {
                VD_FW_G.focus_changed = 1;
                VD_FW_G.focused = mm.dat.changefocus.got_focus;
            } break;

            case VD_FW_WIN32_MESSAGE_TYPE_KEYSTATE: {

                int is_down = mm.dat.keystate.down;
                VdFwKey key = vd_fw___vkcode_to_key(mm.dat.keystate.vkcode);

                // VD_FW_G.prev_key_states[key] = VD_FW_G.curr_key_states[key];
                VD_FW_G.curr_key_states[key] = (unsigned char)is_down;

            } break;

            default: break;
        }
    }

    // @note(mdodis): For Raw Input mouse handling, instead of using the message queue
    // We use two sinks with an atomic write index.
    {

        // Get Raw Input mouse delta sink
        LONG curr_sink_index = VD_FW_G.sink_index;
        LONG next_sink_index = (curr_sink_index + 1) % 2;

        // Clear the next sink
        VD_FW_G.mouse_delta_sinks[next_sink_index][0] = 0.f;
        VD_FW_G.mouse_delta_sinks[next_sink_index][1] = 0.f;

        MemoryBarrier();

        // Exchange write index with next_sink_index
        InterlockedExchange(&VD_FW_G.sink_index, next_sink_index);

        // Now we can safely read from the curr_sink_index
        VD_FW_G.mouse_delta[0] = VD_FW_G.mouse_delta_sinks[curr_sink_index][0];
        VD_FW_G.mouse_delta[1] = VD_FW_G.mouse_delta_sinks[curr_sink_index][1];
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
        RECT rect;
        GetWindowRect(VD_FW_G.hwnd, &rect);

        INT center_x = (rect.left + rect.right)  / 2;
        INT center_y = (rect.top  + rect.bottom) / 2;

        SetCursorPos(center_x, center_y);
    }

    return 1;
}

VD_FW_API int vd_fw_swap_buffers(void)
{
    SwapBuffers(VD_FW_G.hdc);
    // @note(mdodis): This needs to happen, otherwise the window animations and taskbar don't get redrawn if the window
    // is maximized to either section of the screen or the whole screen
    DwmFlush();

    if (glFenceSync && glClientWaitSync && glDeleteSync) {
        GLsync fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        if (fence) {
            glClientWaitSync(fence, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000000ULL);
            glDeleteSync(fence);
        }
    }

    if (VD_FW_G.curr_frame.flags & VD_FW_WIN32_FLAGS_WAKE_COND_VAR) {
        WakeConditionVariable(&VD_FW_G.cond_var);
    }
    return 1;
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

    VD_FW__CHECK_TRUE(PostMessageA(
        VD_FW_G.hwnd,
        VD_FW_WIN32_SIZE,
        0, /* WPARAM */
        lparam));
}

VD_FW_API void vd_fw_set_fullscreen(int on)
{
    if (VD_FW_G.is_fullscreen == on) {
        return;
    }

    VD_FW_G.is_fullscreen = on;

    VD_FW__CHECK_TRUE(PostMessageA(
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
    if (c > 16) {
        c = 16;
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
    BOOL result = VD_FW_G.proc_swapInterval(on);

    return result == TRUE ? on : 0;
}

VD_FW_API int vd_fw_get_mouse_state(int *x, int *y)
{
    if (x) *x = VD_FW_G.mouse[0];
    if (y) *y = VD_FW_G.mouse[1];

    int result = 0;

    result |= VD_FW_G.lmousedown ? VD_FW_MOUSE_STATE_LEFT_BUTTON_DOWN   : 0;
    result |= VD_FW_G.rmousedown ? VD_FW_MOUSE_STATE_RIGHT_BUTTON_DOWN  : 0;
    result |= VD_FW_G.mmousedown ? VD_FW_MOUSE_STATE_MIDDLE_BUTTON_DOWN : 0;
    return result;
}

VD_FW_API int vd_fw_get_key_pressed(int key)
{
    return !VD_FW_G.prev_key_states[key] && VD_FW_G.curr_key_states[key];
}

VD_FW_API int vd_fw_get_key_down(int key)
{
    return VD_FW_G.curr_key_states[key];
}

VD_FW_API int vd_fw_get_gamepad_down(int index, int button)
{
    return VD_FW_G.gamepad_curr_states[index].buttons[button];
}

VD_FW_API int vd_fw_get_gamepad_axis(int index, int axis, float *out)
{
    *out = VD_FW_G.gamepad_curr_states[index].axes[axis];
    return 1;
}

VD_FW_API void vd_fw_set_mouse_capture(int on)
{
    if (on) {
        SetCapture(VD_FW_G.hwnd);
    } else {
        ReleaseCapture();
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
        VD_FW__CHECK_TRUE(PostMessageA(
            VD_FW_G.hwnd,
            VD_FW_WIN32_SHOW_CURSOR,
            0, /* WPARAM */
            0  /* LPARAM */));
    } else {
        VD_FW__CHECK_TRUE(PostMessageA(
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
    return (float)GetDpiForWindow(VD_FW_G.hwnd) / 90.f;
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

    VD_FW__CHECK_TRUE(PostMessageA(
        VD_FW_G.hwnd,
        VD_FW_WIN32_UPDATE_TITLE,
        0, /* WPARAM */
        0  /* LPARAM */));
}

static inline void *vd_fw_memcpy(void *dst, void *src, size_t count)
{
    for (size_t i = 0; i < count; ++i) ((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
    return dst;
}

static inline void *vd_fw_memset(void *dst, unsigned char val, size_t num)
{
    for (size_t i = 0; i < num; ++i) ((unsigned char *)dst)[i] = val;
    return dst;
}

VD_FW_API void vd_fw_set_app_icon(void *pixels, int width, int height)
{
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = width;
    bmi.bmiHeader.biHeight      = height;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB; 

    void *bits = 0;
    HDC hdc = GetDC(NULL);
    HBITMAP hbitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, &bits, NULL, 0);
    VD_FW__CHECK_NULL(hbitmap);
    HBITMAP hmaskbitmap = CreateBitmap(width, height, 1, 1, NULL);
    VD_FW__CHECK_NULL(hmaskbitmap);
    ReleaseDC(NULL, hdc);

    VD_FW__CHECK_NULL(hbitmap);

    vd_fw_memcpy(bits, pixels, width * height * 4);

    ICONINFO ii = {0};
    ii.fIcon    = TRUE;
    ii.hbmColor = hbitmap;
    ii.hbmMask  = hmaskbitmap;
    HICON icon = CreateIconIndirect(&ii);
    VD_FW__CHECK_NULL(icon);

    VD_FW__CHECK_TRUE(PostMessageA(
        VD_FW_G.hwnd,
        WM_SETICON,
        ICON_SMALL, /* WPARAM */
        (LPARAM)icon));
    VD_FW__CHECK_TRUE(PostMessageA(
        VD_FW_G.hwnd,
        WM_SETICON,
        ICON_BIG, /* WPARAM */
        (LPARAM)icon));
}

VD_FW_API unsigned long long vd_fw_delta_ns(void)
{
    return VD_FW_G.last_ns;
}

static BOOL IsWindows11OrGreater()
{
    OSVERSIONINFOEX vi;
    ULONGLONG conditions;

    memset (&vi, 0, sizeof(vi));
    vi.dwOSVersionInfoSize = sizeof(vi);
    vi.dwMajorVersion = 10;
    vi.dwMinorVersion = 0;
    vi.dwBuildNumber = 21996;
    conditions = VerSetConditionMask (0,
            VER_MAJORVERSION, VER_GREATER_EQUAL);
    conditions = VerSetConditionMask (conditions,
            VER_MINORVERSION, VER_GREATER_EQUAL);
    conditions = VerSetConditionMask (conditions,
            VER_BUILDNUMBER, VER_GREATER_EQUAL);

    return VerifyVersionInfo (&vi,
            VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER,
            conditions) != FALSE;
}


static int is_windows_11_or_greater()
{
    return IsWindows11OrGreater();
}

static DWORD vd_fw__win_thread_proc(LPVOID param)
{
    (void)param;
    VD_FW_G.t_running = TRUE;
    VD_FW_SANITY_CHECK();

    WNDCLASSEX wcx;
    ZeroMemory(&wcx, sizeof(wcx));
    wcx.cbSize         = sizeof(wcx);
    wcx.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcx.hInstance      = NULL;
    wcx.lpfnWndProc    = vd_fw__wndproc;
    wcx.lpszClassName  = TEXT("FWCLASS");
    wcx.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcx.hCursor        = LoadCursor(NULL, IDC_ARROW);
    if (!RegisterClassEx(&wcx)) {
        return 0;
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

    VD_FW_G.hwnd = CreateWindowEx(
        0,
        TEXT("FWCLASS"),
        TEXT("FW Window"),
        window_style,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        640,
        480,
        0 /* hwndParent */,
        0 /* hMenu */,
        0 /* hInstance */,
        0 /* lpParam */);

    // @note(mdodis): This is an old thing from Windows 7 (I think... era). W
    // SetLayeredWindowAttributes(VD_FW_G.hwnd, RGB(255, 0, 255), 255, LWA_COLORKEY);



    // @todo(mdodis): Consider using
    // Windows 8 or less -- nothing
    // Windows 10 -- dark/acrylic
    // Windows 11 -- mica
    if (VD_FW_G.draw_decorations) {
        DWORD t = TRUE;
        DwmSetWindowAttribute(VD_FW_G.hwnd, 20, &t, sizeof(t));
    }
    vd_fw__composition_changed();
    VD_FW_SANITY_CHECK();
    vd_fw__theme_changed();
    VD_FW_SANITY_CHECK();

    RECT rect;
    GetClientRect(VD_FW_G.hwnd, &rect);
    VD_FW_G.w = rect.right - rect.left;
    VD_FW_G.h = rect.bottom - rect.top;
    VD_FW_G.next_frame.w = VD_FW_G.w;
    VD_FW_G.next_frame.h = VD_FW_G.h;


    // SetWindowPos(VD_FW_G.hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_SHOWWINDOW);
    ShowWindow(VD_FW_G.hwnd, SW_SHOW);
    VD_FW__CHECK_NONZERO(UpdateWindow(VD_FW_G.hwnd));

    VD_FW__CHECK_TRUE(ReleaseSemaphore(VD_FW_G.sem_window_ready, 1, NULL));

    // Register raw input mouse
    {
        RAWINPUTDEVICE rids[] = {
            {
                .usUsagePage   = 0x01, // Generic desktop controls
                .usUsage       = 0x02, // Mouse
                .dwFlags       = 0, // RIDEV_INPUTSINK,
                .hwndTarget    = VD_FW_G.hwnd,
            },
            {

                .usUsagePage   = 0x01, // Generic desktop controls
                .usUsage       = 0x05, // Gamepad
                .dwFlags       = RIDEV_DEVNOTIFY,
                .hwndTarget    = VD_FW_G.hwnd,
            },
        };
        VD_FW__CHECK_TRUE(RegisterRawInputDevices(rids, 2, sizeof(rids[0])));
    }

    VD_FW_G.last_window_style = window_style;
    VD_FW_G.t_paint_ready = 1;

    while (VD_FW_G.t_running) {
        MSG message;
        while (GetMessage(&message, VD_FW_G.hwnd, 0, 0)) {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
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
    // @todo(mdodis): if file is > 4GB
    if (!ReadFile(hfile, memory, (DWORD)sz.QuadPart, &bytes_read, 0)) {
        HeapFree(GetProcessHeap(), 0, memory);
        return 0;
    }

    memory[sz.QuadPart] = 0;
    return memory;
}

VD_FW_API void vd_fw__free_mem(void *memory)
{
    HeapFree(GetProcessHeap(), 0, memory);
}

static void vd_fw__gl_debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    (void)userParam;
    (void)severity;
    (void)id;
    (void)type;
    (void)source;

    DWORD written;
    WriteConsole(
        GetStdHandle(STD_OUTPUT_HANDLE),
        message,
        length,
        &written,
        0);

    WriteConsole(
        GetStdHandle(STD_OUTPUT_HANDLE),
        "\n",
        1,
        &written,
        0);
}

static int vd_fw__hit_test(int x, int y)
{
    POINT mouse;
    mouse.x = x;
    mouse.y = y;
    ScreenToClient(VD_FW_G.hwnd, &mouse);

    RECT client;
    GetClientRect(VD_FW_G.hwnd, &client);
    int width  = client.right - client.left;
    int height = client.bottom - client.top;

    /* The horizontal frame should be the same size as the vertical frame,
       since the NONCLIENTMETRICS structure does not distinguish between them */
    int frame_size = GetSystemMetrics(SM_CXFRAME) +
                     GetSystemMetrics(SM_CXPADDEDBORDER);
    /* The diagonal size handles are wider than the frame */
    int diagonal_width = frame_size * 2 + GetSystemMetrics(SM_CXBORDER);

    if (!VD_FW_G.draw_decorations && IsMaximized(VD_FW_G.hwnd)) {
        mouse.y += frame_size;
    }

    if (!PtInRect(&client, mouse)) {
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
    VD_FW_WIN32_PROFILE_BEGIN(composition_changed);

    BOOL enabled = FALSE;
    VD_FW__CHECK_HRESULT(DwmIsCompositionEnabled(&enabled));
    VD_FW_G.composition_enabled = enabled;

    if (enabled) {
        MARGINS m = {-1};
        VD_FW__CHECK_HRESULT(DwmExtendFrameIntoClientArea(VD_FW_G.hwnd, &m));

        // @note(mdodis): If we set this to disabled, then every time we resize the Windows 7 frame gets drawn behind
        // Additionally alpha compositing is done on fragments that haven't received a full alpha.
        // Also see vd_fw__wndproc, WM_NCPAINT
        {
            DWORD value = DWMNCRP_USEWINDOWSTYLE;
            VD_FW__CHECK_HRESULT(DwmSetWindowAttribute(VD_FW_G.hwnd, DWMWA_NCRENDERING_POLICY, &value, sizeof(value)));
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
    RECT old_rgn = VD_FW_G.rgn;

    if (IsMaximized(VD_FW_G.hwnd)) {
        // @note(mdodis): If the window is maximized when get the client and window rects and set the region subtracted
        // by the overall window's top left coordinates
        WINDOWINFO window_info = {};
        window_info.cbSize = sizeof(window_info);
        GetWindowInfo(VD_FW_G.hwnd, &window_info);
        VD_FW_G.rgn = (RECT) {
            .left   = window_info.rcClient.left   - window_info.rcWindow.left,
            .top    = window_info.rcClient.top    - window_info.rcWindow.top,
            .right  = window_info.rcClient.right  - window_info.rcWindow.left,
            .bottom = window_info.rcClient.bottom - window_info.rcWindow.top,
        };
    } else if (!VD_FW_G.composition_enabled) {
        // @note(mdodis): If composition is enabled, set the window's region to something really high so that shadows of
        // the window are still drawn
        VD_FW_G.rgn = (RECT) {
            .left   = 0,
            .top    = 0,
            .right  = 32767,
            .bottom = 32767,
        };
    } else {
        // @note(mdodis): Otherwise, the window's region is left unchanged
        VD_FW_G.rgn = (RECT) {
            .left   = 0,
            .top    = 0,
            .right  = 0,
            .bottom = 0,
        };
    }

    if (EqualRect(&VD_FW_G.rgn, &old_rgn)) {
        return;
    }

    RECT zero_rect = {0};
    if (EqualRect(&VD_FW_G.rgn, &zero_rect)) {
        SetWindowRgn(VD_FW_G.hwnd, NULL, TRUE);
    } else {
        SetWindowRgn(VD_FW_G.hwnd, CreateRectRgnIndirect(&VD_FW_G.rgn), TRUE);
    }
}

static void vd_fw__theme_changed(void)
{
    VD_FW_G.theme_enabled = IsThemeActive();
}

static LRESULT vd_fw__nccalcsize(WPARAM wparam, LPARAM lparam)
{
    int borderless = !VD_FW_G.draw_decorations;
    if (wparam && borderless) {

        NCCALCSIZE_PARAMS *params = (NCCALCSIZE_PARAMS*)lparam;
        if (IsMaximized(VD_FW_G.hwnd)) {
            HMONITOR monitor = MonitorFromWindow(VD_FW_G.hwnd, MONITOR_DEFAULTTONULL);
            if (!monitor) {
                return 0;
            }

            MONITORINFO monitor_info = {0};
            monitor_info.cbSize = sizeof(monitor_info);
            if (!GetMonitorInfoW(monitor, &monitor_info)) {
                return 0;
            }

            params->rgrc[0] = monitor_info.rcWork;
            return 0;
        } else {
            params->rgrc[0].bottom += 1;
            return WVR_VALIDRECTS;
        }

    } else {
        return DefWindowProcW(VD_FW_G.hwnd, WM_NCCALCSIZE, wparam, lparam);
    }
}

static BOOL vd_fw__has_autohide_taskbar(UINT edge, RECT monitor)
{
    if (IsWindows8Point1OrGreater()) {
        APPBARDATA appbar_data = {0};
        appbar_data.cbSize = sizeof(appbar_data);
        appbar_data.uEdge  = edge;
        appbar_data.rc     = monitor;
        return SHAppBarMessage(ABM_GETAUTOHIDEBAREX, &appbar_data) != 0;
    }

    if (monitor.left != 0 || monitor.top != 0) {
        return FALSE;
    }

    APPBARDATA appbar_data = {0};
    appbar_data.cbSize = sizeof(appbar_data);
    appbar_data.uEdge  = edge;
    return SHAppBarMessage(ABM_GETAUTOHIDEBAR, &appbar_data) != 0;
}

static void vd_fw__window_pos_changed(WINDOWPOS *pos)
{
#if 0
    VD_FW_WIN32_PROFILE_BEGIN(window_pos_changed);
#endif

    if (VD_FW_G.draw_decorations) {
        RECT client;
        GetClientRect(VD_FW_G.hwnd, &client);
        VD_FW_G.w = client.right - client.left;
        VD_FW_G.h = client.bottom - client.top;

    } else {
        VD_FW_G.w = pos->cx;
        VD_FW_G.h = pos->cy;

        // @note(mdodis): When window is maximized, pos->x and pos->y become -8, -8
        // So, subtract them from the overall width and height 2 times each so that
        // the maximized viewport is fully shown instead of clipped at the top and right
        if (IsMaximized(VD_FW_G.hwnd)) {
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

static LRESULT vd_fw__handle_invisible(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    // @note(mdodis): Prevent windows from drawing the default (Windows 7) titlebar and frames by toggling it temporarily
    VD_FW_WIN32_PROFILE_BEGIN(handle_invisible);

    LONG_PTR old_style = GetWindowLongPtr(hwnd, GWL_STYLE);

    SetWindowLongPtrW(hwnd, GWL_STYLE, old_style & ~WS_VISIBLE);    
    LRESULT result = DefWindowProcW(hwnd, msg, wparam, lparam);
    SetWindowLongPtrW(hwnd, GWL_STYLE, old_style);

    VD_FW_WIN32_PROFILE_END(handle_invisible);

    return result;
}

static int vd_fw__get_min_max_axial_value(PHIDP_PREPARSED_DATA ppd, USAGE usage_page, USAGE usage,
                                          int *min_value, int *max_value)
{
    static HIDP_VALUE_CAPS value_caps[4];
    USHORT num_value_caps = 4;
    NTSTATUS status = HidP_GetSpecificValueCaps(HidP_Input, usage_page, 0, usage,
                                     value_caps, &num_value_caps,
                                     ppd);

    if (status != HIDP_STATUS_SUCCESS) {
        return 0;
    }

    *min_value = value_caps[0].LogicalMin;
    *max_value = value_caps[0].LogicalMax;

    if (value_caps[0].LogicalMax == -1) {
        *max_value = 0xFFFF;
    }

    return 1;
}

static unsigned char vd_fw__map_usb_id_to_input(int usage)
{
    switch (usage) {
        case 0x01: return VD_FW_GAMEPAD_A;
        case 0x02: return VD_FW_GAMEPAD_B;
        case 0x03: return VD_FW_GAMEPAD_X;
        case 0x04: return VD_FW_GAMEPAD_Y;
        case 0x07: return VD_FW_GAMEPAD_SELECT;
        case 0x08: return VD_FW_GAMEPAD_START;
        case 0x05: return VD_FW_GAMEPAD_L1;
        case 0x06: return VD_FW_GAMEPAD_R1;
        default:   return VD_FW_GAMEPAD_UNKNOWN; 
    }
}

static unsigned char vd_fw__map_axial_usage_to_axis(int usage)
{
    switch (usage) {
        case 0x30: return VD_FW_GAMEPAD_LH;
        case 0x31: return VD_FW_GAMEPAD_LV;
        case 0x32: return VD_FW_GAMEPAD_LT;
        case 0x33: return VD_FW_GAMEPAD_RH;
        case 0x34: return VD_FW_GAMEPAD_RV;
        case 0x35: return VD_FW_GAMEPAD_RT;
        default: return 0xFF;
    }
}

static LRESULT vd_fw__wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    switch (msg) {

        case WM_CLOSE: {
            DestroyWindow(hwnd);
        } break;

        case WM_DESTROY: {
            ReleaseSemaphore(VD_FW_G.sem_closed, 1, NULL);
            PostQuitMessage(0);
            VD_FW_G.t_running = FALSE;
        } break;

        case WM_DWMCOMPOSITIONCHANGED: {
            if (VD_FW_G.draw_decorations) {
                result = DefWindowProc(hwnd, msg, wparam, lparam);

                BOOL enabled = FALSE;
                VD_FW__CHECK_HRESULT(DwmIsCompositionEnabled(&enabled));
                VD_FW_G.composition_enabled = enabled;
            } else {
                vd_fw__composition_changed();
            }
        } break;

        case WM_PAINT: {
            if (!VD_FW_G.t_paint_ready) break;

            VD_FW_WIN32_PROFILE_BEGIN(wm_paint);

            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
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
            EndPaint(hwnd, &ps);

            VD_FW_WIN32_PROFILE_END(wm_paint);
        } break;

        case WM_NCPAINT: {
            if (VD_FW_G.draw_decorations) {
                result = DefWindowProc(hwnd, msg, wparam, lparam);
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
                result = DefWindowProc(hwnd, msg, wparam, lparam);

                // EndPaint(hwnd, &ps);
            }
            VD_FW_WIN32_PROFILE_END(wm_ncpaint);
        } break;

        case WM_DPICHANGED: {
            RECT *rect = (RECT*)lparam;
            SetWindowPos(hwnd, 0, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, SWP_NOZORDER | SWP_NOACTIVATE);
        } break;

        case WM_ERASEBKGND: {
            result = 1;
        } break;

        case WM_NCACTIVATE: {
            if (!VD_FW_G.draw_decorations) {
                // @note(mdodis): DefWindowProc doesn't repaint border if lparam == -1
                // See: https://blogs.msdn.microsoft.com/wpfsdk/2008/09/08/custom-window-chrome-in-wpf/
                result = DefWindowProc(hwnd, msg, wparam, -1);
            } else {
                result = DefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_NCCALCSIZE: {
            if (!VD_FW_G.draw_decorations) {
                vd_fw__nccalcsize(wparam, lparam);
            } else {
                result = DefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;


        case WM_NCHITTEST: {
            if (!VD_FW_G.draw_decorations) {
                result = vd_fw__hit_test(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            } else {
                result = DefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_NCUAHDRAWCAPTION:
        case WM_NCUAHDRAWFRAME: {
            if (!VD_FW_G.draw_decorations) {
                result = 0;
            } else {
                result = DefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_SETICON:
        case WM_SETTEXT: {
            if (!VD_FW_G.draw_decorations) {
                if (!VD_FW_G.composition_enabled && !VD_FW_G.theme_enabled) {
                    result = vd_fw__handle_invisible(hwnd, msg, wparam, lparam);
                } else {
                    result = DefWindowProc(hwnd, msg, wparam, lparam);
                }
            } else {
                result = DefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_THEMECHANGED: {
            if (!VD_FW_G.draw_decorations) {
                vd_fw__theme_changed();
            }

            result = DefWindowProc(hwnd, msg, wparam, lparam);
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
        } break;

        case WM_INPUT: {
            UINT data_size = sizeof(RAWINPUT) * VD_FW_WIN32_RAW_INPUT_BUFFER_COUNT;
            UINT num_bytes_copied = GetRawInputData(
                (HRAWINPUT)lparam,
                RID_INPUT,
                VD_FW_G.raw_input_buffer,
                &data_size,
                sizeof(RAWINPUTHEADER));

            if (num_bytes_copied == ((UINT)-1)) {
                break;
            }

            RAWINPUT *raw = VD_FW_G.raw_input_buffer;
            if (raw->header.dwType == RIM_TYPEMOUSE) {
                LONG dx = raw->data.mouse.lLastX;
                LONG dy = raw->data.mouse.lLastY;

                LONG write_index = VD_FW_G.sink_index;
                MemoryBarrier();

                VD_FW_G.mouse_delta_sinks[write_index][0] = VD_FW_G.mouse_delta_sinks[write_index][0] * 0.8f + dx * 0.2f;
                VD_FW_G.mouse_delta_sinks[write_index][1] = VD_FW_G.mouse_delta_sinks[write_index][1] * 0.8f + dy * 0.2f;
            } else if (raw->header.dwType == RIM_TYPEHID) {

                VdFw__Win32GamepadInfo *gamepad_info = &VD_FW_G.gamepad_infos[0];
                VdFw__GamepadButtonState button_states[VD_FW_GAMEPAD_BUTTON_MAX] = {0};

                for (DWORD ri = 0; ri < raw->data.hid.dwCount; ++ri) {
                    BYTE *bytes = &raw->data.hid.bRawData[0] + ri * (raw->data.hid.dwSizeHid);

                    // Buttons
                    {
                        static USAGE usages[32];
                        ULONG usage_count = 32;

                        if (HidP_GetUsages(HidP_Input, 0x09, 0, usages, &usage_count, gamepad_info->ppd, (PCHAR)bytes, raw->data.hid.dwSizeHid) == HIDP_STATUS_SUCCESS) {
                            for (ULONG i = 0; i < usage_count; ++i) {

                                for (int j = 0; j < gamepad_info->config.num_digital_mappings; ++j) {
                                    if (usages[i] == gamepad_info->config.digital_mappings[j].id) {
                                        button_states[gamepad_info->config.digital_mappings[j].input] = 1;
                                    }
                                }
                            }
                        }
                    }

                    // Hat Switch
                    {
                        if (gamepad_info->config.num_hat_switch_mappings > 0) {
                            ULONG directional_value;
                            if (HidP_GetUsageValue(HidP_Input,
                                                   0x01, 0, 0x39, &directional_value,
                                                   gamepad_info->ppd,
                                                   (PCHAR)bytes, raw->data.hid.dwSizeHid) == HIDP_STATUS_SUCCESS)
                            {
                                if (gamepad_info->config.hat_switch_mappings[0].logical_range == 8) {
                                    static const int hat_to_mask[9] = {
                                        0x00, // Centered
                                        0x01, // Up
                                        0x03, // Up+Right
                                        0x02, // Right
                                        0x06, // Down+Right
                                        0x04, // Down
                                        0x0C, // Down+Left
                                        0x08, // Left
                                        0x09, // Up+Left
                                    };
                                    int mask = hat_to_mask[directional_value];

                                    button_states[VD_FW_GAMEPAD_DUP] =
                                        (mask & gamepad_info->config.hat_switch_mappings[0].ids[0]) ? 1 : 0;
                                    button_states[VD_FW_GAMEPAD_DRIGHT] =
                                        (mask & gamepad_info->config.hat_switch_mappings[0].ids[1]) ? 1 : 0;
                                    button_states[VD_FW_GAMEPAD_DDOWN] =
                                        (mask & gamepad_info->config.hat_switch_mappings[0].ids[2]) ? 1 : 0;
                                    button_states[VD_FW_GAMEPAD_DLEFT] =
                                        (mask & gamepad_info->config.hat_switch_mappings[0].ids[3]) ? 1 : 0;
                                }
                            }
                        }
                    }

                    // Axes
                    {
                        for (int i = 0; i < gamepad_info->config.num_axial_mappings; ++i) {
                            ULONG value;

                            USAGE usage = gamepad_info->config.axial_mappings[i].id;
                            if (HidP_GetUsageValue(HidP_Input, 0x01, 0, usage, &value, gamepad_info->ppd, (PCHAR)bytes, raw->data.hid.dwSizeHid) == HIDP_STATUS_SUCCESS) {

                                LONG min_value = gamepad_info->config.axial_mappings[i].min_value;
                                LONG max_value = gamepad_info->config.axial_mappings[i].max_value;

                                // Clamp known value

                                float value01 = 0.f;
                                if (gamepad_info->config.axial_mappings[i].input <= VD_FW_GAMEPAD_RV) {
                                    value01 = (float)(value - min_value) / (float)(max_value - min_value);
                                    value01 *= 2.f;
                                    value01 -= 1.f;
                                } else {
                                    if (max_value > min_value) {
                                        if ((LONG)value < min_value) {
                                            value = min_value;
                                        } else if ((LONG)value > max_value) {
                                            value = max_value;
                                        }

                                        value01 = (float)(value - min_value) / (float)(max_value - min_value);
                                    } else {

                                        if ((LONG)value < max_value) {
                                            value = max_value;
                                        } else if ((LONG)value > min_value) {
                                            value = min_value;
                                        }

                                        value01 = (float)(value - max_value) / (float)(min_value - max_value);
                                        value01 = 1.0f - value01;
                                    }
                                }

                                VD_FW_G.gamepad_curr_states[0].axes[gamepad_info->config.axial_mappings[i].input] = value01;

                            }
                        }
                    }
                }

                vd_fw_memcpy(&VD_FW_G.gamepad_prev_states[0].buttons, &VD_FW_G.gamepad_curr_states[0].buttons, sizeof(VD_FW_G.gamepad_curr_states[0].buttons));

                for (int i = 0; i < VD_FW_GAMEPAD_BUTTON_MAX; ++i) {
                    VD_FW_G.gamepad_curr_states[0].buttons[i] = button_states[i];
                }
            }

        } break;

        case WM_INPUT_DEVICE_CHANGE: {
            HANDLE device_handle = (HANDLE)lparam;

            if (wparam == GIDC_ARRIVAL) {
                RID_DEVICE_INFO device_info;
                device_info.cbSize = sizeof(device_info);
                UINT cb_size = sizeof(device_info);
                UINT device_info_result = GetRawInputDeviceInfoA(
                    device_handle,
                    RIDI_DEVICEINFO,
                    &device_info,
                    &cb_size);

                if ((device_info_result) == ((UINT)-1)) {
                    break;
                }

                if (device_info.dwType != RIM_TYPEHID) {
                    break;
                }

                unsigned short vendor_id    = (unsigned short)device_info.hid.dwVendorId;
                unsigned short product_id   = (unsigned short)device_info.hid.dwProductId;
                unsigned short version      = (unsigned short)device_info.hid.dwVersionNumber;

                unsigned char guid[16] = {0};
                guid[0] = 0x03;
                guid[1] = 0x00;
                guid[2] = 0x00;
                guid[3] = 0x00;

                guid[4] = vendor_id & 0xFF;
                guid[5] = (vendor_id >> 8) & 0xFF;

                guid[8] = product_id & 0xFF;
                guid[9] = (product_id >> 8) & 0xFF;

                guid[12] = version & 0xFF;
                guid[13] = (version >> 8) & 0xFF;

                VdFw__Win32GamepadInfo *new_gamepad = NULL;
                for (int i = 0; i < VD_FW_GAMEPAD_COUNT_MAX; ++i) {
                    if (!VD_FW_G.gamepad_infos[i].connected) {
                        new_gamepad = &VD_FW_G.gamepad_infos[i];
                        break;
                    }
                }

                if (new_gamepad == NULL) {
                    break;
                }

                UINT ppd_req_size = 0;
                if (GetRawInputDeviceInfoA(
                    device_handle,
                    RIDI_PREPARSEDDATA,
                    NULL,
                    &ppd_req_size) == ((UINT)-1))
                {
                    break;
                }

                new_gamepad->ppd = (PHIDP_PREPARSED_DATA)HeapAlloc(GetProcessHeap(), 0, ppd_req_size);

                if (GetRawInputDeviceInfoA(
                    device_handle,
                    RIDI_PREPARSEDDATA,
                    new_gamepad->ppd,
                    &ppd_req_size) == ((UINT)-1))
                {
                    break;
                }

                new_gamepad->config.num_digital_mappings = 0;
                new_gamepad->config.num_axial_mappings = 0;
                new_gamepad->config.num_hat_switch_mappings = 0;
                new_gamepad->splitz = 1;
                new_gamepad->handle = device_handle;
                for (int i = 0; i < 16; ++i) new_gamepad->guid[i] = guid[i];
                new_gamepad->connected = TRUE;
                VD_FW_G.num_gamepads_present++;

                HIDP_CAPS caps;
                if (HidP_GetCaps(new_gamepad->ppd, &caps) != HIDP_STATUS_SUCCESS) {
                    break;
                }

                // HID Button Caps
                // - Either are specified individually, (i.e 10 buttons on controller -> 10 button caps)
                // - Or, they are specified in a range (10 buttons on controller -> 1 button cap with range 10..1)
                // So, to handle this, we iterate on the button caps, and then iterate on the range, if that caps
                // entry happens to be a range
                static HIDP_BUTTON_CAPS button_caps[32];
                USHORT num_button_caps = caps.NumberInputButtonCaps;
                if (HidP_GetButtonCaps(HidP_Input, button_caps, &num_button_caps, new_gamepad->ppd) == HIDP_STATUS_SUCCESS) {
                    for (int i = 0; i < num_button_caps; ++i) {
                        if (button_caps[i].IsRange) {
                            int count = 1 + (button_caps[i].Range.DataIndexMax - button_caps[i].Range.DataIndexMin);

                            for (unsigned short j = 0; j < count; ++j) {
                                unsigned short usage = button_caps[i].Range.UsageMin + j;
                                unsigned char input_value = vd_fw__map_usb_id_to_input(usage);

                                if (input_value == VD_FW_GAMEPAD_UNKNOWN) {
                                    continue;
                                }

                                int idx = new_gamepad->config.num_digital_mappings++;
                                new_gamepad->config.digital_mappings[idx].input = input_value;
                                new_gamepad->config.digital_mappings[idx].id = usage;
                            }
                        } else {

                            unsigned short usage = button_caps[i].NotRange.Usage;
                            unsigned char input_value = vd_fw__map_usb_id_to_input(usage);

                            if (input_value == VD_FW_GAMEPAD_UNKNOWN) {
                                continue;
                            }

                            int idx = new_gamepad->config.num_digital_mappings++;
                            new_gamepad->config.digital_mappings[idx].input = input_value;
                            new_gamepad->config.digital_mappings[idx].id = usage;
                        }
                    }
                }

                // HID Value Caps:
                // - Are only useful afaik when they're not ranges
                static HIDP_VALUE_CAPS value_caps[32];
                USHORT num_value_caps = caps.NumberInputValueCaps;
                if (HidP_GetValueCaps(HidP_Input, value_caps, &num_value_caps, new_gamepad->ppd) == HIDP_STATUS_SUCCESS) {
                    for (int i = 0; i < num_value_caps; ++i) {
                        if (value_caps[i].IsRange)
                            continue;

                        if (value_caps[i].NotRange.Usage == 0x0039) {
                            // Usage 0x0039: GENERIC HAT
                            // We compute logical range reported by the hat switch
                            // - For LogicalMin: 1, LogicalMax: 4:
                            //   > We know that the hat switch can only encode 4 different directions at a time:
                            //   > Up, Right, Down, Left (in that order)
                            // - For LogicalMin: 1, LogicalMax: 8:
                            //   > We know that the hat switch can encode 8 different directions:
                            //   > Up, UpRight, Right, RightDown, Down, DownLeft, Left, UpLeft.
                            int logical_range = value_caps[i].LogicalMax - value_caps[i].LogicalMin + 1;
                            new_gamepad->config.hat_switch_mappings[0].logical_range = logical_range;
                            new_gamepad->config.hat_switch_mappings[0].ids[0] = 0x01;
                            new_gamepad->config.hat_switch_mappings[0].ids[1] = 0x02;
                            new_gamepad->config.hat_switch_mappings[0].ids[2] = 0x04;
                            new_gamepad->config.hat_switch_mappings[0].ids[3] = 0x08;
                            new_gamepad->config.num_hat_switch_mappings = 1;
                        } else if ((value_caps[i].NotRange.Usage >= 0x30) && (value_caps[i].NotRange.Usage <= 0x35)) {
                            // Usages 0x0030 - 0x0035: X,Y,Z,Rx,Ry,Rz
                            unsigned char axis_input = vd_fw__map_axial_usage_to_axis(value_caps[i].NotRange.Usage);

                            printf("Axis: %02x (%2d)\n", value_caps[i].NotRange.Usage, value_caps[i].NotRange.Usage);
                            printf("--------------------\n");
                            printf("DataIndex: %d\n", value_caps[i].NotRange.DataIndex);
                            printf("ReportCount: %d\n", value_caps[i].ReportCount);
                            printf("--------------------\n");
                            if (axis_input == 0xFF) {
                                continue;
                            }

                            if (value_caps[i].NotRange.Usage == 0x35) {
                                new_gamepad->splitz = 0;
                            }

                            int min_value = value_caps[i].LogicalMin;
                            int max_value = value_caps[i].LogicalMax;

                            if (max_value == -1) {
                                max_value = (1u << value_caps[i].BitSize) - 1;
                            }

                            int idx = new_gamepad->config.num_axial_mappings++;
                            new_gamepad->config.axial_mappings[idx].id = value_caps[i].NotRange.Usage;
                            new_gamepad->config.axial_mappings[idx].input = axis_input;
                            new_gamepad->config.axial_mappings[idx].min_value = min_value;
                            new_gamepad->config.axial_mappings[idx].max_value = max_value;

                        }
                    }

                    if (new_gamepad->splitz) {

                        // Find LT
                        int lt_idx = -1;
                        for (int i = 0; i < new_gamepad->config.num_axial_mappings; ++i) {
                            if (new_gamepad->config.axial_mappings[i].input == VD_FW_GAMEPAD_LT) {
                                lt_idx = i;
                                break;
                            }
                        }

                        if (lt_idx != -1) {
                            int lt_min = new_gamepad->config.axial_mappings[lt_idx].min_value;
                            int lt_max = new_gamepad->config.axial_mappings[lt_idx].max_value;
                            int lt_range = lt_max - lt_min;
                            int lt_middle = lt_min + (lt_range / 2);

                            int idx = new_gamepad->config.num_axial_mappings++;
                            new_gamepad->config.axial_mappings[idx].id = new_gamepad->config.axial_mappings[lt_idx].id;
                            new_gamepad->config.axial_mappings[idx].input = VD_FW_GAMEPAD_RT;
                            new_gamepad->config.axial_mappings[idx].min_value = lt_middle;
                            new_gamepad->config.axial_mappings[idx].max_value = lt_min;

                            new_gamepad->config.axial_mappings[lt_idx].min_value = lt_middle;
                            new_gamepad->config.axial_mappings[lt_idx].max_value = lt_max;
                        }

                    }
                }

                // unsigned short a0 = 0x30;
                // unsigned short a1 = 0x31;
                // unsigned short a2 = 0x32;
                // unsigned short a3 = 0x33;
                // unsigned short a4 = 0x34;
                // unsigned short a5 = 0x35;

                // new_gamepad->config.axial_mappings[0].id         = a0;
                // new_gamepad->config.axial_mappings[0].input      = VD_FW_GAMEPAD_LH;
                // (vd_fw__get_min_max_axial_value(new_gamepad->ppd, 0x01, a0,
                //                                &new_gamepad->config.axial_mappings[0].min_value,
                //                                &new_gamepad->config.axial_mappings[0].max_value));

                // new_gamepad->config.axial_mappings[1].id         = a1;
                // new_gamepad->config.axial_mappings[1].input      = VD_FW_GAMEPAD_LV;
                // (vd_fw__get_min_max_axial_value(new_gamepad->ppd, 0x01, a1,
                //                                &new_gamepad->config.axial_mappings[1].min_value,
                //                                &new_gamepad->config.axial_mappings[1].max_value));

                // new_gamepad->config.axial_mappings[2].id         = a3;
                // new_gamepad->config.axial_mappings[2].input      = VD_FW_GAMEPAD_RH;
                // (vd_fw__get_min_max_axial_value(new_gamepad->ppd, 0x01, a3,
                //                                &new_gamepad->config.axial_mappings[2].min_value,
                //                                &new_gamepad->config.axial_mappings[2].max_value));

                // new_gamepad->config.axial_mappings[3].id         = a4;
                // new_gamepad->config.axial_mappings[3].input      = VD_FW_GAMEPAD_RV;
                // (vd_fw__get_min_max_axial_value(new_gamepad->ppd, 0x01, a4,
                //                                &new_gamepad->config.axial_mappings[3].min_value,
                //                                &new_gamepad->config.axial_mappings[3].max_value));

                // new_gamepad->config.axial_mappings[4].id         = a2;
                // new_gamepad->config.axial_mappings[4].input      = VD_FW_GAMEPAD_LT;
                // (vd_fw__get_min_max_axial_value(new_gamepad->ppd, 0x01, a2,
                //                                &new_gamepad->config.axial_mappings[4].min_value,
                //                                &new_gamepad->config.axial_mappings[4].max_value));

                // new_gamepad->config.axial_mappings[5].id         = a5;
                // new_gamepad->config.axial_mappings[5].input      = VD_FW_GAMEPAD_RT;
                // (vd_fw__get_min_max_axial_value(new_gamepad->ppd, 0x01, a5,
                //                                &new_gamepad->config.axial_mappings[5].min_value,
                //                                &new_gamepad->config.axial_mappings[5].max_value));
                // new_gamepad->config.num_axial_mappings = 6;
                // // @todo(mdodis): first figure out what is a1..a5 then map the behavior

            } else {

                VdFw__Win32GamepadInfo *disconnected_gamepad = NULL;
                for (int i = 0; i < VD_FW_GAMEPAD_COUNT_MAX; ++i) {
                    if (VD_FW_G.gamepad_infos[i].handle == device_handle) {
                        disconnected_gamepad = &VD_FW_G.gamepad_infos[i];
                        break;
                    }
                }

                if (disconnected_gamepad == NULL) {
                    break;
                }

                disconnected_gamepad->connected = FALSE;
                disconnected_gamepad->handle = INVALID_HANDLE_VALUE;

                VD_FW_G.num_gamepads_present--;
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
                    vkcode = LOWORD(MapVirtualKeyW(scancode, MAPVK_VSC_TO_VK_EX));
                    break;
            }

            int is_down = msg == WM_KEYDOWN;

            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_KEYSTATE;
            m.dat.mousebtn.down   = is_down;
            m.dat.keystate.vkcode = vkcode;
            vd_fw__msgbuf_w(&m);
        } break;

        case WM_MBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONUP:
        case WM_LBUTTONDOWN: {
            int down = 0;
            DWORD code = 0;

            switch (msg) {

                case WM_MBUTTONUP:    down = 0; code = VK_MBUTTON; break;
                case WM_MBUTTONDOWN:  down = 1; code = VK_MBUTTON; break;
                case WM_RBUTTONUP:    down = 0; code = VK_RBUTTON; break;
                case WM_RBUTTONDOWN:  down = 1; code = VK_RBUTTON; break;
                case WM_LBUTTONUP:    down = 0; code = VK_LBUTTON; break;
                case WM_LBUTTONDOWN:  down = 1; code = VK_LBUTTON; break;
                default: break;
            }

            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_MOUSEBTN;
            m.dat.mousebtn.down = down;
            m.dat.mousebtn.vkbutton = code;
            vd_fw__msgbuf_w(&m);
        } break;

        // case WM_NCMBUTTONUP:
        // case WM_NCMBUTTONDOWN:
        // case WM_NCRBUTTONUP:
        // case WM_NCRBUTTONDOWN:
        // case WM_NCLBUTTONUP:
        // case WM_NCLBUTTONDOWN: {

        //     if (!VD_FW_G.draw_decorations) {
        //         int down = 0;
        //         DWORD code = 0;

        //         switch (msg) {

        //             case WM_NCMBUTTONUP:    down = 0; code = VK_MBUTTON; break;
        //             case WM_NCMBUTTONDOWN:  down = 1; code = VK_MBUTTON; break;
        //             case WM_NCRBUTTONUP:    down = 0; code = VK_RBUTTON; break;
        //             case WM_NCRBUTTONDOWN:  down = 1; code = VK_RBUTTON; break;
        //             case WM_NCLBUTTONUP:    down = 0; code = VK_LBUTTON; break;
        //             case WM_NCLBUTTONDOWN:  down = 1; code = VK_LBUTTON; break;
        //             default: break;
        //         }

        //         VdFw__Win32Message m;
        //         m.msg = VD_FW_WIN32_MESSAGE_TYPE_MOUSEBTN;
        //         m.dat.mousebtn.down = down;
        //         m.dat.mousebtn.vkbutton = code;
        //         vd_fw__msgbuf_w(&m);
        //     }

        //     result = DefWindowProc(hwnd, msg, wparam, lparam);
        // } break;

        // @todo(mdodis):
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

            int x = GET_X_LPARAM(lparam);
            int y = GET_Y_LPARAM(lparam);

            if (msg == WM_NCMOUSEMOVE) {
                if (!VD_FW_G.receive_ncmouse_on && VD_FW_G.nccaption_set) {
                    break;
                }

                RECT rect;
                GetWindowRect(hwnd, &rect);

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
            BOOL should_show = (BOOL)wparam;
            ShowCursor(should_show);
        } break;

        case VD_FW_WIN32_FULLSCREEN: {
            VD_FW_WIN32_PROFILE_BEGIN(fw_fullscreen);
            BOOL should_be_fullscreen = (BOOL)lparam;

            if (should_be_fullscreen) {

                GetWindowRect(VD_FW_G.hwnd, &VD_FW_G.windowed_rect);
                GetWindowPlacement(VD_FW_G.hwnd, &VD_FW_G.last_window_placement);

                HMONITOR monitor = MonitorFromWindow(VD_FW_G.hwnd, MONITOR_DEFAULTTOPRIMARY);
                MONITORINFO monitor_info = {0};
                monitor_info.cbSize = sizeof(monitor_info);
                VD_FW__CHECK_NONZERO(GetMonitorInfo(monitor, &monitor_info));

                LONG style = VD_FW_G.last_window_style & ~(WS_OVERLAPPEDWINDOW);
                style |= WS_VISIBLE;

                SetWindowLong(VD_FW_G.hwnd, GWL_STYLE, style);
                SetWindowPos(VD_FW_G.hwnd, HWND_TOP,
                             monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
                             monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
                             monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
                             SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            } else {
                SetWindowLong(VD_FW_G.hwnd, GWL_STYLE, VD_FW_G.last_window_style);
                SetWindowPlacement(VD_FW_G.hwnd, &VD_FW_G.last_window_placement);
                SetWindowPos(VD_FW_G.hwnd, NULL,
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
            WORD width  = LOWORD(lparam);
            WORD height = HIWORD(lparam);

            RECT rect;
            GetWindowRect(VD_FW_G.hwnd, &rect);

            RECT newrect;
            newrect.left = rect.left;
            newrect.right = rect.left + width;
            newrect.top = rect.top;
            newrect.bottom = rect.top + height;

            SetWindowPos(VD_FW_G.hwnd, HWND_TOP,
                         newrect.left, newrect.top,
                         newrect.right - newrect.left,
                         newrect.bottom - newrect.top,
                         SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

        } break;

        case VD_FW_WIN32_UPDATE_TITLE: {
            SetWindowTextA(VD_FW_G.hwnd, VD_FW_G.title);
        } break;

        case WM_MOUSEHWHEEL: {
            if (!VD_FW_G.t_paint_ready) { result = DefWindowProc(hwnd, msg, wparam, lparam); break; }
            int delta = GET_WHEEL_DELTA_WPARAM(wparam);
            float dx = (float)delta / (float)WHEEL_DELTA;
            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_SCROLL;
            m.dat.scroll.dx = dx;
            m.dat.scroll.dy = 0.f;
            vd_fw__msgbuf_w(&m);
        } break;

        case WM_MOUSEWHEEL: {
            if (!VD_FW_G.t_paint_ready) { result = DefWindowProc(hwnd, msg, wparam, lparam); break; }
            int delta = GET_WHEEL_DELTA_WPARAM(wparam);
            float dy = (float)delta / (float)WHEEL_DELTA;
            VdFw__Win32Message m;
            m.msg = VD_FW_WIN32_MESSAGE_TYPE_SCROLL;
            m.dat.scroll.dx = 0.f;
            m.dat.scroll.dy = dy;
            vd_fw__msgbuf_w(&m);
        } break;

        default: {
            result = DefWindowProc(hwnd, msg, wparam, lparam);
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

#if VD_FW_NO_CRT
#pragma function(memset)
void *__cdecl memset(void *dest, int value, size_t num)
{
    __stosb((unsigned char *)dest, (unsigned char)value, num);
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

#undef VD_FW_G
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
#define VD_FW_G Vd_Fw_Globals

@interface VdFwWindowDelegate : NSObject<NSApplicationDelegate, NSWindowDelegate>
@end
@interface VdFwContentView : NSView
@end
@interface VdFwWindow : NSWindow
@end

typedef struct {
    VdFwWindow                  *window;
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
    BOOL                        draw_decorations;
    int                         ncrect_count;
    NSRect                      ncrects[16];
    NSRect                      nccaption;
    int                         nccaption_set;
    BOOL                        mouse_is_locked;
    NSPoint                     last_mouse;
    NSPoint                     mouse_delta;
    NSPoint                     mouse_pos_scaled;
    unsigned char               curr_key_states[VD_FW_KEY_MAX];
    unsigned char               prev_key_states[VD_FW_KEY_MAX];
    int                         focus_changed;
    int                         focused;
} VdFw__MacOsInternalData;

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


static VdFwWindowDelegate *Vd_Fw_Delegate;

@implementation VdFwWindowDelegate
    NSPoint initialLocation;

- (void)windowDidBecomeKey:(NSNotification *)notification {
    VD_FW_G.focus_changed = 1;
    VD_FW_G.focused = 1;
}

- (void)windowDidResignKey:(NSNotification *)notification {
    VD_FW_G.focus_changed = 1;
    VD_FW_G.focused = 0;
}

- (void)windowWillClose:(NSNotification*)notification {
    VD_FW_G.should_close = YES;
}

- (void)applicationDidFinishLaunching:(NSNotification *)notification {
}

- (void)windowDidResize:(NSNotification *)notification {
    [VD_FW_G.content_view setNeedsDisplay: YES];
    [VD_FW_G.gl_context update];
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

// - (BOOL)performKeyEquivalent:(NSEvent *)event {
//     if (event.type == NSEventTypeKeyDown) {
//         // swallow key events you don't want to beep
//         return YES;
//     }
//     return [super performKeyEquivalent:event];
// }
@end

VD_FW_API int vd_fw_init(VdFwInitInfo *info)
{
    VD_FW_G.draw_decorations = 1;
    if (info) {
        VD_FW_G.draw_decorations = !info->window_options.borderless;
    }

    [NSApplication sharedApplication];
    [NSApp setActivationPolicy: NSApplicationActivationPolicyRegular];
    [NSEvent setMouseCoalescingEnabled:NO];

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
        // if (!VD_FW_G.draw_decorations) {
        //     window_style_mask              |= NSWindowStyleMaskFullSizeContentView;
        // }

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
        [VD_FW_G.window           makeKeyAndOrderFront: nil];
        [VD_FW_G.window                   setHasShadow: YES];
        [VD_FW_G.window setAllowsConcurrentViewDrawing: YES];

        NSOpenGLPixelFormatAttribute attrs[] = {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFAAlphaSize, 8,
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
        // NSView *content_view = [VD_FW_G.window contentView];
        [fw_view setWantsLayer: YES];
        // [[fw_view layer] setDrawsAsynchronously: YES];
        [fw_view setAcceptsTouchEvents:YES]; // optional for touch, but ok
        [fw_view addTrackingArea:[[NSTrackingArea alloc] initWithRect:fw_view.bounds
                                                             options:NSTrackingMouseMoved |
                                                                     NSTrackingActiveAlways |
                                                                     NSTrackingInVisibleRect
                                                               owner:fw_view
                                                            userInfo:nil]];

        [VD_FW_G.gl_context setView: fw_view];
        [VD_FW_G.gl_context makeCurrentContext];

        [VD_FW_G.window setDelegate:delegate];
        [NSApp activateIgnoringOtherApps:YES];
    }

    VdFwGlVersion version = VD_FW_GL_VERSION_3_3;
    if (info && info->gl.version != 0) {
        version = info->gl.version;
    }
    vd_fw__load_opengl(version);

    mach_timebase_info(&VD_FW_G.time_base);
    VD_FW_G.last_time = mach_absolute_time();

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
    if (dx) *dx = VD_FW_G.mouse_delta.x; 
    if (dy) *dy = VD_FW_G.mouse_delta.y;
}

VD_FW_API int vd_fw_get_mouse_state(int *x, int *y)
{
    int result = 0;

    @autoreleasepool {
        if (!VD_FW_G.window) {
            if (x) *x = 0;
            if (y) *y = 0;
            return 0;
        }

        // // Mouse location in screen coordinates
        // NSPoint loc = [NSEvent mouseLocation];

        // // Convert to window coordinates
        // NSPoint window_point = [VD_FW_G.window convertPointFromScreen:loc];

        // // Convert to content view coordinates
        // NSView *cv = [VD_FW_G.window contentView];
        // NSPoint view_point = [cv convertPoint:window_point fromView:nil];
        // NSRect cvf = [cv frame];

        // // Cocoa’s origin is bottom-left, OpenGL expects same
        // if (x) *x = (view_point.x) * VD_FW_G.scale;
        // if (y) *y = (cvf.size.height - view_point.y) * VD_FW_G.scale;

        if (x) *x = VD_FW_G.mouse_pos_scaled.x;
        if (y) *y = VD_FW_G.mouse_pos_scaled.y;

        // Get the mouse state
        BOOL left_down  = ([NSEvent pressedMouseButtons] & (1 << 0)) != 0;
        BOOL right_down = ([NSEvent pressedMouseButtons] & (1 << 1)) != 0;

        result |= left_down  ? VD_FW_MOUSE_STATE_LEFT_BUTTON_DOWN : 0;
        result |= right_down ? VD_FW_MOUSE_STATE_RIGHT_BUTTON_DOWN : 0;
    }

    return result;
}

VD_FW_API void vd_fw_set_mouse_locked(int locked)
{
    if (VD_FW_G.mouse_is_locked == locked) {
        return;
    }

    VD_FW_G.mouse_is_locked = locked;

    if (locked) {
        CGDisplayHideCursor(kCGDirectMainDisplay);
    } else {
        CGDisplayShowCursor(kCGDirectMainDisplay);
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
    VD_FW_G.wheel_moved = 0;
    VD_FW_G.wheel[0] = 0.f;
    VD_FW_G.wheel[1] = 0.f;

    VD_FW_G.mouse_delta.x = 0.f;
    VD_FW_G.mouse_delta.y = 0.f;

    VD_FW_G.focus_changed = 0;

    for (int i = 0; i < VD_FW_KEY_MAX; ++i) {
        VD_FW_G.prev_key_states[i] = VD_FW_G.curr_key_states[i];
    }

    @autoreleasepool {
        NSEvent *event;
        while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                           untilDate:[NSDate distantPast]
                                              inMode:NSDefaultRunLoopMode
                                             dequeue:YES]))
        {
            NSEventType type = [event type];

            // Handle Cmd+Q manually
            if (event.type == NSEventTypeKeyDown &&
                (event.modifierFlags & NSEventModifierFlagCommand) &&
                [[event charactersIgnoringModifiers].lowercaseString isEqualToString:@"q"])
            {
                [NSApp terminate:nil];
                continue; // skip sending to AppKit
            }

            int event_handled = 1;

            switch (type) {
                case NSEventTypeScrollWheel: {

                    if ([event hasPreciseScrollingDeltas]) {
                        VD_FW_G.wheel[0] += [event scrollingDeltaX] * 0.05f;
                        VD_FW_G.wheel[1] += [event scrollingDeltaY] * 0.05f;
                    } else {
                        VD_FW_G.wheel[0] += [event deltaX];
                        VD_FW_G.wheel[1] += [event deltaY];
                    }

                    VD_FW_G.wheel_moved = 1;
                } break;

                case NSEventTypeFlagsChanged: {


                    NSEventModifierFlags flags = [event modifierFlags];
                    unsigned short keycode = [event keyCode];

                    unsigned char shift_down = (flags & NSEventModifierFlagShift) ? 1 : 0;
                    unsigned char option_down = (flags & NSEventModifierFlagOption) ? 1 : 0;
                    unsigned char control_down = (flags & NSEventModifierFlagControl) ? 1 : 0;

                    switch (keycode) {
                        case 56: VD_FW_G.curr_key_states[VD_FW_KEY_LSHIFT] = shift_down; break;
                        case 60: VD_FW_G.curr_key_states[VD_FW_KEY_RSHIFT] = shift_down; break;
                        case 59: VD_FW_G.curr_key_states[VD_FW_KEY_LCONTROL] = control_down; break;
                        case 61: VD_FW_G.curr_key_states[VD_FW_KEY_RCONTROL] = option_down; break;
                        default: break;
                    }
                } break;

                case NSEventTypeKeyUp:
                case NSEventTypeKeyDown: {
                    BOOL is_key_down = [event type] == NSEventTypeKeyDown;
                    // BOOL is_repeat = [event isARepeat];
                    unsigned short keycode = [event keyCode];

                    VdFwKey key = vd_fw__translate_mac_keycode(keycode);

                    // VD_FW_G.prev_key_states[key] = VD_FW_G.curr_key_states[key];
                    VD_FW_G.curr_key_states[key] = (unsigned char)is_key_down;

                } break;

                case NSEventTypeLeftMouseDown: {
                    NSPoint view_point = [event locationInWindow];

                    NSPoint p = NSMakePoint(view_point.x, view_point.y);

                    int hit_drag_area = 0;

                    p.x *= VD_FW_G.scale;
                    p.y *= VD_FW_G.scale;

                    if (VD_FW_G.draw_decorations) {
                        break;
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
                } break;

                case NSEventTypeLeftMouseUp: {
                    VD_FW_G.dragging = FALSE;
                } break;

                case NSEventTypeMouseMoved: {

                    NSPoint loc = [event locationInWindow];

                    // Convert to content view coordinates
                    NSView *cv = [VD_FW_G.window contentView];
                    NSRect cvf = [cv frame];

                    NSPoint loc_top_left_origin = NSMakePoint(loc.x, cvf.size.height - loc.y);

                    NSPoint delta = NSMakePoint(
                        loc_top_left_origin.x - VD_FW_G.last_mouse.x,
                        loc_top_left_origin.y - VD_FW_G.last_mouse.y
                    );

                    VD_FW_G.mouse_delta.x += delta.x;
                    VD_FW_G.mouse_delta.y += delta.y;

                    VD_FW_G.mouse_pos_scaled = NSMakePoint(
                        loc_top_left_origin.x * VD_FW_G.scale,
                        loc_top_left_origin.y * VD_FW_G.scale
                    );

                    VD_FW_G.last_mouse = loc_top_left_origin;
                } break;

                case NSEventTypeLeftMouseDragged: {

                    NSPoint view_point = [event locationInWindow];

                    NSPoint p = [VD_FW_G.window convertPointToScreen: view_point];

                    if (VD_FW_G.dragging) {

                        NSPoint new_pos = NSMakePoint(p.x - VD_FW_G.drag_start_location.x,
                                                      p.y - VD_FW_G.drag_start_location.y);

                        [VD_FW_G.window setFrameOrigin: new_pos];
                    }
                } break;

                default: event_handled = 0; break;
            }

            switch (type) {
                case NSEventTypeLeftMouseDown:
                case NSEventTypeLeftMouseUp: {
                    event_handled = 0;
                } break;

                case NSEventTypeLeftMouseDragged:
                case NSEventTypeRightMouseDragged:
                case NSEventTypeOtherMouseDragged:
                case NSEventTypeMouseMoved: {
                    if (!VD_FW_G.mouse_is_locked) {
                        break;
                    }

                    event_handled = 0;
                    NSRect cvf = [VD_FW_G.window frame];

                    NSPoint screen_loc = [NSEvent mouseLocation];

                    CGFloat w = NSMaxX(cvf) - NSMinX(cvf);
                    CGFloat h = NSMaxY(cvf) - NSMinY(cvf);

                    if (!NSPointInRect(screen_loc, cvf)) {
                        CGWarpMouseCursorPosition(CGPointMake(NSMinX(cvf) + w * .5f, NSMinY(cvf) + h * .5f));
                        VD_FW_G.last_mouse.x = w * .5f;
                        VD_FW_G.last_mouse.y = h * .5f;
                    }
                } break;
                default: break;
            }

            if (!event_handled) {
                [NSApp sendEvent: event];
            }
        }
    }

    return !VD_FW_G.should_close;
}

VD_FW_API int vd_fw_swap_buffers(void)
{

    VD_FW_G.last_time = mach_absolute_time();
    @autoreleasepool {
        [VD_FW_G.gl_context flushBuffer];
    }
    return 1;
}

VD_FW_API int vd_fw_get_focused(int *focused)
{
    *focused = VD_FW_G.focused;
    return VD_FW_G.focus_changed;
}

VD_FW_API int vd_fw_get_size(int *w, int *h)
{
    NSRect rect = [[VD_FW_G.window contentView] frame];
    if (w) *w = (int)rect.size.width * VD_FW_G.scale;
    if (h) *h = (int)rect.size.height * VD_FW_G.scale;
    return 0;
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
    if (c > 16) {
        c = 16;
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

#undef VD_FW_G
#endif // _WIN32, __APPLE__

#if !defined(__APPLE__)
/* ----GL VERSION 1.2------------------------------------------------------------------------------------------------ */
PFNGLDRAWRANGEELEMENTSPROC       glDrawRangeElements;
PFNGLTEXIMAGE3DPROC              glTexImage3D;
PFNGLTEXSUBIMAGE3DPROC           glTexSubImage3D;
PFNGLCOPYTEXSUBIMAGE3DPROC       glCopyTexSubImage3D;
/* ----GL VERSION 1.3------------------------------------------------------------------------------------------------ */
PFNGLACTIVETEXTUREPROC           glActiveTexture;
PFNGLSAMPLECOVERAGEPROC          glSampleCoverage;
PFNGLCOMPRESSEDTEXIMAGE3DPROC    glCompressedTexImage3D;
PFNGLCOMPRESSEDTEXIMAGE2DPROC    glCompressedTexImage2D;
PFNGLCOMPRESSEDTEXIMAGE1DPROC    glCompressedTexImage1D;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
PFNGLGETCOMPRESSEDTEXIMAGEPROC   glGetCompressedTexImage;
/* ----GL VERSION 1.4------------------------------------------------------------------------------------------------ */
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
PFNGLMULTIDRAWARRAYSPROC   glMultiDrawArrays;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
PFNGLPOINTPARAMETERFPROC   glPointParameterf;
PFNGLPOINTPARAMETERFVPROC  glPointParameterfv;
PFNGLPOINTPARAMETERIPROC   glPointParameteri;
PFNGLPOINTPARAMETERIVPROC  glPointParameteriv;
PFNGLBLENDCOLORPROC        glBlendColor;
PFNGLBLENDEQUATIONPROC     glBlendEquation;
/* ----GL VERSION 1.5------------------------------------------------------------------------------------------------ */
PFNGLGENQUERIESPROC           glGenQueries;
PFNGLDELETEQUERIESPROC        glDeleteQueries;
PFNGLISQUERYPROC              glIsQuery;
PFNGLBEGINQUERYPROC           glBeginQuery;
PFNGLENDQUERYPROC             glEndQuery;
PFNGLGETQUERYIVPROC           glGetQueryiv;
PFNGLGETQUERYOBJECTIVPROC     glGetQueryObjectiv;
PFNGLGETQUERYOBJECTUIVPROC    glGetQueryObjectuiv;
PFNGLBINDBUFFERPROC           glBindBuffer;
PFNGLDELETEBUFFERSPROC        glDeleteBuffers;
PFNGLGENBUFFERSPROC           glGenBuffers;
PFNGLISBUFFERPROC             glIsBuffer;
PFNGLBUFFERDATAPROC           glBufferData;
PFNGLBUFFERSUBDATAPROC        glBufferSubData;
PFNGLGETBUFFERSUBDATAPROC     glGetBufferSubData;
PFNGLMAPBUFFERPROC            glMapBuffer;
PFNGLUNMAPBUFFERPROC          glUnmapBuffer;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
PFNGLGETBUFFERPOINTERVPROC    glGetBufferPointerv;
/* ----GL VERSION 2.0------------------------------------------------------------------------------------------------ */
PFNGLBLENDEQUATIONSEPARATEPROC    glBlendEquationSeparate;
PFNGLDRAWBUFFERSPROC              glDrawBuffers;
PFNGLSTENCILOPSEPARATEPROC        glStencilOpSeparate;
PFNGLSTENCILFUNCSEPARATEPROC      glStencilFuncSeparate;
PFNGLSTENCILMASKSEPARATEPROC      glStencilMaskSeparate;
PFNGLATTACHSHADERPROC             glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation;
PFNGLCOMPILESHADERPROC            glCompileShader;
PFNGLCREATEPROGRAMPROC            glCreateProgram;
PFNGLCREATESHADERPROC             glCreateShader;
PFNGLDELETEPROGRAMPROC            glDeleteProgram;
PFNGLDELETESHADERPROC             glDeleteShader;
PFNGLDETACHSHADERPROC             glDetachShader;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
PFNGLGETACTIVEATTRIBPROC          glGetActiveAttrib;
PFNGLGETACTIVEUNIFORMPROC         glGetActiveUniform;
PFNGLGETATTACHEDSHADERSPROC       glGetAttachedShaders;
PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
PFNGLGETPROGRAMIVPROC             glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
PFNGLGETSHADERIVPROC              glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
PFNGLGETSHADERSOURCEPROC          glGetShaderSource;
PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation;
PFNGLGETUNIFORMFVPROC             glGetUniformfv;
PFNGLGETUNIFORMIVPROC             glGetUniformiv;
PFNGLGETVERTEXATTRIBDVPROC        glGetVertexAttribdv;
PFNGLGETVERTEXATTRIBFVPROC        glGetVertexAttribfv;
PFNGLGETVERTEXATTRIBIVPROC        glGetVertexAttribiv;
PFNGLGETVERTEXATTRIBPOINTERVPROC  glGetVertexAttribPointerv;
PFNGLISPROGRAMPROC                glIsProgram;
PFNGLISSHADERPROC                 glIsShader;
PFNGLLINKPROGRAMPROC              glLinkProgram;
PFNGLSHADERSOURCEPROC             glShaderSource;
PFNGLUSEPROGRAMPROC               glUseProgram;
PFNGLUNIFORM1FPROC                glUniform1f;
PFNGLUNIFORM2FPROC                glUniform2f;
PFNGLUNIFORM3FPROC                glUniform3f;
PFNGLUNIFORM4FPROC                glUniform4f;
PFNGLUNIFORM1IPROC                glUniform1i;
PFNGLUNIFORM2IPROC                glUniform2i;
PFNGLUNIFORM3IPROC                glUniform3i;
PFNGLUNIFORM4IPROC                glUniform4i;
PFNGLUNIFORM1FVPROC               glUniform1fv;
PFNGLUNIFORM2FVPROC               glUniform2fv;
PFNGLUNIFORM3FVPROC               glUniform3fv;
PFNGLUNIFORM4FVPROC               glUniform4fv;
PFNGLUNIFORM1IVPROC               glUniform1iv;
PFNGLUNIFORM2IVPROC               glUniform2iv;
PFNGLUNIFORM3IVPROC               glUniform3iv;
PFNGLUNIFORM4IVPROC               glUniform4iv;
PFNGLUNIFORMMATRIX2FVPROC         glUniformMatrix2fv;
PFNGLUNIFORMMATRIX3FVPROC         glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv;
PFNGLVALIDATEPROGRAMPROC          glValidateProgram;
PFNGLVERTEXATTRIB1DPROC           glVertexAttrib1d;
PFNGLVERTEXATTRIB1DVPROC          glVertexAttrib1dv;
PFNGLVERTEXATTRIB1FPROC           glVertexAttrib1f;
PFNGLVERTEXATTRIB1FVPROC          glVertexAttrib1fv;
PFNGLVERTEXATTRIB1SPROC           glVertexAttrib1s;
PFNGLVERTEXATTRIB1SVPROC          glVertexAttrib1sv;
PFNGLVERTEXATTRIB2DPROC           glVertexAttrib2d;
PFNGLVERTEXATTRIB2DVPROC          glVertexAttrib2dv;
PFNGLVERTEXATTRIB2FPROC           glVertexAttrib2f;
PFNGLVERTEXATTRIB2FVPROC          glVertexAttrib2fv;
PFNGLVERTEXATTRIB2SPROC           glVertexAttrib2s;
PFNGLVERTEXATTRIB2SVPROC          glVertexAttrib2sv;
PFNGLVERTEXATTRIB3DPROC           glVertexAttrib3d;
PFNGLVERTEXATTRIB3DVPROC          glVertexAttrib3dv;
PFNGLVERTEXATTRIB3FPROC           glVertexAttrib3f;
PFNGLVERTEXATTRIB3FVPROC          glVertexAttrib3fv;
PFNGLVERTEXATTRIB3SPROC           glVertexAttrib3s;
PFNGLVERTEXATTRIB3SVPROC          glVertexAttrib3sv;
PFNGLVERTEXATTRIB4NBVPROC         glVertexAttrib4Nbv;
PFNGLVERTEXATTRIB4NIVPROC         glVertexAttrib4Niv;
PFNGLVERTEXATTRIB4NSVPROC         glVertexAttrib4Nsv;
PFNGLVERTEXATTRIB4NUBPROC         glVertexAttrib4Nub;
PFNGLVERTEXATTRIB4NUBVPROC        glVertexAttrib4Nubv;
PFNGLVERTEXATTRIB4NUIVPROC        glVertexAttrib4Nuiv;
PFNGLVERTEXATTRIB4NUSVPROC        glVertexAttrib4Nusv;
PFNGLVERTEXATTRIB4BVPROC          glVertexAttrib4bv;
PFNGLVERTEXATTRIB4DPROC           glVertexAttrib4d;
PFNGLVERTEXATTRIB4DVPROC          glVertexAttrib4dv;
PFNGLVERTEXATTRIB4FPROC           glVertexAttrib4f;
PFNGLVERTEXATTRIB4FVPROC          glVertexAttrib4fv;
PFNGLVERTEXATTRIB4IVPROC          glVertexAttrib4iv;
PFNGLVERTEXATTRIB4SPROC           glVertexAttrib4s;
PFNGLVERTEXATTRIB4SVPROC          glVertexAttrib4sv;
PFNGLVERTEXATTRIB4UBVPROC         glVertexAttrib4ubv;
PFNGLVERTEXATTRIB4UIVPROC         glVertexAttrib4uiv;
PFNGLVERTEXATTRIB4USVPROC         glVertexAttrib4usv;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
/* ----GL VERSION 2.1------------------------------------------------------------------------------------------------ */
PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv;
PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv;
PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;
/* ----GL VERSION 3.0------------------------------------------------------------------------------------------------ */
PFNGLCOLORMASKIPROC                          glColorMaski;
PFNGLGETBOOLEANI_VPROC                       glGetBooleani_v;
PFNGLGETINTEGERI_VPROC                       glGetIntegeri_v;
PFNGLENABLEIPROC                             glEnablei;
PFNGLDISABLEIPROC                            glDisablei;
PFNGLISENABLEDIPROC                          glIsEnabledi;
PFNGLBEGINTRANSFORMFEEDBACKPROC              glBeginTransformFeedback;
PFNGLENDTRANSFORMFEEDBACKPROC                glEndTransformFeedback;
PFNGLBINDBUFFERRANGEPROC                     glBindBufferRange;
PFNGLBINDBUFFERBASEPROC                      glBindBufferBase;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC           glTransformFeedbackVaryings;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC         glGetTransformFeedbackVarying;
PFNGLCLAMPCOLORPROC                          glClampColor;
PFNGLBEGINCONDITIONALRENDERPROC              glBeginConditionalRender;
PFNGLENDCONDITIONALRENDERPROC                glEndConditionalRender;
PFNGLVERTEXATTRIBIPOINTERPROC                glVertexAttribIPointer;
PFNGLGETVERTEXATTRIBIIVPROC                  glGetVertexAttribIiv;
PFNGLGETVERTEXATTRIBIUIVPROC                 glGetVertexAttribIuiv;
PFNGLVERTEXATTRIBI1IPROC                     glVertexAttribI1i;
PFNGLVERTEXATTRIBI2IPROC                     glVertexAttribI2i;
PFNGLVERTEXATTRIBI3IPROC                     glVertexAttribI3i;
PFNGLVERTEXATTRIBI4IPROC                     glVertexAttribI4i;
PFNGLVERTEXATTRIBI1UIPROC                    glVertexAttribI1ui;
PFNGLVERTEXATTRIBI2UIPROC                    glVertexAttribI2ui;
PFNGLVERTEXATTRIBI3UIPROC                    glVertexAttribI3ui;
PFNGLVERTEXATTRIBI4UIPROC                    glVertexAttribI4ui;
PFNGLVERTEXATTRIBI1IVPROC                    glVertexAttribI1iv;
PFNGLVERTEXATTRIBI2IVPROC                    glVertexAttribI2iv;
PFNGLVERTEXATTRIBI3IVPROC                    glVertexAttribI3iv;
PFNGLVERTEXATTRIBI4IVPROC                    glVertexAttribI4iv;
PFNGLVERTEXATTRIBI1UIVPROC                   glVertexAttribI1uiv;
PFNGLVERTEXATTRIBI2UIVPROC                   glVertexAttribI2uiv;
PFNGLVERTEXATTRIBI3UIVPROC                   glVertexAttribI3uiv;
PFNGLVERTEXATTRIBI4UIVPROC                   glVertexAttribI4uiv;
PFNGLVERTEXATTRIBI4BVPROC                    glVertexAttribI4bv;
PFNGLVERTEXATTRIBI4SVPROC                    glVertexAttribI4sv;
PFNGLVERTEXATTRIBI4UBVPROC                   glVertexAttribI4ubv;
PFNGLVERTEXATTRIBI4USVPROC                   glVertexAttribI4usv;
PFNGLGETUNIFORMUIVPROC                       glGetUniformuiv;
PFNGLBINDFRAGDATALOCATIONPROC                glBindFragDataLocation;
PFNGLGETFRAGDATALOCATIONPROC                 glGetFragDataLocation;
PFNGLUNIFORM1UIPROC                          glUniform1ui;
PFNGLUNIFORM2UIPROC                          glUniform2ui;
PFNGLUNIFORM3UIPROC                          glUniform3ui;
PFNGLUNIFORM4UIPROC                          glUniform4ui;
PFNGLUNIFORM1UIVPROC                         glUniform1uiv;
PFNGLUNIFORM2UIVPROC                         glUniform2uiv;
PFNGLUNIFORM3UIVPROC                         glUniform3uiv;
PFNGLUNIFORM4UIVPROC                         glUniform4uiv;
PFNGLTEXPARAMETERIIVPROC                     glTexParameterIiv;
PFNGLTEXPARAMETERIUIVPROC                    glTexParameterIuiv;
PFNGLGETTEXPARAMETERIIVPROC                  glGetTexParameterIiv;
PFNGLGETTEXPARAMETERIUIVPROC                 glGetTexParameterIuiv;
PFNGLCLEARBUFFERIVPROC                       glClearBufferiv;
PFNGLCLEARBUFFERUIVPROC                      glClearBufferuiv;
PFNGLCLEARBUFFERFVPROC                       glClearBufferfv;
PFNGLCLEARBUFFERFIPROC                       glClearBufferfi;
PFNGLGETSTRINGIPROC                          glGetStringi;
PFNGLISRENDERBUFFERPROC                      glIsRenderbuffer;
PFNGLBINDRENDERBUFFERPROC                    glBindRenderbuffer;
PFNGLDELETERENDERBUFFERSPROC                 glDeleteRenderbuffers;
PFNGLGENRENDERBUFFERSPROC                    glGenRenderbuffers;
PFNGLRENDERBUFFERSTORAGEPROC                 glRenderbufferStorage;
PFNGLGETRENDERBUFFERPARAMETERIVPROC          glGetRenderbufferParameteriv;
PFNGLISFRAMEBUFFERPROC                       glIsFramebuffer;
PFNGLBINDFRAMEBUFFERPROC                     glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC                  glDeleteFramebuffers;
PFNGLGENFRAMEBUFFERSPROC                     glGenFramebuffers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC              glCheckFramebufferStatus;
PFNGLFRAMEBUFFERTEXTURE1DPROC                glFramebufferTexture1D;
PFNGLFRAMEBUFFERTEXTURE2DPROC                glFramebufferTexture2D;
PFNGLFRAMEBUFFERTEXTURE3DPROC                glFramebufferTexture3D;
PFNGLFRAMEBUFFERRENDERBUFFERPROC             glFramebufferRenderbuffer;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
PFNGLGENERATEMIPMAPPROC                      glGenerateMipmap;
PFNGLBLITFRAMEBUFFERPROC                     glBlitFramebuffer;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC      glRenderbufferStorageMultisample;
PFNGLFRAMEBUFFERTEXTURELAYERPROC             glFramebufferTextureLayer;
PFNGLMAPBUFFERRANGEPROC                      glMapBufferRange;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC              glFlushMappedBufferRange;
PFNGLBINDVERTEXARRAYPROC                     glBindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC                  glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC                     glGenVertexArrays;
PFNGLISVERTEXARRAYPROC                       glIsVertexArray;
/* ----GL VERSION 3.1------------------------------------------------------------------------------------------------ */
PFNGLDRAWARRAYSINSTANCEDPROC       glDrawArraysInstanced;
PFNGLDRAWELEMENTSINSTANCEDPROC     glDrawElementsInstanced;
PFNGLTEXBUFFERPROC                 glTexBuffer;
PFNGLPRIMITIVERESTARTINDEXPROC     glPrimitiveRestartIndex;
PFNGLCOPYBUFFERSUBDATAPROC         glCopyBufferSubData;
PFNGLGETUNIFORMINDICESPROC         glGetUniformIndices;
PFNGLGETACTIVEUNIFORMSIVPROC       glGetActiveUniformsiv;
PFNGLGETACTIVEUNIFORMNAMEPROC      glGetActiveUniformName;
PFNGLGETUNIFORMBLOCKINDEXPROC      glGetUniformBlockIndex;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC   glGetActiveUniformBlockiv;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
PFNGLUNIFORMBLOCKBINDINGPROC       glUniformBlockBinding;
/* ----GL VERSION 3.2------------------------------------------------------------------------------------------------ */
PFNGLDRAWELEMENTSBASEVERTEXPROC          glDrawElementsBaseVertex;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC     glDrawRangeElementsBaseVertex;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC     glMultiDrawElementsBaseVertex;
PFNGLPROVOKINGVERTEXPROC                 glProvokingVertex;
PFNGLFENCESYNCPROC                       glFenceSync;
PFNGLISSYNCPROC                          glIsSync;
PFNGLDELETESYNCPROC                      glDeleteSync;
PFNGLCLIENTWAITSYNCPROC                  glClientWaitSync;
PFNGLWAITSYNCPROC                        glWaitSync;
PFNGLGETINTEGER64VPROC                   glGetInteger64v;
PFNGLGETSYNCIVPROC                       glGetSynciv;
PFNGLGETINTEGER64I_VPROC                 glGetInteger64i_v;
PFNGLGETBUFFERPARAMETERI64VPROC          glGetBufferParameteri64v;
PFNGLFRAMEBUFFERTEXTUREPROC              glFramebufferTexture;
PFNGLTEXIMAGE2DMULTISAMPLEPROC           glTexImage2DMultisample;
PFNGLTEXIMAGE3DMULTISAMPLEPROC           glTexImage3DMultisample;
PFNGLGETMULTISAMPLEFVPROC                glGetMultisamplefv;
PFNGLSAMPLEMASKIPROC                     glSampleMaski;
/* ----GL VERSION 3.3------------------------------------------------------------------------------------------------ */
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed;
PFNGLGETFRAGDATAINDEXPROC            glGetFragDataIndex;
PFNGLGENSAMPLERSPROC                 glGenSamplers;
PFNGLDELETESAMPLERSPROC              glDeleteSamplers;
PFNGLISSAMPLERPROC                   glIsSampler;
PFNGLBINDSAMPLERPROC                 glBindSampler;
PFNGLSAMPLERPARAMETERIPROC           glSamplerParameteri;
PFNGLSAMPLERPARAMETERIVPROC          glSamplerParameteriv;
PFNGLSAMPLERPARAMETERFPROC           glSamplerParameterf;
PFNGLSAMPLERPARAMETERFVPROC          glSamplerParameterfv;
PFNGLSAMPLERPARAMETERIIVPROC         glSamplerParameterIiv;
PFNGLSAMPLERPARAMETERIUIVPROC        glSamplerParameterIuiv;
PFNGLGETSAMPLERPARAMETERIVPROC       glGetSamplerParameteriv;
PFNGLGETSAMPLERPARAMETERIIVPROC      glGetSamplerParameterIiv;
PFNGLGETSAMPLERPARAMETERFVPROC       glGetSamplerParameterfv;
PFNGLGETSAMPLERPARAMETERIUIVPROC     glGetSamplerParameterIuiv;
PFNGLQUERYCOUNTERPROC                glQueryCounter;
PFNGLGETQUERYOBJECTI64VPROC          glGetQueryObjecti64v;
PFNGLGETQUERYOBJECTUI64VPROC         glGetQueryObjectui64v;
PFNGLVERTEXATTRIBDIVISORPROC         glVertexAttribDivisor;
PFNGLVERTEXATTRIBP1UIPROC            glVertexAttribP1ui;
PFNGLVERTEXATTRIBP1UIVPROC           glVertexAttribP1uiv;
PFNGLVERTEXATTRIBP2UIPROC            glVertexAttribP2ui;
PFNGLVERTEXATTRIBP2UIVPROC           glVertexAttribP2uiv;
PFNGLVERTEXATTRIBP3UIPROC            glVertexAttribP3ui;
PFNGLVERTEXATTRIBP3UIVPROC           glVertexAttribP3uiv;
PFNGLVERTEXATTRIBP4UIPROC            glVertexAttribP4ui;
PFNGLVERTEXATTRIBP4UIVPROC           glVertexAttribP4uiv;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
/* ----GL VERSION 4.0------------------------------------------------------------------------------------------------ */
PFNGLMINSAMPLESHADINGPROC               glMinSampleShading;
PFNGLBLENDEQUATIONIPROC                 glBlendEquationi;
PFNGLBLENDEQUATIONSEPARATEIPROC         glBlendEquationSeparatei;
PFNGLBLENDFUNCIPROC                     glBlendFunci;
PFNGLBLENDFUNCSEPARATEIPROC             glBlendFuncSeparatei;
PFNGLDRAWARRAYSINDIRECTPROC             glDrawArraysIndirect;
PFNGLDRAWELEMENTSINDIRECTPROC           glDrawElementsIndirect;
PFNGLUNIFORM1DPROC                      glUniform1d;
PFNGLUNIFORM2DPROC                      glUniform2d;
PFNGLUNIFORM3DPROC                      glUniform3d;
PFNGLUNIFORM4DPROC                      glUniform4d;
PFNGLUNIFORM1DVPROC                     glUniform1dv;
PFNGLUNIFORM2DVPROC                     glUniform2dv;
PFNGLUNIFORM3DVPROC                     glUniform3dv;
PFNGLUNIFORM4DVPROC                     glUniform4dv;
PFNGLUNIFORMMATRIX2DVPROC               glUniformMatrix2dv;
PFNGLUNIFORMMATRIX3DVPROC               glUniformMatrix3dv;
PFNGLUNIFORMMATRIX4DVPROC               glUniformMatrix4dv;
PFNGLUNIFORMMATRIX2X3DVPROC             glUniformMatrix2x3dv;
PFNGLUNIFORMMATRIX2X4DVPROC             glUniformMatrix2x4dv;
PFNGLUNIFORMMATRIX3X2DVPROC             glUniformMatrix3x2dv;
PFNGLUNIFORMMATRIX3X4DVPROC             glUniformMatrix3x4dv;
PFNGLUNIFORMMATRIX4X2DVPROC             glUniformMatrix4x2dv;
PFNGLUNIFORMMATRIX4X3DVPROC             glUniformMatrix4x3dv;
PFNGLGETUNIFORMDVPROC                   glGetUniformdv;
PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC   glGetSubroutineUniformLocation;
PFNGLGETSUBROUTINEINDEXPROC             glGetSubroutineIndex;
PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC   glGetActiveSubroutineUniformiv;
PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC glGetActiveSubroutineUniformName;
PFNGLGETACTIVESUBROUTINENAMEPROC        glGetActiveSubroutineName;
PFNGLUNIFORMSUBROUTINESUIVPROC          glUniformSubroutinesuiv;
PFNGLGETUNIFORMSUBROUTINEUIVPROC        glGetUniformSubroutineuiv;
PFNGLGETPROGRAMSTAGEIVPROC              glGetProgramStageiv;
PFNGLPATCHPARAMETERIPROC                glPatchParameteri;
PFNGLPATCHPARAMETERFVPROC               glPatchParameterfv;
PFNGLBINDTRANSFORMFEEDBACKPROC          glBindTransformFeedback;
PFNGLDELETETRANSFORMFEEDBACKSPROC       glDeleteTransformFeedbacks;
PFNGLGENTRANSFORMFEEDBACKSPROC          glGenTransformFeedbacks;
PFNGLISTRANSFORMFEEDBACKPROC            glIsTransformFeedback;
PFNGLPAUSETRANSFORMFEEDBACKPROC         glPauseTransformFeedback;
PFNGLRESUMETRANSFORMFEEDBACKPROC        glResumeTransformFeedback;
PFNGLDRAWTRANSFORMFEEDBACKPROC          glDrawTransformFeedback;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC    glDrawTransformFeedbackStream;
PFNGLBEGINQUERYINDEXEDPROC              glBeginQueryIndexed;
PFNGLENDQUERYINDEXEDPROC                glEndQueryIndexed;
PFNGLGETQUERYINDEXEDIVPROC              glGetQueryIndexediv;
/* ----GL VERSION 4.1------------------------------------------------------------------------------------------------ */
PFNGLRELEASESHADERCOMPILERPROC     glReleaseShaderCompiler;
PFNGLSHADERBINARYPROC              glShaderBinary;
PFNGLGETSHADERPRECISIONFORMATPROC  glGetShaderPrecisionFormat;
PFNGLDEPTHRANGEFPROC               glDepthRangef;
PFNGLCLEARDEPTHFPROC               glClearDepthf;
PFNGLGETPROGRAMBINARYPROC          glGetProgramBinary;
PFNGLPROGRAMBINARYPROC             glProgramBinary;
PFNGLPROGRAMPARAMETERIPROC         glProgramParameteri;
PFNGLUSEPROGRAMSTAGESPROC          glUseProgramStages;
PFNGLACTIVESHADERPROGRAMPROC       glActiveShaderProgram;
PFNGLCREATESHADERPROGRAMVPROC      glCreateShaderProgramv;
PFNGLBINDPROGRAMPIPELINEPROC       glBindProgramPipeline;
PFNGLDELETEPROGRAMPIPELINESPROC    glDeleteProgramPipelines;
PFNGLGENPROGRAMPIPELINESPROC       glGenProgramPipelines;
PFNGLISPROGRAMPIPELINEPROC         glIsProgramPipeline;
PFNGLGETPROGRAMPIPELINEIVPROC      glGetProgramPipelineiv;
PFNGLPROGRAMUNIFORM1IPROC          glProgramUniform1i;
PFNGLPROGRAMUNIFORM1IVPROC         glProgramUniform1iv;
PFNGLPROGRAMUNIFORM1FPROC          glProgramUniform1f;
PFNGLPROGRAMUNIFORM1FVPROC         glProgramUniform1fv;
PFNGLPROGRAMUNIFORM1DPROC          glProgramUniform1d;
PFNGLPROGRAMUNIFORM1DVPROC         glProgramUniform1dv;
PFNGLPROGRAMUNIFORM1UIPROC         glProgramUniform1ui;
PFNGLPROGRAMUNIFORM1UIVPROC        glProgramUniform1uiv;
PFNGLPROGRAMUNIFORM2IPROC          glProgramUniform2i;
PFNGLPROGRAMUNIFORM2IVPROC         glProgramUniform2iv;
PFNGLPROGRAMUNIFORM2FPROC          glProgramUniform2f;
PFNGLPROGRAMUNIFORM2FVPROC         glProgramUniform2fv;
PFNGLPROGRAMUNIFORM2DPROC          glProgramUniform2d;
PFNGLPROGRAMUNIFORM2DVPROC         glProgramUniform2dv;
PFNGLPROGRAMUNIFORM2UIPROC         glProgramUniform2ui;
PFNGLPROGRAMUNIFORM2UIVPROC        glProgramUniform2uiv;
PFNGLPROGRAMUNIFORM3IPROC          glProgramUniform3i;
PFNGLPROGRAMUNIFORM3IVPROC         glProgramUniform3iv;
PFNGLPROGRAMUNIFORM3FPROC          glProgramUniform3f;
PFNGLPROGRAMUNIFORM3FVPROC         glProgramUniform3fv;
PFNGLPROGRAMUNIFORM3DPROC          glProgramUniform3d;
PFNGLPROGRAMUNIFORM3DVPROC         glProgramUniform3dv;
PFNGLPROGRAMUNIFORM3UIPROC         glProgramUniform3ui;
PFNGLPROGRAMUNIFORM3UIVPROC        glProgramUniform3uiv;
PFNGLPROGRAMUNIFORM4IPROC          glProgramUniform4i;
PFNGLPROGRAMUNIFORM4IVPROC         glProgramUniform4iv;
PFNGLPROGRAMUNIFORM4FPROC          glProgramUniform4f;
PFNGLPROGRAMUNIFORM4FVPROC         glProgramUniform4fv;
PFNGLPROGRAMUNIFORM4DPROC          glProgramUniform4d;
PFNGLPROGRAMUNIFORM4DVPROC         glProgramUniform4dv;
PFNGLPROGRAMUNIFORM4UIPROC         glProgramUniform4ui;
PFNGLPROGRAMUNIFORM4UIVPROC        glProgramUniform4uiv;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC   glProgramUniformMatrix2fv;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC   glProgramUniformMatrix3fv;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC   glProgramUniformMatrix4fv;
PFNGLPROGRAMUNIFORMMATRIX2DVPROC   glProgramUniformMatrix2dv;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC   glProgramUniformMatrix3dv;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC   glProgramUniformMatrix4dv;
PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv;
PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv;
PFNGLVALIDATEPROGRAMPIPELINEPROC   glValidateProgramPipeline;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog;
PFNGLVERTEXATTRIBL1DPROC           glVertexAttribL1d;
PFNGLVERTEXATTRIBL2DPROC           glVertexAttribL2d;
PFNGLVERTEXATTRIBL3DPROC           glVertexAttribL3d;
PFNGLVERTEXATTRIBL4DPROC           glVertexAttribL4d;
PFNGLVERTEXATTRIBL1DVPROC          glVertexAttribL1dv;
PFNGLVERTEXATTRIBL2DVPROC          glVertexAttribL2dv;
PFNGLVERTEXATTRIBL3DVPROC          glVertexAttribL3dv;
PFNGLVERTEXATTRIBL4DVPROC          glVertexAttribL4dv;
PFNGLVERTEXATTRIBLPOINTERPROC      glVertexAttribLPointer;
PFNGLGETVERTEXATTRIBLDVPROC        glGetVertexAttribLdv;
PFNGLVIEWPORTARRAYVPROC            glViewportArrayv;
PFNGLVIEWPORTINDEXEDFPROC          glViewportIndexedf;
PFNGLVIEWPORTINDEXEDFVPROC         glViewportIndexedfv;
PFNGLSCISSORARRAYVPROC             glScissorArrayv;
PFNGLSCISSORINDEXEDPROC            glScissorIndexed;
PFNGLSCISSORINDEXEDVPROC           glScissorIndexedv;
PFNGLDEPTHRANGEARRAYVPROC          glDepthRangeArrayv;
PFNGLDEPTHRANGEINDEXEDPROC         glDepthRangeIndexed;
PFNGLGETFLOATI_VPROC               glGetFloati_v;
PFNGLGETDOUBLEI_VPROC              glGetDoublei_v;
/* ----GL VERSION 4.2------------------------------------------------------------------------------------------------ */
PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC             glDrawArraysInstancedBaseInstance;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC           glDrawElementsInstancedBaseInstance;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance;
PFNGLGETINTERNALFORMATIVPROC                         glGetInternalformativ;
PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC              glGetActiveAtomicCounterBufferiv;
PFNGLBINDIMAGETEXTUREPROC                            glBindImageTexture;
PFNGLMEMORYBARRIERPROC                               glMemoryBarrier;
PFNGLTEXSTORAGE1DPROC                                glTexStorage1D;
PFNGLTEXSTORAGE2DPROC                                glTexStorage2D;
PFNGLTEXSTORAGE3DPROC                                glTexStorage3D;
PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC              glDrawTransformFeedbackInstanced;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC        glDrawTransformFeedbackStreamInstanced;
/* ----GL VERSION 4.3------------------------------------------------------------------------------------------------ */
PFNGLCLEARBUFFERDATAPROC                 glClearBufferData;
PFNGLCLEARBUFFERSUBDATAPROC              glClearBufferSubData;
PFNGLDISPATCHCOMPUTEPROC                 glDispatchCompute;
PFNGLDISPATCHCOMPUTEINDIRECTPROC         glDispatchComputeIndirect;
PFNGLCOPYIMAGESUBDATAPROC                glCopyImageSubData;
PFNGLFRAMEBUFFERPARAMETERIPROC           glFramebufferParameteri;
PFNGLGETFRAMEBUFFERPARAMETERIVPROC       glGetFramebufferParameteriv;
PFNGLGETINTERNALFORMATI64VPROC           glGetInternalformati64v;
PFNGLINVALIDATETEXSUBIMAGEPROC           glInvalidateTexSubImage;
PFNGLINVALIDATETEXIMAGEPROC              glInvalidateTexImage;
PFNGLINVALIDATEBUFFERSUBDATAPROC         glInvalidateBufferSubData;
PFNGLINVALIDATEBUFFERDATAPROC            glInvalidateBufferData;
PFNGLINVALIDATEFRAMEBUFFERPROC           glInvalidateFramebuffer;
PFNGLINVALIDATESUBFRAMEBUFFERPROC        glInvalidateSubFramebuffer;
PFNGLMULTIDRAWARRAYSINDIRECTPROC         glMultiDrawArraysIndirect;
PFNGLMULTIDRAWELEMENTSINDIRECTPROC       glMultiDrawElementsIndirect;
PFNGLGETPROGRAMINTERFACEIVPROC           glGetProgramInterfaceiv;
PFNGLGETPROGRAMRESOURCEINDEXPROC         glGetProgramResourceIndex;
PFNGLGETPROGRAMRESOURCENAMEPROC          glGetProgramResourceName;
PFNGLGETPROGRAMRESOURCEIVPROC            glGetProgramResourceiv;
PFNGLGETPROGRAMRESOURCELOCATIONPROC      glGetProgramResourceLocation;
PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex;
PFNGLSHADERSTORAGEBLOCKBINDINGPROC       glShaderStorageBlockBinding;
PFNGLTEXBUFFERRANGEPROC                  glTexBufferRange;
PFNGLTEXSTORAGE2DMULTISAMPLEPROC         glTexStorage2DMultisample;
PFNGLTEXSTORAGE3DMULTISAMPLEPROC         glTexStorage3DMultisample;
PFNGLTEXTUREVIEWPROC                     glTextureView;
PFNGLBINDVERTEXBUFFERPROC                glBindVertexBuffer;
PFNGLVERTEXATTRIBFORMATPROC              glVertexAttribFormat;
PFNGLVERTEXATTRIBIFORMATPROC             glVertexAttribIFormat;
PFNGLVERTEXATTRIBLFORMATPROC             glVertexAttribLFormat;
PFNGLVERTEXATTRIBBINDINGPROC             glVertexAttribBinding;
PFNGLVERTEXBINDINGDIVISORPROC            glVertexBindingDivisor;
PFNGLDEBUGMESSAGECONTROLPROC             glDebugMessageControl;
PFNGLDEBUGMESSAGEINSERTPROC              glDebugMessageInsert;
PFNGLDEBUGMESSAGECALLBACKPROC            glDebugMessageCallback;
PFNGLGETDEBUGMESSAGELOGPROC              glGetDebugMessageLog;
PFNGLPUSHDEBUGGROUPPROC                  glPushDebugGroup;
PFNGLPOPDEBUGGROUPPROC                   glPopDebugGroup;
PFNGLOBJECTLABELPROC                     glObjectLabel;
PFNGLGETOBJECTLABELPROC                  glGetObjectLabel;
PFNGLOBJECTPTRLABELPROC                  glObjectPtrLabel;
PFNGLGETOBJECTPTRLABELPROC               glGetObjectPtrLabel;
/* ----GL VERSION 4.4------------------------------------------------------------------------------------------------ */
PFNGLBUFFERSTORAGEPROC     glBufferStorage;
PFNGLCLEARTEXIMAGEPROC     glClearTexImage;
PFNGLCLEARTEXSUBIMAGEPROC  glClearTexSubImage;
PFNGLBINDBUFFERSBASEPROC   glBindBuffersBase;
PFNGLBINDBUFFERSRANGEPROC  glBindBuffersRange;
PFNGLBINDTEXTURESPROC      glBindTextures;
PFNGLBINDSAMPLERSPROC      glBindSamplers;
PFNGLBINDIMAGETEXTURESPROC glBindImageTextures;
PFNGLBINDVERTEXBUFFERSPROC glBindVertexBuffers;
/* ----GL VERSION 4.5------------------------------------------------------------------------------------------------ */
PFNGLCLIPCONTROLPROC                              glClipControl;
PFNGLCREATETRANSFORMFEEDBACKSPROC                 glCreateTransformFeedbacks;
PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC              glTransformFeedbackBufferBase;
PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC             glTransformFeedbackBufferRange;
PFNGLGETTRANSFORMFEEDBACKIVPROC                   glGetTransformFeedbackiv;
PFNGLGETTRANSFORMFEEDBACKI_VPROC                  glGetTransformFeedbacki_v;
PFNGLGETTRANSFORMFEEDBACKI64_VPROC                glGetTransformFeedbacki64_v;
PFNGLCREATEBUFFERSPROC                            glCreateBuffers;
PFNGLNAMEDBUFFERSTORAGEPROC                       glNamedBufferStorage;
PFNGLNAMEDBUFFERDATAPROC                          glNamedBufferData;
PFNGLNAMEDBUFFERSUBDATAPROC                       glNamedBufferSubData;
PFNGLCOPYNAMEDBUFFERSUBDATAPROC                   glCopyNamedBufferSubData;
PFNGLCLEARNAMEDBUFFERDATAPROC                     glClearNamedBufferData;
PFNGLCLEARNAMEDBUFFERSUBDATAPROC                  glClearNamedBufferSubData;
PFNGLMAPNAMEDBUFFERPROC                           glMapNamedBuffer;
PFNGLMAPNAMEDBUFFERRANGEPROC                      glMapNamedBufferRange;
PFNGLUNMAPNAMEDBUFFERPROC                         glUnmapNamedBuffer;
PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC              glFlushMappedNamedBufferRange;
PFNGLGETNAMEDBUFFERPARAMETERIVPROC                glGetNamedBufferParameteriv;
PFNGLGETNAMEDBUFFERPARAMETERI64VPROC              glGetNamedBufferParameteri64v;
PFNGLGETNAMEDBUFFERPOINTERVPROC                   glGetNamedBufferPointerv;
PFNGLGETNAMEDBUFFERSUBDATAPROC                    glGetNamedBufferSubData;
PFNGLCREATEFRAMEBUFFERSPROC                       glCreateFramebuffers;
PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC             glNamedFramebufferRenderbuffer;
PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC               glNamedFramebufferParameteri;
PFNGLNAMEDFRAMEBUFFERTEXTUREPROC                  glNamedFramebufferTexture;
PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC             glNamedFramebufferTextureLayer;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC               glNamedFramebufferDrawBuffer;
PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC              glNamedFramebufferDrawBuffers;
PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC               glNamedFramebufferReadBuffer;
PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC           glInvalidateNamedFramebufferData;
PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC        glInvalidateNamedFramebufferSubData;
PFNGLCLEARNAMEDFRAMEBUFFERIVPROC                  glClearNamedFramebufferiv;
PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC                 glClearNamedFramebufferuiv;
PFNGLCLEARNAMEDFRAMEBUFFERFVPROC                  glClearNamedFramebufferfv;
PFNGLCLEARNAMEDFRAMEBUFFERFIPROC                  glClearNamedFramebufferfi;
PFNGLBLITNAMEDFRAMEBUFFERPROC                     glBlitNamedFramebuffer;
PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC              glCheckNamedFramebufferStatus;
PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC           glGetNamedFramebufferParameteriv;
PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetNamedFramebufferAttachmentParameteriv;
PFNGLCREATERENDERBUFFERSPROC                      glCreateRenderbuffers;
PFNGLNAMEDRENDERBUFFERSTORAGEPROC                 glNamedRenderbufferStorage;
PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC      glNamedRenderbufferStorageMultisample;
PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC          glGetNamedRenderbufferParameteriv;
PFNGLCREATETEXTURESPROC                           glCreateTextures;
PFNGLTEXTUREBUFFERPROC                            glTextureBuffer;
PFNGLTEXTUREBUFFERRANGEPROC                       glTextureBufferRange;
PFNGLTEXTURESTORAGE1DPROC                         glTextureStorage1D;
PFNGLTEXTURESTORAGE2DPROC                         glTextureStorage2D;
PFNGLTEXTURESTORAGE3DPROC                         glTextureStorage3D;
PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC              glTextureStorage2DMultisample;
PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC              glTextureStorage3DMultisample;
PFNGLTEXTURESUBIMAGE1DPROC                        glTextureSubImage1D;
PFNGLTEXTURESUBIMAGE2DPROC                        glTextureSubImage2D;
PFNGLTEXTURESUBIMAGE3DPROC                        glTextureSubImage3D;
PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC              glCompressedTextureSubImage1D;
PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC              glCompressedTextureSubImage2D;
PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC              glCompressedTextureSubImage3D;
PFNGLCOPYTEXTURESUBIMAGE1DPROC                    glCopyTextureSubImage1D;
PFNGLCOPYTEXTURESUBIMAGE2DPROC                    glCopyTextureSubImage2D;
PFNGLCOPYTEXTURESUBIMAGE3DPROC                    glCopyTextureSubImage3D;
PFNGLTEXTUREPARAMETERFPROC                        glTextureParameterf;
PFNGLTEXTUREPARAMETERFVPROC                       glTextureParameterfv;
PFNGLTEXTUREPARAMETERIPROC                        glTextureParameteri;
PFNGLTEXTUREPARAMETERIIVPROC                      glTextureParameterIiv;
PFNGLTEXTUREPARAMETERIUIVPROC                     glTextureParameterIuiv;
PFNGLTEXTUREPARAMETERIVPROC                       glTextureParameteriv;
PFNGLGENERATETEXTUREMIPMAPPROC                    glGenerateTextureMipmap;
PFNGLBINDTEXTUREUNITPROC                          glBindTextureUnit;
PFNGLGETTEXTUREIMAGEPROC                          glGetTextureImage;
PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC                glGetCompressedTextureImage;
PFNGLGETTEXTURELEVELPARAMETERFVPROC               glGetTextureLevelParameterfv;
PFNGLGETTEXTURELEVELPARAMETERIVPROC               glGetTextureLevelParameteriv;
PFNGLGETTEXTUREPARAMETERFVPROC                    glGetTextureParameterfv;
PFNGLGETTEXTUREPARAMETERIIVPROC                   glGetTextureParameterIiv;
PFNGLGETTEXTUREPARAMETERIUIVPROC                  glGetTextureParameterIuiv;
PFNGLGETTEXTUREPARAMETERIVPROC                    glGetTextureParameteriv;
PFNGLCREATEVERTEXARRAYSPROC                       glCreateVertexArrays;
PFNGLDISABLEVERTEXARRAYATTRIBPROC                 glDisableVertexArrayAttrib;
PFNGLENABLEVERTEXARRAYATTRIBPROC                  glEnableVertexArrayAttrib;
PFNGLVERTEXARRAYELEMENTBUFFERPROC                 glVertexArrayElementBuffer;
PFNGLVERTEXARRAYVERTEXBUFFERPROC                  glVertexArrayVertexBuffer;
PFNGLVERTEXARRAYVERTEXBUFFERSPROC                 glVertexArrayVertexBuffers;
PFNGLVERTEXARRAYATTRIBBINDINGPROC                 glVertexArrayAttribBinding;
PFNGLVERTEXARRAYATTRIBFORMATPROC                  glVertexArrayAttribFormat;
PFNGLVERTEXARRAYATTRIBIFORMATPROC                 glVertexArrayAttribIFormat;
PFNGLVERTEXARRAYATTRIBLFORMATPROC                 glVertexArrayAttribLFormat;
PFNGLVERTEXARRAYBINDINGDIVISORPROC                glVertexArrayBindingDivisor;
PFNGLGETVERTEXARRAYIVPROC                         glGetVertexArrayiv;
PFNGLGETVERTEXARRAYINDEXEDIVPROC                  glGetVertexArrayIndexediv;
PFNGLGETVERTEXARRAYINDEXED64IVPROC                glGetVertexArrayIndexed64iv;
PFNGLCREATESAMPLERSPROC                           glCreateSamplers;
PFNGLCREATEPROGRAMPIPELINESPROC                   glCreateProgramPipelines;
PFNGLCREATEQUERIESPROC                            glCreateQueries;
PFNGLGETQUERYBUFFEROBJECTI64VPROC                 glGetQueryBufferObjecti64v;
PFNGLGETQUERYBUFFEROBJECTIVPROC                   glGetQueryBufferObjectiv;
PFNGLGETQUERYBUFFEROBJECTUI64VPROC                glGetQueryBufferObjectui64v;
PFNGLGETQUERYBUFFEROBJECTUIVPROC                  glGetQueryBufferObjectuiv;
PFNGLMEMORYBARRIERBYREGIONPROC                    glMemoryBarrierByRegion;
PFNGLGETTEXTURESUBIMAGEPROC                       glGetTextureSubImage;
PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC             glGetCompressedTextureSubImage;
PFNGLGETGRAPHICSRESETSTATUSPROC                   glGetGraphicsResetStatus;
PFNGLGETNCOMPRESSEDTEXIMAGEPROC                   glGetnCompressedTexImage;
PFNGLGETNTEXIMAGEPROC                             glGetnTexImage;
PFNGLGETNUNIFORMDVPROC                            glGetnUniformdv;
PFNGLGETNUNIFORMFVPROC                            glGetnUniformfv;
PFNGLGETNUNIFORMIVPROC                            glGetnUniformiv;
PFNGLGETNUNIFORMUIVPROC                           glGetnUniformuiv;
PFNGLREADNPIXELSPROC                              glReadnPixels;
PFNGLTEXTUREBARRIERPROC                           glTextureBarrier;
/* ----GL VERSION 4.6------------------------------------------------------------------------------------------------ */
PFNGLSPECIALIZESHADERPROC                         glSpecializeShader;
PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC             glMultiDrawArraysIndirectCount;
PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC           glMultiDrawElementsIndirectCount;
PFNGLPOLYGONOFFSETCLAMPPROC                       glPolygonOffsetClamp;

#endif // !defined(__APPLE__)

static void vd_fw__load_opengl(VdFwGlVersion version)
{
#if defined(__APPLE__)
    // @todo(mdodis): This check
    // if (version > VD_FW_GL_VERION_4_1) {

    // }
#else
#define LOAD(p, s) s = (p)vd_fw__gl_get_proc_address(#s)

    if (version >= VD_FW_GL_VERSION_1_2) {
        LOAD(PFNGLDRAWRANGEELEMENTSPROC, glDrawRangeElements);
        LOAD(PFNGLTEXIMAGE3DPROC,        glTexImage3D);
        LOAD(PFNGLTEXSUBIMAGE3DPROC,     glTexSubImage3D);
        LOAD(PFNGLCOPYTEXSUBIMAGE3DPROC, glCopyTexSubImage3D);
    }

    if (version >= VD_FW_GL_VERSION_1_3) {
        LOAD(PFNGLACTIVETEXTUREPROC,           glActiveTexture);
        LOAD(PFNGLSAMPLECOVERAGEPROC,          glSampleCoverage);
        LOAD(PFNGLCOMPRESSEDTEXIMAGE3DPROC,    glCompressedTexImage3D);
        LOAD(PFNGLCOMPRESSEDTEXIMAGE2DPROC,    glCompressedTexImage2D);
        LOAD(PFNGLCOMPRESSEDTEXIMAGE1DPROC,    glCompressedTexImage1D);
        LOAD(PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC, glCompressedTexSubImage3D);
        LOAD(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC, glCompressedTexSubImage2D);
        LOAD(PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC, glCompressedTexSubImage1D);
        LOAD(PFNGLGETCOMPRESSEDTEXIMAGEPROC,   glGetCompressedTexImage);
    }

    if (version >= VD_FW_GL_VERSION_1_4) {
        LOAD(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate);
        LOAD(PFNGLMULTIDRAWARRAYSPROC,   glMultiDrawArrays);
        LOAD(PFNGLMULTIDRAWELEMENTSPROC, glMultiDrawElements);
        LOAD(PFNGLPOINTPARAMETERFPROC,   glPointParameterf);
        LOAD(PFNGLPOINTPARAMETERFVPROC,  glPointParameterfv);
        LOAD(PFNGLPOINTPARAMETERIPROC,   glPointParameteri);
        LOAD(PFNGLPOINTPARAMETERIVPROC,  glPointParameteriv);
        LOAD(PFNGLBLENDCOLORPROC,        glBlendColor);
        LOAD(PFNGLBLENDEQUATIONPROC,     glBlendEquation);
    }

    if (version >= VD_FW_GL_VERSION_1_5) {
        LOAD(PFNGLGENQUERIESPROC,           glGenQueries);
        LOAD(PFNGLDELETEQUERIESPROC,        glDeleteQueries);
        LOAD(PFNGLISQUERYPROC,              glIsQuery);
        LOAD(PFNGLBEGINQUERYPROC,           glBeginQuery);
        LOAD(PFNGLENDQUERYPROC,             glEndQuery);
        LOAD(PFNGLGETQUERYIVPROC,           glGetQueryiv);
        LOAD(PFNGLGETQUERYOBJECTIVPROC,     glGetQueryObjectiv);
        LOAD(PFNGLGETQUERYOBJECTUIVPROC,    glGetQueryObjectuiv);
        LOAD(PFNGLBINDBUFFERPROC,           glBindBuffer);
        LOAD(PFNGLDELETEBUFFERSPROC,        glDeleteBuffers);
        LOAD(PFNGLGENBUFFERSPROC,           glGenBuffers);
        LOAD(PFNGLISBUFFERPROC,             glIsBuffer);
        LOAD(PFNGLBUFFERDATAPROC,           glBufferData);
        LOAD(PFNGLBUFFERSUBDATAPROC,        glBufferSubData);
        LOAD(PFNGLGETBUFFERSUBDATAPROC,     glGetBufferSubData);
        LOAD(PFNGLMAPBUFFERPROC,            glMapBuffer);
        LOAD(PFNGLUNMAPBUFFERPROC,          glUnmapBuffer);
        LOAD(PFNGLGETBUFFERPARAMETERIVPROC, glGetBufferParameteriv);
        LOAD(PFNGLGETBUFFERPOINTERVPROC,    glGetBufferPointerv);
    }

    if (version >= VD_FW_GL_VERSION_2_0) {
        LOAD(PFNGLBLENDEQUATIONSEPARATEPROC,    glBlendEquationSeparate);
        LOAD(PFNGLDRAWBUFFERSPROC,              glDrawBuffers);
        LOAD(PFNGLSTENCILOPSEPARATEPROC,        glStencilOpSeparate);
        LOAD(PFNGLSTENCILFUNCSEPARATEPROC,      glStencilFuncSeparate);
        LOAD(PFNGLSTENCILMASKSEPARATEPROC,      glStencilMaskSeparate);
        LOAD(PFNGLATTACHSHADERPROC,             glAttachShader);
        LOAD(PFNGLBINDATTRIBLOCATIONPROC,       glBindAttribLocation);
        LOAD(PFNGLCOMPILESHADERPROC,            glCompileShader);
        LOAD(PFNGLCREATEPROGRAMPROC,            glCreateProgram);
        LOAD(PFNGLCREATESHADERPROC,             glCreateShader);
        LOAD(PFNGLDELETEPROGRAMPROC,            glDeleteProgram);
        LOAD(PFNGLDELETESHADERPROC,             glDeleteShader);
        LOAD(PFNGLDETACHSHADERPROC,             glDetachShader);
        LOAD(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
        LOAD(PFNGLENABLEVERTEXATTRIBARRAYPROC,  glEnableVertexAttribArray);
        LOAD(PFNGLGETACTIVEATTRIBPROC,          glGetActiveAttrib);
        LOAD(PFNGLGETACTIVEUNIFORMPROC,         glGetActiveUniform);
        LOAD(PFNGLGETATTACHEDSHADERSPROC,       glGetAttachedShaders);
        LOAD(PFNGLGETATTRIBLOCATIONPROC,        glGetAttribLocation);
        LOAD(PFNGLGETPROGRAMIVPROC,             glGetProgramiv);
        LOAD(PFNGLGETPROGRAMINFOLOGPROC,        glGetProgramInfoLog);
        LOAD(PFNGLGETSHADERIVPROC,              glGetShaderiv);
        LOAD(PFNGLGETSHADERINFOLOGPROC,         glGetShaderInfoLog);
        LOAD(PFNGLGETSHADERSOURCEPROC,          glGetShaderSource);
        LOAD(PFNGLGETUNIFORMLOCATIONPROC,       glGetUniformLocation);
        LOAD(PFNGLGETUNIFORMFVPROC,             glGetUniformfv);
        LOAD(PFNGLGETUNIFORMIVPROC,             glGetUniformiv);
        LOAD(PFNGLGETVERTEXATTRIBDVPROC,        glGetVertexAttribdv);
        LOAD(PFNGLGETVERTEXATTRIBFVPROC,        glGetVertexAttribfv);
        LOAD(PFNGLGETVERTEXATTRIBIVPROC,        glGetVertexAttribiv);
        LOAD(PFNGLGETVERTEXATTRIBPOINTERVPROC,  glGetVertexAttribPointerv);
        LOAD(PFNGLISPROGRAMPROC,                glIsProgram);
        LOAD(PFNGLISSHADERPROC,                 glIsShader);
        LOAD(PFNGLLINKPROGRAMPROC,              glLinkProgram);
        LOAD(PFNGLSHADERSOURCEPROC,             glShaderSource);
        LOAD(PFNGLUSEPROGRAMPROC,               glUseProgram);
        LOAD(PFNGLUNIFORM1FPROC,                glUniform1f);
        LOAD(PFNGLUNIFORM2FPROC,                glUniform2f);
        LOAD(PFNGLUNIFORM3FPROC,                glUniform3f);
        LOAD(PFNGLUNIFORM4FPROC,                glUniform4f);
        LOAD(PFNGLUNIFORM1IPROC,                glUniform1i);
        LOAD(PFNGLUNIFORM2IPROC,                glUniform2i);
        LOAD(PFNGLUNIFORM3IPROC,                glUniform3i);
        LOAD(PFNGLUNIFORM4IPROC,                glUniform4i);
        LOAD(PFNGLUNIFORM1FVPROC,               glUniform1fv);
        LOAD(PFNGLUNIFORM2FVPROC,               glUniform2fv);
        LOAD(PFNGLUNIFORM3FVPROC,               glUniform3fv);
        LOAD(PFNGLUNIFORM4FVPROC,               glUniform4fv);
        LOAD(PFNGLUNIFORM1IVPROC,               glUniform1iv);
        LOAD(PFNGLUNIFORM2IVPROC,               glUniform2iv);
        LOAD(PFNGLUNIFORM3IVPROC,               glUniform3iv);
        LOAD(PFNGLUNIFORM4IVPROC,               glUniform4iv);
        LOAD(PFNGLUNIFORMMATRIX2FVPROC,         glUniformMatrix2fv);
        LOAD(PFNGLUNIFORMMATRIX3FVPROC,         glUniformMatrix3fv);
        LOAD(PFNGLUNIFORMMATRIX4FVPROC,         glUniformMatrix4fv);
        LOAD(PFNGLVALIDATEPROGRAMPROC,          glValidateProgram);
        LOAD(PFNGLVERTEXATTRIB1DPROC,           glVertexAttrib1d);
        LOAD(PFNGLVERTEXATTRIB1DVPROC,          glVertexAttrib1dv);
        LOAD(PFNGLVERTEXATTRIB1FPROC,           glVertexAttrib1f);
        LOAD(PFNGLVERTEXATTRIB1FVPROC,          glVertexAttrib1fv);
        LOAD(PFNGLVERTEXATTRIB1SPROC,           glVertexAttrib1s);
        LOAD(PFNGLVERTEXATTRIB1SVPROC,          glVertexAttrib1sv);
        LOAD(PFNGLVERTEXATTRIB2DPROC,           glVertexAttrib2d);
        LOAD(PFNGLVERTEXATTRIB2DVPROC,          glVertexAttrib2dv);
        LOAD(PFNGLVERTEXATTRIB2FPROC,           glVertexAttrib2f);
        LOAD(PFNGLVERTEXATTRIB2FVPROC,          glVertexAttrib2fv);
        LOAD(PFNGLVERTEXATTRIB2SPROC,           glVertexAttrib2s);
        LOAD(PFNGLVERTEXATTRIB2SVPROC,          glVertexAttrib2sv);
        LOAD(PFNGLVERTEXATTRIB3DPROC,           glVertexAttrib3d);
        LOAD(PFNGLVERTEXATTRIB3DVPROC,          glVertexAttrib3dv);
        LOAD(PFNGLVERTEXATTRIB3FPROC,           glVertexAttrib3f);
        LOAD(PFNGLVERTEXATTRIB3FVPROC,          glVertexAttrib3fv);
        LOAD(PFNGLVERTEXATTRIB3SPROC,           glVertexAttrib3s);
        LOAD(PFNGLVERTEXATTRIB3SVPROC,          glVertexAttrib3sv);
        LOAD(PFNGLVERTEXATTRIB4NBVPROC,         glVertexAttrib4Nbv);
        LOAD(PFNGLVERTEXATTRIB4NIVPROC,         glVertexAttrib4Niv);
        LOAD(PFNGLVERTEXATTRIB4NSVPROC,         glVertexAttrib4Nsv);
        LOAD(PFNGLVERTEXATTRIB4NUBPROC,         glVertexAttrib4Nub);
        LOAD(PFNGLVERTEXATTRIB4NUBVPROC,        glVertexAttrib4Nubv);
        LOAD(PFNGLVERTEXATTRIB4NUIVPROC,        glVertexAttrib4Nuiv);
        LOAD(PFNGLVERTEXATTRIB4NUSVPROC,        glVertexAttrib4Nusv);
        LOAD(PFNGLVERTEXATTRIB4BVPROC,          glVertexAttrib4bv);
        LOAD(PFNGLVERTEXATTRIB4DPROC,           glVertexAttrib4d);
        LOAD(PFNGLVERTEXATTRIB4DVPROC,          glVertexAttrib4dv);
        LOAD(PFNGLVERTEXATTRIB4FPROC,           glVertexAttrib4f);
        LOAD(PFNGLVERTEXATTRIB4FVPROC,          glVertexAttrib4fv);
        LOAD(PFNGLVERTEXATTRIB4IVPROC,          glVertexAttrib4iv);
        LOAD(PFNGLVERTEXATTRIB4SPROC,           glVertexAttrib4s);
        LOAD(PFNGLVERTEXATTRIB4SVPROC,          glVertexAttrib4sv);
        LOAD(PFNGLVERTEXATTRIB4UBVPROC,         glVertexAttrib4ubv);
        LOAD(PFNGLVERTEXATTRIB4UIVPROC,         glVertexAttrib4uiv);
        LOAD(PFNGLVERTEXATTRIB4USVPROC,         glVertexAttrib4usv);
        LOAD(PFNGLVERTEXATTRIBPOINTERPROC,      glVertexAttribPointer);
    }

    if (version >= VD_FW_GL_VERSION_2_1) {
        LOAD(PFNGLUNIFORMMATRIX2X3FVPROC, glUniformMatrix2x3fv);
        LOAD(PFNGLUNIFORMMATRIX3X2FVPROC, glUniformMatrix3x2fv);
        LOAD(PFNGLUNIFORMMATRIX2X4FVPROC, glUniformMatrix2x4fv);
        LOAD(PFNGLUNIFORMMATRIX4X2FVPROC, glUniformMatrix4x2fv);
        LOAD(PFNGLUNIFORMMATRIX3X4FVPROC, glUniformMatrix3x4fv);
        LOAD(PFNGLUNIFORMMATRIX4X3FVPROC, glUniformMatrix4x3fv);
    }

    if (version >= VD_FW_GL_VERSION_3_0) {
        LOAD(PFNGLCOLORMASKIPROC,                          glColorMaski);
        LOAD(PFNGLGETBOOLEANI_VPROC,                       glGetBooleani_v);
        LOAD(PFNGLGETINTEGERI_VPROC,                       glGetIntegeri_v);
        LOAD(PFNGLENABLEIPROC,                             glEnablei);
        LOAD(PFNGLDISABLEIPROC,                            glDisablei);
        LOAD(PFNGLISENABLEDIPROC,                          glIsEnabledi);
        LOAD(PFNGLBEGINTRANSFORMFEEDBACKPROC,              glBeginTransformFeedback);
        LOAD(PFNGLENDTRANSFORMFEEDBACKPROC,                glEndTransformFeedback);
        LOAD(PFNGLBINDBUFFERRANGEPROC,                     glBindBufferRange);
        LOAD(PFNGLBINDBUFFERBASEPROC,                      glBindBufferBase);
        LOAD(PFNGLTRANSFORMFEEDBACKVARYINGSPROC,           glTransformFeedbackVaryings);
        LOAD(PFNGLGETTRANSFORMFEEDBACKVARYINGPROC,         glGetTransformFeedbackVarying);
        LOAD(PFNGLCLAMPCOLORPROC,                          glClampColor);
        LOAD(PFNGLBEGINCONDITIONALRENDERPROC,              glBeginConditionalRender);
        LOAD(PFNGLENDCONDITIONALRENDERPROC,                glEndConditionalRender);
        LOAD(PFNGLVERTEXATTRIBIPOINTERPROC,                glVertexAttribIPointer);
        LOAD(PFNGLGETVERTEXATTRIBIIVPROC,                  glGetVertexAttribIiv);
        LOAD(PFNGLGETVERTEXATTRIBIUIVPROC,                 glGetVertexAttribIuiv);
        LOAD(PFNGLVERTEXATTRIBI1IPROC,                     glVertexAttribI1i);
        LOAD(PFNGLVERTEXATTRIBI2IPROC,                     glVertexAttribI2i);
        LOAD(PFNGLVERTEXATTRIBI3IPROC,                     glVertexAttribI3i);
        LOAD(PFNGLVERTEXATTRIBI4IPROC,                     glVertexAttribI4i);
        LOAD(PFNGLVERTEXATTRIBI1UIPROC,                    glVertexAttribI1ui);
        LOAD(PFNGLVERTEXATTRIBI2UIPROC,                    glVertexAttribI2ui);
        LOAD(PFNGLVERTEXATTRIBI3UIPROC,                    glVertexAttribI3ui);
        LOAD(PFNGLVERTEXATTRIBI4UIPROC,                    glVertexAttribI4ui);
        LOAD(PFNGLVERTEXATTRIBI1IVPROC,                    glVertexAttribI1iv);
        LOAD(PFNGLVERTEXATTRIBI2IVPROC,                    glVertexAttribI2iv);
        LOAD(PFNGLVERTEXATTRIBI3IVPROC,                    glVertexAttribI3iv);
        LOAD(PFNGLVERTEXATTRIBI4IVPROC,                    glVertexAttribI4iv);
        LOAD(PFNGLVERTEXATTRIBI1UIVPROC,                   glVertexAttribI1uiv);
        LOAD(PFNGLVERTEXATTRIBI2UIVPROC,                   glVertexAttribI2uiv);
        LOAD(PFNGLVERTEXATTRIBI3UIVPROC,                   glVertexAttribI3uiv);
        LOAD(PFNGLVERTEXATTRIBI4UIVPROC,                   glVertexAttribI4uiv);
        LOAD(PFNGLVERTEXATTRIBI4BVPROC,                    glVertexAttribI4bv);
        LOAD(PFNGLVERTEXATTRIBI4SVPROC,                    glVertexAttribI4sv);
        LOAD(PFNGLVERTEXATTRIBI4UBVPROC,                   glVertexAttribI4ubv);
        LOAD(PFNGLVERTEXATTRIBI4USVPROC,                   glVertexAttribI4usv);
        LOAD(PFNGLGETUNIFORMUIVPROC,                       glGetUniformuiv);
        LOAD(PFNGLBINDFRAGDATALOCATIONPROC,                glBindFragDataLocation);
        LOAD(PFNGLGETFRAGDATALOCATIONPROC,                 glGetFragDataLocation);
        LOAD(PFNGLUNIFORM1UIPROC,                          glUniform1ui);
        LOAD(PFNGLUNIFORM2UIPROC,                          glUniform2ui);
        LOAD(PFNGLUNIFORM3UIPROC,                          glUniform3ui);
        LOAD(PFNGLUNIFORM4UIPROC,                          glUniform4ui);
        LOAD(PFNGLUNIFORM1UIVPROC,                         glUniform1uiv);
        LOAD(PFNGLUNIFORM2UIVPROC,                         glUniform2uiv);
        LOAD(PFNGLUNIFORM3UIVPROC,                         glUniform3uiv);
        LOAD(PFNGLUNIFORM4UIVPROC,                         glUniform4uiv);
        LOAD(PFNGLTEXPARAMETERIIVPROC,                     glTexParameterIiv);
        LOAD(PFNGLTEXPARAMETERIUIVPROC,                    glTexParameterIuiv);
        LOAD(PFNGLGETTEXPARAMETERIIVPROC,                  glGetTexParameterIiv);
        LOAD(PFNGLGETTEXPARAMETERIUIVPROC,                 glGetTexParameterIuiv);
        LOAD(PFNGLCLEARBUFFERIVPROC,                       glClearBufferiv);
        LOAD(PFNGLCLEARBUFFERUIVPROC,                      glClearBufferuiv);
        LOAD(PFNGLCLEARBUFFERFVPROC,                       glClearBufferfv);
        LOAD(PFNGLCLEARBUFFERFIPROC,                       glClearBufferfi);
        LOAD(PFNGLGETSTRINGIPROC,                          glGetStringi);
        LOAD(PFNGLISRENDERBUFFERPROC,                      glIsRenderbuffer);
        LOAD(PFNGLBINDRENDERBUFFERPROC,                    glBindRenderbuffer);
        LOAD(PFNGLDELETERENDERBUFFERSPROC,                 glDeleteRenderbuffers);
        LOAD(PFNGLGENRENDERBUFFERSPROC,                    glGenRenderbuffers);
        LOAD(PFNGLRENDERBUFFERSTORAGEPROC,                 glRenderbufferStorage);
        LOAD(PFNGLGETRENDERBUFFERPARAMETERIVPROC,          glGetRenderbufferParameteriv);
        LOAD(PFNGLISFRAMEBUFFERPROC,                       glIsFramebuffer);
        LOAD(PFNGLBINDFRAMEBUFFERPROC,                     glBindFramebuffer);
        LOAD(PFNGLDELETEFRAMEBUFFERSPROC,                  glDeleteFramebuffers);
        LOAD(PFNGLGENFRAMEBUFFERSPROC,                     glGenFramebuffers);
        LOAD(PFNGLCHECKFRAMEBUFFERSTATUSPROC,              glCheckFramebufferStatus);
        LOAD(PFNGLFRAMEBUFFERTEXTURE1DPROC,                glFramebufferTexture1D);
        LOAD(PFNGLFRAMEBUFFERTEXTURE2DPROC,                glFramebufferTexture2D);
        LOAD(PFNGLFRAMEBUFFERTEXTURE3DPROC,                glFramebufferTexture3D);
        LOAD(PFNGLFRAMEBUFFERRENDERBUFFERPROC,             glFramebufferRenderbuffer);
        LOAD(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC, glGetFramebufferAttachmentParameteriv);
        LOAD(PFNGLGENERATEMIPMAPPROC,                      glGenerateMipmap);
        LOAD(PFNGLBLITFRAMEBUFFERPROC,                     glBlitFramebuffer);
        LOAD(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC,      glRenderbufferStorageMultisample);
        LOAD(PFNGLFRAMEBUFFERTEXTURELAYERPROC,             glFramebufferTextureLayer);
        LOAD(PFNGLMAPBUFFERRANGEPROC,                      glMapBufferRange);
        LOAD(PFNGLFLUSHMAPPEDBUFFERRANGEPROC,              glFlushMappedBufferRange);
        LOAD(PFNGLBINDVERTEXARRAYPROC,                     glBindVertexArray);
        LOAD(PFNGLDELETEVERTEXARRAYSPROC,                  glDeleteVertexArrays);
        LOAD(PFNGLGENVERTEXARRAYSPROC,                     glGenVertexArrays);
        LOAD(PFNGLISVERTEXARRAYPROC,                       glIsVertexArray);
    }

    if (version >= VD_FW_GL_VERSION_3_1) {
        LOAD(PFNGLDRAWARRAYSINSTANCEDPROC,       glDrawArraysInstanced);
        LOAD(PFNGLDRAWELEMENTSINSTANCEDPROC,     glDrawElementsInstanced);
        LOAD(PFNGLTEXBUFFERPROC,                 glTexBuffer);
        LOAD(PFNGLPRIMITIVERESTARTINDEXPROC,     glPrimitiveRestartIndex);
        LOAD(PFNGLCOPYBUFFERSUBDATAPROC,         glCopyBufferSubData);
        LOAD(PFNGLGETUNIFORMINDICESPROC,         glGetUniformIndices);
        LOAD(PFNGLGETACTIVEUNIFORMSIVPROC,       glGetActiveUniformsiv);
        LOAD(PFNGLGETACTIVEUNIFORMNAMEPROC,      glGetActiveUniformName);
        LOAD(PFNGLGETUNIFORMBLOCKINDEXPROC,      glGetUniformBlockIndex);
        LOAD(PFNGLGETACTIVEUNIFORMBLOCKIVPROC,   glGetActiveUniformBlockiv);
        LOAD(PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC, glGetActiveUniformBlockName);
        LOAD(PFNGLUNIFORMBLOCKBINDINGPROC,       glUniformBlockBinding);
    }

    if (version >= VD_FW_GL_VERSION_3_2) {
        LOAD(PFNGLDRAWELEMENTSBASEVERTEXPROC,          glDrawElementsBaseVertex);
        LOAD(PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC,     glDrawRangeElementsBaseVertex);
        LOAD(PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC, glDrawElementsInstancedBaseVertex);
        LOAD(PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC,     glMultiDrawElementsBaseVertex);
        LOAD(PFNGLPROVOKINGVERTEXPROC,                 glProvokingVertex);
        LOAD(PFNGLFENCESYNCPROC,                       glFenceSync);
        LOAD(PFNGLISSYNCPROC,                          glIsSync);
        LOAD(PFNGLDELETESYNCPROC,                      glDeleteSync);
        LOAD(PFNGLCLIENTWAITSYNCPROC,                  glClientWaitSync);
        LOAD(PFNGLWAITSYNCPROC,                        glWaitSync);
        LOAD(PFNGLGETINTEGER64VPROC,                   glGetInteger64v);
        LOAD(PFNGLGETSYNCIVPROC,                       glGetSynciv);
        LOAD(PFNGLGETINTEGER64I_VPROC,                 glGetInteger64i_v);
        LOAD(PFNGLGETBUFFERPARAMETERI64VPROC,          glGetBufferParameteri64v);
        LOAD(PFNGLFRAMEBUFFERTEXTUREPROC,              glFramebufferTexture);
        LOAD(PFNGLTEXIMAGE2DMULTISAMPLEPROC,           glTexImage2DMultisample);
        LOAD(PFNGLTEXIMAGE3DMULTISAMPLEPROC,           glTexImage3DMultisample);
        LOAD(PFNGLGETMULTISAMPLEFVPROC,                glGetMultisamplefv);
        LOAD(PFNGLSAMPLEMASKIPROC,                     glSampleMaski);
    }

    if (version >= VD_FW_GL_VERSION_3_3) {
        LOAD(PFNGLBINDFRAGDATALOCATIONINDEXEDPROC, glBindFragDataLocationIndexed);
        LOAD(PFNGLGETFRAGDATAINDEXPROC,            glGetFragDataIndex);
        LOAD(PFNGLGENSAMPLERSPROC,                 glGenSamplers);
        LOAD(PFNGLDELETESAMPLERSPROC,              glDeleteSamplers);
        LOAD(PFNGLISSAMPLERPROC,                   glIsSampler);
        LOAD(PFNGLBINDSAMPLERPROC,                 glBindSampler);
        LOAD(PFNGLSAMPLERPARAMETERIPROC,           glSamplerParameteri);
        LOAD(PFNGLSAMPLERPARAMETERIVPROC,          glSamplerParameteriv);
        LOAD(PFNGLSAMPLERPARAMETERFPROC,           glSamplerParameterf);
        LOAD(PFNGLSAMPLERPARAMETERFVPROC,          glSamplerParameterfv);
        LOAD(PFNGLSAMPLERPARAMETERIIVPROC,         glSamplerParameterIiv);
        LOAD(PFNGLSAMPLERPARAMETERIUIVPROC,        glSamplerParameterIuiv);
        LOAD(PFNGLGETSAMPLERPARAMETERIVPROC,       glGetSamplerParameteriv);
        LOAD(PFNGLGETSAMPLERPARAMETERIIVPROC,      glGetSamplerParameterIiv);
        LOAD(PFNGLGETSAMPLERPARAMETERFVPROC,       glGetSamplerParameterfv);
        LOAD(PFNGLGETSAMPLERPARAMETERIUIVPROC,     glGetSamplerParameterIuiv);
        LOAD(PFNGLQUERYCOUNTERPROC,                glQueryCounter);
        LOAD(PFNGLGETQUERYOBJECTI64VPROC,          glGetQueryObjecti64v);
        LOAD(PFNGLGETQUERYOBJECTUI64VPROC,         glGetQueryObjectui64v);
        LOAD(PFNGLVERTEXATTRIBDIVISORPROC,         glVertexAttribDivisor);
        LOAD(PFNGLVERTEXATTRIBP1UIPROC,            glVertexAttribP1ui);
        LOAD(PFNGLVERTEXATTRIBP1UIVPROC,           glVertexAttribP1uiv);
        LOAD(PFNGLVERTEXATTRIBP2UIPROC,            glVertexAttribP2ui);
        LOAD(PFNGLVERTEXATTRIBP2UIVPROC,           glVertexAttribP2uiv);
        LOAD(PFNGLVERTEXATTRIBP3UIPROC,            glVertexAttribP3ui);
        LOAD(PFNGLVERTEXATTRIBP3UIVPROC,           glVertexAttribP3uiv);
        LOAD(PFNGLVERTEXATTRIBP4UIPROC,            glVertexAttribP4ui);
        LOAD(PFNGLVERTEXATTRIBP4UIVPROC,           glVertexAttribP4uiv);
    }

    if (version >= VD_FW_GL_VERSION_4_0) {
        LOAD(PFNGLMINSAMPLESHADINGPROC,               glMinSampleShading);
        LOAD(PFNGLBLENDEQUATIONIPROC,                 glBlendEquationi);
        LOAD(PFNGLBLENDEQUATIONSEPARATEIPROC,         glBlendEquationSeparatei);
        LOAD(PFNGLBLENDFUNCIPROC,                     glBlendFunci);
        LOAD(PFNGLBLENDFUNCSEPARATEIPROC,             glBlendFuncSeparatei);
        LOAD(PFNGLDRAWARRAYSINDIRECTPROC,             glDrawArraysIndirect);
        LOAD(PFNGLDRAWELEMENTSINDIRECTPROC,           glDrawElementsIndirect);
        LOAD(PFNGLUNIFORM1DPROC,                      glUniform1d);
        LOAD(PFNGLUNIFORM2DPROC,                      glUniform2d);
        LOAD(PFNGLUNIFORM3DPROC,                      glUniform3d);
        LOAD(PFNGLUNIFORM4DPROC,                      glUniform4d);
        LOAD(PFNGLUNIFORM1DVPROC,                     glUniform1dv);
        LOAD(PFNGLUNIFORM2DVPROC,                     glUniform2dv);
        LOAD(PFNGLUNIFORM3DVPROC,                     glUniform3dv);
        LOAD(PFNGLUNIFORM4DVPROC,                     glUniform4dv);
        LOAD(PFNGLUNIFORMMATRIX2DVPROC,               glUniformMatrix2dv);
        LOAD(PFNGLUNIFORMMATRIX3DVPROC,               glUniformMatrix3dv);
        LOAD(PFNGLUNIFORMMATRIX4DVPROC,               glUniformMatrix4dv);
        LOAD(PFNGLUNIFORMMATRIX2X3DVPROC,             glUniformMatrix2x3dv);
        LOAD(PFNGLUNIFORMMATRIX2X4DVPROC,             glUniformMatrix2x4dv);
        LOAD(PFNGLUNIFORMMATRIX3X2DVPROC,             glUniformMatrix3x2dv);
        LOAD(PFNGLUNIFORMMATRIX3X4DVPROC,             glUniformMatrix3x4dv);
        LOAD(PFNGLUNIFORMMATRIX4X2DVPROC,             glUniformMatrix4x2dv);
        LOAD(PFNGLUNIFORMMATRIX4X3DVPROC,             glUniformMatrix4x3dv);
        LOAD(PFNGLGETUNIFORMDVPROC,                   glGetUniformdv);
        LOAD(PFNGLGETSUBROUTINEUNIFORMLOCATIONPROC,   glGetSubroutineUniformLocation);
        LOAD(PFNGLGETSUBROUTINEINDEXPROC,             glGetSubroutineIndex);
        LOAD(PFNGLGETACTIVESUBROUTINEUNIFORMIVPROC,   glGetActiveSubroutineUniformiv);
        LOAD(PFNGLGETACTIVESUBROUTINEUNIFORMNAMEPROC, glGetActiveSubroutineUniformName);
        LOAD(PFNGLGETACTIVESUBROUTINENAMEPROC,        glGetActiveSubroutineName);
        LOAD(PFNGLUNIFORMSUBROUTINESUIVPROC,          glUniformSubroutinesuiv);
        LOAD(PFNGLGETUNIFORMSUBROUTINEUIVPROC,        glGetUniformSubroutineuiv);
        LOAD(PFNGLGETPROGRAMSTAGEIVPROC,              glGetProgramStageiv);
        LOAD(PFNGLPATCHPARAMETERIPROC,                glPatchParameteri);
        LOAD(PFNGLPATCHPARAMETERFVPROC,               glPatchParameterfv);
        LOAD(PFNGLBINDTRANSFORMFEEDBACKPROC,          glBindTransformFeedback);
        LOAD(PFNGLDELETETRANSFORMFEEDBACKSPROC,       glDeleteTransformFeedbacks);
        LOAD(PFNGLGENTRANSFORMFEEDBACKSPROC,          glGenTransformFeedbacks);
        LOAD(PFNGLISTRANSFORMFEEDBACKPROC,            glIsTransformFeedback);
        LOAD(PFNGLPAUSETRANSFORMFEEDBACKPROC,         glPauseTransformFeedback);
        LOAD(PFNGLRESUMETRANSFORMFEEDBACKPROC,        glResumeTransformFeedback);
        LOAD(PFNGLDRAWTRANSFORMFEEDBACKPROC,          glDrawTransformFeedback);
        LOAD(PFNGLDRAWTRANSFORMFEEDBACKSTREAMPROC,    glDrawTransformFeedbackStream);
        LOAD(PFNGLBEGINQUERYINDEXEDPROC,              glBeginQueryIndexed);
        LOAD(PFNGLENDQUERYINDEXEDPROC,                glEndQueryIndexed);
        LOAD(PFNGLGETQUERYINDEXEDIVPROC,              glGetQueryIndexediv);
    }

    if (version >= VD_FW_GL_VERSION_4_1) {
        LOAD(PFNGLRELEASESHADERCOMPILERPROC,     glReleaseShaderCompiler);
        LOAD(PFNGLSHADERBINARYPROC,              glShaderBinary);
        LOAD(PFNGLGETSHADERPRECISIONFORMATPROC,  glGetShaderPrecisionFormat);
        LOAD(PFNGLDEPTHRANGEFPROC,               glDepthRangef);
        LOAD(PFNGLCLEARDEPTHFPROC,               glClearDepthf);
        LOAD(PFNGLGETPROGRAMBINARYPROC,          glGetProgramBinary);
        LOAD(PFNGLPROGRAMBINARYPROC,             glProgramBinary);
        LOAD(PFNGLPROGRAMPARAMETERIPROC,         glProgramParameteri);
        LOAD(PFNGLUSEPROGRAMSTAGESPROC,          glUseProgramStages);
        LOAD(PFNGLACTIVESHADERPROGRAMPROC,       glActiveShaderProgram);
        LOAD(PFNGLCREATESHADERPROGRAMVPROC,      glCreateShaderProgramv);
        LOAD(PFNGLBINDPROGRAMPIPELINEPROC,       glBindProgramPipeline);
        LOAD(PFNGLDELETEPROGRAMPIPELINESPROC,    glDeleteProgramPipelines);
        LOAD(PFNGLGENPROGRAMPIPELINESPROC,       glGenProgramPipelines);
        LOAD(PFNGLISPROGRAMPIPELINEPROC,         glIsProgramPipeline);
        LOAD(PFNGLGETPROGRAMPIPELINEIVPROC,      glGetProgramPipelineiv);
        LOAD(PFNGLPROGRAMUNIFORM1IPROC,          glProgramUniform1i);
        LOAD(PFNGLPROGRAMUNIFORM1IVPROC,         glProgramUniform1iv);
        LOAD(PFNGLPROGRAMUNIFORM1FPROC,          glProgramUniform1f);
        LOAD(PFNGLPROGRAMUNIFORM1FVPROC,         glProgramUniform1fv);
        LOAD(PFNGLPROGRAMUNIFORM1DPROC,          glProgramUniform1d);
        LOAD(PFNGLPROGRAMUNIFORM1DVPROC,         glProgramUniform1dv);
        LOAD(PFNGLPROGRAMUNIFORM1UIPROC,         glProgramUniform1ui);
        LOAD(PFNGLPROGRAMUNIFORM1UIVPROC,        glProgramUniform1uiv);
        LOAD(PFNGLPROGRAMUNIFORM2IPROC,          glProgramUniform2i);
        LOAD(PFNGLPROGRAMUNIFORM2IVPROC,         glProgramUniform2iv);
        LOAD(PFNGLPROGRAMUNIFORM2FPROC,          glProgramUniform2f);
        LOAD(PFNGLPROGRAMUNIFORM2FVPROC,         glProgramUniform2fv);
        LOAD(PFNGLPROGRAMUNIFORM2DPROC,          glProgramUniform2d);
        LOAD(PFNGLPROGRAMUNIFORM2DVPROC,         glProgramUniform2dv);
        LOAD(PFNGLPROGRAMUNIFORM2UIPROC,         glProgramUniform2ui);
        LOAD(PFNGLPROGRAMUNIFORM2UIVPROC,        glProgramUniform2uiv);
        LOAD(PFNGLPROGRAMUNIFORM3IPROC,          glProgramUniform3i);
        LOAD(PFNGLPROGRAMUNIFORM3IVPROC,         glProgramUniform3iv);
        LOAD(PFNGLPROGRAMUNIFORM3FPROC,          glProgramUniform3f);
        LOAD(PFNGLPROGRAMUNIFORM3FVPROC,         glProgramUniform3fv);
        LOAD(PFNGLPROGRAMUNIFORM3DPROC,          glProgramUniform3d);
        LOAD(PFNGLPROGRAMUNIFORM3DVPROC,         glProgramUniform3dv);
        LOAD(PFNGLPROGRAMUNIFORM3UIPROC,         glProgramUniform3ui);
        LOAD(PFNGLPROGRAMUNIFORM3UIVPROC,        glProgramUniform3uiv);
        LOAD(PFNGLPROGRAMUNIFORM4IPROC,          glProgramUniform4i);
        LOAD(PFNGLPROGRAMUNIFORM4IVPROC,         glProgramUniform4iv);
        LOAD(PFNGLPROGRAMUNIFORM4FPROC,          glProgramUniform4f);
        LOAD(PFNGLPROGRAMUNIFORM4FVPROC,         glProgramUniform4fv);
        LOAD(PFNGLPROGRAMUNIFORM4DPROC,          glProgramUniform4d);
        LOAD(PFNGLPROGRAMUNIFORM4DVPROC,         glProgramUniform4dv);
        LOAD(PFNGLPROGRAMUNIFORM4UIPROC,         glProgramUniform4ui);
        LOAD(PFNGLPROGRAMUNIFORM4UIVPROC,        glProgramUniform4uiv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX2FVPROC,   glProgramUniformMatrix2fv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX3FVPROC,   glProgramUniformMatrix3fv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX4FVPROC,   glProgramUniformMatrix4fv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX2DVPROC,   glProgramUniformMatrix2dv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX3DVPROC,   glProgramUniformMatrix3dv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX4DVPROC,   glProgramUniformMatrix4dv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC, glProgramUniformMatrix2x3fv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC, glProgramUniformMatrix3x2fv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC, glProgramUniformMatrix2x4fv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC, glProgramUniformMatrix4x2fv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC, glProgramUniformMatrix3x4fv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC, glProgramUniformMatrix4x3fv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC, glProgramUniformMatrix2x3dv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC, glProgramUniformMatrix3x2dv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC, glProgramUniformMatrix2x4dv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC, glProgramUniformMatrix4x2dv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC, glProgramUniformMatrix3x4dv);
        LOAD(PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC, glProgramUniformMatrix4x3dv);
        LOAD(PFNGLVALIDATEPROGRAMPIPELINEPROC,   glValidateProgramPipeline);
        LOAD(PFNGLGETPROGRAMPIPELINEINFOLOGPROC, glGetProgramPipelineInfoLog);
        LOAD(PFNGLVERTEXATTRIBL1DPROC,           glVertexAttribL1d);
        LOAD(PFNGLVERTEXATTRIBL2DPROC,           glVertexAttribL2d);
        LOAD(PFNGLVERTEXATTRIBL3DPROC,           glVertexAttribL3d);
        LOAD(PFNGLVERTEXATTRIBL4DPROC,           glVertexAttribL4d);
        LOAD(PFNGLVERTEXATTRIBL1DVPROC,          glVertexAttribL1dv);
        LOAD(PFNGLVERTEXATTRIBL2DVPROC,          glVertexAttribL2dv);
        LOAD(PFNGLVERTEXATTRIBL3DVPROC,          glVertexAttribL3dv);
        LOAD(PFNGLVERTEXATTRIBL4DVPROC,          glVertexAttribL4dv);
        LOAD(PFNGLVERTEXATTRIBLPOINTERPROC,      glVertexAttribLPointer);
        LOAD(PFNGLGETVERTEXATTRIBLDVPROC,        glGetVertexAttribLdv);
        LOAD(PFNGLVIEWPORTARRAYVPROC,            glViewportArrayv);
        LOAD(PFNGLVIEWPORTINDEXEDFPROC,          glViewportIndexedf);
        LOAD(PFNGLVIEWPORTINDEXEDFVPROC,         glViewportIndexedfv);
        LOAD(PFNGLSCISSORARRAYVPROC,             glScissorArrayv);
        LOAD(PFNGLSCISSORINDEXEDPROC,            glScissorIndexed);
        LOAD(PFNGLSCISSORINDEXEDVPROC,           glScissorIndexedv);
        LOAD(PFNGLDEPTHRANGEARRAYVPROC,          glDepthRangeArrayv);
        LOAD(PFNGLDEPTHRANGEINDEXEDPROC,         glDepthRangeIndexed);
        LOAD(PFNGLGETFLOATI_VPROC,               glGetFloati_v);
        LOAD(PFNGLGETDOUBLEI_VPROC,              glGetDoublei_v);
    }

    if (version >= VD_FW_GL_VERSION_4_2) {
        LOAD(PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC,             glDrawArraysInstancedBaseInstance);
        LOAD(PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC,           glDrawElementsInstancedBaseInstance);
        LOAD(PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC, glDrawElementsInstancedBaseVertexBaseInstance);
        LOAD(PFNGLGETINTERNALFORMATIVPROC,                         glGetInternalformativ);
        LOAD(PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC,              glGetActiveAtomicCounterBufferiv);
        LOAD(PFNGLBINDIMAGETEXTUREPROC,                            glBindImageTexture);
        LOAD(PFNGLMEMORYBARRIERPROC,                               glMemoryBarrier);
        LOAD(PFNGLTEXSTORAGE1DPROC,                                glTexStorage1D);
        LOAD(PFNGLTEXSTORAGE2DPROC,                                glTexStorage2D);
        LOAD(PFNGLTEXSTORAGE3DPROC,                                glTexStorage3D);
        LOAD(PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC,              glDrawTransformFeedbackInstanced);
        LOAD(PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC,        glDrawTransformFeedbackStreamInstanced);
    }

    if (version >= VD_FW_GL_VERSION_4_3) {
        LOAD(PFNGLCLEARBUFFERDATAPROC,                 glClearBufferData);
        LOAD(PFNGLCLEARBUFFERSUBDATAPROC,              glClearBufferSubData);
        LOAD(PFNGLDISPATCHCOMPUTEPROC,                 glDispatchCompute);
        LOAD(PFNGLDISPATCHCOMPUTEINDIRECTPROC,         glDispatchComputeIndirect);
        LOAD(PFNGLCOPYIMAGESUBDATAPROC,                glCopyImageSubData);
        LOAD(PFNGLFRAMEBUFFERPARAMETERIPROC,           glFramebufferParameteri);
        LOAD(PFNGLGETFRAMEBUFFERPARAMETERIVPROC,       glGetFramebufferParameteriv);
        LOAD(PFNGLGETINTERNALFORMATI64VPROC,           glGetInternalformati64v);
        LOAD(PFNGLINVALIDATETEXSUBIMAGEPROC,           glInvalidateTexSubImage);
        LOAD(PFNGLINVALIDATETEXIMAGEPROC,              glInvalidateTexImage);
        LOAD(PFNGLINVALIDATEBUFFERSUBDATAPROC,         glInvalidateBufferSubData);
        LOAD(PFNGLINVALIDATEBUFFERDATAPROC,            glInvalidateBufferData);
        LOAD(PFNGLINVALIDATEFRAMEBUFFERPROC,           glInvalidateFramebuffer);
        LOAD(PFNGLINVALIDATESUBFRAMEBUFFERPROC,        glInvalidateSubFramebuffer);
        LOAD(PFNGLMULTIDRAWARRAYSINDIRECTPROC,         glMultiDrawArraysIndirect);
        LOAD(PFNGLMULTIDRAWELEMENTSINDIRECTPROC,       glMultiDrawElementsIndirect);
        LOAD(PFNGLGETPROGRAMINTERFACEIVPROC,           glGetProgramInterfaceiv);
        LOAD(PFNGLGETPROGRAMRESOURCEINDEXPROC,         glGetProgramResourceIndex);
        LOAD(PFNGLGETPROGRAMRESOURCENAMEPROC,          glGetProgramResourceName);
        LOAD(PFNGLGETPROGRAMRESOURCEIVPROC,            glGetProgramResourceiv);
        LOAD(PFNGLGETPROGRAMRESOURCELOCATIONPROC,      glGetProgramResourceLocation);
        LOAD(PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC, glGetProgramResourceLocationIndex);
        LOAD(PFNGLSHADERSTORAGEBLOCKBINDINGPROC,       glShaderStorageBlockBinding);
        LOAD(PFNGLTEXBUFFERRANGEPROC,                  glTexBufferRange);
        LOAD(PFNGLTEXSTORAGE2DMULTISAMPLEPROC,         glTexStorage2DMultisample);
        LOAD(PFNGLTEXSTORAGE3DMULTISAMPLEPROC,         glTexStorage3DMultisample);
        LOAD(PFNGLTEXTUREVIEWPROC,                     glTextureView);
        LOAD(PFNGLBINDVERTEXBUFFERPROC,                glBindVertexBuffer);
        LOAD(PFNGLVERTEXATTRIBFORMATPROC,              glVertexAttribFormat);
        LOAD(PFNGLVERTEXATTRIBIFORMATPROC,             glVertexAttribIFormat);
        LOAD(PFNGLVERTEXATTRIBLFORMATPROC,             glVertexAttribLFormat);
        LOAD(PFNGLVERTEXATTRIBBINDINGPROC,             glVertexAttribBinding);
        LOAD(PFNGLVERTEXBINDINGDIVISORPROC,            glVertexBindingDivisor);
        LOAD(PFNGLDEBUGMESSAGECONTROLPROC,             glDebugMessageControl);
        LOAD(PFNGLDEBUGMESSAGEINSERTPROC,              glDebugMessageInsert);
        LOAD(PFNGLDEBUGMESSAGECALLBACKPROC,            glDebugMessageCallback);
        LOAD(PFNGLGETDEBUGMESSAGELOGPROC,              glGetDebugMessageLog);
        LOAD(PFNGLPUSHDEBUGGROUPPROC,                  glPushDebugGroup);
        LOAD(PFNGLPOPDEBUGGROUPPROC,                   glPopDebugGroup);
        LOAD(PFNGLOBJECTLABELPROC,                     glObjectLabel);
        LOAD(PFNGLGETOBJECTLABELPROC,                  glGetObjectLabel);
        LOAD(PFNGLOBJECTPTRLABELPROC,                  glObjectPtrLabel);
        LOAD(PFNGLGETOBJECTPTRLABELPROC,               glGetObjectPtrLabel);
    }

    if (version >= VD_FW_GL_VERSION_4_4) {
        LOAD(PFNGLBUFFERSTORAGEPROC,     glBufferStorage);
        LOAD(PFNGLCLEARTEXIMAGEPROC,     glClearTexImage);
        LOAD(PFNGLCLEARTEXSUBIMAGEPROC,  glClearTexSubImage);
        LOAD(PFNGLBINDBUFFERSBASEPROC,   glBindBuffersBase);
        LOAD(PFNGLBINDBUFFERSRANGEPROC,  glBindBuffersRange);
        LOAD(PFNGLBINDTEXTURESPROC,      glBindTextures);
        LOAD(PFNGLBINDSAMPLERSPROC,      glBindSamplers);
        LOAD(PFNGLBINDIMAGETEXTURESPROC, glBindImageTextures);
        LOAD(PFNGLBINDVERTEXBUFFERSPROC, glBindVertexBuffers);
    }

    if (version >= VD_FW_GL_VERSION_4_5) {
        LOAD(PFNGLCLIPCONTROLPROC,                              glClipControl);
        LOAD(PFNGLCREATETRANSFORMFEEDBACKSPROC,                 glCreateTransformFeedbacks);
        LOAD(PFNGLTRANSFORMFEEDBACKBUFFERBASEPROC,              glTransformFeedbackBufferBase);
        LOAD(PFNGLTRANSFORMFEEDBACKBUFFERRANGEPROC,             glTransformFeedbackBufferRange);
        LOAD(PFNGLGETTRANSFORMFEEDBACKIVPROC,                   glGetTransformFeedbackiv);
        LOAD(PFNGLGETTRANSFORMFEEDBACKI_VPROC,                  glGetTransformFeedbacki_v);
        LOAD(PFNGLGETTRANSFORMFEEDBACKI64_VPROC,                glGetTransformFeedbacki64_v);
        LOAD(PFNGLCREATEBUFFERSPROC,                            glCreateBuffers);
        LOAD(PFNGLNAMEDBUFFERSTORAGEPROC,                       glNamedBufferStorage);
        LOAD(PFNGLNAMEDBUFFERDATAPROC,                          glNamedBufferData);
        LOAD(PFNGLNAMEDBUFFERSUBDATAPROC,                       glNamedBufferSubData);
        LOAD(PFNGLCOPYNAMEDBUFFERSUBDATAPROC,                   glCopyNamedBufferSubData);
        LOAD(PFNGLCLEARNAMEDBUFFERDATAPROC,                     glClearNamedBufferData);
        LOAD(PFNGLCLEARNAMEDBUFFERSUBDATAPROC,                  glClearNamedBufferSubData);
        LOAD(PFNGLMAPNAMEDBUFFERPROC,                           glMapNamedBuffer);
        LOAD(PFNGLMAPNAMEDBUFFERRANGEPROC,                      glMapNamedBufferRange);
        LOAD(PFNGLUNMAPNAMEDBUFFERPROC,                         glUnmapNamedBuffer);
        LOAD(PFNGLFLUSHMAPPEDNAMEDBUFFERRANGEPROC,              glFlushMappedNamedBufferRange);
        LOAD(PFNGLGETNAMEDBUFFERPARAMETERIVPROC,                glGetNamedBufferParameteriv);
        LOAD(PFNGLGETNAMEDBUFFERPARAMETERI64VPROC,              glGetNamedBufferParameteri64v);
        LOAD(PFNGLGETNAMEDBUFFERPOINTERVPROC,                   glGetNamedBufferPointerv);
        LOAD(PFNGLGETNAMEDBUFFERSUBDATAPROC,                    glGetNamedBufferSubData);
        LOAD(PFNGLCREATEFRAMEBUFFERSPROC,                       glCreateFramebuffers);
        LOAD(PFNGLNAMEDFRAMEBUFFERRENDERBUFFERPROC,             glNamedFramebufferRenderbuffer);
        LOAD(PFNGLNAMEDFRAMEBUFFERPARAMETERIPROC,               glNamedFramebufferParameteri);
        LOAD(PFNGLNAMEDFRAMEBUFFERTEXTUREPROC,                  glNamedFramebufferTexture);
        LOAD(PFNGLNAMEDFRAMEBUFFERTEXTURELAYERPROC,             glNamedFramebufferTextureLayer);
        LOAD(PFNGLNAMEDFRAMEBUFFERDRAWBUFFERPROC,               glNamedFramebufferDrawBuffer);
        LOAD(PFNGLNAMEDFRAMEBUFFERDRAWBUFFERSPROC,              glNamedFramebufferDrawBuffers);
        LOAD(PFNGLNAMEDFRAMEBUFFERREADBUFFERPROC,               glNamedFramebufferReadBuffer);
        LOAD(PFNGLINVALIDATENAMEDFRAMEBUFFERDATAPROC,           glInvalidateNamedFramebufferData);
        LOAD(PFNGLINVALIDATENAMEDFRAMEBUFFERSUBDATAPROC,        glInvalidateNamedFramebufferSubData);
        LOAD(PFNGLCLEARNAMEDFRAMEBUFFERIVPROC,                  glClearNamedFramebufferiv);
        LOAD(PFNGLCLEARNAMEDFRAMEBUFFERUIVPROC,                 glClearNamedFramebufferuiv);
        LOAD(PFNGLCLEARNAMEDFRAMEBUFFERFVPROC,                  glClearNamedFramebufferfv);
        LOAD(PFNGLCLEARNAMEDFRAMEBUFFERFIPROC,                  glClearNamedFramebufferfi);
        LOAD(PFNGLBLITNAMEDFRAMEBUFFERPROC,                     glBlitNamedFramebuffer);
        LOAD(PFNGLCHECKNAMEDFRAMEBUFFERSTATUSPROC,              glCheckNamedFramebufferStatus);
        LOAD(PFNGLGETNAMEDFRAMEBUFFERPARAMETERIVPROC,           glGetNamedFramebufferParameteriv);
        LOAD(PFNGLGETNAMEDFRAMEBUFFERATTACHMENTPARAMETERIVPROC, glGetNamedFramebufferAttachmentParameteriv);
        LOAD(PFNGLCREATERENDERBUFFERSPROC,                      glCreateRenderbuffers);
        LOAD(PFNGLNAMEDRENDERBUFFERSTORAGEPROC,                 glNamedRenderbufferStorage);
        LOAD(PFNGLNAMEDRENDERBUFFERSTORAGEMULTISAMPLEPROC,      glNamedRenderbufferStorageMultisample);
        LOAD(PFNGLGETNAMEDRENDERBUFFERPARAMETERIVPROC,          glGetNamedRenderbufferParameteriv);
        LOAD(PFNGLCREATETEXTURESPROC,                           glCreateTextures);
        LOAD(PFNGLTEXTUREBUFFERPROC,                            glTextureBuffer);
        LOAD(PFNGLTEXTUREBUFFERRANGEPROC,                       glTextureBufferRange);
        LOAD(PFNGLTEXTURESTORAGE1DPROC,                         glTextureStorage1D);
        LOAD(PFNGLTEXTURESTORAGE2DPROC,                         glTextureStorage2D);
        LOAD(PFNGLTEXTURESTORAGE3DPROC,                         glTextureStorage3D);
        LOAD(PFNGLTEXTURESTORAGE2DMULTISAMPLEPROC,              glTextureStorage2DMultisample);
        LOAD(PFNGLTEXTURESTORAGE3DMULTISAMPLEPROC,              glTextureStorage3DMultisample);
        LOAD(PFNGLTEXTURESUBIMAGE1DPROC,                        glTextureSubImage1D);
        LOAD(PFNGLTEXTURESUBIMAGE2DPROC,                        glTextureSubImage2D);
        LOAD(PFNGLTEXTURESUBIMAGE3DPROC,                        glTextureSubImage3D);
        LOAD(PFNGLCOMPRESSEDTEXTURESUBIMAGE1DPROC,              glCompressedTextureSubImage1D);
        LOAD(PFNGLCOMPRESSEDTEXTURESUBIMAGE2DPROC,              glCompressedTextureSubImage2D);
        LOAD(PFNGLCOMPRESSEDTEXTURESUBIMAGE3DPROC,              glCompressedTextureSubImage3D);
        LOAD(PFNGLCOPYTEXTURESUBIMAGE1DPROC,                    glCopyTextureSubImage1D);
        LOAD(PFNGLCOPYTEXTURESUBIMAGE2DPROC,                    glCopyTextureSubImage2D);
        LOAD(PFNGLCOPYTEXTURESUBIMAGE3DPROC,                    glCopyTextureSubImage3D);
        LOAD(PFNGLTEXTUREPARAMETERFPROC,                        glTextureParameterf);
        LOAD(PFNGLTEXTUREPARAMETERFVPROC,                       glTextureParameterfv);
        LOAD(PFNGLTEXTUREPARAMETERIPROC,                        glTextureParameteri);
        LOAD(PFNGLTEXTUREPARAMETERIIVPROC,                      glTextureParameterIiv);
        LOAD(PFNGLTEXTUREPARAMETERIUIVPROC,                     glTextureParameterIuiv);
        LOAD(PFNGLTEXTUREPARAMETERIVPROC,                       glTextureParameteriv);
        LOAD(PFNGLGENERATETEXTUREMIPMAPPROC,                    glGenerateTextureMipmap);
        LOAD(PFNGLBINDTEXTUREUNITPROC,                          glBindTextureUnit);
        LOAD(PFNGLGETTEXTUREIMAGEPROC,                          glGetTextureImage);
        LOAD(PFNGLGETCOMPRESSEDTEXTUREIMAGEPROC,                glGetCompressedTextureImage);
        LOAD(PFNGLGETTEXTURELEVELPARAMETERFVPROC,               glGetTextureLevelParameterfv);
        LOAD(PFNGLGETTEXTURELEVELPARAMETERIVPROC,               glGetTextureLevelParameteriv);
        LOAD(PFNGLGETTEXTUREPARAMETERFVPROC,                    glGetTextureParameterfv);
        LOAD(PFNGLGETTEXTUREPARAMETERIIVPROC,                   glGetTextureParameterIiv);
        LOAD(PFNGLGETTEXTUREPARAMETERIUIVPROC,                  glGetTextureParameterIuiv);
        LOAD(PFNGLGETTEXTUREPARAMETERIVPROC,                    glGetTextureParameteriv);
        LOAD(PFNGLCREATEVERTEXARRAYSPROC,                       glCreateVertexArrays);
        LOAD(PFNGLDISABLEVERTEXARRAYATTRIBPROC,                 glDisableVertexArrayAttrib);
        LOAD(PFNGLENABLEVERTEXARRAYATTRIBPROC,                  glEnableVertexArrayAttrib);
        LOAD(PFNGLVERTEXARRAYELEMENTBUFFERPROC,                 glVertexArrayElementBuffer);
        LOAD(PFNGLVERTEXARRAYVERTEXBUFFERPROC,                  glVertexArrayVertexBuffer);
        LOAD(PFNGLVERTEXARRAYVERTEXBUFFERSPROC,                 glVertexArrayVertexBuffers);
        LOAD(PFNGLVERTEXARRAYATTRIBBINDINGPROC,                 glVertexArrayAttribBinding);
        LOAD(PFNGLVERTEXARRAYATTRIBFORMATPROC,                  glVertexArrayAttribFormat);
        LOAD(PFNGLVERTEXARRAYATTRIBIFORMATPROC,                 glVertexArrayAttribIFormat);
        LOAD(PFNGLVERTEXARRAYATTRIBLFORMATPROC,                 glVertexArrayAttribLFormat);
        LOAD(PFNGLVERTEXARRAYBINDINGDIVISORPROC,                glVertexArrayBindingDivisor);
        LOAD(PFNGLGETVERTEXARRAYIVPROC,                         glGetVertexArrayiv);
        LOAD(PFNGLGETVERTEXARRAYINDEXEDIVPROC,                  glGetVertexArrayIndexediv);
        LOAD(PFNGLGETVERTEXARRAYINDEXED64IVPROC,                glGetVertexArrayIndexed64iv);
        LOAD(PFNGLCREATESAMPLERSPROC,                           glCreateSamplers);
        LOAD(PFNGLCREATEPROGRAMPIPELINESPROC,                   glCreateProgramPipelines);
        LOAD(PFNGLCREATEQUERIESPROC,                            glCreateQueries);
        LOAD(PFNGLGETQUERYBUFFEROBJECTI64VPROC,                 glGetQueryBufferObjecti64v);
        LOAD(PFNGLGETQUERYBUFFEROBJECTIVPROC,                   glGetQueryBufferObjectiv);
        LOAD(PFNGLGETQUERYBUFFEROBJECTUI64VPROC,                glGetQueryBufferObjectui64v);
        LOAD(PFNGLGETQUERYBUFFEROBJECTUIVPROC,                  glGetQueryBufferObjectuiv);
        LOAD(PFNGLMEMORYBARRIERBYREGIONPROC,                    glMemoryBarrierByRegion);
        LOAD(PFNGLGETTEXTURESUBIMAGEPROC,                       glGetTextureSubImage);
        LOAD(PFNGLGETCOMPRESSEDTEXTURESUBIMAGEPROC,             glGetCompressedTextureSubImage);
        LOAD(PFNGLGETGRAPHICSRESETSTATUSPROC,                   glGetGraphicsResetStatus);
        LOAD(PFNGLGETNCOMPRESSEDTEXIMAGEPROC,                   glGetnCompressedTexImage);
        LOAD(PFNGLGETNTEXIMAGEPROC,                             glGetnTexImage);
        LOAD(PFNGLGETNUNIFORMDVPROC,                            glGetnUniformdv);
        LOAD(PFNGLGETNUNIFORMFVPROC,                            glGetnUniformfv);
        LOAD(PFNGLGETNUNIFORMIVPROC,                            glGetnUniformiv);
        LOAD(PFNGLGETNUNIFORMUIVPROC,                           glGetnUniformuiv);
        LOAD(PFNGLREADNPIXELSPROC,                              glReadnPixels);
        LOAD(PFNGLTEXTUREBARRIERPROC,                           glTextureBarrier);
    }

    if (version >= VD_FW_GL_VERSION_4_6) {
        LOAD(PFNGLSPECIALIZESHADERPROC,                         glSpecializeShader);
        LOAD(PFNGLMULTIDRAWARRAYSINDIRECTCOUNTPROC,             glMultiDrawArraysIndirectCount);
        LOAD(PFNGLMULTIDRAWELEMENTSINDIRECTCOUNTPROC,           glMultiDrawElementsIndirectCount);
        LOAD(PFNGLPOLYGONOFFSETCLAMPPROC,                       glPolygonOffsetClamp);
    }

#undef LOAD
#endif 
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

VD_FW_INL const char *vd_fw_get_key_name(VdFwKey k)
{
    static const char *translation_table[VD_FW_KEY_MAX] = {
        "Unknown",       // VD_FW_KEY_UNKNOWN
        "F1",            // VD_FW_KEY_F1
        "F2",            // VD_FW_KEY_F2
        "F3",            // VD_FW_KEY_F3
        "F4",            // VD_FW_KEY_F4
        "F5",            // VD_FW_KEY_F5
        "F6",            // VD_FW_KEY_F6
        "F7",            // VD_FW_KEY_F7
        "F8",            // VD_FW_KEY_F8
        "F9",            // VD_FW_KEY_F9
        "F10",           // VD_FW_KEY_F10
        "F11",           // VD_FW_KEY_F11
        "F12",           // VD_FW_KEY_F12
        "F13",           // VD_FW_KEY_F13
        "F14",           // VD_FW_KEY_F14
        "F15",           // VD_FW_KEY_F15
        "F16",           // VD_FW_KEY_F16
        "F17",           // VD_FW_KEY_F17
        "F18",           // VD_FW_KEY_F18
        "F19",           // VD_FW_KEY_F19
        "F20",           // VD_FW_KEY_F20
        "F21",           // VD_FW_KEY_F21
        "F22",           // VD_FW_KEY_F22
        "F23",           // VD_FW_KEY_F23
        "F24",           // VD_FW_KEY_F24
        "Backspace",     // VD_FW_KEY_BACKSPACE
        "Ins",           // VD_FW_KEY_INS
        "Home",          // VD_FW_KEY_HOME
        "Pgup",          // VD_FW_KEY_PGUP
        "Del",           // VD_FW_KEY_DEL
        "End",           // VD_FW_KEY_END
        "Pgdn",          // VD_FW_KEY_PGDN
        "Space",         // VD_FW_KEY_SPACE
        "Lcontrol",      // VD_FW_KEY_LCONTROL
        "Rcontrol",      // VD_FW_KEY_RCONTROL
        "Lalt",          // VD_FW_KEY_LALT
        "Ralt",          // VD_FW_KEY_RALT
        "Lshift",        // VD_FW_KEY_LSHIFT
        "Rshift",        // VD_FW_KEY_RSHIFT
        "Quote",         // VD_FW_KEY_QUOTE
        "ArrowUp",       // VD_FW_KEY_ARROW_UP
        "ArrowLeft",     // VD_FW_KEY_ARROW_LEFT
        "ArrowDown",     // VD_FW_KEY_ARROW_DOWN
        "ArrowRight",    // VD_FW_KEY_ARROW_RIGHT
        "Comma",         // VD_FW_KEY_COMMA
        "Minus",         // VD_FW_KEY_MINUS
        "Dot",           // VD_FW_KEY_DOT
        "SlashForward",  // VD_FW_KEY_SLASH_FORWARD
        "0",             // VD_FW_KEY_0
        "1",             // VD_FW_KEY_1
        "2",             // VD_FW_KEY_2
        "3",             // VD_FW_KEY_3
        "4",             // VD_FW_KEY_4
        "5",             // VD_FW_KEY_5
        "6",             // VD_FW_KEY_6
        "7",             // VD_FW_KEY_7
        "8",             // VD_FW_KEY_8
        "9",             // VD_FW_KEY_9
        "Enter",         // VD_FW_KEY_ENTER
        "Semicolon",     // VD_FW_KEY_SEMICOLON
        "Tab",           // VD_FW_KEY_TAB
        "Equals",        // VD_FW_KEY_EQUALS
        "Capital",       // VD_FW_KEY_CAPITAL
        "Escape",        // VD_FW_KEY_ESCAPE
        "Reserved1",     // VD_FW_KEY_RESERVED1
        "A",             // VD_FW_KEY_A
        "B",             // VD_FW_KEY_B
        "C",             // VD_FW_KEY_C
        "D",             // VD_FW_KEY_D
        "E",             // VD_FW_KEY_E
        "F",             // VD_FW_KEY_F
        "G",             // VD_FW_KEY_G
        "H",             // VD_FW_KEY_H
        "I",             // VD_FW_KEY_I
        "J",             // VD_FW_KEY_J
        "K",             // VD_FW_KEY_K
        "L",             // VD_FW_KEY_L
        "M",             // VD_FW_KEY_M
        "N",             // VD_FW_KEY_N
        "O",             // VD_FW_KEY_O
        "P",             // VD_FW_KEY_P
        "Q",             // VD_FW_KEY_Q
        "R",             // VD_FW_KEY_R
        "S",             // VD_FW_KEY_S
        "T",             // VD_FW_KEY_T
        "U",             // VD_FW_KEY_U
        "V",             // VD_FW_KEY_V
        "W",             // VD_FW_KEY_W
        "X",             // VD_FW_KEY_X
        "Y",             // VD_FW_KEY_Y
        "Z",             // VD_FW_KEY_Z
        "BracketOpen",   // VD_FW_KEY_BRACKET_OPEN
        "SlashBack",     // VD_FW_KEY_SLASH_BACK
        "BracketClose",  // VD_FW_KEY_BRACKET_CLOSE
        "MediaNext",     // VD_FW_KEY_MEDIA_NEXT
        "MediaPrev",     // VD_FW_KEY_MEDIA_PREV
        "Backtick",      // VD_FW_KEY_BACKTICK
        "MediaPlay",     // VD_FW_KEY_MEDIA_PLAY
        "Numpad0",       // VD_FW_KEY_NUMPAD_0
        "Numpad1",       // VD_FW_KEY_NUMPAD_1
        "Numpad2",       // VD_FW_KEY_NUMPAD_2
        "Numpad3",       // VD_FW_KEY_NUMPAD_3
        "Numpad4",       // VD_FW_KEY_NUMPAD_4
        "Numpad5",       // VD_FW_KEY_NUMPAD_5
        "Numpad6",       // VD_FW_KEY_NUMPAD_6
        "Numpad7",       // VD_FW_KEY_NUMPAD_7
        "Numpad8",       // VD_FW_KEY_NUMPAD_8
        "Numpad9",       // VD_FW_KEY_NUMPAD_9
    };

    return translation_table[k];
}



#if defined(__APPLE__)
#pragma clang diagnostic pop
#endif
#endif // VD_FW_IMPL