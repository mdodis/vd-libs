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
    char       buffer[512]; // Max string column I saw in the most recent file is ~400 columns

    if (argc < 3) {
        fprintf(stderr, "Invocation: gamecontrollerdb_gen <gamecontrollerdb.txt> <Platform:(Windows|Mac OS X|Android|iOS)\n");
        return -1;
    }

    dbfile = argv[1];

    f = fopen(dbfile, "r");

    while (fgets(buffer, sizeof(buffer), f)) {
        VdFwGamepadDBEntry db_entry;
        if (!vd_fw_parse_gamepad_db_entry(buffer, (int)strlen(buffer), &db_entry)) {
            continue;
        }

        for (int i = 0; i < 16; ++i) {
            printf("%02x", db_entry.guid[i]);
        }

        for (int i = 0; (db_entry.map.mappings[i].kind != VD_FW_GAMEPAD_MAPPING_SOURCE_KIND_NONE) && (i < VD_FW_GAMEPAD_MAX_MAPPINGS); ++i) {
            VdFwGamepadMapEntry *entry = &db_entry.map.mappings[i];

            switch (entry->target) {
                case VD_FW_GAMEPAD_A:      printf("A:     -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_B:      printf("B:     -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_X:      printf("X:     -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_Y:      printf("Y:     -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_DUP:    printf("DUP    -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_DDOWN:  printf("DDOWN  -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_DLEFT:  printf("DLEFT  -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_DRIGHT: printf("DRIGHT -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_START:  printf("START  -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_SELECT: printf("SELECT -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_L1:     printf("L1     -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_R1:     printf("R1     -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_L3:     printf("L3     -> b%02d\n", entry->index); break;
                case VD_FW_GAMEPAD_R3:     printf("R3     -> b%02d\n", entry->index); break;
                default: break;
            }
        }
        printf("\n");
    }

    fclose(f);

    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
