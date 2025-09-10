/**
 * embed.c - Simple embedding utility
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

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Invocation: <program name> file_to_embed identifier");
        return -1;
    }

    const char *file_to_read = argv[1];
    const char *name = argv[2];

    size_t newline_per_byte = 32;

    FILE *f = fopen(file_to_read, "rb");
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *result = (unsigned char*)malloc(size);
    fread(result, size, 1, f);
    fclose(f);

    printf("static unsigned char %s[%llu] = {\n", name, size);

    for (size_t i = 0; i < size; ++i) {
        printf("0x%02x", result[i]);

        if (i != (size - 1)) {
            printf(",");
        }

        if (((i + 1) % newline_per_byte) == 0) {
            printf("\n");
        }

    }

    printf("\n};\n");

    return 0;
}