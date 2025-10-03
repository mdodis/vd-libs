/**
 * vd_docuspec.h - Reference implementation of DOCUSPEC language
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
#ifndef VD_DSPC_H
#define VD_DSPC_H

#ifdef VD_DSPC_STATIC
#define VD_DSPC_API static
#else
#define VD_DSPC_API extern
#endif // VD_DSPC_STATIC

#ifndef    VD_DSPC_INL
#define    VD_DSPC_INL static inline
#endif // !VD_DSPC_INL

/**
 * Set to zero to disable inclusion of stdlib.h, but make sure to provide VdDspcDocument::alloc!
 */
#ifndef    VD_DSPC_STDLIB
#define    VD_DSPC_STDLIB 1
#endif // !VD_DSPC_STDLIB

/**
 * Set manually to enable/disable assertions
 */
#ifndef    VD_DSPC_ASSERTIONS
#define    VD_DSPC_ASSERTIONS 1
#endif // !VD_DSPC_ASSERTIONS

typedef struct VdDspc__Arena {
    unsigned char *buf;
    size_t        buf_len;
    size_t        curr_offset;
} VdDspc__Arena;

typedef struct VdDspcStr {
    const char *s;
    size_t     l;
} VdDspcStr;

typedef struct VdDspcStrNode VdDspcStrNode; 
struct VdDspcStrNode {
    VdDspcStrNode *next;
    VdDspcStrNode *prev;
    const char    *str;
    size_t        len;
};

typedef struct VdDspcStrList {
    VdDspcStrNode *first;
    VdDspcStrNode *last;
} VdDspcStrList;

typedef struct VdDspcTag VdDspcTag;
struct VdDspcTag {
    VdDspcTag *next;
    VdDspcTag *prev;

    VdDspcStr  name;
    VdDspcStr  value;
};

typedef struct VdDspcSection VdDspcSection;
struct VdDspcSection {
    VdDspcSection *next;
    VdDspcSection *prev;
    VdDspcSection *first;
    VdDspcSection *last;
    VdDspcSection *parent;

    VdDspcStr     section_id;
    VdDspcTag     tag_sentinel;

    VdDspcStrList text_content;
};

typedef struct VdDspcTree VdDspcTree; 
struct VdDspcTree {
    VdDspcTree    *next;
    VdDspcTree    *prev;
    VdDspcSection global;
    void          *userdata;
};

typedef struct VdDspc__Lex {
    const char *content;
    size_t     content_size;
    size_t     cur_fwd;
    size_t     cur_back;
    size_t     line;
    size_t     column;
} VdDspc__Lex;

typedef struct VdDspc__Lang {
    VdDspcTree  section_stack[256];
    int         section_stack_count;
} VdDspc__Lang;

typedef enum {
    VD_DSPC__TOKEN_TYPE_UNKNOWN = 0,
    VD_DSPC__TOKEN_TYPE_AT,
    VD_DSPC__TOKEN_TYPE_ID,
    VD_DSPC__TOKEN_TYPE_TAGS_BEGIN,
    VD_DSPC__TOKEN_TYPE_TAGS_END,
    VD_DSPC__TOKEN_TYPE_QUOTED_STRING,
    VD_DSPC__TOKEN_TYPE_SECTION_BEGIN,
    VD_DSPC__TOKEN_TYPE_SECTION_END,
    VD_DSPC__TOKEN_TYPE_TEXT_CONTENT,
    VD_DSPC__TOKEN_TYPE_PARAGRAPH,
    VD_DSPC__TOKEN_TYPE_ASSIGNMENT,
    VD_DSPC__TOKEN_TYPE_CODE_CONTENT,
    VD_DSPC__TOKEN_TYPE_END        = -1,
} VdDspc__TokenType;

typedef struct VdDspc__Token {
    VdDspc__Lex       lexstate;
    VdDspc__TokenType type;
    union {
        VdDspcStrList text_content;
        VdDspcStr     quoted_string;
        VdDspcStr     verbatim_string;
    } dat;
} VdDspc__Token;

typedef struct VdDspcDocument {
    VdDspcTree      sentinel;
    void*           (*alloc)(void *, size_t, size_t);

    int             arenas_len;
    int             arenas_cap;
    VdDspc__Arena   *arenas;

    VdDspcSection   *curr_section;
    VdDspc__Lex     lexstate;
} VdDspcDocument;

#define VD_DSPC_PROC_LOG(name) void name(const char *message)
typedef VD_DSPC_PROC_LOG(VdDspcProcLog);

VD_DSPC_API void             vd_dspc_set_log_fn(VdDspcProcLog *log_fptr);
VD_DSPC_API void             vd_dspc_document_init(VdDspcDocument *doc, void* (*alloc)(void *, size_t, size_t));
VD_DSPC_API int              vd_dspc_document_add(VdDspcDocument *doc, const char *content, size_t content_size,
                                                  void *userdata);
VD_DSPC_API VdDspcTree*      vd_dspc_document_first_tree(VdDspcDocument *doc);
VD_DSPC_API VdDspcTree*      vd_dspc_document_next_tree(VdDspcDocument *doc, VdDspcTree *tree);
VD_DSPC_API VdDspcSection*   vd_dspc_tree_first_section(VdDspcTree *tree);
VD_DSPC_API VdDspcTag*       vd_dspc_section_first_tag(VdDspcSection *section);
VD_DSPC_API VdDspcTag*       vd_dspc_section_next_tag(VdDspcSection *section, VdDspcTag *tag);
VD_DSPC_API VdDspcTag*       vd_dspc_section_find_tag_with_name(VdDspcSection *section, const char *search);
VD_DSPC_API VdDspcStrNode*   vd_dspc_str_list_first_node(VdDspcStrList *list);
VD_DSPC_API VdDspcStrNode*   vd_dspc_str_list_next_node(VdDspcStrNode *node);
VD_DSPC_API int              vd_dspc_str_list_is_empty(VdDspcStrList *list);

#endif // !VD_DSPC_H

#ifdef VD_DSPC_IMPL

#if VD_DSPC_STDLIB
#include <stdlib.h>
#include <string.h>
#include <assert.h>
static void *vd_dspc__stdlib_alloc(void *prev, size_t prev_size, size_t new_size)
{
    (void)prev_size;

    if (new_size == 0) {
        free(prev);
        return 0;
    }

    return realloc(prev, new_size);
}

#if VD_DSPC_ASSERTIONS
#define VD_DSPC_ASSERT(x) assert(x)
#endif // VD_DSPC_ASSERTIONS

#endif // VD_DSPC_STDLIB

#ifndef VD_DSPC_ASSERT
#define VD_DSPC_ASSERT(x)
#endif // !VD_DSPC_ASSERT

static VdDspcProcLog *Vd_Dspc__Log_Fn = NULL;

static char Vd_Dspc__Log_Buf[2048];

static VD_DSPC_PROC_LOG(vd_dspc__log_stdout);

#define VD_DSPC_LOG(fmt, ...) do {                           \
        snprintf(Vd_Dspc__Log_Buf, 2048, fmt, __VA_ARGS__);  \
        Vd_Dspc__Log_Fn(Vd_Dspc__Log_Buf);                   \
    } while(0)

/* ----UTILITIES----------------------------------------------------------------------------------------------------- */
static int             vd_dspc__is_alpha(int c);
static int             vd_dspc__is_num(int c);
static int             vd_dspc__is_whitespace(int c);
static int             vd_dspc__is_newline(int c);
static int             vd_dspc__is_consumable_whitespace(int c);
static int             vd_dspc__is_valid_id_continue(int c);
static char*           vd_dspc__token_type_to_string(VdDspc__TokenType ttype);
static size_t          vd_dspc__token_string_size(VdDspc__Token *tok);
static char*           vd_dspc__token_string_begin(VdDspc__Token *tok);

/* ----ARENAS-------------------------------------------------------------------------------------------------------- */
static void*           vd_dspc__push(VdDspcDocument *doc, size_t size);

/* ----TREES--------------------------------------------------------------------------------------------------------- */
static VdDspcTree*     vd_dspc__push_tree(VdDspcDocument *doc);
static VdDspcTree*     vd_dspc__tree(VdDspcDocument *doc);

/* ----SECTIONS------------------------------------------------------------------------------------------------------ */
static VdDspcSection*  vd_dspc__push_section(VdDspcDocument *doc, const char *id_begin_string, size_t id_len_string);

/* ----TAGS---------------------------------------------------------------------------------------------------------- */
static VdDspcTag*      vd_dspc__push_tag(VdDspcDocument *doc, VdDspcSection *section,
                                         VdDspcStr tag_name, VdDspcStr tag_value);

/* ----STRLIST------------------------------------------------------------------------------------------------------- */
static void            vd_dspc__str_list_push_token(VdDspcDocument *doc, VdDspcStrList *list, VdDspc__Token *tok);
static void            vd_dspc__str_list_push_verbatim(VdDspcDocument *doc, VdDspcStrList *list, VdDspc__Token *tok);
static VdDspcStrNode*  vd_dspc__push_str_node(VdDspcDocument *doc);

/* ----LEXER--------------------------------------------------------------------------------------------------------- */
static int             vd_dspc__lex_end(VdDspc__Lex *lex);
static int             vd_dspc__lex_nextn(VdDspc__Lex *lex, size_t n);
static int             vd_dspc__lex_char(VdDspc__Lex *lex);
static VdDspc__Token   vd_dspc__lex_token(VdDspcDocument *doc, VdDspc__Lex *lex);
static void            vd_dspc__lex_consume(VdDspc__Lex *lex, VdDspc__Token *token);
static VdDspc__Token   vd_dspc__token_new(VdDspc__Lex *lex, VdDspc__TokenType type);
static void            vd_dspc__token_log(VdDspc__Token *tok);
static int             vd_dspc__skip_whitespace_all(VdDspc__Lex *lex);
static int             vd_dspc__skip_newlines(VdDspc__Lex *lex);
static VdDspc__Lex     vd_dspc__lex_save(VdDspc__Lex *lex);
static void            vd_dspc__lex_load(VdDspc__Lex *target, VdDspc__Lex *src);

/* ----PARSER-------------------------------------------------------------------------------------------------------- */
static int             vd_dspc__lang_section(VdDspcDocument *doc);

VD_DSPC_API void vd_dspc_set_log_fn(VdDspcProcLog *log_fptr)
{
    Vd_Dspc__Log_Fn = log_fptr;
}

VD_DSPC_API void vd_dspc_document_init(VdDspcDocument *doc, void* (*alloc)(void *, size_t, size_t))
{
    if (Vd_Dspc__Log_Fn == NULL) {
        Vd_Dspc__Log_Fn = vd_dspc__log_stdout;
    }

    doc->sentinel.next = &doc->sentinel;
    doc->sentinel.prev = &doc->sentinel;
    doc->arenas        = NULL;
    doc->arenas_len    = 0;
    doc->arenas_cap    = 0;
    doc->alloc = alloc;
    if (doc->alloc == NULL) {
        doc->alloc = vd_dspc__stdlib_alloc;
    }
}

VD_DSPC_API int vd_dspc_document_add(VdDspcDocument *doc, const char *content, size_t content_size, void *userdata)
{
    VdDspcTree *tree = vd_dspc__push_tree(doc);
    tree->userdata = userdata;

    doc->lexstate.content      = content;
    doc->lexstate.content_size = content_size;
    doc->lexstate.cur_fwd      = 0;
    doc->lexstate.cur_back     = 0;
    doc->lexstate.line         = 0;
    doc->lexstate.column       = 0;

    while (!vd_dspc__lex_end(&doc->lexstate)) {
        if (!vd_dspc__lang_section(doc)) {
            break;
        }
    }

    return 0;
}

VD_DSPC_API VdDspcTree *vd_dspc_document_first_tree(VdDspcDocument *doc)
{
    return (doc->sentinel.next != &doc->sentinel) ? doc->sentinel.next : 0;
}

VD_DSPC_API VdDspcTree *vd_dspc_document_next_tree(VdDspcDocument *doc, VdDspcTree *tree)
{
    return (tree->next != &doc->sentinel) ? tree->next : 0;
}

VD_DSPC_API VdDspcSection *vd_dspc_tree_first_section(VdDspcTree *tree)
{
    return (&tree->global);
}

VD_DSPC_API VdDspcTag *vd_dspc_section_first_tag(VdDspcSection *section)
{
    return (section->tag_sentinel.next != &section->tag_sentinel) ? section->tag_sentinel.next : 0;
}

VD_DSPC_API VdDspcTag *vd_dspc_section_next_tag(VdDspcSection *section, VdDspcTag *tag)
{
    return (tag->next != &section->tag_sentinel) ? tag->next : 0;
}

static int vd_dspc_str_eq(VdDspcStr a, VdDspcStr b)
{
    if (a.l != b.l) return 0;

    for (size_t i = 0; i < a.l; ++i) {
        if (a.s[i] != b.s[i]) return 0;
    }

    return 1;
}

VD_DSPC_API VdDspcTag *vd_dspc_section_find_tag_with_name(VdDspcSection *section, const char *search)
{
    int len = search == 0 ? 0 : strlen(search);

    VdDspcStr search_str = { search, len };

    for (VdDspcTag *tag = vd_dspc_section_first_tag(section); tag; tag = vd_dspc_section_next_tag(section, tag)) {
        if (len == 0 && tag->name.l == 0) {
            return tag;
        }

        if (vd_dspc_str_eq(tag->name, search_str)) {
            return tag;
        }
    }

    return 0;
}

VD_DSPC_API VdDspcStrNode *vd_dspc_str_list_first_node(VdDspcStrList *list)
{
    return list->first;
}

VD_DSPC_API VdDspcStrNode *vd_dspc_str_list_next_node(VdDspcStrNode *node)
{
    return node->next;
}

VD_DSPC_API int vd_dspc_str_list_is_empty(VdDspcStrList *list)
{
    return list->first == 0;
}

/* ----PARSER IMPL--------------------------------------------------------------------------------------------------- */
static void vd_dspc__lang_text_content(VdDspcDocument *doc, VdDspc__Token *token, const char *identifier, size_t identifier_size)
{
    vd_dspc__lex_consume(&doc->lexstate, token);

    const char *id_begin_string = identifier;
    size_t id_len_string        = identifier_size;
    VdDspcSection *new_section  = vd_dspc__push_section(doc, id_begin_string, id_len_string);
    new_section->text_content = token->dat.text_content;
}

static int vd_dspc__lang_section(VdDspcDocument *doc)
{
    VdDspc__Token token = vd_dspc__lex_token(doc, &doc->lexstate);

    if (token.type == VD_DSPC__TOKEN_TYPE_AT) {
        vd_dspc__lex_consume(&doc->lexstate, &token);

        VdDspc__Token id_token = vd_dspc__lex_token(doc, &doc->lexstate);
        if (id_token.type != VD_DSPC__TOKEN_TYPE_ID) {
            VD_DSPC_LOG("Error: %llu:%llu expected: id token, instead got: %s",
                id_token.lexstate.line + 1, id_token.lexstate.column + 1,
                vd_dspc__token_type_to_string(id_token.type));
            return 0;
        }
        vd_dspc__lex_consume(&doc->lexstate, &id_token);

        // Push new section
        const char *id_begin_string = vd_dspc__token_string_begin(&id_token);
        size_t id_len_string        = vd_dspc__token_string_size(&id_token);
        VdDspcSection *new_section  = vd_dspc__push_section(doc, id_begin_string, id_len_string);

        // Parse tags
        VdDspc__Token tags_begin_token = vd_dspc__lex_token(doc, &doc->lexstate);
        if (tags_begin_token.type != VD_DSPC__TOKEN_TYPE_TAGS_BEGIN) {

            VD_DSPC_LOG("Error: %llu:%llu expected: '(' token, instead got: %s",
                tags_begin_token.lexstate.line + 1, tags_begin_token.lexstate.column + 1,
                vd_dspc__token_type_to_string(tags_begin_token.type));
        }
        vd_dspc__lex_consume(&doc->lexstate, &tags_begin_token);

        VdDspc__Token t = vd_dspc__lex_token(doc, &doc->lexstate);
        while (t.type != VD_DSPC__TOKEN_TYPE_TAGS_END) {

            VdDspcStr tag_name = {0, 0};
            VdDspcStr tag_value = {0, 0};

            // If it's quoted, insert it into the unnamed section of the section tags
            if (t.type == VD_DSPC__TOKEN_TYPE_QUOTED_STRING) {
                tag_value = t.dat.quoted_string;
                vd_dspc__lex_consume(&doc->lexstate, &t);
            } else if (t.type == VD_DSPC__TOKEN_TYPE_ID) {

                tag_name.s = vd_dspc__token_string_begin(&t);
                tag_name.l = vd_dspc__token_string_size(&t);
                vd_dspc__lex_consume(&doc->lexstate, &t);

                // Expect = "quoted"
                VdDspc__Token assignment_token = vd_dspc__lex_token(doc, &doc->lexstate);
                if (assignment_token.type != VD_DSPC__TOKEN_TYPE_ASSIGNMENT) {
                    VD_DSPC_LOG("Error: %llu:%llu expected: '=' token, instead got: %s",
                        assignment_token.lexstate.line + 1, assignment_token.lexstate.column + 1,
                        vd_dspc__token_type_to_string(assignment_token.type));
                    return 0;
                }
                vd_dspc__lex_consume(&doc->lexstate, &assignment_token);

                VdDspc__Token value_token = vd_dspc__lex_token(doc, &doc->lexstate);
                if (value_token.type != VD_DSPC__TOKEN_TYPE_QUOTED_STRING) {
                    VD_DSPC_LOG("Error: %llu:%llu expected: '\"quoted string\"' token, instead got: %s",
                        value_token.lexstate.line + 1, value_token.lexstate.column + 1,
                        vd_dspc__token_type_to_string(value_token.type));
                    return 0;
                }

                tag_value = value_token.dat.quoted_string;
                vd_dspc__lex_consume(&doc->lexstate, &value_token);
            } else {
                VD_DSPC_LOG("Error: %llu:%llu expected either unnamed value or named valued, instead got: %s",
                    t.lexstate.line + 1, t.lexstate.column + 1,
                    vd_dspc__token_type_to_string(t.type));
                return 0;
            }

            vd_dspc__push_tag(doc, new_section, tag_name, tag_value);

            t = vd_dspc__lex_token(doc, &doc->lexstate);
        }

        // Consume tags end token
        vd_dspc__lex_consume(&doc->lexstate, &t);

        // Parse Section content
        VdDspc__Token token_after_tags = vd_dspc__lex_token(doc, &doc->lexstate);

        if (token_after_tags.type != VD_DSPC__TOKEN_TYPE_SECTION_BEGIN) {
            // We can continue parsing for a new section since this one has no children
            return 1;
        }

        VdDspc__Token section_begin_token = token_after_tags;
        vd_dspc__lex_consume(&doc->lexstate, &section_begin_token);

        // At this point, we push into the child
        doc->curr_section = new_section;

        while (1) {
            VdDspc__Lex saved_state = vd_dspc__lex_save(&doc->lexstate);
            if (!vd_dspc__lang_section(doc)) {
                vd_dspc__lex_load(&doc->lexstate, &saved_state);
                break;
            }
        }

        t = vd_dspc__lex_token(doc, &doc->lexstate);
        if (t.type != VD_DSPC__TOKEN_TYPE_SECTION_END) {
            VD_DSPC_LOG("Error: %llu:%llu expected '}', instead got: %s",
                t.lexstate.line + 1, t.lexstate.column + 1,
                vd_dspc__token_type_to_string(t.type));
        }

        doc->curr_section = new_section->parent;
        vd_dspc__lex_consume(&doc->lexstate, &t);

        return 1;
    } else if (token.type == VD_DSPC__TOKEN_TYPE_TEXT_CONTENT) {
        vd_dspc__lang_text_content(doc, &token, "text", 4);
        return 1;
    } else if (token.type == VD_DSPC__TOKEN_TYPE_PARAGRAPH) {
        vd_dspc__lang_text_content(doc, &token, "para", 4);
        return 1;
    } else if (token.type == VD_DSPC__TOKEN_TYPE_CODE_CONTENT) {
        vd_dspc__lex_consume(&doc->lexstate, &token);

        const char *id_begin_string = "verb";
        size_t id_len_string        = 4;
        VdDspcSection *new_section  = vd_dspc__push_section(doc, id_begin_string, id_len_string);
        VdDspcStrList list = {0};
        vd_dspc__str_list_push_verbatim(doc, &list, &token);
        new_section->text_content = list;
        return 1;
    }

    return 0;
}

/* ----LEXER IMPL---------------------------------------------------------------------------------------------------- */
static int vd_dspc__lex_end(VdDspc__Lex *lex)
{
    return lex->cur_fwd >= lex->content_size;
}

static int vd_dspc__lex_nextn(VdDspc__Lex *lex, size_t n)
{
    while (n != 0) {
        if (vd_dspc__lex_end(lex)) {
            return 0;
        }

        if (lex->content[lex->cur_fwd] == '\n') {
            lex->line++;
            lex->column = 0;
        } else {
            lex->column++;
        }

        lex->cur_fwd++;
        n--;
    }

    return 1;
}

static int vd_dspc__lex_char(VdDspc__Lex *lex)
{
    if (vd_dspc__lex_end(lex)) {
        return -1;
    }

    return lex->content[lex->cur_fwd];
}

static int vd_dspc__lex_literal(VdDspcDocument *doc, VdDspc__Token *result, const char *identifier, size_t identifier_size, VdDspc__TokenType resulting_type)
{
    const char *text = identifier;
    size_t text_len = identifier_size;
    int id_is_equal_to_text = 0;
    size_t id_size = result->lexstate.cur_fwd - result->lexstate.cur_back;

    if (id_size == text_len) {
        const char *content = result->lexstate.content;
        id_is_equal_to_text = 
            (content[result->lexstate.cur_back + 0] == text[0]) &&
            (content[result->lexstate.cur_back + 1] == text[1]) &&
            (content[result->lexstate.cur_back + 2] == text[2]) &&
            (content[result->lexstate.cur_back + 3] == text[3]);

        if (id_is_equal_to_text) {
            result->type = resulting_type;
            // Go into inner loop to get the text content

            // Skip whitespace and find SECTION BEGIN
            vd_dspc__skip_whitespace_all(&result->lexstate);

            if (vd_dspc__lex_char(&result->lexstate) != '{') {
                VD_DSPC_LOG("Error: %llu:%llu Expected \'{\' after \'text\' section.",
                            result->lexstate.line + 1, result->lexstate.column + 1);
                result->type = VD_DSPC__TOKEN_TYPE_UNKNOWN;
                return 0;
            }
            vd_dspc__lex_nextn(&result->lexstate, 1);

            // Skip leading whitespace, if any
            vd_dspc__skip_whitespace_all(&result->lexstate);

            VdDspc__Token text_content_token = *result;
            text_content_token.lexstate.cur_back = text_content_token.lexstate.cur_fwd;

            int text_parsing_ended = 0;

            VdDspcStrList list = {0};

            while (!text_parsing_ended) {

                int c = vd_dspc__lex_char(&text_content_token.lexstate);

                if (c == '}') {
                    text_parsing_ended = 1;
                    continue;
                }

                if (vd_dspc__is_consumable_whitespace(c)) {
                    vd_dspc__str_list_push_token(doc, &list, &text_content_token);

                    vd_dspc__skip_whitespace_all(&text_content_token.lexstate);
                    text_content_token.lexstate.cur_back = text_content_token.lexstate.cur_fwd;
                } else {
                    vd_dspc__lex_nextn(&text_content_token.lexstate, 1);
                }
            }

            if (text_content_token.lexstate.cur_fwd != text_content_token.lexstate.cur_back) {
                vd_dspc__str_list_push_token(doc, &list, &text_content_token);
            }

            vd_dspc__lex_nextn(&text_content_token.lexstate, 1);

            result->lexstate.cur_fwd = text_content_token.lexstate.cur_fwd;
            result->dat.text_content = list;
            return 1;
        }
    }

    return 0;
}

static VdDspc__Token vd_dspc__lex_token(VdDspcDocument *doc, VdDspc__Lex *lex)
{
    vd_dspc__skip_whitespace_all(lex);
    VdDspc__Lex saved_state = vd_dspc__lex_save(lex);

    VdDspc__Token result;
    result.type     = VD_DSPC__TOKEN_TYPE_UNKNOWN;
    result.lexstate = saved_state;

    int c = vd_dspc__lex_char(lex);

    switch (c) {
        case '@':  result.type = VD_DSPC__TOKEN_TYPE_AT;            vd_dspc__lex_nextn(&result.lexstate, 1); break;
        case '(':  result.type = VD_DSPC__TOKEN_TYPE_TAGS_BEGIN;    vd_dspc__lex_nextn(&result.lexstate, 1); break;
        case ')':  result.type = VD_DSPC__TOKEN_TYPE_TAGS_END;      vd_dspc__lex_nextn(&result.lexstate, 1); break;
        case '{':  result.type = VD_DSPC__TOKEN_TYPE_SECTION_BEGIN; vd_dspc__lex_nextn(&result.lexstate, 1); break;
        case '}':  result.type = VD_DSPC__TOKEN_TYPE_SECTION_END;   vd_dspc__lex_nextn(&result.lexstate, 1); break;
        case '=':  result.type = VD_DSPC__TOKEN_TYPE_ASSIGNMENT;    vd_dspc__lex_nextn(&result.lexstate, 1); break;
        case -1:   result.type = VD_DSPC__TOKEN_TYPE_END;                                                    break;
        case '\"': {
            // Parse quoted string
            vd_dspc__lex_nextn(&result.lexstate, 1);
            c = vd_dspc__lex_char(&result.lexstate);

            while (c != '\"') {

                if (!vd_dspc__lex_nextn(&result.lexstate, 1)) {
                    VD_DSPC_LOG("Error: %llu:%llu: Quoted string starting here was never unquoted!",
                                saved_state.line, saved_state.column);
                    return result;
                }

                c = vd_dspc__lex_char(&result.lexstate);
            }
            // Include also the ending "
            vd_dspc__lex_nextn(&result.lexstate, 1);

            result.type = VD_DSPC__TOKEN_TYPE_QUOTED_STRING;
            result.dat.quoted_string.s = vd_dspc__token_string_begin(&result) + 1;
            result.dat.quoted_string.l = vd_dspc__token_string_size(&result) - 2;

        } break;
        default: {
            // Attempt to parse identifier
            if (vd_dspc__is_alpha(c)) {
                while (vd_dspc__is_valid_id_continue(c)) {
                    if (!vd_dspc__lex_nextn(&result.lexstate, 1)) {
                        result.type = VD_DSPC__TOKEN_TYPE_END;
                        return result;
                    }

                    c = vd_dspc__lex_char(&result.lexstate);
                } 

                result.type = VD_DSPC__TOKEN_TYPE_ID;

                // TEXT CONTENT
                if (vd_dspc__lex_literal(doc, &result, "text", 4, VD_DSPC__TOKEN_TYPE_TEXT_CONTENT)) {
                    break;
                }

                if (vd_dspc__lex_literal(doc, &result, "para", 4, VD_DSPC__TOKEN_TYPE_PARAGRAPH)) {
                    break;
                }

                if (0) {
                    const char *text = "text";
                    size_t text_len = 4;
                    int id_is_equal_to_text = 0;
                    size_t id_size = result.lexstate.cur_fwd - result.lexstate.cur_back;

                    if (id_size == text_len) {
                        const char *content = result.lexstate.content;
                        id_is_equal_to_text = 
                            (content[result.lexstate.cur_back + 0] == text[0]) &&
                            (content[result.lexstate.cur_back + 1] == text[1]) &&
                            (content[result.lexstate.cur_back + 2] == text[2]) &&
                            (content[result.lexstate.cur_back + 3] == text[3]);

                        if (id_is_equal_to_text) {
                            result.type = VD_DSPC__TOKEN_TYPE_TEXT_CONTENT;
                            // Go into inner loop to get the text content

                            // Skip whitespace and find '{'
                            vd_dspc__skip_whitespace_all(&result.lexstate);

                            if (vd_dspc__lex_char(&result.lexstate) != '{') {
                                VD_DSPC_LOG("Error: %llu:%llu Expected \'{\' after \'text\' section.",
                                            result.lexstate.line + 1, result.lexstate.column + 1);
                                result.type = VD_DSPC__TOKEN_TYPE_UNKNOWN;
                                return result;
                            }
                            vd_dspc__lex_nextn(&result.lexstate, 1);

                            // Skip leading whitespace, if any
                            vd_dspc__skip_whitespace_all(&result.lexstate);

                            VdDspc__Token text_content_token = result;
                            text_content_token.lexstate.cur_back = text_content_token.lexstate.cur_fwd;

                            int text_parsing_ended = 0;

                            VdDspcStrList list = {0};

                            while (!text_parsing_ended) {

                                c = vd_dspc__lex_char(&text_content_token.lexstate);

                                if (c == '}') {
                                    text_parsing_ended = 1;
                                    continue;
                                }

                                if (vd_dspc__is_consumable_whitespace(c)) {
                                    vd_dspc__str_list_push_token(doc, &list, &text_content_token);

                                    vd_dspc__skip_whitespace_all(&text_content_token.lexstate);
                                    text_content_token.lexstate.cur_back = text_content_token.lexstate.cur_fwd;
                                } else {
                                    vd_dspc__lex_nextn(&text_content_token.lexstate, 1);
                                }
                            }

                            if (text_content_token.lexstate.cur_fwd != text_content_token.lexstate.cur_back) {
                                vd_dspc__str_list_push_token(doc, &list, &text_content_token);
                            }

                            vd_dspc__lex_nextn(&text_content_token.lexstate, 1);

                            result.lexstate.cur_fwd = text_content_token.lexstate.cur_fwd;
                            result.dat.text_content = list;
                            break;
                        }
                    }
                }

                // CODE CONTENT
                {
                    const char *code = "verb";
                    size_t code_len = 4;
                    int id_is_equal_to_code = 0;
                    size_t id_size = result.lexstate.cur_fwd - result.lexstate.cur_back;

                    if (id_size == code_len) {
                        const char *content = result.lexstate.content;
                        id_is_equal_to_code = 
                            (content[result.lexstate.cur_back + 0] == code[0]) &&
                            (content[result.lexstate.cur_back + 1] == code[1]) &&
                            (content[result.lexstate.cur_back + 2] == code[2]) &&
                            (content[result.lexstate.cur_back + 3] == code[3]);

                        if (id_is_equal_to_code) {
                            result.type = VD_DSPC__TOKEN_TYPE_CODE_CONTENT;
                            vd_dspc__skip_whitespace_all(&result.lexstate);

                            // Check for ```
                            {
                                int count = 0;

                                while (count < 3) {
                                    if (vd_dspc__lex_char(&result.lexstate) != '`') {
                                        VD_DSPC_LOG("Error: %llu:%llu Expected \'```\' after \'code\' section.",
                                                    result.lexstate.line + 1, result.lexstate.column + 1);
                                        result.type = VD_DSPC__TOKEN_TYPE_UNKNOWN;
                                        return result;
                                    }
                                    vd_dspc__lex_nextn(&result.lexstate, 1);
                                    count++;
                                }
                            }

                            vd_dspc__skip_newlines(&result.lexstate);

                            VdDspc__Token code_content_token = result;
                            code_content_token.lexstate.cur_back = code_content_token.lexstate.cur_fwd;
                            int text_parsing_ended = 0;

                            while (!text_parsing_ended) {

                                c = vd_dspc__lex_char(&code_content_token.lexstate);

                                if (c == '`') {
                                    int count = 0;

                                    while (count < 3) {
                                        if (vd_dspc__lex_char(&code_content_token.lexstate) != '`') {
                                            break;
                                        }
                                        vd_dspc__lex_nextn(&code_content_token.lexstate, 1);
                                        count++;
                                    }

                                    if (count == 3) {
                                        text_parsing_ended = 1;
                                        continue;
                                    }
                                } else {
                                    vd_dspc__lex_nextn(&code_content_token.lexstate, 1);
                                }

                            }

                            result.dat.verbatim_string.s = vd_dspc__token_string_begin(&code_content_token);
                            result.dat.verbatim_string.l = vd_dspc__token_string_size(&code_content_token) - 3;
                            result.lexstate.cur_fwd = code_content_token.lexstate.cur_fwd;
                            break;
                        }
                    }
                }
            }

        } break;
    }

    return result;
}

static void vd_dspc__lex_consume(VdDspc__Lex *lex, VdDspc__Token *token)
{
    *lex = token->lexstate;
    lex->cur_back = lex->cur_fwd;
}

static VdDspc__Token vd_dspc__token_new(VdDspc__Lex *lex, VdDspc__TokenType type)
{
    VdDspc__Token result;
    result.lexstate = *lex;
    result.type = type;
    return result;
}

static void vd_dspc__token_log(VdDspc__Token *tok)
{
    VD_DSPC_LOG("%llu:%llu: %s", tok->lexstate.line + 1, tok->lexstate.column + 1,
                                 vd_dspc__token_type_to_string(tok->type));
    if (tok->type == VD_DSPC__TOKEN_TYPE_TEXT_CONTENT) {

        VdDspcStrNode *curr_node = tok->dat.text_content.first;

        while (curr_node) {
            VD_DSPC_LOG("%.*s", (int)curr_node->len, curr_node->str);
            curr_node = curr_node->next;
        }

    } else {
        VD_DSPC_LOG("%.*s", (int)vd_dspc__token_string_size(tok), vd_dspc__token_string_begin(tok));
    }
}

static int vd_dspc__skip_whitespace_all(VdDspc__Lex *lex)
{
    int c = vd_dspc__lex_char(lex);
    int any_whitespace_skipped = 0;
    while (vd_dspc__is_whitespace(c)) {
        if (!vd_dspc__lex_nextn(lex, 1)) {
            return 0;
        }
        any_whitespace_skipped = 1;
        c = vd_dspc__lex_char(lex);
    }

    if (any_whitespace_skipped) {
        if (vd_dspc__is_whitespace(c)) {
            vd_dspc__lex_nextn(lex, 1);
        }
        lex->cur_back = lex->cur_fwd;
    }

    return 1;
}

static int vd_dspc__skip_newlines(VdDspc__Lex *lex)
{
    int c = vd_dspc__lex_char(lex);
    int any_whitespace_skipped = 0;
    while (vd_dspc__is_newline(c)) {
        if (!vd_dspc__lex_nextn(lex, 1)) {
            return 0;
        }
        any_whitespace_skipped = 1;
        c = vd_dspc__lex_char(lex);
    }

    if (any_whitespace_skipped) {
        if (vd_dspc__is_newline(c)) {
            vd_dspc__lex_nextn(lex, 1);
        }
        lex->cur_back = lex->cur_fwd;
    }

    return 1;
}

static VdDspc__Lex vd_dspc__lex_save(VdDspc__Lex *lex)
{
    VdDspc__Lex result = *lex;
    return result;
}

static void vd_dspc__lex_load(VdDspc__Lex *target, VdDspc__Lex *src)
{
    *target = *src;
}

/* ----TREES IMPL---------------------------------------------------------------------------------------------------- */
static VdDspcTree *vd_dspc__push_tree(VdDspcDocument *doc)
{
    VdDspcTree *new_tree = vd_dspc__push(doc, sizeof(VdDspcTree));
    new_tree->next           = &doc->sentinel;
    new_tree->prev           = doc->sentinel.prev;
    doc->sentinel.prev->next = new_tree;
    doc->sentinel.prev       = new_tree;

    new_tree->global.next = &new_tree->global;
    new_tree->global.prev = &new_tree->global;
    new_tree->global.first = 0;
    new_tree->global.last = 0;
    new_tree->global.parent = 0;
    new_tree->global.section_id.s = ".global";
    new_tree->global.section_id.l = sizeof(".global") - 1;

    doc->curr_section = &new_tree->global;

    return new_tree;
}

static VdDspcStrNode *vd_dspc__push_str_node(VdDspcDocument *doc)
{
    VdDspcStrNode *new_node = vd_dspc__push(doc, sizeof(VdDspcStrNode));
    new_node->next = 0;
    new_node->prev = 0;
    return new_node;
}

/* ----SECTIONS IMPL------------------------------------------------------------------------------------------------- */
static VdDspcSection *vd_dspc__push_section(VdDspcDocument *doc, const char *id_begin_string, size_t id_len_string)
{
    VdDspcSection *parent = doc->curr_section;
    VdDspcSection *new_section = vd_dspc__push(doc, sizeof(VdDspcSection));

    new_section->first = 0;
    new_section->last = 0;
    new_section->next = 0;
    new_section->prev = 0;
    new_section->parent = parent;
    new_section->section_id.s = id_begin_string;
    new_section->section_id.l = id_len_string;
    new_section->tag_sentinel.next = &new_section->tag_sentinel;
    new_section->tag_sentinel.prev = &new_section->tag_sentinel;
    new_section->text_content.first = 0;
    new_section->text_content.last = 0;

    if (!parent->first) {
        // First child of this parent
        parent->first = new_section;
        parent->last = new_section;
    } else {
        // Append to end of children
        new_section->prev = parent->last;
        parent->last->next = new_section;
        parent->last = new_section;
    }

    return new_section;
}

/* ----TAGS---------------------------------------------------------------------------------------------------------- */
static VdDspcTag *vd_dspc__push_tag(VdDspcDocument *doc, VdDspcSection *section,
                                    VdDspcStr tag_name, VdDspcStr tag_value)
{
    VdDspcTag *new_tag = vd_dspc__push(doc, sizeof(VdDspcTag));

    new_tag->prev = section->tag_sentinel.prev;
    section->tag_sentinel.prev->next = new_tag;
    section->tag_sentinel.prev = new_tag;

    new_tag->name = tag_name;
    new_tag->value = tag_value;
    return new_tag;
}

/* ----STRLIST IMPL-------------------------------------------------------------------------------------------------- */
static void vd_dspc__str_list_push_token(VdDspcDocument *doc, VdDspcStrList *list, VdDspc__Token *tok)
{
    VdDspcStrNode *node = vd_dspc__push_str_node(doc);
    if (list->first == 0) {
        list->first = node;
        list->last = node;
    } else {
        node->prev = list->last;
        list->last->next = node;
        list->last = node;
    }

    node->str = tok->lexstate.content + tok->lexstate.cur_back;
    node->len = tok->lexstate.cur_fwd - tok->lexstate.cur_back;
}

static void vd_dspc__str_list_push_verbatim(VdDspcDocument *doc, VdDspcStrList *list, VdDspc__Token *tok)
{
    VdDspcStrNode *node = vd_dspc__push_str_node(doc);
    if (list->first == 0) {
        list->first = node;
        list->last = node;
    } else {
        node->prev = list->last;
        list->last->next = node;
        list->last = node;
    }

    node->str = tok->dat.verbatim_string.s;
    node->len = tok->dat.verbatim_string.l;
}

static VdDspcTree *vd_dspc__tree(VdDspcDocument *doc)
{
    return (doc->sentinel.prev == &doc->sentinel) ? 0 : doc->sentinel.prev;
}

/* ----ARENAS IMPL--------------------------------------------------------------------------------------------------- */
static void *vd_dspc__push(VdDspcDocument *doc, size_t size)
{
    int default_arena_count = 4;
    int default_arena_size = 1024 * 1024;

    VD_DSPC_ASSERT(size < default_arena_size);

    // Allocate arenas array if not allocated
    if (doc->arenas == 0) {
        doc->arenas = (VdDspc__Arena*)doc->alloc(0, 0, sizeof(VdDspc__Arena) * default_arena_count);
        doc->arenas_cap = default_arena_count;

        doc->arenas[0].buf         = (unsigned char*)doc->alloc(0, 0, default_arena_size);
        doc->arenas[0].buf_len     = default_arena_size;
        doc->arenas[0].curr_offset = 0;
        doc->arenas_len = 1;
    }

    int curr_arena = doc->arenas_len - 1;

    if ((doc->arenas[curr_arena].curr_offset + size) > doc->arenas[curr_arena].buf_len) {

        if (doc->arenas_len == doc->arenas_cap) {
            // resize arena ptr array
            doc->arenas = doc->alloc(doc->arenas, (doc->arenas_cap)     * sizeof(VdDspc__Arena),
                                                  (doc->arenas_cap * 2) * sizeof(VdDspc__Arena));
            doc->arenas_cap *= 2;
        }

        curr_arena = doc->arenas_len++;
        doc->arenas[curr_arena].buf         = (unsigned char*)doc->alloc(0, 0, default_arena_size);
        doc->arenas[curr_arena].buf_len     = default_arena_size;
        doc->arenas[curr_arena].curr_offset = 0;
    }

    void *result = (void*)&doc->arenas[curr_arena].buf[doc->arenas[curr_arena].curr_offset];
    doc->arenas[curr_arena].curr_offset += size;
    return result;
}

/* ----UTILITIES IMPL------------------------------------------------------------------------------------------------ */
static int vd_dspc__is_alpha(int c)
{
    return ((c >= 'a') && (c <= 'z')) ||
           ((c >= 'A') && (c <= 'Z'));
}

static int vd_dspc__is_num(int c)
{
    return (c >= '0') && (c <= '9');    
}

static int vd_dspc__is_whitespace(int c)
{
    return (c == '\n' || c == '\r' || c == ' ' || c == '\t');
}

static int vd_dspc__is_newline(int c)
{
    return (c == '\n' || c == '\r');    
}

static int vd_dspc__is_consumable_whitespace(int c)
{
    return vd_dspc__is_newline(c) || c == '\t';
}

static int vd_dspc__is_valid_id_continue(int c) {
    return vd_dspc__is_alpha(c) || vd_dspc__is_num(c) || c == '-' || c == '_';
}

static size_t vd_dspc__token_string_size(VdDspc__Token *tok)
{
    assert(tok->lexstate.cur_fwd >= tok->lexstate.cur_back);
    size_t sz = tok->lexstate.cur_fwd - tok->lexstate.cur_back;
    if (sz == 0) sz = 1;
    return sz;
}

static char *vd_dspc__token_string_begin(VdDspc__Token *tok)
{
    return (char*)tok->lexstate.content + tok->lexstate.cur_back;
}

static char *vd_dspc__token_type_to_string(VdDspc__TokenType ttype)
{
    switch (ttype) {
        case VD_DSPC__TOKEN_TYPE_UNKNOWN:       return "unknown";
        case VD_DSPC__TOKEN_TYPE_AT:            return "at";
        case VD_DSPC__TOKEN_TYPE_ID:            return "id";
        case VD_DSPC__TOKEN_TYPE_END:           return "end";
        case VD_DSPC__TOKEN_TYPE_TAGS_BEGIN:    return "tags_begin";
        case VD_DSPC__TOKEN_TYPE_TAGS_END:      return "tags_end";
        case VD_DSPC__TOKEN_TYPE_QUOTED_STRING: return "\"quoted\"";
        case VD_DSPC__TOKEN_TYPE_SECTION_BEGIN: return "section_begin";
        case VD_DSPC__TOKEN_TYPE_SECTION_END:   return "section_end";
        case VD_DSPC__TOKEN_TYPE_TEXT_CONTENT:  return "text_content";
        case VD_DSPC__TOKEN_TYPE_PARAGRAPH:     return "paragraph";
        case VD_DSPC__TOKEN_TYPE_ASSIGNMENT:    return "=";
        case VD_DSPC__TOKEN_TYPE_CODE_CONTENT:  return "code";
        default: break;
    }

    return "<?>";
}

static VD_DSPC_PROC_LOG(vd_dspc__log_stdout)
{
    size_t max_line = 100;
    size_t curr_line = 0;

    size_t len = strlen(message);

    for (size_t i = 0; i < len; ++i) {
        curr_line++;

        char c = message[i];

        switch (c) {
            case '\n': printf("\\n"); break;
            case '\r': printf("\\r"); break;
            case '\t': printf("\\t"); break;
            default: {
                fputc(message[i], stdout);
            } break;
        }

        if (curr_line >= max_line) {
            curr_line = 0;
            printf("\n");
        }
    }
    printf("\n");
}

#endif // VD_DSPC_IMPL