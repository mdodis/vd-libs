/* vd_ft.h Common Interface between platform-specific & platform-agnostic font backends 
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
 * TODO
 * - Parse table
 */
#ifndef VD_FT_H
#define VD_FT_H
#define VD_FT_VERSION_MAJOR    0
#define VD_FT_VERSION_MINOR    0
#define VD_FT_VERSION_PATCH    1
#define VD_FT_VERSION          ((VD_FT_VERSION_MAJOR << 16) | (VD_FT_VERSION_MINOR << 8) | (VD_FT_VERSION_PATCH))

#ifndef VD_FT_API
#   ifdef VD_FT_STATIC
#       define VD_FT_API static
#   else
#       define VD_FT_API extern
#   endif // VD_FT_STATIC
#endif // !VD_FT_API

#ifndef VD_FT_INL
#   define VD_FT_INL static inline
#endif // !VD_FT_INL

typedef struct {
    void *reserved0; // Win32(HANDLE to font resource)
    void *reserved1; // Win32(HFONT)
    char *name;
    int  name_len;
    int  units_per_em;
    int  ascent, descent, linegap;
} VdFtFont;

VD_FT_API int       vd_ft_font_load(VdFtFont *font, void *memory, int size, int index);
VD_FT_API VdFtFont* vd_ft_get_system_fonts(int *count);

#endif // !VD_FT_H

#ifdef VD_FT_IMPL
#include <stdint.h>
#include <string.h>

#define VD_FT__MIN(x, y) ((x) < (y) ? (x) : (y))
static void* vd_ft__realloc_mem(void *prev_ptr, size_t size);
static void  vd_ft__free_mem(void *ptr);
static int   vd_ft__mac_roman_to_wide(uint8_t *strdata, uint16_t string_length, wchar_t *wbuf, int wbuf_len);
static int   vd_ft__latin1_to_wide(uint8_t *strdata, uint16_t string_length, wchar_t *wbuf, int wbuf_len);
static int   vd_ft__wide_to_utf8(wchar_t *wbuf, int wlen, char *buf, int blen);
static int   vd_ft__utf8_to_wide(char *buf, int blen, wchar_t *wbuf, int wlen);

static uint16_t vd_ft__swapu16(uint16_t i)
{
    uint16_t result = 0;
    result |= (i >> 8) & 0xFF;
    result |= (i << 8) & 0xFF00;
    return result;
}

static uint32_t vd_ft__swapu32(uint32_t i)
{
    uint32_t result = 0;
    uint8_t b0 = (i >> 24) & 0xFF;
    uint8_t b1 = (i >> 16) & 0xFF;
    uint8_t b2 = (i >>  8) & 0xFF;
    uint8_t b3 = (i >>  0) & 0xFF;
    result = (b0 << 0) | (b1 << 8) | (b2 << 16) | (b3 << 24);
    return result;
}

static void vd_ft__swapwstr(wchar_t *s, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        s[i] = vd_ft__swapu16(s[i]);
    }
}

typedef struct {
    uint32_t  version;
    uint16_t  num_tables;
    uint16_t  search_range;
    uint16_t  entry_selector;
    uint16_t  range_shift;
} VdFt__TTFOffsetTable;

typedef struct {
    char     tag[4];
    uint32_t checksum;
    uint32_t offset;
    uint32_t length;
} VdFt__TTFTableDirectory;

typedef struct {
    uint16_t  format_selector;
    uint16_t  count_name_records;
    uint16_t  offset_start_of_string_storage;
} VdFt__TTFNamingTable;

typedef struct {
    uint16_t  platform_id;
    uint16_t  encoding_id;
    uint16_t  language_id;
    uint16_t  name_id;
    uint16_t  string_length;
    uint16_t  offset_from_storage_area;
} VdFt__TTFNameRecord;

static char vd_ft__to_lower(char c)
{
    if ((c >= 'A') && (c <= 'Z')) return c - 'A' + 'a';
    return c;
}

static int vd_ft__read_ptr(void *dst, size_t dst_size, uint8_t *data, int size, size_t *offset)
{
    if ((*offset + dst_size) > size) {
        return 0;
    }

    memcpy(dst, data + *offset, dst_size);
    *offset += dst_size;
    return 1;
}

static int vd_ft__get_ttf_name_records(uint8_t *data, int size, char **out_name, int *out_name_size)
{
#define VD_FT__CHECK_1(expr) if ((expr) != 1) { return 0; }
    size_t offset = 0;

    VdFt__TTFOffsetTable offset_table;

    // Read offset table
    {
        offset_table = *((VdFt__TTFOffsetTable*)(data + offset));
        vd_ft__read_ptr(&offset_table, sizeof(offset_table), data, size, &offset);
    }

    for (uint16_t i = 0; i < offset_table.num_tables; ++i) {
        VdFt__TTFTableDirectory table_dir;
        vd_ft__read_ptr(&table_dir, sizeof(table_dir), data, size, &offset);

        char tag_lower[4];
        for (int j = 0; j < 4; ++j) {
            tag_lower[j] = vd_ft__to_lower(table_dir.tag[j]);
        }

        int tag_is_name = tag_lower[0] == 'n' &&
                          tag_lower[1] == 'a' &&
                          tag_lower[2] == 'm' &&
                          tag_lower[3] == 'e';

        if (!tag_is_name) {
            continue;
        }

        table_dir.offset = vd_ft__swapu32(table_dir.offset);
        offset = table_dir.offset;

        VdFt__TTFNamingTable naming_table;
        vd_ft__read_ptr(&naming_table, sizeof(naming_table), data, size, &offset);

        naming_table.count_name_records             = vd_ft__swapu16(naming_table.count_name_records);
        naming_table.offset_start_of_string_storage = vd_ft__swapu16(naming_table.offset_start_of_string_storage);
        naming_table.format_selector                = vd_ft__swapu16(naming_table.format_selector);

        VdFt__TTFNameRecord *name_records = vd_ft__realloc_mem(NULL, sizeof(VdFt__TTFNameRecord) * naming_table.count_name_records);
        for (uint16_t name_record_index = 0; name_record_index < naming_table.count_name_records; ++name_record_index) {
            VdFt__TTFNameRecord name_record;
            vd_ft__read_ptr(&name_record, sizeof(name_record), data, size, &offset);
            name_record.platform_id              = vd_ft__swapu16(name_record.platform_id);
            name_record.encoding_id              = vd_ft__swapu16(name_record.encoding_id);
            name_record.language_id              = vd_ft__swapu16(name_record.language_id);
            name_record.name_id                  = vd_ft__swapu16(name_record.name_id);
            name_record.string_length            = vd_ft__swapu16(name_record.string_length);
            name_record.offset_from_storage_area = vd_ft__swapu16(name_record.offset_from_storage_area);
            name_records[name_record_index] = name_record;
        }

        VdFt__TTFNameRecord *best_name_record = NULL;
        // @todo(mdodis): Switch to single memory buffer instead of allocating here
        for (uint16_t name_record_index = 0; name_record_index < naming_table.count_name_records; ++name_record_index) {
            VdFt__TTFNameRecord *name_record = &name_records[name_record_index];

            if ((name_record->name_id == 1) && (name_record->platform_id == 3)) {
                best_name_record = name_record;
                break;
            }
        }

        if (best_name_record == NULL) {
            for (uint16_t name_record_index = 0; name_record_index < naming_table.count_name_records; ++name_record_index) {
                VdFt__TTFNameRecord *name_record = &name_records[name_record_index];
                if (name_record->name_id == 1) {
                    best_name_record = name_record;
                    break;
                }

            }
        }

        if (best_name_record == NULL) {
            goto CANDIDATE_FAIL_CONTINUE;
        }

        // Decode name string to UTF-8
        {
            VdFt__TTFNameRecord *name_record = best_name_record;

            offset = table_dir.offset + 
                     naming_table.offset_start_of_string_storage + 
                     name_record->offset_from_storage_area;

            uint8_t *strdata = data + offset;

            wchar_t wbuf[512];

            int wlen = 0;
            if (name_record->platform_id == 0 || (name_record->platform_id == 3 && (name_record->encoding_id == 0 || name_record->encoding_id == 1))) {
                // UTF-16BE
                wlen = VD_FT__MIN(name_record->string_length / 2, (uint16_t)(sizeof(wbuf) / sizeof(wbuf[0]) - 1));
                for (int c = 0; c < wlen; c++)
                    wbuf[c] = (strdata[2 * c] << 8) | strdata[2 * c + 1];
                wbuf[wlen] = L'\0';
            } else if (name_record->platform_id == 3 && name_record->encoding_id == 10) {
                // UCS-4
                int count = name_record->string_length / 4;
                for (int c = 0; c < count && wlen < (int)(sizeof(wbuf)/sizeof(wbuf[0]) - 1); c++) {
                    uint32_t cp = (strdata[4*c]   << 24) |
                                  (strdata[4*c+1] << 16) |
                                  (strdata[4*c+2] <<  8) |
                                  (strdata[4*c+3]);
                    if (cp <= 0xFFFF)
                        wbuf[wlen++] = (wchar_t)cp;
                    else
                        wbuf[wlen++] = L'?';
                }
                wbuf[wlen] = L'\0';
            } else if (name_record->platform_id == 1) {
                // Mac-Roman CP10000
                wlen = vd_ft__mac_roman_to_wide(strdata, name_record->string_length, wbuf, sizeof(wbuf) / sizeof(wbuf[0]));
            } else {
                // Fallback: Latin-1
                wlen = vd_ft__latin1_to_wide(strdata, name_record->string_length, wbuf, sizeof(wbuf) / sizeof(wbuf[0]));
            }

            // Convert to UTF-8
            int utf8_len = vd_ft__wide_to_utf8(wbuf, wlen, 0, 0);
            char *utf8 = (char*)vd_ft__realloc_mem(NULL, utf8_len + 1);
            int utf8_wrt = vd_ft__wide_to_utf8(wbuf, wlen, utf8, utf8_len);
            utf8[utf8_wrt] = 0;
            *out_name = utf8;
            *out_name_size = utf8_wrt;
        }

        vd_ft__free_mem(name_records);
        break;

CANDIDATE_FAIL_CONTINUE: 
        vd_ft__free_mem(name_records);
        continue;
    }
#undef VD_FT__CHECK_1

    return 1;
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
typedef uint32_t            VdFtUINT32;
typedef uint16_t            VdFtUINT16;
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

VD_FT_DECLARE_HANDLE(VdFtHWND);
VD_FT_DECLARE_HANDLE(VdFtHINSTANCE);
VD_FT_DECLARE_HANDLE(VdFtHDC);
VD_FT_DECLARE_HANDLE(VdFtHMONITOR);
typedef VdFtHINSTANCE VdFtHMODULE;

extern VdFtHMODULE LoadLibraryA(VdFtLPCSTR path);
extern void*       HeapAlloc(VdFtHANDLE hHeap, VdFtDWORD dwFlags, VdFtSIZE_T dwBytes);
extern VdFtHANDLE  GetProcessHeap();
extern void*       HeapReAlloc(VdFtHANDLE hHeap, VdFtDWORD dwFlags, void *lpMem, VdFtSIZE_T dwBytes);
extern VdFtBOOL    HeapFree(VdFtHANDLE hHeap, VdFtDWORD dwFlags, void *lpMem);
extern int         MultiByteToWideChar(VdFtUINT CodePage, VdFtDWORD dwFlags, VdFtLPCSTR lpMultiByteStr, int cbMultiByte, VdFtLPWSTR lpWideCharStr, int cchWideChar);
extern int         WideCharToMultiByte(VdFtUINT CodePage, VdFtDWORD dwFlags, VdFtLPCWSTR lpWideCharStr, int cchWideChar, VdFtLPSTR lpMultiByteStr, int cbMultiByte, VdFtLPSTR lpDefaultChar, VdFtBOOL *lpUsedDefaultChar);
extern void*       GetProcAddress(VdFtHMODULE hModule, VdFtLPCSTR lpProcName);

#else
typedef HWND      VdFtHWND;
typedef DWORD     VdFtDWORD;
typedef WORD      VdFtWORD;
typedef LPWSTR    VdFtLPWSTR;
typedef LPCWSTR   VdFtLPCWSTR;
typedef LPCSTR    VdFtLPCSTR;
typedef DWORD     VdFtDWORD;
typedef UINT      VdFtUINT;
typedef UINT32    VdFtUINT32;
typedef UINT16    VdFtUINT16;
typedef HMODULE   VdFtHMODULE;
typedef HRESULT   VdFtHRESULT;
typedef ULONG     VdFtULONG;
typedef HDC       VdFtHDC;
typedef HMONITOR  VdFtHMONITOR;
typedef HANDLE    VdFtHANDLE;
typedef LPARAM    VdFtLPARAM;
typedef BOOL      VdFtPBOOL;
typedef LONG      VdFtLONG;
typedef BYTE      VdFtBYTE;
typedef WCHAR     VdFtWCHAR;
typedef BOOL      VdFtBOOL;
typedef FLOAT     VdFtFLOAT;
#endif // !_MINWINDEF_

typedef struct VdFttagLOGFONTW {
    VdFtLONG  lfHeight;
    VdFtLONG  lfWidth;
    VdFtLONG  lfEscapement;
    VdFtLONG  lfOrientation;
    VdFtLONG  lfWeight;
    VdFtBYTE  lfItalic;
    VdFtBYTE  lfUnderline;
    VdFtBYTE  lfStrikeOut;
    VdFtBYTE  lfCharSet;
    VdFtBYTE  lfOutPrecision;
    VdFtBYTE  lfClipPrecision;
    VdFtBYTE  lfQuality;
    VdFtBYTE  lfPitchAndFamily;
    VdFtWCHAR lfFaceName[32];
} VdFtLOGFONTW, *VdFtPLOGFONTW, *VdFtNPLOGFONTW, *VdFtLPLOGFONTW;

typedef struct VdFttagTEXTMETRICW {
    VdFtLONG  tmHeight;
    VdFtLONG  tmAscent;
    VdFtLONG  tmDescent;
    VdFtLONG  tmInternalLeading;
    VdFtLONG  tmExternalLeading;
    VdFtLONG  tmAveCharWidth;
    VdFtLONG  tmMaxCharWidth;
    VdFtLONG  tmWeight;
    VdFtLONG  tmOverhang;
    VdFtLONG  tmDigitizedAspectX;
    VdFtLONG  tmDigitizedAspectY;
    VdFtWCHAR tmFirstChar;
    VdFtWCHAR tmLastChar;
    VdFtWCHAR tmDefaultChar;
    VdFtWCHAR tmBreakChar;
    VdFtBYTE  tmItalic;
    VdFtBYTE  tmUnderlined;
    VdFtBYTE  tmStruckOut;
    VdFtBYTE  tmPitchAndFamily;
    VdFtBYTE  tmCharSet;
} VdFtTEXTMETRICW, *VdFtPTEXTMETRICW, *VdFtNPTEXTMETRICW, *VdFtLPTEXTMETRICW;

typedef struct VdFt_GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} VdFtGUID;

typedef struct VdFt_FILETIME {
  VdFtDWORD dwLowDateTime;
  VdFtDWORD dwHighDateTime;
} VdFtFILETIME, *VdFtPFILETIME, *VdFtLPFILETIME;

VD_FT_INL int vd_ft__guid_compare(const VdFtGUID *a, const VdFtGUID *b)
{
    return (a->Data1 == b->Data1)       && (a->Data2 == b->Data2)       && (a->Data3 == b->Data3) &&
           (a->Data4[0] == b->Data4[0]) && (a->Data4[1] == b->Data4[1]) &&
           (a->Data4[2] == b->Data4[2]) && (a->Data4[3] == b->Data4[3]) &&
           (a->Data4[4] == b->Data4[4]) && (a->Data4[5] == b->Data4[5]) &&
           (a->Data4[6] == b->Data4[6]) && (a->Data4[7] == b->Data4[7]);
}

typedef VdFtGUID            VdFtIID;
typedef VdFtGUID*           VdFtREFGUID;
#define VdFtREFCLSID        const VdFtIID *
typedef VdFtIID*            VdFtREFIID;
typedef struct VdFtIUnknown VdFtIUnknown;
typedef VdFtIUnknown*       VdFtLPUNKNOWN;
typedef VdFtULONG           VdFtSFGAOF;
typedef VdFtDWORD           VdFtSICHINTF;

typedef struct VdFtIDWriteFactory VdFtIDWriteFactory;
typedef struct VdFtIDWriteFactory1 VdFtIDWriteFactory1;

typedef enum {
    VD_FT_DWRITE_FACTORY_TYPE_SHARED,
    VD_FT_DWRITE_FACTORY_TYPE_ISOLATED
} VdFtDWRITE_FACTORY_TYPE;

typedef enum {
    VD_FT_DWRITE_FONT_FACE_TYPE_CFF,
    VD_FT_DWRITE_FONT_FACE_TYPE_TRUETYPE,
    VD_FT_DWRITE_FONT_FACE_TYPE_OPENTYPE_COLLECTION,
    VD_FT_DWRITE_FONT_FACE_TYPE_TYPE1,
    VD_FT_DWRITE_FONT_FACE_TYPE_VECTOR,
    VD_FT_DWRITE_FONT_FACE_TYPE_BITMAP,
    VD_FT_DWRITE_FONT_FACE_TYPE_UNKNOWN,
    VD_FT_DWRITE_FONT_FACE_TYPE_RAW_CFF,
    VD_FT_DWRITE_FONT_FACE_TYPE_TRUETYPE_COLLECTION = VD_FT_DWRITE_FONT_FACE_TYPE_OPENTYPE_COLLECTION,
} VdFtDWRITE_FONT_FACE_TYPE;

typedef enum {
    VD_FT_DWRITE_FONT_SIMULATIONS_NONE    = 0x0000,
    VD_FT_DWRITE_FONT_SIMULATIONS_BOLD    = 0x0001,
    VD_FT_DWRITE_FONT_SIMULATIONS_OBLIQUE = 0x0002
} VdFtDWRITE_FONT_SIMULATIONS;

typedef enum {
    VD_FT_DWRITE_PIXEL_GEOMETRY_FLAT,
    VD_FT_DWRITE_PIXEL_GEOMETRY_RGB,
    VD_FT_DWRITE_PIXEL_GEOMETRY_BGR
} VdFtDWRITE_PIXEL_GEOMETRY;

typedef enum {
    VD_FT_DWRITE_RENDERING_MODE_DEFAULT,
    VD_FT_DWRITE_RENDERING_MODE_ALIASED,
    VD_FT_DWRITE_RENDERING_MODE_GDI_CLASSIC,
    VD_FT_DWRITE_RENDERING_MODE_GDI_NATURAL,
    VD_FT_DWRITE_RENDERING_MODE_NATURAL,
    VD_FT_DWRITE_RENDERING_MODE_NATURAL_SYMMETRIC,
    VD_FT_DWRITE_RENDERING_MODE_OUTLINE,
    VD_FT_DWRITE_RENDERING_MODE_CLEARTYPE_GDI_CLASSIC         = VD_FT_DWRITE_RENDERING_MODE_GDI_CLASSIC,
    VD_FT_DWRITE_RENDERING_MODE_CLEARTYPE_GDI_NATURAL         = VD_FT_DWRITE_RENDERING_MODE_GDI_NATURAL,
    VD_FT_DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL             = VD_FT_DWRITE_RENDERING_MODE_NATURAL,
    VD_FT_DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL_SYMMETRIC   = VD_FT_DWRITE_RENDERING_MODE_NATURAL_SYMMETRIC
} VdFtDWRITE_RENDERING_MODE;

typedef enum {
    VD_FT_DWRITE_FONT_WEIGHT_THIN = 100,
    VD_FT_DWRITE_FONT_WEIGHT_EXTRA_LIGHT = 200,
    VD_FT_DWRITE_FONT_WEIGHT_ULTRA_LIGHT = 200,
    VD_FT_DWRITE_FONT_WEIGHT_LIGHT = 300,
    VD_FT_DWRITE_FONT_WEIGHT_SEMI_LIGHT = 350,
    VD_FT_DWRITE_FONT_WEIGHT_NORMAL = 400,
    VD_FT_DWRITE_FONT_WEIGHT_REGULAR = 400,
    VD_FT_DWRITE_FONT_WEIGHT_MEDIUM = 500,
    VD_FT_DWRITE_FONT_WEIGHT_DEMI_BOLD = 600,
    VD_FT_DWRITE_FONT_WEIGHT_SEMI_BOLD = 600,
    VD_FT_DWRITE_FONT_WEIGHT_BOLD = 700,
    VD_FT_DWRITE_FONT_WEIGHT_EXTRA_BOLD = 800,
    VD_FT_DWRITE_FONT_WEIGHT_ULTRA_BOLD = 800,
    VD_FT_DWRITE_FONT_WEIGHT_BLACK = 900,
    VD_FT_DWRITE_FONT_WEIGHT_HEAVY = 900,
    VD_FT_DWRITE_FONT_WEIGHT_EXTRA_BLACK = 950,
    VD_FT_DWRITE_FONT_WEIGHT_ULTRA_BLACK = 950
} VdFtDWRITE_FONT_WEIGHT;

typedef enum {
    VD_FT_DWRITE_FONT_STRETCH_UNDEFINED = 0,
    VD_FT_DWRITE_FONT_STRETCH_ULTRA_CONDENSED = 1,
    VD_FT_DWRITE_FONT_STRETCH_EXTRA_CONDENSED = 2,
    VD_FT_DWRITE_FONT_STRETCH_CONDENSED = 3,
    VD_FT_DWRITE_FONT_STRETCH_SEMI_CONDENSED = 4,
    VD_FT_DWRITE_FONT_STRETCH_NORMAL = 5,
    VD_FT_DWRITE_FONT_STRETCH_MEDIUM = 5,
    VD_FT_DWRITE_FONT_STRETCH_SEMI_EXPANDED = 6,
    VD_FT_DWRITE_FONT_STRETCH_EXPANDED = 7,
    VD_FT_DWRITE_FONT_STRETCH_EXTRA_EXPANDED = 8,
    VD_FT_DWRITE_FONT_STRETCH_ULTRA_EXPANDED = 9
} VdFtDWRITE_FONT_STRETCH;

typedef enum {
    VD_FT_DWRITE_FONT_STYLE_NORMAL,
    VD_FT_DWRITE_FONT_STYLE_OBLIQUE,
    VD_FT_DWRITE_FONT_STYLE_ITALIC
} VdFtDWRITE_FONT_STYLE;

typedef enum {
    VD_FT_DWRITE_NUMBER_SUBSTITUTION_METHOD_FROM_CULTURE,
    VD_FT_DWRITE_NUMBER_SUBSTITUTION_METHOD_CONTEXTUAL,
    VD_FT_DWRITE_NUMBER_SUBSTITUTION_METHOD_NONE,
    VD_FT_DWRITE_NUMBER_SUBSTITUTION_METHOD_NATIONAL,
    VD_FT_DWRITE_NUMBER_SUBSTITUTION_METHOD_TRADITIONAL
} VdFtDWRITE_NUMBER_SUBSTITUTION_METHOD;

typedef enum {
    VD_FT_DWRITE_MEASURING_MODE_NATURAL = 0,
    VD_FT_DWRITE_MEASURING_MODE_GDI_CLASSIC = 1,
    VD_FT_DWRITE_MEASURING_MODE_GDI_NATURAL = 2
} VdFtDWRITE_MEASURING_MODE;

typedef struct {
    VdFtFLOAT m11;
    VdFtFLOAT m12;
    VdFtFLOAT m21;
    VdFtFLOAT m22;
    VdFtFLOAT dx;
    VdFtFLOAT dy;
} VdFtDWRITE_MATRIX;

typedef struct {
    VdFtFLOAT advanceOffset;
    VdFtFLOAT ascenderOffset;
} VdFtDWRITE_GLYPH_OFFSET;

typedef struct {
    VdFtIUnknown /* IDWriteFontFace */ * fontFace;
    VdFtFLOAT fontEmSize;
    VdFtUINT32 glyphCount;
    VdFtUINT16 const* glyphIndices;
    VdFtFLOAT const* glyphAdvances;
    VdFtDWRITE_GLYPH_OFFSET const* glyphOffsets;
    VdFtBOOL isSideways;
    VdFtUINT32 bidiLevel;
} VdFtDWRITE_GLYPH_RUN;

#pragma pack(pop)

typedef int (VdFtFONTENUMPROCW)(const VdFtLOGFONTW *lpelfe, const VdFtTEXTMETRICW *lpntme, VdFtDWORD FontType, VdFtLPARAM lParam);
typedef int (*VdFt__ProcEnumFontFamiliesExW)(VdFtHDC hdc, VdFtLPLOGFONTW lpLogfont, VdFtFONTENUMPROCW lpProc, VdFtLPARAM lParam, VdFtDWORD dwFlags);
typedef VdFtHANDLE (*VdFt__ProcAddFontMemResourceEx)(void *pFileView, VdFtDWORD cjSize, void *pvReserved, VdFtDWORD *pNumFonts);
typedef VdFtHDC (*VdFt__ProcCreateCompatibleDC)(VdFtHDC hdc);

static VdFt__ProcAddFontMemResourceEx VdFt__AddFontMemResourceEx;
static VdFt__ProcEnumFontFamiliesExW  VdFt__EnumFontFamiliesExW;
static VdFt__ProcCreateCompatibleDC VdFt__CreateCompatibleDC;

typedef struct {
    VdFtHRESULT (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG   (__stdcall *Release)(VdFtIUnknown *This);
} VdFtIUnknownVtbl;
struct VdFtIUnknown { const VdFtIUnknownVtbl *lpVtbl; };

#define VD_FT_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) VdFtGUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

VD_FT_DEFINE_GUID(VD_FT_IID_IDWriteFactory,  0xb859ee5a, 0xd838, 0x4b5b, 0xa2, 0xe8, 0x1a, 0xdc, 0x7d, 0x93, 0xdb, 0x48);
typedef struct {
    /* IUnknown */
    VdFtHRESULT (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG   (__stdcall *Release)(VdFtIUnknown *This);

    /* IDWriteFactory */
    VdFtHRESULT (__stdcall *GetSystemFontCollection)(VdFtIUnknown** /*IDWriteFontCollection*/ fontCollection, VdFtBOOL checkForUpdates);
    VdFtHRESULT (__stdcall *CreateCustomFontCollection)(VdFtIUnknown* /*IDWriteFontCollectionLoader*/, void const *collectionKey, VdFtUINT32 collectionKeySize, VdFtIUnknown** /*IDWriteFontCollection*/ fontCollection);
    VdFtHRESULT (__stdcall *RegisterFontCollectionLoader)(VdFtIUnknown* /*IDWriteFontCollectionLoader*/ fontCollectionLoader);
    VdFtHRESULT (__stdcall *UnregisterFontCollectionLoader)(VdFtIUnknown* /*IDWriteFontCollectionLoader*/ fontCollectionLoader);
    VdFtHRESULT (__stdcall *CreateFontFileReference)(VdFtWCHAR const* filePath, VdFtFILETIME const* lastWriteTime, VdFtIUnknown /*IDWriteFontFile*/ ** fontFile);
    VdFtHRESULT (__stdcall *CreateCustomFontFileReference)(void const* fontFileReferenceKey, VdFtUINT32 fontFileReferenceKeySize, VdFtIUnknown /* IDWriteFontFileLoader */ * fontFileLoader, VdFtIUnknown /* IDWriteFontFile */ ** fontFile);
    VdFtHRESULT (__stdcall *CreateFontFace)(VdFtDWRITE_FONT_FACE_TYPE fontFaceType, VdFtUINT32 numberOfFiles, VdFtIUnknown /*IDWriteFontFile*/ * const* fontFiles, VdFtUINT32 faceIndex, VdFtDWRITE_FONT_SIMULATIONS fontFaceSimulationFlags, VdFtIUnknown /* IDWriteFontFace */ ** fontFace);
    VdFtHRESULT (__stdcall *CreateRenderingParams)(VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *CreateMonitorRenderingParams)(VdFtHMONITOR monitor, VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *CreateCustomRenderingParams)(VdFtFLOAT gamma, VdFtFLOAT enhancedContrast, VdFtFLOAT clearTypeLevel, VdFtDWRITE_PIXEL_GEOMETRY pixelGeometry, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *RegisterFontFileLoader)(VdFtIUnknown /* IDWriteFontFileLoader */ * fontFileLoader);
    VdFtHRESULT (__stdcall *UnregisterFontFileLoader)(VdFtIUnknown /* IDWriteFontFileLoader */ * fontFileLoader);
    VdFtHRESULT (__stdcall *CreateTextFormat)(VdFtWCHAR const* fontFamilyName, VdFtIUnknown /* IDWriteFontCollection */ * fontCollection, VdFtDWRITE_FONT_WEIGHT fontWeight, VdFtDWRITE_FONT_STYLE fontStyle, VdFtDWRITE_FONT_STRETCH fontStretch, VdFtFLOAT fontSize, VdFtWCHAR const* localeName, VdFtIUnknown /* IDWriteTextFormat */ ** textFormat);
    VdFtHRESULT (__stdcall *CreateTypography)(VdFtIUnknown /* IDWriteTypography */ ** typography);
    VdFtHRESULT (__stdcall *GetGdiInterop)(VdFtIUnknown /* IDWriteGdiInterop */ ** gdiInterop);
    VdFtHRESULT (__stdcall *CreateTextLayout)(VdFtWCHAR const* string, VdFtUINT32 stringLength, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtFLOAT maxWidth, VdFtFLOAT maxHeight, VdFtIUnknown /* IDWriteTextLayout */ ** textLayout);
    VdFtHRESULT (__stdcall *CreateGdiCompatibleTextLayout)(VdFtWCHAR const* string, VdFtUINT32 stringLength, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtFLOAT layoutWidth, VdFtFLOAT layoutHeight, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtBOOL useGdiNatural, VdFtIUnknown /* IDWriteTextLayout */ ** textLayout);
    VdFtHRESULT (__stdcall *CreateEllipsisTrimmingSign)(VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtIUnknown /* IDWriteInlineObject */ ** trimmingSign);
    VdFtHRESULT (__stdcall *CreateTextAnalyzer)(VdFtIUnknown /* IDWriteTextAnalyzer */ ** textAnalyzer);
    VdFtHRESULT (__stdcall *CreateNumberSubstitution)(VdFtDWRITE_NUMBER_SUBSTITUTION_METHOD substitutionMethod, VdFtWCHAR const* localeName, VdFtBOOL ignoreUserOverride, VdFtIUnknown /* IDWriteNumberSubstitution */ ** numberSubstitution);
    VdFtHRESULT (__stdcall *CreateGlyphRunAnalysis)(VdFtDWRITE_GLYPH_RUN const* glyphRun, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtDWRITE_MEASURING_MODE measuringMode, VdFtFLOAT baselineOriginX, VdFtFLOAT baselineOriginY, VdFtIUnknown /* IDWriteGlyphRunAnalysis */ ** glyphRunAnalysis);

    // @todo(mdodis): IDWriteFactory rest (dwrite.h)
} VdFtIDWriteFactoryVtbl;
struct VdFtIDWriteFactory { const VdFtIDWriteFactoryVtbl *lpVtbl; };


VD_FT_DEFINE_GUID(VD_FT_IID_IDWriteFactory1, 0x30572f99, 0xdac6, 0x41db, 0xa1, 0x6e, 0x04, 0x86, 0x30, 0x7e, 0x60, 0x6a);
typedef struct {
    /* IUnknown */
    VdFtHRESULT (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG   (__stdcall *Release)(VdFtIUnknown *This);

    /* IDWriteFactory */
    VdFtHRESULT (__stdcall *GetSystemFontCollection)(VdFtIUnknown** /*IDWriteFontCollection*/ fontCollection, VdFtBOOL checkForUpdates);
    VdFtHRESULT (__stdcall *CreateCustomFontCollection)(VdFtIUnknown* /*IDWriteFontCollectionLoader*/, void const *collectionKey, VdFtUINT32 collectionKeySize, VdFtIUnknown** /*IDWriteFontCollection*/ fontCollection);
    VdFtHRESULT (__stdcall *RegisterFontCollectionLoader)(VdFtIUnknown* /*IDWriteFontCollectionLoader*/ fontCollectionLoader);
    VdFtHRESULT (__stdcall *UnregisterFontCollectionLoader)(VdFtIUnknown* /*IDWriteFontCollectionLoader*/ fontCollectionLoader);
    VdFtHRESULT (__stdcall *CreateFontFileReference)(VdFtWCHAR const* filePath, VdFtFILETIME const* lastWriteTime, VdFtIUnknown /*IDWriteFontFile*/ ** fontFile);
    VdFtHRESULT (__stdcall *CreateCustomFontFileReference)(void const* fontFileReferenceKey, VdFtUINT32 fontFileReferenceKeySize, VdFtIUnknown /* IDWriteFontFileLoader */ * fontFileLoader, VdFtIUnknown /* IDWriteFontFile */ ** fontFile);
    VdFtHRESULT (__stdcall *CreateFontFace)(VdFtDWRITE_FONT_FACE_TYPE fontFaceType, VdFtUINT32 numberOfFiles, VdFtIUnknown /*IDWriteFontFile*/ * const* fontFiles, VdFtUINT32 faceIndex, VdFtDWRITE_FONT_SIMULATIONS fontFaceSimulationFlags, VdFtIUnknown /* IDWriteFontFace */ ** fontFace);
    VdFtHRESULT (__stdcall *CreateRenderingParams)(VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *CreateMonitorRenderingParams)(VdFtHMONITOR monitor, VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *CreateCustomRenderingParams)(VdFtFLOAT gamma, VdFtFLOAT enhancedContrast, VdFtFLOAT clearTypeLevel, VdFtDWRITE_PIXEL_GEOMETRY pixelGeometry, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *RegisterFontFileLoader)(VdFtIUnknown /* IDWriteFontFileLoader */ * fontFileLoader);
    VdFtHRESULT (__stdcall *UnregisterFontFileLoader)(VdFtIUnknown /* IDWriteFontFileLoader */ * fontFileLoader);
    VdFtHRESULT (__stdcall *CreateTextFormat)(VdFtWCHAR const* fontFamilyName, VdFtIUnknown /* IDWriteFontCollection */ * fontCollection, VdFtDWRITE_FONT_WEIGHT fontWeight, VdFtDWRITE_FONT_STYLE fontStyle, VdFtDWRITE_FONT_STRETCH fontStretch, VdFtFLOAT fontSize, VdFtWCHAR const* localeName, VdFtIUnknown /* IDWriteTextFormat */ ** textFormat);
    VdFtHRESULT (__stdcall *CreateTypography)(VdFtIUnknown /* IDWriteTypography */ ** typography);
    VdFtHRESULT (__stdcall *GetGdiInterop)(VdFtIUnknown /* IDWriteGdiInterop */ ** gdiInterop);
    VdFtHRESULT (__stdcall *CreateTextLayout)(VdFtWCHAR const* string, VdFtUINT32 stringLength, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtFLOAT maxWidth, VdFtFLOAT maxHeight, VdFtIUnknown /* IDWriteTextLayout */ ** textLayout);
    VdFtHRESULT (__stdcall *CreateGdiCompatibleTextLayout)(VdFtWCHAR const* string, VdFtUINT32 stringLength, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtFLOAT layoutWidth, VdFtFLOAT layoutHeight, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtBOOL useGdiNatural, VdFtIUnknown /* IDWriteTextLayout */ ** textLayout);
    VdFtHRESULT (__stdcall *CreateEllipsisTrimmingSign)(VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtIUnknown /* IDWriteInlineObject */ ** trimmingSign);
    VdFtHRESULT (__stdcall *CreateTextAnalyzer)(VdFtIUnknown /* IDWriteTextAnalyzer */ ** textAnalyzer);
    VdFtHRESULT (__stdcall *CreateNumberSubstitution)(VdFtDWRITE_NUMBER_SUBSTITUTION_METHOD substitutionMethod, VdFtWCHAR const* localeName, VdFtBOOL ignoreUserOverride, VdFtIUnknown /* IDWriteNumberSubstitution */ ** numberSubstitution);
    VdFtHRESULT (__stdcall *CreateGlyphRunAnalysis)(VdFtDWRITE_GLYPH_RUN const* glyphRun, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtDWRITE_MEASURING_MODE measuringMode, VdFtFLOAT baselineOriginX, VdFtFLOAT baselineOriginY, VdFtIUnknown /* IDWriteGlyphRunAnalysis */ ** glyphRunAnalysis);

    /* IDWriteFactory1 */
    VdFtHRESULT (__stdcall *GetEudcFontCollection)(VdFtIUnknown /* IDWriteFontCollection */ ** fontCollection, VdFtBOOL checkForUpdates /*= FALSE*/);
    VdFtHRESULT (__stdcall *CreateCustomRenderingParams_1)(VdFtFLOAT gamma, VdFtFLOAT enhancedContrast, VdFtFLOAT enhancedContrastGrayscale, VdFtFLOAT clearTypeLevel, VdFtDWRITE_PIXEL_GEOMETRY pixelGeometry, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtIUnknown /* IDWriteRenderingParams1 */ ** renderingParams);
} VdFtIDWriteFactory1Vtbl;
struct VdFtIDWriteFactory1 { const VdFtIDWriteFactory1Vtbl *lpVtbl; };

typedef VdFtHRESULT (*VdFt__ProcDWriteCreateFactory)(VdFtDWRITE_FACTORY_TYPE factoryType, VdFtREFIID iid, VdFtIUnknown **factory);
static VdFt__ProcDWriteCreateFactory VdFt__DWriteCreateFactory;

typedef struct {
    int initialized;
    VdFtHDC dc;
} Vd_Ft__Win32InternalData;

Vd_Ft__Win32InternalData Vd_Ft_G = {0};

static void vd_ft__win32_init(void);

VD_FT_API int vd_ft_font_load(VdFtFont *font, void *memory, int size, int index)
{
    (void)index;

    vd_ft__win32_init();

    if (!vd_ft__get_ttf_name_records(memory, size, &font->name, &font->name_len)) {
        printf("FAILED\n");
    } else {
        printf("SUCCESS\n");
    }

    VdFtDWORD num_fonts;
    VdFtHANDLE font_mem_handle = VdFt__AddFontMemResourceEx(memory, size, 0, &num_fonts);

    wchar_t wbuf[512];
    int wrt = vd_ft__utf8_to_wide(font->name, font->name_len, wbuf, 511);
    wbuf[wrt] = 0;

    font->reserved0 = (void*)font_mem_handle;
    return 1;
}

int EnumFontFamExProc(const VdFtLOGFONTW *lpelfe, const VdFtTEXTMETRICW *lpntme, VdFtDWORD FontType, VdFtLPARAM lParam)
{
    (void)lParam;
    (void)FontType;
    (void)lpntme;

    if (lpelfe->lfFaceName[0] == L'@') {
        return 1;
    }

    printf("%S %d C%d I%d\n", lpelfe->lfFaceName, lpelfe->lfWeight, lpelfe->lfCharSet, lpelfe->lfItalic);
    return 1;
}

VD_FT_API VdFtFont* vd_ft_get_system_fonts(int *count)
{
    (void)count;

    vd_ft__win32_init();
    VdFtLOGFONTW lf = {0};
    lf.lfCharSet = 0;
    VdFt__EnumFontFamiliesExW(Vd_Ft_G.dc, &lf, EnumFontFamExProc, 0, 0);
    return 0;
}

static void vd_ft__win32_init(void)
{
    if (Vd_Ft_G.initialized) {
        return;
    }


    {
        VdFtHMODULE mod = LoadLibraryA("Gdi32.dll");
        VdFt__CreateCompatibleDC = (VdFt__ProcCreateCompatibleDC)GetProcAddress(mod, "CreateCompatibleDC");
        VdFt__EnumFontFamiliesExW = (VdFt__ProcEnumFontFamiliesExW)GetProcAddress(mod, "EnumFontFamiliesExW");
        VdFt__AddFontMemResourceEx = (VdFt__ProcAddFontMemResourceEx)GetProcAddress(mod, "AddFontMemResourceEx");
    }

    {
        VdFtHMODULE mod = LoadLibraryA("DWrite.dll");
        VdFt__DWriteCreateFactory = (VdFt__ProcDWriteCreateFactory)GetProcAddress(mod, "DWriteCreateFactory");
    }

    Vd_Ft_G.dc = VdFt__CreateCompatibleDC(NULL);


    Vd_Ft_G.initialized = 1;
}

static void *vd_ft__realloc_mem(void *prev_ptr, size_t size)
{
    if (prev_ptr == 0) {
        return HeapAlloc(GetProcessHeap(), 0, size);
    } else {
        return HeapReAlloc(GetProcessHeap(), 0, prev_ptr, size);
    }
}

static void vd_ft__free_mem(void *memory)
{
    HeapFree(GetProcessHeap(), 0, memory);
}

static int vd_ft__mac_roman_to_wide(uint8_t *strdata, uint16_t string_length, wchar_t *wbuf, int wbuf_len)
{
    return MultiByteToWideChar(10000, 0, (VdFtLPCSTR)strdata, string_length, wbuf, wbuf_len);
}

static int vd_ft__latin1_to_wide(uint8_t *strdata, uint16_t string_length, wchar_t *wbuf, int wbuf_len)
{
    return MultiByteToWideChar(28591, 0, (VdFtLPCSTR)strdata, string_length, wbuf, wbuf_len);
}

static int vd_ft__wide_to_utf8(wchar_t *wbuf, int wlen, char *buf, int blen)
{
    return WideCharToMultiByte(65001, 0, wbuf, wlen, buf, blen, NULL, NULL);
}

static int vd_ft__utf8_to_wide(char *buf, int blen, wchar_t *wbuf, int wlen)
{
    return MultiByteToWideChar(65001, 8, buf, blen, wbuf, wlen);
}

#endif // _WIN32

#endif // VD_FT_IMPL