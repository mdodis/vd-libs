#define VD_USE_CRT 1
#define VD_MACRO_ABBREVIATIONS 1
#include <stdio.h>
#include <stdlib.h>
#include "vd.h"
#include "vd_docuspec.h"

#define PUT_LINE(fmt, ...) fprintf(out, fmt "\n", ## __VA_ARGS__)
#define PUT(fmt, ...) fprintf(out, fmt , ## __VA_ARGS__)

#define ERR_EXIT(fmt, ...) do { \
        fprintf(stderr, fmt, ## __VA_ARGS__); \
        exit(-1);                          \
    } while (0)

typedef struct {
    Str   id;
    void  (*process)(VdDspcSection*, FILE*, int);
} Processor;

typedef struct {
    Str         file_path_relative_to_source_dir;
    Str         title;
    b32         notoc;
} SourceFile;

typedef struct {
    VdDspcSection *section;
    int           id;
} IndexedSection;

typedef struct {
    Arena               global_arena;
    Str                 source_dir;
    dynarray SourceFile *source_files;
    VdDspcDocument      document;
    SourceFile          *current_source_file;
    FILE                *search_index_file;
    int                 search_index_id;
} Workspace;

static Str make_str_from_section_id(VdDspcSection *section);
static Str make_str_from_tag_name(VdDspcTag *tag);
static Str make_str_from_tag_value(VdDspcTag *tag);
static Str make_str_from_str_node(VdDspcStrNode *node);
static Workspace *get_workspace(void);
static SourceFile *get_current_source_file(void);
static void process_child(VdDspcSection *section, FILE *out, int depth);
static void process_children(VdDspcSection *section, FILE *out, int depth);
static void process_verbatim_html(VdDspcSection *section, FILE *out, int depth);
static void search_index_begin_entry(FILE *out);
static void search_index_end_entry(FILE *out);
static int  search_index_id(void);
static int  search_index_curr_id(void);


static void process_apigen(VdDspcSection *section, FILE *out, int depth);
static void process_notoc(VdDspcSection *section, FILE *out, int depth);
static void process_paste(VdDspcSection *section, FILE *out, int depth);
static void process_api_remarks(VdDspcSection *section, FILE *out, int depth);
static void process_api_params(VdDspcSection *section, FILE *out, int depth);
static void process_api_brief(VdDspcSection *section, FILE *out, int depth);
static void process_api_details(VdDspcSection *section, FILE *out, int depth);
static void process_api_decl(VdDspcSection *section, FILE *out, int depth);
static void process_api(VdDspcSection *section, FILE *out, int depth);
static void process_rev(VdDspcSection *section, FILE *out, int depth);
static void process_tblock(VdDspcSection *section, FILE *out, int depth);
static void process_carousel(VdDspcSection *section, FILE *out, int depth);
static void process_code(VdDspcSection *section, FILE *out, int depth);
static void process_copyright(VdDspcSection *section, FILE *out, int depth);
static void process_br(VdDspcSection *section, FILE *out, int depth);
static void process_accordion(VdDspcSection *section, FILE *out, int depth);
static void process_section(VdDspcSection *section, FILE *out, int depth);
static void process_text(VdDspcSection *section, FILE *out, int depth);
static void process_para(VdDspcSection *section, FILE *out, int depth);
static Processor Processor_Table[] = {
    {LIT_INLINE("paste"),             process_paste},
    {LIT_INLINE("notoc"),             process_notoc},
    {LIT_INLINE("api"),               process_api},
    {LIT_INLINE("api-decl"),          process_api_decl},
    {LIT_INLINE("api-details"),       process_api_details},
    {LIT_INLINE("api-brief"),         process_api_brief},
    {LIT_INLINE("api-params"),        process_api_params},
    {LIT_INLINE("api-remarks"),       process_api_remarks},
    {LIT_INLINE("copyright"),         process_copyright},
    {LIT_INLINE("br"),                process_br},
    {LIT_INLINE("accordion"),         process_accordion},
    {LIT_INLINE("carousel"),          process_carousel},
    {LIT_INLINE("section"),           process_section},
    {LIT_INLINE("apigen"),            process_apigen},
    {LIT_INLINE("text"),              process_text},
    {LIT_INLINE("para"),              process_para},
    {LIT_INLINE("verb"),              process_code},
    {LIT_INLINE("tblock"),            process_tblock},
    {LIT_INLINE("rev"),               process_rev},
    {LIT_INLINE("div"),               process_verbatim_html},
    {LIT_INLINE("span"),              process_verbatim_html},
    {LIT_INLINE("img"),               process_verbatim_html},
    {LIT_INLINE("h5"),                process_verbatim_html},
    {LIT_INLINE("p"),                 process_verbatim_html},
    {LIT_INLINE("a"),                 process_verbatim_html},
    {LIT_INLINE("table"),             process_verbatim_html},
    {LIT_INLINE("tr"),                process_verbatim_html},
    {LIT_INLINE("th"),                process_verbatim_html},
    {LIT_INLINE("td"),                process_verbatim_html},
    {LIT_INLINE("tbody"),             process_verbatim_html},
    {LIT_INLINE("button"),            process_verbatim_html},
    {LIT_INLINE("script"),            process_verbatim_html},
};






static void parse_and_generate_tree(FILE *f, FILE *out, Arena *temp_arena);

static void process_apigen(VdDspcSection *section, FILE *out, int depth)
{
    Arena *temp_arena = VD_GET_SCRATCH_ARENA();

    Str file_str = make_str_from_tag_value(vd_dspc_section_first_tag(section));

    StrBuilder bld;
    str_builder_init(&bld, temp_arena);
    str_builder_push_str(&bld, file_str);
    str_builder_null_terminate(&bld);
    const char *filepath = str_builder_compose(&bld, NULL).s;

    LOGF("Reading api from: %s", filepath);

    FILE *f = fopen(filepath, "rb");

    parse_and_generate_tree(f, out, temp_arena);

    fclose(f);


    VD_RETURN_SCRATCH_ARENA(temp_arena);
}

static void process_notoc(VdDspcSection *section, FILE *out, int depth)
{
    get_current_source_file()->notoc = VD_TRUE;
}

static void process_paste(VdDspcSection *section, FILE *out, int depth)
{
    Arena *temp_arena = VD_GET_SCRATCH_ARENA();
    Str filepath = make_str_from_tag_value(vd_dspc_section_first_tag(section));

    StrBuilder bld;
    str_builder_init(&bld, temp_arena);
    str_builder_push_str(&bld, filepath);
    str_builder_null_terminate(&bld);
    const char *cstr_filepath = str_builder_compose(&bld, NULL).s;

    FILE *f = fopen(cstr_filepath, "r");
    char buf[1024];

    while (fgets(buf, 1024, f)) {
        PUT("%s", buf);
    }

    fclose(f);

    VD_RETURN_SCRATCH_ARENA(temp_arena);
}

static void process_api_remarks(VdDspcSection *section, FILE *out, int depth)
{
    PUT_LINE("<h6>Remarks</h6>");
    PUT_LINE("<p>");
    process_children(section, out, depth);
    PUT_LINE("</p>");
}

static void process_api_params(VdDspcSection *section, FILE *out, int depth)
{
    PUT_LINE("<table class=\"table\">");
    PUT_LINE("<tbody>");
    for (VdDspcSection *child = section->first; child; child = child->next) {
        Str param_name = make_str_from_tag_value(vd_dspc_section_first_tag(child));
        PUT_LINE("<tr>");
        PUT_LINE("<td>%.*s</td>", STR_EXPAND(param_name));
        PUT_LINE("<td>");
        process_children(child, out, depth);
        PUT_LINE("</td>");
        PUT_LINE("</tr>");
    }
    PUT_LINE("</tbody>");
    PUT_LINE("</table>");
}

static void process_api_brief(VdDspcSection *section, FILE *out, int depth)
{
    PUT_LINE("<p class=\"item-desc\">");
    process_children(section, out, depth);
    PUT_LINE("</p>");
}

static void process_api_details(VdDspcSection *section, FILE *out, int depth)
{
    PUT_LINE("<div class=\"apidetails\">");
    process_children(section, out, depth);
    PUT_LINE("</div>");
}

static void process_api_decl(VdDspcSection *section, FILE *out, int depth)
{
    Str decl_str = make_str_from_tag_value(vd_dspc_section_find_tag_with_name(section, NULL));
    PUT_LINE("<span class=\"item-decl\">%.*s</span>", STR_EXPAND(decl_str));
}

static void process_api(VdDspcSection *section, FILE *out, int depth)
{
    Str id_str = make_str_from_tag_value(vd_dspc_section_find_tag_with_name(section, NULL));

    PUT_LINE("<div id=\"%.*s\" class=\"L2 apiitem function\">", STR_EXPAND(id_str));

    process_children(section, out, depth);

    PUT_LINE("</div>");
}

static void process_rev(VdDspcSection *section, FILE *out, int depth)
{
    PUT_LINE("<div class=\"list-group\">");
    for (VdDspcSection *child = section->first; child; child = child->next) {
        Str version_str = make_str_from_tag_value(vd_dspc_section_find_tag_with_name(child, "version"));
        Str date_str = make_str_from_tag_value(vd_dspc_section_find_tag_with_name(child, "date"));
        Str avail_str = make_str_from_tag_value(vd_dspc_section_find_tag_with_name(child, "available"));

        if (str_eq(avail_str, LIT("no"))) {
            PUT_LINE("<a class=\"list-group-item revision-planned\">");
        } else {
            PUT_LINE("<a class=\"list-group-item\">");
        }
                PUT_LINE("<div class=\"d-flex w-100 justify-content-between\">");

                    PUT_LINE("<h5 class=\"mb-1\">%.*s</h5>", STR_EXPAND(version_str));
                    PUT_LINE("<small class=\"text-body-secondary\">%.*s</small>", STR_EXPAND(date_str));
                PUT_LINE("</div>");

                PUT_LINE("<p class=\"mb-1\">");
                    process_child(child->first, out, depth);
                PUT_LINE("</p>");
            PUT_LINE("</a>");
    }
    PUT_LINE("</div>");
}

static void process_tblock(VdDspcSection *section, FILE *out, int depth)
{
    PUT_LINE("<pre class=\"rounded copy\"><code class=\"language-txt\">");
    VdDspcStrNode *node = vd_dspc_str_list_first_node(&section->first->text_content);
    Str s = make_str_from_str_node(node);
    for (usize i = 0; i < s.len; ++i) {
        char c = s.s[i];

        if (c == '\r') {
            continue;
        }
        PUT("%c", c);
    }
    PUT_LINE("</code></pre>");
}

static void process_carousel(VdDspcSection *section, FILE *out, int depth)
{
    VdDspcTag *carousel_tag = vd_dspc_section_first_tag(section);
    Str carousel_tag_id = make_str_from_tag_value(carousel_tag);

    PUT_LINE("<div id=\"%.*s\" class=\"rounded carousel slide\">", STR_EXPAND(carousel_tag_id));
        PUT_LINE("<div class=\"carousel-inner\">");
            int first = 0;
            for (VdDspcSection *child = section->first; child; child = child->next) {
                Str child_image_path = make_str_from_tag_value(vd_dspc_section_first_tag(child));

                if (!first) {
                    PUT_LINE("<div class=\"carousel-item active\">");
                    first = 1;
                } else {
                    PUT_LINE("<div class=\"carousel-item\">");
                }

                PUT_LINE("<img class=\"img-fluid\" src=\"%.*s\">", STR_EXPAND(child_image_path));

                PUT_LINE("</div>");
            }
        PUT_LINE("</div>");

        PUT_LINE("<button class=\"carousel-control-prev\" type=\"button\" data-bs-target=\"#%.*s\""
                 "        data-bs-slide=\"prev\">",
                 STR_EXPAND(carousel_tag_id));
            PUT_LINE("<span class=\"carousel-control-prev-icon\" style=\"filter: invert(1) grayscale(100%%) brightness(200%%);\""
                     "      aria-hidden=\"true\"></span>");
            PUT_LINE("<span class=\"visually-hidden\">Previous</span>");
        PUT_LINE("</button>");

        PUT_LINE("<button class=\"carousel-control-next\" type=\"button\" data-bs-target=\"#%.*s\""
                 "        data-bs-slide=\"next\">",
                 STR_EXPAND(carousel_tag_id));
            PUT_LINE("<span class=\"carousel-control-next-icon\" style=\"filter: invert(1) grayscale(100%%) brightness(200%%);\""
                     "      aria-hidden=\"true\"></span>");
            PUT_LINE("<span class=\"visually-hidden\">Next</span>");
        PUT_LINE("</button>");
    PUT_LINE("</div>");
}

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
    VdDspcStrNode *node = vd_dspc_str_list_first_node(&section->text_content);
    while (node) {

        PUT_LINE("%.*s", STR_EXPAND(make_str_from_str_node(node)));

        node = vd_dspc_str_list_next_node(node);
    }
}

static void process_para(VdDspcSection *section, FILE *out, int depth)
{
    PUT_LINE("<p>");
    VdDspcStrNode *node = vd_dspc_str_list_first_node(&section->text_content);
    while (node) {

        PUT_LINE("%.*s", STR_EXPAND(make_str_from_str_node(node)));

        node = vd_dspc_str_list_next_node(node);
    }
    PUT_LINE("</p>");
}

static void process_code(VdDspcSection *section, FILE *out, int depth)
{
    PUT_LINE("<pre class=\"rounded copy\"><code class=\"language-cpp\">");
    VdDspcStrNode *node = vd_dspc_str_list_first_node(&section->text_content);
    Str s = make_str_from_str_node(node);
    for (usize i = 0; i < s.len; ++i) {
        char c = s.s[i];

        if (c == '\r') {
            continue;
        }
        PUT("%c", c);
    }
    PUT_LINE("</code></pre>");
}

static void output_section_id(VdDspcSection *section, FILE *out)
{
    Str title = make_str_from_tag_value(vd_dspc_section_first_tag(section));
    for (int i = 0; i < title.len; ++i) {
        char c = title.s[i];
        if (is_upppercase(c)) {
            PUT("%c", uppercase_to_lowercase(c));
        } else if (c == ' ') {
            PUT("-");
        } else if (c == '&') {
            PUT("and");
        } else {
            PUT("%c", c);
        }
    }
}

static void process_section(VdDspcSection *section, FILE *out, int depth)
{
    Str title = make_str_from_tag_value(vd_dspc_section_first_tag(section));
    PUT("<section class=\"L%d section\" id=\"", depth);
    output_section_id(section, out);
    PUT("\">\n");
    PUT_LINE("<h4>%.*s</h4>", STR_EXPAND(title));
    if (depth == 1) {
        PUT_LINE("<hr>");
    }
    for (VdDspcSection *child = section->first; child; child = child->next) {
        process_child(child, out, depth + 1);
    }
    PUT_LINE("</section>");
    if (depth != 1) {
        PUT_LINE("<br>");
    }
}

static void parse_api_function(char *buf, size_t buf_len, FILE *f, FILE *out, Arena *temp_arena)
{
    ArenaSave save = arena_save(temp_arena);

    char *brief_part = 0;
    char *return_part = 0;
    dynarray const char **param_part = 0;
    dynarray_init_with_cap(param_part, temp_arena, 12);


    while (fgets(buf, buf_len, f)) {

        int i = 0;

        if (buf[i + 0] == ' ' &&
            buf[i + 1] == '*' &&
            buf[i + 2] == '/')
        {
            break;
        }

        // Skip whitespaces
        while ((i < buf_len) && (buf[i] == ' ' || buf[i] == '*') && (buf[i] != '\0')) {
            i++;
        }

        // LOGF("Buf: %s parse", buf + i);
        if (buf[i + 0] == '@' &&
            buf[i + 1] == 'b' &&
            buf[i + 2] == 'r' &&
            buf[i + 3] == 'i' &&
            buf[i + 4] == 'e' &&
            buf[i + 5] == 'f')
        {
            const char *s = buf + i + 6;
            brief_part = cstr_dup(temp_arena, (cstr)s);
            continue;
        }

        if (buf[i + 0] == '@' &&
            buf[i + 1] == 'p' &&
            buf[i + 2] == 'a' &&
            buf[i + 3] == 'r' &&
            buf[i + 4] == 'a' &&
            buf[i + 5] == 'm')
        {
            const char *s = buf + i + 6;
            const char *sd = cstr_dup(temp_arena, (cstr)s);

            dynarray_add(param_part, sd);
            continue;
        }

        if (buf[i + 0] == '@' &&
            buf[i + 1] == 'r' && 
            buf[i + 2] == 'e' && 
            buf[i + 3] == 't' && 
            buf[i + 4] == 'u' && 
            buf[i + 5] == 'r' && 
            buf[i + 6] == 'n')
        {

            const char *s = buf + i + 7;
            const char *sd = cstr_dup(temp_arena, (cstr)s);

            return_part = (char*)sd;
            continue;
        }

    }

    // Get the function decl
    assert(fgets(buf, buf_len, f));
    const char *func_decl = buf;

    Str func_decl_str = str_from_cstr((char*)func_decl);
    Str id_str = func_decl_str;

    // Get function id
    {
        // Find first parenthesis
        usize paren_pos = str_first_of(func_decl_str, LIT("("), 0);
        usize id_pos = paren_pos;

        // Walk backwards until we find a space
        while (id_pos > 0 && func_decl_str.s[id_pos] != ' ') {
            id_pos--;
        }

        id_str.s = func_decl_str.s + id_pos + 1;
        id_str.len = paren_pos - id_pos - 1;

    }
    LOGF("%.*s", STR_EXPAND(id_str));

    Str func_decl_pretty = func_decl_str;
    // Get decl without _API, etc, i.e skip first word
    {
        u64 i = 0;
        u64 l = func_decl_str.len;

        while ((i < l) && (func_decl_str.s[i] != ' ')) {
            i++;
        }

        func_decl_pretty = str_chop_left(func_decl_str, i);

    }

    FILE *prevout = out;
    {
        out = get_workspace()->search_index_file;
        search_index_begin_entry(get_workspace()->search_index_file);


        PUT_LINE("\"id\": %d,", search_index_curr_id());
        PUT_LINE("\"page\": \"%.*s.html\",", STR_EXPAND(get_workspace()->current_source_file->title));
        PUT_LINE("\"title\": \"%.*s\",", STR_EXPAND(get_workspace()->current_source_file->title));
        PUT_LINE("\"section\": \"%.*s\",", STR_EXPAND(id_str));
        PUT_LINE("\"subtitle\": \"%.*s\",", STR_EXPAND(id_str));
        PUT("\"contents\": \"");
        PUT("%.*s", STR_EXPAND(id_str));
        PUT("\"");


        search_index_end_entry(get_workspace()->search_index_file);
    }

    out = prevout;



    PUT_LINE("<div id=\"%.*s\" class=\"L2 apiitem function\">", STR_EXPAND(id_str));
        PUT_LINE("<span class=\"item-decl\">%.*s</span>", STR_EXPAND(func_decl_pretty));
        PUT_LINE("<div class=\"apidetails\">");
            PUT_LINE("<p class=\"item-desc\">");
            PUT_LINE("%s", brief_part);
            PUT_LINE("</p>");


            PUT_LINE("<table class=\"table\">");
            PUT_LINE("<tbody>");
            for (usize pi = 0; pi < dynarray_len(param_part); ++pi) {
                const char *param = param_part[pi];

                Str param_name = str_null();
                Str param_desc = str_null();

                {
                    usize l = cstr_len((char*)param);
                    usize i = 0;

                    // Find name part
                    while ((i < l) && !is_letter(param[i])) {
                        i++;
                    }

                    usize pos_param_name_start = i++;
                    while ((i < l) && (param[i] != ' ')) {
                        i++;
                    }
                    usize pos_param_name_end = i;

                    while ((i < l) && !is_letter(param[i])) {
                        i++;
                    }
                    usize pos_param_desc_start = i;

                    param_name.s = (char*)param + pos_param_name_start;
                    param_name.len = pos_param_name_end - pos_param_name_start;

                    param_desc.s = (char*)param + pos_param_desc_start;
                    param_desc.len = l - pos_param_desc_start;

                }

                PUT_LINE("<tr>");
                PUT_LINE("<td>%.*s</td>", STR_EXPAND(param_name));
                PUT_LINE("<td>");
                PUT_LINE("%.*s", STR_EXPAND(param_desc));
                PUT_LINE("</td>");
                PUT_LINE("</tr>");
            }

            if (return_part) {
                PUT_LINE("<tr>");
                PUT_LINE("<td>Returns</td>");
                PUT_LINE("<td>");
                PUT_LINE("%s", return_part);
                PUT_LINE("</td>");
                PUT_LINE("</tr>");
            }

            PUT_LINE("</tbody>");
            PUT_LINE("</table>");
        PUT_LINE("</div>");
    PUT_LINE("</div>");

    arena_restore(save);
}

static void parse_and_generate_tree(FILE *f, FILE *out, Arena *temp_arena)
{
    char buf[1024];

    while (fgets(buf, 1024, f)) {
        if (buf[0] == '/' &&
            buf[1] == '*' &&
            buf[2] == '*')
        {
            parse_api_function(buf, 1024, f, out, temp_arena);
        }
    }
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
static void generate_index_entry_for_section(VdDspcSection *section, SourceFile *source_file, FILE *out, int *index_id);
static void output_section_id(VdDspcSection *section, FILE *out);
static void search_and_add_indexed_sections(VdDspcSection *section, IndexedSection *dynarray*indexed_sections, int *index_id);
static void generate_indexed_section_index(VdDspcSection *section, SourceFile *source_file, FILE *out);

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

    Arg arg = arg_new(argc, (char**)argv);
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

    {
        Arena *temp_arena = VD_GET_SCRATCH_ARENA();
        StrBuilder bld;
        str_builder_init(&bld, temp_arena);
        str_builder_push_cstr(&bld, directory_to_write_to);
        str_builder_push_str(&bld, LIT("search-index.json"));
        str_builder_null_terminate(&bld);
        const char *output_file = str_builder_compose(&bld, NULL).s;

        workspace.search_index_file = fopen(output_file, "w");
        fprintf(workspace.search_index_file, "[\n");
        VD_RETURN_SCRATCH_ARENA(temp_arena);

        workspace.search_index_id = 1;
    }

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

    // Generate html for all source files
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

        workspace.current_source_file = source_file;
        FILE *f = fopen(output_file.s, "w");
        generate_html_for_tree(tree, f);
        fclose(f);

        VD_RETURN_SCRATCH_ARENA(temp_arena);
    }

    Arena *temp_arena = VD_GET_SCRATCH_ARENA();
    for (VdDspcTree *tree = vd_dspc_document_first_tree(&workspace.document); tree; tree = vd_dspc_document_next_tree(&workspace.document, tree)) {
        FILE *out = workspace.search_index_file;
        SourceFile *source_file = (SourceFile*)tree->userdata;

        if (str_eq(source_file->title, LIT("index"))) {
            continue;
        }

        dynarray IndexedSection *indexed_sections = 0;
        dynarray_init(indexed_sections, temp_arena);

        search_and_add_indexed_sections(vd_dspc_tree_first_section(tree), &indexed_sections, &workspace.search_index_id);

        for (usize i = 0; i < dynarray_len(indexed_sections); ++i) {
            IndexedSection *indexed_section = &indexed_sections[i];

            search_index_begin_entry(out);

            PUT_LINE("\"id\": %d,", indexed_section->id);
            PUT_LINE("\"page\": \"%.*s.html\",", STR_EXPAND(source_file->title));
            PUT_LINE("\"title\": \"%.*s\",", STR_EXPAND(source_file->title));
            PUT("\"section\": \"");
            output_section_id(indexed_section->section, out);
            PUT("\",\n");

            PUT_LINE("\"subtitle\": \"%.*s\",", STR_EXPAND(make_str_from_tag_value(vd_dspc_section_first_tag(indexed_section->section))));

            PUT("\"contents\": \"");
            for (VdDspcSection *child = indexed_section->section->first; child; child = child->next) {
                generate_indexed_section_index(child, source_file, out);
            }

            PUT("\"");
            search_index_end_entry(out);
        }

    }
    fprintf(workspace.search_index_file, "]\n");
    fclose(workspace.search_index_file);

    VD_RETURN_SCRATCH_ARENA(temp_arena);
    return 0;
}

static void generate_indexed_section_index(VdDspcSection *section, SourceFile *source_file, FILE *out)
{
    Str section_id = make_str_from_section_id(section);
    if (str_eq(section_id, LIT("section"))) {
        return;
    }

    for (VdDspcStrNode *node = vd_dspc_str_list_first_node(&section->text_content); node; node = vd_dspc_str_list_next_node(node)) {
        Str text = make_str_from_str_node(node);

        for (usize i = 0; i < text.len; ++i) {
            char c = text.s[i];
            int c_is_whitespace = 
                (c == '\t') ||
                (c == '\r') ||
                (c == '\n') ||
                (c == '\b');

            if (c_is_whitespace) {
                PUT(" ");
            } else {
                if (c == '\'' || c == '\"' || c == '\\') {
                    continue;
                }
                PUT("%c", text.s[i]);
            }
        }
    }

    for (VdDspcSection *child = section->first; child; child = child->next) {
        generate_indexed_section_index(child, source_file, out);
    }
}

static void search_and_add_indexed_sections(VdDspcSection *section, IndexedSection *dynarray*indexed_sections, int *index_id)
{
    Str section_id = make_str_from_section_id(section);
    if (str_eq(section_id, LIT("section"))) {
        IndexedSection new_indexed_section = {
            .section = section,
            .id = (*index_id)++,
        };
        dynarray_add(*indexed_sections, new_indexed_section);
    }

    for (VdDspcSection *child = section->first; child; child = child->next) {
        search_and_add_indexed_sections(child, indexed_sections, index_id);
    }
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

static void process_children(VdDspcSection *section, FILE *out, int depth)
{
    for (VdDspcSection *child = section->first; child; child = child->next) {
        process_child(child, out, depth);
    }
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

static SourceFile *get_current_source_file(void)
{
    return get_workspace()->current_source_file;
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
                        PUT_LINE("<img    src=\"./assets/vd-logo-p@1x.png\"");
                            PUT_LINE("srcset=\"./assets/vd-logo-p@1x.png 1x, /assets/vd-logo-p@2x.png 2x\"");
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
                        PUT_LINE("<div class=\"search-container ms-auto me-auto me-sm-0\">");
                            PUT_LINE("<input type=\"search\" id=\"searchbox\" class=\"form-control\" placeholder=\"Search\" aria-label=\"Search\" autocomplete=\"off\" autofocus>");
                            PUT_LINE("<ul id=\"search-results\" class=\"list-group position-absolute\" style=\"top:100%%; z-index:1000; display:none;\"></ul>");
                        PUT_LINE("</div>");

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

    PUT_LINE("<script src=\"https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.11.1/highlight.min.js\"></script>");
    PUT_LINE("<script>hljs.highlightAll();</script>");
    PUT_LINE("<script src=\"https://cdn.jsdelivr.net/npm/bootstrap@5.3.8/dist/js/bootstrap.bundle.min.js\"");
    PUT_LINE("        integrity=\"sha384-FKyoEForCGlyvwx9Hj09JcYn3nv7wiPVlz7YYwJrWVcXK/BmnVDxM+D2scQbITxI\"");
    PUT_LINE("        crossorigin=\"anonymous\"></script>");
    PUT_LINE("<script src=\"https://cdn.jsdelivr.net/npm/fuse.js@6.6.2/dist/fuse.min.js\"></script>");
    if (!get_current_source_file()->notoc) {
        PUT_LINE("<script src=\"./auto-toc.js\"></script>");
    }
    PUT_LINE("<script src=\"./search.js\"></script>");
    PUT_LINE("<script src=\"./index.js\"></script>");

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
        .title = str_null(),
        .notoc = VD_FALSE,
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

static int search_index_id(void)
{
    return get_workspace()->search_index_id++;
}

static int search_index_curr_id(void)
{
    return get_workspace()->search_index_id - 1;
}

static void search_index_begin_entry(FILE *out)
{
    int id = search_index_id();

    if (id == 1) {
        PUT_LINE("{");
    } else {
        PUT_LINE(",{");
    }
}

static void search_index_end_entry(FILE *out)
{
    PUT_LINE("}");
}

#define VD_DSPC_IMPL
#include "vd_docuspec.h"

#define VD_IMPL
#include "vd.h"
