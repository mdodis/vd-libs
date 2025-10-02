#define VD_USE_CRT 1
#define VD_MACRO_ABBREVIATIONS 1
#include <stdio.h>
#include <stdlib.h>
#include "vd.h"
#include "vd_docuspec.h"

#define ERR_EXIT(fmt, ...) do { \
        fprintf(stderr, fmt, __VA_ARGS__); \
        exit(-1);                          \
    } while (0)

#define INVOC_EXIT() ERR_EXIT("Invocation: docuspec_html <DOCUSPEC file>\n" \
                                                         "\t-o\tSet output html file.")

static void traverse_section(VdDspcSection *section, int inset);

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        INVOC_EXIT();
    }

    const char *file_to_read = 0;

    Arg arg = arg_new(argc, argv);
    arg_skip_program_name(&arg);

    while (!arg_at_end(&arg)) {

        Str argname;
        Str argvalue_str;

        if (!arg_get_name(&arg, &argname)) {
            VD_ASSERT(arg_get_str(&arg, &argvalue_str));

            file_to_read = argvalue_str.s;

            continue;
        }

        if (str_eq(argname, LIT("o"))) {
            if (!arg_get_str(&arg, &argvalue_str)) {
                ERR_EXIT("Expected argument after -o!");
            }
        }
    }

    FILE *f = fopen(file_to_read, "rb");
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *result = (char*)malloc(size + 1);
    fread(result, size, 1, f);
    fclose(f);

    VdDspcDocument document;
    vd_dspc_document_init(&document, NULL);
    vd_dspc_document_add(&document, result, size, NULL);

    VdDspcTree *tree = vd_dspc_document_first_tree(&document);
    traverse_section(&tree->global, 0);

    return 0;
}

static void traverse_section(VdDspcSection *section, int inset)
{
    if (!section) return;

    for (int i = 0; i < inset; ++i) printf(" ");
    printf("%.*s", (int)section->section_id.l, section->section_id.s);
    printf(" (");
    for (VdDspcTag *tag = vd_dspc_section_first_tag(section); tag; tag = vd_dspc_section_next_tag(section, tag)) {
        printf("%.*s = \"%.*s\"",
               (int)tag->name.l, tag->name.s,
               (int)tag->value.l, tag->value.s);
    }
    printf(") ");
    printf("\n");

    if (!vd_dspc_str_list_is_empty(&section->text_content)) {
        
        for (VdDspcStrNode *node = vd_dspc_str_list_first_node(&section->text_content);
             node;
             node = vd_dspc_str_list_next_node(node)) 
        {
            for (int i = 0; i < inset; ++i) printf(" ");
            printf("%.*s\n", (int)node->len, node->str);
        }
    }

    for (VdDspcSection *child = section->first; child; child = child->next) {
        traverse_section(child, inset + 2);
    }
}

#define VD_DSPC_IMPL
#include "vd_docuspec.h"

#define VD_IMPL
#include "vd.h"
