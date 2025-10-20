/**
 * gamecontrollerdb_gen.c - gamecontrollerdb.txt parser
 * ---------------------------------------------------------------------------------------------------------------------
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org/>
 * ---------------------------------------------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "vd_fw.h"

char *move_to_comma(char *c)
{
    while ((*c != 0) && (*c != ',')) {
        c++;
    }

    if (*c == 0) {
        return 0;
    } else {
        return c;
    }
}

int main(int argc, char const *argv[])
{
    FILE       *f;
    const char *dbfile;
    const char *dbplatform;
    char       buffer[512]; // Max string column I saw in the most recent file is ~400 columns

    if (argc < 3) {
        fprintf(stderr, "Invocation: gamecontrollerdb_gen <gamecontrollerdb.txt> <Platform:(Windows|Mac OS X|Linux|Android|iOS)\n");
        return -1;
    }

    dbfile = argv[1];
    dbplatform = argv[2];

    VdFwPlatform expected_platform = VD_FW_PLATFORM_UNKNOWN;
    if (strcmp(dbplatform, "Windows") == 0) {
        expected_platform = VD_FW_PLATFORM_WINDOWS;
    } else if (strcmp(dbplatform, "Mac OS X")) {
        expected_platform = VD_FW_PLATFORM_MACOS;
    } else if (strcmp(dbplatform, "Linux")) {
        expected_platform = VD_FW_PLATFORM_LINUX;
    } else if (strcmp(dbplatform, "Android")) {
        expected_platform = VD_FW_PLATFORM_ANDROID;
    } else if (strcmp(dbplatform, "iOS")) {
        expected_platform = VD_FW_PLATFORM_IOS;
    }

    f = fopen(dbfile, "r");
    int count = 0;
    while (fgets(buffer, sizeof(buffer), f)) {
        VdFwGamepadDBEntry db_entry;
        VdFwPlatform platform;
        if (!vd_fw_parse_gamepad_db_entry(buffer, (int)strlen(buffer), &db_entry, &platform)) {
            continue;
        }

        if (platform != expected_platform) {
            continue;
        }
        count++;
    }
    fseek(f, 0, SEEK_SET);

    printf("VdFwGamepadDBEntry Gamepad_Db_Entries[%d] = {\n", count);
    while (fgets(buffer, sizeof(buffer), f)) {
        VdFwGamepadDBEntry db_entry;
        VdFwPlatform platform;
        if (!vd_fw_parse_gamepad_db_entry(buffer, (int)strlen(buffer), &db_entry, &platform)) {
            continue;
        }

        if (platform != expected_platform) {
            continue;
        }

        {
            printf("\t{"); 
            for (int j = 0; j < 16; ++j) {
                printf("0x%02x", db_entry.guid.dat[j]);
                if (j < 15) {
                    printf(",");
                }
            }
            printf("},\n"); 
        }

        printf("\t{");
        for (int i = 0; (db_entry.map.mappings[i].kind != VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_NONE) && (i < VD_FW_GAMEPAD_MAX_MAPPINGS); ++i) {

            VdFwGamepadMapEntry *entry = &db_entry.map.mappings[i];


            int is_axis = 0;
            const char *source_kind = "ERROR";
            const char *target = "TARGET_ERROR";

            switch (entry->kind) {
                case VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON: {
                    source_kind = "VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_BUTTON";
                } break;
                case VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_HAT: {
                    source_kind = "VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_HAT";
                } break;
                case VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS: {
                    source_kind = "VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_AXIS";
                    is_axis = 1;
                } break;
                default: {
                    source_kind = "OTHEERRRR";
                } break;
            }

            if (is_axis) {
                switch (entry->target) {
                    case VD_FW_GAMEPAD_LT: target = "VD_FW_GAMEPAD_LT"; break;
                    case VD_FW_GAMEPAD_RT: target = "VD_FW_GAMEPAD_RT"; break;
                    case VD_FW_GAMEPAD_LH: target = "VD_FW_GAMEPAD_LH"; break;
                    case VD_FW_GAMEPAD_LV: target = "VD_FW_GAMEPAD_LV"; break;
                    case VD_FW_GAMEPAD_RH: target = "VD_FW_GAMEPAD_RH"; break;
                    case VD_FW_GAMEPAD_RV: target = "VD_FW_GAMEPAD_RV"; break;
                    default: break;
                }
            } else {
                switch (entry->target) {
                    case VD_FW_GAMEPAD_A:      target = "VD_FW_GAMEPAD_A";      break;
                    case VD_FW_GAMEPAD_B:      target = "VD_FW_GAMEPAD_B";      break;
                    case VD_FW_GAMEPAD_X:      target = "VD_FW_GAMEPAD_X";      break;
                    case VD_FW_GAMEPAD_Y:      target = "VD_FW_GAMEPAD_Y";      break;
                    case VD_FW_GAMEPAD_DUP:    target = "VD_FW_GAMEPAD_DUP";    break;
                    case VD_FW_GAMEPAD_DDOWN:  target = "VD_FW_GAMEPAD_DDOWN";  break;
                    case VD_FW_GAMEPAD_DLEFT:  target = "VD_FW_GAMEPAD_DLEFT";  break;
                    case VD_FW_GAMEPAD_DRIGHT: target = "VD_FW_GAMEPAD_DRIGHT"; break;
                    case VD_FW_GAMEPAD_START:  target = "VD_FW_GAMEPAD_START";  break;
                    case VD_FW_GAMEPAD_SELECT: target = "VD_FW_GAMEPAD_SELECT"; break;
                    case VD_FW_GAMEPAD_L1:     target = "VD_FW_GAMEPAD_L1";     break;
                    case VD_FW_GAMEPAD_R1:     target = "VD_FW_GAMEPAD_R1";     break;
                    case VD_FW_GAMEPAD_L3:     target = "VD_FW_GAMEPAD_L3";     break;
                    case VD_FW_GAMEPAD_R3:     target = "VD_FW_GAMEPAD_R3";     break;
                    default: break;
                }
            }
            printf("{%42s,%30s,%3d},", source_kind, target, entry->index);
        }
        printf("\t},\n");
    }
    printf("}\n");

    fclose(f);

    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
