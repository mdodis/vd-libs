/* vd_fc.h - Controller API
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
#ifndef VD_FC_H
#define VD_FC_H
#define VD_FC_VERSION_MAJOR    0
#define VD_FC_VERSION_MINOR    0
#define VD_FC_VERSION_PATCH    2
#define VD_FC_VERSION          ((VD_FC_VERSION_MAJOR << 16) | (VD_FC_VERSION_MINOR << 8) | (VD_FC_VERSION_PATCH))

#ifndef VD_FC_API
#   ifdef VD_FC_STATIC
#       define VD_FC_API static
#   else
#       define VD_FC_API extern
#   endif // VD_FC_STATIC
#endif // !VD_FC_API

#ifndef VD_FC_INL
#   define VD_FC_INL static inline
#endif // VD_FC_INL

typedef enum {
    VD_FC_TYPE_INVALID = -1,
    VD_FC_TYPE_CONTROLLER = 0,
    VD_FC_TYPE_WHEEL,
    VD_FC_TYPE_MAX,
} VdFcType;

typedef enum {
    VD_FC_CLASS_NES,
    // class:megadrive    | 1 PoV, 3 Control, 1 System
    VD_FC_CLASS_MEGADRIVE,
    // class:genesis      | 1 PoV, 6 Control, 2 System
    VD_FC_CLASS_GENESIS,
    // class:snes         | 1 PoV, 4 Control, 2 System, 2 Symmetrical
    VD_FC_CLASS_SNES,
    // class:ps1          | 1 PoV, 4 Control, 2 System, 4 Symmetrical
    VD_FC_CLASS_PS1,
    // class:joycon       |        4 Control, 2 System, 2 Symmetrical, 1 Clickable Stick
    VD_FC_CLASS_JOYCON,
    // class:n64          | 1 PoV, 6 Control, 2 System, 2 Symmetrical, 1 Stick
    VD_FC_CLASS_N64,
    // class:ps2          | 1 PoV, 4 Control, 2 System, 4 Symmetrical, 2 Clickable Sticks
    VD_FC_CLASS_PS2,
    // class:xbox         | 1 PoV, 4 Control, 2 System, 2 Symmetrical, 2 Clickable Sticks, 2 Symmetrical Axes
    VD_FC_CLASS_XBOX,
    // class:ps4          | 1 PoV, 4 Control, 2 System, 2 Symmetrical, 2 Clickable Sticks, 2 Symmetrical Axes, 1 Touchpad
    VD_FC_CLASS_PS4,
    // class:steamdeck    | 1 PoV, 4 Control, 2 System, 6 Symmetrical, 2 Clickable Sticks, 2 Symmetrical Axes, 2 Touchpads 
    VD_FC_CLASS_STEAMDECK,
    VD_FC_CLASS_MAX,
} VdFcClass;

typedef enum {
    VD_FC_FACE_UNKNOWN = 0,
    VD_FC_FACE_NUMBERED,    /* face:numbered */
    VD_FC_FACE_XBOX,        /* face:xbox */
    VD_FC_FACE_PLAYSTATION, /* face:playstation */
    VD_FC_FACE_NINTENDO,    /* face:nintendo */
    VD_FC_FACE_MAX,
} VdFcFace;

/**
 * @brief Initialize the library.
 */
VD_FC_API void          vd_fc_init(void);

/**
 * @brief Poll for changes
 */
VD_FC_API void          vd_fc_poll(void);

/**
 * @brief Get device count
 * @return  The number of devices (max VD_FC_DEVICE_COUNT_MAX)
 */
VD_FC_API int           vd_fc_count(void);

/* ----DEVICE CHANGES------------------------------------------------------------------------------------------------ */
VD_FC_API int*          vd_fc_dropped(int *count);
VD_FC_API int*          vd_fc_arrived(int *count);

/* ----DEVICE PROPERTIES--------------------------------------------------------------------------------------------- */
VD_FC_API VdFcType      vd_fc_type(int id);
VD_FC_API int           vd_fc_button_count(int id);
VD_FC_API int           vd_fc_hat_count(int id);
VD_FC_API int           vd_fc_axis_count(int id);

/* ----DEVICE INPUTS------------------------------------------------------------------------------------------------- */
VD_FC_API unsigned int  vd_fc_raw_button_down(int id, int button_id);
VD_FC_API unsigned int  vd_fc_raw_hat_value(int id, int hat_id);
VD_FC_API unsigned int  vd_fc_raw_axis_value(int id, int axis_id);

VD_FC_API int           vd_fc_button_down(int id, int sym_button_id);
VD_FC_API unsigned int  vd_fc_button_press_count(int id, int sym_button_id);
VD_FC_API float         vd_fc_axis_value(int id, int axis_id);

#endif // !VD_FC_H

#ifdef VD_FC_IMPL

#ifndef VD_FC_LOG
#   include <stdio.h>
#   define VD_FC_LOG(fmt, ...) printf("vd_fc: " fmt "\n", ##__VA_ARGS__)
#endif // !VD_FC_LOG

#ifndef VD_FC_MEMSET
#   include <stdlib.h>
#   define VD_FC_MEMSET(ptr, val, num) memset(ptr, val, num)
#endif // !VD_FC_MEMSET

#ifndef VD_FC_MEMCMP
#   include <string.h>
#   define VD_FC_MEMCMP(lhs, rhs, count) memcmp(lhs, rhs, count)
#endif // !VD_FC_MEMCMP

#ifndef VD_FC_MEMCPY
#   include <string.h>
#   define VD_FC_MEMCPY(dst, src, count) memcpy(dst, src, count)
#endif // !VD_FC_MEMCPY

#ifndef VD_FC_DEVICE_COUNT_MAX
#   define VD_FC_DEVICE_COUNT_MAX 16
#endif // !VD_FC_DEVICE_COUNT_MAX

#ifndef VD_FC_DEVICE_NAME_MAX
#   define VD_FC_DEVICE_NAME_MAX 64
#endif // !VD_FC_DEVICE_NAME_MAX

/* ----CROSS PLATFORM PROCEDURES------------------------------------------------------------------------------------- */
typedef struct VdFc__Device VdFc__Device;
static int          vd_fc_device_classify_auto(VdFc__Device *device);
static const char*  vd_fc_class_str(VdFcClass klass);

#ifdef _WIN32
#ifndef _MINWINDEF_
#define VD_FC_DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

typedef unsigned long        VdFcDWORD;
typedef int                  VdFcBOOL;
typedef unsigned char        VdFcBYTE;
typedef unsigned short       VdFcWORD;
typedef float                VdFcFLOAT;
typedef VdFcFLOAT*          VdFcPFLOAT;
typedef VdFcBOOL*           VdFcPBOOL;
typedef VdFcBOOL*           VdFcLPBOOL;
typedef VdFcBYTE*           VdFcPBYTE;
typedef VdFcBYTE*           VdFcLPBYTE;
typedef int*                 VdFcPINT;
typedef int*                 VdFcLPINT;
typedef VdFcWORD*           VdFcPWORD;
typedef VdFcWORD*           VdFcLPWORD;
typedef long*                VdFcLPLONG;
typedef VdFcDWORD*          VdFcPDWORD;
typedef VdFcDWORD*          VdFcLPDWORD;
typedef void*                VdFcLPVOID;
typedef const void*          VdFcLPCVOID;
typedef int                  VdFcINT;
typedef unsigned int         VdFcUINT;
typedef unsigned int*        VdFcPUINT;
typedef uint64_t            VdFcUINT64;
typedef uint32_t            VdFcUINT32;
typedef int32_t             VdFcINT32;
typedef uint16_t            VdFcUINT16;
typedef uint8_t             VdFcUINT8;
typedef int16_t             VdFcINT16;
typedef void*               VdFcHANDLE;
typedef VdFcHANDLE*         VdFcPHANDLE;
typedef char                VdFcCHAR;
typedef const VdFcCHAR*     VdFcLPCSTR, *VdFcPCSTR;
typedef VdFcCHAR*           VdFcLPSTR;
typedef long                VdFcLONG;
typedef unsigned long       VdFcULONG;
typedef VdFcULONG*          VdFcPULONG;
typedef unsigned short      VdFcUSHORT;
typedef VdFcUSHORT*         VdFcPUSHORT;
typedef unsigned char       VdFcUCHAR;
typedef VdFcUCHAR*          VdFcPUCHAR;
typedef VdFcBYTE            VdFcBOOLEAN;
typedef VdFcCHAR*           VdFcPCHAR, * VdFcLPCH, * VdFcPCH;
typedef long                VdFcHRESULT;
typedef __int64             VdFcLONGLONG;
typedef unsigned __int64    VdFcULONGLONG;
typedef VdFcULONGLONG       VdFcDWORDLONG;
typedef VdFcDWORDLONG*      VdFcPDWORDLONG;
typedef __int64             VdFcINT_PTR, * VdFcPINT_PTR;
typedef unsigned __int64    VdFcUINT_PTR, * VdFcPUINT_PTR;
typedef __int64             VdFcLONG_PTR, * VdFcPLONG_PTR;
typedef unsigned __int64    VdFcULONG_PTR, * VdFcPULONG_PTR;
typedef VdFcUINT_PTR        VdFcWPARAM;
typedef VdFcLONG_PTR        VdFcLPARAM;
typedef VdFcLONG_PTR        VdFcLRESULT;
typedef wchar_t             VdFcWCHAR;
typedef const VdFcWCHAR*    VdFcLPCWSTR, * VdFcPCWSTR;
typedef VdFcWCHAR*          VdFcLPWSTR;
typedef VdFcWORD            VdFcATOM;
typedef VdFcULONG_PTR       VdFcDWORD_PTR, * VdFcPDWORD_PTR;
typedef VdFcULONG_PTR       VdFcSIZE_T;
typedef short               VdFcSHORT;
typedef VdFcDWORD           VdFcCOLORREF;
typedef VdFcDWORD*          VdFcLPCOLORREF;

VD_FC_DECLARE_HANDLE(VdFcHWND);
VD_FC_DECLARE_HANDLE(VdFcHINSTANCE);
VD_FC_DECLARE_HANDLE(VdFcHDC);
VD_FC_DECLARE_HANDLE(VdFcHMONITOR);
VD_FC_DECLARE_HANDLE(VdFcHGDIOBJ);
VD_FC_DECLARE_HANDLE(VdFcHWND);
VD_FC_DECLARE_HANDLE(VdFcHICON);
VD_FC_DECLARE_HANDLE(VdFcHBITMAP);
VD_FC_DECLARE_HANDLE(VdFcHBRUSH);
VD_FW_DECLARE_HANDLE(VdFcHMENU);

typedef struct VdFc_FILETIME {
  VdFcDWORD dwLowDateTime;
  VdFcDWORD dwHighDateTime;
} VdFcFILETIME, *VdFcPFILETIME, *VdFcLPFILETIME;

typedef VdFcHINSTANCE VdFcHMODULE;

extern VdFcHMODULE LoadLibraryA(VdFcLPCSTR path);
extern void*       HeapAlloc(VdFcHANDLE hHeap, VdFcDWORD dwFlags, VdFcSIZE_T dwBytes);
extern VdFcHANDLE  GetProcessHeap();
extern void*       HeapReAlloc(VdFcHANDLE hHeap, VdFcDWORD dwFlags, void *lpMem, VdFcSIZE_T dwBytes);
extern VdFcBOOL    HeapFree(VdFcHANDLE hHeap, VdFcDWORD dwFlags, void *lpMem);
extern int         MultiByteToWideChar(VdFcUINT CodePage, VdFcDWORD dwFlags, VdFcLPCSTR lpMultiByteStr, int cbMultiByte, VdFcLPWSTR lpWideCharStr, int cchWideChar);
extern int         WideCharToMultiByte(VdFcUINT CodePage, VdFcDWORD dwFlags, VdFcLPCWSTR lpWideCharStr, int cchWideChar, VdFcLPSTR lpMultiByteStr, int cbMultiByte, VdFcLPSTR lpDefaultChar, VdFcBOOL *lpUsedDefaultChar);
extern void*       GetProcAddress(VdFcHMODULE hModule, VdFcLPCSTR lpProcName);
extern VdFcHMODULE GetModuleHandleA(VdFcLPCSTR lpModuleName);
extern int         WideCharToMultiByte(VdFcUINT CodePage, VdFcDWORD dwFlags, VdFcLPCWSTR lpWideCharStr, int cchWideChar, VdFcLPSTR lpMultiByteStr, int cbMultiByte, VdFcLPSTR lpDefaultChar, VdFcBOOL *lpUsedDefaultChar);
#else
typedef HWND        VdFcHWND;
typedef DWORD       VdFcDWORD;
typedef WORD        VdFcWORD;
typedef LPWSTR      VdFcLPWSTR;
typedef LPCWSTR     VdFcLPCWSTR;
typedef LPCSTR      VdFcLPCSTR;
typedef DWORD       VdFcDWORD;
typedef UINT        VdFcUINT;
typedef UINT64      VdFcUINT64;
typedef UINT32      VdFcUINT32;
typedef INT32       VdFcINT32;
typedef UINT16      VdFcUINT16;
typedef UINT8       VdFcUINT8;
typedef INT16       VdFcINT16;
typedef HMODULE     VdFcHMODULE;
typedef HRESULT     VdFcHRESULT;
typedef ULONG       VdFcULONG;
typedef HDC         VdFcHDC;
typedef HMONITOR    VdFcHMONITOR;
typedef HANDLE      VdFcHANDLE;
typedef HGDIOBJ     VdFcHGDIOBJ;
typedef HBITMAP     VdFcHBITMAP;
typedef LPARAM      VdFcLPARAM;
typedef BOOL        VdFcPBOOL;
typedef LONG        VdFcLONG;
typedef BYTE        VdFcBYTE;
typedef WCHAR       VdFcWCHAR;
typedef BOOL        VdFcBOOL;
typedef LPVOID      VdFcLPVOID;
typedef FLOAT       VdFcFLOAT;
typedef LPLONG      VdFcLPLONG;
typedef VdFcDWORD   VdFcCOLORREF;
typedef VdFcDWORD*  VdFcLPCOLORREF;
typedef LPDWORD     VdFcLPDWORD;
typedef HINSTANCE   VdFcHINSTANCE;
typedef UINT_PTR    VdFcUINT_PTR, * VdFcPUINT_PTR;
typedef FILETIME    VdFcFILETIME;
typedef LONG_PTR    VdFcLONG_PTR;
typedef LRESULT     VdFcLRESULT;
typedef HICON       VdFcHICON;
typedef HCURSOR     VdFcHCURSOR;
typedef HBITMAP     VdFcHBITMAP;
typedef HBRUSH      VdFcHBRUSH;
typedef ATOM        VdFcATOM;
typedef HMENU       VdFcHMENU;
#endif // !_MINWINDEF_

typedef struct VdFc_GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} VdFcGUID;

typedef VdFcGUID            VdFcIID;
typedef VdFcGUID*           VdFcREFGUID;
#define VdFcREFCLSID        const VdFcIID *
typedef VdFcIID*            VdFcREFIID;
typedef struct VdFcIUnknown VdFcIUnknown;
typedef VdFcIUnknown*       VdFcLPUNKNOWN;
typedef VdFcULONG           VdFcSFGAOF;
typedef VdFcDWORD           VdFcSICHINTF;

typedef VdFcLONG_PTR        VdFcLPARAM;
typedef VdFcUINT_PTR        VdFcWPARAM;
typedef VdFcLRESULT(*VdFcWNDPROC)(VdFcHWND, VdFcUINT, VdFcWPARAM, VdFcLPARAM);

typedef struct VdFc__Win32_tagWNDCLASSW {
  VdFcUINT      style;
  VdFcWNDPROC   lpfnWndProc;
  int           cbClsExtra;
  int           cbWndExtra;
  VdFcHINSTANCE hInstance;
  VdFcHICON     hIcon;
  VdFcHCURSOR   hCursor;
  VdFcHBRUSH    hbrBackground;
  VdFcLPCWSTR   lpszMenuName;
  VdFcLPCWSTR   lpszClassName;
} VdFc__Win32_WNDCLASSW, *VdFc__Win32_PWNDCLASSW, *VdFc__Win32_NPWNDCLASSW, *VdFc__Win32_LPWNDCLASSW;

typedef struct VdFctagPOINT
{
    VdFcLONG  x;
    VdFcLONG  y;
} VdFcPOINT, * VdFcPPOINT, * VdFcNPPOINT, * VdFcLPPOINT;

typedef struct VdFctagMSG {
    VdFcHWND        hwnd;
    VdFcUINT        message;
    VdFcWPARAM      wParam;
    VdFcLPARAM      lParam;
    VdFcDWORD       time;
    VdFcPOINT       pt;
#ifdef _MAC
    DWORD       lPrivate;
#endif
} VdFcMSG, * VdFcPMSG, * VdFcNPMSG, * VdFcLPMSG;

#define VD_FC__WIN32_FUNCTIONS \
    V("dinput8.dll") \
    X(VdFcHRESULT, DirectInput8Create, (VdFcHINSTANCE hinst, VdFcDWORD dwVersion, VdFcREFIID riidltf, VdFcLPVOID *ppvOut, VdFcLPUNKNOWN punkOuter)) \
    VE() \
    V("User32.dll") \
    X(VdFcATOM, RegisterClassW, (const VdFc__Win32_WNDCLASSW*)) \
    X(VdFcHWND, CreateWindowExW, (DWORD dwExStyle, VdFcLPCWSTR lpClassName, VdFcLPCWSTR lpWindowName, VdFcDWORD dwStyle, int X, int Y, int nWidth, int nHeight, VdFcHWND hWndParent, VdFcHMENU hMenu, VdFcHINSTANCE hInstance, VdFcLPVOID lpParam)) \
    X(VdFcLRESULT, DefWindowProcW, (VdFcHWND hWnd, VdFcUINT Msg, VdFcWPARAM wParam, VdFcLPARAM lParam)) \
    X(VdFcBOOL, GetMessageW, (VdFcLPMSG lpMsg, VdFcHWND hWnd, VdFcUINT wMsgFilterMin, VdFcUINT wMsgFilterMax)) \
    X(VdFcBOOL, TranslateMessage, (const VdFcMSG* lpMsg)) \
    X(VdFcLRESULT, DispatchMessageW, (const VdFcMSG* lpMsg)) \
    X(VdFcBOOL, PeekMessageW, (VdFcLPMSG lpMsg, VdFcHWND hWnd, VdFcUINT wMsgFilterMin, VdFcUINT wMsgFilterMax, VdFcUINT wRemoveMsg)) \
    VE() \

#define V(dllpath)
#define _X2(s, e) s##e
#define X(retval, name, params) typedef retval (*_X2(VdFcProc,name))params; static _X2(VdFcProc,name) _X2(VdFc,name);
#define VE()
VD_FC__WIN32_FUNCTIONS
#undef V
#undef _X2
#undef X
#undef VE

#define VD_FC_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) VdFcGUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

typedef struct VdFc__Win32_IUnknown VdFc__Win32_IUnknown;

typedef struct VdFc__Win32_IDirectInput8W VdFc__Win32_IDirectInput8W;
VD_FC_DEFINE_GUID(VdFc__Win32_IID_IDirectInput8W,0xBF798031,0x483A,0x4DA2,0xAA,0x99,0x5D,0x64,0xED,0x36,0x97,0x00);

typedef struct VdFc__Win32_IDirectInputDevice8W VdFc__Win32_IDirectInputDevice8W;
VD_FC_DEFINE_GUID(VdFc__Win32_IID_IDirectInputDevice8W,0x54D41081,0xDC15,0x4833,0xA4,0x1B,0x74,0x8F,0x73,0xA3,0x81,0x79);

VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_XAxis,   0xa36d02e0,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_YAxis,   0xa36d02e1,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_ZAxis,   0xa36d02e2,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_RxAxis,  0xa36d02f4,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_RyAxis,  0xa36d02f5,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_RzAxis,  0xa36d02e3,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_Slider,  0xa36d02e4,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_PoV,     0xa36d02f2,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);

typedef struct VdFc__Win32_DIDEVICEINSTANCEW {
    VdFcDWORD   dwSize;
    VdFcGUID    guidInstance;
    VdFcGUID    guidProduct;
    VdFcDWORD   dwDevType;
    VdFcWCHAR   tszInstanceName[MAX_PATH];
    VdFcWCHAR   tszProductName[MAX_PATH];
    VdFcGUID    guidFFDriver;
    VdFcWORD    wUsagePage;
    VdFcWORD    wUsage;
} VdFc__Win32_DIDEVICEINSTANCEW, *VdFc__Win32_LPDIDEVICEINSTANCEW;

typedef struct VdFc__Win32_DIACTIONW {
    VdFcUINT_PTR    uAppData;
    VdFcDWORD       dwSemantic;
    VdFcDWORD       dwFlags;
    union {
        VdFcLPCWSTR     lptszActionName;
        VdFcUINT        uResIdString;
    } DUMMYUNIONNAME;
    VdFcGUID        guidInstance;
    VdFcDWORD       dwObjID;
    VdFcDWORD       dwHow;
} VdFc__Win32_DIACTIONW, *VdFc__Win32_LPDIACTIONW;

typedef struct VdFc__Win32_DIACTIONFORMATW {
   VdFcDWORD       dwSize;
   VdFcDWORD       dwActionSize;
   VdFcDWORD       dwDataSize;
   VdFcDWORD       dwNumActions;
   VdFc__Win32_LPDIACTIONW rgoAction;
   VdFcGUID        guidActionMap;
   VdFcDWORD       dwGenre;
   VdFcDWORD       dwBufferSize;
   VdFcLONG        lAxisMin;
   VdFcLONG        lAxisMax;
   VdFcHINSTANCE   hInstString;
   VdFcFILETIME    ftTimeStamp;
   VdFcDWORD       dwCRC;
   VdFcWCHAR       tszActionMap[MAX_PATH];
} VdFc__Win32_DIACTIONFORMATW, *VdFc__Win32_LPDIACTIONFORMATW;

typedef struct VdFc__Win32_DICOLORSET{
    VdFcDWORD dwSize;
    VdFcDWORD cTextFore;
    VdFcDWORD cTextHighlight;
    VdFcDWORD cCalloutLine;
    VdFcDWORD cCalloutHighlight;
    VdFcDWORD cBorder;
    VdFcDWORD cControlFill;
    VdFcDWORD cHighlightFill;
    VdFcDWORD cAreaFill;
} VdFc__Win32_DICOLORSET, *VdFc__Win32_LPDICOLORSET;
typedef const VdFc__Win32_DICOLORSET *VdFc__Win32_LPCDICOLORSET;

typedef struct VdFc__Win32_DICONFIGUREDEVICESPARAMSW{
    VdFcDWORD                      dwSize;
    VdFcDWORD                      dwcUsers;
    VdFcLPWSTR                     lptszUserNames;
    VdFcDWORD                      dwcFormats;
    VdFc__Win32_LPDIACTIONFORMATW  lprgFormats;
    VdFcHWND                       hwnd;
    VdFc__Win32_DICOLORSET         dics;
    VdFc__Win32_IUnknown *lpUnkDDSTarget;
} VdFc__Win32_DICONFIGUREDEVICESPARAMSW, *VdFc__Win32_LPDICONFIGUREDEVICESPARAMSW;

typedef struct VdFc__Win32_DIPROPHEADER {
    VdFcDWORD   dwSize;
    VdFcDWORD   dwHeaderSize;
    VdFcDWORD   dwObj;
    VdFcDWORD   dwHow;
} VdFc__Win32_DIPROPHEADER, *VdFc__Win32_LPDIPROPHEADER;
typedef const VdFc__Win32_DIPROPHEADER* VdFc__Win32_LPCDIPROPHEADER;

typedef struct VdFc__Win32_DIPROPDWORD {
    VdFc__Win32_DIPROPHEADER diph;
    VdFcDWORD   dwData;
} VdFc__Win32_DIPROPDWORD, *VdFc__Win32_LPDIPROPDWORD;

typedef struct VdFc__Win32_DIDEVCAPS {
    VdFcDWORD   dwSize;
    VdFcDWORD   dwFlags;
    VdFcDWORD   dwDevType;
    VdFcDWORD   dwAxes;
    VdFcDWORD   dwButtons;
    VdFcDWORD   dwPOVs;
    VdFcDWORD   dwFFSamplePeriod;
    VdFcDWORD   dwFFMinTimeResolution;
    VdFcDWORD   dwFirmwareRevision;
    VdFcDWORD   dwHardwareRevision;
    VdFcDWORD   dwFFDriverVersion;
} VdFc__Win32_DIDEVCAPS, *VdFc__Win32_LPDIDEVCAPS;

typedef struct VdFc__Win32_DIDEVICEOBJECTINSTANCEW {
    VdFcDWORD   dwSize;
    VdFcGUID    guidType;
    VdFcDWORD   dwOfs;
    VdFcDWORD   dwType;
    VdFcDWORD   dwFlags;
    VdFcWCHAR   tszName[MAX_PATH];
    VdFcDWORD   dwFFMaxForce;
    VdFcDWORD   dwFFForceResolution;
    VdFcWORD    wCollectionNumber;
    VdFcWORD    wDesignatorIndex;
    VdFcWORD    wUsagePage;
    VdFcWORD    wUsage;
    VdFcDWORD   dwDimension;
    VdFcWORD    wExponent;
    VdFcWORD    wReportId;
} VdFc__Win32_DIDEVICEOBJECTINSTANCEW, *VdFc__Win32_LPDIDEVICEOBJECTINSTANCEW;
typedef const VdFc__Win32_DIDEVICEOBJECTINSTANCEW *VdFc__Win32_LPCDIDEVICEOBJECTINSTANCEW;

typedef struct VdFc__Win32_DIDEVICEOBJECTDATA {
    VdFcDWORD       dwOfs;
    VdFcDWORD       dwData;
    VdFcDWORD       dwTimeStamp;
    VdFcDWORD       dwSequence;
    VdFcUINT_PTR    uAppData;
} VdFc__Win32_DIDEVICEOBJECTDATA, *VdFc__Win32_LPDIDEVICEOBJECTDATA;

typedef struct VdFc__Win32_DIOBJECTDATAFORMAT {
    const VdFcGUID *pguid;
    VdFcDWORD   dwOfs;
    VdFcDWORD   dwType;
    VdFcDWORD   dwFlags;
} VdFc__Win32_DIOBJECTDATAFORMAT, *VdFc__Win32_LPDIOBJECTDATAFORMAT;
typedef const VdFc__Win32_DIOBJECTDATAFORMAT *VdFc__Win32_LPCDIOBJECTDATAFORMAT;

typedef struct VdFc__Win32_DIDATAFORMAT {
    DWORD   dwSize;
    DWORD   dwObjSize;
    DWORD   dwFlags;
    DWORD   dwDataSize;
    DWORD   dwNumObjs;
    VdFc__Win32_LPDIOBJECTDATAFORMAT rgodf;
} VdFc__Win32_DIDATAFORMAT, *VdFc__Win32_LPDIDATAFORMAT;
typedef const VdFc__Win32_DIDATAFORMAT *VdFc__Win32_LPCDIDATAFORMAT;

typedef struct VdFc__Win32_DIENVELOPE {
    VdFcDWORD dwSize;                   /* sizeof(DIENVELOPE)   */
    VdFcDWORD dwAttackLevel;
    VdFcDWORD dwAttackTime;             /* Microseconds         */
    VdFcDWORD dwFadeLevel;
    VdFcDWORD dwFadeTime;               /* Microseconds         */
} VdFc__Win32_DIENVELOPE, *VdFc__Win32_LPDIENVELOPE;
typedef const VdFc__Win32_DIENVELOPE *VdFc__Win32_LPCDIENVELOPE;

typedef struct VdFc__Win32_DIEFFECT {
    VdFcDWORD dwSize;                   /* sizeof(DIEFFECT)     */
    VdFcDWORD dwFlags;                  /* DIEFF_*              */
    VdFcDWORD dwDuration;               /* Microseconds         */
    VdFcDWORD dwSamplePeriod;           /* Microseconds         */
    VdFcDWORD dwGain;
    VdFcDWORD dwTriggerButton;          /* or DIEB_NOTRIGGER    */
    VdFcDWORD dwTriggerRepeatInterval;  /* Microseconds         */
    VdFcDWORD cAxes;                    /* Number of axes       */
    VdFcLPDWORD rgdwAxes;               /* Array of axes        */
    VdFcLPLONG rglDirection;            /* Array of directions  */
    VdFc__Win32_LPDIENVELOPE lpEnvelope;/* Optional             */
    VdFcDWORD cbTypeSpecificParams;     /* Size of params       */
    VdFcLPVOID lpvTypeSpecificParams;   /* Pointer to params    */
    VdFcDWORD  dwStartDelay;            /* Microseconds         */
} VdFc__Win32_DIEFFECT, *VdFc__Win32_LPDIEFFECT;
typedef VdFc__Win32_DIEFFECT VdFc__Win32_DIEFFECT_DX6;
typedef VdFc__Win32_LPDIEFFECT VdFc__Win32_LPDIEFFECT_DX6;
typedef const VdFc__Win32_DIEFFECT *VdFc__Win32_LPCDIEFFECT;

typedef struct VdFc__Win32_DIJOYSTATE {
    VdFcLONG    lX;                     /* x-axis position              */
    VdFcLONG    lY;                     /* y-axis position              */
    VdFcLONG    lZ;                     /* z-axis position              */
    VdFcLONG    lRx;                    /* x-axis rotation              */
    VdFcLONG    lRy;                    /* y-axis rotation              */
    VdFcLONG    lRz;                    /* z-axis rotation              */
    VdFcLONG    rglSlider[2];           /* extra axes positions         */
    VdFcDWORD   rgdwPOV[4];             /* POV directions               */
    VdFcBYTE    rgbButtons[32];         /* 32 buttons                   */
} VdFc__Win32_DIJOYSTATE, *VdFc__Win32_LPDIJOYSTATE;

typedef struct VdFc__Win32_DIJOYSTATE2 {
    VdFcLONG    lX;                     /* x-axis position              */
    VdFcLONG    lY;                     /* y-axis position              */
    VdFcLONG    lZ;                     /* z-axis position              */
    VdFcLONG    lRx;                    /* x-axis rotation              */
    VdFcLONG    lRy;                    /* y-axis rotation              */
    VdFcLONG    lRz;                    /* z-axis rotation              */
    VdFcLONG    rglSlider[2];           /* extra axes positions         */
    VdFcDWORD   rgdwPOV[4];             /* POV directions               */
    VdFcBYTE    rgbButtons[128];        /* 128 buttons                  */
    VdFcLONG    lVX;                    /* x-axis velocity              */
    VdFcLONG    lVY;                    /* y-axis velocity              */
    VdFcLONG    lVZ;                    /* z-axis velocity              */
    VdFcLONG    lVRx;                   /* x-axis angular velocity      */
    VdFcLONG    lVRy;                   /* y-axis angular velocity      */
    VdFcLONG    lVRz;                   /* z-axis angular velocity      */
    VdFcLONG    rglVSlider[2];          /* extra axes velocities        */
    VdFcLONG    lAX;                    /* x-axis acceleration          */
    VdFcLONG    lAY;                    /* y-axis acceleration          */
    VdFcLONG    lAZ;                    /* z-axis acceleration          */
    VdFcLONG    lARx;                   /* x-axis angular acceleration  */
    VdFcLONG    lARy;                   /* y-axis angular acceleration  */
    VdFcLONG    lARz;                   /* z-axis angular acceleration  */
    VdFcLONG    rglASlider[2];          /* extra axes accelerations     */
    VdFcLONG    lFX;                    /* x-axis force                 */
    VdFcLONG    lFY;                    /* y-axis force                 */
    VdFcLONG    lFZ;                    /* z-axis force                 */
    VdFcLONG    lFRx;                   /* x-axis torque                */
    VdFcLONG    lFRy;                   /* y-axis torque                */
    VdFcLONG    lFRz;                   /* z-axis torque                */
    VdFcLONG    rglFSlider[2];          /* extra axes forces            */
} VdFc__Win32_DIJOYSTATE2, *VdFc__Win32_LPDIJOYSTATE2;

typedef VdFcBOOL (*VdFc__Win32_LPDIENUMDEVICESCALLBACKW)(VdFc__Win32_LPDIDEVICEINSTANCEW, void*);
typedef VdFcBOOL (*VdFc__Win32_LPDIENUMDEVICESBYSEMANTICSCBW)(VdFc__Win32_LPDIDEVICEINSTANCEW, VdFc__Win32_IDirectInput8W*, VdFcDWORD, VdFcDWORD, void*);
typedef VdFcBOOL (*VdFc__Win32_LPDICONFIGUREDEVICESCALLBACK)(VdFc__Win32_IUnknown *, void*);
typedef VdFcBOOL (*VdFc__Win32_LPDIENUMDEVICEOBJECTSCALLBACKW)(VdFc__Win32_LPCDIDEVICEOBJECTINSTANCEW, void*);

typedef struct {
    VdFcHRESULT (__stdcall *QueryInterface)(VdFc__Win32_IUnknown *This, VdFcREFIID riid, void **ppvObject);
    VdFcULONG   (__stdcall *AddRef)(VdFc__Win32_IUnknown *This);
    VdFcULONG   (__stdcall *Release)(VdFc__Win32_IUnknown *This);
} VdFc__Win32_IUnknownVtbl;
struct VdFc__Win32_IUnknown { const VdFc__Win32_IUnknownVtbl *lpVtbl; };

typedef struct {
    /* IUnknown */
    VdFcHRESULT (__stdcall *QueryInterface)(VdFc__Win32_IDirectInput8W *This, VdFcREFIID riid, void **ppvObject);
    VdFcULONG   (__stdcall *AddRef)(VdFc__Win32_IDirectInput8W *This);
    VdFcULONG   (__stdcall *Release)(VdFc__Win32_IDirectInput8W *This);

    /* IDirectInput8W */
    VdFcHRESULT (__stdcall *CreateDevice)(VdFc__Win32_IDirectInput8W *This, VdFcREFGUID , VdFc__Win32_IDirectInputDevice8W **, VdFc__Win32_IUnknown **);
    VdFcHRESULT (__stdcall *EnumDevices)(VdFc__Win32_IDirectInput8W *This, VdFcDWORD, VdFc__Win32_LPDIENUMDEVICESCALLBACKW, void*, VdFcDWORD);
    VdFcHRESULT (__stdcall *GetDeviceStatus)(VdFc__Win32_IDirectInput8W *This, VdFcREFGUID );
    VdFcHRESULT (__stdcall *RunControlPanel)(VdFc__Win32_IDirectInput8W *This, VdFcHWND, VdFcDWORD);
    VdFcHRESULT (__stdcall *Initialize)(VdFc__Win32_IDirectInput8W *This, VdFcHINSTANCE, VdFcDWORD);
    VdFcHRESULT (__stdcall *FindDevice)(VdFc__Win32_IDirectInput8W *This, VdFcREFGUID, VdFcLPCWSTR, VdFcGUID*);
    VdFcHRESULT (__stdcall *EnumDevicesBySemantics)(VdFc__Win32_IDirectInput8W *This, VdFcLPCWSTR, VdFc__Win32_LPDIACTIONFORMATW, VdFc__Win32_LPDIDEVICEINSTANCEW, void*, VdFcDWORD);
    VdFcHRESULT (__stdcall *ConfigureDevices)(VdFc__Win32_IDirectInput8W *This, VdFc__Win32_LPDICONFIGUREDEVICESCALLBACK, VdFc__Win32_LPDICONFIGUREDEVICESPARAMSW, VdFcDWORD, VdFcLPVOID);
} VdFc__Win32_IDirectInput8WVtbl;
struct VdFc__Win32_IDirectInput8W { const VdFc__Win32_IDirectInput8WVtbl *lpVtbl; };

typedef struct {
    /* IUnknown */
    VdFcHRESULT (__stdcall *QueryInterface)(VdFc__Win32_IDirectInputDevice8W *This, VdFcREFIID riid, void **ppvObject);
    VdFcULONG   (__stdcall *AddRef)(VdFc__Win32_IDirectInputDevice8W *This);
    VdFcULONG   (__stdcall *Release)(VdFc__Win32_IDirectInputDevice8W *This);

    /* IDirectInputDevice8W */
    VdFcHRESULT (__stdcall *GetCapabilities)(VdFc__Win32_IDirectInputDevice8W*, VdFc__Win32_LPDIDEVCAPS);
    VdFcHRESULT (__stdcall *EnumObjects)(VdFc__Win32_IDirectInputDevice8W*, VdFc__Win32_LPDIENUMDEVICEOBJECTSCALLBACKW, void*, VdFcDWORD);
    VdFcHRESULT (__stdcall *GetProperty)(VdFc__Win32_IDirectInputDevice8W*, VdFcREFGUID, VdFc__Win32_LPDIPROPHEADER);
    VdFcHRESULT (__stdcall *SetProperty)(VdFc__Win32_IDirectInputDevice8W*, VdFcREFGUID, VdFc__Win32_LPCDIPROPHEADER);
    VdFcHRESULT (__stdcall *Acquire)(VdFc__Win32_IDirectInputDevice8W*);
    VdFcHRESULT (__stdcall *Unacquire)(VdFc__Win32_IDirectInputDevice8W*);
    VdFcHRESULT (__stdcall *GetDeviceState)(VdFc__Win32_IDirectInputDevice8W*, VdFcDWORD, void*);
    VdFcHRESULT (__stdcall *GetDeviceData)(VdFc__Win32_IDirectInputDevice8W*, VdFcDWORD, VdFc__Win32_LPDIDEVICEOBJECTDATA, VdFcLPDWORD, VdFcDWORD);
    VdFcHRESULT (__stdcall *SetDataFormat)(VdFc__Win32_IDirectInputDevice8W*, VdFc__Win32_LPCDIDATAFORMAT);
    VdFcHRESULT (__stdcall *SetEventNotification)(VdFc__Win32_IDirectInputDevice8W*, VdFcHANDLE);
    VdFcHRESULT (__stdcall *SetCooperativeLevel)(VdFc__Win32_IDirectInputDevice8W*, VdFcHWND, VdFcDWORD);
    VdFcHRESULT (__stdcall *GetObjectInfo)(VdFc__Win32_IDirectInputDevice8W*, VdFc__Win32_LPDIDEVICEOBJECTINSTANCEW, VdFcDWORD, VdFcDWORD);
    VdFcHRESULT (__stdcall *GetDeviceInfo)(VdFc__Win32_IDirectInputDevice8W*, VdFc__Win32_LPDIDEVICEINSTANCEW);
    VdFcHRESULT (__stdcall *RunControlPanel)(VdFc__Win32_IDirectInputDevice8W*, VdFcHWND, VdFcDWORD);
    VdFcHRESULT (__stdcall *Initialize)(VdFc__Win32_IDirectInputDevice8W*, VdFcHINSTANCE, VdFcDWORD, VdFcREFGUID);
    VdFcHRESULT (__stdcall *CreateEffect)(VdFc__Win32_IDirectInputDevice8W*, VdFcREFGUID, VdFc__Win32_LPCDIEFFECT, void/*IDirectInputEffect*/**, VdFcLPUNKNOWN);
    VdFcHRESULT (__stdcall *EnumEffects)(VdFc__Win32_IDirectInputDevice8W*, void*/*LPDIENUMEFFECTSCALLBACKW*/, void*, VdFcDWORD);
    VdFcHRESULT (__stdcall *GetEffectInfo)(VdFc__Win32_IDirectInputDevice8W*, void*/*LPDIEFFECTINFOW*/, VdFcREFGUID);
    VdFcHRESULT (__stdcall *GetForceFeedbackState)(VdFc__Win32_IDirectInputDevice8W*, VdFcLPDWORD);
    VdFcHRESULT (__stdcall *SendForceFeedbackCommand)(VdFc__Win32_IDirectInputDevice8W*, VdFcDWORD);
    VdFcHRESULT (__stdcall *EnumCreatedEffectObjects)(VdFc__Win32_IDirectInputDevice8W*, void*/*LPDIENUMCREATEDEFFECTOBJECTSCALLBACK*/, void*, VdFcDWORD);
    VdFcHRESULT (__stdcall *Escape)(VdFc__Win32_IDirectInputDevice8W*, void*/*LPDIEFFESCAPE*/);
    VdFcHRESULT (__stdcall *Poll)(VdFc__Win32_IDirectInputDevice8W*);
    VdFcHRESULT (__stdcall *SendDeviceData)(VdFc__Win32_IDirectInputDevice8W*, VdFcDWORD, void*/*LPCDIDEVICEOBJECTDATA*/, VdFcLPDWORD, VdFcDWORD);
    VdFcHRESULT (__stdcall *EnumEffectsInFile)(VdFc__Win32_IDirectInputDevice8W*, VdFcLPCWSTR, void*/*LPDIENUMEFFECTSINFILECALLBACK*/, void*, VdFcDWORD);
    VdFcHRESULT (__stdcall *WriteEffectToFile)(VdFc__Win32_IDirectInputDevice8W*, VdFcLPCWSTR, VdFcDWORD, void*/*LPDIFILEEFFECT*/, VdFcDWORD);
    VdFcHRESULT (__stdcall *BuildActionMap)(VdFc__Win32_IDirectInputDevice8W*, void*/*LPDIACTIONFORMATW*/, VdFcLPCWSTR, VdFcDWORD);
    VdFcHRESULT (__stdcall *SetActionMap)(VdFc__Win32_IDirectInputDevice8W*, void*/*LPDIACTIONFORMATW*/, VdFcLPCWSTR, VdFcDWORD);
    VdFcHRESULT (__stdcall *GetImageInfo)(VdFc__Win32_IDirectInputDevice8W*, void*/*LPDIDEVICEIMAGEINFOHEADERW*/);
} VdFc__Win32_IDirectInputDevice8WVtbl;
struct VdFc__Win32_IDirectInputDevice8W { const VdFc__Win32_IDirectInputDevice8WVtbl *lpVtbl; };

#define VD_FC__WIN32_DI8DEVCLASS_ALL             0
#define VD_FC__WIN32_DI8DEVCLASS_DEVICE          1
#define VD_FC__WIN32_DI8DEVCLASS_POINTER         2
#define VD_FC__WIN32_DI8DEVCLASS_KEYBOARD        3
#define VD_FC__WIN32_DI8DEVCLASS_GAMECTRL        4

#define VD_FC__WIN32_DIEDFL_ALLDEVICES       0x00000000
#define VD_FC__WIN32_DIEDFL_ATTACHEDONLY     0x00000001
#define VD_FC__WIN32_DIEDFL_FORCEFEEDBACK    0x00000100

#define VD_FC__WIN32_FIELD_OFFSET(type, field)    ((VdFcLONG)(VdFcLONG_PTR)&(((type *)0)->field))
#define VD_FC__WIN32_DIJOFS_X            VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE, lX)
#define VD_FC__WIN32_DIJOFS_Y            VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE, lY)
#define VD_FC__WIN32_DIJOFS_Z            VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE, lZ)
#define VD_FC__WIN32_DIJOFS_RX           VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE, lRx)
#define VD_FC__WIN32_DIJOFS_RY           VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE, lRy)
#define VD_FC__WIN32_DIJOFS_RZ           VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE, lRz)
#define VD_FC__WIN32_DIJOFS_SLIDER(n)   (VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE, rglSlider) + (n) * sizeof(VdFcLONG))
#define VD_FC__WIN32_DIJOFS_POV(n)      (VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE, rgdwPOV) + (n) * sizeof(VdFcDWORD))
#define VD_FC__WIN32_DIJOFS_BUTTON(n)   (VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE, rgbButtons) + (n))
#define VD_FC__WIN32_DIDFT_ALL           0x00000000
#define VD_FC__WIN32_DIDFT_RELAXIS       0x00000001
#define VD_FC__WIN32_DIDFT_ABSAXIS       0x00000002
#define VD_FC__WIN32_DIDFT_AXIS          0x00000003
#define VD_FC__WIN32_DIDFT_PSHBUTTON     0x00000004
#define VD_FC__WIN32_DIDFT_TGLBUTTON     0x00000008
#define VD_FC__WIN32_DIDFT_BUTTON        0x0000000C
#define VD_FC__WIN32_DIDFT_POV           0x00000010
#define VD_FC__WIN32_DIDFT_COLLECTION    0x00000040
#define VD_FC__WIN32_DIDFT_NODATA        0x00000080
#define VD_FC__WIN32_DIDFT_OPTIONAL      0x80000000
#define VD_FC__WIN32_DIDFT_ANYINSTANCE   0x00FFFF00
#define VD_FC__WIN32_DIDOI_FFACTUATOR        0x00000001
#define VD_FC__WIN32_DIDOI_FFEFFECTTRIGGER   0x00000002
#define VD_FC__WIN32_DIDOI_POLLED            0x00008000
#define VD_FC__WIN32_DIDOI_ASPECTPOSITION    0x00000100
#define VD_FC__WIN32_DIDOI_ASPECTVELOCITY    0x00000200
#define VD_FC__WIN32_DIDOI_ASPECTACCEL       0x00000300
#define VD_FC__WIN32_DIDOI_ASPECTFORCE       0x00000400
#define VD_FC__WIN32_DIDOI_ASPECTMASK        0x00000F00
#define VD_FC__WIN32_DIPH_DEVICE             0
#define VD_FC__WIN32_DIPH_BYOFFSET           1
#define VD_FC__WIN32_DIPH_BYID               2
#ifdef __cplusplus
#define VD_FC__WIN32_MAKEDIPROP(prop)    (*(const VdFcGUID *)(prop))
#else
#define VD_FC__WIN32_MAKEDIPROP(prop)    ((VdFcREFGUID)(prop))
#endif

#define VD_FC__WIN32_DIPROP_BUFFERSIZE               VD_FC__WIN32_MAKEDIPROP(1)
#define VD_FC__WIN32_DIPROP_AXISMODE                 VD_FC__WIN32_MAKEDIPROP(2)
#define VD_FC__WIN32_DIPROPAXISMODE_ABS              0
#define VD_FC__WIN32_DIPROPAXISMODE_REL              1
#define VD_FC__WIN32_DIPROP_GRANULARITY              VD_FC__WIN32_MAKEDIPROP(3)
#define VD_FC__WIN32_DIPROP_RANGE                    VD_FC__WIN32_MAKEDIPROP(4)
#define VD_FC__WIN32_DIPROP_DEADZONE                 VD_FC__WIN32_MAKEDIPROP(5)
#define VD_FC__WIN32_DIPROP_SATURATION               VD_FC__WIN32_MAKEDIPROP(6)
#define VD_FC__WIN32_DIPROP_FFGAIN                   VD_FC__WIN32_MAKEDIPROP(7)
#define VD_FC__WIN32_DIPROP_FFLOAD                   VD_FC__WIN32_MAKEDIPROP(8)
#define VD_FC__WIN32_DIPROP_AUTOCENTER               VD_FC__WIN32_MAKEDIPROP(9)
#define VD_FC__WIN32_DIPROPAUTOCENTER_OFF            0
#define VD_FC__WIN32_DIPROPAUTOCENTER_ON             1
#define VD_FC__WIN32_DIPROP_CALIBRATIONMODE          VD_FC__WIN32_MAKEDIPROP(10)
#define VD_FC__WIN32_DIPROPCALIBRATIONMODE_COOKED    0
#define VD_FC__WIN32_DIPROPCALIBRATIONMODE_RAW       1
#define VD_FC__WIN32_DIPROP_CALIBRATION              VD_FC__WIN32_MAKEDIPROP(11)
#define VD_FC__WIN32_DIPROP_GUIDANDPATH              VD_FC__WIN32_MAKEDIPROP(12)
#define VD_FC__WIN32_DIPROP_INSTANCENAME             VD_FC__WIN32_MAKEDIPROP(13)
#define VD_FC__WIN32_DIPROP_PRODUCTNAME              VD_FC__WIN32_MAKEDIPROP(14)
#define VD_FC__WIN32_DIPROP_JOYSTICKID               VD_FC__WIN32_MAKEDIPROP(15)
#define VD_FC__WIN32_DIPROP_GETPORTDISPLAYNAME       VD_FC__WIN32_MAKEDIPROP(16)
#define VD_FC__WIN32_DIPROP_PHYSICALRANGE            VD_FC__WIN32_MAKEDIPROP(18)
#define VD_FC__WIN32_DIPROP_LOGICALRANGE             VD_FC__WIN32_MAKEDIPROP(19)
#define VD_FC__WIN32_DIPROP_KEYNAME                  VD_FC__WIN32_MAKEDIPROP(20)
#define VD_FC__WIN32_DIPROP_CPOINTS                  VD_FC__WIN32_MAKEDIPROP(21)
#define VD_FC__WIN32_DIPROP_APPDATA                  VD_FC__WIN32_MAKEDIPROP(22)
#define VD_FC__WIN32_DIPROP_SCANCODE                 VD_FC__WIN32_MAKEDIPROP(23)
#define VD_FC__WIN32_DIPROP_VIDPID                   VD_FC__WIN32_MAKEDIPROP(24)
#define VD_FC__WIN32_DIPROP_USERNAME                 VD_FC__WIN32_MAKEDIPROP(25)
#define VD_FC__WIN32_DIPROP_TYPENAME                 VD_FC__WIN32_MAKEDIPROP(26)


static VdFc__Win32_DIOBJECTDATAFORMAT Vd_Fc__Win32ObjectDataFormats[] = {
    { &Vd_Fc__Win32_Guid_XAxis,               VD_FC__WIN32_DIJOFS_X,         VD_FC__WIN32_DIDFT_AXIS    |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,VD_FC__WIN32_DIDOI_ASPECTPOSITION },
    { &Vd_Fc__Win32_Guid_YAxis,               VD_FC__WIN32_DIJOFS_Y,         VD_FC__WIN32_DIDFT_AXIS    |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,VD_FC__WIN32_DIDOI_ASPECTPOSITION },
    { &Vd_Fc__Win32_Guid_ZAxis,               VD_FC__WIN32_DIJOFS_Z,         VD_FC__WIN32_DIDFT_AXIS    |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,VD_FC__WIN32_DIDOI_ASPECTPOSITION },
    { &Vd_Fc__Win32_Guid_RxAxis,              VD_FC__WIN32_DIJOFS_RX,        VD_FC__WIN32_DIDFT_AXIS    |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,VD_FC__WIN32_DIDOI_ASPECTPOSITION },
    { &Vd_Fc__Win32_Guid_RyAxis,              VD_FC__WIN32_DIJOFS_RY,        VD_FC__WIN32_DIDFT_AXIS    |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,VD_FC__WIN32_DIDOI_ASPECTPOSITION },
    { &Vd_Fc__Win32_Guid_RzAxis,              VD_FC__WIN32_DIJOFS_RZ,        VD_FC__WIN32_DIDFT_AXIS    |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,VD_FC__WIN32_DIDOI_ASPECTPOSITION },
    { &Vd_Fc__Win32_Guid_Slider,              VD_FC__WIN32_DIJOFS_SLIDER(0), VD_FC__WIN32_DIDFT_AXIS    |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,VD_FC__WIN32_DIDOI_ASPECTPOSITION },
    { &Vd_Fc__Win32_Guid_Slider,              VD_FC__WIN32_DIJOFS_SLIDER(1), VD_FC__WIN32_DIDFT_AXIS    |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,VD_FC__WIN32_DIDOI_ASPECTPOSITION },
    { &Vd_Fc__Win32_Guid_PoV,                 VD_FC__WIN32_DIJOFS_POV(0),    VD_FC__WIN32_DIDFT_POV     |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { &Vd_Fc__Win32_Guid_PoV,                 VD_FC__WIN32_DIJOFS_POV(1),    VD_FC__WIN32_DIDFT_POV     |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { &Vd_Fc__Win32_Guid_PoV,                 VD_FC__WIN32_DIJOFS_POV(2),    VD_FC__WIN32_DIDFT_POV     |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { &Vd_Fc__Win32_Guid_PoV,                 VD_FC__WIN32_DIJOFS_POV(3),    VD_FC__WIN32_DIDFT_POV     |VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(0),     VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(1),     VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(2),     VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(3),     VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(4),     VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(5),     VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(6),     VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(7),     VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(8),     VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(9),     VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(10),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(11),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(12),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(13),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(14),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(15),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(16),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(17),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(18),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(19),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(20),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(21),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(22),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(23),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(24),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(25),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(26),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(27),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(28),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(29),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(30),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
    { NULL,VD_FC__WIN32_DIJOFS_BUTTON(31),    VD_FC__WIN32_DIDFT_BUTTON|     VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_ANYINSTANCE,0 },
};

static VdFc__Win32_DIOBJECTDATAFORMAT Vd_Fc__Win32_DIJOYSTATE2_Data_Formats[] = {
  { &Vd_Fc__Win32_Guid_XAxis,VD_FC__WIN32_DIJOFS_X,VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_YAxis,VD_FC__WIN32_DIJOFS_Y,VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_ZAxis,VD_FC__WIN32_DIJOFS_Z,VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RxAxis,VD_FC__WIN32_DIJOFS_RX,VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RyAxis,VD_FC__WIN32_DIJOFS_RY,VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RzAxis,VD_FC__WIN32_DIJOFS_RZ,VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_Slider,VD_FC__WIN32_DIJOFS_SLIDER(0),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_Slider,VD_FC__WIN32_DIJOFS_SLIDER(1),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_PoV,VD_FC__WIN32_DIJOFS_POV(0),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_POV|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_PoV,VD_FC__WIN32_DIJOFS_POV(1),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_POV|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_PoV,VD_FC__WIN32_DIJOFS_POV(2),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_POV|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_PoV,VD_FC__WIN32_DIJOFS_POV(3),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_POV|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(0),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(1),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(2),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(3),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(4),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(5),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(6),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(7),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(8),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(9),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(10),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(11),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(12),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(13),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(14),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(15),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(16),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(17),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(18),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(19),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(20),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(21),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(22),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(23),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(24),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(25),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(26),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(27),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(28),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(29),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(30),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(31),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(32),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(33),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(34),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(35),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(36),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(37),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(38),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(39),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(40),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(41),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(42),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(43),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(44),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(45),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(46),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(47),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(48),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(49),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(50),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(51),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(52),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(53),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(54),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(55),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(56),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(57),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(58),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(59),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(60),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(61),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(62),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(63),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(64),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(65),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(66),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(67),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(68),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(69),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(70),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(71),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(72),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(73),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(74),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(75),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(76),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(77),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(78),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(79),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(80),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(81),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(82),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(83),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(84),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(85),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(86),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(87),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(88),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(89),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(90),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(91),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(92),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(93),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(94),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(95),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(96),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(97),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(98),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(99),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(100),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(101),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(102),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(103),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(104),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(105),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(106),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(107),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(108),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(109),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(110),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(111),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(112),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(113),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(114),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(115),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(116),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(117),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(118),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(119),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(120),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(121),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(122),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(123),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(124),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(125),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(126),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { NULL,VD_FC__WIN32_DIJOFS_BUTTON(127),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_BUTTON|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_XAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVX),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_YAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVY),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_ZAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVZ),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RxAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVRx),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RyAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVRy),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RzAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVRz),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_Slider,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglVSlider[0]),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_Slider,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglVSlider[1]),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_XAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lAX),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_YAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lAY),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_ZAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lAZ),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RxAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lARx),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RyAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lARy),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RzAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lARz),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_Slider,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglASlider[0]),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_Slider,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglASlider[1]),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_XAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFX),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_YAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFY),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_ZAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFZ),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RxAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFRx),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RyAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFRy),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_RzAxis,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFRz),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_Slider,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglFSlider[0]),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
  { &Vd_Fc__Win32_Guid_Slider,VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglFSlider[1]),VD_FC__WIN32_DIDFT_OPTIONAL|VD_FC__WIN32_DIDFT_AXIS|VD_FC__WIN32_DIDFT_ANYINSTANCE,0},
};

static VdFc__Win32_DIDATAFORMAT Vd_Fc__Win32_DIJOYSTATE2_Data_Format = {
    sizeof(VdFc__Win32_DIDATAFORMAT),
    sizeof(VdFc__Win32_DIOBJECTDATAFORMAT),
    VD_FC__WIN32_DIDFT_ABSAXIS,
    sizeof(VdFc__Win32_DIJOYSTATE2),
    sizeof(Vd_Fc__Win32_DIJOYSTATE2_Data_Formats) / sizeof(Vd_Fc__Win32_DIJOYSTATE2_Data_Formats[0]),
    Vd_Fc__Win32_DIJOYSTATE2_Data_Formats,
};

struct VdFc__Device {
    VdFc__Win32_IDirectInputDevice8W    *dinput_device;
    VdFcGUID                            dinput_instance_guid;
    VdFc__Win32_DIJOYSTATE2             state;
    int                                 present;

    int                                 num_buttons;
    int                                 num_hats;
    int                                 num_axes;

    int                                 num_name;
    char                                name[VD_FC_DEVICE_NAME_MAX];

    VdFcClass                           klass;
};

typedef struct {
    int                                 has_polled_once;

    VdFcHWND                            hwnd;
    VdFc__Win32_IDirectInput8W          *dinput;

    VdFc__Device                        devices[VD_FC_DEVICE_COUNT_MAX];
    int                                 num_devices;

    int                                 num_devices_dropped;
    int                                 devices_dropped[VD_FC_DEVICE_COUNT_MAX];

    int                                 num_devices_arrived;
    int                                 devices_arrived[VD_FC_DEVICE_COUNT_MAX];
} VdFc__Win32InternalData;
static VdFc__Win32InternalData Vd_Fc_G = {0};

static VdFcBOOL           vd_fc__win32_dinput_enum_devices(VdFc__Win32_LPDIDEVICEINSTANCEW inst, void *usr);
static VdFc__Device*      vd_fc__win32_alloc_device(int *id);
static VdFcLRESULT        vd_fc__win32_wndproc(VdFcHWND hwnd, VdFcUINT msg, VdFcWPARAM wparam, VdFcLPARAM lparam);

VD_FC_API void vd_fc_init(void)
{
#define V(dllpath) { HMODULE m = LoadLibraryA(dllpath);
#define _X2(s, e) s##e
#define X(retval, name, params) _X2(VdFc,name) = (_X2(VdFcProc,name))GetProcAddress(m, #name);
#define VE()       }
        VD_FC__WIN32_FUNCTIONS
#undef V
#undef X
#undef VE

    VdFc__Win32_WNDCLASSW wndclass;
    VD_FC_MEMSET(&wndclass, 0, sizeof(wndclass));
    wndclass.hInstance = GetModuleHandleA(NULL);
    wndclass.lpfnWndProc = vd_fc__win32_wndproc;
    wndclass.lpszClassName = L"vd_fc_class";
    VdFcRegisterClassW(&wndclass);

    Vd_Fc_G.hwnd = VdFcCreateWindowExW(0, L"vd_fc_class", L"", 0, 0, 0, 0, 0, 0, 0, GetModuleHandleA(NULL), 0);

    if (VdFcDirectInput8Create) {
        VdFc__Win32_IDirectInput8W *dinput = NULL;
        if (VdFcDirectInput8Create(GetModuleHandleA(NULL), 0x0800, &VdFc__Win32_IID_IDirectInput8W,
                                   (void**)&dinput,
                                   0) == 0)
        {
            Vd_Fc_G.dinput = dinput;
            dinput->lpVtbl->EnumDevices(dinput, VD_FC__WIN32_DI8DEVCLASS_GAMECTRL,
                                        vd_fc__win32_dinput_enum_devices, 0,
                                        VD_FC__WIN32_DIEDFL_ALLDEVICES);

        }
    }
}

VD_FC_API void vd_fc_poll(void)
{
    Vd_Fc_G.num_devices_dropped = 0;
    if (Vd_Fc_G.has_polled_once) {
        Vd_Fc_G.num_devices_arrived = 0;
    }

    // Poll & Check if any device was dropped
    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        VdFc__Device *dev = &Vd_Fc_G.devices[i];
        if (dev->dinput_device) {
            VdFcHRESULT hpoll = dev->dinput_device->lpVtbl->Poll(dev->dinput_device);
            if ((hpoll != 0) && (hpoll != 1)) {
                Vd_Fc_G.devices_dropped[Vd_Fc_G.num_devices_dropped++] = i;
                Vd_Fc_G.devices[i].present = 0;
                VD_FC_LOG("Device %d dropped, hpoll = %d\n", i, hpoll);
            }
        }
    }

    // Reallocate devices
    if (Vd_Fc_G.num_devices_dropped) {
        int write_index = 0;

        for (int read_index = 0; read_index < Vd_Fc_G.num_devices; ++read_index) {
            VdFc__Device *dev = &Vd_Fc_G.devices[read_index];

            if (dev->present) {
                if (write_index != read_index) {
                    Vd_Fc_G.devices[write_index] = Vd_Fc_G.devices[read_index];
                }
                write_index++;
            } else {
                if (dev->dinput_device) {
                    dev->dinput_device->lpVtbl->Unacquire(dev->dinput_device);
                    dev->dinput_device->lpVtbl->Release(dev->dinput_device);
                    dev->dinput_device = NULL;
                }
            }
        }

        Vd_Fc_G.num_devices = write_index;
        VD_FC_LOG("Vd_Fc_G.num_devices = %d", Vd_Fc_G.num_devices);
    }

    VdFcMSG msg;
    while (VdFcPeekMessageW(&msg, NULL, 0u, 0u, 0x0001 /* PM_REMOVE */)) {
        VdFcTranslateMessage(&msg);
        VdFcDispatchMessageW(&msg);
    }

    // Poll Current Devices
    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        VdFc__Device *dev = &Vd_Fc_G.devices[i];
        if (dev->dinput_device) {
            dev->dinput_device->lpVtbl->GetDeviceState(dev->dinput_device,
                                                       sizeof(dev->state), (void*)&dev->state);

        }
    }

    Vd_Fc_G.has_polled_once = 1;
}

VD_FC_API int vd_fc_count(void)
{
    return Vd_Fc_G.num_devices;
}

VD_FC_API int *vd_fc_dropped(int *count)
{
    if (count) {
        *count = Vd_Fc_G.num_devices_dropped;
    }

    return Vd_Fc_G.devices_dropped;
}

VD_FC_API int *vd_fc_arrived(int *count)
{
    if (count) {
        *count = Vd_Fc_G.num_devices_arrived;
    }

    return Vd_Fc_G.devices_arrived;
}

VD_FC_API VdFcType vd_fc_type(int id)
{
    if (id < Vd_Fc_G.num_devices) {
        return VD_FC_TYPE_CONTROLLER;
    } else {
        return VD_FC_TYPE_INVALID;
    }
}

VD_FC_API unsigned int vd_fc_raw_button_down(int id, int button_id)
{
    VdFc__Device *dev = &Vd_Fc_G.devices[id];
    if (button_id < 128) {
        return dev->state.rgbButtons[button_id];
    } else {
        return 0;
    }
}

static VdFcBOOL vd_fc__win32_dinput_enum_devices(VdFc__Win32_LPDIDEVICEINSTANCEW inst, void *usr)
{
    int device_already_acquired = 0;
    (void)usr;

    if (Vd_Fc_G.num_devices == VD_FC_DEVICE_COUNT_MAX) {
        return 0;
    }

    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        if (VD_FC_MEMCMP(&Vd_Fc_G.devices[i].dinput_instance_guid, &inst->guidInstance, sizeof(inst->guidInstance)) == 0) {
            device_already_acquired = 1;
        }
    }

    if (!device_already_acquired) {
        VdFc__Win32_IDirectInputDevice8W *device = NULL;
        if (Vd_Fc_G.dinput->lpVtbl->CreateDevice(Vd_Fc_G.dinput, &inst->guidInstance, &device, NULL) == 0) {

            if (device->lpVtbl->SetDataFormat(device, &Vd_Fc__Win32_DIJOYSTATE2_Data_Format) == 0) {

                VdFc__Win32_DIPROPDWORD property;
                VD_FC_MEMSET(&property, 0, sizeof(property));
                property.diph.dwSize       = sizeof(property);
                property.diph.dwHeaderSize = sizeof(property.diph);
                property.diph.dwHow        = VD_FC__WIN32_DIPH_DEVICE;
                property.dwData            = VD_FC__WIN32_DIPROPAXISMODE_ABS;

                if (device->lpVtbl->SetProperty(device, VD_FC__WIN32_DIPROP_AXISMODE, &property.diph) == 0) {
                    VdFc__Win32_DIDEVCAPS caps;
                    VD_FC_MEMSET(&caps, 0, sizeof(caps));
                    caps.dwSize = sizeof(caps);
                    device->lpVtbl->GetCapabilities(device, &caps);

                    if (device->lpVtbl->Acquire(device) == 0) {
                        int id;
                        VdFc__Device *fc_device  = vd_fc__win32_alloc_device(&id);
                        fc_device->dinput_device = device;
                        fc_device->num_buttons   = caps.dwButtons;
                        fc_device->num_hats      = caps.dwPOVs;
                        fc_device->num_axes      = caps.dwAxes;
                        fc_device->present       = 1;

                        fc_device->num_name = WideCharToMultiByte(65001 /*CP_UTF8*/, 0,
                                                                  inst->tszInstanceName, -1,
                                                                  fc_device->name, VD_FC_DEVICE_NAME_MAX - 1, // null terminator
                                                                  NULL, NULL);
                        if (fc_device->num_name == 0) {
                            VD_FC_MEMCPY(fc_device->name, "Unknown Device", 14);
                            fc_device->num_name = 14;
                        }

                        fc_device->dinput_instance_guid = inst->guidInstance;
                        fc_device->name[fc_device->num_name] = 0;

                        vd_fc_device_classify_auto(fc_device);

                        Vd_Fc_G.devices_arrived[Vd_Fc_G.num_devices_arrived++] = id;

                        VD_FC_LOG("Device initialized: %S %S", inst->tszProductName, inst->tszInstanceName);
                        VD_FC_LOG("           Buttons: %d", caps.dwButtons);
                        VD_FC_LOG("              Hats: %d", caps.dwPOVs);
                        VD_FC_LOG("              Axes: %d", caps.dwAxes);
                        VD_FC_LOG("             class: %s", vd_fc_class_str(fc_device->klass));
                    } else {
                        VD_FC_LOG("Error. Device could not be acquired.");
                    }
                } else {
                    VD_FC_LOG("Error. Failed to SetProperty (AXISMODE -> DIPROPAXISMODE_ABS) for device");
                }
            } else {
                printf("Error. Failed to SetDataFormat data format");
            }
        } else {
            printf("Error. Failed to create device");
        }
    }

    return 1;
}

static VdFc__Device *vd_fc__win32_alloc_device(int *id)
{
    *id = Vd_Fc_G.num_devices;
    Vd_Fc_G.num_devices++;
    return &Vd_Fc_G.devices[*id];
}

static VdFcLRESULT vd_fc__win32_wndproc(VdFcHWND hwnd, VdFcUINT msg, VdFcWPARAM wparam, VdFcLPARAM lparam)
{
    (void)hwnd;
    (void)msg;
    (void)wparam;
    (void)lparam;

    VdFcLRESULT result = 0;
    switch (msg)    {
        case 0x0219 /*WM_DEVICECHANGE*/: {
            Vd_Fc_G.dinput->lpVtbl->EnumDevices(Vd_Fc_G.dinput, VD_FC__WIN32_DI8DEVCLASS_GAMECTRL,
                                                vd_fc__win32_dinput_enum_devices, 0,
                                                VD_FC__WIN32_DIEDFL_ALLDEVICES);
            break;
        }

        default: {
            result = VdFcDefWindowProcW(hwnd, msg, wparam, lparam);
        } break;
    }
    return result;
}

#endif // _WIN32

/* ----CROSS PLATFORM PROCEDURES IMPL-------------------------------------------------------------------------------- */
static int vd_fc_device_classify_auto(VdFc__Device *device)
{
    VdFcClass device_class;
    if (device->num_hats == 0) {
        device_class = VD_FC_CLASS_JOYCON;
    } else {

        if (device->num_buttons < 4) {
            device_class = VD_FC_CLASS_NES;
        } else if (device->num_buttons == 4) {
            device_class = VD_FC_CLASS_MEGADRIVE;
        } else if (device->num_buttons <= 6) {
            device_class = VD_FC_CLASS_GENESIS;
        } else if (device->num_buttons <= 8) {
            device_class = VD_FC_CLASS_SNES;
        } else if (device->num_buttons <= 10) {
            device_class = VD_FC_CLASS_PS1;
        } else {
            if (device->num_axes <= 2) {
                device_class = VD_FC_CLASS_N64;
            } else if (device->num_axes <= 4) {
                device_class = VD_FC_CLASS_PS2;
            } else if (device->num_axes <= 8) {
                device_class = VD_FC_CLASS_XBOX;
            } else {
                // @todo(mdodis): touchpad detection for PS4/Steam Deck
                if (device->num_buttons < 12) {
                    device_class = VD_FC_CLASS_PS4;
                } else {
                    device_class = VD_FC_CLASS_STEAMDECK;
                }
            }
        }
    }

    device->klass = device_class;
    return 1;
}

static const char *vd_fc_class_str(VdFcClass klass)
{
    switch (klass) {
        case VD_FC_CLASS_NES:       return "nes";
        case VD_FC_CLASS_MEGADRIVE: return "megadrive";
        case VD_FC_CLASS_GENESIS:   return "genesis";
        case VD_FC_CLASS_SNES:      return "snes";
        case VD_FC_CLASS_PS1:       return "ps1";
        case VD_FC_CLASS_JOYCON:    return "joycon";
        case VD_FC_CLASS_N64:       return "n64";
        case VD_FC_CLASS_PS2:       return "ps2";
        case VD_FC_CLASS_XBOX:      return "xbox";
        case VD_FC_CLASS_PS4:       return "ps4";
        case VD_FC_CLASS_STEAMDECK: return "steamdeck";
        default:                    return "N/A";
    }
}

VD_FC_API int vd_fc_button_count(int id)
{
    if (id < Vd_Fc_G.num_devices) {
        return Vd_Fc_G.devices[id].num_buttons;
    } else {
        return 0;
    }
}

VD_FC_API int vd_fc_hat_count(int id)
{
    if (id < Vd_Fc_G.num_devices) {
        return Vd_Fc_G.devices[id].num_hats;
    } else {
        return 0;
    }
}

VD_FC_API int vd_fc_axis_count(int id)
{
    if (id < Vd_Fc_G.num_devices) {
        return Vd_Fc_G.devices[id].num_axes;
    } else {
        return 0;
    }
}

#endif // VD_FC_IMPL
