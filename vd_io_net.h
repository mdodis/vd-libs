/* vd_io_net.h - Networking
 * ---------------------------------------------------------------------------------------------------------------------
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
 *
 * NOTES
 * - It is imperative that a cryptographically secure rng be used to replace VD_IO_NET_RANDOM.
 *   By default it just uses rand.
 */
#ifndef VD_IO_NET_H
#define VD_IO_NET_H
#define VD_IO_NET_VERSION_MAJOR    0
#define VD_IO_NET_VERSION_MINOR    0
#define VD_IO_NET_VERSION_PATCH    1
#define VD_IO_NET_VERSION          ((VD_IO_NET_VERSION_MAJOR << 16) | (VD_IO_NET_VERSION_MINOR << 8) | (VD_IO_NET_VERSION_PATCH))
#ifndef VD_IO_NET_API
#   ifdef VD_IO_NET_STATIC
#       define VD_IO_NET_API static
#   else
#       define VD_IO_NET_API extern
#   endif // VD_IO_NET_STATIC
#endif // !VD_IO_NET_API

#ifndef VD_IO_H
#error "vd_io_net.h requires vd_io.h. Please include it before including this"
#endif // !VD_IO_H

#include <stdint.h>

/* ----TLS----------------------------------------------------------------------------------------------------------- */
typedef enum {
    VD_IO_NET_TLS_STATE_PROCESSING = 0,
    VD_IO_NET_TLS_STATE_READY = 1,
} VdIoNetTlsState;

typedef uint8_t VdIoNet__TlsHandshakeType;

typedef struct {
    VdIoEndpoint endpoint;
    size_t       memory_size;
    void         *memory;
} VdIoNetTlsClientInfo;

typedef struct {
    VdIo                      *io;
    VdIoNetTlsState           state;
    VdIoHn                    socket;

    // Internal
    uint8_t                   *buf;
    size_t                    bufsize;

    size_t                    client_hello_size;

    uint8_t                   private_key[32];

    VdIoCallbackInfo          on_connect_cb;
    VdIoNet__TlsHandshakeType next_handshake_state;
} VdIoNetTls;

VD_IO_NET_API size_t     vd_io_net_tls_client_info_mem_size(VdIoNetTlsClientInfo *info);

VD_IO_NET_API VdIoErr    vd_io_net_tls_connect(VdIo *io, VdIoNetTlsClientInfo *info, VdIoNetTls *tls,
                                               VdIoCallback *cb, void *usr);

#endif // !VD_IO_NET_H


#ifdef VD_IO_NET_IMPL
#ifndef VD_IO_H
#error "vd_io_net.h requires vd_io.h. Please include it before including this"
#endif // !VD_IO_H

#ifndef VD_IO_NET_ASSERTIONS
#define VD_IO_NET_ASSERTIONS 0
#endif // !VD_IO_NET_ASSERTIONS

#if VD_IO_NET_ASSERTIONS
#   ifndef VD_IO_NET_ASSERT
#      include <assert.h>
#      define VD_IO_NET_ASSERT(x) assert(x)
#   endif // !VD_IO_NET_ASSERT
#endif // VD_IO_NET_ASSERTIONS

#if defined(_MSC_VER)
#  define VD_IO_NET__HOST_LITTLE_ENDIAN 1
#elif __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#  define VD_IO_NET__HOST_LITTLE_ENDIAN 1
#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#  define VD_IO_NET__HOST_LITTLE_ENDIAN 0
#else
#  error "Unknown byte order"
#endif

static uint16_t vd_io_net__swap16(uint16_t x)
{
    return ((uint16_t)((x << 8) | (x >> 8)));
}

#if VD_IO_NET__HOST_LITTLE_ENDIAN
#   define VD_IO_NET__BE16_(h0, h1) 0x##h1##h0
#   define VD_IO_NET__BE16(h0, h1) VD_IO_NET__BE16_(h0, h1)
#   define VD_IO_NET__SWAP16_BE(x) vd_io_net__swap16(x)
#else
#   define VD_IO_NET__BE16_(h0, h1) 0x##h0##h1
#   define VD_IO_NET__BE16(h0, h1) VD_IO_NET__BE16_(h0, h1)
#   define VD_IO_NET__SWAP16_BE(x) x
#endif // VD_IO_NET__HOST_LITTLE_ENDIAN

#ifndef VD_IO_NET_SHA256
#   define VD_IO_NET_SHA256(bytes, len, out) vd_io_net__sha256(bytes, len, out)
#endif // !VD_IO_NET_SHA256

/* ----RANDOM-------------------------------------------------------------------------------------------------------- */
#ifndef VD_IO_NET_RANDOM
#include <stdlib.h>
#define VD_IO_NET_RANDOM(ptr, len) vd_io_net__sys_random((uint8_t*)(ptr), (size_t)(len))

static void vd_io_net__sys_random(uint8_t *ptr, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        ptr[i] = (uint8_t)(rand() % 255);
    }
}
#endif // !VD_IO_NET_RANDOM

/* ----Curve25519---------------------------------------------------------------------------------------------------- */
typedef struct {
    int64_t x[16];
} VdIoNet__C25519;

static const VdIoNet__C25519 VdIoNet__121665 = {{0xDB41, 1}};

static VdIoNet__C25519 vd_io_net__c25519_unpack(uint8_t in[32])
{
    VdIoNet__C25519 result;
    int i;

    for (i = 0; i < 16; ++i) {
        result.x[i] = in[2*i] + ((int64_t)in[2*i + 1] << 8);
    }

    result.x[15] &= 0x7fff;
    return result;
}

static void vd_io_net__c25519_carry(VdIoNet__C25519 *e)
{
    int i;
    int64_t carry;

    for (i = 0; i < 16; ++i) {
        carry = e->x[i] >> 16;
        e->x[i] -= carry << 16;

        if (i < 15) {
            e->x[i + 1] += carry;
        } else {
            e->x[0] += 38 * carry;
        }
    }
}

static VdIoNet__C25519 vd_io_net__c25519_add(VdIoNet__C25519 *a, VdIoNet__C25519 *b)
{
    VdIoNet__C25519 result;
    for (int i = 0; i < 16; ++i) {
        result.x[i] = a->x[i] + b->x[i];
    }
    return result;
}

static VdIoNet__C25519 vd_io_net__c25519_sub(VdIoNet__C25519 *a, VdIoNet__C25519 *b)
{
    VdIoNet__C25519 result;
    for (int i = 0; i < 16; ++i) {
        result.x[i] = a->x[i] - b->x[i];
    }
    return result;
}

static VdIoNet__C25519 vd_io_net__c25519_mul(const VdIoNet__C25519 *a, const VdIoNet__C25519 *b)
{
    VdIoNet__C25519 result;
    int64_t i, j, product[31];
    for (i = 0; i < 31; ++i) {
        product[i] = 0;
    }

    for (i = 0; i < 16; ++i) {
        for (j = 0; j < 16; ++j) {
            product[i+j] += a->x[i] * b->x[j];
        }
    }

    for (i = 0; i < 15; ++i) {
        product[i] += 38 * product[i + 16];
    }

    for (i = 0; i < 16; ++i) {
        result.x[i] = product[i];
    }

    vd_io_net__c25519_carry(&result);
    vd_io_net__c25519_carry(&result);

    return result;
}

static VdIoNet__C25519 vd_io_net__c25519_inverse(VdIoNet__C25519 *in)
{
    VdIoNet__C25519 result, c;
    int i;

    for (i = 0; i < 16; ++i) {
        c.x[i] = in->x[i];
    }

    for (i = 253; i >= 0; --i) {
        c = vd_io_net__c25519_mul(&c, &c);

        if ((i != 2) && (i != 4)) {
            c = vd_io_net__c25519_mul(&c, in);
        }
    }

    for (i = 0; i < 16; ++i) {
        result.x[i] = c.x[i];
    }

    return result;
}

static void vd_io_net__c25519_swap(VdIoNet__C25519 *p, VdIoNet__C25519 *q, int f)
{
    int64_t t, i, c = ~(f - 1);
    for (i = 0; i < 16; ++i) {
        t = c & (p->x[i] ^ q->x[i]);
        p->x[i] ^= t;
        q->x[i] ^= t;
    }
}

static void vd_io_net__c25519_pack(VdIoNet__C25519 *in, uint8_t out[32])
{
    int i, j, carry;
    VdIoNet__C25519 m, t;

    for (i = 0; i < 16; ++i) {
        t.x[i] = in->x[i];
    }

    vd_io_net__c25519_carry(&t);
    vd_io_net__c25519_carry(&t);
    vd_io_net__c25519_carry(&t);

    for (j = 0; j < 2; ++j) {
        m.x[0] = t.x[0] - 0xffed;
        for (i = 1; i < 15; ++i) {
            m.x[i] = t.x[i] - 0xffff - ((m.x[i - 1] >> 16) & 1);
            m.x[i - 1] &= 0xffff;
        }

        m.x[15] = t.x[15] - 0x7fff - ((m.x[14] >> 16) & 1);
        carry = (m.x[15] >> 16) & 1;
        m.x[14] &= 0xffff;
        vd_io_net__c25519_swap(&t, &m, 1 - carry);
    }

    for (i = 0; i < 16; ++i) {
        out[i*2] = t.x[i] & 0xff;
        out[i*2+1] = (uint8_t)(t.x[i] >> 8);
    }
}

static void vd_io_net__c25519_scalar_mult(uint8_t out[32], uint8_t scalar[32], uint8_t point[32])
{
    uint8_t clamped[32];
    int bit, i;
    VdIoNet__C25519 a, b, c, d, e, f, x;

    for (i = 0; i < 32; ++i) {
        clamped[i] = scalar[i];
    }
    clamped[0] &= 0xf8;
    clamped[31] = (clamped[31] & 0x7f) | 0x40;

    x = vd_io_net__c25519_unpack(point);

    for (i = 0; i < 16; ++i) {
        b.x[i] = x.x[i];
        d.x[i] = a.x[i] = c.x[i] = 0;
    }

    a.x[0] = d.x[0] = 1;

    for (i = 254; i >= 0; --i) {
        bit = (clamped[i >> 3] >> (i & 7)) & 1;

        vd_io_net__c25519_swap(&a, &b, bit);
        vd_io_net__c25519_swap(&c, &d, bit);

        e = vd_io_net__c25519_add(&a, &c);
        a = vd_io_net__c25519_sub(&a, &c);
        c = vd_io_net__c25519_add(&b, &d);
        b = vd_io_net__c25519_sub(&b, &d);
        d = vd_io_net__c25519_mul(&e, &e);
        f = vd_io_net__c25519_mul(&a, &a);
        a = vd_io_net__c25519_mul(&c, &a);
        c = vd_io_net__c25519_mul(&b, &e);
        e = vd_io_net__c25519_add(&a, &c);
        a = vd_io_net__c25519_sub(&a, &c);
        b = vd_io_net__c25519_mul(&a, &a);
        c = vd_io_net__c25519_sub(&d, &f);
        a = vd_io_net__c25519_mul(&c, &VdIoNet__121665);
        a = vd_io_net__c25519_add(&a, &d);
        c = vd_io_net__c25519_mul(&c, &a);
        a = vd_io_net__c25519_mul(&d, &f);
        d = vd_io_net__c25519_mul(&b, &x);
        b = vd_io_net__c25519_mul(&e, &e);
        vd_io_net__c25519_swap(&a, &b, bit);
        vd_io_net__c25519_swap(&c, &d, bit);
    }

    c = vd_io_net__c25519_inverse(&c);
    a = vd_io_net__c25519_mul(&a, &c);
    vd_io_net__c25519_pack(&a, out);
}

/* ----SHA256-------------------------------------------------------------------------------------------------------- */
#ifndef VD_IO_NET_SHA256
static void vd_io_net__sha256(const uint8_t *bytes, size_t len, uint8_t out[32])
{
#define S(x, n) (((((uint32_t)(x)&0xFFFFFFFFUL)>>(uint32_t)((n)&31))|((uint32_t)(x)<<(uint32_t)((32-((n)&31))&31)))&0xFFFFFFFFUL)
#define R(x, n) (((x)&0xFFFFFFFFUL)>>(n))
#define Gamma0(x) (S(x, 7) ^ S(x, 18) ^ R(x, 3))
#define Gamma1(x) (S(x, 17) ^ S(x, 19) ^ R(x, 10))
#define RND(a,b,c,d,e,f,g,h,i) \
    t0 = h + (S(e, 6) ^ S(e, 11) ^ S(e, 25)) + (g ^ (e & (f ^ g))) + K[i] + W[i]; \
    t1 = (S(a, 2) ^ S(a, 13) ^ S(a, 22)) + (((a | b) & c) | (a & b)); \
    d += t0; \
    h  = t0 + t1;
#define STORE32H(x, y) \
    (y)[0] = (unsigned char)(((x)>>24)&255); (y)[1] = (unsigned char)(((x)>>16)&255); \
    (y)[2] = (unsigned char)(((x)>>8)&255); (y)[3] = (unsigned char)((x)&255);
#define LOAD32H(x, y) \
    x = ((uint32_t)((y)[0]&255)<<24)|((uint32_t)((y)[1]&255)<<16)|((uint32_t)((y)[2]&255)<<8)|((uint32_t)((y)[3]&255));
#define STORE64H(x, y) \
    (y)[0] = (unsigned char)(((x)>>56)&255); (y)[1] = (unsigned char)(((x)>>48)&255); \
    (y)[2] = (unsigned char)(((x)>>40)&255); (y)[3] = (unsigned char)(((x)>>32)&255); \
    (y)[4] = (unsigned char)(((x)>>24)&255); (y)[5] = (unsigned char)(((x)>>16)&255); \
    (y)[6] = (unsigned char)(((x)>>8)&255); (y)[7] = (unsigned char)((x)&255);
#define SHA256_COMPRESS(buff) \
    for (int i = 0; i < 8; i++) S[i] = sha256_state[i]; \
    for (int i = 0; i < 16; i++) LOAD32H(W[i], buff + (4*i)); \
    for (int i = 16; i < 64; i++) W[i] = Gamma1(W[i-2]) + W[i-7] + Gamma0(W[i-15]) + W[i-16]; \
    for (int i = 0; i < 64; i++) { \
        RND(S[0],S[1],S[2],S[3],S[4],S[5],S[6],S[7],i); \
        t = S[7]; S[7] = S[6]; S[6] = S[5]; S[5] = S[4]; \
        S[4] = S[3]; S[3] = S[2]; S[2] = S[1]; S[1] = S[0]; S[0] = t; \
    } \
    for (int i = 0; i < 8; i++) sha256_state[i] = sha256_state[i] + S[i];

    //writes the sha256 hash of the first "len" bytes in buffer "in" to buffer "out"
    //returns 0 on success, may return non-zero in future versions to indicate error
    const uint32_t K[64] = {
        0x428a2f98UL, 0x71374491UL, 0xb5c0fbcfUL, 0xe9b5dba5UL,
        0x3956c25bUL, 0x59f111f1UL, 0x923f82a4UL, 0xab1c5ed5UL,
        0xd807aa98UL, 0x12835b01UL, 0x243185beUL, 0x550c7dc3UL,
        0x72be5d74UL, 0x80deb1feUL, 0x9bdc06a7UL, 0xc19bf174UL,
        0xe49b69c1UL, 0xefbe4786UL, 0x0fc19dc6UL, 0x240ca1ccUL,
        0x2de92c6fUL, 0x4a7484aaUL, 0x5cb0a9dcUL, 0x76f988daUL,
        0x983e5152UL, 0xa831c66dUL, 0xb00327c8UL, 0xbf597fc7UL,
        0xc6e00bf3UL, 0xd5a79147UL, 0x06ca6351UL, 0x14292967UL,
        0x27b70a85UL, 0x2e1b2138UL, 0x4d2c6dfcUL, 0x53380d13UL,
        0x650a7354UL, 0x766a0abbUL, 0x81c2c92eUL, 0x92722c85UL,
        0xa2bfe8a1UL, 0xa81a664bUL, 0xc24b8b70UL, 0xc76c51a3UL,
        0xd192e819UL, 0xd6990624UL, 0xf40e3585UL, 0x106aa070UL,
        0x19a4c116UL, 0x1e376c08UL, 0x2748774cUL, 0x34b0bcb5UL,
        0x391c0cb3UL, 0x4ed8aa4aUL, 0x5b9cca4fUL, 0x682e6ff3UL,
        0x748f82eeUL, 0x78a5636fUL, 0x84c87814UL, 0x8cc70208UL,
        0x90befffaUL, 0xa4506cebUL, 0xbef9a3f7UL, 0xc67178f2UL
    };
    uint64_t sha256_length = 0;
    uint32_t sha256_state[8] = {
        0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL, 0xA54FF53AUL,
        0x510E527FUL, 0x9B05688CUL, 0x1F83D9ABUL, 0x5BE0CD19UL
    }, S[8], W[64], t0, t1, t;
    unsigned char sha256_buf[64];
    //process input in 64 byte chunks
    while (len >= 64) {
       SHA256_COMPRESS(in);
       sha256_length += 64 * 8;
       in += 64;
       len -= 64;
    }
    //copy remaining bytes into sha256_buf
    memcpy(sha256_buf, in, len);
    //finish up (len now number of bytes in sha256_buf)
    sha256_length += len * 8;
    sha256_buf[len++] = 0x80;
    //pad then compress if length is above 56 bytes
    if (len > 56) {
        while (len < 64) sha256_buf[len++] = 0;
        SHA256_COMPRESS(sha256_buf);
        len = 0;
    }
    //pad up to 56 bytes
    while (len < 56) sha256_buf[len++] = 0;
    //store length and compress
    STORE64H(sha256_length, sha256_buf + 56);
    SHA256_COMPRESS(sha256_buf);
    //copy output
    for (int i = 0; i < 8; i++) {
        STORE32H(sha256_state[i], out + 4*i);
    }

#undef S
#undef R
#undef Gamma0
#undef Gamma1
#undef RND
#undef STORE32H
#undef LOAD32H
#undef STORE64H
#undef SHA256_COMPRESS
}
#endif // !VD_IO_NET_SHA256

/* ----TLS----------------------------------------------------------------------------------------------------------- */
enum VD_IO_NET__TLS_HANDSHAKE_TYPE_ {
    VD_IO_NET__TLS_HANDSHAKE_TYPE_HELLO_REQUEST_RESERVED        = 0x0,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CLIENT_HELLO                  = 0x1,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_SERVER_HELLO                  = 0x2,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_HELLO_VERIFY_REQUEST_RESERVED = 0x3,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_NEW_SESSION_TICKET            = 0x4,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_END_OF_EARLY_DATA             = 0x5,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_HELLO_RETRY_REQUEST_RESERVED  = 0x6,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_ENCRYPTED_EXTENSIONS          = 0x8,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_SERVER_KEY_EXCHANGE_RESERVED  = 0xc,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CERTIFICATE_REQUEST           = 0xd,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_SERVER_HELLO_DONE_RESERVED    = 0xe,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CERTIFICATE_VERIFY            = 0xf,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CLIENT_KEY_EXCHANGE_RESERVED  = 0x10,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_FINISHED                      = 0x14,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CERTIFICATE_URL_RESERVED      = 0x15,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CERTIFICATE_STATUS_RESERVED   = 0x16,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_SUPPLEMENTAL_DATA_RESERVED    = 0x17,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_KEY_UPDATE                    = 0x18,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_MESSAGE_HASH                  = 0xfe,
};

enum VD_IO_NET__TLS_RECORD_TYPE_ {
    VD_IO_NET__TLS_RECORD_TYPE_HANDSHAKE = 0x16,
};
typedef uint8_t VdIoNet__TlsRecordType;

enum VD_IO_NET__TLS_CIPHER_SUITE_ {
    VD_IO_NET__TLS_CIPHER_SUITE_AES_128_GCM_SHA256       = VD_IO_NET__BE16(13, 01),
    VD_IO_NET__TLS_CIPHER_SUITE_AES_256_GCM_SHA384       = VD_IO_NET__BE16(13, 02),
    VD_IO_NET__TLS_CIPHER_SUITE_CHACHA20_POLY1305_SHA256 = VD_IO_NET__BE16(13, 03),
    VD_IO_NET__TLS_CIPHER_SUITE_AES_128_CCM_SHA256       = VD_IO_NET__BE16(13, 04),
    VD_IO_NET__TLS_CIPHER_SUITE_AES_128_CCM_8_SHA256     = VD_IO_NET__BE16(13, 05),
};
typedef uint16_t VdIoNet__TlsCipherSuite;

enum VD_IO_NET__TLS_EXTENSION_KIND_ {
    VD_IO_NET__TLS_EXTENSION_KIND_SERVER_NAME                            = VD_IO_NET__BE16(00, 00),
    VD_IO_NET__TLS_EXTENSION_KIND_MAX_FRAGMENT_LENGTH                    = VD_IO_NET__BE16(00, 01),
    VD_IO_NET__TLS_EXTENSION_KIND_STATUS_REQUEST                         = VD_IO_NET__BE16(00, 05),
    VD_IO_NET__TLS_EXTENSION_KIND_SUPPORTED_GROUPS                       = VD_IO_NET__BE16(00, 0a),
    VD_IO_NET__TLS_EXTENSION_KIND_EC_POINT_FORMATS                       = VD_IO_NET__BE16(00, 0b),
    VD_IO_NET__TLS_EXTENSION_KIND_SIGNATURE_ALGORITHMS                   = VD_IO_NET__BE16(00, 0d),
    VD_IO_NET__TLS_EXTENSION_KIND_USE_SRTP                               = VD_IO_NET__BE16(00, 0e),
    VD_IO_NET__TLS_EXTENSION_KIND_HEARTBEAT                              = VD_IO_NET__BE16(00, 0f),
    VD_IO_NET__TLS_EXTENSION_KIND_APPLICATION_LAYER_PROTOCOL_NEGOTIATION = VD_IO_NET__BE16(00, 10),
    VD_IO_NET__TLS_EXTENSION_KIND_SIGNED_CERTIFICATE_TIMESTAMP           = VD_IO_NET__BE16(00, 12),
    VD_IO_NET__TLS_EXTENSION_KIND_CLIENT_CERTIFICATE_TYPE                = VD_IO_NET__BE16(00, 13),
    VD_IO_NET__TLS_EXTENSION_KIND_SERVER_CERTIFICATE_TYPE                = VD_IO_NET__BE16(00, 14),
    VD_IO_NET__TLS_EXTENSION_KIND_PADDING                                = VD_IO_NET__BE16(00, 15),
    VD_IO_NET__TLS_EXTENSION_KIND_PRE_SHARED_KEY                         = VD_IO_NET__BE16(00, 29),
    VD_IO_NET__TLS_EXTENSION_KIND_EARLY_DATA                             = VD_IO_NET__BE16(00, 2a),
    VD_IO_NET__TLS_EXTENSION_KIND_SUPPORTED_VERSIONS                     = VD_IO_NET__BE16(00, 2b),
    VD_IO_NET__TLS_EXTENSION_KIND_COOKIE                                 = VD_IO_NET__BE16(00, 2c),
    VD_IO_NET__TLS_EXTENSION_KIND_PSK_KEY_EXCHANGE_MODES                 = VD_IO_NET__BE16(00, 2d),
    VD_IO_NET__TLS_EXTENSION_KIND_CERTIFICATE_AUTHORITIES                = VD_IO_NET__BE16(00, 2f),
    VD_IO_NET__TLS_EXTENSION_KIND_OID_FILTERS                            = VD_IO_NET__BE16(00, 30),
    VD_IO_NET__TLS_EXTENSION_KIND_POST_HANDSHAKE_AUTH                    = VD_IO_NET__BE16(00, 31),
    VD_IO_NET__TLS_EXTENSION_KIND_SIGNATURE_ALGORITHMS_CERT              = VD_IO_NET__BE16(00, 32),
    VD_IO_NET__TLS_EXTENSION_KIND_KEY_SHARE                              = VD_IO_NET__BE16(00, 33),
};
typedef uint16_t VdIoNet__TlsExtensionKind;

enum VD_IO_NET__TLS_PROTOCOL_VERSION_ {
    VD_IO_NET__TLS_PROTOCOL_VERSION_SSL_3_0 = VD_IO_NET__BE16(03, 00),
    VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_0 = VD_IO_NET__BE16(03, 01),
    VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_1 = VD_IO_NET__BE16(03, 02),
    VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_2 = VD_IO_NET__BE16(03, 03),
    VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_3 = VD_IO_NET__BE16(03, 04),
};
typedef uint16_t VdIoNet__TlsProtocolVersion;

enum VD_IO_NET__TLS_SIGNATURE_SCHEME_ {
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA256       = VD_IO_NET__BE16(04, 01),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA384       = VD_IO_NET__BE16(05, 01),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA512       = VD_IO_NET__BE16(06, 01),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ECDSA_SECP256R1_SHA256 = VD_IO_NET__BE16(04, 03),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ECDSA_SECP384R1_SHA384 = VD_IO_NET__BE16(05, 03),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ECDSA_SECP521R1_SHA512 = VD_IO_NET__BE16(06, 03),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256    = VD_IO_NET__BE16(08, 04),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA384    = VD_IO_NET__BE16(08, 05),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA512    = VD_IO_NET__BE16(08, 06),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ED25519                = VD_IO_NET__BE16(08, 07),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ED448                  = VD_IO_NET__BE16(08, 08),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_PSS_SHA256     = VD_IO_NET__BE16(08, 09),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_PSS_SHA384     = VD_IO_NET__BE16(08, 0a),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_PSS_SHA512     = VD_IO_NET__BE16(08, 0b),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA1         = VD_IO_NET__BE16(02, 01),
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ECDSA_SHA1             = VD_IO_NET__BE16(02, 03),
};
typedef uint16_t VdIoNet__TlsSignatureScheme;

enum VD_IO_NET__TLS_EC_POINT_FORMAT_ {
    VD_IO_NET__TLS_EC_POINT_FORMAT_UNCOMPRESSED              = 0,
    VD_IO_NET__TLS_EC_POINT_FORMAT_ANSIX962_COMPRESSED_PRIME = 1,
    VD_IO_NET__TLS_EC_POINT_FORMAT_ANSIX962_COMPRESSED_CHAR2 = 2,
};
typedef uint8_t VdIoNet__TlsEcPointFormat;

enum VD_IO_NET__TLS_NAMED_GROUP_ {
    VD_IO_NET__TLS_NAMED_GROUP_SECP256R1 = VD_IO_NET__BE16(00, 17),
    VD_IO_NET__TLS_NAMED_GROUP_SECP384R1 = VD_IO_NET__BE16(00, 18),
    VD_IO_NET__TLS_NAMED_GROUP_SECP521R1 = VD_IO_NET__BE16(00, 19),
    VD_IO_NET__TLS_NAMED_GROUP_X25519    = VD_IO_NET__BE16(00, 1d),
    VD_IO_NET__TLS_NAMED_GROUP_X448      = VD_IO_NET__BE16(00, 1e),
};
typedef uint16_t VdIoNet__TlsNamedGroup;

enum VD_IO_NET__TLS_PSK_KEY_EXCHANGE_MODE_ {
    VD_IO_NET__TLS_PSK_KEY_EXCHANGE_MODE_PSK_KE     = 0,
    VD_IO_NET__TLS_PSK_KEY_EXCHANGE_MODE_PSK_DHE_KE = 1,
};
typedef uint8_t VdIoNet__TlsPskKeyExchangeMode;

#pragma pack(push, 1)
typedef struct {
    VdIoNet__TlsRecordType                       type;
    VdIoNet__TlsProtocolVersion                  version;
    uint16_t                                     len;
} VdIoNet__TlsRecordHeader;

typedef struct {
    VdIoNet__TlsHandshakeType                    type;
    uint16_t                                     len;
} VdIoNet__TlsHandshakeHeader;

typedef struct {
    uint8_t                                      size;
    VdIoNet__TlsProtocolVersion                  versions[1];
} VdIoNet__Tls13ClientSupportedVersions;

typedef struct {
    uint16_t                                     size;
    VdIoNet__TlsSignatureScheme                  schemes[2];
} VdIoNet__Tls13ClientSignatureSchemeList;

typedef struct {
    uint8_t                                      size;
    VdIoNet__TlsEcPointFormat                    formats[1];
} VdIoNet__Tls13ClientHelloEcPointFormats;

typedef struct {
    uint16_t                                     size;
    VdIoNet__TlsNamedGroup                       groups[1];
} VdIoNet__Tls13ClientHelloSupportedGroups;

typedef struct {
    uint8_t                                      size;
    VdIoNet__TlsPskKeyExchangeMode               modes[1];
} VdIoNet__Tls13ClientHelloPskExchangeModes;

typedef struct {
    VdIoNet__TlsNamedGroup                       group;
    uint16_t                                     len;
    uint8_t                                      pk[32];
} VdIoNet__Tls13ClientHelloKeyShareEntryX25519;

typedef struct {
    uint16_t                                     len;
    VdIoNet__Tls13ClientHelloKeyShareEntryX25519 entry;
} VdIoNet__Tls13ClientHelloKeyShare;

typedef struct {
    VdIoNet__TlsExtensionKind                    kind;
    uint16_t                                     len;
} VdIoNet__TlsServerNamePreamble;

typedef struct {
    uint8_t                                      type;
    uint16_t                                     name_len;
} VdIoNet__TlsServerNameEntryPreamble;

typedef struct {
    VdIoNet__TlsProtocolVersion              legacy_version;
    uint8_t                                  random[32];
    uint8_t                                  session_id_len;
    uint8_t                                  session_id[32];
    uint16_t                                 cipher_suites_len;
    VdIoNet__TlsCipherSuite                  cipher_suites[2];
    uint8_t                                  legacy_compression_methods_len;
    uint8_t                                  legacy_compression_methods[1];
    uint16_t                                 extensions_len;
} VdIoNet__Tls13ClientHello;

typedef struct {
    VdIoNet__TlsExtensionKind                   supported_versions_kind;
    uint16_t                                    supported_versions_len;
    VdIoNet__Tls13ClientSupportedVersions       supported_versions;

    VdIoNet__TlsExtensionKind                   signature_algorithms_kind;
    uint16_t                                    signature_algorithms_len;
    VdIoNet__Tls13ClientSignatureSchemeList     signature_algorithms;

    VdIoNet__TlsExtensionKind                   ec_point_formats_kind;
    uint16_t                                    ec_point_formats_len;
    VdIoNet__Tls13ClientHelloEcPointFormats     ec_point_formats;

    VdIoNet__TlsExtensionKind                   supported_groups_kind;
    uint16_t                                    supported_groups_len;
    VdIoNet__Tls13ClientHelloSupportedGroups    supported_groups;

    VdIoNet__TlsExtensionKind                   psk_key_exchange_modes_kind;
    uint16_t                                    psk_key_exchange_modes_len;
    VdIoNet__Tls13ClientHelloPskExchangeModes   psk_key_exchange_modes;

    VdIoNet__TlsExtensionKind                   key_share_kind;
    uint16_t                                    key_share_len;
    VdIoNet__Tls13ClientHelloKeyShare           key_share;
} VdIoNet__Tls13ClientHelloExtensions;
#pragma pack(pop)

/*
9.2.  Mandatory-to-Implement Extensions

   In the absence of an application profile standard specifying
   otherwise, a TLS-compliant application MUST implement the following
   TLS extensions:

   -  Supported Versions ("supported_versions"; Section 4.2.1)

   -  Cookie ("cookie"; Section 4.2.2)

   -  Signature Algorithms ("signature_algorithms"; Section 4.2.3)

   -  Signature Algorithms Certificate ("signature_algorithms_cert";
      Section 4.2.3)

   -  Negotiated Groups ("supported_groups"; Section 4.2.7)

   -  Key Share ("key_share"; Section 4.2.8)

   -  Server Name Indication ("server_name"; Section 3 of [RFC6066])

   All implementations MUST send and use these extensions when offering
   applicable features:


   -  "pre_shared_key" is REQUIRED for PSK key agreement.

   -  "psk_key_exchange_modes" is REQUIRED for PSK key agreement.
*/

static VdIoNet__TlsRecordHeader Vd_Io_Net__Tls_Client_Hello_Record_Header_Proto = {
    /* type = */ VD_IO_NET__TLS_RECORD_TYPE_HANDSHAKE,
    /* version = */ VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_0,
    /* len = */ VD_IO_NET__BE16(00, 00),
};

static VdIoNet__TlsHandshakeHeader Vd_Io_Net__Tls_Client_Hello_Handshake_Header_Proto = {
    /* type = */ VD_IO_NET__TLS_HANDSHAKE_TYPE_CLIENT_HELLO,
    /* len = */ VD_IO_NET__BE16(00, 00),
};

static VdIoNet__Tls13ClientHello Vd_Io_Net__Tls_Client_Hello_Proto = {
    /* legacy_version = */ VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_2,
    /* random = */ {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
    },
    /* session_id_len = */ 32,
    /* session_id = */ {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
    },
    /* cipher_suites_len = */ VD_IO_NET__BE16(00, 04),
    /* cipher_suites = */ {
        VD_IO_NET__TLS_CIPHER_SUITE_AES_128_GCM_SHA256, 
        VD_IO_NET__TLS_CIPHER_SUITE_AES_256_GCM_SHA384, 
    },
    /* legacy_compression_methods_len = */ 1,
    /* legacy_compression_methods = */ {0},
    /* extensions_len = sizeof(VdIoNet__Tls13ClientHelloExtensions) */ VD_IO_NET__BE16(00, 4F),
};

static VdIoNet__Tls13ClientHelloExtensions Vd_Io_Net__Tls_Client_Hello_Extensions_Proto = {
    /* supported_versions_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_SUPPORTED_VERSIONS,
    /* supported_versions_len = sizeof(VdIoNet__Tls13ClientSupportedVersions) */ VD_IO_NET__BE16(00, 03),
    /* supported_versions = */ {
        /* size = */ sizeof(VdIoNet__TlsProtocolVersion) * 1,
        /* versions = */ {
            VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_3, 
        },
    },

    /* signature_algorithms_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_SIGNATURE_ALGORITHMS,
    /* signature_algorithms_len = sizeof(VdIoNet__Tls13ClientSignatureSchemeList) */ VD_IO_NET__BE16(00, 06),
    /* signature_algorithms = */ {
        /* size = sizeof(VdIoNet__TlsSignatureScheme) * 2 */ VD_IO_NET__BE16(00, 04),
        /* schemes = */ {
            VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA256,
            VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256,
        },
    },

    /* ec_point_formats_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_EC_POINT_FORMATS,
    /* ec_point_formats_len = sizeof(VdIoNet__Tls13ClientHelloEcPointFormats) */ VD_IO_NET__BE16(00, 02),
    /* ec_point_formats = */ {
        /* size = */ sizeof(VdIoNet__TlsEcPointFormat) * 1,
        /* formats = */ {
            VD_IO_NET__TLS_EC_POINT_FORMAT_UNCOMPRESSED,
        },
    },

    /* supported_groups_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_SUPPORTED_GROUPS,
    /* supported_groups_len = sizeof(VdIoNet__Tls13ClientHelloSupportedGroups) */ VD_IO_NET__BE16(00, 04),
    /* supported_groups = */ {
        /* size = sizeof(VdIoNet__TlsNamedGroup) * 1 */ VD_IO_NET__BE16(00, 02),
        /* groups = */ {
            VD_IO_NET__TLS_NAMED_GROUP_X25519,
        },
    },

    /* psk_key_exchange_modes_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_PSK_KEY_EXCHANGE_MODES,
    /* psk_key_exchange_modes_len = sizeof(VdIoNet__Tls13ClientHelloPskExchangeModes) */ VD_IO_NET__BE16(00, 02),
    /* psk_key_exchange_modes = */ {
        /* size = */ sizeof(VdIoNet__TlsPskKeyExchangeMode) * 1,
        /* modes = */ {
            VD_IO_NET__TLS_PSK_KEY_EXCHANGE_MODE_PSK_DHE_KE,
        },
    },

    /* key_share_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_KEY_SHARE,
    /* key_share_len = sizeof(VdIoNet__Tls13ClientHelloKeyShare) */ VD_IO_NET__BE16(00, 26),
    /* key_share = */ {
        /* len = sizeof(VdIoNet__Tls13ClientHelloKeyShareEntryX25519) */ VD_IO_NET__BE16(00, 24),
        /* entry = */ {
            /* group = */ VD_IO_NET__TLS_NAMED_GROUP_X25519,
            /* len = */ VD_IO_NET__BE16(00, 20),
            /* pk = */ {
                0, 0, 0, 0, 0, 0, 0, 0, 
                0, 0, 0, 0, 0, 0, 0, 0, 
                0, 0, 0, 0, 0, 0, 0, 0, 
                0, 0, 0, 0, 0, 0, 0, 0, 
            },
        },
    },
};

static VdIoNet__TlsServerNamePreamble Vd_Io_Net__Tls_Client_Hello_Server_Name_Proto = {
    /* kind = */ VD_IO_NET__TLS_EXTENSION_KIND_SERVER_NAME,
    /* len = */ 0, // Filled out later
};

static VdIoNet__TlsServerNameEntryPreamble Vd_Io_Net__Tls_Client_Hello_Server_Name_Entry_Proto = {
    /* type = */ 0, // Server name
    /* name_len = */ 0,
};

static size_t vd_io_net__strlen(const char *s)
{
    size_t result = 0;
    while (*s++) {
        result++;
    }
    return result;
}

static void vd_io_net__size_assertions(void)
{
    VD_IO_NET_ASSERT(sizeof(VdIoNet__Tls13ClientHelloExtensions) == 0x004F);
    VD_IO_NET_ASSERT(sizeof(VdIoNet__Tls13ClientSupportedVersions) == 0x0003);
    VD_IO_NET_ASSERT(sizeof(VdIoNet__Tls13ClientSignatureSchemeList) == 0x0006);
    VD_IO_NET_ASSERT((sizeof(VdIoNet__TlsSignatureScheme) * 2) == 0x0004);
    VD_IO_NET_ASSERT(sizeof(VdIoNet__Tls13ClientHelloEcPointFormats) == 0x0002);
    VD_IO_NET_ASSERT(sizeof(VdIoNet__Tls13ClientHelloSupportedGroups) == 0x0004);
    VD_IO_NET_ASSERT((sizeof(VdIoNet__TlsNamedGroup) * 1) == 0x0002);
    VD_IO_NET_ASSERT(sizeof(VdIoNet__Tls13ClientHelloPskExchangeModes) == 0x0002);
    VD_IO_NET_ASSERT(sizeof(VdIoNet__Tls13ClientHelloKeyShare) == 0x0026);
    VD_IO_NET_ASSERT(sizeof(VdIoNet__Tls13ClientHelloKeyShareEntryX25519) == 0x0024);
}

static VD_IO_CALLBACK(vd_io_net_tls__on_connect);
static VD_IO_CALLBACK(vd_io_net_tls__on_client_hello_sent);
static VD_IO_CALLBACK(vd_io_net__on_client_hello_recvd);

static size_t vd_io_net__tls_client_hello_size(VdIoNetTlsClientInfo *info)
{
    if (info->endpoint.domain_len == 0) {
        info->endpoint.domain_len = (int)vd_io_net__strlen(info->endpoint.domain);
    }

    size_t result = 0;

    result += sizeof(VdIoNet__TlsRecordHeader);

    result += sizeof(VdIoNet__TlsHandshakeHeader);

    result += sizeof(VdIoNet__Tls13ClientHello);
    result += sizeof(VdIoNet__Tls13ClientHelloExtensions);

    result += sizeof(VdIoNet__TlsServerNamePreamble);
    result += sizeof(VdIoNet__TlsServerNameEntryPreamble);
    result += info->endpoint.domain_len;

    return result;
}

VD_IO_NET_API size_t vd_io_net_tls_client_info_mem_size(VdIoNetTlsClientInfo *info)
{
    return vd_io_net__tls_client_hello_size(info);
}

VD_IO_NET_API VdIoErr vd_io_net_tls_connect(VdIo *io, VdIoNetTlsClientInfo *info, VdIoNetTls *tls,
                                            VdIoCallback *cb, void *usr)
{
    vd_io_net__size_assertions();

    VdIoErr err;
    size_t hello_size, total_size;
    size_t offset;

    if (info->endpoint.domain_len == 0) {
        info->endpoint.domain_len = (int)vd_io_net__strlen(info->endpoint.domain);
    }

    tls->io = io;
    tls->state = VD_IO_NET_TLS_STATE_PROCESSING;

    err = VD_IO_ERR_OK;

    total_size = vd_io_net_tls_client_info_mem_size(info);
    hello_size = vd_io_net__tls_client_hello_size(info);

    if (info->memory_size < total_size) {
        goto END;
    }

    tls->buf = (uint8_t*)info->memory;
    tls->bufsize = info->memory_size;
    tls->client_hello_size = hello_size;
    tls->on_connect_cb.cb = cb;
    tls->on_connect_cb.usr = usr;
    tls->next_handshake_state = VD_IO_NET__TLS_HANDSHAKE_TYPE_CLIENT_HELLO;

    VD_IO_NET_RANDOM(tls->private_key, 32);

    vd_io_net__c25519_scalar_mult()

    offset = 0;

    VdIoNet__TlsRecordHeader *hello_hdr = (VdIoNet__TlsRecordHeader*)VD_IO_MEMCPY(tls->buf + offset,
                                                                                  &Vd_Io_Net__Tls_Client_Hello_Record_Header_Proto,
                                                                                  sizeof(Vd_Io_Net__Tls_Client_Hello_Record_Header_Proto));
    offset += sizeof(Vd_Io_Net__Tls_Client_Hello_Record_Header_Proto);
    hello_hdr->len = VD_IO_NET__SWAP16_BE((uint16_t)(hello_size - offset));


    VdIoNet__TlsHandshakeHeader *hello_hndsk = (VdIoNet__TlsHandshakeHeader*)VD_IO_MEMCPY(tls->buf + offset,
                                                                                          &Vd_Io_Net__Tls_Client_Hello_Handshake_Header_Proto,
                                                                                          sizeof(Vd_Io_Net__Tls_Client_Hello_Handshake_Header_Proto));
    offset += sizeof(Vd_Io_Net__Tls_Client_Hello_Handshake_Header_Proto);
    hello_hndsk->len = VD_IO_NET__SWAP16_BE((uint16_t)(hello_size - offset));

    VdIoNet__Tls13ClientHello *hello = (VdIoNet__Tls13ClientHello*)VD_IO_MEMCPY(tls->buf + offset, 
                                                                                &Vd_Io_Net__Tls_Client_Hello_Proto, 
                                                                                sizeof(Vd_Io_Net__Tls_Client_Hello_Proto));
    offset += sizeof(Vd_Io_Net__Tls_Client_Hello_Proto);
    hello->extensions_len = VD_IO_NET__SWAP16_BE((uint16_t)(hello_size - offset));

    VdIoNet__Tls13ClientHelloExtensions *extensions = (VdIoNet__Tls13ClientHelloExtensions*)VD_IO_MEMCPY(tls->buf + offset, 
                                                                                                         &Vd_Io_Net__Tls_Client_Hello_Extensions_Proto,
                                                                                                         sizeof(Vd_Io_Net__Tls_Client_Hello_Extensions_Proto));
    offset += sizeof(Vd_Io_Net__Tls_Client_Hello_Extensions_Proto);

    VdIoNet__TlsServerNamePreamble *server_name_pre = (VdIoNet__TlsServerNamePreamble*)VD_IO_MEMCPY(tls->buf + offset, 
                                                                                                    &Vd_Io_Net__Tls_Client_Hello_Server_Name_Proto,
                                                                                                    sizeof(Vd_Io_Net__Tls_Client_Hello_Server_Name_Proto));
    offset += sizeof(Vd_Io_Net__Tls_Client_Hello_Server_Name_Proto);
    server_name_pre->len = VD_IO_NET__SWAP16_BE((uint16_t)(hello_size - offset));

    VdIoNet__TlsServerNameEntryPreamble *server_name_entry = (VdIoNet__TlsServerNameEntryPreamble*)VD_IO_MEMCPY(tls->buf + offset, 
                                                                                                                &Vd_Io_Net__Tls_Client_Hello_Server_Name_Entry_Proto,
                                                                                                                sizeof(Vd_Io_Net__Tls_Client_Hello_Server_Name_Entry_Proto));
    offset += sizeof(Vd_Io_Net__Tls_Client_Hello_Server_Name_Entry_Proto);
    server_name_entry->name_len = VD_IO_NET__SWAP16_BE((uint16_t)(hello_size - offset));

    VD_IO_MEMCPY(tls->buf + offset, info->endpoint.domain, info->endpoint.domain_len);

    {
        VdIoErr connect_err = vd_io_connect(tls->io, info->endpoint.domain, info->endpoint.port,
                                            VD_IO_NET_TCP, &tls->socket,
                                            vd_io_net_tls__on_connect, (void*)tls);
        if (connect_err != VD_IO_ERR_OK) {
            err = connect_err;
            goto END;
        }
    }

END:
    return err;
}

static VD_IO_CALLBACK(vd_io_net_tls__on_connect)
{
    VdIoNetTls *tls = (VdIoNetTls*)usr;

    if (evt->err != VD_IO_ERR_OK) {
        // @todo(mdodis): invoke callback on target
        return;
    }


    vd_io_hn_send(io, tls->socket, tls->buf, tls->client_hello_size, vd_io_net_tls__on_client_hello_sent, (void*)tls);
}

static VD_IO_CALLBACK(vd_io_net_tls__on_client_hello_sent)
{
    VdIoNetTls *tls = (VdIoNetTls*)usr;
    if (evt->err != VD_IO_ERR_OK) {
        printf("Client Hello send failed\n");
        return;
    }

    vd_io_hn_recv(io, evt->data.send.receiver, tls->buf, tls->bufsize, vd_io_net__on_client_hello_recvd, (void*)tls);
}

static VD_IO_CALLBACK(vd_io_net__on_client_hello_recvd)
{
    VdIoNetTls *tls = (VdIoNetTls*)usr;
    if (evt->err != VD_IO_ERR_OK) {
        printf("Server Hello recv failed\n");
        return;
    }

    printf("Server hello: %zd bytes transfered.\n", evt->data.recv.buffer_written);
}

#endif // VD_IO_NET_IMPL