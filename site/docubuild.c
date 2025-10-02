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
static void collect_source_file(File *file, void *userdata);
static void generate_html_for_tree(VdDspcTree *tree, FILE *out);

typedef struct {
    Str file_path_relative_to_source_dir;
} SourceFile;

typedef struct {
    Arena               global_arena;
    Str                 source_dir;
    dynarray SourceFile *source_files;
    VdDspcDocument      document;
} Workspace;

int main(int argc, char const *argv[])
{
    vd_init(NULL);


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

    Workspace workspace = {
        .source_dir = vd_str_from_cstr((char*)directory_to_open),
        .global_arena = vd_arena_from_malloc(MEGABYTES(64)),
    };

    dynarray_init_with_cap(workspace.source_files, &workspace.global_arena, 8);
    vd_directory_walk_recursively(directory_to_open, collect_source_file, &workspace);;


    vd_dspc_document_init(&workspace.document, NULL);

    // Open and parse all source files
    for (usize i = 0; i < dynarray_len(workspace.source_files); ++i) {
        SourceFile *source_file = &workspace.source_files[i];

        Arena *temp_arena = VD_GET_SCRATCH_ARENA();

        StrBuilder bld;
        str_builder_init(&bld, temp_arena);
        str_builder_push_str(&bld, workspace.source_dir);
        str_builder_push_str(&bld, LIT("/"));
        str_builder_push_str(&bld, source_file->file_path_relative_to_source_dir);
        str_builder_null_terminate(&bld);
        Str path_relative_to_cwd = str_builder_compose(&bld, NULL);

        Vdusize len;
        Vdu8 *file = vd_dump_file_to_bytes(&workspace.global_arena, path_relative_to_cwd.s, &len);
        VD_ASSERT(file != 0);

        {
            vd_dspc_document_add(&workspace.document, (char*)file, len);
        }

        VD_RETURN_SCRATCH_ARENA(temp_arena);
    }

    VdDspcTree *tree = vd_dspc_document_first_tree(&workspace.document);
    FILE *f = fopen("./siteout.html", "w");
    generate_html_for_tree(tree, f);
    fclose(f);

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

#define PUT_LINE(fmt, ...) fprintf(out, fmt "\n", __VA_ARGS__)

static Str make_str_from_tag_value(VdDspcTag *tag) {
    Str result = {(char*)tag->value.s, tag->value.l};
    return result;
}

static void generate_html_for_section_recursively(VdDspcSection *section, FILE *out) {

}

static void generate_html_for_tree(VdDspcTree *tree, FILE *out)
{
    PUT_LINE("<!DOCTYPE html>");

    VdDspcSection *global_section = vd_dspc_tree_first_section(tree);
    for (VdDspcSection *child = global_section->first; child; child = child->next) {
        Str section_id = {(char*)child->section_id.s, child->section_id.l};

        if (0) {
        } else if (str_eq(section_id, LIT("title"))) {
            VdDspcTag *tag_title = vd_dspc_section_first_tag(child);
            Str value_str = make_str_from_tag_value(tag_title);

            PUT_LINE("<head>");
            PUT_LINE("<meta charset=\"utf-8\">");
            PUT_LINE("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            PUT_LINE("<title>%.*s</title>", STR_EXPAND(value_str));
            PUT_LINE("<link rel=\"apple-touch-icon\" sizes=\"180x180\" href=\"/apple-touch-icon.png\">");
            PUT_LINE("<link rel=\"icon\" type=\"image/png\" sizes=\"32x32\" href=\"/favicon-32x32.png\">");
            PUT_LINE("<link rel=\"icon\" type=\"image/png\" sizes=\"16x16\" href=\"/favicon-16x16.png\">");
            PUT_LINE("<link rel=\"manifest\" href=\"/site.webmanifest\">");
            PUT_LINE("<meta charset=\"utf-8\">");
            PUT_LINE("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            PUT_LINE("<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB\" crossorigin=\"anonymous\">");
            PUT_LINE("<link rel=\"stylesheet\" href=\"/style.css\">");;
            PUT_LINE("</head>");

        } else if (str_eq(section_id, LIT("category"))) {
            VdDspcTag *tag_category = vd_dspc_section_first_tag(child);
            PUT_LINE("<!-- category: %.*s -->", (int)tag_category->value.l, tag_category->value.s);
        } 
    }
}

static void collect_source_file(File *file, void *userdata)
{
    Str extension = str_chop_left(file->name, str_last_of(file->name, LIT("."), VD_STR_MAX));
    VD_ASSERT(extension.len != 0);
    if (!str_eq(extension, LIT("docuspec"))) {
        return;
    }

    Workspace *workspace = (Workspace*)userdata;
    SourceFile new_source_file = {
        .file_path_relative_to_source_dir = str_dup(&workspace->global_arena, file->name),
    };
    dynarray_add(workspace->source_files, new_source_file);
}

#define VD_DSPC_IMPL
#include "vd_docuspec.h"

#define VD_IMPL
#include "vd.h"
