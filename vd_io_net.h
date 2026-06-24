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
    VdIo                      *io;
    VdIoNetTlsState           state;
    VdIoHn                    socket;

    // Internal
    VdIoCallbackInfo          on_connect_cb;
    VdIoNet__TlsHandshakeType next_handshake_state;
} VdIoNetTls;

VD_IO_NET_API VdIoErr    vd_io_net_tls_connect(VdIo *io, const char *ip, int port, VdIoNetTls *tls,
                                               VdIoCallback *cb, void *usr);

#endif // !VD_IO_NET_H


#ifdef VD_IO_NET_IMPL
#ifndef VD_IO_H
#error "vd_io_net.h requires vd_io.h. Please include it before including this"
#endif // !VD_IO_H

#ifndef VD_IO_NET_SHA256
#   define VD_IO_NET_SHA256(bytes, len, out) vd_io_net__sha256(bytes, len, out)
#endif // !VD_IO_NET_SHA256

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
    VD_IO_NET__TLS_HANDSHAKE_TYPE_HELLO_REQUEST_RESERVED = 0,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CLIENT_HELLO = 1,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_SERVER_HELLO = 2,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_HELLO_VERIFY_REQUEST_RESERVED = 3,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_NEW_SESSION_TICKET = 4,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_END_OF_EARLY_DATA = 5,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_HELLO_RETRY_REQUEST_RESERVED = 6,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_ENCRYPTED_EXTENSIONS = 8,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_SERVER_KEY_EXCHANGE_RESERVED = 12,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CERTIFICATE_REQUEST = 13,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_SERVER_HELLO_DONE_RESERVED = 14,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CERTIFICATE_VERIFY = 15,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CLIENT_KEY_EXCHANGE_RESERVED = 16,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_FINISHED = 20,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CERTIFICATE_URL_RESERVED = 21,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_CERTIFICATE_STATUS_RESERVED = 22,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_SUPPLEMENTAL_DATA_RESERVED = 23,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_KEY_UPDATE = 24,
    VD_IO_NET__TLS_HANDSHAKE_TYPE_MESSAGE_HASH = 254,
};

enum VD_IO_NET__TLS_CIPHER_SUITE_ {
    VD_IO_NET__TLS_CIPHER_SUITE_AES_128_GCM_SHA256 = 0x1301,
    VD_IO_NET__TLS_CIPHER_SUITE_AES_256_GCM_SHA384 = 0x1302,
    VD_IO_NET__TLS_CIPHER_SUITE_CHACHA20_POLY1305_SHA256 = 0x1303,
    VD_IO_NET__TLS_CIPHER_SUITE_AES_128_CCM_SHA256 = 0x1304,
    VD_IO_NET__TLS_CIPHER_SUITE_AES_128_CCM_8_SHA256 = 0x1305,
};
typedef uint16_t VdIoNet__TlsCipherSuite;

enum VD_IO_NET__TLS_EXTENSION_KIND_ {
    VD_IO_NET__TLS_EXTENSION_KIND_SERVER_NAME = 0,
    VD_IO_NET__TLS_EXTENSION_KIND_MAX_FRAGMENT_LENGTH = 1,
    VD_IO_NET__TLS_EXTENSION_KIND_STATUS_REQUEST = 5,
    VD_IO_NET__TLS_EXTENSION_KIND_SUPPORTED_GROUPS = 10,
    VD_IO_NET__TLS_EXTENSION_KIND_EC_POINT_FORMATS = 11,
    VD_IO_NET__TLS_EXTENSION_KIND_SIGNATURE_ALGORITHMS = 13,
    VD_IO_NET__TLS_EXTENSION_KIND_USE_SRTP = 14,
    VD_IO_NET__TLS_EXTENSION_KIND_HEARTBEAT = 15,
    VD_IO_NET__TLS_EXTENSION_KIND_APPLICATION_LAYER_PROTOCOL_NEGOTIATION = 16,
    VD_IO_NET__TLS_EXTENSION_KIND_SIGNED_CERTIFICATE_TIMESTAMP = 18,
    VD_IO_NET__TLS_EXTENSION_KIND_CLIENT_CERTIFICATE_TYPE = 19,
    VD_IO_NET__TLS_EXTENSION_KIND_SERVER_CERTIFICATE_TYPE = 20,
    VD_IO_NET__TLS_EXTENSION_KIND_PADDING = 21,
    VD_IO_NET__TLS_EXTENSION_KIND_PRE_SHARED_KEY = 41,
    VD_IO_NET__TLS_EXTENSION_KIND_EARLY_DATA = 42,
    VD_IO_NET__TLS_EXTENSION_KIND_SUPPORTED_VERSIONS = 43,
    VD_IO_NET__TLS_EXTENSION_KIND_COOKIE = 44,
    VD_IO_NET__TLS_EXTENSION_KIND_PSK_KEY_EXCHANGE_MODES = 45,
    VD_IO_NET__TLS_EXTENSION_KIND_CERTIFICATE_AUTHORITIES = 47,
    VD_IO_NET__TLS_EXTENSION_KIND_OID_FILTERS = 48,
    VD_IO_NET__TLS_EXTENSION_KIND_POST_HANDSHAKE_AUTH = 49,
    VD_IO_NET__TLS_EXTENSION_KIND_SIGNATURE_ALGORITHMS_CERT = 50,
    VD_IO_NET__TLS_EXTENSION_KIND_KEY_SHARE = 51,
};
typedef uint16_t VdIoNet__TlsExtensionKind;

enum VD_IO_NET__TLS_PROTOCOL_VERSION_ {
    VD_IO_NET__TLS_PROTOCOL_VERSION_SSL_3_0 = 0x0300,
    VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_0 = 0x0301,
    VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_1 = 0x0302,
    VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_2 = 0x0303,
    VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_3 = 0x0304,
};
typedef uint16_t VdIoNet__TlsProtocolVersion;

enum VD_IO_NET__TLS_SIGNATURE_SCHEME_ {
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA256 = 0x0401,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA384 = 0x0501,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA512 = 0x0601,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ECDSA_SECP256R1_SHA256 = 0x0403,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ECDSA_SECP384R1_SHA384 = 0x0503,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ECDSA_SECP521R1_SHA512 = 0x0603,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256 = 0x0804,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA384 = 0x0805,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA512 = 0x0806,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ED25519 = 0x0807,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ED448 = 0x0808,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_PSS_SHA256 = 0x0809,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_PSS_SHA384 = 0x080a,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_PSS_SHA512 = 0x080b,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA1 = 0x0201,
    VD_IO_NET__TLS_SIGNATURE_SCHEME_ECDSA_SHA1 = 0x0203,
};
typedef uint16_t VdIoNet__TlsSignatureScheme;

enum VD_IO_NET__TLS_EC_POINT_FORMAT_ {
    VD_IO_NET__TLS_EC_POINT_FORMAT_UNCOMPRESSED = 0,
    VD_IO_NET__TLS_EC_POINT_FORMAT_ANSIX962_COMPRESSED_PRIME = 1,
    VD_IO_NET__TLS_EC_POINT_FORMAT_ANSIX962_COMPRESSED_CHAR2 = 2,
};
typedef uint8_t VdIoNet__TlsEcPointFormat;

enum VD_IO_NET__TLS_NAMED_GROUP_ {
    VD_IO_NET__TLS_NAMED_GROUP_SECP256R1 = 23,
    VD_IO_NET__TLS_NAMED_GROUP_SECP384R1 = 24,
    VD_IO_NET__TLS_NAMED_GROUP_SECP521R1 = 25,
    VD_IO_NET__TLS_NAMED_GROUP_X25519 = 29,
    VD_IO_NET__TLS_NAMED_GROUP_X448 = 30,
};
typedef uint16_t VdIoNet__TlsNamedGroup;

enum VD_IO_NET__TLS_PSK_KEY_EXCHANGE_MODE_ {
    VD_IO_NET__TLS_PSK_KEY_EXCHANGE_MODE_PSK_KE = 0,
    VD_IO_NET__TLS_PSK_KEY_EXCHANGE_MODE_PSK_DHE_KE = 1,
};
typedef uint8_t VdIoNet__TlsPskKeyExchangeMode;

#pragma pack(push, 1)
typedef struct {
    VdIoNet__TlsHandshakeType type;
    uint32_t length : 24;
} VdIoNet__TlsHandshakeHeader;
#pragma pack(pop)

static VD_IO_CALLBACK(vd_io_net_tls__on_connect);

VD_IO_NET_API VdIoNetTls vd_io_net_tls_make(VdIo *io, VdIoHn socket)
{
    VdIoNetTls result;
    result.io = io;
    result.state = VD_IO_NET_TLS_STATE_PROCESSING;
    result.socket = socket;
    return result;
}

VD_IO_NET_API VdIoErr vd_io_net_tls_connect(VdIo *io, const char *ip, int port, VdIoNetTls *tls,
                                            VdIoCallback *cb, void *usr)
{
    VdIoErr err;

    tls->io = io;
    tls->state = VD_IO_NET_TLS_STATE_PROCESSING;

    err = VD_IO_ERR_OK;

    tls->on_connect_cb.cb = cb;
    tls->on_connect_cb.usr = usr;
    tls->next_handshake_state = VD_IO_NET__TLS_HANDSHAKE_TYPE_CLIENT_HELLO;
    {
        VdIoErr connect_err = vd_io_connect(tls->io, ip, port, VD_IO_NET_TCP, &tls->socket,
                                            vd_io_net_tls__on_connect, (void*)tls);
        if (connect_err != VD_IO_ERR_OK) {
            err = connect_err;
            goto END;
        }
    }

END:
    return err;
}

#pragma pack(push, 1)
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
    VdIoNet__TlsProtocolVersion              legacy_version;
    uint8_t                                  random[32];
    uint8_t                                  session_id_len;
    // @todo(mdodis): uint8_t session_id[session_id_len]
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

   -  "supported_versions" is REQUIRED for all ClientHello, ServerHello,
      and HelloRetryRequest messages.

   -  "signature_algorithms" is REQUIRED for certificate authentication.

   -  "supported_groups" is REQUIRED for ClientHello messages using DHE
      or ECDHE key exchange.

   -  "key_share" is REQUIRED for DHE or ECDHE key exchange.

   -  "pre_shared_key" is REQUIRED for PSK key agreement.

   -  "psk_key_exchange_modes" is REQUIRED for PSK key agreement.
*/

static VdIoNet__Tls13ClientHello Vd_Io_Net__Tls_Client_Hello_Proto = {
    /* legacy_version = */ VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_2,
    /* random = */ {
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 
    },
    /* session_id_len = */ 0,
    /* cipher_suites_len = */ 2,
    /* cipher_suites = */ {
        VD_IO_NET__TLS_CIPHER_SUITE_AES_128_GCM_SHA256, 
        VD_IO_NET__TLS_CIPHER_SUITE_AES_256_GCM_SHA384, 
    },
    /* legacy_compression_methods_len = */ 1,
    /* legacy_compression_methods = */ {0},
    /* extensions_len = */ sizeof(VdIoNet__Tls13ClientHelloExtensions),
};

static VdIoNet__Tls13ClientHelloExtensions Vd_Io_Net__Tls_Client_Hello_Extensions_Proto = {
    /* supported_versions_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_SUPPORTED_VERSIONS,
    /* supported_versions_len = */ sizeof(VdIoNet__Tls13ClientSupportedVersions),
    /* supported_versions = */ {
        /* size = */ sizeof(VdIoNet__TlsProtocolVersion) * 1,
        /* versions = */ {
            VD_IO_NET__TLS_PROTOCOL_VERSION_TLS_1_3, 
        },
    },

    /* signature_algorithms_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_SIGNATURE_ALGORITHMS,
    /* signature_algorithms_len = */ sizeof(VdIoNet__Tls13ClientSignatureSchemeList),
    /* signature_algorithms = */ {
        /* size = */ sizeof(VdIoNet__TlsSignatureScheme) * 2,
        /* schemes = */ {
            VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PKCS1_SHA256,
            VD_IO_NET__TLS_SIGNATURE_SCHEME_RSA_PSS_RSAE_SHA256,
        },
    },

    /* ec_point_formats_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_EC_POINT_FORMATS,
    /* ec_point_formats_len = */ sizeof(VdIoNet__Tls13ClientHelloEcPointFormats),
    /* ec_point_formats = */ {
        /* size = */ sizeof(VdIoNet__TlsEcPointFormat) * 1,
        /* formats = */ {
            VD_IO_NET__TLS_EC_POINT_FORMAT_UNCOMPRESSED,
        },
    },

    /* supported_groups_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_SUPPORTED_GROUPS,
    /* supported_groups_len = */ sizeof(VdIoNet__Tls13ClientHelloSupportedGroups),
    /* supported_groups = */ {
        /* size = */ sizeof(VdIoNet__TlsNamedGroup) * 1,
        /* groups = */ {
            VD_IO_NET__TLS_NAMED_GROUP_X25519,
        },
    },

    /* psk_key_exchange_modes_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_PSK_KEY_EXCHANGE_MODES,
    /* psk_key_exchange_modes_len = */ sizeof(VdIoNet__Tls13ClientHelloPskExchangeModes),
    /* psk_key_exchange_modes = */ {
        /* size = */ sizeof(VdIoNet__TlsPskKeyExchangeMode) * 1,
        /* modes = */ {
            VD_IO_NET__TLS_PSK_KEY_EXCHANGE_MODE_PSK_DHE_KE,
        },
    },

    /* key_share_kind = */ VD_IO_NET__TLS_EXTENSION_KIND_KEY_SHARE,
    /* key_share_len = */ sizeof(VdIoNet__Tls13ClientHelloKeyShare),
    /* key_share = */ {
        /* len = */ sizeof(VdIoNet__Tls13ClientHelloKeyShareEntryX25519),
        /* entry = */ {
            /* group = */ VD_IO_NET__TLS_NAMED_GROUP_X25519,
            /* len = */ 32,
            /* pk = */ {
                0, 0, 0, 0, 0, 0, 0, 0, 
                0, 0, 0, 0, 0, 0, 0, 0, 
                0, 0, 0, 0, 0, 0, 0, 0, 
                0, 0, 0, 0, 0, 0, 0, 0, 
            },
        },
    },

};

static VD_IO_CALLBACK(vd_io_net_tls__on_connect)
{
    VdIoNetTls *tls = (VdIoNetTls*)usr;

    if (evt->err != VD_IO_ERR_OK) {
        // @todo(mdodis): invoke callback on target
        return;
    }

}


#endif // VD_IO_NET_IMPL