/**
 * vd_fw.h - Gets you a window with OpenGL running on platforms that support it
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
#ifndef VD_FW_H
#define VD_FW_H
#define VD_FW_VERSION_MAJOR    0
#define VD_FW_VERSION_MINOR    0
#define VD_FW_VERSION_PATCH    1
#define VD_FW_VERSION          ((VD_FW_VERSION_MAJOR << 16) | (VD_FW_VERSION_MINOR << 8) | (VD_FW_VERSION_PATCH))

#if defined(__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#import <OpenGL/gl3.h>
#endif
 

typedef enum {
    VD_FW_GL_VERSION_BASIC = 0,
    VD_FW_GL_VERSION_1_0   = 1,
    VD_FW_GL_VERSION_1_2   = 12,
    VD_FW_GL_VERSION_1_3   = 13,
    VD_FW_GL_VERSION_1_4   = 14,
    VD_FW_GL_VERSION_1_5   = 15,
    VD_FW_GL_VERSION_2_0   = 20,
    VD_FW_GL_VERSION_2_1   = 21,
    VD_FW_GL_VERSION_3_0   = 30,
    VD_FW_GL_VERSION_3_1   = 31,
    VD_FW_GL_VERSION_3_2   = 32,
    VD_FW_GL_VERSION_3_3   = 33,
} VdFwGlVersion;

typedef struct __VD_FW_InitInfo {
    struct {
        VdFwGlVersion version;
        int           debug_on;
    } gl;

    struct {
        int           draw_default_borders;
    } window_options;
} VdFwInitInfo;

extern int                vd_fw_init(VdFwInitInfo *info);
extern int                vd_fw_running(void);
extern int                vd_fw_swap_buffers(void);
extern int                vd_fw_get_size(int *w, int *h);
extern unsigned long long vd_fw_delta_ns(void);
extern int                vd_fw_set_vsync_on(int on);
extern void               vd_fw_draw_window_border(void);
extern int                vd_fw_get_mouse_state(int *x, int *y);
static inline int         vd_fw_get_mouse_statef(float *x, float *y);
static inline float       vd_fw_delta_s(void);
static inline void        vd_fw_u_ortho(float left, float right, float bottom, float top, float near, float far, float out[16]);
static inline int         vd_fw_u_point_in_rect(float x, float y, float rx, float ry, float rw, float rh);

static inline float vd_fw_delta_s(void)
{
    unsigned long long ns  = vd_fw_delta_ns();
    double ms              = (double)ns / 1000000.0;
    double s64             = ms         / 1000.0;
    float s                = (float)s64;
    return s;
}

static inline int vd_fw_u_point_in_rect(float x, float y, float rx, float ry, float rw, float rh)
{
    return (x >= rx) && (x <= (rx + rw)) &&
           (y >= ry) && (y <= (ry + rh));
}

static inline int vd_fw_get_mouse_statef(float *x, float *y)
{
    int xi, yi;
    int result = vd_fw_get_mouse_state(&xi, &yi);

    if (x) *x = (float)xi;
    if (y) *y = (float)yi;

    return result;
}

static inline void vd_fw_u_ortho(float left, float right, float bottom, float top, float near, float far, float out[16])
{
    out[0]  = 2.0f / (right - left);               out[1]  = 0.0f;                              out[2]  = 0.0f;                          out[3]  = 0.0f;
    out[4]  = 0.0f;                                out[5]  = 2.0f / (top - bottom);             out[6]  = 0.0f;                          out[7]  = 0.0f;
    out[8]  = 0.0f;                                out[9]  = 0.0f;                              out[10] = -2.0f / (far - near);          out[11] = 0.0f;
    out[12] = - (right + left) / (right - left);   out[13] = - (top + bottom) / (top - bottom); out[14] = - (far + near) / (far - near); out[15] = 1.0f;
}

#if _WIN32
#define VD_FW_WIN32_SUBSYSTEM_CONSOLE 1
#define VD_FW_WIN32_SUBSYSTEM_WINDOWS 2

#ifndef VD_FW_WIN32_SUBSYSTEM
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#endif // !VD_FW_WIN32_SUBSYSTEM

#ifndef VD_FW_NO_CRT
#define VD_FW_NO_CRT 0
#endif
#endif // _WIN32

#if !defined(__APPLE__)
/* ----GL TYPEDEFS--------------------------------------------------------------------------------------------------- */
typedef void               GLvoid;
typedef unsigned int       GLenum;
typedef float              GLfloat;
typedef int                GLint;
typedef int                GLsizei;
typedef unsigned int       GLbitfield;
typedef double             GLdouble;
typedef unsigned int       GLuint;
typedef unsigned char      GLboolean;
typedef unsigned char      GLubyte;
typedef float              GLclampf;
typedef double             GLclampd;
typedef char               GLbyte;
typedef signed short       GLshort;
typedef unsigned short     GLushort;
typedef signed long int    GLsizeiptr;
typedef signed long int    GLintptr;
typedef char               GLchar;
typedef unsigned short int GLhalf;
typedef struct __GLsync *  GLsync;
#ifdef _WIN32
typedef unsigned __int64   GLuint64;
typedef __int64            GLint64;
#endif // _WIN32

/* ----GL CONSTANTS-------------------------------------------------------------------------------------------------- */
#define GL_DEPTH_BUFFER_BIT                              0x00000100
#define GL_STENCIL_BUFFER_BIT                            0x00000400
#define GL_COLOR_BUFFER_BIT                              0x00004000
#define GL_FALSE                                         0
#define GL_TRUE                                          1
#define GL_POINTS                                        0x0000
#define GL_LINES                                         0x0001
#define GL_LINE_LOOP                                     0x0002
#define GL_LINE_STRIP                                    0x0003
#define GL_TRIANGLES                                     0x0004
#define GL_TRIANGLE_STRIP                                0x0005
#define GL_TRIANGLE_FAN                                  0x0006
#define GL_QUADS                                         0x0007
#define GL_NEVER                                         0x0200
#define GL_LESS                                          0x0201
#define GL_EQUAL                                         0x0202
#define GL_LEQUAL                                        0x0203
#define GL_GREATER                                       0x0204
#define GL_NOTEQUAL                                      0x0205
#define GL_GEQUAL                                        0x0206
#define GL_ALWAYS                                        0x0207
#define GL_ZERO                                          0
#define GL_ONE                                           1
#define GL_SRC_COLOR                                     0x0300
#define GL_ONE_MINUS_SRC_COLOR                           0x0301
#define GL_SRC_ALPHA                                     0x0302
#define GL_ONE_MINUS_SRC_ALPHA                           0x0303
#define GL_DST_ALPHA                                     0x0304
#define GL_ONE_MINUS_DST_ALPHA                           0x0305
#define GL_DST_COLOR                                     0x0306
#define GL_ONE_MINUS_DST_COLOR                           0x0307
#define GL_SRC_ALPHA_SATURATE                            0x0308
#define GL_NONE                                          0
#define GL_FRONT_LEFT                                    0x0400
#define GL_FRONT_RIGHT                                   0x0401
#define GL_BACK_LEFT                                     0x0402
#define GL_BACK_RIGHT                                    0x0403
#define GL_FRONT                                         0x0404
#define GL_BACK                                          0x0405
#define GL_LEFT                                          0x0406
#define GL_RIGHT                                         0x0407
#define GL_FRONT_AND_BACK                                0x0408
#define GL_NO_ERROR                                      0
#define GL_INVALID_ENUM                                  0x0500
#define GL_INVALID_VALUE                                 0x0501
#define GL_INVALID_OPERATION                             0x0502
#define GL_OUT_OF_MEMORY                                 0x0505
#define GL_CW                                            0x0900
#define GL_CCW                                           0x0901
#define GL_POINT_SIZE                                    0x0B11
#define GL_POINT_SIZE_RANGE                              0x0B12
#define GL_POINT_SIZE_GRANULARITY                        0x0B13
#define GL_LINE_SMOOTH                                   0x0B20
#define GL_LINE_WIDTH                                    0x0B21
#define GL_LINE_WIDTH_RANGE                              0x0B22
#define GL_LINE_WIDTH_GRANULARITY                        0x0B23
#define GL_POLYGON_MODE                                  0x0B40
#define GL_POLYGON_SMOOTH                                0x0B41
#define GL_CULL_FACE                                     0x0B44
#define GL_CULL_FACE_MODE                                0x0B45
#define GL_FRONT_FACE                                    0x0B46
#define GL_DEPTH_RANGE                                   0x0B70
#define GL_DEPTH_TEST                                    0x0B71
#define GL_DEPTH_WRITEMASK                               0x0B72
#define GL_DEPTH_CLEAR_VALUE                             0x0B73
#define GL_DEPTH_FUNC                                    0x0B74
#define GL_STENCIL_TEST                                  0x0B90
#define GL_STENCIL_CLEAR_VALUE                           0x0B91
#define GL_STENCIL_FUNC                                  0x0B92
#define GL_STENCIL_VALUE_MASK                            0x0B93
#define GL_STENCIL_FAIL                                  0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL                       0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS                       0x0B96
#define GL_STENCIL_REF                                   0x0B97
#define GL_STENCIL_WRITEMASK                             0x0B98
#define GL_VIEWPORT                                      0x0BA2
#define GL_DITHER                                        0x0BD0
#define GL_BLEND_DST                                     0x0BE0
#define GL_BLEND_SRC                                     0x0BE1
#define GL_BLEND                                         0x0BE2
#define GL_LOGIC_OP_MODE                                 0x0BF0
#define GL_DRAW_BUFFER                                   0x0C01
#define GL_READ_BUFFER                                   0x0C02
#define GL_SCISSOR_BOX                                   0x0C10
#define GL_SCISSOR_TEST                                  0x0C11
#define GL_COLOR_CLEAR_VALUE                             0x0C22
#define GL_COLOR_WRITEMASK                               0x0C23
#define GL_DOUBLEBUFFER                                  0x0C32
#define GL_STEREO                                        0x0C33
#define GL_LINE_SMOOTH_HINT                              0x0C52
#define GL_POLYGON_SMOOTH_HINT                           0x0C53
#define GL_UNPACK_SWAP_BYTES                             0x0CF0
#define GL_UNPACK_LSB_FIRST                              0x0CF1
#define GL_UNPACK_ROW_LENGTH                             0x0CF2
#define GL_UNPACK_SKIP_ROWS                              0x0CF3
#define GL_UNPACK_SKIP_PIXELS                            0x0CF4
#define GL_UNPACK_ALIGNMENT                              0x0CF5
#define GL_PACK_SWAP_BYTES                               0x0D00
#define GL_PACK_LSB_FIRST                                0x0D01
#define GL_PACK_ROW_LENGTH                               0x0D02
#define GL_PACK_SKIP_ROWS                                0x0D03
#define GL_PACK_SKIP_PIXELS                              0x0D04
#define GL_PACK_ALIGNMENT                                0x0D05
#define GL_MAX_TEXTURE_SIZE                              0x0D33
#define GL_MAX_VIEWPORT_DIMS                             0x0D3A
#define GL_SUBPIXEL_BITS                                 0x0D50
#define GL_TEXTURE_1D                                    0x0DE0
#define GL_TEXTURE_2D                                    0x0DE1
#define GL_TEXTURE_WIDTH                                 0x1000
#define GL_TEXTURE_HEIGHT                                0x1001
#define GL_TEXTURE_BORDER_COLOR                          0x1004
#define GL_DONT_CARE                                     0x1100
#define GL_FASTEST                                       0x1101
#define GL_NICEST                                        0x1102
#define GL_BYTE                                          0x1400
#define GL_UNSIGNED_BYTE                                 0x1401
#define GL_SHORT                                         0x1402
#define GL_UNSIGNED_SHORT                                0x1403
#define GL_INT                                           0x1404
#define GL_UNSIGNED_INT                                  0x1405
#define GL_FLOAT                                         0x1406
#define GL_STACK_OVERFLOW                                0x0503
#define GL_STACK_UNDERFLOW                               0x0504
#define GL_CLEAR                                         0x1500
#define GL_AND                                           0x1501
#define GL_AND_REVERSE                                   0x1502
#define GL_COPY                                          0x1503
#define GL_AND_INVERTED                                  0x1504
#define GL_NOOP                                          0x1505
#define GL_XOR                                           0x1506
#define GL_OR                                            0x1507
#define GL_NOR                                           0x1508
#define GL_EQUIV                                         0x1509
#define GL_INVERT                                        0x150A
#define GL_OR_REVERSE                                    0x150B
#define GL_COPY_INVERTED                                 0x150C
#define GL_OR_INVERTED                                   0x150D
#define GL_NAND                                          0x150E
#define GL_SET                                           0x150F
#define GL_TEXTURE                                       0x1702
#define GL_COLOR                                         0x1800
#define GL_DEPTH                                         0x1801
#define GL_STENCIL                                       0x1802
#define GL_STENCIL_INDEX                                 0x1901
#define GL_DEPTH_COMPONENT                               0x1902
#define GL_RED                                           0x1903
#define GL_GREEN                                         0x1904
#define GL_BLUE                                          0x1905
#define GL_ALPHA                                         0x1906
#define GL_RGB                                           0x1907
#define GL_RGBA                                          0x1908
#define GL_POINT                                         0x1B00
#define GL_LINE                                          0x1B01
#define GL_FILL                                          0x1B02
#define GL_KEEP                                          0x1E00
#define GL_REPLACE                                       0x1E01
#define GL_INCR                                          0x1E02
#define GL_DECR                                          0x1E03
#define GL_VENDOR                                        0x1F00
#define GL_RENDERER                                      0x1F01
#define GL_VERSION                                       0x1F02
#define GL_EXTENSIONS                                    0x1F03
#define GL_NEAREST                                       0x2600
#define GL_LINEAR                                        0x2601
#define GL_NEAREST_MIPMAP_NEAREST                        0x2700
#define GL_LINEAR_MIPMAP_NEAREST                         0x2701
#define GL_NEAREST_MIPMAP_LINEAR                         0x2702
#define GL_LINEAR_MIPMAP_LINEAR                          0x2703
#define GL_TEXTURE_MAG_FILTER                            0x2800
#define GL_TEXTURE_MIN_FILTER                            0x2801
#define GL_TEXTURE_WRAP_S                                0x2802
#define GL_TEXTURE_WRAP_T                                0x2803
#define GL_REPEAT                                        0x2901
#define GL_COLOR_LOGIC_OP                                0x0BF2
#define GL_POLYGON_OFFSET_UNITS                          0x2A00
#define GL_POLYGON_OFFSET_POINT                          0x2A01
#define GL_POLYGON_OFFSET_LINE                           0x2A02
#define GL_POLYGON_OFFSET_FILL                           0x8037
#define GL_POLYGON_OFFSET_FACTOR                         0x8038
#define GL_TEXTURE_BINDING_1D                            0x8068
#define GL_TEXTURE_BINDING_2D                            0x8069
#define GL_TEXTURE_INTERNAL_FORMAT                       0x1003
#define GL_TEXTURE_RED_SIZE                              0x805C
#define GL_TEXTURE_GREEN_SIZE                            0x805D
#define GL_TEXTURE_BLUE_SIZE                             0x805E
#define GL_TEXTURE_ALPHA_SIZE                            0x805F
#define GL_DOUBLE                                        0x140A
#define GL_PROXY_TEXTURE_1D                              0x8063
#define GL_PROXY_TEXTURE_2D                              0x8064
#define GL_R3_G3_B2                                      0x2A10
#define GL_RGB4                                          0x804F
#define GL_RGB5                                          0x8050
#define GL_RGB8                                          0x8051
#define GL_RGB10                                         0x8052
#define GL_RGB12                                         0x8053
#define GL_RGB16                                         0x8054
#define GL_RGBA2                                         0x8055
#define GL_RGBA4                                         0x8056
#define GL_RGB5_A1                                       0x8057
#define GL_RGBA8                                         0x8058
#define GL_RGB10_A2                                      0x8059
#define GL_RGBA12                                        0x805A
#define GL_RGBA16                                        0x805B
#define GL_VERTEX_ARRAY                                  0x8074
#define GL_UNSIGNED_BYTE_3_3_2                           0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4                        0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1                        0x8034
#define GL_UNSIGNED_INT_8_8_8_8                          0x8035
#define GL_UNSIGNED_INT_10_10_10_2                       0x8036
#define GL_TEXTURE_BINDING_3D                            0x806A
#define GL_PACK_SKIP_IMAGES                              0x806B
#define GL_PACK_IMAGE_HEIGHT                             0x806C
#define GL_UNPACK_SKIP_IMAGES                            0x806D
#define GL_UNPACK_IMAGE_HEIGHT                           0x806E
#define GL_TEXTURE_3D                                    0x806F
#define GL_PROXY_TEXTURE_3D                              0x8070
#define GL_TEXTURE_DEPTH                                 0x8071
#define GL_TEXTURE_WRAP_R                                0x8072
#define GL_MAX_3D_TEXTURE_SIZE                           0x8073
#define GL_UNSIGNED_BYTE_2_3_3_REV                       0x8362
#define GL_UNSIGNED_SHORT_5_6_5                          0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV                      0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV                    0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV                    0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV                      0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV                   0x8368
#define GL_BGR                                           0x80E0
#define GL_BGRA                                          0x80E1
#define GL_MAX_ELEMENTS_VERTICES                         0x80E8
#define GL_MAX_ELEMENTS_INDICES                          0x80E9
#define GL_CLAMP_TO_EDGE                                 0x812F
#define GL_TEXTURE_MIN_LOD                               0x813A
#define GL_TEXTURE_MAX_LOD                               0x813B
#define GL_TEXTURE_BASE_LEVEL                            0x813C
#define GL_TEXTURE_MAX_LEVEL                             0x813D
#define GL_SMOOTH_POINT_SIZE_RANGE                       0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY                 0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE                       0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY                 0x0B23
#define GL_ALIASED_LINE_WIDTH_RANGE                      0x846E
#define GL_TEXTURE0                                      0x84C0
#define GL_TEXTURE1                                      0x84C1
#define GL_TEXTURE2                                      0x84C2
#define GL_TEXTURE3                                      0x84C3
#define GL_TEXTURE4                                      0x84C4
#define GL_TEXTURE5                                      0x84C5
#define GL_TEXTURE6                                      0x84C6
#define GL_TEXTURE7                                      0x84C7
#define GL_TEXTURE8                                      0x84C8
#define GL_TEXTURE9                                      0x84C9
#define GL_TEXTURE10                                     0x84CA
#define GL_TEXTURE11                                     0x84CB
#define GL_TEXTURE12                                     0x84CC
#define GL_TEXTURE13                                     0x84CD
#define GL_TEXTURE14                                     0x84CE
#define GL_TEXTURE15                                     0x84CF
#define GL_TEXTURE16                                     0x84D0
#define GL_TEXTURE17                                     0x84D1
#define GL_TEXTURE18                                     0x84D2
#define GL_TEXTURE19                                     0x84D3
#define GL_TEXTURE20                                     0x84D4
#define GL_TEXTURE21                                     0x84D5
#define GL_TEXTURE22                                     0x84D6
#define GL_TEXTURE23                                     0x84D7
#define GL_TEXTURE24                                     0x84D8
#define GL_TEXTURE25                                     0x84D9
#define GL_TEXTURE26                                     0x84DA
#define GL_TEXTURE27                                     0x84DB
#define GL_TEXTURE28                                     0x84DC
#define GL_TEXTURE29                                     0x84DD
#define GL_TEXTURE30                                     0x84DE
#define GL_TEXTURE31                                     0x84DF
#define GL_ACTIVE_TEXTURE                                0x84E0
#define GL_MULTISAMPLE                                   0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE                      0x809E
#define GL_SAMPLE_ALPHA_TO_ONE                           0x809F
#define GL_SAMPLE_COVERAGE                               0x80A0
#define GL_SAMPLE_BUFFERS                                0x80A8
#define GL_SAMPLES                                       0x80A9
#define GL_SAMPLE_COVERAGE_VALUE                         0x80AA
#define GL_SAMPLE_COVERAGE_INVERT                        0x80AB
#define GL_TEXTURE_CUBE_MAP                              0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP                      0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X                   0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X                   0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y                   0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y                   0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z                   0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z                   0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP                        0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE                     0x851C
#define GL_COMPRESSED_RGB                                0x84ED
#define GL_COMPRESSED_RGBA                               0x84EE
#define GL_TEXTURE_COMPRESSION_HINT                      0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE                 0x86A0
#define GL_TEXTURE_COMPRESSED                            0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS                0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS                    0x86A3
#define GL_CLAMP_TO_BORDER                               0x812D
#define GL_BLEND_DST_RGB                                 0x80C8
#define GL_BLEND_SRC_RGB                                 0x80C9
#define GL_BLEND_DST_ALPHA                               0x80CA
#define GL_BLEND_SRC_ALPHA                               0x80CB
#define GL_POINT_FADE_THRESHOLD_SIZE                     0x8128
#define GL_DEPTH_COMPONENT16                             0x81A5
#define GL_DEPTH_COMPONENT24                             0x81A6
#define GL_DEPTH_COMPONENT32                             0x81A7
#define GL_MIRRORED_REPEAT                               0x8370
#define GL_MAX_TEXTURE_LOD_BIAS                          0x84FD
#define GL_TEXTURE_LOD_BIAS                              0x8501
#define GL_INCR_WRAP                                     0x8507
#define GL_DECR_WRAP                                     0x8508
#define GL_TEXTURE_DEPTH_SIZE                            0x884A
#define GL_TEXTURE_COMPARE_MODE                          0x884C
#define GL_TEXTURE_COMPARE_FUNC                          0x884D
#define GL_BLEND_COLOR                                   0x8005
#define GL_BLEND_EQUATION                                0x8009
#define GL_CONSTANT_COLOR                                0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR                      0x8002
#define GL_CONSTANT_ALPHA                                0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA                      0x8004
#define GL_FUNC_ADD                                      0x8006
#define GL_FUNC_REVERSE_SUBTRACT                         0x800B
#define GL_FUNC_SUBTRACT                                 0x800A
#define GL_MIN                                           0x8007
#define GL_MAX                                           0x8008
#define GL_BUFFER_SIZE                                   0x8764
#define GL_BUFFER_USAGE                                  0x8765
#define GL_QUERY_COUNTER_BITS                            0x8864
#define GL_CURRENT_QUERY                                 0x8865
#define GL_QUERY_RESULT                                  0x8866
#define GL_QUERY_RESULT_AVAILABLE                        0x8867
#define GL_ARRAY_BUFFER                                  0x8892
#define GL_ELEMENT_ARRAY_BUFFER                          0x8893
#define GL_ARRAY_BUFFER_BINDING                          0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING                  0x8895
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING            0x889F
#define GL_READ_ONLY                                     0x88B8
#define GL_WRITE_ONLY                                    0x88B9
#define GL_READ_WRITE                                    0x88BA
#define GL_BUFFER_ACCESS                                 0x88BB
#define GL_BUFFER_MAPPED                                 0x88BC
#define GL_BUFFER_MAP_POINTER                            0x88BD
#define GL_STREAM_DRAW                                   0x88E0
#define GL_STREAM_READ                                   0x88E1
#define GL_STREAM_COPY                                   0x88E2
#define GL_STATIC_DRAW                                   0x88E4
#define GL_STATIC_READ                                   0x88E5
#define GL_STATIC_COPY                                   0x88E6
#define GL_DYNAMIC_DRAW                                  0x88E8
#define GL_DYNAMIC_READ                                  0x88E9
#define GL_DYNAMIC_COPY                                  0x88EA
#define GL_SAMPLES_PASSED                                0x8914
#define GL_SRC1_ALPHA                                    0x8589
#define GL_BLEND_EQUATION_RGB                            0x8009
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED                   0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE                      0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE                    0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE                      0x8625
#define GL_CURRENT_VERTEX_ATTRIB                         0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE                     0x8642
#define GL_VERTEX_ATTRIB_ARRAY_POINTER                   0x8645
#define GL_STENCIL_BACK_FUNC                             0x8800
#define GL_STENCIL_BACK_FAIL                             0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL                  0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS                  0x8803
#define GL_MAX_DRAW_BUFFERS                              0x8824
#define GL_DRAW_BUFFER0                                  0x8825
#define GL_DRAW_BUFFER1                                  0x8826
#define GL_DRAW_BUFFER2                                  0x8827
#define GL_DRAW_BUFFER3                                  0x8828
#define GL_DRAW_BUFFER4                                  0x8829
#define GL_DRAW_BUFFER5                                  0x882A
#define GL_DRAW_BUFFER6                                  0x882B
#define GL_DRAW_BUFFER7                                  0x882C
#define GL_DRAW_BUFFER8                                  0x882D
#define GL_DRAW_BUFFER9                                  0x882E
#define GL_DRAW_BUFFER10                                 0x882F
#define GL_DRAW_BUFFER11                                 0x8830
#define GL_DRAW_BUFFER12                                 0x8831
#define GL_DRAW_BUFFER13                                 0x8832
#define GL_DRAW_BUFFER14                                 0x8833
#define GL_DRAW_BUFFER15                                 0x8834
#define GL_BLEND_EQUATION_ALPHA                          0x883D
#define GL_MAX_VERTEX_ATTRIBS                            0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED                0x886A
#define GL_MAX_TEXTURE_IMAGE_UNITS                       0x8872
#define GL_FRAGMENT_SHADER                               0x8B30
#define GL_VERTEX_SHADER                                 0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS               0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS                 0x8B4A
#define GL_MAX_VARYING_FLOATS                            0x8B4B
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS                0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS              0x8B4D
#define GL_SHADER_TYPE                                   0x8B4F
#define GL_FLOAT_VEC2                                    0x8B50
#define GL_FLOAT_VEC3                                    0x8B51
#define GL_FLOAT_VEC4                                    0x8B52
#define GL_INT_VEC2                                      0x8B53
#define GL_INT_VEC3                                      0x8B54
#define GL_INT_VEC4                                      0x8B55
#define GL_BOOL                                          0x8B56
#define GL_BOOL_VEC2                                     0x8B57
#define GL_BOOL_VEC3                                     0x8B58
#define GL_BOOL_VEC4                                     0x8B59
#define GL_FLOAT_MAT2                                    0x8B5A
#define GL_FLOAT_MAT3                                    0x8B5B
#define GL_FLOAT_MAT4                                    0x8B5C
#define GL_SAMPLER_1D                                    0x8B5D
#define GL_SAMPLER_2D                                    0x8B5E
#define GL_SAMPLER_3D                                    0x8B5F
#define GL_SAMPLER_CUBE                                  0x8B60
#define GL_SAMPLER_1D_SHADOW                             0x8B61
#define GL_SAMPLER_2D_SHADOW                             0x8B62
#define GL_DELETE_STATUS                                 0x8B80
#define GL_COMPILE_STATUS                                0x8B81
#define GL_LINK_STATUS                                   0x8B82
#define GL_VALIDATE_STATUS                               0x8B83
#define GL_INFO_LOG_LENGTH                               0x8B84
#define GL_ATTACHED_SHADERS                              0x8B85
#define GL_ACTIVE_UNIFORMS                               0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH                     0x8B87
#define GL_SHADER_SOURCE_LENGTH                          0x8B88
#define GL_ACTIVE_ATTRIBUTES                             0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH                   0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT               0x8B8B
#define GL_SHADING_LANGUAGE_VERSION                      0x8B8C
#define GL_CURRENT_PROGRAM                               0x8B8D
#define GL_POINT_SPRITE_COORD_ORIGIN                     0x8CA0
#define GL_LOWER_LEFT                                    0x8CA1
#define GL_UPPER_LEFT                                    0x8CA2
#define GL_STENCIL_BACK_REF                              0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK                       0x8CA4
#define GL_STENCIL_BACK_WRITEMASK                        0x8CA5
#define GL_PIXEL_PACK_BUFFER                             0x88EB
#define GL_PIXEL_UNPACK_BUFFER                           0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING                     0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING                   0x88EF
#define GL_FLOAT_MAT2x3                                  0x8B65
#define GL_FLOAT_MAT2x4                                  0x8B66
#define GL_FLOAT_MAT3x2                                  0x8B67
#define GL_FLOAT_MAT3x4                                  0x8B68
#define GL_FLOAT_MAT4x2                                  0x8B69
#define GL_FLOAT_MAT4x3                                  0x8B6A
#define GL_SRGB                                          0x8C40
#define GL_SRGB8                                         0x8C41
#define GL_SRGB_ALPHA                                    0x8C42
#define GL_SRGB8_ALPHA8                                  0x8C43
#define GL_COMPRESSED_SRGB                               0x8C48
#define GL_COMPRESSED_SRGB_ALPHA                         0x8C49
#define GL_COMPARE_REF_TO_TEXTURE                        0x884E
#define GL_CLIP_DISTANCE0                                0x3000
#define GL_CLIP_DISTANCE1                                0x3001
#define GL_CLIP_DISTANCE2                                0x3002
#define GL_CLIP_DISTANCE3                                0x3003
#define GL_CLIP_DISTANCE4                                0x3004
#define GL_CLIP_DISTANCE5                                0x3005
#define GL_CLIP_DISTANCE6                                0x3006
#define GL_CLIP_DISTANCE7                                0x3007
#define GL_MAX_CLIP_DISTANCES                            0x0D32
#define GL_MAJOR_VERSION                                 0x821B
#define GL_MINOR_VERSION                                 0x821C
#define GL_NUM_EXTENSIONS                                0x821D
#define GL_CONTEXT_FLAGS                                 0x821E
#define GL_COMPRESSED_RED                                0x8225
#define GL_COMPRESSED_RG                                 0x8226
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT           0x00000001
#define GL_RGBA32F                                       0x8814
#define GL_RGB32F                                        0x8815
#define GL_RGBA16F                                       0x881A
#define GL_RGB16F                                        0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER                   0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS                      0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET                      0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET                      0x8905
#define GL_CLAMP_READ_COLOR                              0x891C
#define GL_FIXED_ONLY                                    0x891D
#define GL_MAX_VARYING_COMPONENTS                        0x8B4B
#define GL_TEXTURE_1D_ARRAY                              0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY                        0x8C19
#define GL_TEXTURE_2D_ARRAY                              0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY                        0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY                      0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY                      0x8C1D
#define GL_R11F_G11F_B10F                                0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV                  0x8C3B
#define GL_RGB9_E5                                       0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV                      0x8C3E
#define GL_TEXTURE_SHARED_SIZE                           0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH         0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE                0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS    0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS                   0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START               0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE                0x8C85
#define GL_PRIMITIVES_GENERATED                          0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN         0x8C88
#define GL_RASTERIZER_DISCARD                            0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS       0x8C8B
#define GL_INTERLEAVED_ATTRIBS                           0x8C8C
#define GL_SEPARATE_ATTRIBS                              0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER                     0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING             0x8C8F
#define GL_RGBA32UI                                      0x8D70
#define GL_RGB32UI                                       0x8D71
#define GL_RGBA16UI                                      0x8D76
#define GL_RGB16UI                                       0x8D77
#define GL_RGBA8UI                                       0x8D7C
#define GL_RGB8UI                                        0x8D7D
#define GL_RGBA32I                                       0x8D82
#define GL_RGB32I                                        0x8D83
#define GL_RGBA16I                                       0x8D88
#define GL_RGB16I                                        0x8D89
#define GL_RGBA8I                                        0x8D8E
#define GL_RGB8I                                         0x8D8F
#define GL_RED_INTEGER                                   0x8D94
#define GL_GREEN_INTEGER                                 0x8D95
#define GL_BLUE_INTEGER                                  0x8D96
#define GL_RGB_INTEGER                                   0x8D98
#define GL_RGBA_INTEGER                                  0x8D99
#define GL_BGR_INTEGER                                   0x8D9A
#define GL_BGRA_INTEGER                                  0x8D9B
#define GL_SAMPLER_1D_ARRAY                              0x8DC0
#define GL_SAMPLER_2D_ARRAY                              0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW                       0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW                       0x8DC4
#define GL_SAMPLER_CUBE_SHADOW                           0x8DC5
#define GL_UNSIGNED_INT_VEC2                             0x8DC6
#define GL_UNSIGNED_INT_VEC3                             0x8DC7
#define GL_UNSIGNED_INT_VEC4                             0x8DC8
#define GL_INT_SAMPLER_1D                                0x8DC9
#define GL_INT_SAMPLER_2D                                0x8DCA
#define GL_INT_SAMPLER_3D                                0x8DCB
#define GL_INT_SAMPLER_CUBE                              0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY                          0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY                          0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D                       0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D                       0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D                       0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE                     0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY                 0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY                 0x8DD7
#define GL_QUERY_WAIT                                    0x8E13
#define GL_QUERY_NO_WAIT                                 0x8E14
#define GL_QUERY_BY_REGION_WAIT                          0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT                       0x8E16
#define GL_BUFFER_ACCESS_FLAGS                           0x911F
#define GL_BUFFER_MAP_LENGTH                             0x9120
#define GL_BUFFER_MAP_OFFSET                             0x9121
#define GL_DEPTH_COMPONENT32F                            0x8CAC
#define GL_DEPTH32F_STENCIL8                             0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV                0x8DAD
#define GL_INVALID_FRAMEBUFFER_OPERATION                 0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING         0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE         0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE               0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE             0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE              0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE             0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE             0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE           0x8217
#define GL_FRAMEBUFFER_DEFAULT                           0x8218
#define GL_FRAMEBUFFER_UNDEFINED                         0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT                      0x821A
#define GL_MAX_RENDERBUFFER_SIZE                         0x84E8
#define GL_DEPTH_STENCIL                                 0x84F9
#define GL_UNSIGNED_INT_24_8                             0x84FA
#define GL_DEPTH24_STENCIL8                              0x88F0
#define GL_TEXTURE_STENCIL_SIZE                          0x88F1
#define GL_TEXTURE_RED_TYPE                              0x8C10
#define GL_TEXTURE_GREEN_TYPE                            0x8C11
#define GL_TEXTURE_BLUE_TYPE                             0x8C12
#define GL_TEXTURE_ALPHA_TYPE                            0x8C13
#define GL_TEXTURE_DEPTH_TYPE                            0x8C16
#define GL_UNSIGNED_NORMALIZED                           0x8C17
#define GL_FRAMEBUFFER_BINDING                           0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING                      0x8CA6
#define GL_RENDERBUFFER_BINDING                          0x8CA7
#define GL_READ_FRAMEBUFFER                              0x8CA8
#define GL_DRAW_FRAMEBUFFER                              0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING                      0x8CAA
#define GL_RENDERBUFFER_SAMPLES                          0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE            0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME            0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL          0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE  0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER          0x8CD4
#define GL_FRAMEBUFFER_COMPLETE                          0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT             0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT     0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER            0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER            0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED                       0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS                         0x8CDF
#define GL_COLOR_ATTACHMENT0                             0x8CE0
#define GL_COLOR_ATTACHMENT1                             0x8CE1
#define GL_COLOR_ATTACHMENT2                             0x8CE2
#define GL_COLOR_ATTACHMENT3                             0x8CE3
#define GL_COLOR_ATTACHMENT4                             0x8CE4
#define GL_COLOR_ATTACHMENT5                             0x8CE5
#define GL_COLOR_ATTACHMENT6                             0x8CE6
#define GL_COLOR_ATTACHMENT7                             0x8CE7
#define GL_COLOR_ATTACHMENT8                             0x8CE8
#define GL_COLOR_ATTACHMENT9                             0x8CE9
#define GL_COLOR_ATTACHMENT10                            0x8CEA
#define GL_COLOR_ATTACHMENT11                            0x8CEB
#define GL_COLOR_ATTACHMENT12                            0x8CEC
#define GL_COLOR_ATTACHMENT13                            0x8CED
#define GL_COLOR_ATTACHMENT14                            0x8CEE
#define GL_COLOR_ATTACHMENT15                            0x8CEF
#define GL_COLOR_ATTACHMENT16                            0x8CF0
#define GL_COLOR_ATTACHMENT17                            0x8CF1
#define GL_COLOR_ATTACHMENT18                            0x8CF2
#define GL_COLOR_ATTACHMENT19                            0x8CF3
#define GL_COLOR_ATTACHMENT20                            0x8CF4
#define GL_COLOR_ATTACHMENT21                            0x8CF5
#define GL_COLOR_ATTACHMENT22                            0x8CF6
#define GL_COLOR_ATTACHMENT23                            0x8CF7
#define GL_COLOR_ATTACHMENT24                            0x8CF8
#define GL_COLOR_ATTACHMENT25                            0x8CF9
#define GL_COLOR_ATTACHMENT26                            0x8CFA
#define GL_COLOR_ATTACHMENT27                            0x8CFB
#define GL_COLOR_ATTACHMENT28                            0x8CFC
#define GL_COLOR_ATTACHMENT29                            0x8CFD
#define GL_COLOR_ATTACHMENT30                            0x8CFE
#define GL_COLOR_ATTACHMENT31                            0x8CFF
#define GL_DEPTH_ATTACHMENT                              0x8D00
#define GL_STENCIL_ATTACHMENT                            0x8D20
#define GL_FRAMEBUFFER                                   0x8D40
#define GL_RENDERBUFFER                                  0x8D41
#define GL_RENDERBUFFER_WIDTH                            0x8D42
#define GL_RENDERBUFFER_HEIGHT                           0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT                  0x8D44
#define GL_STENCIL_INDEX1                                0x8D46
#define GL_STENCIL_INDEX4                                0x8D47
#define GL_STENCIL_INDEX8                                0x8D48
#define GL_STENCIL_INDEX16                               0x8D49
#define GL_RENDERBUFFER_RED_SIZE                         0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE                       0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE                        0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE                       0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE                       0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE                     0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE            0x8D56
#define GL_MAX_SAMPLES                                   0x8D57
#define GL_FRAMEBUFFER_SRGB                              0x8DB9
#define GL_HALF_FLOAT                                    0x140B
#define GL_MAP_READ_BIT                                  0x0001
#define GL_MAP_WRITE_BIT                                 0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT                      0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT                     0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT                        0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT                        0x0020
#define GL_COMPRESSED_RED_RGTC1                          0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1                   0x8DBC
#define GL_COMPRESSED_RG_RGTC2                           0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2                    0x8DBE
#define GL_RG                                            0x8227
#define GL_RG_INTEGER                                    0x8228
#define GL_R8                                            0x8229
#define GL_R16                                           0x822A
#define GL_RG8                                           0x822B
#define GL_RG16                                          0x822C
#define GL_R16F                                          0x822D
#define GL_R32F                                          0x822E
#define GL_RG16F                                         0x822F
#define GL_RG32F                                         0x8230
#define GL_R8I                                           0x8231
#define GL_R8UI                                          0x8232
#define GL_R16I                                          0x8233
#define GL_R16UI                                         0x8234
#define GL_R32I                                          0x8235
#define GL_R32UI                                         0x8236
#define GL_RG8I                                          0x8237
#define GL_RG8UI                                         0x8238
#define GL_RG16I                                         0x8239
#define GL_RG16UI                                        0x823A
#define GL_RG32I                                         0x823B
#define GL_RG32UI                                        0x823C
#define GL_VERTEX_ARRAY_BINDING                          0x85B5
#define GL_SAMPLER_2D_RECT                               0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW                        0x8B64
#define GL_SAMPLER_BUFFER                                0x8DC2
#define GL_INT_SAMPLER_2D_RECT                           0x8DCD
#define GL_INT_SAMPLER_BUFFER                            0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT                  0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER                   0x8DD8
#define GL_TEXTURE_BUFFER                                0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE                       0x8C2B
#define GL_TEXTURE_BINDING_BUFFER                        0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING             0x8C2D
#define GL_TEXTURE_RECTANGLE                             0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE                     0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE                       0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE                    0x84F8
#define GL_R8_SNORM                                      0x8F94
#define GL_RG8_SNORM                                     0x8F95
#define GL_RGB8_SNORM                                    0x8F96
#define GL_RGBA8_SNORM                                   0x8F97
#define GL_R16_SNORM                                     0x8F98
#define GL_RG16_SNORM                                    0x8F99
#define GL_RGB16_SNORM                                   0x8F9A
#define GL_RGBA16_SNORM                                  0x8F9B
#define GL_SIGNED_NORMALIZED                             0x8F9C
#define GL_PRIMITIVE_RESTART                             0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX                       0x8F9E
#define GL_COPY_READ_BUFFER                              0x8F36
#define GL_COPY_WRITE_BUFFER                             0x8F37
#define GL_UNIFORM_BUFFER                                0x8A11
#define GL_UNIFORM_BUFFER_BINDING                        0x8A28
#define GL_UNIFORM_BUFFER_START                          0x8A29
#define GL_UNIFORM_BUFFER_SIZE                           0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS                     0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS                   0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS                   0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS                   0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS                   0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE                        0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS        0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS      0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS      0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT               0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH          0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS                         0x8A36
#define GL_UNIFORM_TYPE                                  0x8A37
#define GL_UNIFORM_SIZE                                  0x8A38
#define GL_UNIFORM_NAME_LENGTH                           0x8A39
#define GL_UNIFORM_BLOCK_INDEX                           0x8A3A
#define GL_UNIFORM_OFFSET                                0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE                          0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE                         0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR                          0x8A3E
#define GL_UNIFORM_BLOCK_BINDING                         0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE                       0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH                     0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS                 0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES          0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER     0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER   0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER   0x8A46
#define GL_INVALID_INDEX                                 0xFFFFFFFFu
#define GL_CONTEXT_CORE_PROFILE_BIT                      0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT             0x00000002
#define GL_LINES_ADJACENCY                               0x000A
#define GL_LINE_STRIP_ADJACENCY                          0x000B
#define GL_TRIANGLES_ADJACENCY                           0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY                      0x000D
#define GL_PROGRAM_POINT_SIZE                            0x8642
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS              0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED                0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS          0x8DA8
#define GL_GEOMETRY_SHADER                               0x8DD9
#define GL_GEOMETRY_VERTICES_OUT                         0x8916
#define GL_GEOMETRY_INPUT_TYPE                           0x8917
#define GL_GEOMETRY_OUTPUT_TYPE                          0x8918
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS               0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES                  0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS          0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS                  0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS                 0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS                0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS                 0x9125
#define GL_CONTEXT_PROFILE_MASK                          0x9126
#define GL_DEPTH_CLAMP                                   0x864F
#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION      0x8E4C
#define GL_FIRST_VERTEX_CONVENTION                       0x8E4D
#define GL_LAST_VERTEX_CONVENTION                        0x8E4E
#define GL_PROVOKING_VERTEX                              0x8E4F
#define GL_TEXTURE_CUBE_MAP_SEAMLESS                     0x884F
#define GL_MAX_SERVER_WAIT_TIMEOUT                       0x9111
#define GL_OBJECT_TYPE                                   0x9112
#define GL_SYNC_CONDITION                                0x9113
#define GL_SYNC_STATUS                                   0x9114
#define GL_SYNC_FLAGS                                    0x9115
#define GL_SYNC_FENCE                                    0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE                    0x9117
#define GL_UNSIGNALED                                    0x9118
#define GL_SIGNALED                                      0x9119
#define GL_ALREADY_SIGNALED                              0x911A
#define GL_TIMEOUT_EXPIRED                               0x911B
#define GL_CONDITION_SATISFIED                           0x911C
#define GL_WAIT_FAILED                                   0x911D
#define GL_TIMEOUT_IGNORED                               0xFFFFFFFFFFFFFFFFull
#define GL_SYNC_FLUSH_COMMANDS_BIT                       0x00000001
#define GL_SAMPLE_POSITION                               0x8E50
#define GL_SAMPLE_MASK                                   0x8E51
#define GL_SAMPLE_MASK_VALUE                             0x8E52
#define GL_MAX_SAMPLE_MASK_WORDS                         0x8E59
#define GL_TEXTURE_2D_MULTISAMPLE                        0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE                  0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY                  0x9102
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY            0x9103
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE                0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY          0x9105
#define GL_TEXTURE_SAMPLES                               0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS                0x9107
#define GL_SAMPLER_2D_MULTISAMPLE                        0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE                    0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE           0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY                  0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY              0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY     0x910D
#define GL_MAX_COLOR_TEXTURE_SAMPLES                     0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES                     0x910F
#define GL_MAX_INTEGER_SAMPLES                           0x9110
#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR                   0x88FE
#define GL_SRC1_COLOR                                    0x88F9
#define GL_ONE_MINUS_SRC1_COLOR                          0x88FA
#define GL_ONE_MINUS_SRC1_ALPHA                          0x88FB
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS                  0x88FC
#define GL_ANY_SAMPLES_PASSED                            0x8C2F
#define GL_SAMPLER_BINDING                               0x8919
#define GL_RGB10_A2UI                                    0x906F
#define GL_TEXTURE_SWIZZLE_R                             0x8E42
#define GL_TEXTURE_SWIZZLE_G                             0x8E43
#define GL_TEXTURE_SWIZZLE_B                             0x8E44
#define GL_TEXTURE_SWIZZLE_A                             0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA                          0x8E46
#define GL_TIME_ELAPSED                                  0x88BF
#define GL_TIMESTAMP                                     0x8E28
#define GL_INT_2_10_10_10_REV                            0x8D9F

/* ----GL VERSION 1.0------------------------------------------------------------------------------------------------ */
typedef void                           (*PFNGLCULLFACEPROC)               (GLenum mode);
typedef void                           (*PFNGLFRONTFACEPROC)              (GLenum mode);
typedef void                           (*PFNGLHINTPROC)                   (GLenum target, GLenum mode);
typedef void                           (*PFNGLLINEWIDTHPROC)              (GLfloat width);
typedef void                           (*PFNGLPOINTSIZEPROC)              (GLfloat size);
typedef void                           (*PFNGLPOLYGONMODEPROC)            (GLenum face, GLenum mode);
typedef void                           (*PFNGLSCISSORPROC)                (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void                           (*PFNGLTEXPARAMETERFPROC)          (GLenum target, GLenum pname, GLfloat param);
typedef void                           (*PFNGLTEXPARAMETERFVPROC)         (GLenum target, GLenum pname, const GLfloat *params);
typedef void                           (*PFNGLTEXPARAMETERIPROC)          (GLenum target, GLenum pname, GLint param);
typedef void                           (*PFNGLTEXPARAMETERIVPROC)         (GLenum target, GLenum pname, const GLint *params);
typedef void                           (*PFNGLTEXIMAGE1DPROC)             (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void                           (*PFNGLTEXIMAGE2DPROC)             (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void                           (*PFNGLDRAWBUFFERPROC)             (GLenum buf);
typedef void                           (*PFNGLCLEARPROC)                  (GLbitfield mask);
typedef void                           (*PFNGLCLEARCOLORPROC)             (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void                           (*PFNGLCLEARSTENCILPROC)           (GLint s);
typedef void                           (*PFNGLCLEARDEPTHPROC)             (GLdouble depth);
typedef void                           (*PFNGLSTENCILMASKPROC)            (GLuint mask);
typedef void                           (*PFNGLCOLORMASKPROC)              (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
typedef void                           (*PFNGLDEPTHMASKPROC)              (GLboolean flag);
typedef void                           (*PFNGLDISABLEPROC)                (GLenum cap);
typedef void                           (*PFNGLENABLEPROC)                 (GLenum cap);
typedef void                           (*PFNGLFINISHPROC)                 (void);
typedef void                           (*PFNGLFLUSHPROC)                  (void);
typedef void                           (*PFNGLBLENDFUNCPROC)              (GLenum sfactor, GLenum dfactor);
typedef void                           (*PFNGLLOGICOPPROC)                (GLenum opcode);
typedef void                           (*PFNGLSTENCILFUNCPROC)            (GLenum func, GLint ref, GLuint mask);
typedef void                           (*PFNGLSTENCILOPPROC)              (GLenum fail, GLenum zfail, GLenum zpass);
typedef void                           (*PFNGLDEPTHFUNCPROC)              (GLenum func);
typedef void                           (*PFNGLPIXELSTOREFPROC)            (GLenum pname, GLfloat param);
typedef void                           (*PFNGLPIXELSTOREIPROC)            (GLenum pname, GLint param);
typedef void                           (*PFNGLREADBUFFERPROC)             (GLenum src);
typedef void                           (*PFNGLREADPIXELSPROC)             (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
typedef void                           (*PFNGLGETBOOLEANVPROC)            (GLenum pname, GLboolean *data);
typedef void                           (*PFNGLGETDOUBLEVPROC)             (GLenum pname, GLdouble *data);
typedef GLenum                         (*PFNGLGETERRORPROC)               (void);
typedef void                           (*PFNGLGETFLOATVPROC)              (GLenum pname, GLfloat *data);
typedef void                           (*PFNGLGETINTEGERVPROC)            (GLenum pname, GLint *data);
typedef const GLubyte *                (*PFNGLGETSTRINGPROC)              (GLenum name);
typedef void                           (*PFNGLGETTEXIMAGEPROC)            (GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
typedef void                           (*PFNGLGETTEXPARAMETERFVPROC)      (GLenum target, GLenum pname, GLfloat *params);
typedef void                           (*PFNGLGETTEXPARAMETERIVPROC)      (GLenum target, GLenum pname, GLint *params);
typedef void                           (*PFNGLGETTEXLEVELPARAMETERFVPROC) (GLenum target, GLint level, GLenum pname, GLfloat *params);
typedef void                           (*PFNGLGETTEXLEVELPARAMETERIVPROC) (GLenum target, GLint level, GLenum pname, GLint *params);
typedef GLboolean                      (*PFNGLISENABLEDPROC)              (GLenum cap);
typedef void                           (*PFNGLDEPTHRANGEPROC)             (GLdouble n, GLdouble f);
typedef void                           (*PFNGLVIEWPORTPROC)               (GLint x, GLint y, GLsizei width, GLsizei height);
extern void glCullFace                 (GLenum mode);
extern void glFrontFace                (GLenum mode);
extern void glHint                     (GLenum target, GLenum mode);
extern void glLineWidth                (GLfloat width);
extern void glPointSize                (GLfloat size);
extern void glPolygonMode              (GLenum face, GLenum mode);
extern void glScissor                  (GLint x, GLint y, GLsizei width, GLsizei height);
extern void glTexParameterf            (GLenum target, GLenum pname, GLfloat param);
extern void glTexParameterfv           (GLenum target, GLenum pname, const GLfloat *params);
extern void glTexParameteri            (GLenum target, GLenum pname, GLint param);
extern void glTexParameteriv           (GLenum target, GLenum pname, const GLint *params);
extern void glTexImage1D               (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
extern void glTexImage2D               (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
extern void glDrawBuffer               (GLenum buf);
extern void glClear                    (GLbitfield mask);
extern void glClearColor               (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void glClearStencil             (GLint s);
extern void glStencilMask              (GLuint mask);
extern void glColorMask                (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern void glDepthMask                (GLboolean flag);
extern void glDisable                  (GLenum cap);
extern void glEnable                   (GLenum cap);
extern void glFinish                   (void);
extern void glFlush                    (void);
extern void glBlendFunc                (GLenum sfactor, GLenum dfactor);
extern void glLogicOp                  (GLenum opcode);
extern void glStencilFunc              (GLenum func, GLint ref, GLuint mask);
extern void glStencilOp                (GLenum fail, GLenum zfail, GLenum zpass);
extern void glDepthFunc                (GLenum func);
extern void glPixelStoref              (GLenum pname, GLfloat param);
extern void glPixelStorei              (GLenum pname, GLint param);
extern void glReadBuffer               (GLenum src);
extern void glReadPixels               (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, void *pixels);
extern void glGetBooleanv              (GLenum pname, GLboolean *data);
extern void glGetDoublev               (GLenum pname, GLdouble *data);
extern GLenum glGetError               (void);
extern void glGetFloatv                (GLenum pname, GLfloat *data);
extern void glGetIntegerv              (GLenum pname, GLint *data);
extern const GLubyte *glGetString      (GLenum name);
extern void glGetTexImage              (GLenum target, GLint level, GLenum format, GLenum type, void *pixels);
extern void glGetTexParameterfv        (GLenum target, GLenum pname, GLfloat *params);
extern void glGetTexParameteriv        (GLenum target, GLenum pname, GLint *params);
extern void glGetTexLevelParameterfv   (GLenum target, GLint level, GLenum pname, GLfloat *params);
extern void glGetTexLevelParameteriv   (GLenum target, GLint level, GLenum pname, GLint *params);
extern GLboolean glIsEnabled           (GLenum cap);
extern void glDepthRange               (GLdouble n, GLdouble f);
extern void glViewport                 (GLint x, GLint y, GLsizei width, GLsizei height);
extern void glAccum                    (GLenum op, GLfloat value);
extern void glAlphaFunc                (GLenum func, GLclampf ref);
extern GLboolean glAreTexturesResident (GLsizei n, const GLuint *textures, GLboolean *residences);
extern void glArrayElement             (GLint i);
extern void glBegin                    (GLenum mode);
extern void glBindTexture              (GLenum target, GLuint texture);
extern void glBitmap                   (GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap);
extern void glBlendFunc                (GLenum sfactor, GLenum dfactor);
extern void glCallList                 (GLuint list);
extern void glCallLists                (GLsizei n, GLenum type, const GLvoid *lists);
extern void glClear                    (GLbitfield mask);
extern void glClearAccum               (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void glClearColor               (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
extern void glClearDepth               (GLclampd depth);
extern void glClearIndex               (GLfloat c);
extern void glClearStencil             (GLint s);
extern void glClipPlane                (GLenum plane, const GLdouble *equation);
extern void glColor3b                  (GLbyte red, GLbyte green, GLbyte blue);
extern void glColor3bv                 (const GLbyte *v);
extern void glColor3d                  (GLdouble red, GLdouble green, GLdouble blue);
extern void glColor3dv                 (const GLdouble *v);
extern void glColor3f                  (GLfloat red, GLfloat green, GLfloat blue);
extern void glColor3fv                 (const GLfloat *v);
extern void glColor3i                  (GLint red, GLint green, GLint blue);
extern void glColor3iv                 (const GLint *v);
extern void glColor3s                  (GLshort red, GLshort green, GLshort blue);
extern void glColor3sv                 (const GLshort *v);
extern void glColor3ub                 (GLubyte red, GLubyte green, GLubyte blue);
extern void glColor3ubv                (const GLubyte *v);
extern void glColor3ui                 (GLuint red, GLuint green, GLuint blue);
extern void glColor3uiv                (const GLuint *v);
extern void glColor3us                 (GLushort red, GLushort green, GLushort blue);
extern void glColor3usv                (const GLushort *v);
extern void glColor4b                  (GLbyte red, GLbyte green, GLbyte blue, GLbyte alpha);
extern void glColor4bv                 (const GLbyte *v);
extern void glColor4d                  (GLdouble red, GLdouble green, GLdouble blue, GLdouble alpha);
extern void glColor4dv                 (const GLdouble *v);
extern void glColor4f                  (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void glColor4fv                 (const GLfloat *v);
extern void glColor4i                  (GLint red, GLint green, GLint blue, GLint alpha);
extern void glColor4iv                 (const GLint *v);
extern void glColor4s                  (GLshort red, GLshort green, GLshort blue, GLshort alpha);
extern void glColor4sv                 (const GLshort *v);
extern void glColor4ub                 (GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha);
extern void glColor4ubv                (const GLubyte *v);
extern void glColor4ui                 (GLuint red, GLuint green, GLuint blue, GLuint alpha);
extern void glColor4uiv                (const GLuint *v);
extern void glColor4us                 (GLushort red, GLushort green, GLushort blue, GLushort alpha);
extern void glColor4usv                (const GLushort *v);
extern void glColorMask                (GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
extern void glColorMaterial            (GLenum face, GLenum mode);
extern void glColorPointer             (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glCopyPixels               (GLint x, GLint y, GLsizei width, GLsizei height, GLenum type);
extern void glCopyTexImage1D           (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLint border);
extern void glCopyTexImage2D           (GLenum target, GLint level, GLenum internalFormat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void glCopyTexSubImage1D        (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern void glCopyTexSubImage2D        (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void glCullFace                 (GLenum mode);
extern void glDeleteLists              (GLuint list, GLsizei range);
extern void glDeleteTextures           (GLsizei n, const GLuint *textures);
extern void glDepthFunc                (GLenum func);
extern void glDepthMask                (GLboolean flag);
extern void glDepthRange               (GLclampd zNear, GLclampd zFar);
extern void glDisable                  (GLenum cap);
extern void glDisableClientState       (GLenum array);
extern void glDrawArrays               (GLenum mode, GLint first, GLsizei count);
extern void glDrawBuffer               (GLenum mode);
extern void glDrawElements             (GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
extern void glDrawPixels               (GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern void glEdgeFlag                 (GLboolean flag);
extern void glEdgeFlagPointer          (GLsizei stride, const GLvoid *pointer);
extern void glEdgeFlagv                (const GLboolean *flag);
extern void glEnable                   (GLenum cap);
extern void glEnableClientState        (GLenum array);
extern void glEnd                      (void);
extern void glEndList                  (void);
extern void glEvalCoord1d              (GLdouble u);
extern void glEvalCoord1dv             (const GLdouble *u);
extern void glEvalCoord1f              (GLfloat u);
extern void glEvalCoord1fv             (const GLfloat *u);
extern void glEvalCoord2d              (GLdouble u, GLdouble v);
extern void glEvalCoord2dv             (const GLdouble *u);
extern void glEvalCoord2f              (GLfloat u, GLfloat v);
extern void glEvalCoord2fv             (const GLfloat *u);
extern void glEvalMesh1                (GLenum mode, GLint i1, GLint i2);
extern void glEvalMesh2                (GLenum mode, GLint i1, GLint i2, GLint j1, GLint j2);
extern void glEvalPoint1               (GLint i);
extern void glEvalPoint2               (GLint i, GLint j);
extern void glFeedbackBuffer           (GLsizei size, GLenum type, GLfloat *buffer);
extern void glFinish                   (void);
extern void glFlush                    (void);
extern void glFogf                     (GLenum pname, GLfloat param);
extern void glFogfv                    (GLenum pname, const GLfloat *params);
extern void glFogi                     (GLenum pname, GLint param);
extern void glFogiv                    (GLenum pname, const GLint *params);
extern void glFrontFace                (GLenum mode);
extern void glFrustum                  (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern GLuint glGenLists               (GLsizei range);
extern void glGenTextures              (GLsizei n, GLuint *textures);
extern void glGetBooleanv              (GLenum pname, GLboolean *params);
extern void glGetClipPlane             (GLenum plane, GLdouble *equation);
extern void glGetDoublev               (GLenum pname, GLdouble *params);
extern GLenum glGetError               (void);
extern void glGetFloatv                (GLenum pname, GLfloat *params);
extern void glGetIntegerv              (GLenum pname, GLint *params);
extern void glGetLightfv               (GLenum light, GLenum pname, GLfloat *params);
extern void glGetLightiv               (GLenum light, GLenum pname, GLint *params);
extern void glGetMapdv                 (GLenum target, GLenum query, GLdouble *v);
extern void glGetMapfv                 (GLenum target, GLenum query, GLfloat *v);
extern void glGetMapiv                 (GLenum target, GLenum query, GLint *v);
extern void glGetMaterialfv            (GLenum face, GLenum pname, GLfloat *params);
extern void glGetMaterialiv            (GLenum face, GLenum pname, GLint *params);
extern void glGetPixelMapfv            (GLenum map, GLfloat *values);
extern void glGetPixelMapuiv           (GLenum map, GLuint *values);
extern void glGetPixelMapusv           (GLenum map, GLushort *values);
extern void glGetPointerv              (GLenum pname, GLvoid* *params);
extern void glGetPolygonStipple        (GLubyte *mask);
extern const GLubyte * glGetString     (GLenum name);
extern void glGetTexEnvfv              (GLenum target, GLenum pname, GLfloat *params);
extern void glGetTexEnviv              (GLenum target, GLenum pname, GLint *params);
extern void glGetTexGendv              (GLenum coord, GLenum pname, GLdouble *params);
extern void glGetTexGenfv              (GLenum coord, GLenum pname, GLfloat *params);
extern void glGetTexGeniv              (GLenum coord, GLenum pname, GLint *params);
extern void glGetTexImage              (GLenum target, GLint level, GLenum format, GLenum type, GLvoid *pixels);
extern void glGetTexLevelParameterfv   (GLenum target, GLint level, GLenum pname, GLfloat *params);
extern void glGetTexLevelParameteriv   (GLenum target, GLint level, GLenum pname, GLint *params);
extern void glGetTexParameterfv        (GLenum target, GLenum pname, GLfloat *params);
extern void glGetTexParameteriv        (GLenum target, GLenum pname, GLint *params);
extern void glHint                     (GLenum target, GLenum mode);
extern void glIndexMask                (GLuint mask);
extern void glIndexPointer             (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glIndexd                   (GLdouble c);
extern void glIndexdv                  (const GLdouble *c);
extern void glIndexf                   (GLfloat c);
extern void glIndexfv                  (const GLfloat *c);
extern void glIndexi                   (GLint c);
extern void glIndexiv                  (const GLint *c);
extern void glIndexs                   (GLshort c);
extern void glIndexsv                  (const GLshort *c);
extern void glIndexub                  (GLubyte c);
extern void glIndexubv                 (const GLubyte *c);
extern void glInitNames                (void);
extern void glInterleavedArrays        (GLenum format, GLsizei stride, const GLvoid *pointer);
extern GLboolean glIsEnabled           (GLenum cap);
extern GLboolean glIsList              (GLuint list);
extern GLboolean glIsTexture           (GLuint texture);
extern void glLightModelf              (GLenum pname, GLfloat param);
extern void glLightModelfv             (GLenum pname, const GLfloat *params);
extern void glLightModeli              (GLenum pname, GLint param);
extern void glLightModeliv             (GLenum pname, const GLint *params);
extern void glLightf                   (GLenum light, GLenum pname, GLfloat param);
extern void glLightfv                  (GLenum light, GLenum pname, const GLfloat *params);
extern void glLighti                   (GLenum light, GLenum pname, GLint param);
extern void glLightiv                  (GLenum light, GLenum pname, const GLint *params);
extern void glLineStipple              (GLint factor, GLushort pattern);
extern void glLineWidth                (GLfloat width);
extern void glListBase                 (GLuint base);
extern void glLoadIdentity             (void);
extern void glLoadMatrixd              (const GLdouble *m);
extern void glLoadMatrixf              (const GLfloat *m);
extern void glLoadName                 (GLuint name);
extern void glLogicOp                  (GLenum opcode);
extern void glMap1d                    (GLenum target, GLdouble u1, GLdouble u2, GLint stride, GLint order, const GLdouble *points);
extern void glMap1f                    (GLenum target, GLfloat u1, GLfloat u2, GLint stride, GLint order, const GLfloat *points);
extern void glMap2d                    (GLenum target, GLdouble u1, GLdouble u2, GLint ustride, GLint uorder, GLdouble v1, GLdouble v2, GLint vstride, GLint vorder, const GLdouble *points);
extern void glMap2f                    (GLenum target, GLfloat u1, GLfloat u2, GLint ustride, GLint uorder, GLfloat v1, GLfloat v2, GLint vstride, GLint vorder, const GLfloat *points);
extern void glMapGrid1d                (GLint un, GLdouble u1, GLdouble u2);
extern void glMapGrid1f                (GLint un, GLfloat u1, GLfloat u2);
extern void glMapGrid2d                (GLint un, GLdouble u1, GLdouble u2, GLint vn, GLdouble v1, GLdouble v2);
extern void glMapGrid2f                (GLint un, GLfloat u1, GLfloat u2, GLint vn, GLfloat v1, GLfloat v2);
extern void glMaterialf                (GLenum face, GLenum pname, GLfloat param);
extern void glMaterialfv               (GLenum face, GLenum pname, const GLfloat *params);
extern void glMateriali                (GLenum face, GLenum pname, GLint param);
extern void glMaterialiv               (GLenum face, GLenum pname, const GLint *params);
extern void glMatrixMode               (GLenum mode);
extern void glMultMatrixd              (const GLdouble *m);
extern void glMultMatrixf              (const GLfloat *m);
extern void glNewList                  (GLuint list, GLenum mode);
extern void glNormal3b                 (GLbyte nx, GLbyte ny, GLbyte nz);
extern void glNormal3bv                (const GLbyte *v);
extern void glNormal3d                 (GLdouble nx, GLdouble ny, GLdouble nz);
extern void glNormal3dv                (const GLdouble *v);
extern void glNormal3f                 (GLfloat nx, GLfloat ny, GLfloat nz);
extern void glNormal3fv                (const GLfloat *v);
extern void glNormal3i                 (GLint nx, GLint ny, GLint nz);
extern void glNormal3iv                (const GLint *v);
extern void glNormal3s                 (GLshort nx, GLshort ny, GLshort nz);
extern void glNormal3sv                (const GLshort *v);
extern void glNormalPointer            (GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glOrtho                    (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
extern void glPassThrough              (GLfloat token);
extern void glPixelMapfv               (GLenum map, GLsizei mapsize, const GLfloat *values);
extern void glPixelMapuiv              (GLenum map, GLsizei mapsize, const GLuint *values);
extern void glPixelMapusv              (GLenum map, GLsizei mapsize, const GLushort *values);
extern void glPixelStoref              (GLenum pname, GLfloat param);
extern void glPixelStorei              (GLenum pname, GLint param);
extern void glPixelTransferf           (GLenum pname, GLfloat param);
extern void glPixelTransferi           (GLenum pname, GLint param);
extern void glPixelZoom                (GLfloat xfactor, GLfloat yfactor);
extern void glPointSize                (GLfloat size);
extern void glPolygonMode              (GLenum face, GLenum mode);
extern void glPolygonOffset            (GLfloat factor, GLfloat units);
extern void glPolygonStipple           (const GLubyte *mask);
extern void glPopAttrib                (void);
extern void glPopClientAttrib          (void);
extern void glPopMatrix                (void);
extern void glPopName                  (void);
extern void glPrioritizeTextures       (GLsizei n, const GLuint *textures, const GLclampf *priorities);
extern void glPushAttrib               (GLbitfield mask);
extern void glPushClientAttrib         (GLbitfield mask);
extern void glPushMatrix               (void);
extern void glPushName                 (GLuint name);
extern void glRasterPos2d              (GLdouble x, GLdouble y);
extern void glRasterPos2dv             (const GLdouble *v);
extern void glRasterPos2f              (GLfloat x, GLfloat y);
extern void glRasterPos2fv             (const GLfloat *v);
extern void glRasterPos2i              (GLint x, GLint y);
extern void glRasterPos2iv             (const GLint *v);
extern void glRasterPos2s              (GLshort x, GLshort y);
extern void glRasterPos2sv             (const GLshort *v);
extern void glRasterPos3d              (GLdouble x, GLdouble y, GLdouble z);
extern void glRasterPos3dv             (const GLdouble *v);
extern void glRasterPos3f              (GLfloat x, GLfloat y, GLfloat z);
extern void glRasterPos3fv             (const GLfloat *v);
extern void glRasterPos3i              (GLint x, GLint y, GLint z);
extern void glRasterPos3iv             (const GLint *v);
extern void glRasterPos3s              (GLshort x, GLshort y, GLshort z);
extern void glRasterPos3sv             (const GLshort *v);
extern void glRasterPos4d              (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glRasterPos4dv             (const GLdouble *v);
extern void glRasterPos4f              (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glRasterPos4fv             (const GLfloat *v);
extern void glRasterPos4i              (GLint x, GLint y, GLint z, GLint w);
extern void glRasterPos4iv             (const GLint *v);
extern void glRasterPos4s              (GLshort x, GLshort y, GLshort z, GLshort w);
extern void glRasterPos4sv             (const GLshort *v);
extern void glReadBuffer               (GLenum mode);
extern void glReadPixels               (GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
extern void glRectd                    (GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2);
extern void glRectdv                   (const GLdouble *v1, const GLdouble *v2);
extern void glRectf                    (GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
extern void glRectfv                   (const GLfloat *v1, const GLfloat *v2);
extern void glRecti                    (GLint x1, GLint y1, GLint x2, GLint y2);
extern void glRectiv                   (const GLint *v1, const GLint *v2);
extern void glRects                    (GLshort x1, GLshort y1, GLshort x2, GLshort y2);
extern void glRectsv                   (const GLshort *v1, const GLshort *v2);
extern GLint glRenderMode              (GLenum mode);
extern void glRotated                  (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
extern void glRotatef                  (GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern void glScaled                   (GLdouble x, GLdouble y, GLdouble z);
extern void glScalef                   (GLfloat x, GLfloat y, GLfloat z);
extern void glScissor                  (GLint x, GLint y, GLsizei width, GLsizei height);
extern void glSelectBuffer             (GLsizei size, GLuint *buffer);
extern void glShadeModel               (GLenum mode);
extern void glStencilFunc              (GLenum func, GLint ref, GLuint mask);
extern void glStencilMask              (GLuint mask);
extern void glStencilOp                (GLenum fail, GLenum zfail, GLenum zpass);
extern void glTexCoord1d               (GLdouble s);
extern void glTexCoord1dv              (const GLdouble *v);
extern void glTexCoord1f               (GLfloat s);
extern void glTexCoord1fv              (const GLfloat *v);
extern void glTexCoord1i               (GLint s);
extern void glTexCoord1iv              (const GLint *v);
extern void glTexCoord1s               (GLshort s);
extern void glTexCoord1sv              (const GLshort *v);
extern void glTexCoord2d               (GLdouble s, GLdouble t);
extern void glTexCoord2dv              (const GLdouble *v);
extern void glTexCoord2f               (GLfloat s, GLfloat t);
extern void glTexCoord2fv              (const GLfloat *v);
extern void glTexCoord2i               (GLint s, GLint t);
extern void glTexCoord2iv              (const GLint *v);
extern void glTexCoord2s               (GLshort s, GLshort t);
extern void glTexCoord2sv              (const GLshort *v);
extern void glTexCoord3d               (GLdouble s, GLdouble t, GLdouble r);
extern void glTexCoord3dv              (const GLdouble *v);
extern void glTexCoord3f               (GLfloat s, GLfloat t, GLfloat r);
extern void glTexCoord3fv              (const GLfloat *v);
extern void glTexCoord3i               (GLint s, GLint t, GLint r);
extern void glTexCoord3iv              (const GLint *v);
extern void glTexCoord3s               (GLshort s, GLshort t, GLshort r);
extern void glTexCoord3sv              (const GLshort *v);
extern void glTexCoord4d               (GLdouble s, GLdouble t, GLdouble r, GLdouble q);
extern void glTexCoord4dv              (const GLdouble *v);
extern void glTexCoord4f               (GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern void glTexCoord4fv              (const GLfloat *v);
extern void glTexCoord4i               (GLint s, GLint t, GLint r, GLint q);
extern void glTexCoord4iv              (const GLint *v);
extern void glTexCoord4s               (GLshort s, GLshort t, GLshort r, GLshort q);
extern void glTexCoord4sv              (const GLshort *v);
extern void glTexCoordPointer          (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glTexEnvf                  (GLenum target, GLenum pname, GLfloat param);
extern void glTexEnvfv                 (GLenum target, GLenum pname, const GLfloat *params);
extern void glTexEnvi                  (GLenum target, GLenum pname, GLint param);
extern void glTexEnviv                 (GLenum target, GLenum pname, const GLint *params);
extern void glTexGend                  (GLenum coord, GLenum pname, GLdouble param);
extern void glTexGendv                 (GLenum coord, GLenum pname, const GLdouble *params);
extern void glTexGenf                  (GLenum coord, GLenum pname, GLfloat param);
extern void glTexGenfv                 (GLenum coord, GLenum pname, const GLfloat *params);
extern void glTexGeni                  (GLenum coord, GLenum pname, GLint param);
extern void glTexGeniv                 (GLenum coord, GLenum pname, const GLint *params);
extern void glTexImage1D               (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTexImage2D               (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTexParameterf            (GLenum target, GLenum pname, GLfloat param);
extern void glTexParameterfv           (GLenum target, GLenum pname, const GLfloat *params);
extern void glTexParameteri            (GLenum target, GLenum pname, GLint param);
extern void glTexParameteriv           (GLenum target, GLenum pname, const GLint *params);
extern void glTexSubImage1D            (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTexSubImage2D            (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern void glTranslated               (GLdouble x, GLdouble y, GLdouble z);
extern void glTranslatef               (GLfloat x, GLfloat y, GLfloat z);
extern void glVertex2d                 (GLdouble x, GLdouble y);
extern void glVertex2dv                (const GLdouble *v);
extern void glVertex2f                 (GLfloat x, GLfloat y);
extern void glVertex2fv                (const GLfloat *v);
extern void glVertex2i                 (GLint x, GLint y);
extern void glVertex2iv                (const GLint *v);
extern void glVertex2s                 (GLshort x, GLshort y);
extern void glVertex2sv                (const GLshort *v);
extern void glVertex3d                 (GLdouble x, GLdouble y, GLdouble z);
extern void glVertex3dv                (const GLdouble *v);
extern void glVertex3f                 (GLfloat x, GLfloat y, GLfloat z);
extern void glVertex3fv                (const GLfloat *v);
extern void glVertex3i                 (GLint x, GLint y, GLint z);
extern void glVertex3iv                (const GLint *v);
extern void glVertex3s                 (GLshort x, GLshort y, GLshort z);
extern void glVertex3sv                (const GLshort *v);
extern void glVertex4d                 (GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void glVertex4dv                (const GLdouble *v);
extern void glVertex4f                 (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void glVertex4fv                (const GLfloat *v);
extern void glVertex4i                 (GLint x, GLint y, GLint z, GLint w);
extern void glVertex4iv                (const GLint *v);
extern void glVertex4s                 (GLshort x, GLshort y, GLshort z, GLshort w);
extern void glVertex4sv                (const GLshort *v);
extern void glVertexPointer            (GLint size, GLenum type, GLsizei stride, const GLvoid *pointer);
extern void glViewport                 (GLint x, GLint y, GLsizei width, GLsizei height);
/* ----GL VERSION 1.1------------------------------------------------------------------------------------------------ */
typedef void      (* PFNGLDRAWARRAYSPROC)        (GLenum mode, GLint first, GLsizei count);
typedef void      (* PFNGLDRAWELEMENTSPROC)      (GLenum mode, GLsizei count, GLenum type, const void *indices);
typedef void      (* PFNGLGETPOINTERVPROC)       (GLenum pname, void **params);
typedef void      (* PFNGLPOLYGONOFFSETPROC)     (GLfloat factor, GLfloat units);
typedef void      (* PFNGLCOPYTEXIMAGE1DPROC)    (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
typedef void      (* PFNGLCOPYTEXIMAGE2DPROC)    (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
typedef void      (* PFNGLCOPYTEXSUBIMAGE1DPROC) (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
typedef void      (* PFNGLCOPYTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
typedef void      (* PFNGLTEXSUBIMAGE1DPROC)     (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void      (* PFNGLTEXSUBIMAGE2DPROC)     (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void      (* PFNGLBINDTEXTUREPROC)       (GLenum target, GLuint texture);
typedef void      (* PFNGLDELETETEXTURESPROC)    (GLsizei n, const GLuint *textures);
typedef void      (* PFNGLGENTEXTURESPROC)       (GLsizei n, GLuint *textures);
typedef GLboolean (* PFNGLISTEXTUREPROC)         (GLuint texture);
extern void      glDrawArrays        (GLenum mode, GLint first, GLsizei count);
extern void      glDrawElements      (GLenum mode, GLsizei count, GLenum type, const void *indices);
extern void      glGetPointerv       (GLenum pname, void **params);
extern void      glPolygonOffset     (GLfloat factor, GLfloat units);
extern void      glCopyTexImage1D    (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
extern void      glCopyTexImage2D    (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void      glCopyTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern void      glCopyTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern void      glTexSubImage1D     (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
extern void      glTexSubImage2D     (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
extern void      glBindTexture       (GLenum target, GLuint texture);
extern void      glDeleteTextures    (GLsizei n, const GLuint *textures);
extern void      glGenTextures       (GLsizei n, GLuint *textures);
extern GLboolean glIsTexture         (GLuint texture);
/* ----GL VERSION 1.2------------------------------------------------------------------------------------------------ */
typedef void (* PFNGLDRAWRANGEELEMENTSPROC) (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices);
typedef void (* PFNGLTEXIMAGE3DPROC)        (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void (* PFNGLTEXSUBIMAGE3DPROC)     (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
typedef void (* PFNGLCOPYTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
extern PFNGLDRAWRANGEELEMENTSPROC glDrawRangeElements;
extern PFNGLTEXIMAGE3DPROC        glTexImage3D;
extern PFNGLTEXSUBIMAGE3DPROC     glTexSubImage3D;
extern PFNGLCOPYTEXSUBIMAGE3DPROC glCopyTexSubImage3D;
/* ----GL VERSION 1.3------------------------------------------------------------------------------------------------ */
typedef void (* PFNGLACTIVETEXTUREPROC)           (GLenum texture);
typedef void (* PFNGLSAMPLECOVERAGEPROC)          (GLfloat value, GLboolean invert);
typedef void (* PFNGLCOMPRESSEDTEXIMAGE3DPROC)    (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
typedef void (* PFNGLCOMPRESSEDTEXIMAGE2DPROC)    (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
typedef void (* PFNGLCOMPRESSEDTEXIMAGE1DPROC)    (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
typedef void (* PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLsizei imageSize, const void *data);
typedef void (* PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC) (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLsizei imageSize, const void *data);
typedef void (* PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC) (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLsizei imageSize, const void *data);
typedef void (* PFNGLGETCOMPRESSEDTEXIMAGEPROC)   (GLenum target, GLint level, void *img);
extern PFNGLACTIVETEXTUREPROC           glActiveTexture;
extern PFNGLSAMPLECOVERAGEPROC          glSampleCoverage;
extern PFNGLCOMPRESSEDTEXIMAGE3DPROC    glCompressedTexImage3D;
extern PFNGLCOMPRESSEDTEXIMAGE2DPROC    glCompressedTexImage2D;
extern PFNGLCOMPRESSEDTEXIMAGE1DPROC    glCompressedTexImage1D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
extern PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
extern PFNGLGETCOMPRESSEDTEXIMAGEPROC   glGetCompressedTexImage;
/* ----GL VERSION 1.4------------------------------------------------------------------------------------------------ */
typedef void (*PFNGLBLENDFUNCSEPARATEPROC) (GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
typedef void (*PFNGLMULTIDRAWARRAYSPROC) (GLenum mode, const GLint *first, const GLsizei *count, GLsizei drawcount);
typedef void (*PFNGLMULTIDRAWELEMENTSPROC) (GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount);
typedef void (*PFNGLPOINTPARAMETERFPROC) (GLenum pname, GLfloat param);
typedef void (*PFNGLPOINTPARAMETERFVPROC) (GLenum pname, const GLfloat *params);
typedef void (*PFNGLPOINTPARAMETERIPROC) (GLenum pname, GLint param);
typedef void (*PFNGLPOINTPARAMETERIVPROC) (GLenum pname, const GLint *params);
typedef void (*PFNGLBLENDCOLORPROC) (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (*PFNGLBLENDEQUATIONPROC) (GLenum mode);
extern PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
extern PFNGLMULTIDRAWARRAYSPROC   glMultiDrawArrays;
extern PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
extern PFNGLPOINTPARAMETERFPROC   glPointParameterf;
extern PFNGLPOINTPARAMETERFVPROC  glPointParameterfv;
extern PFNGLPOINTPARAMETERIPROC   glPointParameteri;
extern PFNGLPOINTPARAMETERIVPROC  glPointParameteriv;
extern PFNGLBLENDCOLORPROC        glBlendColor;
extern PFNGLBLENDEQUATIONPROC     glBlendEquation;
/* ----GL VERSION 1.5------------------------------------------------------------------------------------------------ */
typedef void      (*PFNGLGENQUERIESPROC)           (GLsizei n, GLuint *ids);
typedef void      (*PFNGLDELETEQUERIESPROC)        (GLsizei n, const GLuint *ids);
typedef GLboolean (*PFNGLISQUERYPROC)              (GLuint id);
typedef void      (*PFNGLBEGINQUERYPROC)           (GLenum target, GLuint id);
typedef void      (*PFNGLENDQUERYPROC)             (GLenum target);
typedef void      (*PFNGLGETQUERYIVPROC)           (GLenum target, GLenum pname, GLint *params);
typedef void      (*PFNGLGETQUERYOBJECTIVPROC)     (GLuint id, GLenum pname, GLint *params);
typedef void      (*PFNGLGETQUERYOBJECTUIVPROC)    (GLuint id, GLenum pname, GLuint *params);
typedef void      (*PFNGLBINDBUFFERPROC)           (GLenum target, GLuint buffer);
typedef void      (*PFNGLDELETEBUFFERSPROC)        (GLsizei n, const GLuint *buffers);
typedef void      (*PFNGLGENBUFFERSPROC)           (GLsizei n, GLuint *buffers);
typedef GLboolean (*PFNGLISBUFFERPROC)             (GLuint buffer);
typedef void      (*PFNGLBUFFERDATAPROC)           (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void      (*PFNGLBUFFERSUBDATAPROC)        (GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
typedef void      (*PFNGLGETBUFFERSUBDATAPROC)     (GLenum target, GLintptr offset, GLsizeiptr size, void *data);
typedef void *    (*PFNGLMAPBUFFERPROC)            (GLenum target, GLenum access);
typedef GLboolean (*PFNGLUNMAPBUFFERPROC)          (GLenum target);
typedef void      (*PFNGLGETBUFFERPARAMETERIVPROC) (GLenum target, GLenum pname, GLint *params);
typedef void      (*PFNGLGETBUFFERPOINTERVPROC)    (GLenum target, GLenum pname, void **params);
extern PFNGLGENQUERIESPROC           glGenQueries;
extern PFNGLDELETEQUERIESPROC        glDeleteQueries;
extern PFNGLISQUERYPROC              glIsQuery;
extern PFNGLBEGINQUERYPROC           glBeginQuery;
extern PFNGLENDQUERYPROC             glEndQuery;
extern PFNGLGETQUERYIVPROC           glGetQueryiv;
extern PFNGLGETQUERYOBJECTIVPROC     glGetQueryObjectiv;
extern PFNGLGETQUERYOBJECTUIVPROC    glGetQueryObjectuiv;
extern PFNGLBINDBUFFERPROC           glBindBuffer;
extern PFNGLDELETEBUFFERSPROC        glDeleteBuffers;
extern PFNGLGENBUFFERSPROC           glGenBuffers;
extern PFNGLISBUFFERPROC             glIsBuffer;
extern PFNGLBUFFERDATAPROC           glBufferData;
extern PFNGLBUFFERSUBDATAPROC        glBufferSubData;
extern PFNGLGETBUFFERSUBDATAPROC     glGetBufferSubData;
extern PFNGLMAPBUFFERPROC            glMapBuffer;
extern PFNGLUNMAPBUFFERPROC          glUnmapBuffer;
extern PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
extern PFNGLGETBUFFERPOINTERVPROC    glGetBufferPointerv;
/* ----GL VERSION 2.0------------------------------------------------------------------------------------------------ */
typedef void (*PFNGLBLENDEQUATIONSEPARATEPROC)    (GLenum modeRGB, GLenum modeAlpha);
typedef void (*PFNGLDRAWBUFFERSPROC)              (GLsizei n, const GLenum *bufs);
typedef void (*PFNGLSTENCILOPSEPARATEPROC)        (GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
typedef void (*PFNGLSTENCILFUNCSEPARATEPROC)      (GLenum face, GLenum func, GLint ref, GLuint mask);
typedef void (*PFNGLSTENCILMASKSEPARATEPROC)      (GLenum face, GLuint mask);
typedef void (*PFNGLATTACHSHADERPROC)             (GLuint program, GLuint shader);
typedef void (*PFNGLBINDATTRIBLOCATIONPROC)       (GLuint program, GLuint index, const GLchar *name);
typedef void (*PFNGLCOMPILESHADERPROC)            (GLuint shader);
typedef GLuint (*PFNGLCREATEPROGRAMPROC)          (void);
typedef GLuint (*PFNGLCREATESHADERPROC)           (GLenum type);
typedef void (*PFNGLDELETEPROGRAMPROC)            (GLuint program);
typedef void (*PFNGLDELETESHADERPROC)             (GLuint shader);
typedef void (*PFNGLDETACHSHADERPROC)             (GLuint program, GLuint shader);
typedef void (*PFNGLDISABLEVERTEXATTRIBARRAYPROC) (GLuint index);
typedef void (*PFNGLENABLEVERTEXATTRIBARRAYPROC)  (GLuint index);
typedef void (*PFNGLGETACTIVEATTRIBPROC)          (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (*PFNGLGETACTIVEUNIFORMPROC)         (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLint *size, GLenum *type, GLchar *name);
typedef void (*PFNGLGETATTACHEDSHADERSPROC)       (GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
typedef GLint (*PFNGLGETATTRIBLOCATIONPROC)       (GLuint program, const GLchar *name);
typedef void (*PFNGLGETPROGRAMIVPROC)             (GLuint program, GLenum pname, GLint *params);
typedef void (*PFNGLGETPROGRAMINFOLOGPROC)        (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (*PFNGLGETSHADERIVPROC)              (GLuint shader, GLenum pname, GLint *params);
typedef void (*PFNGLGETSHADERINFOLOGPROC)         (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void (*PFNGLGETSHADERSOURCEPROC)          (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
typedef GLint (*PFNGLGETUNIFORMLOCATIONPROC)      (GLuint program, const GLchar *name);
typedef void (*PFNGLGETUNIFORMFVPROC)             (GLuint program, GLint location, GLfloat *params);
typedef void (*PFNGLGETUNIFORMIVPROC)             (GLuint program, GLint location, GLint *params);
typedef void (*PFNGLGETVERTEXATTRIBDVPROC)        (GLuint index, GLenum pname, GLdouble *params);
typedef void (*PFNGLGETVERTEXATTRIBFVPROC)        (GLuint index, GLenum pname, GLfloat *params);
typedef void (*PFNGLGETVERTEXATTRIBIVPROC)        (GLuint index, GLenum pname, GLint *params);
typedef void (*PFNGLGETVERTEXATTRIBPOINTERVPROC)  (GLuint index, GLenum pname, void **pointer);
typedef GLboolean (*PFNGLISPROGRAMPROC)           (GLuint program);
typedef GLboolean (*PFNGLISSHADERPROC)            (GLuint shader);
typedef void (*PFNGLLINKPROGRAMPROC)              (GLuint program);
typedef void (*PFNGLSHADERSOURCEPROC)             (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void (*PFNGLUSEPROGRAMPROC)               (GLuint program);
typedef void (*PFNGLUNIFORM1FPROC)                (GLint location, GLfloat v0);
typedef void (*PFNGLUNIFORM2FPROC)                (GLint location, GLfloat v0, GLfloat v1);
typedef void (*PFNGLUNIFORM3FPROC)                (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void (*PFNGLUNIFORM4FPROC)                (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void (*PFNGLUNIFORM1IPROC)                (GLint location, GLint v0);
typedef void (*PFNGLUNIFORM2IPROC)                (GLint location, GLint v0, GLint v1);
typedef void (*PFNGLUNIFORM3IPROC)                (GLint location, GLint v0, GLint v1, GLint v2);
typedef void (*PFNGLUNIFORM4IPROC)                (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void (*PFNGLUNIFORM1FVPROC)               (GLint location, GLsizei count, const GLfloat *value);
typedef void (*PFNGLUNIFORM2FVPROC)               (GLint location, GLsizei count, const GLfloat *value);
typedef void (*PFNGLUNIFORM3FVPROC)               (GLint location, GLsizei count, const GLfloat *value);
typedef void (*PFNGLUNIFORM4FVPROC)               (GLint location, GLsizei count, const GLfloat *value);
typedef void (*PFNGLUNIFORM1IVPROC)               (GLint location, GLsizei count, const GLint *value);
typedef void (*PFNGLUNIFORM2IVPROC)               (GLint location, GLsizei count, const GLint *value);
typedef void (*PFNGLUNIFORM3IVPROC)               (GLint location, GLsizei count, const GLint *value);
typedef void (*PFNGLUNIFORM4IVPROC)               (GLint location, GLsizei count, const GLint *value);
typedef void (*PFNGLUNIFORMMATRIX2FVPROC)         (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX3FVPROC)         (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX4FVPROC)         (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLVALIDATEPROGRAMPROC)          (GLuint program);
typedef void (*PFNGLVERTEXATTRIB1DPROC)           (GLuint index, GLdouble x);
typedef void (*PFNGLVERTEXATTRIB1DVPROC)          (GLuint index, const GLdouble *v);
typedef void (*PFNGLVERTEXATTRIB1FPROC)           (GLuint index, GLfloat x);
typedef void (*PFNGLVERTEXATTRIB1FVPROC)          (GLuint index, const GLfloat *v);
typedef void (*PFNGLVERTEXATTRIB1SPROC)           (GLuint index, GLshort x);
typedef void (*PFNGLVERTEXATTRIB1SVPROC)          (GLuint index, const GLshort *v);
typedef void (*PFNGLVERTEXATTRIB2DPROC)           (GLuint index, GLdouble x, GLdouble y);
typedef void (*PFNGLVERTEXATTRIB2DVPROC)          (GLuint index, const GLdouble *v);
typedef void (*PFNGLVERTEXATTRIB2FPROC)           (GLuint index, GLfloat x, GLfloat y);
typedef void (*PFNGLVERTEXATTRIB2FVPROC)          (GLuint index, const GLfloat *v);
typedef void (*PFNGLVERTEXATTRIB2SPROC)           (GLuint index, GLshort x, GLshort y);
typedef void (*PFNGLVERTEXATTRIB2SVPROC)          (GLuint index, const GLshort *v);
typedef void (*PFNGLVERTEXATTRIB3DPROC)           (GLuint index, GLdouble x, GLdouble y, GLdouble z);
typedef void (*PFNGLVERTEXATTRIB3DVPROC)          (GLuint index, const GLdouble *v);
typedef void (*PFNGLVERTEXATTRIB3FPROC)           (GLuint index, GLfloat x, GLfloat y, GLfloat z);
typedef void (*PFNGLVERTEXATTRIB3FVPROC)          (GLuint index, const GLfloat *v);
typedef void (*PFNGLVERTEXATTRIB3SPROC)           (GLuint index, GLshort x, GLshort y, GLshort z);
typedef void (*PFNGLVERTEXATTRIB3SVPROC)          (GLuint index, const GLshort *v);
typedef void (*PFNGLVERTEXATTRIB4NBVPROC)         (GLuint index, const GLbyte *v);
typedef void (*PFNGLVERTEXATTRIB4NIVPROC)         (GLuint index, const GLint *v);
typedef void (*PFNGLVERTEXATTRIB4NSVPROC)         (GLuint index, const GLshort *v);
typedef void (*PFNGLVERTEXATTRIB4NUBPROC)         (GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
typedef void (*PFNGLVERTEXATTRIB4NUBVPROC)        (GLuint index, const GLubyte *v);
typedef void (*PFNGLVERTEXATTRIB4NUIVPROC)        (GLuint index, const GLuint *v);
typedef void (*PFNGLVERTEXATTRIB4NUSVPROC)        (GLuint index, const GLushort *v);
typedef void (*PFNGLVERTEXATTRIB4BVPROC)          (GLuint index, const GLbyte *v);
typedef void (*PFNGLVERTEXATTRIB4DPROC)           (GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
typedef void (*PFNGLVERTEXATTRIB4DVPROC)          (GLuint index, const GLdouble *v);
typedef void (*PFNGLVERTEXATTRIB4FPROC)           (GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
typedef void (*PFNGLVERTEXATTRIB4FVPROC)          (GLuint index, const GLfloat *v);
typedef void (*PFNGLVERTEXATTRIB4IVPROC)          (GLuint index, const GLint *v);
typedef void (*PFNGLVERTEXATTRIB4SPROC)           (GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
typedef void (*PFNGLVERTEXATTRIB4SVPROC)          (GLuint index, const GLshort *v);
typedef void (*PFNGLVERTEXATTRIB4UBVPROC)         (GLuint index, const GLubyte *v);
typedef void (*PFNGLVERTEXATTRIB4UIVPROC)         (GLuint index, const GLuint *v);
typedef void (*PFNGLVERTEXATTRIB4USVPROC)         (GLuint index, const GLushort *v);
typedef void (*PFNGLVERTEXATTRIBPOINTERPROC)      (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
extern PFNGLBLENDEQUATIONSEPARATEPROC    glBlendEquationSeparate;
extern PFNGLDRAWBUFFERSPROC              glDrawBuffers;
extern PFNGLSTENCILOPSEPARATEPROC        glStencilOpSeparate;
extern PFNGLSTENCILFUNCSEPARATEPROC      glStencilFuncSeparate;
extern PFNGLSTENCILMASKSEPARATEPROC      glStencilMaskSeparate;
extern PFNGLATTACHSHADERPROC             glAttachShader;
extern PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation;
extern PFNGLCOMPILESHADERPROC            glCompileShader;
extern PFNGLCREATEPROGRAMPROC            glCreateProgram;
extern PFNGLCREATESHADERPROC             glCreateShader;
extern PFNGLDELETEPROGRAMPROC            glDeleteProgram;
extern PFNGLDELETESHADERPROC             glDeleteShader;
extern PFNGLDETACHSHADERPROC             glDetachShader;
extern PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
extern PFNGLGETACTIVEATTRIBPROC          glGetActiveAttrib;
extern PFNGLGETACTIVEUNIFORMPROC         glGetActiveUniform;
extern PFNGLGETATTACHEDSHADERSPROC       glGetAttachedShaders;
extern PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
extern PFNGLGETPROGRAMIVPROC             glGetProgramiv;
extern PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
extern PFNGLGETSHADERIVPROC              glGetShaderiv;
extern PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
extern PFNGLGETSHADERSOURCEPROC          glGetShaderSource;
extern PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation;
extern PFNGLGETUNIFORMFVPROC             glGetUniformfv;
extern PFNGLGETUNIFORMIVPROC             glGetUniformiv;
extern PFNGLGETVERTEXATTRIBDVPROC        glGetVertexAttribdv;
extern PFNGLGETVERTEXATTRIBFVPROC        glGetVertexAttribfv;
extern PFNGLGETVERTEXATTRIBIVPROC        glGetVertexAttribiv;
extern PFNGLGETVERTEXATTRIBPOINTERVPROC  glGetVertexAttribPointerv;
extern PFNGLISPROGRAMPROC                glIsProgram;
extern PFNGLISSHADERPROC                 glIsShader;
extern PFNGLLINKPROGRAMPROC              glLinkProgram;
extern PFNGLSHADERSOURCEPROC             glShaderSource;
extern PFNGLUSEPROGRAMPROC               glUseProgram;
extern PFNGLUNIFORM1FPROC                glUniform1f;
extern PFNGLUNIFORM2FPROC                glUniform2f;
extern PFNGLUNIFORM3FPROC                glUniform3f;
extern PFNGLUNIFORM4FPROC                glUniform4f;
extern PFNGLUNIFORM1IPROC                glUniform1i;
extern PFNGLUNIFORM2IPROC                glUniform2i;
extern PFNGLUNIFORM3IPROC                glUniform3i;
extern PFNGLUNIFORM4IPROC                glUniform4i;
extern PFNGLUNIFORM1FVPROC               glUniform1fv;
extern PFNGLUNIFORM2FVPROC               glUniform2fv;
extern PFNGLUNIFORM3FVPROC               glUniform3fv;
extern PFNGLUNIFORM4FVPROC               glUniform4fv;
extern PFNGLUNIFORM1IVPROC               glUniform1iv;
extern PFNGLUNIFORM2IVPROC               glUniform2iv;
extern PFNGLUNIFORM3IVPROC               glUniform3iv;
extern PFNGLUNIFORM4IVPROC               glUniform4iv;
extern PFNGLUNIFORMMATRIX2FVPROC         glUniformMatrix2fv;
extern PFNGLUNIFORMMATRIX3FVPROC         glUniformMatrix3fv;
extern PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv;
extern PFNGLVALIDATEPROGRAMPROC          glValidateProgram;
extern PFNGLVERTEXATTRIB1DPROC           glVertexAttrib1d;
extern PFNGLVERTEXATTRIB1DVPROC          glVertexAttrib1dv;
extern PFNGLVERTEXATTRIB1FPROC           glVertexAttrib1f;
extern PFNGLVERTEXATTRIB1FVPROC          glVertexAttrib1fv;
extern PFNGLVERTEXATTRIB1SPROC           glVertexAttrib1s;
extern PFNGLVERTEXATTRIB1SVPROC          glVertexAttrib1sv;
extern PFNGLVERTEXATTRIB2DPROC           glVertexAttrib2d;
extern PFNGLVERTEXATTRIB2DVPROC          glVertexAttrib2dv;
extern PFNGLVERTEXATTRIB2FPROC           glVertexAttrib2f;
extern PFNGLVERTEXATTRIB2FVPROC          glVertexAttrib2fv;
extern PFNGLVERTEXATTRIB2SPROC           glVertexAttrib2s;
extern PFNGLVERTEXATTRIB2SVPROC          glVertexAttrib2sv;
extern PFNGLVERTEXATTRIB3DPROC           glVertexAttrib3d;
extern PFNGLVERTEXATTRIB3DVPROC          glVertexAttrib3dv;
extern PFNGLVERTEXATTRIB3FPROC           glVertexAttrib3f;
extern PFNGLVERTEXATTRIB3FVPROC          glVertexAttrib3fv;
extern PFNGLVERTEXATTRIB3SPROC           glVertexAttrib3s;
extern PFNGLVERTEXATTRIB3SVPROC          glVertexAttrib3sv;
extern PFNGLVERTEXATTRIB4NBVPROC         glVertexAttrib4Nbv;
extern PFNGLVERTEXATTRIB4NIVPROC         glVertexAttrib4Niv;
extern PFNGLVERTEXATTRIB4NSVPROC         glVertexAttrib4Nsv;
extern PFNGLVERTEXATTRIB4NUBPROC         glVertexAttrib4Nub;
extern PFNGLVERTEXATTRIB4NUBVPROC        glVertexAttrib4Nubv;
extern PFNGLVERTEXATTRIB4NUIVPROC        glVertexAttrib4Nuiv;
extern PFNGLVERTEXATTRIB4NUSVPROC        glVertexAttrib4Nusv;
extern PFNGLVERTEXATTRIB4BVPROC          glVertexAttrib4bv;
extern PFNGLVERTEXATTRIB4DPROC           glVertexAttrib4d;
extern PFNGLVERTEXATTRIB4DVPROC          glVertexAttrib4dv;
extern PFNGLVERTEXATTRIB4FPROC           glVertexAttrib4f;
extern PFNGLVERTEXATTRIB4FVPROC          glVertexAttrib4fv;
extern PFNGLVERTEXATTRIB4IVPROC          glVertexAttrib4iv;
extern PFNGLVERTEXATTRIB4SPROC           glVertexAttrib4s;
extern PFNGLVERTEXATTRIB4SVPROC          glVertexAttrib4sv;
extern PFNGLVERTEXATTRIB4UBVPROC         glVertexAttrib4ubv;
extern PFNGLVERTEXATTRIB4UIVPROC         glVertexAttrib4uiv;
extern PFNGLVERTEXATTRIB4USVPROC         glVertexAttrib4usv;
extern PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
/* ----GL VERSION 2.1------------------------------------------------------------------------------------------------ */
typedef void (*PFNGLUNIFORMMATRIX2X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX3X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX2X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX4X2FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX3X4FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void (*PFNGLUNIFORMMATRIX4X3FVPROC) (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv;
extern PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
extern PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv;
extern PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv;
extern PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
extern PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;
/* ----GL VERSION 3.0------------------------------------------------------------------------------------------------ */
typedef void            (*PFNGLCOLORMASKIPROC)                          (GLuint index, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
typedef void            (*PFNGLGETBOOLEANI_VPROC)                       (GLenum target, GLuint index, GLboolean *data);
typedef void            (*PFNGLGETINTEGERI_VPROC)                       (GLenum target, GLuint index, GLint *data);
typedef void            (*PFNGLENABLEIPROC)                             (GLenum target, GLuint index);
typedef void            (*PFNGLDISABLEIPROC)                            (GLenum target, GLuint index);
typedef GLboolean       (*PFNGLISENABLEDIPROC)                          (GLenum target, GLuint index);
typedef void            (*PFNGLBEGINTRANSFORMFEEDBACKPROC)              (GLenum primitiveMode);
typedef void            (*PFNGLENDTRANSFORMFEEDBACKPROC)                (void);
typedef void            (*PFNGLBINDBUFFERRANGEPROC)                     (GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
typedef void            (*PFNGLBINDBUFFERBASEPROC)                      (GLenum target, GLuint index, GLuint buffer);
typedef void            (*PFNGLTRANSFORMFEEDBACKVARYINGSPROC)           (GLuint program, GLsizei count, const GLchar *const*varyings, GLenum bufferMode);
typedef void            (*PFNGLGETTRANSFORMFEEDBACKVARYINGPROC)         (GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
typedef void            (*PFNGLCLAMPCOLORPROC)                          (GLenum target, GLenum clamp);
typedef void            (*PFNGLBEGINCONDITIONALRENDERPROC)              (GLuint id, GLenum mode);
typedef void            (*PFNGLENDCONDITIONALRENDERPROC)                (void);
typedef void            (*PFNGLVERTEXATTRIBIPOINTERPROC)                (GLuint index, GLint size, GLenum type, GLsizei stride, const void *pointer);
typedef void            (*PFNGLGETVERTEXATTRIBIIVPROC)                  (GLuint index, GLenum pname, GLint *params);
typedef void            (*PFNGLGETVERTEXATTRIBIUIVPROC)                 (GLuint index, GLenum pname, GLuint *params);
typedef void            (*PFNGLVERTEXATTRIBI1IPROC)                     (GLuint index, GLint x);
typedef void            (*PFNGLVERTEXATTRIBI2IPROC)                     (GLuint index, GLint x, GLint y);
typedef void            (*PFNGLVERTEXATTRIBI3IPROC)                     (GLuint index, GLint x, GLint y, GLint z);
typedef void            (*PFNGLVERTEXATTRIBI4IPROC)                     (GLuint index, GLint x, GLint y, GLint z, GLint w);
typedef void            (*PFNGLVERTEXATTRIBI1UIPROC)                    (GLuint index, GLuint x);
typedef void            (*PFNGLVERTEXATTRIBI2UIPROC)                    (GLuint index, GLuint x, GLuint y);
typedef void            (*PFNGLVERTEXATTRIBI3UIPROC)                    (GLuint index, GLuint x, GLuint y, GLuint z);
typedef void            (*PFNGLVERTEXATTRIBI4UIPROC)                    (GLuint index, GLuint x, GLuint y, GLuint z, GLuint w);
typedef void            (*PFNGLVERTEXATTRIBI1IVPROC)                    (GLuint index, const GLint *v);
typedef void            (*PFNGLVERTEXATTRIBI2IVPROC)                    (GLuint index, const GLint *v);
typedef void            (*PFNGLVERTEXATTRIBI3IVPROC)                    (GLuint index, const GLint *v);
typedef void            (*PFNGLVERTEXATTRIBI4IVPROC)                    (GLuint index, const GLint *v);
typedef void            (*PFNGLVERTEXATTRIBI1UIVPROC)                   (GLuint index, const GLuint *v);
typedef void            (*PFNGLVERTEXATTRIBI2UIVPROC)                   (GLuint index, const GLuint *v);
typedef void            (*PFNGLVERTEXATTRIBI3UIVPROC)                   (GLuint index, const GLuint *v);
typedef void            (*PFNGLVERTEXATTRIBI4UIVPROC)                   (GLuint index, const GLuint *v);
typedef void            (*PFNGLVERTEXATTRIBI4BVPROC)                    (GLuint index, const GLbyte *v);
typedef void            (*PFNGLVERTEXATTRIBI4SVPROC)                    (GLuint index, const GLshort *v);
typedef void            (*PFNGLVERTEXATTRIBI4UBVPROC)                   (GLuint index, const GLubyte *v);
typedef void            (*PFNGLVERTEXATTRIBI4USVPROC)                   (GLuint index, const GLushort *v);
typedef void            (*PFNGLGETUNIFORMUIVPROC)                       (GLuint program, GLint location, GLuint *params);
typedef void            (*PFNGLBINDFRAGDATALOCATIONPROC)                (GLuint program, GLuint color, const GLchar *name);
typedef GLint           (*PFNGLGETFRAGDATALOCATIONPROC)                 (GLuint program, const GLchar *name);
typedef void            (*PFNGLUNIFORM1UIPROC)                          (GLint location, GLuint v0);
typedef void            (*PFNGLUNIFORM2UIPROC)                          (GLint location, GLuint v0, GLuint v1);
typedef void            (*PFNGLUNIFORM3UIPROC)                          (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void            (*PFNGLUNIFORM4UIPROC)                          (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void            (*PFNGLUNIFORM1UIVPROC)                         (GLint location, GLsizei count, const GLuint *value);
typedef void            (*PFNGLUNIFORM2UIVPROC)                         (GLint location, GLsizei count, const GLuint *value);
typedef void            (*PFNGLUNIFORM3UIVPROC)                         (GLint location, GLsizei count, const GLuint *value);
typedef void            (*PFNGLUNIFORM4UIVPROC)                         (GLint location, GLsizei count, const GLuint *value);
typedef void            (*PFNGLTEXPARAMETERIIVPROC)                     (GLenum target, GLenum pname, const GLint *params);
typedef void            (*PFNGLTEXPARAMETERIUIVPROC)                    (GLenum target, GLenum pname, const GLuint *params);
typedef void            (*PFNGLGETTEXPARAMETERIIVPROC)                  (GLenum target, GLenum pname, GLint *params);
typedef void            (*PFNGLGETTEXPARAMETERIUIVPROC)                 (GLenum target, GLenum pname, GLuint *params);
typedef void            (*PFNGLCLEARBUFFERIVPROC)                       (GLenum buffer, GLint drawbuffer, const GLint *value);
typedef void            (*PFNGLCLEARBUFFERUIVPROC)                      (GLenum buffer, GLint drawbuffer, const GLuint *value);
typedef void            (*PFNGLCLEARBUFFERFVPROC)                       (GLenum buffer, GLint drawbuffer, const GLfloat *value);
typedef void            (*PFNGLCLEARBUFFERFIPROC)                       (GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
typedef const GLubyte * (*PFNGLGETSTRINGIPROC)                          (GLenum name, GLuint index);
typedef GLboolean       (*PFNGLISRENDERBUFFERPROC)                      (GLuint renderbuffer);
typedef void            (*PFNGLBINDRENDERBUFFERPROC)                    (GLenum target, GLuint renderbuffer);
typedef void            (*PFNGLDELETERENDERBUFFERSPROC)                 (GLsizei n, const GLuint *renderbuffers);
typedef void            (*PFNGLGENRENDERBUFFERSPROC)                    (GLsizei n, GLuint *renderbuffers);
typedef void            (*PFNGLRENDERBUFFERSTORAGEPROC)                 (GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
typedef void            (*PFNGLGETRENDERBUFFERPARAMETERIVPROC)          (GLenum target, GLenum pname, GLint *params);
typedef GLboolean       (*PFNGLISFRAMEBUFFERPROC)                       (GLuint framebuffer);
typedef void            (*PFNGLBINDFRAMEBUFFERPROC)                     (GLenum target, GLuint framebuffer);
typedef void            (*PFNGLDELETEFRAMEBUFFERSPROC)                  (GLsizei n, const GLuint *framebuffers);
typedef void            (*PFNGLGENFRAMEBUFFERSPROC)                     (GLsizei n, GLuint *framebuffers);
typedef GLenum          (*PFNGLCHECKFRAMEBUFFERSTATUSPROC)              (GLenum target);
typedef void            (*PFNGLFRAMEBUFFERTEXTURE1DPROC)                (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void            (*PFNGLFRAMEBUFFERTEXTURE2DPROC)                (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void            (*PFNGLFRAMEBUFFERTEXTURE3DPROC)                (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void            (*PFNGLFRAMEBUFFERRENDERBUFFERPROC)             (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void            (*PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC) (GLenum target, GLenum attachment, GLenum pname, GLint *params);
typedef void            (*PFNGLGENERATEMIPMAPPROC)                      (GLenum target);
typedef void            (*PFNGLBLITFRAMEBUFFERPROC)                     (GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
typedef void            (*PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)      (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
typedef void            (*PFNGLFRAMEBUFFERTEXTURELAYERPROC)             (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void *          (*PFNGLMAPBUFFERRANGEPROC)                      (GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
typedef void            (*PFNGLFLUSHMAPPEDBUFFERRANGEPROC)              (GLenum target, GLintptr offset, GLsizeiptr length);
typedef void            (*PFNGLBINDVERTEXARRAYPROC)                     (GLuint array);
typedef void            (*PFNGLDELETEVERTEXARRAYSPROC)                  (GLsizei n, const GLuint *arrays);
typedef void            (*PFNGLGENVERTEXARRAYSPROC)                     (GLsizei n, GLuint *arrays);
typedef GLboolean       (*PFNGLISVERTEXARRAYPROC)                       (GLuint array);
extern PFNGLCOLORMASKIPROC                          glColorMaski;
extern PFNGLGETBOOLEANI_VPROC                       glGetBooleani_v;
extern PFNGLGETINTEGERI_VPROC                       glGetIntegeri_v;
extern PFNGLENABLEIPROC                             glEnablei;
extern PFNGLDISABLEIPROC                            glDisablei;
extern PFNGLISENABLEDIPROC                          glIsEnabledi;
extern PFNGLBEGINTRANSFORMFEEDBACKPROC              glBeginTransformFeedback;
extern PFNGLENDTRANSFORMFEEDBACKPROC                glEndTransformFeedback;
extern PFNGLBINDBUFFERRANGEPROC                     glBindBufferRange;
extern PFNGLBINDBUFFERBASEPROC                      glBindBufferBase;
extern PFNGLTRANSFORMFEEDBACKVARYINGSPROC           glTransformFeedbackVaryings;
extern PFNGLGETTRANSFORMFEEDBACKVARYINGPROC         glGetTransformFeedbackVarying;
extern PFNGLCLAMPCOLORPROC                          glClampColor;
extern PFNGLBEGINCONDITIONALRENDERPROC              glBeginConditionalRender;
extern PFNGLENDCONDITIONALRENDERPROC                glEndConditionalRender;
extern PFNGLVERTEXATTRIBIPOINTERPROC                glVertexAttribIPointer;
extern PFNGLGETVERTEXATTRIBIIVPROC                  glGetVertexAttribIiv;
extern PFNGLGETVERTEXATTRIBIUIVPROC                 glGetVertexAttribIuiv;
extern PFNGLVERTEXATTRIBI1IPROC                     glVertexAttribI1i;
extern PFNGLVERTEXATTRIBI2IPROC                     glVertexAttribI2i;
extern PFNGLVERTEXATTRIBI3IPROC                     glVertexAttribI3i;
extern PFNGLVERTEXATTRIBI4IPROC                     glVertexAttribI4i;
extern PFNGLVERTEXATTRIBI1UIPROC                    glVertexAttribI1ui;
extern PFNGLVERTEXATTRIBI2UIPROC                    glVertexAttribI2ui;
extern PFNGLVERTEXATTRIBI3UIPROC                    glVertexAttribI3ui;
extern PFNGLVERTEXATTRIBI4UIPROC                    glVertexAttribI4ui;
extern PFNGLVERTEXATTRIBI1IVPROC                    glVertexAttribI1iv;
extern PFNGLVERTEXATTRIBI2IVPROC                    glVertexAttribI2iv;
extern PFNGLVERTEXATTRIBI3IVPROC                    glVertexAttribI3iv;
extern PFNGLVERTEXATTRIBI4IVPROC                    glVertexAttribI4iv;
extern PFNGLVERTEXATTRIBI1UIVPROC                   glVertexAttribI1uiv;
extern PFNGLVERTEXATTRIBI2UIVPROC                   glVertexAttribI2uiv;
extern PFNGLVERTEXATTRIBI3UIVPROC                   glVertexAttribI3uiv;
extern PFNGLVERTEXATTRIBI4UIVPROC                   glVertexAttribI4uiv;
extern PFNGLVERTEXATTRIBI4BVPROC                    glVertexAttribI4bv;
extern PFNGLVERTEXATTRIBI4SVPROC                    glVertexAttribI4sv;
extern PFNGLVERTEXATTRIBI4UBVPROC                   glVertexAttribI4ubv;
extern PFNGLVERTEXATTRIBI4USVPROC                   glVertexAttribI4usv;
extern PFNGLGETUNIFORMUIVPROC                       glGetUniformuiv;
extern PFNGLBINDFRAGDATALOCATIONPROC                glBindFragDataLocation;
extern PFNGLGETFRAGDATALOCATIONPROC                 glGetFragDataLocation;
extern PFNGLUNIFORM1UIPROC                          glUniform1ui;
extern PFNGLUNIFORM2UIPROC                          glUniform2ui;
extern PFNGLUNIFORM3UIPROC                          glUniform3ui;
extern PFNGLUNIFORM4UIPROC                          glUniform4ui;
extern PFNGLUNIFORM1UIVPROC                         glUniform1uiv;
extern PFNGLUNIFORM2UIVPROC                         glUniform2uiv;
extern PFNGLUNIFORM3UIVPROC                         glUniform3uiv;
extern PFNGLUNIFORM4UIVPROC                         glUniform4uiv;
extern PFNGLTEXPARAMETERIIVPROC                     glTexParameterIiv;
extern PFNGLTEXPARAMETERIUIVPROC                    glTexParameterIuiv;
extern PFNGLGETTEXPARAMETERIIVPROC                  glGetTexParameterIiv;
extern PFNGLGETTEXPARAMETERIUIVPROC                 glGetTexParameterIuiv;
extern PFNGLCLEARBUFFERIVPROC                       glClearBufferiv;
extern PFNGLCLEARBUFFERUIVPROC                      glClearBufferuiv;
extern PFNGLCLEARBUFFERFVPROC                       glClearBufferfv;
extern PFNGLCLEARBUFFERFIPROC                       glClearBufferfi;
extern PFNGLGETSTRINGIPROC                          glGetStringi;
extern PFNGLISRENDERBUFFERPROC                      glIsRenderbuffer;
extern PFNGLBINDRENDERBUFFERPROC                    glBindRenderbuffer;
extern PFNGLDELETERENDERBUFFERSPROC                 glDeleteRenderbuffers;
extern PFNGLGENRENDERBUFFERSPROC                    glGenRenderbuffers;
extern PFNGLRENDERBUFFERSTORAGEPROC                 glRenderbufferStorage;
extern PFNGLGETRENDERBUFFERPARAMETERIVPROC          glGetRenderbufferParameteriv;
extern PFNGLISFRAMEBUFFERPROC                       glIsFramebuffer;
extern PFNGLBINDFRAMEBUFFERPROC                     glBindFramebuffer;
extern PFNGLDELETEFRAMEBUFFERSPROC                  glDeleteFramebuffers;
extern PFNGLGENFRAMEBUFFERSPROC                     glGenFramebuffers;
extern PFNGLCHECKFRAMEBUFFERSTATUSPROC              glCheckFramebufferStatus;
extern PFNGLFRAMEBUFFERTEXTURE1DPROC                glFramebufferTexture1D;
extern PFNGLFRAMEBUFFERTEXTURE2DPROC                glFramebufferTexture2D;
extern PFNGLFRAMEBUFFERTEXTURE3DPROC                glFramebufferTexture3D;
extern PFNGLFRAMEBUFFERRENDERBUFFERPROC             glFramebufferRenderbuffer;
extern PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
extern PFNGLGENERATEMIPMAPPROC                      glGenerateMipmap;
extern PFNGLBLITFRAMEBUFFERPROC                     glBlitFramebuffer;
extern PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC      glRenderbufferStorageMultisample;
extern PFNGLFRAMEBUFFERTEXTURELAYERPROC             glFramebufferTextureLayer;
extern PFNGLMAPBUFFERRANGEPROC                      glMapBufferRange;
extern PFNGLFLUSHMAPPEDBUFFERRANGEPROC              glFlushMappedBufferRange;
extern PFNGLBINDVERTEXARRAYPROC                     glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSPROC                  glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSPROC                     glGenVertexArrays;
extern PFNGLISVERTEXARRAYPROC                       glIsVertexArray;
/* ----GL VERSION 3.1------------------------------------------------------------------------------------------------ */
typedef void   (*PFNGLDRAWARRAYSINSTANCEDPROC)       (GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
typedef void   (*PFNGLDRAWELEMENTSINSTANCEDPROC)     (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount);
typedef void   (*PFNGLTEXBUFFERPROC)                 (GLenum target, GLenum internalformat, GLuint buffer);
typedef void   (*PFNGLPRIMITIVERESTARTINDEXPROC)     (GLuint index);
typedef void   (*PFNGLCOPYBUFFERSUBDATAPROC)         (GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);
typedef void   (*PFNGLGETUNIFORMINDICESPROC)         (GLuint program, GLsizei uniformCount, const GLchar *const*uniformNames, GLuint *uniformIndices);
typedef void   (*PFNGLGETACTIVEUNIFORMSIVPROC)       (GLuint program, GLsizei uniformCount, const GLuint *uniformIndices, GLenum pname, GLint *params);
typedef void   (*PFNGLGETACTIVEUNIFORMNAMEPROC)      (GLuint program, GLuint uniformIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformName);
typedef GLuint (*PFNGLGETUNIFORMBLOCKINDEXPROC)      (GLuint program, const GLchar *uniformBlockName);
typedef void   (*PFNGLGETACTIVEUNIFORMBLOCKIVPROC)   (GLuint program, GLuint uniformBlockIndex, GLenum pname, GLint *params);
typedef void   (*PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC) (GLuint program, GLuint uniformBlockIndex, GLsizei bufSize, GLsizei *length, GLchar *uniformBlockName);
typedef void   (*PFNGLUNIFORMBLOCKBINDINGPROC)       (GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
extern PFNGLDRAWARRAYSINSTANCEDPROC       glDrawArraysInstanced;
extern PFNGLDRAWELEMENTSINSTANCEDPROC     glDrawElementsInstanced;
extern PFNGLTEXBUFFERPROC                 glTexBuffer;
extern PFNGLPRIMITIVERESTARTINDEXPROC     glPrimitiveRestartIndex;
extern PFNGLCOPYBUFFERSUBDATAPROC         glCopyBufferSubData;
extern PFNGLGETUNIFORMINDICESPROC         glGetUniformIndices;
extern PFNGLGETACTIVEUNIFORMSIVPROC       glGetActiveUniformsiv;
extern PFNGLGETACTIVEUNIFORMNAMEPROC      glGetActiveUniformName;
extern PFNGLGETUNIFORMBLOCKINDEXPROC      glGetUniformBlockIndex;
extern PFNGLGETACTIVEUNIFORMBLOCKIVPROC   glGetActiveUniformBlockiv;
extern PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
extern PFNGLUNIFORMBLOCKBINDINGPROC       glUniformBlockBinding;
/* ----GL VERSION 3.2------------------------------------------------------------------------------------------------ */
typedef void      (*PFNGLDRAWELEMENTSBASEVERTEXPROC)          (GLenum mode, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void      (*PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC)     (GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const void *indices, GLint basevertex);
typedef void      (*PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC) (GLenum mode, GLsizei count, GLenum type, const void *indices, GLsizei instancecount, GLint basevertex);
typedef void      (*PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC)     (GLenum mode, const GLsizei *count, GLenum type, const void *const*indices, GLsizei drawcount, const GLint *basevertex);
typedef void      (*PFNGLPROVOKINGVERTEXPROC)                 (GLenum mode);
typedef GLsync    (*PFNGLFENCESYNCPROC)                       (GLenum condition, GLbitfield flags);
typedef GLboolean (*PFNGLISSYNCPROC)                          (GLsync sync);
typedef void      (*PFNGLDELETESYNCPROC)                      (GLsync sync);
typedef GLenum    (*PFNGLCLIENTWAITSYNCPROC)                  (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void      (*PFNGLWAITSYNCPROC)                        (GLsync sync, GLbitfield flags, GLuint64 timeout);
typedef void      (*PFNGLGETINTEGER64VPROC)                   (GLenum pname, GLint64 *data);
typedef void      (*PFNGLGETSYNCIVPROC)                       (GLsync sync, GLenum pname, GLsizei count, GLsizei *length, GLint *values);
typedef void      (*PFNGLGETINTEGER64I_VPROC)                 (GLenum target, GLuint index, GLint64 *data);
typedef void      (*PFNGLGETBUFFERPARAMETERI64VPROC)          (GLenum target, GLenum pname, GLint64 *params);
typedef void      (*PFNGLFRAMEBUFFERTEXTUREPROC)              (GLenum target, GLenum attachment, GLuint texture, GLint level);
typedef void      (*PFNGLTEXIMAGE2DMULTISAMPLEPROC)           (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLboolean fixedsamplelocations);
typedef void      (*PFNGLTEXIMAGE3DMULTISAMPLEPROC)           (GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLboolean fixedsamplelocations);
typedef void      (*PFNGLGETMULTISAMPLEFVPROC)                (GLenum pname, GLuint index, GLfloat *val);
typedef void      (*PFNGLSAMPLEMASKIPROC)                     (GLuint maskNumber, GLbitfield mask);
extern PFNGLDRAWELEMENTSBASEVERTEXPROC          glDrawElementsBaseVertex;
extern PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC     glDrawRangeElementsBaseVertex;
extern PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex;
extern PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC     glMultiDrawElementsBaseVertex;
extern PFNGLPROVOKINGVERTEXPROC                 glProvokingVertex;
extern PFNGLFENCESYNCPROC                       glFenceSync;
extern PFNGLISSYNCPROC                          glIsSync;
extern PFNGLDELETESYNCPROC                      glDeleteSync;
extern PFNGLCLIENTWAITSYNCPROC                  glClientWaitSync;
extern PFNGLWAITSYNCPROC                        glWaitSync;
extern PFNGLGETINTEGER64VPROC                   glGetInteger64v;
extern PFNGLGETSYNCIVPROC                       glGetSynciv;
extern PFNGLGETINTEGER64I_VPROC                 glGetInteger64i_v;
extern PFNGLGETBUFFERPARAMETERI64VPROC          glGetBufferParameteri64v;
extern PFNGLFRAMEBUFFERTEXTUREPROC              glFramebufferTexture;
extern PFNGLTEXIMAGE2DMULTISAMPLEPROC           glTexImage2DMultisample;
extern PFNGLTEXIMAGE3DMULTISAMPLEPROC           glTexImage3DMultisample;
extern PFNGLGETMULTISAMPLEFVPROC                glGetMultisamplefv;
extern PFNGLSAMPLEMASKIPROC                     glSampleMaski;
/* ----GL VERSION 3.3------------------------------------------------------------------------------------------------ */
typedef void      (*PFNGLBINDFRAGDATALOCATIONINDEXEDPROC) (GLuint program, GLuint colorNumber, GLuint index, const GLchar *name);
typedef GLint     (*PFNGLGETFRAGDATAINDEXPROC)            (GLuint program, const GLchar *name);
typedef void      (*PFNGLGENSAMPLERSPROC)                 (GLsizei count, GLuint *samplers);
typedef void      (*PFNGLDELETESAMPLERSPROC)              (GLsizei count, const GLuint *samplers);
typedef GLboolean (*PFNGLISSAMPLERPROC)                   (GLuint sampler);
typedef void      (*PFNGLBINDSAMPLERPROC)                 (GLuint unit, GLuint sampler);
typedef void      (*PFNGLSAMPLERPARAMETERIPROC)           (GLuint sampler, GLenum pname, GLint param);
typedef void      (*PFNGLSAMPLERPARAMETERIVPROC)          (GLuint sampler, GLenum pname, const GLint *param);
typedef void      (*PFNGLSAMPLERPARAMETERFPROC)           (GLuint sampler, GLenum pname, GLfloat param);
typedef void      (*PFNGLSAMPLERPARAMETERFVPROC)          (GLuint sampler, GLenum pname, const GLfloat *param);
typedef void      (*PFNGLSAMPLERPARAMETERIIVPROC)         (GLuint sampler, GLenum pname, const GLint *param);
typedef void      (*PFNGLSAMPLERPARAMETERIUIVPROC)        (GLuint sampler, GLenum pname, const GLuint *param);
typedef void      (*PFNGLGETSAMPLERPARAMETERIVPROC)       (GLuint sampler, GLenum pname, GLint *params);
typedef void      (*PFNGLGETSAMPLERPARAMETERIIVPROC)      (GLuint sampler, GLenum pname, GLint *params);
typedef void      (*PFNGLGETSAMPLERPARAMETERFVPROC)       (GLuint sampler, GLenum pname, GLfloat *params);
typedef void      (*PFNGLGETSAMPLERPARAMETERIUIVPROC)     (GLuint sampler, GLenum pname, GLuint *params);
typedef void      (*PFNGLQUERYCOUNTERPROC)                (GLuint id, GLenum target);
typedef void      (*PFNGLGETQUERYOBJECTI64VPROC)          (GLuint id, GLenum pname, GLint64 *params);
typedef void      (*PFNGLGETQUERYOBJECTUI64VPROC)         (GLuint id, GLenum pname, GLuint64 *params);
typedef void      (*PFNGLVERTEXATTRIBDIVISORPROC)         (GLuint index, GLuint divisor);
typedef void      (*PFNGLVERTEXATTRIBP1UIPROC)            (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void      (*PFNGLVERTEXATTRIBP1UIVPROC)           (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
typedef void      (*PFNGLVERTEXATTRIBP2UIPROC)            (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void      (*PFNGLVERTEXATTRIBP2UIVPROC)           (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
typedef void      (*PFNGLVERTEXATTRIBP3UIPROC)            (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void      (*PFNGLVERTEXATTRIBP3UIVPROC)           (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
typedef void      (*PFNGLVERTEXATTRIBP4UIPROC)            (GLuint index, GLenum type, GLboolean normalized, GLuint value);
typedef void      (*PFNGLVERTEXATTRIBP4UIVPROC)           (GLuint index, GLenum type, GLboolean normalized, const GLuint *value);
extern PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed;
extern PFNGLGETFRAGDATAINDEXPROC            glGetFragDataIndex;
extern PFNGLGENSAMPLERSPROC                 glGenSamplers;
extern PFNGLDELETESAMPLERSPROC              glDeleteSamplers;
extern PFNGLISSAMPLERPROC                   glIsSampler;
extern PFNGLBINDSAMPLERPROC                 glBindSampler;
extern PFNGLSAMPLERPARAMETERIPROC           glSamplerParameteri;
extern PFNGLSAMPLERPARAMETERIVPROC          glSamplerParameteriv;
extern PFNGLSAMPLERPARAMETERFPROC           glSamplerParameterf;
extern PFNGLSAMPLERPARAMETERFVPROC          glSamplerParameterfv;
extern PFNGLSAMPLERPARAMETERIIVPROC         glSamplerParameterIiv;
extern PFNGLSAMPLERPARAMETERIUIVPROC        glSamplerParameterIuiv;
extern PFNGLGETSAMPLERPARAMETERIVPROC       glGetSamplerParameteriv;
extern PFNGLGETSAMPLERPARAMETERIIVPROC      glGetSamplerParameterIiv;
extern PFNGLGETSAMPLERPARAMETERFVPROC       glGetSamplerParameterfv;
extern PFNGLGETSAMPLERPARAMETERIUIVPROC     glGetSamplerParameterIuiv;
extern PFNGLQUERYCOUNTERPROC                glQueryCounter;
extern PFNGLGETQUERYOBJECTI64VPROC          glGetQueryObjecti64v;
extern PFNGLGETQUERYOBJECTUI64VPROC         glGetQueryObjectui64v;
extern PFNGLVERTEXATTRIBDIVISORPROC         glVertexAttribDivisor;
extern PFNGLVERTEXATTRIBP1UIPROC            glVertexAttribP1ui;
extern PFNGLVERTEXATTRIBP1UIVPROC           glVertexAttribP1uiv;
extern PFNGLVERTEXATTRIBP2UIPROC            glVertexAttribP2ui;
extern PFNGLVERTEXATTRIBP2UIVPROC           glVertexAttribP2uiv;
extern PFNGLVERTEXATTRIBP3UIPROC            glVertexAttribP3ui;
extern PFNGLVERTEXATTRIBP3UIVPROC           glVertexAttribP3uiv;
extern PFNGLVERTEXATTRIBP4UIPROC            glVertexAttribP4ui;
extern PFNGLVERTEXATTRIBP4UIVPROC           glVertexAttribP4uiv;

// @todo(mdodis): GL 4.3 CORE
typedef void (*GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);
typedef void (*PFNGLDEBUGMESSAGECALLBACKPROC) (GLDEBUGPROC callback, const void *userParam);
extern PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
#endif // !defined(__APPLE__)

#if defined(__APPLE__)
#pragma clang diagnostic pop
#endif
#endif // !VD_FW_H

#ifdef VD_FW_IMPL

#if defined(__APPLE__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif

static void vd_fw__load_opengl(VdFwGlVersion version);
static void *vd_fw__gl_get_proc_address(const char *name);

#ifdef _WIN32
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "Shcore.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "kernel32.lib")
#if VD_FW_WIN32_SUBSYSTEM == VD_FW_WIN32_SUBSYSTEM_CONSOLE
#pragma comment(linker, "/subsystem:console")
#else
#pragma comment(linker, "/subsystem:windows")
#endif // VD_FW_WIN32_SUBSYSTEM == VD_FW_WIN32_SUBSYSTEM_CONSOLE
#if VD_FW_NO_CRT
#pragma comment(linker, "/NODEFAULTLIB:libcmt.lib")
#pragma comment(linker, "/NODEFAULTLIB:libcmtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:msvcrt.lib")
#pragma comment(linker, "/NODEFAULTLIB:msvcrtd.lib")
#pragma comment(linker, "/NODEFAULTLIB:oldnames.lib")
#endif // VD_FW_NO_CRT
// #define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
// #define NOSYSMETRICS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define NORASTEROPS
#define OEMRESOURCE
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
// #define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <uxtheme.h>
#include <dwmapi.h>
#include <shellscalingapi.h>
#include <versionhelpers.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#define min min
#define max max
#ifdef FAILED
#undef FAILED
#endif
// #ifdef RGB
// #undef RGB
// #endif
#ifdef FIXED
#undef FIXED
#endif

#ifndef WM_NCUAHDRAWCAPTION
#define WM_NCUAHDRAWCAPTION (0x00AE)
#endif
#ifndef WM_NCUAHDRAWFRAME
#define WM_NCUAHDRAWFRAME (0x00AF)
#endif

typedef BOOL (WINAPI * PFNWGLSWAPINTERVALEXTPROC) (int interval);

typedef struct {
    HWND                        hwnd;
    HDC                         hdc;
    HGLRC                       hglrc;
    RECT                        rgn;
    BOOL                        theme_enabled;
    BOOL                        composition_enabled;
    HANDLE                      win_thread;
    DWORD                       win_thread_id;
    DWORD                       main_thread_id;
    HANDLE                      sem_window_ready;
    LARGE_INTEGER               frequency;
    LARGE_INTEGER               performance_counter;
    PFNWGLSWAPINTERVALEXTPROC   proc_swapInterval;
    HANDLE                      sem_closed;
    volatile BOOL               t_running;
    BOOL                        draw_decorations;
} VdFw__Win32InternalData;

static VdFw__Win32InternalData Vd_Fw_Globals = {0};
static LRESULT vd_fw__wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
static void    vd_fw__composition_changed(void);
static void    vd_fw__update_region(void);
static void    vd_fw__theme_changed(void);
static LRESULT vd_fw__nccalcsize(WPARAM wparam, LPARAM lparam);
static BOOL    vd_fw__has_autohide_taskbar(UINT edge, RECT monitor);
static void    vd_fw__window_pos_changed(WINDOWPOS *pos);
static LRESULT vd_fw__handle_invisible(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
static DWORD   vd_fw__win_thread_proc(LPVOID param);
static void    vd_fw__gl_debug_message_callback(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);

#if VD_FW_NO_CRT
#define VD_FW__CHECK_HRESULT(expr) expr
#define VD_FW__CHECK_INT(expr) expr
#define VD_FW__CHECK_NONZERO(expr) expr
#define VD_FW_SANITY_CHECK()
#define VD_FW__CHECK_NULL(expr) expr
#define VD_FW__CHECK_TRUE(expr) expr
#else
#include <stdio.h>
#define VD_FW__CHECK_HRESULT(expr) do {\
    if ((expr) != S_OK) { printf("Failed at: %s\n", #expr); DebugBreak(); } \
} while (0)

#define VD_FW__CHECK_INT(expr) do {\
    if ((expr) != 0) { printf("Failed at: %s\n", #expr); DebugBreak(); } \
} while (0)

#define VD_FW__CHECK_NONZERO(expr) do {\
    if ((expr) == 0) { printf("Failed at: %s\nGetLastError: %d", #expr, GetLastError()); DebugBreak(); } \
} while (0)

#define VD_FW_SANITY_CHECK() do { \
    DWORD error = GetLastError(); \
    if (error != ERROR_SUCCESS) { printf("GetLastError: %d\n", error); DebugBreak(); } \
} while (0)

#define VD_FW__CHECK_NULL(expr) do {\
    if ((expr) == 0) { printf("Failed at: %s\n GetLastError: %d", #expr, GetLastError()); DebugBreak(); } \
} while (0)

#define VD_FW__CHECK_TRUE(expr) do {\
    if ((expr) != TRUE) { printf("Failed at: %s\n GetLastError: %d", #expr, GetLastError()); DebugBreak(); } \
} while (0)
#endif // VD_FW_NO_CRT

#define VD_FW_G Vd_Fw_Globals

static void *vd_fw__gl_get_proc_address(const char *name)
{
    return (void*)wglGetProcAddress(name);
}

int vd_fw_init(VdFwInitInfo *info)
{
    if (info != NULL) {
        VD_FW_G.draw_decorations = info->window_options.draw_default_borders;
    }

    VD_FW_G.main_thread_id = GetCurrentThreadId();

    VD_FW_G.sem_window_ready = CreateSemaphoreA(
        NULL,
        0,
        1,
        NULL);

    VD_FW_G.sem_closed = CreateSemaphoreA(
        NULL,
        0,
        1,
        NULL);

    // DWORD spin_count = 0;
    // VD_FW__CHECK_TRUE(InitializeCriticalSectionAndSpinCount(
    //     &VD_FW_G.render_section,
    //     spin_count));

    VD_FW_G.win_thread = CreateThread(
        NULL,
        0,
        vd_fw__win_thread_proc,
        &VD_FW_G.main_thread_id,
        0,
        &VD_FW_G.win_thread_id);

    WaitForSingleObject(VD_FW_G.sem_window_ready, INFINITE);

    VD_FW_G.hdc = GetDC(VD_FW_G.hwnd);

    PIXELFORMATDESCRIPTOR pfd = {
      sizeof(PIXELFORMATDESCRIPTOR),
      1,                                // Version Number
      PFD_DRAW_TO_WINDOW |              // Format Must Support Window
      PFD_SUPPORT_OPENGL |              // Format Must Support OpenGL
      PFD_SUPPORT_COMPOSITION |      // Format Must Support Composition
      PFD_DOUBLEBUFFER,                 // Must Support Double Buffering
      PFD_TYPE_RGBA,                    // Request An RGBA Format
      32,                               // Select Our Color Depth
      0, 0, 0, 0, 0, 0,                 // Color Bits Ignored
      8,                                // An Alpha Buffer
      0,                                // Shift Bit Ignored
      0,                                // No Accumulation Buffer
      0, 0, 0, 0,                       // Accumulation Bits Ignored
      24,                               // 16Bit Z-Buffer (Depth Buffer)
      8,                                // Some Stencil Buffer
      0,                                // No Auxiliary Buffer
      PFD_MAIN_PLANE,                   // Main Drawing Layer
      0,                                // Reserved
      0, 0, 0                           // Layer Masks Ignored
    };
    int pf = ChoosePixelFormat(VD_FW_G.hdc, &pfd);
    SetPixelFormat(VD_FW_G.hdc, pf, &pfd);

    VD_FW_G.hglrc = wglCreateContext(VD_FW_G.hdc);

    VD_FW__CHECK_NULL(VD_FW_G.hglrc);
    VD_FW__CHECK_TRUE(wglMakeCurrent(VD_FW_G.hdc, VD_FW_G.hglrc));

    VD_FW_G.proc_swapInterval = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");

    VdFwGlVersion version = VD_FW_GL_VERSION_3_3;
    if (info && info->gl.version != VD_FW_GL_VERSION_BASIC) {
        version = info->gl.version;
    }
    vd_fw__load_opengl(version);

    if (info != 0) {
#if VD_FW_WIN32_SUBSYSTEM == VD_FW_WIN32_SUBSYSTEM_WINDOWS
        if (info->gl.debug_on) {
            AllocConsole();
            DWORD written;
            SetConsoleTitle("DEBUG CONSOLE");
            WriteConsole(
                GetStdHandle(STD_OUTPUT_HANDLE),
                "Console allocated for debugging\n",
                sizeof("Console allocated for debugging\n") - 1,
                &written,
                0);
        }
#endif
    }

    if (info != 0 && info->gl.debug_on) {
        glDebugMessageCallback = (PFNGLDEBUGMESSAGECALLBACKPROC)wglGetProcAddress("glDebugMessageCallback");

        if (!glDebugMessageCallback) {
            DWORD written;
            WriteConsole(
                GetStdHandle(STD_OUTPUT_HANDLE),
                "ERROR: Failed to load glDebugMessageCallback!",
                sizeof("ERROR: Failed to load glDebugMessageCallback!") - 1,
                &written,
                0);
        } else {
            glEnable(0x92E0 /* GL_DEBUG_OUTPUT */);
            glDebugMessageCallback(vd_fw__gl_debug_message_callback, 0);
        }
    }

    QueryPerformanceFrequency(&VD_FW_G.frequency);
    QueryPerformanceCounter(&VD_FW_G.performance_counter);
    return 1;
}

int vd_fw_running(void)
{
    DWORD result = WaitForSingleObject(VD_FW_G.sem_closed, 0);
    if (result == WAIT_OBJECT_0) {
        return 0;
    } else if (result == WAIT_TIMEOUT) {
        return 1;
    } else {
        return 0;
    }
}

int vd_fw_swap_buffers(void)
{
    QueryPerformanceCounter(&VD_FW_G.performance_counter);
    SwapBuffers(VD_FW_G.hdc);
    return 1;
}

int vd_fw_get_size(int *w, int *h)
{
    RECT rect;
    GetClientRect(VD_FW_G.hwnd, &rect);
    *w = rect.right - rect.left;
    *h = rect.bottom - rect.top;
    return 0;
}

int vd_fw_set_vsync_on(int on)
{
    BOOL result = VD_FW_G.proc_swapInterval(on);

    return result == TRUE ? on : 0;
}

int vd_fw_get_mouse_state(int *x, int *y)
{
    POINT p;
    GetCursorPos(&p);

    ScreenToClient(VD_FW_G.hwnd, &p);

    if (x) *x = p.x;
    if (y) *y = p.y;

    return 0;
}

unsigned long long vd_fw_delta_ns(void)
{
    LARGE_INTEGER now_performance_counter;
    QueryPerformanceCounter(&now_performance_counter);
    LARGE_INTEGER delta;
    delta.QuadPart = now_performance_counter.QuadPart - VD_FW_G.performance_counter.QuadPart;
    unsigned long long q  =  delta.QuadPart / VD_FW_G.frequency.QuadPart;
    unsigned long long r  =  delta.QuadPart % VD_FW_G.frequency.QuadPart;
    unsigned long long ns =  q * 1000000000ULL;
    ns                    += (r * 1000000000ULL) / VD_FW_G.frequency.QuadPart;
    return ns;
}

static DWORD vd_fw__win_thread_proc(LPVOID param)
{
    (void)param;
    VD_FW_G.t_running = TRUE;

    SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
    VD_FW_SANITY_CHECK();

    WNDCLASSEXW wcx;
    ZeroMemory(&wcx, sizeof(wcx));
    wcx.cbSize         = sizeof(wcx);
    wcx.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcx.hInstance      = NULL;
    wcx.lpfnWndProc    = vd_fw__wndproc;
    wcx.lpszClassName  = L"FWCLASS";
    wcx.hbrBackground  = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcx.hCursor        = LoadCursorA(NULL, IDC_ARROW);
    if (!RegisterClassExW(&wcx)) {
        return 0;
    }

    VD_FW_G.hwnd = CreateWindowExW(
        WS_EX_APPWINDOW | WS_EX_LAYERED,
        L"FWCLASS",
        L"FW Window",
        WS_OVERLAPPEDWINDOW | WS_SIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1600,
        900,
        0 /* hwndParent */,
        0 /* hMenu */,
        0 /* hInstance */,
        0 /* lpParam */);

    SetLayeredWindowAttributes(VD_FW_G.hwnd, RGB(255, 0, 255), 0, LWA_COLORKEY);

    vd_fw__composition_changed();
    VD_FW_SANITY_CHECK();
    vd_fw__theme_changed();
    VD_FW_SANITY_CHECK();

    // SetWindowPos(VD_FW_G.hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_SHOWWINDOW);
    ShowWindow(VD_FW_G.hwnd, SW_SHOW);
    VD_FW__CHECK_NONZERO(UpdateWindow(VD_FW_G.hwnd));

    VD_FW__CHECK_TRUE(ReleaseSemaphore(VD_FW_G.sem_window_ready, 1, NULL));

    while (VD_FW_G.t_running) {
        MSG message;
        while (GetMessage(&message, VD_FW_G.hwnd, 0, 0)) {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }

    return 0;
}

static void vd_fw__gl_debug_message_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    (void)userParam;
    (void)severity;
    (void)id;
    (void)type;
    (void)source;

    DWORD written;
    WriteConsole(
        GetStdHandle(STD_OUTPUT_HANDLE),
        message,
        length,
        &written,
        0);

    WriteConsole(
        GetStdHandle(STD_OUTPUT_HANDLE),
        "\n",
        1,
        &written,
        0);
}

static int vd_fw__hit_test(int x, int y)
{
    if (IsMaximized(VD_FW_G.hwnd)) {
        return HTCLIENT;
    }

    POINT mouse;
    mouse.x = x;
    mouse.y = y;
    ScreenToClient(VD_FW_G.hwnd, &mouse);

    RECT client;
    GetClientRect(VD_FW_G.hwnd, &client);
    int width  = client.right - client.left;
    int height = client.bottom - client.top;

    /* The horizontal frame should be the same size as the vertical frame,
       since the NONCLIENTMETRICS structure does not distinguish between them */
    int frame_size = GetSystemMetrics(SM_CXFRAME) +
                     GetSystemMetrics(SM_CXPADDEDBORDER);
    /* The diagonal size handles are wider than the frame */
    int diagonal_width = frame_size * 2 + GetSystemMetrics(SM_CXBORDER);

    if (!PtInRect(&client, mouse)) {
        return HTNOWHERE;
    }

    if (mouse.y < frame_size) {
        if (mouse.x < diagonal_width) {
            return HTTOPLEFT;
        }

        if (mouse.x >= width - diagonal_width) {
            return HTTOPRIGHT;
        }

        return HTTOP;
    }

    if (mouse.y >= height - frame_size) {
        if (mouse.x < diagonal_width) {
            return HTBOTTOMLEFT;
        }

        if (mouse.x >= width - diagonal_width) {
            return HTBOTTOMRIGHT;
        }

        return HTBOTTOM;
    }

    if (mouse.x < frame_size) {
        return HTLEFT;
    }

    if (mouse.x >= width - frame_size) {
        return HTRIGHT;
    }

    if (mouse.y < 30) {
        return HTCLIENT;
    }
    return HTNOWHERE;
}

static void vd_fw__composition_changed(void)
{
    BOOL enabled = FALSE;
    VD_FW__CHECK_HRESULT(DwmIsCompositionEnabled(&enabled));
    VD_FW_G.composition_enabled = enabled;

    if (enabled) {
        // MARGINS m = {0, 0, 1, 0};
        MARGINS m = {-1};
        VD_FW__CHECK_HRESULT(DwmExtendFrameIntoClientArea(VD_FW_G.hwnd, &m));
        DWORD value = DWMNCRP_ENABLED;
        VD_FW__CHECK_HRESULT(DwmSetWindowAttribute(VD_FW_G.hwnd, DWMWA_NCRENDERING_POLICY, &value, sizeof(value)));
    }

    vd_fw__update_region();
}

static void vd_fw__update_region(void)
{
    RECT old_rgn = VD_FW_G.rgn;

    if (IsMaximized(VD_FW_G.hwnd)) {
        WINDOWINFO window_info = {};
        window_info.cbSize = sizeof(window_info);
        GetWindowInfo(VD_FW_G.hwnd, &window_info);

        VD_FW_G.rgn = (RECT) {
            .left   = window_info.rcClient.left   - window_info.rcWindow.left,
            .top    = window_info.rcClient.top    - window_info.rcWindow.top,
            .right  = window_info.rcClient.right  - window_info.rcWindow.left,
            .bottom = window_info.rcClient.bottom - window_info.rcWindow.top,
        };
    } else if (!VD_FW_G.composition_enabled) {
        VD_FW_G.rgn = (RECT) {
            .left   = 0,
            .top    = 0,
            .right  = 32767,
            .bottom = 32767,
        };
    } else {
        VD_FW_G.rgn = (RECT) {
            .left   = 0,
            .top    = 0,
            .right  = 0,
            .bottom = 0,
        };
    }

    if (EqualRect(&VD_FW_G.rgn, &old_rgn)) {
        return;
    }

    RECT zero_rect = {0};
    if (EqualRect(&VD_FW_G.rgn, &zero_rect)) {
        SetWindowRgn(VD_FW_G.hwnd, NULL, TRUE);
    } else {
        SetWindowRgn(VD_FW_G.hwnd, CreateRectRgnIndirect(&VD_FW_G.rgn), TRUE);
    }
}

static void vd_fw__theme_changed(void)
{
    VD_FW_G.theme_enabled = IsThemeActive();
}

static LRESULT vd_fw__nccalcsize(WPARAM wparam, LPARAM lparam)
{
    int borderless = 1;
    if (wparam && borderless) {

        NCCALCSIZE_PARAMS *params = (NCCALCSIZE_PARAMS*)lparam;
        if (IsMaximized(VD_FW_G.hwnd)) {
            HMONITOR monitor = MonitorFromWindow(VD_FW_G.hwnd, MONITOR_DEFAULTTONULL);
            if (!monitor) {
                return 0;
            }

            MONITORINFO monitor_info = {0};
            monitor_info.cbSize = sizeof(monitor_info);
            if (!GetMonitorInfoW(monitor, &monitor_info)) {
                return 0;
            }

            params->rgrc[0] = monitor_info.rcWork;
            return 0;
        } else {
            params->rgrc[0].bottom += 1;
            return WVR_VALIDRECTS;
        }

    } else {
        return DefWindowProcW(VD_FW_G.hwnd, WM_NCCALCSIZE, wparam, lparam);
    }
    // if (!VD_FW_G.draw_decorations) {
    //     if (wparam == TRUE) {
    //         return;
    //     }
    // }

    // union {
    //     LPARAM lparam;
    //     RECT   *rect;
    // } params = { .lparam = lparam };

    // RECT non_client = *params.rect;
    // DefWindowProcW(VD_FW_G.hwnd, WM_NCCALCSIZE, wparam, params.lparam);

    // RECT client = *params.rect;

    // if (IsMaximized(VD_FW_G.hwnd)) {
    //     WINDOWINFO window_info = {0};
    //     window_info.cbSize = sizeof(window_info);
    //     GetWindowInfo(VD_FW_G.hwnd, &window_info);

    //     *params.rect = (RECT) {
    //         .left   = client.left,
    //         .top    = non_client.top + window_info.cyWindowBorders,
    //         .right  = client.right,
    //         .bottom = client.bottom,
    //     };

    //     HMONITOR monitor = MonitorFromWindow(VD_FW_G.hwnd, MONITOR_DEFAULTTOPRIMARY);
    //     MONITORINFO monitor_info = {0};
    //     monitor_info.cbSize = sizeof(monitor_info);
    //     GetMonitorInfoW(monitor, &monitor_info);

    //     if (EqualRect(params.rect, &monitor_info.rcMonitor)) {
    //         if (vd_fw__has_autohide_taskbar(ABE_BOTTOM, monitor_info.rcMonitor)) {
    //             params.rect->bottom--;
    //         } else if (vd_fw__has_autohide_taskbar(ABE_LEFT, monitor_info.rcMonitor)) {
    //             params.rect->left++;
    //         } else if (vd_fw__has_autohide_taskbar(ABE_TOP, monitor_info.rcMonitor)) {
    //             params.rect->top++;
    //         } else if (vd_fw__has_autohide_taskbar(ABE_RIGHT, monitor_info.rcMonitor)) {
    //             params.rect->right--;
    //         }
    //     } else {
    //         *params.rect = non_client;
    //     }
    // }
}

static BOOL vd_fw__has_autohide_taskbar(UINT edge, RECT monitor)
{
    if (IsWindows8Point1OrGreater()) {
        APPBARDATA appbar_data = {0};
        appbar_data.cbSize = sizeof(appbar_data);
        appbar_data.uEdge  = edge;
        appbar_data.rc     = monitor;
        return SHAppBarMessage(ABM_GETAUTOHIDEBAREX, &appbar_data) != 0;
    }

    if (monitor.left != 0 || monitor.top != 0) {
        return FALSE;
    }

    APPBARDATA appbar_data = {0};
    appbar_data.cbSize = sizeof(appbar_data);
    appbar_data.uEdge  = edge;
    return SHAppBarMessage(ABM_GETAUTOHIDEBAR, &appbar_data) != 0;
}

static void vd_fw__window_pos_changed(WINDOWPOS *pos)
{
    RECT client;
    GetClientRect(VD_FW_G.hwnd, &client);
    if (pos->flags & SWP_FRAMECHANGED) {
        vd_fw__update_region();
    }
    VD_FW__CHECK_NONZERO(InvalidateRect(VD_FW_G.hwnd, &client, TRUE));
}

static LRESULT vd_fw__handle_invisible(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    LONG_PTR old_style = GetWindowLongPtr(hwnd, GWL_STYLE);

    SetWindowLongPtrW(hwnd, GWL_STYLE, old_style & ~WS_VISIBLE);    
    LRESULT result = DefWindowProcW(hwnd, msg, wparam, lparam);
    SetWindowLongPtrW(hwnd, GWL_STYLE, old_style);    

    return result;
}

static void vd_fw__paint(void)
{
    // render();
    // PAINTSTRUCT ps;
    // HDC dc = BeginPaint(VD_FW_G.hwnd, &ps);
    // HBRUSH bb = CreateSolidBrush(RGB(0, 255, 0));

    // RECT rect;
    // GetClientRect(VD_FW_G.hwnd, &rect);
    // int width = rect.right - rect.left;
    // int height = rect.bottom - rect.top;

    // /* Draw a rectangle on the border of the client area for testing */
    // FillRect(dc, &(RECT) { 0, 0, 1, height }, bb);
    // FillRect(dc, &(RECT) { 0, 0, width, 1 }, bb);
    // FillRect(dc, &(RECT) { width - 1, 0, width, height }, bb);
    // FillRect(dc, &(RECT) { 0, height - 1, width, height }, bb);

    // DeleteObject(bb);
    // EndPaint(VD_FW_G.hwnd, &ps);
}

static LRESULT vd_fw__wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    static UINT timer_id = 0;

    LRESULT result = 0;
    switch (msg) {

        case WM_CLOSE: {
            DestroyWindow(hwnd);
        } break;

        case WM_DESTROY: {
            ReleaseSemaphore(VD_FW_G.sem_closed, 1, NULL);
            PostQuitMessage(0);
            VD_FW_G.t_running = FALSE;
        } break;

        case WM_DWMCOMPOSITIONCHANGED: {
            vd_fw__composition_changed();
        } break;

        // @todo(mdodis): WM_KEYDOWN

        case WM_ERASEBKGND: {
            result = 1;
        } break;

        case WM_LBUTTONDOWN: {
            ReleaseCapture();
            SendMessageW(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        } break;

        case WM_NCACTIVATE: {
            result = DefWindowProc(hwnd, msg, wparam, -1);
        } break;

        case WM_NCCALCSIZE: {
            vd_fw__nccalcsize(wparam, lparam);
        } break;

        case WM_NCHITTEST: {
            result = vd_fw__hit_test(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
        } break;

        case WM_NCPAINT: {
            if (VD_FW_G.composition_enabled) {
                result = DefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_NCUAHDRAWCAPTION:
        case WM_NCUAHDRAWFRAME: {
            result = 0;
        } break;

        case WM_PAINT: {
            // render();
            // vd_fw_swap_buffers();
            DwmFlush();
        } break;

        case WM_SETICON:
        case WM_SETTEXT: {
            if (!VD_FW_G.composition_enabled && !VD_FW_G.theme_enabled) {
                result = vd_fw__handle_invisible(hwnd, msg, wparam, lparam);
            } else {
                result = DefWindowProc(hwnd, msg, wparam, lparam);
            }
        } break;

        case WM_THEMECHANGED: {
            vd_fw__theme_changed();
            result = DefWindowProc(hwnd, msg, wparam, lparam);
        } break;

        case WM_WINDOWPOSCHANGED: {
            vd_fw__window_pos_changed((WINDOWPOS*)lparam);
        } break;

        case WM_ENTERSIZEMOVE: {
            // VD_FW__CHECK_NULL(SetTimer(VD_FW_G.hwnd, (UINT_PTR)&timer_id, USER_TIMER_MINIMUM, NULL));

        } break;

        case WM_EXITSIZEMOVE: {
            // KillTimer(VD_FW_G.hwnd, (UINT_PTR)&timer_id);
        } break;

        case WM_TIMER: {
            // render();
            // vd_fw_swap_buffers();
            // DwmFlush();
            result = 1;
        } break;

        case WM_MOVING:
        case WM_SIZING: {
            DwmFlush();
        } break;

        // case WM_SYSCOMMAND: {
        //     if ((wparam & 0xfff0) == SC_KEYMENU)
        //         return 0;
        // } break;


        // case WM_MOUSEMOVE: {
        //     if ((VdFw__WindowState == VD_FW__MOVING) || (VdFw__WindowState == VD_FW__SIZING)) {
        //         POINT cursor;
        //         GetCursorPos(&cursor);

        //         int dx = cursor.x - VdFw__DragStart.x;
        //         int dy = cursor.y - VdFw__DragStart.y;

        //         RECT new_rect = VdFw__DragStartRect;

        //         if (VdFw__WindowState == VD_FW__MOVING) {
        //             OffsetRect(&new_rect, dx, dy);
        //         } else {
        //         }

        //         SetWindowPos(VD_FW_G.hwnd, NULL,
        //             new_rect.left, new_rect.top,
        //             new_rect.right - new_rect.left,
        //             new_rect.bottom - new_rect.top,
        //             SWP_NOZORDER | SWP_NOACTIVATE);
        //     }
        // } break;

        // case WM_LBUTTONUP: {
        //     if ((VdFw__WindowState == VD_FW__MOVING) || (VdFw__WindowState == VD_FW__SIZING)) {
        //         VdFw__WindowState = 0;
        //         ReleaseCapture();
        //     }
        // } break;

        default: {
            result = DefWindowProc(hwnd, msg, wparam, lparam);
        } break;
    }    
    return result;
}

#if VD_FW_NO_CRT
#pragma function(memset)
void *__cdecl memset(void *dest, int value, size_t num)
{
    __stosb((unsigned char *)dest, (unsigned char)value, num);
    return dest;
}

int _fltused;
#ifdef _M_IX86

    // float to int64 cast
    // on /arch:IA32
    // on /arch:SSE
    // on /arch:SSE2 with /d2noftol3 compiler switch
    __declspec(naked) void _ftol2()
    {
        __asm
        {
            fistp qword ptr[esp - 8]
            mov   edx, [esp - 4]
            mov   eax, [esp - 8]
            ret
        }
    }

    // float to int64 cast on /arch:IA32
    __declspec(naked) void _ftol2_sse()
    {
        __asm
        {
            fistp dword ptr[esp - 4]
            mov   eax, [esp - 4]
            ret
        }
    }

    // float to uint32 cast on / arch:SSE2
    __declspec(naked) void _ftoui3()
    {

    }

    // float to int64 cast on / arch:SSE2
    __declspec(naked) void _ftol3()
    {
        
    }

    // float to uint64 cast on / arch:SSE2
    __declspec(naked) void _ftoul3()
    {

    }

    // int64 to float cast on / arch:SSE2
    __declspec(naked) void _ltod3()
    {

    }

    // uint64 to float cast on / arch:SSE2
    __declspec(naked) void _ultod3()
    {

    }

#endif

#if VD_FW_WIN32_SUBSYSTEM == VD_FW_WIN32_SUBSYSTEM_CONSOLE
int mainCRTStartup(void)
{
    int result = main(0, 0);
    ExitProcess(result);
}
#else
LRESULT WinMainCRTStartup(void)
{
    int result = main(0, 0);
    ExitProcess(result);
}
#endif // VD_FW_WIN32_SUBSYSTEM == VD_FW_WIN32_SUBSYSTEM_CONSOLE
#else
int wWinMain(HINSTANCE hinstance, HINSTANCE prev_instance, LPWSTR cmdline, int nshowcmd)
{
    VD_UNUSED(hinstance);
    VD_UNUSED(prev_instance);
    VD_UNUSED(cmdline);
    VD_UNUSED(nshowcmd);
    int result = main(0, 0);
    ExitProcess(result);
}
#endif // VD_FW_NO_CRT

#undef VD_FW_G
#elif defined(__APPLE__)
#import <AppKit/AppKit.h>
#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <QuartzCore/CVDisplayLink.h>
#import <os/log.h>
#import <mach/mach_time.h>
#define VD_FW_G Vd_Fw_Globals

typedef struct {
    NSWindow                    *window;
    NSOpenGLContext             *gl_context;
    BOOL                        should_close;
    mach_timebase_info_data_t   time_base;
    uint64_t                    last_time;
    CGFloat                     scale;
} VdFw__MacOsInternalData;

static VdFw__MacOsInternalData Vd_Fw_Globals;

@interface VdFwWindowDelegate : NSObject<NSWindowDelegate>
@end

@implementation VdFwWindowDelegate
- (void)windowWillClose:(NSNotification*)notification {
    VD_FW_G.should_close = YES;
}
@end

int vd_fw_init(VdFwInitInfo *info)
{
    @autoreleasepool {
        VD_FW_G.scale = [[NSScreen mainScreen] backingScaleFactor];

        [NSApplication sharedApplication];
        [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
        [NSApp activateIgnoringOtherApps:YES];
        [NSEvent setMouseCoalescingEnabled:NO];

        int w = 640;
        int h = 480;
        NSRect frame = NSMakeRect(100, 100, w, h);
        VD_FW_G.window = [[NSWindow alloc] initWithContentRect:frame
                                           styleMask:(
                                                NSWindowStyleMaskTitled |
                                                NSWindowStyleMaskClosable |
                                                NSWindowStyleMaskResizable)
                                           backing: NSBackingStoreBuffered
                                           defer: NO];
        [VD_FW_G.window setTitle:[NSString stringWithUTF8String:"F"]];
        [VD_FW_G.window makeKeyAndOrderFront:nil];

        NSOpenGLPixelFormatAttribute attrs[] = {
            NSOpenGLPFAOpenGLProfile, NSOpenGLProfileVersion3_2Core,
            NSOpenGLPFAColorSize, 24,
            NSOpenGLPFAAlphaSize, 8,
            NSOpenGLPFADepthSize, 24,
            NSOpenGLPFADoubleBuffer,
            NSOpenGLPFAAccelerated,
            0
        };

        NSOpenGLPixelFormat *pf = [[NSOpenGLPixelFormat alloc] initWithAttributes:attrs];
        VD_FW_G.gl_context = [[NSOpenGLContext alloc] initWithFormat:pf shareContext:nil];
        [VD_FW_G.gl_context setView:[VD_FW_G.window contentView]];
        [VD_FW_G.gl_context makeCurrentContext];

        VdFwWindowDelegate *delegate = [[VdFwWindowDelegate alloc] init];
        [VD_FW_G.window setDelegate:delegate];
    }

    VdFwGlVersion version = VD_FW_GL_VERSION_3_3;
    if (info && info->gl.version != 0) {
        version = info->gl.version;
    }
    vd_fw__load_opengl(version);

    mach_timebase_info(&VD_FW_G.time_base);
    VD_FW_G.last_time = mach_absolute_time();

    return 1;
}

unsigned long long vd_fw_delta_ns(void)
{
    uint64_t now = mach_absolute_time();
    uint64_t ns = (now - VD_FW_G.last_time) * VD_FW_G.time_base.numer / VD_FW_G.time_base.denom;
    return ns;
}

int vd_fw_get_mouse_state(int *x, int *y)
{
    @autoreleasepool {
        if (!VD_FW_G.window) {
            if (x) *x = 0;
            if (y) *y = 0;
            return 0;
        }

        // Mouse location in screen coordinates
        NSPoint loc = [NSEvent mouseLocation];

        // Convert to window coordinates
        NSPoint windowPoint = [VD_FW_G.window convertPointFromScreen:loc];

        // Convert to content view coordinates
        NSView *cv = [VD_FW_G.window contentView];
        NSPoint viewPoint = [cv convertPoint:windowPoint fromView:nil];
        NSRect cvf = [cv frame];

        // Cocoa’s origin is bottom-left, OpenGL expects same
        if (x) *x = (viewPoint.x) * VD_FW_G.scale;
        if (y) *y = (cvf.size.height - viewPoint.y) * VD_FW_G.scale;
    }

    return 0;
}

int vd_fw_running(void)
{
    @autoreleasepool {
        NSEvent *event;
        while ((event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                           untilDate:[NSDate distantPast]
                                              inMode:NSDefaultRunLoopMode
                                             dequeue:YES])) {
            [NSApp sendEvent:event];
        }
    }

    return !VD_FW_G.should_close;
}

int vd_fw_swap_buffers(void)
{
    @autoreleasepool {
        [VD_FW_G.gl_context flushBuffer];
    }

    return 1;
}

int vd_fw_get_size(int *w, int *h)
{
    NSRect rect = [[VD_FW_G.window contentView] frame];
    if (w) *w = (int)rect.size.width * VD_FW_G.scale;
    if (h) *h = (int)rect.size.height * VD_FW_G.scale;
    return 0;
}

int vd_fw_set_vsync_on(int on)
{
    @autoreleasepool {
        if (VD_FW_G.gl_context) {
            GLint sync = on;
            [VD_FW_G.gl_context setValues:&sync forParameter:NSOpenGLCPSwapInterval];
        }
    }
    return 1;
}

static void *vd_fw__gl_get_proc_address(const char *name)
{
    return 0;
}

#undef VD_FW_G
#endif // _WIN32, __APPLE__

#if !defined(__APPLE__)
/* ----GL VERSION 1.2------------------------------------------------------------------------------------------------ */
PFNGLDRAWRANGEELEMENTSPROC       glDrawRangeElements;
PFNGLTEXIMAGE3DPROC              glTexImage3D;
PFNGLTEXSUBIMAGE3DPROC           glTexSubImage3D;
PFNGLCOPYTEXSUBIMAGE3DPROC       glCopyTexSubImage3D;
/* ----GL VERSION 1.3------------------------------------------------------------------------------------------------ */
PFNGLACTIVETEXTUREPROC           glActiveTexture;
PFNGLSAMPLECOVERAGEPROC          glSampleCoverage;
PFNGLCOMPRESSEDTEXIMAGE3DPROC    glCompressedTexImage3D;
PFNGLCOMPRESSEDTEXIMAGE2DPROC    glCompressedTexImage2D;
PFNGLCOMPRESSEDTEXIMAGE1DPROC    glCompressedTexImage1D;
PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC glCompressedTexSubImage3D;
PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC glCompressedTexSubImage2D;
PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC glCompressedTexSubImage1D;
PFNGLGETCOMPRESSEDTEXIMAGEPROC   glGetCompressedTexImage;
/* ----GL VERSION 1.4------------------------------------------------------------------------------------------------ */
PFNGLBLENDFUNCSEPARATEPROC glBlendFuncSeparate;
PFNGLMULTIDRAWARRAYSPROC   glMultiDrawArrays;
PFNGLMULTIDRAWELEMENTSPROC glMultiDrawElements;
PFNGLPOINTPARAMETERFPROC   glPointParameterf;
PFNGLPOINTPARAMETERFVPROC  glPointParameterfv;
PFNGLPOINTPARAMETERIPROC   glPointParameteri;
PFNGLPOINTPARAMETERIVPROC  glPointParameteriv;
PFNGLBLENDCOLORPROC        glBlendColor;
PFNGLBLENDEQUATIONPROC     glBlendEquation;
/* ----GL VERSION 1.5------------------------------------------------------------------------------------------------ */
PFNGLGENQUERIESPROC           glGenQueries;
PFNGLDELETEQUERIESPROC        glDeleteQueries;
PFNGLISQUERYPROC              glIsQuery;
PFNGLBEGINQUERYPROC           glBeginQuery;
PFNGLENDQUERYPROC             glEndQuery;
PFNGLGETQUERYIVPROC           glGetQueryiv;
PFNGLGETQUERYOBJECTIVPROC     glGetQueryObjectiv;
PFNGLGETQUERYOBJECTUIVPROC    glGetQueryObjectuiv;
PFNGLBINDBUFFERPROC           glBindBuffer;
PFNGLDELETEBUFFERSPROC        glDeleteBuffers;
PFNGLGENBUFFERSPROC           glGenBuffers;
PFNGLISBUFFERPROC             glIsBuffer;
PFNGLBUFFERDATAPROC           glBufferData;
PFNGLBUFFERSUBDATAPROC        glBufferSubData;
PFNGLGETBUFFERSUBDATAPROC     glGetBufferSubData;
PFNGLMAPBUFFERPROC            glMapBuffer;
PFNGLUNMAPBUFFERPROC          glUnmapBuffer;
PFNGLGETBUFFERPARAMETERIVPROC glGetBufferParameteriv;
PFNGLGETBUFFERPOINTERVPROC    glGetBufferPointerv;
/* ----GL VERSION 2.0------------------------------------------------------------------------------------------------ */
PFNGLBLENDEQUATIONSEPARATEPROC    glBlendEquationSeparate;
PFNGLDRAWBUFFERSPROC              glDrawBuffers;
PFNGLSTENCILOPSEPARATEPROC        glStencilOpSeparate;
PFNGLSTENCILFUNCSEPARATEPROC      glStencilFuncSeparate;
PFNGLSTENCILMASKSEPARATEPROC      glStencilMaskSeparate;
PFNGLATTACHSHADERPROC             glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC       glBindAttribLocation;
PFNGLCOMPILESHADERPROC            glCompileShader;
PFNGLCREATEPROGRAMPROC            glCreateProgram;
PFNGLCREATESHADERPROC             glCreateShader;
PFNGLDELETEPROGRAMPROC            glDeleteProgram;
PFNGLDELETESHADERPROC             glDeleteShader;
PFNGLDETACHSHADERPROC             glDetachShader;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray;
PFNGLGETACTIVEATTRIBPROC          glGetActiveAttrib;
PFNGLGETACTIVEUNIFORMPROC         glGetActiveUniform;
PFNGLGETATTACHEDSHADERSPROC       glGetAttachedShaders;
PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation;
PFNGLGETPROGRAMIVPROC             glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC        glGetProgramInfoLog;
PFNGLGETSHADERIVPROC              glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC         glGetShaderInfoLog;
PFNGLGETSHADERSOURCEPROC          glGetShaderSource;
PFNGLGETUNIFORMLOCATIONPROC       glGetUniformLocation;
PFNGLGETUNIFORMFVPROC             glGetUniformfv;
PFNGLGETUNIFORMIVPROC             glGetUniformiv;
PFNGLGETVERTEXATTRIBDVPROC        glGetVertexAttribdv;
PFNGLGETVERTEXATTRIBFVPROC        glGetVertexAttribfv;
PFNGLGETVERTEXATTRIBIVPROC        glGetVertexAttribiv;
PFNGLGETVERTEXATTRIBPOINTERVPROC  glGetVertexAttribPointerv;
PFNGLISPROGRAMPROC                glIsProgram;
PFNGLISSHADERPROC                 glIsShader;
PFNGLLINKPROGRAMPROC              glLinkProgram;
PFNGLSHADERSOURCEPROC             glShaderSource;
PFNGLUSEPROGRAMPROC               glUseProgram;
PFNGLUNIFORM1FPROC                glUniform1f;
PFNGLUNIFORM2FPROC                glUniform2f;
PFNGLUNIFORM3FPROC                glUniform3f;
PFNGLUNIFORM4FPROC                glUniform4f;
PFNGLUNIFORM1IPROC                glUniform1i;
PFNGLUNIFORM2IPROC                glUniform2i;
PFNGLUNIFORM3IPROC                glUniform3i;
PFNGLUNIFORM4IPROC                glUniform4i;
PFNGLUNIFORM1FVPROC               glUniform1fv;
PFNGLUNIFORM2FVPROC               glUniform2fv;
PFNGLUNIFORM3FVPROC               glUniform3fv;
PFNGLUNIFORM4FVPROC               glUniform4fv;
PFNGLUNIFORM1IVPROC               glUniform1iv;
PFNGLUNIFORM2IVPROC               glUniform2iv;
PFNGLUNIFORM3IVPROC               glUniform3iv;
PFNGLUNIFORM4IVPROC               glUniform4iv;
PFNGLUNIFORMMATRIX2FVPROC         glUniformMatrix2fv;
PFNGLUNIFORMMATRIX3FVPROC         glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC         glUniformMatrix4fv;
PFNGLVALIDATEPROGRAMPROC          glValidateProgram;
PFNGLVERTEXATTRIB1DPROC           glVertexAttrib1d;
PFNGLVERTEXATTRIB1DVPROC          glVertexAttrib1dv;
PFNGLVERTEXATTRIB1FPROC           glVertexAttrib1f;
PFNGLVERTEXATTRIB1FVPROC          glVertexAttrib1fv;
PFNGLVERTEXATTRIB1SPROC           glVertexAttrib1s;
PFNGLVERTEXATTRIB1SVPROC          glVertexAttrib1sv;
PFNGLVERTEXATTRIB2DPROC           glVertexAttrib2d;
PFNGLVERTEXATTRIB2DVPROC          glVertexAttrib2dv;
PFNGLVERTEXATTRIB2FPROC           glVertexAttrib2f;
PFNGLVERTEXATTRIB2FVPROC          glVertexAttrib2fv;
PFNGLVERTEXATTRIB2SPROC           glVertexAttrib2s;
PFNGLVERTEXATTRIB2SVPROC          glVertexAttrib2sv;
PFNGLVERTEXATTRIB3DPROC           glVertexAttrib3d;
PFNGLVERTEXATTRIB3DVPROC          glVertexAttrib3dv;
PFNGLVERTEXATTRIB3FPROC           glVertexAttrib3f;
PFNGLVERTEXATTRIB3FVPROC          glVertexAttrib3fv;
PFNGLVERTEXATTRIB3SPROC           glVertexAttrib3s;
PFNGLVERTEXATTRIB3SVPROC          glVertexAttrib3sv;
PFNGLVERTEXATTRIB4NBVPROC         glVertexAttrib4Nbv;
PFNGLVERTEXATTRIB4NIVPROC         glVertexAttrib4Niv;
PFNGLVERTEXATTRIB4NSVPROC         glVertexAttrib4Nsv;
PFNGLVERTEXATTRIB4NUBPROC         glVertexAttrib4Nub;
PFNGLVERTEXATTRIB4NUBVPROC        glVertexAttrib4Nubv;
PFNGLVERTEXATTRIB4NUIVPROC        glVertexAttrib4Nuiv;
PFNGLVERTEXATTRIB4NUSVPROC        glVertexAttrib4Nusv;
PFNGLVERTEXATTRIB4BVPROC          glVertexAttrib4bv;
PFNGLVERTEXATTRIB4DPROC           glVertexAttrib4d;
PFNGLVERTEXATTRIB4DVPROC          glVertexAttrib4dv;
PFNGLVERTEXATTRIB4FPROC           glVertexAttrib4f;
PFNGLVERTEXATTRIB4FVPROC          glVertexAttrib4fv;
PFNGLVERTEXATTRIB4IVPROC          glVertexAttrib4iv;
PFNGLVERTEXATTRIB4SPROC           glVertexAttrib4s;
PFNGLVERTEXATTRIB4SVPROC          glVertexAttrib4sv;
PFNGLVERTEXATTRIB4UBVPROC         glVertexAttrib4ubv;
PFNGLVERTEXATTRIB4UIVPROC         glVertexAttrib4uiv;
PFNGLVERTEXATTRIB4USVPROC         glVertexAttrib4usv;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer;
/* ----GL VERSION 2.1------------------------------------------------------------------------------------------------ */
PFNGLUNIFORMMATRIX2X3FVPROC glUniformMatrix2x3fv;
PFNGLUNIFORMMATRIX3X2FVPROC glUniformMatrix3x2fv;
PFNGLUNIFORMMATRIX2X4FVPROC glUniformMatrix2x4fv;
PFNGLUNIFORMMATRIX4X2FVPROC glUniformMatrix4x2fv;
PFNGLUNIFORMMATRIX3X4FVPROC glUniformMatrix3x4fv;
PFNGLUNIFORMMATRIX4X3FVPROC glUniformMatrix4x3fv;
/* ----GL VERSION 3.0------------------------------------------------------------------------------------------------ */
PFNGLCOLORMASKIPROC                          glColorMaski;
PFNGLGETBOOLEANI_VPROC                       glGetBooleani_v;
PFNGLGETINTEGERI_VPROC                       glGetIntegeri_v;
PFNGLENABLEIPROC                             glEnablei;
PFNGLDISABLEIPROC                            glDisablei;
PFNGLISENABLEDIPROC                          glIsEnabledi;
PFNGLBEGINTRANSFORMFEEDBACKPROC              glBeginTransformFeedback;
PFNGLENDTRANSFORMFEEDBACKPROC                glEndTransformFeedback;
PFNGLBINDBUFFERRANGEPROC                     glBindBufferRange;
PFNGLBINDBUFFERBASEPROC                      glBindBufferBase;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC           glTransformFeedbackVaryings;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC         glGetTransformFeedbackVarying;
PFNGLCLAMPCOLORPROC                          glClampColor;
PFNGLBEGINCONDITIONALRENDERPROC              glBeginConditionalRender;
PFNGLENDCONDITIONALRENDERPROC                glEndConditionalRender;
PFNGLVERTEXATTRIBIPOINTERPROC                glVertexAttribIPointer;
PFNGLGETVERTEXATTRIBIIVPROC                  glGetVertexAttribIiv;
PFNGLGETVERTEXATTRIBIUIVPROC                 glGetVertexAttribIuiv;
PFNGLVERTEXATTRIBI1IPROC                     glVertexAttribI1i;
PFNGLVERTEXATTRIBI2IPROC                     glVertexAttribI2i;
PFNGLVERTEXATTRIBI3IPROC                     glVertexAttribI3i;
PFNGLVERTEXATTRIBI4IPROC                     glVertexAttribI4i;
PFNGLVERTEXATTRIBI1UIPROC                    glVertexAttribI1ui;
PFNGLVERTEXATTRIBI2UIPROC                    glVertexAttribI2ui;
PFNGLVERTEXATTRIBI3UIPROC                    glVertexAttribI3ui;
PFNGLVERTEXATTRIBI4UIPROC                    glVertexAttribI4ui;
PFNGLVERTEXATTRIBI1IVPROC                    glVertexAttribI1iv;
PFNGLVERTEXATTRIBI2IVPROC                    glVertexAttribI2iv;
PFNGLVERTEXATTRIBI3IVPROC                    glVertexAttribI3iv;
PFNGLVERTEXATTRIBI4IVPROC                    glVertexAttribI4iv;
PFNGLVERTEXATTRIBI1UIVPROC                   glVertexAttribI1uiv;
PFNGLVERTEXATTRIBI2UIVPROC                   glVertexAttribI2uiv;
PFNGLVERTEXATTRIBI3UIVPROC                   glVertexAttribI3uiv;
PFNGLVERTEXATTRIBI4UIVPROC                   glVertexAttribI4uiv;
PFNGLVERTEXATTRIBI4BVPROC                    glVertexAttribI4bv;
PFNGLVERTEXATTRIBI4SVPROC                    glVertexAttribI4sv;
PFNGLVERTEXATTRIBI4UBVPROC                   glVertexAttribI4ubv;
PFNGLVERTEXATTRIBI4USVPROC                   glVertexAttribI4usv;
PFNGLGETUNIFORMUIVPROC                       glGetUniformuiv;
PFNGLBINDFRAGDATALOCATIONPROC                glBindFragDataLocation;
PFNGLGETFRAGDATALOCATIONPROC                 glGetFragDataLocation;
PFNGLUNIFORM1UIPROC                          glUniform1ui;
PFNGLUNIFORM2UIPROC                          glUniform2ui;
PFNGLUNIFORM3UIPROC                          glUniform3ui;
PFNGLUNIFORM4UIPROC                          glUniform4ui;
PFNGLUNIFORM1UIVPROC                         glUniform1uiv;
PFNGLUNIFORM2UIVPROC                         glUniform2uiv;
PFNGLUNIFORM3UIVPROC                         glUniform3uiv;
PFNGLUNIFORM4UIVPROC                         glUniform4uiv;
PFNGLTEXPARAMETERIIVPROC                     glTexParameterIiv;
PFNGLTEXPARAMETERIUIVPROC                    glTexParameterIuiv;
PFNGLGETTEXPARAMETERIIVPROC                  glGetTexParameterIiv;
PFNGLGETTEXPARAMETERIUIVPROC                 glGetTexParameterIuiv;
PFNGLCLEARBUFFERIVPROC                       glClearBufferiv;
PFNGLCLEARBUFFERUIVPROC                      glClearBufferuiv;
PFNGLCLEARBUFFERFVPROC                       glClearBufferfv;
PFNGLCLEARBUFFERFIPROC                       glClearBufferfi;
PFNGLGETSTRINGIPROC                          glGetStringi;
PFNGLISRENDERBUFFERPROC                      glIsRenderbuffer;
PFNGLBINDRENDERBUFFERPROC                    glBindRenderbuffer;
PFNGLDELETERENDERBUFFERSPROC                 glDeleteRenderbuffers;
PFNGLGENRENDERBUFFERSPROC                    glGenRenderbuffers;
PFNGLRENDERBUFFERSTORAGEPROC                 glRenderbufferStorage;
PFNGLGETRENDERBUFFERPARAMETERIVPROC          glGetRenderbufferParameteriv;
PFNGLISFRAMEBUFFERPROC                       glIsFramebuffer;
PFNGLBINDFRAMEBUFFERPROC                     glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC                  glDeleteFramebuffers;
PFNGLGENFRAMEBUFFERSPROC                     glGenFramebuffers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC              glCheckFramebufferStatus;
PFNGLFRAMEBUFFERTEXTURE1DPROC                glFramebufferTexture1D;
PFNGLFRAMEBUFFERTEXTURE2DPROC                glFramebufferTexture2D;
PFNGLFRAMEBUFFERTEXTURE3DPROC                glFramebufferTexture3D;
PFNGLFRAMEBUFFERRENDERBUFFERPROC             glFramebufferRenderbuffer;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
PFNGLGENERATEMIPMAPPROC                      glGenerateMipmap;
PFNGLBLITFRAMEBUFFERPROC                     glBlitFramebuffer;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC      glRenderbufferStorageMultisample;
PFNGLFRAMEBUFFERTEXTURELAYERPROC             glFramebufferTextureLayer;
PFNGLMAPBUFFERRANGEPROC                      glMapBufferRange;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC              glFlushMappedBufferRange;
PFNGLBINDVERTEXARRAYPROC                     glBindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC                  glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC                     glGenVertexArrays;
PFNGLISVERTEXARRAYPROC                       glIsVertexArray;
/* ----GL VERSION 3.1------------------------------------------------------------------------------------------------ */
PFNGLDRAWARRAYSINSTANCEDPROC       glDrawArraysInstanced;
PFNGLDRAWELEMENTSINSTANCEDPROC     glDrawElementsInstanced;
PFNGLTEXBUFFERPROC                 glTexBuffer;
PFNGLPRIMITIVERESTARTINDEXPROC     glPrimitiveRestartIndex;
PFNGLCOPYBUFFERSUBDATAPROC         glCopyBufferSubData;
PFNGLGETUNIFORMINDICESPROC         glGetUniformIndices;
PFNGLGETACTIVEUNIFORMSIVPROC       glGetActiveUniformsiv;
PFNGLGETACTIVEUNIFORMNAMEPROC      glGetActiveUniformName;
PFNGLGETUNIFORMBLOCKINDEXPROC      glGetUniformBlockIndex;
PFNGLGETACTIVEUNIFORMBLOCKIVPROC   glGetActiveUniformBlockiv;
PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC glGetActiveUniformBlockName;
PFNGLUNIFORMBLOCKBINDINGPROC       glUniformBlockBinding;
/* ----GL VERSION 3.2------------------------------------------------------------------------------------------------ */
PFNGLDRAWELEMENTSBASEVERTEXPROC          glDrawElementsBaseVertex;
PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC     glDrawRangeElementsBaseVertex;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC glDrawElementsInstancedBaseVertex;
PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC     glMultiDrawElementsBaseVertex;
PFNGLPROVOKINGVERTEXPROC                 glProvokingVertex;
PFNGLFENCESYNCPROC                       glFenceSync;
PFNGLISSYNCPROC                          glIsSync;
PFNGLDELETESYNCPROC                      glDeleteSync;
PFNGLCLIENTWAITSYNCPROC                  glClientWaitSync;
PFNGLWAITSYNCPROC                        glWaitSync;
PFNGLGETINTEGER64VPROC                   glGetInteger64v;
PFNGLGETSYNCIVPROC                       glGetSynciv;
PFNGLGETINTEGER64I_VPROC                 glGetInteger64i_v;
PFNGLGETBUFFERPARAMETERI64VPROC          glGetBufferParameteri64v;
PFNGLFRAMEBUFFERTEXTUREPROC              glFramebufferTexture;
PFNGLTEXIMAGE2DMULTISAMPLEPROC           glTexImage2DMultisample;
PFNGLTEXIMAGE3DMULTISAMPLEPROC           glTexImage3DMultisample;
PFNGLGETMULTISAMPLEFVPROC                glGetMultisamplefv;
PFNGLSAMPLEMASKIPROC                     glSampleMaski;
/* ----GL VERSION 3.3------------------------------------------------------------------------------------------------ */
PFNGLBINDFRAGDATALOCATIONINDEXEDPROC glBindFragDataLocationIndexed;
PFNGLGETFRAGDATAINDEXPROC            glGetFragDataIndex;
PFNGLGENSAMPLERSPROC                 glGenSamplers;
PFNGLDELETESAMPLERSPROC              glDeleteSamplers;
PFNGLISSAMPLERPROC                   glIsSampler;
PFNGLBINDSAMPLERPROC                 glBindSampler;
PFNGLSAMPLERPARAMETERIPROC           glSamplerParameteri;
PFNGLSAMPLERPARAMETERIVPROC          glSamplerParameteriv;
PFNGLSAMPLERPARAMETERFPROC           glSamplerParameterf;
PFNGLSAMPLERPARAMETERFVPROC          glSamplerParameterfv;
PFNGLSAMPLERPARAMETERIIVPROC         glSamplerParameterIiv;
PFNGLSAMPLERPARAMETERIUIVPROC        glSamplerParameterIuiv;
PFNGLGETSAMPLERPARAMETERIVPROC       glGetSamplerParameteriv;
PFNGLGETSAMPLERPARAMETERIIVPROC      glGetSamplerParameterIiv;
PFNGLGETSAMPLERPARAMETERFVPROC       glGetSamplerParameterfv;
PFNGLGETSAMPLERPARAMETERIUIVPROC     glGetSamplerParameterIuiv;
PFNGLQUERYCOUNTERPROC                glQueryCounter;
PFNGLGETQUERYOBJECTI64VPROC          glGetQueryObjecti64v;
PFNGLGETQUERYOBJECTUI64VPROC         glGetQueryObjectui64v;
PFNGLVERTEXATTRIBDIVISORPROC         glVertexAttribDivisor;
PFNGLVERTEXATTRIBP1UIPROC            glVertexAttribP1ui;
PFNGLVERTEXATTRIBP1UIVPROC           glVertexAttribP1uiv;
PFNGLVERTEXATTRIBP2UIPROC            glVertexAttribP2ui;
PFNGLVERTEXATTRIBP2UIVPROC           glVertexAttribP2uiv;
PFNGLVERTEXATTRIBP3UIPROC            glVertexAttribP3ui;
PFNGLVERTEXATTRIBP3UIVPROC           glVertexAttribP3uiv;
PFNGLVERTEXATTRIBP4UIPROC            glVertexAttribP4ui;
PFNGLVERTEXATTRIBP4UIVPROC           glVertexAttribP4uiv;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
#endif // !defined(__APPLE__)

static void vd_fw__load_opengl(VdFwGlVersion version)
{
#if defined(__APPLE__)
    // @todo(mdodis): This check
    // if (version > VD_FW_GL_VERION_4_1) {

    // }
#else
#define LOAD(p, s) s = (p)vd_fw__gl_get_proc_address(#s)

    if (version >= VD_FW_GL_VERSION_1_2) {
        LOAD(PFNGLDRAWRANGEELEMENTSPROC, glDrawRangeElements);
        LOAD(PFNGLTEXIMAGE3DPROC,        glTexImage3D);
        LOAD(PFNGLTEXSUBIMAGE3DPROC,     glTexSubImage3D);
        LOAD(PFNGLCOPYTEXSUBIMAGE3DPROC, glCopyTexSubImage3D);
    }

    if (version >= VD_FW_GL_VERSION_1_3) {
        LOAD(PFNGLACTIVETEXTUREPROC,           glActiveTexture);
        LOAD(PFNGLSAMPLECOVERAGEPROC,          glSampleCoverage);
        LOAD(PFNGLCOMPRESSEDTEXIMAGE3DPROC,    glCompressedTexImage3D);
        LOAD(PFNGLCOMPRESSEDTEXIMAGE2DPROC,    glCompressedTexImage2D);
        LOAD(PFNGLCOMPRESSEDTEXIMAGE1DPROC,    glCompressedTexImage1D);
        LOAD(PFNGLCOMPRESSEDTEXSUBIMAGE3DPROC, glCompressedTexSubImage3D);
        LOAD(PFNGLCOMPRESSEDTEXSUBIMAGE2DPROC, glCompressedTexSubImage2D);
        LOAD(PFNGLCOMPRESSEDTEXSUBIMAGE1DPROC, glCompressedTexSubImage1D);
        LOAD(PFNGLGETCOMPRESSEDTEXIMAGEPROC,   glGetCompressedTexImage);
    }

    if (version >= VD_FW_GL_VERSION_1_4) {
        LOAD(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate);
        LOAD(PFNGLMULTIDRAWARRAYSPROC,   glMultiDrawArrays);
        LOAD(PFNGLMULTIDRAWELEMENTSPROC, glMultiDrawElements);
        LOAD(PFNGLPOINTPARAMETERFPROC,   glPointParameterf);
        LOAD(PFNGLPOINTPARAMETERFVPROC,  glPointParameterfv);
        LOAD(PFNGLPOINTPARAMETERIPROC,   glPointParameteri);
        LOAD(PFNGLPOINTPARAMETERIVPROC,  glPointParameteriv);
        LOAD(PFNGLBLENDCOLORPROC,        glBlendColor);
        LOAD(PFNGLBLENDEQUATIONPROC,     glBlendEquation);
    }

    if (version >= VD_FW_GL_VERSION_1_5) {
        LOAD(PFNGLGENQUERIESPROC,           glGenQueries);
        LOAD(PFNGLDELETEQUERIESPROC,        glDeleteQueries);
        LOAD(PFNGLISQUERYPROC,              glIsQuery);
        LOAD(PFNGLBEGINQUERYPROC,           glBeginQuery);
        LOAD(PFNGLENDQUERYPROC,             glEndQuery);
        LOAD(PFNGLGETQUERYIVPROC,           glGetQueryiv);
        LOAD(PFNGLGETQUERYOBJECTIVPROC,     glGetQueryObjectiv);
        LOAD(PFNGLGETQUERYOBJECTUIVPROC,    glGetQueryObjectuiv);
        LOAD(PFNGLBINDBUFFERPROC,           glBindBuffer);
        LOAD(PFNGLDELETEBUFFERSPROC,        glDeleteBuffers);
        LOAD(PFNGLGENBUFFERSPROC,           glGenBuffers);
        LOAD(PFNGLISBUFFERPROC,             glIsBuffer);
        LOAD(PFNGLBUFFERDATAPROC,           glBufferData);
        LOAD(PFNGLBUFFERSUBDATAPROC,        glBufferSubData);
        LOAD(PFNGLGETBUFFERSUBDATAPROC,     glGetBufferSubData);
        LOAD(PFNGLMAPBUFFERPROC,            glMapBuffer);
        LOAD(PFNGLUNMAPBUFFERPROC,          glUnmapBuffer);
        LOAD(PFNGLGETBUFFERPARAMETERIVPROC, glGetBufferParameteriv);
        LOAD(PFNGLGETBUFFERPOINTERVPROC,    glGetBufferPointerv);
    }

    if (version >= VD_FW_GL_VERSION_2_0) {
        LOAD(PFNGLBLENDEQUATIONSEPARATEPROC,    glBlendEquationSeparate);
        LOAD(PFNGLDRAWBUFFERSPROC,              glDrawBuffers);
        LOAD(PFNGLSTENCILOPSEPARATEPROC,        glStencilOpSeparate);
        LOAD(PFNGLSTENCILFUNCSEPARATEPROC,      glStencilFuncSeparate);
        LOAD(PFNGLSTENCILMASKSEPARATEPROC,      glStencilMaskSeparate);
        LOAD(PFNGLATTACHSHADERPROC,             glAttachShader);
        LOAD(PFNGLBINDATTRIBLOCATIONPROC,       glBindAttribLocation);
        LOAD(PFNGLCOMPILESHADERPROC,            glCompileShader);
        LOAD(PFNGLCREATEPROGRAMPROC,            glCreateProgram);
        LOAD(PFNGLCREATESHADERPROC,             glCreateShader);
        LOAD(PFNGLDELETEPROGRAMPROC,            glDeleteProgram);
        LOAD(PFNGLDELETESHADERPROC,             glDeleteShader);
        LOAD(PFNGLDETACHSHADERPROC,             glDetachShader);
        LOAD(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
        LOAD(PFNGLENABLEVERTEXATTRIBARRAYPROC,  glEnableVertexAttribArray);
        LOAD(PFNGLGETACTIVEATTRIBPROC,          glGetActiveAttrib);
        LOAD(PFNGLGETACTIVEUNIFORMPROC,         glGetActiveUniform);
        LOAD(PFNGLGETATTACHEDSHADERSPROC,       glGetAttachedShaders);
        LOAD(PFNGLGETATTRIBLOCATIONPROC,        glGetAttribLocation);
        LOAD(PFNGLGETPROGRAMIVPROC,             glGetProgramiv);
        LOAD(PFNGLGETPROGRAMINFOLOGPROC,        glGetProgramInfoLog);
        LOAD(PFNGLGETSHADERIVPROC,              glGetShaderiv);
        LOAD(PFNGLGETSHADERINFOLOGPROC,         glGetShaderInfoLog);
        LOAD(PFNGLGETSHADERSOURCEPROC,          glGetShaderSource);
        LOAD(PFNGLGETUNIFORMLOCATIONPROC,       glGetUniformLocation);
        LOAD(PFNGLGETUNIFORMFVPROC,             glGetUniformfv);
        LOAD(PFNGLGETUNIFORMIVPROC,             glGetUniformiv);
        LOAD(PFNGLGETVERTEXATTRIBDVPROC,        glGetVertexAttribdv);
        LOAD(PFNGLGETVERTEXATTRIBFVPROC,        glGetVertexAttribfv);
        LOAD(PFNGLGETVERTEXATTRIBIVPROC,        glGetVertexAttribiv);
        LOAD(PFNGLGETVERTEXATTRIBPOINTERVPROC,  glGetVertexAttribPointerv);
        LOAD(PFNGLISPROGRAMPROC,                glIsProgram);
        LOAD(PFNGLISSHADERPROC,                 glIsShader);
        LOAD(PFNGLLINKPROGRAMPROC,              glLinkProgram);
        LOAD(PFNGLSHADERSOURCEPROC,             glShaderSource);
        LOAD(PFNGLUSEPROGRAMPROC,               glUseProgram);
        LOAD(PFNGLUNIFORM1FPROC,                glUniform1f);
        LOAD(PFNGLUNIFORM2FPROC,                glUniform2f);
        LOAD(PFNGLUNIFORM3FPROC,                glUniform3f);
        LOAD(PFNGLUNIFORM4FPROC,                glUniform4f);
        LOAD(PFNGLUNIFORM1IPROC,                glUniform1i);
        LOAD(PFNGLUNIFORM2IPROC,                glUniform2i);
        LOAD(PFNGLUNIFORM3IPROC,                glUniform3i);
        LOAD(PFNGLUNIFORM4IPROC,                glUniform4i);
        LOAD(PFNGLUNIFORM1FVPROC,               glUniform1fv);
        LOAD(PFNGLUNIFORM2FVPROC,               glUniform2fv);
        LOAD(PFNGLUNIFORM3FVPROC,               glUniform3fv);
        LOAD(PFNGLUNIFORM4FVPROC,               glUniform4fv);
        LOAD(PFNGLUNIFORM1IVPROC,               glUniform1iv);
        LOAD(PFNGLUNIFORM2IVPROC,               glUniform2iv);
        LOAD(PFNGLUNIFORM3IVPROC,               glUniform3iv);
        LOAD(PFNGLUNIFORM4IVPROC,               glUniform4iv);
        LOAD(PFNGLUNIFORMMATRIX2FVPROC,         glUniformMatrix2fv);
        LOAD(PFNGLUNIFORMMATRIX3FVPROC,         glUniformMatrix3fv);
        LOAD(PFNGLUNIFORMMATRIX4FVPROC,         glUniformMatrix4fv);
        LOAD(PFNGLVALIDATEPROGRAMPROC,          glValidateProgram);
        LOAD(PFNGLVERTEXATTRIB1DPROC,           glVertexAttrib1d);
        LOAD(PFNGLVERTEXATTRIB1DVPROC,          glVertexAttrib1dv);
        LOAD(PFNGLVERTEXATTRIB1FPROC,           glVertexAttrib1f);
        LOAD(PFNGLVERTEXATTRIB1FVPROC,          glVertexAttrib1fv);
        LOAD(PFNGLVERTEXATTRIB1SPROC,           glVertexAttrib1s);
        LOAD(PFNGLVERTEXATTRIB1SVPROC,          glVertexAttrib1sv);
        LOAD(PFNGLVERTEXATTRIB2DPROC,           glVertexAttrib2d);
        LOAD(PFNGLVERTEXATTRIB2DVPROC,          glVertexAttrib2dv);
        LOAD(PFNGLVERTEXATTRIB2FPROC,           glVertexAttrib2f);
        LOAD(PFNGLVERTEXATTRIB2FVPROC,          glVertexAttrib2fv);
        LOAD(PFNGLVERTEXATTRIB2SPROC,           glVertexAttrib2s);
        LOAD(PFNGLVERTEXATTRIB2SVPROC,          glVertexAttrib2sv);
        LOAD(PFNGLVERTEXATTRIB3DPROC,           glVertexAttrib3d);
        LOAD(PFNGLVERTEXATTRIB3DVPROC,          glVertexAttrib3dv);
        LOAD(PFNGLVERTEXATTRIB3FPROC,           glVertexAttrib3f);
        LOAD(PFNGLVERTEXATTRIB3FVPROC,          glVertexAttrib3fv);
        LOAD(PFNGLVERTEXATTRIB3SPROC,           glVertexAttrib3s);
        LOAD(PFNGLVERTEXATTRIB3SVPROC,          glVertexAttrib3sv);
        LOAD(PFNGLVERTEXATTRIB4NBVPROC,         glVertexAttrib4Nbv);
        LOAD(PFNGLVERTEXATTRIB4NIVPROC,         glVertexAttrib4Niv);
        LOAD(PFNGLVERTEXATTRIB4NSVPROC,         glVertexAttrib4Nsv);
        LOAD(PFNGLVERTEXATTRIB4NUBPROC,         glVertexAttrib4Nub);
        LOAD(PFNGLVERTEXATTRIB4NUBVPROC,        glVertexAttrib4Nubv);
        LOAD(PFNGLVERTEXATTRIB4NUIVPROC,        glVertexAttrib4Nuiv);
        LOAD(PFNGLVERTEXATTRIB4NUSVPROC,        glVertexAttrib4Nusv);
        LOAD(PFNGLVERTEXATTRIB4BVPROC,          glVertexAttrib4bv);
        LOAD(PFNGLVERTEXATTRIB4DPROC,           glVertexAttrib4d);
        LOAD(PFNGLVERTEXATTRIB4DVPROC,          glVertexAttrib4dv);
        LOAD(PFNGLVERTEXATTRIB4FPROC,           glVertexAttrib4f);
        LOAD(PFNGLVERTEXATTRIB4FVPROC,          glVertexAttrib4fv);
        LOAD(PFNGLVERTEXATTRIB4IVPROC,          glVertexAttrib4iv);
        LOAD(PFNGLVERTEXATTRIB4SPROC,           glVertexAttrib4s);
        LOAD(PFNGLVERTEXATTRIB4SVPROC,          glVertexAttrib4sv);
        LOAD(PFNGLVERTEXATTRIB4UBVPROC,         glVertexAttrib4ubv);
        LOAD(PFNGLVERTEXATTRIB4UIVPROC,         glVertexAttrib4uiv);
        LOAD(PFNGLVERTEXATTRIB4USVPROC,         glVertexAttrib4usv);
        LOAD(PFNGLVERTEXATTRIBPOINTERPROC,      glVertexAttribPointer);
    }

    if (version >= VD_FW_GL_VERSION_2_1) {
        LOAD(PFNGLUNIFORMMATRIX2X3FVPROC, glUniformMatrix2x3fv);
        LOAD(PFNGLUNIFORMMATRIX3X2FVPROC, glUniformMatrix3x2fv);
        LOAD(PFNGLUNIFORMMATRIX2X4FVPROC, glUniformMatrix2x4fv);
        LOAD(PFNGLUNIFORMMATRIX4X2FVPROC, glUniformMatrix4x2fv);
        LOAD(PFNGLUNIFORMMATRIX3X4FVPROC, glUniformMatrix3x4fv);
        LOAD(PFNGLUNIFORMMATRIX4X3FVPROC, glUniformMatrix4x3fv);
    }

    if (version >= VD_FW_GL_VERSION_3_0) {
        LOAD(PFNGLCOLORMASKIPROC,                          glColorMaski);
        LOAD(PFNGLGETBOOLEANI_VPROC,                       glGetBooleani_v);
        LOAD(PFNGLGETINTEGERI_VPROC,                       glGetIntegeri_v);
        LOAD(PFNGLENABLEIPROC,                             glEnablei);
        LOAD(PFNGLDISABLEIPROC,                            glDisablei);
        LOAD(PFNGLISENABLEDIPROC,                          glIsEnabledi);
        LOAD(PFNGLBEGINTRANSFORMFEEDBACKPROC,              glBeginTransformFeedback);
        LOAD(PFNGLENDTRANSFORMFEEDBACKPROC,                glEndTransformFeedback);
        LOAD(PFNGLBINDBUFFERRANGEPROC,                     glBindBufferRange);
        LOAD(PFNGLBINDBUFFERBASEPROC,                      glBindBufferBase);
        LOAD(PFNGLTRANSFORMFEEDBACKVARYINGSPROC,           glTransformFeedbackVaryings);
        LOAD(PFNGLGETTRANSFORMFEEDBACKVARYINGPROC,         glGetTransformFeedbackVarying);
        LOAD(PFNGLCLAMPCOLORPROC,                          glClampColor);
        LOAD(PFNGLBEGINCONDITIONALRENDERPROC,              glBeginConditionalRender);
        LOAD(PFNGLENDCONDITIONALRENDERPROC,                glEndConditionalRender);
        LOAD(PFNGLVERTEXATTRIBIPOINTERPROC,                glVertexAttribIPointer);
        LOAD(PFNGLGETVERTEXATTRIBIIVPROC,                  glGetVertexAttribIiv);
        LOAD(PFNGLGETVERTEXATTRIBIUIVPROC,                 glGetVertexAttribIuiv);
        LOAD(PFNGLVERTEXATTRIBI1IPROC,                     glVertexAttribI1i);
        LOAD(PFNGLVERTEXATTRIBI2IPROC,                     glVertexAttribI2i);
        LOAD(PFNGLVERTEXATTRIBI3IPROC,                     glVertexAttribI3i);
        LOAD(PFNGLVERTEXATTRIBI4IPROC,                     glVertexAttribI4i);
        LOAD(PFNGLVERTEXATTRIBI1UIPROC,                    glVertexAttribI1ui);
        LOAD(PFNGLVERTEXATTRIBI2UIPROC,                    glVertexAttribI2ui);
        LOAD(PFNGLVERTEXATTRIBI3UIPROC,                    glVertexAttribI3ui);
        LOAD(PFNGLVERTEXATTRIBI4UIPROC,                    glVertexAttribI4ui);
        LOAD(PFNGLVERTEXATTRIBI1IVPROC,                    glVertexAttribI1iv);
        LOAD(PFNGLVERTEXATTRIBI2IVPROC,                    glVertexAttribI2iv);
        LOAD(PFNGLVERTEXATTRIBI3IVPROC,                    glVertexAttribI3iv);
        LOAD(PFNGLVERTEXATTRIBI4IVPROC,                    glVertexAttribI4iv);
        LOAD(PFNGLVERTEXATTRIBI1UIVPROC,                   glVertexAttribI1uiv);
        LOAD(PFNGLVERTEXATTRIBI2UIVPROC,                   glVertexAttribI2uiv);
        LOAD(PFNGLVERTEXATTRIBI3UIVPROC,                   glVertexAttribI3uiv);
        LOAD(PFNGLVERTEXATTRIBI4UIVPROC,                   glVertexAttribI4uiv);
        LOAD(PFNGLVERTEXATTRIBI4BVPROC,                    glVertexAttribI4bv);
        LOAD(PFNGLVERTEXATTRIBI4SVPROC,                    glVertexAttribI4sv);
        LOAD(PFNGLVERTEXATTRIBI4UBVPROC,                   glVertexAttribI4ubv);
        LOAD(PFNGLVERTEXATTRIBI4USVPROC,                   glVertexAttribI4usv);
        LOAD(PFNGLGETUNIFORMUIVPROC,                       glGetUniformuiv);
        LOAD(PFNGLBINDFRAGDATALOCATIONPROC,                glBindFragDataLocation);
        LOAD(PFNGLGETFRAGDATALOCATIONPROC,                 glGetFragDataLocation);
        LOAD(PFNGLUNIFORM1UIPROC,                          glUniform1ui);
        LOAD(PFNGLUNIFORM2UIPROC,                          glUniform2ui);
        LOAD(PFNGLUNIFORM3UIPROC,                          glUniform3ui);
        LOAD(PFNGLUNIFORM4UIPROC,                          glUniform4ui);
        LOAD(PFNGLUNIFORM1UIVPROC,                         glUniform1uiv);
        LOAD(PFNGLUNIFORM2UIVPROC,                         glUniform2uiv);
        LOAD(PFNGLUNIFORM3UIVPROC,                         glUniform3uiv);
        LOAD(PFNGLUNIFORM4UIVPROC,                         glUniform4uiv);
        LOAD(PFNGLTEXPARAMETERIIVPROC,                     glTexParameterIiv);
        LOAD(PFNGLTEXPARAMETERIUIVPROC,                    glTexParameterIuiv);
        LOAD(PFNGLGETTEXPARAMETERIIVPROC,                  glGetTexParameterIiv);
        LOAD(PFNGLGETTEXPARAMETERIUIVPROC,                 glGetTexParameterIuiv);
        LOAD(PFNGLCLEARBUFFERIVPROC,                       glClearBufferiv);
        LOAD(PFNGLCLEARBUFFERUIVPROC,                      glClearBufferuiv);
        LOAD(PFNGLCLEARBUFFERFVPROC,                       glClearBufferfv);
        LOAD(PFNGLCLEARBUFFERFIPROC,                       glClearBufferfi);
        LOAD(PFNGLGETSTRINGIPROC,                          glGetStringi);
        LOAD(PFNGLISRENDERBUFFERPROC,                      glIsRenderbuffer);
        LOAD(PFNGLBINDRENDERBUFFERPROC,                    glBindRenderbuffer);
        LOAD(PFNGLDELETERENDERBUFFERSPROC,                 glDeleteRenderbuffers);
        LOAD(PFNGLGENRENDERBUFFERSPROC,                    glGenRenderbuffers);
        LOAD(PFNGLRENDERBUFFERSTORAGEPROC,                 glRenderbufferStorage);
        LOAD(PFNGLGETRENDERBUFFERPARAMETERIVPROC,          glGetRenderbufferParameteriv);
        LOAD(PFNGLISFRAMEBUFFERPROC,                       glIsFramebuffer);
        LOAD(PFNGLBINDFRAMEBUFFERPROC,                     glBindFramebuffer);
        LOAD(PFNGLDELETEFRAMEBUFFERSPROC,                  glDeleteFramebuffers);
        LOAD(PFNGLGENFRAMEBUFFERSPROC,                     glGenFramebuffers);
        LOAD(PFNGLCHECKFRAMEBUFFERSTATUSPROC,              glCheckFramebufferStatus);
        LOAD(PFNGLFRAMEBUFFERTEXTURE1DPROC,                glFramebufferTexture1D);
        LOAD(PFNGLFRAMEBUFFERTEXTURE2DPROC,                glFramebufferTexture2D);
        LOAD(PFNGLFRAMEBUFFERTEXTURE3DPROC,                glFramebufferTexture3D);
        LOAD(PFNGLFRAMEBUFFERRENDERBUFFERPROC,             glFramebufferRenderbuffer);
        LOAD(PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC, glGetFramebufferAttachmentParameteriv);
        LOAD(PFNGLGENERATEMIPMAPPROC,                      glGenerateMipmap);
        LOAD(PFNGLBLITFRAMEBUFFERPROC,                     glBlitFramebuffer);
        LOAD(PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC,      glRenderbufferStorageMultisample);
        LOAD(PFNGLFRAMEBUFFERTEXTURELAYERPROC,             glFramebufferTextureLayer);
        LOAD(PFNGLMAPBUFFERRANGEPROC,                      glMapBufferRange);
        LOAD(PFNGLFLUSHMAPPEDBUFFERRANGEPROC,              glFlushMappedBufferRange);
        LOAD(PFNGLBINDVERTEXARRAYPROC,                     glBindVertexArray);
        LOAD(PFNGLDELETEVERTEXARRAYSPROC,                  glDeleteVertexArrays);
        LOAD(PFNGLGENVERTEXARRAYSPROC,                     glGenVertexArrays);
        LOAD(PFNGLISVERTEXARRAYPROC,                       glIsVertexArray);
    }

    if (version >= VD_FW_GL_VERSION_3_1) {
        LOAD(PFNGLDRAWARRAYSINSTANCEDPROC,       glDrawArraysInstanced);
        LOAD(PFNGLDRAWELEMENTSINSTANCEDPROC,     glDrawElementsInstanced);
        LOAD(PFNGLTEXBUFFERPROC,                 glTexBuffer);
        LOAD(PFNGLPRIMITIVERESTARTINDEXPROC,     glPrimitiveRestartIndex);
        LOAD(PFNGLCOPYBUFFERSUBDATAPROC,         glCopyBufferSubData);
        LOAD(PFNGLGETUNIFORMINDICESPROC,         glGetUniformIndices);
        LOAD(PFNGLGETACTIVEUNIFORMSIVPROC,       glGetActiveUniformsiv);
        LOAD(PFNGLGETACTIVEUNIFORMNAMEPROC,      glGetActiveUniformName);
        LOAD(PFNGLGETUNIFORMBLOCKINDEXPROC,      glGetUniformBlockIndex);
        LOAD(PFNGLGETACTIVEUNIFORMBLOCKIVPROC,   glGetActiveUniformBlockiv);
        LOAD(PFNGLGETACTIVEUNIFORMBLOCKNAMEPROC, glGetActiveUniformBlockName);
        LOAD(PFNGLUNIFORMBLOCKBINDINGPROC,       glUniformBlockBinding);
    }

    if (version >= VD_FW_GL_VERSION_3_2) {
        LOAD(PFNGLDRAWELEMENTSBASEVERTEXPROC,          glDrawElementsBaseVertex);
        LOAD(PFNGLDRAWRANGEELEMENTSBASEVERTEXPROC,     glDrawRangeElementsBaseVertex);
        LOAD(PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXPROC, glDrawElementsInstancedBaseVertex);
        LOAD(PFNGLMULTIDRAWELEMENTSBASEVERTEXPROC,     glMultiDrawElementsBaseVertex);
        LOAD(PFNGLPROVOKINGVERTEXPROC,                 glProvokingVertex);
        LOAD(PFNGLFENCESYNCPROC,                       glFenceSync);
        LOAD(PFNGLISSYNCPROC,                          glIsSync);
        LOAD(PFNGLDELETESYNCPROC,                      glDeleteSync);
        LOAD(PFNGLCLIENTWAITSYNCPROC,                  glClientWaitSync);
        LOAD(PFNGLWAITSYNCPROC,                        glWaitSync);
        LOAD(PFNGLGETINTEGER64VPROC,                   glGetInteger64v);
        LOAD(PFNGLGETSYNCIVPROC,                       glGetSynciv);
        LOAD(PFNGLGETINTEGER64I_VPROC,                 glGetInteger64i_v);
        LOAD(PFNGLGETBUFFERPARAMETERI64VPROC,          glGetBufferParameteri64v);
        LOAD(PFNGLFRAMEBUFFERTEXTUREPROC,              glFramebufferTexture);
        LOAD(PFNGLTEXIMAGE2DMULTISAMPLEPROC,           glTexImage2DMultisample);
        LOAD(PFNGLTEXIMAGE3DMULTISAMPLEPROC,           glTexImage3DMultisample);
        LOAD(PFNGLGETMULTISAMPLEFVPROC,                glGetMultisamplefv);
        LOAD(PFNGLSAMPLEMASKIPROC,                     glSampleMaski);
    }

    if (version >= VD_FW_GL_VERSION_3_3) {
        LOAD(PFNGLBINDFRAGDATALOCATIONINDEXEDPROC, glBindFragDataLocationIndexed);
        LOAD(PFNGLGETFRAGDATAINDEXPROC,            glGetFragDataIndex);
        LOAD(PFNGLGENSAMPLERSPROC,                 glGenSamplers);
        LOAD(PFNGLDELETESAMPLERSPROC,              glDeleteSamplers);
        LOAD(PFNGLISSAMPLERPROC,                   glIsSampler);
        LOAD(PFNGLBINDSAMPLERPROC,                 glBindSampler);
        LOAD(PFNGLSAMPLERPARAMETERIPROC,           glSamplerParameteri);
        LOAD(PFNGLSAMPLERPARAMETERIVPROC,          glSamplerParameteriv);
        LOAD(PFNGLSAMPLERPARAMETERFPROC,           glSamplerParameterf);
        LOAD(PFNGLSAMPLERPARAMETERFVPROC,          glSamplerParameterfv);
        LOAD(PFNGLSAMPLERPARAMETERIIVPROC,         glSamplerParameterIiv);
        LOAD(PFNGLSAMPLERPARAMETERIUIVPROC,        glSamplerParameterIuiv);
        LOAD(PFNGLGETSAMPLERPARAMETERIVPROC,       glGetSamplerParameteriv);
        LOAD(PFNGLGETSAMPLERPARAMETERIIVPROC,      glGetSamplerParameterIiv);
        LOAD(PFNGLGETSAMPLERPARAMETERFVPROC,       glGetSamplerParameterfv);
        LOAD(PFNGLGETSAMPLERPARAMETERIUIVPROC,     glGetSamplerParameterIuiv);
        LOAD(PFNGLQUERYCOUNTERPROC,                glQueryCounter);
        LOAD(PFNGLGETQUERYOBJECTI64VPROC,          glGetQueryObjecti64v);
        LOAD(PFNGLGETQUERYOBJECTUI64VPROC,         glGetQueryObjectui64v);
        LOAD(PFNGLVERTEXATTRIBDIVISORPROC,         glVertexAttribDivisor);
        LOAD(PFNGLVERTEXATTRIBP1UIPROC,            glVertexAttribP1ui);
        LOAD(PFNGLVERTEXATTRIBP1UIVPROC,           glVertexAttribP1uiv);
        LOAD(PFNGLVERTEXATTRIBP2UIPROC,            glVertexAttribP2ui);
        LOAD(PFNGLVERTEXATTRIBP2UIVPROC,           glVertexAttribP2uiv);
        LOAD(PFNGLVERTEXATTRIBP3UIPROC,            glVertexAttribP3ui);
        LOAD(PFNGLVERTEXATTRIBP3UIVPROC,           glVertexAttribP3uiv);
        LOAD(PFNGLVERTEXATTRIBP4UIPROC,            glVertexAttribP4ui);
        LOAD(PFNGLVERTEXATTRIBP4UIVPROC,           glVertexAttribP4uiv);
    }
#undef LOAD
#endif 
}

enum {
    VD_FW__CLOSE_BUTTON_ID = 0,
    VD_FW__MAXIMIZE_BUTTON_ID = 1,
    VD_FW__MAX_IDS,
};

typedef struct {
    int   is_hovered;
    int   was_hovered;
    float location[4];
    float color[4];
} VdFw__Elem;

typedef struct {
    float start_color[4];
    float end_color[4];
    float now_color[4];
    float timer;
    float time;
    int ease_mode;
} VdFw__Tween;

typedef struct {
    VdFw__Elem  e[VD_FW__MAX_IDS];
    VdFw__Tween t[VD_FW__MAX_IDS];

    int        initialized;
    GLint      projection_location;
    GLint      rect_location;
    GLint      color_location;
    GLuint     vao;
    GLuint     program;
    GLuint     vbo;
    GLuint     ibo;
    float      mouse[2];
    float      window[2];
} VdFw__GenericInternalData;

static void vd_fw__write_location(VdFw__GenericInternalData *data, int id, float location[4])
{
    VdFw__Elem *e = &data->e[id];
    e->location[0] = location[0];
    e->location[1] = location[1];
    e->location[2] = location[2];
    e->location[3] = location[3];

    int inside = vd_fw_u_point_in_rect(data->mouse[0], data->mouse[1], location[0], location[1], location[2], location[3]);

    e->was_hovered = e->is_hovered;
    e->is_hovered = inside;
}

static void vd_fw__write_color(VdFw__GenericInternalData *data, int id, float color[4])
{
    VdFw__Elem *e = &data->e[id];
    e->color[0] = color[0];
    e->color[1] = color[1];
    e->color[2] = color[2];
    e->color[3] = color[3];
}

static int vd_fw__was_hovered(VdFw__GenericInternalData *data, int id) {
    return data->e[id].was_hovered;
}

static int vd_fw__is_hovered(VdFw__GenericInternalData *data, int id) {
    return data->e[id].is_hovered;
}

static int vd_fw__was_unhovered(VdFw__GenericInternalData *data, int id) {
    return data->e[id].was_hovered && !data->e[id].is_hovered;
}

static void vd_fw__anim_update(VdFw__GenericInternalData *data, int id, float new_color[4], float time, int mode) {
    VdFw__Tween *w = &data->t[id];
    w->timer = 0.f;
    w->time = time;
    w->ease_mode = mode;

    w->start_color[0] = w->end_color[0];
    w->start_color[1] = w->end_color[1];
    w->start_color[2] = w->end_color[2];
    w->start_color[3] = w->end_color[3];

    w->end_color[0] = new_color[0];
    w->end_color[1] = new_color[1];
    w->end_color[2] = new_color[2];
    w->end_color[3] = new_color[3];
}

static float vd_fw__ease(VdFw__Tween *w, float x) {
    if (w->ease_mode == 0) {
        return x;
    } else if (w->ease_mode == 1) {
        float p = (-2.f * x + 2.f);
        return x < 0.5f ? 16.f * x * x * x * x * x : 1.f - (p*p*p*p*p) / 2.f;
    }

    return x;
}

static void vd_fw__animate(VdFw__GenericInternalData *data, int id) {

    // VdFw__Elem  *e = &data->e[id];
    VdFw__Tween *w = &data->t[id];

    float s = vd_fw_delta_s();

    if (w->time <= 0.000001f) {
        return;
    }

    w->timer += s;

    if (w->timer > w->time) {
        w->timer = w->time;
        vd_fw__write_color(data, id, w->end_color);
    } else {

        float t = w->timer / w->time;

        float now[4] = {
            (1.f - vd_fw__ease(w, t)) * w->start_color[0] + vd_fw__ease(w, t) * w->end_color[0],
            (1.f - vd_fw__ease(w, t)) * w->start_color[1] + vd_fw__ease(w, t) * w->end_color[1],
            (1.f - vd_fw__ease(w, t)) * w->start_color[2] + vd_fw__ease(w, t) * w->end_color[2],
            (1.f - vd_fw__ease(w, t)) * w->start_color[3] + vd_fw__ease(w, t) * w->end_color[3],
        };
        vd_fw__write_color(data, id, now);
    }

}

#define VD_FW_GL_VERSION_3_3_INTERNAL_VERTEX_SHADER_SOURCE                                                             \
    "#version 330 core                                                                                             \n" \
    "uniform mat4 uProjection;                                                                                     \n" \
    "uniform vec4 uRect;                                                                                           \n" \
    "void main() {                                                                                                 \n" \
    "   vec2 positions[6] = vec2[](                                                                                \n" \
    "       vec2(0.0, 0.0), // top-left                                                                            \n" \
    "       vec2(1.0, 0.0), // top-right                                                                           \n" \
    "       vec2(1.0, 1.0), // bottom-right                                                                        \n" \
    "       vec2(1.0, 1.0), // bottom-right                                                                        \n" \
    "       vec2(0.0, 1.0), // bottom-left                                                                         \n" \
    "       vec2(0.0, 0.0)  // top-left                                                                            \n" \
    "   );                                                                                                         \n" \
    "   vec2 p = vec2(positions[gl_VertexID].x * uRect.z + uRect.x, positions[gl_VertexID].y * uRect.w + uRect.y); \n" \
    "   gl_Position = uProjection * vec4(p, 0.0, 1.0);                                                             \n" \
    "}                                                                                                             \n" \

#define VD_FW_GL_VERSION_3_3_INTERNAL_FRAGMENT_SHADER_SOURCE                                                           \
    "#version 330 core                                                                                             \n" \
    "uniform vec4 uColor;                                                                                          \n" \
    "out vec4 FragColor;                                                                                           \n" \
    "void main() {                                                                                                 \n" \
    "   FragColor = uColor;                                                                                        \n" \
    "}                                                                                                             \n" \

void vd_fw_draw_window_border(void)
{
    static VdFw__GenericInternalData data = {0};

    if (!data.initialized) {
        data.initialized = 1;
        glGenVertexArrays(1, &data.vao);
        glBindVertexArray(data.vao);

        const char *vertex_shader = VD_FW_GL_VERSION_3_3_INTERNAL_VERTEX_SHADER_SOURCE;
        GLuint vshd = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshd, 1, &vertex_shader, 0);
        glCompileShader(vshd);

        const char *fragment_shader = VD_FW_GL_VERSION_3_3_INTERNAL_FRAGMENT_SHADER_SOURCE;
        GLuint fshd = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshd, 1, &fragment_shader, 0);
        glCompileShader(fshd);

        data.program = glCreateProgram();
        glAttachShader(data.program, vshd);
        glAttachShader(data.program, fshd);
        glLinkProgram(data.program);

        data.projection_location = glGetUniformLocation(data.program, "uProjection");
        data.rect_location       = glGetUniformLocation(data.program, "uRect");
        data.color_location      = glGetUniformLocation(data.program, "uColor");

        glBindVertexArray(0);
    }
    glUseProgram(data.program);

    float width;
    float height;
    {

        int widthi, heighti;
        vd_fw_get_size(&widthi, &heighti);

        width  = (float)widthi;
        height = (float)heighti;
        float projection[16];
        vd_fw_u_ortho(0.0f, width, height, 0.0f, -1.0f, 1.0f, projection);
        glUniformMatrix4fv(data.projection_location, 1, GL_FALSE, projection);

        vd_fw_get_mouse_statef(&data.mouse[0], &data.mouse[1]);
    }

    glBindVertexArray(data.vao);

#define PUT_RECT(x, y, w, h, r, g, b, a) do {     \
    glUniform4f(data.rect_location, x, y, w, h);  \
    glUniform4f(data.color_location, r, g, b, a); \
    glDrawArrays(GL_TRIANGLES, 0, 6);             \
} while(0)

    float button_size = 24.f;
    float offset = width - button_size;

    float close_button_normal[4] = {.7f, 0.f, 0.f, 1.f};
    float close_button_hover[4]  = {1.f, 0.f, 0.f, 1.f};

    vd_fw__write_location(&data, VD_FW__CLOSE_BUTTON_ID, (float[]){offset, 0.f, button_size, button_size});
    offset -= button_size;

    if (!vd_fw__was_hovered(&data, VD_FW__CLOSE_BUTTON_ID) && vd_fw__is_hovered(&data, VD_FW__CLOSE_BUTTON_ID)) {
        vd_fw__anim_update(&data, VD_FW__CLOSE_BUTTON_ID, close_button_hover, 0.16f, 1);
    } else if (vd_fw__was_hovered(&data, VD_FW__CLOSE_BUTTON_ID) && !vd_fw__is_hovered(&data, VD_FW__CLOSE_BUTTON_ID)) {
        vd_fw__anim_update(&data, VD_FW__CLOSE_BUTTON_ID, close_button_normal, 0.32f, 1);
    } else {
        vd_fw__write_color(&data, VD_FW__CLOSE_BUTTON_ID, close_button_normal);
    }

    float maximize_button_normal[4] = {0.2f, 0.7f, 0.2f, 1.f};
    float maximize_button_hover[4]  = {0.3f, 0.5f, 0.3f, 1.f};
    vd_fw__write_location(&data, VD_FW__MAXIMIZE_BUTTON_ID, (float[]){offset, 0.f, button_size, button_size});
    offset -= button_size;

    if (!vd_fw__was_hovered(&data, VD_FW__MAXIMIZE_BUTTON_ID) && vd_fw__is_hovered(&data, VD_FW__MAXIMIZE_BUTTON_ID)) {
        vd_fw__anim_update(&data, VD_FW__MAXIMIZE_BUTTON_ID, maximize_button_hover, 0.16f, 1);
        // OutputDebugStringA("HOVER\n");
    } else if (vd_fw__was_hovered(&data, VD_FW__MAXIMIZE_BUTTON_ID) && !vd_fw__is_hovered(&data, VD_FW__MAXIMIZE_BUTTON_ID)) {
        vd_fw__anim_update(&data, VD_FW__MAXIMIZE_BUTTON_ID, maximize_button_normal, 0.32f, 1);
    } else {
        vd_fw__write_color(&data, VD_FW__MAXIMIZE_BUTTON_ID, maximize_button_normal);
    }

    vd_fw__animate(&data, VD_FW__CLOSE_BUTTON_ID);
    vd_fw__animate(&data, VD_FW__MAXIMIZE_BUTTON_ID);
    for (int i = 0; i < VD_FW__MAX_IDS; ++i) {


        VdFw__Elem *e = &data.e[i];
        PUT_RECT(
            e->location[0], e->location[1], e->location[2], e->location[3],
            e->color[0], e->color[1], e->color[2], e->color[3]);
    }
    
    glBindVertexArray(0);

    glUseProgram(0);
#undef PUT_RECT
}

#if defined(__APPLE__)
#pragma clang diagnostic pop
#endif
#endif // VD_FW_IMPL