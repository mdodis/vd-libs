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

#ifndef VD_XR_ABORT
#   include <assert.h>
#   define VD_XR_ABORT(message) do { assert(0 && message); *(char*)0 = *message; } while(0)
#endif // !VD_XR_ABORT

#ifndef VD_XR_ASSERTIONS
#   define VD_XR_ASSERTIONS 0
#endif // !VD_XR_ASSERTIONS

#if VD_XR_ASSERTIONS
#   define VD_XR_ASSERT(x) do { if (!(x)) { VD_XR_ABORT("Assertion Failed: " #x " At: " __FILE__ ":" #__LINE__ ); } } while (0);
#else
#   define VD_XR_ASSERT(x) (x)
#endif // VD_XR_ASSERTIONS

#ifdef _WIN32
#define XR_OS_WINDOWS 1
#endif

#ifndef XR_OS_WINDOWS
#   define XR_OS_WINDOWS 0
#endif // !XR_OS_WINDOWS

#ifndef XR_OS_ANDROID
#   define XR_OS_ANDROID 0
#endif // !XR_OS_ANDROID

#ifndef XR_OS_APPLE
#   define XR_OS_APPLE 0
#endif // !XR_OS_APPLE

#ifndef XR_OS_LINUX
#   define XR_OS_LINUX 0
#endif // !XR_OS_LINUX

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

#if defined(XR_OS_LINUX) || defined(XR_OS_APPLE) || defined(XR_OS_ANDROID)
#define VD_XR__PATH_SEPARATOR ':'
#define VD_XR__DIRECTORY_SYMBOL '/'
#elif defined(XR_OS_WINDOWS)
#define VD_XR__PATH_SEPARATOR ';'
#define VD_XR__DIRECTORY_SYMBOL '\\'
#else
#define VD_XR__PATH_SEPARATOR ':'
#define VD_XR__DIRECTORY_SYMBOL '/'
#endif

typedef uint8_t VdXr__ArenaFlags;

static void*    vd_xr__realloc_mem(void *prev_ptr, size_t size);
static void     vd_xr__free_mem(void *ptr);
static void*    vd_xr__resize_buffer(void *buffer, size_t element_size, int required_capacity, int *cap);
static int      vd_xr__strlen(const char *s);
static int      vd_xr__strncmp(const char *s, const char *t, int count);
static int      vd_xr__str_first_of(const char *s, char c);
static int      vd_xr__str_first_of_with_start(const char *s, char c, int start);

VD_XR_INL void *vd_xr__memset(void *dest, int value, size_t num)
{
    for (size_t i = 0; i < num; ++i) ((uint8_t*)dest)[i] = (uint8_t)value;
    return dest;
}

VD_XR_INL void *vd_xr__memcpy(void *dst, void *src, size_t count)
{
    for (size_t i = 0; i < count; ++i) ((unsigned char*)dst)[i] = ((unsigned char*)src)[i];
    return dst;
}

VD_XR_INL void *vd_xr__memmove(void *dest, void *src, size_t num) {
    uint8_t* d = (uint8_t*)dest;
    uint8_t* s = (uint8_t*)src;

    if (d < s) {
        for (size_t i = 0; i < num; ++i) {
            d[i] = s[i];
        }
    } else if (s < d) {
        for (size_t i = num; i > 0; --i) {
            d[i - 1] = s[i - 1];
        }
    }

    return d;
}

VD_XR_INL int vd_xr__is_power_of_two(size_t x) {
    return (x & (x - 1)) == 0;
}

VD_XR_INL uintptr_t vd_xr__align_forward(uintptr_t ptr, size_t align) {
    VD_XR_ASSERT(vd_xr__is_power_of_two(align));

    uintptr_t p, a, modulo;

    p = ptr;
    a = (uintptr_t)align;
    modulo = p & (a - 1);

    if (modulo != 0) {
        p += a - modulo;
    }

    return p;
}

typedef struct VdXr__Arena {
    uint8_t             *buf;
    size_t              buf_len;
    size_t              prev_offset;
    size_t              curr_offset;
    VdXr__ArenaFlags    flags;
    uint8_t             reserved[7];
} VdXr__Arena;

typedef struct __VD_ArenaSave {
    /** The saved arena. */
    VdXr__Arena     *arena;
    /** The previous offset in the arena. */
    size_t          prev_offset;
    /** The current offset in the arena. */
    size_t          curr_offset;
} VdXr__ArenaSave;


void                        vd_xr__arena_init(VdXr__Arena *a, void *buf, size_t len);
void*                       vd_xr__arena_alloc_align(VdXr__Arena *a, size_t size, size_t align);
void*                       vd_xr__arena_resize_align(VdXr__Arena *a, void *old_memory, size_t old_size, size_t new_size, size_t align);
void                        vd_xr__arena_clear(VdXr__Arena *a);
int                         vd_xr__arena_free(VdXr__Arena *a, void *memory, size_t size);
VD_XR_INL VdXr__ArenaSave   vd_xr__arena_save(VdXr__Arena *a)                                                       { VdXr__ArenaSave result = { a, a->prev_offset, a->curr_offset }; return result; }
VD_XR_INL void              vd_xr__arena_restore(VdXr__ArenaSave save)                                              { save.arena->prev_offset = save.prev_offset; save.arena->curr_offset = save.curr_offset; }
VD_XR_INL void*             vd_xr__arena_alloc(VdXr__Arena *a, size_t size)                                         { return vd_xr__arena_alloc_align(a, size, 8);}
VD_XR_INL void*             vd_xr__arena_resize(VdXr__Arena *a, void *old_memory, size_t old_size, size_t new_size) { return vd_xr__arena_resize_align(a, old_memory, old_size, new_size, 8); }
VD_XR_INL VdXr__Arena       vd_xr__arena_from_malloc(size_t size)                                               { VdXr__Arena result; vd_xr__arena_init(&result, vd_xr__realloc_mem(0, size), size); return result; }

#define VD_XR__ARENA_PUSH_ARRAY(a, x, count) (x*)vd_xr__arena_alloc(a, sizeof(x) * count)
#define VD_XR__ARENA_PUSH_STRUCT(a, x)       VD_XR__ARENA_PUSH_ARRAY(a, x, 1)
#define VD_XR__ARENA_FROM_SYSTEM(a, size)    (vd_xr__arena_init(a, VD_MALLOC(size), size))

typedef struct {
    uint32_t     len;
    uint32_t     cap;
    VdXr__Arena  *arena;
} VdXr__DynArrayHeader;

#define VD_XR__DYNARRAY_HEADER(a)                      ((VdXr__DynArrayHeader*)(((uint8_t*)a) - sizeof(VdXr__DynArrayHeader)))
#define VD_XR__DYNARRAY_INIT(a, arena)                 ((a) = vd__dynarray_grow(a, sizeof(*(a)), 1, 0, arena))
#define VD_XR__DYNARRAY_INIT_WITH_CAP(a, arena, cap)   ((a) = vd__dynarray_grow(a, sizeof(*(a)), cap, cap, arena))
#define VD_XR__DYNARRAY_ADD(a, v)                      (VD_XR__DYNARRAY_CHECKGROW(a, 1), (a)[VD_XR__DYNARRAY_HEADER(a)->len++] = (v))
#define VD_XR__DYNARRAY_PUSH(a)                        (VD_XR__DYNARRAY_CHECKGROW(a, 1), &((a)[VD_XR__DYNARRAY_HEADER(a)->len++]))
#define VD_XR__DYNARRAY_ADDN(a, n)                     (VD_XR__DYNARRAY_CHECKGROW(a, n), VD_XR__DYNARRAY_HEADER(a)->len += (n))
#define VD_XR__DYNARRAY_CLEAR(a)                       ((a) ? VD_XR__DYNARRAY_HEADER(a)->len = 0 : 0)
#define VD_XR__DYNARRAY_POP(a)                         (VD_XR__DYNARRAY_HEADER(a)->len--, (a)[VD_XR__DYNARRAY_HEADER(a)->len])
#define VD_XR__DYNARRAY_LAST(a)                        ((a)[VD_XR__DYNARRAY_HEADER(a)->len - 1])
#define VD_XR__DYNARRAY_LEN(a)                         ((a) ? VD_XR__DYNARRAY_HEADER(a)->len : 0)
#define VD_XR__DYNARRAY_CAP(a)                         ((a) ? VD_XR__DYNARRAY_HEADER(a)->cap : 0)
#define VD_XR__DYNARRAY_DEL(a, i)                      do { uint32_t _i_ = (i); if (_i_ == (VD_XR__DYNARRAY_LEN(a) - 1)) { (VD_XR__DYNARRAY_HEADER(a)->len--); } else {  (a)[_i_] = VD_XR__DYNARRAY_LAST(a); (VD_XR__DYNARRAY_HEADER(a)->len--); } } while (0)
#define VD_XR__DYNARRAY_ARENAP(a)                      ((a) ? VD_XR__DYNARRAY_HEADER(a)->arena : 0)
#define VD_XR__DYNARRAY_GROW(a, b, c)                  ((a) = vd__dynarray_grow((a), sizeof(*(a)), (b), (c), VD_XR__DYNARRAY_ARENAP(a)))
#define VD_XR__DYNARRAY_PTR_CHECKED(a, i)              ((i < VD_XR__DYNARRAY_LEN(a)) ? &(a)[i] : 0)

// @todo(mdodis): fix & check dynarray & fixedarray (they use invalid macros and dynarray is not tested)
#define VD_XR__DYNARRAY_CHECKGROW(a, n)                      \
    ((!(a) || VD_XR__DYNARRAY_HEADER(a)->len + (n) > VD_XR__DYNARRAY_HEADER(a)->cap) \
    ? (VD_XR__DYNARRAY_GROW(a, n, 0), 0) : 0)

VD_XR_INL void *vd__dynarray_grow(void *a, size_t tsize, uint32_t addlen, uint32_t mincap, VdXr__Arena *arena)
{
    size_t min_len = VD_XR__DYNARRAY_LEN(a) + addlen;

    if (min_len > mincap) {
        mincap = (uint32_t)min_len;
    }

    if (mincap <= VD_XR__DYNARRAY_CAP(a)) {
        return a;
    }

    if (mincap < (2 * VD_XR__DYNARRAY_CAP(a))) {
        mincap = 2 * VD_XR__DYNARRAY_CAP(a);
    } else if (mincap < 4) {
        mincap = 4;
    }

    void *b = vd_xr__arena_resize(arena, 
        a ? VD_XR__DYNARRAY_HEADER(a) : 0,
        VD_XR__DYNARRAY_CAP(a) == 0 ? 0 : tsize * VD_XR__DYNARRAY_CAP(a) + sizeof(VdXr__DynArrayHeader),
        tsize * mincap + sizeof(VdXr__DynArrayHeader));

    b = (uint8_t*)b + sizeof(VdXr__DynArrayHeader);

    if (a == 0) {
        VD_XR__DYNARRAY_HEADER(b)->len = 0;
        VD_XR__DYNARRAY_HEADER(b)->arena = arena;
    }

    VD_XR__DYNARRAY_HEADER(b)->cap = mincap;
    return b;
}

$$xr_generated_dispatch_table.h$$

#define VD_XR__LOADER_PROPERTY_XDG_CONFIG_DIRS_NAME "XDG_CONFIG_DIRS"
#define VD_XR__LOADER_PROPERTY_XDG_DATA_DIRS_NAME "XDG_DATA_DIRS"
#define VD_XR__LOADER_PROPERTY_XDG_DATA_HOME_NAME "XDG_DATA_HOME"
#define VD_XR__LOADER_PROPERTY_HOME_NAME "HOME"

typedef enum {
    VD_XR__LOADER_PROPERTY_NONE = 0,
    VD_XR__LOADER_PROPERTY_XR_ENABLE_API_LAYERS,
    VD_XR__LOADER_PROPERTY_XR_RUNTIME_JSON_ENV_VAR,
#if !defined(XR_OS_WINDOWS) && !defined(XR_OS_ANDROID)
    VD_XR__LOADER_PROPERTY_XDG_CONFIG_DIRS,
    VD_XR__LOADER_PROPERTY_XDG_DATA_DIRS,
    VD_XR__LOADER_PROPERTY_XDG_DATA_HOME,
    VD_XR__LOADER_PROPERTY_HOME,
#endif
    VD_XR__LOADER_PROPERTY_COUNT
} VdXr__LoaderProperty;

typedef enum {
    VD_XR__MANIFEST_FILE_TYPE_IMPLICIT_API_LAYER,
    VD_XR__MANIFEST_FILE_TYPE_EXPLICIT_API_LAYER,
    VD_XR__MANIFEST_FILE_TYPE_UNDEFINED,
    VD_XR__MANIFEST_FILE_TYPE_RUNTIME,
} VdXr__ManifestFileType;

typedef struct {
    int  name_len;
    char name[128];

    int library_path_len;
    char library_path[256];
} VdXr__RuntimeManifestFileData;

typedef struct {
    VdXr__ManifestFileType file_type;
    int version_major;
    int version_minor;
    int version_patch;
    union {
        VdXr__RuntimeManifestFileData runtime;
    } data;
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
    const char          *s;
    int                 l;
} VdXr__StringRange;

typedef struct {
    VdXr__StringBuffer buffer;
    int                overriden;
} VdXr__LoaderPropertyStorage;

static void                 vd_xr__string_buffer_empty(VdXr__StringBuffer *buf);
static char*                vd_xr__string_buffer_push(VdXr__StringBuffer *buf, const char *str);
static char*                vd_xr__string_buffer_push_range(VdXr__StringBuffer *buf, VdXr__StringRange range);
static char*                vd_xr__string_buffer_pushu16(VdXr__StringBuffer *buf, uint16_t i);
static char*                vd_xr__string_buffer_pushchar(VdXr__StringBuffer *buf, char c);
static void*                vd_xr__string_buffer_pushraw(VdXr__StringBuffer *buf, int required_capacity, int update_len);

static void                 vd_xr__init(void);
static void                 vd_xr__manifest_file_list_empty(VdXr__ManifestFileList *list);
static void                 vd_xr__manifest_file_list_push(VdXr__ManifestFileList *list, VdXr__ManifestFile *file);
static XrResult             vd_xr__find_manifest_files(VdXr__ManifestFileType type, VdXr__ManifestFileList *list);
static XrResult             vd_xr__find_runtime_manifest_files(VdXr__ManifestFileList *list);
static XrResult             vd_xr__load_runtime(void);
static XrResult             vd_xr__get_instance_extension_properties_from_runtime(void);
static void                 vd_xr__add_files_in_path(VdXr__ManifestFileList *list, int is_directory_list, const char *search_path);
static void                 vd_xr__check_all_files_in_path(VdXr__StringRange search_path, int is_directory_list, VdXr__ManifestFileList *list);
static int                  vd_xr__load_manifest_file(VdXr__ManifestFile *file, char *json, size_t len);

static const char*          vd_xr__get_loader_property(VdXr__LoaderProperty property);
static void                 vd_xr__copy_include_paths(VdXr__StringBuffer *buf, int is_directory_list, const char *cur_path, const char *rel_path);

static VdXr__StringRange    vd_xr__substr(const char *s, int i, int count);

#if XR_OS_WINDOWS
static int                  vd_xr__utf8_to_wide(char *buf, int blen, wchar_t *wbuf, int wlen);
static wchar_t*             vd_xr__utf8_to_wide_arena(VdXr__Arena *arena, char *buf, int blen, int *wlen);
static int                  vd_xr__wide_to_utf8(wchar_t *wbuf, int wlen, char *buf, int blen);
#endif

typedef struct {
    XrGeneratedDispatchTable        *dispatch_table;
    VdXr__ManifestFileList          manbuf;
    VdXr__ManifestFileList          runmanbuf;
    VdXr__StringBuffer              strbuf;
    VdXr__StringBuffer              pathbuf;
    VdXr__LoaderPropertyStorage     properties[VD_XR__LOADER_PROPERTY_COUNT];
    VdXr__Arena                     arena;
    int                             initialized;
    int                             runtime_loaded;
} VdXr__InternalData;

static VdXr__InternalData Vd_Xr_G;

$$xr_generated_loader.cpp$$
$$xr_generated_dispatch_table.c$$

#ifdef _WIN32

#pragma pack(push, 1)
/* ----WIN32 BASE---------------------------------------------------------------------------------------------------- */
#ifndef _MINWINDEF_
#define VD_XR_DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

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
typedef struct VdXr_SECURITY_ATTRIBUTES {
  VdXrDWORD  nLength;
  VdXrLPVOID lpSecurityDescriptor;
  VdXrBOOL   bInheritHandle;
} VdXrSECURITY_ATTRIBUTES, *VdXrPSECURITY_ATTRIBUTES, *VdXrLPSECURITY_ATTRIBUTES;

typedef union VdXr_LARGE_INTEGER {
  struct {
    VdXrDWORD LowPart;
    VdXrLONG  HighPart;
  } DUMMYSTRUCTNAME;
  struct {
    VdXrDWORD LowPart;
    VdXrLONG  HighPart;
  } u;
  VdXrLONGLONG QuadPart;
} VdXrLARGE_INTEGER, *VdXrPLARGE_INTEGER;

typedef struct VdXr_OVERLAPPED {
  VdXrULONG_PTR Internal;
  VdXrULONG_PTR InternalHigh;
  union {
    struct {
      VdXrDWORD Offset;
      VdXrDWORD OffsetHigh;
    } DUMMYSTRUCTNAME;
    void* Pointer;
  } DUMMYUNIONNAME;
  VdXrHANDLE    hEvent;
} VdXrOVERLAPPED, *VdXrLPOVERLAPPED;

VD_XR_DECLARE_HANDLE(VdXrHWND);
VD_XR_DECLARE_HANDLE(VdXrHINSTANCE);
VD_XR_DECLARE_HANDLE(VdXrHDC);
VD_XR_DECLARE_HANDLE(VdXrHMONITOR);
VD_XR_DECLARE_HANDLE(VdXrHGDIOBJ);
VD_XR_DECLARE_HANDLE(VdXrHBITMAP);
VD_XR_DECLARE_HANDLE(VdXrHKEY);

typedef VdXrHINSTANCE VdXrHMODULE;

extern VdXrHMODULE LoadLibraryA(VdXrLPCSTR path);
extern VdXrBOOL    CloseHandle(VdXrHANDLE hObject);
extern VdXrHANDLE  GetCurrentProcess();
extern void*       HeapAlloc(VdXrHANDLE hHeap, VdXrDWORD dwFlags, VdXrSIZE_T dwBytes);
extern VdXrHANDLE  GetProcessHeap();
extern void*       HeapReAlloc(VdXrHANDLE hHeap, VdXrDWORD dwFlags, void *lpMem, VdXrSIZE_T dwBytes);
extern VdXrBOOL    HeapFree(VdXrHANDLE hHeap, VdXrDWORD dwFlags, void *lpMem);
extern int         MultiByteToWideChar(VdXrUINT CodePage, VdXrDWORD dwFlags, VdXrLPCSTR lpMultiByteStr, int cbMultiByte, VdXrLPWSTR lpWideCharStr, int cchWideChar);
extern int         WideCharToMultiByte(VdXrUINT CodePage, VdXrDWORD dwFlags, VdXrLPCWSTR lpWideCharStr, int cchWideChar, VdXrLPSTR lpMultiByteStr, int cbMultiByte, VdXrLPSTR lpDefaultChar, VdXrBOOL *lpUsedDefaultChar);
extern void*       GetProcAddress(VdXrHMODULE hModule, VdXrLPCSTR lpProcName);
extern VdXrDWORD   GetEnvironmentVariableW(VdXrLPCWSTR lpName, VdXrLPWSTR lpBuffer, VdXrDWORD nSize);
extern VdXrDWORD   GetFileAttributesW(VdXrLPCWSTR lpFileName);
extern VdXrDWORD   GetFullPathNameW(VdXrLPCWSTR lpFileName, VdXrDWORD nBufferLength, VdXrLPWSTR lpBuffer, VdXrLPWSTR *lpFilePart);
extern VdXrHANDLE  CreateFileW(VdXrLPCWSTR lpFileName, VdXrDWORD dwDesiredAccess, VdXrDWORD dwShareMode, VdXrLPSECURITY_ATTRIBUTES lpSecurityAttributes, VdXrDWORD dwCreationDisposition, VdXrDWORD dwFlagsAndAttributes, VdXrHANDLE hTemplateFile);
extern VdXrBOOL    GetFileSizeEx(VdXrHANDLE hFile, VdXrPLARGE_INTEGER lpFileSize);
extern VdXrBOOL    ReadFile(VdXrHANDLE hFile, VdXrLPVOID lpBuffer, VdXrDWORD nNumberOfBytesToRead, VdXrLPDWORD lpNumberOfBytesRead, VdXrLPOVERLAPPED lpOverlapped);

#else
typedef HWND            VdXrHWND;
typedef DWORD           VdXrDWORD;
typedef WORD            VdXrWORD;
typedef LPWSTR          VdXrLPWSTR;
typedef LPCWSTR         VdXrLPCWSTR;
typedef LPCSTR          VdXrLPCSTR;
typedef DWORD           VdXrDWORD;
typedef UINT            VdXrUINT;
typedef UINT64          VdXrUINT64;
typedef UINT32          VdXrUINT32;
typedef INT32           VdXrINT32;
typedef UINT16          VdXrUINT16;
typedef UINT8           VdXrUINT8;
typedef INT16           VdXrINT16;
typedef HMODULE         VdXrHMODULE;
typedef HRESULT         VdXrHRESULT;
typedef ULONG           VdXrULONG;
typedef HDC             VdXrHDC;
typedef HMONITOR        VdXrHMONITOR;
typedef HANDLE          VdXrHANDLE;
typedef HGDIOBJ         VdXrHGDIOBJ;
typedef HBITMAP         VdXrHBITMAP;
typedef LPARAM          VdXrLPARAM;
typedef BOOL            VdXrPBOOL;
typedef LONG            VdXrLONG;
typedef BYTE            VdXrBYTE;
typedef WCHAR           VdXrWCHAR;
typedef BOOL            VdXrBOOL;
typedef LPVOID          VdXrLPVOID;
typedef FLOAT           VdXrFLOAT;
typedef VdXrDWORD       VdXrCOLORREF;
typedef VdXrDWORD*      VdXrLPCOLORREF;
typedef LARGE_INTEGER   VdXrLARGE_INTEGER;
#endif // !_MINWINDEF_

typedef VdXrLONG VdXrLSTATUS;

typedef VdXrDWORD VdXrACCESS_MASK;
typedef VdXrACCESS_MASK VdXrREGSAM;

#define VD_XR__WIN32_KEY_QUERY_VALUE         (0x0001)
#define VD_XR__WIN32_KEY_SET_VALUE           (0x0002)
#define VD_XR__WIN32_KEY_CREATE_SUB_KEY      (0x0004)
#define VD_XR__WIN32_KEY_ENUMERATE_SUB_KEYS  (0x0008)
#define VD_XR__WIN32_KEY_NOTIFY              (0x0010)
#define VD_XR__WIN32_KEY_CREATE_LINK         (0x0020)
#define VD_XR__WIN32_KEY_WOW64_32KEY         (0x0200)
#define VD_XR__WIN32_KEY_WOW64_64KEY         (0x0100)
#define VD_XR__WIN32_KEY_WOW64_RES           (0x0300)


#define VD_XR__WIN32_DELETE                           (0x00010000L)
#define VD_XR__WIN32_READ_CONTROL                     (0x00020000L)
#define VD_XR__WIN32_WRITE_DAC                        (0x00040000L)
#define VD_XR__WIN32_WRITE_OWNER                      (0x00080000L)
#define VD_XR__WIN32_SYNCHRONIZE                      (0x00100000L)

#define VD_XR__WIN32_STANDARD_RIGHTS_READ             (VD_XR__WIN32_READ_CONTROL)

#define VD_XR__WIN32_KEY_READ       ((VD_XR__WIN32_STANDARD_RIGHTS_READ       |\
                                     VD_XR__WIN32_KEY_QUERY_VALUE             |\
                                     VD_XR__WIN32_KEY_ENUMERATE_SUB_KEYS      |\
                                     VD_XR__WIN32_KEY_NOTIFY)                  \
                                     &                                         \
                                    (~VD_XR__WIN32_SYNCHRONIZE))

#define VD_XR__WIN32_KEY_WRITE      ((VD_XR__WIN32_STANDARD_RIGHTS_WRITE      |\
                                     VD_XR__WIN32_KEY_SET_VALUE               |\
                                     VD_XR__WIN32_KEY_CREATE_SUB_KEY)          \
                                     &                                         \
                                    (~VD_XR__WIN32_SYNCHRONIZE))

#define VD_XR__WIN32_KEY_EXECUTE    ((VD_XR__WIN32_KEY_READ)                   \
                                     &                                         \
                                    (~VD_XR__WIN32_SYNCHRONIZE))

#define VD_XR__WIN32_KEY_ALL_ACCESS      ((VD_XR__WIN32_STANDARD_RIGHTS_ALL  |\
                                    VD_XR__WIN32_KEY_QUERY_VALUE             |\
                                    VD_XR__WIN32_KEY_SET_VALUE               |\
                                    VD_XR__WIN32_KEY_CREATE_SUB_KEY          |\
                                    VD_XR__WIN32_KEY_ENUMERATE_SUB_KEYS      |\
                                    VD_XR__WIN32_KEY_NOTIFY                  |\
                                    VD_XR__WIN32_KEY_CREATE_LINK)             \
                                    &                                         \
                                    (~VD_XR__WIN32_SYNCHRONIZE))

#define VD_XR__WIN32_TOKEN_ASSIGN_PRIMARY    (0x0001)
#define VD_XR__WIN32_TOKEN_DUPLICATE         (0x0002)
#define VD_XR__WIN32_TOKEN_IMPERSONATE       (0x0004)
#define VD_XR__WIN32_TOKEN_QUERY             (0x0008)
#define VD_XR__WIN32_TOKEN_QUERY_SOURCE      (0x0010)
#define VD_XR__WIN32_TOKEN_ADJUST_PRIVILEGES (0x0020)
#define VD_XR__WIN32_TOKEN_ADJUST_GROUPS     (0x0040)
#define VD_XR__WIN32_TOKEN_ADJUST_DEFAULT    (0x0080)
#define VD_XR__WIN32_TOKEN_ADJUST_SESSIONID  (0x0100)

#define VD_XR__WIN32_SECURITY_MANDATORY_LABEL_AUTHORITY          {0,0,0,0,0,16}
#define VD_XR__WIN32_SECURITY_MANDATORY_UNTRUSTED_RID            (0x00000000L)
#define VD_XR__WIN32_SECURITY_MANDATORY_LOW_RID                  (0x00001000L)
#define VD_XR__WIN32_SECURITY_MANDATORY_MEDIUM_RID               (0x00002000L)
#define VD_XR__WIN32_SECURITY_MANDATORY_MEDIUM_PLUS_RID          (VD_XR__WIN32_SECURITY_MANDATORY_MEDIUM_RID + 0x100)
#define VD_XR__WIN32_SECURITY_MANDATORY_HIGH_RID                 (0x00003000L)
#define VD_XR__WIN32_SECURITY_MANDATORY_SYSTEM_RID               (0x00004000L)
#define VD_XR__WIN32_SECURITY_MANDATORY_PROTECTED_PROCESS_RID    (0x00005000L)

#define VD_XR__WIN32_RRF_RT_REG_NONE        0x00000001
#define VD_XR__WIN32_RRF_RT_REG_SZ          0x00000002
#define VD_XR__WIN32_RRF_RT_REG_EXPAND_SZ   0x00000004
#define VD_XR__WIN32_RRF_RT_REG_BINARY      0x00000008
#define VD_XR__WIN32_RRF_RT_REG_DWORD       0x00000010
#define VD_XR__WIN32_RRF_RT_REG_MULTI_SZ    0x00000020
#define VD_XR__WIN32_RRF_RT_REG_QWORD       0x00000040
#define VD_XR__WIN32_RRF_RT_DWORD           (VD_XR__WIN32_RRF_RT_REG_BINARY | VD_XR__WIN32_RRF_RT_REG_DWORD)
#define VD_XR__WIN32_RRF_RT_QWORD           (VD_XR__WIN32_RRF_RT_REG_BINARY | VD_XR__WIN32_RRF_RT_REG_QWORD)
#define VD_XR__WIN32_RRF_RT_ANY             0x0000ffff
#define VD_XR__WIN32_RRF_NOEXPAND           0x10000000  // do not automatically expand environment strings if value is of type REG_EXPAND_SZ
#define VD_XR__WIN32_RRF_ZEROONFAILURE      0x20000000  // if pvData is not NULL, set content to all zeros on failure


#define VD_XR__WIN32_REG_NONE                    ( 0ul )
#define VD_XR__WIN32_REG_SZ                      ( 1ul )
#define VD_XR__WIN32_REG_EXPAND_SZ               ( 2ul )
#define VD_XR__WIN32_REG_BINARY                  ( 3ul )
#define VD_XR__WIN32_REG_DWORD                   ( 4ul )
#define VD_XR__WIN32_REG_DWORD_LITTLE_ENDIAN     ( 4ul )
#define VD_XR__WIN32_REG_DWORD_BIG_ENDIAN        ( 5ul )
#define VD_XR__WIN32_REG_LINK                    ( 6ul )
#define VD_XR__WIN32_REG_MULTI_SZ                ( 7ul )
#define VD_XR__WIN32_REG_RESOURCE_LIST           ( 8ul )
#define VD_XR__WIN32_REG_FULL_RESOURCE_DESCRIPTOR ( 9ul )
#define VD_XR__WIN32_REG_RESOURCE_REQUIREMENTS_LIST ( 10ul )
#define VD_XR__WIN32_REG_QWORD                   ( 11ul )
#define VD_XR__WIN32_REG_QWORD_LITTLE_ENDIAN     ( 11ul )
#define VD_XR__WIN32_FILE_READ_DATA            ( 0x0001 )    // file & pipe
#define VD_XR__WIN32_FILE_LIST_DIRECTORY       ( 0x0001 )    // directory

#define VD_XR__WIN32_FILE_WRITE_DATA           ( 0x0002 )    // file & pipe
#define VD_XR__WIN32_FILE_ADD_FILE             ( 0x0002 )    // directory

#define VD_XR__WIN32_FILE_APPEND_DATA          ( 0x0004 )    // file
#define VD_XR__WIN32_FILE_ADD_SUBDIRECTORY     ( 0x0004 )    // directory
#define VD_XR__WIN32_FILE_CREATE_PIPE_INSTANCE ( 0x0004 )    // named pipe


#define VD_XR__WIN32_FILE_READ_EA              ( 0x0008 )    // file & directory

#define VD_XR__WIN32_FILE_WRITE_EA             ( 0x0010 )    // file & directory

#define VD_XR__WIN32_FILE_EXECUTE              ( 0x0020 )    // file
#define VD_XR__WIN32_FILE_TRAVERSE             ( 0x0020 )    // directory

#define VD_XR__WIN32_FILE_DELETE_CHILD         ( 0x0040 )    // directory

#define VD_XR__WIN32_FILE_READ_ATTRIBUTES      ( 0x0080 )    // all

#define VD_XR__WIN32_FILE_WRITE_ATTRIBUTES     ( 0x0100 )    // all

#define VD_XR__WIN32_GENERIC_READ                     (0x80000000L)
#define VD_XR__WIN32_GENERIC_WRITE                    (0x40000000L)
#define VD_XR__WIN32_GENERIC_EXECUTE                  (0x20000000L)
#define VD_XR__WIN32_GENERIC_ALL                      (0x10000000L)

#define VD_XR__WIN32_FILE_GENERIC_READ         (VD_XR__WIN32_STANDARD_RIGHTS_READ     |\
                                                VD_XR__WIN32_FILE_READ_DATA           |\
                                                VD_XR__WIN32_FILE_READ_ATTRIBUTES     |\
                                                VD_XR__WIN32_FILE_READ_EA             |\
                                                VD_XR__WIN32_SYNCHRONIZE)


#define VD_XR__WIN32_FILE_GENERIC_WRITE        (VD_XR__WIN32_STANDARD_RIGHTS_WRITE    |\
                                                VD_XR__WIN32_FILE_WRITE_DATA          |\
                                                VD_XR__WIN32_FILE_WRITE_ATTRIBUTES    |\
                                                VD_XR__WIN32_FILE_WRITE_EA            |\
                                                VD_XR__WIN32_FILE_APPEND_DATA         |\
                                                VD_XR__WIN32_SYNCHRONIZE)


#define VD_XR__WIN32_FILE_GENERIC_EXECUTE      (VD_XR__WIN32_STANDARD_RIGHTS_EXECUTE  |\
                                                VD_XR__WIN32_FILE_READ_ATTRIBUTES     |\
                                                VD_XR__WIN32_FILE_EXECUTE             |\
                                                VD_XR__WIN32_SYNCHRONIZE)

#define VD_XR__WIN32_FILE_SHARE_READ                 0x00000001  
#define VD_XR__WIN32_FILE_SHARE_WRITE                0x00000002  
#define VD_XR__WIN32_FILE_SHARE_DELETE               0x00000004  
#define VD_XR__WIN32_FILE_ATTRIBUTE_READONLY             0x00000001  
#define VD_XR__WIN32_FILE_ATTRIBUTE_HIDDEN               0x00000002  
#define VD_XR__WIN32_FILE_ATTRIBUTE_SYSTEM               0x00000004  
#define VD_XR__WIN32_FILE_ATTRIBUTE_DIRECTORY            0x00000010  
#define VD_XR__WIN32_FILE_ATTRIBUTE_ARCHIVE              0x00000020  
#define VD_XR__WIN32_FILE_ATTRIBUTE_DEVICE               0x00000040  
#define VD_XR__WIN32_FILE_ATTRIBUTE_NORMAL               0x00000080  
#define VD_XR__WIN32_FILE_ATTRIBUTE_TEMPORARY            0x00000100  

#define VD_XR__WIN32_INVALID_FILE_ATTRIBUTES ((VdXrDWORD)-1)

typedef enum VdXr_TOKEN_INFORMATION_CLASS {
  VD_XR_TokenUser = 1,
  VD_XR_TokenGroups,
  VD_XR_TokenPrivileges,
  VD_XR_TokenOwner,
  VD_XR_TokenPrimaryGroup,
  VD_XR_TokenDefaultDacl,
  VD_XR_TokenSource,
  VD_XR_TokenType,
  VD_XR_TokenImpersonationLevel,
  VD_XR_TokenStatistics,
  VD_XR_TokenRestrictedSids,
  VD_XR_TokenSessionId,
  VD_XR_TokenGroupsAndPrivileges,
  VD_XR_TokenSessionReference,
  VD_XR_TokenSandBoxInert,
  VD_XR_TokenAuditPolicy,
  VD_XR_TokenOrigin,
  VD_XR_TokenElevationType,
  VD_XR_TokenLinkedToken,
  VD_XR_TokenElevation,
  VD_XR_TokenHasRestrictions,
  VD_XR_TokenAccessInformation,
  VD_XR_TokenVirtualizationAllowed,
  VD_XR_TokenVirtualizationEnabled,
  VD_XR_TokenIntegrityLevel,
  VD_XR_TokenUIAccess,
  VD_XR_TokenMandatoryPolicy,
  VD_XR_TokenLogonSid,
  VD_XR_TokenIsAppContainer,
  VD_XR_TokenCapabilities,
  VD_XR_TokenAppContainerSid,
  VD_XR_TokenAppContainerNumber,
  VD_XR_TokenUserClaimAttributes,
  VD_XR_TokenDeviceClaimAttributes,
  VD_XR_TokenRestrictedUserClaimAttributes,
  VD_XR_TokenRestrictedDeviceClaimAttributes,
  VD_XR_TokenDeviceGroups,
  VD_XR_TokenRestrictedDeviceGroups,
  VD_XR_TokenSecurityAttributes,
  VD_XR_TokenIsRestricted,
  VD_XR_TokenProcessTrustLevel,
  VD_XR_TokenPrivateNameSpace,
  VD_XR_TokenSingletonAttributes,
  VD_XR_TokenBnoIsolation,
  VD_XR_TokenChildProcessFlags,
  VD_XR_TokenIsLessPrivilegedAppContainer,
  VD_XR_TokenIsSandboxed,
  VD_XR_TokenIsAppSilo,
  VD_XR_TokenLoggingInformation,
  VD_XR_TokenLearningMode,
  VD_XR_MaxTokenInfoClass
} VdXrTOKEN_INFORMATION_CLASS, *VdXrPTOKEN_INFORMATION_CLASS;

typedef struct VdXr_SID_IDENTIFIER_AUTHORITY {
    VdXrBYTE  Value[6];
} VdXrSID_IDENTIFIER_AUTHORITY, *VdXrPSID_IDENTIFIER_AUTHORITY;

typedef struct VdXr_SID {
    VdXrBYTE  Revision;
    VdXrBYTE  SubAuthorityCount;
    VdXrSID_IDENTIFIER_AUTHORITY IdentifierAuthority;
    VdXrDWORD SubAuthority[1];
} VdXrSID, *VdXrPISID;

typedef struct VdXr_SID_AND_ATTRIBUTES {
  VdXrPISID Sid;
  VdXrDWORD Attributes;
} VdXrSID_AND_ATTRIBUTES, *VdXrPSID_AND_ATTRIBUTES;

typedef struct VdXr_TOKEN_MANDATORY_LABEL {
    VdXrSID_AND_ATTRIBUTES Label;
} VdXrTOKEN_MANDATORY_LABEL, *VdXrPTOKEN_MANDATORY_LABEL;

#define VD_XR__SID_MAX_SUB_AUTHORITIES          (15)
#define VD_XR__SID_RECOMMENDED_SUB_AUTHORITIES  (1)    // Will change to around 6

                                                // in a future release.
#define VD_XR__SECURITY_MAX_SID_SIZE  \
      (sizeof(VdXrSID) - sizeof(VdXrDWORD) + (VD_XR__SID_MAX_SUB_AUTHORITIES * sizeof(VdXrDWORD)))


#define VD_XR__WIN32_HKEY_CURRENT_USER                   (( VdXrHKEY ) (VdXrULONG_PTR)((VdXrLONG)0x80000001) )
#define VD_XR__WIN32_HKEY_LOCAL_MACHINE                  (( VdXrHKEY ) (VdXrULONG_PTR)((VdXrLONG)0x80000002) )

typedef VdXrLSTATUS (*VdXr__ProcRegOpenKeyExW)(VdXrHKEY hKey, VdXrLPCWSTR lpSubKey, VdXrDWORD ulOptions, VdXrREGSAM samDesired, VdXrHKEY *phkResult);
typedef VdXrLSTATUS (*VdXr__ProcRegGetValueW)(VdXrHKEY hkey, VdXrLPCWSTR lpSubKey, VdXrLPCWSTR lpValue, VdXrDWORD dwFlags, VdXrLPDWORD pdwType, void *pvData, VdXrLPDWORD pcbData);
typedef VdXrLSTATUS (*VdXr__ProcRegCloseKey)(VdXrHKEY hKey);
typedef VdXrBOOL    (*VdXr__ProcOpenProcessToken)(VdXrHANDLE ProcessHandle, VdXrDWORD DesiredAccess, VdXrPHANDLE TokenHandle);
typedef VdXrBOOL    (*VdXr__ProcGetTokenInformation)(VdXrHANDLE TokenHandle, VdXrTOKEN_INFORMATION_CLASS TokenInformationClass, VdXrLPVOID TokenInformation, VdXrDWORD TokenInformationLength, VdXrPDWORD ReturnLength);
typedef VdXrUCHAR*  (*VdXr__ProcGetSidSubAuthorityCount)(VdXrPISID pSid);
typedef VdXrPDWORD  (*VdXr__ProcGetSidSubAuthority)(VdXrPISID pSid, VdXrDWORD nSubAuthority);

#pragma pack(pop)
static VdXr__ProcRegOpenKeyExW VdXr__RegOpenKeyExW;
static VdXr__ProcRegGetValueW VdXr__RegGetValueW;
static VdXr__ProcRegCloseKey VdXr__RegCloseKey;
static VdXr__ProcOpenProcessToken VdXr__OpenProcessToken;
static VdXr__ProcGetTokenInformation VdXr__GetTokenInformation;
static VdXr__ProcGetSidSubAuthorityCount VdXr__GetSidSubAuthorityCount;
static VdXr__ProcGetSidSubAuthority VdXr__GetSidSubAuthority;

static void                 vd_xr__win32_init(void);
static int                  vd_xr__read_data_files_in_hive(VdXrHKEY hive, const wchar_t *location, VdXr__ManifestFileList *list);
static int                  vd_xr__is_high_integrity_level(void);

static int vd_xr__utf8_to_wide(char *buf, int blen, wchar_t *wbuf, int wlen)
{
    return MultiByteToWideChar(65001, 8, buf, blen, wbuf, wlen);
}

static wchar_t *vd_xr__utf8_to_wide_arena(VdXr__Arena *arena, char *buf, int blen, int *wlen)
{
    int count = vd_xr__utf8_to_wide(buf, blen, 0, 0);
    wchar_t *wbuf = (wchar_t*)vd_xr__arena_alloc(arena, sizeof(wchar_t) * (count + 1));
    int written = vd_xr__utf8_to_wide(buf, blen, wbuf, count);

    if (wlen) *wlen = count;

    wbuf[written] = 0;

    return wbuf;
}

static int vd_xr__wide_to_utf8(wchar_t *wbuf, int wlen, char *buf, int blen)
{
    return WideCharToMultiByte(65001, 0, wbuf, wlen, buf, blen, NULL, NULL);
}

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

static int vd_xr__read_data_files_in_hive(VdXrHKEY hive, const wchar_t *location, VdXr__ManifestFileList *list)
{
    VdXrHKEY hkey;
    VdXrLONG open_val = VdXr__RegOpenKeyExW(hive, location, 0, VD_XR__WIN32_KEY_QUERY_VALUE, &hkey);

    if (open_val != 0) {
        return 0;
    }

    VdXr__RegCloseKey(hkey);

    return 1;
}

static int vd_xr__is_high_integrity_level(void)
{
    static int cached_result = 0;

    if (cached_result != 0) {
        return cached_result > 0 ? 1 : 0;
    }

    VdXrHANDLE token;
    if (VdXr__OpenProcessToken(GetCurrentProcess(),
                               VD_XR__WIN32_TOKEN_QUERY | VD_XR__WIN32_TOKEN_QUERY_SOURCE,
                               &token))
    {
        uint8_t label_buf[VD_XR__SECURITY_MAX_SID_SIZE + sizeof(VdXrDWORD)] = {0};
        VdXrDWORD buffer_size;

        if (VdXr__GetTokenInformation(token, VD_XR_TokenIntegrityLevel, label_buf, sizeof(label_buf), &buffer_size) != 0) {
            VdXrTOKEN_MANDATORY_LABEL *mandatory_label = (VdXrTOKEN_MANDATORY_LABEL*)label_buf;

            if (mandatory_label->Label.Sid != 0) {
                VdXrDWORD sub_auth_count = *VdXr__GetSidSubAuthorityCount(mandatory_label->Label.Sid);
                VdXrDWORD integrity_level = *VdXr__GetSidSubAuthority(mandatory_label->Label.Sid, sub_auth_count - 1);

                cached_result = integrity_level > VD_XR__WIN32_SECURITY_MANDATORY_MEDIUM_RID
                    ? 1
                    : -1;
            }
        }

        CloseHandle(token);
    }

    return cached_result > 0 ? 1 : 0;
}

static void vd_xr__win32_init(void)
{
    {
        VdXrHMODULE mod = LoadLibraryA("Advapi32.dll");
        VdXr__RegOpenKeyExW = (VdXr__ProcRegOpenKeyExW)GetProcAddress(mod, "RegOpenKeyExW");
        VdXr__RegGetValueW = (VdXr__ProcRegGetValueW)GetProcAddress(mod, "RegGetValueW");
        VdXr__RegCloseKey = (VdXr__ProcRegCloseKey)GetProcAddress(mod, "RegCloseKey");
        VdXr__OpenProcessToken = (VdXr__ProcOpenProcessToken)GetProcAddress(mod, "OpenProcessToken");
        VdXr__GetTokenInformation = (VdXr__ProcGetTokenInformation)GetProcAddress(mod, "GetTokenInformation");
        VdXr__GetSidSubAuthorityCount = (VdXr__ProcGetSidSubAuthorityCount)GetProcAddress(mod, "GetSidSubAuthorityCount");
        VdXr__GetSidSubAuthority = (VdXr__ProcGetSidSubAuthority)GetProcAddress(mod, "GetSidSubAuthority");

    }
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

    vd_xr__memcpy(buf->ptr + buf->len, (char*)str, str_len);
    buf->len += str_len;
    buf->ptr[buf->len] = '\0';
    return buf->ptr;
}

static char *vd_xr__string_buffer_push_range(VdXr__StringBuffer *buf, VdXr__StringRange range)
{
    int str_len = range.l;
    int required_capacity = buf->len + str_len + 1;
    buf->ptr = (char*)vd_xr__resize_buffer(buf->ptr, sizeof(char), required_capacity, &buf->cap);

    vd_xr__memcpy(buf->ptr + buf->len, (char*)range.s, str_len);
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
        ibuf[c] = (i % 10) + '0';
        i = i / 10;
        c++;
    }

    return vd_xr__string_buffer_push(buf, ibuf);
}

static char *vd_xr__string_buffer_pushchar(VdXr__StringBuffer *buf, char c)
{
    char b[2];
    b[0] = c;
    b[1] = 0;
    return vd_xr__string_buffer_push(buf, b);
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
    VdXr__LoaderProperty override_env_var = VD_XR__LOADER_PROPERTY_NONE;
#if XR_OS_WINDOWS
    const char *registry_location = 0;
#endif

    switch (type) {
        case VD_XR__MANIFEST_FILE_TYPE_IMPLICIT_API_LAYER: {
            vd_xr__string_buffer_push(strbuf, OPENXR_IMPLICIT_API_LAYER_RELATIVE_PATH);
#if XR_OS_WINDOWS
            registry_location = OPENXR_IMPLICIT_API_LAYER_REGISTRY_LOCATION;
#endif
        } break;

        case VD_XR__MANIFEST_FILE_TYPE_EXPLICIT_API_LAYER: {
            vd_xr__string_buffer_push(strbuf, OPENXR_EXPLICIT_API_LAYER_RELATIVE_PATH);
            override_env_var = VD_XR__LOADER_PROPERTY_XR_ENABLE_API_LAYERS;
#if XR_OS_WINDOWS
            registry_location = OPENXR_EXPLICIT_API_LAYER_REGISTRY_LOCATION;
#endif
        } break;

        case VD_XR__MANIFEST_FILE_TYPE_UNDEFINED:
        case VD_XR__MANIFEST_FILE_TYPE_RUNTIME:
        default: {
            return XR_ERROR_FILE_ACCESS_ERROR;
        } break;
    }

    VdXr__StringBuffer *search_path = &Vd_Xr_G.pathbuf;
    // Find data files in search paths
    {
        int override_active = 0;
        const char *relative_path = strbuf->ptr;
        // ReadDataFilesInSearchPaths(override_env_var, relative_path, override_active, filenames);

        const char *override_path = 0;

        int is_override_env_var_empty = override_env_var != VD_XR__LOADER_PROPERTY_NONE;

        if (!is_override_env_var_empty) {
            int permit_override = 1;
#if !XR_OS_WINDOWS
            if (geteuid() != getuid() || getegid() != getgid()) {
                permit_override = 0;
            }
#endif
            if (permit_override) {
                override_path = vd_xr__get_loader_property(override_env_var);
            }
        }

        if (override_path != 0) {
            vd_xr__copy_include_paths(search_path, 1, override_path, "");
            override_active = 1;
        } else {
            override_active = 0;

#if !XR_OS_WINDOWS && !XR_OS_ANDROID
#error "Unsupported platform!"
#elif XR_OS_ANDROID
#error "Unsupported platform!"
#else
            (void)relative_path;
#endif
        }

        vd_xr__add_files_in_path(list, 1, search_path->ptr);
    }

#if defined(XR_OS_WINDOWS)
    {
        // ReadLayerDataFilesInRegistry
        VdXr__StringBuffer *full_registry_location_buf = &Vd_Xr_G.strbuf;
        vd_xr__string_buffer_empty(full_registry_location_buf);
        vd_xr__string_buffer_push(full_registry_location_buf, OPENXR_REGISTRY_LOCATION);
        vd_xr__string_buffer_pushu16(full_registry_location_buf, XR_VERSION_MAJOR(XR_CURRENT_API_VERSION));
        const char *full_registry_location = vd_xr__string_buffer_push(full_registry_location_buf, registry_location);
        int full_registry_location_len = full_registry_location_buf->len;

        wchar_t buf[128];

        int buf_end = vd_xr__utf8_to_wide((char*)full_registry_location, full_registry_location_len, buf, sizeof(buf));
        if (buf_end <= 0) {
            return 0;
        }

        buf[buf_end] = 0;

        int found = vd_xr__read_data_files_in_hive(VD_XR__WIN32_HKEY_LOCAL_MACHINE, buf, list);
        if (!vd_xr__is_high_integrity_level()) {
            found |= vd_xr__read_data_files_in_hive(VD_XR__WIN32_HKEY_CURRENT_USER, buf, list);
        }
    }
#endif // XR_OS_WINDOWS

    return XR_SUCCESS;
}

static void vd_xr__add_files_in_path(VdXr__ManifestFileList *list, int is_directory_list, const char *search_path)
{
    int last_found = 0;
    int found = vd_xr__str_first_of(search_path, VD_XR__PATH_SEPARATOR);
    VdXr__StringRange cur_search;

    while (found != -1) {
        int len = found - last_found;
        cur_search = vd_xr__substr(search_path, last_found, len);

        vd_xr__check_all_files_in_path(cur_search, is_directory_list, list);

        last_found = found;
        while (found == last_found) {
            last_found = found + 1;
            found = vd_xr__str_first_of_with_start(search_path, VD_XR__PATH_SEPARATOR, last_found);
        }
    }

    if (last_found < vd_xr__strlen(search_path)) {
        cur_search = vd_xr__substr(search_path, last_found, -1);
        vd_xr__check_all_files_in_path(cur_search, is_directory_list, list);
    }
}

static XrResult vd_xr__load_runtime(void)
{
    if (Vd_Xr_G.runtime_loaded) {
        return XR_SUCCESS;
    }

    // @note(mdodis): RuntimeManifestFile::FindManifestFiles
    {

        VdXr__ManifestFileList *list = &Vd_Xr_G.runmanbuf;
        vd_xr__manifest_file_list_empty(list);
        vd_xr__get_loader_property(VD_XR__LOADER_PROPERTY_XR_RUNTIME_JSON_ENV_VAR);

        VdXr__ManifestFile file = {0};
#if XR_OS_WINDOWS
        const char *runtime_registry_location = "";
        const char *default_runtime_value_name = "ActiveRuntime";

        // ReadRuntimeDataFilesInRegistry
        VdXrHKEY hkey;
        VdXr__StringBuffer *strbuf = &Vd_Xr_G.strbuf;
        vd_xr__string_buffer_empty(strbuf);

        vd_xr__string_buffer_push(strbuf, OPENXR_REGISTRY_LOCATION);
        vd_xr__string_buffer_pushu16(strbuf, XR_VERSION_MAJOR(XR_CURRENT_API_VERSION));
        const char *full_registry_location = vd_xr__string_buffer_push(strbuf, runtime_registry_location);
        size_t full_registry_location_len = strbuf->len;

        VdXr__ArenaSave save = vd_xr__arena_save(&Vd_Xr_G.arena);
        int full_registry_locationw_len;
        wchar_t *full_registry_locationw = vd_xr__utf8_to_wide_arena(&Vd_Xr_G.arena, 
                                                                     full_registry_location, full_registry_location_len,
                                                                     &full_registry_locationw_len);

        int default_runtime_value_namew_len;
        wchar_t *default_runtime_value_namew = vd_xr__utf8_to_wide_arena(&Vd_Xr_G.arena, 
                                                                         default_runtime_value_name, vd_xr__strlen(default_runtime_value_name),
                                                                         &default_runtime_value_namew_len);

        VdXrLONG open_val = VdXr__RegOpenKeyExW(VD_XR__WIN32_HKEY_LOCAL_MACHINE, full_registry_locationw,
                                                0, VD_XR__WIN32_KEY_QUERY_VALUE, &hkey);
        if (open_val == 0) {
            wchar_t *full_pathw = 0;
            wchar_t value_w[1024];
            VdXrDWORD value_w_size = sizeof(value_w);
            if (VdXr__RegGetValueW(hkey, 0, default_runtime_value_namew,
                                   VD_XR__WIN32_RRF_RT_REG_SZ | VD_XR__WIN32_REG_EXPAND_SZ | VD_XR__WIN32_RRF_ZEROONFAILURE,
                                   0, (VdXrLPBYTE)&value_w, &value_w_size) == 0)
            {
                VdXrDWORD attr = GetFileAttributesW(value_w);
                int is_regular_file = attr != VD_XR__WIN32_INVALID_FILE_ATTRIBUTES && !(attr & VD_XR__WIN32_FILE_ATTRIBUTE_DIRECTORY);

                if (is_regular_file) {

                    VdXrDWORD nreq = GetFullPathNameW(value_w, 0, 0, NULL);
                    full_pathw = VD_XR__ARENA_PUSH_ARRAY(&Vd_Xr_G.arena, wchar_t, nreq + 1);
                    GetFullPathNameW(value_w, nreq, full_pathw, NULL);
                    full_pathw[nreq] = 0;

                    VdXrHANDLE file_handle = CreateFileW(full_pathw, VD_XR__WIN32_GENERIC_READ,
                                                         VD_XR__WIN32_FILE_SHARE_READ, 0, 4 /* OPEN_ALWAYS */,
                                                         VD_XR__WIN32_FILE_ATTRIBUTE_NORMAL, 0);
                    if (file_handle != 0) {

                        VdXrLARGE_INTEGER file_size = {0};
                        GetFileSizeEx(file_handle, &file_size);
                        char *json = (char*)vd_xr__arena_alloc(&Vd_Xr_G.arena, file_size.QuadPart);
                        VdXrDWORD num_bytes_read;
                        if (ReadFile(file_handle, json, file_size.QuadPart, &num_bytes_read, 0) && (num_bytes_read >= file_size.QuadPart)) {
                            vd_xr__load_manifest_file(&file, json, file_size.QuadPart);
                        }
                    }
                }
            }

            VdXr__RegCloseKey(hkey);
        }


        vd_xr__arena_restore(save);
#endif

    }


    Vd_Xr_G.runtime_loaded = 1;
    return XR_SUCCESS;
}

static XrResult vd_xr__get_instance_extension_properties_from_runtime(void)
{
    return XR_SUCCESS;
}

static void vd_xr__check_all_files_in_path(VdXr__StringRange search_path, int is_directory_list, VdXr__ManifestFileList *list)
{

}

static int vd_xr__load_manifest_file(VdXr__ManifestFile *file, char *json, size_t len)
{
    size_t cur = 0;
    size_t nxt = 0;
#define VD_XR_EXPECT(c)     (((cur < len) && (json[cur] == c)) ? 1 : 0)
#define VD_XR_MUST(c)       do { if (!VD_XR_EXPECT(c)) { return 0; } else { VD_XR_NEXT(); } } while(0)
#define VD_XR_PEEK()        ((cur < len) ? (int)json[cur] : 0)
#define VD_XR_NEXT()        ((++cur >= len) ? 0 : 1)
#define VD_XR_SKIP()        while ((cur < len) && ((json[cur] == ' ') || (json[cur] == '\t') || (json[cur] == '\r') || (json[cur] == '\n'))) cur++;
#define VD_XR_DONE()        (cur >= len)
#define VD_XR_CURSUB(t)     ((vd_xr__strncmp(json + cur, t, sizeof(t) - 1) == 0) ? (cur += (sizeof(t) - 1)) : 0)
#define VD_XR_INT(var)      while ((cur < len) && (json[cur] >= '0') && (json[cur <= '9'])) { var *= 10; var += json[cur] - '0'; cur++; }
#define VD_XR_STR(var, l)   while ((cur < len) && (json[cur] != '\"')) { var[l++] = json[cur]; cur++; }

    VD_XR_SKIP();
    VD_XR_MUST('{');

    while (!VD_XR_DONE()) {

        VD_XR_SKIP();

        if (VD_XR_CURSUB("\"file_format_version\"")) { VD_XR_SKIP(); VD_XR_MUST(':'); VD_XR_SKIP();
            VD_XR_MUST('\"');
            int major = 0;
            VD_XR_INT(major);
            VD_XR_MUST('.');

            int minor = 0;
            VD_XR_INT(minor);
            VD_XR_MUST('.');

            int patch = 0;
            VD_XR_INT(patch);

            VD_XR_MUST('\"');
            file->version_major = major;
            file->version_minor = minor;
            file->version_patch = patch;

            VD_XR_SKIP();
            VD_XR_MUST(',');
        } else if (VD_XR_CURSUB("\"runtime\"")) { VD_XR_SKIP(); VD_XR_MUST(':'); VD_XR_SKIP();
            VD_XR_MUST('{');

            file->file_type = VD_XR__MANIFEST_FILE_TYPE_RUNTIME;

            while (!VD_XR_DONE() && !VD_XR_EXPECT('}')) {
                VD_XR_SKIP();

                if (VD_XR_CURSUB("\"name\"")) { VD_XR_SKIP(); VD_XR_MUST(':'); VD_XR_SKIP();

                    VD_XR_MUST('\"');

                    file->data.runtime.name_len = 0;
                    VD_XR_STR(file->data.runtime.name, file->data.runtime.name_len);

                    VD_XR_MUST('\"');
                    VD_XR_SKIP();
                    VD_XR_MUST(',');
                } else if (VD_XR_CURSUB("\"library_path\"")) { VD_XR_SKIP(); VD_XR_MUST(':'); VD_XR_SKIP();

                    VD_XR_MUST('\"');

                    file->data.runtime.library_path_len = 0;
                    VD_XR_STR(file->data.runtime.library_path, file->data.runtime.library_path_len);

                    VD_XR_MUST('\"');
                    VD_XR_SKIP();
                    VD_XR_MUST(',');
                }
            }
            
            VD_XR_MUST('}');
        }
    }

    VD_XR_MUST('}');

    return 1;

#undef VD_XR_EXPECT
#undef VD_XR_MUST
#undef VD_XR_PEEK
#undef VD_XR_NEXT
#undef VD_XR_SKIP
#undef VD_XR_DONE
#undef VD_XR_CURSUB
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

        case VD_XR__LOADER_PROPERTY_XR_RUNTIME_JSON_ENV_VAR: {
            env_var_name = OPENXR_RUNTIME_JSON_ENV_VAR;
            env_var_name_len = sizeof(OPENXR_RUNTIME_JSON_ENV_VAR) - 1;
        } break;

#if !defined(XR_OS_WINDOWS) && !defined(XR_OS_ANDROID)
        case VD_XR__LOADER_PROPERTY_XDG_CONFIG_DIRS: {
            env_var_name = VD_XR__LOADER_PROPERTY_XDG_CONFIG_DIRS_NAME;
            env_var_name_len = sizeof(VD_XR__LOADER_PROPERTY_XDG_CONFIG_DIRS_NAME) - 1;
        } break;

        case VD_XR__LOADER_PROPERTY_XDG_DATA_DIRS: {
            env_var_name = VD_XR__LOADER_PROPERTY_XDG_DATA_DIRS_NAME;
            env_var_name_len = sizeof(VD_XR__LOADER_PROPERTY_XDG_DATA_DIRS_NAME) - 1;
        } break;

        case VD_XR__LOADER_PROPERTY_XDG_DATA_HOME: {
            env_var_name = VD_XR__LOADER_PROPERTY_XDG_DATA_HOME_NAME;
            env_var_name_len = sizeof(VD_XR__LOADER_PROPERTY_XDG_DATA_HOME_NAME) - 1;
        } break;

        case VD_XR__LOADER_PROPERTY_HOME: {
            env_var_name = VD_XR__LOADER_PROPERTY_HOME_NAME;
            env_var_name_len = sizeof(VD_XR__LOADER_PROPERTY_HOME_NAME) - 1;
        } break;
#endif // !defined(XR_OS_WINDOWS) && !defined(XR_OS_ANDROID)

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

static void vd_xr__copy_include_paths(VdXr__StringBuffer *buf, int is_directory_list, const char *cur_path, const char *rel_path)
{
    if (cur_path != 0) {
        int cur_path_len = vd_xr__strlen(cur_path);

        int last_found = 0;
        int found = vd_xr__str_first_of(cur_path, VD_XR__PATH_SEPARATOR);

        while (found != -1) {
            int len = found - last_found;

            vd_xr__string_buffer_push_range(buf, vd_xr__substr(cur_path, last_found, len));

            if (is_directory_list && (cur_path[found - 1] != '\\' && cur_path[found - 1] != '/')) {
                vd_xr__string_buffer_pushchar(buf, VD_XR__DIRECTORY_SYMBOL);
            }

            vd_xr__string_buffer_push(buf, rel_path);
            vd_xr__string_buffer_pushchar(buf, VD_XR__PATH_SEPARATOR);
        }

        int last_char = cur_path_len - 1;
        if (last_found != last_char) {
            vd_xr__string_buffer_push_range(buf, vd_xr__substr(cur_path, last_found, cur_path_len));

            if (is_directory_list && (cur_path[last_char] != '\\' && cur_path[last_char] != '/')) {
                vd_xr__string_buffer_pushchar(buf, VD_XR__DIRECTORY_SYMBOL);
            }

            vd_xr__string_buffer_push(buf, rel_path);
            vd_xr__string_buffer_pushchar(buf, VD_XR__PATH_SEPARATOR);
        }
    }
}

static VdXr__StringRange vd_xr__substr(const char *s, int i, int count)
{
    VdXr__StringRange result;
    result.s = s + i;

    if (count < 0) {
        count = vd_xr__strlen(s) - i;
    }

    result.l = count;
    return result;
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
    XrResult result;
    VdXr__ManifestFileList *manifest_files;
    XrExtensionProperties *props_array;
    VdXr__ArenaSave arena_save;

    vd_xr__init();

    manifest_files = &Vd_Xr_G.manbuf;
    props_array = 0;
    result = XR_SUCCESS;

    arena_save = vd_xr__arena_save(&Vd_Xr_G.arena);

    if (propertyCountOutput == 0)
    {
        result = XR_ERROR_VALIDATION_FAILURE;
    }

    int just_layer_props = 0;

    if ((layerName != 0) && vd_xr__strlen(layerName) != 0) {
        just_layer_props = 1;
    }

    // @note(mdodis): ApiLayerInterface::GetInstanceExtensionProperties
    if (XR_SUCCEEDED(result)) {
        result = vd_xr__find_manifest_files(VD_XR__MANIFEST_FILE_TYPE_IMPLICIT_API_LAYER, manifest_files);
        // <--
        
        if (XR_SUCCEEDED(result) && !just_layer_props) {
            result = vd_xr__load_runtime();

            if (XR_SUCCEEDED(result)) {
                vd_xr__get_instance_extension_properties_from_runtime();
            }
        }
    }


    if (XR_SUCCEEDED(result)) {

        if (just_layer_props) {

        } else {
        }
    }

    vd_xr__arena_restore(arena_save);
    return result;
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
    if (s == 0) {
        return 0;
    }

    while (*s++) r++;
    return r;
}

static int vd_xr__strncmp(const char *s, const char *t, int count)
{
    while ((*s) && (*t) && (count > 0)) {
        int d = *s - *t;

        if (d != 0) {
            return d;
        }

        s++;
        t++;
        count--;
    }

    return 0;
}

static int vd_xr__str_first_of(const char *s, char c)
{
    int i = 0;

    if (s == 0) {
        return -1;
    }

    while (*s) {
        if (*s == c) {
            return i;
        }
        i++;
        s++;
    }

    return -1;
}

static int vd_xr__str_first_of_with_start(const char *s, char c, int start)
{
    int i = start;
    s += start;

    while (*s) {
        if (*s == c) {
            return i;
        }
        i++;
        s++;
    }

    return -1;
}

void vd_xr__arena_init(VdXr__Arena *a, void *buf, size_t len)
{
    a->buf = (uint8_t*)buf;
    a->buf_len = len;
    a->curr_offset = 0;
    a->prev_offset = 0;
}

void *vd_xr__arena_alloc_align(VdXr__Arena *a, size_t size, size_t align)
{
    uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
    uintptr_t offset = vd_xr__align_forward(curr_ptr, align);
    offset -= (uintptr_t)a->buf;

    if (offset + size <= a->buf_len) {
        void *ptr = 0;
        ptr = &a->buf[offset];
        a->prev_offset = offset;
        a->curr_offset = offset + size;

        vd_xr__memset(ptr, 0, size);
        return ptr;
    }

    VD_XR_ABORT("vd_xr__arena_alloc_align");
    return 0;
}

void *vd_xr__arena_resize_align(VdXr__Arena *a, void *old_memory, size_t old_size, size_t new_size, size_t align)
{
    VD_XR_ASSERT(vd_xr__is_power_of_two(align));

    uint8_t* old_mem = (uint8_t*)old_memory;

    if (old_mem == 0 || old_size == 0) {
        return vd_xr__arena_alloc_align(a, new_size, align);
    } else if (a->buf <= old_mem && old_mem < a->buf + a->buf_len) {
        if (a->buf + a->prev_offset == old_mem) {
            a->curr_offset = a->prev_offset + new_size;
            if (new_size > old_size) {
                vd_xr__memset(&a->buf[a->curr_offset], 0, new_size - old_size);
            }

            return old_memory;
        } else {
            void *new_memory = vd_xr__arena_alloc_align(a, new_size, align);
            size_t copy_size = old_size < new_size ? old_size : new_size;

            vd_xr__memmove(new_memory, old_memory, copy_size);
            return new_memory;
        }
    } else {
        VD_XR_ABORT("vd_xr__arena_resize_align");
    }
    return 0;
}

void vd_xr__arena_clear(VdXr__Arena *a)
{
    vd_xr__memset(a->buf, 0, a->curr_offset);
    a->curr_offset = 0;
    a->prev_offset = 0;
}

int vd_xr__arena_free(VdXr__Arena *a, void *memory, size_t size)
{
    (void)(size);

    uintptr_t last_ptr = (uintptr_t)(a->buf + a->prev_offset);
    uintptr_t mptr     = (uintptr_t)memory;

    if (mptr == last_ptr) {
        a->curr_offset = a->prev_offset;
        a->prev_offset = 0;
        return 1;
    }

    return 0;
}

static void vd_xr__init(void)
{
    if (Vd_Xr_G.initialized) {
        return;    
    }

    Vd_Xr_G.initialized = 1;

#if XR_OS_WINDOWS
    vd_xr__win32_init();
#endif

    Vd_Xr_G.arena = vd_xr__arena_from_malloc(1024 * 1024 * 4);
}

#endif // VD_XR_IMPL