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
#define VD_FC_VERSION_PATCH    1
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

#include <stdint.h>

typedef enum {
    VD_FC_TYPE_INVALID = -1,
    VD_FC_TYPE_CONTROLLER = 0,
    VD_FC_TYPE_WHEEL,
    VD_FC_TYPE_MAX,
} VdFcType;

typedef enum {
    VD_FC_CLASS_INVALID = -1,
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

typedef struct {
    int         idx;
    uint64_t    uid;
} VdFcIndexUID;

typedef union {
    uint8_t dat[16];
    struct {
        uint16_t bus;
        uint16_t crc;
        uint16_t vendor_id;
        uint16_t reserved0;
        uint16_t product_id;
        uint16_t reserved1;
        uint16_t version;
        uint8_t  driver_signature;
        uint8_t  driver_data;
    } parts;
} VdFcGuid;

/**
 * @brief Initialize the library.
 */
VD_FC_API void              vd_fc_init(void);

/**
 * @brief Poll for changes
 */
VD_FC_API void              vd_fc_poll(void);

/**
 * @brief Get device count
 * @return  The number of devices (max VD_FC_DEVICE_COUNT_MAX)
 */
VD_FC_API int               vd_fc_count(void);

/**
 * @brief Deinitialize the library.
 */
VD_FC_API void              vd_fc_quit(void);

/* ----DEVICE CHANGES------------------------------------------------------------------------------------------------ */
/**
 * @brief Get which device IDs where lost or disconnected
 * @param  count (Optional) Count of IDs
 * @return       The lost devices array
 */
VD_FC_API VdFcIndexUID*     vd_fc_dropped(int *count);

/**
 * @brief Get which device IDs where connected
 * @param  count (Optional) Count of IDs
 * @return       The connected devices array
 */
VD_FC_API VdFcIndexUID*     vd_fc_arrived(int *count);

/* ----DEVICE PROPERTIES--------------------------------------------------------------------------------------------- */
VD_FC_API VdFcGuid          vd_fc_guid(int id);
VD_FC_API VdFcType          vd_fc_type(int id);
VD_FC_API VdFcClass         vd_fc_class(int id);
VD_FC_API int               vd_fc_button_count(int id);
VD_FC_API int               vd_fc_hat_count(int id);
VD_FC_API int               vd_fc_axis_count(int id);
VD_FC_API uint64_t          vd_fc_uid(int id);
VD_FC_API const char*       vd_fc_driver(int id);
VD_FC_API int               vd_fc_id_from_uid(uint64_t uid);
VD_FC_API const char*       vd_fc_name(int id);

/* ----DEVICE FORCE FEEDBACK----------------------------------------------------------------------------------------- */
typedef struct {
    float left_motor;
    float right_motor;
    float left_trigger;
    float right_trigger;
} VdFcRumbleState;

VD_FC_API int               vd_fc_ff_rumble_supported(int id);
VD_FC_API float             vd_fc_ff_gain_get(int id);
VD_FC_API void              vd_fc_ff_gain_set(int id, float gain);
VD_FC_API void              vd_fc_ff_rumble_set(int id, VdFcRumbleState *rumble);
VD_FC_API void              vd_fc_ff_rumble_set_motors(int id, float left_motor, float right_motor);
VD_FC_API VdFcRumbleState   vd_fc_ff_rumble_get(int id);
VD_FC_API int               vd_fc_ff_rumble_any(int id);

/* ----SYMBOLIC INPUTS----------------------------------------------------------------------------------------------- */
typedef struct {
    float         axes[34];
    unsigned char buttons[160]; // 128 Buttons + 4 hats of 8 directions
} VdFcState;

VD_FC_API int               vd_fc_button_down(int id, int sym_button_id);
VD_FC_API unsigned int      vd_fc_button_press_count(int id, int sym_button_id);
VD_FC_API float             vd_fc_axis_value(int id, int axis_id);

/* ----RAW INPUTS---------------------------------------------------------------------------------------------------- */
VD_FC_API unsigned int      vd_fc_raw_button_down(int id, int button_id);
VD_FC_API unsigned int      vd_fc_raw_hat_value(int id, int hat_id);
VD_FC_API long long         vd_fc_raw_axis_value(int id, int axis_id);

/* ----MAPPINGS------------------------------------------------------------------------------------------------------ */
typedef enum {
    VD_FC_INPUT_TYPE_INVALID,

    // Read as 0 - 1, Written as 0 - 1
    VD_FC_INPUT_TYPE_BUTTON,

    // Read as [min, max], Written as 0.f - 1.f
    VD_FC_INPUT_TYPE_AXIS,

    // Read based on op
    // We only ever get input from here, and put it in the other two
    VD_FC_INPUT_TYPE_HAT,
} VdFcInputType;

typedef enum {
    VD_FC_OP_NONE = 0,
    VD_FC_OP_INVERT = 1 << 0,
} VdFcOp;

typedef struct {
    VdFcInputType type;       // Where to get/put the input (button, hat, axis)
    int           index;      // At which index to get/put the input [0, 128)
    long long     min_value;  // Min value of the reading
    long long     max_value;  // Max value of the reading
    VdFcOp        op;         // Additional processing
} VdFcInputDesc;

typedef struct {
    VdFcInputType type;       // Where to get/put the input (button, hat, axis)
    int           index;      // At which index to get/put the input [0, 128)
} VdFcOutputDesc;

typedef struct {
    VdFcInputDesc  src;
    VdFcOutputDesc dst;
} VdFcMappingEntry;

typedef struct {
    int              num_entries;
    VdFcMappingEntry entries[170];
} VdFcMapping;

typedef struct {
    VdFcGuid    guid;
    VdFcMapping map;
} VdFcDbMapping;

VD_FC_API void              vd_fc_mapping_add(VdFcMapping *mapping);

#endif // !VD_FC_H

#ifdef VD_FC_IMPL

#define VD_FC_ENDIANNESS_LE 1
#define VD_FC_ENDIANNESS_BE 0
#ifndef VD_FC_ENDIANNESS
#   if defined(_MSC_VER)
#       if defined(_M_IX86) || defined(_M_X64) || defined(_M_ARM) || defined(_M_ARM64)
#           define VD_FC_ENDIANNESS VD_FC_ENDIANNESS_LE
#       else
#           define VD_FC_ENDIANNESS VD_FC_ENDIANNESS_BE
#       endif
#   elif defined(__APPLE__)
#       define VD_FC_ENDIANNESS VD_FC_ENDIANNESS_LE
#   endif
#endif // VD_FC_ENDIANESS

#define VD_FC_SWAP16(x) ((uint16_t)((uint8_t)(x << 8) | (uint8_t)(x >> 8)))
#if VD_FC_ENDIANNESS == VD_FC_ENDIANNESS_LE
#   define VD_FC_SWAP16LE(x) (x)
#else
#   define VD_FC_SWAP16LE(x) VD_FC_SWAP16(x)
#endif

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

#ifndef VD_FC_REALLOC
#   include <stdlib.h>
#   define VD_FC_REALLOC(prev, prev_sz, new_sz) realloc(prev, new_sz)
#endif // !VD_FC_REALLOC

#ifndef VD_FC_ASSERT
#   include <assert.h>
#   define VD_FC_ASSERT(expr) assert(expr)
#endif // !VD_FC_ASSERT

#ifndef VD_FC_DEVICE_COUNT_MAX
#   define VD_FC_DEVICE_COUNT_MAX 16
#endif // !VD_FC_DEVICE_COUNT_MAX

#ifndef VD_FC_DEVICE_NAME_MAX
#   define VD_FC_DEVICE_NAME_MAX 64
#endif // !VD_FC_DEVICE_NAME_MAX

#ifndef VD_FC_WIN32_RAW_INPUT_BUFFER_COUNT
#   define VD_FC_WIN32_RAW_INPUT_BUFFER_COUNT 1024
#endif // !VD_FC_WIN32_RAW_INPUT_BUFFER_COUNT

/* ----CROSS PLATFORM PROCEDURES------------------------------------------------------------------------------------- */
typedef struct VdFc__Device             VdFc__Device;
typedef struct VdFc__CommonDeviceData   VdFc__CommonDeviceData;

enum {
    VD_FC__COMMON_DEVICE_FLAGS_PRODUCT_BITMASK = 0b111,
    VD_FC__COMMON_DEVICE_NONE                  = 0b001,
    VD_FC__COMMON_DEVICE_XBOX                  = 0b010,
    VD_FC__COMMON_DEVICE_DS4                   = 0b011,
    VD_FC__COMMON_DEVICE_FLAGS_BITMASK         = 0xFFFFFFF8,
    VD_FC__COMMON_DEVICE_WIRELESS              = 1 << 3,
};
typedef uint32_t VdFc__CommonDeviceFlags;

struct VdFc__CommonDeviceData {
    VdFc__CommonDeviceFlags flags;
    const char              *identified_name;
};

static int                      vd_fc_device_classify_auto(VdFc__Device *device);
static const char*              vd_fc_class_str(VdFcClass klass);
static uint16_t                 vd_fc__crc16(unsigned short crc, void *data, unsigned int len);
static uint16_t                 vd_fc__crc16_byte(uint8_t r);
static VdFcGuid                 vd_fc__make_gamepad_guid(uint16_t bus, uint16_t vendor, uint16_t product, uint16_t version,
                                                           char *vendor_name, char *product_name,
                                                           uint8_t driver_signature, uint8_t driver_data);
static int                      vd_fc__strlen(const char *s);
static size_t                   vd_fc__strlcpy(char *dst, const char *src, size_t maxlen);
static uint64_t                 vd_fc__uid_next(void);
static float                    vd_fc__clampf(float v, float m, float x);
static int                      vd_fc__approxeq(float a, float b);
static int                      vd_fc__approxeq_rumble(VdFcRumbleState *a, VdFcRumbleState *b);
static VdFc__CommonDeviceData   vd_fc__device_get_common_data(uint16_t vendor_id, uint16_t product_id, uint32_t output_report_size);

#ifdef _WIN32
#ifndef _MINWINDEF_
#define VD_FC_DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

typedef unsigned long        VdFcDWORD;
typedef int                  VdFcBOOL;
typedef unsigned char        VdFcBYTE;
typedef unsigned short      VdFcWORD;
typedef float               VdFcFLOAT;
typedef VdFcFLOAT*          VdFcPFLOAT;
typedef VdFcBOOL*           VdFcPBOOL;
typedef VdFcBOOL*           VdFcLPBOOL;
typedef VdFcBYTE*           VdFcPBYTE;
typedef VdFcBYTE*           VdFcLPBYTE;
typedef int*                VdFcPINT;
typedef int*                VdFcLPINT;
typedef VdFcWORD*           VdFcPWORD;
typedef VdFcWORD*           VdFcLPWORD;
typedef long*               VdFcLPLONG;
typedef VdFcDWORD*          VdFcPDWORD;
typedef VdFcDWORD*          VdFcLPDWORD;
typedef void*               VdFcLPVOID;
typedef const void*         VdFcLPCVOID;
typedef int                 VdFcINT;
typedef unsigned int        VdFcUINT;
typedef unsigned int*       VdFcPUINT;
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
typedef VdFcCHAR*           VdFcPCHAR;
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
VD_FC_DECLARE_HANDLE(VdFcHMENU);

typedef struct VdFc_FILETIME {
  VdFcDWORD dwLowDateTime;
  VdFcDWORD dwHighDateTime;
} VdFcFILETIME, *VdFcPFILETIME, *VdFcLPFILETIME;

typedef struct VdFc_OVERLAPPED {
    VdFcULONG_PTR Internal;
    VdFcULONG_PTR InternalHigh;
    union {
        struct {
          VdFcDWORD Offset;
          VdFcDWORD OffsetHigh;
        } DUMMYSTRUCTNAME;
        void* Pointer;
    } DUMMYUNIONNAME;
    VdFcHANDLE    hEvent;
} VdFcOVERLAPPED, *VdFcLPOVERLAPPED;

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
extern VdFcBOOL    DeviceIoControl(VdFcHANDLE hDevice, VdFcDWORD dwIoControlCode, void* lpInBuffer, VdFcDWORD nInBufferSize, void* lpOutBuffer, VdFcDWORD nOutBufferSize, VdFcLPDWORD lpBytesReturned, VdFcLPOVERLAPPED lpOverlapped);
extern VdFcBOOL    CancelIo(VdFcHANDLE hFile);
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
typedef USHORT      VdFcUSHORT;
typedef BOOLEAN     VdFcBOOLEAN;
typedef PUINT       VdFcPUINT;
typedef UCHAR       VdFcUCHAR;
typedef PUSHORT     VdFcPUSHORT;
typedef PULONG      VdFcPULONG;
typedef PCHAR       VdFcPCHAR;
typedef SHORT       VdFcSHORT;
typedef DWORD_PTR   VdFcDWORD_PTR;
typedef OVERLAPPED  VdFcOVERLAPPED;
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

#pragma pack(push, 1)
typedef struct VdFc_HIDP_DATA {
  VdFcUSHORT DataIndex;
  VdFcUSHORT Reserved;
  union {
    VdFcULONG   RawValue;
    VdFcBOOLEAN On;
  } dat;
} VdFcHIDP_DATA, *VdFcPHIDP_DATA;

typedef VdFcLONG                         VdFcNTSTATUS;
typedef struct VdFc_HIDP_PREPARSED_DATA* VdFcPHIDP_PREPARSED_DATA;
typedef VdFcUSHORT                       VdFcUSAGE, * VdFcPUSAGE;

#define VD_FC_FACILITY_HID_ERROR_CODE 0x11
#define VD_FC_HIDP_ERROR_CODES(SEV, CODE) \
        ((VdFcNTSTATUS) (((SEV) << 28) | (VD_FC_FACILITY_HID_ERROR_CODE << 16) | (CODE)))

#define VD_FC_HIDP_STATUS_SUCCESS                  (VD_FC_HIDP_ERROR_CODES(0x0,0))
#define VD_FC_HIDP_STATUS_NULL                     (VD_FC_HIDP_ERROR_CODES(0x8,1))

#define VD_FC_HIDP_STATUS_INVALID_PREPARSED_DATA   (VD_FC_HIDP_ERROR_CODES(0xC,1))
#define VD_FC_HIDP_STATUS_INVALID_REPORT_TYPE      (VD_FC_HIDP_ERROR_CODES(0xC,2))
#define VD_FC_HIDP_STATUS_INVALID_REPORT_LENGTH    (VD_FC_HIDP_ERROR_CODES(0xC,3))
#define VD_FC_HIDP_STATUS_USAGE_NOT_FOUND          (VD_FC_HIDP_ERROR_CODES(0xC,4))
#define VD_FC_HIDP_STATUS_VALUE_OUT_OF_RANGE       (VD_FC_HIDP_ERROR_CODES(0xC,5))
#define VD_FC_HIDP_STATUS_BAD_LOG_PHY_VALUES       (VD_FC_HIDP_ERROR_CODES(0xC,6))
#define VD_FC_HIDP_STATUS_BUFFER_TOO_SMALL         (VD_FC_HIDP_ERROR_CODES(0xC,7))
#define VD_FC_HIDP_STATUS_INTERNAL_ERROR           (VD_FC_HIDP_ERROR_CODES(0xC,8))
#define VD_FC_HIDP_STATUS_I8042_TRANS_UNKNOWN      (VD_FC_HIDP_ERROR_CODES(0xC,9))
#define VD_FC_HIDP_STATUS_INCOMPATIBLE_REPORT_ID   (VD_FC_HIDP_ERROR_CODES(0xC,0xA))
#define VD_FC_HIDP_STATUS_NOT_VALUE_ARRAY          (VD_FC_HIDP_ERROR_CODES(0xC,0xB))
#define VD_FC_HIDP_STATUS_IS_VALUE_ARRAY           (VD_FC_HIDP_ERROR_CODES(0xC,0xC))
#define VD_FC_HIDP_STATUS_DATA_INDEX_NOT_FOUND     (VD_FC_HIDP_ERROR_CODES(0xC,0xD))
#define VD_FC_HIDP_STATUS_DATA_INDEX_OUT_OF_RANGE  (VD_FC_HIDP_ERROR_CODES(0xC,0xE))
#define VD_FC_HIDP_STATUS_BUTTON_NOT_PRESSED       (VD_FC_HIDP_ERROR_CODES(0xC,0xF))
#define VD_FC_HIDP_STATUS_REPORT_DOES_NOT_EXIST    (VD_FC_HIDP_ERROR_CODES(0xC,0x10))
#define VD_FC_HIDP_STATUS_NOT_IMPLEMENTED          (VD_FC_HIDP_ERROR_CODES(0xC,0x20))
#define VD_FC_HIDP_STATUS_NOT_BUTTON_ARRAY         (VD_FC_HIDP_ERROR_CODES(0xC,0x21))
#define VD_FC_RIDI_PREPARSEDDATA                    0x20000005
#define VD_FC_RIDI_DEVICENAME                       0x20000007
#define VD_FC_RIDI_DEVICEINFO                       0x2000000b

typedef enum VdFc_HIDP_REPORT_TYPE
{
    VdFcHidP_Input,
    VdFcHidP_Output,
    VdFcHidP_Feature
} VdFcHIDP_REPORT_TYPE;

typedef struct VdFc_HIDP_CAPS
{
    VdFcUSAGE    Usage;
    VdFcUSAGE    UsagePage;
    VdFcUSHORT   InputReportByteLength;
    VdFcUSHORT   OutputReportByteLength;
    VdFcUSHORT   FeatureReportByteLength;
    VdFcUSHORT   Reserved[17];
    VdFcUSHORT   NumberLinkCollectionNodes;
    VdFcUSHORT   NumberInputButtonCaps;
    VdFcUSHORT   NumberInputValueCaps;
    VdFcUSHORT   NumberInputDataIndices;
    VdFcUSHORT   NumberOutputButtonCaps;
    VdFcUSHORT   NumberOutputValueCaps;
    VdFcUSHORT   NumberOutputDataIndices;
    VdFcUSHORT   NumberFeatureButtonCaps;
    VdFcUSHORT   NumberFeatureValueCaps;
    VdFcUSHORT   NumberFeatureDataIndices;
} VdFcHIDP_CAPS, * VdFcPHIDP_CAPS;

typedef struct VdFc_HIDP_BUTTON_CAPS
{
    VdFcUSAGE    UsagePage;
    VdFcUCHAR    ReportID;
    VdFcBOOLEAN  IsAlias;
    VdFcUSHORT   BitField;
    VdFcUSHORT   LinkCollection;
    VdFcUSAGE    LinkUsage;
    VdFcUSAGE    LinkUsagePage;
    VdFcBOOLEAN  IsRange;
    VdFcBOOLEAN  IsStringRange;
    VdFcBOOLEAN  IsDesignatorRange;
    VdFcBOOLEAN  IsAbsolute;
    VdFcUSHORT   ReportCount;
    VdFcUSHORT   Reserved2;
    VdFcULONG    Reserved[9];
    union {
        struct {
            VdFcUSAGE    UsageMin, UsageMax;
            VdFcUSHORT   StringMin, StringMax;
            VdFcUSHORT   DesignatorMin, DesignatorMax;
            VdFcUSHORT   DataIndexMin, DataIndexMax;
        } Range;
        struct {
            VdFcUSAGE    Usage, Reserved1;
            VdFcUSHORT   StringIndex, Reserved2;
            VdFcUSHORT   DesignatorIndex, Reserved3;
            VdFcUSHORT   DataIndex, Reserved4;
        } NotRange;
    } v;
} VdFcHIDP_BUTTON_CAPS, * VdFcPHIDP_BUTTON_CAPS;

typedef struct VdFc_HIDP_VALUE_CAPS
{
    VdFcUSAGE    UsagePage;
    VdFcUCHAR    ReportID;
    VdFcBOOLEAN  IsAlias;
    VdFcUSHORT   BitField;
    VdFcUSHORT   LinkCollection;
    VdFcUSAGE    LinkUsage;
    VdFcUSAGE    LinkUsagePage;
    VdFcBOOLEAN  IsRange;
    VdFcBOOLEAN  IsStringRange;
    VdFcBOOLEAN  IsDesignatorRange;
    VdFcBOOLEAN  IsAbsolute;
    VdFcBOOLEAN  HasNull;
    VdFcUCHAR    Reserved;
    VdFcUSHORT   BitSize;
    VdFcUSHORT   ReportCount;
    VdFcUSHORT   Reserved2[5];
    VdFcULONG    UnitsExp;
    VdFcULONG    Units;
    VdFcLONG     LogicalMin, LogicalMax;
    VdFcLONG     PhysicalMin, PhysicalMax;

    union {
        struct {
            VdFcUSAGE    UsageMin, UsageMax;
            VdFcUSHORT   StringMin, StringMax;
            VdFcUSHORT   DesignatorMin, DesignatorMax;
            VdFcUSHORT   DataIndexMin, DataIndexMax;
        } Range;

        struct {
            VdFcUSAGE    Usage, Reserved1;
            VdFcUSHORT   StringIndex, Reserved2;
            VdFcUSHORT   DesignatorIndex, Reserved3;
            VdFcUSHORT   DataIndex, Reserved4;
        } NotRange;
    } v;
} VdFcHIDP_VALUE_CAPS, * VdFcPHIDP_VALUE_CAPS;

typedef struct VdFctagRAWINPUTDEVICE {
    VdFcUSHORT usUsagePage; // Toplevel collection UsagePage
    VdFcUSHORT usUsage;     // Toplevel collection Usage
    VdFcDWORD dwFlags;
    VdFcHWND hwndTarget;    // Target hwnd. NULL = follows keyboard focus
} VdFcRAWINPUTDEVICE, * VdFcPRAWINPUTDEVICE, * VdFcLPRAWINPUTDEVICE;
typedef const VdFcRAWINPUTDEVICE* VdFcPCRAWINPUTDEVICE;

typedef struct VdFctagRAWINPUTHEADER {
    VdFcDWORD dwType;
    VdFcDWORD dwSize;
    VdFcHANDLE hDevice;
    VdFcWPARAM wParam;
} VdFcRAWINPUTHEADER, * VdFcPRAWINPUTHEADER, * VdFcLPRAWINPUTHEADER;

#pragma pack(push, 8)
typedef struct VdFctagRAWMOUSE {
    VdFcUSHORT usFlags;
    union {
        VdFcULONG ulButtons;
        struct {
            VdFcUSHORT  usButtonFlags;
            VdFcUSHORT  usButtonData;
        } fd;
    } v;
    VdFcULONG ulRawButtons;
    VdFcLONG lLastX;
    VdFcLONG lLastY;
    VdFcULONG ulExtraInformation;

} VdFcRAWMOUSE, * VdFcPRAWMOUSE, * VdFcLPRAWMOUSE;
#pragma pack(pop)

typedef struct VdFctagRAWKEYBOARD {
    VdFcUSHORT MakeCode;
    VdFcUSHORT Flags;
    VdFcUSHORT Reserved;
    VdFcUSHORT VKey;
    VdFcUINT   Message;
    VdFcULONG ExtraInformation;
} VdFcRAWKEYBOARD, * VdFcPRAWKEYBOARD, * VdFcLPRAWKEYBOARD;

typedef struct VdFctagRAWHID {
    VdFcDWORD dwSizeHid;
    VdFcDWORD dwCount;
    VdFcBYTE bRawData[1];
} VdFcRAWHID, * VdFcPRAWHID, * VdFcLPRAWHID;

typedef struct VdFctagRAWINPUT {
    VdFcRAWINPUTHEADER header;
    union {
        VdFcRAWMOUSE    mouse;
        VdFcRAWKEYBOARD keyboard;
        VdFcRAWHID      hid;
    } data;
} VdFcRAWINPUT, * VdFcPRAWINPUT, * VdFcLPRAWINPUT;

typedef struct VdFctagRID_DEVICE_INFO_MOUSE {
    VdFcDWORD dwId;
    VdFcDWORD dwNumberOfButtons;
    VdFcDWORD dwSampleRate;
    VdFcBOOL  fHasHorizontalWheel;
} VdFcRID_DEVICE_INFO_MOUSE, * VdFcPRID_DEVICE_INFO_MOUSE;

typedef struct VdFctagRID_DEVICE_INFO_KEYBOARD {
    VdFcDWORD dwType;
    VdFcDWORD dwSubType;
    VdFcDWORD dwKeyboardMode;
    VdFcDWORD dwNumberOfFunctionKeys;
    VdFcDWORD dwNumberOfIndicators;
    VdFcDWORD dwNumberOfKeysTotal;
} VdFcRID_DEVICE_INFO_KEYBOARD, * VdFcPRID_DEVICE_INFO_KEYBOARD;

typedef struct VdFctagRID_DEVICE_INFO_HID {
    VdFcDWORD dwVendorId;
    VdFcDWORD dwProductId;
    VdFcDWORD dwVersionNumber;
    VdFcUSHORT usUsagePage;
    VdFcUSHORT usUsage;
} VdFcRID_DEVICE_INFO_HID, * VdFcPRID_DEVICE_INFO_HID;

typedef struct VdFctagRID_DEVICE_INFO {
    VdFcDWORD cbSize;
    VdFcDWORD dwType;
    union {
        VdFcRID_DEVICE_INFO_MOUSE mouse;
        VdFcRID_DEVICE_INFO_KEYBOARD keyboard;
        VdFcRID_DEVICE_INFO_HID hid;
    } v;
} VdFcRID_DEVICE_INFO, * VdFcPRID_DEVICE_INFO, * VdFcLPRID_DEVICE_INFO;

#pragma pack(push, 8)
typedef struct VdFctagRAWINPUTDEVICELIST {
  VdFcHANDLE hDevice;
  VdFcDWORD  dwType;
} VdFcRAWINPUTDEVICELIST, *VdFcPRAWINPUTDEVICELIST;
#pragma pack(pop)

typedef struct VdFc_DEV_BROADCAST_DEVICEINTERFACE_A {
  VdFcDWORD dbcc_size;
  VdFcDWORD dbcc_devicetype;
  VdFcDWORD dbcc_reserved;
  VdFcGUID  dbcc_classguid;
  char  dbcc_name[1];
} VdFcDEV_BROADCAST_DEVICEINTERFACE_A, *VdFcPDEV_BROADCAST_DEVICEINTERFACE_A;

struct VdFc_DEV_BROADCAST_HDR {     /* */
    VdFcDWORD       dbch_size;
    VdFcDWORD       dbch_devicetype;
    VdFcDWORD       dbch_reserved;
};

typedef struct  VdFc_DEV_BROADCAST_HDR      VdFcDEV_BROADCAST_HDR;
typedef         VdFcDEV_BROADCAST_HDR       *VdFcPDEV_BROADCAST_HDR;

#pragma pack(pop)

#define VD_FC__WIN32_FUNCTIONS \
    V("dinput8.dll") \
    X(VdFcHRESULT,  DirectInput8Create, (VdFcHINSTANCE hinst, VdFcDWORD dwVersion, VdFcREFIID riidltf, VdFcLPVOID *ppvOut, VdFcLPUNKNOWN punkOuter)) \
    VE() \
    V("User32.dll") \
    X(VdFcATOM,     RegisterClassW, (const VdFc__Win32_WNDCLASSW*)) \
    X(VdFcBOOL,     UnregisterClassW, (VdFcLPCWSTR lpClassName, VdFcHINSTANCE hInstance)) \
    X(VdFcHWND,     CreateWindowExW, (DWORD dwExStyle, VdFcLPCWSTR lpClassName, VdFcLPCWSTR lpWindowName, VdFcDWORD dwStyle, int X, int Y, int nWidth, int nHeight, VdFcHWND hWndParent, VdFcHMENU hMenu, VdFcHINSTANCE hInstance, VdFcLPVOID lpParam)) \
    X(VdFcLRESULT,  DefWindowProcW, (VdFcHWND hWnd, VdFcUINT Msg, VdFcWPARAM wParam, VdFcLPARAM lParam)) \
    X(VdFcBOOL,     GetMessageW, (VdFcLPMSG lpMsg, VdFcHWND hWnd, VdFcUINT wMsgFilterMin, VdFcUINT wMsgFilterMax)) \
    X(VdFcBOOL,     TranslateMessage, (const VdFcMSG* lpMsg)) \
    X(VdFcLRESULT,  DispatchMessageW, (const VdFcMSG* lpMsg)) \
    X(VdFcBOOL,     PeekMessageW, (VdFcLPMSG lpMsg, VdFcHWND hWnd, VdFcUINT wMsgFilterMin, VdFcUINT wMsgFilterMax, VdFcUINT wRemoveMsg)) \
    X(VdFcBOOL,     RegisterRawInputDevices, (VdFcPCRAWINPUTDEVICE pRawInputDevices, VdFcUINT uiNumDevices, VdFcUINT cbSize)) \
    X(VdFcUINT,     GetRawInputData, (VdFcHANDLE hRawInput, VdFcUINT uiCommand, VdFcLPVOID pData, VdFcPUINT pcbSize, VdFcUINT cbSizeHeader)) \
    X(VdFcUINT,     GetRawInputDeviceInfoA, (VdFcHANDLE hDevice, VdFcUINT uiCommand, VdFcLPVOID pData, VdFcPUINT pcbSize)) \
    X(VdFcUINT,     GetRawInputDeviceInfoW, (VdFcHANDLE hDevice, VdFcUINT uiCommand, VdFcLPVOID pData, VdFcPUINT pcbSize)) \
    X(VdFcUINT,     GetRawInputDeviceList, (VdFcPRAWINPUTDEVICELIST pRawInputDeviceList, VdFcPUINT puiNumDevices, VdFcUINT cbSize)) \
    X(void*,        RegisterDeviceNotificationA, (VdFcHANDLE hRecipient, void *NotificationFilter, VdFcDWORD Flags)) \
    X(VdFcBOOL,     DestroyWindow, (VdFcHWND hWnd)) \
    VE() \
    V("Hid.dll") \
    X(VdFcNTSTATUS, HidP_GetCaps, (VdFcPHIDP_PREPARSED_DATA PreparsedData, VdFcPHIDP_CAPS Capabilities)) \
    X(VdFcNTSTATUS, HidP_GetButtonCaps, (VdFcHIDP_REPORT_TYPE ReportType, VdFcPHIDP_BUTTON_CAPS ButtonCaps, VdFcPUSHORT ButtonCapsLength, VdFcPHIDP_PREPARSED_DATA PreparsedData)) \
    X(VdFcNTSTATUS, HidP_GetValueCaps, (VdFcHIDP_REPORT_TYPE ReportType, VdFcPHIDP_VALUE_CAPS ValueCaps, VdFcPUSHORT ValueCapsLength, VdFcPHIDP_PREPARSED_DATA PreparsedData)) \
    X(VdFcNTSTATUS, HidP_GetUsages, (VdFcHIDP_REPORT_TYPE ReportType, VdFcUSAGE UsagePage, VdFcUSHORT LinkCollection, VdFcPUSAGE UsageList, VdFcPULONG UsageLength, VdFcPHIDP_PREPARSED_DATA PreparsedData, VdFcPCHAR Report, VdFcULONG ReportLength)) \
    X(VdFcNTSTATUS, HidP_GetData, (VdFcHIDP_REPORT_TYPE ReportType, VdFcPHIDP_DATA DataList, VdFcPULONG DataLength, VdFcPHIDP_PREPARSED_DATA PreparsedData, VdFcPCHAR Report, VdFcULONG ReportLength)) \
    X(VdFcULONG,    HidP_MaxUsageListLength, (VdFcHIDP_REPORT_TYPE ReportType, VdFcUSAGE UsagePage, VdFcPHIDP_PREPARSED_DATA PreparsedData)) \
    X(VdFcULONG,    HidP_MaxDataListLength, (VdFcHIDP_REPORT_TYPE ReportType, VdFcPHIDP_PREPARSED_DATA PreparsedData)) \
    X(VdFcNTSTATUS, HidP_GetUsageValue, (VdFcHIDP_REPORT_TYPE ReportType, VdFcUSAGE UsagePage, VdFcUSHORT LinkCollection, VdFcUSAGE Usage, VdFcPULONG UsageValue, VdFcPHIDP_PREPARSED_DATA PreparsedData, VdFcPCHAR Report, VdFcULONG ReportLength)) \
    X(VdFcBOOLEAN,  HidD_GetManufacturerString, (VdFcHANDLE HidDeviceObject, void *Buffer, VdFcULONG BufferLength)) \
    X(VdFcBOOLEAN,  HidD_GetProductString, (VdFcHANDLE HidDeviceObject, void *Buffer, VdFcULONG BufferLength)) \
    X(VdFcBOOLEAN,  HidD_SetFeature, (VdFcHANDLE HidDeviceObject, void *ReportBuffer, VdFcULONG ReportBufferLength)) \
    X(VdFcBOOLEAN,  HidD_SetOutputReport, (VdFcHANDLE HidDeviceObject, void *ReportBuffer, VdFcULONG ReportBufferLength)) \
    X(VdFcBOOLEAN,  HidD_GetPreparsedData, (VdFcHANDLE HidDeviceObject, VdFcPHIDP_PREPARSED_DATA *PreparsedData)) \
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

#define VD_FC_XINPUT_GAMEPAD_DPAD_UP          0x0001
#define VD_FC_XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define VD_FC_XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define VD_FC_XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define VD_FC_XINPUT_GAMEPAD_START            0x0010
#define VD_FC_XINPUT_GAMEPAD_BACK             0x0020
#define VD_FC_XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define VD_FC_XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define VD_FC_XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define VD_FC_XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200
#define VD_FC_XINPUT_GAMEPAD_A                0x1000
#define VD_FC_XINPUT_GAMEPAD_B                0x2000
#define VD_FC_XINPUT_GAMEPAD_X                0x4000
#define VD_FC_XINPUT_GAMEPAD_Y                0x8000
#define VD_FC_XINPUT_MAX_GAMEPADS             4

typedef struct VdFc_XINPUT_GAMEPAD {
    VdFcWORD  wButtons;
    VdFcBYTE  bLeftTrigger;
    VdFcBYTE  bRightTrigger;
    VdFcSHORT sThumbLX;
    VdFcSHORT sThumbLY;
    VdFcSHORT sThumbRX;
    VdFcSHORT sThumbRY;
} VdFcXINPUT_GAMEPAD, * VdFcPXINPUT_GAMEPAD;

typedef struct VdFc_XINPUT_STATE {
    VdFcDWORD          dwPacketNumber;
    VdFcXINPUT_GAMEPAD Gamepad;
} VdFcXINPUT_STATE, * VdFcPXINPUT_STATE;

typedef struct VdFc_XINPUT_VIBRATION {
    VdFcWORD wLeftMotorSpeed;
    VdFcWORD wRightMotorSpeed;
} VdFcXINPUT_VIBRATION, *VdFcPXINPUT_VIBRATION;

#define VD_FC_PROC_XInputGetState(name) VdFcDWORD name(VdFcDWORD dwUserIndex, VdFcXINPUT_STATE* pState)
typedef VD_FC_PROC_XInputGetState(VdFcProcXInputGetState);
static VdFcProcXInputGetState *VdFcXInputGetState;

#define VD_FC_PROC_XInputSetState(name) VdFcDWORD name(VdFcDWORD dwUserIndex, VdFcXINPUT_VIBRATION* pVibration)
typedef VD_FC_PROC_XInputSetState(VdFcProcXInputSetState);
static VdFcProcXInputSetState *VdFcXInputSetState;

#define VD_FC_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) VdFcGUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

typedef struct VdFc__Win32_IUnknown VdFc__Win32_IUnknown;

typedef struct VdFc__Win32_IDirectInput8W VdFc__Win32_IDirectInput8W;
VD_FC_DEFINE_GUID(VdFc__Win32_IID_IDirectInput8W,0xBF798031,0x483A,0x4DA2,0xAA,0x99,0x5D,0x64,0xED,0x36,0x97,0x00);

typedef struct VdFc__Win32_IDirectInputDevice8W VdFc__Win32_IDirectInputDevice8W;
VD_FC_DEFINE_GUID(VdFc__Win32_IID_IDirectInputDevice8W,0x54D41081,0xDC15,0x4833,0xA4,0x1B,0x74,0x8F,0x73,0xA3,0x81,0x79);

VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_XAxis,    0xa36d02e0,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_YAxis,    0xa36d02e1,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_ZAxis,    0xa36d02e2,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_RxAxis,   0xa36d02f4,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_RyAxis,   0xa36d02f5,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_RzAxis,   0xa36d02e3,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_Slider,   0xa36d02e4,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__Win32_Guid_PoV,      0xa36d02f2,0xc9f3,0x11cf,0xbf,0xc7,0x44,0x45,0x53,0x54,0x00,0x00);
VD_FC_DEFINE_GUID(Vd_Fc__WIn32_Guid_HIDClass, 0x745A17A0,0x74D3,0x11D0,0xB6,0xFE,0x00,0xA0,0xC9,0x0F,0x57,0xDA);

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

#define VD_FC__WIN32_MAKEWORD(a, b)      ((VdFcWORD)(((VdFcBYTE)(((VdFcDWORD_PTR)(a)) & 0xff)) | ((VdFcWORD)((VdFcBYTE)(((VdFcDWORD_PTR)(b)) & 0xff))) << 8))
#define VD_FC__WIN32_MAKELONG(a, b)      ((VdFcLONG)(((VdFcWORD)(((VdFcDWORD_PTR)(a)) & 0xffff)) | ((VdFcDWORD)((VdFcWORD)(((VdFcDWORD_PTR)(b)) & 0xffff))) << 16))
#define VD_FC__WIN32_LOWORD(l)           ((VdFcWORD)(((VdFcDWORD_PTR)(l)) & 0xffff))
#define VD_FC__WIN32_HIWORD(l)           ((VdFcWORD)((((VdFcDWORD_PTR)(l)) >> 16) & 0xffff))
#define VD_FC__WIN32_LOBYTE(w)           ((VdFcBYTE)(((VdFcDWORD_PTR)(w)) & 0xff))
#define VD_FC__WIN32_HIBYTE(w)           ((VdFcBYTE)((((VdFcDWORD_PTR)(w)) >> 8) & 0xff))

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

static unsigned int Vd_Fc__Win32_DIJOYSTATE2_AxisOffsets[] = {
    VD_FC__WIN32_DIJOFS_X,
    VD_FC__WIN32_DIJOFS_Y,
    VD_FC__WIN32_DIJOFS_Z,
    VD_FC__WIN32_DIJOFS_RX,
    VD_FC__WIN32_DIJOFS_RY,
    VD_FC__WIN32_DIJOFS_RZ,
    VD_FC__WIN32_DIJOFS_SLIDER(0),
    VD_FC__WIN32_DIJOFS_SLIDER(1),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVX),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVY),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVZ),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVRx),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVRy),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lVRz),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglVSlider[0]),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglVSlider[1]),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lAX),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lAY),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lAZ),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lARx),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lARy),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lARz),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglASlider[0]),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglASlider[1]),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFX),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFY),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFZ),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFRx),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFRy),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,lFRz),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglFSlider[0]),
    VD_FC__WIN32_FIELD_OFFSET(VdFc__Win32_DIJOYSTATE2,rglFSlider[1]),
};

static VdFc__Win32_DIDATAFORMAT Vd_Fc__Win32_DIJOYSTATE2_Data_Format = {
    sizeof(VdFc__Win32_DIDATAFORMAT),
    sizeof(VdFc__Win32_DIOBJECTDATAFORMAT),
    VD_FC__WIN32_DIDFT_ABSAXIS,
    sizeof(VdFc__Win32_DIJOYSTATE2),
    sizeof(Vd_Fc__Win32_DIJOYSTATE2_Data_Formats) / sizeof(Vd_Fc__Win32_DIJOYSTATE2_Data_Formats[0]),
    Vd_Fc__Win32_DIJOYSTATE2_Data_Formats,
};

typedef enum {
    VD_FC__WIN32_DEVICE_DRIVER_NONE,

    // DirectInput8
    VD_FC__WIN32_DEVICE_DRIVER_DINPUT,

    // Raw Input w/ XInput
    VD_FC__WIN32_DEVICE_DRIVER_RINPUT,
} VdFc__Win32DeviceDriver;

typedef enum {
    VD_FC__WIN32_OUTPUT_METHOD_NONE = 0,
    VD_FC__WIN32_OUTPUT_METHOD_XBOX_DEVICE_IO_CONTROL,
    VD_FC__WIN32_OUTPUT_METHOD_XINPUT_SET_STATE,
    VD_FC__WIN32_OUTPUT_METHOD_DUALSHOCK_WRITE_FILE,
} VdFc__Win32OutputMethod;

typedef struct {
    VdFcXINPUT_STATE state;
    int connected;
} VdFc__Win32XInputState;

typedef struct {
    VdFc__Win32_DIJOYSTATE2 state;
    VdFcGUID instance_guid;
} VdFc__Win32DeviceDirectInputData;

typedef struct {
    uint16_t data_index;
    uint8_t  state;
} VdFc__Win32DeviceRawInputButton;

typedef struct {
    uint16_t data_index;
    uint8_t  state;
} VdFc__Win32DeviceRawInputHat;

typedef struct {
    uint16_t data_index;
    int      state;
} VdFc__Win32DeviceRawInputAxis;

typedef struct {
    int                                 hidp_data_count;
    VdFcHIDP_DATA                       hidp_data[166];
    VdFc__Win32DeviceRawInputButton     buttons[128];
    VdFc__Win32DeviceRawInputHat        hats[5];
    VdFc__Win32DeviceRawInputAxis       axes[33];
    int                                 is_xinput_device;
    int                                 is_xbox;
    int                                 correlation_idx;
    uint32_t                            correlation_button_states;
    int                                 splitz;
    uint16_t                            splitz_idx;
    VdFcHANDLE                          devhandle;
} VdFc__Win32DeviceRawInputData;

typedef union {
    VdFc__Win32DeviceDirectInputData    dinput;
    VdFc__Win32DeviceRawInputData       rinput;
} VdFc__Win32DeviceData;

struct VdFc__Device {
    // Driver & Basic Info
    VdFc__Win32DeviceDriver             driver;
    int                                 present;
    int                                 just_arrived;

    // RAWINPUT
    VdFcUINT                            ppd_size;
    VdFcPHIDP_PREPARSED_DATA            ppd;

    // DirectInput
    VdFc__Win32_IDirectInputDevice8W    *dinput_device;

    VdFcOVERLAPPED                      overlapped;
    VdFcHANDLE                          write_handle;
    VdFc__Win32OutputMethod             output_method;

    // Data Unions based on .driver
    VdFc__Win32DeviceData               data;

    // Data - Same declarations across platforms
    VdFc__CommonDeviceData              common;
    VdFcGuid                            guid;
    VdFcClass                           klass;
    int                                 num_buttons;
    int                                 num_hats;
    int                                 num_axes;
    int                                 num_name;
    char                                name[VD_FC_DEVICE_NAME_MAX];
    uint64_t                            uid;
    float                               ff_gain;
    VdFcRumbleState                     ff_rumble;
    int                                 ff_rumble_resubmit;
};

typedef struct {
    int                                 has_polled_once;

    // Dummy Window
    VdFcHWND                            hwnd;
    void*                               hdev_notify;

    // Direct Input
    VdFc__Win32_IDirectInput8W          *dinput;
    int                                 dinput_enumerate_devices;

    // RAWINPUT + XInput
    VdFcRAWINPUTDEVICELIST              raw_input_device_list[128];
    VdFcRAWINPUT                        raw_input_buffer[VD_FC_WIN32_RAW_INPUT_BUFFER_COUNT];
    VdFcHMODULE                         xinput;
    int                                 xinput_correlation_cleared_this_frame;
    int                                 xinput_polled_this_frame;
    VdFc__Win32XInputState              xinput_states[4];

    // Devices - Same declarations across platforms
    VdFc__Device                        devices[VD_FC_DEVICE_COUNT_MAX];
    int                                 num_devices;

    int                                 num_devices_dropped;
    VdFcIndexUID                        devices_dropped[VD_FC_DEVICE_COUNT_MAX];

    int                                 num_devices_arrived;
    VdFcIndexUID                        devices_arrived[VD_FC_DEVICE_COUNT_MAX];
    uint64_t                            uid_next;

} VdFc__Win32InternalData;
static VdFc__Win32InternalData Vd_Fc_G = {0};

static VdFc__Device*            vd_fc__win32_alloc_device(int *id);
static VdFcLRESULT              vd_fc__win32_wndproc(VdFcHWND hwnd, VdFcUINT msg, VdFcWPARAM wparam, VdFcLPARAM lparam);
static void                     vd_fc__win32_remove_dropped_devices(void);
static VdFcBOOL                 vd_fc__win32_dinput_enum_devices(VdFc__Win32_LPDIDEVICEINSTANCEW inst, void *usr);
static int                      vd_fc__win32_xinput_supported(VdFcGUID *guid, uint32_t *out_opt_report_size);
static int                      vd_fc__win32_xinput_supported_handle(VdFcHANDLE devhandle);
static int                      vd_fc__win32_xinput_supported_instance_path(const char *name, VdFcUINT len);
static int                      vd_fc__win32_rawinput_handle_gidc_arrival(VdFcHANDLE devhandle);
static int                      vd_fc__win32_rawinput_handle_gidc_removal(VdFcHANDLE devhandle);
static int                      vd_fc__win32_rawinput_handle_state(VdFc__Device *device, VdFcBYTE *bytes, VdFcDWORD size_hid);
static VdFcHIDP_DATA*           vd_fc__win32_rawinput_find_data_by_data_index(VdFcHIDP_DATA *p, VdFcULONG data_count, VdFcUSHORT data_index_q);
static void                     vd_fc__win32_rawinput_xinput_poll(void);
static void                     vd_fc__win32_rawinput_xinput_correlate(VdFc__Device *device);
static void                     vd_fc__win32_rawinput_xinput_clear_correlation(void);
static int                      vd_fc__win32_device_is_xbox(uint16_t vid, uint16_t pid);
static VdFc__Win32OutputMethod  vd_fc__win32_output_method_from_device(VdFc__Win32DeviceDriver driver, int is_xbox, int correlation_idx);

VD_FC_API void vd_fc_init(void)
{
    Vd_Fc_G.uid_next = 1;

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

    // XInput
    {
        const char* xinput_dll_name[] = {
            "xinput1_4.dll",   // Windows 8+
            "xinput1_3.dll",   // DirectX SDK, Windows XP...
            "xinput9_1_0.dll", // Windows Vista, 7...
        };

        VdFcHMODULE m = NULL;
        for (int i = 0; i < 3; ++i) {
            m = LoadLibraryA(xinput_dll_name[i]);

            if (m != NULL) {
                break;
            }
        }

        Vd_Fc_G.xinput = m;
        if (Vd_Fc_G.xinput) {
            VdFcXInputGetState = (VdFcProcXInputGetState*)GetProcAddress(m, "XInputGetState");
            VdFcXInputSetState = (VdFcProcXInputSetState*)GetProcAddress(m, "XInputSetState");
        }
    }

    // Raw Input
    {
        VdFcRAWINPUTDEVICE rid;
        rid.usUsagePage = 0x01; // Generic Desktop Controls
        rid.usUsage     = 0x05; // Gamepad
        rid.dwFlags     = 0x00002000 | 0x00000100; // RIDEV_DEVNOTIFY | RIDEV_INPUTSINK
        rid.hwndTarget  = Vd_Fc_G.hwnd;
        VdFcRegisterRawInputDevices(&rid, 1, sizeof(rid));
    }

    // Direct Input
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

        VdFcDEV_BROADCAST_DEVICEINTERFACE_A filter;
        VD_FC_MEMSET(&filter, 0, sizeof(filter));
        filter.dbcc_devicetype = 0x00000005 /*DBT_DEVTYP_DEVICEINTERFACE*/;
        filter.dbcc_size = sizeof(filter);
        filter.dbcc_classguid = Vd_Fc__WIn32_Guid_HIDClass;

        void *hdev_notify = VdFcRegisterDeviceNotificationA(Vd_Fc_G.hwnd, (void*)&filter,
                                                            0x00000000 /*DEVICE_NOTIFY_WINDOW_HANDLE*/ |
                                                            0x00000004 /*DEVICE_NOTIFY_ALL_INTERFACE_CLASSES*/);
        if (hdev_notify == 0) {
            VD_FC_LOG("Error. RegisterDeviceNotificationA failed.");
        }

        Vd_Fc_G.hdev_notify = hdev_notify;
    }
}

VD_FC_API void vd_fc_quit(void)
{
    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        Vd_Fc_G.devices[i].present = 0;
    }

    vd_fc__win32_remove_dropped_devices();

    if (Vd_Fc_G.xinput != 0) {
        FreeLibrary(Vd_Fc_G.xinput);
    }

    Vd_Fc_G.dinput->lpVtbl->Release(Vd_Fc_G.dinput);

    VdFcDestroyWindow(Vd_Fc_G.hwnd);
    VdFcUnregisterClassW(L"vd_fc_class", GetModuleHandleA(NULL));
}

VD_FC_API void vd_fc_poll(void)
{
    Vd_Fc_G.num_devices_dropped = 0;
    Vd_Fc_G.num_devices_arrived = 0;
    Vd_Fc_G.xinput_polled_this_frame = 0;
    Vd_Fc_G.xinput_correlation_cleared_this_frame = 0;
    Vd_Fc_G.dinput_enumerate_devices = 0;

    // Poll & Check if any device was dropped
    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        VdFc__Device *dev = &Vd_Fc_G.devices[i];
        if (dev->dinput_device) {
            VdFcHRESULT hpoll = dev->dinput_device->lpVtbl->Poll(dev->dinput_device);
            if ((hpoll != 0) && (hpoll != 1)) {
                Vd_Fc_G.devices[i].present = 0;
                VD_FC_LOG("Device %d dropped, hpoll = %d\n", i, hpoll);
            }
        }
    }

    // Get DirectInput8 arrivals
    // Get RAWINPUT arrivals/drops/inputs
    VdFcMSG msg;
    while (VdFcPeekMessageW(&msg, NULL, 0u, 0u, 0x0001 /* PM_REMOVE */)) {
        VdFcTranslateMessage(&msg);
        VdFcDispatchMessageW(&msg);
    }

    if (Vd_Fc_G.dinput_enumerate_devices) {
        Vd_Fc_G.dinput->lpVtbl->EnumDevices(Vd_Fc_G.dinput, VD_FC__WIN32_DI8DEVCLASS_GAMECTRL,
                                        vd_fc__win32_dinput_enum_devices, 0,
                                        VD_FC__WIN32_DIEDFL_ALLDEVICES);
    }

    // Re-allocate devices
    vd_fc__win32_remove_dropped_devices();

    // For each new device, add it to arrivals
    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        VdFc__Device *dev = &Vd_Fc_G.devices[i];
        if (dev->just_arrived) {
            dev->just_arrived = 0;
            dev->uid = vd_fc__uid_next();

            Vd_Fc_G.devices_arrived[Vd_Fc_G.num_devices_arrived].idx = i;
            Vd_Fc_G.devices_arrived[Vd_Fc_G.num_devices_arrived].uid = dev->uid;

            Vd_Fc_G.num_devices_arrived++;
        }
    }

    // Poll DirectInput Devices & Apply FF
    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        VdFc__Device *dev = &Vd_Fc_G.devices[i];
        if (dev->driver == VD_FC__WIN32_DEVICE_DRIVER_DINPUT) {
            dev->dinput_device->lpVtbl->GetDeviceState(dev->dinput_device,
                                                       sizeof(dev->data.dinput.state), (void*)&dev->data.dinput.state);
        }

        if (dev->ff_rumble_resubmit) {
            dev->ff_rumble_resubmit = 0;
            float left_motor    = dev->ff_rumble.left_motor  * dev->ff_gain;
            float right_motor   = dev->ff_rumble.right_motor * dev->ff_gain;
            float left_trigger  = dev->ff_rumble.left_trigger * dev->ff_gain;
            float right_trigger = dev->ff_rumble.right_trigger * dev->ff_gain;
            (void)left_trigger;
            (void)right_trigger;

            switch (dev->output_method) {
                case VD_FC__WIN32_OUTPUT_METHOD_XBOX_DEVICE_IO_CONTROL: {
                    // @note(mdodis): We should only get DeviceIoControl rumble method from an xbox device,
                    //                since they're all XInput.
                    VD_FC_ASSERT(dev->driver == VD_FC__WIN32_DEVICE_DRIVER_RINPUT);

                    uint8_t left_motor_freq  = (uint8_t)(vd_fc__clampf(left_motor  * 255.f, 0, 255));
                    uint8_t right_motor_freq = (uint8_t)(vd_fc__clampf(right_motor * 255.f, 0, 255));
                    // VD_FC_LOG("DeviceIoControl %f/%d %f/%d", left_motor, left_motor_freq, right_motor, right_motor_freq);

                    // @note(mdodis): Code sourced from Mārtiņš Možeiko. Thanks!
                    //                https://gist.github.com/mmozeiko/b8ccc54037a5eaf35432396feabbe435
                    uint8_t in[5] = {
                        0, 0,
                        left_motor_freq, right_motor_freq,
                        2,
                    };

                    // @todo(mdodis): Handle rumble setting when an xbox controller is correlated to XInput dwUserIndex

                    if (dev->write_handle != 0) {

                        VdFcDWORD bytes_transferred;
                        VdFcBOOL finished = GetOverlappedResult(dev->write_handle, &dev->overlapped,
                                                                &bytes_transferred, 0);

                        if (!finished) {
                            CancelIo(dev->write_handle);
                        }
                        DeviceIoControl(dev->write_handle, 0x8000a010, in, sizeof(in), NULL, 0, NULL, &dev->overlapped);
                    }

                } break;

                case VD_FC__WIN32_OUTPUT_METHOD_XINPUT_SET_STATE: {
                    VD_FC_ASSERT(dev->driver == VD_FC__WIN32_DEVICE_DRIVER_RINPUT);
                    VD_FC_ASSERT(dev->data.rinput.correlation_idx != -1);
                    uint16_t left_motor_freq  = (uint16_t)(vd_fc__clampf(left_motor  * 65535.f, 0, 65535));
                    uint16_t right_motor_freq = (uint16_t)(vd_fc__clampf(right_motor * 65535.f, 0, 65535));

                    VdFcXINPUT_VIBRATION vibration;
                    vibration.wLeftMotorSpeed = left_motor_freq;
                    vibration.wRightMotorSpeed = right_motor_freq;
                    VdFcXInputSetState(dev->data.rinput.correlation_idx, &vibration);
                } break;

                default: {} break;
            }
        }
    }
}

VD_FC_API int vd_fc_count(void)
{
    return Vd_Fc_G.num_devices;
}

VD_FC_API VdFcIndexUID *vd_fc_dropped(int *count)
{
    if (count) {
        *count = Vd_Fc_G.num_devices_dropped;
    }

    return Vd_Fc_G.devices_dropped;
}

VD_FC_API VdFcIndexUID *vd_fc_arrived(int *count)
{
    if (count) {
        *count = Vd_Fc_G.num_devices_arrived;
    }

    return Vd_Fc_G.devices_arrived;
}

VD_FC_API VdFcGuid vd_fc_guid(int id)
{
    VdFcGuid result = {0};

    if (id < Vd_Fc_G.num_devices) {
        result = Vd_Fc_G.devices[id].guid;
    }

    return result;
}

VD_FC_API VdFcType vd_fc_type(int id)
{
    if (id < Vd_Fc_G.num_devices) {
        return VD_FC_TYPE_CONTROLLER;
    } else {
        return VD_FC_TYPE_INVALID;
    }
}

VD_FC_API VdFcClass vd_fc_class(int id)
{
    if (id < Vd_Fc_G.num_devices) {
        return Vd_Fc_G.devices[id].klass;
    } else {
        return VD_FC_CLASS_INVALID;
    }
}

VD_FC_API unsigned int vd_fc_raw_button_down(int id, int button_id)
{
    if (id < Vd_Fc_G.num_devices) {
        VdFc__Device *dev = &Vd_Fc_G.devices[id];
        if (dev->driver == VD_FC__WIN32_DEVICE_DRIVER_DINPUT) {
            if (button_id < dev->num_buttons) {
                return dev->data.dinput.state.rgbButtons[button_id] ? 1 : 0;
            } else {
                return 0;
            }
        } else if (dev->driver == VD_FC__WIN32_DEVICE_DRIVER_RINPUT) {
            if (button_id < dev->num_buttons) {
                return dev->data.rinput.buttons[button_id].state;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

VD_FC_API unsigned int vd_fc_raw_hat_value(int id, int hat_id)
{
    if (id < Vd_Fc_G.num_devices) {
        VdFc__Device *dev = &Vd_Fc_G.devices[id];
        if (dev->driver == VD_FC__WIN32_DEVICE_DRIVER_DINPUT) {
            if (hat_id < dev->num_hats) {
                return dev->data.dinput.state.rgdwPOV[hat_id];
            } else {
                return 0;
            }
        } else if (dev->driver == VD_FC__WIN32_DEVICE_DRIVER_RINPUT) {
            if (hat_id < dev->num_hats) {
                return dev->data.rinput.hats[hat_id].state;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

VD_FC_API long long vd_fc_raw_axis_value(int id, int axis_id)
{
    if (id < Vd_Fc_G.num_devices) {
        VdFc__Device *dev = &Vd_Fc_G.devices[id];

        if (dev->driver == VD_FC__WIN32_DEVICE_DRIVER_DINPUT) {
            if (axis_id < dev->num_axes) {
                VdFcLONG *rd_value = (VdFcLONG*)(((unsigned char*)&dev->data.dinput.state) + Vd_Fc__Win32_DIJOYSTATE2_AxisOffsets[axis_id]);
                return *rd_value;
            } else {
                return 0x8000;
            }
        } else if (dev->driver == VD_FC__WIN32_DEVICE_DRIVER_RINPUT) {
            if (axis_id < dev->num_axes) {
                return dev->data.rinput.axes[axis_id].state;
            } else {
                return 0x8000;
            }
        } else {
            return 0x8000;
        }
    } else {
        return 0x8000;
    }
}

static VdFcBOOL vd_fc__win32_dinput_enum_devices(VdFc__Win32_LPDIDEVICEINSTANCEW inst, void *usr)
{
    int device_already_acquired = 0;
    (void)usr;

    uint32_t output_report_size;
    if (vd_fc__win32_xinput_supported(&inst->guidProduct, &output_report_size)) {
        return 1;
    }

    if (Vd_Fc_G.num_devices == VD_FC_DEVICE_COUNT_MAX) {
        return 0;
    }

    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        if (Vd_Fc_G.devices[i].driver == VD_FC__WIN32_DEVICE_DRIVER_DINPUT) {
            if (VD_FC_MEMCMP(&Vd_Fc_G.devices[i].data.dinput.instance_guid, &inst->guidInstance, sizeof(inst->guidInstance)) == 0) {
                device_already_acquired = 1;
            }
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
                        VdFcGuid guid;
                        VdFc__Device *fc_device              = vd_fc__win32_alloc_device(&id);
                        fc_device->dinput_device             = device;
                        fc_device->num_buttons               = caps.dwButtons;
                        fc_device->num_hats                  = caps.dwPOVs;
                        fc_device->num_axes                  = caps.dwAxes;
                        fc_device->present                   = 1;
                        fc_device->data.dinput.instance_guid = inst->guidInstance;

                        fc_device->num_name = WideCharToMultiByte(65001 /*CP_UTF8*/, 0,
                                                                  inst->tszInstanceName, -1,
                                                                  fc_device->name, VD_FC_DEVICE_NAME_MAX - 1, // null terminator
                                                                  NULL, NULL);
                        if (fc_device->num_name == 0) {
                            VD_FC_MEMCPY(fc_device->name, "Unknown Device", 14);
                            fc_device->num_name = 14;
                        }
                        fc_device->name[fc_device->num_name] = 0;

                        if (VD_FC_MEMCMP(&inst->guidProduct.Data4[2], "PIDVID", 6) == 0) {
                            uint16_t vid = (inst->guidProduct.Data1 & 0x0000FFFF);
                            uint16_t pid = (inst->guidProduct.Data1 & 0xFFFF0000) >> 16;
                            uint16_t bus = 0x03;
                            guid.parts.bus              = VD_FC_SWAP16LE(bus);
                            guid.parts.crc              = 0x0000;
                            guid.parts.vendor_id        = VD_FC_SWAP16LE(vid);
                            guid.parts.reserved0        = 0x0000;
                            guid.parts.product_id       = VD_FC_SWAP16LE(pid);
                            guid.parts.reserved1        = 0x0000;
                            guid.parts.version          = 0x0000;
                            guid.parts.driver_signature = 0x00;
                            guid.parts.driver_data      = 0x00;
                            fc_device->common = vd_fc__device_get_common_data(vid, pid, output_report_size);
                        } else {
                            uint16_t bus = 0x05;
                            guid.parts.bus = VD_FC_SWAP16LE(bus);
                            guid.parts.crc = 0x0000;
                            for (int i = 0; i < 11; ++i) {
                                guid.dat[4 + i] = fc_device->name[i];
                            }
                            fc_device->common = vd_fc__device_get_common_data(0, 0, output_report_size);
                        }
                        fc_device->guid = guid;

                        fc_device->driver = VD_FC__WIN32_DEVICE_DRIVER_DINPUT;
                        fc_device->just_arrived = 1;

                        vd_fc_device_classify_auto(fc_device);

                        uint8_t *inst_guid = (uint8_t*)&inst->guidInstance;
                        VD_FC_LOG("Device initialized: '%s'", fc_device->name);
                        VD_FC_LOG("            Driver: %s", "Direct Input");
                        VD_FC_LOG("           Buttons: %d", caps.dwButtons);
                        VD_FC_LOG("              Hats: %d", caps.dwPOVs);
                        VD_FC_LOG("              Axes: %d", caps.dwAxes);
                        VD_FC_LOG("             class: %s", vd_fc_class_str(fc_device->klass));
                        VD_FC_LOG("     Instance GUID: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                                                       inst_guid[0],  inst_guid[1],  inst_guid[2],  inst_guid[3],
                                                       inst_guid[4],  inst_guid[5],  inst_guid[6],  inst_guid[7],
                                                       inst_guid[8],  inst_guid[9],  inst_guid[10], inst_guid[11],
                                                       inst_guid[12], inst_guid[13], inst_guid[14], inst_guid[15]);
                        VD_FC_LOG("              GUID: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                                                       guid.dat[0],  guid.dat[1],  guid.dat[2],  guid.dat[3],
                                                       guid.dat[4],  guid.dat[5],  guid.dat[6],  guid.dat[7],
                                                       guid.dat[8],  guid.dat[9],  guid.dat[10], guid.dat[11],
                                                       guid.dat[12], guid.dat[13], guid.dat[14], guid.dat[15]);
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
            if (Vd_Fc_G.hdev_notify) {
                if (wparam == 0x8000/*DBT_DEVICEARRIVAL*/) {
                    Vd_Fc_G.dinput_enumerate_devices = 1;

                    VdFcDEV_BROADCAST_HDR *dbh = (VdFcDEV_BROADCAST_HDR*)lparam;
                    if (dbh->dbch_devicetype == 5) {
                        VdFcDEV_BROADCAST_DEVICEINTERFACE_A *dbcc =(VdFcDEV_BROADCAST_DEVICEINTERFACE_A*)dbh;
                        VdFcGUID dev_guid = {0};
                        if (Vd_Fc_G.dinput->lpVtbl->FindDevice(Vd_Fc_G.dinput, &dbcc->dbcc_classguid, (wchar_t*)&dbcc->dbcc_name[0], &dev_guid) == 0) {
                            VD_FC_LOG("DBT Device is dinput device.\n");
                        }
                    }

                }
            } else {
                Vd_Fc_G.dinput_enumerate_devices = 1;
            }
        } break;

        case 0x00FE /*WM_INPUT_DEVICE_CHANGE*/: {
            VdFcHANDLE device_handle = (VdFcHANDLE)lparam;
            if (wparam == 1 /*GIDC_ARRIVAL*/) {
                vd_fc__win32_rawinput_handle_gidc_arrival(device_handle);
            } else if (wparam == 2 /*GIDC_REMOVAL*/) {
                vd_fc__win32_rawinput_handle_gidc_removal(device_handle);
            }
        } break;

        case 0x00FF /*WM_INPUT*/: {
            VdFcUINT data_size = sizeof(VdFcRAWINPUT) * VD_FC_WIN32_RAW_INPUT_BUFFER_COUNT;
            VdFcUINT num_bytes_copied = VdFcGetRawInputData((VdFcHANDLE)lparam,
                                                            0x10000003 /*RID_INPUT*/,
                                                            Vd_Fc_G.raw_input_buffer,
                                                            &data_size,
                                                            sizeof(VdFcRAWINPUTHEADER));
            if (num_bytes_copied == ((VdFcUINT)-1)) {
                break;
            }

            VdFcRAWINPUT *raw = Vd_Fc_G.raw_input_buffer;
            if (raw->header.dwType == 2 /*RIM_TYPEHID*/) {
                int gamepad_index = -1;
                for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
                    if (Vd_Fc_G.devices[i].driver == VD_FC__WIN32_DEVICE_DRIVER_RINPUT) {

                        if (Vd_Fc_G.devices[i].data.rinput.devhandle == raw->header.hDevice) {
                            gamepad_index = i;
                            break;
                        }

                    }
                }

                if (gamepad_index != -1) {
                    for (VdFwDWORD ri = 0; ri < raw->data.hid.dwCount; ++ri) {
                        VdFwBYTE *bytes = &raw->data.hid.bRawData[0] + ri * (raw->data.hid.dwSizeHid);
                        vd_fc__win32_rawinput_handle_state(&Vd_Fc_G.devices[gamepad_index], bytes, raw->data.hid.dwSizeHid);
                    }
                }
            }
        } break;

        default: {
            result = VdFcDefWindowProcW(hwnd, msg, wparam, lparam);
        } break;
    }
    return result;
}

static int vd_fc__win32_xinput_supported(VdFcGUID *guid, uint32_t *out_opt_report_size)
{
    VdFcRAWINPUTDEVICELIST *raw_input_device_list = Vd_Fc_G.raw_input_device_list;
    VdFcUINT max_raw_input_devices = sizeof(Vd_Fc_G.raw_input_device_list) / sizeof(Vd_Fc_G.raw_input_device_list[0]);
    VdFcUINT device_count = 0;

    VdFcUINT r = VdFcGetRawInputDeviceList(0, &device_count, sizeof(*raw_input_device_list));
    if (r == ((VdFcUINT)-1)) {
        VD_FC_LOG("Error. GetRawInputDeviceList failed with %d", GetLastError());
        return 0;
    }

    if (device_count > max_raw_input_devices) {
        return 0;
    }

    r = VdFcGetRawInputDeviceList(raw_input_device_list, &device_count, sizeof(*raw_input_device_list));
    if (r == ((VdFcUINT)-1)) {
        return 0;
    }

    for (VdFcUINT i = 0; i < device_count; ++i) {
        VdFcRID_DEVICE_INFO rdi = {0};
        VdFcUINT size = sizeof(rdi);
        rdi.cbSize = sizeof(rdi);

        if (raw_input_device_list[i].dwType != 2 /*RIM_TYPEHID*/) {
            continue; 
        }

        if (VdFcGetRawInputDeviceInfoA(raw_input_device_list[i].hDevice,
                                       VD_FC_RIDI_DEVICEINFO,
                                       &rdi, &size) == (VdFcUINT)(-1))
        {
            continue;
        }


        VdFcLONG vid_pid = VD_FC__WIN32_MAKELONG(rdi.v.hid.dwVendorId, rdi.v.hid.dwProductId);
        if (vid_pid != (VdFcLONG)(guid->Data1)) {
            continue;
        }

        if (out_opt_report_size) {
            static char device_instance_path[256];
            VdFcUINT device_instance_path_len;
            VD_FC_MEMSET(device_instance_path, 0, sizeof(device_instance_path));
            device_instance_path_len = sizeof(device_instance_path);
            if (VdFcGetRawInputDeviceInfoA(raw_input_device_list[i].hDevice,
                                           VD_FC_RIDI_DEVICENAME,
                                           device_instance_path, &device_instance_path_len) != ((VdFcUINT)-1))
            {
                HANDLE device_file = CreateFileA(device_instance_path,
                                                 GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                                 NULL, OPEN_EXISTING, 0, NULL);
                VdFcPHIDP_PREPARSED_DATA ppd = 0;
                *out_opt_report_size = 0;
                if (VdFcHidD_GetPreparsedData(device_file, &ppd)) {

                    VdFcHIDP_CAPS caps;
                    if (VdFcHidP_GetCaps(ppd, &caps) != ((VdFcUINT)-1)) {
                        *out_opt_report_size = caps.InputReportByteLength;
                    }
                }

                CloseHandle(device_file);
            } else {
                VD_FC_LOG("Error. GetRawInputDeviceInfoA for RIDI_DEVICENAME failed with %d", GetLastError());
            }

        }

        if (vd_fc__win32_xinput_supported_handle(raw_input_device_list[i].hDevice)) {
            return 1;
        }
    }

    return 0;
}

static int vd_fc__win32_xinput_supported_handle(VdFcHANDLE devhandle)
{
    static char name[256];
    VdFcUINT size;
    VD_FC_MEMSET(name, 0, sizeof(name));
    size = sizeof(name);

    if (VdFcGetRawInputDeviceInfoA(devhandle,
                                   VD_FC_RIDI_DEVICENAME,
                                   name, &size) == ((VdFcUINT)-1))
    {
        return 0;
    }

    return vd_fc__win32_xinput_supported_instance_path(name, size);
}

static int vd_fc__win32_xinput_supported_instance_path(const char *name, VdFcUINT len)
{
    for (VdFcUINT c = 0; c < len; ++c) {
        if ((c + 2) > len) {
            continue;
        }

        if (name[c + 0] == ('I') &&
            name[c + 1] == ('G') &&
            name[c + 2] == ('_'))
        {
            return 1;
        }
    }

    return 0;
}

static int vd_fc__win32_rawinput_handle_gidc_arrival(VdFcHANDLE devhandle)
{
    if (Vd_Fc_G.num_devices == VD_FC_DEVICE_COUNT_MAX) {
        return 0;
    }

    VdFc__Device *device = &Vd_Fc_G.devices[Vd_Fc_G.num_devices];

    VdFcRID_DEVICE_INFO device_info;
    int is_xinput_device;

    device_info.cbSize = sizeof(device_info);
    VdFcUINT cb_size = sizeof(device_info);

    VdFcUINT device_info_result = VdFcGetRawInputDeviceInfoA(
        devhandle,
        VD_FC_RIDI_DEVICEINFO,
        &device_info,
        &cb_size);

    if (device_info_result == ((VdFcUINT)-1)) {
        return 0;
    }

    if (device_info.dwType != 2 /*RIM_TYPEHID*/) {
        return 0;
    }

    static char device_instance_path[256];
    VdFcUINT device_instance_path_len;
    VD_FC_MEMSET(device_instance_path, 0, sizeof(device_instance_path));
    device_instance_path_len = sizeof(device_instance_path);

    if (VdFcGetRawInputDeviceInfoA(devhandle,
                                   VD_FC_RIDI_DEVICENAME,
                                   device_instance_path, &device_instance_path_len) == ((VdFcUINT)-1))
    {
        VD_FC_LOG("Error. GetRawInputDeviceInfoA for RIDI_DEVICENAME");
        return 0;
    }

    is_xinput_device = vd_fc__win32_xinput_supported_instance_path(device_instance_path, device_instance_path_len);
    if ((Vd_Fc_G.dinput != 0) && !is_xinput_device) {
        // Device should be picked up by Direct Input
        return 0;
    }

    UINT ppd_req_size = 0;
    if (VdFcGetRawInputDeviceInfoA(
        devhandle,
        VD_FC_RIDI_PREPARSEDDATA,
        NULL,
        &ppd_req_size) == ((UINT)-1))
    {
        VD_FC_LOG("Error. GetRawInputDeviceInfoA for RIDI_PREPARSEDDATA");
        return 0;
    }

    device->ppd = (VdFcPHIDP_PREPARSED_DATA)VD_FC_REALLOC(device->ppd, device->ppd_size, ppd_req_size);
    device->ppd_size = ppd_req_size;
    if (VdFcGetRawInputDeviceInfoA(devhandle, VD_FC_RIDI_PREPARSEDDATA,
                                   device->ppd,
                                   &ppd_req_size) == ((UINT)-1))
    {
        VD_FC_LOG("Error. GetRawInputDeviceInfoA for RIDI_PREPARSEDDATA");
        return 0;
    }


    static char manufacturer_string[128];
    VD_FC_MEMSET(manufacturer_string, 0, sizeof(manufacturer_string));

    static char product_string[128];
    VD_FC_MEMSET(product_string, 0, sizeof(product_string));

    uint16_t vendor_id  = (uint16_t)device_info.v.hid.dwVendorId;
    uint16_t product_id = (uint16_t)device_info.v.hid.dwProductId;
    uint16_t version    = (uint16_t)device_info.v.hid.dwVersionNumber;
    int is_xbox         = vd_fc__win32_device_is_xbox(vendor_id, product_id);

    device->common      = vd_fc__device_get_common_data(vendor_id, product_id, 0);

    VdFcHANDLE write_handle = NULL;

    // Get Manufacturer & Product Strings
    {
        static wchar_t temp_wstring[128];

        HANDLE device_file = CreateFileA(device_instance_path,
                                         GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                                         NULL, OPEN_EXISTING, 0, NULL);

        temp_wstring[0] = 0;
        if (VdFcHidD_GetManufacturerString(device_file, temp_wstring, sizeof(temp_wstring))) {
            int w = WideCharToMultiByte(65001 /*CP_UTF8*/, 0,
                                        temp_wstring, -1,
                                        manufacturer_string, sizeof(manufacturer_string),
                                        NULL, NULL);
            manufacturer_string[w] = 0;
            vd_fc__strlcpy(device->name, manufacturer_string, VD_FC_DEVICE_NAME_MAX);
        }

        temp_wstring[0] = 0;
        if (VdFcHidD_GetProductString(device_file, temp_wstring, sizeof(temp_wstring))) {
            int w = WideCharToMultiByte(65001 /*CP_UTF8*/, 0,
                                        temp_wstring, -1,
                                        product_string, sizeof(product_string),
                                        NULL, NULL);
            product_string[w] = 0;

            vd_fc__strlcpy(device->name, product_string, VD_FC_DEVICE_NAME_MAX);
        }

        CloseHandle(device_file);

        write_handle = CreateFileA(device_instance_path,
                                   GENERIC_READ | GENERIC_WRITE,
                                   FILE_SHARE_READ | FILE_SHARE_WRITE,
                                   NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
    }

    VdFcGuid guid = vd_fc__make_gamepad_guid(0x03, vendor_id, product_id, version, manufacturer_string, product_string, 'r', 0);

    int num_buttons = 0;
    int num_hats = 0;
    int num_axes = 0;

    VdFwULONG data_count = VdFcHidP_MaxDataListLength(VdFcHidP_Input, device->ppd);
    if (data_count > sizeof(device->data.rinput.hidp_data)/sizeof(device->data.rinput.hidp_data[0])) {
        VD_FC_LOG("Warning. HidP_MaxDataListLength reported %lu data indices, which are more than max supported (%llu)",
                  data_count, sizeof(device->data.rinput.hidp_data)/sizeof(device->data.rinput.hidp_data[0]));
        data_count = sizeof(device->data.rinput.hidp_data)/sizeof(device->data.rinput.hidp_data[0]);
    }

    VdFcHIDP_CAPS caps;
    if (VdFcHidP_GetCaps(device->ppd, &caps) == ((VdFcUINT)-1)) {
        VD_FC_LOG("Error. HidP_GetCaps failed");
        return 0;
    }

    static VdFcHIDP_BUTTON_CAPS button_caps[128];
    VdFcUSHORT num_button_caps = caps.NumberInputButtonCaps;
    if (num_button_caps > 128) {
        VD_FC_LOG("Warning. HidP_GetCaps reported %u button capabilities, which are more than max supported (%d)",
                  caps.NumberInputButtonCaps, 128);
        num_button_caps = 128;
    }

    if (VdFcHidP_GetButtonCaps(VdFcHidP_Input, button_caps, &num_button_caps, device->ppd) == ((VdFcUINT)-1)) {
        VD_FC_LOG("Error. HidP_GetButtonCaps failed");
        return 0;
    }

    // Count total buttons
    for (int i = 0; i < num_button_caps; ++i) {
        if (button_caps[i].UsagePage != 0x0009 /*Button*/) {
            continue;
        }

        if (button_caps[i].IsRange) {
            num_buttons += 1 + button_caps[i].v.Range.DataIndexMax - button_caps[i].v.Range.DataIndexMin;
        } else {
            num_buttons++;
        }
    }

    if (num_buttons > 128) {
        VD_FC_LOG("Warning. Counting result of HidP_GetButtonCaps with ranges returned %d buttons, which are more than max supported(%d)",
                  num_buttons, 128);
        num_buttons = 128;
    }

    static VdFcHIDP_VALUE_CAPS value_caps[38];
    VdFcUSHORT num_value_caps = caps.NumberInputValueCaps;
    if (num_value_caps > 38) {
        VD_FC_LOG("Warning. HidP_GetCaps reported %u value capabilities, which are more than max supported (%d)",
                  caps.NumberInputValueCaps, 38);
        num_button_caps = 38;
    }

    if (VdFcHidP_GetValueCaps(VdFcHidP_Input, value_caps, &num_value_caps, device->ppd) == ((VdFcUINT)-1)) {
        VD_FC_LOG("Error. HidP_GetValueCaps failed");
        return 0;
    }

    // Sort value caps by v.NotRange.Usage
    for (int i = 0; i < num_value_caps - 1; ++i) {
        for (int j = i + 1; j < num_value_caps; ++j) {
            if (value_caps[i].v.NotRange.Usage > value_caps[j].v.NotRange.Usage) {
                VdFcHIDP_VALUE_CAPS temp = value_caps[i];
                value_caps[i] = value_caps[j];
                value_caps[j] = temp;
            }
        }
    }

    device->data.rinput.splitz = 0;
    for (int i = 0; i < num_value_caps; ++i) {
        if (value_caps[i].IsRange) {
            continue;
        }

        if (value_caps[i].v.NotRange.Usage == 0x0039 /*Hat*/) {
            num_hats++;
            continue;
        }

        if (is_xinput_device && value_caps[i].v.NotRange.Usage == 0x0032 /*GenericZ*/) {
            device->data.rinput.splitz = 1;
            device->data.rinput.splitz_idx = value_caps[i].v.NotRange.DataIndex;
            continue; 
        }

        num_axes++;
    }

    if (num_hats > 5) {
        VD_FC_LOG("Warning. Counting result of HidP_GetValueCaps returned %d hats, which are more than max supported(%d)",
                  num_hats, 5);
        num_hats = 5;
    }

    if ((num_axes + device->data.rinput.splitz * 2) > 33) {
        VD_FC_LOG("Warning. Counting result of HidP_GetValueCaps returned %d axes, which are more than max supported(%d)",
                  (num_axes + device->data.rinput.splitz * 2), 33);
        num_axes = 33 - device->data.rinput.splitz * 2;
    }

    int count_buttons = 0;
    int count_hats = 0;
    int count_axes = 0;

    // Collect Buttons
    for (int i = 0; i < num_button_caps; ++i) {
        if (count_buttons == num_buttons) {
            break;
        }

        if (button_caps[i].UsagePage != 0x0009 /*Button*/) {
            continue;
        }

        if (button_caps[i].IsRange) {
            VdFwUSHORT usage_count = 1 + button_caps[i].v.Range.DataIndexMax - button_caps[i].v.Range.DataIndexMin;
            for (int j = 0; j < usage_count; ++j) {
                if (count_buttons == num_buttons) {
                    break;
                }

                device->data.rinput.buttons[count_buttons].data_index = (uint16_t)(button_caps[i].v.Range.DataIndexMin + j);
                device->data.rinput.buttons[count_buttons].state      = 0;
                count_buttons++;
            }

       } else {
            device->data.rinput.buttons[count_buttons].data_index = button_caps[i].v.NotRange.DataIndex;
            device->data.rinput.buttons[count_buttons].state      = 0;
            count_buttons++;
       }

    }

    // Collect Hats & Axes
    for (int i = 0; i < num_value_caps; ++i) {
        if ((count_hats == num_hats) && (count_axes == num_axes)) {
            break;
        }

        if (value_caps[i].IsRange) {
            continue;
        }

        if (value_caps[i].v.NotRange.Usage == 0x0039 /*Hat*/) {
            if (count_hats < num_hats) {
                device->data.rinput.hats[count_hats].data_index = value_caps[i].v.NotRange.DataIndex;
                device->data.rinput.hats[count_hats].state      = 0;
                count_hats++;
            } 

            continue;
        }

        if (device->data.rinput.splitz && value_caps[i].v.NotRange.Usage == 0x0032 /*GenericZ*/) {
            continue;
        }

        if (count_axes < num_axes) {
            device->data.rinput.axes[count_axes].data_index = value_caps[i].v.NotRange.DataIndex;
            device->data.rinput.axes[count_axes].state      = 0;
            count_axes++;
        }
    }

    num_axes += device->data.rinput.splitz * 2;

    if (is_xinput_device) {
        vd_fc__win32_rawinput_xinput_clear_correlation();
    }

    device->driver                       = VD_FC__WIN32_DEVICE_DRIVER_RINPUT;
    device->data.rinput.devhandle        = devhandle;
    device->data.rinput.hidp_data_count  = data_count;
    device->data.rinput.correlation_idx  = -1;
    device->data.rinput.is_xinput_device = is_xinput_device;
    device->data.rinput.is_xbox          = is_xbox;
    device->write_handle                 = write_handle;
    device->num_buttons                  = num_buttons;
    device->num_axes                     = num_axes;
    device->num_hats                     = num_hats;
    device->guid                         = guid;
    device->present                      = 1;
    device->just_arrived                 = 1;
    device->ff_gain                      = 1.f;
    VD_FC_MEMSET(&device->ff_rumble, 0, sizeof(device->ff_rumble));

    device->output_method = vd_fc__win32_output_method_from_device(device->driver, is_xbox, -1);

    vd_fc_device_classify_auto(device);

    VD_FC_LOG("Device initialized: '%s'", device->name);
    VD_FC_LOG("            Driver: %s", "Raw Input");
    VD_FC_LOG("           Buttons: %d", num_buttons);
    VD_FC_LOG("              Hats: %d", num_hats);
    VD_FC_LOG("              Axes: %d", num_axes);
    VD_FC_LOG("             class: %s", vd_fc_class_str(device->klass));
    VD_FC_LOG("              GUID: %02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
                                   guid.dat[0],  guid.dat[1],  guid.dat[2],  guid.dat[3],
                                   guid.dat[4],  guid.dat[5],  guid.dat[6],  guid.dat[7],
                                   guid.dat[8],  guid.dat[9],  guid.dat[10], guid.dat[11],
                                   guid.dat[12], guid.dat[13], guid.dat[14], guid.dat[15]);
    Vd_Fc_G.num_devices++;
    return 1;
}

static int vd_fc__win32_rawinput_handle_gidc_removal(VdFcHANDLE devhandle)
{
    int removed_index = -1;
    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        if (Vd_Fc_G.devices[i].driver != VD_FC__WIN32_DEVICE_DRIVER_RINPUT) {
            continue;
        }

        if (Vd_Fc_G.devices[i].data.rinput.devhandle == devhandle) {
            removed_index = i;
            break;
        }
    }

    if (removed_index == -1) {
        return 1;
    }

    Vd_Fc_G.devices[removed_index].present = 0;
    if (Vd_Fc_G.devices[removed_index].data.rinput.is_xinput_device) {
        vd_fc__win32_rawinput_xinput_clear_correlation();
    }

    return 0;
}

static int vd_fc__win32_rawinput_handle_state(VdFc__Device *device, VdFcBYTE *bytes, VdFcDWORD size_hid)
{
    vd_fc__win32_rawinput_xinput_poll();

    VdFcULONG data_count = device->data.rinput.hidp_data_count;
    VdFcNTSTATUS status = VdFcHidP_GetData(VdFcHidP_Input,
                                           device->data.rinput.hidp_data, &data_count,
                                           device->ppd,
                                           (VdFcPCHAR)bytes, size_hid);

    if (status == ((VdFcUINT)-1)) {
        return 0;
    }

    const int xinput_correlation_button_indices[] = {
        0, 1, 2, 3, // SOUTH, EAST, WEST, NORTH
        9, 10,      // LEFT_SHOULDER, RIGHT_SHOULDER
        4, 6,       // BACK, START
        7, 8,       // LEFT_STICK, RIGHT_STICK
    };

    for (int i = 0; i < device->num_buttons; ++i) {
        VdFc__Win32DeviceRawInputButton *btn = &device->data.rinput.buttons[i];

        // Find Data with DataIndex
        VdFcHIDP_DATA *data = vd_fc__win32_rawinput_find_data_by_data_index(device->data.rinput.hidp_data, data_count,
                                                                            btn->data_index);

        uint8_t on = 0;
        if (data) {
            on = data->dat.On ? 1 : 0;
        }

        btn->state = on;

        if (device->data.rinput.is_xinput_device) {
            if (i < (sizeof(xinput_correlation_button_indices) / sizeof(xinput_correlation_button_indices[0]))) {
                int button_bit = 1 << xinput_correlation_button_indices[i];

                uint32_t correlation_button_states = device->data.rinput.correlation_button_states;
                correlation_button_states = (correlation_button_states & (~button_bit)) | (button_bit * (on));
                device->data.rinput.correlation_button_states = correlation_button_states;
            }
        }
    }

    for (int i = 0; i < device->num_hats; ++i) {
        VdFc__Win32DeviceRawInputHat *hat = &device->data.rinput.hats[i];

        // Find Data with DataIndex
        VdFcHIDP_DATA *data = vd_fc__win32_rawinput_find_data_by_data_index(device->data.rinput.hidp_data, data_count,
                                                                            hat->data_index);

        if (data) {
            const uint8_t hat_states[] = {
                0,
                1,
                1 | 2,
                2,
                4 | 2,
                4,
                4 | 8,
                8,
                1 | 8,
                0,
            };

            if (data->dat.RawValue < sizeof(hat_states)) {
                hat->state = hat_states[data->dat.RawValue];
            } else {
                hat->state = 0; // Centered
            }
        } else {
            hat->state = 0; // Centered
        }
    }

    int num_axes = device->num_axes - device->data.rinput.splitz * 2;
    for (int i = 0; i < num_axes; ++i) {
        VdFc__Win32DeviceRawInputAxis *axis = &device->data.rinput.axes[i];

        // Find Data with DataIndex
        VdFcHIDP_DATA *data = vd_fc__win32_rawinput_find_data_by_data_index(device->data.rinput.hidp_data, data_count,
                                                                            axis->data_index);
        if (data) {
            axis->state = (int)(uint16_t)data->dat.RawValue - 0x8000;
        }
    }

    if (device->data.rinput.splitz) {
        int left_trigger = device->num_axes - 2;
        int right_trigger = device->num_axes - 1;

        VdFcHIDP_DATA *data = vd_fc__win32_rawinput_find_data_by_data_index(device->data.rinput.hidp_data, data_count,
                                                                            device->data.rinput.splitz_idx);
        int16_t left_value;
        int16_t right_value;
        if (data) {
            int16_t value = (int16_t)(uint16_t)data->dat.RawValue - 0x8000;
            left_value  = (value > 0) ? (+value * 2 - 32767) : -32768;
            right_value = (value < 0) ? (-value * 2 - 32769) : -32768;
        } else {
            left_value  = -32768;
            right_value = -32768;
        }

        device->data.rinput.axes[left_trigger].state = left_value;
        device->data.rinput.axes[right_trigger].state = right_value;
    }

    if (device->data.rinput.is_xinput_device) {
        vd_fc__win32_rawinput_xinput_correlate(device);

        if (device->data.rinput.correlation_idx != -1) {
            int left_value  = Vd_Fc_G.xinput_states[device->data.rinput.correlation_idx].state.Gamepad.bLeftTrigger;
            int right_value = Vd_Fc_G.xinput_states[device->data.rinput.correlation_idx].state.Gamepad.bRightTrigger;


            if (device->data.rinput.splitz) {
                int left_trigger = device->num_axes - 2;
                int right_trigger = device->num_axes - 1;
                device->data.rinput.axes[left_trigger].state = ((left_value * 257) - 32768);
                device->data.rinput.axes[right_trigger].state = ((right_value * 257) - 32768);
            }
        }
    }

    return 1;
}

static VdFcHIDP_DATA *vd_fc__win32_rawinput_find_data_by_data_index(VdFcHIDP_DATA *p, VdFcULONG data_count, VdFcUSHORT data_index_q)
{
    VdFcHIDP_DATA *data = 0;
    for (VdFcULONG i = 0; i < data_count; ++i) {
        if (p[i].DataIndex == data_index_q) {
            data = &p[i];
            break;
        }
    }

    return data;
}

static void vd_fc__win32_rawinput_xinput_poll(void)
{
    if (Vd_Fc_G.xinput_polled_this_frame) {
        return;
    }

    for (int i = 0; i < 4; ++i) {
        Vd_Fc_G.xinput_states[i].connected = VdFcXInputGetState(i, &Vd_Fc_G.xinput_states[i].state) == 0;
    }


    Vd_Fc_G.xinput_polled_this_frame = 1;
}

static void vd_fc__win32_rawinput_xinput_correlate(VdFc__Device *device)
{
    if (!Vd_Fc_G.xinput_polled_this_frame) {
        return;
    }

    if (device->data.rinput.correlation_idx != -1) {
        return;
    }

    for (int i = 0; i < 4; ++i) {
        VdFc__Win32XInputState *xinput_state = &Vd_Fc_G.xinput_states[i];
        if (!xinput_state->connected) {
            continue;
        }

        int matched = 0;

        // Buttons
        {

            int button_states = device->data.rinput.correlation_button_states;

            // 0x0400 = Guide Button
            int any_xinput_buttons_pressed = (xinput_state->state.Gamepad.wButtons & ~0x0400);
            if (any_xinput_buttons_pressed) {
                matched = matched ||
                    (
                        // SOUTH, EAST, WEST, NORTH
                        (((button_states >> 0) & 1) == ((xinput_state->state.Gamepad.wButtons &              VD_FC_XINPUT_GAMEPAD_A) ? 1 : 0)) &&
                        (((button_states >> 1) & 1) == ((xinput_state->state.Gamepad.wButtons &              VD_FC_XINPUT_GAMEPAD_B) ? 1 : 0)) &&
                        (((button_states >> 2) & 1) == ((xinput_state->state.Gamepad.wButtons &              VD_FC_XINPUT_GAMEPAD_X) ? 1 : 0)) &&
                        (((button_states >> 3) & 1) == ((xinput_state->state.Gamepad.wButtons &              VD_FC_XINPUT_GAMEPAD_Y) ? 1 : 0)) &&
                        // LEFT_SHOULDER, RIGHT_SHOULDER(
                        (((button_states >> 4) & 1) == ((xinput_state->state.Gamepad.wButtons &  VD_FC_XINPUT_GAMEPAD_LEFT_SHOULDER) ? 1 : 0)) &&
                        (((button_states >> 5) & 1) == ((xinput_state->state.Gamepad.wButtons & VD_FC_XINPUT_GAMEPAD_RIGHT_SHOULDER) ? 1 : 0)) &&
                        // LEFT_STICK, RIGHT_STICK
                        (((button_states >> 6) & 1) == ((xinput_state->state.Gamepad.wButtons &     VD_FC_XINPUT_GAMEPAD_LEFT_THUMB) ? 1 : 0)) &&
                        (((button_states >> 7) & 1) == ((xinput_state->state.Gamepad.wButtons &    VD_FC_XINPUT_GAMEPAD_RIGHT_THUMB) ? 1 : 0))
                    );

            }
        }

        if (matched) {
            VD_FC_LOG("Gamepad correlated to dwUserIndex: %d", i);
            device->data.rinput.correlation_idx = i;
            device->ff_rumble_resubmit = 1;
            device->output_method = vd_fc__win32_output_method_from_device(device->driver, device->data.rinput.is_xbox, i);
            break;
        }
    }
}

static void vd_fc__win32_rawinput_xinput_clear_correlation(void)
{
    if (Vd_Fc_G.xinput_correlation_cleared_this_frame) {
        return;
    }

    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        VdFc__Device *dev = &Vd_Fc_G.devices[i];
        if (dev->driver == VD_FC__WIN32_DEVICE_DRIVER_RINPUT) {
            dev->data.rinput.correlation_idx = -1;
            dev->ff_rumble_resubmit = 1;
            dev->output_method = vd_fc__win32_output_method_from_device(dev->driver, dev->data.rinput.is_xbox, -1);
        }
    }

    Vd_Fc_G.xinput_correlation_cleared_this_frame = 1;
}

static int vd_fc__win32_device_is_xbox(uint16_t vid, uint16_t pid)
{
    if (vid != 0x045e /* Microsoft vendor id*/) {
        return 0;
    }

    // @note(mdodis): Product IDs sourced from SDL. Thanks!
    switch (pid) {
        // USB_PRODUCT_XBOX360_XUSB_CONTROLLER (XUSB driver software PID)
        case 0x02a1: return 1;
        // USB_PRODUCT_XBOX360_WIRED_CONTROLLER              
        case 0x028e: return 1;
        // USB_PRODUCT_XBOX360_WIRELESS_RECEIVER             
        case 0x0719: return 1;
        // USB_PRODUCT_XBOX360_WIRELESS_RECEIVER_THIRDPARTY1 
        case 0x02a9: return 1;
        // USB_PRODUCT_XBOX360_WIRELESS_RECEIVER_THIRDPARTY2 
        case 0x0291: return 1;
        // USB_PRODUCT_XBOX_ONE_ADAPTIVE                     
        case 0x0b0a: return 1;
        // USB_PRODUCT_XBOX_ONE_ADAPTIVE_BLUETOOTH           
        case 0x0b0c: return 1;
        // USB_PRODUCT_XBOX_ONE_ADAPTIVE_BLE                 
        case 0x0b21: return 1;
        // USB_PRODUCT_XBOX_ONE_ELITE_SERIES_1               
        case 0x02e3: return 1;
        // USB_PRODUCT_XBOX_ONE_ELITE_SERIES_2               
        case 0x0b00: return 1;
        // USB_PRODUCT_XBOX_ONE_ELITE_SERIES_2_BLUETOOTH     
        case 0x0b05: return 1;
        // USB_PRODUCT_XBOX_ONE_ELITE_SERIES_2_BLE           
        case 0x0b22: return 1;
        // USB_PRODUCT_XBOX_ONE_S                            
        case 0x02ea: return 1;
        // USB_PRODUCT_XBOX_ONE_S_REV1_BLUETOOTH             
        case 0x02e0: return 1;
        // USB_PRODUCT_XBOX_ONE_S_REV2_BLUETOOTH             
        case 0x02fd: return 1;
        // USB_PRODUCT_XBOX_ONE_S_REV2_BLE                   
        case 0x0b20: return 1;
        // USB_PRODUCT_XBOX_SERIES_X                         
        case 0x0b12: return 1;
        // USB_PRODUCT_XBOX_SERIES_X_BLE                     
        case 0x0b13: return 1;
        // USB_PRODUCT_XBOX_ONE_XBOXGIP_CONTROLLER (XBOXGIP driver software PID)
        case 0x02ff: return 1;
        default:     return 0;
    }
}

static VdFc__Win32OutputMethod vd_fc__win32_output_method_from_device(VdFc__Win32DeviceDriver driver, int is_xbox, int correlation_idx)
{

    VdFc__Win32OutputMethod method = VD_FC__WIN32_OUTPUT_METHOD_NONE;
    if (driver == VD_FC__WIN32_DEVICE_DRIVER_DINPUT) {
        method = VD_FC__WIN32_OUTPUT_METHOD_NONE;
    } else if (driver == VD_FC__WIN32_DEVICE_DRIVER_RINPUT) {

        if (correlation_idx != -1) {
            method = VD_FC__WIN32_OUTPUT_METHOD_XINPUT_SET_STATE;
        } else if (is_xbox) {
            // @note(mdodis)
            // XInput correlation allows us to get proper triggers for xbox controllers, but with this method we can't
            // have rumble before that controller is correlated. Since we now know how to send rumble packets
            // without XInput, we can do that and if/when it's correlated later, we can use the XInput stuff.
            method = VD_FC__WIN32_OUTPUT_METHOD_XBOX_DEVICE_IO_CONTROL;
        } else {
            method = VD_FC__WIN32_OUTPUT_METHOD_NONE;
        }
    }

    return method;
}

static void vd_fc__win32_remove_dropped_devices(void)
{

    int old_num = Vd_Fc_G.num_devices;
    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        VdFc__Device *dev = &Vd_Fc_G.devices[i];
        if (!dev->present) {
            Vd_Fc_G.devices_dropped[Vd_Fc_G.num_devices_dropped].idx = i;
            Vd_Fc_G.devices_dropped[Vd_Fc_G.num_devices_dropped].uid = Vd_Fc_G.devices[i].uid;
            Vd_Fc_G.num_devices_dropped++;
        }
    }

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

                CloseHandle(dev->write_handle);
                if (dev->driver == VD_FC__WIN32_DEVICE_DRIVER_DINPUT) {
                    if (dev->dinput_device) {
                        dev->dinput_device->lpVtbl->Unacquire(dev->dinput_device);
                        dev->dinput_device->lpVtbl->Release(dev->dinput_device);
                        dev->dinput_device = NULL;
                    }
                } else {
                }
            }
        }

        for (int i = write_index; i < old_num; ++i) {
            VD_FC_MEMSET(&Vd_Fc_G.devices[i], 0, sizeof(Vd_Fc_G.devices[0]));
        }

        Vd_Fc_G.num_devices = write_index;
    }
}

VD_FC_API const char *vd_fc_driver(int id)
{
    if (id < Vd_Fc_G.num_devices) {
        VdFc__Device *dev = &Vd_Fc_G.devices[id];
        if (dev->driver == VD_FC__WIN32_DEVICE_DRIVER_RINPUT) {
            if (dev->data.rinput.correlation_idx == -1) {
                return "Raw Input";
            } else {
                return "Raw Input/XInput";
            }
        } else {
            return "Direct Input";
        }
    } else {
        return "N/A";
    }
}

VD_FC_API int vd_fc_ff_rumble_supported(int id)
{
    if (id < Vd_Fc_G.num_devices) {
        return Vd_Fc_G.devices[id].output_method != VD_FC__WIN32_OUTPUT_METHOD_NONE;
    } else {
        return 0;
    }
}

VD_FC_API float vd_fc_ff_gain_get(int id)
{
    if (id < Vd_Fc_G.num_devices) {
        return Vd_Fc_G.devices[id].ff_gain; 
    } else {
        return 1.f;
    }
}

VD_FC_API void vd_fc_ff_gain_set(int id, float gain)
{
    if (id < Vd_Fc_G.num_devices) {
        if (vd_fc__approxeq(gain, Vd_Fc_G.devices[id].ff_gain)) {
            return;
        }

        Vd_Fc_G.devices[id].ff_gain = gain;
        Vd_Fc_G.devices[id].ff_rumble_resubmit = 1;
    }
}

VD_FC_API void vd_fc_ff_rumble_set(int id, VdFcRumbleState *rumble)
{
    if (id < Vd_Fc_G.num_devices) {
        if (vd_fc__approxeq_rumble(rumble, &Vd_Fc_G.devices[id].ff_rumble)) {
            return;   
        }

        Vd_Fc_G.devices[id].ff_rumble = *rumble;
        Vd_Fc_G.devices[id].ff_rumble_resubmit = 1;
    }
}

VD_FC_API void vd_fc_ff_rumble_set_motors(int id, float left_motor, float right_motor)
{
    VdFcRumbleState state = vd_fc_ff_rumble_get(id);
    state.left_motor = left_motor;
    state.right_motor = right_motor;
    vd_fc_ff_rumble_set(id, &state);
}

VD_FC_API VdFcRumbleState vd_fc_ff_rumble_get(int id)
{
    VdFcRumbleState state;

    if (id < Vd_Fc_G.num_devices) {
        state = Vd_Fc_G.devices[id].ff_rumble; 
    } else {
        VD_FC_MEMSET(&state, 0, sizeof(state));
    }

    return state;
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

static uint16_t vd_fc__crc16(unsigned short crc, void *data, unsigned int len)
{
    unsigned int i;
    for (i = 0; i < len; ++i) {
        crc = vd_fw__crc16_byte((VdFwU8)crc ^ ((VdFwU8*)data)[i]) ^ crc >> 8;
    }
    return crc;
}

static uint16_t vd_fc__crc16_byte(uint8_t r)
{
    uint16_t result = 0;
    int i;

    for (i = 0; i < 8; ++i) {
        result = ((result ^ r) & 1 ? 0xa001 : 0) ^ result >> 1;
        r >>= 1;
    }

    return result;
} 

static VdFcGuid vd_fc__make_gamepad_guid(uint16_t bus, uint16_t vendor, uint16_t product, uint16_t version,
                                           char *vendor_name, char *product_name,
                                           uint8_t driver_signature, uint8_t driver_data)
{
    VdFcGuid result;
    uint16_t *guid16 = (uint16_t*)result.dat;
    uint16_t crc = 0;

    VD_FW_MEMSET(&result, 0, sizeof(result));

    if (vendor_name && *vendor_name && product_name && *product_name) {
        crc = vd_fc__crc16(crc, vendor_name,  vd_fw__strlen(vendor_name));
        crc = vd_fc__crc16(crc, " ", 1);
        crc = vd_fc__crc16(crc, product_name, vd_fw__strlen(product_name));
    } else if (product_name) {
        crc = vd_fc__crc16(crc, product_name, vd_fw__strlen(product_name));
    }

    *guid16++ = VD_FC_SWAP16LE(bus);
    *guid16++ = VD_FC_SWAP16LE(crc);

    if (vendor) {
        *guid16++ = VD_FC_SWAP16LE(vendor);
        *guid16++ = 0;
        *guid16++ = VD_FC_SWAP16LE(product);
        *guid16++ = 0;
        *guid16++ = VD_FC_SWAP16LE(version);
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

static int vd_fc__strlen(const char *s)
{
    int r = 0;
    while (*s++) r++;
    return r;
}

static size_t vd_fc__strlcpy(char *dst, const char *src, size_t maxlen)
{
    size_t srclen = vd_fc__strlen(src);
    if (maxlen > 0) {
        size_t len = srclen < (maxlen - 1) ? srclen : (maxlen - 1);
        VD_FC_MEMCPY(dst, (void*)src, len);
        dst[len] = '\0';
    }
    return srclen;
}

static uint64_t vd_fc__uid_next(void)
{
    if (Vd_Fc_G.uid_next == 0) {
        Vd_Fc_G.uid_next++;
    }

    return Vd_Fc_G.uid_next++;
}

static float vd_fc__clampf(float v, float m, float x)
{
    if (v < m) return m;
    if (v > x) return x;
    return v;
}

static int vd_fc__approxeq(float a, float b)
{
    float v = a - b;
    if (v < 0) v = -v;

    if (v < 0.00001f) {
        return 1;
    }

    return 0;
}

static int vd_fc__approxeq_rumble(VdFcRumbleState *a, VdFcRumbleState *b)
{
    return vd_fc__approxeq(a->left_motor, b->left_motor) &&
           vd_fc__approxeq(a->right_motor, b->right_motor) &&
           vd_fc__approxeq(a->left_trigger, b->left_trigger) &&
           vd_fc__approxeq(a->right_trigger, b->right_trigger);
}

static VdFc__CommonDeviceData vd_fc__device_get_common_data(uint16_t vendor_id, uint16_t product_id, uint32_t output_report_size)
{
    VdFc__CommonDeviceData result;
    result.flags = 0;
    result.identified_name = 0;

    if (vendor_id == 0x045e /* Microsoft */) {
        switch (product_id) {
            // USB_PRODUCT_XBOX360_XUSB_CONTROLLER (XUSB driver software PID)
            case 0x02a1: result.identified_name = "XBOX 360 Controller";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX; break;
            // USB_PRODUCT_XBOX360_WIRED_CONTROLLER
            case 0x028e: result.identified_name = "XBOX 360 Controller (Wired)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX; break;
            // USB_PRODUCT_XBOX360_WIRELESS_RECEIVER
            case 0x0719: result.identified_name = "XBOX 360 Controller (Wireless)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX | VD_FC__COMMON_DEVICE_WIRELESS; break;
            // USB_PRODUCT_XBOX360_WIRELESS_RECEIVER_THIRDPARTY1
            case 0x02a9: result.identified_name = "XBOX 360 Controller (Wireless)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX | VD_FC__COMMON_DEVICE_WIRELESS; break;
            // USB_PRODUCT_XBOX360_WIRELESS_RECEIVER_THIRDPARTY2
            case 0x0291: result.identified_name = "XBOX 360 Controller (Wireless)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX | VD_FC__COMMON_DEVICE_WIRELESS; break;
            // USB_PRODUCT_XBOX_ONE_ADAPTIVE
            case 0x0b0a: result.identified_name = "XBOX One Controller";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX; break;
            // USB_PRODUCT_XBOX_ONE_ADAPTIVE_BLUETOOTH
            case 0x0b0c: result.identified_name = "XBOX One Controller (BT)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX | VD_FC__COMMON_DEVICE_WIRELESS; break;
            // USB_PRODUCT_XBOX_ONE_ADAPTIVE_BLE
            case 0x0b21: result.identified_name = "XBOX One Controller (BLE)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX | VD_FC__COMMON_DEVICE_WIRELESS; break;
            // USB_PRODUCT_XBOX_ONE_ELITE_SERIES_1
            case 0x02e3: result.identified_name = "XBOX One Elite Series 1 Controller";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX; break;
            // USB_PRODUCT_XBOX_ONE_ELITE_SERIES_2
            case 0x0b00: result.identified_name = "XBOX One Elite Series 2 Controller";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX; break;
            // USB_PRODUCT_XBOX_ONE_ELITE_SERIES_2_BLUETOOTH
            case 0x0b05: result.identified_name = "XBOX One Elite Series 2 Controller (BT)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX | VD_FC__COMMON_DEVICE_WIRELESS; break;
            // USB_PRODUCT_XBOX_ONE_ELITE_SERIES_2_BLE
            case 0x0b22: result.identified_name = "XBOX One Elite Series 2 Controller (BLE)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX | VD_FC__COMMON_DEVICE_WIRELESS; break;
            // USB_PRODUCT_XBOX_ONE_S
            case 0x02ea: result.identified_name = "XBOX One S Controller";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX; break;
            // USB_PRODUCT_XBOX_ONE_S_REV1_BLUETOOTH
            case 0x02e0: result.identified_name = "XBOX One S Controller Rv.1 (BT)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX | VD_FC__COMMON_DEVICE_WIRELESS; break;
            // USB_PRODUCT_XBOX_ONE_S_REV2_BLUETOOTH
            case 0x02fd: result.identified_name = "XBOX One S Controller Rv.2 (BT)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX | VD_FC__COMMON_DEVICE_WIRELESS; break;
            // USB_PRODUCT_XBOX_ONE_S_REV2_BLE
            case 0x0b20: result.identified_name = "XBOX One S Controller Rv.2 (BLE)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX | VD_FC__COMMON_DEVICE_WIRELESS; break;
            // USB_PRODUCT_XBOX_SERIES_X
            case 0x0b12: result.identified_name = "XBOX Series X Controller";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX; break;
            // USB_PRODUCT_XBOX_SERIES_X_BLE
            case 0x0b13: result.identified_name = "XBOX Series X Controller (BLE)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX | VD_FC__COMMON_DEVICE_WIRELESS; break;
            // USB_PRODUCT_XBOX_ONE_XBOXGIP_CONTROLLER (XBOXGIP driver software PID)
            case 0x02ff: result.identified_name = "XBOX One Controller (GIP)";
                         result.flags |= VD_FC__COMMON_DEVICE_XBOX; break;
            default: break;
        }
    } else if (vendor_id == 0x054c /* Sony */) {
        switch (product_id) {
            case 0x05c4: result.identified_name = "Sony DualShock 4 Gen1";
                         result.flags |= VD_FC__COMMON_DEVICE_DS4; break;
            case 0x09cc: result.identified_name = "Sony DualShock 4 Gen2";
                         result.flags |= VD_FC__COMMON_DEVICE_DS4; break;
            default: break;
        }
    }

    uint32_t device_type = result.flags & VD_FC__COMMON_DEVICE_FLAGS_PRODUCT_BITMASK;
    if (device_type == VD_FC__COMMON_DEVICE_DS4) {
        if (output_report_size == 547) {
            result.flags |= VD_FC__COMMON_DEVICE_WIRELESS;
        }
    }

    return result;
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

VD_FC_API uint64_t vd_fc_uid(int id)
{
    if (id < Vd_Fc_G.num_devices) {
        return Vd_Fc_G.devices[id].uid;
    } else {
        return 0;
    }
}

VD_FC_API int vd_fc_id_from_uid(uint64_t uid)
{
    for (int i = 0; i < Vd_Fc_G.num_devices; ++i) {
        if (Vd_Fc_G.devices[i].uid == uid) {
            return i;
        }
    }
    return -1;
}

VD_FC_API const char *vd_fc_name(int id)
{
    if (id < Vd_Fc_G.num_devices) {
        if (Vd_Fc_G.devices[id].common.identified_name != 0) {
            return Vd_Fc_G.devices[id].common.identified_name;
        } else {
            return Vd_Fc_G.devices[id].name;
        }
    } else {
        return "N/A";
    }
}

VD_FC_API int vd_fc_ff_rumble_any(int id)
{
    VdFcRumbleState state = vd_fc_ff_rumble_get(id);

    float accum = state.left_motor + state.right_motor + state.left_trigger + state.right_trigger;

    if (accum > 0.00001f) {
        return 1;
    }

    return 0;
}

#endif // VD_FC_IMPL
