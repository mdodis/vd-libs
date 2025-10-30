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

#ifndef VD_FT_API
#   ifdef VD_FT_STATIC
#       define VD_FT_API static
#   else
#       define VD_FT_API extern
#   endif // VD_FT_STATIC
#endif // !VD_FT_API

typedef struct {
    void *reserved0;
    char *name;
    int  units_per_em;
    int  ascent, descent, linegap;
} VdFtFont;

VD_FT_API int vd_ft_font_load(VdFtFont *font, void *memory, int size, int index);

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

static int vd_ft__get_ttf_name_records(uint8_t *data, int size, char **out_name)
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
            return 0;
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
        }

        break;
    }
#undef VD_FT__CHECK_1

    return 1;
}

#ifdef _WIN32

#include <Windows.h>

#pragma comment(lib, "Gdi32.lib")

typedef struct {
    int initialized;
    HDC dc;
} Vd_Ft__Win32InternalData;

Vd_Ft__Win32InternalData Vd_Ft_G = {0};

static void vd_ft__win32_init(void);

VD_FT_API int vd_ft_font_load(VdFtFont *font, void *memory, int size, int index)
{
    if (!Vd_Ft_G.initialized) {
        vd_ft__win32_init();
        Vd_Ft_G.initialized = 1;
    }

    if (!vd_ft__get_ttf_name_records(memory, size, &font->name)) {
        printf("FAILED\n");
    } else {
        printf("SUCCESS\n");
    }

    DWORD num_fonts;
    AddFontMemResourceEx(memory, size, 0, &num_fonts);
    return 1;
}

static void vd_ft__win32_init(void)
{
    Vd_Ft_G.dc = CreateCompatibleDC(NULL);
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
    return MultiByteToWideChar(10000, 0, (LPCSTR)strdata, string_length, wbuf, wbuf_len);
}

static int vd_ft__latin1_to_wide(uint8_t *strdata, uint16_t string_length, wchar_t *wbuf, int wbuf_len)
{
    return MultiByteToWideChar(28591, 0, (LPCSTR)strdata, string_length, wbuf, wbuf_len);
}

static int vd_ft__wide_to_utf8(wchar_t *wbuf, int wlen, char *buf, int blen)
{
    return WideCharToMultiByte(CP_UTF8, 0, wbuf, wlen, buf, blen, NULL, NULL);
}

#endif // _WIN32

#endif // VD_FT_IMPL