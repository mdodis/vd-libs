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
        char *guid = 0;
        char *name = 0;
        char *a = 0;
        char *b = 0;
        char *x = 0;
        char *y = 0;
        char *back = 0;
        char *start = 0;
        char *dpup = 0;
        char *dpright = 0;
        char *dpdown = 0;
        char *dpleft = 0;
        char *left_shoulder = 0;
        char *right_shoulder = 0;
        char *left_trigger = 0;
        char *right_trigger = 0;
        char *left_stick = 0;
        char *right_stick = 0;
        char *leftx = 0;
        char *lefty = 0;
        char *rightx = 0;
        char *righty = 0;
        char *platform = 0;
        char *c;

        if (!isdigit(buffer[0])) {
            continue;
        }

        // Parse the string.
        // We expect:
        // - GUID
        // - Name
        // The rest are mappings, so we handle them accordingly.

        // GUID
        guid = buffer;
        c = move_to_comma(buffer);
        *c++ = 0;

        // Name
        name = c;
        c = move_to_comma(c);
        *c++ = 0;

        // Rest
        while (*c && (*c != '\n') && (*c != '\r')) {

            if      (strncmp(c, "a",                1) == 0)              a = c +  1 + 1;
            else if (strncmp(c, "b",                1) == 0)              b = c +  1 + 1;
            else if (strncmp(c, "x",                1) == 0)              x = c +  1 + 1;
            else if (strncmp(c, "y",                1) == 0)              y = c +  1 + 1;
            else if (strncmp(c, "back",             4) == 0)           back = c +  4 + 1;
            else if (strncmp(c, "dpdown",           6) == 0)         dpdown = c +  6 + 1;
            else if (strncmp(c, "dpleft",           6) == 0)         dpleft = c +  6 + 1;
            else if (strncmp(c, "dpright",          7) == 0)        dpright = c +  7 + 1;
            else if (strncmp(c, "dpup",             4) == 0)           dpup = c +  4 + 1;
            else if (strncmp(c, "leftshoulder",    12) == 0)  left_shoulder = c + 12 + 1;
            else if (strncmp(c, "leftstick",        9) == 0)     left_stick = c +  9 + 1;
            else if (strncmp(c, "lefttrigger",     11) == 0)   left_trigger = c + 11 + 1;
            else if (strncmp(c, "leftx",            5) == 0)          leftx = c +  5 + 1;
            else if (strncmp(c, "lefty",            5) == 0)          lefty = c +  5 + 1;
            else if (strncmp(c, "lefty",            5) == 0)          lefty = c +  5 + 1;
            else if (strncmp(c, "rightshoulder",   13) == 0) right_shoulder = c + 13 + 1;
            else if (strncmp(c, "rightstick",      10) == 0)    right_stick = c + 10 + 1;
            else if (strncmp(c, "righttrigger",    12) == 0)  right_trigger = c + 12 + 1;
            else if (strncmp(c, "rightx",           6) == 0)         rightx = c +  6 + 1;
            else if (strncmp(c, "righty",           6) == 0)         righty = c +  6 + 1;
            else if (strncmp(c, "start",            5) == 0)          start = c +  5 + 1;
            else if (strncmp(c, "platform",         8) == 0)       platform = c +  8 + 1;

            c = move_to_comma(c);
            *c++ = 0;

        }

        printf("%s :: %s\n", guid, name);
        printf("%-14s -> %5s\n", "a", a);

    }

    fclose(f);

    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
