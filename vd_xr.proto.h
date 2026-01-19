/* vd_xr.h - OpenXR Loader
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
#ifndef VD_XR_H
#define VD_XR_H
#define VD_XR_VERSION_MAJOR    0
#define VD_XR_VERSION_MINOR    0
#define VD_XR_VERSION_PATCH    1
#define VD_XR_VERSION          ((VD_XR_VERSION_MAJOR << 16) | (VD_XR_VERSION_MINOR << 8) | (VD_XR_VERSION_PATCH))

#ifndef VD_XR_API
#   ifdef VD_XR_STATIC
#       define VD_XR_API static
#   else
#       define VD_XR_API extern
#   endif // VD_XR_STATIC
#endif // !VD_XR_API

#ifndef VD_XR_INL
#   define VD_XR_INL static inline
#endif // !VD_XR_INL

#include <stdint.h>
$$openxr.h$$

#endif // !VD_XR_H

#ifdef VD_XR_IMPL

static void*    vd_xr__realloc_mem(void *prev_ptr, size_t size);
static void     vd_xr__free_mem(void *ptr);
static void*    vd_xr__resize_buffer(void *buffer, size_t element_size, int required_capacity, int *cap);

$$xr_generated_dispatch_table.h$$

typedef struct {
    XrGeneratedDispatchTable *dispatch_table;
} Vd_Xr__InternalData;

static Vd_Xr__InternalData Vd_Xr_G;

$$xr_generated_loader.cpp$$
$$xr_generated_dispatch_table.c$$

typedef enum {
    VD_XR__MANIFEST_FILE_TYPE_IMPLICIT_API_LAYER,
    VD_XR__MANIFEST_FILE_TYPE_EXPLICIT_API_LAYER,
} VdXr__ManifestFileType;

XRAPI_ATTR XrResult XRAPI_CALL xrEnumerateApiLayerProperties(uint32_t propertyCapacityInput,
                                                             uint32_t *propertyCountOutput,
                                                             XrApiLayerProperties *properties)
{
    list_manifest_files()
}

static void *vd_xr__resize_buffer(void *buffer, size_t element_size, int required_capacity, int *cap)
{
    if (required_capacity <= *cap) {
        return buffer;
    }

    int resize_capacity = required_capacity * 2;
    buffer = vd_ft__realloc_mem(buffer, element_size * resize_capacity);
    *cap = resize_capacity;
    return buffer;
}

#ifdef _WIN32

#pragma pack(push, 1)
/* ----WIN32 BASE---------------------------------------------------------------------------------------------------- */
#ifndef _MINWINDEF_
#define VD_FT_DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

typedef unsigned long        VdFtDWORD;
typedef int                  VdFtBOOL;
typedef unsigned char        VdFtBYTE;
typedef unsigned short       VdFtWORD;
typedef float                VdFtFLOAT;
typedef VdFtFLOAT*          VdFtPFLOAT;
typedef VdFtBOOL*           VdFtPBOOL;
typedef VdFtBOOL*           VdFtLPBOOL;
typedef VdFtBYTE*           VdFtPBYTE;
typedef VdFtBYTE*           VdFtLPBYTE;
typedef int*                 VdFtPINT;
typedef int*                 VdFtLPINT;
typedef VdFtWORD*           VdFtPWORD;
typedef VdFtWORD*           VdFtLPWORD;
typedef long*                VdFtLPLONG;
typedef VdFtDWORD*          VdFtPDWORD;
typedef VdFtDWORD*          VdFtLPDWORD;
typedef void*                VdFtLPVOID;
typedef const void*          VdFtLPCVOID;
typedef int                  VdFtINT;
typedef unsigned int         VdFtUINT;
typedef unsigned int*        VdFtPUINT;
typedef uint64_t            VdFtUINT64;
typedef uint32_t            VdFtUINT32;
typedef int32_t             VdFtINT32;
typedef uint16_t            VdFtUINT16;
typedef uint8_t             VdFtUINT8;
typedef int16_t             VdFtINT16;
typedef void*               VdFtHANDLE;
typedef VdFtHANDLE*         VdFtPHANDLE;
typedef char                VdFtCHAR;
typedef const VdFtCHAR*     VdFtLPCSTR, *VdFtPCSTR;
typedef VdFtCHAR*           VdFtLPSTR;
typedef long                VdFtLONG;
typedef unsigned long       VdFtULONG;
typedef VdFtULONG*          VdFtPULONG;
typedef unsigned short      VdFtUSHORT;
typedef VdFtUSHORT*         VdFtPUSHORT;
typedef unsigned char       VdFtUCHAR;
typedef VdFtUCHAR*          VdFtPUCHAR;
typedef VdFtBYTE            VdFtBOOLEAN;
typedef VdFtCHAR*           VdFtPCHAR, * VdFtLPCH, * VdFtPCH;
typedef long                VdFtHRESULT;
typedef __int64             VdFtLONGLONG;
typedef unsigned __int64    VdFtULONGLONG;
typedef VdFtULONGLONG       VdFtDWORDLONG;
typedef VdFtDWORDLONG*      VdFtPDWORDLONG;
typedef __int64             VdFtINT_PTR, * VdFtPINT_PTR;
typedef unsigned __int64    VdFtUINT_PTR, * VdFtPUINT_PTR;
typedef __int64             VdFtLONG_PTR, * VdFtPLONG_PTR;
typedef unsigned __int64    VdFtULONG_PTR, * VdFtPULONG_PTR;
typedef VdFtUINT_PTR        VdFtWPARAM;
typedef VdFtLONG_PTR        VdFtLPARAM;
typedef VdFtLONG_PTR        VdFtLRESULT;
typedef wchar_t             VdFtWCHAR;
typedef const VdFtWCHAR*    VdFtLPCWSTR, * VdFtPCWSTR;
typedef VdFtWCHAR*          VdFtLPWSTR;
typedef VdFtWORD            VdFtATOM;
typedef VdFtULONG_PTR       VdFtDWORD_PTR, * VdFtPDWORD_PTR;
typedef VdFtULONG_PTR       VdFtSIZE_T;
typedef short               VdFtSHORT;
typedef VdFtDWORD           VdFtCOLORREF;
typedef VdFtDWORD*          VdFtLPCOLORREF;

VD_FT_DECLARE_HANDLE(VdFtHWND);
VD_FT_DECLARE_HANDLE(VdFtHINSTANCE);
VD_FT_DECLARE_HANDLE(VdFtHDC);
VD_FT_DECLARE_HANDLE(VdFtHMONITOR);
VD_FT_DECLARE_HANDLE(VdFtHGDIOBJ);
VD_FT_DECLARE_HANDLE(VdFtHBITMAP);
typedef VdFtHINSTANCE VdFtHMODULE;

extern VdFtHMODULE LoadLibraryA(VdFtLPCSTR path);
extern void*       HeapAlloc(VdFtHANDLE hHeap, VdFtDWORD dwFlags, VdFtSIZE_T dwBytes);
extern VdFtHANDLE  GetProcessHeap();
extern void*       HeapReAlloc(VdFtHANDLE hHeap, VdFtDWORD dwFlags, void *lpMem, VdFtSIZE_T dwBytes);
extern VdFtBOOL    HeapFree(VdFtHANDLE hHeap, VdFtDWORD dwFlags, void *lpMem);
extern void*       GetProcAddress(VdFtHMODULE hModule, VdFtLPCSTR lpProcName);

#else
typedef HWND        VdFtHWND;
typedef DWORD       VdFtDWORD;
typedef WORD        VdFtWORD;
typedef LPWSTR      VdFtLPWSTR;
typedef LPCWSTR     VdFtLPCWSTR;
typedef LPCSTR      VdFtLPCSTR;
typedef DWORD       VdFtDWORD;
typedef UINT        VdFtUINT;
typedef UINT64      VdFtUINT64;
typedef UINT32      VdFtUINT32;
typedef INT32       VdFtINT32;
typedef UINT16      VdFtUINT16;
typedef UINT8       VdFtUINT8;
typedef INT16       VdFtINT16;
typedef HMODULE     VdFtHMODULE;
typedef HRESULT     VdFtHRESULT;
typedef ULONG       VdFtULONG;
typedef HDC         VdFtHDC;
typedef HMONITOR    VdFtHMONITOR;
typedef HANDLE      VdFtHANDLE;
typedef HGDIOBJ     VdFtHGDIOBJ;
typedef HBITMAP     VdFtHBITMAP;
typedef LPARAM      VdFtLPARAM;
typedef BOOL        VdFtPBOOL;
typedef LONG        VdFtLONG;
typedef BYTE        VdFtBYTE;
typedef WCHAR       VdFtWCHAR;
typedef BOOL        VdFtBOOL;
typedef LPVOID      VdFtLPVOID;
typedef FLOAT       VdFtFLOAT;
typedef VdFtDWORD   VdFtCOLORREF;
typedef VdFtDWORD*  VdFtLPCOLORREF;
#endif // !_MINWINDEF_

static void *vd_xr__realloc_mem(void *prev_ptr, size_t size)
{
    if (prev_ptr == 0) {
        return HeapAlloc(GetProcessHeap(), 0, size);
    } else {
        return HeapReAlloc(GetProcessHeap(), 0, prev_ptr, size);
    }
}

static void vd_xr__free_mem(void *ptr)
{
    HeapFree(GetProcessHeap(), 0, memory);
}
#endif // _WIN32

#endif // VD_XR_IMPL