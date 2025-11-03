#include <stdio.h>
#include <stdlib.h>

#include "vd_fw.h"
#include "vd_ft.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
    vd_fw_init(NULL);

    void *font_memory;
    size_t font_size;
    {
        FILE *f = fopen("C:\\Windows\\Fonts\\Arial.ttf", "rb");
        fseek(f, 0, SEEK_END);
        size_t size = ftell(f);
        fseek(f, 0, SEEK_SET);
        unsigned char *result = malloc(size);
        fread(result, size, 1, f);
        fclose(f);

        font_memory = result;
        font_size = size;
    }

    VdFtFont font;
    vd_ft_font_load(&font, font_memory, (int)font_size, 0);

    while (vd_fw_running()) {
        vd_fw_swap_buffers();
    }
}

#define VD_FT_IMPL
#include "vd_ft.h"

#define VD_FW_IMPL
#include "vd_fw.h"
