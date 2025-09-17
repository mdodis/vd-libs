/**
 * \internal
 * vd_inix.h - An extended INI file format parser
 * 
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
 */
#ifndef VD_INIX_H
#define VD_INIX_H
#define VD_INIX_VERSION_MAJOR    0
#define VD_INIX_VERSION_MINOR    0
#define VD_INIX_VERSION_PATCH    1
#define VD_INIX_VERSION          ((VD_INIX_VERSION_MAJOR << 16) | (VD_INIX_VERSION_MINOR << 8) | (VD_INIX_VERSION_PATCH))

#ifndef VD_H
#error "vd_inix.h requires vd.h"
#endif // !VD_H

typedef struct VdInixSection VdInixSection;
typedef struct VdInixEntry   VdInixEntry;

typedef enum {
    VD_INIX_ENTRY_TYPE_ENUMERATION,
    VD_INIX_ENTRY_TYPE_INTEGER,
    VD_INIX_ENTRY_TYPE_RAW_STRING,
} VdInixEntryType;

struct VdInixEntry {
    VdInixEntryType type;
    VdStr           name;
    VdInixEntry     *next;

    union {
        Vdu64 enumeration;
        Vdu64 integer; 
        VdStr raw_string;
    } val;
};

struct VdInixSection {
    VdStr         name;
    VdInixEntry   *entry;
    VdInixSection *next;
};

typedef struct __VD_Inix_Result {
    Vdu32         line;
    Vdu32         column; 
    VdInixSection *global_section;
} VdInixResult;

typedef struct __VD_Inix_Mapping {
    VdStr key;
    Vdu64 value;
} VdInixMapping;

typedef struct __VD_Inix_InixParseInfo {
    VdStr         contents;
    Vdusize       num_mappings;
    VdInixMapping *mappings;
} VdInixParseInfo;

VdInixResult vd_inix_parse(VdArena *arena, VdInixParseInfo *info);

#if VD_MACRO_ABBREVIATIONS
#define INIX_ENTRY_TYPE_ENUMERATION VD_INIX_ENTRY_TYPE_ENUMERATION
#define INIX_ENTRY_TYPE_INTEGER     VD_INIX_ENTRY_TYPE_INTEGER
#define INIX_ENTRY_TYPE_RAW_STRING  VD_INIX_ENTRY_TYPE_RAW_STRING
#define InixResult                  VdInixResult
#define InixSection                 VdInixSection
#define InixEntryType               VdInixEntryType
#define InixEntry                   VdInixEntry
#define InixMapping                 VdInixMapping
#define InixParseInfo               VdInixParseInfo
#define inix_parse                  vd_inix_parse
#endif 

#endif // !VD_INIX_H

#ifdef VD_INIX_IMPL

typedef enum {
    VD__INIX_LEX_TOKEN_INVALID = 0,
    VD__INIX_LEX_TOKEN_BEGIN_SECTION = '[',
    VD__INIX_LEX_TOKEN_END_SECTION   = ']',
    VD__INIX_LEX_TOKEN_ASSIGNMENT    = '=',
    VD__INIX_LEX_TOKEN_END_STATEMENT = '\n',
    VD__INIX_LEX_TOKEN_ENUMERATION,
    VD__INIX_LEX_TOKEN_INTEGER,
    VD__INIX_LEX_TOKEN_RAW_STRING,
    VD__INIX_LEX_TOKEN_STRING,
    VD__INIX_LEX_TOKEN_END           = -1,
} Vd__InixLexTokenType;

Vdcstr vd__inix_lex_token_type_str(Vd__InixLexTokenType t) {
    switch (t) {
        case VD__INIX_LEX_TOKEN_INVALID:       return "INIX_LEX_TOKEN_INVALID"; break;
        case VD__INIX_LEX_TOKEN_BEGIN_SECTION: return "INIX_LEX_TOKEN_BEGIN_SECTION"; break;
        case VD__INIX_LEX_TOKEN_END_SECTION:   return "INIX_LEX_TOKEN_END_SECTION"; break;
        case VD__INIX_LEX_TOKEN_ASSIGNMENT:    return "INIX_LEX_TOKEN_ASSIGNMENT"; break;
        case VD__INIX_LEX_TOKEN_END_STATEMENT: return "INIX_LEX_TOKEN_END_STATEMENT"; break;
        case VD__INIX_LEX_TOKEN_ENUMERATION:   return "INIX_LEX_TOKEN_ENUMERATION"; break;
        case VD__INIX_LEX_TOKEN_INTEGER:       return "INIX_LEX_TOKEN_INTEGER"; break;
        case VD__INIX_LEX_TOKEN_RAW_STRING:    return "INIX_LEX_TOKEN_RAW_STRING"; break;
        case VD__INIX_LEX_TOKEN_END:           return "INIX_LEX_TOKEN_END"; break;
        default:                                return "UNDEFINED_TOKEN";
    }    
}

typedef struct {
    Vdu64 cursor;
    Vdu64 cursor_from;
    int line;
    int column;
} Vd__InixLexState;

typedef struct {
    Vd__InixLexState       lexstate;
    Vd__InixLexTokenType   type;
    VdStr                 raw_string;
} Vd__InixLexToken;

typedef struct {
    VdStr           contents;
    Vd__InixLexState lex;
    Vdi32           token_count;
    Vd__InixLexToken next_token;
    Vdb32           debug_log_tokens;

    VdArena         *arena;
    VdInixSection   *global_section;
    VdInixSection   *curr_section;
    VdInixEntry     *curr_entry;

    Vdusize         num_mappings;
    VdInixMapping   *mappings;

} Vd__InixContext;

Vdb32 vd__inix_at_end(Vd__InixContext *c) {
    return c->lex.cursor >= c->contents.len;
}
    
Vdb32 vd__inix_advance(Vd__InixContext *c) {
    if (vd__inix_at_end(c)) {
        return VD_FALSE;
    }

    if (c->contents.s[c->lex.cursor] == '\n') {
        c->lex.line++;
        c->lex.column = 0;
    } else {
        c->lex.column++;
    }

    c->lex.cursor++;

    return VD_TRUE;
}

int vd__inix_char(Vd__InixContext *c) {
    if (vd__inix_at_end(c)) {
        return -1;
    }

    return c->contents.s[c->lex.cursor];
}

Vd__InixLexState vd__inix_save_state(Vd__InixContext *c) {
    return (Vd__InixLexState) { c->lex.cursor, c->lex.cursor_from, c->lex.line, c->lex.column };
}

void vd__inix_load_state(Vd__InixContext *c, Vd__InixLexState l) {
    c->lex.cursor = l.cursor;
    c->lex.cursor_from = l.cursor_from;
    c->lex.line = l.line;
    c->lex.column = l.column;
}

Vdb32 vd__inix_char_n(Vd__InixContext *c, VdStr s) {
    Vd__InixLexState state = vd__inix_save_state(c);
    Vdu64 i = 0;
    int h = vd__inix_char(c);

    while ((i < s.len) && (s.s[i] == h)) {
        if (!vd__inix_advance(c)) {
            break;
        }
        h = vd__inix_char(c);
        i++;
    }

    if (i == s.len) {
        return VD_TRUE;
    } else {
        vd__inix_load_state(c, state);
        return VD_FALSE;
    }
}

void vd__inix_advance_n(Vd__InixContext *c, Vdusize n) {
    for (Vdu64 i = 0; i < n; ++i) {
        vd__inix_advance(c);
    }
}

void vd__inix_eat_spaces(Vd__InixContext *c) {
    int i = vd__inix_char(c);
    while (i == ' ' || i == '\t' || i == '\r') {
        vd__inix_advance(c);
        i = vd__inix_char(c);
    }
}

void vd__inix_eat_comments(Vd__InixContext *context) {
    if (!vd__inix_char_n(context, VD_LIT("/*"))) {
        return;
    }

    while (!vd__inix_char_n(context, VD_LIT("*/"))) {
        vd__inix_advance(context); 
    }
}


void vd__inix_advance_token(Vd__InixContext *c, Vd__InixLexToken token) {
    vd__inix_advance_n(c, (token.lexstate.cursor - token.lexstate.cursor_from));
}

Vdb32 vd__inix_try_parse_integer(Vd__InixContext *context, Vdu64 *cursor_begin)
{
    Vd__InixLexState lexstate = vd__inix_save_state(context);

    int num_digits = 0;
    int c = vd__inix_char(context);
    *cursor_begin = context->lex.cursor;

    while (vd_is_ascii_digit(c)) {
        vd__inix_advance(context);
        c = vd__inix_char(context);
        num_digits++;
    }

    if (num_digits > 0) {

        return VD_TRUE;
    } else {
        vd__inix_load_state(context, lexstate);
        return VD_FALSE;
    }
}

Vdb32 vd__inix_try_parse_simple_string(Vd__InixContext *context, Vdu64 *cursor_begin, Vdu64 *cursor_end)
{
    *cursor_begin = context->lex.cursor;
    if (!vd__inix_char_n(context, VD_LIT("\""))) {
        return VD_FALSE;
    }

    vd__inix_advance(context);

    while (!vd__inix_char_n(context, VD_LIT("\""))) {
        if (!vd__inix_advance(context)) {
            return VD_FALSE;
        }
    }

    *cursor_end = context->lex.cursor;
    return VD_TRUE;
}

Vdb32 vd__inix_try_parse_raw_string(Vd__InixContext *context, Vdu64 *cursor_begin, Vdu64 *cursor_end)
{
    *cursor_begin = context->lex.cursor;
    if (!vd__inix_char_n(context, VD_LIT("@>"))) {
        return VD_FALSE;
    }

    while (!vd__inix_char_n(context, VD_LIT("@<"))) {
        if (!vd__inix_advance(context)) {
            return VD_FALSE;
        }
    }

    *cursor_end = context->lex.cursor;
    return VD_TRUE;
}

Vdb32 vd__inix_try_parse_cdecl(Vd__InixContext *context, Vdu64 *cursor_begin)
{
    int c = vd__inix_char(context);
    *cursor_begin = context->lex.cursor;

    if (!vd_is_cdecl_start(c)) {
        return VD_FALSE;
    }

    vd__inix_advance(context);
    c = vd__inix_char(context);

    while (vd_is_cdecl_continue(c)) {
        vd__inix_advance(context);
        c = vd__inix_char(context);
    }

    return VD_TRUE;
}

Vd__InixLexToken vd__inix_peek_token(Vd__InixContext *context) {
    vd__inix_eat_spaces(context);
    vd__inix_eat_comments(context);
    vd__inix_eat_spaces(context);

    Vd__InixLexState save = vd__inix_save_state(context);

    Vd__InixLexToken result;
    result.lexstate = context->lex;
    result.lexstate.cursor_from = result.lexstate.cursor;
    result.lexstate.cursor += 1;
    int c = vd__inix_char(context);

    switch (c) {
        case -1:   result.type = VD__INIX_LEX_TOKEN_END;           break;
        case '\0': result.type = VD__INIX_LEX_TOKEN_INVALID;       break;
        case '[':  result.type = VD__INIX_LEX_TOKEN_BEGIN_SECTION; break;
        case ']':  result.type = VD__INIX_LEX_TOKEN_END_SECTION;   break;
        case '=':  result.type = VD__INIX_LEX_TOKEN_ASSIGNMENT;    break;
        case '\n': result.type = VD__INIX_LEX_TOKEN_END_STATEMENT; break;
        case '\"': {
            Vdu64 cursor_begin, cursor_end;
            if (vd__inix_try_parse_simple_string(context, &cursor_begin, &cursor_end)) {
                result.type = VD__INIX_LEX_TOKEN_STRING;
                result.lexstate = context->lex;
                result.lexstate.cursor_from = cursor_begin;
                result.lexstate.cursor = cursor_end;
                result.raw_string = (VdStr) {&context->contents.s[cursor_begin + 1], cursor_end - cursor_begin - 2 };
            } else {
                result.type = VD__INIX_LEX_TOKEN_INVALID;
            }
        } break;
        case '@': {
            Vdu64 cursor_begin, cursor_end;
            if (vd__inix_try_parse_raw_string(context, &cursor_begin, &cursor_end)) {
                result.type = VD__INIX_LEX_TOKEN_RAW_STRING;
                result.lexstate = context->lex;
                result.lexstate.cursor_from = cursor_begin;
                result.lexstate.cursor = cursor_end;
                result.raw_string = (VdStr) {&context->contents.s[cursor_begin + 2], cursor_end - cursor_begin - 4 };
            } else {
                result.type = VD__INIX_LEX_TOKEN_INVALID;
            }
        } break;
        default: {
            Vdu64 cursor_begin;
            if (vd__inix_try_parse_integer(context, &cursor_begin)) {
                result.type = VD__INIX_LEX_TOKEN_INTEGER;
                result.lexstate.cursor = context->lex.cursor;
                result.lexstate.cursor_from = cursor_begin;
            } else if (vd__inix_try_parse_cdecl(context, &cursor_begin)) {
                result.type = VD__INIX_LEX_TOKEN_ENUMERATION;
                result.lexstate.cursor = context->lex.cursor;
                result.lexstate.cursor_from = cursor_begin;
            } else {
                result.type = VD__INIX_LEX_TOKEN_INVALID;
            }
        } break;
    }

    vd__inix_load_state(context, save);
    return result;
}

void vd__inix_consume_token(Vd__InixContext *context, Vd__InixLexToken token) {
    if (context->debug_log_tokens) {
        VdStr s = {&context->contents.s[token.lexstate.cursor_from] , token.lexstate.cursor - token.lexstate.cursor_from};
        VdStr tstr = {0};
        
        switch (token.type) {
            case VD__INIX_LEX_TOKEN_INVALID:       tstr = VD_LIT("INIX_LEX_TOKEN_INVALID"); break;
            case VD__INIX_LEX_TOKEN_BEGIN_SECTION: tstr = VD_LIT("INIX_LEX_TOKEN_BEGIN_SECTION"); break;
            case VD__INIX_LEX_TOKEN_END_SECTION:   tstr = VD_LIT("INIX_LEX_TOKEN_END_SECTION"); break;
            case VD__INIX_LEX_TOKEN_ASSIGNMENT:    tstr = VD_LIT("INIX_LEX_TOKEN_ASSIGNMENT"); break;
            case VD__INIX_LEX_TOKEN_END_STATEMENT: tstr = VD_LIT("INIX_LEX_TOKEN_END_STATEMENT"); break;
            case VD__INIX_LEX_TOKEN_ENUMERATION:   tstr = VD_LIT("INIX_LEX_TOKEN_ENUMERATION"); break;
            case VD__INIX_LEX_TOKEN_INTEGER:       tstr = VD_LIT("INIX_LEX_TOKEN_INTEGER"); break;
            case VD__INIX_LEX_TOKEN_RAW_STRING:    tstr = VD_LIT("INIX_LEX_TOKEN_RAW_STRING"); break;
            case VD__INIX_LEX_TOKEN_STRING:        tstr = VD_LIT("INIX_LEX_TOKEN_STRING"); break;
            case VD__INIX_LEX_TOKEN_END:           tstr = VD_LIT("INIX_LEX_TOKEN_END"); break;
            default: break;

        }
        if (token.type == VD__INIX_LEX_TOKEN_RAW_STRING) {
            VD_LOGF("Consume %2d:%2d :: %.*s: {{%.*s}}", 1 + token.lexstate.line, token.lexstate.column, VD_STR_EXPAND(tstr), VD_STR_EXPAND(token.raw_string));
        } else if (token.type == VD__INIX_LEX_TOKEN_STRING) {
            VD_LOGF("Consume %2d:%2d :: %.*s: {{%.*s}}", 1 + token.lexstate.line, token.lexstate.column, VD_STR_EXPAND(tstr), VD_STR_EXPAND(token.raw_string));
        } else if (token.type != VD__INIX_LEX_TOKEN_END) {
            VD_LOGF("Consume %2d:%2d :: %.*s: %.*s", 1 + token.lexstate.line, token.lexstate.column, VD_STR_EXPAND(tstr), VD_STR_EXPAND(s));
        } else {
            VD_LOGF("Consume %.*s", VD_STR_EXPAND(tstr));
        }
    }

    vd__inix_advance_token(context, token);
}


Vdb32 vd__inix_expect_type(Vd__InixContext *context, Vd__InixLexTokenType type, Vd__InixLexToken *tok)
{
    Vd__InixLexToken token = vd__inix_peek_token(context);
    if (token.type != type) {
        return VD_FALSE;
    }

    if (tok) {
        *tok = token;
    }

    return VD_TRUE;
}

VdInixSection *vd__inix_push_section(Vd__InixContext *context) {
    VdInixSection *result = vd_arena_alloc(context->arena, sizeof(VdInixSection));
    context->curr_section->next = result;
    context->curr_section = result;
    context->curr_entry = 0;
    return result;
}

VdInixEntry *vd__inix_push_entry(Vd__InixContext *context, VdInixEntryType type) {
    VdInixEntry *result = vd_arena_alloc(context->arena, sizeof(VdInixEntry));

    result->type = type;
    if (context->curr_entry) {
        context->curr_entry->next = result;
        context->curr_entry = result;
    } else {
        context->curr_section->entry = result;
        context->curr_entry = result;
    }

    return result;
}

Vdb32 vd__inix_parse_section(Vd__InixContext *context)
{
    Vd__InixLexToken first = vd__inix_peek_token(context);    
    if (first.type == VD__INIX_LEX_TOKEN_BEGIN_SECTION) {
        vd__inix_consume_token(context, first);

        Vd__InixLexToken enumeration;
        if (!vd__inix_expect_type(context, VD__INIX_LEX_TOKEN_ENUMERATION, &enumeration)) {
            VD_ERRF("%d:%d: Expected INIX_LEX_TOKEN_ENUMERATION, instead got: %s",
                enumeration.lexstate.line, enumeration.lexstate.column,
                vd__inix_lex_token_type_str(enumeration.type));
            return VD_FALSE;
        }
        vd__inix_consume_token(context, enumeration);

        Vd__InixLexToken end_section;
        if (!vd__inix_expect_type(context, VD__INIX_LEX_TOKEN_END_SECTION, &end_section)) {

            VD_ERRF("%d:%d: Expected INIX_LEX_TOKEN_END_SECTION, instead got: %s",
                enumeration.lexstate.line, enumeration.lexstate.column,
                vd__inix_lex_token_type_str(enumeration.type));

            return VD_FALSE;
        }
        vd__inix_consume_token(context, end_section);

        VdStr section_name = {
            &context->contents.s[enumeration.lexstate.cursor_from],
            enumeration.lexstate.cursor - enumeration.lexstate.cursor_from,
        };

        VdInixSection *section = vd__inix_push_section(context);
        section->name = section_name;

        // LOGF("Section: [%.*s]", VD_STR_EXPAND(section_name));
        return VD_TRUE;
    }

    return VD_FALSE;
}

static Vdb32 vd__inix_get_enumeration(Vd__InixContext *context, VdStr name, Vdu64 *out_enumeration)
{
    for (Vdusize i = 0; i < context->num_mappings; ++i) {
        if (vd_str_eq(name, context->mappings[i].key)) {
            *out_enumeration = context->mappings[i].value;
            return VD_TRUE;
        }
    }

    return VD_FALSE;
}

Vdb32 vd__inix_parse_expression(Vd__InixContext *context)
{
    Vd__InixLexToken first = vd__inix_peek_token(context);    
    if (first.type == VD__INIX_LEX_TOKEN_ENUMERATION) {
        vd__inix_consume_token(context, first);

        VdStr first_string = {
            &context->contents.s[first.lexstate.cursor_from],
            first.lexstate.cursor - first.lexstate.cursor_from,
        };

        Vd__InixLexToken assignment;
        if (!vd__inix_expect_type(context, VD__INIX_LEX_TOKEN_ASSIGNMENT, &assignment)) {
            VD_ERRF("%d:%d: Expected INIX_LEX_TOKEN_ASSIGNMENT, instead got: %s",
                assignment.lexstate.line, assignment.lexstate.column,
                vd__inix_lex_token_type_str(assignment.type));

            return VD_FALSE;
        }
        vd__inix_consume_token(context, assignment);


        Vd__InixLexToken val = vd__inix_peek_token(context);

        if (val.type == VD__INIX_LEX_TOKEN_STRING) {
            vd__inix_consume_token(context, val);

            VdStr raw_string = val.raw_string;

            VdInixEntry *entry = vd__inix_push_entry(context, VD_INIX_ENTRY_TYPE_RAW_STRING);
            entry->name = first_string;
            entry->val.raw_string = raw_string;

            // LOGF("String assignment: %.*s = %.*s", VD_STR_EXPAND(first_string), VD_STR_EXPAND(raw_string));
            return VD_TRUE;

        } else if (val.type == VD__INIX_LEX_TOKEN_RAW_STRING) {
            vd__inix_consume_token(context, val);

            VdStr raw_string = val.raw_string;

            VdInixEntry *entry = vd__inix_push_entry(context, VD_INIX_ENTRY_TYPE_RAW_STRING);
            entry->name = first_string;
            entry->val.raw_string = raw_string;

            // LOGF("Raw string assignment: %.*s = %.*s", VD_STR_EXPAND(first_string), VD_STR_EXPAND(raw_string));
            return VD_TRUE;

        } else if (val.type == VD__INIX_LEX_TOKEN_ENUMERATION) {
            vd__inix_consume_token(context, val);

            VdInixEntry *entry = vd__inix_push_entry(context, VD_INIX_ENTRY_TYPE_ENUMERATION);
            entry->name = first_string;

            VdStr val_string = {
                &context->contents.s[val.lexstate.cursor_from],
                val.lexstate.cursor - val.lexstate.cursor_from,
            };

            if (!vd__inix_get_enumeration(context, val_string, &entry->val.enumeration)) {
                VD_ERRF("%d:%d: Enumeration '%.*s' is not a valid enumeration",
                    val.lexstate.line, val.lexstate.column,
                    VD_STR_EXPAND(entry->name));
                return VD_FALSE;
            }

            // LOGF("Enumeration assignment: %.*s = %.*s", VD_STR_EXPAND(first_string), VD_STR_EXPAND(val_string));

            return VD_TRUE;
        } else if (val.type == VD__INIX_LEX_TOKEN_INTEGER) {
            vd__inix_consume_token(context, val);

            VdStr val_string = {
                &context->contents.s[val.lexstate.cursor_from],
                val.lexstate.cursor - val.lexstate.cursor_from,
            };

            Vdu64 number;
            if (!vd_parse_u64(val_string, &number)) {
                VD_ERRF("%d:%d: Failed to parse integer: %s",
                    val.lexstate.line, val.lexstate.column,
                    vd__inix_lex_token_type_str(val.type));
                return VD_FALSE;
            }

            VdInixEntry *entry = vd__inix_push_entry(context, VD_INIX_ENTRY_TYPE_INTEGER);
            entry->name = first_string;
            entry->val.integer = number;

            // LOGF("Integer assignment: %.*s = %llu", VD_STR_EXPAND(first_string), number);
            return VD_TRUE;
        } else {
            VD_ERRF("%d:%d: Expected INIX_LEX_TOKEN_ENUMERATION or INIX_LEX_TOKEN_INTEGER, instead got: %s",
                val.lexstate.line, val.lexstate.column,
                vd__inix_lex_token_type_str(val.type));
            return VD_FALSE;
        }
    }

    return VD_FALSE;
}

Vdb32 vd__inix_parse_statement(Vd__InixContext *context)
{
    Vd__InixLexToken first = vd__inix_peek_token(context);    

    VdStr first_string = {
        &context->contents.s[first.lexstate.cursor_from],
        first.lexstate.cursor - first.lexstate.cursor_from,
    };

    if (first.type == VD__INIX_LEX_TOKEN_END) {
        vd__inix_consume_token(context, first);
        return VD_TRUE;
    }

    if (first.type == VD__INIX_LEX_TOKEN_END_STATEMENT) {
        vd__inix_consume_token(context, first);
        return VD_TRUE;
    }

    if (vd__inix_parse_section(context)) {
        return VD_TRUE;
    }

    if (vd__inix_parse_expression(context)) {
        return VD_TRUE;
    }


    VD_ERRF("%d:%d: Expected INIX_LEX_TOKEN_BEGIN_SECTION or INIX_LEX_TOKEN_ENUMERATION, instead got: %s, %.*s",
        first.lexstate.line, first.lexstate.column,
        vd__inix_lex_token_type_str(first.type),
        VD_STR_EXPAND(first_string));

    return VD_FALSE;
}

static VdStr Vd__Inix_Global_Section_Name = VD_LIT_INLINE(".global");

VdInixResult vd_inix_parse(VdArena *arena, VdInixParseInfo *info)
{
    VdInixSection *global_section = vd_arena_alloc(arena, sizeof(VdInixSection));
    global_section->name = Vd__Inix_Global_Section_Name;

    Vd__InixContext context = {
        .contents            = info->contents,
        .arena               = arena,
        .global_section      = global_section,
        .curr_section        = global_section,
        .num_mappings        = info->num_mappings,
        .mappings            = info->mappings,
        .debug_log_tokens    = 0,
    };

    while (!vd__inix_at_end(&context)) {
        if (!vd__inix_parse_statement(&context)) {
            return (VdInixResult) { .global_section = 0, };
        }
    }

    return (VdInixResult) {
        .global_section = global_section,
    };
}

#endif // VD_INIX_IMPL
