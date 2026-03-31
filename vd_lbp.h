/**
 * vd_lbp.h - Little Big Planet Serialization
 * ---------------------------------------------------------------------------------------------------------------------
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <https://unlicense.org/>
 * ---------------------------------------------------------------------------------------------------------------------
 */
#ifndef VD_LBP_H
#define VD_LBP_H
#define VD_LBP_VERSION_MAJOR    0
#define VD_LBP_VERSION_MINOR    0
#define VD_LBP_VERSION_PATCH    1
#define VD_LBP_VERSION          ((VD_LBP_VERSION_MAJOR << 16) | (VD_LBP_VERSION_MINOR << 8) | (VD_LBP_VERSION_PATCH))

#ifndef VD_LBP_INL
#   define VD_LBP_INL static inline
#endif // !VD_LBP_INL

typedef struct VdLbpSerializer VdLbpSerializer;
struct VdLbpSerializer {
    int  ver;
    int  writing;
    void *ctx;
    int (*proc)(VdLbpSerializer *ser, void *bytes, int size);
    int (*fin)(VdLbpSerializer *ser);
};

#ifndef VD_LBP_SER_PREFIX_CPP
#   define VD_LBP_SER_PREFIX_CPP serialize
#endif // !VD_LBP_SER_PREFIX_CPP

#define VD_LBP_SER_PROC_NAME_(prefix, type) prefix##type
#define VD_LBP_SER_PROC_NAME(type) VD_LBP_SER_PROC_NAME_(serialize_, type)
#define VD_LBP_SER_PROC(type) int VD_LBP_SER_PROC_NAME(type)(VdLbpSerializer *ser, type *datum)
#define VD_LBP_SER_CALL_(type) VD_LBP_SER_PROC_NAME(type)
#define VD_LBP_SER_CALL(ser, field, type) VD_LBP_SER_CALL_(type)(ser, &datum->field)
#define VD_LBP_SER_CALL_FIRST(ser, field, type) VD_LBP_SER_CALL_(type)(ser, &field)

#define VD_LBP_SER(ser, field, type) VD_LBP_SER_CALL(ser, field, type)
#define VD_LBP_ADD(ser, field, type, version_added) \
    do { \
        if (ser->ver >= version_added) { \
            VD_LBP_SER(ser, field, type); \
        } \
    } while (0)

#define VD_LBP_REM(ser, field, type, version_added, version_removed) \
    type field = {0}; \
    do { \
        if ((ser->ver >= version_added) && (ser->ver < version_removed)) { \
            VD_LBP_SERIALIZE(ser, field, type); \
        } \
    } while (0)

#ifdef __cplusplus
#   define VD_LBP_SER_FORWARD(type) \
        template<> int VD_LBP_SER_PREFIX_CPP<type>(VdLbpSerializer *ser, type *datum) \
        { return VD_LBP_SER_PROC_NAME(ser, datum); }
#else
#   define VD_LBP_SER_FORWARD(type)
#endif // __cplusplus

#define VD_LBP_SER_IMPL(type) \
    VD_LBP_INL VD_LBP_SER_PROC(type); \
    VD_LBP_SER_FORWARD(type) \
    VD_LBP_INL VD_LBP_SER_PROC(type)

#include <stdint.h>
#define VD_LBP_BASE_TYPE_IMPL(type) VD_LBP_INL VD_LBP_SER_PROC(type) { return ser->proc(ser, datum, sizeof(*datum)); }
VD_LBP_BASE_TYPE_IMPL(int8_t)
VD_LBP_BASE_TYPE_IMPL(uint8_t)
VD_LBP_BASE_TYPE_IMPL(int16_t)
VD_LBP_BASE_TYPE_IMPL(uint16_t)
VD_LBP_BASE_TYPE_IMPL(int32_t)
VD_LBP_BASE_TYPE_IMPL(uint32_t)
VD_LBP_BASE_TYPE_IMPL(int64_t)
VD_LBP_BASE_TYPE_IMPL(uint64_t)
VD_LBP_BASE_TYPE_IMPL(float)
VD_LBP_BASE_TYPE_IMPL(double)

#define VD_LBP_SERIALIZE(ser, ptr, type) VD_LBP_SER_CALL_FIRST(ser, ptr, type)

#ifndef VD_LBP_STDIO
#   define VD_LBP_STDIO 1
#endif // !VD_LBP_STDIO

#if VD_LBP_STDIO
#include <stdio.h>
VD_LBP_INL int vd_lbp_stdio_do(VdLbpSerializer *ser, void *bytes, int size)
{
    int result;
    FILE *f;

    result = 0;
    f = (FILE*)ser->ctx;
    if (f) {
        if (ser->writing) {
            result = (int)fwrite(bytes, size, 1, f);
        } else {
            result = (int)fread(bytes, size, 1, f);
        }
    }

    return result == 1;
}

int vd_lbp_stdio_fin(VdLbpSerializer *ser)
{
    fclose((FILE*)ser->ctx);
    return 1;
}

VD_LBP_INL VdLbpSerializer vd_lbp_stdio_serializer_make(const char *file, int writing, int version_curr)
{
    const char *mode = "rb";
    if (writing) {
        mode = "wb";
    }

    FILE *f = fopen(file, mode);

    int result_ver = version_curr;
    if (!writing) {
        int f_version = 0;
        if (fread(&f_version, sizeof(f_version), 1, f) == 0) {
            printf("Corrupted file.\n");
            fclose(f);
            f = 0;
        }

        if (f_version > version_curr) {
            printf("Can't read file from the future (%d >= %d).\n", f_version, version_curr);
            fclose(f);
            f = 0;
        }

        result_ver = f_version;
    } else {
        fwrite(&version_curr, sizeof(version_curr), 1, f);
    }

    VdLbpSerializer result;
    result.ver     = result_ver;
    result.writing = writing;
    result.ctx     = f;
    result.proc    = vd_lbp_stdio_do;
    result.fin     = vd_lbp_stdio_fin;
    return result;
}
#endif // VD_LBP_STDIO

#endif VD_LBP_H