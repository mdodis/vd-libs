/**
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

typedef struct VD(InixSection) VD(InixSection);
typedef struct VD(InixEntry)   VD(InixEntry);

typedef enum {
    VD_(INIX_ENTRY_TYPE_ENUMERATION),
    VD_(INIX_ENTRY_TYPE_INTEGER),
    VD_(INIX_ENTRY_TYPE_RAW_STRING),
} VD(InixEntryType);

struct VD(InixEntry) {
    VD(InixEntryType) type;
    VD(Str)           name;
    VD(InixEntry)     *next;

    union {
        VD(u64) enumeration;
        VD(u64) integer; 
        VD(Str) raw_string;
    } val;
};

struct VD(InixSection) {
    VD(Str)         name;
    VD(InixEntry)   *entry;
    VD(InixSection) *next;
};

typedef struct __VD_Inix_Result {
    VD(u32)         line;
    VD(u32)         column; 
    VD(InixSection) *global_section;
} VD(InixResult);

typedef struct __VD_Inix_Mapping {
    VD(Str) key;
    VD(u64) value;
} VD(InixMapping);

typedef struct __VD_Inix_InixParseInfo {
    VD(Str)         contents;
    VD(usize)       num_mappings;
    VD(InixMapping) *mappings;
} VD(InixParseInfo);

VD(InixResult) VDF(inix_parse)(VD(Arena) *arena, VD(InixParseInfo) *info);

#endif // !VD_INIX_H

#ifdef VD_INIX_IMPL

typedef enum {
    VDI(INIX_LEX_TOKEN_INVALID) = 0,
    VDI(INIX_LEX_TOKEN_BEGIN_SECTION) = '[',
    VDI(INIX_LEX_TOKEN_END_SECTION)   = ']',
    VDI(INIX_LEX_TOKEN_ASSIGNMENT)    = '=',
    VDI(INIX_LEX_TOKEN_END_STATEMENT) = '\n',
    VDI(INIX_LEX_TOKEN_ENUMERATION),
    VDI(INIX_LEX_TOKEN_INTEGER),
    VDI(INIX_LEX_TOKEN_RAW_STRING),
    VDI(INIX_LEX_TOKEN_STRING),
    VDI(INIX_LEX_TOKEN_END)           = -1,
} VDI(InixLexTokenType);

VD(cstr) VDI(inix_lex_token_type_str)(VDI(InixLexTokenType) t) {
    switch (t) {
        case VDI(INIX_LEX_TOKEN_INVALID):       return "INIX_LEX_TOKEN_INVALID"; break;
        case VDI(INIX_LEX_TOKEN_BEGIN_SECTION): return "INIX_LEX_TOKEN_BEGIN_SECTION"; break;
        case VDI(INIX_LEX_TOKEN_END_SECTION):   return "INIX_LEX_TOKEN_END_SECTION"; break;
        case VDI(INIX_LEX_TOKEN_ASSIGNMENT):    return "INIX_LEX_TOKEN_ASSIGNMENT"; break;
        case VDI(INIX_LEX_TOKEN_END_STATEMENT): return "INIX_LEX_TOKEN_END_STATEMENT"; break;
        case VDI(INIX_LEX_TOKEN_ENUMERATION):   return "INIX_LEX_TOKEN_ENUMERATION"; break;
        case VDI(INIX_LEX_TOKEN_INTEGER):       return "INIX_LEX_TOKEN_INTEGER"; break;
        case VDI(INIX_LEX_TOKEN_RAW_STRING):    return "INIX_LEX_TOKEN_RAW_STRING"; break;
        case VDI(INIX_LEX_TOKEN_END):           return "INIX_LEX_TOKEN_END"; break;
        default:                                return "UNDEFINED_TOKEN";
    }    
}

typedef struct {
    VD(u64) cursor;
    VD(u64) cursor_from;
    int line;
    int column;
} VDI(InixLexState);

typedef struct {
    VDI(InixLexState)       lexstate;
    VDI(InixLexTokenType)   type;
    VD(Str)                 raw_string;
} VDI(InixLexToken);

typedef struct {
    VD(Str)           contents;
    VDI(InixLexState) lex;
    VD(i32)           token_count;
    VDI(InixLexToken) next_token;
    VD(b32)           debug_log_tokens;

    VD(Arena)         *arena;
    VD(InixSection)   *global_section;
    VD(InixSection)   *curr_section;
    VD(InixEntry)     *curr_entry;

    VD(usize)         num_mappings;
    VD(InixMapping)   *mappings;

} VDI(InixContext);

VD(b32) VDI(inix_at_end)(VDI(InixContext) *c) {
    return c->lex.cursor >= c->contents.len;
}
    
VD(b32) VDI(inix_advance)(VDI(InixContext) *c) {
    if (VDI(inix_at_end)(c)) {
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

int VDI(inix_char)(VDI(InixContext) *c) {
    if (VDI(inix_at_end)(c)) {
        return -1;
    }

    return c->contents.s[c->lex.cursor];
}

VDI(InixLexState) VDI(inix_save_state)(VDI(InixContext) *c) {
    return (VDI(InixLexState)) { c->lex.cursor, c->lex.cursor_from, c->lex.line, c->lex.column };
}

void VDI(inix_load_state)(VDI(InixContext) *c, VDI(InixLexState) l) {
    c->lex.cursor = l.cursor;
    c->lex.cursor_from = l.cursor_from;
    c->lex.line = l.line;
    c->lex.column = l.column;
}

VD(b32) VDI(inix_char_n)(VDI(InixContext) *c, VD(Str) s) {
    VDI(InixLexState) state = VDI(inix_save_state)(c);
    VD(u64) i = 0;
    int h = VDI(inix_char)(c);

    while ((i < s.len) && (s.s[i] == h)) {
        if (!VDI(inix_advance)(c)) {
            break;
        }
        h = VDI(inix_char)(c);
        i++;
    }

    if (i == s.len) {
        return VD_TRUE;
    } else {
        VDI(inix_load_state)(c, state);
        return VD_FALSE;
    }
}

void VDI(inix_advance_n)(VDI(InixContext) *c, VD(usize) n) {
    for (VD(u64) i = 0; i < n; ++i) {
        VDI(inix_advance)(c);
    }
}

void VDI(inix_eat_spaces)(VDI(InixContext) *c) {
    int i = VDI(inix_char)(c);
    while (i == ' ' || i == '\t') {
        VDI(inix_advance)(c);
        i = VDI(inix_char)(c);
    }
}

void VDI(inix_eat_comments)(VDI(InixContext) *context) {
    if (!VDI(inix_char_n)(context, VD_LIT("/*"))) {
        return;
    }

    while (!VDI(inix_char_n)(context, VD_LIT("*/"))) {
        VDI(inix_advance)(context); 
    }
}


void VDI(inix_advance_token)(VDI(InixContext) *c, VDI(InixLexToken) token) {
    VDI(inix_advance_n)(c, (token.lexstate.cursor - token.lexstate.cursor_from));
}

VD(b32) VDI(inix_try_parse_integer)(VDI(InixContext) *context, VD(u64) *cursor_begin)
{
    VDI(InixLexState) lexstate = VDI(inix_save_state(context));

    int num_digits = 0;
    int c = VDI(inix_char)(context);
    *cursor_begin = context->lex.cursor;

    while (VDF(is_ascii_digit)(c)) {
        VDI(inix_advance)(context);
        c = VDI(inix_char)(context);
        num_digits++;
    }

    if (num_digits > 0) {

        return VD_TRUE;
    } else {
        VDI(inix_load_state)(context, lexstate);
        return VD_FALSE;
    }
}

VD(b32) VDI(inix_try_parse_simple_string)(VDI(InixContext) *context, VD(u64) *cursor_begin, VD(u64) *cursor_end)
{
    *cursor_begin = context->lex.cursor;
    if (!VDI(inix_char_n)(context, VD_LIT("\""))) {
        return VD_FALSE;
    }

    VDI(inix_advance)(context);

    while (!VDI(inix_char_n)(context, VD_LIT("\""))) {
        if (!VDI(inix_advance)(context)) {
            return VD_FALSE;
        }
    }

    *cursor_end = context->lex.cursor;
    return VD_TRUE;
}

VD(b32) VDI(inix_try_parse_raw_string)(VDI(InixContext) *context, VD(u64) *cursor_begin, VD(u64) *cursor_end)
{
    *cursor_begin = context->lex.cursor;
    if (!VDI(inix_char_n)(context, VD_LIT("@>"))) {
        return VD_FALSE;
    }

    while (!VDI(inix_char_n)(context, VD_LIT("@<"))) {
        if (!VDI(inix_advance)(context)) {
            return VD_FALSE;
        }
    }

    *cursor_end = context->lex.cursor;
    return VD_TRUE;
}

VD(b32) VDI(inix_try_parse_cdecl)(VDI(InixContext) *context, VD(u64) *cursor_begin)
{
    int c = VDI(inix_char)(context);
    *cursor_begin = context->lex.cursor;

    if (!VDF(is_cdecl_start)(c)) {
        return VD_FALSE;
    }

    VDI(inix_advance)(context);
    c = VDI(inix_char)(context);

    while (VDF(is_cdecl_continue)(c)) {
        VDI(inix_advance)(context);
        c = VDI(inix_char)(context);
    }

    return VD_TRUE;
}

VDI(InixLexToken) VDI(inix_peek_token)(VDI(InixContext) *context) {
    VDI(inix_eat_spaces)(context);
    VDI(inix_eat_comments)(context);

    VDI(InixLexState) save = VDI(inix_save_state)(context);

    VDI(InixLexToken) result;
    result.lexstate = context->lex;
    result.lexstate.cursor_from = result.lexstate.cursor;
    result.lexstate.cursor += 1;
    int c = VDI(inix_char)(context);

    switch (c) {
        case -1:   result.type = VDI(INIX_LEX_TOKEN_END);           break;
        case '\0': result.type = VDI(INIX_LEX_TOKEN_INVALID);       break;
        case '[':  result.type = VDI(INIX_LEX_TOKEN_BEGIN_SECTION); break;
        case ']':  result.type = VDI(INIX_LEX_TOKEN_END_SECTION);   break;
        case '=':  result.type = VDI(INIX_LEX_TOKEN_ASSIGNMENT);    break;
        case '\n': result.type = VDI(INIX_LEX_TOKEN_END_STATEMENT); break;
        case '\"': {
            VD(u64) cursor_begin, cursor_end;
            if (VDI(inix_try_parse_simple_string)(context, &cursor_begin, &cursor_end)) {
                result.type = VDI(INIX_LEX_TOKEN_STRING);
                result.lexstate = context->lex;
                result.lexstate.cursor_from = cursor_begin;
                result.lexstate.cursor = cursor_end;
                result.raw_string = (VD(Str)) {&context->contents.s[cursor_begin + 1], cursor_end - cursor_begin - 2 };
            } else {
                result.type = VDI(INIX_LEX_TOKEN_INVALID);
            }
        } break;
        case '@': {
            VD(u64) cursor_begin, cursor_end;
            if (VDI(inix_try_parse_raw_string)(context, &cursor_begin, &cursor_end)) {
                result.type = VDI(INIX_LEX_TOKEN_RAW_STRING);
                result.lexstate = context->lex;
                result.lexstate.cursor_from = cursor_begin;
                result.lexstate.cursor = cursor_end;
                result.raw_string = (VD(Str)) {&context->contents.s[cursor_begin + 2], cursor_end - cursor_begin - 4 };
            } else {
                result.type = VDI(INIX_LEX_TOKEN_INVALID);
            }
        } break;
        default: {
            VD(u64) cursor_begin;
            if (VDI(inix_try_parse_integer)(context, &cursor_begin)) {
                result.type = VDI(INIX_LEX_TOKEN_INTEGER);
                result.lexstate.cursor = context->lex.cursor;
                result.lexstate.cursor_from = cursor_begin;
            } else if (VDI(inix_try_parse_cdecl(context, &cursor_begin))) {
                result.type = VDI(INIX_LEX_TOKEN_ENUMERATION);
                result.lexstate.cursor = context->lex.cursor;
                result.lexstate.cursor_from = cursor_begin;
            } else {
                result.type = VDI(INIX_LEX_TOKEN_INVALID);
            }
        } break;
    }

    VDI(inix_load_state)(context, save);
    return result;
}

void VDI(inix_consume_token)(VDI(InixContext) *context, VDI(InixLexToken) token) {
    if (context->debug_log_tokens) {
        VD(Str) s = {&context->contents.s[token.lexstate.cursor_from] , token.lexstate.cursor - token.lexstate.cursor_from};
        VD(Str) tstr;
        switch (token.type) {
            case VDI(INIX_LEX_TOKEN_INVALID):       tstr = VD_LIT("INIX_LEX_TOKEN_INVALID"); break;
            case VDI(INIX_LEX_TOKEN_BEGIN_SECTION): tstr = VD_LIT("INIX_LEX_TOKEN_BEGIN_SECTION"); break;
            case VDI(INIX_LEX_TOKEN_END_SECTION):   tstr = VD_LIT("INIX_LEX_TOKEN_END_SECTION"); break;
            case VDI(INIX_LEX_TOKEN_ASSIGNMENT):    tstr = VD_LIT("INIX_LEX_TOKEN_ASSIGNMENT"); break;
            case VDI(INIX_LEX_TOKEN_END_STATEMENT): tstr = VD_LIT("INIX_LEX_TOKEN_END_STATEMENT"); break;
            case VDI(INIX_LEX_TOKEN_ENUMERATION):   tstr = VD_LIT("INIX_LEX_TOKEN_ENUMERATION"); break;
            case VDI(INIX_LEX_TOKEN_INTEGER):       tstr = VD_LIT("INIX_LEX_TOKEN_INTEGER"); break;
            case VDI(INIX_LEX_TOKEN_RAW_STRING):    tstr = VD_LIT("INIX_LEX_TOKEN_RAW_STRING"); break;
            case VDI(INIX_LEX_TOKEN_STRING):        tstr = VD_LIT("INIX_LEX_TOKEN_STRING"); break;
            case VDI(INIX_LEX_TOKEN_END):           tstr = VD_LIT("INIX_LEX_TOKEN_END"); break;
            default: break;

        }
        if (token.type == VDI(INIX_LEX_TOKEN_RAW_STRING)) {
            VD_LOGF("Consume %2d:%2d :: %.*s: {{%.*s}}", 1 + token.lexstate.line, token.lexstate.column, VD_STR_EXPAND(tstr), VD_STR_EXPAND(token.raw_string));
        } else if (token.type == VDI(INIX_LEX_TOKEN_STRING)) {
            VD_LOGF("Consume %2d:%2d :: %.*s: {{%.*s}}", 1 + token.lexstate.line, token.lexstate.column, VD_STR_EXPAND(tstr), VD_STR_EXPAND(token.raw_string));
        } else if (token.type != VDI(INIX_LEX_TOKEN_END)) {
            VD_LOGF("Consume %2d:%2d :: %.*s: %.*s", 1 + token.lexstate.line, token.lexstate.column, VD_STR_EXPAND(tstr), VD_STR_EXPAND(s));
        } else {
            VD_LOGF("Consume %.*s", VD_STR_EXPAND(tstr));
        }
    }

    VDI(inix_advance_token)(context, token);
}


VD(b32) VDI(inix_expect_type)(VDI(InixContext) *context, VDI(InixLexTokenType) type, VDI(InixLexToken) *tok)
{
    VDI(InixLexToken) token = VDI(inix_peek_token)(context);
    if (token.type != type) {
        return VD_FALSE;
    }

    if (tok) {
        *tok = token;
    }

    return VD_TRUE;
}

VD(InixSection) *VDI(inix_push_section)(VDI(InixContext) *context) {
    VD(InixSection) *result = VDF(arena_alloc)(context->arena, sizeof(VD(InixSection)));
    context->curr_section->next = result;
    context->curr_section = result;
    context->curr_entry = 0;
    return result;
}

VD(InixEntry) *VDI(inix_push_entry)(VDI(InixContext) *context, VD(InixEntryType) type) {
    VD(InixEntry) *result = VDF(arena_alloc)(context->arena, sizeof(VD(InixEntry)));

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

VD(b32) VDI(inix_parse_section)(VDI(InixContext) *context)
{
    VDI(InixLexToken) first = VDI(inix_peek_token)(context);    
    if (first.type == VDI(INIX_LEX_TOKEN_BEGIN_SECTION)) {
        VDI(inix_consume_token)(context, first);

        VDI(InixLexToken) enumeration;
        if (!VDI(inix_expect_type)(context, VDI(INIX_LEX_TOKEN_ENUMERATION), &enumeration)) {
            VD_ERRF("%d:%d: Expected INIX_LEX_TOKEN_ENUMERATION, instead got: %s",
                enumeration.lexstate.line, enumeration.lexstate.column,
                VDI(inix_lex_token_type_str)(enumeration.type));
            return VD_FALSE;
        }
        VDI(inix_consume_token)(context, enumeration);

        VDI(InixLexToken) end_section;
        if (!VDI(inix_expect_type)(context, VDI(INIX_LEX_TOKEN_END_SECTION), &end_section)) {

            VD_ERRF("%d:%d: Expected INIX_LEX_TOKEN_END_SECTION, instead got: %s",
                enumeration.lexstate.line, enumeration.lexstate.column,
                VDI(inix_lex_token_type_str)(enumeration.type));

            return VD_FALSE;
        }
        VDI(inix_consume_token)(context, end_section);

        VD(Str) section_name = {
            &context->contents.s[enumeration.lexstate.cursor_from],
            enumeration.lexstate.cursor - enumeration.lexstate.cursor_from,
        };

        VD(InixSection) *section = VDI(inix_push_section)(context);
        section->name = section_name;

        // LOGF("Section: [%.*s]", VD_STR_EXPAND(section_name));
        return VD_TRUE;
    }

    return VD_FALSE;
}

static VD(b32) VDI(inix_get_enumeration)(VDI(InixContext) *context, VD(Str) name, VD(u64) *out_enumeration)
{
    for (VD(usize) i = 0; i < context->num_mappings; ++i) {
        if (VD(str_eq)(name, context->mappings[i].key)) {
            *out_enumeration = context->mappings[i].value;
            return VD_TRUE;
        }
    }

    return VD_FALSE;
}

VD(b32) VDI(inix_parse_expression)(VDI(InixContext) *context)
{
    VDI(InixLexToken) first = VDI(inix_peek_token)(context);    
    if (first.type == VDI(INIX_LEX_TOKEN_ENUMERATION)) {
        VDI(inix_consume_token)(context, first);

        VD(Str) first_string = {
            &context->contents.s[first.lexstate.cursor_from],
            first.lexstate.cursor - first.lexstate.cursor_from,
        };

        VDI(InixLexToken) assignment;
        if (!VDI(inix_expect_type)(context, VDI(INIX_LEX_TOKEN_ASSIGNMENT), &assignment)) {
            VD_ERRF("%d:%d: Expected INIX_LEX_TOKEN_ASSIGNMENT, instead got: %s",
                assignment.lexstate.line, assignment.lexstate.column,
                VDI(inix_lex_token_type_str)(assignment.type));

            return VD_FALSE;
        }
        VDI(inix_consume_token)(context, assignment);


        VDI(InixLexToken) val = VDI(inix_peek_token)(context);

        if (val.type == VDI(INIX_LEX_TOKEN_STRING)) {
            VDI(inix_consume_token)(context, val);

            VD(Str) raw_string = val.raw_string;

            VD(InixEntry) *entry = VDI(inix_push_entry)(context, VD_(INIX_ENTRY_TYPE_RAW_STRING));
            entry->name = first_string;
            entry->val.raw_string = raw_string;

            // LOGF("String assignment: %.*s = %.*s", VD_STR_EXPAND(first_string), VD_STR_EXPAND(raw_string));
            return VD_TRUE;

        } else if (val.type == VDI(INIX_LEX_TOKEN_RAW_STRING)) {
            VDI(inix_consume_token)(context, val);

            VD(Str) raw_string = val.raw_string;

            VD(InixEntry) *entry = VDI(inix_push_entry)(context, VD_(INIX_ENTRY_TYPE_RAW_STRING));
            entry->name = first_string;
            entry->val.raw_string = raw_string;

            // LOGF("Raw string assignment: %.*s = %.*s", VD_STR_EXPAND(first_string), VD_STR_EXPAND(raw_string));
            return VD_TRUE;

        } else if (val.type == VDI(INIX_LEX_TOKEN_ENUMERATION)) {
            VDI(inix_consume_token)(context, val);

            VD(InixEntry) *entry = VDI(inix_push_entry)(context, VD_(INIX_ENTRY_TYPE_ENUMERATION));
            entry->name = first_string;

            VD(Str) val_string = {
                &context->contents.s[val.lexstate.cursor_from],
                val.lexstate.cursor - val.lexstate.cursor_from,
            };

            if (!VDI(inix_get_enumeration)(context, val_string, &entry->val.enumeration)) {
                VD_ERRF("%d:%d: Enumeration '%.*s' is not a valid enumeration",
                    val.lexstate.line, val.lexstate.column,
                    VD_STR_EXPAND(entry->name));
                return VD_FALSE;
            }

            // LOGF("Enumeration assignment: %.*s = %.*s", VD_STR_EXPAND(first_string), VD_STR_EXPAND(val_string));

            return VD_TRUE;
        } else if (val.type == VDI(INIX_LEX_TOKEN_INTEGER)) {
            VDI(inix_consume_token)(context, val);

            VD(Str) val_string = {
                &context->contents.s[val.lexstate.cursor_from],
                val.lexstate.cursor - val.lexstate.cursor_from,
            };

            VD(u64) number;
            if (!VDF(parse_u64)(val_string, &number)) {
                VD_ERRF("%d:%d: Failed to parse integer: %s",
                    val.lexstate.line, val.lexstate.column,
                    VDI(inix_lex_token_type_str)(val.type));
                return VD_FALSE;
            }

            VD(InixEntry) *entry = VDI(inix_push_entry)(context, VD_(INIX_ENTRY_TYPE_INTEGER));
            entry->name = first_string;
            entry->val.integer = number;

            // LOGF("Integer assignment: %.*s = %llu", VD_STR_EXPAND(first_string), number);
            return VD_TRUE;
        } else {
            VD_ERRF("%d:%d: Expected INIX_LEX_TOKEN_ENUMERATION or INIX_LEX_TOKEN_INTEGER, instead got: %s",
                val.lexstate.line, val.lexstate.column,
                VDI(inix_lex_token_type_str)(val.type));
            return VD_FALSE;
        }
    }

    return VD_FALSE;
}

VD(b32) VDI(inix_parse_statement)(VDI(InixContext) *context)
{
    VDI(InixLexToken) first = VDI(inix_peek_token)(context);    

    if (first.type == VDI(INIX_LEX_TOKEN_END)) {
        VDI(inix_consume_token)(context, first);
        return VD_TRUE;
    }

    if (first.type == VDI(INIX_LEX_TOKEN_END_STATEMENT)) {
        VDI(inix_consume_token)(context, first);
        return VD_TRUE;
    }

    if (VDI(inix_parse_section)(context)) {
        return VD_TRUE;
    }

    if (VDI(inix_parse_expression)(context)) {
        return VD_TRUE;
    }


    VD_ERRF("%d:%d: Expected INIX_LEX_TOKEN_BEGIN_SECTION or INIX_LEX_TOKEN_ENUMERATION, instead got: %s",
        first.lexstate.line, first.lexstate.column,
        VDI(inix_lex_token_type_str)(first.type));

    return VD_FALSE;
}

static VD(Str) VDI(Inix_Global_Section_Name) = VD_LIT(".global");

VD(InixResult) VDF(inix_parse)(VD(Arena) *arena, VD(InixParseInfo) *info)
{
    VD(InixSection) *global_section = VDF(arena_alloc)(arena, sizeof(VD(InixSection)));
    global_section->name = VDI(Inix_Global_Section_Name);

    VDI(InixContext) context = {
        .contents            = info->contents,
        .arena               = arena,
        .global_section      = global_section,
        .curr_section        = global_section,
        .num_mappings        = info->num_mappings,
        .mappings            = info->mappings,
        .debug_log_tokens    = 0,
    };

    while (!VDI(inix_at_end)(&context)) {
        if (!VDI(inix_parse_statement)(&context)) {
            return (VD(InixResult)) { .global_section = 0, };
        }
    }

    return (VD(InixResult)) {
        .global_section = global_section,
    };
}

#endif // VD_INIX_IMPL
