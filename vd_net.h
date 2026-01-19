/*
 * vd_net.h - Networking library
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
 * TODO
 */
#ifndef VD_NET_H
#define VD_NET_H
#define VD_NET_VERSION_MAJOR    0
#define VD_NET_VERSION_MINOR    0
#define VD_NET_VERSION_PATCH    1
#define VD_NET_VERSION          ((VD_NET_VERSION_MAJOR << 16) | (VD_NET_VERSION_MINOR << 8) | (VD_NET_VERSION_PATCH))

#ifndef VD_NET_API
#   ifdef VD_NET_STATIC
#       define VD_NET_API static
#   else
#       define VD_NET_API extern
#   endif // VD_NET_STATIC
#endif // !VD_NET_API

#ifndef VD_NET_INL
#   define VD_NET_INL static inline
#endif // !VD_NET_INL

#include <stddef.h>
#include <stdint.h>

enum {
    VD_NET_AF_UNSPEC = 0,
    VD_NET_AF_UNIX = 1,
    VD_NET_AF_INET = 2,
    VD_NET_AF_INET6 = 23,

    VD_NET_SOCK_STREAM = 1,
    VD_NET_SOCK_DGRAM = 2,
    VD_NET_SOCK_RAW = 3,
    VD_NET_SOCK_RDM = 4,
    VD_NET_SOCK_SEQPACKET = 5,

    VD_NET_AI_PASSIVE = 0x00000001,
    VD_NET_AI_CANONNAME = 0x00000002,
    VD_NET_AI_NUMERICHOST = 0x00000004,
    VD_NET_AI_NUMERICSERV = 0x00000008,
    VD_NET_AI_DNS_ONLY = 0x00000010,
    VD_NET_AI_FORCE_CLEAR_TEXT = 0x00000020,
    VD_NET_AI_BYPASS_DNS_CACHE = 0x00000040,
    VD_NET_AI_RETURN_TTL = 0x00000080,

    VD_NET_INET_ADDRSTRLEN = 22,
    VD_NET_INET6_ADDRSTRLEN = 65, 

    VD_NET_PF_INET = VD_NET_AF_INET,
    VD_NET_PF_INET6 = VD_NET_AF_INET6,
};

#pragma pack(push, 4)

typedef struct {
    uint32_t                s_addr;
} VdNetAddrIn;

typedef struct {
    uint8_t                 s6_addr[16];
} VdNetAddrIn6;

typedef struct {
   unsigned short           sa_family;
   char                     sa_data[14];
} VdNetSockAddr;

typedef struct {
    short                   sin_family;
    unsigned short          sin_port;
    VdNetAddrIn             sin_addr;
    char                    sin_zero[8];
} VdNetSockAddrIn;

typedef struct {
    short                   sin6_family;
    unsigned short          sin6_port;
    unsigned long           sin6_flowinfo;
    VdNetAddrIn6            sin6_addr;
    unsigned long           sin6_scope_id;
} VdNetSockAddrIn6;

typedef struct VdNetAddrInfo {
    int                     ai_flags;       // AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST
    int                     ai_family;      // PF_xxx
    int                     ai_socktype;    // SOCK_xxx
    int                     ai_protocol;    // 0 or IPPROTO_xxx for IPv4 and IPv6
    size_t                  ai_addrlen;     // Length of ai_addr
    char                    *ai_canonname;  // Canonical name for nodename
    VdNetSockAddr           *ai_addr;       // Binary address
    struct VdNetAddrInfo    *ai_next;       // Next structure in linked list
} VdNetAddrInfo;
#pragma pack(pop)

#ifdef _WIN32
typedef uintptr_t VdNetSocket;
#else
typedef int       VdNetSocket;
#endif // _WIN32, else

VD_NET_API int          vd_net_getaddrinfo(const char *node, const char *service, const VdNetAddrInfo *hints, VdNetAddrInfo **res);
VD_NET_API void         vd_net_freeaddrinfo(VdNetAddrInfo *first);
VD_NET_API const char*  vd_net_inet_ntop(int af, const void *restrict src, char *dst, size_t size);

#endif // !VD_NET_H

#ifdef VD_NET_IMPL

#ifdef _WIN32
#pragma pack(push, 4)

#define VD_NET_DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

#ifndef _MINWINDEF_
typedef unsigned long        VdNetDWORD;
typedef int                  VdNetBOOL;
typedef unsigned char        VdNetBYTE;
typedef unsigned short       VdNetWORD;
typedef float                VdNetFLOAT;
typedef VdNetFLOAT*          VdNetPFLOAT;
typedef VdNetBOOL*           VdNetPBOOL;
typedef VdNetBOOL*           VdNetLPBOOL;
typedef VdNetBYTE*           VdNetPBYTE;
typedef VdNetBYTE*           VdNetLPBYTE;
typedef int*                 VdNetPINT;
typedef int*                 VdNetLPINT;
typedef VdNetWORD*           VdNetPWORD;
typedef VdNetWORD*           VdNetLPWORD;
typedef long*                VdNetLPLONG;
typedef VdNetDWORD*          VdNetPDWORD;
typedef VdNetDWORD*          VdNetLPDWORD;
typedef void*                VdNetLPVOID;
typedef const void*          VdNetLPCVOID;
typedef int                  VdNetINT;
typedef unsigned int         VdNetUINT;
typedef unsigned int*        VdNetPUINT;
typedef void*                VdNetHANDLE;
typedef VdNetHANDLE*         VdNetPHANDLE;
typedef char                 VdNetCHAR;
typedef const VdNetCHAR*     VdNetLPCSTR, *VdNetPCSTR;
typedef VdNetCHAR*           VdNetPSTR;
typedef VdNetCHAR*           VdNetLPSTR;
typedef long                 VdNetLONG;
typedef unsigned long        VdNetULONG;
typedef VdNetULONG*          VdNetPULONG;
typedef unsigned short       VdNetUSHORT;
typedef VdNetUSHORT*         VdNetPUSHORT;
typedef unsigned char        VdNetUCHAR;
typedef VdNetUCHAR*          VdNetPUCHAR;
typedef VdNetBYTE            VdNetBOOLEAN;
typedef VdNetCHAR*           VdNetPCHAR, * VdNetLPCH, * VdNetPCH;
typedef long                 VdNetHRESULT;
typedef __int64              VdNetLONGLONG;
typedef unsigned __int64     VdNetULONGLONG;
typedef VdNetULONGLONG       VdNetDWORDLONG;
typedef VdNetDWORDLONG*      VdNetPDWORDLONG;
typedef __int64              VdNetINT_PTR, * VdNetPINT_PTR;
typedef unsigned __int64     VdNetUINT_PTR, * VdNetPUINT_PTR;
typedef __int64              VdNetLONG_PTR, * VdNetPLONG_PTR;
typedef unsigned __int64     VdNetULONG_PTR, * VdNetPULONG_PTR;
typedef VdNetUINT_PTR        VdNetWPARAM;
typedef VdNetLONG_PTR        VdNetLPARAM;
typedef VdNetLONG_PTR        VdNetLRESULT;
typedef wchar_t              VdNetWCHAR;
typedef const VdNetWCHAR*    VdNetLPCWSTR, * VdNetPCWSTR;
typedef VdNetWCHAR*          VdNetLPWSTR;
typedef VdNetWORD            VdNetATOM;
typedef VdNetULONG_PTR       VdNetDWORD_PTR, * VdNetPDWORD_PTR;
typedef VdNetULONG_PTR       VdNetSIZE_T;
typedef short                VdNetSHORT;
typedef VdNetDWORD           VdNetCOLORREF;
typedef VdNetDWORD*          VdNetLPCOLORREF;

VD_NET_DECLARE_HANDLE(VdNetHWND);
VD_NET_DECLARE_HANDLE(VdNetHINSTANCE);
VD_NET_DECLARE_HANDLE(VdNetHDC);
VD_NET_DECLARE_HANDLE(VdNetHMONITOR);
VD_NET_DECLARE_HANDLE(VdNetHGDIOBJ);
VD_NET_DECLARE_HANDLE(VdNetHBITMAP);
typedef VdNetHINSTANCE VdNetHMODULE;
#else
#endif // !_MINWINDEF_
typedef VdNetUINT_PTR        VdNetSOCKET;

typedef struct VdNet__sockaddr {
    unsigned short sa_family;    // Address family.
    char           sa_data[14];  // Up to 14 bytes of direct address.
} VdNet__SOCKADDR, *VdNet__PSOCKADDR, *VdNet__LPSOCKADDR;

typedef struct VdNet__AddrInfo {
    int                     ai_flags;       // AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST
    int                     ai_family;      // PF_xxx
    int                     ai_socktype;    // SOCK_xxx
    int                     ai_protocol;    // 0 or IPPROTO_xxx for IPv4 and IPv6
    size_t                  ai_addrlen;     // Length of ai_addr
    char                    *ai_canonname;  // Canonical name for nodename
    struct VdNet__SOCKADDR  *ai_addr;       // Binary address
    struct VdNet__AddrInfo  *ai_next;       // Next structure in linked list
} VdNet__ADDRINFOA, *VdNet__PADDRINFOA;

#define VD_NET__WSADESCRIPTION_LEN      256
#define VD_NET__WSASYS_STATUS_LEN       128

typedef struct VdNet__WSAData {
        VdNetWORD               wVersion;
        VdNetWORD               wHighVersion;
#ifdef _WIN64
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char *                  lpVendorInfo;
        char                    szDescription[VD_NET__WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[VD_NET__WSASYS_STATUS_LEN+1];
#else
        char                    szDescription[VD_NET__WSADESCRIPTION_LEN+1];
        char                    szSystemStatus[VD_NET__WSASYS_STATUS_LEN+1];
        unsigned short          iMaxSockets;
        unsigned short          iMaxUdpDg;
        char *                  lpVendorInfo;
#endif
} VdNet__WSADATA, *VdNet__LPWSADATA;


typedef VdNetINT (*VdNet__Procgetaddrinfo)(VdNetPCSTR pNodeName, VdNetPCSTR pServiceName, const VdNet__ADDRINFOA *pHints, VdNet__PADDRINFOA *ppResult);
static VdNet__Procgetaddrinfo VdNet__getaddrinfo;

typedef void (*VdNet__Procfreeaddrinfo)(VdNet__PADDRINFOA pAddrInfo);
static VdNet__Procfreeaddrinfo VdNet__freeaddrinfo;

typedef VdNetPCSTR (*VdNet__Procinet_ntop)(VdNetINT Family, const void *pAddr, VdNetPCSTR pStringBuf, size_t StringBufSize);
static VdNet__Procinet_ntop VdNet__inet_ntop;

typedef int (*VdNet__ProcWSAStartup)(VdNetWORD wVersionRequired,VdNet__LPWSADATA lpWSAData);
static VdNet__ProcWSAStartup VdNet__WSAStartup;

extern VdNetHMODULE LoadLibraryA(VdNetLPCSTR path);
extern void*        GetProcAddress(VdNetHMODULE hModule, VdNetLPCSTR lpProcName);

#define VD_NET__MAKEWORD(a, b) ((VdNetWORD)(((VdNetBYTE)(((VdNetDWORD_PTR)(a)) & 0xff)) | ((VdNetWORD)((VdNetBYTE)(((VdNetDWORD_PTR)(b)) & 0xff))) << 8))

#pragma pack(pop)

typedef struct {
    int wsa_initialized;
} VdNet__Win32InternalData;

static VdNet__Win32InternalData Vd_Net_G = {0};

static void vd_net__win32_check_init(void);

VD_NET_API int vd_net_getaddrinfo(const char *node, const char *service, const VdNetAddrInfo *hints, VdNetAddrInfo **res)
{
    vd_net__win32_check_init();
    return VdNet__getaddrinfo(node, service, (const VdNet__ADDRINFOA*)hints, (VdNet__PADDRINFOA*)res);
}

VD_NET_API void vd_net_freeaddrinfo(VdNetAddrInfo *first)
{
    vd_net__win32_check_init();
    VdNet__freeaddrinfo((VdNet__PADDRINFOA)first);
}

VD_NET_API const char *vd_net_inet_ntop(int af, const void *restrict src, char *dst, size_t size)
{
    vd_net__win32_check_init();
    return VdNet__inet_ntop(af, (const void*)src, dst, size);
}

static void vd_net__win32_check_init(void)
{
    if (Vd_Net_G.wsa_initialized) {
        return;
    }

    Vd_Net_G.wsa_initialized = 1;


    {
        VdNetHMODULE mod = LoadLibraryA("Ws2_32.dll");
        VdNet__WSAStartup = (VdNet__ProcWSAStartup)GetProcAddress(mod, "WSAStartup");
        VdNet__getaddrinfo = (VdNet__Procgetaddrinfo)GetProcAddress(mod, "getaddrinfo");
        VdNet__freeaddrinfo = (VdNet__Procfreeaddrinfo)GetProcAddress(mod, "freeaddrinfo");
        VdNet__inet_ntop = (VdNet__Procinet_ntop)GetProcAddress(mod, "inet_ntop");
    }

    VdNetWORD version_requested = VD_NET__MAKEWORD(2, 2);
    VdNet__WSADATA data;
    VdNet__WSAStartup(version_requested, &data);

}

#endif // _WIN32

#endif // VD_NET_IMPL
