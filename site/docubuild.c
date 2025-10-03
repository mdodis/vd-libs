#define VD_USE_CRT 1
#define VD_MACRO_ABBREVIATIONS 1
#include <stdio.h>
#include <stdlib.h>
#include "vd.h"
#include "vd_docuspec.h"

#define PUT_LINE(fmt, ...) fprintf(out, fmt "\n", __VA_ARGS__)
#define PUT(fmt, ...) fprintf(out, fmt , __VA_ARGS__)

#define ERR_EXIT(fmt, ...) do { \
        fprintf(stderr, fmt, __VA_ARGS__); \
        exit(-1);                          \
    } while (0)

typedef struct {
    Str   id;
    void  (*process)(VdDspcSection*, FILE*, int);
} Processor;

typedef struct {
    Str         file_path_relative_to_source_dir;
    Str         title;
} SourceFile;

typedef struct {
    Arena               global_arena;
    Str                 source_dir;
    dynarray SourceFile *source_files;
    VdDspcDocument      document;
} Workspace;

static Str make_str_from_section_id(VdDspcSection *section);
static Str make_str_from_tag_name(VdDspcTag *tag);
static Str make_str_from_tag_value(VdDspcTag *tag);
static Str make_str_from_str_node(VdDspcStrNode *node);
static Workspace *get_workspace(void);
static void process_child(VdDspcSection *section,   FILE *out, int depth);
static void process_verbatim_html(VdDspcSection *section, FILE *out, int depth);

static void process_copyright(VdDspcSection *section, FILE *out, int depth);
static void process_br(VdDspcSection *section, FILE *out, int depth);
static void process_accordion(VdDspcSection *section, FILE *out, int depth);
static void process_section(VdDspcSection *section, FILE *out, int depth);
static void process_text(VdDspcSection *section, FILE *out, int depth);
static Processor Processor_Table[] = {
    {LIT_INLINE("copyright"),         process_copyright},
    {LIT_INLINE("br"),                process_br},
    {LIT_INLINE("accordion"),         process_accordion},
    {LIT_INLINE("section"),           process_section},
    {LIT_INLINE("text"),              process_text},
    {LIT_INLINE("div"),               process_verbatim_html},
    {LIT_INLINE("img"),               process_verbatim_html},
    {LIT_INLINE("h5"),                process_verbatim_html},
    {LIT_INLINE("p"),                 process_verbatim_html},
    {LIT_INLINE("a"),                 process_verbatim_html},
};















static void process_copyright(VdDspcSection *section, FILE *out, int depth)
{
    PUT_LINE("<div class=\"d-flex\">");
        PUT_LINE("<span class=\"text-muted flex-grow-1 align-self-end\">&copy; Michael Dodis, 2025-2026.</span>");
        PUT_LINE("<span class=\"text-muted flex-grow-0 text-end align-self-end\">This page was created with DOCUSPEC</span>");
    PUT_LINE("</div>");
}


static void process_br(VdDspcSection *section, FILE *out, int depth)
{
    PUT_LINE("<br>");
}

static void process_accordion(VdDspcSection *section, FILE *out, int depth)
{
    VdDspcTag *tag_id = vd_dspc_section_find_tag_with_name(section, "id");
    Str tag_id_value = make_str_from_tag_value(tag_id);
    if (!tag_id) {
        ERR_EXIT("@accordion must have id!");
    }

    PUT_LINE("<div class=\"accordion\" id=\"%.*s\">", STR_EXPAND(tag_id_value));

    int child_count = 0;
    for (VdDspcSection *child = section->first; child; child = child->next) {
        Str child_id_str = make_str_from_section_id(child);
        
        if (!str_eq(child_id_str, LIT("accordion-item"))) {
            ERR_EXIT("Expected accordion-item under accordion directive!");
        }

        VdDspcTag *tag_title = vd_dspc_section_find_tag_with_name(child, NULL);
        Str tag_title_value = make_str_from_tag_value(tag_title);

        PUT_LINE("<div class=\"accordion-item\">");
        PUT_LINE("<p class=\"accordion-header\">");
        PUT_LINE("<button class=\"accordion-button\" type=\"button\""
                 "        data-bs-toggle=\"collapse\" data-bs-target=\"#%.*scollapse%d\""
                 "        aria-expanded=\"true\" aria-controls=\"%.*scollapse%d\">",
                 STR_EXPAND(tag_id_value), child_count,
                 STR_EXPAND(tag_id_value), child_count);
        PUT_LINE("%.*s", STR_EXPAND(tag_title_value));
        PUT_LINE("</button>");
        PUT_LINE("</p>");

        PUT_LINE("<div id=\"%.*scollapse%d\" class=\"accordion-collapse collapse\""
                 "     data-bs-parent=\"#%.*s\">",
                 STR_EXPAND(tag_id_value), child_count,
                 STR_EXPAND(tag_id_value));
        PUT_LINE("<div class=\"accordion-body\">");
        process_child(child->first, out, depth);
        PUT_LINE("</div>");
        PUT_LINE("</div>");

        PUT_LINE("</div>");

        child_count++;

    }

    PUT_LINE("</div>");
}

static void process_text(VdDspcSection *section, FILE *out, int depth)
{
    PUT_LINE("<p>");
    VdDspcStrNode *node = vd_dspc_str_list_first_node(&section->text_content);
    while (node) {

        PUT_LINE("%.*s", STR_EXPAND(make_str_from_str_node(node)));

        node = vd_dspc_str_list_next_node(node);
    }
    PUT_LINE("</p>");
}

static void process_section(VdDspcSection *section, FILE *out, int depth)
{
    Str title = make_str_from_tag_value(vd_dspc_section_first_tag(section));
    PUT("<section class=\"L%d section\" id=\"", depth);
    for (int i = 0; i < title.len; ++i) {
        char c = title.s[i];
        if (is_upppercase(c)) {
            PUT("%c", uppercase_to_lowercase(c));
        } else if (c == ' ') {
            PUT("-");
        } else {
            PUT("%c", c);
        }
    }
    PUT("\">\n");
    PUT_LINE("<h4>%.*s</h4>", STR_EXPAND(title));
    if (depth == 1) {
        PUT_LINE("<hr>");
    }
    for (VdDspcSection *child = section->first; child; child = child->next) {
        process_child(child, out, depth + 1);
    }
    PUT_LINE("</section>");
}


/* ------------------------------------------------------------------------------------------------------------------ */
/* ----DOCUBUILD IMPLEMENTATION-------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------------------------------ */
static int Processor_Table_Length;

#define INVOC_EXIT() ERR_EXIT("Invocation: docubuild <Directory>\n" \
                                                     "\t-o\tSet output directory." \
                                                     "\n")

static void traverse_section(VdDspcSection *section, int inset);
static void collect_source_file(File *file, void *userdata);
static void generate_html_for_tree(VdDspcTree *tree, FILE *out);

static Workspace *Global_Workspace;

int main(int argc, char const *argv[])
{
    vd_init(NULL);
    Processor_Table_Length = ARRAY_COUNT(Processor_Table);

    if (argc < 2) {
        INVOC_EXIT();
    }

    const char *directory_to_open = 0;
    const char *directory_to_write_to = 0;

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
            directory_to_write_to = argvalue_str.s;
        }
    }

    if (directory_to_write_to == 0) {
        ERR_EXIT("You need to specify write directory with -o!");
    }

    Workspace workspace = {
        .source_dir = vd_str_from_cstr((char*)directory_to_open),
        .global_arena = vd_arena_from_malloc(MEGABYTES(64)),
    };

    Global_Workspace = &workspace;

    dynarray_init_with_cap(workspace.source_files, &workspace.global_arena, 8);
    vd_directory_walk_recursively(directory_to_open, collect_source_file, &workspace);;


    vd_dspc_document_init(&workspace.document, NULL);

    // Open and parse all source files
    for (usize i = 0; i < dynarray_len(workspace.source_files); ++i) {
        SourceFile *source_file = &workspace.source_files[i];

        Arena *temp_arena = VD_GET_SCRATCH_ARENA();
        Str title = source_file->file_path_relative_to_source_dir;
        title = str_chop_right(title, str_last_of(title, LIT("."), VD_STR_MAX) - 1);
        source_file->title = title;

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
            vd_dspc_document_add(&workspace.document, (char*)file, len, source_file);
        }

        VD_RETURN_SCRATCH_ARENA(temp_arena);
    }

    for (VdDspcTree *tree = vd_dspc_document_first_tree(&workspace.document); tree; tree = vd_dspc_document_next_tree(&workspace.document, tree)) {
        Arena *temp_arena = VD_GET_SCRATCH_ARENA();
        SourceFile *source_file = (SourceFile*)tree->userdata;

        Str file_path_relative_to_source_dir_without_extension = str_chop_right(
            source_file->file_path_relative_to_source_dir,
            str_last_of(source_file->file_path_relative_to_source_dir, LIT("."), VD_STR_MAX) - 1);

        StrBuilder bld;
        str_builder_init(&bld, temp_arena);
        str_builder_push_cstr(&bld, directory_to_write_to);
        str_builder_push_str(&bld, file_path_relative_to_source_dir_without_extension);
        str_builder_push_cstr(&bld, ".html");
        str_builder_null_terminate(&bld);
        Str output_file = str_builder_compose(&bld, NULL);

        FILE *f = fopen(output_file.s, "w");
        generate_html_for_tree(tree, f);
        fclose(f);

        VD_RETURN_SCRATCH_ARENA(temp_arena);
    }

    return 0;
}

static void process_verbatim_html(VdDspcSection *section, FILE *out, int depth)
{
    Str section_id = make_str_from_section_id(section);

    PUT("<%.*s ", STR_EXPAND(section_id));

    for (VdDspcTag *tag = vd_dspc_section_first_tag(section); tag; tag = vd_dspc_section_next_tag(section, tag)) {
        Str tag_name = make_str_from_tag_name(tag);
        Str tag_value = make_str_from_tag_value(tag);
        PUT("%.*s=\"%.*s\" ", STR_EXPAND(tag_name), STR_EXPAND(tag_value));
    }
    PUT(">\n");

    for (VdDspcSection *child = section->first; child; child = child->next) {
        process_child(child, out, depth);
    }

    PUT("</%.*s>\n", STR_EXPAND(section_id));
}

static void process_child(VdDspcSection *section, FILE *out, int depth)
{
    Str section_id = {(char*)section->section_id.s, section->section_id.l};

    for (int i = 0; i < Processor_Table_Length; ++i) {
        if (str_eq(section_id, Processor_Table[i].id)) {
            Processor_Table[i].process(section, out, depth);
            break;
        }
    }
}

static Workspace *get_workspace(void)
{
    return Global_Workspace;
}

static void generate_html_for_tree(VdDspcTree *tree, FILE *out)
{
    PUT_LINE("<!DOCTYPE html>");
    PUT_LINE("<html lang=\"en\" data-bs-theme=\"dark\">");

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
            PUT_LINE("<link rel=\"apple-touch-icon\" sizes=\"180x180\" href=\"/apple-touch-icon.png\">");
            PUT_LINE("<link rel=\"icon\" type=\"image/png\" sizes=\"32x32\" href=\"/favicon-32x32.png\">");
            PUT_LINE("<link rel=\"icon\" type=\"image/png\" sizes=\"16x16\" href=\"/favicon-16x16.png\">");
            PUT_LINE("<link rel=\"manifest\" href=\"/site.webmanifest\">");
            PUT_LINE("<meta charset=\"utf-8\">");
            PUT_LINE("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            PUT_LINE("<title>%.*s</title>", STR_EXPAND(value_str));
            PUT_LINE("<link href=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/css/bootstrap.min.css\" rel=\"stylesheet\" integrity=\"sha384-sRIl4kxILFvY47J16cr9ZwB07vP4J8+LH7qKQnuqkuIAvNWLzeN8tE5YBujZqJLB\" crossorigin=\"anonymous\">");
            PUT_LINE("<link rel=\"stylesheet\" href=\"/style.css\">");;
            PUT_LINE("</head>");

            PUT_LINE("<body class=\"d-flex flex-column min-vh-100\" data-bs-spy=\"scroll\" data-bs-target=\"#toc\" data-bs-offset=\"50\" tabindex=\"0\">");
            PUT_LINE("<!-- Begin Navbar -->");
            PUT_LINE("<nav id=\"mainnav\" class=\"navbar sticky-top navbar-expand-md bg-body-tertiary\">");
                PUT_LINE("<div class=\"container-fluid bg-body-tertiary\">");
                    PUT_LINE("<a class=\"navbar-brand\" href=\"#\">");
                        PUT_LINE("<img    src=\"/assets/vd-logo-p@1x.png\"");
                            PUT_LINE("srcset=\"/assets/vd-logo-p@1x.png 1x, /assets/vd-logo-p@2x.png 2x\"");
                                PUT_LINE("alt=\"Brand\"");
                            PUT_LINE("width=\"64\"");
                            PUT_LINE("height=\"38.5\">");
                    PUT_LINE("</a>");
                    PUT_LINE("<button class=\"navbar-toggler\" type=\"button\" data-bs-toggle=\"collapse\" data-bs-target=\"#navbarNav\" ");
                            PUT_LINE("aria-controls=\"navbarNav\" aria-expanded=\"false\" aria-label=\"Toggle navigation\">");
                        PUT_LINE("<span class=\"navbar-toggler-icon\"></span>");
                    PUT_LINE("</button>");
                    PUT_LINE("<div class=\"collapse navbar-collapse\" id=\"navbarNav\">");
                        PUT_LINE("<ul class=\"navbar-nav\">");
                            PUT_LINE("<li class=\"nav-item\">");
                                PUT_LINE("<a class=\"nav-link\" aria-current=\"page\" href=\"/index.html\">Home</a>");
                            PUT_LINE("</li>");
                            PUT_LINE("<!-- Begin Files -->");
                            PUT_LINE("<li class=\"nav-item dropdown\">");
                                PUT_LINE("<a class=\"nav-link dropdown-toggle\" href=\"#\" id=\"documentationDropdown\" role=\"button\"  data-bs-toggle=\"dropdown\" aria-expanded=\"false\">");
                                    PUT_LINE("Documentation");
                                PUT_LINE("</a>");
                                PUT_LINE("<ul class=\"dropdown-menu\" aria-labelledby=\"documentationDropdown\">");
                                    for (VdDspcTree *tree = vd_dspc_document_first_tree(&get_workspace()->document); tree; tree = vd_dspc_document_next_tree(&get_workspace()->document, tree)) {
                                        SourceFile *source_file = (SourceFile*)tree->userdata;

                                        if (str_eq(source_file->title, LIT("index"))) {
                                            continue;
                                        }

                                        PUT_LINE("<li><a class=\"dropdown-item\" href=\"/%.*s.html\">%.*s.h</a></li>",
                                            STR_EXPAND(source_file->title),
                                            STR_EXPAND(source_file->title));
                                    }
                                PUT_LINE("</ul>");
                            PUT_LINE("</li>");
                            PUT_LINE("<!--   End Files -->");
                        PUT_LINE("</ul>");
                    PUT_LINE("</div>");
                PUT_LINE("</div>");
            PUT_LINE("</nav>");
            PUT_LINE("<!--   End Navbar -->");

            PUT_LINE("<div class=\"container flex-grow-1\">");
                PUT_LINE("<div class=\"row justify-content-center\">");
                    PUT_LINE("<div id=\"main-content\" class=\"col-12 col-sm-12 flexijustify-midpoint col-md-8 col-lg-8 text-start mx-auto mx-lg-4\">");

                        PUT_LINE("<nav id=\"toc\" data-toc class=\"toc-outline d-none midpoint-flex flexijustify-grow p-0 nav outline-section flex-column\">");
                        PUT_LINE("</nav>");

        } else if (str_eq(section_id, LIT("category"))) {
            VdDspcTag *tag_category = vd_dspc_section_first_tag(child);
            PUT_LINE("<!-- category: %.*s -->", (int)tag_category->value.l, tag_category->value.s);
        } else {
            process_child(child, out, 1);
        }
    }

            PUT_LINE("</div>");
        PUT_LINE("</div>");
    PUT_LINE("</div>");

    PUT_LINE("<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/js/bootstrap.bundle.min.js\"");
    PUT_LINE("        integrity=\"sha384-FKyoEForCGlyvwx9Hj09JcYn3nv7wiPVlz7YYwJrWVcXK/BmnVDxM+D2scQbITxI\"");
    PUT_LINE("        crossorigin=\"anonymous\"></script>");
    PUT_LINE("<script src=\"https://cdn.jsdelivr.net/npm/fuse.js@6.6.2/dist/fuse.min.js\"></script>");
    PUT_LINE("<script src=\"/auto-toc.js\"></script>");
    PUT_LINE("<script src=\"/index.js\"></script>");

    PUT_LINE("</body>");
    PUT_LINE("</html>");
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

static Str make_str_from_section_id(VdDspcSection *section)
{
    Str result = {(char*)section->section_id.s, section->section_id.l};   
    return result;
}

static Str make_str_from_tag_name(VdDspcTag *tag)
{
    Str result = {(char*)tag->name.s, tag->name.l};
    return result;
}

static Str make_str_from_tag_value(VdDspcTag *tag)
{
    Str result = {(char*)tag->value.s, tag->value.l};
    return result;
}

static Str make_str_from_str_node(VdDspcStrNode *node)
{
    Str result = {(char*)node->str, node->len};
    return result;
}

#define VD_DSPC_IMPL
#include "vd_docuspec.h"

#define VD_IMPL
#include "vd.h"
