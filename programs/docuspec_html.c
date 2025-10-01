#include <stdio.h>
#include <stdlib.h>
#include "vd_docuspec.h"

static void traverse_section(VdDspcSection *section, int inset);

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

    VdDspcTree *tree = vd_dspc_document_first_tree(&document);
    traverse_section(&tree->global, 0);

    return 0;
}

static void traverse_section(VdDspcSection *section, int inset)
{
    if (!section) return;

    for (int i = 0; i < inset; ++i) printf(" ");
    printf("%.*s\n", (int)section->section_id.l, section->section_id.s);

    for (VdDspcSection *child = section->first; child; child = child->next) {
        traverse_section(child, inset + 2);
    }
}

#define VD_DSPC_IMPL
#include "vd_docuspec.h"
