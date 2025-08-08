/**
 * vd_cg.h - A maths header file to use with C99
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
#ifndef VD_CG_H
#define VD_CG_H
#define VD_CG_VERSION_MAJOR 0
#define VD_CG_VERSION_MINOR 0
#define VD_CG_VERSION_PATCH 1
#define VD_CG_VERSION       ((VD_CG_VERSION_MAJOR << 16) | (VD_CG_VERSION_MINOR << 8) | (VD_CG_VERSION_PATCH))

#ifndef VD_H
#error "vd_cg.h requires vd.h"
#endif // !VD_H

#define VD_CG_PI32              3.14159265359f
#define VD_CG_RAD2DEG_COEFF  ((VD(f32))180.0f/VD_CG_PI32)
#define VD_CG_DEG2RAD_COEFF  ((VD(f32))VD_CG_PI32/180.0f)

#if VD_MACRO_ABBREVIATIONS
#define deg2rad(degrees) (VD_CG_DEG2RAD_COEFF * (degrees))
#endif // VD_MACRO_ABBREVIATIONS

#ifndef VD_CG_MPROCS_CUSTOM
#define VD_CG_MPROCS_CUSTOM 0
#endif // !VD_CG_MPROCS_CUSTOM

#if !VD_CG_MPROCS_CUSTOM
#include <math.h>
#define VD_CG_FSIN      sinf
#define VD_CG_DSIN      sin
#define VD_CG_FCOS      cosf
#define VD_CG_DCOS      cos
#define VD_CG_FTAN      tanf
#define VD_CG_DTAN      tan
#define VD_CG_FSQRT     sqrtf
#define VD_CG_DSQRT     sqrt
#define VD_CG_FACOS     acosf
#define VD_CG_DACOS     acos
#define VD_CG_MIN(a, b) ((a) < (b) ? (b) : (a))
#define VD_CG_MAX(a, b) ((a) > (b) ? (b) : (a))
#define VD_CG_ABS(a)    ((a) > 0 ? (a) : -(a))
#endif // !VD_CG_MPROCS_CUSTOM

#if VD_MACRO_ABBREVIATIONS
#define fsin       VD_CG_FSIN
#define fcos       VD_CG_FCOS
#define ftan       VD_CG_FTAN
#define fsqrt      VD_CG_FSQRT
#define facos      VD_CG_FACOS
#define dsin       VD_CG_DSIN
#define dcos       VD_CG_DCOS
#define dtan       VD_CG_DTAN
#define dsqrt      VD_CG_DSQRT
#define dacos      VD_CG_DACOS
#define vmin(a, b) VD_CG_MIN(a,b)
#define vmax(a, b) VD_CG_MAX(a,b)
#endif

/* ----SINGLE PRECISION---------------------------------------------------------------------------------------------- */
typedef union __VD_CG_f2 {
    VD(f32)  e[2];
    struct { VD(f32) x, y; };
    struct { VD(f32) r, g; };
} VD(f2);

typedef union __VD_CG_f3 {
    VD(f32)  e[3];
    struct   { VD(f32) x, y, z; };
    struct   { VD(f2) xy; VD(f32) __ignore0; };
    struct   { VD(f32) __ignore2; VD(f2) yz; };
} VD(f3);

typedef union __VD_CG_f4 {
    VD(f32)  e[4];
    struct   { VD(f32) x, y, z, w; };
    struct   { VD(f2) xy; VD(f2) zw; };
    struct   { VD(f32) __ignore3; VD(f2) yz; VD(f32) __ignore2; };
    struct   { VD(f3) xyz; VD(f32) __ignore0; };
    struct   { VD(f32) __ignore1; VD(f3) yzw; };
} VD(f4);

typedef union __VD_CG_fm2x2 {
    VD(f32) e[2][2];
    VD(f2)  c[2];
} VD(fm2x2);

typedef union __VD_CG_fm3x3 {
    VD(f32) e[3][3];
    VD(f3)  c[3];
} VD(fm3x3);

typedef union __VD_CG_fm4x4 {
    VD(f32) e[4][4];
    VD(f4)  c[4];
    struct { VD(f32) a0, a1, a2, a3, b0, b1, b2, b3, c0, c1, c2, c3, d0, d1, d2, d3; };
} VD(fm4x4);

typedef union __VD_CG_f4 VD(fquat);

/* ----DOUBLE PRECISION---------------------------------------------------------------------------------------------- */
typedef union __VD_CG_d2 {
    VD(f64)  e[2];
    struct { VD(f64) x, y; };
    struct { VD(f64) r, g; };
} VD(d2);

typedef union __VD_CG_d3 {
    VD(f64)  e[3];
    struct   { VD(f64) x, y, z; };
    struct   { VD(d2) xy; VD(f64) __ignore0; };
    struct   { VD(f64) __ignore2; VD(d2) yz; };
} VD(d3);

typedef union __VD_CG_d4 {
    VD(f64)  e[4];
    struct   { VD(f64) x, y, z, w; };
    struct   { VD(d2) xy; VD(d2) zw; };
    struct   { VD(f64) __ignore3; VD(d2) yz; VD(f64) __ignore2; };
    struct   { VD(d3) xyz; VD(f64) __ignore0; };
    struct   { VD(f64) __ignore1; VD(d3) yzw; };
} VD(d4);

typedef union __VD_CG_dm2x2 {
    VD(f64) e[2][2];
    VD(d2)  c[2];
} VD(dm2x2);

typedef union __VD_CG_dm3x3 {
    VD(f64) e[3][3];
    VD(d3)  c[3];
} VD(dm3x3);

typedef union __VD_CG_dm4x4 {
    VD(f64) e[4][4];
    VD(d4)  c[4];
} VD(dm4x4);

typedef union __VD_CG_d4 VD(dquat);

/* ----DEFAULT PRECISION--------------------------------------------------------------------------------------------- */
#ifndef VD_CG_DEFAULT_PRECISION_CUSTOM
#define VD_CG_DEFAULT_PRECISION_SINGLE 1
#define VD_CG_DEFAULT_PRECISION_DOUBLE 0
#endif // !VD_CG_DEFAULT_PRECISION_CUSTOM

/* ----REAL & RX TYPEDEFS-------------------------------------------------------------------------------------------- */
#if VD_CG_DEFAULT_PRECISION_SINGLE
typedef VD(f32)     VD(real);
typedef VD(f2)      VD(r2);
typedef VD(f3)      VD(r3);
typedef VD(f4)      VD(r4);
typedef VD(fm2x2)   VD(rm2x2);
typedef VD(fm3x3)   VD(rm3x3);
typedef VD(fm4x4)   VD(rm4x4);
typedef VD(fquat)   VD(rquat);
#define VD_CG_RSIN  VD_CG_FSIN
#define VD_CG_RCOS  VD_CG_FCOS
#define VD_CG_RTAN  VD_CG_FTAN
#define VD_CG_RSQRT VD_CG_FSQRT
#define VD_CG_RACOS VD_CG_FACOS
#elif VD_CG_DEFAULT_PRECISION_DOUBLE
typedef VD(d32)     VD(real);
typedef VD(d2)      VD(r2);
typedef VD(d3)      VD(r3);
typedef VD(d4)      VD(r4);
typedef VD(dm2x2)   VD(rm2x2);
typedef VD(dm3x3)   VD(rm3x3);
typedef VD(dm4x4)   VD(rm4x4);
typedef VD(dquat)   VD(rquat);
#define VD_CG_RSIN  VD_CG_DSIN
#define VD_CG_RCOS  VD_CG_DCOS
#define VD_CG_RTAN  VD_CG_DTAN
#define VD_CG_RSQRT VD_CG_DSQRT
#define VD_CG_RACOS VD_CG_DACOS
#endif // VD_CG_DEFAULT_PRECISION_SINGLE, VD_CG_DEFAULT_PRECISION_DOUBLE

#if VD_MACRO_ABBREVIATIONS
#define rsin       VD_CG_RSIN
#define rcos       VD_CG_RCOS
#define rtan       VD_CG_RTAN
#define rsqrt      VD_CG_RSQRT
#define racos      VD_CG_RACOS
#endif // VD_MACRO_ABBREVIATIONS

static VD_INLINE VD(f2)    VDF(F2)        (VD(f32) x, VD(f32) y)                           { return (VD(f2)) {.e = {x, y}};       }
static VD_INLINE VD(f3)    VDF(F3)        (VD(f32) x, VD(f32) y, VD(f32) z)                { return (VD(f3)) {.e = {x, y, z}};    }
static VD_INLINE VD(f4)    VDF(F4)        (VD(f32) x, VD(f32) y, VD(f32) z, VD(f32) w)     { return (VD(f4)) {.e = {x, y, z, w}}; }
static VD_INLINE VD(fm4x4) VDF(FM4x4)     (VD(f32) a0, VD(f32) a1, VD(f32) a2, VD(f32) a3,
                                           VD(f32) b0, VD(f32) b1, VD(f32) b2, VD(f32) b3,
                                           VD(f32) c0, VD(f32) c1, VD(f32) c2, VD(f32) c3,
                                           VD(f32) d0, VD(f32) d1, VD(f32) d2, VD(f32) d3) { return (VD(fm4x4)) {.e = { {a0, a1, a2, a3}, {b0, b1, b2, b3}, {c0, c1, c2, c3}, {d0, d1, d2, d3} } }; }
static VD_INLINE VD(fm4x4) VDF(FM4x4_all) (VD(f32) v)                                      { return VDF(FM4x4)(v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v); }

/* ----VECTOR ALGEBRA------------------------------------------------------------------------------------------------ */

/* ----MATRIX ALGEBRA------------------------------------------------------------------------------------------------ */
static VD_INLINE VD(fm4x4) VDF(ftranspose4x4)(VD(fm4x4) *m);
static VD_INLINE VD(fm4x4) VDF(fmul4x4)      (VD(fm4x4) *a, VD(fm4x4) *b);

/* ----COORDINATE SYSTEMS-------------------------------------------------------------------------------------------- */
static VD_INLINE VD(fm4x4) VDF(fidentity4x4)      (void);
static VD_INLINE VD(fm4x4) VDF(fperspective4x4)   (VD(f32) fovyrad, VD(f32) aspect, VD(f32) near, VD(f32) far);
static VD_INLINE VD(fm4x4) VDF(fperspective4x4_vk)(VD(f32) fovyrad, VD(f32) aspect, VD(f32) near, VD(f32) far);
static VD_INLINE VD(fm4x4) VDF(ftranslation4x4)   (VD(f3) v);
static VD_INLINE VD(fm4x4) VDF(frotation_yaw4x4)  (VD(f32) rad);
static VD_INLINE void      VDF(ftranslate4x4)     (VD(fm4x4) *m, VD(f3) v);
static VD_INLINE void      VDF(frotate_yaw4x4)    (VD(fm4x4) *m, VD(f32) rad);

/* ----LOGGING------------------------------------------------------------------------------------------------------- */

static VD_INLINE void      VDF(fprint4x4)(VD(fm4x4) *m);

/* ----VECTOR ALGEBRA IMPL------------------------------------------------------------------------------------------- */

/* ----MATRIX ALGEBRA IMPL------------------------------------------------------------------------------------------- */
static VD_INLINE VD(fm4x4) VDF(ftranspose4x4)(VD(fm4x4) *m)
{
    return VDF(FM4x4)(
        m->a0, m->b0, m->c0, m->d0,
        m->a1, m->b1, m->c1, m->d1,
        m->a2, m->b2, m->c2, m->d2,
        m->a3, m->b3, m->c3, m->d3);
}

static VD_INLINE VD(fm4x4) VDF(fmul4x4)(VD(fm4x4) *a, VD(fm4x4) *b)
{
    VD(fm4x4) result = VDF(FM4x4_all)(0.f);
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            for (int k = 0; k < 4; ++k) {
                result.e[r][c] += a->e[r][k] * b->e[k][c];
            }
        }
    }
    // for (int c = 0; c < 4; ++c) {       // For each column of the result
    //     for (int r = 0; r < 4; ++r) {   // For each row of the result
    //         result.e[c][r] =
    //             a->e[0][r] * b->e[c][0] +
    //             a->e[1][r] * b->e[c][1] +
    //             a->e[2][r] * b->e[c][2] +
    //             a->e[3][r] * b->e[c][3];
    //     }
    // }
    return result;
}

/* ----COORDINATE SYSTEMS IMPL--------------------------------------------------------------------------------------- */
static VD_INLINE VD(fm4x4) VDF(fidentity4x4)(void)
{
    return VDF(FM4x4)(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}

static VD_INLINE VD(fm4x4) VDF(fperspective4x4)(VD(f32) fovyrad, VD(f32) aspect, VD(f32) near, VD(f32) far)
{
    VD(f32) half_tan_fovy = VD_CG_FTAN(fovyrad * 0.5f);

    return VDF(FM4x4)(
        1.f / ((half_tan_fovy) * aspect), 0.f,                  0.f,                           0.f,
        0.f,                              1.f / half_tan_fovy,  0.f,                           0.f,
        0.f,                              0.f,                  far / (far - near),            1.f,
        0.f,                              0.f,                  - (near * far) / (far - near), 0.f);
}

static VD_INLINE VD(fm4x4) VDF(fperspective4x4_vk)(VD(f32) fovyrad, VD(f32) aspect, VD(f32) near, VD(f32) far)
{
    VD(f32) half_tan_fovy = VD_CG_FTAN(fovyrad * 0.5f);

    return VDF(FM4x4)(
        1.f / ((half_tan_fovy) * aspect), 0.f,                  0.f,                           0.f,
        0.f,                             -1.f / half_tan_fovy,  0.f,                           0.f,
        0.f,                              0.f,                  far / (far - near),            1.f,
        0.f,                              0.f,                  - (near * far) / (far - near), 0.f);
}

static VD_INLINE VD(fm4x4) VDF(ftranslation4x4)(VD(f3) v)
{
    return VDF(FM4x4)(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        v.x, v.y, v.z, 1.f);
}

static VD_INLINE VD(fm4x4) VDF(frotation_yaw4x4)(VD(f32) rad)
{
    return VDF(FM4x4)(
        VD_CG_FCOS(rad), 0.f, -VD_CG_FSIN(rad), 0.f,
        0.f,             1.f, 0.f,              0.f,
        VD_CG_FSIN(rad), 0.f, VD_CG_FCOS(rad),  0.f,
        0.f,             0.f, 0.f,              1.f);
}

static VD_INLINE void VDF(ftranslate4x4)(VD(fm4x4) *m, VD(f3) v)
{
    VD(fm4x4) t = VDF(ftranslation4x4)(v);
    VD(fm4x4) r = fmul4x4(m, &t);
    *m = r;
}

static VD_INLINE void VDF(frotate_yaw4x4)(VD(fm4x4) *m, VD(f32) rad)
{
    VD(fm4x4) t = VDF(frotation_yaw4x4)(rad);
    VD(fm4x4) r = fmul4x4(m, &t);
    *m = r;
}

/* ----LOGGING IMPL-------------------------------------------------------------------------------------------------- */
static VD_INLINE void VDF(fprint4x4)(VD(fm4x4) *m)
{
    VD_LOGF("| %04.2f %04.2f %04.2f %04.2f | ", m->a0, m->a1, m->a2, m->a3);
    VD_LOGF("| %04.2f %04.2f %04.2f %04.2f | ", m->b0, m->b1, m->b2, m->b3);
    VD_LOGF("| %04.2f %04.2f %04.2f %04.2f | ", m->c0, m->c1, m->c2, m->c3);
    VD_LOGF("| %04.2f %04.2f %04.2f %04.2f | ", m->d0, m->d1, m->d2, m->d3);
}

#endif // !VD_CG_H

#ifdef VD_CG_IMPLEMENTATION


#ifndef VD_CG_INCLUDE_INTERNAL_TESTS
#define VD_CG_INCLUDE_INTERNAL_TESTS 0
#endif // !VD_CG_INCLUDE_INTERNAL_TESTS

#if VD_CG_INCLUDE_INTERNAL_TESTS

VD_TEST("Test fm4x4")

#endif // VD_CG_INCLUDE_INTERNAL_TESTS

#endif // VD_CG_IMPLEMENTATION