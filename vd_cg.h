/**
 * \internal
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
typedef float   VdCgf32;
typedef double  VdCgf64;
typedef int     VdCgi32;
#else
typedef Vdf32   VdCgf32;
typedef Vdf64   VdCgf64;
typedef Vdi32   VdCgi32;
#endif // !VD_H

#define VD_CG_PI32           3.14159265359f
#define VD_CG_RAD2DEG_COEFF  ((VdCgf32)180.0f/VD_CG_PI32)
#define VD_CG_DEG2RAD_COEFF  ((VdCgf32)VD_CG_PI32/180.0f)
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
    VdCgf32  e[2];
    struct { VdCgf32 x, y; };
    struct { VdCgf32 r, g; };
} Vdf2;

typedef union __VD_CG_f3 {
    VdCgf32  e[3];
    struct   { VdCgf32 x, y, z; };
    struct   { Vdf2 xy; VdCgf32 __ignore0; };
    struct   { VdCgf32 __ignore2; Vdf2 yz; };
} Vdf3;

typedef union __VD_CG_f4 {
    VdCgf32  e[4];
    struct   { VdCgf32 x, y, z, w; };
    struct   { Vdf2 xy; Vdf2 zw; };
    struct   { VdCgf32 __ignore3; Vdf2 yz; VdCgf32 __ignore2; };
    struct   { Vdf3 xyz; VdCgf32 __ignore0; };
    struct   { VdCgf32 __ignore1; Vdf3 yzw; };
} Vdf4;

typedef union __VD_CG_f2x2 {
    VdCgf32 e[2][2];
    Vdf2  c[2];
} Vdf2x2;

typedef union __VD_CG_f3x3 {
    VdCgf32 e[3][3];
    Vdf3  c[3];
} Vdf3x3;

typedef union __VD_CG_f4x4 {
    VdCgf32 e[4][4];
    Vdf4  c[4];
    struct { VdCgf32 a0, a1, a2, a3, b0, b1, b2, b3, c0, c1, c2, c3, d0, d1, d2, d3; };
} Vdf4x4;

typedef union __VD_CG_f4 Vdfquat;

/* ----DOUBLE PRECISION---------------------------------------------------------------------------------------------- */
typedef union __VD_CG_d2 {
    VdCgf64  e[2];
    struct { VdCgf64 x, y; };
    struct { VdCgf64 r, g; };
} Vdd2;

typedef union __VD_CG_d3 {
    VdCgf64  e[3];
    struct   { VdCgf64 x, y, z; };
    struct   { Vdd2 xy; VdCgf64 __ignore0; };
    struct   { VdCgf64 __ignore2; Vdd2 yz; };
} Vdd3;

typedef union __VD_CG_d4 {
    VdCgf64  e[4];
    struct   { VdCgf64 x, y, z, w; };
    struct   { Vdd2 xy; Vdd2 zw; };
    struct   { VdCgf64 __ignore3; Vdd2 yz; VdCgf64 __ignore2; };
    struct   { Vdd3 xyz; VdCgf64 __ignore0; };
    struct   { VdCgf64 __ignore1; Vdd3 yzw; };
} Vdd4;

typedef union __VD_CG_d2x2 {
    VdCgf64 e[2][2];
    Vdd2  c[2];
} Vdd2x2;

typedef union __VD_CG_d3x3 {
    VdCgf64 e[3][3];
    Vdd3  c[3];
} Vdd3x3;

typedef union __VD_CG_d4x4 {
    VdCgf64 e[4][4];
    Vdd4  c[4];
} Vdd4x4;

typedef union __VD_CG_d4 Vddquat;
#pragma pack(pop)

/* ----DEFAULT PRECISION--------------------------------------------------------------------------------------------- */
#ifndef VD_CG_DEFAULT_PRECISION_CUSTOM
#define VD_CG_DEFAULT_PRECISION_SINGLE 1
#define VD_CG_DEFAULT_PRECISION_DOUBLE 0
#endif // !VD_CG_DEFAULT_PRECISION_CUSTOM

/* ----INTEGERS------------------------------------------------------------------------------------------------------ */
typedef union __VD_CG_i2 {
    VdCgi32  e[2];
    struct { VdCgi32 x, y; };
} Vds2;

/* ----REAL & RX TYPEDEFS-------------------------------------------------------------------------------------------- */
#if VD_CG_DEFAULT_PRECISION_SINGLE
typedef VdCgf32   Vdrea;
typedef Vdf2      Vdr2;
typedef Vdf3      Vdr3;
typedef Vdf4      Vdr4;
typedef Vdf2x2    Vdr2x2;
typedef Vdf3x3    Vdr3x3;
typedef Vdf4x4    Vdr4x4;
typedef Vdfquat   Vdrquat;
#define VD_CG_RSIN  VD_CG_FSIN
#define VD_CG_RCOS  VD_CG_FCOS
#define VD_CG_RTAN  VD_CG_FTAN
#define VD_CG_RSQRT VD_CG_FSQRT
#define VD_CG_RACOS VD_CG_FACOS
#elif VD_CG_DEFAULT_PRECISION_DOUBLE
typedef VdCgf64   Vdrea;
typedef Vdd2      Vdr2;
typedef Vdd3      Vdr3;
typedef Vdd4      Vdr4;
typedef Vdd2x2    Vdr2x2;
typedef Vdd3x3    Vdr3x3;
typedef Vdd4x4    Vdr4x4;
typedef Vddquat   Vdrquat;
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
static VD_INLINE Vdf2    vd_fm2              (VdCgf32 x, VdCgf32 y)                           { return (Vdf2) {.e = {x, y}};       }
static VD_INLINE Vdd2    vd_dm2              (VdCgf64 x, VdCgf64 y)                           { return (Vdd2) {.e = {x, y}};       }
static VD_INLINE Vdr2    vd_rm2              (Vdrea x, Vdrea y)                               { return (Vdr2) {.e = {x, y}};       }
static VD_INLINE Vdf2    vd_fall2            (VdCgf32 s)                                      { return vd_fm2(s, s);               }
static VD_INLINE Vdd2    vd_dall2            (VdCgf64 s)                                      { return vd_dm2(s, s);               }
static VD_INLINE Vdr2    vd_rall2            (Vdrea s)                                        { return vd_rm2(s, s);               }
static VD_INLINE Vdf2    vd_fzero2           (void)                                           { return vd_fall2(0.f);              }
static VD_INLINE Vdd2    vd_dzero2           (void)                                           { return vd_dall2(0.f);              }
static VD_INLINE Vdr2    vd_rzero2           (void)                                           { return vd_rall2(0.f);              }

static VD_INLINE Vdf3    vd_fm3              (VdCgf32 x, VdCgf32 y, VdCgf32 z)                { return (Vdf3) {.e = {x, y, z}};    }
static VD_INLINE Vdd3    vd_dm3              (VdCgf64 x, VdCgf64 y, VdCgf64 z)                { return (Vdd3) {.e = {x, y, z}};    }
static VD_INLINE Vdr3    vd_rm3              (Vdrea x, Vdrea y, Vdrea z)                      { return (Vdr3) {.e = {x, y, z}};    }
static VD_INLINE Vdf3    vd_fall3            (VdCgf32 s)                                      { return vd_fm3(s, s, s);            }
static VD_INLINE Vdd3    vd_dall3            (VdCgf64 s)                                      { return vd_dm3(s, s, s);            }
static VD_INLINE Vdr3    vd_rall3            (Vdrea s)                                        { return vd_rm3(s, s, s);            }
static VD_INLINE Vdf3    vd_fzero3           (void)                                           { return vd_fall3(0.f);              }
static VD_INLINE Vdd3    vd_dzero3           (void)                                           { return vd_dall3(0.f);              }
static VD_INLINE Vdr3    vd_rzero3           (void)                                           { return vd_rall3(0.f);              }

static VD_INLINE Vdf4    vd_fm4              (VdCgf32 x, VdCgf32 y, VdCgf32 z, VdCgf32 w)     { return (Vdf4) {.e = {x, y, z, w}}; }
static VD_INLINE Vdd4    vd_dm4              (VdCgf64 x, VdCgf64 y, VdCgf64 z, VdCgf64 w)     { return (Vdd4) {.e = {x, y, z, w}}; }
static VD_INLINE Vdr4    vd_rm4              (Vdrea x, Vdrea y, Vdrea z, Vdrea w)             { return (Vdr4) {.e = {x, y, z, w}}; }
static VD_INLINE Vdf4    vd_fall4            (VdCgf32 s)                                      { return vd_fm4(s, s, s, s);         }
static VD_INLINE Vdd4    vd_dall4            (VdCgf64 s)                                      { return vd_dm4(s, s, s, s);         }
static VD_INLINE Vdr4    vd_rall4            (Vdrea s)                                        { return vd_rm4(s, s, s, s);         }
static VD_INLINE Vdf4    vd_fzero4           (void)                                           { return vd_fall4(0.f);              }
static VD_INLINE Vdd4    vd_dzero4           (void)                                           { return vd_dall4(0.f);              }
static VD_INLINE Vdr4    vd_rzero4           (void)                                           { return vd_rall4(0.f);              }

static VD_INLINE Vdf4x4  vd_fm4x4            (VdCgf32 a0, VdCgf32 a1, VdCgf32 a2, VdCgf32 a3,
                                              VdCgf32 b0, VdCgf32 b1, VdCgf32 b2, VdCgf32 b3,
                                              VdCgf32 c0, VdCgf32 c1, VdCgf32 c2, VdCgf32 c3,
                                              VdCgf32 d0, VdCgf32 d1, VdCgf32 d2, VdCgf32 d3) { return (Vdf4x4)  {.e = { {a0, a1, a2, a3}, {b0, b1, b2, b3}, {c0, c1, c2, c3}, {d0, d1, d2, d3} } }; }
static VD_INLINE Vdd4x4  vd_dm4x4            (VdCgf64 a0, VdCgf64 a1, VdCgf64 a2, VdCgf64 a3,
                                              VdCgf64 b0, VdCgf64 b1, VdCgf64 b2, VdCgf64 b3,
                                              VdCgf64 c0, VdCgf64 c1, VdCgf64 c2, VdCgf64 c3,
                                              VdCgf64 d0, VdCgf64 d1, VdCgf64 d2, VdCgf64 d3) { return (Vdd4x4)  {.e = { {a0, a1, a2, a3}, {b0, b1, b2, b3}, {c0, c1, c2, c3}, {d0, d1, d2, d3} } }; }
static VD_INLINE Vdr4x4  vd_rm4x4            (Vdrea a0, Vdrea a1, Vdrea a2, Vdrea a3,
                                              Vdrea b0, Vdrea b1, Vdrea b2, Vdrea b3,
                                              Vdrea c0, Vdrea c1, Vdrea c2, Vdrea c3,
                                              Vdrea d0, Vdrea d1, Vdrea d2, Vdrea d3)         { return (Vdr4x4)  {.e = { {a0, a1, a2, a3}, {b0, b1, b2, b3}, {c0, c1, c2, c3}, {d0, d1, d2, d3} } }; }

static VD_INLINE Vdf4x4  vd_fall4x4          (VdCgf32 v)                                      { return vd_fm4x4  (v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v);                                                  }
static VD_INLINE Vdd4x4  vd_dall4x4          (VdCgf64 v)                                      { return vd_dm4x4  (v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v);                                                  }
static VD_INLINE Vdr4x4  vd_rall4x4          (Vdrea v)                                        { return vd_rm4x4  (v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v);                                                  }

static VD_INLINE Vdfquat vd_fmquat           (Vdf3 ipart, VdCgf32 rpart)                      { return (Vdfquat) {.e = {ipart.x, ipart.y, ipart.z, rpart}};                                          }
static VD_INLINE Vddquat vd_dmquat           (Vdd3 ipart, VdCgf64 rpart)                      { return (Vddquat) {.e = {ipart.x, ipart.y, ipart.z, rpart}};                                          }
static VD_INLINE Vdrquat vd_rmquat           (Vdr3 ipart, Vdrea rpart)                        { return (Vdrquat) {.e = {ipart.x, ipart.y, ipart.z, rpart}};                                          }

/* ----VECTOR ALGEBRA------------------------------------------------------------------------------------------------ */
static VD_INLINE Vdf2    vd_fadd2            (Vdf2 a, Vdf2 b)                                 { return vd_fm2(a.x + b.x, a.y + b.y); }
static VD_INLINE Vdd2    vd_dadd2            (Vdd2 a, Vdd2 b)                                 { return vd_dm2(a.x + b.x, a.y + b.y); }
static VD_INLINE Vdr2    vd_radd2            (Vdr2 a, Vdr2 b)                                 { return vd_rm2(a.x + b.x, a.y + b.y); }

static VD_INLINE Vdf3    vd_fadd3            (Vdf3 a, Vdf3 b)                                 { return vd_fm3(a.x + b.x, a.y + b.y, a.z + b.z); }
static VD_INLINE Vdd3    vd_dadd3            (Vdd3 a, Vdd3 b)                                 { return vd_dm3(a.x + b.x, a.y + b.y, a.z + b.z); }
static VD_INLINE Vdr3    vd_radd3            (Vdr3 a, Vdr3 b)                                 { return vd_rm3(a.x + b.x, a.y + b.y, a.z + b.z); }

static VD_INLINE Vdf4    vd_fadd4            (Vdf4 a, Vdf4 b)                                 { return vd_fm4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
static VD_INLINE Vdd4    vd_dadd4            (Vdd4 a, Vdd4 b)                                 { return vd_dm4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
static VD_INLINE Vdr4    vd_radd4            (Vdr4 a, Vdr4 b)                                 { return vd_rm4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

static VD_INLINE Vdf2    vd_fsub2            (Vdf2 a, Vdf2 b)                                 { return vd_fm2(a.x - b.x, a.y - b.y); }
static VD_INLINE Vdd2    vd_dsub2            (Vdd2 a, Vdd2 b)                                 { return vd_dm2(a.x - b.x, a.y - b.y); }
static VD_INLINE Vdr2    vd_rsub2            (Vdr2 a, Vdr2 b)                                 { return vd_rm2(a.x - b.x, a.y - b.y); }

static VD_INLINE Vdf3    vd_fsub3            (Vdf3 a, Vdf3 b)                                 { return vd_fm3(a.x - b.x, a.y - b.y, a.z - b.z); }
static VD_INLINE Vdd3    vd_dsub3            (Vdd3 a, Vdd3 b)                                 { return vd_dm3(a.x - b.x, a.y - b.y, a.z - b.z); }
static VD_INLINE Vdr3    vd_rsub3            (Vdr3 a, Vdr3 b)                                 { return vd_rm3(a.x - b.x, a.y - b.y, a.z - b.z); }

static VD_INLINE Vdf4    vd_fsub4            (Vdf4 a, Vdf4 b)                                 { return vd_fm4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
static VD_INLINE Vdd4    vd_dsub4            (Vdd4 a, Vdd4 b)                                 { return vd_dm4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
static VD_INLINE Vdr4    vd_rsub4            (Vdr4 a, Vdr4 b)                                 { return vd_rm4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

static VD_INLINE Vdf2    vd_fscale2          (Vdf2 v, VdCgf32 s)                              { return vd_fm2(v.x * s, v.y * s); }
static VD_INLINE Vdd2    vd_dscale2          (Vdd2 v, VdCgf64 s)                              { return vd_dm2(v.x * s, v.y * s); }
static VD_INLINE Vdr2    vd_rscale2          (Vdr2 v, Vdrea s)                                { return vd_rm2(v.x * s, v.y * s); }

static VD_INLINE Vdf3    vd_fscale3          (Vdf3 v, VdCgf32 s)                              { return vd_fm3(v.x * s, v.y * s, v.z * s); }
static VD_INLINE Vdd3    vd_dscale3          (Vdd3 v, VdCgf64 s)                              { return vd_dm3(v.x * s, v.y * s, v.z * s); }
static VD_INLINE Vdr3    vd_rscale3          (Vdr3 v, Vdrea s)                                { return vd_rm3(v.x * s, v.y * s, v.z * s); }

static VD_INLINE Vdf2    vd_fdiv2            (Vdf2 v, VdCgf32 s)                              { return vd_fm2(v.x / s, v.y / s); }
static VD_INLINE Vdd2    vd_ddiv2            (Vdd2 v, VdCgf64 s)                              { return vd_dm2(v.x / s, v.y / s); }
static VD_INLINE Vdr2    vd_rdiv2            (Vdr2 v, Vdrea s)                                { return vd_rm2(v.x / s, v.y / s); }

static VD_INLINE Vdf3    vd_fdiv3            (Vdf3 v, VdCgf32 s)                              { return vd_fm3(v.x / s, v.y / s, v.z / s); }
static VD_INLINE Vdd3    vd_ddiv3            (Vdd3 v, VdCgf64 s)                              { return vd_dm3(v.x / s, v.y / s, v.z / s); }
static VD_INLINE Vdr3    vd_rdiv3            (Vdr3 v, Vdrea s)                                { return vd_rm3(v.x / s, v.y / s, v.z / s); }

static VD_INLINE Vdf4    vd_fdiv4            (Vdf4 v, VdCgf32 s)                              { return vd_fm4(v.x / s, v.y / s, v.z / s, v.w / s); }
static VD_INLINE Vdd4    vd_ddiv4            (Vdd4 v, VdCgf64 s)                              { return vd_dm4(v.x / s, v.y / s, v.z / s, v.w / s); }
static VD_INLINE Vdr4    vd_rdiv4            (Vdr4 v, Vdrea s)                                { return vd_rm4(v.x / s, v.y / s, v.z / s, v.w / s); }

static VD_INLINE VdCgf32 vd_fdot2            (Vdf2 a, Vdf2 b)                                 { return a.x * b.x + a.y * b.y; }
static VD_INLINE VdCgf64 vd_ddot2            (Vdd2 a, Vdd2 b)                                 { return a.x * b.x + a.y * b.y; }
static VD_INLINE Vdrea   vd_rdot2            (Vdr2 a, Vdr2 b)                                 { return a.x * b.x + a.y * b.y; }

static VD_INLINE VdCgf32 vd_fdot3            (Vdf3 a, Vdf3 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z; }
static VD_INLINE VdCgf64 vd_ddot3            (Vdd3 a, Vdd3 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z; }
static VD_INLINE Vdrea   vd_rdot3            (Vdr3 a, Vdr3 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z; }

static VD_INLINE VdCgf32 vd_fdot4            (Vdf4 a, Vdf4 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
static VD_INLINE VdCgf64 vd_ddot4            (Vdd4 a, Vdd4 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
static VD_INLINE Vdrea   vd_rdot4            (Vdr4 a, Vdr4 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

static VD_INLINE VdCgf32 vd_flensq2          (Vdf2 v)                                         { return vd_fdot2(v, v); }
static VD_INLINE VdCgf64 vd_dlensq2          (Vdd2 v)                                         { return vd_ddot2(v, v); }
static VD_INLINE Vdrea   vd_rlensq2          (Vdr2 v)                                         { return vd_rdot2(v, v); }

static VD_INLINE VdCgf32 vd_flensq3          (Vdf3 v)                                         { return vd_fdot3(v, v); }
static VD_INLINE VdCgf64 vd_dlensq3          (Vdd3 v)                                         { return vd_ddot3(v, v); }
static VD_INLINE Vdrea   vd_rlensq3          (Vdr3 v)                                         { return vd_rdot3(v, v); }

static VD_INLINE VdCgf32 vd_flensq4          (Vdf4 v)                                         { return vd_fdot4(v, v); }
static VD_INLINE VdCgf64 vd_dlensq4          (Vdd4 v)                                         { return vd_ddot4(v, v); }
static VD_INLINE Vdrea   vd_rlensq4          (Vdr4 v)                                         { return vd_rdot4(v, v); }

static VD_INLINE Vdf2    vd_fnormalize2      (Vdf2 v)                                         { VdCgf32 l = vd_flensq2(v); return vd_fdiv2(v, VD_CG_FSQRT(l)); }
static VD_INLINE Vdd2    vd_dnormalize2      (Vdd2 v)                                         { VdCgf64 l = vd_dlensq2(v); return vd_ddiv2(v, VD_CG_DSQRT(l)); }
static VD_INLINE Vdr2    vd_rnormalize2      (Vdr2 v)                                         { Vdrea l   = vd_rlensq2(v); return vd_rdiv2(v, VD_CG_RSQRT(l)); }

static VD_INLINE Vdf3    vd_fnormalize3      (Vdf3 v)                                         { VdCgf32 l = vd_flensq3(v); return vd_fdiv3(v, VD_CG_FSQRT(l)); }
static VD_INLINE Vdd3    vd_dnormalize3      (Vdd3 v)                                         { VdCgf64 l = vd_dlensq3(v); return vd_ddiv3(v, VD_CG_DSQRT(l)); }
static VD_INLINE Vdr3    vd_rnormalize3      (Vdr3 v)                                         { Vdrea l   = vd_rlensq3(v); return vd_rdiv3(v, VD_CG_RSQRT(l)); }

static VD_INLINE Vdf4    vd_fnormalize4      (Vdf4 v)                                         { VdCgf32 l = vd_flensq4(v); return vd_fdiv4(v, VD_CG_FSQRT(l)); }
static VD_INLINE Vdd4    vd_dnormalize4      (Vdd4 v)                                         { VdCgf64 l = vd_dlensq4(v); return vd_ddiv4(v, VD_CG_DSQRT(l)); }
static VD_INLINE Vdr4    vd_rnormalize4      (Vdr4 v)                                         { Vdrea l   = vd_rlensq4(v); return vd_rdiv4(v, VD_CG_RSQRT(l)); }

static VD_INLINE Vdf2    vd_fnoz2            (Vdf2 v)                                         { VdCgf32 l = vd_flensq2(v); if (l == 0.f) return vd_fzero2(); else return vd_fdiv2(v, VD_CG_FSQRT(l)); }
static VD_INLINE Vdd2    vd_dnoz2            (Vdd2 v)                                         { VdCgf64 l = vd_dlensq2(v); if (l == 0.f) return vd_dzero2(); else return vd_ddiv2(v, VD_CG_DSQRT(l)); }
static VD_INLINE Vdr2    vd_rnoz2            (Vdr2 v)                                         { Vdrea l   = vd_rlensq2(v); if (l == 0.f) return vd_rzero2(); else return vd_rdiv2(v, VD_CG_RSQRT(l)); }
static VD_INLINE Vdf3    vd_fnoz3            (Vdf3 v)                                         { VdCgf32 l = vd_flensq3(v); if (l == 0.f) return vd_fzero3(); else return vd_fdiv3(v, VD_CG_FSQRT(l)); }
static VD_INLINE Vdd3    vd_dnoz3            (Vdd3 v)                                         { VdCgf64 l = vd_dlensq3(v); if (l == 0.f) return vd_dzero3(); else return vd_ddiv3(v, VD_CG_DSQRT(l)); }
static VD_INLINE Vdr3    vd_rnoz3            (Vdr3 v)                                         { Vdrea l   = vd_rlensq3(v); if (l == 0.f) return vd_rzero3(); else return vd_rdiv3(v, VD_CG_RSQRT(l)); }
static VD_INLINE Vdf4    vd_fnoz4            (Vdf4 v)                                         { VdCgf32 l = vd_flensq4(v); if (l == 0.f) return vd_fzero4(); else return vd_fdiv4(v, VD_CG_FSQRT(l)); }
static VD_INLINE Vdd4    vd_dnoz4            (Vdd4 v)                                         { VdCgf64 l = vd_dlensq4(v); if (l == 0.f) return vd_dzero4(); else return vd_ddiv4(v, VD_CG_DSQRT(l)); }
static VD_INLINE Vdr4    vd_rnoz4            (Vdr4 v)                                         { Vdrea l   = vd_rlensq4(v); if (l == 0.f) return vd_rzero4(); else return vd_rdiv4(v, VD_CG_RSQRT(l)); }

static VD_INLINE Vdf3    vd_fcross3          (Vdf3 a, Vdf3 b)                                 { return vd_fm3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
static VD_INLINE Vdd3    vd_dcross3          (Vdd3 a, Vdd3 b)                                 { return vd_dm3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
static VD_INLINE Vdr3    vd_rcross3          (Vdr3 a, Vdr3 b)                                 { return vd_rm3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

/* ----MATRIX ALGEBRA------------------------------------------------------------------------------------------------ */
static VD_INLINE Vdf4x4  vd_ftranspose4x4     (Vdf4x4 *m);
static VD_INLINE Vdf4x4  vd_fmul4x4           (Vdf4x4 *a, Vdf4x4 *b);

/* ----QUATERNION ALGEBRA-------------------------------------------------------------------------------------------- */
static VD_INLINE Vdfquat vd_fidentityquat    (void)                                           { return vd_fmquat(vd_fzero3(), 1.f); }
static VD_INLINE Vddquat vd_didentityquat    (void)                                           { return vd_dmquat(vd_dzero3(), 1.0); }
static VD_INLINE Vdrquat vd_ridentityquat    (void)                                           { return vd_rmquat(vd_rzero3(), 1.0); }

static VD_INLINE VdCgf32 vd_flensqquat       (Vdfquat q)                                      { return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w; }
static VD_INLINE VdCgf64 vd_dlensqquat       (Vddquat q)                                      { return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w; }
static VD_INLINE Vdrea   vd_rlensqquat       (Vdrquat q)                                      { return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w; }

static VD_INLINE Vdfquat vd_fnormalizequat   (Vdfquat q)                                      { VdCgf32 l = vd_flensqquat(q); return vd_fdiv4(q, VD_CG_FSQRT(l)); }
static VD_INLINE Vddquat vd_dnormalizequat   (Vddquat q)                                      { VdCgf64 l = vd_dlensqquat(q); return vd_ddiv4(q, VD_CG_DSQRT(l)); }
static VD_INLINE Vdrquat vd_rnormalizequat   (Vdrquat q)                                      { Vdrea l = vd_rlensqquat(q); return vd_rdiv4(q, VD_CG_RSQRT(l)); }

static VD_INLINE Vdfquat vd_fnozquat         (Vdfquat q)                                      { VdCgf32 l = vd_flensqquat(q); if (l == 0.f) return vd_fidentityquat(); else return vd_fdiv4(q, VD_CG_FSQRT(l)); }
static VD_INLINE Vddquat vd_dnozquat         (Vddquat q)                                      { VdCgf64 l = vd_dlensqquat(q); if (l == 0.0) return vd_didentityquat(); else return vd_ddiv4(q, VD_CG_DSQRT(l)); }
static VD_INLINE Vdrquat vd_rnozquat         (Vdrquat q)                                      { Vdrea l = vd_rlensqquat(q); if (l == 0.0) return vd_ridentityquat(); else return vd_rdiv4(q, VD_CG_RSQRT(l)); }

static VD_INLINE Vdfquat vd_faxis_anglequat  (Vdf3 axis, VdCgf32 angle)                       { return vd_fmquat(vd_fscale3(axis, VD_CG_FSIN(angle * .5f)), 1.f); }
static VD_INLINE Vdfquat vd_fmulquat         (Vdfquat q1, Vdfquat q2);
static VD_INLINE Vdf3    vd_fmulquat3        (Vdfquat q, Vdf3 v);
static VD_INLINE Vdf4x4  vd_fto4x4quat       (Vdfquat q);
static VD_INLINE Vdfquat vd_feulerquat       (Vdf3 euler);

/* ----COORDINATE SYSTEMS-------------------------------------------------------------------------------------------- */
static VD_INLINE Vdf4x4  vd_fidentity4x4      (void);
static VD_INLINE Vdf4x4  vd_fdx_to_vk4x4      (void);
static VD_INLINE Vdf4x4  vd_fperspective4x4   (VdCgf32 fovyrad, VdCgf32 aspect, VdCgf32 pnr, VdCgf32 pfr);
static VD_INLINE Vdf4x4  vd_fperspective4x4_vk(VdCgf32 fovyrad, VdCgf32 aspect, VdCgf32 pnr, VdCgf32 pfr);
static VD_INLINE Vdf4x4  vd_flookat4x4        (Vdf3 fwd, Vdf3 up, Vdf3 right);
static VD_INLINE Vdf4x4  vd_ftranslation4x4   (Vdf3 v);
static VD_INLINE Vdf4x4  vd_frotation_yaw4x4  (VdCgf32 rad);
static VD_INLINE void    vd_ftranslate4x4     (Vdf4x4 *m, Vdf3 v);
static VD_INLINE void    vd_frotate_yaw4x4    (Vdf4x4 *m, VdCgf32 rad);
static VD_INLINE Vdf4x4  vd_fvk_to_dx4x4      (void);

/* ----LOGGING------------------------------------------------------------------------------------------------------- */
static VD_INLINE void     vd_fprint4x4          (Vdf4x4 *m);

/* ----UTILITY------------------------------------------------------------------------------------------------------- */
static VD_INLINE VdCgf32  vd_fwrap_degrees      (VdCgf32 d)                                     { return d - VD_CG_FTAU * VD_CG_FFLOOR(d / VD_CG_FTAU);       }
static VD_INLINE VdCgf32  vd_fclamp             (VdCgf32 min, VdCgf32 x, VdCgf32 max)           { if (x < min) return min; if (x > max) return max; return x; }

/* ----VECTOR ALGEBRA IMPL------------------------------------------------------------------------------------------- */

/* ----MATRIX ALGEBRA IMPL------------------------------------------------------------------------------------------- */
static VD_INLINE Vdf4x4 vd_ftranspose4x4(Vdf4x4 *m)
{
    return vd_fm4x4(
        m->a0, m->b0, m->c0, m->d0,
        m->a1, m->b1, m->c1, m->d1,
        m->a2, m->b2, m->c2, m->d2,
        m->a3, m->b3, m->c3, m->d3);
}

static VD_INLINE Vdf4x4 vd_fmul4x4(Vdf4x4 *a, Vdf4x4 *b)
{
    Vdf4x4 result = vd_fall4x4(0.f);
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
static VD_INLINE Vdfquat vd_fmulquat(Vdfquat q1, Vdfquat q2)
{
    Vdfquat result;
    result.x =  q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
    result.y = -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
    result.z =  q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
    result.w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;
    return result;
}

static VD_INLINE Vdf3 vd_fmulquat3(Vdfquat q, Vdf3 v)
{
    Vdf3  u = q.xyz;
    VdCgf32 s = q.w;

    Vdf3 result = vd_fscale3(u, 2.f * vd_fdot3(u, v));
    result        = vd_fadd3(result, vd_fscale3(v, s * s - vd_fdot3(u, u)));
    result        = vd_fadd3(result, vd_fscale3(vd_fcross3(u, v), 2.f * s));
    return result;
}

static VD_INLINE Vdf4x4 vd_fto4x4quat(Vdfquat q)
{
    VdCgf32 qxx = (q.x * q.x);
    VdCgf32 qyy = (q.y * q.y);
    VdCgf32 qzz = (q.z * q.z);
    VdCgf32 qxz = (q.x * q.z);
    VdCgf32 qxy = (q.x * q.y);
    VdCgf32 qyz = (q.y * q.z);
    VdCgf32 qwx = (q.w * q.x);
    VdCgf32 qwy = (q.w * q.y);
    VdCgf32 qwz = (q.w * q.z);
    return vd_fm4x4(
        1.f - 2.f * (qyy + qzz), 2.f * (qxy + qwz),       2.f * (qxz - qwy),       0.f,
        2.f * (qxy - qwz),       1.f - 2.f * (qxx + qzz), 2.f * (qyz + qwx),       0.f,
        2.f * (qxz + qwy),       2.f * (qyz - qwx),       1.f - 2.f * (qxx + qyy), 0.f,
        0.f,                     0.f,                     0.f,                     1.f);
}

static VD_INLINE Vdfquat vd_feulerquat(Vdf3 euler)
{
    VdCgf32 cy = VD_CG_FCOS(euler.y * 0.5f); // yaw
    VdCgf32 sy = VD_CG_FSIN(euler.y * 0.5f);
    VdCgf32 cp = VD_CG_FCOS(euler.x * 0.5f); // pitch
    VdCgf32 sp = VD_CG_FSIN(euler.x * 0.5f);
    VdCgf32 cr = VD_CG_FCOS(euler.z * 0.5f); // roll
    VdCgf32 sr = VD_CG_FSIN(euler.z * 0.5f);

    Vdfquat q;
    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * sp * cr + sy * cp * sr;
    q.y = sy * cp * cr - cy * sp * sr;
    q.z = cy * cp * sr - sy * sp * cr;
    return q;
}

/* ----COORDINATE SYSTEMS IMPL--------------------------------------------------------------------------------------- */
static VD_INLINE Vdf4x4 vd_fidentity4x4(void)
{
    return vd_fm4x4(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}

static VD_INLINE Vdf4x4 vd_fdx_to_vk4x4(void)
{
    return vd_fm4x4(
       -1.f, 0.f, 0.f, 0.f,
        0.f,-1.f, 0.f, 0.f,
        0.f, 0.f,-1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}

static VD_INLINE Vdf4x4 vd_fperspective4x4(VdCgf32 fovyrad, VdCgf32 aspect, VdCgf32 pnr, VdCgf32 pfr)
{
    VdCgf32 half_tan_fovy = VD_CG_FTAN(fovyrad * 0.5f);

    return vd_fm4x4(
        1.f / ((half_tan_fovy) * aspect), 0.f,                  0.f,                0.f,
        0.f,                              1.f / half_tan_fovy,  0.f,                0.f,
        0.f,                              0.f,                  pfr / (pfr - pnr), -(pnr * pfr) / (pfr - pnr),
        0.f,                              0.f,                  1.f,                0.f);
}

static VD_INLINE Vdf4x4 vd_fperspective4x4_vk(VdCgf32 fovyrad, VdCgf32 aspect, VdCgf32 pnr, VdCgf32 pfr)
{
    VdCgf32 half_tan_fovy = VD_CG_FTAN(fovyrad * 0.5f);

    return vd_fm4x4(
        1.f / ((half_tan_fovy) * aspect), 0.f,                  0.f,                0.f,
        0.f,                              1.f / half_tan_fovy,  0.f,                0.f,
        0.f,                              0.f,                  pfr / (pfr - pnr), -(pnr * pfr) / (pfr - pnr),
        0.f,                              0.f,                  1.f,                0.f);
}

static VD_INLINE Vdf4x4 vd_flookat4x4(Vdf3 from, Vdf3 to, Vdf3 up)
{
    Vdf3 fwd = vd_fsub3(from, to);
    fwd = vd_fnoz3(fwd);

    Vdf3 right = vd_fcross3(up, fwd);
    right = vd_fnoz3(right);

    Vdf3 up2 = vd_fcross3(fwd, right);

    Vdf3 xaxis = right;
    Vdf3 yaxis = up2;
    Vdf3 zaxis = fwd;

    Vdf3 meye = vd_fscale3(from, -1.f);

    return vd_fm4x4(
        xaxis.x, xaxis.y, xaxis.z, vd_fdot3(xaxis, meye),
        yaxis.x, yaxis.y, yaxis.z, vd_fdot3(yaxis, meye),
        zaxis.x, zaxis.y, zaxis.z, vd_fdot3(zaxis, meye),
        0.f, 0.f, 0.f, 1.f);
}

static VD_INLINE Vdf4x4 vd_ftranslation4x4(Vdf3 v)
{
    return vd_fm4x4(
        1.f, 0.f, 0.f, v.x,
        0.f, 1.f, 0.f, v.y,
        0.f, 0.f, 1.f, v.z,
        0.f, 0.f, 0.f, 1.f);
}

static VD_INLINE Vdf4x4 vd_frotation_yaw4x4(VdCgf32 rad)
{
    return vd_fm4x4(
        VD_CG_FCOS(rad), 0.f, -VD_CG_FSIN(rad), 0.f,
        0.f,             1.f, 0.f,              0.f,
        VD_CG_FSIN(rad), 0.f, VD_CG_FCOS(rad),  0.f,
        0.f,             0.f, 0.f,              1.f);
}

static VD_INLINE void vd_ftranslate4x4(Vdf4x4 *m, Vdf3 v)
{
    Vdf4x4 t = vd_ftranslation4x4(v);
    Vdf4x4 r = vd_fmul4x4(m, &t);
    *m = r;
}

static VD_INLINE void vd_frotate_yaw4x4(Vdf4x4 *m, VdCgf32 rad)
{
    Vdf4x4 t = vd_frotation_yaw4x4(rad);
    Vdf4x4 r = vd_fmul4x4(m, &t);
    *m = r;
}

static VD_INLINE Vdf4x4 vd_fvk_to_dx4x4(void)
{
    return vd_fm4x4(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f,-1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}

/* ----LOGGING IMPL-------------------------------------------------------------------------------------------------- */
static VD_INLINE void vd_fprint4x4(Vdf4x4 *m)
{
    VD_LOGF("| %04.2f %04.2f %04.2f %04.2f | ", m->a0, m->a1, m->a2, m->a3);
    VD_LOGF("| %04.2f %04.2f %04.2f %04.2f | ", m->b0, m->b1, m->b2, m->b3);
    VD_LOGF("| %04.2f %04.2f %04.2f %04.2f | ", m->c0, m->c1, m->c2, m->c3);
    VD_LOGF("| %04.2f %04.2f %04.2f %04.2f | ", m->d0, m->d1, m->d2, m->d3);
}

#if VD_MACRO_ABBREVIATIONS
#define rea                      Vdrea
#define f2                       Vdf2
#define f3                       Vdf3
#define f4                       Vdf4
#define f2x2                     Vdf2x2
#define f3x3                     Vdf3x3
#define f4x4                     Vdf4x4
#define fquat                    Vdfquat
#define d2                       Vdd2
#define d3                       Vdd3
#define d4                       Vdd4
#define d2x2                     Vdd2x2
#define d3x3                     Vdd3x3
#define d4x4                     Vdd4x4
#define dquat                    Vddquat
#define r2                       Vdr2
#define r3                       Vdr3
#define r4                       Vdr4
#define r2x2                     Vdr2x2
#define r3x3                     Vdr3x3
#define r4x4                     Vdr4x4
#define rquat                    Vdrquat

/* ----INITIALIZATION------------------------------------------------------------------------------------------------ */
#define dm2                      vd_dm2
#define rm2                      vd_rm2
#define fm2                      vd_fm2
#define fall2                    vd_fall2
#define dall2                    vd_dall2
#define rall2                    vd_rall2
#define fzero2                   vd_fzero2
#define dzero2                   vd_dzero2
#define rzero2                   vd_rzero2
#define fm3                      vd_fm3
#define dm3                      vd_dm3
#define rm3                      vd_rm3
#define fall3                    vd_fall3
#define dall3                    vd_dall3
#define rall3                    vd_rall3
#define fzero3                   vd_fzero3
#define dzero3                   vd_dzero3
#define rzero3                   vd_rzero3
#define fm4                      vd_fm4
#define dm4                      vd_dm4
#define rm4                      vd_rm4
#define fall4                    vd_fall4
#define dall4                    vd_dall4
#define rall4                    vd_rall4
#define fzero4                   vd_fzero4
#define dzero4                   vd_dzero4
#define rzero4                   vd_rzero4
#define fm4x4                    vd_fm4x4
#define dm4x4                    vd_dm4x4
#define rm4x4                    vd_rm4x4
#define fall4x4                  vd_fall4x4
#define dall4x4                  vd_dall4x4
#define rall4x4                  vd_rall4x4
#define fmquat                   vd_fmquat
#define dmquat                   vd_dmquat
#define rmquat                   vd_rmquat

/* ----VECTOR ALGEBRA------------------------------------------------------------------------------------------------ */
#define fadd2                    vd_fadd2 
#define dadd2                    vd_dadd2 
#define radd2                    vd_radd2 
#define fadd3                    vd_fadd3 
#define dadd3                    vd_dadd3 
#define radd3                    vd_radd3 
#define fadd4                    vd_fadd4 
#define dadd4                    vd_dadd4 
#define radd4                    vd_radd4 
#define fsub2                    vd_fsub2 
#define dsub2                    vd_dsub2 
#define rsub2                    vd_rsub2 
#define fsub3                    vd_fsub3 
#define dsub3                    vd_dsub3 
#define rsub3                    vd_rsub3 
#define fsub4                    vd_fsub4 
#define dsub4                    vd_dsub4 
#define rsub4                    vd_rsub4 
#define fscale2                  vd_fscale2 
#define dscale2                  vd_dscale2 
#define rscale2                  vd_rscale2 
#define fscale3                  vd_fscale3 
#define dscale3                  vd_dscale3 
#define rscale3                  vd_rscale3 
#define fdiv2                    vd_fdiv2 
#define ddiv2                    vd_ddiv2 
#define rdiv2                    vd_rdiv2 
#define fdiv3                    vd_fdiv3 
#define ddiv3                    vd_ddiv3 
#define rdiv3                    vd_rdiv3 
#define fdiv4                    vd_fdiv4 
#define ddiv4                    vd_ddiv4 
#define rdiv4                    vd_rdiv4 
#define fdot2                    vd_fdot2 
#define ddot2                    vd_ddot2 
#define rdot2                    vd_rdot2 
#define fdot3                    vd_fdot3 
#define ddot3                    vd_ddot3 
#define rdot3                    vd_rdot3 
#define fdot4                    vd_fdot4 
#define ddot4                    vd_ddot4 
#define rdot4                    vd_rdot4 
#define flensq2                  vd_flensq2 
#define dlensq2                  vd_dlensq2 
#define rlensq2                  vd_rlensq2 
#define flensq3                  vd_flensq3 
#define dlensq3                  vd_dlensq3 
#define rlensq3                  vd_rlensq3 
#define flensq4                  vd_flensq4 
#define dlensq4                  vd_dlensq4 
#define rlensq4                  vd_rlensq4 
#define fnormalize2              vd_fnormalize2 
#define dnormalize2              vd_dnormalize2 
#define rnormalize2              vd_rnormalize2 
#define fnormalize3              vd_fnormalize3 
#define dnormalize3              vd_dnormalize3 
#define rnormalize3              vd_rnormalize3 
#define fnormalize4              vd_fnormalize4 
#define dnormalize4              vd_dnormalize4 
#define rnormalize4              vd_rnormalize4 
#define fnoz2                    vd_fnoz2 
#define dnoz2                    vd_dnoz2 
#define rnoz2                    vd_rnoz2 
#define fnoz3                    vd_fnoz3 
#define dnoz3                    vd_dnoz3 
#define rnoz3                    vd_rnoz3 
#define fnoz4                    vd_fnoz4 
#define dnoz4                    vd_dnoz4 
#define rnoz4                    vd_rnoz4 
#define fcross3                  vd_fcross3 
#define dcross3                  vd_dcross3 
#define rcross3                  vd_rcross3 

/* ----MATRIX ALGEBRA------------------------------------------------------------------------------------------------ */
#define ftranspose4x4            vd_ftranspose4x4 
#define fmul4x4                  vd_fmul4x4 

/* ----QUATERNION ALGEBRA-------------------------------------------------------------------------------------------- */
#define fidentityquat            vd_fidentityquat 
#define didentityquat            vd_didentityquat 
#define ridentityquat            vd_ridentityquat 
#define flensqquat               vd_flensqquat 
#define dlensqquat               vd_dlensqquat 
#define rlensqquat               vd_rlensqquat 
#define fnormalizequat           vd_fnormalizequat 
#define dnormalizequat           vd_dnormalizequat 
#define rnormalizequat           vd_rnormalizequat 
#define fnozquat                 vd_fnozquat 
#define dnozquat                 vd_dnozquat 
#define rnozquat                 vd_rnozquat 
#define faxis_anglequat          vd_faxis_anglequat 
#define fmulquat                 vd_fmulquat 
#define fmulquat3                vd_fmulquat3 
#define fto4x4quat               vd_fto4x4quat 
#define feulerquat               vd_feulerquat 

/* ----COORDINATE SYSTEMS-------------------------------------------------------------------------------------------- */
#define fidentity4x4             vd_fidentity4x4 
#define fdx_to_vk4x4             vd_fdx_to_vk4x4 
#define fperspective4x4          vd_fperspective4x4 
#define fperspective4x4_vk       vd_fperspective4x4_vk 
#define flookat4x4               vd_flookat4x4 
#define ftranslation4x4          vd_ftranslation4x4 
#define frotation_yaw4x4         vd_frotation_yaw4x4 
#define ftranslate4x4            vd_ftranslate4x4 
#define frotate_yaw4x4           vd_frotate_yaw4x4 
#define fvk_to_dx4x4             vd_fvk_to_dx4x4 

/* ----LOGGING------------------------------------------------------------------------------------------------------- */
#define fprint4x4                vd_fprint4x4 

/* ----UTILITY------------------------------------------------------------------------------------------------------- */
#define fwrap_degrees            vd_fwrap_degrees 
#define fclamp                   vd_fclamp 

#endif // VD_MACRO_ABBREVIATIONS

#endif // !VD_CG_H

#ifdef VD_CG_IMPLEMENTATION


#ifndef VD_CG_INCLUDE_INTERNAL_TESTS
#define VD_CG_INCLUDE_INTERNAL_TESTS 0
#endif // !VD_CG_INCLUDE_INTERNAL_TESTS

#if VD_CG_INCLUDE_INTERNAL_TESTS

#endif // VD_CG_INCLUDE_INTERNAL_TESTS

#endif // VD_CG_IMPLEMENTATION