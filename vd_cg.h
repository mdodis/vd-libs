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

#define VD_CG_PI32           3.14159265359f
#define VD_CG_RAD2DEG_COEFF  ((VD(f32))180.0f/VD_CG_PI32)
#define VD_CG_DEG2RAD_COEFF  ((VD(f32))VD_CG_PI32/180.0f)
#define VD_CG_FTAU           360.0f
#define VD_CG_DTAU           360.0

#if VD_MACRO_ABBREVIATIONS
#define fdeg2rad(degrees) (VD_CG_DEG2RAD_COEFF * (degrees))
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
#define VD_CG_FFLOOR    floorf
#define VD_CG_DFLOOR    floor
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

#pragma pack(push, 1)
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

typedef union __VD_CG_f2x2 {
    VD(f32) e[2][2];
    VD(f2)  c[2];
} VD(f2x2);

typedef union __VD_CG_f3x3 {
    VD(f32) e[3][3];
    VD(f3)  c[3];
} VD(f3x3);

typedef union __VD_CG_f4x4 {
    VD(f32) e[4][4];
    VD(f4)  c[4];
    struct { VD(f32) a0, a1, a2, a3, b0, b1, b2, b3, c0, c1, c2, c3, d0, d1, d2, d3; };
} VD(f4x4);

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

typedef union __VD_CG_d2x2 {
    VD(f64) e[2][2];
    VD(d2)  c[2];
} VD(d2x2);

typedef union __VD_CG_d3x3 {
    VD(f64) e[3][3];
    VD(d3)  c[3];
} VD(d3x3);

typedef union __VD_CG_d4x4 {
    VD(f64) e[4][4];
    VD(d4)  c[4];
} VD(d4x4);

typedef union __VD_CG_d4 VD(dquat);
#pragma pack(pop)

/* ----DEFAULT PRECISION--------------------------------------------------------------------------------------------- */
#ifndef VD_CG_DEFAULT_PRECISION_CUSTOM
#define VD_CG_DEFAULT_PRECISION_SINGLE 1
#define VD_CG_DEFAULT_PRECISION_DOUBLE 0
#endif // !VD_CG_DEFAULT_PRECISION_CUSTOM

/* ----INTEGERS------------------------------------------------------------------------------------------------------ */
typedef union __VD_CG_i2 {
    VD(i32)  e[2];
    struct { VD(i32) x, y; };
} VD(s2);

/* ----REAL & RX TYPEDEFS-------------------------------------------------------------------------------------------- */
#if VD_CG_DEFAULT_PRECISION_SINGLE
typedef VD(f32)     VD(rea);
typedef VD(f2)      VD(r2);
typedef VD(f3)      VD(r3);
typedef VD(f4)      VD(r4);
typedef VD(f2x2)    VD(r2x2);
typedef VD(f3x3)    VD(r3x3);
typedef VD(f4x4)    VD(r4x4);
typedef VD(fquat)   VD(rquat);
#define VD_CG_RSIN  VD_CG_FSIN
#define VD_CG_RCOS  VD_CG_FCOS
#define VD_CG_RTAN  VD_CG_FTAN
#define VD_CG_RSQRT VD_CG_FSQRT
#define VD_CG_RACOS VD_CG_FACOS
#elif VD_CG_DEFAULT_PRECISION_DOUBLE
typedef VD(f64)     VD(rea);
typedef VD(d2)      VD(r2);
typedef VD(d3)      VD(r3);
typedef VD(d4)      VD(r4);
typedef VD(d2x2)    VD(r2x2);
typedef VD(d3x3)    VD(r3x3);
typedef VD(d4x4)    VD(r4x4);
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

/* ----INITIALIZATION------------------------------------------------------------------------------------------------ */
static VD_INLINE VD(f2)    VDF(fm2)              (VD(f32) x, VD(f32) y)                           { return (VD(f2)) {.e = {x, y}};       }
static VD_INLINE VD(d2)    VDF(dm2)              (VD(f64) x, VD(f64) y)                           { return (VD(d2)) {.e = {x, y}};       }
static VD_INLINE VD(r2)    VDF(rm2)              (VD(rea) x, VD(rea) y)                           { return (VD(r2)) {.e = {x, y}};       }
static VD_INLINE VD(f2)    VDF(fall2)            (VD(f32) s)                                      { return VDF(fm2)(s, s);               }
static VD_INLINE VD(d2)    VDF(dall2)            (VD(f64) s)                                      { return VDF(dm2)(s, s);               }
static VD_INLINE VD(r2)    VDF(rall2)            (VD(rea) s)                                      { return VDF(rm2)(s, s);               }
static VD_INLINE VD(f2)    VDF(fzero2)           (void)                                           { return VDF(fall2)(0.f);              }
static VD_INLINE VD(d2)    VDF(dzero2)           (void)                                           { return VDF(dall2)(0.f);              }
static VD_INLINE VD(r2)    VDF(rzero2)           (void)                                           { return VDF(rall2)(0.f);              }

static VD_INLINE VD(f3)    VDF(fm3)              (VD(f32) x, VD(f32) y, VD(f32) z)                { return (VD(f3)) {.e = {x, y, z}};    }
static VD_INLINE VD(d3)    VDF(dm3)              (VD(f64) x, VD(f64) y, VD(f64) z)                { return (VD(d3)) {.e = {x, y, z}};    }
static VD_INLINE VD(r3)    VDF(rm3)              (VD(rea) x, VD(rea) y, VD(rea) z)                { return (VD(r3)) {.e = {x, y, z}};    }
static VD_INLINE VD(f3)    VDF(fall3)            (VD(f32) s)                                      { return VDF(fm3)(s, s, s);            }
static VD_INLINE VD(d3)    VDF(dall3)            (VD(f64) s)                                      { return VDF(dm3)(s, s, s);            }
static VD_INLINE VD(r3)    VDF(rall3)            (VD(rea) s)                                      { return VDF(rm3)(s, s, s);            }
static VD_INLINE VD(f3)    VDF(fzero3)           (void)                                           { return VDF(fall3)(0.f);              }
static VD_INLINE VD(d3)    VDF(dzero3)           (void)                                           { return VDF(dall3)(0.f);              }
static VD_INLINE VD(r3)    VDF(rzero3)           (void)                                           { return VDF(rall3)(0.f);              }

static VD_INLINE VD(f4)    VDF(fm4)              (VD(f32) x, VD(f32) y, VD(f32) z, VD(f32) w)     { return (VD(f4)) {.e = {x, y, z, w}}; }
static VD_INLINE VD(d4)    VDF(dm4)              (VD(f64) x, VD(f64) y, VD(f64) z, VD(f64) w)     { return (VD(d4)) {.e = {x, y, z, w}}; }
static VD_INLINE VD(r4)    VDF(rm4)              (VD(rea) x, VD(rea) y, VD(rea) z, VD(rea) w)     { return (VD(r4)) {.e = {x, y, z, w}}; }
static VD_INLINE VD(f4)    VDF(fall4)            (VD(f32) s)                                      { return VDF(fm4)(s, s, s, s);         }
static VD_INLINE VD(d4)    VDF(dall4)            (VD(f64) s)                                      { return VDF(dm4)(s, s, s, s);         }
static VD_INLINE VD(r4)    VDF(rall4)            (VD(rea) s)                                      { return VDF(rm4)(s, s, s, s);         }
static VD_INLINE VD(f4)    VDF(fzero4)           (void)                                           { return VDF(fall4)(0.f);              }
static VD_INLINE VD(d4)    VDF(dzero4)           (void)                                           { return VDF(dall4)(0.f);              }
static VD_INLINE VD(r4)    VDF(rzero4)           (void)                                           { return VDF(rall4)(0.f);              }

static VD_INLINE VD(f4x4)  VDF(fm4x4)            (VD(f32) a0, VD(f32) a1, VD(f32) a2, VD(f32) a3,
                                                  VD(f32) b0, VD(f32) b1, VD(f32) b2, VD(f32) b3,
                                                  VD(f32) c0, VD(f32) c1, VD(f32) c2, VD(f32) c3,
                                                  VD(f32) d0, VD(f32) d1, VD(f32) d2, VD(f32) d3) { return (VD(f4x4))  {.e = { {a0, a1, a2, a3}, {b0, b1, b2, b3}, {c0, c1, c2, c3}, {d0, d1, d2, d3} } }; }
static VD_INLINE VD(d4x4)  VDF(dm4x4)            (VD(f64) a0, VD(f64) a1, VD(f64) a2, VD(f64) a3,
                                                  VD(f64) b0, VD(f64) b1, VD(f64) b2, VD(f64) b3,
                                                  VD(f64) c0, VD(f64) c1, VD(f64) c2, VD(f64) c3,
                                                  VD(f64) d0, VD(f64) d1, VD(f64) d2, VD(f64) d3) { return (VD(d4x4))  {.e = { {a0, a1, a2, a3}, {b0, b1, b2, b3}, {c0, c1, c2, c3}, {d0, d1, d2, d3} } }; }
static VD_INLINE VD(r4x4)  VDF(rm4x4)            (VD(rea) a0, VD(rea) a1, VD(rea) a2, VD(rea) a3,
                                                  VD(rea) b0, VD(rea) b1, VD(rea) b2, VD(rea) b3,
                                                  VD(rea) c0, VD(rea) c1, VD(rea) c2, VD(rea) c3,
                                                  VD(rea) d0, VD(rea) d1, VD(rea) d2, VD(rea) d3) { return (VD(r4x4))  {.e = { {a0, a1, a2, a3}, {b0, b1, b2, b3}, {c0, c1, c2, c3}, {d0, d1, d2, d3} } }; }

static VD_INLINE VD(f4x4)  VDF(fall4x4)          (VD(f32) v)                                      { return VDF(fm4x4)  (v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v);                                                  }
static VD_INLINE VD(d4x4)  VDF(dall4x4)          (VD(f64) v)                                      { return VDF(dm4x4)  (v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v);                                                  }
static VD_INLINE VD(r4x4)  VDF(rall4x4)          (VD(rea) v)                                      { return VDF(rm4x4)  (v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v);                                                  }

static VD_INLINE VD(fquat) VDF(fmquat)           (VD(f3) ipart, VD(f32) rpart)                    { return (VD(fquat)) {.e = {ipart.x, ipart.y, ipart.z, rpart}};                                          }
static VD_INLINE VD(dquat) VDF(dmquat)           (VD(d3) ipart, VD(f64) rpart)                    { return (VD(dquat)) {.e = {ipart.x, ipart.y, ipart.z, rpart}};                                          }
static VD_INLINE VD(rquat) VDF(rmquat)           (VD(r3) ipart, VD(rea) rpart)                    { return (VD(rquat)) {.e = {ipart.x, ipart.y, ipart.z, rpart}};                                          }

/* ----VECTOR ALGEBRA------------------------------------------------------------------------------------------------ */
static VD_INLINE VD(f2)    VDF(fadd2)            (VD(f2) a, VD(f2) b)                             { return VDF(fm2)(a.x + b.x, a.y + b.y); }
static VD_INLINE VD(d2)    VDF(dadd2)            (VD(d2) a, VD(d2) b)                             { return VDF(dm2)(a.x + b.x, a.y + b.y); }
static VD_INLINE VD(r2)    VDF(radd2)            (VD(r2) a, VD(r2) b)                             { return VDF(rm2)(a.x + b.x, a.y + b.y); }

static VD_INLINE VD(f3)    VDF(fadd3)            (VD(f3) a, VD(f3) b)                             { return VDF(fm3)(a.x + b.x, a.y + b.y, a.z + b.z); }
static VD_INLINE VD(d3)    VDF(dadd3)            (VD(d3) a, VD(d3) b)                             { return VDF(dm3)(a.x + b.x, a.y + b.y, a.z + b.z); }
static VD_INLINE VD(r3)    VDF(radd3)            (VD(r3) a, VD(r3) b)                             { return VDF(rm3)(a.x + b.x, a.y + b.y, a.z + b.z); }

static VD_INLINE VD(f4)    VDF(fadd4)            (VD(f4) a, VD(f4) b)                             { return VDF(fm4)(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
static VD_INLINE VD(d4)    VDF(dadd4)            (VD(d4) a, VD(d4) b)                             { return VDF(dm4)(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
static VD_INLINE VD(r4)    VDF(radd4)            (VD(r4) a, VD(r4) b)                             { return VDF(rm4)(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

static VD_INLINE VD(f2)    VDF(fsub2)            (VD(f2) a, VD(f2) b)                             { return VDF(fm2)(a.x - b.x, a.y - b.y); }
static VD_INLINE VD(d2)    VDF(dsub2)            (VD(d2) a, VD(d2) b)                             { return VDF(dm2)(a.x - b.x, a.y - b.y); }
static VD_INLINE VD(r2)    VDF(rsub2)            (VD(r2) a, VD(r2) b)                             { return VDF(rm2)(a.x - b.x, a.y - b.y); }

static VD_INLINE VD(f3)    VDF(fsub3)            (VD(f3) a, VD(f3) b)                             { return VDF(fm3)(a.x - b.x, a.y - b.y, a.z - b.z); }
static VD_INLINE VD(d3)    VDF(dsub3)            (VD(d3) a, VD(d3) b)                             { return VDF(dm3)(a.x - b.x, a.y - b.y, a.z - b.z); }
static VD_INLINE VD(r3)    VDF(rsub3)            (VD(r3) a, VD(r3) b)                             { return VDF(rm3)(a.x - b.x, a.y - b.y, a.z - b.z); }

static VD_INLINE VD(f4)    VDF(fsub4)            (VD(f4) a, VD(f4) b)                             { return VDF(fm4)(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
static VD_INLINE VD(d4)    VDF(dsub4)            (VD(d4) a, VD(d4) b)                             { return VDF(dm4)(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
static VD_INLINE VD(r4)    VDF(rsub4)            (VD(r4) a, VD(r4) b)                             { return VDF(rm4)(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

static VD_INLINE VD(f3)    VDF(fscale3)          (VD(f3) v, VD(f32) s)                            { return VDF(fm3)(v.x * s, v.y * s, v.z * s); }
static VD_INLINE VD(d3)    VDF(dscale3)          (VD(d3) v, VD(f64) s)                            { return VDF(dm3)(v.x * s, v.y * s, v.z * s); }
static VD_INLINE VD(r3)    VDF(rscale3)          (VD(r3) v, VD(rea) s)                            { return VDF(rm3)(v.x * s, v.y * s, v.z * s); }

static VD_INLINE VD(f2)    VDF(fdiv2)            (VD(f2) v, VD(f32) s)                            { return VDF(fm2)(v.x / s, v.y / s); }
static VD_INLINE VD(d2)    VDF(ddiv2)            (VD(d2) v, VD(f64) s)                            { return VDF(dm2)(v.x / s, v.y / s); }
static VD_INLINE VD(r2)    VDF(rdiv2)            (VD(r2) v, VD(rea) s)                            { return VDF(rm2)(v.x / s, v.y / s); }

static VD_INLINE VD(f3)    VDF(fdiv3)            (VD(f3) v, VD(f32) s)                            { return VDF(fm3)(v.x / s, v.y / s, v.z / s); }
static VD_INLINE VD(d3)    VDF(ddiv3)            (VD(d3) v, VD(f64) s)                            { return VDF(dm3)(v.x / s, v.y / s, v.z / s); }
static VD_INLINE VD(r3)    VDF(rdiv3)            (VD(r3) v, VD(rea) s)                            { return VDF(rm3)(v.x / s, v.y / s, v.z / s); }

static VD_INLINE VD(f4)    VDF(fdiv4)            (VD(f4) v, VD(f32) s)                            { return VDF(fm4)(v.x / s, v.y / s, v.z / s, v.w / s); }
static VD_INLINE VD(d4)    VDF(ddiv4)            (VD(d4) v, VD(f64) s)                            { return VDF(dm4)(v.x / s, v.y / s, v.z / s, v.w / s); }
static VD_INLINE VD(r4)    VDF(rdiv4)            (VD(r4) v, VD(rea) s)                            { return VDF(rm4)(v.x / s, v.y / s, v.z / s, v.w / s); }

static VD_INLINE VD(f32)   VDF(fdot2)            (VD(f2) a, VD(f2) b)                             { return a.x * a.x + a.y * a.y; }
static VD_INLINE VD(f64)   VDF(ddot2)            (VD(d2) a, VD(d2) b)                             { return a.x * a.x + a.y * a.y; }
static VD_INLINE VD(rea)   VDF(rdot2)            (VD(r2) a, VD(r2) b)                             { return a.x * a.x + a.y * a.y; }

static VD_INLINE VD(f32)   VDF(fdot3)            (VD(f3) a, VD(f3) b)                             { return a.x * b.x + a.y * b.y + a.z * b.z; }
static VD_INLINE VD(f64)   VDF(ddot3)            (VD(d3) a, VD(d3) b)                             { return a.x * b.x + a.y * b.y + a.z * b.z; }
static VD_INLINE VD(rea)   VDF(rdot3)            (VD(r3) a, VD(r3) b)                             { return a.x * b.x + a.y * b.y + a.z * b.z; }

static VD_INLINE VD(f32)   VDF(fdot4)            (VD(f4) a, VD(f4) b)                             { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
static VD_INLINE VD(f64)   VDF(ddot4)            (VD(d4) a, VD(d4) b)                             { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
static VD_INLINE VD(rea)   VDF(rdot4)            (VD(r4) a, VD(r4) b)                             { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

static VD_INLINE VD(f32)   VDF(flensq2)          (VD(f2) v)                                       { return VDF(fdot2)(v, v); }
static VD_INLINE VD(f64)   VDF(dlensq2)          (VD(d2) v)                                       { return VDF(ddot2)(v, v); }
static VD_INLINE VD(rea)   VDF(rlensq2)          (VD(r2) v)                                       { return VDF(rdot2)(v, v); }

static VD_INLINE VD(f32)   VDF(flensq3)          (VD(f3) v)                                       { return VDF(fdot3)(v, v); }
static VD_INLINE VD(f64)   VDF(dlensq3)          (VD(d3) v)                                       { return VDF(ddot3)(v, v); }
static VD_INLINE VD(rea)   VDF(rlensq3)          (VD(r3) v)                                       { return VDF(rdot3)(v, v); }

static VD_INLINE VD(f32)   VDF(flensq4)          (VD(f4) v)                                       { return VDF(fdot4)(v, v); }
static VD_INLINE VD(f64)   VDF(dlensq4)          (VD(d4) v)                                       { return VDF(ddot4)(v, v); }
static VD_INLINE VD(rea)   VDF(rlensq4)          (VD(r4) v)                                       { return VDF(rdot4)(v, v); }

static VD_INLINE VD(f2)    VDF(fnormalize2)      (VD(f2) v)                                       { VD(f32) l = VDF(flensq2)(v); return VDF(fdiv2)(v, VD_CG_FSQRT(l)); }
static VD_INLINE VD(d2)    VDF(dnormalize2)      (VD(d2) v)                                       { VD(f64) l = VDF(dlensq2)(v); return VDF(ddiv2)(v, VD_CG_DSQRT(l)); }
static VD_INLINE VD(r2)    VDF(rnormalize2)      (VD(r2) v)                                       { VD(rea) l = VDF(rlensq2)(v); return VDF(rdiv2)(v, VD_CG_RSQRT(l)); }

static VD_INLINE VD(f3)    VDF(fnormalize3)      (VD(f3) v)                                       { VD(f32) l = VDF(flensq3)(v); return VDF(fdiv3)(v, VD_CG_FSQRT(l)); }
static VD_INLINE VD(d3)    VDF(dnormalize3)      (VD(d3) v)                                       { VD(f64) l = VDF(dlensq3)(v); return VDF(ddiv3)(v, VD_CG_DSQRT(l)); }
static VD_INLINE VD(r3)    VDF(rnormalize3)      (VD(r3) v)                                       { VD(rea) l = VDF(rlensq3)(v); return VDF(rdiv3)(v, VD_CG_RSQRT(l)); }

static VD_INLINE VD(f4)    VDF(fnormalize4)      (VD(f4) v)                                       { VD(f32) l = VDF(flensq4)(v); return VDF(fdiv4)(v, VD_CG_FSQRT(l)); }
static VD_INLINE VD(d4)    VDF(dnormalize4)      (VD(d4) v)                                       { VD(f64) l = VDF(dlensq4)(v); return VDF(ddiv4)(v, VD_CG_DSQRT(l)); }
static VD_INLINE VD(r4)    VDF(rnormalize4)      (VD(r4) v)                                       { VD(rea) l = VDF(rlensq4)(v); return VDF(rdiv4)(v, VD_CG_RSQRT(l)); }

static VD_INLINE VD(f2)    VDF(fnoz2)      (VD(f2) v)                                             { VD(f32) l = VDF(flensq2)(v); if (l == 0.f) return VDF(fzero2)(); else return VDF(fdiv2)(v, VD_CG_FSQRT(l)); }
static VD_INLINE VD(d2)    VDF(dnoz2)      (VD(d2) v)                                             { VD(f64) l = VDF(dlensq2)(v); if (l == 0.f) return VDF(dzero2)(); else return VDF(ddiv2)(v, VD_CG_DSQRT(l)); }
static VD_INLINE VD(r2)    VDF(rnoz2)      (VD(r2) v)                                             { VD(rea) l = VDF(rlensq2)(v); if (l == 0.f) return VDF(rzero2)(); else return VDF(rdiv2)(v, VD_CG_RSQRT(l)); }
static VD_INLINE VD(f3)    VDF(fnoz3)      (VD(f3) v)                                             { VD(f32) l = VDF(flensq3)(v); if (l == 0.f) return VDF(fzero3)(); else return VDF(fdiv3)(v, VD_CG_FSQRT(l)); }
static VD_INLINE VD(d3)    VDF(dnoz3)      (VD(d3) v)                                             { VD(f64) l = VDF(dlensq3)(v); if (l == 0.f) return VDF(dzero3)(); else return VDF(ddiv3)(v, VD_CG_DSQRT(l)); }
static VD_INLINE VD(r3)    VDF(rnoz3)      (VD(r3) v)                                             { VD(rea) l = VDF(rlensq3)(v); if (l == 0.f) return VDF(rzero3)(); else return VDF(rdiv3)(v, VD_CG_RSQRT(l)); }
static VD_INLINE VD(f4)    VDF(fnoz4)      (VD(f4) v)                                             { VD(f32) l = VDF(flensq4)(v); if (l == 0.f) return VDF(fzero4)(); else return VDF(fdiv4)(v, VD_CG_FSQRT(l)); }
static VD_INLINE VD(d4)    VDF(dnoz4)      (VD(d4) v)                                             { VD(f64) l = VDF(dlensq4)(v); if (l == 0.f) return VDF(dzero4)(); else return VDF(ddiv4)(v, VD_CG_DSQRT(l)); }
static VD_INLINE VD(r4)    VDF(rnoz4)      (VD(r4) v)                                             { VD(rea) l = VDF(rlensq4)(v); if (l == 0.f) return VDF(rzero4)(); else return VDF(rdiv4)(v, VD_CG_RSQRT(l)); }

static VD_INLINE VD(f3)    VDF(fcross3)          (VD(f3) a, VD(f3) b)                             { return VDF(fm3)(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
static VD_INLINE VD(d3)    VDF(dcross3)          (VD(d3) a, VD(d3) b)                             { return VDF(dm3)(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
static VD_INLINE VD(r3)    VDF(rcross3)          (VD(r3) a, VD(r3) b)                             { return VDF(rm3)(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

/* ----MATRIX ALGEBRA------------------------------------------------------------------------------------------------ */
static VD_INLINE VD(f4x4)  VDF(ftranspose4x4)     (VD(f4x4) *m);
static VD_INLINE VD(f4x4)  VDF(fmul4x4)           (VD(f4x4) *a, VD(f4x4) *b);

/* ----QUATERNION ALGEBRA-------------------------------------------------------------------------------------------- */
static VD_INLINE VD(fquat) VDF(fidentityquat)    (void)                                           { return VDF(fmquat)(VDF(fzero3)(), 1.f);                               }
static VD_INLINE VD(fquat) VDF(faxis_anglequat)  (VD(f3) axis, VD(f32) angle)                     { return VDF(fmquat)(VDF(fscale3)(axis, VD_CG_FSIN(angle * .5f)), 1.f); }
static VD_INLINE VD(fquat) VDF(fmulquat)         (VD(fquat) q1, VD(fquat) q2);
static VD_INLINE VD(f3)    VDF(fmulquat3)        (VD(fquat) q, VD(f3) v);
static VD_INLINE VD(f4x4)  VDF(fto4x4quat)       (VD(fquat) q);
static VD_INLINE VD(fquat) VDF(feulerquat)       (VD(f3) euler);

/* ----COORDINATE SYSTEMS-------------------------------------------------------------------------------------------- */
static VD_INLINE VD(f4x4)  VDF(fidentity4x4)      (void);
static VD_INLINE VD(f4x4)  VDF(fdx_to_vk4x4)      (void);
static VD_INLINE VD(f4x4)  VDF(fperspective4x4)   (VD(f32) fovyrad, VD(f32) aspect, VD(f32) near, VD(f32) far);
static VD_INLINE VD(f4x4)  VDF(fperspective4x4_vk)(VD(f32) fovyrad, VD(f32) aspect, VD(f32) near, VD(f32) far);
static VD_INLINE VD(f4x4)  VDF(flookat4x4)        (VD(f3) fwd, VD(f3) up, VD(f3) right);
static VD_INLINE VD(f4x4)  VDF(ftranslation4x4)   (VD(f3) v);
static VD_INLINE VD(f4x4)  VDF(frotation_yaw4x4)  (VD(f32) rad);
static VD_INLINE void      VDF(ftranslate4x4)     (VD(f4x4) *m, VD(f3) v);
static VD_INLINE void      VDF(frotate_yaw4x4)    (VD(f4x4) *m, VD(f32) rad);
static VD_INLINE VD(f4x4)  VDF(fvk_to_dx4x4)      (void);

/* ----LOGGING------------------------------------------------------------------------------------------------------- */
static VD_INLINE void     VDF(fprint4x4)          (VD(f4x4) *m);

/* ----UTILITY------------------------------------------------------------------------------------------------------- */
static VD_INLINE VD(f32)  VDF(fwrap_degrees)      (VD(f32) d)                                     { return d - VD_CG_FTAU * VD_CG_FFLOOR(d / VD_CG_FTAU);       }
static VD_INLINE VD(f32)  VDF(fclamp)             (VD(f32) min, VD(f32) x, VD(f32) max)           { if (x < min) return min; if (x > max) return max; return x; }

/* ----VECTOR ALGEBRA IMPL------------------------------------------------------------------------------------------- */

/* ----MATRIX ALGEBRA IMPL------------------------------------------------------------------------------------------- */
static VD_INLINE VD(f4x4) VDF(ftranspose4x4)(VD(f4x4) *m)
{
    return VDF(fm4x4)(
        m->a0, m->b0, m->c0, m->d0,
        m->a1, m->b1, m->c1, m->d1,
        m->a2, m->b2, m->c2, m->d2,
        m->a3, m->b3, m->c3, m->d3);
}

static VD_INLINE VD(f4x4) VDF(fmul4x4)(VD(f4x4) *a, VD(f4x4) *b)
{
    VD(f4x4) result = VDF(fall4x4)(0.f);
    for (int r = 0; r < 4; ++r) {
        for (int c = 0; c < 4; ++c) {
            for (int k = 0; k < 4; ++k) {
                result.e[r][c] += a->e[r][k] * b->e[k][c];
            }
        }
    }
    return result;
}

/* ----QUATERNION ALGEBRA IMPL--------------------------------------------------------------------------------------- */
static VD_INLINE VD(fquat) VDF(fmulquat)(VD(fquat) q1, VD(fquat) q2)
{
    VD(fquat) result;
    result.x =  q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
    result.y = -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
    result.z =  q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
    result.w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;
    return result;
}

static VD_INLINE VD(f3) VDF(fmulquat3)(VD(fquat) q, VD(f3) v)
{
    VD(f3)  u = q.xyz;
    VD(f32) s = q.w;

    VD(f3) result = VDF(fscale3)(u, 2.f * VDF(fdot3)(u, v));
    result        = VDF(fadd3)(result, VDF(fscale3)(v, s * s - VDF(fdot3)(u, u)));
    result        = VDF(fadd3)(result, VDF(fscale3)(VDF(fcross3)(u, v), 2.f * s));
    return result;
}

static VD_INLINE VD(f4x4) VDF(fto4x4quat)(VD(fquat) q)
{
    VD(f32) qxx = (q.x * q.x);
    VD(f32) qyy = (q.y * q.y);
    VD(f32) qzz = (q.z * q.z);
    VD(f32) qxz = (q.x * q.z);
    VD(f32) qxy = (q.x * q.y);
    VD(f32) qyz = (q.y * q.z);
    VD(f32) qwx = (q.w * q.x);
    VD(f32) qwy = (q.w * q.y);
    VD(f32) qwz = (q.w * q.z);
    return VDF(fm4x4)(
        1.f - 2.f * (qyy + qzz), 2.f * (qxy + qwz),       2.f * (qxz - qwy),       0.f,
        2.f * (qxy - qwz),       1.f - 2.f * (qxx + qzz), 2.f * (qyz + qwx),       0.f,
        2.f * (qxz + qwy),       2.f * (qyz - qwx),       1.f - 2.f * (qxx + qyy), 0.f,
        0.f,                     0.f,                     0.f,                     1.f);
}

static VD_INLINE VD(fquat) VDF(feulerquat)(VD(f3) euler)
{
    VD(f32) cy = VD_CG_FCOS(euler.y * 0.5f); // yaw
    VD(f32) sy = VD_CG_FSIN(euler.y * 0.5f);
    VD(f32) cp = VD_CG_FCOS(euler.x * 0.5f); // pitch
    VD(f32) sp = VD_CG_FSIN(euler.x * 0.5f);
    VD(f32) cr = VD_CG_FCOS(euler.z * 0.5f); // roll
    VD(f32) sr = VD_CG_FSIN(euler.z * 0.5f);

    VD(fquat) q;
    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * sp * cr + sy * cp * sr;
    q.y = sy * cp * cr - cy * sp * sr;
    q.z = cy * cp * sr - sy * sp * cr;
    return q;
}

/* ----COORDINATE SYSTEMS IMPL--------------------------------------------------------------------------------------- */
static VD_INLINE VD(f4x4) VDF(fidentity4x4)(void)
{
    return VDF(fm4x4)(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}

static VD_INLINE VD(f4x4) VDF(fdx_to_vk4x4)(void)
{
    return VDF(fm4x4)(
       -1.f, 0.f, 0.f, 0.f,
        0.f,-1.f, 0.f, 0.f,
        0.f, 0.f,-1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}

static VD_INLINE VD(f4x4) VDF(fperspective4x4)(VD(f32) fovyrad, VD(f32) aspect, VD(f32) near, VD(f32) far)
{
    VD(f32) half_tan_fovy = VD_CG_FTAN(fovyrad * 0.5f);

    return VDF(fm4x4)(
        1.f / ((half_tan_fovy) * aspect), 0.f,                  0.f,                0.f,
        0.f,                              1.f / half_tan_fovy,  0.f,                0.f,
        0.f,                              0.f,                  far / (far - near), -(near * far) / (far - near),
        0.f,                              0.f,                  1.f,                0.f);
}

static VD_INLINE VD(f4x4) VDF(fperspective4x4_vk)(VD(f32) fovyrad, VD(f32) aspect, VD(f32) near, VD(f32) far)
{
    VD(f32) half_tan_fovy = VD_CG_FTAN(fovyrad * 0.5f);

    return VDF(fm4x4)(
        1.f / ((half_tan_fovy) * aspect), 0.f,                  0.f,                0.f,
        0.f,                              1.f / half_tan_fovy,  0.f,                0.f,
        0.f,                              0.f,                  far / (far - near), -(near * far) / (far - near),
        0.f,                              0.f,                  1.f,                0.f);
}

static VD_INLINE VD(f4x4) VDF(flookat4x4)(VD(f3) from, VD(f3 to), VD(f3) up)
{
    VD(f3) fwd = VDF(fsub3)(from, to);
    fwd = VDF(fnoz3)(fwd);

    VD(f3) right = VDF(fcross3)(up, fwd);
    right = VDF(fnoz3)(right);

    VD(f3) up2 = VDF(fcross3)(fwd, right);

    // return VDF(fm4x4)(
    //     right.x, right.y, right.z, 0.f,
    //     up2.x,   up2.y,   up2.z,   0.f,
    //     fwd.x,   fwd.y,   fwd.z,   0.f,
    //     -from.x, -from.y, -from.z,  1.f);

    VD(f3) xaxis = right;
    VD(f3) yaxis = up2;
    VD(f3) zaxis = fwd;

    VD(f3) meye = fscale3(from, -1.f);

    return VDF(fm4x4)(
        xaxis.x, xaxis.y, xaxis.z, fdot3(xaxis, meye),
        yaxis.x, yaxis.y, yaxis.z, fdot3(yaxis, meye),
        zaxis.x, zaxis.y, zaxis.z, fdot3(zaxis, meye),
        0.f, 0.f, 0.f, 1.f);
    // return VDF(fm4x4)(
    //     xaxis.x, yaxis.x, zaxis.x, 0.f,
    //     xaxis.y, yaxis.y, zaxis.y, 0.f,
    //     xaxis.z, yaxis.z, zaxis.z, 0.f,
    //     fdot3(xaxis, meye), fdot3(yaxis, meye), fdot3(zaxis, meye),   1.f);
}

static VD_INLINE VD(f4x4) VDF(ftranslation4x4)(VD(f3) v)
{
    // return VDF(fm4x4)(
    //     1.f, 0.f, 0.f, 0.f,
    //     0.f, 1.f, 0.f, 0.f,
    //     0.f, 0.f, 1.f, 0.f,
    //     v.x, v.y, v.z, 1.f);
    return VDF(fm4x4)(
        1.f, 0.f, 0.f, v.x,
        0.f, 1.f, 0.f, v.y,
        0.f, 0.f, 1.f, v.z,
        0.f, 0.f, 0.f, 1.f);
}

static VD_INLINE VD(f4x4) VDF(frotation_yaw4x4)(VD(f32) rad)
{
    return VDF(fm4x4)(
        VD_CG_FCOS(rad), 0.f, -VD_CG_FSIN(rad), 0.f,
        0.f,             1.f, 0.f,              0.f,
        VD_CG_FSIN(rad), 0.f, VD_CG_FCOS(rad),  0.f,
        0.f,             0.f, 0.f,              1.f);
}

static VD_INLINE void VDF(ftranslate4x4)(VD(f4x4) *m, VD(f3) v)
{
    VD(f4x4) t = VDF(ftranslation4x4)(v);
    VD(f4x4) r = fmul4x4(m, &t);
    *m = r;
}

static VD_INLINE void VDF(frotate_yaw4x4)(VD(f4x4) *m, VD(f32) rad)
{
    VD(f4x4) t = VDF(frotation_yaw4x4)(rad);
    VD(f4x4) r = fmul4x4(m, &t);
    *m = r;
}

static VD_INLINE VD(f4x4) VDF(fvk_to_dx4x4)(void)
{
    return VDF(fm4x4)(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f,-1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}

/* ----LOGGING IMPL-------------------------------------------------------------------------------------------------- */
static VD_INLINE void VDF(fprint4x4)(VD(f4x4) *m)
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

VD_TEST("Test f4x4")

#endif // VD_CG_INCLUDE_INTERNAL_TESTS

#endif // VD_CG_IMPLEMENTATION