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

typedef enum {
    VD_FT_BITMAP_FORMAT_A8,
} VdFtBitmapFormat;

typedef struct {
    char     *name;
    int      name_len;
    uint16_t ascent;
    uint16_t descent;
    int16_t  line_gap;
} VdFtFontInfo;

typedef struct {
    float ascent;
    float descent;
    float line_gap;
} VdFtFontMetrics;

typedef struct {
    float baseline_origin_y;
} VdFtGlyphMetrics;

typedef struct {
    int source;
    int index;
} VdFtFontId;

typedef struct {
    unsigned char       *memory;
    unsigned int        x;
    unsigned int        y;
    unsigned int        w;
    unsigned int        h;
    unsigned int        pitch;
    unsigned int        stride;
    VdFtBitmapFormat    format;
} VdFtBitmapRegion;

typedef struct {
    uint32_t            text_start;
    uint32_t            text_len;
    uint32_t            glyph_start;
    uint32_t            glyph_count;
    int                 next_run_idx;

    // @note(mdodis): Internal Use
    uint16_t            script;
    int                 shapes;
} VdFtRun;

typedef struct {
    float advance;
    float ascend;
} VdFtGlyphOffset;

typedef struct {
    uint16_t            *glyph_indices;
    VdFtGlyphOffset     *glyph_offsets;
    float               *glyph_advances;
    size_t              num_glyph_indices;

    VdFtRun             *runs;
    size_t              num_runs;
} VdFtAnalysis;

VD_FT_API VdFtFontId        vd_ft_create_font_from_memory(void *memory, int size);
VD_FT_API VdFtFontId*       vd_ft_get_system_fonts(int *count);

VD_FT_API void              vd_ft_font_get_info(VdFtFontId id, VdFtFontInfo *info);
VD_FT_API VdFtFontMetrics   vd_ft_font_get_metrics(VdFtFontId id, float dd_pixel_scale);
VD_FT_API void              vd_ft_font_get_glyph_indices(VdFtFontId id, uint32_t *codepoints, uint32_t num_codepoints, uint16_t *indices);
VD_FT_API void              vd_ft_font_get_glyph_bounds(VdFtFontId id, float dd_pixel_scale, uint16_t glyph_index, int *width, int *height);
VD_FT_API VdFtGlyphMetrics  vd_ft_font_get_glyph_metrics(VdFtFontId id, float dd_pixel_scale, uint16_t glyph_index);
VD_FT_API VdFtAnalysis      vd_ft_font_analyze(VdFtFontId id, float dd_pixel_scale, const char *text, size_t text_len_bytes);
VD_FT_API VdFtAnalysis      vd_ft_font_analyze_utf16(VdFtFontId id, float dd_pixel_scale, const wchar_t *text, size_t text_len_units);
VD_FT_API VdFtBitmapRegion  vd_ft_font_raster(VdFtFontId id, float dd_pixel_scale, uint16_t *indices, size_t num_indices);

#endif // !VD_FT_H

#ifdef VD_FT_IMPL

#ifndef VD_FT_ABORT
#   define VD_FT_ABORT(message) do { assert(0 && message); *(char*)0 = *message; } while(0)
#endif // !VD_FT_ABORT

#ifndef VD_FT_ASSERTIONS
#   define VD_FT_ASSERTIONS 0
#endif // !VD_FT_ASSERTIONS

#if VD_FT_ASSERTIONS
#   define VD_FT_ASSERT(x) do { if (!(x)) { VD_FT_ABORT("Assertion Failed: " #x " At: " __FILE__ ":" #__LINE__ ); } } while (0);
#else
#   define VD_FT_ASSERT(x) (x)
#endif // VD_FT_ASSERTIONS

#define VD_FT__OFFSET_OF(type, element) ((size_t) & (((type*)0)->element))
#define VD_FT__CONTAINER_OF(ptr, type, member) \
    (type*)(((uint8_t*)ptr) - ((size_t)VD_FT__OFFSET_OF(type, member)))

enum {
    VD_FT_SOURCE_MEMORY = 1,
    VD_FT_SOURCE_SYSTEM = 2,
};

#include <stdint.h>
#include <string.h>

#define VD_FT__MIN(x, y) ((x) < (y) ? (x) : (y))
static void*    vd_ft__realloc_mem(void *prev_ptr, size_t size);
static void     vd_ft__free_mem(void *ptr);
static int      vd_ft__mac_roman_to_wide(uint8_t *strdata, uint16_t string_length, wchar_t *wbuf, int wbuf_len);
static int      vd_ft__latin1_to_wide(uint8_t *strdata, uint16_t string_length, wchar_t *wbuf, int wbuf_len);
static int      vd_ft__wide_to_utf8(wchar_t *wbuf, int wlen, char *buf, int blen);
static int      vd_ft__utf8_to_wide(char *buf, int blen, wchar_t *wbuf, int wlen);
static void*    vd_ft__resize_buffer(void *buffer, size_t element_size, int required_capacity, int *cap);
static void*    vd_ft__resize_buffer_u32(void *buffer, size_t element_size, uint32_t required_capacity, uint32_t *cap);
static wchar_t* vd_ft__utf8_to_utf16_with_mapping_table(const char *utf8_str, size_t utf8_byte_len,
                                                        size_t *utf16_len, size_t *utf16_byte_count,
                                                        size_t *mapping_table, size_t *mapping_table_len);

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

        VdFt__TTFNameRecord *name_records = (VdFt__TTFNameRecord*)vd_ft__realloc_mem(NULL, sizeof(VdFt__TTFNameRecord) * naming_table.count_name_records);
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
extern int         MultiByteToWideChar(VdFtUINT CodePage, VdFtDWORD dwFlags, VdFtLPCSTR lpMultiByteStr, int cbMultiByte, VdFtLPWSTR lpWideCharStr, int cchWideChar);
extern int         WideCharToMultiByte(VdFtUINT CodePage, VdFtDWORD dwFlags, VdFtLPCWSTR lpWideCharStr, int cchWideChar, VdFtLPSTR lpMultiByteStr, int cbMultiByte, VdFtLPSTR lpDefaultChar, VdFtBOOL *lpUsedDefaultChar);
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

#define VD_FT_DC_BRUSH 18
#define VD_FT_DC_PEN 19
#define VD_FT_OBJ_PEN             1
#define VD_FT_OBJ_BRUSH           2
#define VD_FT_OBJ_DC              3
#define VD_FT_OBJ_METADC          4
#define VD_FT_OBJ_PAL             5
#define VD_FT_OBJ_FONT            6
#define VD_FT_OBJ_BITMAP          7
#define VD_FT_OBJ_REGION          8
#define VD_FT_OBJ_METAFILE        9
#define VD_FT_OBJ_MEMDC           10
#define VD_FT_OBJ_EXTPEN          11
#define VD_FT_OBJ_ENHMETADC       12
#define VD_FT_OBJ_ENHMETAFILE     13
#define VD_FT_OBJ_COLORSPACE      14

typedef struct VdFttagRECT
{
    VdFtLONG    left;
    VdFtLONG    top;
    VdFtLONG    right;
    VdFtLONG    bottom;
} VdFtRECT, * VdFtPRECT, * VdFtNPRECT, *VdFtLPRECT;

typedef struct VdFttagBITMAPINFOHEADER {
    VdFtDWORD      biSize;
    VdFtLONG       biWidth;
    VdFtLONG       biHeight;
    VdFtWORD       biPlanes;
    VdFtWORD       biBitCount;
    VdFtDWORD      biCompression;
    VdFtDWORD      biSizeImage;
    VdFtLONG       biXPelsPerMeter;
    VdFtLONG       biYPelsPerMeter;
    VdFtDWORD      biClrUsed;
    VdFtDWORD      biClrImportant;
} VdFtBITMAPINFOHEADER, * VdFtLPBITMAPINFOHEADER, * VdFtPBITMAPINFOHEADER;

#pragma pack(pop)
typedef struct VdFttagBITMAP {
  VdFtLONG   bmType;
  VdFtLONG   bmWidth;
  VdFtLONG   bmHeight;
  VdFtLONG   bmWidthBytes;
  VdFtWORD   bmPlanes;
  VdFtWORD   bmBitsPixel;
  VdFtLPVOID bmBits;
} VdFtBITMAP, *VdFtPBITMAP, *VdFtNPBITMAP, *VdFtLPBITMAP;
#pragma pack(push, 1)

typedef struct VdFttagDIBSECTION {
  VdFtBITMAP           dsBm;
  VdFtBITMAPINFOHEADER dsBmih;
  VdFtDWORD            dsBitfields[3];
  VdFtHANDLE           dshSection;
  VdFtDWORD            dsOffset;
} VdFtDIBSECTION, *VdFtLPDIBSECTION, *VdFtPDIBSECTION;

typedef struct VdFttagSIZE {
    VdFtLONG cx;
    VdFtLONG cy;
} VdFtSIZE, *VdFtPSIZE, *VdFtLPSIZE;

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

typedef struct VdFtIDWriteFactory               VdFtIDWriteFactory;
typedef struct VdFtIDWriteFactory1              VdFtIDWriteFactory1;
typedef struct VdFtIDWriteFactory2              VdFtIDWriteFactory2;
typedef struct VdFtIDWriteFontFileLoader        VdFtIDWriteFontFileLoader;
typedef struct VdFtIDWriteFontFileStream        VdFtIDWriteFontFileStream;
typedef struct VdFtIDWriteRenderingParams       VdFtIDWriteRenderingParams;
typedef struct VdFtIDWriteGdiInterop            VdFtIDWriteGdiInterop;
typedef struct VdFtIDWriteBitmapRenderTarget    VdFtIDWriteBitmapRenderTarget;
typedef struct VdFtIDWriteFontFile              VdFtIDWriteFontFile;
typedef struct VdFtIDWriteFontFace              VdFtIDWriteFontFace;
typedef struct VdFtIDWriteGlyphRunAnalysis      VdFtIDWriteGlyphRunAnalysis;
typedef struct VdFtIDWriteTextAnalysisSink      VdFtIDWriteTextAnalysisSink;
typedef struct VdFtIDWriteTextAnalyzer          VdFtIDWriteTextAnalyzer;
typedef struct VdFtIDWriteTextAnalysisSource    VdFtIDWriteTextAnalysisSource;

typedef enum {
    VD_FT_DWRITE_FACTORY_TYPE_SHARED,
    VD_FT_DWRITE_FACTORY_TYPE_ISOLATED
} VdFtDWRITE_FACTORY_TYPE;

typedef enum {
    VD_FT_DWRITE_FONT_FILE_TYPE_UNKNOWN,
    VD_FT_DWRITE_FONT_FILE_TYPE_CFF,
    VD_FT_DWRITE_FONT_FILE_TYPE_TRUETYPE,
    VD_FT_DWRITE_FONT_FILE_TYPE_OPENTYPE_COLLECTION,
    VD_FT_DWRITE_FONT_FILE_TYPE_TYPE1_PFM,
    VD_FT_DWRITE_FONT_FILE_TYPE_TYPE1_PFB,
    VD_FT_DWRITE_FONT_FILE_TYPE_VECTOR,
    VD_FT_DWRITE_FONT_FILE_TYPE_BITMAP,
    VD_FT_DWRITE_FONT_FILE_TYPE_TRUETYPE_COLLECTION = VD_FT_DWRITE_FONT_FILE_TYPE_OPENTYPE_COLLECTION,
} VdFtDWRITE_FONT_FILE_TYPE;

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

typedef enum {
    VD_FT_DWRITE_SCRIPT_SHAPES_DEFAULT = 0,
    VD_FT_DWRITE_SCRIPT_SHAPES_NO_VISUAL = 1
} VdFtDWRITE_SCRIPT_SHAPES;

typedef enum {
    VD_FT_DWRITE_GRID_FIT_MODE_DEFAULT,
    VD_FT_DWRITE_GRID_FIT_MODE_DISABLED,
    VD_FT_DWRITE_GRID_FIT_MODE_ENABLED
} VdFtDWRITE_GRID_FIT_MODE;

typedef enum {
    VD_FT_DWRITE_READING_DIRECTION_LEFT_TO_RIGHT = 0,
    VD_FT_DWRITE_READING_DIRECTION_RIGHT_TO_LEFT = 1,
    VD_FT_DWRITE_READING_DIRECTION_TOP_TO_BOTTOM = 2,
    VD_FT_DWRITE_READING_DIRECTION_BOTTOM_TO_TOP = 3,
} VdFtDWRITE_READING_DIRECTION;

typedef enum {
    VD_FT_DWRITE_TEXT_ANTIALIAS_MODE_CLEARTYPE,
    VD_FT_DWRITE_TEXT_ANTIALIAS_MODE_GRAYSCALE
} VdFtDWRITE_TEXT_ANTIALIAS_MODE;

typedef enum {
    VD_FT_DWRITE_TEXTURE_ALIASED_1x1,
    VD_FT_DWRITE_TEXTURE_CLEARTYPE_3x1
} VdFtDWRITE_TEXTURE_TYPE;

#define VD_FT_DWRITE_MAKE_OPENTYPE_TAG(a,b,c,d) ( \
    ((VdFtUINT32)((VdFtUINT8)(d)) << 24) | \
    ((VdFtUINT32)((VdFtUINT8)(c)) << 16) | \
    ((VdFtUINT32)((VdFtUINT8)(b)) << 8)  | \
     (VdFtUINT32)((VdFtUINT8)(a)))

typedef enum {
    VD_FT_DWRITE_FONT_FEATURE_TAG_ALTERNATIVE_FRACTIONS               = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('a','f','r','c'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_PETITE_CAPITALS_FROM_CAPITALS       = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('c','2','p','c'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_SMALL_CAPITALS_FROM_CAPITALS        = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('c','2','s','c'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_ALTERNATES               = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('c','a','l','t'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_CASE_SENSITIVE_FORMS                = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('c','a','s','e'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_GLYPH_COMPOSITION_DECOMPOSITION     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('c','c','m','p'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_LIGATURES                = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('c','l','i','g'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_CAPITAL_SPACING                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('c','p','s','p'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_CONTEXTUAL_SWASH                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('c','s','w','h'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_CURSIVE_POSITIONING                 = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('c','u','r','s'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_DEFAULT                             = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('d','f','l','t'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_DISCRETIONARY_LIGATURES             = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('d','l','i','g'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_EXPERT_FORMS                        = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('e','x','p','t'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_FRACTIONS                           = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('f','r','a','c'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_FULL_WIDTH                          = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('f','w','i','d'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_HALF_FORMS                          = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('h','a','l','f'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_HALANT_FORMS                        = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('h','a','l','n'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_ALTERNATE_HALF_WIDTH                = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('h','a','l','t'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_HISTORICAL_FORMS                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('h','i','s','t'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_HORIZONTAL_KANA_ALTERNATES          = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('h','k','n','a'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_HISTORICAL_LIGATURES                = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('h','l','i','g'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_HALF_WIDTH                          = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('h','w','i','d'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_HOJO_KANJI_FORMS                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('h','o','j','o'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_JIS04_FORMS                         = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('j','p','0','4'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_JIS78_FORMS                         = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('j','p','7','8'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_JIS83_FORMS                         = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('j','p','8','3'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_JIS90_FORMS                         = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('j','p','9','0'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_KERNING                             = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('k','e','r','n'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STANDARD_LIGATURES                  = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('l','i','g','a'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_LINING_FIGURES                      = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('l','n','u','m'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_LOCALIZED_FORMS                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('l','o','c','l'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_MARK_POSITIONING                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('m','a','r','k'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_MATHEMATICAL_GREEK                  = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('m','g','r','k'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_MARK_TO_MARK_POSITIONING            = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('m','k','m','k'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_ALTERNATE_ANNOTATION_FORMS          = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('n','a','l','t'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_NLC_KANJI_FORMS                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('n','l','c','k'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_OLD_STYLE_FIGURES                   = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('o','n','u','m'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_ORDINALS                            = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('o','r','d','n'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_PROPORTIONAL_ALTERNATE_WIDTH        = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('p','a','l','t'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_PETITE_CAPITALS                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('p','c','a','p'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_PROPORTIONAL_FIGURES                = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('p','n','u','m'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_PROPORTIONAL_WIDTHS                 = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('p','w','i','d'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_QUARTER_WIDTHS                      = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('q','w','i','d'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_REQUIRED_LIGATURES                  = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('r','l','i','g'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_RUBY_NOTATION_FORMS                 = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('r','u','b','y'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_ALTERNATES                = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','a','l','t'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_SCIENTIFIC_INFERIORS                = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','i','n','f'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_SMALL_CAPITALS                      = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','m','c','p'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_SIMPLIFIED_FORMS                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','m','p','l'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_1                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','0','1'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_2                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','0','2'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_3                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','0','3'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_4                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','0','4'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_5                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','0','5'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_6                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','0','6'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_7                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','0','7'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_8                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','0','8'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_9                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','0','9'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_10                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','1','0'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_11                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','1','1'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_12                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','1','2'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_13                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','1','3'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_14                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','1','4'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_15                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','1','5'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_16                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','1','6'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_17                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','1','7'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_18                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','1','8'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_19                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','1','9'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_STYLISTIC_SET_20                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','s','2','0'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_SUBSCRIPT                           = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','u','b','s'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_SUPERSCRIPT                         = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','u','p','s'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_SWASH                               = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('s','w','s','h'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_TITLING                             = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('t','i','t','l'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_TRADITIONAL_NAME_FORMS              = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('t','n','a','m'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_TABULAR_FIGURES                     = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('t','n','u','m'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_TRADITIONAL_FORMS                   = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('t','r','a','d'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_THIRD_WIDTHS                        = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('t','w','i','d'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_UNICASE                             = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('u','n','i','c'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_VERTICAL_WRITING                    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('v','e','r','t'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_VERTICAL_ALTERNATES_AND_ROTATION    = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('v','r','t','2'),
    VD_FT_DWRITE_FONT_FEATURE_TAG_SLASHED_ZERO                        = VD_FT_DWRITE_MAKE_OPENTYPE_TAG('z','e','r','o'),
} VdFtDWRITE_FONT_FEATURE_TAG;

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
    VdFtIDWriteFontFace *fontFace;
    VdFtFLOAT fontEmSize;
    VdFtUINT32 glyphCount;
    VdFtUINT16 const* glyphIndices;
    VdFtFLOAT const* glyphAdvances;
    VdFtDWRITE_GLYPH_OFFSET const* glyphOffsets;
    VdFtBOOL isSideways;
    VdFtUINT32 bidiLevel;
} VdFtDWRITE_GLYPH_RUN;

typedef struct {
    VdFtWCHAR const* localeName;
    VdFtWCHAR const* string;
    VdFtUINT32 stringLength;
    VdFtUINT16 const* clusterMap;
    VdFtUINT32 textPosition;
} VdFtDWRITE_GLYPH_RUN_DESCRIPTION;

typedef struct {
    VdFtUINT16 designUnitsPerEm;
    VdFtUINT16 ascent;
    VdFtUINT16 descent;
    VdFtINT16 lineGap;
    VdFtUINT16 capHeight;
    VdFtUINT16 xHeight;
    VdFtINT16 underlinePosition;
    VdFtUINT16 underlineThickness;
    VdFtINT16 strikethroughPosition;
    VdFtUINT16 strikethroughThickness;
} VdFtDWRITE_FONT_METRICS;

typedef struct {
    VdFtINT32 leftSideBearing;
    VdFtUINT32 advanceWidth;
    VdFtINT32 rightSideBearing;
    VdFtINT32 topSideBearing;
    VdFtUINT32 advanceHeight;
    VdFtINT32 bottomSideBearing;
    VdFtINT32 verticalOriginY;
} VdFtDWRITE_GLYPH_METRICS;

typedef struct {
    VdFtUINT16 script;
    VdFtDWRITE_SCRIPT_SHAPES shapes;
} VdFtDWRITE_SCRIPT_ANALYSIS;

typedef struct {
    VdFtUINT8 breakConditionBefore  : 2;
    VdFtUINT8 breakConditionAfter   : 2;
    VdFtUINT8 isWhitespace          : 1;
    VdFtUINT8 isSoftHyphen          : 1;
    VdFtUINT8 padding               : 2;
} VdFtDWRITE_LINE_BREAKPOINT;

typedef struct {
    VdFtDWRITE_FONT_FEATURE_TAG nameTag;
    UINT32 parameter;
} VdFtDWRITE_FONT_FEATURE;

typedef struct {
    VdFtDWRITE_FONT_FEATURE* features;
    VdFtUINT32 featureCount;
} VdFtDWRITE_TYPOGRAPHIC_FEATURES;

typedef struct {
    VdFtUINT16 isShapedAlone        : 1;
    VdFtUINT16 reserved1            : 1;
    VdFtUINT16 canBreakShapingAfter : 1;
    VdFtUINT16 reserved             : 13;
} VdFtDWRITE_SHAPING_TEXT_PROPERTIES;

typedef struct {
    VdFtUINT16 justification        : 4;
    VdFtUINT16 isClusterStart       : 1;
    VdFtUINT16 isDiacritic          : 1;
    VdFtUINT16 isZeroWidthSpace     : 1;
    VdFtUINT16 reserved             : 9;
} VdFtDWRITE_SHAPING_GLYPH_PROPERTIES;

typedef int (VdFtFONTENUMPROCW)(const VdFtLOGFONTW *lpelfe, const VdFtTEXTMETRICW *lpntme, VdFtDWORD FontType, VdFtLPARAM lParam);
typedef int (*VdFt__ProcEnumFontFamiliesExW)(VdFtHDC hdc, VdFtLPLOGFONTW lpLogfont, VdFtFONTENUMPROCW lpProc, VdFtLPARAM lParam, VdFtDWORD dwFlags);
typedef VdFtHANDLE (*VdFt__ProcAddFontMemResourceEx)(void *pFileView, VdFtDWORD cjSize, void *pvReserved, VdFtDWORD *pNumFonts);
typedef VdFtHDC (*VdFt__ProcCreateCompatibleDC)(VdFtHDC hdc);
typedef VdFtHGDIOBJ (*VdFt__ProcSelectObject)(VdFtHDC hdc, VdFtHGDIOBJ h);
typedef VdFtHGDIOBJ (*VdFt__ProcGetStockObject)(int i);
typedef VdFtCOLORREF (*VdFt__ProcSetDCPenColor)(VdFtHDC hdc, VdFtCOLORREF color);
typedef VdFtCOLORREF (*VdFt__ProcSetDCBrushColor)(VdFtHDC hdc, VdFtCOLORREF color);
typedef VdFtBOOL (*VdFt__ProcRectangle)(VdFtHDC hdc, int left, int top, int right, int bottom);
typedef VdFtHGDIOBJ (*VdFt__ProcGetCurrentObject)(VdFtHDC hdc, VdFtUINT type);
typedef int (*VdFt__ProcGetObjectW)(VdFtHANDLE h, int c, void *pv);
typedef int (*VdFt__ProcGetObjectA)(VdFtHANDLE h, int c, void *pv);

static VdFt__ProcAddFontMemResourceEx VdFt__AddFontMemResourceEx;
static VdFt__ProcEnumFontFamiliesExW  VdFt__EnumFontFamiliesExW;
static VdFt__ProcCreateCompatibleDC   VdFt__CreateCompatibleDC;
static VdFt__ProcSelectObject         VdFt__SelectObject;
static VdFt__ProcGetStockObject       VdFt__GetStockObject;
static VdFt__ProcSetDCPenColor        VdFt__SetDCPenColor;
static VdFt__ProcSetDCBrushColor      VdFt__SetDCBrushColor;
static VdFt__ProcRectangle            VdFt__Rectangle;
static VdFt__ProcGetCurrentObject     VdFt__GetCurrentObject;
static VdFt__ProcGetObjectW           VdFt__GetObjectW;
static VdFt__ProcGetObjectA           VdFt__GetObjectA;

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
    VdFtHRESULT (__stdcall *GetSystemFontCollection)(VdFtIDWriteFactory *This, VdFtIUnknown** /*IDWriteFontCollection*/ fontCollection, VdFtBOOL checkForUpdates);
    VdFtHRESULT (__stdcall *CreateCustomFontCollection)(VdFtIDWriteFactory *This, VdFtIUnknown* /*IDWriteFontCollectionLoader*/, void const *collectionKey, VdFtUINT32 collectionKeySize, VdFtIUnknown** /*IDWriteFontCollection*/ fontCollection);
    VdFtHRESULT (__stdcall *RegisterFontCollectionLoader)(VdFtIDWriteFactory *This, VdFtIUnknown* /*IDWriteFontCollectionLoader*/ fontCollectionLoader);
    VdFtHRESULT (__stdcall *UnregisterFontCollectionLoader)(VdFtIDWriteFactory *This, VdFtIUnknown* /*IDWriteFontCollectionLoader*/ fontCollectionLoader);
    VdFtHRESULT (__stdcall *CreateFontFileReference)(VdFtIDWriteFactory *This, VdFtWCHAR const* filePath, VdFtFILETIME const* lastWriteTime, VdFtIUnknown /*IDWriteFontFile*/ ** fontFile);
    VdFtHRESULT (__stdcall *CreateCustomFontFileReference)(VdFtIDWriteFactory *This, void const* fontFileReferenceKey, VdFtUINT32 fontFileReferenceKeySize, VdFtIDWriteFontFileLoader *fontFileLoader, VdFtIDWriteFontFile **fontFile);
    VdFtHRESULT (__stdcall *CreateFontFace)(VdFtIDWriteFactory *This, VdFtDWRITE_FONT_FACE_TYPE fontFaceType, VdFtUINT32 numberOfFiles, VdFtIDWriteFontFile * const*fontFiles, VdFtUINT32 faceIndex, VdFtDWRITE_FONT_SIMULATIONS fontFaceSimulationFlags, VdFtIDWriteFontFace **fontFace);
    VdFtHRESULT (__stdcall *CreateRenderingParams)(VdFtIDWriteFactory *This, VdFtIDWriteRenderingParams **renderingParams);
    VdFtHRESULT (__stdcall *CreateMonitorRenderingParams)(VdFtIDWriteFactory *This, VdFtHMONITOR monitor, VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *CreateCustomRenderingParams)(VdFtIDWriteFactory *This, VdFtFLOAT gamma, VdFtFLOAT enhancedContrast, VdFtFLOAT clearTypeLevel, VdFtDWRITE_PIXEL_GEOMETRY pixelGeometry, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *RegisterFontFileLoader)(VdFtIDWriteFactory *This, VdFtIDWriteFontFileLoader *fontFileLoader);
    VdFtHRESULT (__stdcall *UnregisterFontFileLoader)(VdFtIDWriteFactory *This, VdFtIUnknown /* IDWriteFontFileLoader */ * fontFileLoader);
    VdFtHRESULT (__stdcall *CreateTextFormat)(VdFtIDWriteFactory *This, VdFtWCHAR const* fontFamilyName, VdFtIUnknown /* IDWriteFontCollection */ * fontCollection, VdFtDWRITE_FONT_WEIGHT fontWeight, VdFtDWRITE_FONT_STYLE fontStyle, VdFtDWRITE_FONT_STRETCH fontStretch, VdFtFLOAT fontSize, VdFtWCHAR const* localeName, VdFtIUnknown /* IDWriteTextFormat */ ** textFormat);
    VdFtHRESULT (__stdcall *CreateTypography)(VdFtIDWriteFactory *This, VdFtIUnknown /* IDWriteTypography */ ** typography);
    VdFtHRESULT (__stdcall *GetGdiInterop)(VdFtIDWriteFactory *This, VdFtIDWriteGdiInterop **gdiInterop);
    VdFtHRESULT (__stdcall *CreateTextLayout)(VdFtIDWriteFactory *This, VdFtWCHAR const* string, VdFtUINT32 stringLength, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtFLOAT maxWidth, VdFtFLOAT maxHeight, VdFtIUnknown /* IDWriteTextLayout */ ** textLayout);
    VdFtHRESULT (__stdcall *CreateGdiCompatibleTextLayout)(VdFtIDWriteFactory *This, VdFtWCHAR const* string, VdFtUINT32 stringLength, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtFLOAT layoutWidth, VdFtFLOAT layoutHeight, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtBOOL useGdiNatural, VdFtIUnknown /* IDWriteTextLayout */ ** textLayout);
    VdFtHRESULT (__stdcall *CreateEllipsisTrimmingSign)(VdFtIDWriteFactory *This, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtIUnknown /* IDWriteInlineObject */ ** trimmingSign);
    VdFtHRESULT (__stdcall *CreateTextAnalyzer)(VdFtIDWriteFactory *This, VdFtIDWriteTextAnalyzer **textAnalyzer);
    VdFtHRESULT (__stdcall *CreateNumberSubstitution)(VdFtIDWriteFactory *This, VdFtDWRITE_NUMBER_SUBSTITUTION_METHOD substitutionMethod, VdFtWCHAR const* localeName, VdFtBOOL ignoreUserOverride, VdFtIUnknown /* IDWriteNumberSubstitution */ ** numberSubstitution);
    VdFtHRESULT (__stdcall *CreateGlyphRunAnalysis)(VdFtIDWriteFactory *This, VdFtDWRITE_GLYPH_RUN const* glyphRun, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtDWRITE_MEASURING_MODE measuringMode, VdFtFLOAT baselineOriginX, VdFtFLOAT baselineOriginY, VdFtIDWriteGlyphRunAnalysis **glyphRunAnalysis);

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
    VdFtHRESULT (__stdcall *GetSystemFontCollection)(VdFtIDWriteFactory *This, VdFtIUnknown** /*IDWriteFontCollection*/ fontCollection, VdFtBOOL checkForUpdates);
    VdFtHRESULT (__stdcall *CreateCustomFontCollection)(VdFtIDWriteFactory *This, VdFtIUnknown* /*IDWriteFontCollectionLoader*/, void const *collectionKey, VdFtUINT32 collectionKeySize, VdFtIUnknown** /*IDWriteFontCollection*/ fontCollection);
    VdFtHRESULT (__stdcall *RegisterFontCollectionLoader)(VdFtIDWriteFactory *This, VdFtIUnknown* /*IDWriteFontCollectionLoader*/ fontCollectionLoader);
    VdFtHRESULT (__stdcall *UnregisterFontCollectionLoader)(VdFtIDWriteFactory *This, VdFtIUnknown* /*IDWriteFontCollectionLoader*/ fontCollectionLoader);
    VdFtHRESULT (__stdcall *CreateFontFileReference)(VdFtIDWriteFactory *This, VdFtWCHAR const* filePath, VdFtFILETIME const* lastWriteTime, VdFtIUnknown /*IDWriteFontFile*/ ** fontFile);
    VdFtHRESULT (__stdcall *CreateCustomFontFileReference)(VdFtIDWriteFactory *This, void const* fontFileReferenceKey, VdFtUINT32 fontFileReferenceKeySize, VdFtIDWriteFontFileLoader *fontFileLoader, VdFtIDWriteFontFile **fontFile);
    VdFtHRESULT (__stdcall *CreateFontFace)(VdFtIDWriteFactory *This, VdFtDWRITE_FONT_FACE_TYPE fontFaceType, VdFtUINT32 numberOfFiles, VdFtIDWriteFontFile * const*fontFiles, VdFtUINT32 faceIndex, VdFtDWRITE_FONT_SIMULATIONS fontFaceSimulationFlags, VdFtIDWriteFontFace **fontFace);
    VdFtHRESULT (__stdcall *CreateRenderingParams)(VdFtIDWriteFactory *This, VdFtIDWriteRenderingParams **renderingParams);
    VdFtHRESULT (__stdcall *CreateMonitorRenderingParams)(VdFtIDWriteFactory *This, VdFtHMONITOR monitor, VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *CreateCustomRenderingParams)(VdFtIDWriteFactory *This, VdFtFLOAT gamma, VdFtFLOAT enhancedContrast, VdFtFLOAT clearTypeLevel, VdFtDWRITE_PIXEL_GEOMETRY pixelGeometry, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *RegisterFontFileLoader)(VdFtIDWriteFactory *This, VdFtIDWriteFontFileLoader *fontFileLoader);
    VdFtHRESULT (__stdcall *UnregisterFontFileLoader)(VdFtIDWriteFactory *This, VdFtIUnknown /* IDWriteFontFileLoader */ * fontFileLoader);
    VdFtHRESULT (__stdcall *CreateTextFormat)(VdFtIDWriteFactory *This, VdFtWCHAR const* fontFamilyName, VdFtIUnknown /* IDWriteFontCollection */ * fontCollection, VdFtDWRITE_FONT_WEIGHT fontWeight, VdFtDWRITE_FONT_STYLE fontStyle, VdFtDWRITE_FONT_STRETCH fontStretch, VdFtFLOAT fontSize, VdFtWCHAR const* localeName, VdFtIUnknown /* IDWriteTextFormat */ ** textFormat);
    VdFtHRESULT (__stdcall *CreateTypography)(VdFtIDWriteFactory *This, VdFtIUnknown /* IDWriteTypography */ ** typography);
    VdFtHRESULT (__stdcall *GetGdiInterop)(VdFtIDWriteFactory *This, VdFtIDWriteGdiInterop **gdiInterop);
    VdFtHRESULT (__stdcall *CreateTextLayout)(VdFtIDWriteFactory *This, VdFtWCHAR const* string, VdFtUINT32 stringLength, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtFLOAT maxWidth, VdFtFLOAT maxHeight, VdFtIUnknown /* IDWriteTextLayout */ ** textLayout);
    VdFtHRESULT (__stdcall *CreateGdiCompatibleTextLayout)(VdFtIDWriteFactory *This, VdFtWCHAR const* string, VdFtUINT32 stringLength, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtFLOAT layoutWidth, VdFtFLOAT layoutHeight, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtBOOL useGdiNatural, VdFtIUnknown /* IDWriteTextLayout */ ** textLayout);
    VdFtHRESULT (__stdcall *CreateEllipsisTrimmingSign)(VdFtIDWriteFactory *This, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtIUnknown /* IDWriteInlineObject */ ** trimmingSign);
    VdFtHRESULT (__stdcall *CreateTextAnalyzer)(VdFtIDWriteFactory *This, VdFtIDWriteTextAnalyzer **textAnalyzer);
    VdFtHRESULT (__stdcall *CreateNumberSubstitution)(VdFtIDWriteFactory *This, VdFtDWRITE_NUMBER_SUBSTITUTION_METHOD substitutionMethod, VdFtWCHAR const* localeName, VdFtBOOL ignoreUserOverride, VdFtIUnknown /* IDWriteNumberSubstitution */ ** numberSubstitution);
    VdFtHRESULT (__stdcall *CreateGlyphRunAnalysis)(VdFtIDWriteFactory *This, VdFtDWRITE_GLYPH_RUN const* glyphRun, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtDWRITE_MEASURING_MODE measuringMode, VdFtFLOAT baselineOriginX, VdFtFLOAT baselineOriginY, VdFtIDWriteGlyphRunAnalysis **glyphRunAnalysis);

    /* IDWriteFactory1 */
    VdFtHRESULT (__stdcall *GetEudcFontCollection)(VdFtIDWriteFactory1 *This, VdFtIUnknown /* IDWriteFontCollection */ ** fontCollection, VdFtBOOL checkForUpdates /*= FALSE*/);
    VdFtHRESULT (__stdcall *CreateCustomRenderingParams_1)(VdFtIDWriteFactory1 *This, VdFtFLOAT gamma, VdFtFLOAT enhancedContrast, VdFtFLOAT enhancedContrastGrayscale, VdFtFLOAT clearTypeLevel, VdFtDWRITE_PIXEL_GEOMETRY pixelGeometry, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtIUnknown /* IDWriteRenderingParams1 */ ** renderingParams);
} VdFtIDWriteFactory1Vtbl;
struct VdFtIDWriteFactory1 { const VdFtIDWriteFactory1Vtbl *lpVtbl; };

VD_FT_DEFINE_GUID(VD_FT_IID_IDWriteFactory2, 0x0439fc60, 0xca44, 0x4994, 0x8d, 0xee, 0x3a, 0x9a, 0xf7, 0xb7, 0x32, 0xec);
typedef struct {
    /* IUnknown */
    VdFtHRESULT (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG   (__stdcall *Release)(VdFtIUnknown *This);

    /* IDWriteFactory */
    VdFtHRESULT (__stdcall *GetSystemFontCollection)(VdFtIDWriteFactory *This, VdFtIUnknown** /*IDWriteFontCollection*/ fontCollection, VdFtBOOL checkForUpdates);
    VdFtHRESULT (__stdcall *CreateCustomFontCollection)(VdFtIDWriteFactory *This, VdFtIUnknown* /*IDWriteFontCollectionLoader*/, void const *collectionKey, VdFtUINT32 collectionKeySize, VdFtIUnknown** /*IDWriteFontCollection*/ fontCollection);
    VdFtHRESULT (__stdcall *RegisterFontCollectionLoader)(VdFtIDWriteFactory *This, VdFtIUnknown* /*IDWriteFontCollectionLoader*/ fontCollectionLoader);
    VdFtHRESULT (__stdcall *UnregisterFontCollectionLoader)(VdFtIDWriteFactory *This, VdFtIUnknown* /*IDWriteFontCollectionLoader*/ fontCollectionLoader);
    VdFtHRESULT (__stdcall *CreateFontFileReference)(VdFtIDWriteFactory *This, VdFtWCHAR const* filePath, VdFtFILETIME const* lastWriteTime, VdFtIUnknown /*IDWriteFontFile*/ ** fontFile);
    VdFtHRESULT (__stdcall *CreateCustomFontFileReference)(VdFtIDWriteFactory *This, void const* fontFileReferenceKey, VdFtUINT32 fontFileReferenceKeySize, VdFtIDWriteFontFileLoader *fontFileLoader, VdFtIDWriteFontFile **fontFile);
    VdFtHRESULT (__stdcall *CreateFontFace)(VdFtIDWriteFactory *This, VdFtDWRITE_FONT_FACE_TYPE fontFaceType, VdFtUINT32 numberOfFiles, VdFtIDWriteFontFile * const*fontFiles, VdFtUINT32 faceIndex, VdFtDWRITE_FONT_SIMULATIONS fontFaceSimulationFlags, VdFtIDWriteFontFace **fontFace);
    VdFtHRESULT (__stdcall *CreateRenderingParams)(VdFtIDWriteFactory *This, VdFtIDWriteRenderingParams **renderingParams);
    VdFtHRESULT (__stdcall *CreateMonitorRenderingParams)(VdFtIDWriteFactory *This, VdFtHMONITOR monitor, VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *CreateCustomRenderingParams)(VdFtIDWriteFactory *This, VdFtFLOAT gamma, VdFtFLOAT enhancedContrast, VdFtFLOAT clearTypeLevel, VdFtDWRITE_PIXEL_GEOMETRY pixelGeometry, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtIUnknown /* IDWriteRenderingParams */ ** renderingParams);
    VdFtHRESULT (__stdcall *RegisterFontFileLoader)(VdFtIDWriteFactory *This, VdFtIDWriteFontFileLoader *fontFileLoader);
    VdFtHRESULT (__stdcall *UnregisterFontFileLoader)(VdFtIDWriteFactory *This, VdFtIUnknown /* IDWriteFontFileLoader */ * fontFileLoader);
    VdFtHRESULT (__stdcall *CreateTextFormat)(VdFtIDWriteFactory *This, VdFtWCHAR const* fontFamilyName, VdFtIUnknown /* IDWriteFontCollection */ * fontCollection, VdFtDWRITE_FONT_WEIGHT fontWeight, VdFtDWRITE_FONT_STYLE fontStyle, VdFtDWRITE_FONT_STRETCH fontStretch, VdFtFLOAT fontSize, VdFtWCHAR const* localeName, VdFtIUnknown /* IDWriteTextFormat */ ** textFormat);
    VdFtHRESULT (__stdcall *CreateTypography)(VdFtIDWriteFactory *This, VdFtIUnknown /* IDWriteTypography */ ** typography);
    VdFtHRESULT (__stdcall *GetGdiInterop)(VdFtIDWriteFactory *This, VdFtIDWriteGdiInterop **gdiInterop);
    VdFtHRESULT (__stdcall *CreateTextLayout)(VdFtIDWriteFactory *This, VdFtWCHAR const* string, VdFtUINT32 stringLength, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtFLOAT maxWidth, VdFtFLOAT maxHeight, VdFtIUnknown /* IDWriteTextLayout */ ** textLayout);
    VdFtHRESULT (__stdcall *CreateGdiCompatibleTextLayout)(VdFtIDWriteFactory *This, VdFtWCHAR const* string, VdFtUINT32 stringLength, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtFLOAT layoutWidth, VdFtFLOAT layoutHeight, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtBOOL useGdiNatural, VdFtIUnknown /* IDWriteTextLayout */ ** textLayout);
    VdFtHRESULT (__stdcall *CreateEllipsisTrimmingSign)(VdFtIDWriteFactory *This, VdFtIUnknown /* IDWriteTextFormat */ * textFormat, VdFtIUnknown /* IDWriteInlineObject */ ** trimmingSign);
    VdFtHRESULT (__stdcall *CreateTextAnalyzer)(VdFtIDWriteFactory *This, VdFtIDWriteTextAnalyzer **textAnalyzer);
    VdFtHRESULT (__stdcall *CreateNumberSubstitution)(VdFtIDWriteFactory *This, VdFtDWRITE_NUMBER_SUBSTITUTION_METHOD substitutionMethod, VdFtWCHAR const* localeName, VdFtBOOL ignoreUserOverride, VdFtIUnknown /* IDWriteNumberSubstitution */ ** numberSubstitution);
    VdFtHRESULT (__stdcall *CreateGlyphRunAnalysis)(VdFtIDWriteFactory *This, VdFtDWRITE_GLYPH_RUN const* glyphRun, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtDWRITE_MEASURING_MODE measuringMode, VdFtFLOAT baselineOriginX, VdFtFLOAT baselineOriginY, VdFtIDWriteGlyphRunAnalysis **glyphRunAnalysis);

    /* IDWriteFactory1 */
    VdFtHRESULT (__stdcall *GetEudcFontCollection)(VdFtIDWriteFactory1 *This, VdFtIUnknown /* IDWriteFontCollection */ ** fontCollection, VdFtBOOL checkForUpdates /*= FALSE*/);
    VdFtHRESULT (__stdcall *CreateCustomRenderingParams_1)(VdFtIDWriteFactory1 *This, VdFtFLOAT gamma, VdFtFLOAT enhancedContrast, VdFtFLOAT enhancedContrastGrayscale, VdFtFLOAT clearTypeLevel, VdFtDWRITE_PIXEL_GEOMETRY pixelGeometry, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtIUnknown /* IDWriteRenderingParams1 */ ** renderingParams);

    /* IDWriteFactory2 */
    VdFtHRESULT (__stdcall *GetSystemFontFallback)(VdFtIDWriteFactory2 *This, VdFtIUnknown /* IDWriteFontFallback */ ** fontFallback);
    VdFtHRESULT (__stdcall *CreateFontFallbackBuilder)(VdFtIDWriteFactory2 *This, VdFtIUnknown /* IDWriteFontFallbackBuilder */ ** fontFallbackBuilder);
    VdFtHRESULT (__stdcall *TranslateColorGlyphRun)(VdFtIDWriteFactory2 *This, VdFtFLOAT baselineOriginX, VdFtFLOAT baselineOriginY, VdFtDWRITE_GLYPH_RUN const* glyphRun, VdFtDWRITE_GLYPH_RUN_DESCRIPTION const* glyphRunDescription, VdFtDWRITE_MEASURING_MODE measuringMode, VdFtDWRITE_MATRIX const* worldToDeviceTransform, VdFtUINT32 colorPaletteIndex, VdFtIUnknown /* IDWriteColorGlyphRunEnumerator */ ** colorLayers);
    VdFtHRESULT (__stdcall *CreateCustomRenderingParams_2)(VdFtIDWriteFactory2 *This, VdFtFLOAT gamma, VdFtFLOAT enhancedContrast, VdFtFLOAT grayscaleEnhancedContrast, VdFtFLOAT clearTypeLevel, VdFtDWRITE_PIXEL_GEOMETRY pixelGeometry, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtDWRITE_GRID_FIT_MODE gridFitMode, VdFtIUnknown /* IDWriteRenderingParams2 */ ** renderingParams);
    VdFtHRESULT (__stdcall *CreateGlyphRunAnalysis_2)(VdFtIDWriteFactory2 *This, VdFtDWRITE_GLYPH_RUN const* glyphRun, VdFtDWRITE_MATRIX const* transform, VdFtDWRITE_RENDERING_MODE renderingMode, VdFtDWRITE_MEASURING_MODE measuringMode, VdFtDWRITE_GRID_FIT_MODE gridFitMode, VdFtDWRITE_TEXT_ANTIALIAS_MODE antialiasMode, VdFtFLOAT baselineOriginX, VdFtFLOAT baselineOriginY, VdFtIDWriteGlyphRunAnalysis **glyphRunAnalysis);
} VdFtIDWriteFactory2Vtbl;
struct VdFtIDWriteFactory2 { const VdFtIDWriteFactory2Vtbl *lpVtbl; };

VD_FT_DEFINE_GUID(VD_FT_IID_IDWriteFontFileLoader, 0x727cad4e, 0xd6af, 0x4c9e, 0x8a, 0x08, 0xd6, 0x95, 0xb1, 0x1c, 0xaa, 0x49);
typedef struct {
    /* IUnknown */
    VdFtHRESULT (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG   (__stdcall *Release)(VdFtIUnknown *This);

    VdFtHRESULT (__stdcall *CreateStreamFromKey)(VdFtIDWriteFontFileLoader *This, void const* fontFileReferenceKey, VdFtUINT32 fontFileReferenceKeySize, VdFtIDWriteFontFileStream **fontFileStream);
} VdFtIDWriteFontFileLoaderVtbl;
struct VdFtIDWriteFontFileLoader { const VdFtIDWriteFontFileLoaderVtbl *lpVtbl; };

VD_FT_DEFINE_GUID(VD_FT_IID_IDWriteFontFileStream, 0x6d4865fe, 0x0ab8, 0x4d91, 0x8f, 0x62, 0x5d, 0xd6, 0xbe, 0x34, 0xa3, 0xe0);
typedef struct {
    /* IUnknown */
    VdFtHRESULT (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG   (__stdcall *Release)(VdFtIUnknown *This);

    VdFtHRESULT (__stdcall *ReadFileFragment)(VdFtIDWriteFontFileStream *This, void const** fragmentStart, VdFtUINT64 fileOffset, VdFtUINT64 fragmentSize, void** fragmentContext);
    void        (__stdcall *ReleaseFileFragment)(VdFtIDWriteFontFileStream *This, void* fragmentContext);
    VdFtHRESULT (__stdcall *GetFileSize)(VdFtIDWriteFontFileStream *This, VdFtUINT64* fileSize);
    VdFtHRESULT (__stdcall *GetLastWriteTime)(VdFtIDWriteFontFileStream *This, VdFtUINT64* lastWriteTime);
} VdFtIDWriteFontFileStreamVtbl;
struct VdFtIDWriteFontFileStream { const VdFtIDWriteFontFileStreamVtbl *lpVtbl; };

VD_FT_DEFINE_GUID(VD_FT_IID_IDWriteRenderingParams, 0x2f0da53a, 0x2add, 0x47cd, 0x82, 0xee, 0xd9, 0xec, 0x34, 0x68, 0x8e, 0x75);
typedef struct {
    /* IUnknown */
    VdFtHRESULT                 (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG                   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG                   (__stdcall *Release)(VdFtIUnknown *This);

    float                       (__stdcall *GetGamma)(VdFtIDWriteRenderingParams *This);
    float                       (__stdcall *GetEnhancedContrast)(VdFtIDWriteRenderingParams *This);
    float                       (__stdcall *GetClearTypeLevel)(VdFtIDWriteRenderingParams *This);
    VdFtDWRITE_PIXEL_GEOMETRY   (__stdcall *GetPixelGeometry)(VdFtIDWriteRenderingParams *This);
    VdFtDWRITE_RENDERING_MODE   (__stdcall *GetRenderingMode)(VdFtIDWriteRenderingParams *This);

} VdFtIDWriteRenderingParamsVtbl;
struct VdFtIDWriteRenderingParams { const VdFtIDWriteRenderingParamsVtbl *lpVtbl; };

typedef struct {
    /* IUnknown */
    VdFtHRESULT                 (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG                   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG                   (__stdcall *Release)(VdFtIUnknown *This);

    VdFtHRESULT                 (__stdcall *CreateFontFromLOGFONT)(VdFtIDWriteGdiInterop *This, VdFtLOGFONTW const* logFont, VdFtIUnknown /* IDWriteFont */ ** font);
    VdFtHRESULT                 (__stdcall *ConvertFontToLOGFONT)(VdFtIDWriteGdiInterop *This, VdFtIUnknown /* IDWriteFont */ * font, VdFtLOGFONTW* logFont, VdFtBOOL* isSystemFont);
    VdFtHRESULT                 (__stdcall *ConvertFontFaceToLOGFONT)(VdFtIDWriteGdiInterop *This, VdFtIUnknown /* IDWriteFontFace */ * font, VdFtLOGFONTW* logFont);
    VdFtHRESULT                 (__stdcall *CreateFontFaceFromHdc)(VdFtIDWriteGdiInterop *This, VdFtHDC hdc, VdFtIUnknown /* IDWriteFontFace */ ** fontFace);
    VdFtHRESULT                 (__stdcall *CreateBitmapRenderTarget)(VdFtIDWriteGdiInterop *This, VdFtHDC hdc, VdFtUINT32 width, VdFtUINT32 height, VdFtIDWriteBitmapRenderTarget** renderTarget);
} VdFtIDWriteGdiInteropVtbl;
struct VdFtIDWriteGdiInterop { const VdFtIDWriteGdiInteropVtbl *lpVtbl; };

typedef struct {
    /* IUnknown */
    VdFtHRESULT                 (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG                   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG                   (__stdcall *Release)(VdFtIUnknown *This);

    VdFtHRESULT                 (__stdcall *DrawGlyphRun)(VdFtIDWriteBitmapRenderTarget *This, VdFtFLOAT baselineOriginX, VdFtFLOAT baselineOriginY, VdFtDWRITE_MEASURING_MODE measuringMode, VdFtDWRITE_GLYPH_RUN const* glyphRun, VdFtIDWriteRenderingParams* renderingParams, VdFtCOLORREF textColor, VdFtRECT* blackBoxRect /*= NULL*/);
    VdFtHDC                     (__stdcall *GetMemoryDC)(VdFtIDWriteBitmapRenderTarget *This);
    VdFtFLOAT                   (__stdcall *GetPixelsPerDip)(VdFtIDWriteBitmapRenderTarget *This);
    VdFtHRESULT                 (__stdcall *SetPixelsPerDip)(VdFtIDWriteBitmapRenderTarget *This, VdFtFLOAT pixelsPerDip);
    VdFtHRESULT                 (__stdcall *GetCurrentTransform)(VdFtIDWriteBitmapRenderTarget *This, VdFtDWRITE_MATRIX* transform);
    VdFtHRESULT                 (__stdcall *SetCurrentTransform)(VdFtIDWriteBitmapRenderTarget *This, VdFtDWRITE_MATRIX const* transform);
    VdFtHRESULT                 (__stdcall *GetSize)(VdFtIDWriteBitmapRenderTarget *This, VdFtSIZE* size);
    VdFtHRESULT                 (__stdcall *Resize)(VdFtIDWriteBitmapRenderTarget *This, VdFtUINT32 width, VdFtUINT32 height);
} VdFtIDWriteBitmapRenderTargetVtbl;
struct VdFtIDWriteBitmapRenderTarget { const VdFtIDWriteBitmapRenderTargetVtbl *lpVtbl; };

typedef struct {
    /* IUnknown */
    VdFtHRESULT (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG   (__stdcall *Release)(VdFtIUnknown *This);

    VdFtHRESULT (__stdcall *GetReferenceKey)(VdFtIDWriteFontFile *This, void const** fontFileReferenceKey, VdFtUINT32* fontFileReferenceKeySize);
    VdFtHRESULT (__stdcall *GetLoader)(VdFtIDWriteFontFile *This, VdFtIDWriteFontFileLoader** fontFileLoader);
    VdFtHRESULT (__stdcall *Analyze)(VdFtIDWriteFontFile *This, VdFtBOOL* isSupportedFontType, VdFtDWRITE_FONT_FILE_TYPE* fontFileType, VdFtDWRITE_FONT_FACE_TYPE* fontFaceType, VdFtUINT32* numberOfFaces);
} VdFtIDWriteFontFileVtbl;
struct VdFtIDWriteFontFile { const VdFtIDWriteFontFileVtbl *lpVtbl; };

typedef struct {
    /* IUnknown */
    VdFtHRESULT                 (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG                   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG                   (__stdcall *Release)(VdFtIUnknown *This);

    VdFtDWRITE_FONT_FACE_TYPE   (__stdcall *GetType)(VdFtIDWriteFontFace *This);
    VdFtHRESULT                 (__stdcall *GetFiles)(VdFtIDWriteFontFace *This, VdFtUINT32* numberOfFiles, VdFtIDWriteFontFile** fontFiles);
    VdFtUINT32                  (__stdcall *GetIndex)(VdFtIDWriteFontFace *This);
    VdFtDWRITE_FONT_SIMULATIONS (__stdcall *GetSimulations)(VdFtIDWriteFontFace *This);
    VdFtBOOL                    (__stdcall *IsSymbolFont)(VdFtIDWriteFontFace *This);
    void                        (__stdcall *GetMetrics)(VdFtIDWriteFontFace *This, VdFtDWRITE_FONT_METRICS* fontFaceMetrics);
    VdFtUINT16                  (__stdcall *GetGlyphCount)(VdFtIDWriteFontFace *This);
    VdFtHRESULT                 (__stdcall *GetDesignGlyphMetrics)(VdFtIDWriteFontFace *This, VdFtUINT16 const* glyphIndices, VdFtUINT32 glyphCount, VdFtDWRITE_GLYPH_METRICS* glyphMetrics, VdFtBOOL isSideways /*= FALSE*/);
    VdFtHRESULT                 (__stdcall *GetGlyphIndices)(VdFtIDWriteFontFace *This, VdFtUINT32 const* codePoints, VdFtUINT32 codePointCount, VdFtUINT16* glyphIndices);
    VdFtHRESULT                 (__stdcall *TryGetFontTable)(VdFtIDWriteFontFace *This, VdFtUINT32 openTypeTableTag, const void** tableData, VdFtUINT32* tableSize, void** tableContext, VdFtBOOL* exists);
    void                        (__stdcall *ReleaseFontTable)(VdFtIDWriteFontFace *This, void* tableContext);
    VdFtHRESULT                 (__stdcall *GetGlyphRunOutline)(VdFtIDWriteFontFace *This, FLOAT emSize, UINT16 const* glyphIndices, FLOAT const* glyphAdvances, VdFtDWRITE_GLYPH_OFFSET const* glyphOffsets, UINT32 glyphCount, BOOL isSideways, BOOL isRightToLeft, VdFtIUnknown /* IDWriteGeometrySink */ * geometrySink);
    VdFtHRESULT                 (__stdcall *GetRecommendedRenderingMode)(VdFtIDWriteFontFace *This, VdFtFLOAT emSize, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MEASURING_MODE measuringMode, VdFtIDWriteRenderingParams* renderingParams, VdFtDWRITE_RENDERING_MODE* renderingMode);
    VdFtHRESULT                 (__stdcall *GetGdiCompatibleMetrics)(VdFtIDWriteFontFace *This, VdFtFLOAT emSize, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtDWRITE_FONT_METRICS* fontFaceMetrics);
    VdFtHRESULT                 (__stdcall *GetGdiCompatibleGlyphMetrics)(VdFtIDWriteFontFace *This, VdFtFLOAT emSize, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtBOOL useGdiNatural, VdFtUINT16 const* glyphIndices, VdFtUINT32 glyphCount, VdFtDWRITE_GLYPH_METRICS* glyphMetrics, VdFtBOOL isSideways /*= FALSE*/);
} VdFtIDWriteFontFaceVtbl;
struct VdFtIDWriteFontFace { const VdFtIDWriteFontFaceVtbl *lpVtbl; };

typedef struct {
    /* IUnknown */
    VdFtHRESULT                 (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG                   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG                   (__stdcall *Release)(VdFtIUnknown *This);

    VdFtHRESULT                 (__stdcall *GetAlphaTextureBounds)(VdFtIDWriteGlyphRunAnalysis *This, VdFtDWRITE_TEXTURE_TYPE textureType, VdFtRECT* textureBounds);
    VdFtHRESULT                 (__stdcall *CreateAlphaTexture)(VdFtIDWriteGlyphRunAnalysis *This, VdFtDWRITE_TEXTURE_TYPE textureType, VdFtRECT const* textureBounds, VdFtBYTE* alphaValues, VdFtUINT32 bufferSize);
    VdFtHRESULT                 (__stdcall *GetAlphaBlendParams)(VdFtIDWriteGlyphRunAnalysis *This, VdFtIDWriteRenderingParams* renderingParams, VdFtFLOAT* blendGamma, VdFtFLOAT* blendEnhancedContrast, VdFtFLOAT* blendClearTypeLevel);
} VdFtIDWriteGlyphRunAnalysisVtbl;
struct VdFtIDWriteGlyphRunAnalysis { const VdFtIDWriteGlyphRunAnalysisVtbl *lpVtbl; };

typedef struct {
    /* IUnknown */
    VdFtHRESULT                 (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG                   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG                   (__stdcall *Release)(VdFtIUnknown *This);

    VdFtHRESULT                 (__stdcall *SetScriptAnalysis)(VdFtIDWriteTextAnalysisSink *This, VdFtUINT32 textPosition, VdFtUINT32 textLength, VdFtDWRITE_SCRIPT_ANALYSIS const* scriptAnalysis);
    VdFtHRESULT                 (__stdcall *SetLineBreakpoints)(VdFtIDWriteTextAnalysisSink *This, VdFtUINT32 textPosition, VdFtUINT32 textLength, VdFtDWRITE_LINE_BREAKPOINT const* lineBreakpoints);
    VdFtHRESULT                 (__stdcall *SetBidiLevel)(VdFtIDWriteTextAnalysisSink *This, VdFtUINT32 textPosition, VdFtUINT32 textLength, VdFtUINT8 explicitLevel, VdFtUINT8 resolvedLevel);
    VdFtHRESULT                 (__stdcall *SetNumberSubstitution)(VdFtIDWriteTextAnalysisSink *This, VdFtUINT32 textPosition, VdFtUINT32 textLength, VdFtIUnknown /* IDWriteNumberSubstitution */ * numberSubstitution);
} VdFtIDWriteTextAnalysisSinkVtbl;
struct VdFtIDWriteTextAnalysisSink { const VdFtIDWriteTextAnalysisSinkVtbl *lpVtbl; };

typedef struct {
    /* IUnknown */
    VdFtHRESULT                 (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG                   (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG                   (__stdcall *Release)(VdFtIUnknown *This);

    VdFtHRESULT                 (__stdcall *AnalyzeScript)(VdFtIDWriteTextAnalyzer *This, VdFtIDWriteTextAnalysisSource* analysisSource, VdFtUINT32 textPosition, VdFtUINT32 textLength, VdFtIDWriteTextAnalysisSink* analysisSink);
    VdFtHRESULT                 (__stdcall *AnalyzeBidi)(VdFtIDWriteTextAnalyzer *This, VdFtIDWriteTextAnalysisSource* analysisSource, VdFtUINT32 textPosition, VdFtUINT32 textLength, VdFtIDWriteTextAnalysisSink* analysisSink);
    VdFtHRESULT                 (__stdcall *AnalyzeNumberSubstitution)(VdFtIDWriteTextAnalyzer *This, VdFtIDWriteTextAnalysisSource* analysisSource, VdFtUINT32 textPosition, VdFtUINT32 textLength, VdFtIDWriteTextAnalysisSink* analysisSink);
    VdFtHRESULT                 (__stdcall *AnalyzeLineBreakpoints)(VdFtIDWriteTextAnalyzer *This, VdFtIDWriteTextAnalysisSource* analysisSource, VdFtUINT32 textPosition, VdFtUINT32 textLength, VdFtIDWriteTextAnalysisSink* analysisSink);
    VdFtHRESULT                 (__stdcall *GetGlyphs)(VdFtIDWriteTextAnalyzer *This, VdFtWCHAR const* textString, VdFtUINT32 textLength, VdFtIDWriteFontFace* fontFace, VdFtBOOL isSideways, VdFtBOOL isRightToLeft, VdFtDWRITE_SCRIPT_ANALYSIS const* scriptAnalysis, VdFtWCHAR const* localeName, VdFtIUnknown /* IDWriteNumberSubstitution */ * numberSubstitution, VdFtDWRITE_TYPOGRAPHIC_FEATURES const** features, VdFtUINT32 const* featureRangeLengths, VdFtUINT32 featureRanges, VdFtUINT32 maxGlyphCount, VdFtUINT16* clusterMap, VdFtDWRITE_SHAPING_TEXT_PROPERTIES* textProps, VdFtUINT16* glyphIndices, VdFtDWRITE_SHAPING_GLYPH_PROPERTIES* glyphProps, VdFtUINT32* actualGlyphCount);
    VdFtHRESULT                 (__stdcall *GetGlyphPlacements)(VdFtIDWriteTextAnalyzer *This, VdFtWCHAR const* textString, VdFtUINT16 const* clusterMap, VdFtDWRITE_SHAPING_TEXT_PROPERTIES* textProps, VdFtUINT32 textLength, VdFtUINT16 const* glyphIndices, VdFtDWRITE_SHAPING_GLYPH_PROPERTIES const* glyphProps, VdFtUINT32 glyphCount, VdFtIDWriteFontFace* fontFace, VdFtFLOAT fontEmSize, VdFtBOOL isSideways, VdFtBOOL isRightToLeft, VdFtDWRITE_SCRIPT_ANALYSIS const* scriptAnalysis, VdFtWCHAR const* localeName, VdFtDWRITE_TYPOGRAPHIC_FEATURES const** features, VdFtUINT32 const* featureRangeLengths, VdFtUINT32 featureRanges, VdFtFLOAT* glyphAdvances, VdFtDWRITE_GLYPH_OFFSET* glyphOffsets);
    VdFtHRESULT                 (__stdcall *GetGdiCompatibleGlyphPlacements)(VdFtIDWriteTextAnalyzer *This, VdFtWCHAR const* textString, VdFtUINT16 const* clusterMap, VdFtDWRITE_SHAPING_TEXT_PROPERTIES* textProps, VdFtUINT32 textLength, VdFtUINT16 const* glyphIndices, VdFtDWRITE_SHAPING_GLYPH_PROPERTIES const* glyphProps, VdFtUINT32 glyphCount, VdFtIDWriteFontFace * fontFace, VdFtFLOAT fontEmSize, VdFtFLOAT pixelsPerDip, VdFtDWRITE_MATRIX const* transform, VdFtBOOL useGdiNatural, VdFtBOOL isSideways, VdFtBOOL isRightToLeft, VdFtDWRITE_SCRIPT_ANALYSIS const* scriptAnalysis, VdFtWCHAR const* localeName, VdFtDWRITE_TYPOGRAPHIC_FEATURES const** features, VdFtUINT32 const* featureRangeLengths, VdFtUINT32 featureRanges, VdFtFLOAT* glyphAdvances, VdFtDWRITE_GLYPH_OFFSET* glyphOffsets);
} VdFtIDWriteTextAnalyzerVtbl;
struct VdFtIDWriteTextAnalyzer { const VdFtIDWriteTextAnalyzerVtbl *lpVtbl; };

typedef struct {
    /* IUnknown */
    VdFtHRESULT                     (__stdcall *QueryInterface)(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject);
    VdFtULONG                       (__stdcall *AddRef)(VdFtIUnknown *This);
    VdFtULONG                       (__stdcall *Release)(VdFtIUnknown *This);

    VdFtHRESULT                     (__stdcall *GetTextAtPosition)(VdFtIDWriteTextAnalysisSource *This, VdFtUINT32 textPosition, VdFtWCHAR const** textString, VdFtUINT32* textLength);
    VdFtHRESULT                     (__stdcall *GetTextBeforePosition)(VdFtIDWriteTextAnalysisSource *This, VdFtUINT32 textPosition, VdFtWCHAR const** textString, VdFtUINT32* textLength);
    VdFtDWRITE_READING_DIRECTION    (__stdcall *GetParagraphReadingDirection)(VdFtIDWriteTextAnalysisSource *This);
    VdFtHRESULT                     (__stdcall *GetLocaleName)(VdFtIDWriteTextAnalysisSource *This, VdFtUINT32 textPosition, VdFtUINT32* textLength, VdFtWCHAR const** localeName);
    VdFtHRESULT                     (__stdcall *GetNumberSubstitution)(VdFtIDWriteTextAnalysisSource *This, VdFtUINT32 textPosition, VdFtUINT32* textLength, VdFtIUnknown /* IDWriteNumberSubstitution */ ** numberSubstitution);
} VdFtIDWriteTextAnalysisSourceVtbl;
struct VdFtIDWriteTextAnalysisSource { const VdFtIDWriteTextAnalysisSourceVtbl *lpVtbl; };

#pragma pack(pop)

typedef VdFtHRESULT (*VdFt__ProcDWriteCreateFactory)(VdFtDWRITE_FACTORY_TYPE factoryType, VdFtREFIID iid, VdFtIUnknown **factory);
static VdFt__ProcDWriteCreateFactory VdFt__DWriteCreateFactory;

#define VD_FT__WIN32_CHECK_HRESULT(expr) do { VdFtHRESULT _hr_ = (expr); if (_hr_ != 0) { VD_FT_ABORT(#expr " failed"); } } while(0)

typedef struct VdFt__Win32Font VdFt__Win32Font;
struct VdFt__Win32Font  {
    int                                 source;
    VdFtFontInfo                        info;
    VdFtIDWriteFontFace                 *font_face;
    uint16_t                            design_units_per_em;
    union {
        struct {
            void                        *memory;
            size_t                      size;
            VdFtIDWriteFontFileStream   stream;
            VdFtIDWriteFontFile         *font_ref;
        } mem;

        struct {
            VdFtLOGFONTW fontw;
        } sys;
    } data;
};

typedef struct {
    const wchar_t                       *text;
    VdFtUINT32                          text_len_units;

    VdFtIDWriteTextAnalysisSource       source;
    VdFtIDWriteTextAnalysisSourceVtbl   source_vtbl;

    VdFtIDWriteTextAnalysisSink         sink;
    VdFtIDWriteTextAnalysisSinkVtbl     sink_vtbl;

    int                                 current_run_idx;
} VdFt__Win32Analysis;

typedef struct {
    int                                     initialized;
    VdFtHDC                                 dc;
    VdFtIDWriteFactory1                     *factory;
    VdFtIDWriteBitmapRenderTarget           *bitmap_render_target;
    VdFtIDWriteRenderingParams              *rendering_params;

    VdFtIDWriteFontFileLoader               static_font_loader;
    VdFtIDWriteFontFileLoaderVtbl           static_font_loader_vtbl;
    VdFtIDWriteFontFileStreamVtbl           static_font_stream_vtbl;

    VdFtIUnknown                            *number_substitution_contextual;
    VdFt__Win32Analysis                     analysis;

    VdFt__Win32Font                         *font_buffer;
    int                                     font_buffer_len;
    int                                     font_buffer_cap;

    VdFtRun                                 *run_buffer;
    int                                     run_buffer_len;
    int                                     run_buffer_cap;

    VdFtUINT16                              *glyph_indices_buffer;
    VdFtUINT32                              glyph_indices_buffer_len;
    VdFtUINT32                              glyph_indices_buffer_cap;

    VdFtDWRITE_GLYPH_OFFSET                 *glyph_offsets_buffer;
    uint32_t                                glyph_offsets_buffer_len;
    uint32_t                                glyph_offsets_buffer_cap;

    VdFtGlyphOffset                         *user_glyph_offsets_buffer;
    uint32_t                                user_glyph_offsets_buffer_len;
    uint32_t                                user_glyph_offsets_buffer_cap;

    float                                   *glyph_advances_buffer;
    uint32_t                                glyph_advances_buffer_len;
    uint32_t                                glyph_advances_buffer_cap;

    uint16_t                                *glyph_cluster_map_buffer;
    uint32_t                                glyph_cluster_map_buffer_len;
    uint32_t                                glyph_cluster_map_buffer_cap;

    VdFtDWRITE_SHAPING_TEXT_PROPERTIES      *text_props_buffer;
    uint32_t                                text_props_buffer_len;
    uint32_t                                text_props_buffer_cap;

    VdFtDWRITE_SHAPING_GLYPH_PROPERTIES     *glyph_props_buffer;
    uint32_t                                glyph_props_buffer_len;
    uint32_t                                glyph_props_buffer_cap;
} Vd_Ft__Win32InternalData;

Vd_Ft__Win32InternalData Vd_Ft_G = {0};

static void                         vd_ft__win32_init(void);
static VdFtHRESULT                  vd_ft__win32_query_interface_none(VdFtIUnknown *This, VdFtREFIID riid,
                                                                      void **ppvObject);
static VdFtULONG                    vd_ft__win32_add_ref_none(VdFtIUnknown *This);
static VdFtULONG                    vd_ft__win32_release_none(VdFtIUnknown *This);

static VdFtHRESULT                  vd_ft__win32_static_query_interface(VdFtIUnknown *This, VdFtREFIID riid,
                                                                        void **ppvObject);
static VdFtHRESULT                  vd_ft__win32_static_create_stream_from_key(VdFtIDWriteFontFileLoader *This,
                                                                               void const* fontFileReferenceKey,
                                                                               VdFtUINT32 fontFileReferenceKeySize,
                                                                               VdFtIDWriteFontFileStream **fontFileStream);

static VdFtHRESULT                  vd_ft__win32_read_file_fragment(VdFtIDWriteFontFileStream *This,
                                                                    void const** fragmentStart,
                                                                    VdFtUINT64 fileOffset, VdFtUINT64 fragmentSize,
                                                                    void** fragmentContext);
static void                         vd_ft__win32_release_file_fragment(VdFtIDWriteFontFileStream *This,
                                                                       void* fragmentContext);
static VdFtHRESULT                  vd_ft__win32_get_file_size(VdFtIDWriteFontFileStream *This, VdFtUINT64* fileSize);
static VdFtHRESULT                  vd_ft__win32_get_last_write_time(VdFtIDWriteFontFileStream *This,
                                                                     VdFtUINT64* lastWriteTime);
static VdFt__Win32Font*             vd_ft__win32_font_from_id(VdFtFontId id);
static VdFtHRESULT                  vd_ft__win32_get_text_at_position(VdFtIDWriteTextAnalysisSource *This,
                                                                      VdFtUINT32 textPosition,
                                                                      VdFtWCHAR const** textString,
                                                                      VdFtUINT32* textLength);
static VdFtHRESULT                  vd_ft__win32_get_text_before_position(VdFtIDWriteTextAnalysisSource *This,
                                                                          VdFtUINT32 textPosition,
                                                                          VdFtWCHAR const** textString, VdFtUINT32* textLength);
static VdFtDWRITE_READING_DIRECTION vd_ft__win32_get_paragraph_reading_direction(VdFtIDWriteTextAnalysisSource *This);
static VdFtHRESULT                  vd_ft__win32_get_locale_name(VdFtIDWriteTextAnalysisSource *This,
                                                                 VdFtUINT32 textPosition, VdFtUINT32* textLength,
                                                                 VdFtWCHAR const** localeName);
static VdFtHRESULT                  vd_ft__win32_get_number_substitution(VdFtIDWriteTextAnalysisSource *This,
                                                                         VdFtUINT32 textPosition, VdFtUINT32* textLength,
                                                                         VdFtIUnknown /* IDWriteNumberSubstitution */ ** numberSubstitution);

static VdFtHRESULT                  vd_ft__win32_set_script_analysis(VdFtIDWriteTextAnalysisSink *This,
                                                                     VdFtUINT32 textPosition, VdFtUINT32 textLength,
                                                                     VdFtDWRITE_SCRIPT_ANALYSIS const* scriptAnalysis);
static VdFtHRESULT                  vd_ft__win32_set_line_breakpoints(VdFtIDWriteTextAnalysisSink *This,
                                                                      VdFtUINT32 textPosition, VdFtUINT32 textLength,
                                                                      VdFtDWRITE_LINE_BREAKPOINT const* lineBreakpoints);
static VdFtHRESULT                  vd_ft__win32_set_bidi_level(VdFtIDWriteTextAnalysisSink *This,
                                                                VdFtUINT32 textPosition, VdFtUINT32 textLength,
                                                                VdFtUINT8 explicitLevel, VdFtUINT8 resolvedLevel);
static VdFtHRESULT                  vd_ft__win32_set_number_substitution(VdFtIDWriteTextAnalysisSink *This,
                                                                         VdFtUINT32 textPosition, VdFtUINT32 textLength,
                                                                         VdFtIUnknown /* IDWriteNumberSubstitution */ * numberSubstitution);
static void                         vd_ft__win32_analysis_set_current_run(uint32_t text_position);
static void                         vd_ft__win32_analysis_split_current_run(uint32_t split_position);
static VdFtRun*                     vd_ft__win32_analysis_get_next_run(VdFtUINT32 *text_length);
static int                          vd_ft__win32_run_contains_position(VdFtRun *run, uint32_t text_position);
static VdFtUINT32                   vd_ft__win32_estimate_glyph_count(VdFtUINT32 text_len);


VD_FT_API VdFtFontId vd_ft_create_font_from_memory(void *memory, int size)
{
    vd_ft__win32_init();

    char *name;
    int name_len;

    if (!vd_ft__get_ttf_name_records((uint8_t*)memory, size, &name, &name_len)) {
        printf("FAILED\n");
    } else {
        printf("SUCCESS\n");
    }

    // @todo(mdodis): use fixed doubly linked list buffers to keep pointers stable
    Vd_Ft_G.font_buffer = (VdFt__Win32Font*)vd_ft__resize_buffer(Vd_Ft_G.font_buffer, sizeof(*Vd_Ft_G.font_buffer),
                                                                 Vd_Ft_G.font_buffer_len + 1, &Vd_Ft_G.font_buffer_cap);

    int font_index = Vd_Ft_G.font_buffer_len++;
    VdFt__Win32Font *font = &Vd_Ft_G.font_buffer[font_index];
    font->info.name = name;
    font->info.name_len = name_len;
    font->source = VD_FT_SOURCE_MEMORY;
    font->data.mem.memory = memory;
    font->data.mem.size = size;

    VdFtIDWriteFontFile *font_ref = 0;
    VD_FT__WIN32_CHECK_HRESULT(Vd_Ft_G.factory->lpVtbl->CreateCustomFontFileReference((VdFtIDWriteFactory*)Vd_Ft_G.factory,
                                                                                      &font, sizeof(VdFt__Win32Font**),
                                                                                      &Vd_Ft_G.static_font_loader,
                                                                                      &font_ref));

    VdFtIDWriteFontFace *font_face = 0;
    VD_FT__WIN32_CHECK_HRESULT(Vd_Ft_G.factory->lpVtbl->CreateFontFace((VdFtIDWriteFactory*)Vd_Ft_G.factory,
                                                                       VD_FT_DWRITE_FONT_FACE_TYPE_TRUETYPE,
                                                                       1, &font_ref,
                                                                       0, VD_FT_DWRITE_FONT_SIMULATIONS_NONE,
                                                                       &font_face));
    font->data.mem.font_ref = font_ref;
    font->font_face = font_face;

    VdFtDWRITE_FONT_METRICS font_metrics;
    font_face->lpVtbl->GetMetrics(font_face, &font_metrics);

    font->design_units_per_em = font_metrics.designUnitsPerEm;
    font->info.ascent  = font_metrics.ascent;
    font->info.descent = font_metrics.descent;
    font->info.line_gap = font_metrics.lineGap;
  
    VdFtFontId font_id;
    font_id.index = font_index;
    font_id.source = VD_FT_SOURCE_MEMORY;
    return font_id;
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

VD_FT_API VdFtFontId *vd_ft_get_system_fonts(int *count)
{
    (void)count;

    vd_ft__win32_init();
    VdFtLOGFONTW lf = {0};
    lf.lfCharSet = 0;
    VdFt__EnumFontFamiliesExW(Vd_Ft_G.dc, &lf, EnumFontFamExProc, 0, 0);
    return 0;
}

VD_FT_API void vd_ft_font_get_info(VdFtFontId id, VdFtFontInfo *info)
{
    VdFt__Win32Font *font = vd_ft__win32_font_from_id(id);
    *info = font->info;
}

VD_FT_API VdFtFontMetrics vd_ft_font_get_metrics(VdFtFontId id, float dd_pixel_scale)
{
    VdFt__Win32Font *font = vd_ft__win32_font_from_id(id);

    VdFtFontMetrics result = {0};
    result.ascent   = (96.f / 72.f) * ((float)font->info.ascent / (float)font->design_units_per_em) * dd_pixel_scale;
    result.descent  = (96.f / 72.f) * ((float)font->info.descent / (float)font->design_units_per_em) * dd_pixel_scale;
    result.line_gap = (96.f / 72.f) * ((float)font->info.line_gap / (float)font->design_units_per_em) * dd_pixel_scale;

    return result;
}

VD_FT_API void vd_ft_font_get_glyph_indices(VdFtFontId id, uint32_t *codepoints, uint32_t num_codepoints, uint16_t *indices)
{
    VdFt__Win32Font *font = vd_ft__win32_font_from_id(id);
    VD_FT__WIN32_CHECK_HRESULT(font->font_face->lpVtbl->GetGlyphIndices(font->font_face, codepoints, num_codepoints,
                                                                        indices));
}

VD_FT_API void vd_ft_font_get_glyph_bounds(VdFtFontId id, float dd_pixel_scale, uint16_t glyph_index, int *width, int *height)
{
    VdFt__Win32Font *font = vd_ft__win32_font_from_id(id);
    VdFtDWRITE_GLYPH_RUN run = {0};
    run.fontFace = font->font_face;
    run.fontEmSize = (96.f/72.f) * dd_pixel_scale;
    run.glyphCount = 1;
    run.glyphIndices = &glyph_index;
    VdFtIDWriteGlyphRunAnalysis *analysis;
    VD_FT__WIN32_CHECK_HRESULT(Vd_Ft_G.factory->lpVtbl->CreateGlyphRunAnalysis((VdFtIDWriteFactory*)Vd_Ft_G.factory,
                                                                               &run, 1.f, 0,
                                                                               VD_FT_DWRITE_RENDERING_MODE_CLEARTYPE_NATURAL,
                                                                               VD_FT_DWRITE_MEASURING_MODE_NATURAL,
                                                                               0.f, 0.f,
                                                                               &analysis));

    VdFtRECT r;
    VD_FT__WIN32_CHECK_HRESULT(analysis->lpVtbl->GetAlphaTextureBounds(analysis, VD_FT_DWRITE_TEXTURE_CLEARTYPE_3x1, &r));
    *width = r.right - r.left;
    *height = r.bottom - r.top;
}

VD_FT_API VdFtGlyphMetrics vd_ft_font_get_glyph_metrics(VdFtFontId id, float dd_pixel_scale, uint16_t glyph_index)
{
    VdFt__Win32Font *font = vd_ft__win32_font_from_id(id);
    VdFtDWRITE_GLYPH_METRICS metrics;
    font->font_face->lpVtbl->GetDesignGlyphMetrics(font->font_face, &glyph_index, 1, &metrics, 0);

    VdFtGlyphMetrics result = {0};
    result.baseline_origin_y = (96.f / 72.f) * (metrics.verticalOriginY / (float)font->design_units_per_em) * dd_pixel_scale;
    return result;
}

VD_FT_API VdFtAnalysis vd_ft_font_analyze(VdFtFontId id, float dd_pixel_scale, const char *text, size_t text_len_bytes)
{

    VdFtAnalysis result = {0};
    return result;
}

VD_FT_API VdFtAnalysis vd_ft_font_analyze_utf16(VdFtFontId id, float dd_pixel_scale, const wchar_t *text, size_t text_len_units)
{
    VdFt__Win32Font *font = vd_ft__win32_font_from_id(id);

    VdFtIDWriteTextAnalyzer *analyzer;
    VD_FT__WIN32_CHECK_HRESULT(Vd_Ft_G.factory->lpVtbl->CreateTextAnalyzer((VdFtIDWriteFactory*)Vd_Ft_G.factory, &analyzer));

    Vd_Ft_G.run_buffer = (VdFtRun*)vd_ft__resize_buffer((void*)Vd_Ft_G.run_buffer, sizeof(*Vd_Ft_G.run_buffer), 16,
                                                        &Vd_Ft_G.run_buffer_cap);

    VdFtRun *full_run = &Vd_Ft_G.run_buffer[0];
    Vd_Ft_G.run_buffer_len = 1;

    full_run->text_start    = 0;
    full_run->text_len      = (uint32_t)text_len_units;
    full_run->glyph_start   = 0;
    full_run->glyph_count   = 0;
    full_run->next_run_idx  = 0;

    VdFt__Win32Analysis *analysis = &Vd_Ft_G.analysis;
    analysis->text = text;
    analysis->text_len_units = (VdFtUINT32)text_len_units;
    analysis->current_run_idx = 0;

    VD_FT__WIN32_CHECK_HRESULT(analyzer->lpVtbl->AnalyzeLineBreakpoints(analyzer,
                                                                        &analysis->source,
                                                                        0, (VdFtUINT32)text_len_units,
                                                                        &analysis->sink));
    VD_FT__WIN32_CHECK_HRESULT(analyzer->lpVtbl->AnalyzeBidi(analyzer,
                                                             &analysis->source,
                                                             0, (VdFtUINT32)text_len_units,
                                                             &analysis->sink));
    VD_FT__WIN32_CHECK_HRESULT(analyzer->lpVtbl->AnalyzeScript(analyzer,
                                                               &analysis->source,
                                                               0, (VdFtUINT32)text_len_units,
                                                               &analysis->sink));
    VD_FT__WIN32_CHECK_HRESULT(analyzer->lpVtbl->AnalyzeNumberSubstitution(analyzer,
                                                                           &analysis->source,
                                                                           0, (VdFtUINT32)text_len_units,
                                                                           &analysis->sink));

    VdFtUINT32 estimated_glyph_count = vd_ft__win32_estimate_glyph_count((VdFtUINT32)text_len_units);

    Vd_Ft_G.glyph_indices_buffer = vd_ft__resize_buffer_u32(Vd_Ft_G.glyph_indices_buffer, sizeof(*Vd_Ft_G.glyph_indices_buffer),
                                                            estimated_glyph_count,
                                                            &Vd_Ft_G.glyph_indices_buffer_cap);
    Vd_Ft_G.glyph_indices_buffer_len = estimated_glyph_count;

    Vd_Ft_G.glyph_offsets_buffer = vd_ft__resize_buffer_u32(Vd_Ft_G.glyph_offsets_buffer, sizeof(*Vd_Ft_G.glyph_offsets_buffer),
                                                            estimated_glyph_count,
                                                            &Vd_Ft_G.glyph_offsets_buffer_cap);
    Vd_Ft_G.glyph_offsets_buffer_len = estimated_glyph_count;

    Vd_Ft_G.glyph_advances_buffer = vd_ft__resize_buffer_u32(Vd_Ft_G.glyph_advances_buffer, sizeof(*Vd_Ft_G.glyph_advances_buffer),
                                                             estimated_glyph_count,
                                                             &Vd_Ft_G.glyph_advances_buffer_cap);
    Vd_Ft_G.glyph_advances_buffer_len = estimated_glyph_count;

    Vd_Ft_G.glyph_cluster_map_buffer = vd_ft__resize_buffer_u32(Vd_Ft_G.glyph_cluster_map_buffer, sizeof(*Vd_Ft_G.glyph_cluster_map_buffer),
                                                                (uint32_t)text_len_units,
                                                                &Vd_Ft_G.glyph_cluster_map_buffer_cap);
    Vd_Ft_G.glyph_cluster_map_buffer_len = (uint32_t)text_len_units;

    VdFtUINT32 glyph_start = 0;
    for (int i = 0; i < Vd_Ft_G.run_buffer_len; ++i) {
        VdFtRun *run = &Vd_Ft_G.run_buffer[i];
        VdFtUINT32 text_start = run->text_start;
        VdFtUINT32 text_len = run->text_len;
        VdFtUINT32 max_glyph_count = ((VdFtUINT32)(Vd_Ft_G.glyph_indices_buffer_len - glyph_start));
        VdFtUINT32 actual_glyph_count = 0;

        run->glyph_start = glyph_start;
        run->glyph_count = 0;

        if (text_len == 0) {
            continue;
        }


        if (text_len > max_glyph_count) {
            VD_FT_ABORT("TODO");
        }

        Vd_Ft_G.text_props_buffer = vd_ft__resize_buffer_u32(Vd_Ft_G.text_props_buffer, sizeof(*Vd_Ft_G.text_props_buffer),
                                                             text_len,
                                                             &Vd_Ft_G.text_props_buffer_cap);
        Vd_Ft_G.text_props_buffer_len = text_len;

        Vd_Ft_G.glyph_props_buffer = vd_ft__resize_buffer_u32(Vd_Ft_G.glyph_props_buffer, sizeof(*Vd_Ft_G.glyph_props_buffer),
                                                              max_glyph_count,
                                                              &Vd_Ft_G.glyph_props_buffer_cap);
        Vd_Ft_G.glyph_props_buffer_len = max_glyph_count;

        VdFtDWRITE_SCRIPT_ANALYSIS script_analysis;
        script_analysis.script = run->script;
        script_analysis.shapes = run->shapes;

        int success = 1;
        {
            VdFtHRESULT hr = analyzer->lpVtbl->GetGlyphs(analyzer,
                                                         &text[text_start],
                                                         text_len,
                                                         font->font_face,
                                                         0,                  // Sideways?
                                                         0,                  // (Bidi Level % 2)
                                                         &script_analysis,
                                                         L"en-us",           // @todo(mdodis): actual locale
                                                         0,                  // Is Number Substituted?
                                                         NULL,               // Features
                                                         NULL,               // Feature Lengths
                                                         0,                  // Feature Count
                                                         max_glyph_count,
                                                         &Vd_Ft_G.glyph_cluster_map_buffer[text_start],
                                                         Vd_Ft_G.text_props_buffer,
                                                         &Vd_Ft_G.glyph_indices_buffer[glyph_start],
                                                         Vd_Ft_G.glyph_props_buffer,
                                                         &actual_glyph_count);
            if (hr == 122L /* ERROR_INSUFFICIENT_BUFFER */) {
                VD_FT_ABORT("ERROR_INSUFFICIENT_BUFFER");
            }
        }

        if (!success) {
            VdFtAnalysis result = {0};
            return result;
        }

        uint32_t glyph_advances_new_size = (glyph_start + actual_glyph_count) > Vd_Ft_G.glyph_advances_buffer_len
                                         ? (glyph_start + actual_glyph_count)
                                         : Vd_Ft_G.glyph_advances_buffer_len;

        uint32_t glyph_offsets_new_size = (glyph_start + actual_glyph_count) > Vd_Ft_G.glyph_offsets_buffer_len
                                        ? (glyph_start + actual_glyph_count)
                                        : Vd_Ft_G.glyph_offsets_buffer_len;

        Vd_Ft_G.glyph_advances_buffer = vd_ft__resize_buffer_u32(Vd_Ft_G.glyph_advances_buffer, sizeof(*Vd_Ft_G.glyph_advances_buffer),
                                                                 glyph_advances_new_size,
                                                                 &Vd_Ft_G.glyph_advances_buffer_cap);
        Vd_Ft_G.glyph_advances_buffer_len = glyph_advances_new_size;

        Vd_Ft_G.glyph_offsets_buffer = vd_ft__resize_buffer_u32(Vd_Ft_G.glyph_offsets_buffer, sizeof(*Vd_Ft_G.glyph_offsets_buffer),
                                                                glyph_offsets_new_size,
                                                                &Vd_Ft_G.glyph_offsets_buffer_cap);
        Vd_Ft_G.glyph_offsets_buffer_len = glyph_offsets_new_size;

        VD_FT__WIN32_CHECK_HRESULT(analyzer->lpVtbl->GetGlyphPlacements(analyzer,
                                                                        &text[text_start],
                                                                        &Vd_Ft_G.glyph_cluster_map_buffer[text_start],
                                                                        Vd_Ft_G.text_props_buffer,
                                                                        text_len,
                                                                        &Vd_Ft_G.glyph_indices_buffer[glyph_start],
                                                                        Vd_Ft_G.glyph_props_buffer,
                                                                        actual_glyph_count,
                                                                        font->font_face,
                                                                        (96.f / 72.f) * dd_pixel_scale,
                                                                        0,                  // Sideways?
                                                                        0,                  // (Bidi Level % 2)
                                                                        &script_analysis,
                                                                        L"en-us",
                                                                        NULL,               // Features
                                                                        NULL,               // Feature Lengths
                                                                        0,                  // Feature Ranges
                                                                        &Vd_Ft_G.glyph_advances_buffer[glyph_start],
                                                                        &Vd_Ft_G.glyph_offsets_buffer[glyph_start]));

        // @todo(mdodis): If run is only control codes for shaping, zero advances
        run->glyph_count = actual_glyph_count;
        glyph_start += actual_glyph_count;
    }

    // Copy glyph offsets to general format

    Vd_Ft_G.user_glyph_offsets_buffer = vd_ft__resize_buffer_u32(Vd_Ft_G.user_glyph_offsets_buffer, sizeof(*Vd_Ft_G.user_glyph_offsets_buffer),
                                                                 Vd_Ft_G.glyph_offsets_buffer_len,
                                                                 &Vd_Ft_G.user_glyph_offsets_buffer_cap);
    Vd_Ft_G.user_glyph_offsets_buffer_len = Vd_Ft_G.glyph_offsets_buffer_len;
    for (uint32_t i = 0; i < Vd_Ft_G.user_glyph_offsets_buffer_len; ++i) {
        Vd_Ft_G.user_glyph_offsets_buffer[i].advance = Vd_Ft_G.glyph_offsets_buffer[i].advanceOffset;
        Vd_Ft_G.user_glyph_offsets_buffer[i].ascend  = Vd_Ft_G.glyph_offsets_buffer[i].ascenderOffset;
    }


    VdFtAnalysis result = {0};
    result.glyph_indices     = Vd_Ft_G.glyph_indices_buffer;
    result.glyph_advances    = Vd_Ft_G.glyph_advances_buffer;
    result.glyph_offsets     = Vd_Ft_G.user_glyph_offsets_buffer;
    result.num_glyph_indices = Vd_Ft_G.glyph_indices_buffer_len;
    result.runs              = Vd_Ft_G.run_buffer;
    result.num_runs          = Vd_Ft_G.run_buffer_len;
    return result;
}

VD_FT_API VdFtBitmapRegion vd_ft_font_raster(VdFtFontId id, float dd_pixel_scale, uint16_t *indices, size_t num_indices)
{
    VdFt__Win32Font *font = vd_ft__win32_font_from_id(id);

    VdFtDWRITE_GLYPH_RUN run = {0};
    run.fontFace = font->font_face;
    run.fontEmSize = (96.f/72.f) * dd_pixel_scale;
    run.glyphCount = (uint32_t)num_indices;
    run.glyphIndices = indices;
    VdFtCOLORREF fg_color = 0x00FFFFFF;
    VdFtCOLORREF bg_color = 0x000000FF;
    VdFtIDWriteBitmapRenderTarget *render_target = Vd_Ft_G.bitmap_render_target;

    VdFtHDC hdc = render_target->lpVtbl->GetMemoryDC(render_target);
    VdFtHGDIOBJ curr = VdFt__SelectObject(hdc, VdFt__GetStockObject(VD_FT_DC_PEN));
    VdFt__SetDCPenColor(hdc, bg_color);
    VdFt__SelectObject(hdc, VdFt__GetStockObject(VD_FT_DC_BRUSH));
    VdFt__SetDCBrushColor(hdc, bg_color);
    VdFt__Rectangle(hdc, 0, 0, 2048, 2048);
    VdFt__SelectObject(hdc, curr);

    VdFtRECT bbox;
    VD_FT__WIN32_CHECK_HRESULT(render_target->lpVtbl->DrawGlyphRun(render_target,
                                                                   0.f, 100.f,
                                                                   VD_FT_DWRITE_MEASURING_MODE_NATURAL,
                                                                   &run,
                                                                   Vd_Ft_G.rendering_params,
                                                                   fg_color,
                                                                   &bbox));
    VdFtHBITMAP bitmap = (VdFtHBITMAP)VdFt__GetCurrentObject(hdc, VD_FT_OBJ_BITMAP);
    VdFtDIBSECTION dib = {0};
    VdFt__GetObjectW(bitmap, sizeof(dib), &dib);
    VdFtBitmapRegion result;
    result.memory = dib.dsBm.bmBits;
    result.x = bbox.left;
    result.y = bbox.top;
    result.w = bbox.right - bbox.left;
    result.h = bbox.bottom - bbox.top;
    result.pitch = dib.dsBm.bmWidthBytes;
    result.format = VD_FT_BITMAP_FORMAT_A8;
    result.stride = 4;
    return result;
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
        VdFt__SelectObject = (VdFt__ProcSelectObject)GetProcAddress(mod, "SelectObject");
        VdFt__GetStockObject = (VdFt__ProcGetStockObject)GetProcAddress(mod, "GetStockObject");
        VdFt__SetDCPenColor = (VdFt__ProcSetDCPenColor)GetProcAddress(mod, "SetDCPenColor");
        VdFt__SetDCBrushColor = (VdFt__ProcSetDCBrushColor)GetProcAddress(mod, "SetDCBrushColor");
        VdFt__Rectangle = (VdFt__ProcRectangle)GetProcAddress(mod, "Rectangle");
        VdFt__GetCurrentObject = (VdFt__ProcGetCurrentObject)GetProcAddress(mod, "GetCurrentObject");
        VdFt__GetObjectW = (VdFt__ProcGetObjectW)GetProcAddress(mod, "GetObjectW");
        VdFt__GetObjectA = (VdFt__ProcGetObjectA)GetProcAddress(mod, "GetObjectA");

    }

    {
        VdFtHMODULE mod = LoadLibraryA("DWrite.dll");
        VdFt__DWriteCreateFactory = (VdFt__ProcDWriteCreateFactory)GetProcAddress(mod, "DWriteCreateFactory");
    }

    // Vd_Ft_G.dc = VdFt__CreateCompatibleDC(NULL);

    VD_FT__WIN32_CHECK_HRESULT(VdFt__DWriteCreateFactory(VD_FT_DWRITE_FACTORY_TYPE_SHARED, &VD_FT_IID_IDWriteFactory1,
                                                         (VdFtIUnknown**)&Vd_Ft_G.factory));

    Vd_Ft_G.static_font_loader_vtbl.QueryInterface       = vd_ft__win32_query_interface_none;
    Vd_Ft_G.static_font_loader_vtbl.AddRef               = vd_ft__win32_add_ref_none;
    Vd_Ft_G.static_font_loader_vtbl.Release              = vd_ft__win32_release_none;
    Vd_Ft_G.static_font_loader_vtbl.CreateStreamFromKey  = vd_ft__win32_static_create_stream_from_key;
    Vd_Ft_G.static_font_loader.lpVtbl = &Vd_Ft_G.static_font_loader_vtbl;

    Vd_Ft_G.static_font_stream_vtbl.QueryInterface       = vd_ft__win32_query_interface_none;
    Vd_Ft_G.static_font_stream_vtbl.AddRef               = vd_ft__win32_add_ref_none;
    Vd_Ft_G.static_font_stream_vtbl.Release              = vd_ft__win32_release_none;
    Vd_Ft_G.static_font_stream_vtbl.ReadFileFragment     = vd_ft__win32_read_file_fragment;
    Vd_Ft_G.static_font_stream_vtbl.ReleaseFileFragment  = vd_ft__win32_release_file_fragment;
    Vd_Ft_G.static_font_stream_vtbl.GetFileSize          = vd_ft__win32_get_file_size;
    Vd_Ft_G.static_font_stream_vtbl.GetLastWriteTime     = vd_ft__win32_get_last_write_time;

    VD_FT__WIN32_CHECK_HRESULT(Vd_Ft_G.factory->lpVtbl->RegisterFontFileLoader((VdFtIDWriteFactory*)Vd_Ft_G.factory,
                                                                               (VdFtIDWriteFontFileLoader*)&Vd_Ft_G.static_font_loader));

    VdFtIDWriteRenderingParams *rendering_params;
    VD_FT__WIN32_CHECK_HRESULT(Vd_Ft_G.factory->lpVtbl->CreateRenderingParams((VdFtIDWriteFactory*)Vd_Ft_G.factory,
                                                                              &rendering_params));
    Vd_Ft_G.rendering_params = rendering_params;

    VdFtIDWriteGdiInterop *gdi_interop;
    VD_FT__WIN32_CHECK_HRESULT(Vd_Ft_G.factory->lpVtbl->GetGdiInterop((VdFtIDWriteFactory*)Vd_Ft_G.factory,
                                                                      &gdi_interop));

    VdFtIDWriteBitmapRenderTarget *bitmap_render_target;
    VD_FT__WIN32_CHECK_HRESULT(gdi_interop->lpVtbl->CreateBitmapRenderTarget(gdi_interop, 0,
                                                                             2048, 2048, &bitmap_render_target));
    VD_FT__WIN32_CHECK_HRESULT(bitmap_render_target->lpVtbl->SetPixelsPerDip(bitmap_render_target, 1.f));
    Vd_Ft_G.bitmap_render_target = bitmap_render_target;

    VD_FT__WIN32_CHECK_HRESULT(Vd_Ft_G.factory->lpVtbl->CreateNumberSubstitution((VdFtIDWriteFactory*)Vd_Ft_G.factory,
                                                                                 VD_FT_DWRITE_NUMBER_SUBSTITUTION_METHOD_CONTEXTUAL,
                                                                                 L"en-us",
                                                                                 1,
                                                                                 &Vd_Ft_G.number_substitution_contextual));

    VdFt__Win32Analysis *analysis = &Vd_Ft_G.analysis;
    analysis->source_vtbl.QueryInterface               = vd_ft__win32_query_interface_none;
    analysis->source_vtbl.AddRef                       = vd_ft__win32_add_ref_none;
    analysis->source_vtbl.Release                      = vd_ft__win32_release_none;
    analysis->source_vtbl.GetTextAtPosition            = vd_ft__win32_get_text_at_position;
    analysis->source_vtbl.GetTextBeforePosition        = vd_ft__win32_get_text_before_position;
    analysis->source_vtbl.GetParagraphReadingDirection = vd_ft__win32_get_paragraph_reading_direction;
    analysis->source_vtbl.GetLocaleName                = vd_ft__win32_get_locale_name;
    analysis->source_vtbl.GetNumberSubstitution        = vd_ft__win32_get_number_substitution;
    analysis->source.lpVtbl                            = &analysis->source_vtbl;

    analysis->sink_vtbl.QueryInterface = vd_ft__win32_query_interface_none;
    analysis->sink_vtbl.AddRef = vd_ft__win32_add_ref_none;
    analysis->sink_vtbl.Release = vd_ft__win32_release_none;
    analysis->sink_vtbl.SetScriptAnalysis = vd_ft__win32_set_script_analysis;
    analysis->sink_vtbl.SetLineBreakpoints = vd_ft__win32_set_line_breakpoints;
    analysis->sink_vtbl.SetBidiLevel = vd_ft__win32_set_bidi_level;
    analysis->sink_vtbl.SetNumberSubstitution = vd_ft__win32_set_number_substitution;
    analysis->sink.lpVtbl = &analysis->sink_vtbl;

    Vd_Ft_G.initialized = 1;
}

static VdFtHRESULT vd_ft__win32_query_interface_none(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject)
{
    (void)This;
    (void)riid;
    (void)ppvObject;
    return 0x80004002L; // E_NOINTERFACE
    // return 0; // E_NOINTERFACE
}

static VdFtULONG vd_ft__win32_add_ref_none(VdFtIUnknown *This)
{
    (void)This;
    return 1;
}

static VdFtULONG vd_ft__win32_release_none(VdFtIUnknown *This)
{
    (void)This;
    return 1;    
}

static VdFtHRESULT vd_ft__win32_static_query_interface(VdFtIUnknown *This, VdFtREFIID riid, void **ppvObject)
{
    (void)riid;
    *ppvObject = (void*)This; 
    return 0;
}

static VdFtHRESULT vd_ft__win32_static_create_stream_from_key(VdFtIDWriteFontFileLoader *This,
                                                              void const* fontFileReferenceKey,
                                                              VdFtUINT32 fontFileReferenceKeySize,
                                                              VdFtIDWriteFontFileStream **fontFileStream)
{
    (void)This;
    (void)fontFileReferenceKeySize;

    VdFt__Win32Font *font = *(VdFt__Win32Font**)fontFileReferenceKey;
    font->data.mem.stream.lpVtbl = &Vd_Ft_G.static_font_stream_vtbl;
    *fontFileStream = &font->data.mem.stream;

    return 0;
}

static VdFtHRESULT vd_ft__win32_read_file_fragment(VdFtIDWriteFontFileStream *This, void const** fragmentStart,
                                                   VdFtUINT64 fileOffset, VdFtUINT64 fragmentSize,
                                                   void** fragmentContext)
{
    (void)fragmentSize;

    VdFt__Win32Font *font = (VdFt__Win32Font*)VD_FT__CONTAINER_OF(This, VdFt__Win32Font, data.mem.stream);

    *fragmentStart = ((uint8_t*)font->data.mem.memory) + fileOffset;
    *fragmentContext = 0;

    return 0;
}

static void vd_ft__win32_release_file_fragment(VdFtIDWriteFontFileStream *This, void* fragmentContext)
{
    (void)This;
    (void)fragmentContext;
}

static VdFtHRESULT vd_ft__win32_get_file_size(VdFtIDWriteFontFileStream *This, VdFtUINT64* fileSize)
{
    VdFt__Win32Font *font = (VdFt__Win32Font*)VD_FT__CONTAINER_OF(This, VdFt__Win32Font, data.mem.stream);
    *fileSize = font->data.mem.size;
    return 0;
}

static VdFtHRESULT vd_ft__win32_get_last_write_time(VdFtIDWriteFontFileStream *This, VdFtUINT64* lastWriteTime)
{
    (void)This;
    *lastWriteTime = 0;
    return 0;
}

static VdFt__Win32Font *vd_ft__win32_font_from_id(VdFtFontId id)
{
    switch (id.source) {
        case VD_FT_SOURCE_MEMORY: {
            return &Vd_Ft_G.font_buffer[id.index];
        } break;

        default: {
            VD_FT_ABORT("Invalid source!");
            return 0;
        } break;
    }
}

static VdFtHRESULT vd_ft__win32_get_text_at_position(VdFtIDWriteTextAnalysisSource *This,
                                                     VdFtUINT32 textPosition,
                                                     VdFtWCHAR const** textString,
                                                     VdFtUINT32* textLength)
{
    (void)This;
    VdFt__Win32Analysis *analysis = &Vd_Ft_G.analysis;
    if (textPosition >= analysis->text_len_units) {
        *textString = 0;
        *textLength = 0;
    } else {
        *textString = &analysis->text[textPosition];
        *textLength = analysis->text_len_units - textPosition;
    }

    return 0;
}

static VdFtHRESULT vd_ft__win32_get_text_before_position(VdFtIDWriteTextAnalysisSource *This,
                                                         VdFtUINT32 textPosition,
                                                         VdFtWCHAR const** textString, VdFtUINT32* textLength)
{
    (void)This;
    VdFt__Win32Analysis *analysis = &Vd_Ft_G.analysis;
    if ((textPosition == 0) || (textPosition > analysis->text_len_units)) {
        *textString = 0;
        *textLength = 0;
    } else {
        *textString = &analysis->text[0];
        *textLength = textPosition;
    }

    return 0;
}

static VdFtDWRITE_READING_DIRECTION vd_ft__win32_get_paragraph_reading_direction(VdFtIDWriteTextAnalysisSource *This)
{
    (void)This;
    return VD_FT_DWRITE_READING_DIRECTION_LEFT_TO_RIGHT;
}

static VdFtHRESULT vd_ft__win32_get_locale_name(VdFtIDWriteTextAnalysisSource *This,
                                                VdFtUINT32 textPosition, VdFtUINT32* textLength,
                                                VdFtWCHAR const** localeName)
{
    (void)This;
    (void)textPosition;

    *localeName = L"en-us";
    *textLength = 5;
    return 0;
}

static VdFtHRESULT vd_ft__win32_get_number_substitution(VdFtIDWriteTextAnalysisSource *This,
                                                        VdFtUINT32 textPosition, VdFtUINT32* textLength,
                                                        VdFtIUnknown /* IDWriteNumberSubstitution */ ** numberSubstitution)
{
    (void)This;
    VdFt__Win32Analysis *analysis = &Vd_Ft_G.analysis;
    *numberSubstitution = Vd_Ft_G.number_substitution_contextual;
    *textLength = analysis->text_len_units - textPosition;
    return 0;
}

static VdFtHRESULT vd_ft__win32_set_script_analysis(VdFtIDWriteTextAnalysisSink *This,
                                                    VdFtUINT32 textPosition, VdFtUINT32 textLength,
                                                    VdFtDWRITE_SCRIPT_ANALYSIS const* scriptAnalysis)
{
    (void)This;
    (void)scriptAnalysis;

    vd_ft__win32_analysis_set_current_run(textPosition);
    vd_ft__win32_analysis_split_current_run(textPosition);

    while (textLength > 0) {
        VdFtRun *run = vd_ft__win32_analysis_get_next_run(&textLength);
        run->script = scriptAnalysis->script;
        run->shapes = scriptAnalysis->shapes;
    }

    return 0;
}

static VdFtHRESULT vd_ft__win32_set_line_breakpoints(VdFtIDWriteTextAnalysisSink *This,
                                                     VdFtUINT32 textPosition, VdFtUINT32 textLength,
                                                     VdFtDWRITE_LINE_BREAKPOINT const* lineBreakpoints)
{
    (void)This;
    // @todo(mdodis)
    (void)textPosition;
    (void)textLength;
    (void)lineBreakpoints;
    return 0;
}

static VdFtHRESULT vd_ft__win32_set_bidi_level(VdFtIDWriteTextAnalysisSink *This,
                                               VdFtUINT32 textPosition, VdFtUINT32 textLength,
                                               VdFtUINT8 explicitLevel, VdFtUINT8 resolvedLevel)
{
    (void)This;
    (void)explicitLevel;
    (void)resolvedLevel;

    vd_ft__win32_analysis_set_current_run(textPosition);
    vd_ft__win32_analysis_split_current_run(textPosition);
    while (textLength > 0) {
        VdFtRun *run = vd_ft__win32_analysis_get_next_run(&textLength);
        (void)run;
        // @todo(mdodis): set bidi to resolvedLevel
    }

    return 0;
}

static VdFtHRESULT vd_ft__win32_set_number_substitution(VdFtIDWriteTextAnalysisSink *This,
                                                        VdFtUINT32 textPosition, VdFtUINT32 textLength,
                                                        VdFtIUnknown /* IDWriteNumberSubstitution */ * numberSubstitution)
{
    (void)This;
    (void)numberSubstitution;

    vd_ft__win32_analysis_set_current_run(textPosition);
    vd_ft__win32_analysis_split_current_run(textPosition);
    while (textLength > 0) {
        VdFtRun *run = vd_ft__win32_analysis_get_next_run(&textLength);
        (void)run;
        // @todo(mdodis): set substitution flag?
    }

    return 0;
}

static void vd_ft__win32_analysis_set_current_run(uint32_t text_position)
{
    VdFt__Win32Analysis *analysis = &Vd_Ft_G.analysis;

    VdFtRun *current_run = &Vd_Ft_G.run_buffer[analysis->current_run_idx];
    if ((analysis->current_run_idx < Vd_Ft_G.run_buffer_len) &&
        vd_ft__win32_run_contains_position(current_run, text_position))
    {
        return;
    }

    for (int i = 0; i < Vd_Ft_G.run_buffer_len; ++i) {
        if (vd_ft__win32_run_contains_position(&Vd_Ft_G.run_buffer[i], text_position)) {
            analysis->current_run_idx = i;
            break;
        }
    }
}

static void vd_ft__win32_analysis_split_current_run(uint32_t split_position)
{
    VdFt__Win32Analysis *analysis = &Vd_Ft_G.analysis;
    VdFtRun *current_run = &Vd_Ft_G.run_buffer[analysis->current_run_idx];

    uint32_t run_text_start = current_run->text_start;

    if (split_position <= run_text_start) {
        return;
    }

    Vd_Ft_G.run_buffer = (VdFtRun*)vd_ft__resize_buffer((void*)Vd_Ft_G.run_buffer, sizeof(*Vd_Ft_G.run_buffer),
                                                        Vd_Ft_G.run_buffer_len + 1,
                                                        &Vd_Ft_G.run_buffer_cap);
    int new_run_idx = Vd_Ft_G.run_buffer_len++;
    VdFtRun *new_run = &Vd_Ft_G.run_buffer[new_run_idx];
    *new_run = *current_run;

    uint32_t split = split_position - run_text_start;
    new_run->text_start         += split;
    new_run->text_len           -= split;
    current_run->text_len       = split;
    current_run->next_run_idx   = new_run_idx;
    analysis->current_run_idx   = new_run_idx;
}

static VdFtRun *vd_ft__win32_analysis_get_next_run(VdFtUINT32 *text_length)
{
    VdFt__Win32Analysis *analysis = &Vd_Ft_G.analysis;
    VdFtRun *current_run = &Vd_Ft_G.run_buffer[analysis->current_run_idx];
    uint32_t run_text_len = current_run->text_len; 

    if ((*text_length) < run_text_len) {
        run_text_len = *text_length;
        uint32_t run_text_start = Vd_Ft_G.run_buffer[analysis->current_run_idx].text_start;
        vd_ft__win32_analysis_split_current_run(run_text_start + run_text_len);
    } else {
        analysis->current_run_idx = Vd_Ft_G.run_buffer[analysis->current_run_idx].next_run_idx;
    }

    *text_length -= run_text_len;

    return current_run;
}

static int vd_ft__win32_run_contains_position(VdFtRun *run, uint32_t text_position)
{
    if ((run->text_start <= text_position) && (text_position < (run->text_start + run->text_len))) {
        return 1;
    }

    return 0;
}

static VdFtUINT32 vd_ft__win32_estimate_glyph_count(VdFtUINT32 text_len)
{
    return (3 * text_len / 2) + 16;
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

static void *vd_ft__resize_buffer(void *buffer, size_t element_size, int required_capacity, int *cap)
{
    if (required_capacity <= *cap) {
        return buffer;
    }

    int resize_capacity = required_capacity * 2;
    buffer = vd_ft__realloc_mem(buffer, element_size * resize_capacity);
    *cap = resize_capacity;
    return buffer;
}

static void *vd_ft__resize_buffer_u32(void *buffer, size_t element_size, uint32_t required_capacity, uint32_t *cap)
{
    if (required_capacity <= *cap) {
        return buffer;
    }

    uint32_t resize_capacity = required_capacity * 2;
    buffer = vd_ft__realloc_mem(buffer, element_size * resize_capacity);
    *cap = resize_capacity;
    return buffer;
}

#endif // VD_FT_IMPL