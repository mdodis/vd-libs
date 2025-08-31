/**
 * vd_fw.h - Gets you a window with OpenGL running on platforms that support it
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
#ifndef VD_FW_H
#define VD_FW_H
typedef void         GLvoid;
typedef unsigned int GLenum;

extern void glClear(int mask);
extern void glClearColor(float r, float g, float b, float a);

extern int vd_fw_init(void);
extern int vd_fw_running(void);
extern int vd_fw_poll_events(void);
extern int vd_fw_swap_buffers(void);
#endif // !VD_FW_H

#ifdef VD_FW_IMPL
#ifdef _WIN32
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "shell32.lib")
// #define WIN32_LEAN_AND_MEAN
// #define NOGDICAPMASKS
// #define NOSYSMETRICS
// #define NOMENUS
// #define NOICONS
// #define NOSYSCOMMANDS
// #define NORASTEROPS
// #define OEMRESOURCE
// #define NOATOM
// #define NOCLIPBOARD
// #define NOCOLOR
// #define NOCTLMGR
// #define NODRAWTEXT
// #define NOKERNEL
// #define NONLS
// #define NOMEMMGR
// #define NOMETAFILE
// #define NOOPENFILE
// #define NOSCROLL
// #define NOSERVICE
// #define NOSOUND
// #define NOTEXTMETRIC
// #define NOWH
// #define NOCOMM
// #define NOKANJI
// #define NOHELP
// #define NOPROFILER
// #define NODEFERWINDOWPOS
// #define NOMCX
// #define NORPC
// #define NOPROXYSTUB
// #define NOIMAGE
#define NOTAPE
#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <uxtheme.h>
#include <dwmapi.h>
#include <shellscalingapi.h>
#include <versionhelpers.h>
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

static HWND    VdFw__Hwnd;
static HDC     VdFw__Hdc;
static int     VdFw__WindowState = 0;
static HGLRC   VdFw__Hglrc;
static RECT    VdFw__Rgn;
static POINT   VdFw__DragStart;
static RECT    VdFw__DragStartRect;
static int     VdFw__ResizeHit;
static BOOL    VdFw__Theme_Enabled;
static BOOL    Vdfw__Composition_Enabled;
static LRESULT vd_fw__wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
static void    vd_fw__composition_changed(void);
static void    vd_fw__update_region(void);
static void    vd_fw__theme_changed(void);
static void    vd_fw__nccalcsize(WPARAM wparam, LPARAM lparam);
static BOOL    vd_fw__has_autohide_taskbar(UINT edge, RECT monitor);
static void    vd_fw__window_pos_changed(WINDOWPOS *pos);
static LRESULT vd_fw__handle_invisible(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

int vd_fw_init(void)
{
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    VD_FW_SANITY_CHECK();

    WNDCLASSEXW wcx;
    ZeroMemory(&wcx, sizeof(wcx));
    wcx.cbSize         = sizeof(wcx);
    wcx.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcx.hInstance      = NULL;
    wcx.lpfnWndProc    = vd_fw__wndproc;
    wcx.lpszClassName  = L"FWCLASS";
    wcx.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcx.hCursor        = LoadCursorA(NULL, IDC_ARROW);
    if (!RegisterClassExW(&wcx)) {
        return 0;
    }

    VdFw__Hwnd = CreateWindowExW(
        WS_EX_APPWINDOW | WS_EX_LAYERED,
        L"FWCLASS",
        L"FW Window",
        WS_OVERLAPPEDWINDOW | WS_SIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1600,
        900,
        0 /* hwndParent */,
        0 /* hMenu */,
        0 /* hInstance */,
        0 /* lpParam */);

    SetLayeredWindowAttributes(VdFw__Hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);

    vd_fw__composition_changed();
    VD_FW_SANITY_CHECK();
    vd_fw__theme_changed();
    VD_FW_SANITY_CHECK();

    // SetWindowPos(VdFw__Hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_SHOWWINDOW);
    ShowWindow(VdFw__Hwnd, SW_SHOW);
    VD_FW__CHECK_NONZERO(UpdateWindow(VdFw__Hwnd));

    VdFw__Hdc = GetDC(VdFw__Hwnd);

    PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,                                // Version Number
      PFD_DRAW_TO_WINDOW |              // Format Must Support Window
      PFD_SUPPORT_OPENGL |              // Format Must Support OpenGL
      // PFD_SUPPORT_COMPOSITION |         // Format Must Support Composition
      PFD_DOUBLEBUFFER,                 // Must Support Double Buffering
      PFD_TYPE_RGBA,                    // Request An RGBA Format
      32,                               // Select Our Color Depth
      0, 0, 0, 0, 0, 0,                 // Color Bits Ignored
      8,                                // An Alpha Buffer
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
    int pf = ChoosePixelFormat(VdFw__Hdc, &pfd);
    if (SetPixelFormat(VdFw__Hdc, pf, &pfd) != TRUE) {
        return 0;
    }

    VdFw__Hglrc = wglCreateContext(VdFw__Hdc);
    if (VdFw__Hglrc == NULL) {
        printf("A\n");
    }
    VD_FW__CHECK_NULL(VdFw__Hglrc);
    VD_FW__CHECK_TRUE(wglMakeCurrent(VdFw__Hdc, VdFw__Hglrc));

    return 1;
}

int vd_fw_running(void)
{
    return !(VdFw__Hwnd == 0);
}

int vd_fw_poll_events(void)
{
    MSG message;
    while (PeekMessageW(&message, VdFw__Hwnd, 0, 0, PM_REMOVE)) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return 1;
}

int vd_fw_swap_buffers(void)
{
    SwapBuffers(VdFw__Hdc);
    return 1;
}

static int vd_fw__hit_test(int x, int y)
{
    if (IsMaximized(VdFw__Hwnd)) {
        return HTCLIENT;
    }

    POINT mouse;
    mouse.x = x;
    mouse.y = y;
    ScreenToClient(VdFw__Hwnd, &mouse);

    RECT client;
    GetClientRect(VdFw__Hwnd, &client);
    int width  = client.right - client.left;
    int height = client.bottom - client.top;

    /* The horizontal frame should be the same size as the vertical frame,
       since the NONCLIENTMETRICS structure does not distinguish between them */
    int frame_size = GetSystemMetrics(SM_CXFRAME) +
                     GetSystemMetrics(SM_CXPADDEDBORDER);
    /* The diagonal size handles are wider than the frame */
    int diagonal_width = frame_size * 2 + GetSystemMetrics(SM_CXBORDER);

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
    return HTCLIENT;
}

static void vd_fw__composition_changed(void)
{
    BOOL enabled = FALSE;
    VD_FW__CHECK_HRESULT(DwmIsCompositionEnabled(&enabled));
    Vdfw__Composition_Enabled = enabled;

    printf("Composition Enabled: %d\n", enabled);

    if (enabled) {
        // MARGINS m = {0, 0, 1, 0};
        MARGINS m = {-1};
        VD_FW__CHECK_HRESULT(DwmExtendFrameIntoClientArea(VdFw__Hwnd, &m));
        DWORD value = DWMNCRP_ENABLED;
        VD_FW__CHECK_HRESULT(DwmSetWindowAttribute(VdFw__Hwnd, DWMWA_NCRENDERING_POLICY, &value, sizeof(value)));
    }

    vd_fw__update_region();
}

static void vd_fw__update_region(void)
{
    RECT old_rgn = VdFw__Rgn;

    if (IsMaximized(VdFw__Hwnd)) {
        WINDOWINFO window_info = {};
        window_info.cbSize = sizeof(window_info);
        GetWindowInfo(VdFw__Hwnd, &window_info);

        VdFw__Rgn = (RECT) {
            .left   = window_info.rcClient.left   - window_info.rcWindow.left,
            .top    = window_info.rcClient.top    - window_info.rcWindow.top,
            .right  = window_info.rcClient.right  - window_info.rcWindow.left,
            .bottom = window_info.rcClient.bottom - window_info.rcWindow.top,
        };
    } else if (!Vdfw__Composition_Enabled) {
        VdFw__Rgn = (RECT) {
            .left   = 0,
            .top    = 0,
            .right  = 32767,
            .bottom = 32767,
        };
    } else {
        VdFw__Rgn = (RECT) {
            .left   = 0,
            .top    = 0,
            .right  = 0,
            .bottom = 0,
        };
    }

    if (EqualRect(&VdFw__Rgn, &old_rgn)) {
        return;
    }

    RECT zero_rect = {0};
    if (EqualRect(&VdFw__Rgn, &zero_rect)) {
        VD_FW__CHECK_INT(SetWindowRgn(VdFw__Hwnd, NULL, TRUE));
    } else {
        VD_FW__CHECK_INT(SetWindowRgn(VdFw__Hwnd, CreateRectRgnIndirect(&VdFw__Rgn), TRUE));
    }
}

static void vd_fw__theme_changed(void)
{
    VdFw__Theme_Enabled = IsThemeActive();
}

static void vd_fw__nccalcsize(WPARAM wparam, LPARAM lparam)
{
    if (wparam == TRUE) {
        return;
    }

    union {
        LPARAM lparam;
        RECT   *rect;
    } params = { .lparam = lparam };

    RECT non_client = *params.rect;
    DefWindowProcW(VdFw__Hwnd, WM_NCCALCSIZE, wparam, params.lparam);

    RECT client = *params.rect;

    if (IsMaximized(VdFw__Hwnd)) {
        WINDOWINFO window_info = {0};
        window_info.cbSize = sizeof(window_info);
        GetWindowInfo(VdFw__Hwnd, &window_info);

        *params.rect = (RECT) {
            .left   = client.left,
            .top    = non_client.top + window_info.cyWindowBorders,
            .right  = client.right,
            .bottom = client.bottom,
        };

        HMONITOR monitor = MonitorFromWindow(VdFw__Hwnd, MONITOR_DEFAULTTOPRIMARY);
        MONITORINFO monitor_info = {0};
        monitor_info.cbSize = sizeof(monitor_info);
        GetMonitorInfoW(monitor, &monitor_info);

        if (EqualRect(params.rect, &monitor_info.rcMonitor)) {
            if (vd_fw__has_autohide_taskbar(ABE_BOTTOM, monitor_info.rcMonitor)) {
                params.rect->bottom--;
            } else if (vd_fw__has_autohide_taskbar(ABE_LEFT, monitor_info.rcMonitor)) {
                params.rect->left++;
            } else if (vd_fw__has_autohide_taskbar(ABE_TOP, monitor_info.rcMonitor)) {
                params.rect->top++;
            } else if (vd_fw__has_autohide_taskbar(ABE_RIGHT, monitor_info.rcMonitor)) {
                params.rect->right--;
            }
        } else {
            *params.rect = non_client;
        }
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
    RECT client;
    GetClientRect(VdFw__Hwnd, &client);
    if (pos->flags & SWP_FRAMECHANGED) {
        vd_fw__update_region();
    }
    VD_FW__CHECK_NONZERO(InvalidateRect(VdFw__Hwnd, &client, TRUE));
}

static LRESULT vd_fw__handle_invisible(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LONG_PTR old_style = GetWindowLongPtr(hwnd, GWL_STYLE);

    SetWindowLongPtrW(hwnd, GWL_STYLE, old_style & ~WS_VISIBLE);    
    LRESULT result = DefWindowProcW(hwnd, msg, wparam, lparam);
    SetWindowLongPtrW(hwnd, GWL_STYLE, old_style);    

    return result;
}

static void vd_fw__paint(void)
{
    render();
    // PAINTSTRUCT ps;
    // HDC dc = BeginPaint(VdFw__Hwnd, &ps);
    // HBRUSH bb = CreateSolidBrush(RGB(0, 255, 0));

    // RECT rect;
    // GetClientRect(VdFw__Hwnd, &rect);
    // int width = rect.right - rect.left;
    // int height = rect.bottom - rect.top;

    // /* Draw a rectangle on the border of the client area for testing */
    // FillRect(dc, &(RECT) { 0, 0, 1, height }, bb);
    // FillRect(dc, &(RECT) { 0, 0, width, 1 }, bb);
    // FillRect(dc, &(RECT) { width - 1, 0, width, height }, bb);
    // FillRect(dc, &(RECT) { 0, height - 1, width, height }, bb);

    // DeleteObject(bb);
    // EndPaint(VdFw__Hwnd, &ps);
}

static LRESULT vd_fw__wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static UINT timer_id = 0;

    LRESULT result = 0;
    switch (msg) {

        case WM_CLOSE: {
            DestroyWindow(hwnd);
        } break;

        case WM_DESTROY: {
            PostQuitMessage(0);
            VdFw__Hwnd = (HWND)0;
        } break;

        case WM_DWMCOMPOSITIONCHANGED: {
            vd_fw__composition_changed();
        } break;

        // @todo(mdodis): WM_KEYDOWN

        case WM_ERASEBKGND: {
            result = 1;
        } break;

        case WM_LBUTTONDOWN: {
            ReleaseCapture();
            SendMessageW(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        } break;

        case WM_NCACTIVATE: {
            result = DefWindowProc(hwnd, msg, wparam, -1);
        } break;

        case WM_NCCALCSIZE: {
            vd_fw__nccalcsize(wparam, lparam);
        } break;

        case WM_NCHITTEST: {
            result = vd_fw__hit_test(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
        } break;

        case WM_NCPAINT: {
            if (Vdfw__Composition_Enabled) {
                result = DefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_NCUAHDRAWCAPTION:
        case WM_NCUAHDRAWFRAME: {
            result = 0;
        } break;

        case WM_PAINT: {
            render();
            vd_fw_swap_buffers();
            DwmFlush();
        } break;

        case WM_SETICON:
        case WM_SETTEXT: {
            if (!Vdfw__Composition_Enabled && !VdFw__Theme_Enabled) {
                result = vd_fw__handle_invisible(hwnd, msg, wparam, lparam);
            } else {
                result = DefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_THEMECHANGED: {
            vd_fw__theme_changed();
            result = DefWindowProc(hwnd, msg, wparam, lparam);
        } break;

        case WM_WINDOWPOSCHANGED: {
            vd_fw__window_pos_changed((WINDOWPOS*)lparam);
        } break;

        case WM_ENTERSIZEMOVE: {
            VD_FW__CHECK_NULL(SetTimer(VdFw__Hwnd, (UINT_PTR)&timer_id, USER_TIMER_MINIMUM, NULL));

        } break;

        case WM_EXITSIZEMOVE: {
            KillTimer(VdFw__Hwnd, (UINT_PTR)&timer_id);
        } break;

        case WM_TIMER: {
            render();
            vd_fw_swap_buffers();
            DwmFlush();
            result = 1;
        } break;

        case WM_MOVING:
        case WM_SIZING: {
            // DwmFlush();
        } break;

        // case WM_SYSCOMMAND: {
        //     if ((wparam & 0xfff0) == SC_KEYMENU)
        //         return 0;
        // } break;


        // case WM_MOUSEMOVE: {
        //     if ((VdFw__WindowState == VD_FW__MOVING) || (VdFw__WindowState == VD_FW__SIZING)) {
        //         POINT cursor;
        //         GetCursorPos(&cursor);

        //         int dx = cursor.x - VdFw__DragStart.x;
        //         int dy = cursor.y - VdFw__DragStart.y;

        //         RECT new_rect = VdFw__DragStartRect;

        //         if (VdFw__WindowState == VD_FW__MOVING) {
        //             OffsetRect(&new_rect, dx, dy);
        //         } else {
        //         }

        //         SetWindowPos(VdFw__Hwnd, NULL,
        //             new_rect.left, new_rect.top,
        //             new_rect.right - new_rect.left,
        //             new_rect.bottom - new_rect.top,
        //             SWP_NOZORDER | SWP_NOACTIVATE);
        //     }
        // } break;

        // case WM_LBUTTONUP: {
        //     if ((VdFw__WindowState == VD_FW__MOVING) || (VdFw__WindowState == VD_FW__SIZING)) {
        //         VdFw__WindowState = 0;
        //         ReleaseCapture();
        //     }
        // } break;

        default: {
            result = DefWindowProc(hwnd, msg, wparam, lparam);
        } break;
    }    
    return result;
}


#endif // _WIN32

#endif // VD_FW_IMPL