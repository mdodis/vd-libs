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
 *
 * @table Sections
 * | Name              | Description                                     | Status |
 * | ----------------- | ----------------------------------------------- | ------ |
 * | MACRO HELPERS     | Various macros for building other ones          | DONE   |
 * | ----------------  | ----------------------------------------------- | ------ |
 * | HOST COMPILER     |                                                 |        |
 * | ----------------  | ----------------------------------------------- | ------ |
 */
#ifndef VD_H
#define VD_H
#define VD_VERSION_MAJOR    0
#define VD_VERSION_MINOR    0
#define VD_VERSION_PATCH    3
#define VD_VERSION          ((VD_VERSION_MAJOR << 16) | (VD_VERSION_MINOR << 8) | (VD_VERSION_PATCH))

#include <stdint.h>
#include <stddef.h>

/* ----MACRO HELPERS------------------------------------------------------------------------------------------------- */
#ifndef VD_MACRO_ABBREVIATIONS
#define VD_MACRO_ABBREVIATIONS 0
#endif // !VD_MACRO_ABBREVIATIONS

#define _VD_STRING_JOIN2(x, y) x##y
#define VD_STRING_JOIN2(x, y) _VD_STRING_JOIN2(x, y)

#define _VD_STRINGIFY(x) #x
#define VD_STRINGIFY(x) _VD_STRINGIFY(x)

/**
 * @brief Indicates this parameter is unused.
 */
#define VD_UNUSED(x) (void)(x)

#define VD_OFFSET_OF(type, element) ((Vdusize) & (((type*)0)->element))

#define VD_CONTAINER_OF(ptr, type, member) \
    (type*)(((Vdu8ptr)ptr) - ((Vdusize)VD_OFFSET_OF(type, member)))

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
#define VD_PLATFORM_KNOWN 1
#endif // defined(_WIN32) || defined(_WIN64)

#if defined(__linux__)
#define VD_PLATFORM_LINUX 1
#define VD_PLATFORM_KNOWN 1
#endif // defined(__linux__)

#if defined(__APPLE__)
#define VD_PLATFORM_MACOS 1
#define VD_PLATFORM_KNOWN 1
#endif // defined(__APPLE__)

#ifndef VD_PLATFORM_WINDOWS
#define VD_PLATFORM_WINDOWS 0
#endif // !VD_PLATFORM_WINDOWS

#ifndef VD_PLATFORM_LINUX
#define VD_PLATFORM_LINUX 0
#endif // !VD_PLATFORM_LINUX

#ifndef VD_PLATFORM_MACOS
#define VD_PLATFORM_MACOS 0
#endif // !VD_PLATFORM_MACOS

#ifndef VD_PLATFORM_KNOWN
#define VD_PLATFORM_KNOWN 0
#endif // !VD_PLATFORM_KNOWN

#ifndef VD_INCLUDE_PLATFORM_SPECIFIC_FUNCTIONALITY
#define VD_INCLUDE_PLATFORM_SPECIFIC_FUNCTIONALITY 1
#endif // !VD_INCLUDE_PLATFORM_SPECIFIC_FUNCTIONALITY

/* ----TYPES--------------------------------------------------------------------------------------------------------- */
typedef uint8_t       Vdu8;
typedef uint8_t*      Vdu8ptr;
typedef uint16_t      Vdu16;
typedef uint16_t*     Vdu16ptr;
typedef uint32_t      Vdu32;
typedef uint32_t*     Vdu32ptr;
typedef uint64_t      Vdu64;
typedef uint64_t*     Vdu64ptr;
typedef int8_t        Vdi8;
typedef int8_t*       Vdi8ptr;
typedef int16_t       Vdi16;
typedef int16_t*      Vdi16ptr;
typedef int32_t       Vdi32;
typedef int32_t*      Vdi32ptr;
typedef int64_t       Vdi64;
typedef int64_t*      Vdi64ptr;
typedef unsigned char Vduchar;
typedef uintptr_t     Vduptr;
typedef size_t        Vdusize;
typedef char*         Vdcstr;
typedef float         Vdf32;
typedef double        Vdf64;
typedef int32_t       Vdb32;

#define VD_FALSE 0
#define VD_TRUE  1

#if VD_MACRO_ABBREVIATIONS
#define u8     Vdu8
#define u8ptr  Vdu8ptr
#define u16    Vdu16
#define u16ptr Vdu16ptr
#define u32    Vdu32
#define u32ptr Vdu32ptr
#define u64    Vdu64
#define u64ptr Vdu64ptr
#define i8     Vdi8
#define i8ptr  Vdi8ptr
#define i16    Vdi16
#define i16ptr Vdi16ptr
#define i32    Vdi32
#define i32ptr Vdi32ptr
#define i64    Vdi64
#define i64ptr Vdi64ptr
#define uchar  Vduchar
#define uptr   Vduptr
#define usize  Vdusize
#define cstr   Vdcstr
#define f32    Vdf32
#define f64    Vdf64
#define b32    Vdb32
#endif // VD_MACRO_ABBREVIATIONS

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

#define VD_API extern

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
#ifndef VD_USE_CRT
#define VD_USE_CRT 0
#endif // VD_USE_CRT

#if VD_USE_CRT
#include <assert.h>
#include <string.h>

#define VD_DEBUG_BREAK()    assert(0)
#define VD_ASSERT(x)        assert(x)
#define VD_MEMSET(p, v, s)  memset((p), (v), (s))
#define VD_MEMCPY(d, s, c)  memcpy((d), (s), (c))
#define VD_MEMCMP(l, r, c)  memcmp((l), (r), (c))
#define VD_MEMMOVE(d, s, c) memmove((d), (s), (c))
#else

static VD_INLINE void *vd_memset(void *dest, int value, size_t num)
{
    for (size_t i = 0; i < num; ++i) ((Vdi8ptr)dest)[i] = (Vdi8)value;
    return dest;
}

static VD_INLINE void *vd_memcpy(void *dest, const void *src, size_t num)
{
    for (size_t i = 0; i < num; ++i) ((Vdu8ptr)dest)[i] = ((Vdu8ptr)src)[i];
    return dest;
}

static VD_INLINE int vd_memcmp(const void *lhs, const void *rhs, size_t num)
{
    size_t i = 0;
    while (i < num) {
        Vdu8 lhs_c = ((Vdu8ptr)lhs)[i];
        Vdu8 rhs_c = ((Vdu8ptr)rhs)[i];
        if (lhs_c < rhs_c) {
            return -1;
        } else if (lhs_c > rhs_c) {
            return 1;
        }

        i++;
    }

    return 0;
}

static VD_INLINE void *vd_memmove(void *dest, void *src, size_t num) {
    Vdu8ptr d = (Vdu8ptr)dest;
    Vdu8ptr s = (Vdu8ptr)src;

    if (d < s) {
        for (size_t i = 0; i < num; ++i) {
            d[i] = s[i];
        }
    } else if (s < d) {
        for (size_t i = num; i > 0; --i) {
            d[i - 1] = s[i - 1];
        }
    }

    return d;
}

#define VD_DEBUG_BREAK()    (*((char*)0) = 1)
#define VD_ASSERT(x)        do { if (!(x)) { VD_DEBUG_BREAK(); } } while(0)
#define VD_MEMSET(p, v, s)  vd_memset((p), (v), (s))
#define VD_MEMCPY(d, s, c)  vd_memcpy((d), (s), (c))
#define VD_MEMCMP(l, r, c)  vd_memcmp((l), (r), (c))
#define VD_MEMMOVE(d, s, c) vd_memmove((d), (s), (c))

#endif // VD_USE_CRT

#define VD_IMPOSSIBLE() VD_ASSERT(VD_FALSE)
#define VD_TODO()       VD_ASSERT(VD_FALSE)
/* ----SIZES--------------------------------------------------------------------------------------------------------- */
#define VD_KILOBYTES(x) (((Vdusize)(x)) * 1024)
#define VD_MEGABYTES(x) (VD_KILOBYTES(x) * 1024)
#define VD_GIGABYTES(x) (VD_MEGABYTES(x) * 1024)

#define VD_ARRAY_COUNT(s) (sizeof(s) / sizeof(*s))

VD_INLINE Vdb32  vd_is_power_of_two(Vdusize x);
VD_INLINE Vduptr vd_align_forward(Vduptr ptr, Vdusize align);

VD_INLINE Vdb32 vd_is_power_of_two(Vdusize x) {
    return (x & (x - 1)) == 0;
}

VD_INLINE Vduptr vd_align_forward(Vduptr ptr, Vdusize align) {
    VD_ASSERT(vd_is_power_of_two(align));

    Vduptr p, a, modulo;

    p = ptr;
    a = (Vduptr)align;
    modulo = p & (a - 1);

    if (modulo != 0) {
        p += a - modulo;
    }

    return p;
}

#if VD_MACRO_ABBREVIATIONS
#define ARRAY_COUNT(a)            VD_ARRAY_COUNT(a)
#define KILOBYTES(x)              VD_KILOBYTES(x)
#define MEGABYTES(x)              VD_MEGABYTES(x)
#define GIGABYTES(x)              VD_GIGABYTES(x)
#define is_power_of_two(x)        vd_is_power_of_two(x)
#define align_forward(ptr, align) vd_align_forward(ptr, align)
#endif

/* ----UTILITY------------------------------------------------------------------------------------------------------- */
static VD_INLINE Vdi32 vd_ipow32(Vdi32 base, Vdu8 exp);
static VD_INLINE Vdi64 vd_ipow64(Vdi64 base, Vdu8 exp);

static VD_INLINE Vdi32 vd_ipow32(Vdi32 base, Vdu8 exp)
{
    static const Vdu8 highest_bit_set[] = {
        0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    };

    Vdi32 result = 1;
    switch (highest_bit_set[exp]) {
        case 255:
            if (base == 1) return 1;
            if (base == -1) return 1 - 2 * (exp & 1);
            return 0;
        case 5:
            if (exp & 1) result *= base;
            exp >>= 1;
            base *= base;
        case 4:
            if (exp & 1) result *= base;
            exp >>= 1;
            base *= base;
        case 3:
            if (exp & 1) result *= base;
            exp >>= 1;
            base *= base;
        case 2:
            if (exp & 1) result *= base;
            exp >>= 1;
            base *= base;
        case 1:
            if (exp & 1) result *= base;
        default:
            return result;
    }
}

static VD_INLINE Vdi64 vd_ipow64(Vdi64 base, Vdu8 exp)
{
    static const Vdu8 highest_bit_set[] = {
        0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
        255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
    };

    Vdi64 result = 1;

    switch (highest_bit_set[exp]) {
        case 255:
            if (base == 1) return 1;
            if (base == -1) return 1 - 2 * (exp & 1);
            return 0;
        case 6:
            if (exp & 1) result *= base;
            exp >>= 1;
            base *= base;
        case 5:
            if (exp & 1) result *= base;
            exp >>= 1;
            base *= base;
        case 4:
            if (exp & 1) result *= base;
            exp >>= 1;
            base *= base;
        case 3:
            if (exp & 1) result *= base;
            exp >>= 1;
            base *= base;
        case 2:
            if (exp & 1) result *= base;
            exp >>= 1;
            base *= base;
        case 1:
            if (exp & 1) result *= base;
        default:
            return result;
    }
}

#if VD_MACRO_ABBREVIATIONS
#define ipow32(base, exp) vd_ipow32(base, exp)
#define ipow64(base, exp) vd_ipow64(base, exp)
#endif // VD_MACRO_ABBREVIATIONS

/* ----TIMING-------------------------------------------------------------------------------------------------------- */
typedef struct __VD_HiTime {
#if VD_PLATFORM_WINDOWS
    Vdi64  performance_counter;
#else
    hitime time;
#endif // VD_PLATFORM_WINDOWS, else
} VdHiTime;

VdHiTime vd_hitime_get(void);
VdHiTime vd_hitime_sub(VdHiTime a, VdHiTime b);
Vdu64      vd_hitime_ns(VdHiTime time_spec);
Vdf64      vd_hitime_fms64(VdHiTime time_spec);
Vdu64      vd_hitime_ms(VdHiTime time_spec);
static VD_INLINE Vdf32 vd_hitime_s(VdHiTime time_spec) { return (Vdf32)(vd_hitime_ms(time_spec)) / 1000.f; }

#if VD_MACRO_ABBREVIATIONS
#define HiTime                    VdHiTime
#define hitime_get()            vd_hitime_get()
#define hitime_sub(a, b)        vd_hitime_sub(a, b)
#define hitime_ns(time_spec)    vd_hitime_ns(time_spec)
#define hitime_fms64(time_spec) vd_hitime_fms64(time_spec)
#define hitime_ms(time_spec)    vd_hitime_ms(time_spec)
#define hitime_s(time_spec)     vd_hitime_s(time_spec)
#endif // VD_MACRO_ABBREVIATIONS

/* ----VIRTUAL MEMORY------------------------------------------------------------------------------------------------ */
#if VD_INCLUDE_PLATFORM_SPECIFIC_FUNCTIONALITY
#ifndef VD_VM_CUSTOM
#define VD_VM_CUSTOM 0
#endif // !VD_VM_CUSTOM

#if VD_PLATFORM_KNOWN
Vdusize     vd_vm_get_page_size(void);
void*       vd_vm_reserve(Vdusize len);
void*       vd_vm_commit(void *addr, Vdusize len);
void*       vd_vm_decommit(void *addr, Vdusize len);
void        vd_vm_release(void *addr, Vdusize len);

#if VD_MACRO_ABBREVIATIONS
#define vm_get_page_size()     vd_vm_get_page_size()
#define vm_reserve(len)        vd_vm_reserve(len);
#define vm_commit(addr, len)   vd_vm_commit(addr, len);
#define vm_decommit(addr, len) vd_vm_decommit(addr, len);
#define vm_release(addr, len)  vd_vm_release(addr, len);
#endif // VD_MACRO_ABBREVIATIONS
#endif // VD_PLATFORM_KNOWN
#endif // VD_INCLUDE_PLATFORM_SPECIFIC_FUNCTIONALITY

/* ----SYSTEM ALLOCATOR---------------------------------------------------------------------------------------------- */
typedef struct {
    Vduptr  buf;
    Vdusize offset;
    Vdusize page_size;
    Vdusize reserved;
    Vdusize committed;
} VdSystemHeap;

VD_API VdSystemHeap* vd_system_heap_global(void);
VD_API void          vd_system_heap_set_reserve_page_count(VdSystemHeap *h, Vdusize page_count);
VD_API void*         vd_system_heap_alloc(VdSystemHeap *h, Vdusize size, Vdusize align);
VD_API void          vd_system_heap_empty(VdSystemHeap *h);



#ifndef VD_ALLOC_OVERRIDE
#define VD_ALLOC_OVERRIDE 0
#endif // VD_ALLOC_OVERRIDE

#if VD_USE_CRT
#if !VD_ALLOC_OVERRIDE
#include <stdlib.h>
#define VD_REALLOC(ptr, old_size, new_size) realloc(ptr, new_size)
#define VD_FREE(ptr, old_size)              free(ptr)
#endif // !VD_ALLOC_OVERRIDE
#else
#define VD_REALLOC(ptr, old_size, new_size) vd_realloc(ptr, old_size, new_size);
#define VD_FREE(ptr, old_size)              vd_free(ptr, new_size);

static VD_INLINE void *vd_realloc(void *ptr, Vdusize old_size, Vdusize new_size)
{
    VdSystemHeap *h = vd_system_heap_global();
    void *result = vd_system_heap_alloc(h, new_size, 0);

    if (ptr) {
        VD_MEMCPY(result, ptr, old_size);
    }

    return result;
}

static VD_INLINE void *vd_free(void *ptr, Vdusize old_size)
{
    VD_UNUSED(ptr);
    VD_UNUSED(old_size);

    return 0;
}

#endif // VD_USE_CRT

#define VD_MALLOC(size) VD_REALLOC(0, 0, size)

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

enum {
    VD_ARENA_FLAGS_USE_MALLOC = 1 << 7,
};

typedef Vdu8 VdArenaFlags;

typedef struct __VD_Arena {
    Vdu8         *buf;
    Vdusize      buf_len;
    Vdusize      prev_offset;
    Vdusize      curr_offset;
    VdArenaFlags flags;
    Vdu8         reserved[7];
} VdArena;

typedef struct __VD_ArenaSave {
    VdArena   *arena;
    Vdusize    prev_offset;
    Vdusize    curr_offset;
} VdArenaSave;

void                        vd_arena_init(VdArena *a, void *buf, size_t len);
void*                       vd_arena_alloc_align(VdArena *a, size_t size, size_t align);
void*                       vd_arena_resize_align(VdArena *a, void *old_memory, size_t old_size, size_t new_size, size_t align);
void                        vd_arena_clear(VdArena *a);
Vdb32                       vd_arena_free(VdArena *a, void *memory, size_t size);
VD_INLINE VdArenaSave       vd_arena_save(VdArena *a)                                                       { VdArenaSave result = { a, a->prev_offset, a->curr_offset }; return result; }
VD_INLINE void              vd_arena_restore(VdArenaSave save)                                              { save.arena->prev_offset = save.prev_offset; save.arena->curr_offset = save.curr_offset; }
VD_INLINE void*             vd_arena_alloc(VdArena *a, size_t size)                                         { return vd_arena_alloc_align(a, size, VD_ARENA_DEFAULT_ALIGNMENT);}
VD_INLINE void*             vd_arena_resize(VdArena *a, void *old_memory, size_t old_size, size_t new_size) { return vd_arena_resize_align(a, old_memory, old_size, new_size, VD_ARENA_DEFAULT_ALIGNMENT); }

#define VD_ARENA_PUSH_ARRAY(a, x, count) (x*)vd_arena_alloc(a, sizeof(x) * count)
#define VD_ARENA_PUSH_STRUCT(a, x)       VD_ARENA_PUSH_ARRAY(a, x, 1)
#define VD_ARENA_FROM_SYSTEM(a, size)    (vd_arena_init(a, VD_MALLOC(size), size))

#if VD_MACRO_ABBREVIATIONS
#define Arena                                                        VdArena
#define ARENA_FLAGS_USE_MALLOC                                       VD_ARENA_FLAGS_USE_MALLOC
#define arena_init(a, buf, len)                                      vd_arena_init(a, buf, len)
#define arena_alloc_align(a, size, align)                            vd_arena_alloc_align(a, size, align)
#define arena_resize_align(a, old_memory, old_size, new_size, align) vd_arena_resize_align(a, old_memory, old_size, new_size, align)
#define arena_clear(a)                                               vd_arena_clear(a)
#define arena_free(a, memory, size)                                  vd_arena_free(a, memory, size)
#define arena_save(a)                                                vd_arena_save(a)
#define arena_restore(save)                                          vd_arena_restore(save)
#define arena_alloc(a, size)                                         vd_arena_alloc(a, size)
#define arena_resize(a, old_memory, old_size, new_size)              vd_arena_resize(a, old_memory, old_size, new_size)
#define ARENA_PUSH_ARRAY(a, x, count) VD_ARENA_PUSH_ARRAY(a, x, count)
#endif // VD_MACRO_ABBREVIATIONS

/* ----SIMPLE ARRAYS------------------------------------------------------------------------------------------------- */
VD_INLINE void *vd__array_concat(VdArena *a, void *a1, Vdusize na1, void *a2, Vdusize na2, Vdusize isize)
{
    Vdusize alloc_size = isize * (na1 + na2);
    void *result = vd_arena_alloc(a, alloc_size);
    VD_MEMCPY(result, a1, isize * na1);
    VD_MEMCPY((Vdu8*)result + (isize * na1), a2, isize * na2);
    return result;
}

#define VD_ARRAY_CONCAT(a, a1, na1, a2, na2) vd__array_concat((a), (a1), (na1), (void*)(a2), (na2), sizeof(*a1))

/* ----DLIST--------------------------------------------------------------------------------------------------------- */
typedef struct VdDListNode VdDListNode;

struct VdDListNode {
    VdDListNode *next;
    VdDListNode *prev;
};

typedef struct __VD_DList {
    VdDListNode sentinel;
} VdDList;

VD_INLINE void            vd_dlist_init(VdDList *list);
VD_INLINE void            vd_dlist_append(VdDList *list, VdDListNode *node);
VD_INLINE Vdusize         vd_dlist_count(VdDList *list);
VD_INLINE void            vd_dlist_rm(VdDList *list, VdDListNode *node);
VD_INLINE VdDListNode*    vd_dlist_rm_first(VdDList *list);
VD_INLINE VdDListNode*    vd_dlist_rm_last(VdDList *list);
VD_INLINE void            vd_dlist_node_link(VdDListNode *node, VdDListNode *prev, VdDListNode *next);
VD_INLINE void            vd_dlist_node_unlink(VdDListNode *node);

VD_INLINE void           vd_dlist_append(VdDList *list, VdDListNode *node)            { vd_dlist_node_link(node, list->sentinel.prev, &list->sentinel); }
VD_INLINE void           vd_dlist_node_init(VdDListNode *node)                          { node->next = node; node->prev = node; }
VD_INLINE Vdb32          vd_dlist_node_is_empty(VdDListNode *node)                      { return (node->next == node) && (node->prev == node); }
VD_INLINE Vdb32          vd_dlist_is_empty(VdDList *list)                               { return vd_dlist_node_is_empty(&list->sentinel); }
VD_INLINE VdDListNode*   vd_dlist_first(VdDList *list)                                  { return vd_dlist_is_empty(list) ? 0 : list->sentinel.next; }
VD_INLINE VdDListNode*   vd_dlist_last(VdDList *list)                                   { return vd_dlist_is_empty(list) ? 0 : list->sentinel.prev; }
VD_INLINE void           vd_dlist_node_append(VdDListNode *node, VdDListNode *target) { vd_dlist_node_link(node, target, target->next); }

#define VD_DLIST_FOR_EACH(listp, nodename)                       for (VdDListNode *nodename = &(listp)->sentinel; (nodename = nodename->next) != &(listp)->sentinel;)
#define VD_DLIST_FOR_EACH_WITH_INDEX(listp, nodename, indexname) Vdusize indexname = 0; for (VdDListNode *nodename = &(listp)->sentinel; (nodename = nodename->next) != &(listp)->sentinel; indexname++)

VD_INLINE void vd_dlist_init(VdDList *list)
{
    vd_dlist_node_init(&list->sentinel);
}

VD_INLINE void vd_dlist_rm(VdDList *list, VdDListNode *node)
{
    VD_ASSERT(node != &list->sentinel);
    vd_dlist_node_unlink(node);
}

VD_INLINE VdDListNode *vd_dlist_rm_first(VdDList *list)
{
    if (vd_dlist_is_empty(list)) return 0;

    VdDListNode *result = vd_dlist_first(list);
    vd_dlist_rm(list, result);
    return result;
}

VD_INLINE VdDListNode *vd_dlist_rm_last(VdDList *list)
{
    if (vd_dlist_is_empty(list)) return 0;

    VdDListNode *result = vd_dlist_last(list);
    vd_dlist_rm(list, result);
    return result;
}

VD_INLINE void vd_dlist_node_link(VdDListNode *node, VdDListNode *prev, VdDListNode *next)
{
    next->prev = node;
    node->next = next;
    node->prev = prev;
    prev->next = node;
}

VD_INLINE void vd_dlist_node_unlink(VdDListNode *node)
{
    node->next->prev = node->prev;
    node->prev->next = node->next;
    vd_dlist_node_init(node);
}

VD_INLINE Vdusize vd_dlist_count(VdDList *list)
{
    Vdusize count = 0;
    VD_DLIST_FOR_EACH(list, n) {
        count++;
    }
    return count;
}

#if VD_MACRO_ABBREVIATIONS
#define DListNode                            VdDListNode
#define DList                                VdDList
#define vd_dlist_init(list)                  vd_dlist_init(list)
#define vd_dlist_append(list, node)          vd_dlist_append(list, node)
#define vd_dlist_count(list)                 vd_dlist_count(list)
#define vd_dlist_rm(list, node)              vd_dlist_rm(list, node)
#define vd_dlist_rm_first(list)              vd_dlist_rm_first(list)
#define vd_dlist_rm_last(list)               vd_dlist_rm_last(list)
#define vd_dlist_node_link(node, prev, next) vd_dlist_node_link(node, prev, next)
#define vd_dlist_node_unlink(node)           vd_dlist_node_unlink(node)
#define vd_dlist_append(list, node)          vd_dlist_append(list, node)
#define vd_dlist_node_init(node)             vd_dlist_node_init(node)
#define vd_dlist_node_is_empty(node)         vd_dlist_node_is_empty(node)
#define vd_dlist_is_empty(list)              vd_dlist_is_empty(list)
#define vd_dlist_first(list)                 vd_dlist_first(list)
#define vd_dlist_last(list)                  vd_dlist_last(list)
#define vd_dlist_node_append(node, target)   vd_dlist_node_append(node, target)
#endif // VD_MACRO_ABBREVIATIONS

/* ----FIXED ARRAY--------------------------------------------------------------------------------------------------- */
typedef struct __VD_FixedArrayHeader {
    Vdu32 len;
    Vdu32 cap;
} VdFixedArrayHeader;

#define VD_FIXEDARRAY_HEADER(a)                         ((VdFixedArrayHeader*)(((Vdu8*)a) - sizeof(VdFixedArrayHeader)))

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
#define VD_FIXEDARRAY_INIT(a, count, allocator)         ((a) = vd__fixed_array_allocate((allocator), (count), sizeof(*(a)), 0))
#define VD_FIXEDARRAY_INIT_RESERVE(a, len, allocator)   ((a) = vd__fixed_array_allocate((allocator), (len),   sizeof(*(a)), 1))
#define VD_FIXEDARRAY_CHECK(a, n)                       ((VD_FIXEDARRAY_HEADER(a)->len + (n)) <= VD_FIXEDARRAY_HEADER(a)->cap)
#define VD_FIXEDARRAY_ADDN(a, n)                        (VD_ASSERT(VD_FIXEDARRAY_CHECK(a, n)), VD_ARRAY_HEADER(a)->len += n)
#define VD_FIXEDARRAY_ADD(a, v)                         (VD_ASSERT(VD_FIXEDARRAY_CHECK(a, 1)), (a)[VD_FIXEDARRAY_HEADER(a)->len++] = (v))
#define VD_FIXEDARRAY_LEN(a)                            (VD_FIXEDARRAY_HEADER(a)->len)
#define VD_FIXEDARRAY_CAP(a)                            (VD_FIXEDARRAY_HEADER(a)->cap)
#define VD_FIXEDARRAY_CLEAR(a)                          (VD_FIXEDARRAY_HEADER(a)->len = 0)
#define VD_FIXEDARRAY_POP(a)                            ((a)[--VD_FIXEDARRAY_HEADER(a)->len])

VD_INLINE void* vd__fixed_array_allocate(VdArena *arena, Vdu32 capacity, Vdusize isize, Vdb32 mark)
{
    Vdusize alloc_size = sizeof(VdFixedArrayHeader) + capacity * isize;
    void *result = VD_MEMSET(vd_arena_alloc(arena, alloc_size), 0, alloc_size);
    VdFixedArrayHeader *hdr = (VdFixedArrayHeader*)result;

    hdr->cap = capacity;
    if (mark) {
        hdr->len = capacity;
    }

    return (void*) ((Vdu8*)result + sizeof(VdFixedArrayHeader));
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
    Vdu32     len;
    Vdu32     cap;
    VdArena   *arena;
} VdDynArrayHeader;

#define VD_DYNARRAY_HEADER(a)                      ((VdDynArrayHeader*)(((Vdu8*)a) - sizeof(VdDynArrayHeader)))
#define VD_DYNARRAY_INIT(a, arena)                 ((a) = vd__dynarray_grow(a, sizeof(*(a)), 1, 0, arena))
#define VD_DYNARRAY_INIT_WITH_CAP(a, arena, cap)   ((a) = vd__dynarray_grow(a, sizeof(*(a)), cap, 0, arena))
#define VD_DYNARRAY_ADD(a, v)                      (VD_DYNARRAY_CHECKGROW(a, 1), (a)[VD_DYNARRAY_HEADER(a)->len++] = (v))
#define VD_DYNARRAY_PUSH(a)                        (VD_DYNARRAY_CHECKGROW(a, 1), &((a)[VD_DYNARRAY_HEADER(a)->len++]))
#define VD_DYNARRAY_ADDN(a, n)                     (VD_DYNARRAY_CHECKGROW(a, n), VD_DYNARRAY_HEADER(a)->len += (n))
#define VD_DYNARRAY_CLEAR(a)                       ((a) ? VD_DYNARRAY_HEADER(a)->len = 0 : 0)
#define VD_DYNARRAY_POP(a)                         (VD_DYNARRAY_HEADER(a)->len--, (a)[VD_DYNARRAY_HEADER(a)->len])
#define VD_DYNARRAY_LAST(a)                        ((a)[VD_DYNARRAY_HEADER(a)->len - 1])
#define VD_DYNARRAY_LEN(a)                         ((a) ? VD_DYNARRAY_HEADER(a)->len : 0)
#define VD_DYNARRAY_CAP(a)                         ((a) ? VD_DYNARRAY_HEADER(a)->cap : 0)
#define VD_DYNARRAY_ARENAP(a)                      ((a) ? VD_DYNARRAY_HEADER(a)->arena : 0)
#define VD_DYNARRAY_GROW(a, b, c)                  ((a) = vd__dynarray_grow((a), sizeof(*(a)), (b), (c), VD_DYNARRAY_ARENAP(a)))
#define VD_DYNARRAY_PTR_CHECKED(a, i)              ((i < VD_DYNARRAY_LEN(a)) ? &(a)[i] : 0)

// @todo(mdodis): fix & check dynarray & fixedarray (they use invalid macros and dynarray is not tested)
#define VD_DYNARRAY_CHECKGROW(a, n)                      \
    ((!(a) || VD_DYNARRAY_HEADER(a)->len + (n) > VD_DYNARRAY_HEADER(a)->cap) \
    ? (VD_DYNARRAY_GROW(a, n, 0), 0) : 0)

#define VD_DYNARRAY

VD_INLINE void *vd__dynarray_grow(void *a, Vdusize tsize, Vdu32 addlen, Vdu32 mincap, VdArena *arena)
{
    Vdusize min_len = VD_DYNARRAY_LEN(a) + addlen;

    if (min_len > mincap) {
        mincap = (Vdu32)min_len;
    }

    if (mincap <= VD_DYNARRAY_CAP(a)) {
        return a;
    }

    if (mincap < (2 * VD_DYNARRAY_CAP(a))) {
        mincap = 2 * VD_DYNARRAY_CAP(a);
    } else {
        mincap = 4;
    }

    void *b = vd_arena_resize(arena, 
        a ? VD_DYNARRAY_HEADER(a) : 0,
        VD_DYNARRAY_CAP(a) == 0 ? 0 : tsize * VD_DYNARRAY_CAP(a) + sizeof(VdDynArrayHeader),
        tsize * mincap * sizeof(VdDynArrayHeader));

    b = (Vdu8*)b + sizeof(VdDynArrayHeader);

    if (a == 0) {
        VD_DYNARRAY_HEADER(b)->len = 0;
        VD_DYNARRAY_HEADER(b)->arena = arena;
    }

    VD_DYNARRAY_HEADER(b)->cap = mincap;
    return b;
}

#if VD_MACRO_ABBREVIATIONS
#define dynarray_init(a, arena)               VD_DYNARRAY_INIT(a, arena)
#define dynarray_init_with_cap(a, arena, cap) VD_DYNARRAY_INIT_WITH_CAP(a, arena, cap)
#define dynarray_add(a, v)                    VD_DYNARRAY_ADD(a, v)
#define dynarray_push(a)                      VD_DYNARRAY_PUSH(a)
#define dynarray_addn(a, n)                   VD_DYNARRAY_ADDN(a, n)
#define dynarray_clear(a)                     VD_DYNARRAY_CLEAR(a)
#define dynarray_pop(a)                       VD_DYNARRAY_POP(a)
#define dynarray_last(a)                      VD_DYNARRAY_LAST(a)
#define dynarray_len(a)                       VD_DYNARRAY_LEN(a)
#define dynarray_cap(a)                       VD_DYNARRAY_CAP(a)
#define dynarray_ptr_checked(a, i)            VD_DYNARRAY_PTR_CHECKED(a, i)
#define dynarray
#endif

/* ----STR----------------------------------------------------------------------------------------------------------- */
typedef struct __VD_Str {
    char        *s;
    Vdusize   len;
} VdStr;

VD_INLINE Vdb32      vd_cstr_cmp(Vdcstr _a, Vdcstr _b);
VD_INLINE Vdusize    vd_cstr_len(Vdcstr a);
VD_INLINE Vdcstr     vd_cstr_dup(VdArena *arena, Vdcstr s);
VD_INLINE Vdcstr     vd_cstr_cncat(VdArena *arena, Vdcstr a, Vdcstr b);
VD_INLINE Vdcstr     vd_cstr_ncncat(VdArena *arena, Vdusize *num_strings, Vdcstr *strings);
VD_INLINE Vdcstr     vd_cstr_from_str(VdArena *arena, VdStr s);

VD_INLINE VdStr      vd_str_null(void)                                                  { VdStr result = {0, 0}; return result; }
VD_INLINE VdStr      vd_str_from_cstr(Vdcstr s)                                         { VdStr result = { s, vd_cstr_len(s) }; return result; }
VD_INLINE VdStr      vd_str_dup(VdArena *a, VdStr s);
VD_INLINE VdStr      vd_str_dup_from_cstr(VdArena *a, Vdcstr s);
VD_INLINE Vdusize    vd_str_first_of(VdStr s, VdStr q, Vdu64 start);
VD_INLINE Vdb32      vd_str_split(VdStr s, Vdusize at, VdStr *left, VdStr *right);
VD_INLINE VdStr      vd_str_chop_left(VdStr s, Vdusize at)                              { VdStr left, right; return vd_str_split(s, at, &left, &right) ? right : vd_str_null(); }
VD_INLINE VdStr      vd_str_chop_right(VdStr s, Vdusize at)                             { VdStr left, right; return vd_str_split(s, at, &left, &right) ? left : vd_str_null(); }
VD_INLINE Vdb32      vd_str_eq(VdStr a, VdStr b);
VD_INLINE VdStr      vd_str_join(VdArena *arena, VdStr a, VdStr b, Vdb32 null_sep);
VD_INLINE Vdb32      vd_str_ends_with_char(VdStr a, char c)                             { return a.len > 0 ? a.s[a.len - 1] == c : VD_FALSE; }

VD_INLINE Vdb32 vd_cstr_cmp(Vdcstr _a, Vdcstr _b)
{
    Vdcstr a = _a;
    Vdcstr b = _b;

    while (*a == *b) {
        if (*a == 0) return VD_TRUE;

        a++;
        b++;
    }

    return VD_FALSE;
}

VD_INLINE Vdusize vd_cstr_len(Vdcstr a)
{
    Vdusize result = 0;
    while (*a++) result++;
    return result;
}

VD_INLINE VdStr vd_str_dup(VdArena *a, VdStr s) {
    VdStr result;
    result.s = (char*)vd_arena_alloc(a, s.len);
    result.len = s.len;
    VD_MEMCPY(result.s, s.s, s.len);
    return result;
}

VD_INLINE VdStr vd_str_dup_from_cstr(VdArena *a, Vdcstr s) {
    return vd_str_dup(a, vd_str_from_cstr(s));
}

VD_INLINE Vdusize vd_str_first_of(VdStr s, VdStr q, Vdu64 start) {
    if (s.len == 0) return 0;
    if (q.len == 0) return 0;

    Vdusize qindex = 0;
    Vdusize i;
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

VD_INLINE Vdb32 vd_str_split(VdStr s, Vdusize at, VdStr *left, VdStr *right)
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

VD_INLINE Vdb32 vd_str_eq(VdStr a, VdStr b)
{
    if (a.len != b.len) return VD_FALSE;

    for (Vdusize i = 0; i < a.len; ++i) {
        if (a.s[i] != b.s[i]) return VD_FALSE;        
    }

    return VD_TRUE;
}

VD_INLINE Vdcstr vd_cstr_cncat(VdArena *arena, Vdcstr a, Vdcstr b) {
    Vdusize la = vd_cstr_len(a);
    Vdusize lb = vd_cstr_len(b);

    Vdcstr result = (Vdcstr)vd_arena_alloc(arena, la + lb + 1);

    VD_MEMCPY(result, a, la);
    VD_MEMCPY(result + la, b, lb);
    result[la + lb] = 0;
    return result;
}

VD_INLINE Vdcstr vd_cstr_dup(VdArena *arena, Vdcstr s) {
    Vdusize ls = vd_cstr_len(s);
    Vdcstr result = (Vdcstr)vd_arena_alloc(arena, ls + 1);
    VD_MEMCPY(result, s, ls);
    result[ls] = 0;
    return result;
}

VD_INLINE VdStr vd_str_join(VdArena *arena, VdStr a, VdStr b, Vdb32 null_sep)
{
    Vdusize final_size = a.len + b.len + (null_sep ? 1 : 0);
    char *result = (char*)vd_arena_alloc(arena, final_size);

    VD_MEMCPY(result, a.s, a.len);
    VD_MEMCPY(result + a.len, b.s, b.len);
    if (null_sep) {
        result[a.len + b.len] = 0;
    }

    VdStr _r = { result, final_size };
    return _r;
}

VD_INLINE Vdcstr vd_cstr_from_str(VdArena *arena, VdStr s)
{
    char *result = (char*)vd_arena_alloc(arena, s.len + 1);
    VD_MEMCPY(result, s.s, s.len);
    result[s.len] = 0;
    return result;
}

#define VD_LIT(string)        (VdStr) { .s = (string), .len = (sizeof(string) - 1), }
#define VD_LIT_INLINE(string) {(string), (sizeof(string) - 1)}

#define VD_STR_EXPAND(string) (int)(string).len, (string).s

#if VD_MACRO_ABBREVIATIONS
#define Str                 VdStr
#define LIT                 VD_LIT
#define LIT_INLINE          VD_LIT_INLINE
#define STR_EXPAND          VD_STR_EXPAND
#define cstr_cmp            vd_cstr_cmp
#define cstr_len            vd_cstr_len
#define cstr_dup            vd_cstr_dup
#define cstr_cncat          vd_cstr_cncat
#define cstr_ncncat         vd_cstr_ncncat
#define cstr_from_str       vd_cstr_from_str
#define str_null            vd_str_null
#define str_from_cstr       vd_str_from_cstr
#define str_dup             vd_str_dup
#define str_dup_from_cstr   vd_str_dup_from_cstr
#define str_first_of        vd_str_first_of
#define str_split           vd_str_split
#define str_chop_left       vd_str_chop_left
#define str_chop_right      vd_str_chop_right
#define str_eq              vd_str_eq
#define str_join            vd_str_join
#define str_ends_with_char  vd_str_ends_with_char
#endif

/* ----UTF8---------------------------------------------------------------------------------------------------------- */
static VD_INLINE Vdu8 vd_utf8_codepoint_len(Vdu32 codepoint);

static VD_INLINE Vdu8 vd_utf8_codepoint_len(Vdu32 codepoint)
{
    Vdcstr s = (Vdcstr)&codepoint;    
    Vdu8 b = s[0];

    if ((b & 0x80) == 0x00) return 1;
    if ((b & 0xE0) == 0xC0) return 2;
    if ((b & 0xF0) == 0xE0) return 3;
    if ((b & 0xF8) == 0xF0) return 4;

    return VD_U8_MAX;
}

#if VD_MACRO_ABBREVIATIONS
#define utf8_codepoint_len vd_utf8_codepoint_len
#endif // VD_MACRO_ABBREVIATIONS

/* ----STR BUILDER--------------------------------------------------------------------------------------------------- */

/* ----PARSING------------------------------------------------------------------------------------------------------- */
VD_INLINE Vdb32      vd_is_ascii_digit(int c);
VD_INLINE Vdb32      vd_is_letter(int c);
VD_INLINE Vdb32      vd_is_cdecl_start(int c);
VD_INLINE Vdb32      vd_is_cdecl_continue(int c);
Vdb32                vd_parse_u64(VdStr s, Vdu64 *r);

VD_INLINE Vdb32 vd_is_ascii_digit(int c)
{
    return (c >= '0') && (c <= '9');
}

VD_INLINE Vdb32 vd_is_letter(int c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

VD_INLINE Vdb32 vd_is_cdecl_start(int c)
{
    return vd_is_letter(c) || c == '_';
}

VD_INLINE Vdb32 vd_is_cdecl_continue(int c)
{
    return vd_is_cdecl_start(c) || vd_is_ascii_digit(c);
}

#if VD_MACRO_ABBREVIATIONS
#define is_ascii_digit       vd_is_ascii_digit
#define is_letter            vd_is_letter
#define is_cdecl_start       vd_is_cdecl_start
#define is_cdecl_continue    vd_is_cdecl_continue
#define parse_u64            vd_parse_u64
#endif // VD_MACRO_ABBREVIATIONS

/* ----ARG----------------------------------------------------------------------------------------------------------- */
typedef struct __VD_Arg {
    int argc;
    char **argv;

    int argi;
    int ci;
} VdArg;

VdArg vd_arg_new(int argc, char **argv);
void  vd_arg_skip_program_name(VdArg *arg);
void  vd_arg_skip(VdArg *arg);
Vdb32 vd_arg_at_end(VdArg *arg);
Vdb32 vd_arg_get_name(VdArg *arg, VdStr *name);
Vdb32 vd_arg_get_uint(VdArg *arg, Vdu64 *i);
Vdb32 vd_arg_get_str(VdArg *arg, VdStr *str);
Vdb32 vd_arg_expect_char(VdArg *arg, char c);

#if VD_MACRO_ABBREVIATIONS
#define Arg                        VdArg
#define arg_new(argc, argv)        vd_arg_new(argc, argv)
#define arg_skip_program_name(arg) vd_arg_skip_program_name(arg)
#define arg_skip(arg)              vd_arg_skip(arg)
#define arg_at_end(arg)            vd_arg_at_end(arg)
#define arg_get_name(arg, name)    vd_arg_get_name(arg, name)
#define arg_get_uint(arg, i)       vd_arg_get_uint(arg, i)
#define arg_get_str(arg, str)      vd_arg_get_str(arg, str)
#define arg_expect_char(arg, c)    vd_arg_expect_char(arg, c)
#endif // VD_MACRO_ABBREVIATIONS

/* ----HASH---------------------------------------------------------------------------------------------------------- */
#ifndef VD_HASH64_CUSTOM
#define VD_HASH64_CUSTOM 0
#endif // !VD_HASH64_CUSTOM

#if !VD_HASH64_CUSTOM
#define VD_HASH64_DEFAULT_SEED (0x9747b28c)

VD_INLINE Vdu64 vd_hash64(const void *data, Vdu64 len, Vdu32 seed);
VD_INLINE Vdu64 vd_dhash64(const void *data, Vdu64 len);
VD_INLINE Vdu64 vd_dhash64_str(VdStr s);

VD_INLINE Vdu64 vd_hash64(const void *data, Vdu64 len, Vdu32 seed)
{
    const Vdu32 m = 0x5bd1e995;
    const Vdu32 r = 24;

    Vdu64 h = seed ^ len;

    const Vdu8 *bytes = (const Vdu8*)data;

    while (len >= 4)
    {
        Vdu32 k = *(Vdu32*)bytes;

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

VD_INLINE Vdu64 vd_dhash64(const void *data, Vdu64 len) { return vd_hash64(data, len, VD_HASH64_DEFAULT_SEED); }
#endif // !VD_HASH64_CUSTOM

VD_INLINE Vdu64 vd_dhash64_str(VdStr s) { return vd_dhash64(s.s, s.len); }

#if VD_MACRO_ABBREVIATIONS
#define hash64(data, len, seed) vd_hash64(data, len, seed)
#define dhash64(data, len)      vd_dhash64(data, len)
#define dhash64_str(s)          vd_dhash64_str(VdStr s)
#endif // VD_MACRO_ABBREVIATIONS

/* ----STRMAP-------------------------------------------------------------------------------------------------------- */
typedef struct {
    Vdu32   cap;
    Vdu32   cap_total;
    Vdu32   taken;
    Vdu32   tsize;
    VdArena *arena;
} Vd__StrmapHeader;

typedef struct Vd__StrmapBinPrefix Vd__StrmapBinPrefix;

typedef enum {
    VD__STRMAP_GET_BIN_FLAGS_CREATE       = 1 << 1,
    VD__STRMAP_GET_BIN_FLAGS_SET_UNUSED   = 1 << 2,
    VD__STRMAP_GET_BIN_FLAGS_GET_EXISTING = 1 << 3,
} Vd__StrmapGetBinFlags;

typedef enum {
    VD__STRMAP_SET_MODE_NEW_ONLY  = 0,
    VD__STRMAP_SET_MODE_OVERWRITE = 1,
} Vd__StrmapSetMode;

#ifndef VD_STRMAP_KEY_PREFIX_LEN_CUSTOM
#define VD_STRMAP_KEY_PREFIX_LEN 31
#endif // !VD_STRMAP_KEY_PREFIX_LEN_CUSTOM

struct Vd__StrmapBinPrefix {
    Vd__StrmapBinPrefix *next;                                   //  8 bytes
    Vd__StrmapBinPrefix *insq;                                   //  8 bytes
    char                *key_rest;                               //  8 bytes
    Vdu32               key_len;                                 //  4 bytes
    Vdu32               key_rest_cap;                            //  4 bytes
    Vdu8                used;                                    //  1 byte
    char                key_prefix[VD_STRMAP_KEY_PREFIX_LEN];    // 31 bytes (default)
};                                                               // = 64 bytes

/**
 * @sym VdStrmapInitOptions
 *
 * @brief Options to modify Strmap behavior, allocation strategy, etc.
 */
typedef struct __VD_StrmapInitOptions {
    /** A value from [0.3, 0.9] that determines the amount of slots assigned to the addressable region. */
    float address_scale;

    /** The estimated key length in bytes. If this is higher than VD_STRMAP_KEY_PREFIX_LEN, then more space will be allocated up front for key storage. */
    Vdu32 average_key_len;
} VdStrmapInitOptions;

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
#define VD_STRMAP_HEADER(m)                ((Vd__StrmapHeader*)(((Vdu8*)(m)) - sizeof(Vd__StrmapHeader)))
#define VD_STRMAP_INIT(m, arena, cap, o)   ((m) = (vd__strmap_init((arena), sizeof(*m), (cap), (o))))
#define VD_STRMAP_INIT_DEFAULT(m, arena)   VD_STRMAP_INIT((m), (arena), VD_STRMAP_DEFAULT_CAP, 0)
#define VD_STRMAP_SET(m, k, v)             vd__strmap_set((m), (k), (void*)(v), VD__STRMAP_SET_MODE_NEW_ONLY)
#define VD_STRMAP_GET(m, k, v)             vd__strmap_get((m), (k), (void*)(v))
#define VD_STRMAP_GET_PTR(m, k)            vd__strmap_get_ptr((m), (k))
#define VD_STRMAP_RM(m, k)                 (vd__strmap_get_bin((m), (k), VD__STRMAP_GET_BIN_FLAGS_SET_UNUSED) != 0)
#define VD_STRMAP_OVERWRITE(m, k, v)       vd__strmap_set((m), (k), (void*)(v), VD__STRMAP_SET_MODE_OVERWRITE)
#define VD_STRMAP_COUNT(m)                 ((m) == 0 ? 0 : VD_STRMAP_HEADER(m)->taken)
#define VD_STRMAP_TSIZE(m)                 ((m) == 0 ? 0 : VD_STRMAP_HEADER(m)->tsize)
#define VD_STRMAP_TOTAL_CAP(m)             ((m) == 0 ? 0 : VD_STRMAP_HEADER(m)->cap_total)
#define VD_STRMAP_ARENAP(m)                ((m) == 0 ? 0 : VD_STRMAP_HEADER(m)->arena)
#define VD_STRMAP_ENTRY_SIZE(m)            (VD_STRMAP_HEADER(m)->tsize + sizeof(Vd__StrmapBinPrefix))
#define VD_STRMAP_GET_BIN(m, i)            ((Vd__StrmapBinPrefix*) (((Vdu8*)m) + (VD_STRMAP_ENTRY_SIZE(m) * (i))))
#define VD_STRMAP_GET_ENTRY(m, i)          ((void*)((Vdu8*)VD_STRMAP_GET_BIN(m, i) + sizeof(Vd__StrmapBinPrefix)))
#define VD_STRMAP_GET_KEY_PREFIX(m, i)     VD_STRMAP_GET_BIN(m, i)->key_prefix
#define VD_STRMAP_GET_KEY_LEN(m, i)        VD_STRMAP_GET_BIN(m, i)->key_len
#define VD_STRMAP_GET_BIN_USED(m, i)       VD_STRMAP_GET_BIN(m, i)->used
#define VD_STRMAP_GET_BIN_NEXT(m, i)       VD_STRMAP_GET_BIN(m, i)->next
#define VD_STRMAP_GET_BIN_INDEX(m, b)      (size_t)(((uintptr_t)b - (uintptr_t)m) / (uintptr_t)(VD_STRMAP_ENTRY_SIZE(m)))
#define VD_STRMAP_GET_BIN_VPTR(m, i)       ((void*)(((Vdu8*)VD_STRMAP_GET_BIN(m, i)) + sizeof(Vd__StrmapBinPrefix)))
#define VD_STRMAP_BIN_MOVE_TO_VPTR(b)      ((void*)(((Vdu8*)(b)) + sizeof(Vd__StrmapBinPrefix)))
    
void*                 vd__strmap_init(VdArena *arena, Vdu32 tsize, Vdu32 cap, VdStrmapInitOptions *options);
Vd__StrmapBinPrefix*  vd__strmap_get_bin(void *map, VdStr key, Vd__StrmapGetBinFlags op);
VD_INLINE Vdb32       vd__strmap_get(void *map, VdStr key, void *value);
VD_INLINE void*       vd__strmap_get_ptr(void *map, VdStr key);
VD_INLINE Vdb32       vd__strmap_set(void *map, VdStr key, void *value, Vd__StrmapSetMode mode);

VD_INLINE Vdb32 vd__strmap_get(void *map, VdStr key, void *value)
{
    // Look for bin 
    Vd__StrmapBinPrefix *bin = vd__strmap_get_bin(map, key, VD__STRMAP_GET_BIN_FLAGS_GET_EXISTING);
    if (bin == 0) return VD_FALSE;

    // If found the copy over the data
    Vdu8 *bin_data = ((Vdu8*)bin) + sizeof(Vd__StrmapBinPrefix);
    VD_MEMCPY(value, bin_data, VD_STRMAP_HEADER(map)->tsize);
    return VD_TRUE;
}

VD_INLINE void *vd__strmap_get_ptr(void *map, VdStr key)
{
    // Look for bin 
    Vd__StrmapBinPrefix *bin = vd__strmap_get_bin(map, key, VD__STRMAP_GET_BIN_FLAGS_GET_EXISTING);
    if (bin == 0) return 0;

    // If found the copy over the data
    Vdu8 *bin_data = ((Vdu8*)bin) + sizeof(Vd__StrmapBinPrefix);
    return (void*)bin_data;
}

VD_INLINE Vdb32 vd__strmap_set(void *map, VdStr key, void *value, Vd__StrmapSetMode mode)
{
    if (mode == VD__STRMAP_SET_MODE_NEW_ONLY) {
        Vd__StrmapBinPrefix *bin = vd__strmap_get_bin(map, key, VD__STRMAP_GET_BIN_FLAGS_CREATE);
        if (bin == 0) return VD_FALSE;

        Vdu8 *bin_data = ((Vdu8*)bin) + sizeof(Vd__StrmapBinPrefix);
        VD_MEMCPY(bin_data, value, VD_STRMAP_HEADER(map)->tsize);
        return VD_TRUE;
    } else {
        Vd__StrmapBinPrefix *bin = vd__strmap_get_bin(map, key, (Vd__StrmapGetBinFlags)(VD__STRMAP_GET_BIN_FLAGS_CREATE | VD__STRMAP_GET_BIN_FLAGS_GET_EXISTING));
        if (bin == 0) return VD_FALSE;

        Vdu8 *bin_data = ((Vdu8*)bin) + sizeof(Vd__StrmapBinPrefix);
        VD_MEMCPY(bin_data, value, VD_STRMAP_HEADER(map)->tsize);
        return VD_TRUE;
    }
}

#if VD_MACRO_ABBREVIATIONS
#define strmap
#define strmap_init(m, arena, cap, o) VD_STRMAP_INIT(m, arena, cap, o)
#define strmap_init_default(m, arena) VD_STRMAP_INIT_DEFAULT(m, arena)
#define strmap_set(m, k, v)           VD_STRMAP_SET(m, k, v)
#define strmap_get(m, k, v)           VD_STRMAP_GET(m, k, v)
#define strmap_get_ptr(m, k)          VD_STRMAP_GET_PTR(m, k)
#define strmap_rm(m, k)               VD_STRMAP_RM(m, k)
#define strmap_overwrite(m, k, v)     VD_STRMAP_OVERWRITE(m, k, v)
#endif // VD_MACRO_ABBREVIATIONS

/* ----KVMAP--------------------------------------------------------------------------------------------------------- */
typedef struct {
    Vdu32   cap;
    Vdu32   cap_total;
    Vdu32   taken;
    Vdu32   ksize;
    Vdu32   vsize;
} Vd__KVMapHeader;

typedef struct Vd__KVMapBinPrefix Vd__KVMapBinPrefix;

struct Vd__KVMapBinPrefix {
    Vd__KVMapBinPrefix *next;
    Vd__KVMapBinPrefix *insq;
    Vdusize            used;
};

typedef enum {
    VD__KVMAP_GET_BIN_FLAGS_CREATE       = 1 << 1,
    VD__KVMAP_GET_BIN_FLAGS_SET_UNUSED   = 1 << 2,
    VD__KVMAP_GET_BIN_FLAGS_GET_EXISTING = 1 << 3,
} Vd__KVMapGetBinFlags;

typedef enum {
    VD__KVMAP_SET_MODE_NEW_ONLY  = 0,
    VD__KVMAP_SET_MODE_OVERWRITE = 1,
} Vd__KVMapSetMode;


/**
 * @sym VdKVMapInitOptions
 *
 * @brief Options to modify KVMap behavior, allocation strategy, etc.
 */
typedef struct __VD_KVMapInitOptions {
    /** A value from [0.3, 0.9] that determines the amount of slots assigned to the addressable region. */
    float address_scale;
} VdKVMapInitOptions;


/**
 * Expect T: struct { TKey k; TValue v; };
 */

#define VD_KVMAP
#define VD_KVMAP_DEFAULT_CAP              1024
#define VD_KVMAP_HEADER(m)                ((Vd__KVMapHeader*)(((Vdu8*)(m)) - sizeof(Vd__KVMapHeader)))
#define VD_KVMAP_INIT(m, arena, cap, o)   ((m) = (vd__kvmap_init((arena), sizeof(m->k), sizeof(m->v), (cap), (o))))
#define VD_KVMAP_INIT_DEFAULT(m, arena)   VD_KVMAP_INIT((m), (arena), VD_KVMAP_DEFAULT_CAP)
#define VD_KVMAP_SET(m, k, v)             vd__kvmap_set((m), (k), (void*)(v), VD__KVMAP_SET_MODE_NEW_ONLY)
#define VD_KVMAP_GET(m, k, v)             vd__kvmap_get((m), (k), (void*)(v))
#define VD_KVMAP_RM(m, k)                 (vd__kvmap_get_bin((m), (k), VD__KVMAP_GET_BIN_FLAGS_SET_UNUSED) != 0)
#define VD_KVMAP_OVERWRITE(m, k, v)       vd__kvmap_set((m), (k), (void*)(v), VD__KVMAP_SET_MODE_OVERWRITE)
#define VD_KVMAP_COUNT(m)                 ((m) == 0 ? 0 : VD_KVMAP_HEADER(m)->taken)
#define VD_KVMAP_TOTAL_CAP(m)             ((m) == 0 ? 0 : VD_KVMAP_HEADER(m)->cap_total)
#define VD_KVMAP_ARENAP(m)                ((m) == 0 ? 0 : VD_KVMAP_HEADER(m)->arena)
#define VD_KVMAP_KSIZE(m)                 (VD_KVMAP_HEADER(m)->ksize)
#define VD_KVMAP_VSIZE(m)                 (VD_KVMAP_HEADER(m)->vsize)
#define VD_KVMAP_ENTRY_SIZE(m)            (VD_KVMAP_KSIZE(m) + VD_KVMAP_VSIZE(m) + sizeof(Vd__KVMapBinPrefix))
#define VD_KVMAP_GET_BIN(m, i)            ((Vd__KVMapBinPrefix*) (((Vdu8*)m) + (VD_KVMAP_ENTRY_SIZE(m) * (i))))
#define VD_KVMAP_GET_KEY(m, i)            ((void*)((Vdu8*)VD_KVMAP_GET_BIN(m, i) + sizeof(Vd__KVMapBinPrefix)))
#define VD_KVMAP_GET_VAL(m, i)            ((void*)((Vdu8*)VD_KVMAP_GET_BIN(m, i) + sizeof(Vd__KVMapBinPrefix + VD_KVMAP_KSIZE(m))))
#define VD_KVMAP_GET_BIN_USED(m, i)       VD_KVMAP_GET_BIN(m, i)->used
#define VD_KVMAP_GET_BIN_NEXT(m, i)       VD_KVMAP_GET_BIN(m, i)->next
#define VD_KVMAP_GET_BIN_INDEX(m, b)      (size_t)(((uintptr_t)b - (uintptr_t)m) / (uintptr_t)(VD_KVMAP_ENTRY_SIZE(m)))
#define VD_KVMAP_GET_BIN_KPTR(m, i)       ((void*)(((Vdu8*)VD_KVMAP_GET_BIN(m, i)) + sizeof(Vd__KVMapBinPrefix)))
#define VD_KVMAP_GET_BIN_VPTR(m, i)       ((void*)(((Vdu8*)VD_KVMAP_GET_BIN(m, i)) + sizeof(Vd__KVMapBinPrefix) + VD_KVMAP_KSIZE(m)))
#define VD_KVMAP_BIN_MOVE_TO_KPTR(b)      ((void*)(((Vdu8*)(b)) + sizeof(Vd__KVMapBinPrefix)))
#define VD_KVMAP_BIN_MOVE_TO_VPTR(m, b)   ((void*)(((Vdu8*)(b)) + sizeof(Vd__KVMapBinPrefix) + VD_KVMAP_KSIZE(map)))

void*                 vd__kvmap_init(VdArena *arena, Vdu32 ksize, Vdu32 vsize, Vdu32 cap, VdKVMapInitOptions *options);
Vd__KVMapBinPrefix*   vd__kvmap_get_bin(void *map, void *key, Vd__KVMapGetBinFlags op);
VD_INLINE Vdb32       vd__kvmap_get(void *map, void *key, void *value);
VD_INLINE Vdb32       vd__kvmap_set(void *map, void *key, void *value, Vd__KVMapSetMode mode);

VD_INLINE Vdb32 vd__kvmap_get(void *map, void *key, void *value)
{
    // Look for bin 
    Vd__KVMapBinPrefix *bin = vd__kvmap_get_bin(map, key, VD__KVMAP_GET_BIN_FLAGS_GET_EXISTING);
    if (bin == 0) return VD_FALSE;

    // If found the copy over the data
    Vdu8 *bin_data = (Vdu8*)VD_KVMAP_BIN_MOVE_TO_VPTR(map, bin);
    VD_MEMCPY(value, bin_data, VD_KVMAP_HEADER(map)->vsize);
    return VD_TRUE;
}

VD_INLINE Vdb32 vd__kvmap_set(void *map, void *key, void *value, Vd__KVMapSetMode mode)
{
    if (mode == VD__KVMAP_SET_MODE_NEW_ONLY) {
        Vd__KVMapBinPrefix *bin = vd__kvmap_get_bin(map, key, VD__KVMAP_GET_BIN_FLAGS_CREATE);
        if (bin == 0) return VD_FALSE;

        Vdu8ptr bin_data = (Vdu8ptr)VD_KVMAP_BIN_MOVE_TO_VPTR(map, bin);
        VD_MEMCPY(bin_data, value, VD_KVMAP_VSIZE(map));
        return VD_TRUE;
    } else {
        Vd__KVMapBinPrefix *bin = (Vd__KVMapBinPrefix*)vd__kvmap_get_bin(map, key, (Vd__KVMapGetBinFlags)(VD__KVMAP_GET_BIN_FLAGS_CREATE | VD__KVMAP_GET_BIN_FLAGS_GET_EXISTING));
        if (bin == 0) return VD_FALSE;

        Vdu8ptr bin_data = (Vdu8ptr)VD_KVMAP_BIN_MOVE_TO_VPTR(map, bin);
        VD_MEMCPY(bin_data, value, VD_KVMAP_HEADER(map)->vsize);
        return VD_TRUE;
    }
}

#if VD_MACRO_ABBREVIATIONS
#define kvmap
#define kvmap_init(m, arena, cap, o)      VD_KVMAP_INIT(m, arena, cap, o)
#define kvmap_init_default(m, arena)      VD_KVMAP_INIT_DEFAULT(m, arena)
#define kvmap_set(m, k, v)                VD_KVMAP_SET(m, k, v)
#define kvmap_get(m, k, v)                VD_KVMAP_GET(m, k, v)
#define kvmap_rm(m ,k)                    VD_KVMAP_RM(m, k)
#define kvmap_overwrite(m, k, v)          VD_KVMAP_OVERWRITE(m, k, v)
#endif // VD_MACRO_ABBREVIATIONS

/* ----FILESYSTEM---------------------------------------------------------------------------------------------------- */
#include <stdio.h>
VD_INLINE Vdu8*  vd_dump_file_to_bytes(VdArena *arena, Vdcstr file_path, Vdusize *len);
VD_INLINE Vdcstr vd_dump_file_to_cstr(VdArena *arena, Vdcstr file_path, Vdusize *len);

VD_INLINE Vdu8*  vd_dump_file_to_bytes(VdArena *arena, Vdcstr file_path, Vdusize *len)
{
    FILE *f = fopen(file_path, "rb");
    fseek(f, 0, SEEK_END);
    Vdusize size = ftell(f);
    fseek(f, 0, SEEK_SET);

    Vdu8 *result = (Vdu8*)vd_arena_alloc(arena, size);
    fread(result, size, 1, f);
    *len = size;
    return result;
}

VD_INLINE Vdcstr vd_dump_file_to_cstr(VdArena *arena, Vdcstr file_path, Vdusize *len)
{
    FILE *f = fopen(file_path, "rb");
    if (f == 0) return 0;
    
    fseek(f, 0, SEEK_END);
    Vdusize size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *result = (char*)vd_arena_alloc(arena, size + 1);
    fread(result, size, 1, f);
    result[size] = 0;
    *len = size;
    return result;
}

#if VD_MACRO_ABBREVIATIONS
#define dump_file_to_bytes(arena, file_path, len) vd_dump_file_to_bytes(arena, file_path, len);
#define dump_file_to_cstr(arena, file_path, len)  vd_dump_file_to_cstr(arena, file_path, len);
#endif // VD_MACRO_ABBREVIATIONS

/* ----SCRATCH------------------------------------------------------------------------------------------------------- */
#ifndef VD_SCRATCH_PAGE_COUNT
#define VD_SCRATCH_PAGE_COUNT 8
#endif // VD_SCRATCH_PAGE_COUNT

#ifndef VD_SCRATCH_PAGE_SIZE
#define VD_SCRATCH_PAGE_SIZE VD_KILOBYTES(64)
#endif // VD_SCRATCH_PAGE_SIZE 

typedef struct __VD_Scratch {
    VdArena   arenas[VD_SCRATCH_PAGE_COUNT];
    Vdusize   curr_arena;
} VdScratch;

void     vd_scratch_init(VdScratch *scratch);
VdArena* vd_scratch_get_arena(VdScratch *scratch);
void     vd_scratch_return_arena(VdScratch *scratch, VdArena *arena);

#if VD_MACRO_ABBREVIATIONS
#define Scratch                              VdScratch
#define scratch_init(scratch)                vd_scratch_init(scratch)
#define scratch_get_arena(scratch)           vd_scratch_get_arena(scratch)
#define scratch_return_arena(scratch, arena) vd_scratch_return_arena(scratch, arena)
#endif // VD_MACRO_ABBREVIATIONS

/* ----LOG----------------------------------------------------------------------------------------------------------- */
#define VD_PROC_LOG(name) void name(int verbosity, Vdcstr string)
typedef VD_PROC_LOG(VdProcLog);

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

#define VD_LOG_ADD_NEWLINE(s)  s "\n"
#define VD_ERRF(fmt, ...)      VD_LOG_IMPL(VD_LOG_VERBOSITY_ERROR,   VD_LOG_ADD_NEWLINE(fmt), __VA_ARGS__)
#define VD_WRNF(fmt, ...)      VD_LOG_IMPL(VD_LOG_VERBOSITY_WARNING, VD_LOG_ADD_NEWLINE(fmt), __VA_ARGS__)
#define VD_LOGF(fmt, ...)      VD_LOG_IMPL(VD_LOG_VERBOSITY_LOG,     VD_LOG_ADD_NEWLINE(fmt), __VA_ARGS__)
#define VD_DBGF(fmt, ...)      VD_LOG_IMPL(VD_LOG_VERBOSITY_DEBUG,   VD_LOG_ADD_NEWLINE(fmt), __VA_ARGS__)

#if VD_MACRO_ABBREVIATIONS
#define ProcLog           VdProcLog
#define ERRF(fmt, ...)    VD_ERRF(fmt, __VA_ARGS__)
#define WRNF(fmt, ...)    VD_WRNF(fmt, __VA_ARGS__)
#define LOGF(fmt, ...)    VD_LOGF(fmt, __VA_ARGS__)
#define DBGF(fmt, ...)    VD_DBGF(fmt, __VA_ARGS__)
#endif // VD_MACRO_ABBREVIATIONS

/* ----THREAD CONTEXT------------------------------------------------------------------------------------------------ */
#ifndef VD_CUSTOM_THREAD_CONTEXT
typedef struct {
    VdScratch scratch;
} VdThreadContext;

#define VD_THREAD_CONTEXT_TYPE          VdThreadContext
#define VD_THREAD_CONTEXT_VARNAME       Thread_Context
#define VD_THREAD_CONTEXT_SCRATCH(tc)   (&tc->scratch)
#endif // !VD_CUSTOM_THREAD_CONTEXT

#define VD_THREAD_CONTEXT_GET()         (VD_THREAD_CONTEXT_VARNAME)
#define VD_THREAD_CONTEXT_SET(value)    (VD_THREAD_CONTEXT_VARNAME = value)
extern VD_THREAD_CONTEXT_TYPE * VD_THREAD_CONTEXT_VARNAME;

#define VD_SCRATCH()                    VD_THREAD_CONTEXT_SCRATCH(VD_THREAD_CONTEXT_GET())
#define VD_GET_SCRATCH_ARENA()          vd_scratch_get_arena(VD_SCRATCH())
#define VD_RETURN_SCRATCH_ARENA(a)      vd_scratch_return_arena(VD_SCRATCH(), a)

#ifndef VD_ENABLE_SCRATCH_USE_IN_LIBRARY
#define VD_ENABLE_SCRATCH_USE_IN_LIBRARY 0
#endif // !VD_ENABLE_SCRATCH_USE_IN_LIBRARY

#if VD_MACRO_ABBREVIATIONS
#define ThreadContext VdThreadContext
#endif // VD_MACRO_ABBREVIATIONS

/* ----TESTING------------------------------------------------------------------------------------------------------- */
#ifndef VD_INCLUDE_TESTS
#define VD_INCLUDE_TESTS 0
#endif // VD_INCLUDE_TESTS

#if VD_INCLUDE_TESTS && !VD_HOST_COMPILER_UNKNOWN
extern VdArena *Test_Arena;

typedef struct __VD_TestResult {
    int         ok;
    const char  *err;
} VdTestResult;

#define VD_PROC_TEST(name) VdTestResult name(void)
typedef VD_PROC_TEST(VdProcTest);

typedef struct __VD_TestEntry {
    const char   *name;
    VdProcTest *test;
} VdTestEntry;

#define VD_TEST_PROC_ID(counter)        VD_STRING_JOIN2(vd_test_proc_, counter)
#define VD_TEST_ENTRY_ID(counter)       VD_STRING_JOIN2(vd_test_entry_, counter)
#define VD_TEST_REG_ID(counter)         VD_STRING_JOIN2(vd_test_reg_, counter)
#define VD_TEST_REG_PTR_ID(counter)     VD_STRING_JOIN2(vd_test_reg_ptr_, counter)

#if VD_HOST_COMPILER_MSVC
#pragma section(".CRT$XCU", read)

#define VD_TEST_IMPL(string, counter) \
    static VD_PROC_TEST(VD_TEST_PROC_ID(counter)); \
    static void VD_TEST_REG_ID(counter)(void); \
    __declspec(allocate(".CRT$XCU")) void (*VD_TEST_REG_PTR_ID(counter))(void) = VD_TEST_REG_ID(counter); \
    __pragma(comment(linker, "/include:" VD_STRINGIFY(VD_TEST_REG_PTR_ID(counter)))) \
    static void VD_TEST_REG_ID(counter)(void) {\
        vd__test_register(string, VD_TEST_PROC_ID(counter)); \
    } \
    static VD_PROC_TEST(VD_TEST_PROC_ID(counter))

#elif VD_HOST_COMPILER_CLANG
#define VD_TEST_IMPL(string, counter) \
    static VD_PROC_TEST(VD_TEST_PROC_ID(counter)); \
    static void __attribute__((constructor)) VD_TEST_REG_ID(counter)() { \
        vd__test_register(string, VD_TEST_PROC_ID(counter)); \
    } \
    static VD_PROC_TEST(VD_TEST_PROC_ID(counter))

#endif  // VD_HOST_COMPILER_MSVC, VD_HOST_COMPILER_CLANG

extern VdTestEntry *Vd__Test_Entries;
extern Vdusize     Vd__Cap_Test_Entries;
extern Vdu32       Vd__Len_Test_Entries;

static VD_INLINE void vd__test_register(const char *name, VdProcTest *proc)
{
    if ((Vd__Len_Test_Entries + 1) > Vd__Cap_Test_Entries) {
        Vdusize old_cap = Vd__Cap_Test_Entries;
        Vdusize new_cap;
        if (old_cap == 0) {
            new_cap = 64;
        } else {
            new_cap = 2 * old_cap;
        }

        Vd__Test_Entries = (VdTestEntry*)VD_REALLOC(Vd__Test_Entries, old_cap * sizeof(VdTestEntry), new_cap * sizeof(VdTestEntry));
        Vd__Cap_Test_Entries = new_cap;
    }

    VdTestEntry* e = &Vd__Test_Entries[Vd__Len_Test_Entries++];
    e->name = name;
    e->test = proc;
}

#define VD_TEST(string) VD_TEST_IMPL(string, __COUNTER__)

typedef struct {
    VdProcLog *log_impl;
    char        buf[1024];
} VdTestContext;

extern VdTestContext *Vd__Global_Test_Context;

#define VD_TEST_LOG_IMPL(fmt, ...) \
        snprintf(Vd__Global_Test_Context->buf, 1024, fmt "\n", __VA_ARGS__); \
        Vd__Global_Test_Context->log_impl(VD_LOG_VERBOSITY_LOG, Vd__Global_Test_Context->buf); \

#define VD_TEST_LOG(fmt, ...) VD_TEST_LOG_IMPL(fmt, __VA_ARGS__)

extern void vd_test_set_context(VdTestContext *context);
extern void vd_test_get_tests(VdTestEntry **out_entries, Vdu32 *out_num_entries);
extern void vd_test_main(void);

#define VD_TEST_ERR(msg)          return ((VdTestResult) { .ok = 0, .err = msg })
#define VD_TEST_OK()              return ((VdTestResult) { .ok = 1, .err = 0 })
#define VD_TEST_ASSERT(desc, x)   do { if (!(x))       { VD_TEST_ERR(desc "\nExpected: " #x " would be true");  } } while (0)
#define VD_TEST_TRUE(desc, x)     do { if (!(x))       { VD_TEST_ERR(desc "\nExpected: " #x " == true");        } } while (0)
#define VD_TEST_FALSE(desc, x)    do { if ( (x))       { VD_TEST_ERR(desc "\nExpected: " #x " == false");       } } while (0)
#define VD_TEST_EQ(desc, x, y)    do { if ((x) != (y)) { VD_TEST_ERR(desc "\nExpected: " #x " == " #y );        } } while (0)
#define VD_TEST_NEQ(desc, x, y)   do { if ((x) == (y)) { VD_TEST_ERR(desc "\nExpected: " #x " != " #y );        } } while (0)
#define VD_TEST_LT(desc, x, y)    do { if ((x) >= (y)) { VD_TEST_ERR(desc "\nExpected: " #x " < " #y );         } } while (0)
#define VD_TEST_GT(desc, x, y)    do { if ((x) <= (y)) { VD_TEST_ERR(desc "\nExpected: " #x " > " #y );         } } while (0)
#define VD_TEST_LE(desc, x, y)    do { if ((x) >  (y)) { VD_TEST_ERR(desc "\nExpected: " #x " <= " #y );        } } while (0) 
#define VD_TEST_GE(desc, x, y)    do { if ((x) <  (y)) { VD_TEST_ERR(desc "\nExpected: " #x " >= " #y );        } } while (0) 

#if VD_MACRO_ABBREVIATIONS
#define ProcTest                                     VdProcTest
#define TestEntry                                    VdTestEntry
#define TestResult                                   VdTestResult
#define TestContext                                  VdTestContext
#define test_set_context(context)                    vd_test_set_context(context)
#define test_get_tests(out_entries, out_num_entries) vd_test_get_tests(out_entries, out_num_entries)
#define test_main()                                  vd_test_main()
#endif // VD_MACRO_ABBREVIATIONS
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

/* ----TIMING IMPL--------------------------------------------------------------------------------------------------- */
#if VD_PLATFORM_WINDOWS
#include <windows.h>

#ifndef _PROFILEAPI_H_
#if VD_CPP
extern "C" {
#endif // VD_CPP

extern int QueryPerformanceCounter(Vdi64 *perf_count);
extern int QueryPerformanceFrequency(Vdi64 *perf_freq);

#if VD_CPP
}
#endif // VD_CPP
#endif // !_PROFILEAPI_H_

static Vdi64 Vd__Windows_Performance_Frequency = 0;

VdHiTime vd_hitime_get(void)
{
    Vdu64 counter;
    QueryPerformanceCounter((LARGE_INTEGER*)&counter);

    VdHiTime result = { counter };
    return result;
}

VdHiTime vd_hitime_sub(VdHiTime a, VdHiTime b)
{    VdHiTime result = { a.performance_counter - b.performance_counter };
    return result;
}

Vdu64 vd_hitime_ns(VdHiTime time_spec)
{
    // @todo(mdodis): Windows_Performance_Frequency on startup
    if (Vd__Windows_Performance_Frequency == 0) {
        QueryPerformanceFrequency((LARGE_INTEGER*)&Vd__Windows_Performance_Frequency);
    }

    Vdu64 q  =  time_spec.performance_counter / Vd__Windows_Performance_Frequency;
    Vdu64 r  =  time_spec.performance_counter % Vd__Windows_Performance_Frequency;
    Vdu64 ns =  q * 1000000000ULL;
    ns         += (r * 1000000000ULL) / Vd__Windows_Performance_Frequency;
    return ns;
}

Vdf64 vd_hitime_fms64(VdHiTime time_spec)
{
    return ((Vdf64)vd_hitime_ns(time_spec) / 1000000.0);
}

Vdu64 vd_hitime_ms(VdHiTime time_spec)
{
    // @todo(mdodis): Windows_Performance_Frequency on startup
    if (Vd__Windows_Performance_Frequency == 0) {
        QueryPerformanceFrequency((LARGE_INTEGER*)&Vd__Windows_Performance_Frequency);
    }

    return (Vdu64)(((time_spec.performance_counter) * 1000) / Vd__Windows_Performance_Frequency);
}

#else
#error "HiTime not supported on this platform!"
#endif // VD_PLATFORM_WINDOWS

/* ----VIRTUAL MEMORY IMPL------------------------------------------------------------------------------------------- */
#if VD_INCLUDE_PLATFORM_SPECIFIC_FUNCTIONALITY
#if !VD_VM_CUSTOM

#if VD_PLATFORM_MACOS
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>

Vdusize vd_vm_get_page_size(void)
{
    return _SC_PAGE_SIZE;
}

void *vd_vm_reserve(Vdusize len)
{
    void *result = mmap(0, len, PROT_NONE, MAP_ANON | MAP_PRIVATE, -1, 0);
    if (result == MAP_FAILED) {
        fprintf(stderr, "vm_decommit failed: %d\n", errno);
        VD_ASSERT(VD_FALSE);
    }
    return result;
}

void *vd_vm_commit(void *addr, Vdusize len)
{
    void *result = mmap(addr, len, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_PRIVATE | MAP_ANON, -1, 0);
    if (result == MAP_FAILED) {
        fprintf(stderr, "vm_decommit failed: %d\n", errno);
        VD_ASSERT(VD_FALSE);
    }
    return result;
}

void *vd_vm_decommit(void *addr, Vdusize len)
{
    void *result = mmap(addr, len, PROT_NONE, MAP_FIXED | MAP_PRIVATE | MAP_ANON, -1, 0);
    if (result == MAP_FAILED) {
        fprintf(stderr, "vm_decommit failed: %d\n", errno);
        VD_ASSERT(VD_FALSE);
    }
    return result;
}

void vd_vm_release(void *addr, Vdusize len)
{
    munmap(addr, len);
}

#elif VD_PLATFORM_WINDOWS
#include <windows.h>

Vdusize vd_vm_get_page_size(void)
{
    // @todo(mdodis): GetSystemInfo
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    return system_info.dwPageSize;
}

void *vd_vm_reserve(Vdusize len)
{
    return VirtualAlloc(0, len, MEM_RESERVE, PAGE_READWRITE);
}

void *vd_vm_commit(void *addr, Vdusize len)
{
    return VirtualAlloc(addr, len, MEM_COMMIT, PAGE_READWRITE);
}

void *vd_vm_decommit(void *addr, Vdusize len)
{
    return VirtualFree(addr, len, MEM_DECOMMIT) ? 0 : addr;
}

void vd_vm_release(void *addr, Vdusize len)
{
    VD_UNUSED(len);

    VirtualFree(addr, 0, MEM_RELEASE);
}
#endif // VD_PLATFORM_MACOS, VD_PLATFORM_WINDOWS

#endif // !VD_VM_CUSTOM
#endif // VD_INCLUDE_PLATFORM_SPECIFIC_FUNCTIONALITY

/* ----SYSTEM ALLOCATOR IMPL----------------------------------------------------------------------------------------- */
#define VD_SYSTEM_HEAP_RESERVE_PAGE_COUNT 16777216

static VdSystemHeap Vd_System_Heap_Global = {0};

VD_API VdSystemHeap *vd_system_heap_global(void)
{
    return &Vd_System_Heap_Global;
}

VD_API void vd_system_heap_set_reserve_page_count(VdSystemHeap *h, Vdusize page_count)
{
    VD_ASSERT(h->buf != 0 && "Heap has already been allocated!");
    if (h->page_size == 0) {
        h->page_size = vd_vm_get_page_size();
    }

    h->reserved = h->page_size * page_count;
}

VD_API void *vd_system_heap_alloc(VdSystemHeap *h, Vdusize size, Vdusize align)
{
    if (h->buf == 0) {
        if (h->page_size == 0) {
            h->page_size = vd_vm_get_page_size();
        }

        if (h->reserved == 0) {
            h->reserved = h->page_size * VD_SYSTEM_HEAP_RESERVE_PAGE_COUNT;
        }

        h->buf = (Vduptr)vd_vm_reserve(h->reserved);
    }

    // PAGE  0 |---------|
    //         [+++++++++|
    //         |+++++++++|
    //         |+++++++++|
    //         |+++++]   | -> h->buf + h->offset (curr_ptr)
    // PAGE  1 |---------| -> h->buf + h->committed (now)
    //         |*        | -> desired
    //         |         |
    //         |         |
    //         |         |
    // PAGE  2 |---------|
    //         |         |
    //         |  *      | -> desired_end
    //         |         |
    //         |         |
    // PAGE  3 |---------| -> h->buf + h->reserved

    Vduptr now      = (Vduptr)h->buf + (Vduptr)h->committed;
    Vduptr end      = (Vduptr)h->buf + (Vduptr)h->reserved;

    Vduptr curr_ptr    = (Vduptr)h->buf + (Vduptr)h->offset;
    Vduptr desired     = vd_align_forward(curr_ptr, align);
    Vduptr desired_end = desired + size;
    Vduptr offset   = desired - (Vduptr)h->buf;

    if (desired_end > end) {
        VD_DEBUG_BREAK();
    }

    if ((desired_end - curr_ptr) <= now) {
        // Allocation fits inside the page(s)
        // no need to commit more
        h->offset += offset;

        return (void*)desired;
    } else {
        // Need to allocate n pages
        Vdusize num_pages = (desired_end - now) / h->page_size;
        num_pages += ((desired_end - now) % h->page_size) != 0 ? 1 : 0;

        h->committed += num_pages * h->page_size;
        VD_ASSERT(h->committed % h->page_size == 0);
        VD_ASSERT(vd_vm_commit((void*)(h->buf + h->committed), num_pages * h->page_size) != 0);

        h->offset += offset;
        return (void*)desired;
    }
}

VD_API void vd_system_heap_empty(VdSystemHeap *h)
{
    vd_vm_release((void*)h->buf, h->reserved * h->page_size);
}

/* ----ARENA IMPL---------------------------------------------------------------------------------------------------- */
void vd_arena_init(VdArena *a, void *buf, size_t len)
{
    a->buf = buf;
    a->buf_len = len;
    a->curr_offset = 0;
    a->prev_offset = 0;
}

void *vd_arena_alloc_align(VdArena *a, size_t size, size_t align)
{
    uintptr_t curr_ptr = (uintptr_t)a->buf + (uintptr_t)a->curr_offset;
    uintptr_t offset = vd_align_forward(curr_ptr, align);
    offset -= (Vduptr)a->buf;

    if (offset + size <= a->buf_len) {
        void *ptr = 0;
        if (a->flags & VD_ARENA_FLAGS_USE_MALLOC) {
#if VD_USE_CRT
            ptr = VD_MALLOC(size);
#endif // VD_USE_CRT
        } else {
            ptr = &a->buf[offset];
            a->prev_offset = offset;
            a->curr_offset = offset + size;
        }

        VD_MEMSET(ptr, 0, size);
        return ptr;
    }

    VD_DEBUG_BREAK();
    return 0;
}

void *vd_arena_resize_align(VdArena *a, void *old_memory, size_t old_size, size_t new_size, size_t align)
{
    VD_ASSERT(vd_is_power_of_two(align));

    Vdu8* old_mem = (Vdu8*)old_memory;

    if (old_mem == 0 || old_size == 0) {
        return vd_arena_alloc_align(a, new_size, align);
    } else if (a->buf <= old_mem && old_mem < a->buf + a->buf_len) {
        if (a->buf + a->prev_offset == old_mem) {
            a->curr_offset = a->prev_offset + new_size;
            if (new_size > old_size) {
                VD_MEMSET(&a->buf[a->curr_offset], 0, new_size - old_size);
            }

            return old_memory;
        } else {
            void *new_memory = vd_arena_alloc_align(a, new_size, align);
            Vdusize copy_size = old_size < new_size ? old_size : new_size;

            VD_MEMMOVE(new_memory, old_memory, copy_size);
            return new_memory;
        }
    } else {
        VD_DEBUG_BREAK();
    }
    return 0;
}

void vd_arena_clear(VdArena *a)
{
    VD_MEMSET(a->buf, 0, a->curr_offset);
    a->curr_offset = 0;
    a->prev_offset = 0;
}

Vdb32 vd_arena_free(VdArena *a, void *memory, size_t size)
{
    VD_UNUSED(size);

    Vduptr last_ptr = (Vduptr) (a->buf + a->prev_offset);
    Vduptr mptr     = (Vduptr)memory;

    if (mptr == last_ptr) {
        a->curr_offset = a->prev_offset;
        a->prev_offset = 0;
        return VD_TRUE;
    }

    return VD_FALSE;
}

/* ----SCRATCH IMPL-------------------------------------------------------------------------------------------------- */
void vd_scratch_init(VdScratch *scratch)
{
    for (Vdusize i = 0; i < VD_SCRATCH_PAGE_COUNT; ++i)
    {
        void *a = (void*)VD_MALLOC(VD_SCRATCH_PAGE_SIZE);
        VD_MEMSET(a, 0, VD_SCRATCH_PAGE_SIZE);
        vd_arena_init(&scratch->arenas[i], a, VD_SCRATCH_PAGE_SIZE);
    }
}

VdArena *vd_scratch_get_arena(VdScratch *scratch)
{
    VD_ASSERT(scratch->curr_arena < VD_SCRATCH_PAGE_COUNT);
    return &scratch->arenas[scratch->curr_arena++];
}

void vd_scratch_return_arena(VdScratch *scratch, VdArena *arena)
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
Vdb32 vd_parse_u64(VdStr s, Vdu64 *r)
{
    Vdu64 result = 0;
    Vdusize i = 0;
    while (i < s.len && vd_is_ascii_digit(s.s[i])) {
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
Vdb32 vd__arg_advance(VdArg *arg) {
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
            if (vd_arg_at_end(arg)) return VD_FALSE;   \
            if (!vd__arg_advance(arg)) return VD_FALSE; \
        }                                                \
    } while(0)

VdArg vd_arg_new(int argc, char **argv)
{
    return (VdArg) {
        .argc    = argc,
        .argv    = argv,
        .argi    = 0,
        .ci      = 0,
    };
}

void vd_arg_skip_program_name(VdArg *arg)
{
   arg->argi++;
   arg->ci = 0;
}

Vdb32 vd_arg_at_end(VdArg *arg)
{
    return (arg->argi == arg->argc) ||
           (arg->argi == (arg->argc - 1) &&
            arg->argv[arg->argi][arg->ci] == 0);
}

void vd_arg_skip(VdArg *arg)
{
    arg->argi++;
    arg->ci = 0;    
}

Vdb32 vd_arg_get_name(VdArg *arg, VdStr *name)
{
    VD_ARG_CHECK_NEXT(arg);

    if (arg->argv[arg->argi][arg->ci] != '-') {
        return VD_FALSE;
    }

    arg->ci = 1;

    VdStr result;
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

Vdb32 vd_arg_get_uint(VdArg *arg, Vdu64 *i) {
    VD_UNUSED(i);
    VD_TODO();

    VD_ARG_CHECK_NEXT(arg);

    return VD_FALSE;
}

Vdb32 vd_arg_get_str(VdArg *arg, VdStr *str) {
    VD_ARG_CHECK_NEXT(arg);

    VdStr result;
    result.s = &arg->argv[arg->argi][0];
    result.len = 0;

    while (arg->argv[arg->argi][arg->ci] != 0) {
        arg->ci++;
        result.len++;
    }

    *str = result;
    return VD_TRUE;
}

Vdb32 vd_arg_expect_char(VdArg *arg, char c);

#undef VD_ARG_CHECK_NEXT

/* ----STRMAP IMPL--------------------------------------------------------------------------------------------------- */
static Vdb32 vd__strmap_check_key(VdStr check, Vd__StrmapBinPrefix *against) {
    Vdu32 prefix_len       = sizeof(against->key_prefix);    
    Vdu32 first_check_len  = prefix_len < (Vdu32)check.len ? prefix_len : (Vdu32)check.len;
    Vdu32 second_check_len = (Vdu32)check.len - prefix_len;

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

static void vd__strmap_emplace_key(void *map, Vd__StrmapBinPrefix *dst, char *src_key_prefix, char *src_key_rest, Vdu32 src_key_len)
{
    Vdu32 prefix_len     = sizeof(dst->key_prefix);
    Vdu32 first_copy_len = src_key_len > prefix_len ? prefix_len : src_key_len;

    VD_MEMCPY(dst->key_prefix, src_key_prefix, first_copy_len);

    Vdu32 new_len = src_key_len;

    if (src_key_len > prefix_len) {
        src_key_len -= prefix_len;

        // If the source key's length after the prefix doesn't fit in the array allocated for key_rest
        // Or if the key_rest_cap is zero because no key_rest has been allocated, then resize it
        if (src_key_len > dst->key_rest_cap) {

            // Allocate an arena big enough so that statistically, we won't have to ever care about
            // this again
            dst->key_rest = vd_arena_alloc(VD_STRMAP_ARENAP(map), src_key_len * 2);
            dst->key_rest_cap = src_key_len * 2;
        }

        // Copy the rest of the key
        VD_MEMCPY(dst->key_rest, src_key_rest, src_key_len);
    }

    dst->key_len = new_len;
}

static void vd__strmap_copy_key(void *map, VdStr key, Vd__StrmapBinPrefix *bin)
{
    char *key_prefix_part = key.s;
    char *key_rest_part = key.len > sizeof(bin->key_prefix) ? key.s + sizeof(bin->key_prefix) : 0;
    vd__strmap_emplace_key(map, bin, key_prefix_part, key_rest_part, (Vdu32)key.len);
}

void* vd__strmap_init(VdArena *arena, Vdu32 tsize, Vdu32 cap, VdStrmapInitOptions *options)
{
    Vd__StrmapHeader *map;
    const Vdu32 bin_size = sizeof(Vd__StrmapBinPrefix) + tsize;

    map = vd_arena_alloc(arena, sizeof(Vd__StrmapHeader) + bin_size * cap);

    Vd__StrmapBinPrefix *bins = (Vd__StrmapBinPrefix*)(((Vdu8ptr)map) + sizeof(Vd__StrmapHeader));

    float address_scale = 0.863f;
    if (options != 0) address_scale = options->address_scale;

    map->cap_total    = cap;
    map->cap          = (Vdu32) (((Vdf32)cap) * address_scale);
    map->taken        = 0;
    map->tsize        = tsize;
    map->arena        = arena;

    if ((options != 0) && (options->average_key_len > sizeof(((Vd__StrmapBinPrefix*)0)->key_prefix))) {
        for (Vdu32 i = 0; i < map->cap_total; ++i) {
            bins[i].key_rest = vd_arena_alloc(arena, options->average_key_len);
            VD_ASSERT(bins[i].key_rest);

            bins[i].key_rest_cap = options->average_key_len;
        }
    }

    return (void*)((Vdu8*)map + sizeof(Vd__StrmapHeader));
}

// @todo(mdodis): Speed this up if VD_ENABLE_SCRATCH_USE_IN_LIBRARY is enabled
//                by using a simple queue to store re-inserted bins
static void vd__strmap_reinsert_chain(void *map, Vd__StrmapBinPrefix *start)
{
    // Build the insertion queue
    Vd__StrmapBinPrefix *bin = start;
    Vd__StrmapBinPrefix *bin_next = start->next;
    while (bin_next != 0) {
        bin->insq = bin_next;

        bin       = bin_next;
        bin_next  = bin->next;
    }

    // Traverse the queue and for each item, set its bin to be unused, then re-add it
    Vd__StrmapBinPrefix *q = start->insq;
    bin = start;

    start->insq = 0;

    while (q != 0) {
        q->used = VD_FALSE;
        Vd__StrmapBinPrefix *nextq = q->insq;
        q->insq = 0;

        // Compose the key for q into a VdStr
        Vdu64 key_hash;
        {
            Vdusize key_len = q->key_len;
            VdArenaSave save = vd_arena_save(VD_STRMAP_ARENAP(map));

            char *key = vd_arena_alloc(VD_STRMAP_ARENAP(map), key_len);

            Vdusize first_copy_size  = key_len <= sizeof(q->key_prefix) ? key_len : sizeof(q->key_prefix);
            Vdusize second_copy_size = key_len <= sizeof(q->key_prefix) ? 0 : key_len - sizeof(q->key_prefix);

            VD_MEMCPY(key + 0, q->key_prefix, first_copy_size);

            if (second_copy_size > 0) {
                VD_MEMCPY(key + first_copy_size, q->key_rest, second_copy_size);
            }

            key_hash = vd_dhash64(key, key_len);

            vd_arena_restore(save);
        }

        Vdusize bin_index = key_hash % VD_STRMAP_HEADER(map)->cap;

        Vd__StrmapBinPrefix *existing_bin = VD_STRMAP_GET_BIN(map, bin_index);

        // if the key for some reason hashes to the same place, then ignore it
        if (existing_bin != q) {

            if (!existing_bin->used) {
                Vd__StrmapBinPrefix *new_bin = existing_bin;
                void *sptr = VD_STRMAP_BIN_MOVE_TO_VPTR(q);
                void *dptr = VD_STRMAP_BIN_MOVE_TO_VPTR(new_bin);

                new_bin->used = VD_TRUE;
                new_bin->next = 0;
                // Copy key
                vd__strmap_emplace_key(map, new_bin, q->key_prefix, q->key_rest, q->key_len);

                // Copy over value
                VD_MEMCPY(dptr, sptr, VD_STRMAP_TSIZE(map));
            } else {
                // Attempt to traverse the chain, if any
                Vd__StrmapBinPrefix *chain_head = existing_bin;
                while (chain_head->next == 0) {
                    chain_head = chain_head->next;
                }

                Vdu32 cursor = VD_STRMAP_HEADER(map)->cap_total;
                while ((cursor > 0) && (VD_STRMAP_GET_BIN(map, cursor - 1)->used)) {
                    cursor--;
                }

                // After n iterations, if no bin was found we should be at a bin that's still used
                // (i.e bin 0). In this case, return null.
                if (cursor == 0) {
                    VD_IMPOSSIBLE();
                }

                cursor--;
                Vd__StrmapBinPrefix *new_bin = VD_STRMAP_GET_BIN(map, cursor);

                // Allocate the bin
                new_bin->used = VD_TRUE;
                new_bin->next = 0;
                chain_head->next = new_bin;

                void *sptr = VD_STRMAP_BIN_MOVE_TO_VPTR(q);
                void *dptr = VD_STRMAP_BIN_MOVE_TO_VPTR(new_bin);

                // Copy key
                vd__strmap_emplace_key(map, new_bin, q->key_prefix, q->key_rest, q->key_len);

                // Copy over value
                VD_MEMCPY(dptr, sptr, VD_STRMAP_TSIZE(map));
            }
        } else {
            q->used = VD_TRUE;
        }

        q = nextq;
    }
}

Vd__StrmapBinPrefix* vd__strmap_get_bin(void *map, VdStr key, Vd__StrmapGetBinFlags op)
{
    // Get hash
    Vdu64 hash = vd_dhash64_str(key);
    Vdu64 bin_index = hash % VD_STRMAP_HEADER(map)->cap;

    // Find existing bin if any
    Vd__StrmapBinPrefix *existing_bin = VD_STRMAP_GET_BIN(map, bin_index);

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
    if (existing_bin->used || !(op & VD__STRMAP_GET_BIN_FLAGS_CREATE)) {
        Vdb32 found = VD_FALSE;

        // Make sure that the hash didn't put us in a bin where the key is equal to the query key
        if (vd__strmap_check_key(key, existing_bin)) {
            found = VD_TRUE;
        }

        // If the bin isn't what we searched for, traverse the chain until the end to find it.
        while (!found && (existing_bin->next != 0)) {
            existing_bin = existing_bin->next;

            if (vd__strmap_check_key(key, existing_bin)) {
                found = VD_TRUE;
                break;
            }
        }

        if (found) {
            if (op & VD__STRMAP_GET_BIN_FLAGS_SET_UNUSED) {
                // if found is true the bin must be unused, set its flag and return
                existing_bin->used = VD_FALSE;
                VD_STRMAP_HEADER(map)->taken--;
                vd__strmap_reinsert_chain(map, existing_bin);
                return existing_bin;

            } else if (op & VD__STRMAP_GET_BIN_FLAGS_GET_EXISTING) {
                // If the user wants to create a new bin, overwrite existing one, or just get it, and found is true, 
                // this means that the user queried with an existing key. Return the existing bin in thie case.
                return existing_bin;
            } else if (op & VD__STRMAP_GET_BIN_FLAGS_CREATE) {
                // If the user wants to create a new bin, and found is true, this means that the user
                // queried with the same key. Return null in this case.
                return 0;
            }
        }
    }

    // At this point, no bin that matched our key was found so if the user doesn't want a new bin, then
    // fail
    if (!(op & VD__STRMAP_GET_BIN_FLAGS_CREATE)) {
        return 0;
    }

    // If this bin is not used then allocate it
    if (!existing_bin->used) {
        vd__strmap_copy_key(map, key, existing_bin);
        existing_bin->used = VD_TRUE;
        existing_bin->next = 0;
        VD_STRMAP_HEADER(map)->taken++;
        return existing_bin;
    }

    // At this point, we handle a hash collision either for allocating a bin, or traversing the chain
    // due to a previous collision

    // Otherwise search starting from the cellar for an unused bin
    Vdu32 cursor = VD_STRMAP_HEADER(map)->cap_total;
    while ((cursor > 0) && (VD_STRMAP_GET_BIN(map, cursor - 1)->used)) {
        cursor--;
    }

    // After n iterations, if no bin was found we should be at a bin that's still used
    // (i.e bin 0). In this case, return null.
    if (cursor == 0) {
        return 0;
    }

    cursor--;
    Vd__StrmapBinPrefix *new_bin = VD_STRMAP_GET_BIN(map, cursor);

    // Allocate the bin
    new_bin->used = VD_TRUE;
    new_bin->next = 0;

    VD_STRMAP_HEADER(map)->taken++;

    // Mark bin as next. At this point, the previous search through the chain means that existing_bin
    // is now at the end of the chain 
    existing_bin->next = new_bin;

    // Copy new key and return
    vd__strmap_copy_key(map, key, new_bin);
    return new_bin;
}

/* ----KVMAP IMPL---------------------------------------------------------------------------------------------------- */
static Vdb32 vd__kvmap_check_key(Vdusize ksize, void *check, Vd__KVMapBinPrefix *against)
{
    void *key_against = (void *)(((Vdu8ptr)against) + sizeof(Vd__KVMapBinPrefix));
    return VD_MEMCMP(check, key_against, ksize) == 0;
}

static void vd__kvmap_copy_key(void *map, void *key, Vd__KVMapBinPrefix *bin)
{
    void *dst = (void *)(((Vdu8ptr)bin) + sizeof(Vd__KVMapBinPrefix));
    VD_MEMCPY(dst, key, VD_KVMAP_KSIZE(map));
}

// @todo(mdodis): Speed this up if VD_ENABLE_SCRATCH_USE_IN_LIBRARY is enabled
//                by using a simple queue to store re-inserted bins
static void vd__kvmap_reinsert_chain(void *map, Vd__KVMapBinPrefix *start)
{
    // Build the insertion queue
    Vd__KVMapBinPrefix *bin = start;
    Vd__KVMapBinPrefix *bin_next = start->next;
    while (bin_next != 0) {
        bin->insq = bin_next;

        bin       = bin_next;
        bin_next  = bin->next;
    }

    // Traverse the queue and for each item, set its bin to be unused, then re-add it
    Vd__KVMapBinPrefix *q = start->insq;
    bin = start;

    start->insq = 0;

    while (q != 0) {
        q->used = VD_FALSE;
        Vd__KVMapBinPrefix *nextq = q->insq;
        q->insq = 0;

        // Compose the key for q into a VdStr
        Vdu64 key_hash;
        {
            void *k = (void *)(((Vdu8ptr)q) + sizeof(Vd__KVMapBinPrefix));
            key_hash = vd_dhash64(k, VD_KVMAP_KSIZE(map));
        }

        Vdusize bin_index = key_hash % VD_KVMAP_HEADER(map)->cap;

        Vd__KVMapBinPrefix *existing_bin = VD_KVMAP_GET_BIN(map, bin_index);

        // if the key for some reason hashes to the same place, then ignore it
        if (existing_bin != q) {

            if (!existing_bin->used) {
                Vd__KVMapBinPrefix *new_bin = existing_bin;
                void *sptr = VD_KVMAP_BIN_MOVE_TO_VPTR(map, q);
                void *dptr = VD_KVMAP_BIN_MOVE_TO_VPTR(map, new_bin);

                new_bin->used = VD_TRUE;
                new_bin->next = 0;

                // Copy key
                vd__kvmap_copy_key(map, VD_KVMAP_BIN_MOVE_TO_KPTR(q), new_bin);

                // Copy over value
                VD_MEMCPY(dptr, sptr, VD_KVMAP_VSIZE(map));
            } else {
                // Attempt to traverse the chain, if any
                Vd__KVMapBinPrefix *chain_head = existing_bin;
                while (chain_head->next == 0) {
                    chain_head = chain_head->next;
                }

                Vdu32 cursor = VD_KVMAP_HEADER(map)->cap_total;
                while ((cursor > 0) && (VD_KVMAP_GET_BIN(map, cursor - 1)->used)) {
                    cursor--;
                }

                // After n iterations, if no bin was found we should be at a bin that's still used
                // (i.e bin 0). In this case, return null.
                if (cursor == 0) {
                    VD_IMPOSSIBLE();
                }

                cursor--;
                Vd__KVMapBinPrefix *new_bin = VD_KVMAP_GET_BIN(map, cursor);

                // Allocate the bin
                new_bin->used = VD_TRUE;
                new_bin->next = 0;
                chain_head->next = new_bin;

                void *sptr = VD_KVMAP_BIN_MOVE_TO_VPTR(map, q);
                void *dptr = VD_KVMAP_BIN_MOVE_TO_VPTR(map, new_bin);

                // Copy key
                vd__kvmap_copy_key(map, VD_KVMAP_BIN_MOVE_TO_KPTR(q), new_bin);

                // Copy over value
                VD_MEMCPY(dptr, sptr, VD_KVMAP_VSIZE(map));
            }
        } else {
            q->used = VD_TRUE;
        }

        q = nextq;
    }
}
void *vd__kvmap_init(VdArena *arena, Vdu32 ksize, Vdu32 vsize, Vdu32 cap, VdKVMapInitOptions *options)
{
    Vd__KVMapHeader *map;
    const Vdu32 bin_size = sizeof(Vd__KVMapBinPrefix) + ksize + vsize;    

    map = vd_arena_alloc(arena, sizeof(Vd__KVMapHeader) + bin_size * cap);
    float address_scale = 0.863f;
    if (options != 0) address_scale = options->address_scale;

    map->cap_total    = cap;
    map->cap          = (Vdu32) (((Vdf32)cap) * address_scale);
    map->taken        = 0;
    map->ksize        = ksize;
    map->vsize        = vsize;

    return (void*)(((Vdu8*)map) + sizeof(Vd__KVMapHeader));
}

Vd__KVMapBinPrefix *vd__kvmap_get_bin(void *map, void *key, Vd__KVMapGetBinFlags op)
{
    // Get hash
    Vdu64 hash = vd_dhash64(key, VD_KVMAP_KSIZE(map));
    Vdu64 bin_index = hash % VD_KVMAP_HEADER(map)->cap;

    // Find existing bin if any
    Vd__KVMapBinPrefix *existing_bin = VD_KVMAP_GET_BIN(map, bin_index);

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
    if (existing_bin->used || !(op & VD__KVMAP_GET_BIN_FLAGS_CREATE)) {
        Vdb32 found = VD_FALSE;

        // Make sure that the hash didn't put us in a bin where the key is equal to the query key
        if (vd__kvmap_check_key(VD_KVMAP_KSIZE(map), key, existing_bin)) {
            found = VD_TRUE;
        }

        // If the bin isn't what we searched for, traverse the chain until the end to find it.
        while (!found && (existing_bin->next != 0)) {
            existing_bin = existing_bin->next;

            if (vd__kvmap_check_key(VD_KVMAP_KSIZE(map), key, existing_bin)) {
                found = VD_TRUE;
                break;
            }
        }

        if (found) {
            if (op & VD__KVMAP_GET_BIN_FLAGS_SET_UNUSED) {
                // if found is true the bin must be unused, set its flag and return
                existing_bin->used = VD_FALSE;
                VD_KVMAP_HEADER(map)->taken--;
                vd__kvmap_reinsert_chain(map, existing_bin);
                return existing_bin;

            } else if (op & VD__KVMAP_GET_BIN_FLAGS_GET_EXISTING) {
                // If the user wants to create a new bin, overwrite existing one, or just get it, and found is true, 
                // this means that the user queried with an existing key. Return the existing bin in thie case.
                return existing_bin;
            } else if (op & VD__KVMAP_GET_BIN_FLAGS_CREATE) {
                // If the user wants to create a new bin, and found is true, this means that the user
                // queried with the same key. Return null in this case.
                return 0;
            }
        }
    }

    // At this point, no bin that matched our key was found so if the user doesn't want a new bin, then
    // fail
    if (!(op & VD__KVMAP_GET_BIN_FLAGS_CREATE)) {
        return 0;
    }

    // If this bin is not used then allocate it
    if (!existing_bin->used) {
        vd__kvmap_copy_key(map, key, existing_bin);
        existing_bin->used = VD_TRUE;
        existing_bin->next = 0;
        VD_KVMAP_HEADER(map)->taken++;
        return existing_bin;
    }

    // At this point, we handle a hash collision either for allocating a bin, or traversing the chain
    // due to a previous collision

    // Otherwise search starting from the cellar for an unused bin
    Vdu32 cursor = VD_KVMAP_HEADER(map)->cap_total;
    while ((cursor > 0) && (VD_KVMAP_GET_BIN(map, cursor - 1)->used)) {
        cursor--;
    }

    // After n iterations, if no bin was found we should be at a bin that's still used
    // (i.e bin 0). In this case, return null.
    if (cursor == 0) {
        return 0;
    }

    cursor--;
    Vd__KVMapBinPrefix *new_bin = VD_KVMAP_GET_BIN(map, cursor);

    // Allocate the bin
    new_bin->used = VD_TRUE;
    new_bin->next = 0;

    VD_KVMAP_HEADER(map)->taken++;

    // Mark bin as next. At this point, the previous search through the chain means that existing_bin
    // is now at the end of the chain 
    existing_bin->next = new_bin;

    // Copy new key and return
    vd__kvmap_copy_key(map, key, new_bin);
    return new_bin;
}

/* ----TESTING IMPL-------------------------------------------------------------------------------------------------- */
#if VD_INCLUDE_TESTS

static int vd__run_test(VdTestEntry* e) {
    vd_arena_clear(Test_Arena);
    Test_Arena->flags = 0;

    if (!e || !e->name || !e->test) return -1;

    VdTestResult r = e->test();
    if (r.ok) {
        VD_TEST_LOG("[%-60s]   OK", e->name);
        return 1;
    } else {
        VD_TEST_LOG("[%-60s]   FAILED", e->name);
        VD_TEST_LOG("%s", r.err);
        return 0;
    }
}

VdArena *Test_Arena;

#if !VD_HOST_COMPILER_UNKNOWN

VdTestContext *Vd__Global_Test_Context = 0;
VdTestEntry   *Vd__Test_Entries;
Vdusize        Vd__Cap_Test_Entries;
Vdu32          Vd__Len_Test_Entries;
#endif // VD_HOST_COMPILER_UNKNOWN

void vd_test_set_context(VdTestContext *context)
{
    Vd__Global_Test_Context = context;
}

void vd_test_get_tests(VdTestEntry **out_entries, Vdu32 *out_num_entries)
{
    *out_entries = Vd__Test_Entries;
    *out_num_entries = Vd__Len_Test_Entries;
}

#if VD_USE_CRT
static VD_PROC_LOG(vd__test_log) {
    VD_UNUSED(verbosity);
    fputs(string, stdout);
}

#include <stdio.h>
#endif // VD_USE_CRT

void vd_test_main(void)
{
    VdArena a;
    Test_Arena = &a;

#if VD_USE_CRT
    VdTestContext context;
    context.log_impl = vd__test_log;

    if (Vd__Global_Test_Context == 0) {
        vd_test_set_context(&context);
    }
#endif // VD_USE_CRT

    VD_ARENA_FROM_SYSTEM(Test_Arena, VD_MEGABYTES(64));

    int passed = 0, total = 0;
#if !VD_HOST_COMPILER_UNKNOWN 
    for (Vdusize i = 0; i < Vd__Len_Test_Entries; ++i) {
        if (vd__run_test(&Vd__Test_Entries[i])) {
            passed++;
        }
        total++;
    }

    VD_TEST_LOG("[%d/%d]\n", passed, total);
#else
#error "Cannot produce tests for unknown compiler!"
#endif // !VD_HOST_COMPILER_UNKNOWN
}

#if VD_INCLUDE_INTERNAL_TESTS

VD_TEST("ipow64u8") {
    VD_TEST_OK();    
}

typedef struct {
    int           value;
    VdDListNode node;
} Vd__TestListStruct;

VD_TEST("DList/Basic") {
    int num_nodes = 5;
    VdDList list;
    vd_dlist_init(&list);

    Vd__TestListStruct *structs = VD_ARENA_PUSH_ARRAY(Test_Arena, Vd__TestListStruct, num_nodes);

    for (int i = 0; i < num_nodes; ++i) {
        Vd__TestListStruct *s = &structs[i];
        s->value = i;
        vd_dlist_node_init(&s->node);
    }

    for (int i = 0; i < num_nodes; ++i) {
        Vd__TestListStruct *s = &structs[i];
        VD_TEST_EQ("Each node's next when initialized is linked to itself", s->node.next, &s->node);
        VD_TEST_EQ("Each node's prev when initialized is linked to itself", s->node.prev, &s->node);
        vd_dlist_node_init(&s->node);

        VD_TEST_TRUE("dlist_node_is_empty is true", vd_dlist_node_is_empty(&s->node));
    }


    for (int i = 0; i < num_nodes; ++i) {
        Vd__TestListStruct *s = &structs[i];
        vd_dlist_append(&list, &s->node);
    }

    {
        VD_DLIST_FOR_EACH_WITH_INDEX(&list, n, i) {
            Vd__TestListStruct *s = VD_CONTAINER_OF(n, Vd__TestListStruct, node);
            VD_TEST_EQ("Iterating all nodes in order and getting struct value is correct", s->value, (int)i);
        }
        VD_TEST_EQ("After iterating all nodes, the index should be equal to the last node's index + 1", i, (Vdusize)(num_nodes));
    }

    int i = 0;
    while (!vd_dlist_is_empty(&list)) {
        VdDListNode *n = vd_dlist_first(&list);
        vd_dlist_rm(&list, n);

        Vd__TestListStruct *s = VD_CONTAINER_OF(n, Vd__TestListStruct, node);

        VD_TEST_EQ("Removing first node while list is not empty happens in correct order", s->value, i);
        i++;
    }

    VD_TEST_OK();
}

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

VD_TEST("DynArray/Basic") {
    VD_DYNARRAY int *array = 0; 
    VD_DYNARRAY_INIT(array, Test_Arena);

    for (int i = 0; i < 30; ++i) {
        VD_DYNARRAY_ADD(array, i);
        VD_TEST_EQ("Dynamic array insertion is correct", array[i], i);
    }
    VD_TEST_EQ("Dynamic array has correct length", VD_DYNARRAY_LEN(array), 30);

    VD_DYNARRAY_CLEAR(array);
    VD_TEST_EQ("Dynamic array has correct length when cleared", VD_DYNARRAY_LEN(array), 0);

    for (int i = 0; i < 30; ++i) {
        VD_DYNARRAY_ADD(array, i);
        VD_TEST_EQ("Dynamic array insertion after clear is ok", array[i], i);
    }
    VD_TEST_EQ("Dynamic array has correct length after clear and repopulate", VD_DYNARRAY_LEN(array), 30);

    for (int i = 29; i >= 0; --i) {
        int c = VD_DYNARRAY_POP(array);
        VD_TEST_EQ("Array pop i is correct", c, i);
    }

    VD_TEST_EQ("Dynamic array has correct length after n pops", VD_DYNARRAY_LEN(array), 0);

    VD_TEST_OK();
}

/**
 * @note: Uncomment this line to print the map for debugging purposes
 * 
 *  static void print_map(int *map) {
 *      VD_TEST_LOG("%s","// ----------------------");
 *      for (int i = 0; i < (int)VD_STRMAP_TOTAL_CAP(map); ++i) {
 *          if (!VD_STRMAP_GET_BIN_USED(map, i)) {
 *              size_t index = 0;
 *              void *n = VD_STRMAP_GET_BIN_NEXT(map, i);
 *              if (n != 0) {
 *                  index = VD_STRMAP_GET_BIN_INDEX(map, n);
 *                  printf("// %d: [      ] -> %zu = _", i, index);
 *              } else {
 *                  printf("// %d: [      ] -> _ = _", i);
 *              }
 *          } else {
 *              size_t index = 0;
 *              void *n = VD_STRMAP_GET_BIN_NEXT(map, i);
 *              if (n != 0) {
 *                  index = VD_STRMAP_GET_BIN_INDEX(map, n);
 *                  int *v = (int*) VD_STRMAP_GET_BIN_VPTR(map, i);
 *                  printf("// %d: [%6.*s] -> %zu = %d", i, VD_STRMAP_GET_KEY_LEN(map, i), VD_STRMAP_GET_KEY_PREFIX(map, i), index, *v);
 *              } else {
 *                  int *v = (int*) VD_STRMAP_GET_BIN_VPTR(map, i);
 *                  printf("// %d: [%6.*s] -> _ = %d", i, VD_STRMAP_GET_KEY_LEN(map, i), VD_STRMAP_GET_KEY_PREFIX(map, i), *v);
 *              }
 *          }
 *      } 
 *      VD_TEST_LOG("%s","// ----------------------");
 *  }
 */

typedef struct {
    VdStr key;
    int     val;
} Vd__TestCheckMapEntry;

#define VD__TEST_MAP_CHECK_ENTRIES_(map, entryarray) do { if (!Vd__strmap_check_entries(map, (entryarray), VD_ARRAY_COUNT(entryarray), __FILE__, __LINE__)) { VD_TEST_ERR("Map value check failed."); } }while(0)
#define VD__TEST_MAP_CHECK_ENTRIES(map, ...) VD__TEST_MAP_CHECK_ENTRIES_(map, (((Vd__TestCheckMapEntry []){ __VA_ARGS__ })))

static Vdb32 Vd__strmap_check_entries(int *map, Vd__TestCheckMapEntry *entries, int num_entries, const char *f, int l)
{
    for (int i = 0; i < num_entries; ++i) {
        int v;
        if (!VD_STRMAP_GET(map, entries[i].key, &v)) {
            VD_TEST_LOG("Vd__map_check_entries: %s:%d Key not found: %.*s\n", f, l, VD_STR_EXPAND(entries[i].key));
            return VD_FALSE;
        }

        if (v != entries[i].val) {
            VD_TEST_LOG("Vd__map_check_entries: %s:%d Key '%.*s' value expected: %d, got: %d found: \n", f, l, VD_STR_EXPAND(entries[i].key), entries[i].val, v);
            return VD_FALSE;    
        }
    }

    return VD_TRUE;
}

/**
 * @todo(mdodis): Separate test case with large strings
 */
VD_TEST("Strmap/Basic") {

    // @note(mdodis): Uncomment this line to use asan properly
    // Test_Arena->flags |= VD_(ARENA_FLAGS_USE_MALLOC);

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
    VD_TEST_TRUE("Setting a value in the map works", VD_STRMAP_SET(map, VD_LIT("my_123"), &my_123));

    VD__TEST_MAP_CHECK_ENTRIES(map, 




        { VD_LIT("my_123"), 123 },



    );

    VD_TEST_TRUE("Getting a value from the map works", VD_STRMAP_GET(map, VD_LIT("my_123"), &my_123));
    VD_TEST_EQ("The fetched value from the map should be 123", my_123, 123);

    my_123 = 100;
    VD_TEST_FALSE("Writing to the same value should print an error", VD_STRMAP_SET(map, VD_LIT("my_123"), &my_123));

    VD__TEST_MAP_CHECK_ENTRIES(map, 




        { VD_LIT("my_123"), 123 },


        
    );

    VD_TEST_TRUE("Getting a value from the map works after attempting to write", VD_STRMAP_GET(map, VD_LIT("my_123"), &my_123));
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
    VD_TEST_TRUE("Overwriting a value with same key should work", VD_STRMAP_OVERWRITE(map, VD_LIT("my_123"), &my_123));
    VD_TEST_TRUE("Getting a value from the map works after overwrite", VD_STRMAP_GET(map, VD_LIT("my_123"), &my_123));
    VD_TEST_EQ("The fetched value from the map should be 321 after overwrite", my_123, 321);

    VD__TEST_MAP_CHECK_ENTRIES(map, 




        { VD_LIT_INLINE("my_123"), 321 },


        
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
    VD_TEST_TRUE("Overwriting a value with a new key should work", VD_STRMAP_OVERWRITE(map, VD_LIT("other"), &other));
    VD_TEST_TRUE("Getting a value from the map works after overwrite to a new key", VD_STRMAP_GET(map, VD_LIT("other"), &other));
    VD_TEST_EQ("The fetched value from the map should be -40 after overwrite", other, -40);

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { VD_LIT("other"),  -40 },



        { VD_LIT("my_123"), 321 },



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
    VD_TEST_TRUE("Writing 3rd value to a map with size 2/8 should work", VD_STRMAP_SET(map, VD_LIT("3"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { VD_LIT("other"),  -40 },


        {     VD_LIT("3"),    3 },
        { VD_LIT("my_123"), 321 },



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
    VD_TEST_TRUE("Writing 4th value to a map with size 3/8 should work", VD_STRMAP_SET(map, VD_LIT("4"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { VD_LIT("other"),  -40 },


        {     VD_LIT("3"),    3 },
        { VD_LIT("my_123"), 321 },


        {     VD_LIT("4"),    4 },
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
    VD_TEST_TRUE("Writing 5th value to a map with size 4/8 should work", VD_STRMAP_SET(map, VD_LIT("5"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { VD_LIT("other"),  -40 },

        {     VD_LIT("5"),    5 },
        {     VD_LIT("3"),    3 },
        { VD_LIT("my_123"), 321 },


        {     VD_LIT("4"),    4 },
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
    VD_TEST_TRUE("Writing 6th value to a map with size 5/8 should work", VD_STRMAP_SET(map, VD_LIT("6"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { VD_LIT("other"),  -40 },

        {     VD_LIT("5"),    5 },
        {     VD_LIT("3"),    3 },
        { VD_LIT("my_123"), 321 },

        {     VD_LIT("6"),    6 },
        {     VD_LIT("4"),    4 },
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
    VD_TEST_TRUE("Writing 7th value to a map with size 6/8 should work", VD_STRMAP_SET(map, VD_LIT("7"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { VD_LIT("other"),  -40 },

        {     VD_LIT("5"),    5 },
        {     VD_LIT("3"),    3 },
        { VD_LIT("my_123"), 321 },
        {     VD_LIT("7"),    7 },
        {     VD_LIT("6"),    6 },
        {     VD_LIT("4"),    4 },
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
    VD_TEST_TRUE("Writing 8th value to a map with size 7/8 should work", VD_STRMAP_SET(map, VD_LIT("8"), &num));
    num++;

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { VD_LIT("other"),  -40 },
        {     VD_LIT("8"),    8 },
        {     VD_LIT("5"),    5 },
        {     VD_LIT("3"),    3 },
        { VD_LIT("my_123"), 321 },
        {     VD_LIT("7"),    7 },
        {     VD_LIT("6"),    6 },
        {     VD_LIT("4"),    4 },
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
    VD_TEST_FALSE("Writing 9th value to a map with size 8/8 should fail", VD_STRMAP_SET(map, VD_LIT("9"), &num));

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { VD_LIT("other"),  -40 },
        {     VD_LIT("8"),    8 },
        {     VD_LIT("5"),    5 },
        {     VD_LIT("3"),    3 },
        { VD_LIT("my_123"), 321 },
        {     VD_LIT("7"),    7 },
        {     VD_LIT("6"),    6 },
        {     VD_LIT("4"),    4 },
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
    VD_TEST_TRUE("Remove my_123 should work", VD_STRMAP_RM(map, VD_LIT("my_123")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { VD_LIT("other"),  -40 },
        {     VD_LIT("8"),    8 },
        {     VD_LIT("5"),    5 },
        {     VD_LIT("3"),    3 },

        {     VD_LIT("7"),    7 },
        {     VD_LIT("6"),    6 },
        {     VD_LIT("4"),    4 },
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
    VD_TEST_FALSE("Remove non existing key should fail", VD_STRMAP_RM(map, VD_LIT("non")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 
        { VD_LIT("other"),  -40 },
        {     VD_LIT("8"),    8 },
        {     VD_LIT("5"),    5 },
        {     VD_LIT("3"),    3 },

        {     VD_LIT("7"),    7 },
        {     VD_LIT("6"),    6 },
        {     VD_LIT("4"),    4 },
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
    VD_TEST_TRUE("Remove other should work", VD_STRMAP_RM(map, VD_LIT("other")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     VD_LIT("8"),    8 },
        {     VD_LIT("5"),    5 },
        {     VD_LIT("3"),    3 },

        {     VD_LIT("7"),    7 },
        {     VD_LIT("6"),    6 },
        {     VD_LIT("4"),    4 },
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
    VD_TEST_TRUE("Remove 3 should work", VD_STRMAP_RM(map, VD_LIT("3")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     VD_LIT("8"),    8 },
        {     VD_LIT("5"),    5 },


        {     VD_LIT("7"),    7 },
        {     VD_LIT("6"),    6 },
        {     VD_LIT("4"),    4 },
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
    VD_TEST_TRUE("Remove 4 should work", VD_STRMAP_RM(map, VD_LIT("4")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     VD_LIT("8"),    8 },
        {     VD_LIT("5"),    5 },


        {     VD_LIT("7"),    7 },
        {     VD_LIT("6"),    6 },

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
    VD_TEST_TRUE("Remove 5 should work", VD_STRMAP_RM(map, VD_LIT("5")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     VD_LIT("8"),    8 },



        {     VD_LIT("7"),    7 },
        {     VD_LIT("6"),    6 },

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
    VD_TEST_TRUE("Remove 6 should work", VD_STRMAP_RM(map, VD_LIT("6")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     VD_LIT("8"),    8 },



        {     VD_LIT("7"),    7 },


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
    VD_TEST_TRUE("Remove 7 should work", VD_STRMAP_RM(map, VD_LIT("7")));

    VD__TEST_MAP_CHECK_ENTRIES(map, 

        {     VD_LIT("8"),    8 },






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
    VD_TEST_TRUE("Remove 8 should work", VD_STRMAP_RM(map, VD_LIT("8")));

    VD_TEST_OK();
}

#undef VD__TEST_MAP_CHECK_ENTRIES

#pragma pack(push, 1)
typedef struct {
    int a;
    int b;
} Vd__TestKVMapKey;

typedef struct {
    Vd__TestKVMapKey k;
    int               v;
} Vd__TestKVMapKV;
#pragma pack(pop)


static void print_kvmap(Vd__TestKVMapKV *map) {
    VD_TEST_LOG("%s", "// ----------------------");
    for (int i = 0; i < (int)VD_KVMAP_TOTAL_CAP(map); ++i) {
        if (!VD_KVMAP_GET_BIN_USED(map, i)) {
            size_t index = 0;
            void *n = VD_KVMAP_GET_BIN_NEXT(map, i);
            if (n != 0) {
                index = VD_KVMAP_GET_BIN_INDEX(map, n);
                VD_TEST_LOG("// %d: [      ] -> %zu = _", i, index);
            } else {
                VD_TEST_LOG("// %d: [      ] -> _ = _", i);
            }
        } else {
            size_t index = 0;
            void *n = VD_KVMAP_GET_BIN_NEXT(map, i);
            if (n != 0) {
                index = VD_KVMAP_GET_BIN_INDEX(map, n);
                Vd__TestKVMapKey *k = VD_KVMAP_GET_BIN_KPTR(map, i);
                int *v = (int*) VD_KVMAP_GET_BIN_VPTR(map, i);
                VD_TEST_LOG("// %d: [%-2d  %2d] -> %zu = %d", i, k->a, k->b, index, *v);
            } else {
                Vd__TestKVMapKey *k = VD_KVMAP_GET_BIN_KPTR(map, i);
                int *v = (int*) VD_KVMAP_GET_BIN_VPTR(map, i);
                VD_TEST_LOG("// %d: [%-2d  %2d] -> _ = %d", i, k->a, k->b, *v);
            }
        }
    } 
    VD_TEST_LOG("%s", "// ----------------------");
}

VD_TEST("KVMap/Basic") {
    // @todo(mdodis): Finish test just like Strmap/Basic

    VD_KVMAP Vd__TestKVMapKV *map = 0;
    VD_KVMAP_INIT(map, Test_Arena, 8, 0);

    VD_TEST_EQ("Map total capacity should be 8", VD_KVMAP_TOTAL_CAP(map), 8);

    puts("----");
    {
        Vd__TestKVMapKey key = { .a = 1, .b = 1, };
        int value = key.a + key.b;
        VD_TEST_TRUE("Set 1, 1 : 1 + 1 should work on non full map", VD_KVMAP_SET(map, &key, &value));
        print_kvmap(map);
    }

    {
        Vd__TestKVMapKey key = { .a = 2, .b = 1, };
        int value = key.a + key.b;
        VD_TEST_TRUE("Set 2, 1 : 2 + 1 should work on non full map", VD_KVMAP_SET(map, &key, &value));
        print_kvmap(map);
    }

    {
        Vd__TestKVMapKey key = { .a = 2, .b = 2, };
        int value = key.a + key.b;
        VD_TEST_TRUE("Set 2, 2 : 2 + 2 should work on non full map", VD_KVMAP_SET(map, &key, &value));
        print_kvmap(map);
    }

    {
        Vd__TestKVMapKey key = { .a = 3, .b = 2, };
        int value = key.a + key.b;
        VD_TEST_TRUE("Set 3, 2 : 3 + 2 should work on non full map", VD_KVMAP_SET(map, &key, &value));
        print_kvmap(map);
    }

    {
        Vd__TestKVMapKey key = { .a = 3, .b = 3, };
        int value = key.a + key.b;
        VD_TEST_TRUE("Set 3, 3 : 3 + 3 should work on non full map", VD_KVMAP_SET(map, &key, &value));
        print_kvmap(map);
    }

    {
        Vd__TestKVMapKey key = { .a = 4, .b = 3, };
        int value = key.a + key.b;
        VD_TEST_TRUE("Set 4, 3 : 4 + 3 should work on non full map", VD_KVMAP_SET(map, &key, &value));
        print_kvmap(map);
    }

    {
        Vd__TestKVMapKey key = { .a = 3, .b = 4, };
        int value = key.a + key.b;
        VD_TEST_TRUE("Set 3, 4 : 3 + 4 should work on non full map", VD_KVMAP_SET(map, &key, &value));
        print_kvmap(map);
    }

    {
        Vd__TestKVMapKey key = { .a = 4, .b = 4, };
        int value = key.a + key.b;
        VD_TEST_TRUE("Set 4, 4 : 4 + 4 should work on non full map", VD_KVMAP_SET(map, &key, &value));
        print_kvmap(map);
    }

    VD_TEST_OK();
}

#undef VD__TEST_MAP_CHECK_ENTRIES_
#undef VD__TEST_MAP_CHECK_ENTRIES

#endif // VD_INCLUDE_INTERNAL_TESTS
#endif // VD_INCLUDE_TESTS
#endif // VD_IMPL