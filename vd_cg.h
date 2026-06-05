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

/* ----SWIZZLING----------------------------------------------------------------------------------------------------- */
VD_CG_INL F2 fxx2(F2 v) { return fm2(v.x, v.x); }
VD_CG_INL F2 fxy2(F2 v) { return fm2(v.x, v.y); }
VD_CG_INL F2 fyx2(F2 v) { return fm2(v.y, v.x); }
VD_CG_INL F2 fyy2(F2 v) { return fm2(v.y, v.y); }
VD_CG_INL F3 fxxx2(F2 v) { return fm3(v.x, v.x, v.x); }
VD_CG_INL F3 fxxy2(F2 v) { return fm3(v.x, v.x, v.y); }
VD_CG_INL F3 fxyx2(F2 v) { return fm3(v.x, v.y, v.x); }
VD_CG_INL F3 fxyy2(F2 v) { return fm3(v.x, v.y, v.y); }
VD_CG_INL F3 fyxx2(F2 v) { return fm3(v.y, v.x, v.x); }
VD_CG_INL F3 fyxy2(F2 v) { return fm3(v.y, v.x, v.y); }
VD_CG_INL F3 fyyx2(F2 v) { return fm3(v.y, v.y, v.x); }
VD_CG_INL F3 fyyy2(F2 v) { return fm3(v.y, v.y, v.y); }
VD_CG_INL F4 fxxxx2(F2 v) { return fm4(v.x, v.x, v.x, v.x); }
VD_CG_INL F4 fxxxy2(F2 v) { return fm4(v.x, v.x, v.x, v.y); }
VD_CG_INL F4 fxxyx2(F2 v) { return fm4(v.x, v.x, v.y, v.x); }
VD_CG_INL F4 fxxyy2(F2 v) { return fm4(v.x, v.x, v.y, v.y); }
VD_CG_INL F4 fxyxx2(F2 v) { return fm4(v.x, v.y, v.x, v.x); }
VD_CG_INL F4 fxyxy2(F2 v) { return fm4(v.x, v.y, v.x, v.y); }
VD_CG_INL F4 fxyyx2(F2 v) { return fm4(v.x, v.y, v.y, v.x); }
VD_CG_INL F4 fxyyy2(F2 v) { return fm4(v.x, v.y, v.y, v.y); }
VD_CG_INL F4 fyxxx2(F2 v) { return fm4(v.y, v.x, v.x, v.x); }
VD_CG_INL F4 fyxxy2(F2 v) { return fm4(v.y, v.x, v.x, v.y); }
VD_CG_INL F4 fyxyx2(F2 v) { return fm4(v.y, v.x, v.y, v.x); }
VD_CG_INL F4 fyxyy2(F2 v) { return fm4(v.y, v.x, v.y, v.y); }
VD_CG_INL F4 fyyxx2(F2 v) { return fm4(v.y, v.y, v.x, v.x); }
VD_CG_INL F4 fyyxy2(F2 v) { return fm4(v.y, v.y, v.x, v.y); }
VD_CG_INL F4 fyyyx2(F2 v) { return fm4(v.y, v.y, v.y, v.x); }
VD_CG_INL F4 fyyyy2(F2 v) { return fm4(v.y, v.y, v.y, v.y); }
VD_CG_INL F2 fxx3(F3 v) { return fm2(v.x, v.x); }
VD_CG_INL F2 fxy3(F3 v) { return fm2(v.x, v.y); }
VD_CG_INL F2 fxz3(F3 v) { return fm2(v.x, v.z); }
VD_CG_INL F2 fyx3(F3 v) { return fm2(v.y, v.x); }
VD_CG_INL F2 fyy3(F3 v) { return fm2(v.y, v.y); }
VD_CG_INL F2 fyz3(F3 v) { return fm2(v.y, v.z); }
VD_CG_INL F2 fzx3(F3 v) { return fm2(v.z, v.x); }
VD_CG_INL F2 fzy3(F3 v) { return fm2(v.z, v.y); }
VD_CG_INL F2 fzz3(F3 v) { return fm2(v.z, v.z); }
VD_CG_INL F3 fxxx3(F3 v) { return fm3(v.x, v.x, v.x); }
VD_CG_INL F3 fxxy3(F3 v) { return fm3(v.x, v.x, v.y); }
VD_CG_INL F3 fxxz3(F3 v) { return fm3(v.x, v.x, v.z); }
VD_CG_INL F3 fxyx3(F3 v) { return fm3(v.x, v.y, v.x); }
VD_CG_INL F3 fxyy3(F3 v) { return fm3(v.x, v.y, v.y); }
VD_CG_INL F3 fxyz3(F3 v) { return fm3(v.x, v.y, v.z); }
VD_CG_INL F3 fxzx3(F3 v) { return fm3(v.x, v.z, v.x); }
VD_CG_INL F3 fxzy3(F3 v) { return fm3(v.x, v.z, v.y); }
VD_CG_INL F3 fxzz3(F3 v) { return fm3(v.x, v.z, v.z); }
VD_CG_INL F3 fyxx3(F3 v) { return fm3(v.y, v.x, v.x); }
VD_CG_INL F3 fyxy3(F3 v) { return fm3(v.y, v.x, v.y); }
VD_CG_INL F3 fyxz3(F3 v) { return fm3(v.y, v.x, v.z); }
VD_CG_INL F3 fyyx3(F3 v) { return fm3(v.y, v.y, v.x); }
VD_CG_INL F3 fyyy3(F3 v) { return fm3(v.y, v.y, v.y); }
VD_CG_INL F3 fyyz3(F3 v) { return fm3(v.y, v.y, v.z); }
VD_CG_INL F3 fyzx3(F3 v) { return fm3(v.y, v.z, v.x); }
VD_CG_INL F3 fyzy3(F3 v) { return fm3(v.y, v.z, v.y); }
VD_CG_INL F3 fyzz3(F3 v) { return fm3(v.y, v.z, v.z); }
VD_CG_INL F3 fzxx3(F3 v) { return fm3(v.z, v.x, v.x); }
VD_CG_INL F3 fzxy3(F3 v) { return fm3(v.z, v.x, v.y); }
VD_CG_INL F3 fzxz3(F3 v) { return fm3(v.z, v.x, v.z); }
VD_CG_INL F3 fzyx3(F3 v) { return fm3(v.z, v.y, v.x); }
VD_CG_INL F3 fzyy3(F3 v) { return fm3(v.z, v.y, v.y); }
VD_CG_INL F3 fzyz3(F3 v) { return fm3(v.z, v.y, v.z); }
VD_CG_INL F3 fzzx3(F3 v) { return fm3(v.z, v.z, v.x); }
VD_CG_INL F3 fzzy3(F3 v) { return fm3(v.z, v.z, v.y); }
VD_CG_INL F3 fzzz3(F3 v) { return fm3(v.z, v.z, v.z); }
VD_CG_INL F4 fxxxx3(F3 v) { return fm4(v.x, v.x, v.x, v.x); }
VD_CG_INL F4 fxxxy3(F3 v) { return fm4(v.x, v.x, v.x, v.y); }
VD_CG_INL F4 fxxxz3(F3 v) { return fm4(v.x, v.x, v.x, v.z); }
VD_CG_INL F4 fxxyx3(F3 v) { return fm4(v.x, v.x, v.y, v.x); }
VD_CG_INL F4 fxxyy3(F3 v) { return fm4(v.x, v.x, v.y, v.y); }
VD_CG_INL F4 fxxyz3(F3 v) { return fm4(v.x, v.x, v.y, v.z); }
VD_CG_INL F4 fxxzx3(F3 v) { return fm4(v.x, v.x, v.z, v.x); }
VD_CG_INL F4 fxxzy3(F3 v) { return fm4(v.x, v.x, v.z, v.y); }
VD_CG_INL F4 fxxzz3(F3 v) { return fm4(v.x, v.x, v.z, v.z); }
VD_CG_INL F4 fxyxx3(F3 v) { return fm4(v.x, v.y, v.x, v.x); }
VD_CG_INL F4 fxyxy3(F3 v) { return fm4(v.x, v.y, v.x, v.y); }
VD_CG_INL F4 fxyxz3(F3 v) { return fm4(v.x, v.y, v.x, v.z); }
VD_CG_INL F4 fxyyx3(F3 v) { return fm4(v.x, v.y, v.y, v.x); }
VD_CG_INL F4 fxyyy3(F3 v) { return fm4(v.x, v.y, v.y, v.y); }
VD_CG_INL F4 fxyyz3(F3 v) { return fm4(v.x, v.y, v.y, v.z); }
VD_CG_INL F4 fxyzx3(F3 v) { return fm4(v.x, v.y, v.z, v.x); }
VD_CG_INL F4 fxyzy3(F3 v) { return fm4(v.x, v.y, v.z, v.y); }
VD_CG_INL F4 fxyzz3(F3 v) { return fm4(v.x, v.y, v.z, v.z); }
VD_CG_INL F4 fxzxx3(F3 v) { return fm4(v.x, v.z, v.x, v.x); }
VD_CG_INL F4 fxzxy3(F3 v) { return fm4(v.x, v.z, v.x, v.y); }
VD_CG_INL F4 fxzxz3(F3 v) { return fm4(v.x, v.z, v.x, v.z); }
VD_CG_INL F4 fxzyx3(F3 v) { return fm4(v.x, v.z, v.y, v.x); }
VD_CG_INL F4 fxzyy3(F3 v) { return fm4(v.x, v.z, v.y, v.y); }
VD_CG_INL F4 fxzyz3(F3 v) { return fm4(v.x, v.z, v.y, v.z); }
VD_CG_INL F4 fxzzx3(F3 v) { return fm4(v.x, v.z, v.z, v.x); }
VD_CG_INL F4 fxzzy3(F3 v) { return fm4(v.x, v.z, v.z, v.y); }
VD_CG_INL F4 fxzzz3(F3 v) { return fm4(v.x, v.z, v.z, v.z); }
VD_CG_INL F4 fyxxx3(F3 v) { return fm4(v.y, v.x, v.x, v.x); }
VD_CG_INL F4 fyxxy3(F3 v) { return fm4(v.y, v.x, v.x, v.y); }
VD_CG_INL F4 fyxxz3(F3 v) { return fm4(v.y, v.x, v.x, v.z); }
VD_CG_INL F4 fyxyx3(F3 v) { return fm4(v.y, v.x, v.y, v.x); }
VD_CG_INL F4 fyxyy3(F3 v) { return fm4(v.y, v.x, v.y, v.y); }
VD_CG_INL F4 fyxyz3(F3 v) { return fm4(v.y, v.x, v.y, v.z); }
VD_CG_INL F4 fyxzx3(F3 v) { return fm4(v.y, v.x, v.z, v.x); }
VD_CG_INL F4 fyxzy3(F3 v) { return fm4(v.y, v.x, v.z, v.y); }
VD_CG_INL F4 fyxzz3(F3 v) { return fm4(v.y, v.x, v.z, v.z); }
VD_CG_INL F4 fyyxx3(F3 v) { return fm4(v.y, v.y, v.x, v.x); }
VD_CG_INL F4 fyyxy3(F3 v) { return fm4(v.y, v.y, v.x, v.y); }
VD_CG_INL F4 fyyxz3(F3 v) { return fm4(v.y, v.y, v.x, v.z); }
VD_CG_INL F4 fyyyx3(F3 v) { return fm4(v.y, v.y, v.y, v.x); }
VD_CG_INL F4 fyyyy3(F3 v) { return fm4(v.y, v.y, v.y, v.y); }
VD_CG_INL F4 fyyyz3(F3 v) { return fm4(v.y, v.y, v.y, v.z); }
VD_CG_INL F4 fyyzx3(F3 v) { return fm4(v.y, v.y, v.z, v.x); }
VD_CG_INL F4 fyyzy3(F3 v) { return fm4(v.y, v.y, v.z, v.y); }
VD_CG_INL F4 fyyzz3(F3 v) { return fm4(v.y, v.y, v.z, v.z); }
VD_CG_INL F4 fyzxx3(F3 v) { return fm4(v.y, v.z, v.x, v.x); }
VD_CG_INL F4 fyzxy3(F3 v) { return fm4(v.y, v.z, v.x, v.y); }
VD_CG_INL F4 fyzxz3(F3 v) { return fm4(v.y, v.z, v.x, v.z); }
VD_CG_INL F4 fyzyx3(F3 v) { return fm4(v.y, v.z, v.y, v.x); }
VD_CG_INL F4 fyzyy3(F3 v) { return fm4(v.y, v.z, v.y, v.y); }
VD_CG_INL F4 fyzyz3(F3 v) { return fm4(v.y, v.z, v.y, v.z); }
VD_CG_INL F4 fyzzx3(F3 v) { return fm4(v.y, v.z, v.z, v.x); }
VD_CG_INL F4 fyzzy3(F3 v) { return fm4(v.y, v.z, v.z, v.y); }
VD_CG_INL F4 fyzzz3(F3 v) { return fm4(v.y, v.z, v.z, v.z); }
VD_CG_INL F4 fzxxx3(F3 v) { return fm4(v.z, v.x, v.x, v.x); }
VD_CG_INL F4 fzxxy3(F3 v) { return fm4(v.z, v.x, v.x, v.y); }
VD_CG_INL F4 fzxxz3(F3 v) { return fm4(v.z, v.x, v.x, v.z); }
VD_CG_INL F4 fzxyx3(F3 v) { return fm4(v.z, v.x, v.y, v.x); }
VD_CG_INL F4 fzxyy3(F3 v) { return fm4(v.z, v.x, v.y, v.y); }
VD_CG_INL F4 fzxyz3(F3 v) { return fm4(v.z, v.x, v.y, v.z); }
VD_CG_INL F4 fzxzx3(F3 v) { return fm4(v.z, v.x, v.z, v.x); }
VD_CG_INL F4 fzxzy3(F3 v) { return fm4(v.z, v.x, v.z, v.y); }
VD_CG_INL F4 fzxzz3(F3 v) { return fm4(v.z, v.x, v.z, v.z); }
VD_CG_INL F4 fzyxx3(F3 v) { return fm4(v.z, v.y, v.x, v.x); }
VD_CG_INL F4 fzyxy3(F3 v) { return fm4(v.z, v.y, v.x, v.y); }
VD_CG_INL F4 fzyxz3(F3 v) { return fm4(v.z, v.y, v.x, v.z); }
VD_CG_INL F4 fzyyx3(F3 v) { return fm4(v.z, v.y, v.y, v.x); }
VD_CG_INL F4 fzyyy3(F3 v) { return fm4(v.z, v.y, v.y, v.y); }
VD_CG_INL F4 fzyyz3(F3 v) { return fm4(v.z, v.y, v.y, v.z); }
VD_CG_INL F4 fzyzx3(F3 v) { return fm4(v.z, v.y, v.z, v.x); }
VD_CG_INL F4 fzyzy3(F3 v) { return fm4(v.z, v.y, v.z, v.y); }
VD_CG_INL F4 fzyzz3(F3 v) { return fm4(v.z, v.y, v.z, v.z); }
VD_CG_INL F4 fzzxx3(F3 v) { return fm4(v.z, v.z, v.x, v.x); }
VD_CG_INL F4 fzzxy3(F3 v) { return fm4(v.z, v.z, v.x, v.y); }
VD_CG_INL F4 fzzxz3(F3 v) { return fm4(v.z, v.z, v.x, v.z); }
VD_CG_INL F4 fzzyx3(F3 v) { return fm4(v.z, v.z, v.y, v.x); }
VD_CG_INL F4 fzzyy3(F3 v) { return fm4(v.z, v.z, v.y, v.y); }
VD_CG_INL F4 fzzyz3(F3 v) { return fm4(v.z, v.z, v.y, v.z); }
VD_CG_INL F4 fzzzx3(F3 v) { return fm4(v.z, v.z, v.z, v.x); }
VD_CG_INL F4 fzzzy3(F3 v) { return fm4(v.z, v.z, v.z, v.y); }
VD_CG_INL F4 fzzzz3(F3 v) { return fm4(v.z, v.z, v.z, v.z); }
VD_CG_INL F2 fxx4(F4 v) { return fm2(v.x, v.x); }
VD_CG_INL F2 fxy4(F4 v) { return fm2(v.x, v.y); }
VD_CG_INL F2 fxz4(F4 v) { return fm2(v.x, v.z); }
VD_CG_INL F2 fxw4(F4 v) { return fm2(v.x, v.w); }
VD_CG_INL F2 fyx4(F4 v) { return fm2(v.y, v.x); }
VD_CG_INL F2 fyy4(F4 v) { return fm2(v.y, v.y); }
VD_CG_INL F2 fyz4(F4 v) { return fm2(v.y, v.z); }
VD_CG_INL F2 fyw4(F4 v) { return fm2(v.y, v.w); }
VD_CG_INL F2 fzx4(F4 v) { return fm2(v.z, v.x); }
VD_CG_INL F2 fzy4(F4 v) { return fm2(v.z, v.y); }
VD_CG_INL F2 fzz4(F4 v) { return fm2(v.z, v.z); }
VD_CG_INL F2 fzw4(F4 v) { return fm2(v.z, v.w); }
VD_CG_INL F2 fwx4(F4 v) { return fm2(v.w, v.x); }
VD_CG_INL F2 fwy4(F4 v) { return fm2(v.w, v.y); }
VD_CG_INL F2 fwz4(F4 v) { return fm2(v.w, v.z); }
VD_CG_INL F2 fww4(F4 v) { return fm2(v.w, v.w); }
VD_CG_INL F3 fxxx4(F4 v) { return fm3(v.x, v.x, v.x); }
VD_CG_INL F3 fxxy4(F4 v) { return fm3(v.x, v.x, v.y); }
VD_CG_INL F3 fxxz4(F4 v) { return fm3(v.x, v.x, v.z); }
VD_CG_INL F3 fxxw4(F4 v) { return fm3(v.x, v.x, v.w); }
VD_CG_INL F3 fxyx4(F4 v) { return fm3(v.x, v.y, v.x); }
VD_CG_INL F3 fxyy4(F4 v) { return fm3(v.x, v.y, v.y); }
VD_CG_INL F3 fxyz4(F4 v) { return fm3(v.x, v.y, v.z); }
VD_CG_INL F3 fxyw4(F4 v) { return fm3(v.x, v.y, v.w); }
VD_CG_INL F3 fxzx4(F4 v) { return fm3(v.x, v.z, v.x); }
VD_CG_INL F3 fxzy4(F4 v) { return fm3(v.x, v.z, v.y); }
VD_CG_INL F3 fxzz4(F4 v) { return fm3(v.x, v.z, v.z); }
VD_CG_INL F3 fxzw4(F4 v) { return fm3(v.x, v.z, v.w); }
VD_CG_INL F3 fxwx4(F4 v) { return fm3(v.x, v.w, v.x); }
VD_CG_INL F3 fxwy4(F4 v) { return fm3(v.x, v.w, v.y); }
VD_CG_INL F3 fxwz4(F4 v) { return fm3(v.x, v.w, v.z); }
VD_CG_INL F3 fxww4(F4 v) { return fm3(v.x, v.w, v.w); }
VD_CG_INL F3 fyxx4(F4 v) { return fm3(v.y, v.x, v.x); }
VD_CG_INL F3 fyxy4(F4 v) { return fm3(v.y, v.x, v.y); }
VD_CG_INL F3 fyxz4(F4 v) { return fm3(v.y, v.x, v.z); }
VD_CG_INL F3 fyxw4(F4 v) { return fm3(v.y, v.x, v.w); }
VD_CG_INL F3 fyyx4(F4 v) { return fm3(v.y, v.y, v.x); }
VD_CG_INL F3 fyyy4(F4 v) { return fm3(v.y, v.y, v.y); }
VD_CG_INL F3 fyyz4(F4 v) { return fm3(v.y, v.y, v.z); }
VD_CG_INL F3 fyyw4(F4 v) { return fm3(v.y, v.y, v.w); }
VD_CG_INL F3 fyzx4(F4 v) { return fm3(v.y, v.z, v.x); }
VD_CG_INL F3 fyzy4(F4 v) { return fm3(v.y, v.z, v.y); }
VD_CG_INL F3 fyzz4(F4 v) { return fm3(v.y, v.z, v.z); }
VD_CG_INL F3 fyzw4(F4 v) { return fm3(v.y, v.z, v.w); }
VD_CG_INL F3 fywx4(F4 v) { return fm3(v.y, v.w, v.x); }
VD_CG_INL F3 fywy4(F4 v) { return fm3(v.y, v.w, v.y); }
VD_CG_INL F3 fywz4(F4 v) { return fm3(v.y, v.w, v.z); }
VD_CG_INL F3 fyww4(F4 v) { return fm3(v.y, v.w, v.w); }
VD_CG_INL F3 fzxx4(F4 v) { return fm3(v.z, v.x, v.x); }
VD_CG_INL F3 fzxy4(F4 v) { return fm3(v.z, v.x, v.y); }
VD_CG_INL F3 fzxz4(F4 v) { return fm3(v.z, v.x, v.z); }
VD_CG_INL F3 fzxw4(F4 v) { return fm3(v.z, v.x, v.w); }
VD_CG_INL F3 fzyx4(F4 v) { return fm3(v.z, v.y, v.x); }
VD_CG_INL F3 fzyy4(F4 v) { return fm3(v.z, v.y, v.y); }
VD_CG_INL F3 fzyz4(F4 v) { return fm3(v.z, v.y, v.z); }
VD_CG_INL F3 fzyw4(F4 v) { return fm3(v.z, v.y, v.w); }
VD_CG_INL F3 fzzx4(F4 v) { return fm3(v.z, v.z, v.x); }
VD_CG_INL F3 fzzy4(F4 v) { return fm3(v.z, v.z, v.y); }
VD_CG_INL F3 fzzz4(F4 v) { return fm3(v.z, v.z, v.z); }
VD_CG_INL F3 fzzw4(F4 v) { return fm3(v.z, v.z, v.w); }
VD_CG_INL F3 fzwx4(F4 v) { return fm3(v.z, v.w, v.x); }
VD_CG_INL F3 fzwy4(F4 v) { return fm3(v.z, v.w, v.y); }
VD_CG_INL F3 fzwz4(F4 v) { return fm3(v.z, v.w, v.z); }
VD_CG_INL F3 fzww4(F4 v) { return fm3(v.z, v.w, v.w); }
VD_CG_INL F3 fwxx4(F4 v) { return fm3(v.w, v.x, v.x); }
VD_CG_INL F3 fwxy4(F4 v) { return fm3(v.w, v.x, v.y); }
VD_CG_INL F3 fwxz4(F4 v) { return fm3(v.w, v.x, v.z); }
VD_CG_INL F3 fwxw4(F4 v) { return fm3(v.w, v.x, v.w); }
VD_CG_INL F3 fwyx4(F4 v) { return fm3(v.w, v.y, v.x); }
VD_CG_INL F3 fwyy4(F4 v) { return fm3(v.w, v.y, v.y); }
VD_CG_INL F3 fwyz4(F4 v) { return fm3(v.w, v.y, v.z); }
VD_CG_INL F3 fwyw4(F4 v) { return fm3(v.w, v.y, v.w); }
VD_CG_INL F3 fwzx4(F4 v) { return fm3(v.w, v.z, v.x); }
VD_CG_INL F3 fwzy4(F4 v) { return fm3(v.w, v.z, v.y); }
VD_CG_INL F3 fwzz4(F4 v) { return fm3(v.w, v.z, v.z); }
VD_CG_INL F3 fwzw4(F4 v) { return fm3(v.w, v.z, v.w); }
VD_CG_INL F3 fwwx4(F4 v) { return fm3(v.w, v.w, v.x); }
VD_CG_INL F3 fwwy4(F4 v) { return fm3(v.w, v.w, v.y); }
VD_CG_INL F3 fwwz4(F4 v) { return fm3(v.w, v.w, v.z); }
VD_CG_INL F3 fwww4(F4 v) { return fm3(v.w, v.w, v.w); }
VD_CG_INL F4 fxxxx4(F4 v) { return fm4(v.x, v.x, v.x, v.x); }
VD_CG_INL F4 fxxxy4(F4 v) { return fm4(v.x, v.x, v.x, v.y); }
VD_CG_INL F4 fxxxz4(F4 v) { return fm4(v.x, v.x, v.x, v.z); }
VD_CG_INL F4 fxxxw4(F4 v) { return fm4(v.x, v.x, v.x, v.w); }
VD_CG_INL F4 fxxyx4(F4 v) { return fm4(v.x, v.x, v.y, v.x); }
VD_CG_INL F4 fxxyy4(F4 v) { return fm4(v.x, v.x, v.y, v.y); }
VD_CG_INL F4 fxxyz4(F4 v) { return fm4(v.x, v.x, v.y, v.z); }
VD_CG_INL F4 fxxyw4(F4 v) { return fm4(v.x, v.x, v.y, v.w); }
VD_CG_INL F4 fxxzx4(F4 v) { return fm4(v.x, v.x, v.z, v.x); }
VD_CG_INL F4 fxxzy4(F4 v) { return fm4(v.x, v.x, v.z, v.y); }
VD_CG_INL F4 fxxzz4(F4 v) { return fm4(v.x, v.x, v.z, v.z); }
VD_CG_INL F4 fxxzw4(F4 v) { return fm4(v.x, v.x, v.z, v.w); }
VD_CG_INL F4 fxxwx4(F4 v) { return fm4(v.x, v.x, v.w, v.x); }
VD_CG_INL F4 fxxwy4(F4 v) { return fm4(v.x, v.x, v.w, v.y); }
VD_CG_INL F4 fxxwz4(F4 v) { return fm4(v.x, v.x, v.w, v.z); }
VD_CG_INL F4 fxxww4(F4 v) { return fm4(v.x, v.x, v.w, v.w); }
VD_CG_INL F4 fxyxx4(F4 v) { return fm4(v.x, v.y, v.x, v.x); }
VD_CG_INL F4 fxyxy4(F4 v) { return fm4(v.x, v.y, v.x, v.y); }
VD_CG_INL F4 fxyxz4(F4 v) { return fm4(v.x, v.y, v.x, v.z); }
VD_CG_INL F4 fxyxw4(F4 v) { return fm4(v.x, v.y, v.x, v.w); }
VD_CG_INL F4 fxyyx4(F4 v) { return fm4(v.x, v.y, v.y, v.x); }
VD_CG_INL F4 fxyyy4(F4 v) { return fm4(v.x, v.y, v.y, v.y); }
VD_CG_INL F4 fxyyz4(F4 v) { return fm4(v.x, v.y, v.y, v.z); }
VD_CG_INL F4 fxyyw4(F4 v) { return fm4(v.x, v.y, v.y, v.w); }
VD_CG_INL F4 fxyzx4(F4 v) { return fm4(v.x, v.y, v.z, v.x); }
VD_CG_INL F4 fxyzy4(F4 v) { return fm4(v.x, v.y, v.z, v.y); }
VD_CG_INL F4 fxyzz4(F4 v) { return fm4(v.x, v.y, v.z, v.z); }
VD_CG_INL F4 fxyzw4(F4 v) { return fm4(v.x, v.y, v.z, v.w); }
VD_CG_INL F4 fxywx4(F4 v) { return fm4(v.x, v.y, v.w, v.x); }
VD_CG_INL F4 fxywy4(F4 v) { return fm4(v.x, v.y, v.w, v.y); }
VD_CG_INL F4 fxywz4(F4 v) { return fm4(v.x, v.y, v.w, v.z); }
VD_CG_INL F4 fxyww4(F4 v) { return fm4(v.x, v.y, v.w, v.w); }
VD_CG_INL F4 fxzxx4(F4 v) { return fm4(v.x, v.z, v.x, v.x); }
VD_CG_INL F4 fxzxy4(F4 v) { return fm4(v.x, v.z, v.x, v.y); }
VD_CG_INL F4 fxzxz4(F4 v) { return fm4(v.x, v.z, v.x, v.z); }
VD_CG_INL F4 fxzxw4(F4 v) { return fm4(v.x, v.z, v.x, v.w); }
VD_CG_INL F4 fxzyx4(F4 v) { return fm4(v.x, v.z, v.y, v.x); }
VD_CG_INL F4 fxzyy4(F4 v) { return fm4(v.x, v.z, v.y, v.y); }
VD_CG_INL F4 fxzyz4(F4 v) { return fm4(v.x, v.z, v.y, v.z); }
VD_CG_INL F4 fxzyw4(F4 v) { return fm4(v.x, v.z, v.y, v.w); }
VD_CG_INL F4 fxzzx4(F4 v) { return fm4(v.x, v.z, v.z, v.x); }
VD_CG_INL F4 fxzzy4(F4 v) { return fm4(v.x, v.z, v.z, v.y); }
VD_CG_INL F4 fxzzz4(F4 v) { return fm4(v.x, v.z, v.z, v.z); }
VD_CG_INL F4 fxzzw4(F4 v) { return fm4(v.x, v.z, v.z, v.w); }
VD_CG_INL F4 fxzwx4(F4 v) { return fm4(v.x, v.z, v.w, v.x); }
VD_CG_INL F4 fxzwy4(F4 v) { return fm4(v.x, v.z, v.w, v.y); }
VD_CG_INL F4 fxzwz4(F4 v) { return fm4(v.x, v.z, v.w, v.z); }
VD_CG_INL F4 fxzww4(F4 v) { return fm4(v.x, v.z, v.w, v.w); }
VD_CG_INL F4 fxwxx4(F4 v) { return fm4(v.x, v.w, v.x, v.x); }
VD_CG_INL F4 fxwxy4(F4 v) { return fm4(v.x, v.w, v.x, v.y); }
VD_CG_INL F4 fxwxz4(F4 v) { return fm4(v.x, v.w, v.x, v.z); }
VD_CG_INL F4 fxwxw4(F4 v) { return fm4(v.x, v.w, v.x, v.w); }
VD_CG_INL F4 fxwyx4(F4 v) { return fm4(v.x, v.w, v.y, v.x); }
VD_CG_INL F4 fxwyy4(F4 v) { return fm4(v.x, v.w, v.y, v.y); }
VD_CG_INL F4 fxwyz4(F4 v) { return fm4(v.x, v.w, v.y, v.z); }
VD_CG_INL F4 fxwyw4(F4 v) { return fm4(v.x, v.w, v.y, v.w); }
VD_CG_INL F4 fxwzx4(F4 v) { return fm4(v.x, v.w, v.z, v.x); }
VD_CG_INL F4 fxwzy4(F4 v) { return fm4(v.x, v.w, v.z, v.y); }
VD_CG_INL F4 fxwzz4(F4 v) { return fm4(v.x, v.w, v.z, v.z); }
VD_CG_INL F4 fxwzw4(F4 v) { return fm4(v.x, v.w, v.z, v.w); }
VD_CG_INL F4 fxwwx4(F4 v) { return fm4(v.x, v.w, v.w, v.x); }
VD_CG_INL F4 fxwwy4(F4 v) { return fm4(v.x, v.w, v.w, v.y); }
VD_CG_INL F4 fxwwz4(F4 v) { return fm4(v.x, v.w, v.w, v.z); }
VD_CG_INL F4 fxwww4(F4 v) { return fm4(v.x, v.w, v.w, v.w); }
VD_CG_INL F4 fyxxx4(F4 v) { return fm4(v.y, v.x, v.x, v.x); }
VD_CG_INL F4 fyxxy4(F4 v) { return fm4(v.y, v.x, v.x, v.y); }
VD_CG_INL F4 fyxxz4(F4 v) { return fm4(v.y, v.x, v.x, v.z); }
VD_CG_INL F4 fyxxw4(F4 v) { return fm4(v.y, v.x, v.x, v.w); }
VD_CG_INL F4 fyxyx4(F4 v) { return fm4(v.y, v.x, v.y, v.x); }
VD_CG_INL F4 fyxyy4(F4 v) { return fm4(v.y, v.x, v.y, v.y); }
VD_CG_INL F4 fyxyz4(F4 v) { return fm4(v.y, v.x, v.y, v.z); }
VD_CG_INL F4 fyxyw4(F4 v) { return fm4(v.y, v.x, v.y, v.w); }
VD_CG_INL F4 fyxzx4(F4 v) { return fm4(v.y, v.x, v.z, v.x); }
VD_CG_INL F4 fyxzy4(F4 v) { return fm4(v.y, v.x, v.z, v.y); }
VD_CG_INL F4 fyxzz4(F4 v) { return fm4(v.y, v.x, v.z, v.z); }
VD_CG_INL F4 fyxzw4(F4 v) { return fm4(v.y, v.x, v.z, v.w); }
VD_CG_INL F4 fyxwx4(F4 v) { return fm4(v.y, v.x, v.w, v.x); }
VD_CG_INL F4 fyxwy4(F4 v) { return fm4(v.y, v.x, v.w, v.y); }
VD_CG_INL F4 fyxwz4(F4 v) { return fm4(v.y, v.x, v.w, v.z); }
VD_CG_INL F4 fyxww4(F4 v) { return fm4(v.y, v.x, v.w, v.w); }
VD_CG_INL F4 fyyxx4(F4 v) { return fm4(v.y, v.y, v.x, v.x); }
VD_CG_INL F4 fyyxy4(F4 v) { return fm4(v.y, v.y, v.x, v.y); }
VD_CG_INL F4 fyyxz4(F4 v) { return fm4(v.y, v.y, v.x, v.z); }
VD_CG_INL F4 fyyxw4(F4 v) { return fm4(v.y, v.y, v.x, v.w); }
VD_CG_INL F4 fyyyx4(F4 v) { return fm4(v.y, v.y, v.y, v.x); }
VD_CG_INL F4 fyyyy4(F4 v) { return fm4(v.y, v.y, v.y, v.y); }
VD_CG_INL F4 fyyyz4(F4 v) { return fm4(v.y, v.y, v.y, v.z); }
VD_CG_INL F4 fyyyw4(F4 v) { return fm4(v.y, v.y, v.y, v.w); }
VD_CG_INL F4 fyyzx4(F4 v) { return fm4(v.y, v.y, v.z, v.x); }
VD_CG_INL F4 fyyzy4(F4 v) { return fm4(v.y, v.y, v.z, v.y); }
VD_CG_INL F4 fyyzz4(F4 v) { return fm4(v.y, v.y, v.z, v.z); }
VD_CG_INL F4 fyyzw4(F4 v) { return fm4(v.y, v.y, v.z, v.w); }
VD_CG_INL F4 fyywx4(F4 v) { return fm4(v.y, v.y, v.w, v.x); }
VD_CG_INL F4 fyywy4(F4 v) { return fm4(v.y, v.y, v.w, v.y); }
VD_CG_INL F4 fyywz4(F4 v) { return fm4(v.y, v.y, v.w, v.z); }
VD_CG_INL F4 fyyww4(F4 v) { return fm4(v.y, v.y, v.w, v.w); }
VD_CG_INL F4 fyzxx4(F4 v) { return fm4(v.y, v.z, v.x, v.x); }
VD_CG_INL F4 fyzxy4(F4 v) { return fm4(v.y, v.z, v.x, v.y); }
VD_CG_INL F4 fyzxz4(F4 v) { return fm4(v.y, v.z, v.x, v.z); }
VD_CG_INL F4 fyzxw4(F4 v) { return fm4(v.y, v.z, v.x, v.w); }
VD_CG_INL F4 fyzyx4(F4 v) { return fm4(v.y, v.z, v.y, v.x); }
VD_CG_INL F4 fyzyy4(F4 v) { return fm4(v.y, v.z, v.y, v.y); }
VD_CG_INL F4 fyzyz4(F4 v) { return fm4(v.y, v.z, v.y, v.z); }
VD_CG_INL F4 fyzyw4(F4 v) { return fm4(v.y, v.z, v.y, v.w); }
VD_CG_INL F4 fyzzx4(F4 v) { return fm4(v.y, v.z, v.z, v.x); }
VD_CG_INL F4 fyzzy4(F4 v) { return fm4(v.y, v.z, v.z, v.y); }
VD_CG_INL F4 fyzzz4(F4 v) { return fm4(v.y, v.z, v.z, v.z); }
VD_CG_INL F4 fyzzw4(F4 v) { return fm4(v.y, v.z, v.z, v.w); }
VD_CG_INL F4 fyzwx4(F4 v) { return fm4(v.y, v.z, v.w, v.x); }
VD_CG_INL F4 fyzwy4(F4 v) { return fm4(v.y, v.z, v.w, v.y); }
VD_CG_INL F4 fyzwz4(F4 v) { return fm4(v.y, v.z, v.w, v.z); }
VD_CG_INL F4 fyzww4(F4 v) { return fm4(v.y, v.z, v.w, v.w); }
VD_CG_INL F4 fywxx4(F4 v) { return fm4(v.y, v.w, v.x, v.x); }
VD_CG_INL F4 fywxy4(F4 v) { return fm4(v.y, v.w, v.x, v.y); }
VD_CG_INL F4 fywxz4(F4 v) { return fm4(v.y, v.w, v.x, v.z); }
VD_CG_INL F4 fywxw4(F4 v) { return fm4(v.y, v.w, v.x, v.w); }
VD_CG_INL F4 fywyx4(F4 v) { return fm4(v.y, v.w, v.y, v.x); }
VD_CG_INL F4 fywyy4(F4 v) { return fm4(v.y, v.w, v.y, v.y); }
VD_CG_INL F4 fywyz4(F4 v) { return fm4(v.y, v.w, v.y, v.z); }
VD_CG_INL F4 fywyw4(F4 v) { return fm4(v.y, v.w, v.y, v.w); }
VD_CG_INL F4 fywzx4(F4 v) { return fm4(v.y, v.w, v.z, v.x); }
VD_CG_INL F4 fywzy4(F4 v) { return fm4(v.y, v.w, v.z, v.y); }
VD_CG_INL F4 fywzz4(F4 v) { return fm4(v.y, v.w, v.z, v.z); }
VD_CG_INL F4 fywzw4(F4 v) { return fm4(v.y, v.w, v.z, v.w); }
VD_CG_INL F4 fywwx4(F4 v) { return fm4(v.y, v.w, v.w, v.x); }
VD_CG_INL F4 fywwy4(F4 v) { return fm4(v.y, v.w, v.w, v.y); }
VD_CG_INL F4 fywwz4(F4 v) { return fm4(v.y, v.w, v.w, v.z); }
VD_CG_INL F4 fywww4(F4 v) { return fm4(v.y, v.w, v.w, v.w); }
VD_CG_INL F4 fzxxx4(F4 v) { return fm4(v.z, v.x, v.x, v.x); }
VD_CG_INL F4 fzxxy4(F4 v) { return fm4(v.z, v.x, v.x, v.y); }
VD_CG_INL F4 fzxxz4(F4 v) { return fm4(v.z, v.x, v.x, v.z); }
VD_CG_INL F4 fzxxw4(F4 v) { return fm4(v.z, v.x, v.x, v.w); }
VD_CG_INL F4 fzxyx4(F4 v) { return fm4(v.z, v.x, v.y, v.x); }
VD_CG_INL F4 fzxyy4(F4 v) { return fm4(v.z, v.x, v.y, v.y); }
VD_CG_INL F4 fzxyz4(F4 v) { return fm4(v.z, v.x, v.y, v.z); }
VD_CG_INL F4 fzxyw4(F4 v) { return fm4(v.z, v.x, v.y, v.w); }
VD_CG_INL F4 fzxzx4(F4 v) { return fm4(v.z, v.x, v.z, v.x); }
VD_CG_INL F4 fzxzy4(F4 v) { return fm4(v.z, v.x, v.z, v.y); }
VD_CG_INL F4 fzxzz4(F4 v) { return fm4(v.z, v.x, v.z, v.z); }
VD_CG_INL F4 fzxzw4(F4 v) { return fm4(v.z, v.x, v.z, v.w); }
VD_CG_INL F4 fzxwx4(F4 v) { return fm4(v.z, v.x, v.w, v.x); }
VD_CG_INL F4 fzxwy4(F4 v) { return fm4(v.z, v.x, v.w, v.y); }
VD_CG_INL F4 fzxwz4(F4 v) { return fm4(v.z, v.x, v.w, v.z); }
VD_CG_INL F4 fzxww4(F4 v) { return fm4(v.z, v.x, v.w, v.w); }
VD_CG_INL F4 fzyxx4(F4 v) { return fm4(v.z, v.y, v.x, v.x); }
VD_CG_INL F4 fzyxy4(F4 v) { return fm4(v.z, v.y, v.x, v.y); }
VD_CG_INL F4 fzyxz4(F4 v) { return fm4(v.z, v.y, v.x, v.z); }
VD_CG_INL F4 fzyxw4(F4 v) { return fm4(v.z, v.y, v.x, v.w); }
VD_CG_INL F4 fzyyx4(F4 v) { return fm4(v.z, v.y, v.y, v.x); }
VD_CG_INL F4 fzyyy4(F4 v) { return fm4(v.z, v.y, v.y, v.y); }
VD_CG_INL F4 fzyyz4(F4 v) { return fm4(v.z, v.y, v.y, v.z); }
VD_CG_INL F4 fzyyw4(F4 v) { return fm4(v.z, v.y, v.y, v.w); }
VD_CG_INL F4 fzyzx4(F4 v) { return fm4(v.z, v.y, v.z, v.x); }
VD_CG_INL F4 fzyzy4(F4 v) { return fm4(v.z, v.y, v.z, v.y); }
VD_CG_INL F4 fzyzz4(F4 v) { return fm4(v.z, v.y, v.z, v.z); }
VD_CG_INL F4 fzyzw4(F4 v) { return fm4(v.z, v.y, v.z, v.w); }
VD_CG_INL F4 fzywx4(F4 v) { return fm4(v.z, v.y, v.w, v.x); }
VD_CG_INL F4 fzywy4(F4 v) { return fm4(v.z, v.y, v.w, v.y); }
VD_CG_INL F4 fzywz4(F4 v) { return fm4(v.z, v.y, v.w, v.z); }
VD_CG_INL F4 fzyww4(F4 v) { return fm4(v.z, v.y, v.w, v.w); }
VD_CG_INL F4 fzzxx4(F4 v) { return fm4(v.z, v.z, v.x, v.x); }
VD_CG_INL F4 fzzxy4(F4 v) { return fm4(v.z, v.z, v.x, v.y); }
VD_CG_INL F4 fzzxz4(F4 v) { return fm4(v.z, v.z, v.x, v.z); }
VD_CG_INL F4 fzzxw4(F4 v) { return fm4(v.z, v.z, v.x, v.w); }
VD_CG_INL F4 fzzyx4(F4 v) { return fm4(v.z, v.z, v.y, v.x); }
VD_CG_INL F4 fzzyy4(F4 v) { return fm4(v.z, v.z, v.y, v.y); }
VD_CG_INL F4 fzzyz4(F4 v) { return fm4(v.z, v.z, v.y, v.z); }
VD_CG_INL F4 fzzyw4(F4 v) { return fm4(v.z, v.z, v.y, v.w); }
VD_CG_INL F4 fzzzx4(F4 v) { return fm4(v.z, v.z, v.z, v.x); }
VD_CG_INL F4 fzzzy4(F4 v) { return fm4(v.z, v.z, v.z, v.y); }
VD_CG_INL F4 fzzzz4(F4 v) { return fm4(v.z, v.z, v.z, v.z); }
VD_CG_INL F4 fzzzw4(F4 v) { return fm4(v.z, v.z, v.z, v.w); }
VD_CG_INL F4 fzzwx4(F4 v) { return fm4(v.z, v.z, v.w, v.x); }
VD_CG_INL F4 fzzwy4(F4 v) { return fm4(v.z, v.z, v.w, v.y); }
VD_CG_INL F4 fzzwz4(F4 v) { return fm4(v.z, v.z, v.w, v.z); }
VD_CG_INL F4 fzzww4(F4 v) { return fm4(v.z, v.z, v.w, v.w); }
VD_CG_INL F4 fzwxx4(F4 v) { return fm4(v.z, v.w, v.x, v.x); }
VD_CG_INL F4 fzwxy4(F4 v) { return fm4(v.z, v.w, v.x, v.y); }
VD_CG_INL F4 fzwxz4(F4 v) { return fm4(v.z, v.w, v.x, v.z); }
VD_CG_INL F4 fzwxw4(F4 v) { return fm4(v.z, v.w, v.x, v.w); }
VD_CG_INL F4 fzwyx4(F4 v) { return fm4(v.z, v.w, v.y, v.x); }
VD_CG_INL F4 fzwyy4(F4 v) { return fm4(v.z, v.w, v.y, v.y); }
VD_CG_INL F4 fzwyz4(F4 v) { return fm4(v.z, v.w, v.y, v.z); }
VD_CG_INL F4 fzwyw4(F4 v) { return fm4(v.z, v.w, v.y, v.w); }
VD_CG_INL F4 fzwzx4(F4 v) { return fm4(v.z, v.w, v.z, v.x); }
VD_CG_INL F4 fzwzy4(F4 v) { return fm4(v.z, v.w, v.z, v.y); }
VD_CG_INL F4 fzwzz4(F4 v) { return fm4(v.z, v.w, v.z, v.z); }
VD_CG_INL F4 fzwzw4(F4 v) { return fm4(v.z, v.w, v.z, v.w); }
VD_CG_INL F4 fzwwx4(F4 v) { return fm4(v.z, v.w, v.w, v.x); }
VD_CG_INL F4 fzwwy4(F4 v) { return fm4(v.z, v.w, v.w, v.y); }
VD_CG_INL F4 fzwwz4(F4 v) { return fm4(v.z, v.w, v.w, v.z); }
VD_CG_INL F4 fzwww4(F4 v) { return fm4(v.z, v.w, v.w, v.w); }
VD_CG_INL F4 fwxxx4(F4 v) { return fm4(v.w, v.x, v.x, v.x); }
VD_CG_INL F4 fwxxy4(F4 v) { return fm4(v.w, v.x, v.x, v.y); }
VD_CG_INL F4 fwxxz4(F4 v) { return fm4(v.w, v.x, v.x, v.z); }
VD_CG_INL F4 fwxxw4(F4 v) { return fm4(v.w, v.x, v.x, v.w); }
VD_CG_INL F4 fwxyx4(F4 v) { return fm4(v.w, v.x, v.y, v.x); }
VD_CG_INL F4 fwxyy4(F4 v) { return fm4(v.w, v.x, v.y, v.y); }
VD_CG_INL F4 fwxyz4(F4 v) { return fm4(v.w, v.x, v.y, v.z); }
VD_CG_INL F4 fwxyw4(F4 v) { return fm4(v.w, v.x, v.y, v.w); }
VD_CG_INL F4 fwxzx4(F4 v) { return fm4(v.w, v.x, v.z, v.x); }
VD_CG_INL F4 fwxzy4(F4 v) { return fm4(v.w, v.x, v.z, v.y); }
VD_CG_INL F4 fwxzz4(F4 v) { return fm4(v.w, v.x, v.z, v.z); }
VD_CG_INL F4 fwxzw4(F4 v) { return fm4(v.w, v.x, v.z, v.w); }
VD_CG_INL F4 fwxwx4(F4 v) { return fm4(v.w, v.x, v.w, v.x); }
VD_CG_INL F4 fwxwy4(F4 v) { return fm4(v.w, v.x, v.w, v.y); }
VD_CG_INL F4 fwxwz4(F4 v) { return fm4(v.w, v.x, v.w, v.z); }
VD_CG_INL F4 fwxww4(F4 v) { return fm4(v.w, v.x, v.w, v.w); }
VD_CG_INL F4 fwyxx4(F4 v) { return fm4(v.w, v.y, v.x, v.x); }
VD_CG_INL F4 fwyxy4(F4 v) { return fm4(v.w, v.y, v.x, v.y); }
VD_CG_INL F4 fwyxz4(F4 v) { return fm4(v.w, v.y, v.x, v.z); }
VD_CG_INL F4 fwyxw4(F4 v) { return fm4(v.w, v.y, v.x, v.w); }
VD_CG_INL F4 fwyyx4(F4 v) { return fm4(v.w, v.y, v.y, v.x); }
VD_CG_INL F4 fwyyy4(F4 v) { return fm4(v.w, v.y, v.y, v.y); }
VD_CG_INL F4 fwyyz4(F4 v) { return fm4(v.w, v.y, v.y, v.z); }
VD_CG_INL F4 fwyyw4(F4 v) { return fm4(v.w, v.y, v.y, v.w); }
VD_CG_INL F4 fwyzx4(F4 v) { return fm4(v.w, v.y, v.z, v.x); }
VD_CG_INL F4 fwyzy4(F4 v) { return fm4(v.w, v.y, v.z, v.y); }
VD_CG_INL F4 fwyzz4(F4 v) { return fm4(v.w, v.y, v.z, v.z); }
VD_CG_INL F4 fwyzw4(F4 v) { return fm4(v.w, v.y, v.z, v.w); }
VD_CG_INL F4 fwywx4(F4 v) { return fm4(v.w, v.y, v.w, v.x); }
VD_CG_INL F4 fwywy4(F4 v) { return fm4(v.w, v.y, v.w, v.y); }
VD_CG_INL F4 fwywz4(F4 v) { return fm4(v.w, v.y, v.w, v.z); }
VD_CG_INL F4 fwyww4(F4 v) { return fm4(v.w, v.y, v.w, v.w); }
VD_CG_INL F4 fwzxx4(F4 v) { return fm4(v.w, v.z, v.x, v.x); }
VD_CG_INL F4 fwzxy4(F4 v) { return fm4(v.w, v.z, v.x, v.y); }
VD_CG_INL F4 fwzxz4(F4 v) { return fm4(v.w, v.z, v.x, v.z); }
VD_CG_INL F4 fwzxw4(F4 v) { return fm4(v.w, v.z, v.x, v.w); }
VD_CG_INL F4 fwzyx4(F4 v) { return fm4(v.w, v.z, v.y, v.x); }
VD_CG_INL F4 fwzyy4(F4 v) { return fm4(v.w, v.z, v.y, v.y); }
VD_CG_INL F4 fwzyz4(F4 v) { return fm4(v.w, v.z, v.y, v.z); }
VD_CG_INL F4 fwzyw4(F4 v) { return fm4(v.w, v.z, v.y, v.w); }
VD_CG_INL F4 fwzzx4(F4 v) { return fm4(v.w, v.z, v.z, v.x); }
VD_CG_INL F4 fwzzy4(F4 v) { return fm4(v.w, v.z, v.z, v.y); }
VD_CG_INL F4 fwzzz4(F4 v) { return fm4(v.w, v.z, v.z, v.z); }
VD_CG_INL F4 fwzzw4(F4 v) { return fm4(v.w, v.z, v.z, v.w); }
VD_CG_INL F4 fwzwx4(F4 v) { return fm4(v.w, v.z, v.w, v.x); }
VD_CG_INL F4 fwzwy4(F4 v) { return fm4(v.w, v.z, v.w, v.y); }
VD_CG_INL F4 fwzwz4(F4 v) { return fm4(v.w, v.z, v.w, v.z); }
VD_CG_INL F4 fwzww4(F4 v) { return fm4(v.w, v.z, v.w, v.w); }
VD_CG_INL F4 fwwxx4(F4 v) { return fm4(v.w, v.w, v.x, v.x); }
VD_CG_INL F4 fwwxy4(F4 v) { return fm4(v.w, v.w, v.x, v.y); }
VD_CG_INL F4 fwwxz4(F4 v) { return fm4(v.w, v.w, v.x, v.z); }
VD_CG_INL F4 fwwxw4(F4 v) { return fm4(v.w, v.w, v.x, v.w); }
VD_CG_INL F4 fwwyx4(F4 v) { return fm4(v.w, v.w, v.y, v.x); }
VD_CG_INL F4 fwwyy4(F4 v) { return fm4(v.w, v.w, v.y, v.y); }
VD_CG_INL F4 fwwyz4(F4 v) { return fm4(v.w, v.w, v.y, v.z); }
VD_CG_INL F4 fwwyw4(F4 v) { return fm4(v.w, v.w, v.y, v.w); }
VD_CG_INL F4 fwwzx4(F4 v) { return fm4(v.w, v.w, v.z, v.x); }
VD_CG_INL F4 fwwzy4(F4 v) { return fm4(v.w, v.w, v.z, v.y); }
VD_CG_INL F4 fwwzz4(F4 v) { return fm4(v.w, v.w, v.z, v.z); }
VD_CG_INL F4 fwwzw4(F4 v) { return fm4(v.w, v.w, v.z, v.w); }
VD_CG_INL F4 fwwwx4(F4 v) { return fm4(v.w, v.w, v.w, v.x); }
VD_CG_INL F4 fwwwy4(F4 v) { return fm4(v.w, v.w, v.w, v.y); }
VD_CG_INL F4 fwwwz4(F4 v) { return fm4(v.w, v.w, v.w, v.z); }
VD_CG_INL F4 fwwww4(F4 v) { return fm4(v.w, v.w, v.w, v.w); }
VD_CG_INL D2 dxx2(D2 v) { return dm2(v.x, v.x); }
VD_CG_INL D2 dxy2(D2 v) { return dm2(v.x, v.y); }
VD_CG_INL D2 dyx2(D2 v) { return dm2(v.y, v.x); }
VD_CG_INL D2 dyy2(D2 v) { return dm2(v.y, v.y); }
VD_CG_INL D3 dxxx2(D2 v) { return dm3(v.x, v.x, v.x); }
VD_CG_INL D3 dxxy2(D2 v) { return dm3(v.x, v.x, v.y); }
VD_CG_INL D3 dxyx2(D2 v) { return dm3(v.x, v.y, v.x); }
VD_CG_INL D3 dxyy2(D2 v) { return dm3(v.x, v.y, v.y); }
VD_CG_INL D3 dyxx2(D2 v) { return dm3(v.y, v.x, v.x); }
VD_CG_INL D3 dyxy2(D2 v) { return dm3(v.y, v.x, v.y); }
VD_CG_INL D3 dyyx2(D2 v) { return dm3(v.y, v.y, v.x); }
VD_CG_INL D3 dyyy2(D2 v) { return dm3(v.y, v.y, v.y); }
VD_CG_INL D4 dxxxx2(D2 v) { return dm4(v.x, v.x, v.x, v.x); }
VD_CG_INL D4 dxxxy2(D2 v) { return dm4(v.x, v.x, v.x, v.y); }
VD_CG_INL D4 dxxyx2(D2 v) { return dm4(v.x, v.x, v.y, v.x); }
VD_CG_INL D4 dxxyy2(D2 v) { return dm4(v.x, v.x, v.y, v.y); }
VD_CG_INL D4 dxyxx2(D2 v) { return dm4(v.x, v.y, v.x, v.x); }
VD_CG_INL D4 dxyxy2(D2 v) { return dm4(v.x, v.y, v.x, v.y); }
VD_CG_INL D4 dxyyx2(D2 v) { return dm4(v.x, v.y, v.y, v.x); }
VD_CG_INL D4 dxyyy2(D2 v) { return dm4(v.x, v.y, v.y, v.y); }
VD_CG_INL D4 dyxxx2(D2 v) { return dm4(v.y, v.x, v.x, v.x); }
VD_CG_INL D4 dyxxy2(D2 v) { return dm4(v.y, v.x, v.x, v.y); }
VD_CG_INL D4 dyxyx2(D2 v) { return dm4(v.y, v.x, v.y, v.x); }
VD_CG_INL D4 dyxyy2(D2 v) { return dm4(v.y, v.x, v.y, v.y); }
VD_CG_INL D4 dyyxx2(D2 v) { return dm4(v.y, v.y, v.x, v.x); }
VD_CG_INL D4 dyyxy2(D2 v) { return dm4(v.y, v.y, v.x, v.y); }
VD_CG_INL D4 dyyyx2(D2 v) { return dm4(v.y, v.y, v.y, v.x); }
VD_CG_INL D4 dyyyy2(D2 v) { return dm4(v.y, v.y, v.y, v.y); }
VD_CG_INL D2 dxx3(D3 v) { return dm2(v.x, v.x); }
VD_CG_INL D2 dxy3(D3 v) { return dm2(v.x, v.y); }
VD_CG_INL D2 dxz3(D3 v) { return dm2(v.x, v.z); }
VD_CG_INL D2 dyx3(D3 v) { return dm2(v.y, v.x); }
VD_CG_INL D2 dyy3(D3 v) { return dm2(v.y, v.y); }
VD_CG_INL D2 dyz3(D3 v) { return dm2(v.y, v.z); }
VD_CG_INL D2 dzx3(D3 v) { return dm2(v.z, v.x); }
VD_CG_INL D2 dzy3(D3 v) { return dm2(v.z, v.y); }
VD_CG_INL D2 dzz3(D3 v) { return dm2(v.z, v.z); }
VD_CG_INL D3 dxxx3(D3 v) { return dm3(v.x, v.x, v.x); }
VD_CG_INL D3 dxxy3(D3 v) { return dm3(v.x, v.x, v.y); }
VD_CG_INL D3 dxxz3(D3 v) { return dm3(v.x, v.x, v.z); }
VD_CG_INL D3 dxyx3(D3 v) { return dm3(v.x, v.y, v.x); }
VD_CG_INL D3 dxyy3(D3 v) { return dm3(v.x, v.y, v.y); }
VD_CG_INL D3 dxyz3(D3 v) { return dm3(v.x, v.y, v.z); }
VD_CG_INL D3 dxzx3(D3 v) { return dm3(v.x, v.z, v.x); }
VD_CG_INL D3 dxzy3(D3 v) { return dm3(v.x, v.z, v.y); }
VD_CG_INL D3 dxzz3(D3 v) { return dm3(v.x, v.z, v.z); }
VD_CG_INL D3 dyxx3(D3 v) { return dm3(v.y, v.x, v.x); }
VD_CG_INL D3 dyxy3(D3 v) { return dm3(v.y, v.x, v.y); }
VD_CG_INL D3 dyxz3(D3 v) { return dm3(v.y, v.x, v.z); }
VD_CG_INL D3 dyyx3(D3 v) { return dm3(v.y, v.y, v.x); }
VD_CG_INL D3 dyyy3(D3 v) { return dm3(v.y, v.y, v.y); }
VD_CG_INL D3 dyyz3(D3 v) { return dm3(v.y, v.y, v.z); }
VD_CG_INL D3 dyzx3(D3 v) { return dm3(v.y, v.z, v.x); }
VD_CG_INL D3 dyzy3(D3 v) { return dm3(v.y, v.z, v.y); }
VD_CG_INL D3 dyzz3(D3 v) { return dm3(v.y, v.z, v.z); }
VD_CG_INL D3 dzxx3(D3 v) { return dm3(v.z, v.x, v.x); }
VD_CG_INL D3 dzxy3(D3 v) { return dm3(v.z, v.x, v.y); }
VD_CG_INL D3 dzxz3(D3 v) { return dm3(v.z, v.x, v.z); }
VD_CG_INL D3 dzyx3(D3 v) { return dm3(v.z, v.y, v.x); }
VD_CG_INL D3 dzyy3(D3 v) { return dm3(v.z, v.y, v.y); }
VD_CG_INL D3 dzyz3(D3 v) { return dm3(v.z, v.y, v.z); }
VD_CG_INL D3 dzzx3(D3 v) { return dm3(v.z, v.z, v.x); }
VD_CG_INL D3 dzzy3(D3 v) { return dm3(v.z, v.z, v.y); }
VD_CG_INL D3 dzzz3(D3 v) { return dm3(v.z, v.z, v.z); }
VD_CG_INL D4 dxxxx3(D3 v) { return dm4(v.x, v.x, v.x, v.x); }
VD_CG_INL D4 dxxxy3(D3 v) { return dm4(v.x, v.x, v.x, v.y); }
VD_CG_INL D4 dxxxz3(D3 v) { return dm4(v.x, v.x, v.x, v.z); }
VD_CG_INL D4 dxxyx3(D3 v) { return dm4(v.x, v.x, v.y, v.x); }
VD_CG_INL D4 dxxyy3(D3 v) { return dm4(v.x, v.x, v.y, v.y); }
VD_CG_INL D4 dxxyz3(D3 v) { return dm4(v.x, v.x, v.y, v.z); }
VD_CG_INL D4 dxxzx3(D3 v) { return dm4(v.x, v.x, v.z, v.x); }
VD_CG_INL D4 dxxzy3(D3 v) { return dm4(v.x, v.x, v.z, v.y); }
VD_CG_INL D4 dxxzz3(D3 v) { return dm4(v.x, v.x, v.z, v.z); }
VD_CG_INL D4 dxyxx3(D3 v) { return dm4(v.x, v.y, v.x, v.x); }
VD_CG_INL D4 dxyxy3(D3 v) { return dm4(v.x, v.y, v.x, v.y); }
VD_CG_INL D4 dxyxz3(D3 v) { return dm4(v.x, v.y, v.x, v.z); }
VD_CG_INL D4 dxyyx3(D3 v) { return dm4(v.x, v.y, v.y, v.x); }
VD_CG_INL D4 dxyyy3(D3 v) { return dm4(v.x, v.y, v.y, v.y); }
VD_CG_INL D4 dxyyz3(D3 v) { return dm4(v.x, v.y, v.y, v.z); }
VD_CG_INL D4 dxyzx3(D3 v) { return dm4(v.x, v.y, v.z, v.x); }
VD_CG_INL D4 dxyzy3(D3 v) { return dm4(v.x, v.y, v.z, v.y); }
VD_CG_INL D4 dxyzz3(D3 v) { return dm4(v.x, v.y, v.z, v.z); }
VD_CG_INL D4 dxzxx3(D3 v) { return dm4(v.x, v.z, v.x, v.x); }
VD_CG_INL D4 dxzxy3(D3 v) { return dm4(v.x, v.z, v.x, v.y); }
VD_CG_INL D4 dxzxz3(D3 v) { return dm4(v.x, v.z, v.x, v.z); }
VD_CG_INL D4 dxzyx3(D3 v) { return dm4(v.x, v.z, v.y, v.x); }
VD_CG_INL D4 dxzyy3(D3 v) { return dm4(v.x, v.z, v.y, v.y); }
VD_CG_INL D4 dxzyz3(D3 v) { return dm4(v.x, v.z, v.y, v.z); }
VD_CG_INL D4 dxzzx3(D3 v) { return dm4(v.x, v.z, v.z, v.x); }
VD_CG_INL D4 dxzzy3(D3 v) { return dm4(v.x, v.z, v.z, v.y); }
VD_CG_INL D4 dxzzz3(D3 v) { return dm4(v.x, v.z, v.z, v.z); }
VD_CG_INL D4 dyxxx3(D3 v) { return dm4(v.y, v.x, v.x, v.x); }
VD_CG_INL D4 dyxxy3(D3 v) { return dm4(v.y, v.x, v.x, v.y); }
VD_CG_INL D4 dyxxz3(D3 v) { return dm4(v.y, v.x, v.x, v.z); }
VD_CG_INL D4 dyxyx3(D3 v) { return dm4(v.y, v.x, v.y, v.x); }
VD_CG_INL D4 dyxyy3(D3 v) { return dm4(v.y, v.x, v.y, v.y); }
VD_CG_INL D4 dyxyz3(D3 v) { return dm4(v.y, v.x, v.y, v.z); }
VD_CG_INL D4 dyxzx3(D3 v) { return dm4(v.y, v.x, v.z, v.x); }
VD_CG_INL D4 dyxzy3(D3 v) { return dm4(v.y, v.x, v.z, v.y); }
VD_CG_INL D4 dyxzz3(D3 v) { return dm4(v.y, v.x, v.z, v.z); }
VD_CG_INL D4 dyyxx3(D3 v) { return dm4(v.y, v.y, v.x, v.x); }
VD_CG_INL D4 dyyxy3(D3 v) { return dm4(v.y, v.y, v.x, v.y); }
VD_CG_INL D4 dyyxz3(D3 v) { return dm4(v.y, v.y, v.x, v.z); }
VD_CG_INL D4 dyyyx3(D3 v) { return dm4(v.y, v.y, v.y, v.x); }
VD_CG_INL D4 dyyyy3(D3 v) { return dm4(v.y, v.y, v.y, v.y); }
VD_CG_INL D4 dyyyz3(D3 v) { return dm4(v.y, v.y, v.y, v.z); }
VD_CG_INL D4 dyyzx3(D3 v) { return dm4(v.y, v.y, v.z, v.x); }
VD_CG_INL D4 dyyzy3(D3 v) { return dm4(v.y, v.y, v.z, v.y); }
VD_CG_INL D4 dyyzz3(D3 v) { return dm4(v.y, v.y, v.z, v.z); }
VD_CG_INL D4 dyzxx3(D3 v) { return dm4(v.y, v.z, v.x, v.x); }
VD_CG_INL D4 dyzxy3(D3 v) { return dm4(v.y, v.z, v.x, v.y); }
VD_CG_INL D4 dyzxz3(D3 v) { return dm4(v.y, v.z, v.x, v.z); }
VD_CG_INL D4 dyzyx3(D3 v) { return dm4(v.y, v.z, v.y, v.x); }
VD_CG_INL D4 dyzyy3(D3 v) { return dm4(v.y, v.z, v.y, v.y); }
VD_CG_INL D4 dyzyz3(D3 v) { return dm4(v.y, v.z, v.y, v.z); }
VD_CG_INL D4 dyzzx3(D3 v) { return dm4(v.y, v.z, v.z, v.x); }
VD_CG_INL D4 dyzzy3(D3 v) { return dm4(v.y, v.z, v.z, v.y); }
VD_CG_INL D4 dyzzz3(D3 v) { return dm4(v.y, v.z, v.z, v.z); }
VD_CG_INL D4 dzxxx3(D3 v) { return dm4(v.z, v.x, v.x, v.x); }
VD_CG_INL D4 dzxxy3(D3 v) { return dm4(v.z, v.x, v.x, v.y); }
VD_CG_INL D4 dzxxz3(D3 v) { return dm4(v.z, v.x, v.x, v.z); }
VD_CG_INL D4 dzxyx3(D3 v) { return dm4(v.z, v.x, v.y, v.x); }
VD_CG_INL D4 dzxyy3(D3 v) { return dm4(v.z, v.x, v.y, v.y); }
VD_CG_INL D4 dzxyz3(D3 v) { return dm4(v.z, v.x, v.y, v.z); }
VD_CG_INL D4 dzxzx3(D3 v) { return dm4(v.z, v.x, v.z, v.x); }
VD_CG_INL D4 dzxzy3(D3 v) { return dm4(v.z, v.x, v.z, v.y); }
VD_CG_INL D4 dzxzz3(D3 v) { return dm4(v.z, v.x, v.z, v.z); }
VD_CG_INL D4 dzyxx3(D3 v) { return dm4(v.z, v.y, v.x, v.x); }
VD_CG_INL D4 dzyxy3(D3 v) { return dm4(v.z, v.y, v.x, v.y); }
VD_CG_INL D4 dzyxz3(D3 v) { return dm4(v.z, v.y, v.x, v.z); }
VD_CG_INL D4 dzyyx3(D3 v) { return dm4(v.z, v.y, v.y, v.x); }
VD_CG_INL D4 dzyyy3(D3 v) { return dm4(v.z, v.y, v.y, v.y); }
VD_CG_INL D4 dzyyz3(D3 v) { return dm4(v.z, v.y, v.y, v.z); }
VD_CG_INL D4 dzyzx3(D3 v) { return dm4(v.z, v.y, v.z, v.x); }
VD_CG_INL D4 dzyzy3(D3 v) { return dm4(v.z, v.y, v.z, v.y); }
VD_CG_INL D4 dzyzz3(D3 v) { return dm4(v.z, v.y, v.z, v.z); }
VD_CG_INL D4 dzzxx3(D3 v) { return dm4(v.z, v.z, v.x, v.x); }
VD_CG_INL D4 dzzxy3(D3 v) { return dm4(v.z, v.z, v.x, v.y); }
VD_CG_INL D4 dzzxz3(D3 v) { return dm4(v.z, v.z, v.x, v.z); }
VD_CG_INL D4 dzzyx3(D3 v) { return dm4(v.z, v.z, v.y, v.x); }
VD_CG_INL D4 dzzyy3(D3 v) { return dm4(v.z, v.z, v.y, v.y); }
VD_CG_INL D4 dzzyz3(D3 v) { return dm4(v.z, v.z, v.y, v.z); }
VD_CG_INL D4 dzzzx3(D3 v) { return dm4(v.z, v.z, v.z, v.x); }
VD_CG_INL D4 dzzzy3(D3 v) { return dm4(v.z, v.z, v.z, v.y); }
VD_CG_INL D4 dzzzz3(D3 v) { return dm4(v.z, v.z, v.z, v.z); }
VD_CG_INL D2 dxx4(D4 v) { return dm2(v.x, v.x); }
VD_CG_INL D2 dxy4(D4 v) { return dm2(v.x, v.y); }
VD_CG_INL D2 dxz4(D4 v) { return dm2(v.x, v.z); }
VD_CG_INL D2 dxw4(D4 v) { return dm2(v.x, v.w); }
VD_CG_INL D2 dyx4(D4 v) { return dm2(v.y, v.x); }
VD_CG_INL D2 dyy4(D4 v) { return dm2(v.y, v.y); }
VD_CG_INL D2 dyz4(D4 v) { return dm2(v.y, v.z); }
VD_CG_INL D2 dyw4(D4 v) { return dm2(v.y, v.w); }
VD_CG_INL D2 dzx4(D4 v) { return dm2(v.z, v.x); }
VD_CG_INL D2 dzy4(D4 v) { return dm2(v.z, v.y); }
VD_CG_INL D2 dzz4(D4 v) { return dm2(v.z, v.z); }
VD_CG_INL D2 dzw4(D4 v) { return dm2(v.z, v.w); }
VD_CG_INL D2 dwx4(D4 v) { return dm2(v.w, v.x); }
VD_CG_INL D2 dwy4(D4 v) { return dm2(v.w, v.y); }
VD_CG_INL D2 dwz4(D4 v) { return dm2(v.w, v.z); }
VD_CG_INL D2 dww4(D4 v) { return dm2(v.w, v.w); }
VD_CG_INL D3 dxxx4(D4 v) { return dm3(v.x, v.x, v.x); }
VD_CG_INL D3 dxxy4(D4 v) { return dm3(v.x, v.x, v.y); }
VD_CG_INL D3 dxxz4(D4 v) { return dm3(v.x, v.x, v.z); }
VD_CG_INL D3 dxxw4(D4 v) { return dm3(v.x, v.x, v.w); }
VD_CG_INL D3 dxyx4(D4 v) { return dm3(v.x, v.y, v.x); }
VD_CG_INL D3 dxyy4(D4 v) { return dm3(v.x, v.y, v.y); }
VD_CG_INL D3 dxyz4(D4 v) { return dm3(v.x, v.y, v.z); }
VD_CG_INL D3 dxyw4(D4 v) { return dm3(v.x, v.y, v.w); }
VD_CG_INL D3 dxzx4(D4 v) { return dm3(v.x, v.z, v.x); }
VD_CG_INL D3 dxzy4(D4 v) { return dm3(v.x, v.z, v.y); }
VD_CG_INL D3 dxzz4(D4 v) { return dm3(v.x, v.z, v.z); }
VD_CG_INL D3 dxzw4(D4 v) { return dm3(v.x, v.z, v.w); }
VD_CG_INL D3 dxwx4(D4 v) { return dm3(v.x, v.w, v.x); }
VD_CG_INL D3 dxwy4(D4 v) { return dm3(v.x, v.w, v.y); }
VD_CG_INL D3 dxwz4(D4 v) { return dm3(v.x, v.w, v.z); }
VD_CG_INL D3 dxww4(D4 v) { return dm3(v.x, v.w, v.w); }
VD_CG_INL D3 dyxx4(D4 v) { return dm3(v.y, v.x, v.x); }
VD_CG_INL D3 dyxy4(D4 v) { return dm3(v.y, v.x, v.y); }
VD_CG_INL D3 dyxz4(D4 v) { return dm3(v.y, v.x, v.z); }
VD_CG_INL D3 dyxw4(D4 v) { return dm3(v.y, v.x, v.w); }
VD_CG_INL D3 dyyx4(D4 v) { return dm3(v.y, v.y, v.x); }
VD_CG_INL D3 dyyy4(D4 v) { return dm3(v.y, v.y, v.y); }
VD_CG_INL D3 dyyz4(D4 v) { return dm3(v.y, v.y, v.z); }
VD_CG_INL D3 dyyw4(D4 v) { return dm3(v.y, v.y, v.w); }
VD_CG_INL D3 dyzx4(D4 v) { return dm3(v.y, v.z, v.x); }
VD_CG_INL D3 dyzy4(D4 v) { return dm3(v.y, v.z, v.y); }
VD_CG_INL D3 dyzz4(D4 v) { return dm3(v.y, v.z, v.z); }
VD_CG_INL D3 dyzw4(D4 v) { return dm3(v.y, v.z, v.w); }
VD_CG_INL D3 dywx4(D4 v) { return dm3(v.y, v.w, v.x); }
VD_CG_INL D3 dywy4(D4 v) { return dm3(v.y, v.w, v.y); }
VD_CG_INL D3 dywz4(D4 v) { return dm3(v.y, v.w, v.z); }
VD_CG_INL D3 dyww4(D4 v) { return dm3(v.y, v.w, v.w); }
VD_CG_INL D3 dzxx4(D4 v) { return dm3(v.z, v.x, v.x); }
VD_CG_INL D3 dzxy4(D4 v) { return dm3(v.z, v.x, v.y); }
VD_CG_INL D3 dzxz4(D4 v) { return dm3(v.z, v.x, v.z); }
VD_CG_INL D3 dzxw4(D4 v) { return dm3(v.z, v.x, v.w); }
VD_CG_INL D3 dzyx4(D4 v) { return dm3(v.z, v.y, v.x); }
VD_CG_INL D3 dzyy4(D4 v) { return dm3(v.z, v.y, v.y); }
VD_CG_INL D3 dzyz4(D4 v) { return dm3(v.z, v.y, v.z); }
VD_CG_INL D3 dzyw4(D4 v) { return dm3(v.z, v.y, v.w); }
VD_CG_INL D3 dzzx4(D4 v) { return dm3(v.z, v.z, v.x); }
VD_CG_INL D3 dzzy4(D4 v) { return dm3(v.z, v.z, v.y); }
VD_CG_INL D3 dzzz4(D4 v) { return dm3(v.z, v.z, v.z); }
VD_CG_INL D3 dzzw4(D4 v) { return dm3(v.z, v.z, v.w); }
VD_CG_INL D3 dzwx4(D4 v) { return dm3(v.z, v.w, v.x); }
VD_CG_INL D3 dzwy4(D4 v) { return dm3(v.z, v.w, v.y); }
VD_CG_INL D3 dzwz4(D4 v) { return dm3(v.z, v.w, v.z); }
VD_CG_INL D3 dzww4(D4 v) { return dm3(v.z, v.w, v.w); }
VD_CG_INL D3 dwxx4(D4 v) { return dm3(v.w, v.x, v.x); }
VD_CG_INL D3 dwxy4(D4 v) { return dm3(v.w, v.x, v.y); }
VD_CG_INL D3 dwxz4(D4 v) { return dm3(v.w, v.x, v.z); }
VD_CG_INL D3 dwxw4(D4 v) { return dm3(v.w, v.x, v.w); }
VD_CG_INL D3 dwyx4(D4 v) { return dm3(v.w, v.y, v.x); }
VD_CG_INL D3 dwyy4(D4 v) { return dm3(v.w, v.y, v.y); }
VD_CG_INL D3 dwyz4(D4 v) { return dm3(v.w, v.y, v.z); }
VD_CG_INL D3 dwyw4(D4 v) { return dm3(v.w, v.y, v.w); }
VD_CG_INL D3 dwzx4(D4 v) { return dm3(v.w, v.z, v.x); }
VD_CG_INL D3 dwzy4(D4 v) { return dm3(v.w, v.z, v.y); }
VD_CG_INL D3 dwzz4(D4 v) { return dm3(v.w, v.z, v.z); }
VD_CG_INL D3 dwzw4(D4 v) { return dm3(v.w, v.z, v.w); }
VD_CG_INL D3 dwwx4(D4 v) { return dm3(v.w, v.w, v.x); }
VD_CG_INL D3 dwwy4(D4 v) { return dm3(v.w, v.w, v.y); }
VD_CG_INL D3 dwwz4(D4 v) { return dm3(v.w, v.w, v.z); }
VD_CG_INL D3 dwww4(D4 v) { return dm3(v.w, v.w, v.w); }
VD_CG_INL D4 dxxxx4(D4 v) { return dm4(v.x, v.x, v.x, v.x); }
VD_CG_INL D4 dxxxy4(D4 v) { return dm4(v.x, v.x, v.x, v.y); }
VD_CG_INL D4 dxxxz4(D4 v) { return dm4(v.x, v.x, v.x, v.z); }
VD_CG_INL D4 dxxxw4(D4 v) { return dm4(v.x, v.x, v.x, v.w); }
VD_CG_INL D4 dxxyx4(D4 v) { return dm4(v.x, v.x, v.y, v.x); }
VD_CG_INL D4 dxxyy4(D4 v) { return dm4(v.x, v.x, v.y, v.y); }
VD_CG_INL D4 dxxyz4(D4 v) { return dm4(v.x, v.x, v.y, v.z); }
VD_CG_INL D4 dxxyw4(D4 v) { return dm4(v.x, v.x, v.y, v.w); }
VD_CG_INL D4 dxxzx4(D4 v) { return dm4(v.x, v.x, v.z, v.x); }
VD_CG_INL D4 dxxzy4(D4 v) { return dm4(v.x, v.x, v.z, v.y); }
VD_CG_INL D4 dxxzz4(D4 v) { return dm4(v.x, v.x, v.z, v.z); }
VD_CG_INL D4 dxxzw4(D4 v) { return dm4(v.x, v.x, v.z, v.w); }
VD_CG_INL D4 dxxwx4(D4 v) { return dm4(v.x, v.x, v.w, v.x); }
VD_CG_INL D4 dxxwy4(D4 v) { return dm4(v.x, v.x, v.w, v.y); }
VD_CG_INL D4 dxxwz4(D4 v) { return dm4(v.x, v.x, v.w, v.z); }
VD_CG_INL D4 dxxww4(D4 v) { return dm4(v.x, v.x, v.w, v.w); }
VD_CG_INL D4 dxyxx4(D4 v) { return dm4(v.x, v.y, v.x, v.x); }
VD_CG_INL D4 dxyxy4(D4 v) { return dm4(v.x, v.y, v.x, v.y); }
VD_CG_INL D4 dxyxz4(D4 v) { return dm4(v.x, v.y, v.x, v.z); }
VD_CG_INL D4 dxyxw4(D4 v) { return dm4(v.x, v.y, v.x, v.w); }
VD_CG_INL D4 dxyyx4(D4 v) { return dm4(v.x, v.y, v.y, v.x); }
VD_CG_INL D4 dxyyy4(D4 v) { return dm4(v.x, v.y, v.y, v.y); }
VD_CG_INL D4 dxyyz4(D4 v) { return dm4(v.x, v.y, v.y, v.z); }
VD_CG_INL D4 dxyyw4(D4 v) { return dm4(v.x, v.y, v.y, v.w); }
VD_CG_INL D4 dxyzx4(D4 v) { return dm4(v.x, v.y, v.z, v.x); }
VD_CG_INL D4 dxyzy4(D4 v) { return dm4(v.x, v.y, v.z, v.y); }
VD_CG_INL D4 dxyzz4(D4 v) { return dm4(v.x, v.y, v.z, v.z); }
VD_CG_INL D4 dxyzw4(D4 v) { return dm4(v.x, v.y, v.z, v.w); }
VD_CG_INL D4 dxywx4(D4 v) { return dm4(v.x, v.y, v.w, v.x); }
VD_CG_INL D4 dxywy4(D4 v) { return dm4(v.x, v.y, v.w, v.y); }
VD_CG_INL D4 dxywz4(D4 v) { return dm4(v.x, v.y, v.w, v.z); }
VD_CG_INL D4 dxyww4(D4 v) { return dm4(v.x, v.y, v.w, v.w); }
VD_CG_INL D4 dxzxx4(D4 v) { return dm4(v.x, v.z, v.x, v.x); }
VD_CG_INL D4 dxzxy4(D4 v) { return dm4(v.x, v.z, v.x, v.y); }
VD_CG_INL D4 dxzxz4(D4 v) { return dm4(v.x, v.z, v.x, v.z); }
VD_CG_INL D4 dxzxw4(D4 v) { return dm4(v.x, v.z, v.x, v.w); }
VD_CG_INL D4 dxzyx4(D4 v) { return dm4(v.x, v.z, v.y, v.x); }
VD_CG_INL D4 dxzyy4(D4 v) { return dm4(v.x, v.z, v.y, v.y); }
VD_CG_INL D4 dxzyz4(D4 v) { return dm4(v.x, v.z, v.y, v.z); }
VD_CG_INL D4 dxzyw4(D4 v) { return dm4(v.x, v.z, v.y, v.w); }
VD_CG_INL D4 dxzzx4(D4 v) { return dm4(v.x, v.z, v.z, v.x); }
VD_CG_INL D4 dxzzy4(D4 v) { return dm4(v.x, v.z, v.z, v.y); }
VD_CG_INL D4 dxzzz4(D4 v) { return dm4(v.x, v.z, v.z, v.z); }
VD_CG_INL D4 dxzzw4(D4 v) { return dm4(v.x, v.z, v.z, v.w); }
VD_CG_INL D4 dxzwx4(D4 v) { return dm4(v.x, v.z, v.w, v.x); }
VD_CG_INL D4 dxzwy4(D4 v) { return dm4(v.x, v.z, v.w, v.y); }
VD_CG_INL D4 dxzwz4(D4 v) { return dm4(v.x, v.z, v.w, v.z); }
VD_CG_INL D4 dxzww4(D4 v) { return dm4(v.x, v.z, v.w, v.w); }
VD_CG_INL D4 dxwxx4(D4 v) { return dm4(v.x, v.w, v.x, v.x); }
VD_CG_INL D4 dxwxy4(D4 v) { return dm4(v.x, v.w, v.x, v.y); }
VD_CG_INL D4 dxwxz4(D4 v) { return dm4(v.x, v.w, v.x, v.z); }
VD_CG_INL D4 dxwxw4(D4 v) { return dm4(v.x, v.w, v.x, v.w); }
VD_CG_INL D4 dxwyx4(D4 v) { return dm4(v.x, v.w, v.y, v.x); }
VD_CG_INL D4 dxwyy4(D4 v) { return dm4(v.x, v.w, v.y, v.y); }
VD_CG_INL D4 dxwyz4(D4 v) { return dm4(v.x, v.w, v.y, v.z); }
VD_CG_INL D4 dxwyw4(D4 v) { return dm4(v.x, v.w, v.y, v.w); }
VD_CG_INL D4 dxwzx4(D4 v) { return dm4(v.x, v.w, v.z, v.x); }
VD_CG_INL D4 dxwzy4(D4 v) { return dm4(v.x, v.w, v.z, v.y); }
VD_CG_INL D4 dxwzz4(D4 v) { return dm4(v.x, v.w, v.z, v.z); }
VD_CG_INL D4 dxwzw4(D4 v) { return dm4(v.x, v.w, v.z, v.w); }
VD_CG_INL D4 dxwwx4(D4 v) { return dm4(v.x, v.w, v.w, v.x); }
VD_CG_INL D4 dxwwy4(D4 v) { return dm4(v.x, v.w, v.w, v.y); }
VD_CG_INL D4 dxwwz4(D4 v) { return dm4(v.x, v.w, v.w, v.z); }
VD_CG_INL D4 dxwww4(D4 v) { return dm4(v.x, v.w, v.w, v.w); }
VD_CG_INL D4 dyxxx4(D4 v) { return dm4(v.y, v.x, v.x, v.x); }
VD_CG_INL D4 dyxxy4(D4 v) { return dm4(v.y, v.x, v.x, v.y); }
VD_CG_INL D4 dyxxz4(D4 v) { return dm4(v.y, v.x, v.x, v.z); }
VD_CG_INL D4 dyxxw4(D4 v) { return dm4(v.y, v.x, v.x, v.w); }
VD_CG_INL D4 dyxyx4(D4 v) { return dm4(v.y, v.x, v.y, v.x); }
VD_CG_INL D4 dyxyy4(D4 v) { return dm4(v.y, v.x, v.y, v.y); }
VD_CG_INL D4 dyxyz4(D4 v) { return dm4(v.y, v.x, v.y, v.z); }
VD_CG_INL D4 dyxyw4(D4 v) { return dm4(v.y, v.x, v.y, v.w); }
VD_CG_INL D4 dyxzx4(D4 v) { return dm4(v.y, v.x, v.z, v.x); }
VD_CG_INL D4 dyxzy4(D4 v) { return dm4(v.y, v.x, v.z, v.y); }
VD_CG_INL D4 dyxzz4(D4 v) { return dm4(v.y, v.x, v.z, v.z); }
VD_CG_INL D4 dyxzw4(D4 v) { return dm4(v.y, v.x, v.z, v.w); }
VD_CG_INL D4 dyxwx4(D4 v) { return dm4(v.y, v.x, v.w, v.x); }
VD_CG_INL D4 dyxwy4(D4 v) { return dm4(v.y, v.x, v.w, v.y); }
VD_CG_INL D4 dyxwz4(D4 v) { return dm4(v.y, v.x, v.w, v.z); }
VD_CG_INL D4 dyxww4(D4 v) { return dm4(v.y, v.x, v.w, v.w); }
VD_CG_INL D4 dyyxx4(D4 v) { return dm4(v.y, v.y, v.x, v.x); }
VD_CG_INL D4 dyyxy4(D4 v) { return dm4(v.y, v.y, v.x, v.y); }
VD_CG_INL D4 dyyxz4(D4 v) { return dm4(v.y, v.y, v.x, v.z); }
VD_CG_INL D4 dyyxw4(D4 v) { return dm4(v.y, v.y, v.x, v.w); }
VD_CG_INL D4 dyyyx4(D4 v) { return dm4(v.y, v.y, v.y, v.x); }
VD_CG_INL D4 dyyyy4(D4 v) { return dm4(v.y, v.y, v.y, v.y); }
VD_CG_INL D4 dyyyz4(D4 v) { return dm4(v.y, v.y, v.y, v.z); }
VD_CG_INL D4 dyyyw4(D4 v) { return dm4(v.y, v.y, v.y, v.w); }
VD_CG_INL D4 dyyzx4(D4 v) { return dm4(v.y, v.y, v.z, v.x); }
VD_CG_INL D4 dyyzy4(D4 v) { return dm4(v.y, v.y, v.z, v.y); }
VD_CG_INL D4 dyyzz4(D4 v) { return dm4(v.y, v.y, v.z, v.z); }
VD_CG_INL D4 dyyzw4(D4 v) { return dm4(v.y, v.y, v.z, v.w); }
VD_CG_INL D4 dyywx4(D4 v) { return dm4(v.y, v.y, v.w, v.x); }
VD_CG_INL D4 dyywy4(D4 v) { return dm4(v.y, v.y, v.w, v.y); }
VD_CG_INL D4 dyywz4(D4 v) { return dm4(v.y, v.y, v.w, v.z); }
VD_CG_INL D4 dyyww4(D4 v) { return dm4(v.y, v.y, v.w, v.w); }
VD_CG_INL D4 dyzxx4(D4 v) { return dm4(v.y, v.z, v.x, v.x); }
VD_CG_INL D4 dyzxy4(D4 v) { return dm4(v.y, v.z, v.x, v.y); }
VD_CG_INL D4 dyzxz4(D4 v) { return dm4(v.y, v.z, v.x, v.z); }
VD_CG_INL D4 dyzxw4(D4 v) { return dm4(v.y, v.z, v.x, v.w); }
VD_CG_INL D4 dyzyx4(D4 v) { return dm4(v.y, v.z, v.y, v.x); }
VD_CG_INL D4 dyzyy4(D4 v) { return dm4(v.y, v.z, v.y, v.y); }
VD_CG_INL D4 dyzyz4(D4 v) { return dm4(v.y, v.z, v.y, v.z); }
VD_CG_INL D4 dyzyw4(D4 v) { return dm4(v.y, v.z, v.y, v.w); }
VD_CG_INL D4 dyzzx4(D4 v) { return dm4(v.y, v.z, v.z, v.x); }
VD_CG_INL D4 dyzzy4(D4 v) { return dm4(v.y, v.z, v.z, v.y); }
VD_CG_INL D4 dyzzz4(D4 v) { return dm4(v.y, v.z, v.z, v.z); }
VD_CG_INL D4 dyzzw4(D4 v) { return dm4(v.y, v.z, v.z, v.w); }
VD_CG_INL D4 dyzwx4(D4 v) { return dm4(v.y, v.z, v.w, v.x); }
VD_CG_INL D4 dyzwy4(D4 v) { return dm4(v.y, v.z, v.w, v.y); }
VD_CG_INL D4 dyzwz4(D4 v) { return dm4(v.y, v.z, v.w, v.z); }
VD_CG_INL D4 dyzww4(D4 v) { return dm4(v.y, v.z, v.w, v.w); }
VD_CG_INL D4 dywxx4(D4 v) { return dm4(v.y, v.w, v.x, v.x); }
VD_CG_INL D4 dywxy4(D4 v) { return dm4(v.y, v.w, v.x, v.y); }
VD_CG_INL D4 dywxz4(D4 v) { return dm4(v.y, v.w, v.x, v.z); }
VD_CG_INL D4 dywxw4(D4 v) { return dm4(v.y, v.w, v.x, v.w); }
VD_CG_INL D4 dywyx4(D4 v) { return dm4(v.y, v.w, v.y, v.x); }
VD_CG_INL D4 dywyy4(D4 v) { return dm4(v.y, v.w, v.y, v.y); }
VD_CG_INL D4 dywyz4(D4 v) { return dm4(v.y, v.w, v.y, v.z); }
VD_CG_INL D4 dywyw4(D4 v) { return dm4(v.y, v.w, v.y, v.w); }
VD_CG_INL D4 dywzx4(D4 v) { return dm4(v.y, v.w, v.z, v.x); }
VD_CG_INL D4 dywzy4(D4 v) { return dm4(v.y, v.w, v.z, v.y); }
VD_CG_INL D4 dywzz4(D4 v) { return dm4(v.y, v.w, v.z, v.z); }
VD_CG_INL D4 dywzw4(D4 v) { return dm4(v.y, v.w, v.z, v.w); }
VD_CG_INL D4 dywwx4(D4 v) { return dm4(v.y, v.w, v.w, v.x); }
VD_CG_INL D4 dywwy4(D4 v) { return dm4(v.y, v.w, v.w, v.y); }
VD_CG_INL D4 dywwz4(D4 v) { return dm4(v.y, v.w, v.w, v.z); }
VD_CG_INL D4 dywww4(D4 v) { return dm4(v.y, v.w, v.w, v.w); }
VD_CG_INL D4 dzxxx4(D4 v) { return dm4(v.z, v.x, v.x, v.x); }
VD_CG_INL D4 dzxxy4(D4 v) { return dm4(v.z, v.x, v.x, v.y); }
VD_CG_INL D4 dzxxz4(D4 v) { return dm4(v.z, v.x, v.x, v.z); }
VD_CG_INL D4 dzxxw4(D4 v) { return dm4(v.z, v.x, v.x, v.w); }
VD_CG_INL D4 dzxyx4(D4 v) { return dm4(v.z, v.x, v.y, v.x); }
VD_CG_INL D4 dzxyy4(D4 v) { return dm4(v.z, v.x, v.y, v.y); }
VD_CG_INL D4 dzxyz4(D4 v) { return dm4(v.z, v.x, v.y, v.z); }
VD_CG_INL D4 dzxyw4(D4 v) { return dm4(v.z, v.x, v.y, v.w); }
VD_CG_INL D4 dzxzx4(D4 v) { return dm4(v.z, v.x, v.z, v.x); }
VD_CG_INL D4 dzxzy4(D4 v) { return dm4(v.z, v.x, v.z, v.y); }
VD_CG_INL D4 dzxzz4(D4 v) { return dm4(v.z, v.x, v.z, v.z); }
VD_CG_INL D4 dzxzw4(D4 v) { return dm4(v.z, v.x, v.z, v.w); }
VD_CG_INL D4 dzxwx4(D4 v) { return dm4(v.z, v.x, v.w, v.x); }
VD_CG_INL D4 dzxwy4(D4 v) { return dm4(v.z, v.x, v.w, v.y); }
VD_CG_INL D4 dzxwz4(D4 v) { return dm4(v.z, v.x, v.w, v.z); }
VD_CG_INL D4 dzxww4(D4 v) { return dm4(v.z, v.x, v.w, v.w); }
VD_CG_INL D4 dzyxx4(D4 v) { return dm4(v.z, v.y, v.x, v.x); }
VD_CG_INL D4 dzyxy4(D4 v) { return dm4(v.z, v.y, v.x, v.y); }
VD_CG_INL D4 dzyxz4(D4 v) { return dm4(v.z, v.y, v.x, v.z); }
VD_CG_INL D4 dzyxw4(D4 v) { return dm4(v.z, v.y, v.x, v.w); }
VD_CG_INL D4 dzyyx4(D4 v) { return dm4(v.z, v.y, v.y, v.x); }
VD_CG_INL D4 dzyyy4(D4 v) { return dm4(v.z, v.y, v.y, v.y); }
VD_CG_INL D4 dzyyz4(D4 v) { return dm4(v.z, v.y, v.y, v.z); }
VD_CG_INL D4 dzyyw4(D4 v) { return dm4(v.z, v.y, v.y, v.w); }
VD_CG_INL D4 dzyzx4(D4 v) { return dm4(v.z, v.y, v.z, v.x); }
VD_CG_INL D4 dzyzy4(D4 v) { return dm4(v.z, v.y, v.z, v.y); }
VD_CG_INL D4 dzyzz4(D4 v) { return dm4(v.z, v.y, v.z, v.z); }
VD_CG_INL D4 dzyzw4(D4 v) { return dm4(v.z, v.y, v.z, v.w); }
VD_CG_INL D4 dzywx4(D4 v) { return dm4(v.z, v.y, v.w, v.x); }
VD_CG_INL D4 dzywy4(D4 v) { return dm4(v.z, v.y, v.w, v.y); }
VD_CG_INL D4 dzywz4(D4 v) { return dm4(v.z, v.y, v.w, v.z); }
VD_CG_INL D4 dzyww4(D4 v) { return dm4(v.z, v.y, v.w, v.w); }
VD_CG_INL D4 dzzxx4(D4 v) { return dm4(v.z, v.z, v.x, v.x); }
VD_CG_INL D4 dzzxy4(D4 v) { return dm4(v.z, v.z, v.x, v.y); }
VD_CG_INL D4 dzzxz4(D4 v) { return dm4(v.z, v.z, v.x, v.z); }
VD_CG_INL D4 dzzxw4(D4 v) { return dm4(v.z, v.z, v.x, v.w); }
VD_CG_INL D4 dzzyx4(D4 v) { return dm4(v.z, v.z, v.y, v.x); }
VD_CG_INL D4 dzzyy4(D4 v) { return dm4(v.z, v.z, v.y, v.y); }
VD_CG_INL D4 dzzyz4(D4 v) { return dm4(v.z, v.z, v.y, v.z); }
VD_CG_INL D4 dzzyw4(D4 v) { return dm4(v.z, v.z, v.y, v.w); }
VD_CG_INL D4 dzzzx4(D4 v) { return dm4(v.z, v.z, v.z, v.x); }
VD_CG_INL D4 dzzzy4(D4 v) { return dm4(v.z, v.z, v.z, v.y); }
VD_CG_INL D4 dzzzz4(D4 v) { return dm4(v.z, v.z, v.z, v.z); }
VD_CG_INL D4 dzzzw4(D4 v) { return dm4(v.z, v.z, v.z, v.w); }
VD_CG_INL D4 dzzwx4(D4 v) { return dm4(v.z, v.z, v.w, v.x); }
VD_CG_INL D4 dzzwy4(D4 v) { return dm4(v.z, v.z, v.w, v.y); }
VD_CG_INL D4 dzzwz4(D4 v) { return dm4(v.z, v.z, v.w, v.z); }
VD_CG_INL D4 dzzww4(D4 v) { return dm4(v.z, v.z, v.w, v.w); }
VD_CG_INL D4 dzwxx4(D4 v) { return dm4(v.z, v.w, v.x, v.x); }
VD_CG_INL D4 dzwxy4(D4 v) { return dm4(v.z, v.w, v.x, v.y); }
VD_CG_INL D4 dzwxz4(D4 v) { return dm4(v.z, v.w, v.x, v.z); }
VD_CG_INL D4 dzwxw4(D4 v) { return dm4(v.z, v.w, v.x, v.w); }
VD_CG_INL D4 dzwyx4(D4 v) { return dm4(v.z, v.w, v.y, v.x); }
VD_CG_INL D4 dzwyy4(D4 v) { return dm4(v.z, v.w, v.y, v.y); }
VD_CG_INL D4 dzwyz4(D4 v) { return dm4(v.z, v.w, v.y, v.z); }
VD_CG_INL D4 dzwyw4(D4 v) { return dm4(v.z, v.w, v.y, v.w); }
VD_CG_INL D4 dzwzx4(D4 v) { return dm4(v.z, v.w, v.z, v.x); }
VD_CG_INL D4 dzwzy4(D4 v) { return dm4(v.z, v.w, v.z, v.y); }
VD_CG_INL D4 dzwzz4(D4 v) { return dm4(v.z, v.w, v.z, v.z); }
VD_CG_INL D4 dzwzw4(D4 v) { return dm4(v.z, v.w, v.z, v.w); }
VD_CG_INL D4 dzwwx4(D4 v) { return dm4(v.z, v.w, v.w, v.x); }
VD_CG_INL D4 dzwwy4(D4 v) { return dm4(v.z, v.w, v.w, v.y); }
VD_CG_INL D4 dzwwz4(D4 v) { return dm4(v.z, v.w, v.w, v.z); }
VD_CG_INL D4 dzwww4(D4 v) { return dm4(v.z, v.w, v.w, v.w); }
VD_CG_INL D4 dwxxx4(D4 v) { return dm4(v.w, v.x, v.x, v.x); }
VD_CG_INL D4 dwxxy4(D4 v) { return dm4(v.w, v.x, v.x, v.y); }
VD_CG_INL D4 dwxxz4(D4 v) { return dm4(v.w, v.x, v.x, v.z); }
VD_CG_INL D4 dwxxw4(D4 v) { return dm4(v.w, v.x, v.x, v.w); }
VD_CG_INL D4 dwxyx4(D4 v) { return dm4(v.w, v.x, v.y, v.x); }
VD_CG_INL D4 dwxyy4(D4 v) { return dm4(v.w, v.x, v.y, v.y); }
VD_CG_INL D4 dwxyz4(D4 v) { return dm4(v.w, v.x, v.y, v.z); }
VD_CG_INL D4 dwxyw4(D4 v) { return dm4(v.w, v.x, v.y, v.w); }
VD_CG_INL D4 dwxzx4(D4 v) { return dm4(v.w, v.x, v.z, v.x); }
VD_CG_INL D4 dwxzy4(D4 v) { return dm4(v.w, v.x, v.z, v.y); }
VD_CG_INL D4 dwxzz4(D4 v) { return dm4(v.w, v.x, v.z, v.z); }
VD_CG_INL D4 dwxzw4(D4 v) { return dm4(v.w, v.x, v.z, v.w); }
VD_CG_INL D4 dwxwx4(D4 v) { return dm4(v.w, v.x, v.w, v.x); }
VD_CG_INL D4 dwxwy4(D4 v) { return dm4(v.w, v.x, v.w, v.y); }
VD_CG_INL D4 dwxwz4(D4 v) { return dm4(v.w, v.x, v.w, v.z); }
VD_CG_INL D4 dwxww4(D4 v) { return dm4(v.w, v.x, v.w, v.w); }
VD_CG_INL D4 dwyxx4(D4 v) { return dm4(v.w, v.y, v.x, v.x); }
VD_CG_INL D4 dwyxy4(D4 v) { return dm4(v.w, v.y, v.x, v.y); }
VD_CG_INL D4 dwyxz4(D4 v) { return dm4(v.w, v.y, v.x, v.z); }
VD_CG_INL D4 dwyxw4(D4 v) { return dm4(v.w, v.y, v.x, v.w); }
VD_CG_INL D4 dwyyx4(D4 v) { return dm4(v.w, v.y, v.y, v.x); }
VD_CG_INL D4 dwyyy4(D4 v) { return dm4(v.w, v.y, v.y, v.y); }
VD_CG_INL D4 dwyyz4(D4 v) { return dm4(v.w, v.y, v.y, v.z); }
VD_CG_INL D4 dwyyw4(D4 v) { return dm4(v.w, v.y, v.y, v.w); }
VD_CG_INL D4 dwyzx4(D4 v) { return dm4(v.w, v.y, v.z, v.x); }
VD_CG_INL D4 dwyzy4(D4 v) { return dm4(v.w, v.y, v.z, v.y); }
VD_CG_INL D4 dwyzz4(D4 v) { return dm4(v.w, v.y, v.z, v.z); }
VD_CG_INL D4 dwyzw4(D4 v) { return dm4(v.w, v.y, v.z, v.w); }
VD_CG_INL D4 dwywx4(D4 v) { return dm4(v.w, v.y, v.w, v.x); }
VD_CG_INL D4 dwywy4(D4 v) { return dm4(v.w, v.y, v.w, v.y); }
VD_CG_INL D4 dwywz4(D4 v) { return dm4(v.w, v.y, v.w, v.z); }
VD_CG_INL D4 dwyww4(D4 v) { return dm4(v.w, v.y, v.w, v.w); }
VD_CG_INL D4 dwzxx4(D4 v) { return dm4(v.w, v.z, v.x, v.x); }
VD_CG_INL D4 dwzxy4(D4 v) { return dm4(v.w, v.z, v.x, v.y); }
VD_CG_INL D4 dwzxz4(D4 v) { return dm4(v.w, v.z, v.x, v.z); }
VD_CG_INL D4 dwzxw4(D4 v) { return dm4(v.w, v.z, v.x, v.w); }
VD_CG_INL D4 dwzyx4(D4 v) { return dm4(v.w, v.z, v.y, v.x); }
VD_CG_INL D4 dwzyy4(D4 v) { return dm4(v.w, v.z, v.y, v.y); }
VD_CG_INL D4 dwzyz4(D4 v) { return dm4(v.w, v.z, v.y, v.z); }
VD_CG_INL D4 dwzyw4(D4 v) { return dm4(v.w, v.z, v.y, v.w); }
VD_CG_INL D4 dwzzx4(D4 v) { return dm4(v.w, v.z, v.z, v.x); }
VD_CG_INL D4 dwzzy4(D4 v) { return dm4(v.w, v.z, v.z, v.y); }
VD_CG_INL D4 dwzzz4(D4 v) { return dm4(v.w, v.z, v.z, v.z); }
VD_CG_INL D4 dwzzw4(D4 v) { return dm4(v.w, v.z, v.z, v.w); }
VD_CG_INL D4 dwzwx4(D4 v) { return dm4(v.w, v.z, v.w, v.x); }
VD_CG_INL D4 dwzwy4(D4 v) { return dm4(v.w, v.z, v.w, v.y); }
VD_CG_INL D4 dwzwz4(D4 v) { return dm4(v.w, v.z, v.w, v.z); }
VD_CG_INL D4 dwzww4(D4 v) { return dm4(v.w, v.z, v.w, v.w); }
VD_CG_INL D4 dwwxx4(D4 v) { return dm4(v.w, v.w, v.x, v.x); }
VD_CG_INL D4 dwwxy4(D4 v) { return dm4(v.w, v.w, v.x, v.y); }
VD_CG_INL D4 dwwxz4(D4 v) { return dm4(v.w, v.w, v.x, v.z); }
VD_CG_INL D4 dwwxw4(D4 v) { return dm4(v.w, v.w, v.x, v.w); }
VD_CG_INL D4 dwwyx4(D4 v) { return dm4(v.w, v.w, v.y, v.x); }
VD_CG_INL D4 dwwyy4(D4 v) { return dm4(v.w, v.w, v.y, v.y); }
VD_CG_INL D4 dwwyz4(D4 v) { return dm4(v.w, v.w, v.y, v.z); }
VD_CG_INL D4 dwwyw4(D4 v) { return dm4(v.w, v.w, v.y, v.w); }
VD_CG_INL D4 dwwzx4(D4 v) { return dm4(v.w, v.w, v.z, v.x); }
VD_CG_INL D4 dwwzy4(D4 v) { return dm4(v.w, v.w, v.z, v.y); }
VD_CG_INL D4 dwwzz4(D4 v) { return dm4(v.w, v.w, v.z, v.z); }
VD_CG_INL D4 dwwzw4(D4 v) { return dm4(v.w, v.w, v.z, v.w); }
VD_CG_INL D4 dwwwx4(D4 v) { return dm4(v.w, v.w, v.w, v.x); }
VD_CG_INL D4 dwwwy4(D4 v) { return dm4(v.w, v.w, v.w, v.y); }
VD_CG_INL D4 dwwwz4(D4 v) { return dm4(v.w, v.w, v.w, v.z); }
VD_CG_INL D4 dwwww4(D4 v) { return dm4(v.w, v.w, v.w, v.w); }
VD_CG_INL S2 sxx2(S2 v) { return sm2(v.x, v.x); }
VD_CG_INL S2 sxy2(S2 v) { return sm2(v.x, v.y); }
VD_CG_INL S2 syx2(S2 v) { return sm2(v.y, v.x); }
VD_CG_INL S2 syy2(S2 v) { return sm2(v.y, v.y); }
VD_CG_INL S3 sxxx2(S2 v) { return sm3(v.x, v.x, v.x); }
VD_CG_INL S3 sxxy2(S2 v) { return sm3(v.x, v.x, v.y); }
VD_CG_INL S3 sxyx2(S2 v) { return sm3(v.x, v.y, v.x); }
VD_CG_INL S3 sxyy2(S2 v) { return sm3(v.x, v.y, v.y); }
VD_CG_INL S3 syxx2(S2 v) { return sm3(v.y, v.x, v.x); }
VD_CG_INL S3 syxy2(S2 v) { return sm3(v.y, v.x, v.y); }
VD_CG_INL S3 syyx2(S2 v) { return sm3(v.y, v.y, v.x); }
VD_CG_INL S3 syyy2(S2 v) { return sm3(v.y, v.y, v.y); }
VD_CG_INL S4 sxxxx2(S2 v) { return sm4(v.x, v.x, v.x, v.x); }
VD_CG_INL S4 sxxxy2(S2 v) { return sm4(v.x, v.x, v.x, v.y); }
VD_CG_INL S4 sxxyx2(S2 v) { return sm4(v.x, v.x, v.y, v.x); }
VD_CG_INL S4 sxxyy2(S2 v) { return sm4(v.x, v.x, v.y, v.y); }
VD_CG_INL S4 sxyxx2(S2 v) { return sm4(v.x, v.y, v.x, v.x); }
VD_CG_INL S4 sxyxy2(S2 v) { return sm4(v.x, v.y, v.x, v.y); }
VD_CG_INL S4 sxyyx2(S2 v) { return sm4(v.x, v.y, v.y, v.x); }
VD_CG_INL S4 sxyyy2(S2 v) { return sm4(v.x, v.y, v.y, v.y); }
VD_CG_INL S4 syxxx2(S2 v) { return sm4(v.y, v.x, v.x, v.x); }
VD_CG_INL S4 syxxy2(S2 v) { return sm4(v.y, v.x, v.x, v.y); }
VD_CG_INL S4 syxyx2(S2 v) { return sm4(v.y, v.x, v.y, v.x); }
VD_CG_INL S4 syxyy2(S2 v) { return sm4(v.y, v.x, v.y, v.y); }
VD_CG_INL S4 syyxx2(S2 v) { return sm4(v.y, v.y, v.x, v.x); }
VD_CG_INL S4 syyxy2(S2 v) { return sm4(v.y, v.y, v.x, v.y); }
VD_CG_INL S4 syyyx2(S2 v) { return sm4(v.y, v.y, v.y, v.x); }
VD_CG_INL S4 syyyy2(S2 v) { return sm4(v.y, v.y, v.y, v.y); }
VD_CG_INL S2 sxx3(S3 v) { return sm2(v.x, v.x); }
VD_CG_INL S2 sxy3(S3 v) { return sm2(v.x, v.y); }
VD_CG_INL S2 sxz3(S3 v) { return sm2(v.x, v.z); }
VD_CG_INL S2 syx3(S3 v) { return sm2(v.y, v.x); }
VD_CG_INL S2 syy3(S3 v) { return sm2(v.y, v.y); }
VD_CG_INL S2 syz3(S3 v) { return sm2(v.y, v.z); }
VD_CG_INL S2 szx3(S3 v) { return sm2(v.z, v.x); }
VD_CG_INL S2 szy3(S3 v) { return sm2(v.z, v.y); }
VD_CG_INL S2 szz3(S3 v) { return sm2(v.z, v.z); }
VD_CG_INL S3 sxxx3(S3 v) { return sm3(v.x, v.x, v.x); }
VD_CG_INL S3 sxxy3(S3 v) { return sm3(v.x, v.x, v.y); }
VD_CG_INL S3 sxxz3(S3 v) { return sm3(v.x, v.x, v.z); }
VD_CG_INL S3 sxyx3(S3 v) { return sm3(v.x, v.y, v.x); }
VD_CG_INL S3 sxyy3(S3 v) { return sm3(v.x, v.y, v.y); }
VD_CG_INL S3 sxyz3(S3 v) { return sm3(v.x, v.y, v.z); }
VD_CG_INL S3 sxzx3(S3 v) { return sm3(v.x, v.z, v.x); }
VD_CG_INL S3 sxzy3(S3 v) { return sm3(v.x, v.z, v.y); }
VD_CG_INL S3 sxzz3(S3 v) { return sm3(v.x, v.z, v.z); }
VD_CG_INL S3 syxx3(S3 v) { return sm3(v.y, v.x, v.x); }
VD_CG_INL S3 syxy3(S3 v) { return sm3(v.y, v.x, v.y); }
VD_CG_INL S3 syxz3(S3 v) { return sm3(v.y, v.x, v.z); }
VD_CG_INL S3 syyx3(S3 v) { return sm3(v.y, v.y, v.x); }
VD_CG_INL S3 syyy3(S3 v) { return sm3(v.y, v.y, v.y); }
VD_CG_INL S3 syyz3(S3 v) { return sm3(v.y, v.y, v.z); }
VD_CG_INL S3 syzx3(S3 v) { return sm3(v.y, v.z, v.x); }
VD_CG_INL S3 syzy3(S3 v) { return sm3(v.y, v.z, v.y); }
VD_CG_INL S3 syzz3(S3 v) { return sm3(v.y, v.z, v.z); }
VD_CG_INL S3 szxx3(S3 v) { return sm3(v.z, v.x, v.x); }
VD_CG_INL S3 szxy3(S3 v) { return sm3(v.z, v.x, v.y); }
VD_CG_INL S3 szxz3(S3 v) { return sm3(v.z, v.x, v.z); }
VD_CG_INL S3 szyx3(S3 v) { return sm3(v.z, v.y, v.x); }
VD_CG_INL S3 szyy3(S3 v) { return sm3(v.z, v.y, v.y); }
VD_CG_INL S3 szyz3(S3 v) { return sm3(v.z, v.y, v.z); }
VD_CG_INL S3 szzx3(S3 v) { return sm3(v.z, v.z, v.x); }
VD_CG_INL S3 szzy3(S3 v) { return sm3(v.z, v.z, v.y); }
VD_CG_INL S3 szzz3(S3 v) { return sm3(v.z, v.z, v.z); }
VD_CG_INL S4 sxxxx3(S3 v) { return sm4(v.x, v.x, v.x, v.x); }
VD_CG_INL S4 sxxxy3(S3 v) { return sm4(v.x, v.x, v.x, v.y); }
VD_CG_INL S4 sxxxz3(S3 v) { return sm4(v.x, v.x, v.x, v.z); }
VD_CG_INL S4 sxxyx3(S3 v) { return sm4(v.x, v.x, v.y, v.x); }
VD_CG_INL S4 sxxyy3(S3 v) { return sm4(v.x, v.x, v.y, v.y); }
VD_CG_INL S4 sxxyz3(S3 v) { return sm4(v.x, v.x, v.y, v.z); }
VD_CG_INL S4 sxxzx3(S3 v) { return sm4(v.x, v.x, v.z, v.x); }
VD_CG_INL S4 sxxzy3(S3 v) { return sm4(v.x, v.x, v.z, v.y); }
VD_CG_INL S4 sxxzz3(S3 v) { return sm4(v.x, v.x, v.z, v.z); }
VD_CG_INL S4 sxyxx3(S3 v) { return sm4(v.x, v.y, v.x, v.x); }
VD_CG_INL S4 sxyxy3(S3 v) { return sm4(v.x, v.y, v.x, v.y); }
VD_CG_INL S4 sxyxz3(S3 v) { return sm4(v.x, v.y, v.x, v.z); }
VD_CG_INL S4 sxyyx3(S3 v) { return sm4(v.x, v.y, v.y, v.x); }
VD_CG_INL S4 sxyyy3(S3 v) { return sm4(v.x, v.y, v.y, v.y); }
VD_CG_INL S4 sxyyz3(S3 v) { return sm4(v.x, v.y, v.y, v.z); }
VD_CG_INL S4 sxyzx3(S3 v) { return sm4(v.x, v.y, v.z, v.x); }
VD_CG_INL S4 sxyzy3(S3 v) { return sm4(v.x, v.y, v.z, v.y); }
VD_CG_INL S4 sxyzz3(S3 v) { return sm4(v.x, v.y, v.z, v.z); }
VD_CG_INL S4 sxzxx3(S3 v) { return sm4(v.x, v.z, v.x, v.x); }
VD_CG_INL S4 sxzxy3(S3 v) { return sm4(v.x, v.z, v.x, v.y); }
VD_CG_INL S4 sxzxz3(S3 v) { return sm4(v.x, v.z, v.x, v.z); }
VD_CG_INL S4 sxzyx3(S3 v) { return sm4(v.x, v.z, v.y, v.x); }
VD_CG_INL S4 sxzyy3(S3 v) { return sm4(v.x, v.z, v.y, v.y); }
VD_CG_INL S4 sxzyz3(S3 v) { return sm4(v.x, v.z, v.y, v.z); }
VD_CG_INL S4 sxzzx3(S3 v) { return sm4(v.x, v.z, v.z, v.x); }
VD_CG_INL S4 sxzzy3(S3 v) { return sm4(v.x, v.z, v.z, v.y); }
VD_CG_INL S4 sxzzz3(S3 v) { return sm4(v.x, v.z, v.z, v.z); }
VD_CG_INL S4 syxxx3(S3 v) { return sm4(v.y, v.x, v.x, v.x); }
VD_CG_INL S4 syxxy3(S3 v) { return sm4(v.y, v.x, v.x, v.y); }
VD_CG_INL S4 syxxz3(S3 v) { return sm4(v.y, v.x, v.x, v.z); }
VD_CG_INL S4 syxyx3(S3 v) { return sm4(v.y, v.x, v.y, v.x); }
VD_CG_INL S4 syxyy3(S3 v) { return sm4(v.y, v.x, v.y, v.y); }
VD_CG_INL S4 syxyz3(S3 v) { return sm4(v.y, v.x, v.y, v.z); }
VD_CG_INL S4 syxzx3(S3 v) { return sm4(v.y, v.x, v.z, v.x); }
VD_CG_INL S4 syxzy3(S3 v) { return sm4(v.y, v.x, v.z, v.y); }
VD_CG_INL S4 syxzz3(S3 v) { return sm4(v.y, v.x, v.z, v.z); }
VD_CG_INL S4 syyxx3(S3 v) { return sm4(v.y, v.y, v.x, v.x); }
VD_CG_INL S4 syyxy3(S3 v) { return sm4(v.y, v.y, v.x, v.y); }
VD_CG_INL S4 syyxz3(S3 v) { return sm4(v.y, v.y, v.x, v.z); }
VD_CG_INL S4 syyyx3(S3 v) { return sm4(v.y, v.y, v.y, v.x); }
VD_CG_INL S4 syyyy3(S3 v) { return sm4(v.y, v.y, v.y, v.y); }
VD_CG_INL S4 syyyz3(S3 v) { return sm4(v.y, v.y, v.y, v.z); }
VD_CG_INL S4 syyzx3(S3 v) { return sm4(v.y, v.y, v.z, v.x); }
VD_CG_INL S4 syyzy3(S3 v) { return sm4(v.y, v.y, v.z, v.y); }
VD_CG_INL S4 syyzz3(S3 v) { return sm4(v.y, v.y, v.z, v.z); }
VD_CG_INL S4 syzxx3(S3 v) { return sm4(v.y, v.z, v.x, v.x); }
VD_CG_INL S4 syzxy3(S3 v) { return sm4(v.y, v.z, v.x, v.y); }
VD_CG_INL S4 syzxz3(S3 v) { return sm4(v.y, v.z, v.x, v.z); }
VD_CG_INL S4 syzyx3(S3 v) { return sm4(v.y, v.z, v.y, v.x); }
VD_CG_INL S4 syzyy3(S3 v) { return sm4(v.y, v.z, v.y, v.y); }
VD_CG_INL S4 syzyz3(S3 v) { return sm4(v.y, v.z, v.y, v.z); }
VD_CG_INL S4 syzzx3(S3 v) { return sm4(v.y, v.z, v.z, v.x); }
VD_CG_INL S4 syzzy3(S3 v) { return sm4(v.y, v.z, v.z, v.y); }
VD_CG_INL S4 syzzz3(S3 v) { return sm4(v.y, v.z, v.z, v.z); }
VD_CG_INL S4 szxxx3(S3 v) { return sm4(v.z, v.x, v.x, v.x); }
VD_CG_INL S4 szxxy3(S3 v) { return sm4(v.z, v.x, v.x, v.y); }
VD_CG_INL S4 szxxz3(S3 v) { return sm4(v.z, v.x, v.x, v.z); }
VD_CG_INL S4 szxyx3(S3 v) { return sm4(v.z, v.x, v.y, v.x); }
VD_CG_INL S4 szxyy3(S3 v) { return sm4(v.z, v.x, v.y, v.y); }
VD_CG_INL S4 szxyz3(S3 v) { return sm4(v.z, v.x, v.y, v.z); }
VD_CG_INL S4 szxzx3(S3 v) { return sm4(v.z, v.x, v.z, v.x); }
VD_CG_INL S4 szxzy3(S3 v) { return sm4(v.z, v.x, v.z, v.y); }
VD_CG_INL S4 szxzz3(S3 v) { return sm4(v.z, v.x, v.z, v.z); }
VD_CG_INL S4 szyxx3(S3 v) { return sm4(v.z, v.y, v.x, v.x); }
VD_CG_INL S4 szyxy3(S3 v) { return sm4(v.z, v.y, v.x, v.y); }
VD_CG_INL S4 szyxz3(S3 v) { return sm4(v.z, v.y, v.x, v.z); }
VD_CG_INL S4 szyyx3(S3 v) { return sm4(v.z, v.y, v.y, v.x); }
VD_CG_INL S4 szyyy3(S3 v) { return sm4(v.z, v.y, v.y, v.y); }
VD_CG_INL S4 szyyz3(S3 v) { return sm4(v.z, v.y, v.y, v.z); }
VD_CG_INL S4 szyzx3(S3 v) { return sm4(v.z, v.y, v.z, v.x); }
VD_CG_INL S4 szyzy3(S3 v) { return sm4(v.z, v.y, v.z, v.y); }
VD_CG_INL S4 szyzz3(S3 v) { return sm4(v.z, v.y, v.z, v.z); }
VD_CG_INL S4 szzxx3(S3 v) { return sm4(v.z, v.z, v.x, v.x); }
VD_CG_INL S4 szzxy3(S3 v) { return sm4(v.z, v.z, v.x, v.y); }
VD_CG_INL S4 szzxz3(S3 v) { return sm4(v.z, v.z, v.x, v.z); }
VD_CG_INL S4 szzyx3(S3 v) { return sm4(v.z, v.z, v.y, v.x); }
VD_CG_INL S4 szzyy3(S3 v) { return sm4(v.z, v.z, v.y, v.y); }
VD_CG_INL S4 szzyz3(S3 v) { return sm4(v.z, v.z, v.y, v.z); }
VD_CG_INL S4 szzzx3(S3 v) { return sm4(v.z, v.z, v.z, v.x); }
VD_CG_INL S4 szzzy3(S3 v) { return sm4(v.z, v.z, v.z, v.y); }
VD_CG_INL S4 szzzz3(S3 v) { return sm4(v.z, v.z, v.z, v.z); }
VD_CG_INL S2 sxx4(S4 v) { return sm2(v.x, v.x); }
VD_CG_INL S2 sxy4(S4 v) { return sm2(v.x, v.y); }
VD_CG_INL S2 sxz4(S4 v) { return sm2(v.x, v.z); }
VD_CG_INL S2 sxw4(S4 v) { return sm2(v.x, v.w); }
VD_CG_INL S2 syx4(S4 v) { return sm2(v.y, v.x); }
VD_CG_INL S2 syy4(S4 v) { return sm2(v.y, v.y); }
VD_CG_INL S2 syz4(S4 v) { return sm2(v.y, v.z); }
VD_CG_INL S2 syw4(S4 v) { return sm2(v.y, v.w); }
VD_CG_INL S2 szx4(S4 v) { return sm2(v.z, v.x); }
VD_CG_INL S2 szy4(S4 v) { return sm2(v.z, v.y); }
VD_CG_INL S2 szz4(S4 v) { return sm2(v.z, v.z); }
VD_CG_INL S2 szw4(S4 v) { return sm2(v.z, v.w); }
VD_CG_INL S2 swx4(S4 v) { return sm2(v.w, v.x); }
VD_CG_INL S2 swy4(S4 v) { return sm2(v.w, v.y); }
VD_CG_INL S2 swz4(S4 v) { return sm2(v.w, v.z); }
VD_CG_INL S2 sww4(S4 v) { return sm2(v.w, v.w); }
VD_CG_INL S3 sxxx4(S4 v) { return sm3(v.x, v.x, v.x); }
VD_CG_INL S3 sxxy4(S4 v) { return sm3(v.x, v.x, v.y); }
VD_CG_INL S3 sxxz4(S4 v) { return sm3(v.x, v.x, v.z); }
VD_CG_INL S3 sxxw4(S4 v) { return sm3(v.x, v.x, v.w); }
VD_CG_INL S3 sxyx4(S4 v) { return sm3(v.x, v.y, v.x); }
VD_CG_INL S3 sxyy4(S4 v) { return sm3(v.x, v.y, v.y); }
VD_CG_INL S3 sxyz4(S4 v) { return sm3(v.x, v.y, v.z); }
VD_CG_INL S3 sxyw4(S4 v) { return sm3(v.x, v.y, v.w); }
VD_CG_INL S3 sxzx4(S4 v) { return sm3(v.x, v.z, v.x); }
VD_CG_INL S3 sxzy4(S4 v) { return sm3(v.x, v.z, v.y); }
VD_CG_INL S3 sxzz4(S4 v) { return sm3(v.x, v.z, v.z); }
VD_CG_INL S3 sxzw4(S4 v) { return sm3(v.x, v.z, v.w); }
VD_CG_INL S3 sxwx4(S4 v) { return sm3(v.x, v.w, v.x); }
VD_CG_INL S3 sxwy4(S4 v) { return sm3(v.x, v.w, v.y); }
VD_CG_INL S3 sxwz4(S4 v) { return sm3(v.x, v.w, v.z); }
VD_CG_INL S3 sxww4(S4 v) { return sm3(v.x, v.w, v.w); }
VD_CG_INL S3 syxx4(S4 v) { return sm3(v.y, v.x, v.x); }
VD_CG_INL S3 syxy4(S4 v) { return sm3(v.y, v.x, v.y); }
VD_CG_INL S3 syxz4(S4 v) { return sm3(v.y, v.x, v.z); }
VD_CG_INL S3 syxw4(S4 v) { return sm3(v.y, v.x, v.w); }
VD_CG_INL S3 syyx4(S4 v) { return sm3(v.y, v.y, v.x); }
VD_CG_INL S3 syyy4(S4 v) { return sm3(v.y, v.y, v.y); }
VD_CG_INL S3 syyz4(S4 v) { return sm3(v.y, v.y, v.z); }
VD_CG_INL S3 syyw4(S4 v) { return sm3(v.y, v.y, v.w); }
VD_CG_INL S3 syzx4(S4 v) { return sm3(v.y, v.z, v.x); }
VD_CG_INL S3 syzy4(S4 v) { return sm3(v.y, v.z, v.y); }
VD_CG_INL S3 syzz4(S4 v) { return sm3(v.y, v.z, v.z); }
VD_CG_INL S3 syzw4(S4 v) { return sm3(v.y, v.z, v.w); }
VD_CG_INL S3 sywx4(S4 v) { return sm3(v.y, v.w, v.x); }
VD_CG_INL S3 sywy4(S4 v) { return sm3(v.y, v.w, v.y); }
VD_CG_INL S3 sywz4(S4 v) { return sm3(v.y, v.w, v.z); }
VD_CG_INL S3 syww4(S4 v) { return sm3(v.y, v.w, v.w); }
VD_CG_INL S3 szxx4(S4 v) { return sm3(v.z, v.x, v.x); }
VD_CG_INL S3 szxy4(S4 v) { return sm3(v.z, v.x, v.y); }
VD_CG_INL S3 szxz4(S4 v) { return sm3(v.z, v.x, v.z); }
VD_CG_INL S3 szxw4(S4 v) { return sm3(v.z, v.x, v.w); }
VD_CG_INL S3 szyx4(S4 v) { return sm3(v.z, v.y, v.x); }
VD_CG_INL S3 szyy4(S4 v) { return sm3(v.z, v.y, v.y); }
VD_CG_INL S3 szyz4(S4 v) { return sm3(v.z, v.y, v.z); }
VD_CG_INL S3 szyw4(S4 v) { return sm3(v.z, v.y, v.w); }
VD_CG_INL S3 szzx4(S4 v) { return sm3(v.z, v.z, v.x); }
VD_CG_INL S3 szzy4(S4 v) { return sm3(v.z, v.z, v.y); }
VD_CG_INL S3 szzz4(S4 v) { return sm3(v.z, v.z, v.z); }
VD_CG_INL S3 szzw4(S4 v) { return sm3(v.z, v.z, v.w); }
VD_CG_INL S3 szwx4(S4 v) { return sm3(v.z, v.w, v.x); }
VD_CG_INL S3 szwy4(S4 v) { return sm3(v.z, v.w, v.y); }
VD_CG_INL S3 szwz4(S4 v) { return sm3(v.z, v.w, v.z); }
VD_CG_INL S3 szww4(S4 v) { return sm3(v.z, v.w, v.w); }
VD_CG_INL S3 swxx4(S4 v) { return sm3(v.w, v.x, v.x); }
VD_CG_INL S3 swxy4(S4 v) { return sm3(v.w, v.x, v.y); }
VD_CG_INL S3 swxz4(S4 v) { return sm3(v.w, v.x, v.z); }
VD_CG_INL S3 swxw4(S4 v) { return sm3(v.w, v.x, v.w); }
VD_CG_INL S3 swyx4(S4 v) { return sm3(v.w, v.y, v.x); }
VD_CG_INL S3 swyy4(S4 v) { return sm3(v.w, v.y, v.y); }
VD_CG_INL S3 swyz4(S4 v) { return sm3(v.w, v.y, v.z); }
VD_CG_INL S3 swyw4(S4 v) { return sm3(v.w, v.y, v.w); }
VD_CG_INL S3 swzx4(S4 v) { return sm3(v.w, v.z, v.x); }
VD_CG_INL S3 swzy4(S4 v) { return sm3(v.w, v.z, v.y); }
VD_CG_INL S3 swzz4(S4 v) { return sm3(v.w, v.z, v.z); }
VD_CG_INL S3 swzw4(S4 v) { return sm3(v.w, v.z, v.w); }
VD_CG_INL S3 swwx4(S4 v) { return sm3(v.w, v.w, v.x); }
VD_CG_INL S3 swwy4(S4 v) { return sm3(v.w, v.w, v.y); }
VD_CG_INL S3 swwz4(S4 v) { return sm3(v.w, v.w, v.z); }
VD_CG_INL S3 swww4(S4 v) { return sm3(v.w, v.w, v.w); }
VD_CG_INL S4 sxxxx4(S4 v) { return sm4(v.x, v.x, v.x, v.x); }
VD_CG_INL S4 sxxxy4(S4 v) { return sm4(v.x, v.x, v.x, v.y); }
VD_CG_INL S4 sxxxz4(S4 v) { return sm4(v.x, v.x, v.x, v.z); }
VD_CG_INL S4 sxxxw4(S4 v) { return sm4(v.x, v.x, v.x, v.w); }
VD_CG_INL S4 sxxyx4(S4 v) { return sm4(v.x, v.x, v.y, v.x); }
VD_CG_INL S4 sxxyy4(S4 v) { return sm4(v.x, v.x, v.y, v.y); }
VD_CG_INL S4 sxxyz4(S4 v) { return sm4(v.x, v.x, v.y, v.z); }
VD_CG_INL S4 sxxyw4(S4 v) { return sm4(v.x, v.x, v.y, v.w); }
VD_CG_INL S4 sxxzx4(S4 v) { return sm4(v.x, v.x, v.z, v.x); }
VD_CG_INL S4 sxxzy4(S4 v) { return sm4(v.x, v.x, v.z, v.y); }
VD_CG_INL S4 sxxzz4(S4 v) { return sm4(v.x, v.x, v.z, v.z); }
VD_CG_INL S4 sxxzw4(S4 v) { return sm4(v.x, v.x, v.z, v.w); }
VD_CG_INL S4 sxxwx4(S4 v) { return sm4(v.x, v.x, v.w, v.x); }
VD_CG_INL S4 sxxwy4(S4 v) { return sm4(v.x, v.x, v.w, v.y); }
VD_CG_INL S4 sxxwz4(S4 v) { return sm4(v.x, v.x, v.w, v.z); }
VD_CG_INL S4 sxxww4(S4 v) { return sm4(v.x, v.x, v.w, v.w); }
VD_CG_INL S4 sxyxx4(S4 v) { return sm4(v.x, v.y, v.x, v.x); }
VD_CG_INL S4 sxyxy4(S4 v) { return sm4(v.x, v.y, v.x, v.y); }
VD_CG_INL S4 sxyxz4(S4 v) { return sm4(v.x, v.y, v.x, v.z); }
VD_CG_INL S4 sxyxw4(S4 v) { return sm4(v.x, v.y, v.x, v.w); }
VD_CG_INL S4 sxyyx4(S4 v) { return sm4(v.x, v.y, v.y, v.x); }
VD_CG_INL S4 sxyyy4(S4 v) { return sm4(v.x, v.y, v.y, v.y); }
VD_CG_INL S4 sxyyz4(S4 v) { return sm4(v.x, v.y, v.y, v.z); }
VD_CG_INL S4 sxyyw4(S4 v) { return sm4(v.x, v.y, v.y, v.w); }
VD_CG_INL S4 sxyzx4(S4 v) { return sm4(v.x, v.y, v.z, v.x); }
VD_CG_INL S4 sxyzy4(S4 v) { return sm4(v.x, v.y, v.z, v.y); }
VD_CG_INL S4 sxyzz4(S4 v) { return sm4(v.x, v.y, v.z, v.z); }
VD_CG_INL S4 sxyzw4(S4 v) { return sm4(v.x, v.y, v.z, v.w); }
VD_CG_INL S4 sxywx4(S4 v) { return sm4(v.x, v.y, v.w, v.x); }
VD_CG_INL S4 sxywy4(S4 v) { return sm4(v.x, v.y, v.w, v.y); }
VD_CG_INL S4 sxywz4(S4 v) { return sm4(v.x, v.y, v.w, v.z); }
VD_CG_INL S4 sxyww4(S4 v) { return sm4(v.x, v.y, v.w, v.w); }
VD_CG_INL S4 sxzxx4(S4 v) { return sm4(v.x, v.z, v.x, v.x); }
VD_CG_INL S4 sxzxy4(S4 v) { return sm4(v.x, v.z, v.x, v.y); }
VD_CG_INL S4 sxzxz4(S4 v) { return sm4(v.x, v.z, v.x, v.z); }
VD_CG_INL S4 sxzxw4(S4 v) { return sm4(v.x, v.z, v.x, v.w); }
VD_CG_INL S4 sxzyx4(S4 v) { return sm4(v.x, v.z, v.y, v.x); }
VD_CG_INL S4 sxzyy4(S4 v) { return sm4(v.x, v.z, v.y, v.y); }
VD_CG_INL S4 sxzyz4(S4 v) { return sm4(v.x, v.z, v.y, v.z); }
VD_CG_INL S4 sxzyw4(S4 v) { return sm4(v.x, v.z, v.y, v.w); }
VD_CG_INL S4 sxzzx4(S4 v) { return sm4(v.x, v.z, v.z, v.x); }
VD_CG_INL S4 sxzzy4(S4 v) { return sm4(v.x, v.z, v.z, v.y); }
VD_CG_INL S4 sxzzz4(S4 v) { return sm4(v.x, v.z, v.z, v.z); }
VD_CG_INL S4 sxzzw4(S4 v) { return sm4(v.x, v.z, v.z, v.w); }
VD_CG_INL S4 sxzwx4(S4 v) { return sm4(v.x, v.z, v.w, v.x); }
VD_CG_INL S4 sxzwy4(S4 v) { return sm4(v.x, v.z, v.w, v.y); }
VD_CG_INL S4 sxzwz4(S4 v) { return sm4(v.x, v.z, v.w, v.z); }
VD_CG_INL S4 sxzww4(S4 v) { return sm4(v.x, v.z, v.w, v.w); }
VD_CG_INL S4 sxwxx4(S4 v) { return sm4(v.x, v.w, v.x, v.x); }
VD_CG_INL S4 sxwxy4(S4 v) { return sm4(v.x, v.w, v.x, v.y); }
VD_CG_INL S4 sxwxz4(S4 v) { return sm4(v.x, v.w, v.x, v.z); }
VD_CG_INL S4 sxwxw4(S4 v) { return sm4(v.x, v.w, v.x, v.w); }
VD_CG_INL S4 sxwyx4(S4 v) { return sm4(v.x, v.w, v.y, v.x); }
VD_CG_INL S4 sxwyy4(S4 v) { return sm4(v.x, v.w, v.y, v.y); }
VD_CG_INL S4 sxwyz4(S4 v) { return sm4(v.x, v.w, v.y, v.z); }
VD_CG_INL S4 sxwyw4(S4 v) { return sm4(v.x, v.w, v.y, v.w); }
VD_CG_INL S4 sxwzx4(S4 v) { return sm4(v.x, v.w, v.z, v.x); }
VD_CG_INL S4 sxwzy4(S4 v) { return sm4(v.x, v.w, v.z, v.y); }
VD_CG_INL S4 sxwzz4(S4 v) { return sm4(v.x, v.w, v.z, v.z); }
VD_CG_INL S4 sxwzw4(S4 v) { return sm4(v.x, v.w, v.z, v.w); }
VD_CG_INL S4 sxwwx4(S4 v) { return sm4(v.x, v.w, v.w, v.x); }
VD_CG_INL S4 sxwwy4(S4 v) { return sm4(v.x, v.w, v.w, v.y); }
VD_CG_INL S4 sxwwz4(S4 v) { return sm4(v.x, v.w, v.w, v.z); }
VD_CG_INL S4 sxwww4(S4 v) { return sm4(v.x, v.w, v.w, v.w); }
VD_CG_INL S4 syxxx4(S4 v) { return sm4(v.y, v.x, v.x, v.x); }
VD_CG_INL S4 syxxy4(S4 v) { return sm4(v.y, v.x, v.x, v.y); }
VD_CG_INL S4 syxxz4(S4 v) { return sm4(v.y, v.x, v.x, v.z); }
VD_CG_INL S4 syxxw4(S4 v) { return sm4(v.y, v.x, v.x, v.w); }
VD_CG_INL S4 syxyx4(S4 v) { return sm4(v.y, v.x, v.y, v.x); }
VD_CG_INL S4 syxyy4(S4 v) { return sm4(v.y, v.x, v.y, v.y); }
VD_CG_INL S4 syxyz4(S4 v) { return sm4(v.y, v.x, v.y, v.z); }
VD_CG_INL S4 syxyw4(S4 v) { return sm4(v.y, v.x, v.y, v.w); }
VD_CG_INL S4 syxzx4(S4 v) { return sm4(v.y, v.x, v.z, v.x); }
VD_CG_INL S4 syxzy4(S4 v) { return sm4(v.y, v.x, v.z, v.y); }
VD_CG_INL S4 syxzz4(S4 v) { return sm4(v.y, v.x, v.z, v.z); }
VD_CG_INL S4 syxzw4(S4 v) { return sm4(v.y, v.x, v.z, v.w); }
VD_CG_INL S4 syxwx4(S4 v) { return sm4(v.y, v.x, v.w, v.x); }
VD_CG_INL S4 syxwy4(S4 v) { return sm4(v.y, v.x, v.w, v.y); }
VD_CG_INL S4 syxwz4(S4 v) { return sm4(v.y, v.x, v.w, v.z); }
VD_CG_INL S4 syxww4(S4 v) { return sm4(v.y, v.x, v.w, v.w); }
VD_CG_INL S4 syyxx4(S4 v) { return sm4(v.y, v.y, v.x, v.x); }
VD_CG_INL S4 syyxy4(S4 v) { return sm4(v.y, v.y, v.x, v.y); }
VD_CG_INL S4 syyxz4(S4 v) { return sm4(v.y, v.y, v.x, v.z); }
VD_CG_INL S4 syyxw4(S4 v) { return sm4(v.y, v.y, v.x, v.w); }
VD_CG_INL S4 syyyx4(S4 v) { return sm4(v.y, v.y, v.y, v.x); }
VD_CG_INL S4 syyyy4(S4 v) { return sm4(v.y, v.y, v.y, v.y); }
VD_CG_INL S4 syyyz4(S4 v) { return sm4(v.y, v.y, v.y, v.z); }
VD_CG_INL S4 syyyw4(S4 v) { return sm4(v.y, v.y, v.y, v.w); }
VD_CG_INL S4 syyzx4(S4 v) { return sm4(v.y, v.y, v.z, v.x); }
VD_CG_INL S4 syyzy4(S4 v) { return sm4(v.y, v.y, v.z, v.y); }
VD_CG_INL S4 syyzz4(S4 v) { return sm4(v.y, v.y, v.z, v.z); }
VD_CG_INL S4 syyzw4(S4 v) { return sm4(v.y, v.y, v.z, v.w); }
VD_CG_INL S4 syywx4(S4 v) { return sm4(v.y, v.y, v.w, v.x); }
VD_CG_INL S4 syywy4(S4 v) { return sm4(v.y, v.y, v.w, v.y); }
VD_CG_INL S4 syywz4(S4 v) { return sm4(v.y, v.y, v.w, v.z); }
VD_CG_INL S4 syyww4(S4 v) { return sm4(v.y, v.y, v.w, v.w); }
VD_CG_INL S4 syzxx4(S4 v) { return sm4(v.y, v.z, v.x, v.x); }
VD_CG_INL S4 syzxy4(S4 v) { return sm4(v.y, v.z, v.x, v.y); }
VD_CG_INL S4 syzxz4(S4 v) { return sm4(v.y, v.z, v.x, v.z); }
VD_CG_INL S4 syzxw4(S4 v) { return sm4(v.y, v.z, v.x, v.w); }
VD_CG_INL S4 syzyx4(S4 v) { return sm4(v.y, v.z, v.y, v.x); }
VD_CG_INL S4 syzyy4(S4 v) { return sm4(v.y, v.z, v.y, v.y); }
VD_CG_INL S4 syzyz4(S4 v) { return sm4(v.y, v.z, v.y, v.z); }
VD_CG_INL S4 syzyw4(S4 v) { return sm4(v.y, v.z, v.y, v.w); }
VD_CG_INL S4 syzzx4(S4 v) { return sm4(v.y, v.z, v.z, v.x); }
VD_CG_INL S4 syzzy4(S4 v) { return sm4(v.y, v.z, v.z, v.y); }
VD_CG_INL S4 syzzz4(S4 v) { return sm4(v.y, v.z, v.z, v.z); }
VD_CG_INL S4 syzzw4(S4 v) { return sm4(v.y, v.z, v.z, v.w); }
VD_CG_INL S4 syzwx4(S4 v) { return sm4(v.y, v.z, v.w, v.x); }
VD_CG_INL S4 syzwy4(S4 v) { return sm4(v.y, v.z, v.w, v.y); }
VD_CG_INL S4 syzwz4(S4 v) { return sm4(v.y, v.z, v.w, v.z); }
VD_CG_INL S4 syzww4(S4 v) { return sm4(v.y, v.z, v.w, v.w); }
VD_CG_INL S4 sywxx4(S4 v) { return sm4(v.y, v.w, v.x, v.x); }
VD_CG_INL S4 sywxy4(S4 v) { return sm4(v.y, v.w, v.x, v.y); }
VD_CG_INL S4 sywxz4(S4 v) { return sm4(v.y, v.w, v.x, v.z); }
VD_CG_INL S4 sywxw4(S4 v) { return sm4(v.y, v.w, v.x, v.w); }
VD_CG_INL S4 sywyx4(S4 v) { return sm4(v.y, v.w, v.y, v.x); }
VD_CG_INL S4 sywyy4(S4 v) { return sm4(v.y, v.w, v.y, v.y); }
VD_CG_INL S4 sywyz4(S4 v) { return sm4(v.y, v.w, v.y, v.z); }
VD_CG_INL S4 sywyw4(S4 v) { return sm4(v.y, v.w, v.y, v.w); }
VD_CG_INL S4 sywzx4(S4 v) { return sm4(v.y, v.w, v.z, v.x); }
VD_CG_INL S4 sywzy4(S4 v) { return sm4(v.y, v.w, v.z, v.y); }
VD_CG_INL S4 sywzz4(S4 v) { return sm4(v.y, v.w, v.z, v.z); }
VD_CG_INL S4 sywzw4(S4 v) { return sm4(v.y, v.w, v.z, v.w); }
VD_CG_INL S4 sywwx4(S4 v) { return sm4(v.y, v.w, v.w, v.x); }
VD_CG_INL S4 sywwy4(S4 v) { return sm4(v.y, v.w, v.w, v.y); }
VD_CG_INL S4 sywwz4(S4 v) { return sm4(v.y, v.w, v.w, v.z); }
VD_CG_INL S4 sywww4(S4 v) { return sm4(v.y, v.w, v.w, v.w); }
VD_CG_INL S4 szxxx4(S4 v) { return sm4(v.z, v.x, v.x, v.x); }
VD_CG_INL S4 szxxy4(S4 v) { return sm4(v.z, v.x, v.x, v.y); }
VD_CG_INL S4 szxxz4(S4 v) { return sm4(v.z, v.x, v.x, v.z); }
VD_CG_INL S4 szxxw4(S4 v) { return sm4(v.z, v.x, v.x, v.w); }
VD_CG_INL S4 szxyx4(S4 v) { return sm4(v.z, v.x, v.y, v.x); }
VD_CG_INL S4 szxyy4(S4 v) { return sm4(v.z, v.x, v.y, v.y); }
VD_CG_INL S4 szxyz4(S4 v) { return sm4(v.z, v.x, v.y, v.z); }
VD_CG_INL S4 szxyw4(S4 v) { return sm4(v.z, v.x, v.y, v.w); }
VD_CG_INL S4 szxzx4(S4 v) { return sm4(v.z, v.x, v.z, v.x); }
VD_CG_INL S4 szxzy4(S4 v) { return sm4(v.z, v.x, v.z, v.y); }
VD_CG_INL S4 szxzz4(S4 v) { return sm4(v.z, v.x, v.z, v.z); }
VD_CG_INL S4 szxzw4(S4 v) { return sm4(v.z, v.x, v.z, v.w); }
VD_CG_INL S4 szxwx4(S4 v) { return sm4(v.z, v.x, v.w, v.x); }
VD_CG_INL S4 szxwy4(S4 v) { return sm4(v.z, v.x, v.w, v.y); }
VD_CG_INL S4 szxwz4(S4 v) { return sm4(v.z, v.x, v.w, v.z); }
VD_CG_INL S4 szxww4(S4 v) { return sm4(v.z, v.x, v.w, v.w); }
VD_CG_INL S4 szyxx4(S4 v) { return sm4(v.z, v.y, v.x, v.x); }
VD_CG_INL S4 szyxy4(S4 v) { return sm4(v.z, v.y, v.x, v.y); }
VD_CG_INL S4 szyxz4(S4 v) { return sm4(v.z, v.y, v.x, v.z); }
VD_CG_INL S4 szyxw4(S4 v) { return sm4(v.z, v.y, v.x, v.w); }
VD_CG_INL S4 szyyx4(S4 v) { return sm4(v.z, v.y, v.y, v.x); }
VD_CG_INL S4 szyyy4(S4 v) { return sm4(v.z, v.y, v.y, v.y); }
VD_CG_INL S4 szyyz4(S4 v) { return sm4(v.z, v.y, v.y, v.z); }
VD_CG_INL S4 szyyw4(S4 v) { return sm4(v.z, v.y, v.y, v.w); }
VD_CG_INL S4 szyzx4(S4 v) { return sm4(v.z, v.y, v.z, v.x); }
VD_CG_INL S4 szyzy4(S4 v) { return sm4(v.z, v.y, v.z, v.y); }
VD_CG_INL S4 szyzz4(S4 v) { return sm4(v.z, v.y, v.z, v.z); }
VD_CG_INL S4 szyzw4(S4 v) { return sm4(v.z, v.y, v.z, v.w); }
VD_CG_INL S4 szywx4(S4 v) { return sm4(v.z, v.y, v.w, v.x); }
VD_CG_INL S4 szywy4(S4 v) { return sm4(v.z, v.y, v.w, v.y); }
VD_CG_INL S4 szywz4(S4 v) { return sm4(v.z, v.y, v.w, v.z); }
VD_CG_INL S4 szyww4(S4 v) { return sm4(v.z, v.y, v.w, v.w); }
VD_CG_INL S4 szzxx4(S4 v) { return sm4(v.z, v.z, v.x, v.x); }
VD_CG_INL S4 szzxy4(S4 v) { return sm4(v.z, v.z, v.x, v.y); }
VD_CG_INL S4 szzxz4(S4 v) { return sm4(v.z, v.z, v.x, v.z); }
VD_CG_INL S4 szzxw4(S4 v) { return sm4(v.z, v.z, v.x, v.w); }
VD_CG_INL S4 szzyx4(S4 v) { return sm4(v.z, v.z, v.y, v.x); }
VD_CG_INL S4 szzyy4(S4 v) { return sm4(v.z, v.z, v.y, v.y); }
VD_CG_INL S4 szzyz4(S4 v) { return sm4(v.z, v.z, v.y, v.z); }
VD_CG_INL S4 szzyw4(S4 v) { return sm4(v.z, v.z, v.y, v.w); }
VD_CG_INL S4 szzzx4(S4 v) { return sm4(v.z, v.z, v.z, v.x); }
VD_CG_INL S4 szzzy4(S4 v) { return sm4(v.z, v.z, v.z, v.y); }
VD_CG_INL S4 szzzz4(S4 v) { return sm4(v.z, v.z, v.z, v.z); }
VD_CG_INL S4 szzzw4(S4 v) { return sm4(v.z, v.z, v.z, v.w); }
VD_CG_INL S4 szzwx4(S4 v) { return sm4(v.z, v.z, v.w, v.x); }
VD_CG_INL S4 szzwy4(S4 v) { return sm4(v.z, v.z, v.w, v.y); }
VD_CG_INL S4 szzwz4(S4 v) { return sm4(v.z, v.z, v.w, v.z); }
VD_CG_INL S4 szzww4(S4 v) { return sm4(v.z, v.z, v.w, v.w); }
VD_CG_INL S4 szwxx4(S4 v) { return sm4(v.z, v.w, v.x, v.x); }
VD_CG_INL S4 szwxy4(S4 v) { return sm4(v.z, v.w, v.x, v.y); }
VD_CG_INL S4 szwxz4(S4 v) { return sm4(v.z, v.w, v.x, v.z); }
VD_CG_INL S4 szwxw4(S4 v) { return sm4(v.z, v.w, v.x, v.w); }
VD_CG_INL S4 szwyx4(S4 v) { return sm4(v.z, v.w, v.y, v.x); }
VD_CG_INL S4 szwyy4(S4 v) { return sm4(v.z, v.w, v.y, v.y); }
VD_CG_INL S4 szwyz4(S4 v) { return sm4(v.z, v.w, v.y, v.z); }
VD_CG_INL S4 szwyw4(S4 v) { return sm4(v.z, v.w, v.y, v.w); }
VD_CG_INL S4 szwzx4(S4 v) { return sm4(v.z, v.w, v.z, v.x); }
VD_CG_INL S4 szwzy4(S4 v) { return sm4(v.z, v.w, v.z, v.y); }
VD_CG_INL S4 szwzz4(S4 v) { return sm4(v.z, v.w, v.z, v.z); }
VD_CG_INL S4 szwzw4(S4 v) { return sm4(v.z, v.w, v.z, v.w); }
VD_CG_INL S4 szwwx4(S4 v) { return sm4(v.z, v.w, v.w, v.x); }
VD_CG_INL S4 szwwy4(S4 v) { return sm4(v.z, v.w, v.w, v.y); }
VD_CG_INL S4 szwwz4(S4 v) { return sm4(v.z, v.w, v.w, v.z); }
VD_CG_INL S4 szwww4(S4 v) { return sm4(v.z, v.w, v.w, v.w); }
VD_CG_INL S4 swxxx4(S4 v) { return sm4(v.w, v.x, v.x, v.x); }
VD_CG_INL S4 swxxy4(S4 v) { return sm4(v.w, v.x, v.x, v.y); }
VD_CG_INL S4 swxxz4(S4 v) { return sm4(v.w, v.x, v.x, v.z); }
VD_CG_INL S4 swxxw4(S4 v) { return sm4(v.w, v.x, v.x, v.w); }
VD_CG_INL S4 swxyx4(S4 v) { return sm4(v.w, v.x, v.y, v.x); }
VD_CG_INL S4 swxyy4(S4 v) { return sm4(v.w, v.x, v.y, v.y); }
VD_CG_INL S4 swxyz4(S4 v) { return sm4(v.w, v.x, v.y, v.z); }
VD_CG_INL S4 swxyw4(S4 v) { return sm4(v.w, v.x, v.y, v.w); }
VD_CG_INL S4 swxzx4(S4 v) { return sm4(v.w, v.x, v.z, v.x); }
VD_CG_INL S4 swxzy4(S4 v) { return sm4(v.w, v.x, v.z, v.y); }
VD_CG_INL S4 swxzz4(S4 v) { return sm4(v.w, v.x, v.z, v.z); }
VD_CG_INL S4 swxzw4(S4 v) { return sm4(v.w, v.x, v.z, v.w); }
VD_CG_INL S4 swxwx4(S4 v) { return sm4(v.w, v.x, v.w, v.x); }
VD_CG_INL S4 swxwy4(S4 v) { return sm4(v.w, v.x, v.w, v.y); }
VD_CG_INL S4 swxwz4(S4 v) { return sm4(v.w, v.x, v.w, v.z); }
VD_CG_INL S4 swxww4(S4 v) { return sm4(v.w, v.x, v.w, v.w); }
VD_CG_INL S4 swyxx4(S4 v) { return sm4(v.w, v.y, v.x, v.x); }
VD_CG_INL S4 swyxy4(S4 v) { return sm4(v.w, v.y, v.x, v.y); }
VD_CG_INL S4 swyxz4(S4 v) { return sm4(v.w, v.y, v.x, v.z); }
VD_CG_INL S4 swyxw4(S4 v) { return sm4(v.w, v.y, v.x, v.w); }
VD_CG_INL S4 swyyx4(S4 v) { return sm4(v.w, v.y, v.y, v.x); }
VD_CG_INL S4 swyyy4(S4 v) { return sm4(v.w, v.y, v.y, v.y); }
VD_CG_INL S4 swyyz4(S4 v) { return sm4(v.w, v.y, v.y, v.z); }
VD_CG_INL S4 swyyw4(S4 v) { return sm4(v.w, v.y, v.y, v.w); }
VD_CG_INL S4 swyzx4(S4 v) { return sm4(v.w, v.y, v.z, v.x); }
VD_CG_INL S4 swyzy4(S4 v) { return sm4(v.w, v.y, v.z, v.y); }
VD_CG_INL S4 swyzz4(S4 v) { return sm4(v.w, v.y, v.z, v.z); }
VD_CG_INL S4 swyzw4(S4 v) { return sm4(v.w, v.y, v.z, v.w); }
VD_CG_INL S4 swywx4(S4 v) { return sm4(v.w, v.y, v.w, v.x); }
VD_CG_INL S4 swywy4(S4 v) { return sm4(v.w, v.y, v.w, v.y); }
VD_CG_INL S4 swywz4(S4 v) { return sm4(v.w, v.y, v.w, v.z); }
VD_CG_INL S4 swyww4(S4 v) { return sm4(v.w, v.y, v.w, v.w); }
VD_CG_INL S4 swzxx4(S4 v) { return sm4(v.w, v.z, v.x, v.x); }
VD_CG_INL S4 swzxy4(S4 v) { return sm4(v.w, v.z, v.x, v.y); }
VD_CG_INL S4 swzxz4(S4 v) { return sm4(v.w, v.z, v.x, v.z); }
VD_CG_INL S4 swzxw4(S4 v) { return sm4(v.w, v.z, v.x, v.w); }
VD_CG_INL S4 swzyx4(S4 v) { return sm4(v.w, v.z, v.y, v.x); }
VD_CG_INL S4 swzyy4(S4 v) { return sm4(v.w, v.z, v.y, v.y); }
VD_CG_INL S4 swzyz4(S4 v) { return sm4(v.w, v.z, v.y, v.z); }
VD_CG_INL S4 swzyw4(S4 v) { return sm4(v.w, v.z, v.y, v.w); }
VD_CG_INL S4 swzzx4(S4 v) { return sm4(v.w, v.z, v.z, v.x); }
VD_CG_INL S4 swzzy4(S4 v) { return sm4(v.w, v.z, v.z, v.y); }
VD_CG_INL S4 swzzz4(S4 v) { return sm4(v.w, v.z, v.z, v.z); }
VD_CG_INL S4 swzzw4(S4 v) { return sm4(v.w, v.z, v.z, v.w); }
VD_CG_INL S4 swzwx4(S4 v) { return sm4(v.w, v.z, v.w, v.x); }
VD_CG_INL S4 swzwy4(S4 v) { return sm4(v.w, v.z, v.w, v.y); }
VD_CG_INL S4 swzwz4(S4 v) { return sm4(v.w, v.z, v.w, v.z); }
VD_CG_INL S4 swzww4(S4 v) { return sm4(v.w, v.z, v.w, v.w); }
VD_CG_INL S4 swwxx4(S4 v) { return sm4(v.w, v.w, v.x, v.x); }
VD_CG_INL S4 swwxy4(S4 v) { return sm4(v.w, v.w, v.x, v.y); }
VD_CG_INL S4 swwxz4(S4 v) { return sm4(v.w, v.w, v.x, v.z); }
VD_CG_INL S4 swwxw4(S4 v) { return sm4(v.w, v.w, v.x, v.w); }
VD_CG_INL S4 swwyx4(S4 v) { return sm4(v.w, v.w, v.y, v.x); }
VD_CG_INL S4 swwyy4(S4 v) { return sm4(v.w, v.w, v.y, v.y); }
VD_CG_INL S4 swwyz4(S4 v) { return sm4(v.w, v.w, v.y, v.z); }
VD_CG_INL S4 swwyw4(S4 v) { return sm4(v.w, v.w, v.y, v.w); }
VD_CG_INL S4 swwzx4(S4 v) { return sm4(v.w, v.w, v.z, v.x); }
VD_CG_INL S4 swwzy4(S4 v) { return sm4(v.w, v.w, v.z, v.y); }
VD_CG_INL S4 swwzz4(S4 v) { return sm4(v.w, v.w, v.z, v.z); }
VD_CG_INL S4 swwzw4(S4 v) { return sm4(v.w, v.w, v.z, v.w); }
VD_CG_INL S4 swwwx4(S4 v) { return sm4(v.w, v.w, v.w, v.x); }
VD_CG_INL S4 swwwy4(S4 v) { return sm4(v.w, v.w, v.w, v.y); }
VD_CG_INL S4 swwwz4(S4 v) { return sm4(v.w, v.w, v.w, v.z); }
VD_CG_INL S4 swwww4(S4 v) { return sm4(v.w, v.w, v.w, v.w); }

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