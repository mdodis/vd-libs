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

#ifdef _WIN32
#define XR_OS_WINDOWS 1
#endif

#define OPENXR_RELATIVE_PATH "openxr/"
#define OPENXR_IMPLICIT_API_LAYER_RELATIVE_PATH "/api_layers/implicit.d"
#define OPENXR_EXPLICIT_API_LAYER_RELATIVE_PATH "/api_layers/explicit.d"
#ifdef XR_OS_WINDOWS
#define OPENXR_REGISTRY_LOCATION "SOFTWARE\\Khronos\\OpenXR\\"
#define OPENXR_IMPLICIT_API_LAYER_REGISTRY_LOCATION "\\ApiLayers\\Implicit"
#define OPENXR_EXPLICIT_API_LAYER_REGISTRY_LOCATION "\\ApiLayers\\Explicit"
#endif

// OpenXR Loader environment variables of interest
#define OPENXR_RUNTIME_JSON_ENV_VAR "XR_RUNTIME_JSON"
#define OPENXR_API_LAYER_PATH_ENV_VAR "XR_API_LAYER_PATH"

#define OPENXR_ENABLE_LAYERS_ENV_VAR "XR_ENABLE_API_LAYERS"

static void*    vd_xr__realloc_mem(void *prev_ptr, size_t size);
static void     vd_xr__free_mem(void *ptr);
static void*    vd_xr__resize_buffer(void *buffer, size_t element_size, int required_capacity, int *cap);
static int      vd_xr__strlen(const char *s);

VD_XR_INL void *vd_xr__memcpy(void *dst, void *src, size_t count)
{
    for (size_t i = 0; i < count; ++i) ((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
    return dst;
}

$$xr_generated_dispatch_table.h$$

typedef enum {
    VD_XR__LOADER_PROPERTY_XR_ENABLE_API_LAYERS,
    VD_XR__LOADER_PROPERTY_COUNT
} VdXr__LoaderProperty;

typedef enum {
    VD_XR__MANIFEST_FILE_TYPE_IMPLICIT_API_LAYER,
    VD_XR__MANIFEST_FILE_TYPE_EXPLICIT_API_LAYER,
    VD_XR__MANIFEST_FILE_TYPE_UNDEFINED,
    VD_XR__MANIFEST_FILE_TYPE_RUNTIME,
} VdXr__ManifestFileType;

typedef struct {
    VdXr__ManifestFileType file_type;
} VdXr__ManifestFile;

typedef struct {
    VdXr__ManifestFile *ptr;
    int                len;
    int                cap;
} VdXr__ManifestFileList;

typedef struct {
    char               *ptr;
    int                len;
    int                cap;
} VdXr__StringBuffer;

typedef struct {
    VdXr__StringBuffer buffer;
    int                overriden;
} VdXr__LoaderPropertyStorage;

static void         vd_xr__string_buffer_empty(VdXr__StringBuffer *buf);
static char*        vd_xr__string_buffer_push(VdXr__StringBuffer *buf, const char *str);
static char*        vd_xr__string_buffer_pushu16(VdXr__StringBuffer *buf, uint16_t i);
static void*        vd_xr__string_buffer_pushraw(VdXr__StringBuffer *buf, int required_capacity, int update_len);

static void         vd_xr__manifest_file_list_empty(VdXr__ManifestFileList *list);
static void         vd_xr__manifest_file_list_push(VdXr__ManifestFileList *list, VdXr__ManifestFile *file);
static XrResult     vd_xr__find_manifest_files(VdXr__ManifestFileType type, VdXr__ManifestFileList *list);
static const char*  vd_xr__get_loader_property(VdXr__LoaderProperty property);

#if XR_OS_WINDOWS
static int          vd_xr__utf8_to_wide(char *buf, int blen, wchar_t *wbuf, int wlen);
static int          vd_xr__wide_to_utf8(wchar_t *wbuf, int wlen, char *buf, int blen);
#endif

typedef struct {
    XrGeneratedDispatchTable        *dispatch_table;
    VdXr__ManifestFileList          manbuf;
    VdXr__StringBuffer              strbuf;
    VdXr__LoaderPropertyStorage     properties[VD_XR__LOADER_PROPERTY_COUNT];
} VdXr__InternalData;

static VdXr__InternalData Vd_Xr_G;

$$xr_generated_loader.cpp$$
$$xr_generated_dispatch_table.c$$

#ifdef _WIN32

#pragma pack(push, 1)
/* ----WIN32 BASE---------------------------------------------------------------------------------------------------- */
#ifndef _MINWINDEF_
#define VD_FT_DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

typedef unsigned long        VdXrDWORD;
typedef int                  VdXrBOOL;
typedef unsigned char        VdXrBYTE;
typedef unsigned short       VdXrWORD;
typedef float                VdXrFLOAT;
typedef VdXrFLOAT*          VdXrPFLOAT;
typedef VdXrBOOL*           VdXrPBOOL;
typedef VdXrBOOL*           VdXrLPBOOL;
typedef VdXrBYTE*           VdXrPBYTE;
typedef VdXrBYTE*           VdXrLPBYTE;
typedef int*                 VdXrPINT;
typedef int*                 VdXrLPINT;
typedef VdXrWORD*           VdXrPWORD;
typedef VdXrWORD*           VdXrLPWORD;
typedef long*                VdXrLPLONG;
typedef VdXrDWORD*          VdXrPDWORD;
typedef VdXrDWORD*          VdXrLPDWORD;
typedef void*                VdXrLPVOID;
typedef const void*          VdXrLPCVOID;
typedef int                  VdXrINT;
typedef unsigned int         VdXrUINT;
typedef unsigned int*        VdXrPUINT;
typedef uint64_t            VdXrUINT64;
typedef uint32_t            VdXrUINT32;
typedef int32_t             VdXrINT32;
typedef uint16_t            VdXrUINT16;
typedef uint8_t             VdXrUINT8;
typedef int16_t             VdXrINT16;
typedef void*               VdXrHANDLE;
typedef VdXrHANDLE*         VdXrPHANDLE;
typedef char                VdXrCHAR;
typedef const VdXrCHAR*     VdXrLPCSTR, *VdXrPCSTR;
typedef VdXrCHAR*           VdXrLPSTR;
typedef long                VdXrLONG;
typedef unsigned long       VdXrULONG;
typedef VdXrULONG*          VdXrPULONG;
typedef unsigned short      VdXrUSHORT;
typedef VdXrUSHORT*         VdXrPUSHORT;
typedef unsigned char       VdXrUCHAR;
typedef VdXrUCHAR*          VdXrPUCHAR;
typedef VdXrBYTE            VdXrBOOLEAN;
typedef VdXrCHAR*           VdXrPCHAR, * VdXrLPCH, * VdXrPCH;
typedef long                VdXrHRESULT;
typedef __int64             VdXrLONGLONG;
typedef unsigned __int64    VdXrULONGLONG;
typedef VdXrULONGLONG       VdXrDWORDLONG;
typedef VdXrDWORDLONG*      VdXrPDWORDLONG;
typedef __int64             VdXrINT_PTR, * VdXrPINT_PTR;
typedef unsigned __int64    VdXrUINT_PTR, * VdXrPUINT_PTR;
typedef __int64             VdXrLONG_PTR, * VdXrPLONG_PTR;
typedef unsigned __int64    VdXrULONG_PTR, * VdXrPULONG_PTR;
typedef VdXrUINT_PTR        VdXrWPARAM;
typedef VdXrLONG_PTR        VdXrLPARAM;
typedef VdXrLONG_PTR        VdXrLRESULT;
typedef wchar_t             VdXrWCHAR;
typedef const VdXrWCHAR*    VdXrLPCWSTR, * VdXrPCWSTR;
typedef VdXrWCHAR*          VdXrLPWSTR;
typedef VdXrWORD            VdXrATOM;
typedef VdXrULONG_PTR       VdXrDWORD_PTR, * VdXrPDWORD_PTR;
typedef VdXrULONG_PTR       VdXrSIZE_T;
typedef short               VdXrSHORT;
typedef VdXrDWORD           VdXrCOLORREF;
typedef VdXrDWORD*          VdXrLPCOLORREF;

VD_FT_DECLARE_HANDLE(VdXrHWND);
VD_FT_DECLARE_HANDLE(VdXrHINSTANCE);
VD_FT_DECLARE_HANDLE(VdXrHDC);
VD_FT_DECLARE_HANDLE(VdXrHMONITOR);
VD_FT_DECLARE_HANDLE(VdXrHGDIOBJ);
VD_FT_DECLARE_HANDLE(VdXrHBITMAP);
typedef VdXrHINSTANCE VdXrHMODULE;

extern VdXrHMODULE LoadLibraryA(VdXrLPCSTR path);
extern void*       HeapAlloc(VdXrHANDLE hHeap, VdXrDWORD dwFlags, VdXrSIZE_T dwBytes);
extern VdXrHANDLE  GetProcessHeap();
extern void*       HeapReAlloc(VdXrHANDLE hHeap, VdXrDWORD dwFlags, void *lpMem, VdXrSIZE_T dwBytes);
extern VdXrBOOL    HeapFree(VdXrHANDLE hHeap, VdXrDWORD dwFlags, void *lpMem);
extern int         MultiByteToWideChar(VdXrUINT CodePage, VdXrDWORD dwFlags, VdXrLPCSTR lpMultiByteStr, int cbMultiByte, VdXrLPWSTR lpWideCharStr, int cchWideChar);
extern int         WideCharToMultiByte(VdXrUINT CodePage, VdXrDWORD dwFlags, VdXrLPCWSTR lpWideCharStr, int cchWideChar, VdXrLPSTR lpMultiByteStr, int cbMultiByte, VdXrLPSTR lpDefaultChar, VdXrBOOL *lpUsedDefaultChar);
extern void*       GetProcAddress(VdXrHMODULE hModule, VdXrLPCSTR lpProcName);
extern VdXrDWORD   GetEnvironmentVariableW(VdXrLPCWSTR lpName, VdXrLPWSTR lpBuffer, VdXrDWORD nSize);

#else
typedef HWND        VdXrHWND;
typedef DWORD       VdXrDWORD;
typedef WORD        VdXrWORD;
typedef LPWSTR      VdXrLPWSTR;
typedef LPCWSTR     VdXrLPCWSTR;
typedef LPCSTR      VdXrLPCSTR;
typedef DWORD       VdXrDWORD;
typedef UINT        VdXrUINT;
typedef UINT64      VdXrUINT64;
typedef UINT32      VdXrUINT32;
typedef INT32       VdXrINT32;
typedef UINT16      VdXrUINT16;
typedef UINT8       VdXrUINT8;
typedef INT16       VdXrINT16;
typedef HMODULE     VdXrHMODULE;
typedef HRESULT     VdXrHRESULT;
typedef ULONG       VdXrULONG;
typedef HDC         VdXrHDC;
typedef HMONITOR    VdXrHMONITOR;
typedef HANDLE      VdXrHANDLE;
typedef HGDIOBJ     VdXrHGDIOBJ;
typedef HBITMAP     VdXrHBITMAP;
typedef LPARAM      VdXrLPARAM;
typedef BOOL        VdXrPBOOL;
typedef LONG        VdXrLONG;
typedef BYTE        VdXrBYTE;
typedef WCHAR       VdXrWCHAR;
typedef BOOL        VdXrBOOL;
typedef LPVOID      VdXrLPVOID;
typedef FLOAT       VdXrFLOAT;
typedef VdXrDWORD   VdXrCOLORREF;
typedef VdXrDWORD*  VdXrLPCOLORREF;
#endif // !_MINWINDEF_
#pragma pack(pop)

#if XR_OS_WINDOWS
static int vd_xr__utf8_to_wide(char *buf, int blen, wchar_t *wbuf, int wlen)
{
    return MultiByteToWideChar(65001, 8, buf, blen, wbuf, wlen);
}

static int vd_xr__wide_to_utf8(wchar_t *wbuf, int wlen, char *buf, int blen)
{
    return WideCharToMultiByte(65001, 0, wbuf, wlen, buf, blen, NULL, NULL);
}
#endif

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
    HeapFree(GetProcessHeap(), 0, ptr);
}
#endif // _WIN32

static void vd_xr__string_buffer_empty(VdXr__StringBuffer *buf)
{
    buf->len = 0;
}

static char *vd_xr__string_buffer_push(VdXr__StringBuffer *buf, const char *str)
{
    int str_len = vd_xr__strlen(str);
    int required_capacity = buf->len + str_len + 1;
    buf->ptr = (char*)vd_xr__resize_buffer(buf->ptr, sizeof(char), required_capacity, &buf->cap);

    vd_xr__memcpy(buf->ptr + buf->len, str, str_len);
    buf->len += str_len;
    buf->ptr[buf->len] = '\0';
    return buf->ptr;
}

static void *vd_xr__string_buffer_pushraw(VdXr__StringBuffer *buf, int required_capacity, int update_len)
{
    required_capacity += buf->len;
    buf->ptr = (char*)vd_xr__resize_buffer(buf->ptr, sizeof(char), required_capacity, &buf->cap);

    void *result = (void*)(buf->ptr + buf->len);

    if (update_len) {
        buf->len += required_capacity;
    }

    return result;
}

static char *vd_xr__string_buffer_pushu16(VdXr__StringBuffer *buf, uint16_t i)
{
    char ibuf[6] = {0};

    int c = 0;
    while (i > 0) {
        ibuf[c] = (i % 10);
        i = i / 10;
        c++;
    }

    return vd_xr__string_buffer_push(buf, ibuf);
}

static void vd_xr__manifest_file_list_empty(VdXr__ManifestFileList *list)
{
    list->len = 0;    
}

static void vd_xr__manifest_file_list_push(VdXr__ManifestFileList *list, VdXr__ManifestFile *file)
{
    int required_capacity = list->len + 1;
    list->ptr = (VdXr__ManifestFile*)vd_xr__resize_buffer(list->ptr, sizeof(*list->ptr), required_capacity, &list->cap);

    list->ptr[list->len++] = *file;
}

static XrResult vd_xr__find_manifest_files(VdXr__ManifestFileType type, VdXr__ManifestFileList *list)
{
    VdXr__StringBuffer *strbuf = &Vd_Xr_G.strbuf;
    vd_xr__string_buffer_empty(strbuf);
    vd_xr__string_buffer_push(strbuf, OPENXR_RELATIVE_PATH);
    vd_xr__string_buffer_pushu16(strbuf, XR_VERSION_MAJOR(XR_CURRENT_API_VERSION));
    const char *override_env_var = 0;
#ifdef XR_OS_WINDOWS
    const char *registry_location = 0;
#endif

    switch (type) {
        case VD_XR__MANIFEST_FILE_TYPE_IMPLICIT_API_LAYER: {
            vd_xr__string_buffer_push(strbuf, OPENXR_IMPLICIT_API_LAYER_RELATIVE_PATH);
            override_env_var = "";
#ifdef XR_OS_WINDOWS
            registry_location = OPENXR_IMPLICIT_API_LAYER_REGISTRY_LOCATION;
#endif
        } break;

        case VD_XR__MANIFEST_FILE_TYPE_EXPLICIT_API_LAYER: {
            vd_xr__string_buffer_push(strbuf, OPENXR_EXPLICIT_API_LAYER_RELATIVE_PATH);
            override_env_var = "";
#ifdef XR_OS_WINDOWS
            registry_location = OPENXR_EXPLICIT_API_LAYER_REGISTRY_LOCATION;
#endif
        } break;

        case VD_XR__MANIFEST_FILE_TYPE_UNDEFINED:
        case VD_XR__MANIFEST_FILE_TYPE_RUNTIME:
        default: {
            return XR_ERROR_FILE_ACCESS_ERROR;
        } break;
    }

    // Find data files in search paths
    {

    }
}

static const char *vd_xr__get_loader_property(VdXr__LoaderProperty property)
{
    const char *env_var_name = 0;
    int env_var_name_len = 0;

    switch (property) {
        case VD_XR__LOADER_PROPERTY_XR_ENABLE_API_LAYERS: {
            env_var_name = OPENXR_ENABLE_LAYERS_ENV_VAR;
            env_var_name_len = sizeof(OPENXR_ENABLE_LAYERS_ENV_VAR) - 1;
        } break;
        default: return 0;
    }

    VdXr__LoaderPropertyStorage *storage = &Vd_Xr_G.properties[property];

#ifdef XR_OS_WINDOWS
    wchar_t buf[128];

    int buf_end = vd_xr__utf8_to_wide(env_var_name, env_var_name_len, buf, sizeof(buf));
    if (buf_end <= 0) {
        return 0;
    }

    buf[buf_end] = 0;

    VdXrDWORD num_wchars_required = GetEnvironmentVariableW(buf, 0, 0);

    // @todo(mdodis): Temp arena on internals
    static VdXr__StringBuffer wstrbuf = {0};
    wchar_t *woutbuf = (wchar_t*)vd_xr__string_buffer_pushraw(&wstrbuf, num_wchars_required * sizeof(wchar_t), 1);

    VdXrDWORD num_wchars_written = GetEnvironmentVariableW(buf, woutbuf, num_wchars_required);

    if (num_wchars_required != num_wchars_written) {
        return 0;
    }

    int num_chars_required = vd_xr__wide_to_utf8(woutbuf, num_wchars_written, 0, 0);
    vd_xr__string_buffer_empty(&storage->buffer);
    char *result_buf = vd_xr__string_buffer_pushraw(&storage->buffer, num_chars_required + 1, 1);

    int num_chars_written = vd_xr__wide_to_utf8(woutbuf, num_wchars_written, result_buf, num_chars_required + 1);

    if (num_chars_required != num_chars_written) {
        return 0;
    }

    result_buf[num_chars_written] = 0;

#else
#error "Unsupported Platform!"
#endif

    return storage->buffer.ptr;
}

XRAPI_ATTR XrResult XRAPI_CALL xrEnumerateApiLayerProperties(uint32_t propertyCapacityInput,
                                                             uint32_t *propertyCountOutput,
                                                             XrApiLayerProperties *properties)
{
    // @todo(mdodis): implementation
    propertyCountOutput = 0;
    return XR_SUCCESS;
}

XRAPI_ATTR XrResult XRAPI_CALL xrEnumerateInstanceExtensionProperties(const char *layerName,
                                                                      uint32_t propertyCapacityInput,
                                                                      uint32_t *propertyCountOutput,
                                                                      XrExtensionProperties *properties)
{
    VdXr__ManifestFileList *manifest_files = &Vd_Xr_G.manbuf;

    if (propertyCountOutput == 0)
    {
        return XR_ERROR_VALIDATION_FAILURE;
    }

    int just_layer_props = 0;

    if ((layerName == 0) && vd_xr__strlen(layerName) == 0) {
        just_layer_props = 1;
    }

    if (just_layer_props) {

    } else {
        XrResult result = vd_xr__find_manifest_files(VD_XR__MANIFEST_FILE_TYPE_IMPLICIT_API_LAYER, manifest_files);
        if (XR_SUCCEEDED(result)) {

        }
    }

}

static void *vd_xr__resize_buffer(void *buffer, size_t element_size, int required_capacity, int *cap)
{
    if (required_capacity <= *cap) {
        return buffer;
    }

    int resize_capacity = required_capacity * 2;
    buffer = vd_xr__realloc_mem(buffer, element_size * resize_capacity);
    *cap = resize_capacity;
    return buffer;
}


static int vd_xr__strlen(const char *s)
{
    int r = 0;
    while (*s++) r++;
    return r;
}



#endif // VD_XR_IMPL