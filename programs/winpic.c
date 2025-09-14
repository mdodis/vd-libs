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
#endif // WP_ASSERTIONS

static LRESULT do_main(void);
static LRESULT WinMainCRTStartup(void);
static LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

#define WP_VERSION        L"v2.0"
#define WP_CLASS_NAMEW    L"WINPIC Window Class"
#define WP_DEFAULT_TITLE  L"winpic " WP_VERSION

LRESULT do_main(void)
{
    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    WNDCLASSEXW wcx = {0};
    wcx.cbSize         = sizeof(wcx);
    wcx.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcx.hInstance      = NULL;
    wcx.lpfnWndProc    = wndproc;
    wcx.lpszClassName  = WP_CLASS_NAMEW;
    wcx.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcx.hCursor        = LoadCursorW(NULL, IDC_ARROW);
    WP_ASSERT(RegisterClassExW(&wcx));

    RECT rect = {0, 0, 640, 480};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, 0);

    int width  = rect.right  - rect.left;
    int height = rect.bottom - rect.top;

    CreateWindowExW(0,
                    WP_CLASS_NAMEW,
                    WP_DEFAULT_TITLE,
                    WS_OVERLAPPEDWINDOW,
                    CW_USEDEFAULT, CW_USEDEFAULT,
                    width, height,
                    0, 0,
                    GetModuleHandle(0),
                    0);
    return 0;
}

static LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    
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
