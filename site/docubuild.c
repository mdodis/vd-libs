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

#define INVOC_EXIT() ERR_EXIT("Invocation: docubuild <Directory>\n" \
                                                     "\t-o\tSet output directory." \
                                                     "\n")

static void traverse_section(VdDspcSection *section, int inset);

static void on_file(File *file, void *userdata)
{
    (void)userdata;

    printf("File: %.*s\n", (int)file->name.len, file->name.s);
}

int main(int argc, char const *argv[])
{
    if (argc < 2) {
        INVOC_EXIT();
    }

    const char *directory_to_open = 0;

    Arg arg = arg_new(argc, argv);
    arg_skip_program_name(&arg);

    while (!arg_at_end(&arg)) {

        Str argname;
        Str argvalue_str;

        if (!arg_get_name(&arg, &argname)) {
            VD_ASSERT(arg_get_str(&arg, &argvalue_str));

            directory_to_open = argvalue_str.s;

            continue;
        }

        if (str_eq(argname, LIT("o"))) {
            if (!arg_get_str(&arg, &argvalue_str)) {
                ERR_EXIT("Expected argument after -o!");
            }
        }
    }

    vd_directory_walk_recursively(directory_to_open, on_file, 0);;
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
