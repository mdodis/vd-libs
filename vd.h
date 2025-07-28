#ifndef VD_H
#define VD_H
#define VD_VERSION_MAJOR    0
#define VD_VERSION_MINOR    0
#define VD_VERSION_PATCH    2
#define VD_VERSION          ((VD_VERSION_MAJOR << 16) | (VD_VERSION_MINOR << 8) | (VD_VERSION_PATCH))

#include <stdint.h>
#include <stddef.h>

/* ----MACRO HELPERS------------------------------------------------------------------------------------------------- */
#define VD_STRING_JOIN2(x, y) x##y

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
#ifndef VD_DEBUG_BREAK
#include <assert.h>
#define VD_DEBUG_BREAK() assert(0)
#endif // VD_DEBUG_BREAK

#ifndef VD_ASSERT
#include <assert.h>
#define VD_ASSERT(x) assert(x)
#endif // VD_ASSERT

#ifndef VD_MEMSET
#include <string.h>
#define VD_MEMSET(p, v, s) memset(p, v, s)
#endif // VD_MEMSET

#ifndef VD_MEMCPY
#include <string.h>
#define VD_MEMCPY(d, s, c) memcpy(d, s, c)
#endif // VD_MEMCPY

#ifndef VD_MEMMOVE
#include <string.h>
#define VD_MEMMOVE(d, s, c) memmove(d, s, c)
#endif // VD_MEMMOVE

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

typedef struct __VD_Arena {
    VD(u8)      *buf;
    VD(usize)    buf_len;
    VD(usize)    prev_offset;
    VD(usize)    curr_offset;
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

VD_INLINE VD(ArenaSave)     VDF(arena_save)(VD(Arena) *a)                                                       { return (VD(ArenaSave)) { .arena = a, .prev_offset = a->prev_offset, .curr_offset = a->curr_offset, }; }
VD_INLINE void              VDF(arena_restore)(VD(ArenaSave) save)                                              { save.arena->prev_offset = save.prev_offset; save.arena->curr_offset = save.curr_offset; }
VD_INLINE void*             VDF(arena_alloc)(VD(Arena) *a, size_t size)                                         { return VDF(arena_alloc_align)(a, size, VD_ARENA_DEFAULT_ALIGNMENT);}
VD_INLINE void*             VDF(arena_resize)(VD(Arena) *a, void *old_memory, size_t old_size, size_t new_size) { return VDF(arena_resize_align)(a, old_memory, old_size, new_size, VD_ARENA_DEFAULT_ALIGNMENT); }

#define VD_ARENA_PUSH_ARRAY(a, x, count) (x*)arena_alloc(a, sizeof(x) * count)

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

/* ----BUFFER-------------------------------------------------------------------------------------------------------- */
typedef struct {
    VD(u32) len;
    VD(u32) cap;
} VD(FixedArrayHeader);

#define VD_FIXEDARRAY_HEADER(a)                         ((VD(FixedArrayHeader)*)(((VD(u8)*)a) - sizeof(VD(FixedArrayHeader))))
#define VD_FIXEDARRAY_INIT(a, count, allocator)         ((a) = VDI(buffer_allocate)((allocator), (count), sizeof(*(a)), 0))
#define VD_FIXEDARRAY_INIT_RESERVE(a, len, allocator)   ((a) = VDI(buffer_allocate)((allocator), (len),   sizeof(*(a)), 1))
#define VD_FIXEDARRAY_CHECK(a, n)                       ((VD_FIXEDARRAY_HEADER(a)->len + (n)) <= VD_FIXEDARRAY_HEADER(a)->cap)
#define VD_FIXEDARRAY_ADDN(a, n)                        (VD_ASSERT(VD_FIXEDARRAY_CHECK(a, n)), VD_ARRAY_HEADER(a)->len += n)
#define VD_FIXEDARRAY_ADD(a, v)                         (VD_ASSERT(VD_FIXEDARRAY_CHECK(a, 1)), (a)[VD_FIXEDARRAY_HEADER(a)->len++] = (v))
#define VD_FIXEDARRAY_LEN(a)                            (VD_FIXEDARRAY_HEADER(a)->len)
#define VD_FIXEDARRAY_CLEAR(a)                          (VD_FIXEDARRAY_HEADER(a)->len = 0)
#define VD_FIXEDARRAY_POP(a)                            ((a)[VD_FIXEDARRAY_HEADER(a)->len--])
#define VD_FIXEDARRAY_FREE(a, allocator)                (release(allocator, VD_FIXEDARRAY_HEADER(a), VD_FIXEDARRAY_HEADER(a)->cap * sizeof(*(a)) + sizeof(VD(FixedArrayHeader))))
#define VD_FIXEDARRAY

VD_INLINE void* VDI(buffer_allocate)(VD(Arena) *arena, VD(u32) capacity, VD(usize) isize, VD(b32) mark)
{
    VD(usize) alloc_size = sizeof(VD(FixedArrayHeader)) + capacity * isize;
    void *result = VD_MEMSET(arena_alloc(arena, alloc_size), 0, alloc_size);
    VD(FixedArrayHeader) *hdr = result;

    hdr->cap = capacity;
    if (mark) {
        hdr->len = capacity;
    }

    return (void*) ((VD(u8)*)result + sizeof(VD(FixedArrayHeader)));
}


#if VD_MACRO_ABBREVIATIONS
#define fixedarray_init(a, count, allocator)         VD_FIXEDARRAY_INIT(a, count, allocator)
#define fixedarray_init_reserve(a, len, allocator)   VD_FIXEDARRAY_INIT_RESERVE(a, len, allocator)
#define fixedarray_addn(a, n)                        VD_FIXEDARRAY_ADDN(a, n)
#define fixedarray_add(a, v)                         VD_FIXEDARRAY_ADD(a, v)
#define fixedarray_len(a)                            VD_FIXEDARRAY_LEN(a)
#define fixedarray
#endif

/* ----STR----------------------------------------------------------------------------------------------------------- */
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

#define VD_LIT(string)  (VD(Str)) { .s = (string), .len = sizeof(string), }

#if VD_MACRO_ABBREVIATIONS
#define LIT(string)     VD_LIT(string)
#endif

VD_INLINE VD(Str) VDF(str_from_cstr)(VD(cstr) s) { return (VD(Str)) { .s = s, .len = VDF(cstr_len)(s),}; }

/* ----STR BUILDER--------------------------------------------------------------------------------------------------- */

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
void                        VDF(scratch_init)(VD(Scratch) *scratch);
VD(Arena)*                  VDF(scratch_get_arena)(VD(Scratch) *scratch);
void                        VDF(scratch_return_arena)(VD(Scratch) *scratch, VD(Arena) *arena);

/* ----LOG----------------------------------------------------------------------------------------------------------- */
#define VD_PROC_LOG(name) void name(int verbosity, cstr string)
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

/* ----VIRTUAL MEMORY IMPL------------------------------------------------------------------------------------------- */
#if !VD_VM_CUSTOM

#if VD_PLATFORM_MACOS
#include <sys/mman.h>
#include <errno.h>

VD(usize) VDF(vm_get_page_size)()
{
    return PAGE_SIZE;
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

/* ----SCRATCH IMPL-------------------------------------------------------------------------------------------------- */
void VDF(scratch_init)(VD(Scratch) *scratch)
{
    for (usize i = 0; i < VD_SCRATCH_PAGE_COUNT; ++i)
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

#endif // VD_IMPL