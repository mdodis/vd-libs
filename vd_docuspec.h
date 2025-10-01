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
    int        l;
} VdDspcStr;

typedef struct VdDspcStrNode VdDspcStrNode; 
struct VdDspcStrNode {
    VdDspcStrNode *next;
    VdDspcStrNode *prev;
};

typedef struct VdDspcStrList {
    VdDspcStrNode sentinel;
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

    VdDspcTag     tag_sentinel;

    VdDspcStrList text_content;
};

typedef struct VdDspcTree VdDspcTree; 
struct VdDspcTree {
    VdDspcTree    *next;
    VdDspcTree    *prev;
    VdDspcSection global;
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
    VD_DSPC__TOKEN_TYPE_END        = -1,
} VdDspc__TokenType;

typedef struct VdDspc__Token {
    VdDspc__Lex       lexstate;
    VdDspc__TokenType type;
} VdDspc__Token;

typedef struct VdDspcDocument {
    VdDspcTree      sentinel;
    void*           (*alloc)(void *, size_t, size_t);

    int             arenas_len;
    int             arenas_cap;
    VdDspc__Arena   *arenas;

    VdDspc__Lex     lexstate;
} VdDspcDocument;

#define VD_DSPC_PROC_LOG(name) void name(const char *message)
typedef VD_DSPC_PROC_LOG(VdDspcProcLog);

VD_DSPC_API void vd_dspc_set_log_fn(VdDspcProcLog *log_fptr);
VD_DSPC_API void vd_dspc_document_init(VdDspcDocument *doc, void* (*alloc)(void *, size_t, size_t));
VD_DSPC_API int  vd_dspc_document_add(VdDspcDocument *doc, const char *content, size_t content_size);

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

#ifndef    VD_DSPC_ASSERT
#define    VD_DSPC_ASSERT(x)
#endif // !VD_DSPC_ASSERT

static VdDspcProcLog *Vd_Dspc__Log_Fn = NULL;

static char Vd_Dspc__Log_Buf[2048];

static VD_DSPC_PROC_LOG(vd_dspc__log_stdout);

#define VD_DSPC_LOG(fmt, ...) do {                           \
        snprintf(Vd_Dspc__Log_Buf, 2048, fmt, __VA_ARGS__);  \
        Vd_Dspc__Log_Fn(Vd_Dspc__Log_Buf);                   \
    } while(0)

/* ----UTILITIES----------------------------------------------------------------------------------------------------- */
static int    vd_dspc__is_alpha(int c);
static int    vd_dspc__is_num(int c);
static int    vd_dspc__is_whitespace(int c);
static int    vd_dspc__is_newline(int c);
static int    vd_dspc__is_consumable_whitespace(int c);
static int    vd_dspc__is_valid_id_continue(int c);
static char*  vd_dspc__token_type_to_string(VdDspc__TokenType ttype);
static size_t vd_dspc__token_string_size(VdDspc__Token *tok);
static char*  vd_dspc__token_string_begin(VdDspc__Token *tok);

/* ----ARENAS-------------------------------------------------------------------------------------------------------- */
static void *vd_dspc__push(VdDspcDocument *doc, size_t size);

/* ----TREES--------------------------------------------------------------------------------------------------------- */
static VdDspcTree *vd_dspc__push_tree(VdDspcDocument *doc);
static VdDspcTree *vd_dspc__tree(VdDspcDocument *doc);

/* ----LEXER--------------------------------------------------------------------------------------------------------- */
static int           vd_dspc__lex_end(VdDspc__Lex *lex);
static int           vd_dspc__lex_nextn(VdDspc__Lex *lex, size_t n);
static int           vd_dspc__lex_char(VdDspc__Lex *lex);
static VdDspc__Token vd_dspc__lex_token(VdDspc__Lex *lex);
static void          vd_dspc__lex_consume(VdDspc__Lex *lex, VdDspc__Token *token);
static VdDspc__Token vd_dspc__token_new(VdDspc__Lex *lex, VdDspc__TokenType type);
static void          vd_dspc__token_log(VdDspc__Token *tok);
static int           vd_dspc__skip_whitespace_all(VdDspc__Lex *lex);
static VdDspc__Lex   vd_dspc__lex_save(VdDspc__Lex *lex);
static void          vd_dspc__lex_load(VdDspc__Lex *target, VdDspc__Lex *src);

/* ----PARSER-------------------------------------------------------------------------------------------------------- */
static int vd_dspc__lang_section(VdDspcDocument *doc);

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

VD_DSPC_API int vd_dspc_document_add(VdDspcDocument *doc, const char *content, size_t content_size)
{
    vd_dspc__push_tree(doc);
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

/* ----PARSER IMPL--------------------------------------------------------------------------------------------------- */
static int vd_dspc__lang_section(VdDspcDocument *doc)
{
    VdDspc__Token token = vd_dspc__lex_token(&doc->lexstate);
    vd_dspc__token_log(&token);
    vd_dspc__lex_consume(&doc->lexstate, &token);

    return (token.type != VD_DSPC__TOKEN_TYPE_END) && (token.type != VD_DSPC__TOKEN_TYPE_UNKNOWN);
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

static VdDspc__Token vd_dspc__lex_token(VdDspc__Lex *lex)
{
    vd_dspc__skip_whitespace_all(lex);
    VdDspc__Lex saved_state = vd_dspc__lex_save(lex);

    VdDspc__Token result;
    result.type     = VD_DSPC__TOKEN_TYPE_UNKNOWN;
    result.lexstate = saved_state;

    int c = vd_dspc__lex_char(lex);

    switch (c) {
        case '@':             result.type = VD_DSPC__TOKEN_TYPE_AT;            vd_dspc__lex_nextn(&result.lexstate, 1); break;
        case '(':             result.type = VD_DSPC__TOKEN_TYPE_TAGS_BEGIN;    vd_dspc__lex_nextn(&result.lexstate, 1); break;
        case ')':             result.type = VD_DSPC__TOKEN_TYPE_TAGS_END;      vd_dspc__lex_nextn(&result.lexstate, 1); break;
        case '{':             result.type = VD_DSPC__TOKEN_TYPE_SECTION_BEGIN; vd_dspc__lex_nextn(&result.lexstate, 1); break;
        case '}':             result.type = VD_DSPC__TOKEN_TYPE_SECTION_END;   vd_dspc__lex_nextn(&result.lexstate, 1); break;
        case -1:              result.type = VD_DSPC__TOKEN_TYPE_END;                                                    break;
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
                        result.type = VD_DSPC__TOKEN_TYPE_UNKNOWN;
                        VD_DSPC_LOG("FOUND TEXT");
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
    VD_DSPC_LOG("%.*s", (int)vd_dspc__token_string_size(tok), vd_dspc__token_string_begin(tok));
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

    return new_tree;
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
    return vd_dspc__is_alpha(c) || vd_dspc__is_num(c);
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