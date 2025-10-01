// #define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
// #define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
// #define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
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
#include <windowsx.h>
#include <shellapi.h>
#include <uxtheme.h>
#include <dwmapi.h>
#include <shellscalingapi.h>
#include <versionhelpers.h>
#include "ext/glcorearb.h"
#include "ext/glext.h"
#include "ext/wgl.h"

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

#ifndef WP_ASSERTIONS
#define WP_ASSERTIONS 1
#endif // !WP_ASSERTIONS

#if WP_ASSERTIONS
#define WP_ASSERT(expr) do { if (!(expr)) { OutputDebugStringA(#expr " failed"); ExitProcess(100); } } while(0)
#else
#define WP_ASSERT(expr)
#endif // WP_ASSERTIONS

/* ----GL FUNCTIONS-------------------------------------------------------------------------------------------------- */
#include <GL/gl.h>
PFNGLDRAWARRAYSINSTANCEDPROC glDrawArraysInstanced;

/* ----FUNCTIONS----------------------------------------------------------------------------------------------------- */
static LRESULT          do_main(void);
static void             render(void);
static void             resize(void);
static void             start_render_tick_update(void);
static void             stop_render_tick_update(void);
static void             render_timer_proc(HWND hwnd, UINT arg2, UINT_PTR arg3, DWORD arg4);
LRESULT                 WinMainCRTStartup(void);
static LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

#define WP_VERSION            L"v2.0"
#define WP_CLASS_NAMEW        L"WINPIC Window Class"
#define WP_DEFAULT_TITLE      L"winpic " WP_VERSION
#define WP_WSEX               WS_EX_APPWINDOW | WS_EX_LAYERED
#define WP_WS                 WS_OVERLAPPEDWINDOW | WS_SIZEBOX
#define WP_TIMERID_RENDERPROC 1
#define WP_TIMERID_ZOOMPROC   2

static struct {
    HWND      hwnd;
    HDC       hdc;
    HGLRC     hglrc;
    int       will_render;
    int       client_width;
    int       client_height;
    int       render_tick_timer_active;
    float     last_mouse_pos[2];
} App = {0};

LRESULT do_main(void)
{
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

/* ----INITIALIZE WINDOW--------------------------------------------------------------------------------------------- */
    WNDCLASSEXW wcx = {0};
    wcx.cbSize         = sizeof(wcx);
    wcx.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    wcx.hInstance      = NULL;
    wcx.lpfnWndProc    = wndproc;
    wcx.lpszClassName  = WP_CLASS_NAMEW;
    wcx.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcx.hCursor        = LoadCursorW(NULL, IDC_ARROW);
    WP_ASSERT(RegisterClassExW(&wcx));

    RECT rect = {0, 0, 640, 480};
    AdjustWindowRect(&rect, WP_WS, 0);

    int left   = CW_USEDEFAULT;
    int top    = CW_USEDEFAULT;
    int width  = rect.right  - rect.left;
    int height = rect.bottom - rect.top;

    App.hwnd = CreateWindowExW(0,
                               WP_CLASS_NAMEW,
                               WP_DEFAULT_TITLE,
                               WP_WS,
                               left, top,
                               width, height,
                               0, 0,
                               GetModuleHandle(0),
                               0);
    WP_ASSERT(App.hwnd != INVALID_HANDLE_VALUE);
    App.hdc = GetDC(App.hwnd);

/* ----INITIALIZE OPENGL--------------------------------------------------------------------------------------------- */
    {
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

        int pf = ChoosePixelFormat(App.hdc, &pfd);
        SetPixelFormat(App.hdc, pf, &pfd);

        HGLRC temprc = wglCreateContext(App.hdc);
        WP_ASSERT(temprc != 0);
        wglMakeCurrent(App.hdc, temprc);

        int major = 3;
        int minor = 3;
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
            WGL_COLOR_BITS_ARB,     32,
            WGL_ALPHA_BITS_ARB,     8,
            WGL_DEPTH_BITS_ARB,     24,
            WGL_STENCIL_BITS_ARB,   8,

            WGL_SAMPLE_BUFFERS_ARB, 1,
            WGL_SAMPLES_ARB,        4,
            0,
        };

        int pixel_format;
        UINT num_formats;

        wglChoosePixelFormatARB(App.hdc, pixel_attribs, NULL, 1, &pixel_format, &num_formats);

        PIXELFORMATDESCRIPTOR pfdchosen;
        DescribePixelFormat(App.hdc, pixel_format, sizeof(pfdchosen), &pfdchosen);
        SetPixelFormat(App.hdc, pf, &pfdchosen);

        App.hglrc = wglCreateContextAttribsARB(App.hdc, 0, attribs);

        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(temprc);

        wglMakeCurrent(App.hdc, App.hglrc);
    }


    {

#define LOAD(p, s) s = (p)wglGetProcAddress(#s)
        LOAD(PFNGLDRAWARRAYSINSTANCEDPROC,       glDrawArraysInstanced);
#undef LOAD
    }

    ShowWindow(App.hwnd, SW_SHOW);
    UpdateWindow(App.hwnd);

    resize();
    render();
    SwapBuffers(App.hdc);

    MSG message;
    while (GetMessage(&message, App.hwnd, 0, 0) > 0) {
        App.will_render = 0;
        TranslateMessage(&message);
        DispatchMessage(&message);

        if (App.will_render) {
            render();
            SwapBuffers(App.hdc);
        }
    }
    return 0;
}

static void render(void)
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void resize(void)
{
    glViewport(0, 0, App.client_width, App.client_height);
}

static void start_render_tick_update(void)
{
    int tick_rate = 8;

    if (!App.render_tick_timer_active) {
        App.render_tick_timer_active = 1;
        UINT_PTR timer_id = SetTimer(App.hwnd, WP_TIMERID_RENDERPROC, tick_rate, (TIMERPROC)render_timer_proc);
        WP_ASSERT(timer_id);
    }
}

static void stop_render_tick_update(void)
{
    KillTimer(App.hwnd, WP_TIMERID_RENDERPROC);
    App.render_tick_timer_active = 0;
}

static void render_timer_proc(HWND hwnd, UINT arg2, UINT_PTR arg3, DWORD arg4)
{
    (void)arg4;
    (void)arg2;
    (void)hwnd;

    switch (arg3) {
        case WP_TIMERID_RENDERPROC: {
            render();
            SwapBuffers(App.hdc);
        } break;

        default: break;
    }
}

static LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LRESULT result = 0;
    
    switch(msg) {
        
        case WM_DESTROY: {
            PostQuitMessage(0);
        } break;

        case WM_CLOSE: {
            DestroyWindow(hwnd);
        } break;
        
        case WM_PAINT: {
            render();
            SwapBuffers(App.hdc);
        } break;

        case WM_SIZE: {
            App.client_width  = LOWORD(lparam);
            App.client_height = HIWORD(lparam);
            resize();
        } break;
        
        case WM_SIZING: {
            RECT *rect = (RECT *)lparam;
            App.client_width  = rect->right - rect->left;
            App.client_height = rect->bottom - rect->top;
            
            resize();
            // render();
            // SwapBuffers(App.hdc);
        } break;
        
        case WM_ENTERSIZEMOVE: {
            
            // TODO(miked): Get refresh rate of monitor and set timer at the appropriate interval
            start_render_tick_update();
            
        } break;
        
        case WM_EXITSIZEMOVE: {
            stop_render_tick_update();
        } break;
        
        
        case WM_MOUSEMOVE: {
            // float current_pos[2] = {(float)(GET_X_LPARAM(lparam)), (float)(GET_Y_LPARAM(lparam))};
        } break;
        
        case WM_LBUTTONDBLCLK: {
            //PostQuitMessage(0);
        } break;
        
        case WM_RBUTTONUP: {
            POINT point;
            point.x = LOWORD(lparam);
            point.y = HIWORD(lparam);
        } break;
        
        case WM_MOUSEWHEEL: {
            
            LONG xpos = GET_X_LPARAM(lparam);
            LONG ypos = GET_Y_LPARAM(lparam);
            POINT p = {xpos, ypos};
            WP_ASSERT(ScreenToClient(hwnd, &p));
            // // NOTE(miked): lol this definitely didnt take up a lot of debugging time
            // Vec2 pos = {
            //     float(p.x),
            //     float(p.y)
            // };
            
            // float amount = (float)(GET_WHEEL_DELTA_WPARAM(wparam))/120.f;
            // scroll(&g_app, amount, pos);
            
            // TODO(miked): Get refresh rate of monitor and set timer at the appropriate interval
        } break;
        
        case WM_KEYUP: {
            // keypress(wparam);
        } break;
        
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP: {
            // bool was_down = g_app.lmouse_is_down;
            // bool is_down = (msg == WM_LBUTTONDOWN);
            // g_app.lmouse_is_down = is_down;
            
            // if (!was_down && is_down) {
            //     SetCapture(g_app.window_handle);
            //     g_app.last_mouse_pos = Vec2{float(GET_X_LPARAM(lparam)), float(GET_Y_LPARAM(lparam))};
                
            // } else if (was_down && !is_down) {
            //     ReleaseCapture();
            // }
            
        } break;
        
        default: {
            result = DefWindowProc(hwnd, msg, wparam, lparam);
        } break;
    }

    return result;
}

LRESULT WinMainCRTStartup(void)
{
    LRESULT result = do_main();
    ExitProcess((UINT)result);
}

#pragma comment(lib, "User32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(linker, "/subsystem:windows")
#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:msvcrt.lib")
#pragma comment(linker, "/NODEFAULTLIB:msvcrtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:oldnames.lib")

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
