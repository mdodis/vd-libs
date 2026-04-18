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
 *
 * Note that this library does not do namespacing anymore, because you'd either include it and use or not include it
 */
#ifndef VD_CG_H
#define VD_CG_H
#define VD_CG_VERSION_MAJOR 0
#define VD_CG_VERSION_MINOR 1
#define VD_CG_VERSION_PATCH 0
#define VD_CG_VERSION       ((VD_CG_VERSION_MAJOR << 16) | (VD_CG_VERSION_MINOR << 8) | (VD_CG_VERSION_PATCH))

#include <stdint.h>
#include <math.h>

typedef float       F1;
typedef double      D1;
typedef int32_t     S1;

#define F_PI        3.14159265359f
#define D_PI        3.141592653589793115997963468544185161590576171875

#define F_EPSILON   1.19209290E-07F
#define D_EPSILON   2.22e-16

#define F_TAU       (2*F_PI)
#define D_TAU       (2*D_PI)

#define F_DEG_TAU   360.f
#define D_DEG_TAU   360.0

#define fdeg2rad(degrees) ((degrees) * (F_PI/180.0f))
#define frad2deg(radians) ((radians) * (180.0f/F_PI))

#define ddeg2rad(degrees) ((degrees) * (D_PI/180.0))
#define drad2deg(radians) ((radians) * (180.0/D_PI))

#ifndef VD_CG_INL
#   ifdef VD_H
#       define VD_CG_INL VD_INLINE
#   else
#       define VD_CG_INL static inline
#   endif
#endif

#define fsin      sinf
#define dsin      sin
#define fcos      cosf
#define dcos      cos
#define ftan      tanf
#define dtan      tan
#define fsqrt     sqrtf
#define dsqrt     sqrt
#define facos     acosf
#define dacos     acos
#define ffloor    floorf
#define dfloor    floor
#define fpow      powf
#define dpow      pow

#define fmin fminf
#define dmin fmin
#define fmax fmaxf
#define dmax fmax
#define fabs fabsf
#define dabs fabs

#pragma pack(push, 1)
typedef union __VD_CG_f2 { F1 e[2]; struct { F1 x, y; }; struct { F1 r, g; }; } F2;
typedef union __VD_CG_d2 { D1 e[2]; struct { D1 x, y; }; struct { D1 r, g; }; } D2;
typedef union __VD_CG_s2 { S1 e[2]; struct { S1 x, y; }; struct { S1 r, g; }; } S2;
typedef union __VD_CG_f3 { F1 e[3]; struct { F1 x, y, z; }; struct { F2 xy; F1 __ignore0; }; struct { F1 __ignore2; F2 yz; }; } F3;
typedef union __VD_CG_d3 { D1 e[3]; struct { D1 x, y, z; }; struct { D2 xy; D1 __ignore0; }; struct { D1 __ignore2; D2 yz; }; } D3;
typedef union __VD_CG_s3 { S1 e[3]; struct { S1 x, y, z; }; struct { S2 xy; S1 __ignore0; }; struct { S1 __ignore2; S2 yz; }; } S3;
typedef union __VD_CG_f4 { F1 e[4]; struct { F1 x, y, z, w; }; struct { F2 xy; F2 zw; }; struct { F1 __ignore3; F2 yz; F1 __ignore2; }; struct { F3 xyz; F1 __ignore0; }; struct { F1 __ignore1; F3 yzw; }; } F4;
typedef union __VD_CG_d4 { D1 e[4]; struct { D1 x, y, z, w; }; struct { D2 xy; D2 zw; }; struct { D1 __ignore3; D2 yz; D1 __ignore2; }; struct { D3 xyz; D1 __ignore0; }; struct { D1 __ignore1; D3 yzw; }; } D4;
typedef union __VD_CG_s4 { S1 e[4]; struct { S1 x, y, z, w; }; struct { S2 xy; S2 zw; }; struct { S1 __ignore3; S2 yz; S1 __ignore2; }; struct { S3 xyz; S1 __ignore0; }; struct { S1 __ignore1; S3 yzw; }; } S4;

typedef union __VD_CG_f2x2 { F1 e[2][2]; F1 p[4]; F2 c[2]; struct { F1 a0, a1, b0, b1; }; } F2x2;
typedef union __VD_CG_d2x2 { D1 e[2][2]; D1 p[4]; D2 c[2]; struct { D1 a0, a1, b0, b1; }; } D2x2;
typedef union __VD_CG_f3x3 { F1 e[3][3]; F1 p[9]; F3 c[3]; struct { F1 a0, a1, a2, b0, b1, b2, c0, c1, c2; }; } F3x3;
typedef union __VD_CG_d3x3 { D1 e[3][3]; D1 p[9]; D3 c[3]; struct { D1 a0, a1, a2, b0, b1, b2, c0, c1, c2; }; } D3x3;
typedef union __VD_CG_f4x4 { F1 e[4][4]; F1 p[16]; F4 c[4]; struct { F1 a0, a1, a2, a3, b0, b1, b2, b3, c0, c1, c2, c3, d0, d1, d2, d3; }; } F4x4;
typedef union __VD_CG_d4x4 { D1 e[4][4]; D1 p[16]; D4 c[4]; struct { D1 a0, a1, a2, a3, b0, b1, b2, b3, c0, c1, c2, c3, d0, d1, d2, d3; }; } D4x4;

typedef union __VD_CG_f4 FQuat;
typedef union __VD_CG_d4 DQuat;

typedef struct __VD_CG_fline { F3 a, b; } FLine;
typedef struct __VD_CG_dline { D3 a, b; } DLine;

// Hessian Form [x,y,z] is unit normal, [w] is signed distance from origin along normal
typedef union __VD_CG_f4 FPlane;
typedef union __VD_CG_d4 DPlane;

typedef struct __VD_CG_fray { F3 origin; F3 direction; } FRay;
typedef struct __VD_CG_dray { D3 origin; D3 direction; } DRay;

typedef struct __VD_CG_fcylinder { F3 p; F3 q; F1 r; } FCylinder;
typedef struct __VD_CG_dcylinder { D3 p; D3 q; D1 r; } DCylinder;

typedef struct __VD_CG_fsphere { F3 c; F1 r; } FSphere;
typedef struct __VD_CG_dsphere { D3 c; D1 r; } DSphere;

#pragma pack(pop)

/* ----INITIALIZATION------------------------------------------------------------------------------------------------ */
VD_CG_INL F2          fm2              (F1 x, F1 y)                            { F2 r; r.x = x; r.y = y; return r; }
VD_CG_INL D2          dm2              (D1 x, D1 y)                            { D2 r; r.x = x; r.y = y; return r; }
VD_CG_INL S2          sm2              (S1 x, S1 y)                            { S2 r; r.x = x; r.y = y; return r; }
VD_CG_INL F2          fall2            (F1 s)                                  { return fm2(s, s); }
VD_CG_INL D2          dall2            (D1 s)                                  { return dm2(s, s); }
VD_CG_INL S2          sall2            (S1 s)                                  { return sm2(s, s); }
VD_CG_INL F2          fzero2           (void)                                  { return fall2(0.f); }
VD_CG_INL D2          dzero2           (void)                                  { return dall2(0.0); }
VD_CG_INL S2          szero2           (void)                                  { return sall2(0);   }
VD_CG_INL F3          fm3              (F1 x, F1 y, F1 z)                      { F3 r; r.x = x; r.y = y; r.z = z; return r; }
VD_CG_INL D3          dm3              (D1 x, D1 y, D1 z)                      { D3 r; r.x = x; r.y = y; r.z = z; return r; }
VD_CG_INL S3          sm3              (S1 x, S1 y, S1 z)                      { S3 r; r.x = x; r.y = y; r.z = z; return r; }
VD_CG_INL F3          fall3            (F1 s)                                  { return fm3(s, s, s); }
VD_CG_INL D3          dall3            (D1 s)                                  { return dm3(s, s, s); }
VD_CG_INL S3          sall3            (S1 s)                                  { return sm3(s, s, s); }
VD_CG_INL F3          fzero3           (void)                                  { return fall3(0.f); }
VD_CG_INL D3          dzero3           (void)                                  { return dall3(0.0); }
VD_CG_INL S3          szero3           (void)                                  { return sall3(0);   }
VD_CG_INL F3          fm3ts            (F2  t, F1 s)                           { F3 r; r.xy = t; r.z  = s; return r; }
VD_CG_INL D3          dm3ts            (D2  t, D1 s)                           { D3 r; r.xy = t; r.z  = s; return r; }
VD_CG_INL S3          sm3ts            (S2  t, S1 s)                           { S3 r; r.xy = t; r.z  = s; return r; }
VD_CG_INL F3          fm3st            (F1 s, F2  t)                           { F3 r; r.x  = s; r.yz = t; return r; }
VD_CG_INL D3          dm3st            (D1 s, D2  t)                           { D3 r; r.x  = s; r.yz = t; return r; }
VD_CG_INL S3          sm3st            (S1 s, S2  t)                           { S3 r; r.x  = s; r.yz = t; return r; }
VD_CG_INL F4          fm4              (F1 x, F1 y, F1 z, F1 w)                { F4 r; r.x = x; r.y = y; r.z = z; r.w = w; return r; }
VD_CG_INL D4          dm4              (D1 x, D1 y, D1 z, D1 w)                { D4 r; r.x = x; r.y = y; r.z = z; r.w = w; return r; }
VD_CG_INL S4          sm4              (S1 x, S1 y, S1 z, S1 w)                { S4 r; r.x = x; r.y = y; r.z = z; r.w = w; return r; }
VD_CG_INL F4          fall4            (F1 s)                                  { return fm4(s, s, s, s); }
VD_CG_INL D4          dall4            (D1 s)                                  { return dm4(s, s, s, s); }
VD_CG_INL S4          sall4            (S1 s)                                  { return sm4(s, s, s, s); }
VD_CG_INL F4          fzero4           (void)                                  { return fall4(0.f); }
VD_CG_INL D4          dzero4           (void)                                  { return dall4(0.0); }
VD_CG_INL S4          szero4           (void)                                  { return sall4(0);   }
VD_CG_INL F4          fm4tt            (F2 t0, F2 t1)                          { return fm4(t0.x, t0.y, t1.x, t1.y); }
VD_CG_INL D4          dm4tt            (D2 t0, D2 t1)                          { return dm4(t0.x, t0.y, t1.x, t1.y); }
VD_CG_INL S4          sm4tt            (S2 t0, S2 t1)                          { return sm4(t0.x, t0.y, t1.x, t1.y); }
VD_CG_INL F4          fm4tss           (F2 t, F1 s2, F1 s3)                    { return fm4(t.x, t.y, s2, s3); }
VD_CG_INL D4          dm4tss           (D2 t, D1 s2, D1 s3)                    { return dm4(t.x, t.y, s2, s3); }
VD_CG_INL S4          sm4tss           (S2 t, S1 s2, S1 s3)                    { return sm4(t.x, t.y, s2, s3); }
VD_CG_INL F4          fm4sst           (F1 s0, F1 s1, F2 t)                    { return fm4(s0, s1, t.x, t.y); }
VD_CG_INL D4          dm4sst           (D1 s0, D1 s1, D2 t)                    { return dm4(s0, s1, t.x, t.y); }
VD_CG_INL S4          sm4sst           (S1 s0, S1 s1, S2 t)                    { return sm4(s0, s1, t.x, t.y); }
VD_CG_INL F4          fm4sts           (F1 s0, F2 t, F1 s3)                    { return fm4(s0, t.x, t.y, s3); }
VD_CG_INL D4          dm4sts           (D1 s0, D2 t, D1 s3)                    { return dm4(s0, t.x, t.y, s3); }
VD_CG_INL S4          sm4sts           (S1 s0, S2 t, S1 s3)                    { return sm4(s0, t.x, t.y, s3); }
VD_CG_INL F4          fm4ps            (F3 p, F1 s3)                           { return fm4(p.x, p.y, p.z, s3); }
VD_CG_INL D4          dm4ps            (D3 p, D1 s3)                           { return dm4(p.x, p.y, p.z, s3); }
VD_CG_INL F4          fm4sp            (F1 s0, F3 p)                           { return fm4(s0, p.x, p.y, p.z); }
VD_CG_INL D4          dm4sp            (D1 s0, D3 p)                           { return dm4(s0, p.x, p.y, p.z); }
VD_CG_INL S4          sm4sp            (S1 s0, S3 p)                           { return sm4(s0, p.x, p.y, p.z); }
VD_CG_INL F4x4        fm4x4            (F1 a0, F1 a1, F1 a2, F1 a3,
                                        F1 b0, F1 b1, F1 b2, F1 b3,
                                        F1 c0, F1 c1, F1 c2, F1 c3,
                                        F1 d0, F1 d1, F1 d2, F1 d3)            { F4x4 r; r.a0 = a0; r.a1 = a1; r.a2 = a2; r.a3 = a3; r.b0 = b0; r.b1 = b1; r.b2 = b2; r.b3 = b3;   r.c0 = c0; r.c1 = c1; r.c2 = c2; r.c3 = c3; r.d0 = d0; r.d1 = d1; r.d2 = d2; r.d3 = d3;  return r; }
VD_CG_INL D4x4        dm4x4            (D1 a0, D1 a1, D1 a2, D1 a3,
                                        D1 b0, D1 b1, D1 b2, D1 b3,
                                        D1 c0, D1 c1, D1 c2, D1 c3,
                                        D1 d0, D1 d1, D1 d2, D1 d3)            { D4x4 r; r.a0 = a0; r.a1 = a1; r.a2 = a2; r.a3 = a3; r.b0 = b0; r.b1 = b1; r.b2 = b2; r.b3 = b3;   r.c0 = c0; r.c1 = c1; r.c2 = c2; r.c3 = c3; r.d0 = d0; r.d1 = d1; r.d2 = d2; r.d3 = d3;  return r; }
VD_CG_INL F4x4        fall4x4          (F1 v)                                  { return fm4x4  (v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v);                                                  }
VD_CG_INL D4x4        dall4x4          (D1 v)                                  { return dm4x4  (v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v);                                                  }
VD_CG_INL F4x4        fm4x4diagonal    (F1 d)                                  { return fm4x4(  d, 0.f, 0.f, 0.f,
                                                                                              0.f,   d, 0.f, 0.f,
                                                                                              0.f, 0.f,   d, 0.f,
                                                                                              0.f, 0.f, 0.f,   d); }
VD_CG_INL D4x4        dm4x4diagonal    (D1 d)                                  { return dm4x4(  d, 0.0, 0.0, 0.0,
                                                                                              0.0,   d, 0.0, 0.0,
                                                                                              0.0, 0.0,   d, 0.0,
                                                                                              0.0, 0.0, 0.0,   d); }
VD_CG_INL F4x4        fidentity4x4     (void)                                  { return fm4x4diagonal(1.f); }
VD_CG_INL D4x4        didentity4x4     (void)                                  { return dm4x4diagonal(1.0); }

VD_CG_INL FQuat       fmquat           (F3 ipart, F1 rpart)                    { return fm4ps(ipart, rpart); }
VD_CG_INL DQuat       dmquat           (D3 ipart, D1 rpart)                    { return dm4ps(ipart, rpart); }

VD_CG_INL FLine       fmline           (F3 a, F3 b)                            { FLine r; r.a = a; r.b = b; return r; }
VD_CG_INL DLine       dmline           (D3 a, D3 b)                            { DLine r; r.a = a; r.b = b; return r; }

VD_CG_INL FPlane      fhessianplane    (F3 normal, F1 distance)                { FPlane r; r.xyz = normal; r.w = distance; }
VD_CG_INL DPlane      dhessianplane    (D3 normal, D1 distance)                { DPlane r; r.xyz = normal; r.w = distance; }

VD_CG_INL FRay        fmray            (F3 origin, F3 direction)               { FRay r; r.origin = origin; r.direction = direction; return r; }
VD_CG_INL DRay        dmray            (D3 origin, D3 direction)               { DRay r; r.origin = origin; r.direction = direction; return r; }
VD_CG_INL FCylinder   fmcylinder       (F3 p, F3 q, F1 r)                      { FCylinder c; c.p = p; c.q = q; c.r = r; return c; }
VD_CG_INL DCylinder   dmcylinder       (D3 p, D3 q, D1 r)                      { DCylinder c; c.p = p; c.q = q; c.r = r; return c; }

VD_CG_INL FSphere     fmsphere         (F3 c, F1 r)                            { FSphere s; s.c = c; s.r = r; return s; }
VD_CG_INL DSphere     dmsphere         (D3 c, D1 r)                            { DSphere s; s.c = c; s.r = r; return s; }

/* ----TRUNCATION---------------------------------------------------------------------------------------------------- */
VD_CG_INL S1          ftos1            (F1 v)                                  { return (S1)v; }
VD_CG_INL S2          ftos2            (F2 v)                                  { return sm2((S1)v.x, (S1)v.y); }
VD_CG_INL S3          ftos3            (F3 v)                                  { return sm3((S1)v.x, (S1)v.y, (S1)v.z); }
VD_CG_INL S4          ftos4            (F4 v)                                  { return sm4((S1)v.x, (S1)v.y, (S1)v.z, (S1)v.w); }
VD_CG_INL S1          dtos1            (D1 v)                                  { return (S1)v; }
VD_CG_INL S2          dtos2            (D2 v)                                  { return sm2((S1)v.x, (S1)v.y); }
VD_CG_INL S3          dtos3            (D3 v)                                  { return sm3((S1)v.x, (S1)v.y, (S1)v.z); }
VD_CG_INL S4          dtos4            (D4 v)                                  { return sm4((S1)v.x, (S1)v.y, (S1)v.z, (S1)v.w); }
VD_CG_INL F1          dtof1            (D1 v)                                  { return (F1)v; }
VD_CG_INL F2          dtof2            (D2 v)                                  { return fm2((F1)v.x, (F1)v.y); }
VD_CG_INL F3          dtof3            (D3 v)                                  { return fm3((F1)v.x, (F1)v.y, (F1)v.z); }
VD_CG_INL F4          dtof4            (D4 v)                                  { return fm4((F1)v.x, (F1)v.y, (F1)v.z, (F1)v.w); }

// >> Deprecated
VD_CG_INL S2          ftrunc2          (F2 v)                                  { return sm2((S1)v.x, (S1)v.y); }
VD_CG_INL S3          ftrunc3          (F3 v)                                  { return sm3((S1)v.x, (S1)v.y, (S1)v.z); }
VD_CG_INL S4          ftrunc4          (F4 v)                                  { return sm4((S1)v.x, (S1)v.y, (S1)v.z, (S1)v.w); }
// << Deprecated

/* ----WIDENING------------------------------------------------------------------------------------------------------ */
VD_CG_INL F1          stof1            (S1 v)                                  { return (F1)v; }
VD_CG_INL F2          stof2            (S2 v)                                  { return fm2((F1)v.x, (F1)v.y); }
VD_CG_INL F3          stof3            (S3 v)                                  { return fm3((F1)v.x, (F1)v.y, (F1)v.z); }
VD_CG_INL F4          stof4            (S4 v)                                  { return fm4((F1)v.x, (F1)v.y, (F1)v.z, (F1)v.w); }
VD_CG_INL D1          stod1            (S1 v)                                  { return (D1)v; }
VD_CG_INL D2          stod2            (S2 v)                                  { return dm2((F1)v.x, (F1)v.y); }
VD_CG_INL D3          stod3            (S3 v)                                  { return dm3((F1)v.x, (F1)v.y, (F1)v.z); }
VD_CG_INL D4          stod4            (S4 v)                                  { return dm4((F1)v.x, (F1)v.y, (F1)v.z, (F1)v.w); }
VD_CG_INL D1          ftod1            (F1 v)                                  { return (D1)v; }
VD_CG_INL D2          ftod2            (F2 v)                                  { return dm2((F1)v.x, (F1)v.y); }
VD_CG_INL D3          ftod3            (F3 v)                                  { return dm3((F1)v.x, (F1)v.y, (F1)v.z); }
VD_CG_INL D4          ftod4            (F4 v)                                  { return dm4((F1)v.x, (F1)v.y, (F1)v.z, (F1)v.w); }

// >> Deprecated
VD_CG_INL F2          swiden2f         (S2 v)                                  { F2 r; r.x = (F1)v.x; r.y = (F1)v.y; return r; }
// << Deprecated

/* ----UTILITY------------------------------------------------------------------------------------------------------- */
VD_CG_INL F1          fwrap_degrees    (F1 d)                                  { return d - F_TAU * ffloor(d / F_TAU); }
VD_CG_INL F1          fclamp           (F1 i, F1 x, F1 a)                      { if (x < i) return i; if (x > a) return a; return x; }
VD_CG_INL F1          flerp            (F1 a, F1 b, F1 t)                      { return a + t * (b - a); }
VD_CG_INL F1          fsmax            (F1 a, F1 b)                            { return a > b ? a : b; }
VD_CG_INL D1          dsmax            (D1 a, D1 b)                            { return a > b ? a : b; }
VD_CG_INL S1          ssmax            (S1 a, S1 b)                            { return a > b ? a : b; }
VD_CG_INL F1          fsmin            (F1 a, F1 b)                            { return a < b ? a : b; }
VD_CG_INL D1          dsmin            (D1 a, D1 b)                            { return a < b ? a : b; }
VD_CG_INL S1          ssmin            (S1 a, S1 b)                            { return a < b ? a : b; }
VD_CG_INL F1          fsabs            (F1 a)                                  { return a < 0.f ? -a : a; }
VD_CG_INL D1          dsabs            (D1 a)                                  { return a < 0.0 ? -a : a; }
VD_CG_INL S1          ssabs            (S1 a)                                  { return a < 0 ? -a : a; }
VD_CG_INL int         feq              (F1 a, F1 b)                            { return fsabs(b - a) <= F_EPSILON; }
VD_CG_INL int         deq              (D1 a, D1 b)                            { return dsabs(b - a) <= D_EPSILON; }

/* ----VECTOR ALGEBRA------------------------------------------------------------------------------------------------ */
VD_CG_INL F2          fadd2            (F2 a, F2 b)                            { return fm2(a.x + b.x, a.y + b.y); }
VD_CG_INL D2          dadd2            (D2 a, D2 b)                            { return dm2(a.x + b.x, a.y + b.y); }
VD_CG_INL S2          sadd2            (S2 a, S2 b)                            { return sm2(a.x + b.x, a.y + b.y); }
VD_CG_INL F3          fadd3            (F3 a, F3 b)                            { return fm3(a.x + b.x, a.y + b.y, a.z + b.z); }
VD_CG_INL D3          dadd3            (D3 a, D3 b)                            { return dm3(a.x + b.x, a.y + b.y, a.z + b.z); }
VD_CG_INL S3          sadd3            (S3 a, S3 b)                            { return sm3(a.x + b.x, a.y + b.y, a.z + b.z); }
VD_CG_INL F4          fadd4            (F4 a, F4 b)                            { return fm4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
VD_CG_INL D4          dadd4            (D4 a, D4 b)                            { return dm4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }
VD_CG_INL S4          sadd4            (S4 a, S4 b)                            { return sm4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w); }

VD_CG_INL F2          fsub2            (F2 a, F2 b)                            { return fm2(a.x - b.x, a.y - b.y); }
VD_CG_INL D2          dsub2            (D2 a, D2 b)                            { return dm2(a.x - b.x, a.y - b.y); }
VD_CG_INL S2          ssub2            (S2 a, S2 b)                            { return sm2(a.x - b.x, a.y - b.y); }
VD_CG_INL F3          fsub3            (F3 a, F3 b)                            { return fm3(a.x - b.x, a.y - b.y, a.z - b.z); }
VD_CG_INL D3          dsub3            (D3 a, D3 b)                            { return dm3(a.x - b.x, a.y - b.y, a.z - b.z); }
VD_CG_INL S3          ssub3            (S3 a, S3 b)                            { return sm3(a.x - b.x, a.y - b.y, a.z - b.z); }
VD_CG_INL F4          fsub4            (F4 a, F4 b)                            { return fm4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
VD_CG_INL D4          dsub4            (D4 a, D4 b)                            { return dm4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }
VD_CG_INL S4          ssub4            (S4 a, S4 b)                            { return sm4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w); }

VD_CG_INL F2          fscale2          (F2 v, F1 s)                            { return fm2(v.x * s, v.y * s); }
VD_CG_INL D2          dscale2          (D2 v, D1 s)                            { return dm2(v.x * s, v.y * s); }
VD_CG_INL S2          sscale2          (S2 v, S1 s)                            { return sm2(v.x * s, v.y * s); }
VD_CG_INL F3          fscale3          (F3 v, F1 s)                            { return fm3(v.x * s, v.y * s, v.z * s); }
VD_CG_INL D3          dscale3          (D3 v, D1 s)                            { return dm3(v.x * s, v.y * s, v.z * s); }
VD_CG_INL S3          sscale3          (S3 v, S1 s)                            { return sm3(v.x * s, v.y * s, v.z * s); }
VD_CG_INL F4          fscale4          (F4 v, F1 s)                            { return fm4(v.x * s, v.y * s, v.z * s, v.w * s); }
VD_CG_INL D4          dscale4          (D4 v, D1 s)                            { return dm4(v.x * s, v.y * s, v.z * s, v.w * s); }
VD_CG_INL S4          sscale4          (S4 v, S1 s)                            { return sm4(v.x * s, v.y * s, v.z * s, v.w * s); }

VD_CG_INL F2          fcmul2           (F2 a, F2 b)                            { return fm2(a.x * b.x, a.y * b.y); }
VD_CG_INL D2          dcmul2           (D2 a, D2 b)                            { return dm2(a.x * b.x, a.y * b.y); }
VD_CG_INL S2          scmul2           (S2 a, S2 b)                            { return sm2(a.x * b.x, a.y * b.y); }
VD_CG_INL F3          fcmul3           (F3 a, F3 b)                            { return fm3(a.x * b.x, a.y * b.y, a.z * b.z); }
VD_CG_INL D3          dcmul3           (D3 a, D3 b)                            { return dm3(a.x * b.x, a.y * b.y, a.z * b.z); }
VD_CG_INL S3          scmul3           (S3 a, S3 b)                            { return sm3(a.x * b.x, a.y * b.y, a.z * b.z); }
VD_CG_INL F4          fcmul4           (F4 a, F4 b)                            { return fm4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
VD_CG_INL D4          dcmul4           (D4 a, D4 b)                            { return dm4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }
VD_CG_INL S4          scmul4           (S4 a, S4 b)                            { return sm4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w); }

VD_CG_INL F2          fdiv2            (F2 v, F1 s)                            { return fm2(v.x / s, v.y / s); }
VD_CG_INL D2          ddiv2            (D2 v, D1 s)                            { return dm2(v.x / s, v.y / s); }
VD_CG_INL S2          sdiv2            (S2 v, S1 s)                            { return sm2(v.x / s, v.y / s); }
VD_CG_INL F3          fdiv3            (F3 v, F1 s)                            { return fm3(v.x / s, v.y / s, v.z / s); }
VD_CG_INL D3          ddiv3            (D3 v, D1 s)                            { return dm3(v.x / s, v.y / s, v.z / s); }
VD_CG_INL S3          sdiv3            (S3 v, S1 s)                            { return sm3(v.x / s, v.y / s, v.z / s); }
VD_CG_INL F4          fdiv4            (F4 v, F1 s)                            { return fm4(v.x / s, v.y / s, v.z / s, v.w / s); }
VD_CG_INL D4          ddiv4            (D4 v, D1 s)                            { return dm4(v.x / s, v.y / s, v.z / s, v.w / s); }
VD_CG_INL S4          sdiv4            (S4 v, S1 s)                            { return sm4(v.x / s, v.y / s, v.z / s, v.w / s); }

VD_CG_INL F1          fdot2            (F2 a, F2 b)                            { return a.x * b.x + a.y * b.y; }
VD_CG_INL D1          ddot2            (D2 a, D2 b)                            { return a.x * b.x + a.y * b.y; }
VD_CG_INL S1          sdot2            (S2 a, S2 b)                            { return a.x * b.x + a.y * b.y; }
VD_CG_INL F1          fdot3            (F3 a, F3 b)                            { return a.x * b.x + a.y * b.y + a.z * b.z; }
VD_CG_INL D1          ddot3            (D3 a, D3 b)                            { return a.x * b.x + a.y * b.y + a.z * b.z; }
VD_CG_INL S1          sdot3            (S3 a, S3 b)                            { return a.x * b.x + a.y * b.y + a.z * b.z; }
VD_CG_INL F1          fdot4            (F4 a, F4 b)                            { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
VD_CG_INL D1          ddot4            (D4 a, D4 b)                            { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
VD_CG_INL S1          sdot4            (S4 a, S4 b)                            { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }

VD_CG_INL F1          flensq2          (F2 v)                                  { return fdot2(v, v); }
VD_CG_INL D1          dlensq2          (D2 v)                                  { return ddot2(v, v); }
VD_CG_INL S1          slensq2          (S2 v)                                  { return sdot2(v, v); }
VD_CG_INL F1          flensq3          (F3 v)                                  { return fdot3(v, v); }
VD_CG_INL D1          dlensq3          (D3 v)                                  { return ddot3(v, v); }
VD_CG_INL S1          slensq3          (S3 v)                                  { return sdot3(v, v); }
VD_CG_INL F1          flensq4          (F4 v)                                  { return fdot4(v, v); }
VD_CG_INL D1          dlensq4          (D4 v)                                  { return ddot4(v, v); }
VD_CG_INL S1          slensq4          (S4 v)                                  { return sdot4(v, v); }

VD_CG_INL F2          fnormalize2      (F2 v)                                  { F1 l = flensq2(v); return fdiv2(v, fsqrt(l)); }
VD_CG_INL D2          dnormalize2      (D2 v)                                  { D1 l = dlensq2(v); return ddiv2(v, dsqrt(l)); }
VD_CG_INL F3          fnormalize3      (F3 v)                                  { F1 l = flensq3(v); return fdiv3(v, fsqrt(l)); }
VD_CG_INL D3          dnormalize3      (D3 v)                                  { D1 l = dlensq3(v); return ddiv3(v, dsqrt(l)); }
VD_CG_INL F4          fnormalize4      (F4 v)                                  { F1 l = flensq4(v); return fdiv4(v, fsqrt(l)); }
VD_CG_INL D4          dnormalize4      (D4 v)                                  { D1 l = dlensq4(v); return ddiv4(v, dsqrt(l)); }

VD_CG_INL F2          fnoz2            (F2 v)                                  { F1 l = flensq2(v); if (l == 0.f) return fzero2(); else return fdiv2(v, fsqrt(l)); }
VD_CG_INL D2          dnoz2            (D2 v)                                  { D1 l = dlensq2(v); if (l == 0.0) return dzero2(); else return ddiv2(v, dsqrt(l)); }
VD_CG_INL F3          fnoz3            (F3 v)                                  { F1 l = flensq3(v); if (l == 0.f) return fzero3(); else return fdiv3(v, fsqrt(l)); }
VD_CG_INL D3          dnoz3            (D3 v)                                  { D1 l = dlensq3(v); if (l == 0.0) return dzero3(); else return ddiv3(v, dsqrt(l)); }
VD_CG_INL F4          fnoz4            (F4 v)                                  { F1 l = flensq4(v); if (l == 0.f) return fzero4(); else return fdiv4(v, fsqrt(l)); }
VD_CG_INL D4          dnoz4            (D4 v)                                  { D1 l = dlensq4(v); if (l == 0.0) return dzero4(); else return ddiv4(v, dsqrt(l)); }

VD_CG_INL F3          fcross3          (F3 a, F3 b)                            { return fm3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
VD_CG_INL D3          dcross3          (D3 a, D3 b)                            { return dm3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
VD_CG_INL S3          scross3          (S3 a, S3 b)                            { return sm3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }

VD_CG_INL int         feq2             (F2 a, F2 b)                            { return feq(a.x, b.x) && feq(a.y, b.y); }
VD_CG_INL int         deq2             (D2 a, D2 b)                            { return deq(a.x, b.x) && deq(a.y, b.y); }
VD_CG_INL int         seq2             (S2 a, S2 b)                            { return (a.x == b.x)  && (a.y == b.y); }
VD_CG_INL int         feq3             (F3 a, F3 b)                            { return feq(a.x, b.x) && feq(a.y, b.y) && feq(a.z, b.z); }
VD_CG_INL int         deq3             (D3 a, D3 b)                            { return deq(a.x, b.x) && deq(a.y, b.y) && deq(a.z, b.z); }
VD_CG_INL int         seq3             (S3 a, S3 b)                            { return (a.x == b.x)  && (a.y == b.y)  && (a.z == b.z); }
VD_CG_INL int         feq4             (F4 a, F4 b)                            { return feq(a.x, b.x) && feq(a.y, b.y) && feq(a.z, b.z) && feq(a.w, b.w); }
VD_CG_INL int         deq4             (D4 a, D4 b)                            { return deq(a.x, b.x) && deq(a.y, b.y) && deq(a.z, b.z) && deq(a.w, b.w); }
VD_CG_INL int         seq4             (S4 a, S4 b)                            { return (a.x == b.x)  && (a.y == b.y)  && (a.z == b.z)  && (a.w == b.w); }

/* ----MATRIX ALGEBRA------------------------------------------------------------------------------------------- */
VD_CG_INL F4x4        ftranspose4x4    (F4x4 *m)                               { return fm4x4(m->a0, m->b0, m->c0, m->d0,
                                                                                              m->a1, m->b1, m->c1, m->d1,
                                                                                              m->a2, m->b2, m->c2, m->d2,
                                                                                              m->a3, m->b3, m->c3, m->d3); }
VD_CG_INL D4x4        dtranspose4x4    (D4x4 *m)                               { return dm4x4(m->a0, m->b0, m->c0, m->d0,
                                                                                              m->a1, m->b1, m->c1, m->d1,
                                                                                              m->a2, m->b2, m->c2, m->d2,
                                                                                              m->a3, m->b3, m->c3, m->d3); }

VD_CG_INL F4x4        fmul4x4          (F4x4 *a, F4x4 *b)                      { return fm4x4(a->a0*b->a0 + a->a1*b->b0 + a->a2*b->c0 + a->a3*b->d0, a->a0*b->a1 + a->a1*b->b1 + a->a2*b->c1 + a->a3*b->d1, a->a0*b->a2 + a->a1*b->b2 + a->a2*b->c2 + a->a3*b->d2, a->a0*b->a3 + a->a1*b->b3 + a->a2*b->c3 + a->a3*b->d3,
                                                                                              a->b0*b->a0 + a->b1*b->b0 + a->b2*b->c0 + a->b3*b->d0, a->b0*b->a1 + a->b1*b->b1 + a->b2*b->c1 + a->b3*b->d1, a->b0*b->a2 + a->b1*b->b2 + a->b2*b->c2 + a->b3*b->d2, a->b0*b->a3 + a->b1*b->b3 + a->b2*b->c3 + a->b3*b->d3,
                                                                                              a->c0*b->a0 + a->c1*b->b0 + a->c2*b->c0 + a->c3*b->d0, a->c0*b->a1 + a->c1*b->b1 + a->c2*b->c1 + a->c3*b->d1, a->c0*b->a2 + a->c1*b->b2 + a->c2*b->c2 + a->c3*b->d2, a->c0*b->a3 + a->c1*b->b3 + a->c2*b->c3 + a->c3*b->d3,
                                                                                              a->d0*b->a0 + a->d1*b->b0 + a->d2*b->c0 + a->d3*b->d0, a->d0*b->a1 + a->d1*b->b1 + a->d2*b->c1 + a->d3*b->d1, a->d0*b->a2 + a->d1*b->b2 + a->d2*b->c2 + a->d3*b->d2, a->d0*b->a3 + a->d1*b->b3 + a->d2*b->c3 + a->d3*b->d3); }
VD_CG_INL D4x4        dmul4x4          (D4x4 *a, D4x4 *b)                      { return dm4x4(a->a0*b->a0 + a->a1*b->b0 + a->a2*b->c0 + a->a3*b->d0, a->a0*b->a1 + a->a1*b->b1 + a->a2*b->c1 + a->a3*b->d1, a->a0*b->a2 + a->a1*b->b2 + a->a2*b->c2 + a->a3*b->d2, a->a0*b->a3 + a->a1*b->b3 + a->a2*b->c3 + a->a3*b->d3,
                                                                                              a->b0*b->a0 + a->b1*b->b0 + a->b2*b->c0 + a->b3*b->d0, a->b0*b->a1 + a->b1*b->b1 + a->b2*b->c1 + a->b3*b->d1, a->b0*b->a2 + a->b1*b->b2 + a->b2*b->c2 + a->b3*b->d2, a->b0*b->a3 + a->b1*b->b3 + a->b2*b->c3 + a->b3*b->d3,
                                                                                              a->c0*b->a0 + a->c1*b->b0 + a->c2*b->c0 + a->c3*b->d0, a->c0*b->a1 + a->c1*b->b1 + a->c2*b->c1 + a->c3*b->d1, a->c0*b->a2 + a->c1*b->b2 + a->c2*b->c2 + a->c3*b->d2, a->c0*b->a3 + a->c1*b->b3 + a->c2*b->c3 + a->c3*b->d3,
                                                                                              a->d0*b->a0 + a->d1*b->b0 + a->d2*b->c0 + a->d3*b->d0, a->d0*b->a1 + a->d1*b->b1 + a->d2*b->c1 + a->d3*b->d1, a->d0*b->a2 + a->d1*b->b2 + a->d2*b->c2 + a->d3*b->d2, a->d0*b->a3 + a->d1*b->b3 + a->d2*b->c3 + a->d3*b->d3); }
VD_CG_INL F4          fmul4x4_4        (F4x4 *m, F4    v)                      { return fm4(fdot4(fm4(m->a0, m->b0, m->c0, m->d0), v),
                                                                                            fdot4(fm4(m->a1, m->b1, m->c1, m->d1), v),
                                                                                            fdot4(fm4(m->a2, m->b2, m->c2, m->d2), v),
                                                                                            fdot4(fm4(m->a3, m->b3, m->c3, m->d3), v)); }
VD_CG_INL D4          dmul4x4_4        (D4x4 *m, D4    v)                      { return dm4(ddot4(dm4(m->a0, m->b0, m->c0, m->d0), v),
                                                                                            ddot4(dm4(m->a1, m->b1, m->c1, m->d1), v),
                                                                                            ddot4(dm4(m->a2, m->b2, m->c2, m->d2), v),
                                                                                            ddot4(dm4(m->a3, m->b3, m->c3, m->d3), v)); }
VD_CG_INL F3          fmul4x4_3        (F4x4 *m, F3    v)                      { return fmul4x4_4(m, fm4ps(v, 1.f)).xyz; }
VD_CG_INL D3          dmul4x4_3        (D4x4 *m, D3    v)                      { return dmul4x4_4(m, dm4ps(v, 1.f)).xyz; }

VD_CG_INL F4x4        finverse4x4      (F4x4 *m)
{
    int i;
    F4x4 minors;
    F1 D = 0.f;
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

    if (D == 0.f) return fidentity4x4();

    minors.a0 *= +1.f; minors.a1 *= -1.f; minors.a2 *= +1.f; minors.a3 *= -1.f;
    minors.b0 *= -1.f; minors.b1 *= +1.f; minors.b2 *= -1.f; minors.b3 *= +1.f;
    minors.c0 *= +1.f; minors.c1 *= -1.f; minors.c2 *= +1.f; minors.c3 *= -1.f;
    minors.d0 *= -1.f; minors.d1 *= +1.f; minors.d2 *= -1.f; minors.d3 *= +1.f;

    minors = ftranspose4x4(&minors);
    for (i = 0; i < 16; i += 1) {
        minors.p[i] *= 1.f/D;
    }

    return minors;
}

VD_CG_INL D4x4        dinverse4x4      (D4x4 *m)
{
    int i;
    D4x4 minors;
    D1 D = 0.f;
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

    if (D == 0.0) return didentity4x4();

    minors.a0 *= +1.f; minors.a1 *= -1.f; minors.a2 *= +1.f; minors.a3 *= -1.f;
    minors.b0 *= -1.f; minors.b1 *= +1.f; minors.b2 *= -1.f; minors.b3 *= +1.f;
    minors.c0 *= +1.f; minors.c1 *= -1.f; minors.c2 *= +1.f; minors.c3 *= -1.f;
    minors.d0 *= -1.f; minors.d1 *= +1.f; minors.d2 *= -1.f; minors.d3 *= +1.f;

    minors = dtranspose4x4(&minors);
    for (i = 0; i < 16; i += 1) {
        minors.p[i] *= 1.f/D;
    }

    return minors;
}

/* ----QUATERNION ALGEBRA-------------------------------------------------------------------------------------------- */
VD_CG_INL FQuat       fidentityquat    (void)                                  { return fmquat(fzero3(), 1.f); }
VD_CG_INL DQuat       didentityquat    (void)                                  { return dmquat(dzero3(), 1.0); }

VD_CG_INL F1          flensqquat       (FQuat q)                               { return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w; }
VD_CG_INL D1          dlensqquat       (DQuat q)                               { return q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w; }

VD_CG_INL FQuat       fnormalizequat   (FQuat q)                               { F1 l = flensqquat(q); return fdiv4(q, fsqrt(l)); }
VD_CG_INL DQuat       dnormalizequat   (DQuat q)                               { D1 l = dlensqquat(q); return ddiv4(q, dsqrt(l)); }

VD_CG_INL FQuat       fnozquat         (FQuat q)                               { F1 l = flensqquat(q); if (feq(l, 0.f)) return fidentityquat(); else return fdiv4(q, fsqrt(l)); }
VD_CG_INL DQuat       dnozquat         (DQuat q)                               { D1 l = dlensqquat(q); if (deq(l, 0.0)) return didentityquat(); else return ddiv4(q, dsqrt(l)); }

VD_CG_INL FQuat       fconquat         (FQuat q)                               { q.x = -q.x; q.y = -q.y; q.z = -q.z; return q; }
VD_CG_INL DQuat       dconquat         (DQuat q)                               { q.x = -q.x; q.y = -q.y; q.z = -q.z; return q; }

VD_CG_INL FQuat       faxis_anglequat  (F3 axis, F1 angle)                     { return fmquat(fscale3(axis, fsin(angle * .5f)), fcos(angle * .5f)); }
VD_CG_INL DQuat       daxis_anglequat  (D3 axis, D1 angle)                     { return dmquat(dscale3(axis, dsin(angle * .5f)), dcos(angle * .5f)); }

VD_CG_INL FQuat       fmulquat         (FQuat q1, FQuat q2)                    { return fmquat(fm3(q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,
                                                                                                   -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
                                                                                                   q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z),
                                                                                               -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w);    }
VD_CG_INL DQuat       dmulquat         (DQuat q1, DQuat q2)                    { return dmquat(dm3(q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x,
                                                                                                   -q1.x * q2.z + q1.y * q2.w + q1.z * q2.x + q1.w * q2.y,
                                                                                                   q1.x * q2.y - q1.y * q2.x + q1.z * q2.w + q1.w * q2.z),
                                                                                               -q1.x * q2.x - q1.y * q2.y - q1.z * q2.z + q1.w * q2.w);    }
VD_CG_INL F3          fmulquat_3       (FQuat q, F3 v)                         { return fmulquat(fmulquat(q, fmquat(v, 0.f)), fconquat(q)).xyz; }
VD_CG_INL D3          dmulquat_3       (DQuat q, D3 v)                         { return dmulquat(dmulquat(q, dmquat(v, 0.0)), dconquat(q)).xyz; }
VD_CG_INL F4x4        fto4x4quat       (FQuat q)
{
    F1 qxx = (q.x * q.x);
    F1 qyy = (q.y * q.y);
    F1 qzz = (q.z * q.z);
    F1 qxz = (q.x * q.z);
    F1 qxy = (q.x * q.y);
    F1 qyz = (q.y * q.z);
    F1 qwx = (q.w * q.x);
    F1 qwy = (q.w * q.y);
    F1 qwz = (q.w * q.z);
    return fm4x4(
        1.f - 2.f * (qyy + qzz), 2.f * (qxy + qwz),       2.f * (qxz - qwy),       0.f,
        2.f * (qxy - qwz),       1.f - 2.f * (qxx + qzz), 2.f * (qyz + qwx),       0.f,
        2.f * (qxz + qwy),       2.f * (qyz - qwx),       1.f - 2.f * (qxx + qyy), 0.f,
        0.f,                     0.f,                     0.f,                     1.f);
}
VD_CG_INL D4x4        dto4x4quat       (DQuat q)
{
    D1 qxx = (q.x * q.x);
    D1 qyy = (q.y * q.y);
    D1 qzz = (q.z * q.z);
    D1 qxz = (q.x * q.z);
    D1 qxy = (q.x * q.y);
    D1 qyz = (q.y * q.z);
    D1 qwx = (q.w * q.x);
    D1 qwy = (q.w * q.y);
    D1 qwz = (q.w * q.z);
    return dm4x4(
        1.0 - 2.0 * (qyy + qzz), 2.0 * (qxy + qwz),       2.0 * (qxz - qwy),       0.0,
        2.0 * (qxy - qwz),       1.0 - 2.0 * (qxx + qzz), 2.0 * (qyz + qwx),       0.0,
        2.0 * (qxz + qwy),       2.0 * (qyz - qwx),       1.0 - 2.0 * (qxx + qyy), 0.0,
        0.0,                     0.0,                     0.0,                     1.0);
}

VD_CG_INL FQuat       feulerquat       (F3 euler)
{
    F1 cy = fcos(euler.y * 0.5f); // yaw
    F1 sy = fsin(euler.y * 0.5f);
    F1 cp = fcos(euler.x * 0.5f); // pitch
    F1 sp = fsin(euler.x * 0.5f);
    F1 cr = fcos(euler.z * 0.5f); // roll
    F1 sr = fsin(euler.z * 0.5f);

    FQuat q;
    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * sp * cr + sy * cp * sr;
    q.y = sy * cp * cr - cy * sp * sr;
    q.z = cy * cp * sr - sy * sp * cr;
    return q;
}

VD_CG_INL DQuat       deulerquat       (D3 euler)
{
    D1 cy = dcos(euler.y * 0.5); // yaw
    D1 sy = dsin(euler.y * 0.5);
    D1 cp = dcos(euler.x * 0.5); // pitch
    D1 sp = dsin(euler.x * 0.5);
    D1 cr = dcos(euler.z * 0.5); // roll
    D1 sr = dsin(euler.z * 0.5);

    DQuat q;
    q.w = cy * cp * cr + sy * sp * sr;
    q.x = cy * sp * cr + sy * cp * sr;
    q.y = sy * cp * cr - cy * sp * sr;
    q.z = cy * cp * sr - sy * sp * cr;
    return q;
}

VD_CG_INL FQuat       flookrotquat     (F3 fwd, F3 updir)
{
    F3 basis_z = fwd;
    F3 basis_x = fnoz3(fcross3(updir, fwd));
    F3 basis_y = fcross3(fwd, basis_x);

    F1 m00 = basis_x.x;
    F1 m01 = basis_y.x;
    F1 m02 = basis_z.x;

    F1 m10 = basis_x.y;
    F1 m11 = basis_y.y;
    F1 m12 = basis_z.y;

    F1 m20 = basis_x.z;
    F1 m21 = basis_y.z;
    F1 m22 = basis_z.z;

    FQuat q;
    F1 T = m00 + m11 + m22;
    if (T > 0.f) {
        F1 s = fsqrt(T + 1.f) * 2.f;
        q.w = 0.25f * s;
        q.x = (m21 - m12) / s;
        q.y = (m02 - m20) / s;
        q.z = (m10 - m01) / s;
    } else if ((m00 > m11) && (m00 > m22)) {
        F1 s = fsqrt(1.f + m00 - m11 - m22) * 2.f;
        q.w = (m21 - m12) / s;
        q.x = 0.25f * s;
        q.y = (m01 + m10) / s;
        q.z = (m02 + m20) / s;
    } else if (m11 > m22) {
        F1 s = fsqrt(1.f + m11 - m00 - m22) * 2.f;
        q.w = (m02 - m20) / s;
        q.x = (m01 + m10) / s;
        q.y = 0.25f * s;
        q.z = (m12 + m21) / s;
    } else {
        F1 s = fsqrt(1.f + m22 - m00 - m11) * 2.f;
        q.w = (m10 - m01) / s;
        q.x = (m02 - m20) / s;
        q.y = (m12 + m21) / s;
        q.z = 0.25f * s;
    }

    return q;
}

VD_CG_INL DQuat       dlookrotquat     (D3 fwd, D3 updir)
{
    D3 basis_z = fwd;
    D3 basis_x = dnoz3(dcross3(updir, fwd));
    D3 basis_y = dcross3(fwd, basis_x);

    D1 m00 = basis_x.x;
    D1 m01 = basis_y.x;
    D1 m02 = basis_z.x;

    D1 m10 = basis_x.y;
    D1 m11 = basis_y.y;
    D1 m12 = basis_z.y;

    D1 m20 = basis_x.z;
    D1 m21 = basis_y.z;
    D1 m22 = basis_z.z;

    DQuat q;
    D1 T = m00 + m11 + m22;
    if (T > 0.0) {
        D1 s = dsqrt(T + 1.0) * 2.0;
        q.w = 0.25 * s;
        q.x = (m21 - m12) / s;
        q.y = (m02 - m20) / s;
        q.z = (m10 - m01) / s;
    } else if ((m00 > m11) && (m00 > m22)) {
        D1 s = dsqrt(1.0 + m00 - m11 - m22) * 2.0;
        q.w = (m21 - m12) / s;
        q.x = 0.25 * s;
        q.y = (m01 + m10) / s;
        q.z = (m02 + m20) / s;
    } else if (m11 > m22) {
        D1 s = dsqrt(1.0 + m11 - m00 - m22) * 2.0;
        q.w = (m02 - m20) / s;
        q.x = (m01 + m10) / s;
        q.y = 0.25 * s;
        q.z = (m12 + m21) / s;
    } else {
        D1 s = dsqrt(1.0 + m22 - m00 - m11) * 2.0;
        q.w = (m10 - m01) / s;
        q.x = (m02 - m20) / s;
        q.y = (m12 + m21) / s;
        q.z = 0.25 * s;
    }

    return q;
}

/* ----CONVERSIONS--------------------------------------------------------------------------------------------------- */
VD_CG_INL FPlane      fpnormplane      (F3 point, F3 normal)
{
    FPlane r;
    F1 ln = flensq3(normal);
    if (ln == 0.f) return fhessianplane(fzero3(), 0.f);
    ln = fsqrt(ln);

    r.xyz = fdiv3(normal, ln);
    r.w = fdot3(normal, point) / ln;
    return r;
}

VD_CG_INL DPlane      dpnormplane      (D3 point, D3 normal)
{
    DPlane r;
    D1 ln = dlensq3(normal);
    if (ln == 0.0) return dhessianplane(dzero3(), 0.0);

    ln = dsqrt(ln);

    r.xyz = ddiv3(normal, ln);
    r.w = ddot3(normal, point) / ln;
    return r;
}

VD_CG_INL FPlane      fimplicitplane   (F4 coeffs)
{
    FPlane r;
    F1 ln = flensq3(coeffs.xyz);
    if (ln == 0.f) return fhessianplane(fzero3(), 0.f);
    ln = fsqrt(ln);

    r.x = coeffs.x / ln;
    r.y = coeffs.y / ln;
    r.z = coeffs.z / ln;
    r.w = coeffs.w / ln;
    return r;
}

VD_CG_INL DPlane      dimplicitplane   (D4 coeffs)
{
    DPlane r;
    D1 ln = dlensq3(coeffs.xyz);
    if (ln == 0.0) return dhessianplane(dzero3(), 0.0);
    ln = dsqrt(ln);

    r.x = coeffs.x / ln;
    r.y = coeffs.y / ln;
    r.z = coeffs.z / ln;
    r.w = coeffs.w / ln;
    return r;
}

VD_CG_INL FRay        fline_to_ray     (FLine line)                            { FRay r; r.origin = line.a; r.direction = fnoz3(fsub3(line.b, line.a)); return r; }
VD_CG_INL DRay        dline_to_ray     (DLine line)                            { DRay r; r.origin = line.a; r.direction = dnoz3(dsub3(line.b, line.a)); return r; }
VD_CG_INL FLine       fray_to_line     (FRay *ray, F1 t)                       { FLine r; r.a = ray->origin; r.b = fadd3(ray->origin, fscale3(ray->direction, t)); return r;}

/* ----COORDINATE SYSTEMS-------------------------------------------------------------------------------------------- */
/**
 * @brief Build perspective projection matrix for right handed coordinate system, with normalized depth [-1,+1]
 * @param  fovyrad Vertical FoV in radians
 * @param  aspect  Aspect Ratio
 * @param  pnr     Near Plane
 * @param  pfr     Far Plane
 * @return         The matrix
 */
VD_CG_INL F4x4        fperspective4x4    (F1 fovyrad, F1 aspect, F1 pnr, F1 pfr)
{
    F4x4 result;
    F1 f;

    f = 1.f / ftan(fovyrad * 0.5f);

    // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
    result = fm4x4(f / aspect,                       0.f,                  0.f,                        0.f,
                   0.f,                              f,                    0.f,                        0.f,
                   0.f,                              0.f,                  (pnr + pfr) / (pnr - pfr), -1.f,
                   0.f,                              0.f,                  2.f*pnr*pfr/(pnr - pfr),    0.f);

    result.e[2][2] = -result.e[2][2];
    result.e[2][3] = -result.e[2][3];
    return result;
}

VD_CG_INL F4x4        fperspective4x4_rno(F1 fovyrad, F1 aspect, F1 pnr, F1 pfr)
{
    F4x4 result;
    F1 f;

    f = 1.f / ftan(fovyrad * 0.5f);

    // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluPerspective.xml
    result = fm4x4(f / aspect,                       0.f,                  0.f,                        0.f,
                   0.f,                              f,                    0.f,                        0.f,
                   0.f,                              0.f,                  (pnr + pfr) / (pnr - pfr), -1.f,
                   0.f,                              0.f,                  2.f*pnr*pfr/(pnr - pfr),    0.f);
    return result;
}

VD_CG_INL F4x4        fperspective4x4_zo (F1 fovyrad, F1 aspect, F1 pnr, F1 pfr)
{
    F4x4 result;
    F1 f;

    f = 1.f / ftan(fovyrad * 0.5f);

    result = fm4x4(f / aspect,                       0.f,                  0.f,                        0.f,
                   0.f,                              f,                    0.f,                        0.f,
                   0.f,                              0.f,                  pfr / (pfr - pnr),          1.f,
                   0.f,                              0.f,                  -pnr * pfr / (pfr - pnr),   0.f);

    return result;
}

VD_CG_INL F4x4        foblique4x4        (F1 left, F1 right, F1 bottom, F1 top, F1 pnr, F1 pfr)
{
    F4x4 result;
    F1 width;
    F1 height;
    F1 depth;

    width  = right - left;
    height = top - bottom;
    depth  = pfr - pnr;

    result = fm4x4(2.f / width,              0.f,                        0.f,                    0.f,
                   0.f,                      2.f / (height),             0.f,                    0.f,
                   0.f,                      0.f,                        2.f / depth,            0.f,
                   - (right + left) / width, - (top + bottom) / height,  - (pfr + pnr) / depth,  1.f);
    return result;
}

VD_CG_INL F4x4        flookat4x4         (F3 from, F3 to, F3 up)
{
    F3 fwd = fsub3(from, to);
    fwd = fnoz3(fwd);

    F3 right = fcross3(fwd, up);
    right = fnoz3(right);

    F3 up2 = fcross3(right, fwd);

    F3 xaxis = right;
    F3 yaxis = up2;
    F3 zaxis = fwd;

    F3 meye = from;

    return fm4x4(
         xaxis.x,                yaxis.x,               -zaxis.x,               0.f,
         xaxis.y,                yaxis.y,               -zaxis.y,               0.f,
         xaxis.z,                yaxis.z,               -zaxis.z,               0.f,
        -fdot3(xaxis, meye), -fdot3(yaxis, meye), fdot3(zaxis, meye),  1.f);
}

VD_CG_INL F4x4        ftranslation4x4    (F3 v)                                { return fm4x4(1.f, 0.f, 0.f, 0.f,
                                                                                              0.f, 1.f, 0.f, 0.f,
                                                                                              0.f, 0.f, 1.f, 0.f,
                                                                                              v.x, v.y, v.z, 1.f); }

VD_CG_INL F4x4        frotation_pitch4x4 (F1 rad)                              { return fm4x4(1.f, 0.f,       0.f,        0.f,
                                                                                              0.f, fcos(rad), -fsin(rad), 0.f,
                                                                                              0.f, fsin(rad), fcos(rad),  0.f,
                                                                                              0.f, 0.f,       0.f,        1.f); }

VD_CG_INL F4x4        frotation_yaw4x4   (F1 rad)                              { return fm4x4(fcos(rad),  0.f, fsin(rad), 0.f,
                                                                                              0.f,        1.f, 0.f,       0.f,
                                                                                              -fsin(rad), 0.f, fcos(rad), 0.f,
                                                                                              0.f,        0.f, 0.f,       1.f); }

VD_CG_INL F4x4        frotation_roll4x4  (F1 rad)                              { return fm4x4(fcos(rad),  -fsin(rad), 0.f, 0.f,
                                                                                              -fsin(rad), fcos(rad),  0.f, 0.f,
                                                                                              0.f,        0.f,        1.f, 0.f,
                                                                                              0.f,        0.f,        0.f, 1.f); }
VD_CG_INL F4x4        frotation_quat4x4  (FQuat quat)
{
    F1 q0   = quat.w;  F1 q1   = quat.x;  F1 q2   = quat.y;  F1 q3   = quat.z;
    F1 q0q0 = q0 * q0; F1 q1q1 = q1 * q1; F1 q2q2 = q2 * q2; F1 q3q3 = q3 * q3;
    F1 q0q1 = q0 * q1; F1 q0q2 = q0 * q2; F1 q0q3 = q0 * q3;
    F1 q1q2 = q1 * q2; F1 q1q3 = q1 * q3;
    F1 q2q3 = q2 * q3;

    return fm4x4(2.f * (q0q0 + q1q1) - 1.f, 2.f * (q1q2 - q0q3),       2.f * (q1q3 + q0q2),       0.f,
                 2.f * (q1q2 + q0q3),       2.f * (q0q0 + q2q2) - 1.f, 2.f * (q1q3 + q0q1),       0.f,
                 2.f * (q1q3 - q0q2),       2.f * (q2q3 + q0q1),       2.f * (q0q0 + q3q3) - 1.f, 0.f,
                 0.f,                       0.f,                       0.f,                       1.f);
}

VD_CG_INL void          ftranslate4x4       (F4x4 *m, F3 v)                    { F4x4 t = ftranslation4x4(v); F4x4 r = fmul4x4(m, &t); *m = r; }
VD_CG_INL void          frotatequat4x4      (F4x4 *m, FQuat quat)              { F4x4 mq = frotation_quat4x4(quat); F4x4 r  = fmul4x4(m, &mq); *m = r; }
VD_CG_INL void          frotate_yaw4x4      (F4x4 *m, F1 rad)                  { F4x4 t = frotation_yaw4x4(rad); F4x4 r = fmul4x4(m, &t); *m = r; }

/* ----COLLISION DETECTION------------------------------------------------------------------------------------------- */
VD_CG_INL F3            fclosest_point_tri  (F3 p, F3 t0, F3 t1, F3 t2)
{
    F3 a = t0;
    F3 b = t1;
    F3 c = t2;

    // Check if P is in vertex regions outside A
    F3 ab  = fsub3(b, a);
    F3 ac  = fsub3(c, a);
    F3 ap  = fsub3(p, a);
    F1 d1 = fdot3(ab, ap);
    F1 d2 = fdot3(ac, ap);
    if (d1 <= 0.0f && d2 <= 0.0f) return a;

    // Check vertex region outside B
    F3 bp  = fsub3(p, b);
    F1 d3 = fdot3(ab, bp);
    F1 d4 = fdot3(ac, bp);
    if (d3 >= 0.0f && d4 <= d3) return b;

    // Check edge region of AB
    F1 vc = d1 * d4 - d3 * d2;
    if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f) {
        F1 v = d1 / (d1 - d3);
        return fadd3(a, fscale3(ab, v));
    }

    // Check vertex region outside C
    F3 cp  = fsub3(p, c);
    F1 d5 = fdot3(ab, cp);
    F1 d6 = fdot3(ac, cp);
    if (d6 >= 0.0f && d5 <= d6) return c;

    // Check edge region of AC
    F1 vb = d5 * d2 - d1 * d6;
    if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f) {
        F1 w = d2 / (d2 - d6);
        return fadd3(a, fscale3(ac, w));
    }

    // Check edge region of BC
    F1 va = d3 * d6 - d5 * d4;
    if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f) {
        F1 w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
        return fadd3(b, fscale3(fsub3(c, b), w));
    }

    // Inside face region
    F1 denom = 1.0f / (va + vb + vc);
    F1 v = vb * denom;
    F1 w = vc * denom;
    F3 result = fadd3(a, fscale3(ab, v));
    result = fadd3(result, fscale3(ac, w));
    return result;
}

VD_CG_INL int           fray_vs_sphere(FRay ray, FSphere sphere, F1 *t0, F1 *t1)
{
    F3 m = fsub3(ray.origin, sphere.c);
    F1 b = fdot3(m, ray.direction);
    F1 c = flensq3(m) - (sphere.r * sphere.r);

    if (c > 0.f && b > 0.f) {
        return 0;
    }

    F1 D = b * b - c;

    if (D < 0.f) {
        return 0;
    }

    if (t0) {
        *t0 = -b - fsqrt(D);
    }

    if (t1) {
        *t1 = -b + fsqrt(D);
    }

    return 1;
}

VD_CG_INL int           fline_vs_cylinder  (FLine *line, FCylinder *cylinder, F1 *t)
{
    F3 d = fsub3(cylinder->q, cylinder->p);
    F3 m = fsub3(line->a, cylinder->p);
    F3 n = fsub3(line->b, line->a);

    F1 md = fdot3(m, d);
    F1 nd = fdot3(n, d);
    F1 dd = fdot3(d, d);

    if ((md < 0.0f) && ((md + nd) < 0.0f)) {
        return 0;
    }

    if ((md > dd) && ((md + nd) > dd)) {
        return 0;
    }

    F1 nn = fdot3(n, n);
    F1 mn = fdot3(m, n);
    F1  a = dd * nn - nd * nd;
    F1  k = fdot3(m, m) - cylinder->r * cylinder->r;
    F1  c = dd * k - md * md;

    if (fabs(a) < F_EPSILON) {
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

    F1 b = dd * mn - nd * md;
    F1 discriminant = b * b - a * c;
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