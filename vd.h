#ifndef VD_H
#define VD_H
#define VD_VERSION  "0.0.1"
#define VD_DATE     "25 July 2025"

#include <stdint.h>
#include <stddef.h>

/* ----NAMESPACE----------------------------------------------------------------------------------------------------- */
#ifndef VD_NAMESPACE_OVERRIDE
#define VD(x)  Vd##x
#define VDF(x) vd_##x
#define VDI(x) vd__##x
#define VD_(x) VD_#x
#endif // VD_NAMESPACE_OVERRIDE

/* ----HOST COMPILER------------------------------------------------------------------------------------------------- */
#if defined(__GNUC__) || defined(__clang__)
#define VD_HOST_COMPILER_CLANG 1
#endif

#if defined(_MSC_VER)
#define VD_HOST_COMPILER_MSVC 1
#endif

#ifndef VD_HOST_COMPILER_CLANG
#define VD_HOST_COMPILER_CLANG 0
#endif

#ifndef VD_HOST_COMPILER_MSVC
#define VD_HOST_COMPILER_MSVC 0
#endif

#if defined(__cplusplus)
#define VD_CPP 1
#endif

#ifndef VD_CPP
#define VD_CPP 0
#endif

#if VD_CPP
#define VD_C 0
#else
#define VD_C 1
#endif

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
typedef uint16_t      VD(u16);
typedef uint32_t      VD(u32);
typedef uint64_t      VD(u64);
typedef int8_t        VD(i8);
typedef int16_t       VD(i16);
typedef int32_t       VD(i32);
typedef int64_t       VD(i64);
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
#ifndef VD_ASSERT
#include <assert.h>
#define VD_ASSERT(x) assert(x)
#endif // VD_ASSERT

#ifndef VD_MEMSET
#include <string.h>
#define VD_MEMSET(p, v, s) memset(p, v, s)
#endif // VD_MEMSET

#ifndef VD_MEMMOVE
#include <string.h>
#define VD_MEMMOVE(d, s, c) memmove(d, s, c)
#endif // VD_MEMMOVE

/* ----SIZES--------------------------------------------------------------------------------------------------------- */
#define VD_KILOBYTES(x) x * 1024
#define VD_MEGABYTES(x) VD_KILOBYTES(x) * 1024

#define VD_ARRAY_COUNT(s) (sizeof(s) / sizeof(*s))

VD_INLINE VD(b32) VDF(is_power_of_two)(VD(usize) x) {
    return (x & (x - 1)) == 0;
}

VD_INLINE uintptr_t VDF(vd_align_forward)(uintptr_t ptr, size_t align) {
    VD_ASSERT(vd_is_power_of_two(align));

    uintptr_t p, a, modulo;

    p = ptr;
    a = (uintptr_t)align;
    modulo = p & (a - 1);

    if (modulo != 0) {
        p += a - modulo;
    }

    return p;
}

/* ----ARENA--------------------------------------------------------------------------------------------------------- */
#ifndef VD_ARENA_DEFAULT_ALIGNMENT
#define VD_ARENA_DEFAULT_ALIGNMENT (2 * sizeof(void*))
#endif // VD_ARENA_DEFAULT_ALIGNMENT

#ifndef VD_ARENA_ZERO_ON_CLEAR
#define VD_ARENA_ZERO_ON_CLEAR 1
#endif // VD_ARENA_ZERO_ON_CLEAR 

typedef struct __VD_ArenaSave {
    VD(u8)      *buf;
    VD(usize)    buf_len;
    VD(usize)    prev_offset;
    VD(usize)    curr_offset;
} VD(Arena);

typedef struct __VD_Arena {
    VD(Arena)   *arena;
    VD(usize)    prev_offset;
    VD(usize)    curr_offset;
} VD(ArenaSave);

void                    VDF(arena_init)(VD(Arena) *a, void *buf, size_t len);
void*                   VDF(arena_alloc_align)(VD(Arena) *a, size_t size, size_t align);
void*                   VDF(arena_resize_align)(VD(Arena) *a, void *old_memory, size_t old_size, size_t new_size, size_t align);
void                    VDF(arena_clear)(VD(Arena) *a);

VD_INLINE VD(ArenaSave) VDF(arena_save)(VD(Arena) *a) { return (VD(ArenaSave)) { .arena = a, .prev_offset = a->prev_offset, .curr_offset = a->curr_offset, }; }
VD_INLINE void          VDF(arena_restore)(VD(ArenaSave) save) { save.arena->prev_offset = save.prev_offset; save.arena->curr_offset = save.curr_offset; }
VD_INLINE void*         VDF(arena_alloc)(VD(Arena) *a, size_t size) { return VDF(arena_alloc_align)(a, size, VD_ARENA_DEFAULT_ALIGNMENT);}
VD_INLINE void*         VDF(arena_resize)(VD(Arena) *a, void *old_memory, size_t old_size, size_t new_size) { return VDF(arena_resize_align)(a, old_memory, old_size, new_size, VD_ARENA_DEFAULT_ALIGNMENT); }

/* ----STRINGS------------------------------------------------------------------------------------------------------- */
typedef struct __VD_Str {
    char        *s;
    VD(usize)   len;
} VD(Str);

VD_INLINE VD(usize)    VDF(cstr_len)(VD(cstr) a)
{
    VD(usize) result = 0;
    while (*a++) result++;
    return result;
}

VD_INLINE VD(b32)      VDF(cstr_cmp)(VD(cstr) _a, VD(cstr) _b)
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

#define VD_STR(string) (VD(Str)) { .s = (string), .len = sizeof(string), }

VD_INLINE VD(Str) VDF(str_from_cstr)(VD(cstr) s) { return (VD(Str)) { .s = s, .len = VDF(cstr_len)(s),}; }

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


/* ----SCRATCH------------------------------------------------------------------------------------------------------- */

#ifndef VD_SCRATCH_PAGE_COUNT
#define VD_SCRATCH_PAGE_COUNT 64
#endif // VD_SCRATCH_PAGE_COUNT

#ifndef VD_SCRATCH_PAGE_SIZE
#define VD_SCRATCH_PAGE_SIZE VD_KILOBYTES(24)
#endif // VD_SCRATCH_PAGE_SIZE 

typedef struct __VD_Scratch {
    VD(Arena)   arenas[VD_SCRATCH_PAGE_COUNT];
    VD(usize)   curr_arena;
} VD(Scratch);

typedef struct __VD_ScratchSave {
    VD(Scratch)     *scratch;
    VD(usize)       curr_arena;
    VD(usize)       arena_prev_offset;
    VD(usize)       arena_curr_offset;
} VD(ScratchSave);

void                        VDF(scratch_init)(VD(Scratch) *scratch);
void*                       VDF(scratch_alloc_align)(VD(Scratch) *scratch, VD(usize) size, VD(usize) align);
void                        VDF(scratch_clear)(VD(Scratch) *scratch);

VD_INLINE VD(ScratchSave)   VDF(scratch_save)(VD(Scratch) *scratch) { return (VD(ScratchSave)) { scratch, scratch->curr_arena, scratch->arenas[scratch->curr_arena].prev_offset, scratch->arenas[scratch->curr_arena].curr_offset, }; }
VD_INLINE void              VDF(scratch_restore)(VD(ScratchSave) *save)
{
    save->scratch->curr_arena = save->curr_arena;
    save->scratch->arenas[save->scratch->curr_arena].prev_offset = save->arena_prev_offset;
    save->scratch->arenas[save->scratch->curr_arena].curr_offset = save->arena_curr_offset;
    save->scratch = NULL;
}

/* ----TESTING------------------------------------------------------------------------------------------------------- */
#ifndef VD_INCLUDE_TESTS
#define VD_INCLUDE_TESTS 0
#endif // VD_INCLUDE_TESTS

#if VD_INCLUDE_TESTS
#define VD_TEST(name) VD(b32) name(void)
typedef VD_TEST(VD(ProcTest));
#else
#define VD_TEST(name)
#endif

#endif // !VD_H

/* ------------------------------------------------------------------------------------------------------------------ */
/* ----IMPLEMENTATION------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------ */

#ifdef VD_IMPL

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

    if (offset + size <= a->buf_len) {
        void *ptr = &a->buf[offset];
        a->prev_offset = offset;
        a->curr_offset = offset + size;

        VD_MEMSET(ptr, 0, size);
        return ptr;
    }

    return NULL;
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
        return NULL;
    }
}

void VDF(arena_clear)(VD(Arena) *a)
{
    VD_MEMSET(a->buf, 0, a->curr_offset);
    a->curr_offset = 0;
    a->prev_offset = 0;
}

#define VD_ARENA_ALLOC_STRUCT_ALIGN(a, type, align)         (type*)vd_arena_alloc_align(a, sizeof(type), (align))
#define VD_ARENA_ALLOC_STRUCT(a, type) (type*)              (type*)vd_arena_alloc_align(a, sizeof(type), VD_ARENA_DEFAULT_ALIGNMENT)
#define VD_ARENA_ALLOC_N_ALIGN(a, type, count, align)       (type*)vd_arena_alloc_align(a, sizeof(type) * count, align)
#define VD_ARENA_ALLOC_N(a, type, count)                    (type*)vd_arena_alloc_align(a, sizeof(type) * count, VD_ARENA_DEFAULT_ALIGNMENT)

#endif // VD_IMPL