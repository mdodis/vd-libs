/**
 * sembd.c - Simple embedding utility for source files in macros by Michael Dodis <michaeldodisgr@gmail.com>
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
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Invocation: <program name> file_to_embed identifier <?min_len>");
        return -1;
    }

    const char *file_to_read = argv[1];
    const char *name = argv[2];

    int min_len = 0;

    if (argc >= 4) {
        min_len = atoi(argv[3]);
    }

    int max_line_len = 0;

    FILE *f = fopen(file_to_read, "rb");
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *result = (char*)malloc(size + 1);
    fread(result, size, 1, f);
    fclose(f);

    result[size] = 0;

    // Measure max line
    size_t i = 0;
    int curr_line = 0;
    while (i < size) {

        while ((i < size) && (result[i] != '\n' && result[i] != '\r')) {
            curr_line++;
            i++;
        }

        while ((i < size) && (result[i] == '\n' || result[i] == '\r')) {
            i++;
        }

        if (max_line_len < curr_line) {
            max_line_len = curr_line;
        }
        curr_line = 0;
    }

    if (curr_line > max_line_len) {
        max_line_len = curr_line;
    }

    if (max_line_len < min_len) {
        max_line_len = min_len;
    }

    if (max_line_len > 6) {
        max_line_len -= 6;
    }

    printf("#define %s \\\n", name);

    i = 0;
    while (i < size) {
        printf("\"");

        curr_line = 0;
        while ((i < size) && result[i] != '\n' && result[i] != '\r') {
            printf("%c", result[i]);
            curr_line++;
            i++;
        }

        for (int x = curr_line; x < max_line_len; ++x) {
            printf(" ");
        }

        printf("\\n\" \\\n");


        // Skip over line
        if (result[i] == '\r') i++;
        if (result[i] == '\n') i++;
    }
}
