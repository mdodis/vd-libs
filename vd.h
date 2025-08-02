/**
 * vd.h - A 'batteries-included' header file to use with C99
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
#ifndef VD_H
#define VD_H
#define VD_VERSION_MAJOR    0
#define VD_VERSION_MINOR    0
#define VD_VERSION_PATCH    2
#define VD_VERSION          ((VD_VERSION_MAJOR << 16) | (VD_VERSION_MINOR << 8) | (VD_VERSION_PATCH))

#include <stdint.h>
#include <stddef.h>

/* ----MACRO HELPERS------------------------------------------------------------------------------------------------- */
#define _VD_STRING_JOIN2(x, y) x##y
#define VD_STRING_JOIN2(x, y) _VD_STRING_JOIN2(x, y)

/* ----NAMESPACE----------------------------------------------------------------------------------------------------- */
#ifndef VD_NAMESPACE_OVERRIDE
#define VD(x)  VD_STRING_JOIN2(Vd, x)
#define VDF(x) VD_STRING_JOIN2(vd_, x)
#define VDI(x) VD_STRING_JOIN2(vd__, x)
#define VD_(x) VD_STRING_JOIN2(VD_, x)
#define VD_MACRO_ABBREVIATIONS 0
#endif // VD_NAMESPACE_OVERRIDE

/* ----HOST COMPILER------------------------------------------------------------------------------------------------- */
#if defined(__GNUC__) || defined(__clang__)
#define VD_HOST_COMPILER_CLANG 1
#endif // defined(__GNUC__) || defined(__clang__)

#if defined(_MSC_VER)
#define VD_HOST_COMPILER_MSVC 1
#endif // defined(_MSC_VER)

#ifndef VD_HOST_COMPILER_CLANG
#define VD_HOST_COMPILER_CLANG 0
#endif // !VD_HOST_COMPILER_CLANG

#ifndef VD_HOST_COMPILER_MSVC
#define VD_HOST_COMPILER_MSVC 0
#endif // !VD_HOST_COMPILER_MSVC

#if !VD_HOST_COMPILER_CLANG && !VD_HOST_COMPILER_MSVC
#define VD_HOST_COMPILER_UNKNOWN 1
#endif // !VD_HOST_COMPILER_CLANG && !VD_HOST_COMPILER_MSVC

#ifndef VD_HOST_COMPILER_UNKNOWN
#define VD_HOST_COMPILER_UNKNOWN 0
#endif // !VD_HOST_COMPILER_UNKNOWN

#if defined(__cplusplus)
#define VD_CPP 1
#endif // defined(__cplusplus)

#ifndef VD_CPP
#define VD_CPP 0
#endif // !VD_CPP

#if VD_CPP
#define VD_C 0
#else
#define VD_C 1
#endif // VD_CPP

/* ----PLATFORM------------------------------------------------------------------------------------------------------ */
#if defined(_WIN32) || defined(_WIN64)
#define VD_PLATFORM_WINDOWS 1
#endif

#if defined(__linux__)
#define VD_PLATFORM_LINUX 1
#endif

#if defined(__APPLE__)
#define VD_PLATFORM_MACOS 1
#endif

#ifndef VD_PLATFORM_WINDOWS
#define VD_PLATFORM_WINDOWS 0
#endif

#ifndef VD_PLATFORM_LINUX
#define VD_PLATFORM_LINUX 0
#endif

#ifndef VD_PLATFORM_MACOS
#define VD_PLATFORM_MACOS 0
#endif

/* ----TYPES--------------------------------------------------------------------------------------------------------- */
typedef uint8_t       VD(u8);
typedef uint8_t*      VD(u8ptr);
typedef uint16_t      VD(u16);
typedef uint16_t*     VD(u16ptr);
typedef uint32_t      VD(u32);
typedef uint32_t*     VD(u32ptr);
typedef uint64_t      VD(u64);
typedef uint64_t*     VD(u64ptr);
typedef int8_t        VD(i8);
typedef int8_t*       VD(i8ptr);
typedef int16_t       VD(i16);
typedef int16_t*      VD(i16ptr);
typedef int32_t       VD(i32);
typedef int32_t*      VD(i32ptr);
typedef int64_t       VD(i64);
typedef int64_t*      VD(i64ptr);
typedef unsigned char VD(uchar);
typedef uintptr_t     VD(uptr);
typedef size_t        VD(usize);
typedef char*         VD(cstr);
typedef float         VD(f32);
typedef double        VD(f64);
typedef int32_t       VD(b32);

#define VD_FALSE 0
#define VD_TRUE  1

/* ----BUILD CONFIGURATION------------------------------------------------------------------------------------------- */
#ifndef VD_BUILD_DEBUG
#define VD_BUILD_DEBUG 0
#endif

#if VD_BUILD_DEBUG
#define VD_DEBUG 1
#endif

#ifndef VD_DEBUG
#define VD_DEBUG 0
#endif

#if VD_DEBUG
#define VD_SANITY_CHECKS 1
#else
#define VD_SANITY_CHECKS 0
#endif

/* ----KEYWORDS------------------------------------------------------------------------------------------------------ */
#if VD_HOST_COMPILER_CLANG
#define VD_INLINE __attribute__((always_inline)) inline
#elif VD_HOST_COMPILER_MSVC
#define VD_INLINE __forceinline
#else
#define VD_INLINE static inline
#endif

/**
 * @brief Indicates this function should not be exposed as a symbol.
 */
#define VD_INTERNAL static

/**
 * @brief Indicates this parameter is unused.
 */
#define VD_UNUSED(x) (void)(x)

#define VD_OFFSET_OF(type, element) ((VD(usize)) & (((type*)0)->element))

/* ----LIMITS-------------------------------------------------------------------------------------------------------- */
#define VD_U8_MAX    UINT8_MAX
#define VD_U16_MAX   UINT16_MAX
#define VD_U32_MAX   UINT32_MAX
#define VD_U64_MAX   UINT64_MAX
#define VD_I8_MAX    INT8_MAX
#define VD_I16_MAX   INT16_MAX
#define VD_I32_MAX   INT32_MAX
#define VD_I64_MAX   INT64_MAX
#define VD_SIZET_MAX SIZE_MAX

/* ----COMMON FUNCTIONS---------------------------------------------------------------------------------------------- */
#ifndef VD_DEBUG_BREAK
#include <assert.h>
#define VD_DEBUG_BREAK() assert(0)
#endif // VD_DEBUG_BREAK

#ifndef VD_ASSERT
#include <assert.h>
#define VD_ASSERT(x) assert(x)
#endif // !VD_ASSERT

#ifndef VD_MEMSET
#include <string.h>
#define VD_MEMSET(p, v, s) memset((p), (v), (s))
#endif // !VD_MEMSET

#ifndef VD_MEMCPY
#include <string.h>
#define VD_MEMCPY(d, s, c) memcpy((d), (s), (c))
#endif // !VD_MEMCPY

#ifndef VD_MEMCMP
#include <string.h>
#define VD_MEMCMP(l, r, c) memcmp((l), (r), (c))
#endif // !VD_MEMCMP

#ifndef VD_MEMMOVE
#include <string.h>
#define VD_MEMMOVE(d, s, c) memmove((d), (s), (c))
#endif // !VD_MEMMOVE

#define VD_IMPOSSIBLE() VD_ASSERT(false)

/* ----SIZES--------------------------------------------------------------------------------------------------------- */
#define VD_KILOBYTES(x) (((VD(usize))x) * 1024)
#define VD_MEGABYTES(x) (VD_KILOBYTES(x) * 1024)
#define VD_GIGABYTES(x) (VD_MEGABYTES(x) * 1024)

#define VD_ARRAY_COUNT(s) (sizeof(s) / sizeof(*s))

VD_INLINE VD(b32) VDF(is_power_of_two)(VD(usize) x) {
    return (x & (x - 1)) == 0;
}

VD_INLINE uintptr_t VDF(vd_align_forward)(uintptr_t ptr, size_t align) {
    VD_ASSERT(VDF(is_power_of_two)(align));

    uintptr_t p, a, modulo;

    p = ptr;
    a = (uintptr_t)align;
    modulo = p & (a - 1);

    if (modulo != 0) {
        p += a - modulo;
    }

    return p;
}

/* ----VIRTUAL MEMORY------------------------------------------------------------------------------------------------ */
#ifndef VD_VM_CUSTOM
#define VD_VM_CUSTOM 0
#endif // !VD_VM_CUSTOM

VD(usize)   VDF(vm_get_page_size)();
void*       VDF(vm_reserve)(VD(usize) len);
void*       VDF(vm_commit)(void *addr, VD(usize) len);
void*       VDF(vm_decommit)(void *addr, VD(usize) len);
void        VDF(vm_release)(void *addr, VD(usize) len);

/* ----SYSTEM ALLOCATOR---------------------------------------------------------------------------------------------- */
#ifndef VD_ALLOC_OVERRIDE
#define VD_ALLOC_OVERRIDE 0
#endif // VD_ALLOC_OVERRIDE

#if !VD_ALLOC_OVERRIDE
#include <stdlib.h>
#define VD_REALLOC(ptr, old_size, new_size) realloc(ptr, new_size)
#define VD_FREE(ptr, old_size)              free(ptr)
#endif // !VD_ALLOC_OVERRIDE

#define VD_MALLOC(size)                     VD_REALLOC(0, 0, size)

/* ----ALLOCATOR----------------------------------------------------------------------------------------------------- */
#ifndef VD_ALLOC_DEFAULT_ALIGNMENT 
#define VD_ALLOC_DEFAULT_ALIGNMENT (2 * sizeof(void*))
#endif // VD_ALLOC_DEFAULT_ALIGNMENT 

/* ----ARENA--------------------------------------------------------------------------------------------------------- */
#ifndef VD_ARENA_DEFAULT_ALIGNMENT
#define VD_ARENA_DEFAULT_ALIGNMENT VD_ALLOC_DEFAULT_ALIGNMENT
#endif // VD_ARENA_DEFAULT_ALIGNMENT

#ifndef VD_ARENA_ZERO_ON_CLEAR
#define VD_ARENA_ZERO_ON_CLEAR 1
#endif // VD_ARENA_ZERO_ON_CLEAR 

typedef enum : VD(u8) {
    VD_(ARENA_FLAGS_USE_MALLOC) = 1 << 7,
} VD(ArenaFlags);

typedef struct __VD_Arena {
    VD(u8)         *buf;
    VD(usize)      buf_len;
    VD(usize)      prev_offset;
    VD(usize)      curr_offset;
    VD(ArenaFlags) flags;
    VD(u8)         reserved[3];
} VD(Arena);

typedef struct __VD_ArenaSave {
    VD(Arena)   *arena;
    VD(usize)    prev_offset;
    VD(usize)    curr_offset;
} VD(ArenaSave);

void                    VDF(arena_init)(VD(Arena) *a, void *buf, size_t len);
void*                   VDF(arena_alloc_align)(VD(Arena) *a, size_t size, size_t align);
void*                   VDF(arena_resize_align)(VD(Arena) *a, void *old_memory, size_t old_size, size_t new_size, size_t align);
void                    VDF(arena_clear)(VD(Arena) *a);
VD(b32)                 VDF(arena_free)(VD(Arena) *a, void *memory, size_t size);

VD_INLINE VD(ArenaSave)     VDF(arena_save)(VD(Arena) *a)                                                       { return (VD(ArenaSave)) { .arena = a, .prev_offset = a->prev_offset, .curr_offset = a->curr_offset, }; }
VD_INLINE void              VDF(arena_restore)(VD(ArenaSave) save)                                              { save.arena->prev_offset = save.prev_offset; save.arena->curr_offset = save.curr_offset; }
VD_INLINE void*             VDF(arena_alloc)(VD(Arena) *a, size_t size)                                         { return VDF(arena_alloc_align)(a, size, VD_ARENA_DEFAULT_ALIGNMENT);}
VD_INLINE void*             VDF(arena_resize)(VD(Arena) *a, void *old_memory, size_t old_size, size_t new_size) { return VDF(arena_resize_align)(a, old_memory, old_size, new_size, VD_ARENA_DEFAULT_ALIGNMENT); }

#define VD_ARENA_PUSH_ARRAY(a, x, count) (x*)VDF(arena_alloc)(a, sizeof(x) * count)
#define VD_ARENA_FROM_SYSTEM(a, size)    (VDF(arena_init)(a, VD_MALLOC(size), size))
#define VD_ARENA_FREE_SYSTEM(a)          (VD_FREE(a->buf))

#if VD_MACRO_ABBREVIATIONS
#define ARENA_PUSH_ARRAY(a, x, count) VD_ARENA_PUSH_ARRAY(a, x, count)
#endif // VD_MACRO_ABBREVIATIONS

/* ----SIMPLE ARRAYS------------------------------------------------------------------------------------------------- */

VD_INLINE void *VDI(array_concat)(VD(Arena) *a, void *a1, VD(usize) na1, void *a2, VD(usize) na2, VD(usize) isize)
{
    VD(usize) alloc_size = isize * (na1 + na2);
    void *result = VDF(arena_alloc)(a, alloc_size);
    VD_MEMCPY(result, a1, isize * na1);
    VD_MEMCPY((VD(u8)*)result + (isize * na1), a2, isize * na2);
    return result;
}

#define VD_ARRAY_CONCAT(a, a1, na1, a2, na2) VDI(array_concat)((a), (a1), (na1), (void*)(a2), (na2), sizeof(*a1))

/* ----FIXED ARRAY--------------------------------------------------------------------------------------------------- */
typedef struct {
    VD(u32) len;
    VD(u32) cap;
} VD(FixedArrayHeader);

#define VD_FIXEDARRAY_HEADER(a)                         ((VD(FixedArrayHeader)*)(((VD(u8)*)a) - sizeof(VD(FixedArrayHeader))))

/**
 * @sym VD_FIXEDARRAY
 *
 * @brief   A fixed size array with no dynamic reallocation whatsoever
 *
 * @details VD_FIXEDARRAY int *myarray = 0;
 *          Prepend variables like this to indicate they are fixed arrays and not just pointers
 */
#define VD_FIXEDARRAY

/**
 * @sym VD_FIXEDARRAY_INIT
 * Initialize the fixed array as empty with a set count
 *
 * @param a         Array pointer (must be 0 initially)
 * @param count     The maximum capacity of the fixed array
 * @param allocator Pointer to the arena that will allocate the fixed array
 */
#define VD_FIXEDARRAY_INIT(a, count, allocator)         ((a) = VDI(buffer_allocate)((allocator), (count), sizeof(*(a)), 0))
#define VD_FIXEDARRAY_INIT_RESERVE(a, len, allocator)   ((a) = VDI(buffer_allocate)((allocator), (len),   sizeof(*(a)), 1))
#define VD_FIXEDARRAY_CHECK(a, n)                       ((VD_FIXEDARRAY_HEADER(a)->len + (n)) <= VD_FIXEDARRAY_HEADER(a)->cap)
#define VD_FIXEDARRAY_ADDN(a, n)                        (VD_ASSERT(VD_FIXEDARRAY_CHECK(a, n)), VD_ARRAY_HEADER(a)->len += n)
#define VD_FIXEDARRAY_ADD(a, v)                         (VD_ASSERT(VD_FIXEDARRAY_CHECK(a, 1)), (a)[VD_FIXEDARRAY_HEADER(a)->len++] = (v))
#define VD_FIXEDARRAY_LEN(a)                            (VD_FIXEDARRAY_HEADER(a)->len)
#define VD_FIXEDARRAY_CAP(a)                            (VD_FIXEDARRAY_HEADER(a)->cap)
#define VD_FIXEDARRAY_CLEAR(a)                          (VD_FIXEDARRAY_HEADER(a)->len = 0)
#define VD_FIXEDARRAY_POP(a)                            ((a)[--VD_FIXEDARRAY_HEADER(a)->len])

VD_INLINE void* VDI(buffer_allocate)(VD(Arena) *arena, VD(u32) capacity, VD(usize) isize, VD(b32) mark)
{
    VD(usize) alloc_size = sizeof(VD(FixedArrayHeader)) + capacity * isize;
    void *result = VD_MEMSET(VDF(arena_alloc)(arena, alloc_size), 0, alloc_size);
    VD(FixedArrayHeader) *hdr = result;

    hdr->cap = capacity;
    if (mark) {
        hdr->len = capacity;
    }

    return (void*) ((VD(u8)*)result + sizeof(VD(FixedArrayHeader)));
}


#if VD_MACRO_ABBREVIATIONS
/** @sym fixedarray_init Shorthand for @see VD_FIXEDARRAY_INIT */
#define fixedarray_init(a, count, allocator)         VD_FIXEDARRAY_INIT(a, count, allocator)
/** @sym fixedarray_init_reserve Shorthand for @see VD_FIXEDARRAY_INIT_RESERVE */
#define fixedarray_init_reserve(a, len, allocator)   VD_FIXEDARRAY_INIT_RESERVE(a, len, allocator)
#define fixedarray_addn(a, n)                        VD_FIXEDARRAY_ADDN(a, n)
#define fixedarray_add(a, v)                         VD_FIXEDARRAY_ADD(a, v)
#define fixedarray_len(a)                            VD_FIXEDARRAY_LEN(a)
#define fixedarray_cap(a)                            VD_FIXEDARRAY_CAP(a)
#define fixedarray_clear(a)                          VD_FIXEDARRAY_CLEAR(a)
#define fixedarray_pop(a)                            VD_FIXEDARRAY_POP(a)
#define fixedarray
#endif

/* ----DYNAMIC ARRAY------------------------------------------------------------------------------------------------- */
typedef struct {
    VD(u32)     len;
    VD(u32)     cap;
    VD(Arena)   *arena;
} VD(DynArrayHeader);

#define VD_DYNARRAY_HEADER(a)       ((VD(DynArrayHeader)*)(((VD(u8)*)a) - sizeof(VD(DynArrayHeader))))
#define VD_DYNARRAY_INIT(a, arena)  ((a) = VDI(dynarray_grow)(a, sizeof(*(a)), 1, 0, arena))
#define VD_DYNARRAY_ADD(a, v)       (VD_DYNARRAY_CHECKGROW(a, 1), (a)[VD_DYNARRAY_HEADER(a)->len++] = (v))
#define VD_DYNARRAY_PUSH(a, v)      (VD_DYNARRAY_CHECKGROW(a, 1), &((a)[VD_DYNARRAY_HEADER(a)->len++]))
#define VD_DYNARRAY_ADDN(a, n)      (VD_DYNARRAY_CHECKGROW(a, n), VD_DYNARRAY_HEADER(a)->len += (n))
#define VD_DYNARRAY_CLEAR(a)        ((a) ? VD_DYNARRAY_HEADER(a)->len = 0 : 0)
#define VD_DYNARRAY_POP(a)          (VD_DYNARRAY_HEADER(a)->len--, (a)[VD_DYNARRAY_HEADER(a)->len])
#define VD_DYNARRAY_LAST(a)         ((a)[VD_DYNARRAY_HEADER(a)->len - 1])
#define VD_DYNARRAY_LEN(a)          ((a) ? VD_DYNARRAY_HEADER(a)->len : 0)
#define VD_DYNARRAY_CAP(a)          ((a) ? VD_DYNARRAY_HEADER(a)->cap : 0)
#define VD_DYNARRAY_ARENAP(a)       ((a) ? VD_DYNARRAY_HEADER(a)->arena : 0)
#define VD_DYNARRAY_GROW(a, b, c)   ((a) = VDF(dynarray_grow)((a), sizeof(*(a)), (b), (c), VD_DYNARRAY_ARENAP(a)))
#define VD_DYNARRAY_CHECKGROW(a, n)                      \
    ((!(a) || VD_ARRAY_HEADER(a)->len + (n) > VD_ARRAY_HEADER(a)->cap) \
    ? (VD_ARRAY_GROW(a, n, 0), 0) : 0)

#define VD_DYNARRAY

VD_INLINE void *VDI(dynarray_grow)(void *a, VD(usize) tsize, VD(u32) addlen, VD(u32) mincap, VD(Arena) *arena)
{
    VD(usize) min_len = VD_DYNARRAY_LEN(a) + addlen;
    #define VD_DYNARRAY_LAST(a)         ((a)[VD_DYNARRAY_HEADER(a)->len - 1])

    if (min_len > mincap) {
        mincap = min_len;
    }

    if (mincap <= VD_DYNARRAY_CAP(a)) {
        return a;
    }

    if (mincap < (2 * VD_DYNARRAY_CAP(a))) {
        mincap = 2 * VD_DYNARRAY_CAP(a);
    } else {
        mincap = 4;
    }

    void *b = VDF(arena_resize)(arena, 
        a ? VD_DYNARRAY_HEADER(a) : 0,
        VD_DYNARRAY_CAP(a) == 0 ? 0 : tsize * VD_DYNARRAY_CAP(a) + sizeof(VD(DynArrayHeader)),
        tsize * mincap * sizeof(VD(DynArrayHeader)));

    b = (VD(u8)*)b + sizeof(VD(DynArrayHeader));

    if (a == 0) {
        VD_DYNARRAY_HEADER(a)->len = 0;
        VD_DYNARRAY_HEADER(a)->arena = arena;
    }

    VD_DYNARRAY_HEADER(a)->cap = mincap;
    return b;
}

#if VD_MACRO_ABBREVIATIONS
#define dynarray_init(a, arena) VD_DYNARRAY_INIT(a, arena)
#define dynarray_add(a, v)      VD_DYNARRAY_ADD(a, v)
#define dynarray_push(a, v)     VD_DYNARRAY_PUSH(a, v)
#define dynarray_addn(a, n)     VD_DYNARRAY_ADDN(a, n)
#define dynarray_clear(a)       VD_DYNARRAY_CLEAR(a)
#define dynarray_pop(a)         VD_DYNARRAY_POP(a)
#define dynarray_last(a)        VD_DYNARRAY_LAST(a)
#define dynarray_len(a)         VD_DYNARRAY_LEN(a)
#define dynarray_cap(a)         VD_DYNARRAY_CAP(a)
#endif

/* ----STR----------------------------------------------------------------------------------------------------------- */
typedef struct __VD_Str {
    char        *s;
    VD(usize)   len;
} VD(Str);

VD_INLINE VD(b32)      VDF(cstr_cmp)(VD(cstr) _a, VD(cstr) _b);
VD_INLINE VD(usize)    VDF(cstr_len)(VD(cstr) a);
VD_INLINE VD(cstr)     VDF(cstr_dup)(VD(Arena) *arena, VD(cstr) s);
VD_INLINE VD(cstr)     VDF(cstr_cncat)(VD(Arena) *arena, VD(cstr) a, VD(cstr) b);
VD_INLINE VD(cstr)     VDF(cstr_ncncat)(VD(Arena) *arena, VD(usize) *num_strings, VD(cstr) *strings);

VD_INLINE VD(Str)      VDF(str_from_cstr)(VD(cstr) s) { return (VD(Str)) { .s = s, .len = VDF(cstr_len)(s) }; }
VD_INLINE VD(Str)      VDF(str_dup)(VD(Arena) *a, VD(Str) s);
VD_INLINE VD(Str)      VDF(str_dup_from_cstr)(VD(Arena) *a, VD(cstr) s);
VD_INLINE VD(usize)    VDF(str_first_of)(VD(Str) s, VD(Str) q, VD(u64) start);
VD_INLINE VD(b32)      VDF(str_split)(VD(Str) s, VD(usize) at, VD(Str) *left, VD(Str) *right);
VD_INLINE VD(Str)      VDF(str_chop_left)(VD(Str) s, VD(usize) at) { VD(Str) left, right; return VDF(str_split)(s, at, &left, &right) ? right : (VD(Str)){0, 0}; }
VD_INLINE VD(Str)      VDF(str_chop_right)(VD(Str) s, VD(usize) at) { VD(Str) left, right; return VDF(str_split)(s, at, &left, &right) ? left : (VD(Str)){0, 0}; }
VD_INLINE VD(b32)      VDF(str_eq)(VD(Str) a, VD(Str) b);
VD_INLINE VD(Str)      VDF(str_join)(VD(Arena) *arena, VD(Str) a, VD(Str) b, VD(b32) null_sep);
VD_INLINE VD(b32)      VDF(str_ends_with_char)(VD(Str) a, char c) { return a.len > 0 ? a.s[a.len - 1] == c : VD_FALSE; }

VD_INLINE VD(b32) VDF(cstr_cmp)(VD(cstr) _a, VD(cstr) _b)
{
    VD(cstr) a = _a;
    VD(cstr) b = _b;

    while (*a == *b) {
        if (*a == 0) return VD_TRUE;

        a++;
        b++;
    }

    return VD_FALSE;
}

VD_INLINE VD(usize) VDF(cstr_len)(VD(cstr) a)
{
    VD(usize) result = 0;
    while (*a++) result++;
    return result;
}

VD_INLINE VD(Str) VDF(str_dup)(VD(Arena) *a, VD(Str) s) {
    VD(Str) result;
    result.s = VDF(arena_alloc)(a, s.len);
    result.len = s.len;
    VD_MEMCPY(result.s, s.s, s.len);
    return result;
}

VD_INLINE VD(Str) VDF(str_dup_from_cstr)(VD(Arena) *a, VD(cstr) s) {
    return VDF(str_dup)(a, VDF(str_from_cstr)(s));
}

VD_INLINE VD(usize) VDF(str_first_of)(VD(Str) s, VD(Str) q, VD(u64) start) {
    if (s.len == 0) return 0;
    if (q.len == 0) return 0;

    VD(usize) qindex = 0;
    VD(usize) i;
    for (i = start; i < s.len; ++i) {
        if (s.s[i] == q.s[qindex]) {
            qindex++;
        } else {
            qindex = 0;
        }

        if (qindex == q.len) {
            return i - q.len + 1;
        }
    }

    return s.len;
}

VD_INLINE VD(b32) VDF(str_split)(VD(Str) s, VD(usize) at, VD(Str) *left, VD(Str) *right)
{
    if ((s.len < at) || (at >= s.len)) {
        return VD_FALSE;
    }

    left->s    = s.s;
    left->len  = at + 1;
    right->s   = s.s + at + 1;
    right->len = s.len - (at + 1);

    return VD_TRUE;
}

VD_INLINE VD(b32) VDF(str_eq)(VD(Str) a, VD(Str) b)
{
    if (a.len != b.len) return VD_FALSE;

    for (VD(usize) i = 0; i < a.len; ++i) {
        if (a.s[i] != b.s[i]) return VD_FALSE;        
    }

    return VD_TRUE;
}

VD_INLINE VD(cstr) VDF(cstr_cncat)(VD(Arena) *arena, VD(cstr) a, VD(cstr) b) {
    VD(usize) la = VDF(cstr_len)(a);
    VD(usize) lb = VDF(cstr_len)(b);

    VD(cstr) result = (VD(cstr))VDF(arena_alloc)(arena, la + lb + 1);

    VD_MEMCPY(result, a, la);
    VD_MEMCPY(result + la, b, lb);
    result[la + lb] = 0;
    return result;
}

VD_INLINE VD(cstr) VDF(cstr_dup)(VD(Arena) *arena, VD(cstr) s) {
    VD(usize) ls = VDF(cstr_len)(s);
    VD(cstr) result = (VD(cstr))VDF(arena_alloc)(arena, ls + 1);
    VD_MEMCPY(result, s, ls);
    result[ls] = 0;
    return result;
}

VD_INLINE VD(Str) VDF(str_join)(VD(Arena) *arena, VD(Str) a, VD(Str) b, VD(b32) null_sep)
{
    VD(usize) final_size = a.len + b.len + (null_sep ? 1 : 0);
    char *result = (char*)VDF(arena_alloc)(arena, final_size);

    VD_MEMCPY(result, a.s, a.len);
    VD_MEMCPY(result + a.len, b.s, b.len);
    if (null_sep) {
        result[a.len + b.len] = 0;
    }

    return (VD(Str)) { result, final_size };
}

#define VD_LIT(string)  (VD(Str)) { .s = (string), .len = (sizeof(string) - 1), }

#define VD_STR_EXPAND(string) (int)(string).len, (string).s

#if VD_MACRO_ABBREVIATIONS
#define LIT(string)         VD_LIT(string)
#define STR_EXPAND(string)  VD_STR_EXPAND(string)
#endif

/* ----PARSING------------------------------------------------------------------------------------------------------- */
VD_INLINE VD(b32)      VDF(is_ascii_digit)(char c);
VD_INLINE VD(b32)      VDF(is_letter)(char c);
VD_INLINE VD(b32)      VDF(is_cdecl_start)(char c);
VD_INLINE VD(b32)      VDF(is_cdecl_continue)(char c);
VD(b32)                VDF(parse_u64)(VD(Str) s, VD(u64) *r);

VD_INLINE VD(b32) VDF(is_ascii_digit)(char c)
{
    return (c >= '0') && (c <= '9');
}

VD_INLINE VD(b32) VDF(is_letter)(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

VD_INLINE VD(b32) VDF(is_cdecl_start)(char c)
{
    return VDF(is_letter)(c) || c == '_';
}

VD_INLINE VD(b32) VDF(is_cdecl_continue)(char c)
{
    return VDF(is_cdecl_start)(c) || VDF(is_ascii_digit)(c);
}

/* ----ARG----------------------------------------------------------------------------------------------------------- */
typedef struct __VD_Arg {
    int argc;
    char **argv;

    int argi;
    int ci;
} VD(Arg);

VD(Arg) VDF(arg_new)(int argc, char **argv);
void    VDF(arg_skip_program_name)(VD(Arg) *arg);
void    VDF(arg_skip)(VD(Arg) *arg);
VD(b32) VDF(arg_at_end)(VD(Arg) *arg);
VD(b32) VDF(arg_get_name)(VD(Arg) *arg, VD(Str) *name);
VD(b32) VDF(arg_get_uint)(VD(Arg) *arg, VD(u64) *i);
VD(b32) VDF(arg_get_str)(VD(Arg) *arg, VD(Str) *str);
VD(b32) VDF(arg_expect_char)(VD(Arg) *arg, char c);

/* ----HASH---------------------------------------------------------------------------------------------------------- */
#ifndef VD_HASH64_CUSTOM
#define VD_HASH64_CUSTOM 0
#endif // !VD_HASH64_CUSTOM

#if !VD_HASH64_CUSTOM
#define VD_HASH64_DEFAULT_SEED (0x9747b28c)

VD_INLINE VD(u64) VDF(hash64)(const void *data, VD(u64) len, VD(u32) seed)
{
    const VD(u64) m = 0x5bd1e995;
    const VD(u64) r = 24;

    VD(u64) h = seed ^ len;

    const VD(u8) *bytes = (const VD(u8)*)data;

    while (len >= 4)
    {
        VD(u32) k = *(VD(u32)*)bytes;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        bytes += 4;
        len -= 4;   
    }

    switch (len) {
        case 3: h ^= bytes[2] << 16;
        case 2: h ^= bytes[1] << 8;
        case 1: h ^= bytes[0];
                h *= m;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}

VD_INLINE VD(u64) VDF(dhash64)(const void *data, VD(u64) len) { return VDF(hash64)(data, len, VD_HASH64_DEFAULT_SEED); }
#endif // !VD_HASH64_CUSTOM

VD_INLINE VD(u64) VDF(dhash64_str)(VD(Str) s) { return VDF(dhash64)(s.s, s.len); }

/* ----STRMAP-------------------------------------------------------------------------------------------------------- */
typedef struct {
    VD(u32)   cap;
    VD(u32)   cap_total;
    VD(u32)   taken;
    VD(u32)   tsize;
    VD(Arena) *arena;
} VDI(StrmapHeader);

typedef struct VDI(StrmapBinPrefix) VDI(StrmapBinPrefix);

typedef enum {
    VDI(STRMAP_GET_BIN_FLAGS_CREATE)       = 1 << 1,
    VDI(STRMAP_GET_BIN_FLAGS_SET_UNUSED)   = 1 << 2,
    VDI(STRMAP_GET_BIN_FLAGS_GET_EXISTING) = 1 << 3,
} VDI(StrmapGetBinFlags);

typedef enum {
    VDI(STRMAP_SET_NEW_ONLY)  = 0,
    VDI(STRMAP_SET_OVERWRITE) = 1,
} VDI(StrmapSetMode);

struct VDI(StrmapBinPrefix) {
    VDI(StrmapBinPrefix) *next;             //  8 bytes
    VDI(StrmapBinPrefix) *insq;             //  8 bytes
    VD(u32)              key_len;           //  4 bytes
    VD(u32)              key_rest_cap;      //  4 bytes
    VD(u8)               used;              //  1 byte
    char                 *key_rest;         //  8 bytes
    char                 key_prefix[47];    // 47 bytes
};                                          // = 80 bytes

/**
 * @sym VD(StrmapInitOptions)
 *
 * @brief Options to modify Strmap behavior, allocation strategy, etc.
 */
typedef struct __VD_StrmapInitOptions {
    /** A value from [0.3, 0.9] that determines the amount of slots assigned to the addressable region. */
    float address_scale;

    /** The estimated key length in bytes. If this is higher than 47, then more space will be allocated for key storage. */
    VD(u32) average_key_len;
} VD(StrmapInitOptions);

/**
 * @sym VD_STRMAP
 *
 * @brief   A string map is hash table using coalesced hashing
 *
 * @details VD_STRMAP int *my_map = 0;
 *          Prepend variables like this to indicate that they are string maps and not pointers
 */
#define VD_STRMAP
#define VD_STRMAP_DEFAULT_CAP              1024
#define VD_STRMAP_HEADER(m)                ((VDI(StrmapHeader)*)(((VD(u8)*)(m)) - sizeof(VDI(StrmapHeader))))
#define VD_STRMAP_INIT(m, arena, cap, o)   ((m) = (VDI(strmap_init)((arena), sizeof(*m), (cap), (o))))
#define VD_STRMAP_INIT_DEFAULT(m, arena)   VD_STRMAP_INIT((m), (arena), VD_STRMAP_DEFAULT_CAP)
#define VD_STRMAP_SET(m, k, v)             VDI(strmap_set)((m), (k), (void*)(v), VDI(STRMAP_SET_NEW_ONLY))
#define VD_STRMAP_GET(m, k, v)             VDI(strmap_get)((m), (k), (void*)(v))
#define VD_STRMAP_RM(m, k)                 (VDI(strmap_get_bin)((m), (k), VDI(STRMAP_GET_BIN_FLAGS_SET_UNUSED)) != 0)
#define VD_STRMAP_OVERWRITE(m, k, v)       VDI(strmap_set)((m), (k), (void*)(v), VDI(STRMAP_SET_OVERWRITE))
#define VD_STRMAP_COUNT(m)                 ((m) == 0 ? 0 : VD_STRMAP_HEADER(m)->taken)
#define VD_STRMAP_TSIZE(m)                 ((m) == 0 ? 0 : VD_STRMAP_HEADER(m)->tsize)
#define VD_STRMAP_TOTAL_CAP(m)             ((m) == 0 ? 0 : VD_STRMAP_HEADER(m)->cap_total)
#define VD_STRMAP_ARENAP(m)                ((m) == 0 ? 0 : VD_STRMAP_HEADER(m)->arena)
#define VD_STRMAP_ENTRY_SIZE(m)            (VD_STRMAP_HEADER(m)->tsize + sizeof(VDI(StrmapBinPrefix)))
#define VD_STRMAP_GET_BIN(m, i)            ((VDI(StrmapBinPrefix)*) (((VD(u8)*)m) + (VD_STRMAP_ENTRY_SIZE(m) * (i))))
#define VD_STRMAP_GET_ENTRY(m, i)          ((void*)((VD(u8)*)VD_STRMAP_GET_BIN(m, i) + sizeof(VDI(StrmapBinPrefix))))
#define VD_STRMAP_GET_KEY_PREFIX(m, i)     VD_STRMAP_GET_BIN(m, i)->key_prefix
#define VD_STRMAP_GET_KEY_LEN(m, i)        VD_STRMAP_GET_BIN(m, i)->key_len
#define VD_STRMAP_GET_BIN_USED(m, i)       VD_STRMAP_GET_BIN(m, i)->used
#define VD_STRMAP_GET_BIN_NEXT(m, i)       VD_STRMAP_GET_BIN(m, i)->next
#define VD_STRMAP_GET_BIN_INDEX(m, b)      (size_t)(((uintptr_t)b - (uintptr_t)m) / (uintptr_t)(VD_STRMAP_ENTRY_SIZE(m)))
#define VD_STRMAP_GET_BIN_VPTR(m, i)       ((void*)(((VD(u8)*)VD_STRMAP_GET_BIN(m, i)) + sizeof(VDI(StrmapBinPrefix))))
#define VD_STRMAP_BIN_MOVE_TO_VPTR(b)      ((void*)(((VD(u8)*)(b)) + sizeof(VDI(StrmapBinPrefix))))
    
void*                   VDI(strmap_init)(VD(Arena) *arena, VD(u32) tsize, VD(u32) cap, VD(StrmapInitOptions) *options);
VDI(StrmapBinPrefix)*   VDI(strmap_get_bin)(void *map, VD(Str) key, VDI(StrmapGetBinFlags) op);
VD_INLINE VD(b32)       VDI(strmap_get)(void *map, VD(Str) key, void *value);
VD_INLINE VD(b32)       VDI(strmap_set)(void *map, VD(Str) key, void *value, VDI(StrmapSetMode) mode);

VD_INLINE VD(b32) VDI(strmap_get)(void *map, VD(Str) key, void *value)
{
    // Look for bin 
    VDI(StrmapBinPrefix) *bin = VDI(strmap_get_bin)(map, key, VDI(STRMAP_GET_BIN_FLAGS_GET_EXISTING));
    if (bin == 0) return VD_FALSE;

    // If found the copy over the data
    VD(u8) *bin_data = ((VD(u8)*)bin) + sizeof(VDI(StrmapBinPrefix));
    VD_MEMCPY(value, bin_data, VD_STRMAP_HEADER(map)->tsize);
    return VD_TRUE;
}

VD_INLINE VD(b32) VDI(strmap_set)(void *map, VD(Str) key, void *value, VDI(StrmapSetMode) mode)
{
    if (mode == VDI(STRMAP_SET_NEW_ONLY)) {
        VDI(StrmapBinPrefix) *bin = VDI(strmap_get_bin)(map, key, VDI(STRMAP_GET_BIN_FLAGS_CREATE));
        if (bin == 0) return VD_FALSE;

        VD(u8) *bin_data = ((VD(u8)*)bin) + sizeof(VDI(StrmapBinPrefix));
        VD_MEMCPY(bin_data, value, VD_STRMAP_HEADER(map)->tsize);
        return VD_TRUE;
    } else {
        VDI(StrmapBinPrefix) *bin = VDI(strmap_get_bin)(map, key, VDI(STRMAP_GET_BIN_FLAGS_CREATE) | VDI(STRMAP_GET_BIN_FLAGS_GET_EXISTING));
        if (bin == 0) return VD_FALSE;

        VD(u8) *bin_data = ((VD(u8)*)bin) + sizeof(VDI(StrmapBinPrefix));
        VD_MEMCPY(bin_data, value, VD_STRMAP_HEADER(map)->tsize);
        return VD_TRUE;
    }
}

/* ----HANDLEMAP----------------------------------------------------------------------------------------------------- */
typedef struct __VD_HdlMap {
    VD(u64)     initial_cap;
    VD(Arena)   arena;
    void        (*on_free_object)(void *object, void *c);
} VD(HdlMap);

typedef struct __VD_Handle {
    VD(u64)    id;
    VD(HdlMap) *map; 
} VD(Hdl);


/* ----FILESYSTEM---------------------------------------------------------------------------------------------------- */
#include <stdio.h>
VD_INLINE VD(u8)*  VDF(dump_file_to_bytes)(VD(Arena) *arena, VD(cstr) file_path, VD(usize) *len)
{
    FILE *f = fopen(file_path, "rb");
    fseek(f, 0, SEEK_END);
    VD(usize) size = ftell(f);
    fseek(f, 0, SEEK_SET);

    VD(u8) *result = (VD(u8)*)VDF(arena_alloc)(arena, size);
    fread(result, size, 1, f);
    *len = size;
    return result;
}

VD_INLINE VD(cstr) VDF(dump_file_to_cstr)(VD(Arena) *arena, VD(cstr) file_path, VD(usize) *len)
{
    FILE *f = fopen(file_path, "rb");
    if (f == 0) return 0;
    
    fseek(f, 0, SEEK_END);
    VD(usize) size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *result = VDF(arena_alloc)(arena, size + 1);
    fread(result, size, 1, f);
    result[size] = 0;
    *len = size;
    return result;
}


/* ----SCRATCH------------------------------------------------------------------------------------------------------- */
#ifndef VD_SCRATCH_PAGE_COUNT
#define VD_SCRATCH_PAGE_COUNT 8
#endif // VD_SCRATCH_PAGE_COUNT

#ifndef VD_SCRATCH_PAGE_SIZE
#define VD_SCRATCH_PAGE_SIZE VD_KILOBYTES(64)
#endif // VD_SCRATCH_PAGE_SIZE 

typedef struct __VD_Scratch {
    VD(Arena)   arenas[VD_SCRATCH_PAGE_COUNT];
    VD(usize)   curr_arena;
} VD(Scratch);
void                        VDF(scratch_init)(VD(Scratch) *scratch);
VD(Arena)*                  VDF(scratch_get_arena)(VD(Scratch) *scratch);
void                        VDF(scratch_return_arena)(VD(Scratch) *scratch, VD(Arena) *arena);

/* ----LOG----------------------------------------------------------------------------------------------------------- */
#define VD_PROC_LOG(name) void name(int verbosity, VD(cstr) string)
typedef VD_PROC_LOG(VD(ProcLog));

#define VD_LOG_VERBOSITY_ERROR      (-10)
#define VD_LOG_VERBOSITY_WARNING    (0)
#define VD_LOG_VERBOSITY_LOG        (10)
#define VD_LOG_VERBOSITY_DEBUG      (100)

#ifndef VD_CUSTOM_LOG
#define VD_LOG_BUFFER       0
#define VD_LOG_BUFFER_SIZE  0
#define VD_LOG_CALL(verbosity, message)
#endif // !VD_CUSTOM_LOG

#define VD_LOG_IMPL(verbosity, fmt, ...) do {\
        snprintf(VD_LOG_BUFFER, VD_LOG_BUFFER_SIZE, fmt, __VA_ARGS__); \
        VD_LOG_CALL(verbosity, VD_LOG_BUFFER); \
    } while (0)

#define VD_LOG_ADD_NEWLINE(s) s "\n"
#define VD_ERRF(fmt, ...) VD_LOG_IMPL(VD_LOG_VERBOSITY_ERROR,   VD_LOG_ADD_NEWLINE(fmt), __VA_ARGS__)
#define VD_WRNF(fmt, ...) VD_LOG_IMPL(VD_LOG_VERBOSITY_WARNING, VD_LOG_ADD_NEWLINE(fmt), __VA_ARGS__)
#define VD_LOGF(fmt, ...) VD_LOG_IMPL(VD_LOG_VERBOSITY_LOG,     VD_LOG_ADD_NEWLINE(fmt), __VA_ARGS__)
#define VD_DBGF(fmt, ...) VD_LOG_IMPL(VD_LOG_VERBOSITY_DEBUG,   VD_LOG_ADD_NEWLINE(fmt), __VA_ARGS__)

#if VD_MACRO_ABBREVIATIONS
#define ERRF(fmt, ...)    VD_ERRF(fmt, __VA_ARGS__)
#define WRNF(fmt, ...)    VD_WRNF(fmt, __VA_ARGS__)
#define LOGF(fmt, ...)    VD_LOGF(fmt, __VA_ARGS__)
#define DBGF(fmt, ...)    VD_DBGF(fmt, __VA_ARGS__)
#endif // VD_MACRO_ABBREVIATIONS

/* ----THREAD CONTEXT------------------------------------------------------------------------------------------------ */
#ifndef VD_CUSTOM_THREAD_CONTEXT
typedef struct {
    VD(Scratch) scratch;
} VD(ThreadContext);

#define VD_THREAD_CONTEXT_TYPE          VD(ThreadContext)
#define VD_THREAD_CONTEXT_VARNAME       Thread_Context
#define VD_THREAD_CONTEXT_SCRATCH(tc)   (&tc->scratch)
#endif // !VD_CUSTOM_THREAD_CONTEXT

#define VD_THREAD_CONTEXT_GET()         (VD_THREAD_CONTEXT_VARNAME)
#define VD_THREAD_CONTEXT_SET(value)    (VD_THREAD_CONTEXT_VARNAME = value)
extern VD_THREAD_CONTEXT_TYPE * VD_THREAD_CONTEXT_VARNAME;

#define VD_SCRATCH()                    VD_THREAD_CONTEXT_SCRATCH(VD_THREAD_CONTEXT_GET())
#define VD_GET_SCRATCH_ARENA()          VDF(scratch_get_arena)(VD_SCRATCH())
#define VD_RETURN_SCRATCH_ARENA(a)      VDF(scratch_return_arena)(VD_SCRATCH(), a)

#ifndef VD_ENABLE_SCRATCH_USE_IN_LIBRARY
#define VD_ENABLE_SCRATCH_USE_IN_LIBRARY 0
#endif // !VD_ENABLE_SCRATCH_USE_IN_LIBRARY

/* ----TESTING------------------------------------------------------------------------------------------------------- */
#ifndef VD_INCLUDE_TESTS
#define VD_INCLUDE_TESTS 0
#endif // VD_INCLUDE_TESTS

#if VD_INCLUDE_TESTS && !VD_HOST_COMPILER_UNKNOWN
extern VD(Arena) *Test_Arena;

typedef struct __VD_TestResult {
    int         ok;
    const char  *err;
} VD(TestResult);

#define VD_PROC_TEST(name) VD(TestResult) name(void)
typedef VD_PROC_TEST(VD(ProcTest));

typedef struct __VD_TestEntry {
    const char   *name;
    VD(ProcTest) *test;
} VD(TestEntry);

#define VD_TEST_PROC_ID(counter)  VD_STRING_JOIN2(vd_test_proc_, counter)
#define VD_TEST_ENTRY_ID(counter) VD_STRING_JOIN2(vd_test_entry_, counter)
#define VD_TEST_REG_ID(counter)   VD_STRING_JOIN2(vd_test_reg_, counter)

#if VD_HOST_COMPILER_MSVC

#pragma section(".vdtests$a", read)
__declspec(allocate(".vdtests$a")) static VD(TestEntry) *VDI(Test_Start) = 0;

#pragma section(".vdtests$z", read)
__declspec(allocate(".vdtests$z")) static VD(TestEntry) *VDI(Test_End)   = 0;

#define VD_TEST_SECTION __declspec(allocate(".vdtests$m"))
#define VD_TEST_USED

#define VD_TEST_IMPL(string, counter) \
    static VD_PROC_TEST(VD_TEST_PROC_ID(counter)); \
    static VD_TEST_SECTION VD_TEST_USED VD(TestEntry) VD_TEST_ENTRY_ID(counter) = {string, VD_TEST_PROC_ID(counter)}; \
    static VD_PROC_TEST(VD_TEST_PROC_ID(counter))

#elif VD_HOST_COMPILER_CLANG

extern VD(TestEntry) *VDI(Test_Entries);
extern VD(usize)      VDI(Cap_Test_Entries);
extern VD(usize)      VDI(Len_Test_Entries);

VD_INLINE void VDI(test_register)(const char *name, VD(ProcTest) *proc)
{
    if ((VDI(Len_Test_Entries) + 1) > VDI(Cap_Test_Entries)) {
        VD(usize) old_cap = VDI(Cap_Test_Entries);
        VD(usize) new_cap;
        if (old_cap == 0) {
            new_cap = 64;
        } else {
            new_cap = 2 * old_cap;
        }

        VDI(Test_Entries) = VD_REALLOC(VDI(Test_Entries), old_cap * sizeof(VD(TestEntry)), new_cap * sizeof(VD(TestEntry)));
        VDI(Cap_Test_Entries) = new_cap;
    }

    VD(TestEntry*) e = &VDI(Test_Entries)[VDI(Len_Test_Entries)++];
    e->name = name;
    e->test = proc;
}

#define VD_TEST_IMPL(string, counter) \
    static VD_PROC_TEST(VD_TEST_PROC_ID(counter)); \
    static void __attribute__((constructor)) VD_TEST_REG_ID(counter)() { \
        VDI(test_register)(string, VD_TEST_PROC_ID(counter)); \
    } \
    static VD_PROC_TEST(VD_TEST_PROC_ID(counter))

#endif  // VD_HOST_COMPILER_MSVC, VD_HOST_COMPILER_CLANG


#define VD_TEST(string) VD_TEST_IMPL(string, __COUNTER__)

extern void VDF(test_main)(int argc, char **argv);

#define VD_TEST_ERR(msg)          return ((VD(TestResult)) { .ok = 0, .err = msg })
#define VD_TEST_OK()              return ((VD(TestResult)) { .ok = 1, .err = 0 })
#define VD_TEST_ASSERT(desc, x)   do { if (!(x))       { VD_TEST_ERR(desc "\nExpected: " #x " would be true");  } } while (0)
#define VD_TEST_TRUE(desc, x)     do { if (!(x))       { VD_TEST_ERR(desc "\nExpected: " #x " == true");        } } while (0)
#define VD_TEST_FALSE(desc, x)    do { if ( (x))       { VD_TEST_ERR(desc "\nExpected: " #x " == false");       } } while (0)
#define VD_TEST_EQ(desc, x, y)    do { if ((x) != (y)) { VD_TEST_ERR(desc "\nExpected: " #x " == " #y );        } } while (0)
#define VD_TEST_NEQ(desc, x, y)   do { if ((x) == (y)) { VD_TEST_ERR(desc "\nExpected: " #x " != " #y );        } } while (0)
#define VD_TEST_LT(desc, x, y)    do { if ((x) >= (y)) { VD_TEST_ERR(desc "\nExpected: " #x " < " #y );         } } while (0)
#define VD_TEST_GT(desc, x, y)    do { if ((x) <= (y)) { VD_TEST_ERR(desc "\nExpected: " #x " > " #y );         } } while (0)
#define VD_TEST_LE(desc, x, y)    do { if ((x) >  (y)) { VD_TEST_ERR(desc "\nExpected: " #x " <= " #y );        } } while (0) 
#define VD_TEST_GE(desc, x, y)    do { if ((x) <  (y)) { VD_TEST_ERR(desc "\nExpected: " #x " >= " #y );        } } while (0) 

#endif // VD_INCLUDE_TESTS && !VD_HOST_COMPILER_UNKNOWN

// Set this to 1 to include vd.h tests
#ifndef VD_INCLUDE_INTERNAL_TESTS
#define VD_INCLUDE_INTERNAL_TESTS 0
#endif // !VD_INCLUDE_INTERNAL_TESTS


#endif // !VD_H

/* ------------------------------------------------------------------------------------------------------------------ */
/* ----IMPLEMENTATION------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------ */

#ifdef VD_IMPL

/* ----VIRTUAL MEMORY IMPL------------------------------------------------------------------------------------------- */
#if !VD_VM_CUSTOM

#if VD_PLATFORM_MACOS
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>

VD(usize) VDF(vm_get_page_size)()
{
    return _SC_PAGE_SIZE;
}

void *VDF(vm_reserve)(VD(usize) len)
{
    void *result = mmap(0, len, PROT_NONE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (result == MAP_FAILED) {
        fprintf(stderr, "vm_decommit failed: %d\n", errno);
        VD_ASSERT(VD_FALSE);
    }
    return result;
}

void *VDF(vm_commit)(void *addr, VD(usize) len)
{
    void *result = mmap(addr, len, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_PRIVATE | MAP_ANON, -1, 0);
    if (result == MAP_FAILED) {
        fprintf(stderr, "vm_decommit failed: %d\n", errno);
        VD_ASSERT(VD_FALSE);
    }
    return result;
}

void *VDF(vm_decommit)(void *addr, VD(usize) len)
{
    void *result = mmap(addr, len, PROT_NONE, MAP_FIXED | MAP_PRIVATE | MAP_ANON, -1, 0);
    if (result == MAP_FAILED) {
        fprintf(stderr, "vm_decommit failed: %d\n", errno);
        VD_ASSERT(VD_FALSE);
    }
    return result;
}

void VDF(vm_release)(void *addr, VD(usize) len)
{
    munmap(addr, len);
}

#endif // VD_PLATFORM_MACOS

#endif // !VD_VM_CUSTOM

/* ----ARENA IMPL---------------------------------------------------------------------------------------------------- */
void VDF(arena_init)(VD(Arena) *a, void *buf, size_t len)
{
    a->buf = buf;
    a->buf_len = len;
    a->curr_offset = 0;
    a->prev_offset = 0;
}

void *VDF(arena_alloc_align)(VD(Arena) *a, size_t size, size_t align)
{
    uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
    uintptr_t offset = VDF(vd_align_forward)(curr_ptr, align);
    offset -= (VD(uptr))a->buf;

    if (offset + size <= a->buf_len) {
        void *ptr;
        if (a->flags & VD_(ARENA_FLAGS_USE_MALLOC)) {
            ptr = VD_MALLOC(size);
        } else {
            ptr = &a->buf[offset];
            a->prev_offset = offset;
            a->curr_offset = offset + size;
        }

        VD_MEMSET(ptr, 0, size);
        return ptr;
    }

    VD_DEBUG_BREAK();
}

void *VDF(arena_resize_align)(VD(Arena) *a, void *old_memory, size_t old_size, size_t new_size, size_t align)
{
    VD_ASSERT(VDF(is_power_of_two)(align));

    VD(u8)* old_mem = (VD(u8)*)old_memory;

    if (old_mem == 0 || old_size == 0) {
        return VDF(arena_alloc_align)(a, new_size, align);
    } else if (a->buf <= old_mem && old_mem < a->buf + a->buf_len) {
        if (a->buf + a->prev_offset == old_mem) {
            a->curr_offset = a->prev_offset + new_size;
            if (new_size > old_size) {
                VD_MEMSET(&a->buf[a->curr_offset], 0, new_size - old_size);
            }

            return old_memory;
        } else {
            void *new_memory = VDF(arena_alloc_align)(a, new_size, align);
            VD(usize) copy_size = old_size < new_size ? old_size : new_size;

            VD_MEMMOVE(new_memory, old_memory, copy_size);
            return new_memory;
        }
    } else {
        VD_DEBUG_BREAK();
    }
}

void VDF(arena_clear)(VD(Arena) *a)
{
    VD_MEMSET(a->buf, 0, a->curr_offset);
    a->curr_offset = 0;
    a->prev_offset = 0;
}

VD(b32) VDF(arena_free)(VD(Arena) *a, void *memory, size_t size)
{
    VD(uptr) last_ptr = (VD(uptr)) (a->buf + a->prev_offset);
    VD(uptr) mptr     = (VD(uptr))memory;

    if (mptr == last_ptr) {
        a->curr_offset = a->prev_offset;
        a->prev_offset = 0;
        return VD_TRUE;
    }

    return VD_FALSE;
}

/* ----SCRATCH IMPL-------------------------------------------------------------------------------------------------- */
void VDF(scratch_init)(VD(Scratch) *scratch)
{
    for (VD(usize) i = 0; i < VD_SCRATCH_PAGE_COUNT; ++i)
    {
        void *a = VD_MALLOC(VD_SCRATCH_PAGE_SIZE);
        VD_MEMSET(a, 0, VD_SCRATCH_PAGE_SIZE);
        VDF(arena_init)(&scratch->arenas[i], a, VD_SCRATCH_PAGE_SIZE);
    }
}

VD(Arena) *VDF(scratch_get_arena)(VD(Scratch) *scratch)
{
    VD_ASSERT(scratch->curr_arena < VD_SCRATCH_PAGE_COUNT);
    return &scratch->arenas[scratch->curr_arena++];
}

void VDF(scratch_return_arena)(VD(Scratch) *scratch, VD(Arena) *arena)
{
    VD_ASSERT(scratch->curr_arena > 0);
    scratch->curr_arena--;
    VD_ASSERT(&scratch->arenas[scratch->curr_arena] == arena);

    VD_MEMSET(scratch->arenas[scratch->curr_arena].buf, 0, scratch->arenas[scratch->curr_arena].buf_len);
    scratch->arenas[scratch->curr_arena].curr_offset = 0;
    scratch->arenas[scratch->curr_arena].prev_offset = 0;
}

/* ----THREAD CONTEXT IMPL------------------------------------------------------------------------------------------- */
VD_THREAD_CONTEXT_TYPE * VD_THREAD_CONTEXT_VARNAME;

/* ----PARSING IMPL-------------------------------------------------------------------------------------------------- */
VD(b32) VDF(parse_u64)(VD(Str) s, VD(u64) *r)
{
    VD(u64) result = 0;
    VD(usize) i = 0;
    while (i < s.len && VDF(is_ascii_digit)(s.s[i])) {
        result = result * 10 + (s.s[i] - '0');
        i++;
    } 

    if (i != s.len) {
        return VD_FALSE;
    }

    *r = result;

    return VD_TRUE;
}

/* ----ARG IMPL------------------------------------------------------------------------------------------------------ */
VD(b32) VDI(arg_advance)(VD(Arg) *arg) {
    if ((arg->argi + 1) == arg->argc) {
        return VD_FALSE;
    } else {
        arg->argi++;
        arg->ci = 0;
        return VD_TRUE;
    }
}

#define VD_ARG_CHECK_NEXT(arg) do {                      \
        if (arg->ci != 0) {                              \
            if (VDF(arg_at_end)(arg)) return VD_FALSE;   \
            if (!VDI(arg_advance)(arg)) return VD_FALSE; \
        }                                                \
    } while(0)

VD(Arg) VDF(arg_new)(int argc, char **argv)
{
    return (VD(Arg)) {
        .argc    = argc,
        .argv    = argv,
        .argi    = 0,
        .ci      = 0,
    };
}

void VDF(arg_skip_program_name)(VD(Arg) *arg)
{
   arg->argi++;
   arg->ci = 0;
}

VD(b32) VDF(arg_at_end)(VD(Arg) *arg)
{
    return (arg->argi == arg->argc) ||
           (arg->argi == (arg->argc - 1) &&
            arg->argv[arg->argi][arg->ci] == 0);
}

void VDF(arg_skip)(VD(Arg) *arg)
{
    arg->argi++;
    arg->ci = 0;    
}

VD(b32) VDF(arg_get_name)(VD(Arg) *arg, VD(Str) *name)
{
    VD_ARG_CHECK_NEXT(arg);

    if (arg->argv[arg->argi][arg->ci] != '-') {
        return VD_FALSE;
    }

    arg->ci = 1;

    VD(Str) result;
    result.s = &arg->argv[arg->argi][1];
    result.len = 0;

    while (arg->argv[arg->argi][arg->ci] != 0) {
        arg->ci++;
        result.len++;
    }

    if (result.len == 0) {
        return VD_FALSE;
    }

    *name = result;

    return VD_TRUE;
}

VD(b32) VDF(arg_get_uint)(VD(Arg) *arg, VD(u64) *i) {
    VD_ARG_CHECK_NEXT(arg);

    return VD_FALSE;
}

VD(b32) VDF(arg_get_str)(VD(Arg) *arg, VD(Str) *str) {
    VD_ARG_CHECK_NEXT(arg);

    VD(Str) result;
    result.s = &arg->argv[arg->argi][0];
    result.len = 0;

    while (arg->argv[arg->argi][arg->ci] != 0) {
        arg->ci++;
        result.len++;
    }

    *str = result;
    return VD_TRUE;
}

VD(b32) VDF(arg_expect_char)(VD(Arg) *arg, char c);

#undef VD_ARG_CHECK_NEXT

/* ----STRMAP IMPL--------------------------------------------------------------------------------------------------- */
// #define VD_STRMAP_GET_BIN_AT(m, i) \
//     ((VDI(StrmapBinPrefix)*)((VD(u8)*)m) + (VD_STRMAP_HEADER(m)->tsize + sizeof(VDI(StrmapBinPrefix)) * i))

#define VD_STRMAP_GET_BIN_AT(m, i) VD_STRMAP_GET_BIN(m, i)

static VD(b32) VDI(strmap_check_key)(VD(Str) check, VDI(StrmapBinPrefix) *against) {
    VD(u32) prefix_len       = sizeof(against->key_prefix);    
    VD(u32) first_check_len  = prefix_len < check.len ? prefix_len : check.len;
    VD(u32) second_check_len = check.len - prefix_len;

    if (against->key_len != check.len) {
        return VD_FALSE;
    }

    if (VD_MEMCMP(against->key_prefix, check.s, first_check_len)) {
        return VD_FALSE;
    }

    return check.len < prefix_len
        ? VD_TRUE
        : (VD_MEMCMP(against->key_rest, check.s + prefix_len, second_check_len) == 0);
}


static void VDI(strmap_copy_key_old)(void *map, VD(Str) key, VDI(StrmapBinPrefix) *bin)
{
    VD(u32) prefix_len = sizeof(bin->key_prefix);
    VD(u32) key_len    = key.len;
    bin->key_len       = key_len;

    VD(u32) first_copy_len = prefix_len < key_len ? prefix_len : key_len;

    VD_MEMCPY(bin->key_prefix, key.s, first_copy_len);

    if (key_len > prefix_len) {
        bin->key_rest = (char*)VDF(arena_alloc)(VD_STRMAP_HEADER(map)->arena, key_len - prefix_len);
        VD_MEMCPY(bin->key_rest, key.s + prefix_len, key_len - prefix_len);
    }
}

static void VDI(strmap_emplace_key)(void *map, VDI(StrmapBinPrefix) *dst, char *src_key_prefix, char *src_key_rest, VD(u32) src_key_len)
{
    VD(u32) prefix_len     = sizeof(dst->key_prefix);
    VD(u32) first_copy_len = src_key_len > prefix_len ? prefix_len : src_key_len;

    VD_MEMCPY(dst->key_prefix, src_key_prefix, first_copy_len);

    VD(u32) new_len = src_key_len;

    if (src_key_len > prefix_len) {
        src_key_len -= prefix_len;

        // If the source key's length after the prefix doesn't fit in the array allocated for key_rest
        // Or if the key_rest_cap is zero because no key_rest has been allocated, then resize it
        if (src_key_len > dst->key_rest_cap) {

            // Allocate an arena big enough so that statistically, we won't have to ever care about
            // this again
            dst->key_rest = VDF(arena_alloc)(VD_STRMAP_ARENAP(map), src_key_len * 2);
            dst->key_rest_cap = src_key_len * 2;
        }

        // Copy the rest of the key
        VD_MEMCPY(dst->key_rest, src_key_rest, src_key_len);
    }

    dst->key_len = new_len;
}

static void VDI(strmap_copy_key)(void *map, VD(Str) key, VDI(StrmapBinPrefix) *bin)
{
    char *key_prefix_part = key.s;
    char *key_rest_part = key.len > sizeof(bin->key_prefix) ? key.s + sizeof(bin->key_prefix) : 0;
    VDI(strmap_emplace_key)(map, bin, key_prefix_part, key_rest_part, key.len);
}

void* VDI(strmap_init)(VD(Arena) *arena, VD(u32) tsize, VD(u32) cap, VD(StrmapInitOptions) *options)
{
    VDI(StrmapHeader) *map;
    const VD(u32) bin_size = sizeof(VDI(StrmapBinPrefix)) + tsize;

    map = VDF(arena_alloc)(arena, sizeof(VDI(StrmapHeader)) + bin_size * cap);

    VDI(StrmapBinPrefix) *bins = (VDI(StrmapBinPrefix)*)(((VD(u8ptr))map) + sizeof(VDI(StrmapHeader)));

    float address_scale = 0.863f;
    if (options != 0) address_scale = options->address_scale;

    map->cap_total    = cap;
    map->cap          = (VD(u32)) (((VD(f32))cap) * address_scale);
    map->taken        = 0;
    map->tsize        = tsize;
    map->arena        = arena;

    if ((options != 0) && (options->average_key_len > sizeof(((VDI(StrmapBinPrefix)*)0)->key_prefix))) {
        for (VD(u32) i = 0; i < map->cap_total; ++i) {
            bins[i].key_rest = VDF(arena_alloc)(arena, options->average_key_len);
            VD_ASSERT(bins[i].key_rest);

            bins[i].key_rest_cap = options->average_key_len;
        }
    }

    return (void*)((VD(u8)*)map + sizeof(VDI(StrmapHeader)));
}

// @todo(mdodis): Speed this up if VD_ENABLE_SCRATCH_USE_IN_LIBRARY is enabled
//                by using a simple queue to store re-inserted bins
static void VDI(strmap_reinsert_chain)(void *map, VDI(StrmapBinPrefix) *start)
{
    // Build the insertion queue
    VDI(StrmapBinPrefix) *bin = start;
    VDI(StrmapBinPrefix) *bin_next = start->next;
    while (bin_next != 0) {
        bin->insq = bin_next;

        bin       = bin_next;
        bin_next  = bin->next;
    }

    // Traverse the queue and for each item, set its bin to be unused, then re-add it
    VDI(StrmapBinPrefix) *q = start->insq;
    bin = start;

    start->insq = 0;

    while (q != 0) {
        q->used = VD_FALSE;
        VDI(StrmapBinPrefix) *nextq = q->insq;
        q->insq = 0;

        // Compose the key for q into a VD(Str)
        VD(u64) key_hash;
        {
            VD(usize) key_len = q->key_len;
            VD(ArenaSave) save = VDF(arena_save)(VD_STRMAP_ARENAP(map));

            char *key = VDF(arena_alloc)(VD_STRMAP_ARENAP(map), key_len);

            VD(usize) first_copy_size  = key_len <= sizeof(q->key_prefix) ? key_len : sizeof(q->key_prefix);
            VD(usize) second_copy_size = key_len <= sizeof(q->key_prefix) ? 0 : key_len - sizeof(q->key_prefix);

            VD_MEMCPY(key + 0, q->key_prefix, first_copy_size);

            if (second_copy_size > 0) {
                VD_MEMCPY(key + first_copy_size, q->key_rest, second_copy_size);
            }

            key_hash = VDF(dhash64)(key, key_len);

            VDF(arena_restore)(save);
        }

        VD(usize) bin_index = key_hash % VD_STRMAP_HEADER(map)->cap;

        VDI(StrmapBinPrefix) *existing_bin = VD_STRMAP_GET_BIN(map, bin_index);

        // if the key for some reason hashes to the same place, then ignore it
        if (existing_bin != q) {

            if (!existing_bin->used) {
                VDI(StrmapBinPrefix) *new_bin = existing_bin;
                void *sptr = VD_STRMAP_BIN_MOVE_TO_VPTR(q);
                void *dptr = VD_STRMAP_BIN_MOVE_TO_VPTR(new_bin);

                new_bin->used = VD_TRUE;
                new_bin->next = 0;
                // Copy key
                VDI(strmap_emplace_key)(map, new_bin, q->key_prefix, q->key_rest, q->key_len);

                // Copy over value
                VD_MEMCPY(dptr, sptr, VD_STRMAP_TSIZE(map));
            } else {
                // Attempt to traverse the chain, if any
                VDI(StrmapBinPrefix) *chain_head = existing_bin;
                while (chain_head->next == 0) {
                    chain_head = chain_head->next;
                }

                VD(u32) cursor = VD_STRMAP_HEADER(map)->cap_total;
                while ((cursor > 0) && (VD_STRMAP_GET_BIN_AT(map, cursor - 1)->used)) {
                    cursor--;
                }

                // After n iterations, if no bin was found we should be at a bin that's still used
                // (i.e bin 0). In this case, return null.
                if (cursor == 0) {
                    VD_IMPOSSIBLE();
                }

                cursor--;
                VDI(StrmapBinPrefix) *new_bin = VD_STRMAP_GET_BIN_AT(map, cursor);

                // Allocate the bin
                new_bin->used = VD_TRUE;
                new_bin->next = 0;
                chain_head->next = new_bin;

                void *sptr = VD_STRMAP_BIN_MOVE_TO_VPTR(q);
                void *dptr = VD_STRMAP_BIN_MOVE_TO_VPTR(new_bin);

                // Copy key
                VDI(strmap_emplace_key)(map, new_bin, q->key_prefix, q->key_rest, q->key_len);

                // Copy over value
                VD_MEMCPY(dptr, sptr, VD_STRMAP_TSIZE(map));
            }
        } else {
            q->used = VD_TRUE;
        }

        q = nextq;
    }
}

VDI(StrmapBinPrefix)* VDI(strmap_get_bin)(void *map, VD(Str) key, VDI(StrmapGetBinFlags) op)
{
    // Get hash
    VD(u64) hash = VDF(dhash64_str)(key);
    VD(u64) bin_index = hash % VD_STRMAP_HEADER(map)->cap;

    // Find existing bin if any
    VDI(StrmapBinPrefix) *existing_bin = VD_STRMAP_GET_BIN_AT(map, bin_index);

    // If:
    // (existing_bin->used == 0) AND
    //      - We don't need to create a new bin then we want to search anyway since we'll do an early
    //        return after this if statement
    //      - We do need to create a new bin then we stop here, because we found it
    //
    // (existing_bin->used == 1) AND
    //      - We don't need to create a new bin, then go inside this statement to see if we can find
    //        the bin matching the query
    //      - We do need to create a new bin, then go inside anyway because at the end (meaning if
    //        we actually find a place to allocate the bin), we'll want to link the collided bin chain
    //        to the newly allocated bin
    if (existing_bin->used || !(op & VDI(STRMAP_GET_BIN_FLAGS_CREATE))) {
        VD(b32) found = VD_FALSE;


        // Make sure that the hash didn't put us in a bin where the key is equal to the query key
        if (VDI(strmap_check_key)(key, existing_bin)) {
            found = VD_TRUE;
        }

        // If the bin isn't what we searched for, traverse the chain until the end to find it.
        while (!found && (existing_bin->next != 0)) {
            existing_bin = existing_bin->next;

            if (VDI(strmap_check_key)(key, existing_bin)) {
                found = VD_TRUE;
                break;
            }
        }

        if (found) {
            if (op & VDI(STRMAP_GET_BIN_FLAGS_SET_UNUSED)) {
                // if found is true the bin must be unused, set its flag and return
                existing_bin->used = VD_FALSE;
                VD_STRMAP_HEADER(map)->taken--;
                VDI(strmap_reinsert_chain)(map, existing_bin);
                return existing_bin;

            } else if (op & VDI(STRMAP_GET_BIN_FLAGS_GET_EXISTING)) {
                // If the user wants to create a new bin, overwrite existing one, or just get it, and found is true, 
                // this means that the user queried with an existing key. Return the existing bin in thie case.
                return existing_bin;
            } else if (op & VDI(STRMAP_GET_BIN_FLAGS_CREATE)) {
                // If the user wants to create a new bin, and found is true, this means that the user
                // queried with the same key. Return null in this case.
                return 0;
            }
        }
    }

    // At this point, no bin that matched our key was found so if the user doesn't want a new bin, then
    // fail
    if (!(op & VDI(STRMAP_GET_BIN_FLAGS_CREATE))) {
        return 0;
    }

    // If this bin is not used then allocate it
    if (!existing_bin->used) {
        VDI(strmap_copy_key)(map, key, existing_bin);
        existing_bin->used = VD_TRUE;
        existing_bin->next = 0;
        VD_STRMAP_HEADER(map)->taken++;
        return existing_bin;
    }

    // At this point, we handle a hash collision either for allocating a bin, or traversing the chain
    // due to a previous collision

    // Otherwise search starting from the cellar for an unused bin
    VD(u32) cursor = VD_STRMAP_HEADER(map)->cap_total;
    while ((cursor > 0) && (VD_STRMAP_GET_BIN_AT(map, cursor - 1)->used)) {
        cursor--;
    }

    // After n iterations, if no bin was found we should be at a bin that's still used
    // (i.e bin 0). In this case, return null.
    if (cursor == 0) {
        return 0;
    }

    cursor--;
    VDI(StrmapBinPrefix) *new_bin = VD_STRMAP_GET_BIN_AT(map, cursor);

    // Allocate the bin
    new_bin->used = VD_TRUE;
    new_bin->next = 0;

    VD_STRMAP_HEADER(map)->taken++;

    // Mark bin as next. At this point, the previous search through the chain means that existing_bin
    // is now at the end of the chain 
    existing_bin->next = new_bin;

    // Copy new key and return
    VDI(strmap_copy_key)(map, key, new_bin);
    return new_bin;
}

#undef VD_STRMAP_GET_BIN_AT
/* ----TESTING IMPL-------------------------------------------------------------------------------------------------- */
#if VD_INCLUDE_TESTS


static int VDI(run_test)(VD(TestEntry)* e) {
    VDF(arena_clear)(Test_Arena);
    Test_Arena->flags = 0;

    if (!e || !e->name || !e->test) return -1;

    printf("[%-60s]", e->name);
    VD(TestResult) r = e->test();
    if (r.ok) {
        printf(" OK     \n");
        return 1;
    } else {
        printf(" FAILED \n");
        printf("%s\n", r.err);
        return 0;
    }
}

VD(Arena) *Test_Arena;

#if VD_HOST_COMPILER_CLANG
VD(TestEntry) *VDI(Test_Entries);
VD(usize)      VDI(Cap_Test_Entries);
VD(usize)      VDI(Len_Test_Entries);
#endif // VD_HOST_COMPILER_CLANG

#include <stdio.h>
void VDF(test_main)(int argc, char **argv)
{
    VD(Arena) a;
    Test_Arena = &a;

    VD_ARENA_FROM_SYSTEM(Test_Arena, VD_MEGABYTES(64));

    int passed = 0, total = 0;
#if VD_HOST_COMPILER_MSVC 
    VD(TestEntry) **start = &__start_vdtests + 1;
    VD(TestEntry) **end = &__end_vdtests;

    for (VD(TestEntry) **p = start; p < end; ++p) {
        VD(TestEntry) *t = *p;
        if (VDI(run_test)(t)) {
            passed++;
        }
        total++;
    }
#elif VD_HOST_COMPILER_CLANG
    for (VD(usize) i = 0; i < VDI(Len_Test_Entries); ++i) {
        if (VDI(run_test)(&VDI(Test_Entries)[i])) {
            passed++;
        }
        total++;
    }

    printf("[%d/%d]\n", passed, total);
#else
#error "Cannot produce tests for unknown compiler!"
#endif // VD_HOST_COMPILER_MSVC, VD_HOST_COMPILER_CLANG
}

#if VD_INCLUDE_INTERNAL_TESTS

VD_TEST("FixedArray/Basic")
{
    VD_FIXEDARRAY int *array = 0;
    VD_FIXEDARRAY_INIT(array, 30, Test_Arena);

    VD_TEST_EQ("Fixed array has correct capacity", VD_FIXEDARRAY_CAP(array), 30);

    for (int i = 0; i < 30; ++i) {
        VD_FIXEDARRAY_ADD(array, i);
        VD_TEST_EQ("Fixed array insertion is correct", array[i], i);
    }
    VD_TEST_EQ("Fixed array has correct length", VD_FIXEDARRAY_LEN(array), 30);

    VD_FIXEDARRAY_CLEAR(array);
    VD_TEST_EQ("Fixed array has correct length when cleared", VD_FIXEDARRAY_LEN(array), 0);

    for (int i = 0; i < 30; ++i) {
        VD_FIXEDARRAY_ADD(array, i);
        VD_TEST_EQ("Fixed array insertion after clear is ok", array[i], i);
    }
    VD_TEST_EQ("Fixed array has correct length after clear and repopulate", VD_FIXEDARRAY_LEN(array), 30);

    for (int i = 29; i >= 0; --i) {
        int c = VD_FIXEDARRAY_POP(array);
        VD_TEST_EQ("Array pop i is correct", c, i);
    }

    VD_TEST_EQ("Fixed array has correct length after n pops", VD_FIXEDARRAY_LEN(array), 0);

    VD_TEST_OK();
}

static void print_map(int *map) {
    puts("// ----------------------");
    for (int i = 0; i < (int)VD_STRMAP_TOTAL_CAP(map); ++i) {
        if (!VD_STRMAP_GET_BIN_USED(map, i)) {
            size_t index = 0;
            void *n = VD_STRMAP_GET_BIN_NEXT(map, i);
            if (n != 0) {
                index = VD_STRMAP_GET_BIN_INDEX(map, n);
                printf("// %d: [      ] -> %zu = _\n", i, index);
            } else {
                printf("// %d: [      ] -> _ = _\n", i);
            }
        } else {
            size_t index = 0;
            void *n = VD_STRMAP_GET_BIN_NEXT(map, i);
            if (n != 0) {
                index = VD_STRMAP_GET_BIN_INDEX(map, n);
                int *v = (int*) VD_STRMAP_GET_BIN_VPTR(map, i);
                printf("// %d: [%6.*s] -> %zu = %d\n", i, VD_STRMAP_GET_KEY_LEN(map, i), VD_STRMAP_GET_KEY_PREFIX(map, i), index, *v);
            } else {
                int *v = (int*) VD_STRMAP_GET_BIN_VPTR(map, i);
                printf("// %d: [%6.*s] -> _ = %d\n", i, VD_STRMAP_GET_KEY_LEN(map, i), VD_STRMAP_GET_KEY_PREFIX(map, i), *v);
            }
        }
    } 
    puts("// ----------------------");
}

typedef struct {
    VD(Str) key;
    int     val;
} VDI(TestCheckMapEntry);

#define VD__TEST_MAP_CHECK_ENTRIES_(map, entryarray) do { if (!VDI(map_check_entries)(map, (entryarray), VD_ARRAY_COUNT(entryarray), __FILE__, __LINE__)) { VD_TEST_ERR("Map value check failed."); } }while(0)
#define VD__TEST_MAP_CHECK_ENTRIES(map, ...) VD__TEST_MAP_CHECK_ENTRIES_(map, (((VDI(TestCheckMapEntry) []){ __VA_ARGS__ })))

static VD(b32) VDI(map_check_entries)(int *map, VDI(TestCheckMapEntry) *entries, int num_entries, const char *f, int l)
{
    for (int i = 0; i < num_entries; ++i) {
        int v;
        if (!VD_STRMAP_GET(map, entries[i].key, &v)) {
            printf("VDI(map_check_entries): %s:%d Key not found: %.*s\n", f, l, VD_STR_EXPAND(entries[i].key));
            return VD_FALSE;
        }

        if (v != entries[i].val) {
            printf("VDI(map_check_entries): %s:%d Key '%.*s' value expected: %d, got: %d found: \n", f, l, VD_STR_EXPAND(entries[i].key), entries[i].val, v);
            return VD_FALSE;    
        }
    }

    return VD_TRUE;
}

VD_TEST("Strmap/Basic") {

    Test_Arena->flags |= VD_(ARENA_FLAGS_USE_MALLOC);

    VD_STRMAP int *map = 0;
    VD_STRMAP_INIT(map, Test_Arena, 8, 0);

    int my_123 = 123;
    // ----------------------
    // 0: [      ] -> 0 = _
    // 1: [      ] -> 0 = _
    // 2: [      ] -> 0 = _
    // 3: [      ] -> 0 = _
    // 4: [my_123] -> 0 = 123
    // 5: [      ] -> 0 = _
    // 6: [      ] -> 0 = _
    // 7: [      ] -> 0 = _
    // ----------------------
    VD_TEST_TRUE("Setting a value in the map works", VD_STRMAP_SET(map, LIT("my_123"), &my_123));

    VD__TEST_MAP_CHECK_ENTRIES(map, 




        { LIT("my_123"), 123 },



    );

    VD_TEST_TRUE("Getting a value from the map works", VD_STRMAP_GET(map, LIT("my_123"), &my_123));
    VD_TEST_EQ("The fetched value from the map should be 123", my_123, 123);

    my_123 = 100;
    VD_TEST_FALSE("Writing to the same value should print an error", VD_STRMAP_SET(map, LIT("my_123"), &my_123));

    VD__TEST_MAP_CHECK_ENTRIES(map, 




        { LIT("my_123"), 123 },


        
    );

    VD_TEST_TRUE("Getting a value from the map works after attempting to write", VD_STRMAP_GET(map, LIT("my_123"), &my_123));
    VD_TEST_EQ("The fetched value from the map should be 123 even after an invalid operation", my_123, 123);

    // ----------------------
    // 0: [      ] -> 0 = _
    // 1: [      ] -> 0 = _
    // 2: [      ] -> 0 = _
    // 3: [      ] -> 0 = _
    // 4: [my_123] -> 0 = 321
    // 5: [      ] -> 0 = _
    // 6: [      ] -> 0 = _
    // 7: [      ] -> 0 = _
    // ----------------------
    my_123 = 321;
    VD_TEST_TRUE("Overwriting a value with same key should work", VD_STRMAP_OVERWRITE(map, LIT("my_123"), &my_123));
    VD_TEST_TRUE("Getting a value from the map works after overwrite", VD_STRMAP_GET(map, LIT("my_123"), &my_123));
    VD_TEST_EQ("The fetched value from the map should be 321 after overwrite", my_123, 321);

    VD__TEST_MAP_CHECK_ENTRIES(map, 




        { LIT("my_123"), 321 },


        
    );

    // ----------------------
    // 0: [ other] -> 0 = -40
    // 1: [      ] -> 0 = _
    // 2: [      ] -> 0 = _
    // 3: [      ] -> 0 = _
    // 4: [my_123] -> 0 = 321
    // 5: [      ] -> 0 = _
    // 6: [      ] -> 0 = _
    // 7: [      ] -> 0 = _
    // ----------------------
    int other = -40;
    VD_TEST_TRUE("Overwriting a value with a new key should work", VD_STRMAP_OVERWRITE(map, LIT("other"), &other));
    VD_TEST_TRUE("Getting a value from the map works after overwrite to a new key", VD_STRMAP_GET(map, LIT("other"), &other));
    VD_TEST_EQ("The fetched value from the map should be -40 after overwrite", other, -40);

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { LIT("other"),  -40 },



        { LIT("my_123"), 321 },



    );

    // ----------------------
    // 0: [ other] -> 0 = -40
    // 1: [      ] -> 0 = _
    // 2: [      ] -> 0 = _
    // 3: [     3] -> 0 = 3
    // 4: [my_123] -> 0 = 321
    // 5: [      ] -> 0 = _
    // 6: [      ] -> 0 = _
    // 7: [      ] -> 0 = _
    // ----------------------
    int num = 3;
    VD_TEST_TRUE("Writing 3rd value to a map with size 2/8 should work", VD_STRMAP_SET(map, LIT("3"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { LIT("other"),  -40 },


        {     LIT("3"),    3 },
        { LIT("my_123"), 321 },



    );

    // ----------------------
    // 0: [ other] -> 0 = -40
    // 1: [      ] -> 0 = _
    // 2: [      ] -> 0 = _
    // 3: [     3] -> 7 = 3
    // 4: [my_123] -> 0 = 321
    // 5: [      ] -> 0 = _
    // 6: [      ] -> 0 = _
    // 7: [     4] -> 0 = 4
    // ----------------------
    VD_TEST_TRUE("Writing 4th value to a map with size 3/8 should work", VD_STRMAP_SET(map, LIT("4"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { LIT("other"),  -40 },


        {     LIT("3"),    3 },
        { LIT("my_123"), 321 },


        {     LIT("4"),    4 },
    );

    // ----------------------
    // 0: [ other] -> 0 = -40
    // 1: [      ] -> 0 = _
    // 2: [     5] -> 0 = 5
    // 3: [     3] -> 7 = 3
    // 4: [my_123] -> 0 = 321
    // 5: [      ] -> 0 = _
    // 6: [      ] -> 0 = _
    // 7: [     4] -> 0 = 4
    // ----------------------
    VD_TEST_TRUE("Writing 5th value to a map with size 4/8 should work", VD_STRMAP_SET(map, LIT("5"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { LIT("other"),  -40 },

        {     LIT("5"),    5 },
        {     LIT("3"),    3 },
        { LIT("my_123"), 321 },


        {     LIT("4"),    4 },
    );

    // ----------------------
    // 0: [ other] -> 6 = -40
    // 1: [      ] -> 0 = _
    // 2: [     5] -> 0 = 5
    // 3: [     3] -> 7 = 3
    // 4: [my_123] -> 0 = 321
    // 5: [      ] -> 0 = _
    // 6: [     6] -> 0 = 6
    // 7: [     4] -> 0 = 4    
    // ----------------------
    VD_TEST_TRUE("Writing 6th value to a map with size 5/8 should work", VD_STRMAP_SET(map, LIT("6"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { LIT("other"),  -40 },

        {     LIT("5"),    5 },
        {     LIT("3"),    3 },
        { LIT("my_123"), 321 },

        {     LIT("6"),    6 },
        {     LIT("4"),    4 },
    );

    // ----------------------
    // 0: [ other] -> 6 = -40
    // 1: [      ] -> 0 = _
    // 2: [     5] -> 0 = 5
    // 3: [     3] -> 7 = 3
    // 4: [my_123] -> 0 = 321
    // 5: [     7] -> 0 = 7
    // 6: [     6] -> 0 = 6
    // 7: [     4] -> 0 = 4
    // ----------------------
    VD_TEST_TRUE("Writing 7th value to a map with size 6/8 should work", VD_STRMAP_SET(map, LIT("7"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { LIT("other"),  -40 },

        {     LIT("5"),    5 },
        {     LIT("3"),    3 },
        { LIT("my_123"), 321 },
        {     LIT("7"),    7 },
        {     LIT("6"),    6 },
        {     LIT("4"),    4 },
    );

    // ----------------------
    // 0: [ other] -> 6 = -40
    // 1: [     8] -> 0 = 8
    // 2: [     5] -> 0 = 5
    // 3: [     3] -> 7 = 3
    // 4: [my_123] -> 0 = 321
    // 5: [     7] -> 1 = 7
    // 6: [     6] -> 0 = 6
    // 7: [     4] -> 0 = 4    
    // ----------------------
    VD_TEST_TRUE("Writing 8th value to a map with size 7/8 should work", VD_STRMAP_SET(map, LIT("8"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { LIT("other"),  -40 },
        {     LIT("8"),    8 },
        {     LIT("5"),    5 },
        {     LIT("3"),    3 },
        { LIT("my_123"), 321 },
        {     LIT("7"),    7 },
        {     LIT("6"),    6 },
        {     LIT("4"),    4 },
    );

    // ----------------------
    // 0: [ other] -> 6 = -40
    // 1: [     8] -> 0 = 8
    // 2: [     5] -> 0 = 5
    // 3: [     3] -> 7 = 3
    // 4: [my_123] -> 0 = 321
    // 5: [     7] -> 1 = 7
    // 6: [     6] -> 0 = 6
    // 7: [     4] -> 0 = 4
    // ----------------------
    VD_TEST_FALSE("Writing 9th value to a map with size 8/8 should fail", VD_STRMAP_SET(map, LIT("9"), &num));

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { LIT("other"),  -40 },
        {     LIT("8"),    8 },
        {     LIT("5"),    5 },
        {     LIT("3"),    3 },
        { LIT("my_123"), 321 },
        {     LIT("7"),    7 },
        {     LIT("6"),    6 },
        {     LIT("4"),    4 },
    );

    // ----------------------
    // 0: [ other] -> 6 = -40
    // 1: [     8] -> 0 = 8
    // 2: [     5] -> 0 = 5
    // 3: [     3] -> 7 = 3
    // 4: [      ] -> 0 = _
    // 5: [     7] -> 1 = 7
    // 6: [     6] -> 0 = 6
    // 7: [     4] -> 0 = 4
    // ----------------------
    VD_TEST_TRUE("Remove my_123 should work", VD_STRMAP_RM(map, LIT("my_123")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { LIT("other"),  -40 },
        {     LIT("8"),    8 },
        {     LIT("5"),    5 },
        {     LIT("3"),    3 },

        {     LIT("7"),    7 },
        {     LIT("6"),    6 },
        {     LIT("4"),    4 },
    );

    // ----------------------
    // 0: [ other] -> 6 = -40
    // 1: [     8] -> 0 = 8
    // 2: [     5] -> 0 = 5
    // 3: [     3] -> 7 = 3
    // 4: [      ] -> 0 = _
    // 5: [     7] -> 1 = 7
    // 6: [     6] -> 0 = 6
    // 7: [     4] -> 0 = 4
    // ----------------------
    VD_TEST_FALSE("Remove non existing key should fail", VD_STRMAP_RM(map, LIT("non")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { LIT("other"),  -40 },
        {     LIT("8"),    8 },
        {     LIT("5"),    5 },
        {     LIT("3"),    3 },

        {     LIT("7"),    7 },
        {     LIT("6"),    6 },
        {     LIT("4"),    4 },
    );

    // ----------------------
    // 0: [      ] -> 6 = _
    // 1: [     8] -> 0 = 8
    // 2: [     5] -> 0 = 5
    // 3: [     3] -> 7 = 3
    // 4: [      ] -> 0 = _
    // 5: [     7] -> 1 = 7
    // 6: [     6] -> 0 = 6
    // 7: [     4] -> 0 = 4
    // ----------------------
    VD_TEST_TRUE("Remove other should work", VD_STRMAP_RM(map, LIT("other")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     LIT("8"),    8 },
        {     LIT("5"),    5 },
        {     LIT("3"),    3 },

        {     LIT("7"),    7 },
        {     LIT("6"),    6 },
        {     LIT("4"),    4 },
    );

    // ----------------------
    // 0: [      ] -> 6 = _
    // 1: [     8] -> 0 = 8
    // 2: [     5] -> 0 = 5
    // 3: [      ] -> 7 = _
    // 4: [      ] -> 0 = _
    // 5: [     7] -> 1 = 7
    // 6: [     6] -> 0 = 6
    // 7: [     4] -> 0 = 4
    // ----------------------
    VD_TEST_TRUE("Remove 3 should work", VD_STRMAP_RM(map, LIT("3")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     LIT("8"),    8 },
        {     LIT("5"),    5 },


        {     LIT("7"),    7 },
        {     LIT("6"),    6 },
        {     LIT("4"),    4 },
    );

    // ----------------------
    // 0: [      ] -> 6 = _
    // 1: [     8] -> 0 = 8
    // 2: [     5] -> 0 = 5
    // 3: [      ] -> 7 = _
    // 4: [      ] -> 0 = _
    // 5: [     7] -> 1 = 7
    // 6: [     6] -> 0 = 6
    // 7: [      ] -> 0 = _
    // ----------------------
    VD_TEST_TRUE("Remove 4 should work", VD_STRMAP_RM(map, LIT("4")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     LIT("8"),    8 },
        {     LIT("5"),    5 },


        {     LIT("7"),    7 },
        {     LIT("6"),    6 },

    );

    // ----------------------
    // 0: [      ] -> 6 = _
    // 1: [     8] -> 0 = 8
    // 2: [      ] -> 0 = _
    // 3: [      ] -> 7 = _
    // 4: [      ] -> 0 = _
    // 5: [     7] -> 1 = 7
    // 6: [     6] -> 0 = 6
    // 7: [      ] -> 0 = _
    // ----------------------
    VD_TEST_TRUE("Remove 5 should work", VD_STRMAP_RM(map, LIT("5")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     LIT("8"),    8 },



        {     LIT("7"),    7 },
        {     LIT("6"),    6 },

    );

    // ----------------------
    // 0: [      ] -> 6 = _
    // 1: [     8] -> 0 = 8
    // 2: [      ] -> 0 = _
    // 3: [      ] -> 7 = _
    // 4: [      ] -> 0 = _
    // 5: [     7] -> 1 = 7
    // 6: [      ] -> 0 = _
    // 7: [      ] -> 0 = _
    // ----------------------
    VD_TEST_TRUE("Remove 6 should work", VD_STRMAP_RM(map, LIT("6")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     LIT("8"),    8 },



        {     LIT("7"),    7 },


    );

    // ----------------------
    // 0: [      ] -> 6 = _
    // 1: [     8] -> 0 = 8
    // 2: [      ] -> 0 = _
    // 3: [      ] -> 7 = _
    // 4: [      ] -> 0 = _
    // 5: [      ] -> 1 = _
    // 6: [      ] -> 0 = _
    // 7: [      ] -> 0 = _
    // ----------------------
    VD_TEST_TRUE("Remove 7 should work", VD_STRMAP_RM(map, LIT("7")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     LIT("8"),    8 },






    );

    // ----------------------
    // 0: [      ] -> 6 = _
    // 1: [      ] -> 0 = _
    // 2: [      ] -> 0 = _
    // 3: [      ] -> 7 = _
    // 4: [      ] -> 0 = _
    // 5: [      ] -> 1 = _
    // 6: [      ] -> 0 = _
    // 7: [      ] -> 0 = _
    // ----------------------
    VD_TEST_TRUE("Remove 8 should work", VD_STRMAP_RM(map, LIT("8")));

    VD_TEST_OK();
}

#undef VD__TEST_MAP_CHECK_ENTRIES_
#undef VD__TEST_MAP_CHECK_ENTRIES

#endif // VD_INCLUDE_INTERNAL_TESTS
#endif // VD_INCLUDE_TESTS
#endif // VD_IMPL