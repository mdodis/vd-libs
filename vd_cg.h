/**
 * vd_cg.h - A maths header file to use with C99
 * ---------------------------------------------------------------------------------------------------------------------
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
 * ---------------------------------------------------------------------------------------------------------------------
 * I'm just gonna use all the keywords so people don't get confused
 * Column Major Matrices, Left Handed Coordinate System, Post Multiply (column vectors)
 */
#ifndef VD_CG_H
#define VD_CG_H
#define VD_CG_VERSION_MAJOR 0
#define VD_CG_VERSION_MINOR 0
#define VD_CG_VERSION_PATCH 1
#define VD_CG_VERSION       ((VD_CG_VERSION_MAJOR << 16) | (VD_CG_VERSION_MINOR << 8) | (VD_CG_VERSION_PATCH))

#ifndef VD_H
typedef float        VdCgf32;
typedef double       VdCgf64;
typedef int          VdCgi32;
typedef unsigned int VdCgu32;
#else
typedef Vdf32   VdCgf32;
typedef Vdf64   VdCgf64;
typedef Vdi32   VdCgi32;
typedef Vdu32   VdCgu32;
#endif // !VD_H

#define VD_CG_PI32           3.14159265359f
#define VD_CG_FPI            3.14159265359f
#define VD_CG_FPI2           (2.f * 3.14159265359f)
#define VD_CG_RAD2DEG_COEFF  ((VdCgf32)180.0f/VD_CG_PI32)
#define VD_CG_DEG2RAD_COEFF  ((VdCgf32)VD_CG_PI32/180.0f)
#define VD_CG_FTAU           360.0f
#define VD_CG_DTAU           360.0
#define VD_CG_FEPSILON       1.19209290E-07F

#ifndef VD_CG_INL
#   ifdef VD_H
#       define VD_CG_INL VD_INLINE
#   else
#       define VD_CG_INL static inline
#   endif
#endif

#define VD_FDEG2RAD(degrees) (VD_CG_DEG2RAD_COEFF * (degrees))
#if VD_MACRO_ABBREVIATIONS
#define fdeg2rad(degrees) VD_FDEG2RAD(degrees)
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
#define VD_CG_FPOW      powf
#define VD_CG_DPOW      pow
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
#define fpow       VD_CG_FPOW
#define dsin       VD_CG_DSIN
#define dcos       VD_CG_DCOS
#define dtan       VD_CG_DTAN
#define dsqrt      VD_CG_DSQRT
#define dacos      VD_CG_DACOS
#define dpow       VD_CG_DPOW
#define vmin(a, b) VD_CG_MIN(a,b)
#define vmax(a, b) VD_CG_MAX(a,b)
#endif

#pragma pack(push, 1)
/* ----SINGLE PRECISION---------------------------------------------------------------------------------------------- */
typedef union __VD_CG_f2 {
    VdCgf32  e[2];
    struct { VdCgf32 x, y; };
    struct { VdCgf32 r, g; };
} VdF2;

typedef union __VD_CG_f3 {
    VdCgf32  e[3];
    struct   { VdCgf32 x, y, z; };
    struct   { VdF2 xy; VdCgf32 __ignore0; };
    struct   { VdCgf32 __ignore2; VdF2 yz; };
} VdF3;

typedef union __VD_CG_f4 {
    VdCgf32  e[4];
    struct   { VdCgf32 x, y, z, w; };
    struct   { VdF2 xy; VdF2 zw; };
    struct   { VdCgf32 __ignore3; VdF2 yz; VdCgf32 __ignore2; };
    struct   { VdF3 xyz; VdCgf32 __ignore0; };
    struct   { VdCgf32 __ignore1; VdF3 yzw; };
} VdF4;

typedef union __VD_CG_f2x2 {
    VdCgf32 e[2][2];
    VdCgf32 p[4];
    VdF2  c[2];
} VdF2x2;

typedef union __VD_CG_f3x3 {
    VdCgf32 e[3][3];
    VdCgf32 p[9];
    VdF3  c[3];
} VdF3x3;

typedef union __VD_CG_f4x4 {
    VdCgf32 e[4][4];
    VdCgf32 p[16];
    VdF4    c[4];
    struct { VdCgf32 a0, a1, a2, a3,
                     b0, b1, b2, b3,
                     c0, c1, c2, c3,
                     d0, d1, d2, d3; };
} VdF4x4;

typedef union __VD_CG_f4 VdFQuat;

typedef struct __VD_CG_fline {
    VdF3 a, b;
} VdFLine;

typedef struct __VD_CG_fray {
    VdF3 origin;
    VdF3 direction;
} VdFRay;

typedef struct __VD_CG_fcylinder {
    VdF3    p;
    VdF3    q;
    VdCgf32 r;
} VdFCylinder;

/* ----DOUBLE PRECISION---------------------------------------------------------------------------------------------- */
typedef union __VD_CG_d2 {
    VdCgf64  e[2];
    struct { VdCgf64 x, y; };
    struct { VdCgf64 r, g; };
} VdD2;

typedef union __VD_CG_d3 {
    VdCgf64  e[3];
    struct   { VdCgf64 x, y, z; };
    struct   { VdD2 xy; VdCgf64 __ignore0; };
    struct   { VdCgf64 __ignore2; VdD2 yz; };
} VdD3;

typedef union __VD_CG_d4 {
    VdCgf64  e[4];
    struct   { VdCgf64 x, y, z, w; };
    struct   { VdD2 xy; VdD2 zw; };
    struct   { VdCgf64 __ignore3; VdD2 yz; VdCgf64 __ignore2; };
    struct   { VdD3 xyz; VdCgf64 __ignore0; };
    struct   { VdCgf64 __ignore1; VdD3 yzw; };
} VdD4;

typedef union __VD_CG_d2x2 {
    VdCgf64 e[2][2];
    VdD2  c[2];
} VdD2x2;

typedef union __VD_CG_d3x3 {
    VdCgf64 e[3][3];
    VdD3  c[3];
} VdD3x3;

typedef union __VD_CG_d4x4 {
    VdCgf64 e[4][4];
    VdD4    c[4];
    struct { VdCgf64 a0, a1, a2, a3,
                     b0, b1, b2, b3, 
                     c0, c1, c2, c3, 
                     d0, d1, d2, d3; };
} VdD4x4;

typedef union __VD_CG_d4 VdDQuat;

typedef struct __VD_CG_dline {
    VdD3 a, b;
} VdDLine;

typedef struct __VD_CG_dray {
    VdF3 origin;
    VdF3 direction;
} VdDRay;

typedef struct __VD_CG_Dcylinder {
    VdD3    p;
    VdD3    q;
    VdCgf64 r;
} VdDCylinder;

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
typedef VdCgf32     VdRea;
typedef VdF2        VdR2;
typedef VdF3        VdR3;
typedef VdF4        VdR4;
typedef VdF2x2      VdR2x2;
typedef VdF3x3      VdR3x3;
typedef VdF4x4      VdR4x4;
typedef VdFQuat     VdRQuat;
typedef VdFLine     VdRLine;
typedef VdFRay      VdRRay;
typedef VdFCylinder VdRCylinder;
#define VD_CG_RSIN  VD_CG_FSIN
#define VD_CG_RCOS  VD_CG_FCOS
#define VD_CG_RTAN  VD_CG_FTAN
#define VD_CG_RSQRT VD_CG_FSQRT
#define VD_CG_RACOS VD_CG_FACOS
#define VD_CG_RPOW  VD_CG_FPOW
#elif VD_CG_DEFAULT_PRECISION_DOUBLE
typedef VdCgf64     VdRea;
typedef VdD2        VdR2;
typedef VdD3        VdR3;
typedef VdD4        VdR4;
typedef VdD2x2      VdR2x2;
typedef VdD3x3      VdR3x3;
typedef VdD4x4      VdR4x4;
typedef VdDQuat     VdRQuat;
typedef VdDLine     VdRLine;
typedef VdDRay      VdRRay;
typedef VdDCylinder VdRCylinder;
#define VD_CG_RSIN  VD_CG_DSIN
#define VD_CG_RCOS  VD_CG_DCOS
#define VD_CG_RTAN  VD_CG_DTAN
#define VD_CG_RSQRT VD_CG_DSQRT
#define VD_CG_RACOS VD_CG_DACOS
#define VD_CG_RPOW  VD_CG_DPOW
#endif // VD_CG_DEFAULT_PRECISION_SINGLE, VD_CG_DEFAULT_PRECISION_DOUBLE

#if VD_MACRO_ABBREVIATIONS
#define rsin       VD_CG_RSIN
#define rcos       VD_CG_RCOS
#define rtan       VD_CG_RTAN
#define rsqrt      VD_CG_RSQRT
#define racos      VD_CG_RACOS
#define rpow       VD_CG_RPOW
#endif // VD_MACRO_ABBREVIATIONS

/* ----INITIALIZATION------------------------------------------------------------------------------------------------ */
VD_CG_INL VdF2          vd_fm2              (VdCgf32 x, VdCgf32 y)                           { VdF2 r; r.x = x; r.y =y; return r; }
VD_CG_INL VdD2          vd_dm2              (VdCgf64 x, VdCgf64 y)                           { VdD2 r; r.x = x; r.y =y; return r; }
VD_CG_INL VdR2          vd_rm2              (VdRea x, VdRea y)                               { VdR2 r; r.x = x; r.y =y; return r; }
VD_CG_INL VdF2          vd_fall2            (VdCgf32 s)                                      { return vd_fm2(s, s);               }
VD_CG_INL VdD2          vd_dall2            (VdCgf64 s)                                      { return vd_dm2(s, s);               }
VD_CG_INL VdR2          vd_rall2            (VdRea s)                                        { return vd_rm2(s, s);               }
VD_CG_INL VdF2          vd_fzero2           (void)                                           { return vd_fall2(0.f);              }
VD_CG_INL VdD2          vd_dzero2           (void)                                           { return vd_dall2(0.f);              }
VD_CG_INL VdR2          vd_rzero2           (void)                                           { return vd_rall2(0.f);              }

VD_CG_INL VdF3          vd_fm3              (VdCgf32 x, VdCgf32 y, VdCgf32 z)                { VdF3 r; r.x = x; r.y = y; r.z = z; return r; }
VD_CG_INL VdD3          vd_dm3              (VdCgf64 x, VdCgf64 y, VdCgf64 z)                { VdD3 r; r.x = x; r.y = y; r.z = z; return r; }
VD_CG_INL VdR3          vd_rm3              (VdRea x, VdRea y, VdRea z)                      { VdR3 r; r.x = x; r.y = y; r.z = z; return r; }
VD_CG_INL VdF3          vd_fall3            (VdCgf32 s)                                      { return vd_fm3(s, s, s);            }
VD_CG_INL VdD3          vd_dall3            (VdCgf64 s)                                      { return vd_dm3(s, s, s);            }
VD_CG_INL VdR3          vd_rall3            (VdRea s)                                        { return vd_rm3(s, s, s);            }
VD_CG_INL VdF3          vd_fzero3           (void)                                           { return vd_fall3(0.f);              }
VD_CG_INL VdD3          vd_dzero3           (void)                                           { return vd_dall3(0.f);              }
VD_CG_INL VdR3          vd_rzero3           (void)                                           { return vd_rall3(0.f);              }

VD_CG_INL VdF4          vd_fm4              (VdCgf32 x, VdCgf32 y, VdCgf32 z, VdCgf32 w)     { VdF4 r; r.x = x; r.y = y; r.z = z; r.w = w; return r; }
VD_CG_INL VdD4          vd_dm4              (VdCgf64 x, VdCgf64 y, VdCgf64 z, VdCgf64 w)     { VdD4 r; r.x = x; r.y = y; r.z = z; r.w = w; return r; }
VD_CG_INL VdR4          vd_rm4              (VdRea x, VdRea y, VdRea z, VdRea w)             { VdR4 r; r.x = x; r.y = y; r.z = z; r.w = w; return r; }
VD_CG_INL VdF4          vd_fall4            (VdCgf32 s)                                      { return vd_fm4(s, s, s, s);         }
VD_CG_INL VdD4          vd_dall4            (VdCgf64 s)                                      { return vd_dm4(s, s, s, s);         }
VD_CG_INL VdR4          vd_rall4            (VdRea s)                                        { return vd_rm4(s, s, s, s);         }
VD_CG_INL VdF4          vd_fzero4           (void)                                           { return vd_fall4(0.f);              }
VD_CG_INL VdD4          vd_dzero4           (void)                                           { return vd_dall4(0.f);              }
VD_CG_INL VdR4          vd_rzero4           (void)                                           { return vd_rall4(0.f);              }

VD_CG_INL VdF4x4        vd_fm4x4            (VdCgf32 a0, VdCgf32 a1, VdCgf32 a2, VdCgf32 a3,
                                                    VdCgf32 b0, VdCgf32 b1, VdCgf32 b2, VdCgf32 b3,
                                                    VdCgf32 c0, VdCgf32 c1, VdCgf32 c2, VdCgf32 c3,
                                                    VdCgf32 d0, VdCgf32 d1, VdCgf32 d2, VdCgf32 d3) { VdF4x4 r; r.a0 = a0; r.a1 = a1; r.a2 = a2; r.a3 = a3; r.b0 = b0; r.b1 = b1; r.b2 = b2; r.b3 = b3;   r.c0 = c0; r.c1 = c1; r.c2 = c2; r.c3 = c3; r.d0 = d0; r.d1 = d1; r.d2 = d2; r.d3 = d3;  return r; }
VD_CG_INL VdD4x4        vd_dm4x4            (VdCgf64 a0, VdCgf64 a1, VdCgf64 a2, VdCgf64 a3,
                                                    VdCgf64 b0, VdCgf64 b1, VdCgf64 b2, VdCgf64 b3,
                                                    VdCgf64 c0, VdCgf64 c1, VdCgf64 c2, VdCgf64 c3,
                                                    VdCgf64 d0, VdCgf64 d1, VdCgf64 d2, VdCgf64 d3) { VdD4x4 r; r.a0 = a0; r.a1 = a1; r.a2 = a2; r.a3 = a3; r.b0 = b0; r.b1 = b1; r.b2 = b2; r.b3 = b3;   r.c0 = c0; r.c1 = c1; r.c2 = c2; r.c3 = c3; r.d0 = d0; r.d1 = d1; r.d2 = d2; r.d3 = d3;  return r; }
VD_CG_INL VdR4x4        vd_rm4x4            (VdRea a0, VdRea a1, VdRea a2, VdRea a3,
                                                    VdRea b0, VdRea b1, VdRea b2, VdRea b3,
                                                    VdRea c0, VdRea c1, VdRea c2, VdRea c3,
                                                    VdRea d0, VdRea d1, VdRea d2, VdRea d3)         { VdR4x4 r; r.a0 = a0; r.a1 = a1; r.a2 = a2; r.a3 = a3; r.b0 = b0; r.b1 = b1; r.b2 = b2; r.b3 = b3;   r.c0 = c0; r.c1 = c1; r.c2 = c2; r.c3 = c3; r.d0 = d0; r.d1 = d1; r.d2 = d2; r.d3 = d3;  return r; }

VD_CG_INL VdF4x4        vd_fall4x4          (VdCgf32 v)                                      { return vd_fm4x4  (v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v);                                                  }
VD_CG_INL VdD4x4        vd_dall4x4          (VdCgf64 v)                                      { return vd_dm4x4  (v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v);                                                  }
VD_CG_INL VdR4x4        vd_rall4x4          (VdRea v)                                        { return vd_rm4x4  (v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v);                                                  }

VD_CG_INL VdFQuat       vd_fmquat           (VdF3 ipart, VdCgf32 rpart)                      { VdFQuat r; r.x = ipart.x; r.y = ipart.y; r.z = ipart.z; r.w = rpart; return r; }
VD_CG_INL VdDQuat       vd_dmquat           (VdD3 ipart, VdCgf64 rpart)                      { VdDQuat r; r.x = ipart.x; r.y = ipart.y; r.z = ipart.z; r.w = rpart; return r; }
VD_CG_INL VdRQuat       vd_rmquat           (VdR3 ipart, VdRea rpart)                        { VdRQuat r; r.x = ipart.x; r.y = ipart.y; r.z = ipart.z; r.w = rpart; return r; }

VD_CG_INL VdFRay        vd_fmray            (VdF3 origin, VdF3 direction)                    { VdFRay r; r.origin = origin; r.direction = direction; return r; }

VD_CG_INL VdFCylinder   vd_fmcylinder       (VdF3 p, VdF3 q, VdCgf32 r)                      { VdFCylinder c; c.p = p; c.q = q; c.r = r; return c; }

/* ----UTILITY------------------------------------------------------------------------------------------------------- */
VD_CG_INL VdCgf32       vd_fwrap_degrees   (VdCgf32 d)                                      { return d - VD_CG_FTAU * VD_CG_FFLOOR(d / VD_CG_FTAU);       }
VD_CG_INL VdCgf32       vd_fclamp          (VdCgf32 min, VdCgf32 x, VdCgf32 max)            { if (x < min) return min; if (x > max) return max; return x; }
VD_CG_INL VdCgf32       vd_flerp           (VdCgf32 a, VdCgf32 b, VdCgf32 t)                { return a + t * (b - a); }
VD_CG_INL VdCgf32       vd_fabs            (VdCgf32 f)                                      { VdCgu32 a = *((VdCgu32*)(&f)); a &= 0x7FFFFFFF; VdCgf32 *fa = ((VdCgf32*)&a); return *fa;}
VD_CG_INL int           vd_feq             (VdCgf32 a, VdCgf32 b)                           { return vd_fabs(b - a) <= VD_CG_FEPSILON; }

/* ----VECTOR ALGEBRA------------------------------------------------------------------------------------------------ */
VD_CG_INL VdF2          vd_fadd2            (VdF2 a, VdF2 b)                                 { return vd_fm2(a.x + b.x, a.y + b.y); }
VD_CG_INL VdD2          vd_dadd2            (VdD2 a, VdD2 b)                                 { return vd_dm2(a.x + b.x, a.y + b.y); }
VD_CG_INL VdR2          vd_radd2            (VdR2 a, VdR2 b)                                 { return vd_rm2(a.x + b.x, a.y + b.y); }

VD_CG_INL VdF3          vd_fadd3            (VdF3 a, VdF3 b)                                 { return vd_fm3(a.x + b.x, a.y + b.y, a.z + b.z); }
VD_CG_INL VdD3          vd_dadd3            (VdD3 a, VdD3 b)                                 { return vd_dm3(a.x + b.x, a.y + b.y, a.z + b.z); }
VD_CG_INL VdR3          vd_radd3            (VdR3 a, VdR3 b)                                 { return vd_rm3(a.x + b.x, a.y + b.y, a.z + b.z); }

VD_CG_INL VdF4          vd_fadd4            (VdF4 a, VdF4 b)                                 { return vd_fm4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
VD_CG_INL VdD4          vd_dadd4            (VdD4 a, VdD4 b)                                 { return vd_dm4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
VD_CG_INL VdR4          vd_radd4            (VdR4 a, VdR4 b)                                 { return vd_rm4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

VD_CG_INL VdF2          vd_fsub2            (VdF2 a, VdF2 b)                                 { return vd_fm2(a.x - b.x, a.y - b.y); }
VD_CG_INL VdD2          vd_dsub2            (VdD2 a, VdD2 b)                                 { return vd_dm2(a.x - b.x, a.y - b.y); }
VD_CG_INL VdR2          vd_rsub2            (VdR2 a, VdR2 b)                                 { return vd_rm2(a.x - b.x, a.y - b.y); }

VD_CG_INL VdF3          vd_fsub3            (VdF3 a, VdF3 b)                                 { return vd_fm3(a.x - b.x, a.y - b.y, a.z - b.z); }
VD_CG_INL VdD3          vd_dsub3            (VdD3 a, VdD3 b)                                 { return vd_dm3(a.x - b.x, a.y - b.y, a.z - b.z); }
VD_CG_INL VdR3          vd_rsub3            (VdR3 a, VdR3 b)                                 { return vd_rm3(a.x - b.x, a.y - b.y, a.z - b.z); }

VD_CG_INL VdF4          vd_fsub4            (VdF4 a, VdF4 b)                                 { return vd_fm4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
VD_CG_INL VdD4          vd_dsub4            (VdD4 a, VdD4 b)                                 { return vd_dm4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
VD_CG_INL VdR4          vd_rsub4            (VdR4 a, VdR4 b)                                 { return vd_rm4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

VD_CG_INL VdF2          vd_fscale2          (VdF2 v, VdCgf32 s)                              { return vd_fm2(v.x * s, v.y * s); }
VD_CG_INL VdD2          vd_dscale2          (VdD2 v, VdCgf64 s)                              { return vd_dm2(v.x * s, v.y * s); }
VD_CG_INL VdR2          vd_rscale2          (VdR2 v, VdRea s)                                { return vd_rm2(v.x * s, v.y * s); }

VD_CG_INL VdF3          vd_fscale3          (VdF3 v, VdCgf32 s)                              { return vd_fm3(v.x * s, v.y * s, v.z * s); }
VD_CG_INL VdD3          vd_dscale3          (VdD3 v, VdCgf64 s)                              { return vd_dm3(v.x * s, v.y * s, v.z * s); }
VD_CG_INL VdR3          vd_rscale3          (VdR3 v, VdRea s)                                { return vd_rm3(v.x * s, v.y * s, v.z * s); }

VD_CG_INL VdF2          vd_fdiv2            (VdF2 v, VdCgf32 s)                              { return vd_fm2(v.x / s, v.y / s); }
VD_CG_INL VdD2          vd_ddiv2            (VdD2 v, VdCgf64 s)                              { return vd_dm2(v.x / s, v.y / s); }
VD_CG_INL VdR2          vd_rdiv2            (VdR2 v, VdRea s)                                { return vd_rm2(v.x / s, v.y / s); }

VD_CG_INL VdF3          vd_fdiv3            (VdF3 v, VdCgf32 s)                              { return vd_fm3(v.x / s, v.y / s, v.z / s); }
VD_CG_INL VdD3          vd_ddiv3            (VdD3 v, VdCgf64 s)                              { return vd_dm3(v.x / s, v.y / s, v.z / s); }
VD_CG_INL VdR3          vd_rdiv3            (VdR3 v, VdRea s)                                { return vd_rm3(v.x / s, v.y / s, v.z / s); }

VD_CG_INL VdF4          vd_fdiv4            (VdF4 v, VdCgf32 s)                              { return vd_fm4(v.x / s, v.y / s, v.z / s, v.w / s); }
VD_CG_INL VdD4          vd_ddiv4            (VdD4 v, VdCgf64 s)                              { return vd_dm4(v.x / s, v.y / s, v.z / s, v.w / s); }
VD_CG_INL VdR4          vd_rdiv4            (VdR4 v, VdRea s)                                { return vd_rm4(v.x / s, v.y / s, v.z / s, v.w / s); }

VD_CG_INL VdCgf32       vd_fdot2            (VdF2 a, VdF2 b)                                 { return a.x * b.x + a.y * b.y; }
VD_CG_INL VdCgf64       vd_ddot2            (VdD2 a, VdD2 b)                                 { return a.x * b.x + a.y * b.y; }
VD_CG_INL VdRea         vd_rdot2            (VdR2 a, VdR2 b)                                 { return a.x * b.x + a.y * b.y; }

VD_CG_INL VdCgf32       vd_fdot3            (VdF3 a, VdF3 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z; }
VD_CG_INL VdCgf64       vd_ddot3            (VdD3 a, VdD3 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z; }
VD_CG_INL VdRea         vd_rdot3            (VdR3 a, VdR3 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z; }

VD_CG_INL VdCgf32       vd_fdot4            (VdF4 a, VdF4 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
VD_CG_INL VdCgf64       vd_ddot4            (VdD4 a, VdD4 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
VD_CG_INL VdRea         vd_rdot4            (VdR4 a, VdR4 b)                                 { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

VD_CG_INL VdCgf32       vd_flensq2          (VdF2 v)                                         { return vd_fdot2(v, v); }
VD_CG_INL VdCgf64       vd_dlensq2          (VdD2 v)                                         { return vd_ddot2(v, v); }
VD_CG_INL VdRea         vd_rlensq2          (VdR2 v)                                         { return vd_rdot2(v, v); }

VD_CG_INL VdCgf32       vd_flensq3          (VdF3 v)                                         { return vd_fdot3(v, v); }
VD_CG_INL VdCgf64       vd_dlensq3          (VdD3 v)                                         { return vd_ddot3(v, v); }
VD_CG_INL VdRea         vd_rlensq3          (VdR3 v)                                         { return vd_rdot3(v, v); }

VD_CG_INL VdCgf32       vd_flensq4          (VdF4 v)                                         { return vd_fdot4(v, v); }
VD_CG_INL VdCgf64       vd_dlensq4          (VdD4 v)                                         { return vd_ddot4(v, v); }
VD_CG_INL VdRea         vd_rlensq4          (VdR4 v)                                         { return vd_rdot4(v, v); }

VD_CG_INL VdF2          vd_fnormalize2      (VdF2 v)                                         { VdCgf32 l = vd_flensq2(v); return vd_fdiv2(v, VD_CG_FSQRT(l)); }
VD_CG_INL VdD2          vd_dnormalize2      (VdD2 v)                                         { VdCgf64 l = vd_dlensq2(v); return vd_ddiv2(v, VD_CG_DSQRT(l)); }
VD_CG_INL VdR2          vd_rnormalize2      (VdR2 v)                                         { VdRea l   = vd_rlensq2(v); return vd_rdiv2(v, VD_CG_RSQRT(l)); }

VD_CG_INL VdF3          vd_fnormalize3      (VdF3 v)                                         { VdCgf32 l = vd_flensq3(v); return vd_fdiv3(v, VD_CG_FSQRT(l)); }
VD_CG_INL VdD3          vd_dnormalize3      (VdD3 v)                                         { VdCgf64 l = vd_dlensq3(v); return vd_ddiv3(v, VD_CG_DSQRT(l)); }
VD_CG_INL VdR3          vd_rnormalize3      (VdR3 v)                                         { VdRea l   = vd_rlensq3(v); return vd_rdiv3(v, VD_CG_RSQRT(l)); }

VD_CG_INL VdF4          vd_fnormalize4      (VdF4 v)                                         { VdCgf32 l = vd_flensq4(v); return vd_fdiv4(v, VD_CG_FSQRT(l)); }
VD_CG_INL VdD4          vd_dnormalize4      (VdD4 v)                                         { VdCgf64 l = vd_dlensq4(v); return vd_ddiv4(v, VD_CG_DSQRT(l)); }
VD_CG_INL VdR4          vd_rnormalize4      (VdR4 v)                                         { VdRea l   = vd_rlensq4(v); return vd_rdiv4(v, VD_CG_RSQRT(l)); }

VD_CG_INL VdF2          vd_fnoz2            (VdF2 v)                                         { VdCgf32 l = vd_flensq2(v); if (l == 0.f) return vd_fzero2(); else return vd_fdiv2(v, VD_CG_FSQRT(l)); }
VD_CG_INL VdD2          vd_dnoz2            (VdD2 v)                                         { VdCgf64 l = vd_dlensq2(v); if (l == 0.f) return vd_dzero2(); else return vd_ddiv2(v, VD_CG_DSQRT(l)); }
VD_CG_INL VdR2          vd_rnoz2            (VdR2 v)                                         { VdRea l   = vd_rlensq2(v); if (l == 0.f) return vd_rzero2(); else return vd_rdiv2(v, VD_CG_RSQRT(l)); }
VD_CG_INL VdF3          vd_fnoz3            (VdF3 v)                                         { VdCgf32 l = vd_flensq3(v); if (l == 0.f) return vd_fzero3(); else return vd_fdiv3(v, VD_CG_FSQRT(l)); }
VD_CG_INL VdD3          vd_dnoz3            (VdD3 v)                                         { VdCgf64 l = vd_dlensq3(v); if (l == 0.f) return vd_dzero3(); else return vd_ddiv3(v, VD_CG_DSQRT(l)); }
VD_CG_INL VdR3          vd_rnoz3            (VdR3 v)                                         { VdRea l   = vd_rlensq3(v); if (l == 0.f) return vd_rzero3(); else return vd_rdiv3(v, VD_CG_RSQRT(l)); }
VD_CG_INL VdF4          vd_fnoz4            (VdF4 v)                                         { VdCgf32 l = vd_flensq4(v); if (l == 0.f) return vd_fzero4(); else return vd_fdiv4(v, VD_CG_FSQRT(l)); }
VD_CG_INL VdD4          vd_dnoz4            (VdD4 v)                                         { VdCgf64 l = vd_dlensq4(v); if (l == 0.f) return vd_dzero4(); else return vd_ddiv4(v, VD_CG_DSQRT(l)); }
VD_CG_INL VdR4          vd_rnoz4            (VdR4 v)                                         { VdRea l   = vd_rlensq4(v); if (l == 0.f) return vd_rzero4(); else return vd_rdiv4(v, VD_CG_RSQRT(l)); }

VD_CG_INL VdF3          vd_fcross3          (VdF3 a, VdF3 b)                                 { return vd_fm3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
VD_CG_INL VdD3          vd_dcross3          (VdD3 a, VdD3 b)                                 { return vd_dm3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
VD_CG_INL VdR3          vd_rcross3          (VdR3 a, VdR3 b)                                 { return vd_rm3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

VD_CG_INL int           vd_feq2             (VdF2 a, VdF2 b)                                 { return vd_feq(a.x, b.x) && vd_feq(a.y, b.y); }
VD_CG_INL int           vd_feq3             (VdF3 a, VdF3 b)                                 { return vd_feq(a.x, b.x) && vd_feq(a.y, b.y) && vd_feq(a.z, b.z); }
VD_CG_INL int           vd_feq4             (VdF4 a, VdF4 b)                                 { return vd_feq(a.x, b.x) && vd_feq(a.y, b.y) && vd_feq(a.z, b.z) && vd_feq(a.w, b.w); }

/* ----MATRIX ALGEBRA------------------------------------------------------------------------------------------------ */
VD_CG_INL VdF4x4        vd_ftranspose4x4    (VdF4x4 *m);
VD_CG_INL VdF4x4        vd_fmul4x4          (VdF4x4 *a, VdF4x4 *b);
VD_CG_INL VdF4x4        vd_fmul4x4col       (VdF4x4 *a, VdF4x4 *b);
VD_CG_INL VdF3          vd_fmul4x4_3        (VdF4x4 *m, VdF3    v);
VD_CG_INL VdF4          vd_fmul4x4_4        (VdF4x4 *m, VdF4    v);
VD_CG_INL VdF4x4        vd_finverse4x4      (VdF4x4 *m);


/* ----QUATERNION ALGEBRA-------------------------------------------------------------------------------------------- */
VD_CG_INL VdFQuat       vd_fidentityquat    (void)                                           { return vd_fmquat(vd_fzero3(), 1.f); }
VD_CG_INL VdDQuat       vd_didentityquat    (void)                                           { return vd_dmquat(vd_dzero3(), 1.0); }
VD_CG_INL VdRQuat       vd_ridentityquat    (void)                                           { return vd_rmquat(vd_rzero3(), 1.0); }

VD_CG_INL VdCgf32       vd_flensqquat       (VdFQuat q)                                      { return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w; }
VD_CG_INL VdCgf64       vd_dlensqquat       (VdDQuat q)                                      { return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w; }
VD_CG_INL VdRea         vd_rlensqquat       (VdRQuat q)                                      { return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w; }

VD_CG_INL VdFQuat       vd_fnormalizequat   (VdFQuat q)                                      { VdCgf32 l = vd_flensqquat(q); return vd_fdiv4(q, VD_CG_FSQRT(l)); }
VD_CG_INL VdDQuat       vd_dnormalizequat   (VdDQuat q)                                      { VdCgf64 l = vd_dlensqquat(q); return vd_ddiv4(q, VD_CG_DSQRT(l)); }
VD_CG_INL VdRQuat       vd_rnormalizequat   (VdRQuat q)                                      { VdRea l = vd_rlensqquat(q); return vd_rdiv4(q, VD_CG_RSQRT(l)); }

VD_CG_INL VdFQuat       vd_fnozquat         (VdFQuat q)                                      { VdCgf32 l = vd_flensqquat(q); if (vd_feq(l, 0.f)) return vd_fidentityquat(); else return vd_fdiv4(q, VD_CG_FSQRT(l)); }
VD_CG_INL VdDQuat       vd_dnozquat         (VdDQuat q)                                      { VdCgf64 l = vd_dlensqquat(q); if (l == 0.0) return vd_didentityquat(); else return vd_ddiv4(q, VD_CG_DSQRT(l)); }
VD_CG_INL VdRQuat       vd_rnozquat         (VdRQuat q)                                      { VdRea l = vd_rlensqquat(q); if (l == 0.0) return vd_ridentityquat(); else return vd_rdiv4(q, VD_CG_RSQRT(l)); }

VD_CG_INL VdFQuat       vd_fconquat         (VdFQuat q)                                      { q.x = -q.x; q.y = -q.y; q.z = -q.z; return q; }

VD_CG_INL VdFQuat       vd_faxis_anglequat  (VdF3 axis, VdCgf32 angle)                       { return vd_fmquat(vd_fscale3(axis, VD_CG_FSIN(angle * .5f)), VD_CG_FCOS(angle * .5f)); }
VD_CG_INL VdFQuat       vd_fmulquat         (VdFQuat q1, VdFQuat q2);
VD_CG_INL VdF3          vd_fmulquat_3       (VdFQuat q, VdF3 v);
VD_CG_INL VdF4x4        vd_fto4x4quat       (VdFQuat q);
VD_CG_INL VdFQuat       vd_feulerquat       (VdF3 euler);
VD_CG_INL VdFQuat       vd_flookrotquat     (VdF3 fwd, VdF3 updir);

/* ----CONVERSIONS--------------------------------------------------------------------------------------------------- */
VD_CG_INL VdFRay        vd_fline_to_ray     (VdFLine line)                                   { VdFRay r; r.origin = line.a; r.direction = vd_fnoz3(vd_fsub3(line.b, line.a)); return r; }
VD_CG_INL VdFLine       vd_fray_to_line     (VdFRay *ray, float t)                           { VdFLine r; r.a = ray->origin; r.b = vd_fadd3(ray->origin, vd_fscale3(ray->direction, t)); return r;}

/* ----COORDINATE SYSTEMS-------------------------------------------------------------------------------------------- */
VD_CG_INL VdF4x4        vd_fidentity4x4        (void);
VD_CG_INL VdF4x4        vd_fdx_to_vk4x4        (void);
VD_CG_INL VdF4x4        vd_fperspective4x4     (VdCgf32 fovyrad, VdCgf32 aspect, VdCgf32 pnr, VdCgf32 pfr);
VD_CG_INL VdF4x4        vd_fperspective4x4_vk  (VdCgf32 fovyrad, VdCgf32 aspect, VdCgf32 pnr, VdCgf32 pfr);
VD_CG_INL VdF4x4        vd_flookat4x4          (VdF3 from, VdF3 to, VdF3 up);
VD_CG_INL VdF4x4        vd_ftranslation4x4     (VdF3 v);
VD_CG_INL VdF4x4        vd_ftranslation4x4col  (VdF3 v);
VD_CG_INL VdF4x4        vd_frotation_pitch4x4  (VdCgf32 rad);
VD_CG_INL VdF4x4        vd_frotation_yaw4x4    (VdCgf32 rad);
VD_CG_INL VdF4x4        vd_frotation_roll4x4   (VdCgf32 rad);
VD_CG_INL VdF4x4        vd_frotation_quat4x4   (VdFQuat quat);
VD_CG_INL VdF4x4        vd_frotation_quat4x4col(VdFQuat quat);
VD_CG_INL void          vd_ftranslate4x4       (VdF4x4 *m, VdF3 v);
VD_CG_INL void          vd_ftranslate4x4col    (VdF4x4 *m, VdF3 v);
VD_CG_INL void          vd_frotatequat4x4      (VdF4x4 *m, VdFQuat quat);
VD_CG_INL void          vd_frotatequat4x4col   (VdF4x4 *m, VdFQuat quat);
VD_CG_INL void          vd_frotate_yaw4x4      (VdF4x4 *m, VdCgf32 rad);
VD_CG_INL VdF4x4        vd_fvk_to_dx4x4        (void);

/* ----COLLISION DETECTION------------------------------------------------------------------------------------------- */
VD_CG_INL VdF3          vd_fclosest_point_tri (VdF3 p, VdF3 t0, VdF3 t1, VdF3 t2);

VD_CG_INL int           vd_fline_vs_cylinder  (VdFLine *line, VdFCylinder *cylinder, VdCgf32 *t);

/* ----VECTOR ALGEBRA IMPL------------------------------------------------------------------------------------------- */

/* ----MATRIX ALGEBRA IMPL------------------------------------------------------------------------------------------- */
VD_CG_INL VdF4x4 vd_ftranspose4x4(VdF4x4 *m)
{
    return vd_fm4x4(
        m->a0, m->b0, m->c0, m->d0,
        m->a1, m->b1, m->c1, m->d1,
        m->a2, m->b2, m->c2, m->d2,
        m->a3, m->b3, m->c3, m->d3);
}

VD_CG_INL VdF4x4 vd_fmul4x4(VdF4x4 *a, VdF4x4 *b)
{
    VdF4x4 result;
    // Row 0
    result.e[0][0] = a->e[0][0]*b->e[0][0] + a->e[0][1]*b->e[1][0] + a->e[0][2]*b->e[2][0] + a->e[0][3]*b->e[3][0];
    result.e[0][1] = a->e[0][0]*b->e[0][1] + a->e[0][1]*b->e[1][1] + a->e[0][2]*b->e[2][1] + a->e[0][3]*b->e[3][1];
    result.e[0][2] = a->e[0][0]*b->e[0][2] + a->e[0][1]*b->e[1][2] + a->e[0][2]*b->e[2][2] + a->e[0][3]*b->e[3][2];
    result.e[0][3] = a->e[0][0]*b->e[0][3] + a->e[0][1]*b->e[1][3] + a->e[0][2]*b->e[2][3] + a->e[0][3]*b->e[3][3];
    // Row 1
    result.e[1][0] = a->e[1][0]*b->e[0][0] + a->e[1][1]*b->e[1][0] + a->e[1][2]*b->e[2][0] + a->e[1][3]*b->e[3][0];
    result.e[1][1] = a->e[1][0]*b->e[0][1] + a->e[1][1]*b->e[1][1] + a->e[1][2]*b->e[2][1] + a->e[1][3]*b->e[3][1];
    result.e[1][2] = a->e[1][0]*b->e[0][2] + a->e[1][1]*b->e[1][2] + a->e[1][2]*b->e[2][2] + a->e[1][3]*b->e[3][2];
    result.e[1][3] = a->e[1][0]*b->e[0][3] + a->e[1][1]*b->e[1][3] + a->e[1][2]*b->e[2][3] + a->e[1][3]*b->e[3][3];
    // Row 2
    result.e[2][0] = a->e[2][0]*b->e[0][0] + a->e[2][1]*b->e[1][0] + a->e[2][2]*b->e[2][0] + a->e[2][3]*b->e[3][0];
    result.e[2][1] = a->e[2][0]*b->e[0][1] + a->e[2][1]*b->e[1][1] + a->e[2][2]*b->e[2][1] + a->e[2][3]*b->e[3][1];
    result.e[2][2] = a->e[2][0]*b->e[0][2] + a->e[2][1]*b->e[1][2] + a->e[2][2]*b->e[2][2] + a->e[2][3]*b->e[3][2];
    result.e[2][3] = a->e[2][0]*b->e[0][3] + a->e[2][1]*b->e[1][3] + a->e[2][2]*b->e[2][3] + a->e[2][3]*b->e[3][3];
    // Row 3
    result.e[3][0] = a->e[3][0]*b->e[0][0] + a->e[3][1]*b->e[1][0] + a->e[3][2]*b->e[2][0] + a->e[3][3]*b->e[3][0];
    result.e[3][1] = a->e[3][0]*b->e[0][1] + a->e[3][1]*b->e[1][1] + a->e[3][2]*b->e[2][1] + a->e[3][3]*b->e[3][1];
    result.e[3][2] = a->e[3][0]*b->e[0][2] + a->e[3][1]*b->e[1][2] + a->e[3][2]*b->e[2][2] + a->e[3][3]*b->e[3][2];
    result.e[3][3] = a->e[3][0]*b->e[0][3] + a->e[3][1]*b->e[1][3] + a->e[3][2]*b->e[2][3] + a->e[3][3]*b->e[3][3];

    return result;
}

VD_CG_INL VdF4x4 vd_fmul4x4col(VdF4x4 *a, VdF4x4 *b)
{
    VdF4x4 result;
    // Row 0
    result.e[0][0] = a->e[0][0]*b->e[0][0] + a->e[1][0]*b->e[0][1] + a->e[2][0]*b->e[0][2] + a->e[3][0]*b->e[0][3];
    result.e[1][0] = a->e[0][0]*b->e[1][0] + a->e[1][0]*b->e[1][1] + a->e[2][0]*b->e[1][2] + a->e[3][0]*b->e[1][3];
    result.e[2][0] = a->e[0][0]*b->e[2][0] + a->e[1][0]*b->e[2][1] + a->e[2][0]*b->e[2][2] + a->e[3][0]*b->e[2][3];
    result.e[3][0] = a->e[0][0]*b->e[3][0] + a->e[1][0]*b->e[3][1] + a->e[2][0]*b->e[3][2] + a->e[3][0]*b->e[3][3];
    // Row 1
    result.e[0][1] = a->e[0][1]*b->e[0][0] + a->e[1][1]*b->e[0][1] + a->e[2][1]*b->e[0][2] + a->e[3][1]*b->e[0][3];
    result.e[1][1] = a->e[0][1]*b->e[1][0] + a->e[1][1]*b->e[1][1] + a->e[2][1]*b->e[1][2] + a->e[3][1]*b->e[1][3];
    result.e[2][1] = a->e[0][1]*b->e[2][0] + a->e[1][1]*b->e[2][1] + a->e[2][1]*b->e[2][2] + a->e[3][1]*b->e[2][3];
    result.e[3][1] = a->e[0][1]*b->e[3][0] + a->e[1][1]*b->e[3][1] + a->e[2][1]*b->e[3][2] + a->e[3][1]*b->e[3][3];
    // Row 2
    result.e[0][2] = a->e[0][2]*b->e[0][0] + a->e[1][2]*b->e[0][1] + a->e[2][2]*b->e[0][2] + a->e[3][2]*b->e[0][3];
    result.e[1][2] = a->e[0][2]*b->e[1][0] + a->e[1][2]*b->e[1][1] + a->e[2][2]*b->e[1][2] + a->e[3][2]*b->e[1][3];
    result.e[2][2] = a->e[0][2]*b->e[2][0] + a->e[1][2]*b->e[2][1] + a->e[2][2]*b->e[2][2] + a->e[3][2]*b->e[2][3];
    result.e[3][2] = a->e[0][2]*b->e[3][0] + a->e[1][2]*b->e[3][1] + a->e[2][2]*b->e[3][2] + a->e[3][2]*b->e[3][3];
    // Row 3
    result.e[0][3] = a->e[0][3]*b->e[0][0] + a->e[1][3]*b->e[0][1] + a->e[2][3]*b->e[0][2] + a->e[3][3]*b->e[0][3];
    result.e[1][3] = a->e[0][3]*b->e[1][0] + a->e[1][3]*b->e[1][1] + a->e[2][3]*b->e[1][2] + a->e[3][3]*b->e[1][3];
    result.e[2][3] = a->e[0][3]*b->e[2][0] + a->e[1][3]*b->e[2][1] + a->e[2][3]*b->e[2][2] + a->e[3][3]*b->e[2][3];
    result.e[3][3] = a->e[0][3]*b->e[3][0] + a->e[1][3]*b->e[3][1] + a->e[2][3]*b->e[3][2] + a->e[3][3]*b->e[3][3];

    return result;
}

VD_CG_INL VdF3 vd_fmul4x4_3(VdF4x4 *m, VdF3 v)
{
    VdF4 f4;
    f4.xyz = v;
    f4.w = 1.f;

    VdF4 result = vd_fmul4x4_4(m, f4);
    return result.xyz;
}

VD_CG_INL VdF4 vd_fmul4x4_4(VdF4x4 *m, VdF4 v)
{
    VdF4 r0 = vd_fm4(m->a0, m->b0, m->c0, m->d0);
    VdF4 r1 = vd_fm4(m->a1, m->b1, m->c1, m->d1);
    VdF4 r2 = vd_fm4(m->a2, m->b2, m->c2, m->d2);
    VdF4 r3 = vd_fm4(m->a3, m->b3, m->c3, m->d3);

    VdF4 result;
    result.x = vd_fdot4(r0, v);
    result.y = vd_fdot4(r1, v);
    result.z = vd_fdot4(r2, v);
    result.w = vd_fdot4(r3, v);
    return result;
}

VD_CG_INL VdF4x4 vd_finverse4x4(VdF4x4 *m)
{
    int i;
    VdF4x4 minors;
    VdCgf32 D = 0.f;
#define _TMP_MAT3_DET(a, b, c, d, e, f, g, h, i) (a*(e*i - f*h) - b*(d*i - f*g) + c*(d*h - e*g))
    minors.a0 = _TMP_MAT3_DET(m->b1, m->b2, m->b3, m->c1, m->c2, m->c3, m->d1, m->d2, m->d3);
    minors.a1 = _TMP_MAT3_DET(m->b0, m->b2, m->b3, m->c0, m->c2, m->c3, m->d0, m->d2, m->d3);
    minors.a2 = _TMP_MAT3_DET(m->b0, m->b1, m->b3, m->c0, m->c1, m->c3, m->d0, m->d1, m->d3);
    minors.a3 = _TMP_MAT3_DET(m->b0, m->b1, m->b2, m->c0, m->c1, m->c2, m->d0, m->d1, m->d2);
    minors.b0 = _TMP_MAT3_DET(m->a1, m->a2, m->a3, m->c1, m->c2, m->c3, m->d1, m->d2, m->d3);
    minors.b1 = _TMP_MAT3_DET(m->a0, m->a2, m->a3, m->c0, m->c2, m->c3, m->d0, m->d2, m->d3);
    minors.b2 = _TMP_MAT3_DET(m->a0, m->a1, m->a3, m->c0, m->c1, m->c3, m->d0, m->d1, m->d3);
    minors.b3 = _TMP_MAT3_DET(m->a0, m->a1, m->a2, m->c0, m->c1, m->c2, m->d0, m->d1, m->d2);
    minors.c0 = _TMP_MAT3_DET(m->a1, m->a2, m->a3, m->b1, m->b2, m->b3, m->d1, m->d2, m->d3);
    minors.c1 = _TMP_MAT3_DET(m->a0, m->a2, m->a3, m->b0, m->b2, m->b3, m->d0, m->d2, m->d3);
    minors.c2 = _TMP_MAT3_DET(m->a0, m->a1, m->a3, m->b0, m->b1, m->b3, m->d0, m->d1, m->d3);
    minors.c3 = _TMP_MAT3_DET(m->a0, m->a1, m->a2, m->b0, m->b1, m->b2, m->d0, m->d1, m->d2);
    minors.d0 = _TMP_MAT3_DET(m->a1, m->a2, m->a3, m->b1, m->b2, m->b3, m->c1, m->c2, m->c3);
    minors.d1 = _TMP_MAT3_DET(m->a0, m->a2, m->a3, m->b0, m->b2, m->b3, m->c0, m->c2, m->c3);
    minors.d2 = _TMP_MAT3_DET(m->a0, m->a1, m->a3, m->b0, m->b1, m->b3, m->c0, m->c1, m->c3);
    minors.d3 = _TMP_MAT3_DET(m->a0, m->a1, m->a2, m->b0, m->b1, m->b2, m->c0, m->c1, m->c2);
#undef _TMP_MAT3_DET
    D = minors.a0 * m->a0 - minors.a1 * m->a1 + minors.a2 * m->a2 - minors.a3 * m->a3;

    if (D == 0.f) return vd_fidentity4x4();

    minors.a0 *= +1.f; minors.a1 *= -1.f; minors.a2 *= +1.f; minors.a3 *= -1.f;
    minors.b0 *= -1.f; minors.b1 *= +1.f; minors.b2 *= -1.f; minors.b3 *= +1.f;
    minors.c0 *= +1.f; minors.c1 *= -1.f; minors.c2 *= +1.f; minors.c3 *= -1.f;
    minors.d0 *= -1.f; minors.d1 *= +1.f; minors.d2 *= -1.f; minors.d3 *= +1.f;

    minors = vd_ftranspose4x4(&minors);
    for (i = 0; i < 16; i += 1) {
        minors.p[i] *= 1.f/D;
    }

    return minors;
}

/* ----QUATERNION ALGEBRA IMPL--------------------------------------------------------------------------------------- */
VD_CG_INL VdFQuat vd_fmulquat(VdFQuat q1, VdFQuat q2)
{
    VdFQuat result;
    result.x =  q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;
    result.y = -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
    result.z =  q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z;
    result.w = -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w;
    return result;
}

VD_CG_INL VdF3 vd_fmulquat_3(VdFQuat q, VdF3 v)
{
#if 0
    VdF3  u = q.xyz;
    VdCgf32 s = q.w;

    VdF3 result = vd_fscale3(u, 2.f * vd_fdot3(u, v));
    result        = vd_fadd3(result, vd_fscale3(v, s * s - vd_fdot3(u, u)));
    result        = vd_fadd3(result, vd_fscale3(vd_fcross3(u, v), 2.f * s));
#else
    VdFQuat u = vd_fmquat(v, 0.f);
    u = vd_fmulquat(q, u);
    u = vd_fmulquat(u, vd_fconquat(q));
    VdF3 result = u.xyz;
#endif
    return result;
}

VD_CG_INL VdF4x4 vd_fto4x4quat(VdFQuat q)
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

VD_CG_INL VdFQuat vd_feulerquat(VdF3 euler)
{
    VdCgf32 cy = VD_CG_FCOS(euler.y * 0.5f); // yaw
    VdCgf32 sy = VD_CG_FSIN(euler.y * 0.5f);
    VdCgf32 cp = VD_CG_FCOS(euler.x * 0.5f); // pitch
    VdCgf32 sp = VD_CG_FSIN(euler.x * 0.5f);
    VdCgf32 cr = VD_CG_FCOS(euler.z * 0.5f); // roll
    VdCgf32 sr = VD_CG_FSIN(euler.z * 0.5f);

    VdFQuat q;
    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * sp * cr + sy * cp * sr;
    q.y = sy * cp * cr - cy * sp * sr;
    q.z = cy * cp * sr - sy * sp * cr;
    return q;
}

VD_CG_INL VdFQuat vd_flookrotquat(VdF3 fwd, VdF3 updir)
{
    VdF3 basis_z = fwd;
    VdF3 basis_x = vd_fnoz3(vd_fcross3(updir, fwd));
    VdF3 basis_y = vd_fcross3(fwd, basis_x);

    VdCgf32 m00 = basis_x.x;
    VdCgf32 m01 = basis_y.x;
    VdCgf32 m02 = basis_z.x;

    VdCgf32 m10 = basis_x.y;
    VdCgf32 m11 = basis_y.y;
    VdCgf32 m12 = basis_z.y;

    VdCgf32 m20 = basis_x.z;
    VdCgf32 m21 = basis_y.z;
    VdCgf32 m22 = basis_z.z;

    VdFQuat q;
    float T = m00 + m11 + m22;
    if (T > 0.f) {
        float s = VD_CG_FSQRT(T + 1.f) * 2.f;

        q.w = 0.25f * s;
        q.x = (m21 - m12) / s;
        q.y = (m02 - m20) / s;
        q.z = (m10 - m01) / s;
    } else if ((m00 > m11) && (m00 > m22)) {
        float s = VD_CG_FSQRT(1.f + m00 - m11 - m22) * 2.f;
        q.w = (m21 - m12) / s;
        q.x = 0.25f * s;
        q.y = (m01 + m10) / s;
        q.z = (m02 + m20) / s;
    } else if (m11 > m22) {
        float s = VD_CG_FSQRT(1.f + m11 - m00 - m22) * 2.f;
        q.w = (m02 - m20) / s;
        q.x = (m01 + m10) / s;
        q.y = 0.25f * s;
        q.z = (m12 + m21) / s;
    } else {
        float s = VD_CG_FSQRT(1.f + m22 - m00 - m11) * 2.f;
        q.w = (m10 - m01) / s;
        q.x = (m02 - m20) / s;
        q.y = (m12 + m21) / s;
        q.z = 0.25f * s;
    }

    return q;
}

/* ----COORDINATE SYSTEMS IMPL--------------------------------------------------------------------------------------- */
VD_CG_INL VdF4x4 vd_fidentity4x4(void)
{
    return vd_fm4x4(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}

VD_CG_INL VdF4x4 vd_fdx_to_vk4x4(void)
{
    return vd_fm4x4(
       -1.f, 0.f, 0.f, 0.f,
        0.f,-1.f, 0.f, 0.f,
        0.f, 0.f,-1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}

VD_CG_INL VdF4x4 vd_fperspective4x4(VdCgf32 fovyrad, VdCgf32 aspect, VdCgf32 pnr, VdCgf32 pfr)
{
    VdF4x4 result;
    VdCgf32 f;

    f = 1.f / VD_CG_FTAN(fovyrad * 0.5f);

    // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
    result = vd_fm4x4(f / aspect,                       0.f,                  0.f,                        0.f,
                      0.f,                              f,                    0.f,                        0.f,
                      0.f,                              0.f,                  (pnr + pfr) / (pnr - pfr), -1.f,
                      0.f,                              0.f,                  2.f*pnr*pfr/(pnr - pfr),    0.f);

    result.e[2][2] = -result.e[2][2];
    result.e[2][3] = -result.e[2][3];
    return result;
}

VD_CG_INL VdF4x4 vd_fperspective4x4_vk(VdCgf32 fovyrad, VdCgf32 aspect, VdCgf32 pnr, VdCgf32 pfr)
{
    VdCgf32 half_tan_fovy = VD_CG_FTAN(fovyrad * 0.5f);

    return vd_fm4x4(
        1.f / ((half_tan_fovy) * aspect), 0.f,                  0.f,                0.f,
        0.f,                              1.f / half_tan_fovy,  0.f,                0.f,
        0.f,                              0.f,                  pfr / (pfr - pnr), -(pnr * pfr) / (pfr - pnr),
        0.f,                              0.f,                  1.f,                0.f);
}

VD_CG_INL VdF4x4 vd_flookat4x4(VdF3 from, VdF3 to, VdF3 up)
{
    VdF3 fwd = vd_fsub3(from, to);
    fwd = vd_fnoz3(fwd);

    VdF3 right = vd_fcross3(fwd, up);
    right = vd_fnoz3(right);

    VdF3 up2 = vd_fcross3(right, fwd);

    VdF3 xaxis = right;
    VdF3 yaxis = up2;
    VdF3 zaxis = fwd;

    VdF3 meye = from;

    return vd_fm4x4(
         xaxis.x,                yaxis.x,               -zaxis.x,               0.f,
         xaxis.y,                yaxis.y,               -zaxis.y,               0.f,
         xaxis.z,                yaxis.z,               -zaxis.z,               0.f,
        -vd_fdot3(xaxis, meye), -vd_fdot3(yaxis, meye), vd_fdot3(zaxis, meye),  1.f);
}

VD_CG_INL VdF4x4 vd_ftranslation4x4(VdF3 v)
{
    return vd_fm4x4(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        v.x, v.y, v.z, 1.f);
}

VD_CG_INL VdF4x4 vd_ftranslation4x4col(VdF3 v)
{
    return vd_fm4x4(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        v.x, v.y, v.z, 1.f);
}

VD_CG_INL VdF4x4 vd_frotation_pitch4x4(VdCgf32 rad)
{
    return vd_fm4x4(
        1.f,              0.f,             0.f,                 0.f,
        0.f,              VD_CG_FCOS(rad), -VD_CG_FSIN(rad),    0.f,
        0.f,              VD_CG_FSIN(rad), VD_CG_FCOS(rad),     0.f,
        0.f,              0.f,             0.f,                 1.f);
}

VD_CG_INL VdF4x4 vd_frotation_yaw4x4(VdCgf32 rad)
{
    return vd_fm4x4(
        VD_CG_FCOS(rad),  0.f, VD_CG_FSIN(rad),  0.f,
        0.f,              1.f, 0.f,              0.f,
        -VD_CG_FSIN(rad), 0.f, VD_CG_FCOS(rad),  0.f,
        0.f,              0.f, 0.f,              1.f);
}

VD_CG_INL VdF4x4 vd_frotation_roll4x4(VdCgf32 rad)
{
    return vd_fm4x4(
        VD_CG_FCOS(rad),  -VD_CG_FSIN(rad), 0.f, 0.f,
        -VD_CG_FSIN(rad), VD_CG_FCOS(rad),  0.f, 0.f,
        0.f,              0.f,              1.f, 0.f,
        0.f,              0.f,              0.f, 1.f);
}

VD_CG_INL VdF4x4 vd_frotation_quat4x4(VdFQuat quat)
{
    VdCgf32 q0   = quat.w;  VdCgf32 q1   = quat.x;  VdCgf32 q2   = quat.y;  VdCgf32 q3   = quat.z;
    VdCgf32 q0q0 = q0 * q0; VdCgf32 q1q1 = q1 * q1; VdCgf32 q2q2 = q2 * q2; VdCgf32 q3q3 = q3 * q3;
    VdCgf32 q0q1 = q0 * q1; VdCgf32 q0q2 = q0 * q2; VdCgf32 q0q3 = q0 * q3;
    VdCgf32 q1q2 = q1 * q2; VdCgf32 q1q3 = q1 * q3;
    VdCgf32 q2q3 = q2 * q3;

    return vd_fm4x4(
        2.f * (q0q0 + q1q1) - 1.f, 2.f * (q1q2 - q0q3),       2.f * (q1q3 + q0q2),       0.f,
        2.f * (q1q2 + q0q3),       2.f * (q0q0 + q2q2) - 1.f, 2.f * (q1q3 + q0q1),       0.f,
        2.f * (q1q3 - q0q2),       2.f * (q2q3 + q0q1),       2.f * (q0q0 + q3q3) - 1.f, 0.f,
        0.f,                       0.f,                       0.f,                       1.f);
}

VD_CG_INL VdF4x4 vd_frotation_quat4x4col(VdFQuat quat)
{
    VdCgf32 q0   = quat.w;  VdCgf32 q1   = quat.x;  VdCgf32 q2   = quat.y;  VdCgf32 q3   = quat.z;
    VdCgf32 q0q0 = q0 * q0; VdCgf32 q1q1 = q1 * q1; VdCgf32 q2q2 = q2 * q2; VdCgf32 q3q3 = q3 * q3;
    VdCgf32 q0q1 = q0 * q1; VdCgf32 q0q2 = q0 * q2; VdCgf32 q0q3 = q0 * q3;
    VdCgf32 q1q2 = q1 * q2; VdCgf32 q1q3 = q1 * q3;
    VdCgf32 q2q3 = q2 * q3;

    return vd_fm4x4(
        2.f * (q0q0 + q1q1) - 1.f, 2.f * (q1q2 + q0q3),       2.f * (q1q3 - q0q2),       0.f,
        2.f * (q1q2 - q0q3),       2.f * (q0q0 + q2q2) - 1.f, 2.f * (q2q3 + q0q1),       0.f,
        2.f * (q1q3 + q0q2),       2.f * (q1q3 + q0q1),       2.f * (q0q0 + q3q3) - 1.f, 0.f,
        0.f,                       0.f,                       0.f,                       1.f);
}

VD_CG_INL void vd_ftranslate4x4(VdF4x4 *m, VdF3 v)
{
    VdF4x4 t = vd_ftranslation4x4(v);
    VdF4x4 r = vd_fmul4x4(m, &t);
    *m = r;
}

VD_CG_INL void vd_ftranslate4x4col(VdF4x4 *m, VdF3 v)
{
    VdF4x4 t = vd_ftranslation4x4col(v);
    VdF4x4 r = vd_fmul4x4(m, &t);
    *m = r;
}

VD_CG_INL void vd_frotatequat4x4(VdF4x4 *m, VdFQuat quat)
{
    VdF4x4 mq = vd_frotation_quat4x4(quat);
    VdF4x4 r  = vd_fmul4x4(m, &mq);
    *m = r;
}

VD_CG_INL void vd_frotatequat4x4col(VdF4x4 *m, VdFQuat quat)
{
    VdF4x4 mq = vd_frotation_quat4x4col(quat);
    VdF4x4 r  = vd_fmul4x4(m, &mq);
    *m = r;
}

VD_CG_INL void vd_frotate_yaw4x4(VdF4x4 *m, VdCgf32 rad)
{
    VdF4x4 t = vd_frotation_yaw4x4(rad);
    VdF4x4 r = vd_fmul4x4(m, &t);
    *m = r;
}

VD_CG_INL VdF4x4 vd_fvk_to_dx4x4(void)
{
    return vd_fm4x4(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f,-1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}

/* ----COLLISION DETECTION------------------------------------------------------------------------------------------- */
VD_CG_INL VdF3 vd_fclosest_point_tri(VdF3 p, VdF3 t0, VdF3 t1, VdF3 t2)
{
    VdF3 a = t0;
    VdF3 b = t1;
    VdF3 c = t2;

    // Check if P is in vertex regions outside A
    VdF3 ab  = vd_fsub3(b, a);
    VdF3 ac  = vd_fsub3(c, a);
    VdF3 ap  = vd_fsub3(p, a);
    float d1 = vd_fdot3(ab, ap);
    float d2 = vd_fdot3(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f) return a;

    // Check vertex region outside B
    VdF3 bp  = vd_fsub3(p, b);
    float d3 = vd_fdot3(ab, bp);
    float d4 = vd_fdot3(ac, bp);
    if (d3 >= 0.0f && d4 <= d3) return b;

    // Check edge region of AB
    float vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        float v = d1 / (d1 - d3);
        return vd_fadd3(a, vd_fscale3(ab, v));
    }

    // Check vertex region outside C
    VdF3 cp  = vd_fsub3(p, c);
    float d5 = vd_fdot3(ab, cp);
    float d6 = vd_fdot3(ac, cp);
    if (d6 >= 0.0f && d5 <= d6) return c;

    // Check edge region of AC
    float vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        float w = d2 / (d2 - d6);
        return vd_fadd3(a, vd_fscale3(ac, w));
    }

    // Check edge region of BC
    float va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return vd_fadd3(b, vd_fscale3(vd_fsub3(c, b), w));
    }

    // Inside face region
    float denom = 1.0f / (va + vb + vc);
    float v = vb * denom;
    float w = vc * denom;
    VdF3 result = vd_fadd3(a, vd_fscale3(ab, v));
    result = vd_fadd3(result, vd_fscale3(ac, w));
    return result;
}

VD_CG_INL int vd_fline_vs_cylinder(VdFLine *line, VdFCylinder *cylinder, VdCgf32 *t)
{
    VdF3 d = vd_fsub3(cylinder->q, cylinder->p);
    VdF3 m = vd_fsub3(line->a, cylinder->p);
    VdF3 n = vd_fsub3(line->b, line->a);

    VdCgf32 md = vd_fdot3(m, d);
    VdCgf32 nd = vd_fdot3(n, d);
    VdCgf32 dd = vd_fdot3(d, d);

    if ((md < 0.0f) && ((md + nd) < 0.0f)) {
        return 0;
    }

    if ((md > dd) && ((md + nd) > dd)) {
        return 0;
    }

    VdCgf32 nn = vd_fdot3(n, n);
    VdCgf32 mn = vd_fdot3(m, n);
    VdCgf32  a = dd * nn - nd * nd;
    VdCgf32  k = vd_fdot3(m, m) - cylinder->r * cylinder->r;
    VdCgf32  c = dd * k - md * md;

    if (vd_fabs(a) < VD_CG_FEPSILON) {
        if (c > 0.f) {
            return 0;
        }

        if (md < 0.f) {
            *t = -mn / nn;
        } else if (md > dd) {
            *t = (nd - mn) / nn;
        } else {
            *t = 0.f;
        }

        return 1;
    }

    VdCgf32 b = dd * mn - nd * md;
    VdCgf32 discriminant = b * b - a * c;
    if (discriminant < 0.f) {
        return 0;
    }

    *t = (-b - fsqrt(discriminant)) / a;

    if (((*t) < 0.f) || ((*t) > 1.f)) {
        return 0;
    }

    if ((md + (*t) * nd) < 0.f) {
        if (nd <= 0.f) {
            return 0;
        }

        *t = -md / nd;

        return (k + 2.f * (*t) * (mn + (*t) * nn)) <= 0.f;
    } else if ((md + (*t) * nd) > dd) {
        if (nd >= 0.f) {
            return 0;
        }

        *t = (dd - md) / nd;

        return (k + dd - 2.f * md + (*t) * (2.f * (mn - nd) + (*t) * nn)) <= 0.f;
    }

    return 1;
}

#if VD_MACRO_ABBREVIATIONS
#define Rea                      VdRea
#define F2                       VdF2
#define F3                       VdF3
#define F4                       VdF4
#define F2x2                     VdF2x2
#define F3x3                     VdF3x3
#define F4x4                     VdF4x4
#define FQuat                    VdFQuat
#define FLine                    VdFLine
#define FRay                     VdFRay 
#define FCylinder                VdFCylinder
#define D2                       VdD2
#define D3                       VdD3
#define D4                       VdD4
#define D2x2                     VdD2x2
#define D3x3                     VdD3x3
#define D4x4                     VdD4x4
#define DQuat                    VdDQuat
#define DLine                    VdDLine
#define DRay                     VdDRay 
#define DCylinder                VdDCylinder
#define R2                       VdR2
#define R3                       VdR3
#define R4                       VdR4
#define R2x2                     VdR2x2
#define R3x3                     VdR3x3
#define R4x4                     VdR4x4
#define RQuat                    VdRQuat
#define RLine                    VdRLine
#define RRay                     VdRRay
#define RCylinder                VdRCylinder

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
#define fmray                    vd_fmray
#define fmcylinder               vd_fmcylinder

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
#define fmul4x4_3                vd_fmul4x4_3
#define fmul4x4_4                vd_fmul4x4_4
#define fmul4x4col               vd_fmul4x4col

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
#define fmulquat_3               vd_fmulquat_3 
#define fto4x4quat               vd_fto4x4quat 
#define feulerquat               vd_feulerquat 
#define flookrotquat             vd_flookrotquat

/* ----CONVERSIONS--------------------------------------------------------------------------------------------------- */
#define fline_to_ray             vd_fline_to_ray

/* ----COORDINATE SYSTEMS-------------------------------------------------------------------------------------------- */
#define fidentity4x4             vd_fidentity4x4 
#define fdx_to_vk4x4             vd_fdx_to_vk4x4 
#define fperspective4x4          vd_fperspective4x4 
#define fperspective4x4_vk       vd_fperspective4x4_vk 
#define flookat4x4               vd_flookat4x4 
#define ftranslation4x4          vd_ftranslation4x4 
#define ftranslation4x4col       vd_ftranslation4x4col
#define frotation_pitch4x4       vd_frotation_pitch4x4
#define frotation_yaw4x4         vd_frotation_yaw4x4 
#define frotation_roll4x4        vd_frotation_roll4x4
#define frotation_quat4x4        vd_frotation_quat4x4
#define frotation_quat4x4col     vd_frotation_quat4x4col
#define ftranslate4x4            vd_ftranslate4x4 
#define ftranslate4x4col         vd_ftranslate4x4col
#define frotatequat4x4           vd_frotatequat4x4
#define frotatequat4x4col        vd_frotatequat4x4col
#define frotate_yaw4x4           vd_frotate_yaw4x4 
#define fvk_to_dx4x4             vd_fvk_to_dx4x4 

/* ----UTILITY------------------------------------------------------------------------------------------------------- */
#define fwrap_degrees            vd_fwrap_degrees 
#define fclamp                   vd_fclamp 
#define flerp                    vd_flerp

#endif // VD_MACRO_ABBREVIATIONS

#endif // !VD_CG_H

#ifdef VD_CG_IMPL


#ifndef VD_CG_INCLUDE_INTERNAL_TESTS
#   define VD_CG_INCLUDE_INTERNAL_TESTS 0
#endif // !VD_CG_INCLUDE_INTERNAL_TESTS
#if VD_CG_INCLUDE_INTERNAL_TESTS

#ifndef VD_CG_INTERNAL_TESTS_VERBOSE
#   define VD_CG_INTERNAL_TESTS_VERBOSE 0
#endif

static void vd_cg__flog4x4(VdF4x4 *m)
{
#if VD_CG_INTERNAL_TESTS_VERBOSE
    VD_TEST_LOG("| % 4.3f % 4.3f % 4.3f % 4.3f |\n"
                "| % 4.3f % 4.3f % 4.3f % 4.3f |\n"
                "| % 4.3f % 4.3f % 4.3f % 4.3f |\n"
                "| % 4.3f % 4.3f % 4.3f % 4.3f |\n",
                m->c[0].e[0], m->c[1].e[0], m->c[2].e[0], m->c[3].e[0],
                m->c[0].e[1], m->c[1].e[1], m->c[2].e[1], m->c[3].e[1],
                m->c[0].e[2], m->c[1].e[2], m->c[2].e[2], m->c[3].e[2],
                m->c[0].e[3], m->c[1].e[3], m->c[2].e[3], m->c[3].e[3]);
#endif
}

static void vd_cg__flog4(VdF4 *v) {
#if VD_CG_INTERNAL_TESTS_VERBOSE
    VD_TEST_LOG("(%3.3f %3.3f %3.3f %3.3f)", v->x, v->y, v->z, v->w);
#endif
}

static void vd_cg__flog3(VdF3 *v) {
#if VD_CG_INTERNAL_TESTS_VERBOSE
    VD_TEST_LOG("(%3.3f %3.3f %3.3f)", v->x, v->y, v->z);
#endif
}

VD_TEST("fabs") {
    VD_TEST_EQ("-0.f  ==  0.f", vd_fabs(-0.f), 0.f);
    VD_TEST_EQ("+0.f  ==  0.f", vd_fabs(+0.f), 0.f);
    VD_TEST_EQ("-10.f == 10.f", vd_fabs(-10.f), 10.f);
    VD_TEST_EQ("+10.f == 10.f", vd_fabs(+10.f), 10.f);
    VD_TEST_OK();
}

VD_TEST("feq") {
    VD_TEST_TRUE("0.f  ==  0.f", vd_feq(0.f, 0.f));
    VD_TEST_TRUE("-0.f == +0.f", vd_feq(-0.f, +0.f));
    VD_TEST_TRUE("+1.f == +1.f", vd_feq(+1.f, +1.f));
    VD_TEST_FALSE("-1.f != +1.f", vd_feq(-1.f, +1.f));

    VD_TEST_TRUE("(0,0,1) == (0,0,1)", vd_feq3(vd_fm3(0,0,1), vd_fm3(0,0,1)));
    VD_TEST_TRUE("(1,1,1) == (1,1,1)", vd_feq3(vd_fall3(1), vd_fall3(1)));

    VD_TEST_TRUE("(0,0,1,1) == (0,0,1,1)", vd_feq4(vd_fm4(0,0,1,1), vd_fm4(0,0,1,1)));
    VD_TEST_OK();
}

VD_TEST("Affine Transform/Matrix Translation") {
    VdF4x4 m1 = vd_ftranslation4x4(vd_fm3(1,2,3));
    VdF4x4 m2 = vd_ftranslation4x4(vd_fm3(4,5,6));

    vd_cg__flog4x4(&m1);
    vd_cg__flog4x4(&m2);

    VdF4x4 m = vd_fmul4x4(&m1, &m2);
    vd_cg__flog4x4(&m);
    VD_TEST_TRUE("Result Matrix Transformed", vd_feq4(m.c[3], vd_fm4(5,7,9,1)));

    VD_TEST_OK();
}

VD_TEST("Affine Transform/Vector Translation") {
    VdF4x4 m = vd_ftranslation4x4(vd_fm3(1,2,3));
    vd_cg__flog4x4(&m);
    vd_cg__flog4(&m.c[3]);

    VdF4 v = vd_fm4(1,2,3,1);
    VdF3 tv3 = vd_fmul4x4_3(&m, v.xyz);
    VdF4 tv  = vd_fmul4x4_4(&m, v);

    vd_cg__flog4(&tv);
    VD_TEST_TRUE("Vectors Match", vd_feq4(tv, vd_fm4(2,4,6,1)));
    VD_TEST_TRUE("Vectors Match", vd_feq3(tv3, vd_fm3(2,4,6)));
    VD_TEST_OK();
}

VD_TEST("Affine Transform/Vector Rotation (Y/YAW)") {
    VdF4x4 m = vd_frotation_yaw4x4(VD_FDEG2RAD(90.f));
    vd_cg__flog4x4(&m);

    VdF4 v = vd_fm4(1,0,1,1);

    VdF4 tv = vd_fmul4x4_4(&m, v);
    vd_cg__flog4(&tv);

    VD_TEST_TRUE("Vector rotated on XZ Plane with Math Winding of 90deg", vd_feq4(tv, vd_fm4(-1,0,1,1)));
    VD_TEST_OK();
}

VD_TEST("Affine Transform/Matrix Rotation (Y/YAW)") {
    VdF4x4 m = vd_frotation_yaw4x4(VD_FDEG2RAD(90.f));
    vd_cg__flog4x4(&m);

    VdF4x4 c = vd_fidentity4x4();
    c.c[3].xyz = vd_fm3(5,5,5);

    VdF4x4 tm = vd_fmul4x4(&m, &c);
    vd_cg__flog4x4(&tm);
    VD_TEST_OK();
}

VD_TEST("Perspective Projection") {
    VdF4x4 projection = vd_fperspective4x4(VD_FDEG2RAD(60.f), 1.f, 0.01f, 10.f);

    vd_cg__flog4x4(&projection);

    VdF4 v = vd_fm4(0,0,10,1);
    VdF4 p = vd_fmul4x4_4(&projection, v);
    VdF4 c = vd_fdiv4(p, p.w);

    vd_cg__flog4(&p);
    vd_cg__flog4(&c);
    VdF4 t = vd_fm4(0,0,1,1);
    vd_cg__flog4(&t);

    VD_TEST_TRUE("Point is Projected Correctly", vd_feq4(c, t));

    VD_TEST_OK();
}

VD_TEST("Look At Matrix") {
    VdF4x4 lookat = vd_flookat4x4(vd_fm3(0,0,-10), vd_fm3(0,0,1), vd_fm3(0,1,0));
    vd_cg__flog4x4(&lookat);
    VD_TEST_OK();
}

VD_TEST("Quaternion - Vector Multiplication") {
    VdFQuat q = vd_fidentityquat();
    VdF3 v = vd_fmulquat_3(q, vd_fm3(0,0,1));
    vd_cg__flog3(&v);
    VD_TEST_OK();
}

#endif // VD_CG_INCLUDE_INTERNAL_TESTS
#endif // VD_CG_IMPL