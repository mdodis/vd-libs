/**
 * fontello.c - C compatible config generator for https://fontello.com/ by Michael Dodis <michaeldodisgr@gmail.com>
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
        fprintf(stderr, "Invocation: <program> <json file> <prefix>");
        return -1;
    }

    const char *file_to_read = argv[1];
    const char *prefix = argv[2];

    FILE *f = fopen(file_to_read, "rb");
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *result = (char*)malloc(size + 1);
    fread(result, size, 1, f);
    fclose(f);
    result[size] = 0;

    size_t i = 0;

    printf("/* Codegen by fontello.c for file listed below\n");
    printf("%s\n", result);
    printf("*/\n");

    printf("enum {\n");

    while (i < size) {

        while ((i < size) && result[i] == ' ' || result[i] == '\t' || result[i] == '\n' || result[i] == '\r') {
            i++;
        }

        const char *css_begin = "\"css\": \"";
        const char *code_begin = "\"code\": ";

        if (strncmp(result + i, css_begin, strlen(css_begin)) == 0) {
            i += strlen(css_begin);

            const char *id_begin = result + i;
            int c = 0;

            while ((i < size) && result[i] != '\"') {
                i++;
                c++;
            }

            printf("    /* %-60.*s */ ", c, id_begin);
            printf("%s", prefix);
            for (int j = 0; j < c; ++j) {
                char idc = id_begin[j];

                if ((idc >= 'a') && (idc <= 'z')) {
                    idc = idc - 'a' + 'A';
                } else if (idc == '-') {
                    idc = '_';
                }

                printf("%c", idc);
            }

            // Skip " and ,
            i += 2;

            while ((i < size) && result[i] == ' ' || result[i] == '\t' || result[i] == '\n' || result[i] == '\r') {
                i++;
            }

            if (i >= size) {
                fprintf(stderr, "Unexpected eof\n");
                return -1;
            }

            if (strncmp(result + i, code_begin, strlen(code_begin)) == 0) {
                i += strlen(code_begin);

                const char *u_begin = result + i;

                int u = 0;
                while ((i < size) && (result[i] != ',')) {
                    i++;
                    u++;
                }

                printf(" = %.*s", u, u_begin);


            } else {
                fprintf(stderr, "Unexpected token\n");
                return -1;
            }

            printf(",");

            printf("\n");

        } else {
            i++;
        }
    }

    printf("};\n");


    return 0;
}