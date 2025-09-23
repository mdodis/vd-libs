#include <stdio.h>
#include <stdlib.h>
#include "vd_docuspec.h"

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Invocation: docuspec_html <DOCUSPEC file>\n");
        return -1;
    }

    const char *file_to_read = argv[1];
    FILE *f = fopen(file_to_read, "rb");
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *result = (char*)malloc(size + 1);
    fread(result, size, 1, f);
    fclose(f);

    VdDspcDocument document;
    vd_dspc_document_init(&document, NULL);
    vd_dspc_document_add(&document, result, size);

    return 0;
}

#define VD_DSPC_IMPL
#include "vd_docuspec.h"
