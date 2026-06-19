/* vd_io.h - IO Library
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
#ifndef VD_IO_H
#define VD_IO_H
#ifndef VD_IO_API
#   ifdef VD_IO_STATIC
#       define VD_IO_API static
#   else
#       define VD_IO_API extern
#   endif // VD_IO_STATIC
#endif // !VD_IO_API

#include <stdint.h>

typedef enum {
    VD_IO_NET_TCP = 0,
    VD_IO_NET_UDP,
} VdIoFlags;

typedef struct {
    VdIoFlags flags;
    uintptr_t    h;
} VdIoHn;

typedef enum {
    VD_IO_ERR_OK = 0,
    VD_IO_ERR_INTERNAL,
    VD_IO_ERR_INVALID_PARAMETER,
} VdIoErr;

typedef enum {
    VD_IO_EVENT_KIND_ACCEPT,
} VdIoEventKind;

typedef struct {
    VdIoHn socket;
} VdIoEventAcceptData;

typedef union {
    VdIoEventAcceptData accept;
} VdIoEventData;

typedef struct {
    VdIoEventKind kind;
    VdIoEventData data;
    VdIoErr       err;
} VdIoEvent;

#define VD_IO_CALLBACK(name) void name(VdIoEvent *evt, void *usr)
typedef VD_IO_CALLBACK(VdIoCallback);

typedef enum {
    VD_IO_OP_KIND_ACCEPT,
} VdIoOpKind;

typedef struct {
    VdIoHn    listener;
    uintptr_t client;
    //            Win32: (dwLocalAddressLength + dwRemoteAddressLength) * 2
    char      buf[(16 + 16) * 2];
} VdIoOpAcceptData;

typedef union {
    VdIoOpAcceptData accept;
} VdIoOpData;

typedef struct {
    volatile int     used;
    VdIoOpKind       kind;
    VdIoOpData       data;
    VdIoCallback     *callback;
    void             *callback_usr;

    // Internal use
    // Win32         OVERLAPPED
    char             internal[32];
} VdIoOp;

typedef struct {
    union {
        struct {
            uintptr_t iocp;
            uintptr_t acceptex;
            uintptr_t getacceptexsockaddrs;
        } win;
    } u;

    volatile int cap_ops;
    volatile int num_ops;
    volatile VdIoOp *ops;
} VdIo;

typedef struct {
    int max_concurrent_ops;
    size_t op_memory_size;
    void *op_memory;
} VdIoInitInfo;

VD_IO_API VdIoInitInfo  vd_io_init_info_default(void);

VD_IO_API size_t        vd_io_init_info_mem_size(VdIoInitInfo *info);
VD_IO_API VdIoErr       vd_io_init(VdIo *io, VdIoInitInfo *info);
VD_IO_API VdIoErr       vd_io_wait(VdIo *io);
VD_IO_API VdIoErr       vd_io_make_socket(VdIo *io, const char *ip, int port, VdIoFlags flags, VdIoHn *hn);
VD_IO_API VdIoErr       vd_io_hn_accept(VdIo *io, VdIoHn hn, VdIoCallback *cb, void *usr);

VD_IO_API VdIoErr vd_io_net_open(VdIoFlags flags, const char *ip, int port, VdIoHn *hn);

#endif // VD_IO_H

#ifdef VD_IO_IMPL

#ifndef VD_IO_MEMCPY
#   include <string.h>
#   define VD_IO_MEMCPY(dst, src, count) memcpy(dst, src, count)
#endif // !VD_IO_MEMCPY

#ifndef VD_IO_MEMSET
#   include <string.h>
#   define VD_IO_MEMSET(dst, val, num) memset(dst, val, num)
#endif // !VD_IO_MEMSET

static int vd_io__count_ops_from_bytes(size_t bytes)
{
    return (int)(bytes / sizeof(VdIoOp));
}

VD_IO_API VdIoInitInfo vd_io_init_info_default(void)
{
    VdIoInitInfo result;
    result.max_concurrent_ops = 128;
    result.op_memory = 0;
    result.op_memory_size = 0;
    return result;
}

VD_IO_API size_t vd_io_init_info_mem_size(VdIoInitInfo *info)
{
    return info->max_concurrent_ops * sizeof(VdIoOp);
}

static int vd_io__make_op(VdIo *io)
{
    int result = -1;
    for (int i = 0; i < io->cap_ops; ++i) {
        if (_InterlockedCompareExchange(&io->ops[i].used, 1, 0) == 0) {
            result = i;
            break;
        }
    }

    _InterlockedIncrement(&io->num_ops);

    return result;
}

static void vd_io__free_op(VdIo *io, VdIoOp *op)
{
    _InterlockedExchange(&op->used, 0);
    _InterlockedDecrement(&io->num_ops);
}

#ifdef _WIN32
#pragma pack(push, 4)

#define VD_IO_DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

#ifndef _MINWINDEF_
typedef unsigned long       VdIoDWORD;
typedef int                 VdIoBOOL;
typedef unsigned char       VdIoBYTE;
typedef unsigned short      VdIoWORD;
typedef float               VdIoFLOAT;
typedef VdIoFLOAT*          VdIoPFLOAT;
typedef VdIoBOOL*           VdIoPBOOL;
typedef VdIoBOOL*           VdIoLPBOOL;
typedef VdIoBYTE*           VdIoPBYTE;
typedef VdIoBYTE*           VdIoLPBYTE;
typedef int*                VdIoPINT;
typedef int*                VdIoLPINT;
typedef VdIoWORD*           VdIoPWORD;
typedef VdIoWORD*           VdIoLPWORD;
typedef long*               VdIoLPLONG;
typedef VdIoDWORD*          VdIoPDWORD;
typedef VdIoDWORD*          VdIoLPDWORD;
typedef void*               VdIoLPVOID;
typedef const void*         VdIoLPCVOID;
typedef int                 VdIoINT;
typedef unsigned int        VdIoUINT;
typedef unsigned int*       VdIoPUINT;
typedef void*               VdIoHANDLE;
typedef VdIoHANDLE*         VdIoPHANDLE;
typedef char                VdIoCHAR;
typedef const VdIoCHAR*     VdIoLPCSTR, *VdIoPCSTR;
typedef VdIoCHAR*           VdIoPSTR;
typedef VdIoCHAR*           VdIoLPSTR;
typedef long                VdIoLONG;
typedef unsigned long       VdIoULONG;
typedef VdIoULONG*          VdIoPULONG;
typedef unsigned short      VdIoUSHORT;
typedef VdIoUSHORT*         VdIoPUSHORT;
typedef unsigned char       VdIoUCHAR;
typedef VdIoUCHAR*          VdIoPUCHAR;
typedef VdIoBYTE            VdIoBOOLEAN;
typedef VdIoCHAR*           VdIoPCHAR, * VdIoLPCH, * VdIoPCH;
typedef long                VdIoHRESULT;
typedef __int64             VdIoLONGLONG;
typedef unsigned __int64    VdIoULONGLONG;
typedef VdIoULONGLONG       VdIoDWORDLONG;
typedef VdIoDWORDLONG*      VdIoPDWORDLONG;
typedef __int64             VdIoINT_PTR, * VdIoPINT_PTR;
typedef unsigned __int64    VdIoUINT_PTR, * VdIoPUINT_PTR;
typedef __int64             VdIoLONG_PTR, * VdIoPLONG_PTR;
typedef unsigned __int64    VdIoULONG_PTR, * VdIoPULONG_PTR;
typedef VdIoUINT_PTR        VdIoWPARAM;
typedef VdIoLONG_PTR        VdIoLPARAM;
typedef VdIoLONG_PTR        VdIoLRESULT;
typedef wchar_t             VdIoWCHAR;
typedef const VdIoWCHAR*    VdIoLPCWSTR, * VdIoPCWSTR;
typedef VdIoWCHAR*          VdIoLPWSTR;
typedef VdIoWORD            VdIoATOM;
typedef VdIoULONG_PTR       VdIoDWORD_PTR, * VdIoPDWORD_PTR;
typedef VdIoULONG_PTR       VdIoSIZE_T;
typedef short               VdIoSHORT;
typedef VdIoDWORD           VdIoCOLORREF;
typedef VdIoDWORD*          VdIoLPCOLORREF;

VD_IO_DECLARE_HANDLE(VdIoHWND);
VD_IO_DECLARE_HANDLE(VdIoHINSTANCE);
VD_IO_DECLARE_HANDLE(VdIoHDC);
VD_IO_DECLARE_HANDLE(VdIoHMONITOR);
VD_IO_DECLARE_HANDLE(VdIoHGDIOBJ);
VD_IO_DECLARE_HANDLE(VdIoHBITMAP);
typedef VdIoHINSTANCE VdIoHMODULE;
#else
#endif // !_MINWINDEF_
typedef VdIoUINT_PTR        VdIoSOCKET;

struct VdIo__in_addr {
  union {
    struct {
      VdIoUCHAR s_b1;
      VdIoUCHAR s_b2;
      VdIoUCHAR s_b3;
      VdIoUCHAR s_b4;
    } S_un_b;
    struct {
      VdIoUSHORT s_w1;
      VdIoUSHORT s_w2;
    } S_un_w;
    VdIoULONG S_addr;
  } S_un;
};

typedef struct VdIo__sockaddr {
    unsigned short sa_family;    // Address family.
    char           sa_data[14];  // Up to 14 bytes of direct address.
} VdIo__SOCKADDR, *VdIo__PSOCKADDR, *VdIo__LPSOCKADDR;

typedef struct VdIo__sockaddr_in {
  short                sin_family;
  unsigned short       sin_port;
  struct VdIo__in_addr sin_addr;
  char                 sin_zero[8];
} VdIo__SOCKADDR_IN, *VdIo__PSOCKADDR_IN, *VdIo__LPSOCKADDR_IN;

typedef struct VdIo__AddrInfo {
    int                     ai_flags;       // AI_PASSIVE, AI_CANONNAME, AI_NUMERICHOST
    int                     ai_family;      // PF_xxx
    int                     ai_socktype;    // SOCK_xxx
    int                     ai_protocol;    // 0 or IPPROTO_xxx for IPv4 and IPv6
    size_t                  ai_addrlen;     // Length of ai_addr
    char                    *ai_canonname;  // Canonical name for nodename
    VdIo__SOCKADDR_IN       *ai_addr;       // Binary address
    struct VdIo__AddrInfo   *ai_next;       // Next structure in linked list
} VdIo__ADDRINFOA, *VdIo__PADDRINFOA;

#define VD_NET__WSADESCRIPTION_LEN      256
#define VD_NET__WSASYS_STATUS_LEN       128

typedef struct VdIo__WSAData {
        VdIoWORD               wVersion;
        VdIoWORD               wHighVersion;
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
} VdIo__WSADATA, *VdIo__LPWSADATA;

typedef struct VdIo_GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} VdIoGUID;

#define VD_IO_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) VdIoGUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }

typedef struct VdIo_OVERLAPPED {
    VdIoULONG_PTR Internal;
    VdIoULONG_PTR InternalHigh;
    union {
        struct {
          VdIoDWORD Offset;
          VdIoDWORD OffsetHigh;
        } DUMMYSTRUCTNAME;
        void* Pointer;
    } DUMMYUNIONNAME;
    VdIoHANDLE    hEvent;
} VdIoOVERLAPPED, *VdIoLPOVERLAPPED;

typedef struct VdIo_WSAOVERLAPPED {
  VdIoDWORD    Internal;
  VdIoDWORD    InternalHigh;
  VdIoDWORD    Offset;
  VdIoDWORD    OffsetHigh;
  VdIoHANDLE   hEvent;
} VdIoWSAOVERLAPPED, *VdIoLPWSAOVERLAPPED;

typedef void (*VdIoLPWSAOVERLAPPED_COMPLETION_ROUTINE)(VdIoDWORD dwError, VdIoDWORD cbTransferred,
                                                     VdIoLPWSAOVERLAPPED lpOverlapped, VdIoDWORD dwFlags);

typedef struct VdIo_WSAPROTOCOLCHAIN {
  int   ChainLen;
  VdIoDWORD ChainEntries[7];
} VdIoWSAPROTOCOLCHAIN, *VdIoLPWSAPROTOCOLCHAIN;

typedef struct VdIo_WSAPROTOCOL_INFOA {
    VdIoDWORD dwServiceFlags1;
    VdIoDWORD dwServiceFlags2;
    VdIoDWORD dwServiceFlags3;
    VdIoDWORD dwServiceFlags4;
    VdIoDWORD dwProviderFlags;
    VdIoGUID ProviderId;
    VdIoDWORD dwCatalogEntryId;
    VdIoWSAPROTOCOLCHAIN ProtocolChain;
    int iVersion;
    int iAddressFamily;
    int iMaxSockAddr;
    int iMinSockAddr;
    int iSocketType;
    int iProtocol;
    int iProtocolMaxOffset;
    int iNetworkByteOrder;
    int iSecurityScheme;
    VdIoDWORD dwMessageSize;
    VdIoDWORD dwProviderReserved;
    char   szProtocol[256];
} VdIo__WSAPROTOCOL_INFOA, *VdIo__LPWSAPROTOCOL_INFOA;

typedef VdIoINT (*VdIo__Procgetaddrinfo)(VdIoPCSTR pNodeName, VdIoPCSTR pServiceName, const VdIo__ADDRINFOA *pHints, 
                                         VdIo__PADDRINFOA *ppResult);
static VdIo__Procgetaddrinfo VdIo__getaddrinfo;

typedef void (*VdIo__Procfreeaddrinfo)(VdIo__PADDRINFOA pAddrInfo);
static VdIo__Procfreeaddrinfo VdIo__freeaddrinfo;

typedef VdIoPCSTR (*VdIo__Procinet_ntop)(VdIoINT Family, const void *pAddr, VdIoPCSTR pStringBuf, size_t StringBufSize);
static VdIo__Procinet_ntop VdIo__inet_ntop;

typedef VdIoINT (*VdIo__Procinet_pton)(VdIoINT Family, VdIoPCSTR pszAddrString, void *pAddrBuf);
static VdIo__Procinet_pton VdIo__inet_pton;

typedef VdIoUSHORT (*VdIo__Prochtons)(VdIoUSHORT s);
static VdIo__Prochtons VdIo__htons;

typedef VdIoSOCKET (*VdIo__Procsocket)(int af, int type, int protocol);
static VdIo__Procsocket VdIo__socket;

typedef VdIoINT (*VdIo__Procbind)(VdIoSOCKET s, const VdIo__SOCKADDR *addr, int namelen);
static VdIo__Procbind VdIo__bind;

typedef int (*VdIo__Proclisten)(VdIoSOCKET s, int backlog);
static VdIo__Proclisten VdIo__listen;

typedef int (*VdIo__Procclosesocket)(VdIoSOCKET s);
static VdIo__Procclosesocket VdIo__closesocket;

typedef int (*VdIo__ProcWSAStartup)(VdIoWORD wVersionRequired,VdIo__LPWSADATA lpWSAData);
static VdIo__ProcWSAStartup VdIo__WSAStartup;

typedef int (*VdIo__ProcWSAGetLastError)(void);
static VdIo__ProcWSAGetLastError VdIo__WSAGetLastError;

typedef VdIoSOCKET (*VdIo__ProcWSASocketA)(int af, int type, int protocol, VdIo__LPWSAPROTOCOL_INFOA lpProtocolInfo,
                                           unsigned int g, VdIoDWORD dwFlags);
static VdIo__ProcWSASocketA VdIo__WSASocketA;

typedef int (*VdIo__ProcWSAIoctl)(VdIoSOCKET s, VdIoDWORD dwIoControlCode, VdIoLPVOID lpvInBuffer, VdIoDWORD cbInBuffer,
                                  VdIoLPVOID lpvOutBuffer, VdIoDWORD cbOutBuffer, VdIoLPDWORD lpcbBytesReturned,
                                  VdIoLPWSAOVERLAPPED lpOverlapped,
                                  VdIoLPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
static VdIo__ProcWSAIoctl VdIo__WSAIoctl;

typedef VdIoBOOL (*VdIo__ProcAcceptEx)(VdIoSOCKET sListenSocket, VdIoSOCKET sAcceptSocket, void *lpOutputBuffer,
                                       VdIoDWORD dwReceiveDataLength, VdIoDWORD dwLocalAddressLength,
                                       VdIoDWORD dwRemoteAddressLength, VdIoLPDWORD lpdwBytesReceived,
                                       VdIoLPOVERLAPPED lpOverlapped);

typedef void (*VdIo__ProcGetAcceptExSockAddrs)(void *lpOutputBuffer, VdIoDWORD dwReceiveDataLength,
                                               VdIoDWORD dwLocalAddressLength, VdIoDWORD dwRemoteAddressLength,
                                               VdIo__SOCKADDR **LocalSockaddr, VdIoLPINT LocalSockaddrLength,
                                               VdIo__SOCKADDR **RemoteSockaddr, VdIoLPINT RemoteSockaddrLength);

extern VdIoHMODULE LoadLibraryA(VdIoLPCSTR path);
extern void*       GetProcAddress(VdIoHMODULE hModule, VdIoLPCSTR lpProcName);
extern VdIoHANDLE  CreateIoCompletionPort(VdIoHANDLE    FileHandle,
                                          VdIoHANDLE    ExistingCompletionPort,
                                          VdIoULONG_PTR CompletionKey,
                                          VdIoDWORD     NumberOfConcurrentThreads);

extern VdIoBOOL GetQueuedCompletionStatus(VdIoHANDLE CompletionPort, VdIoLPDWORD lpNumberOfBytesTransferred,
                                          VdIoPULONG_PTR lpCompletionKey, VdIoLPOVERLAPPED *lpOverlapped,
                                          VdIoDWORD dwMilliseconds);
extern VdIoDWORD GetLastError(void);

#define VD_IO__MAKEWORD(a, b) \
    ((VdIoWORD)(((VdIoBYTE)(((VdIoDWORD_PTR)(a)) & 0xff)) | ((VdIoWORD)((VdIoBYTE)(((VdIoDWORD_PTR)(b)) & 0xff))) << 8))

#define VD_IO__AF_INET             2
#define VD_IO__SOCK_STREAM         1
#define VD_IO__SOCK_DGRAM          2
#define VD_IO__IPPROTO_TCP         6
#define VD_IO__IPPROTO_UDP         17

#define VD_IO__INVALID_SOCKET  (VdIoSOCKET)(~0)

#define VD_IO__WSA_IO_PENDING (997)

#define VD_IO__SOMAXCONN       0x7fffffff

#define VD_IO__WSA_FLAG_OVERLAPPED 0x01

#define VD_IO__IOC_OUT        0x40000000
#define VD_IO__IOC_IN         0x80000000
#define VD_IO__IOC_INOUT      (VD_IO__IOC_IN|VD_IO__IOC_OUT)
#define VD_IO__IOC_WS2        0x08000000
#define VD_IO__WSAIORW(x,y)   (VD_IO__IOC_INOUT|(x)|(y))
#define VD_IO__SIO_GET_EXTENSION_FUNCTION_POINTER  VD_IO__WSAIORW(VD_IO__IOC_WS2,6)

#define VD_IO__INVALID_HANDLE_VALUE ((VdIoHANDLE)(VdIoLONG_PTR)-1)

#define VD_IO_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) VdIoGUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
static VD_IO_DEFINE_GUID(VdIo__WSAID_ACCEPTEX, 0xb5367df1, 0xcbac, 0x11cf, 0x95, 0xca, 0x00, 0x80, 0x5f, 0x48, 0xa1, 0x92);
static VD_IO_DEFINE_GUID(VdIo__WSAID_GETACCEPTEXSOCKADDRS, 0xb5367df2, 0xcbac, 0x11cf, 0x95,0xca,0x00,0x80,0x5f,0x48,0xa1,0x92);



#pragma pack(pop)

typedef struct {
    int wsa_initialized;
} VdIo__Win32InternalData;

static VdIo__Win32InternalData Vd_Io_G = {0};

static void vd_io__win32_check_init(void);
static VdIo__ProcAcceptEx vd_io__win32_get_acceptex(VdIo *io);
static VdIo__ProcGetAcceptExSockAddrs vd_io__win32_get_getacceptexsockaddrs(VdIo *io);
static VdIoErr vd_io__win32_resolve_ip(const char *ip, uint32_t *ip_n);
static void vd_io__win32_handle_op(VdIo *io, VdIoOp *op);

static void vd_io__win32_check_init(void)
{
    if (Vd_Io_G.wsa_initialized) {
        return;
    }

    Vd_Io_G.wsa_initialized = 1;


    {
        VdIoHMODULE mod = LoadLibraryA("Ws2_32.dll");
        VdIo__WSAStartup = (VdIo__ProcWSAStartup)GetProcAddress(mod, "WSAStartup");
        VdIo__WSAGetLastError = (VdIo__ProcWSAGetLastError)GetProcAddress(mod, "WSAGetLastError");
        VdIo__getaddrinfo = (VdIo__Procgetaddrinfo)GetProcAddress(mod, "getaddrinfo");
        VdIo__freeaddrinfo = (VdIo__Procfreeaddrinfo)GetProcAddress(mod, "freeaddrinfo");
        VdIo__inet_ntop = (VdIo__Procinet_ntop)GetProcAddress(mod, "inet_ntop");
        VdIo__inet_pton = (VdIo__Procinet_pton)GetProcAddress(mod, "inet_pton");
        VdIo__htons = (VdIo__Prochtons)GetProcAddress(mod, "htons");
        VdIo__socket = (VdIo__Procsocket)GetProcAddress(mod, "socket");
        VdIo__bind = (VdIo__Procbind)GetProcAddress(mod, "bind");
        VdIo__listen = (VdIo__Proclisten)GetProcAddress(mod, "listen");
        VdIo__closesocket = (VdIo__Procclosesocket)GetProcAddress(mod, "closesocket");
        VdIo__WSASocketA = (VdIo__ProcWSASocketA)GetProcAddress(mod, "WSASocketA");
        VdIo__WSAIoctl = (VdIo__ProcWSAIoctl)GetProcAddress(mod, "WSAIoctl");
    }

    VdIoWORD version_requested = VD_IO__MAKEWORD(2, 2);
    VdIo__WSADATA data;
    int rc = VdIo__WSAStartup(version_requested, &data);
    if (rc != 0)
    {
        return;
    }

}

static VdIo__ProcAcceptEx vd_io__win32_get_acceptex(VdIo *io)
{
    return (VdIo__ProcAcceptEx)io->u.win.acceptex;
}

static VdIo__ProcGetAcceptExSockAddrs vd_io__win32_get_getacceptexsockaddrs(VdIo *io)
{
    return (VdIo__ProcGetAcceptExSockAddrs)io->u.win.getacceptexsockaddrs;
}

static VdIoErr vd_io__win32_resolve_ip(const char *ip, uint32_t *ip_n)
{
    VdIoErr err = VD_IO_ERR_OK;

    int ip_parse_rc = VdIo__inet_pton(VD_IO__AF_INET, ip, ip_n);
    if (ip_parse_rc == 0) {
        // not valid ip format, try to resolve as hostname
        VdIo__ADDRINFOA hints = {0};
        hints.ai_family = VD_IO__AF_INET;
        VdIo__ADDRINFOA *host_info = 0;
        int resolve_rc = VdIo__getaddrinfo(ip, 0, &hints, &host_info);

        if (resolve_rc == 0) {
            *ip_n = host_info->ai_addr->sin_addr.S_un.S_addr;
            VdIo__freeaddrinfo(host_info);
        } else {
            // @todo(mdodis): WSAGetLastError
            err = VD_IO_ERR_INTERNAL;
            goto END;
        }
    } else if (ip_parse_rc < 0) {
        // @todo(mdodis): WSAGetLastError
        err = VD_IO_ERR_INTERNAL;
        goto END;
    }

END:
    return err;
}

VD_IO_API VdIoErr vd_io_init(VdIo *io, VdIoInitInfo *info)
{
    vd_io__win32_check_init();

    VdIoErr err = VD_IO_ERR_OK;

    size_t want_memory = vd_io_init_info_mem_size(info);

    if (info->op_memory_size < want_memory) {
        err = VD_IO_ERR_INTERNAL; // @todo(mdodis): Memory error
        goto END;
    }

    VdIoSOCKET s = VdIo__WSASocketA(VD_IO__AF_INET, VD_IO__SOCK_STREAM, VD_IO__IPPROTO_TCP, NULL, 0,
                                    VD_IO__WSA_FLAG_OVERLAPPED);

    VdIoDWORD bytes;

    VdIo__WSAIoctl(s, VD_IO__SIO_GET_EXTENSION_FUNCTION_POINTER,
                   &VdIo__WSAID_ACCEPTEX, sizeof(VdIo__WSAID_ACCEPTEX),
                   &io->u.win.acceptex, sizeof(io->u.win.acceptex), &bytes, 0, 0);

    VdIo__WSAIoctl(s, VD_IO__SIO_GET_EXTENSION_FUNCTION_POINTER,
                   &VdIo__WSAID_GETACCEPTEXSOCKADDRS, sizeof(VdIo__WSAID_GETACCEPTEXSOCKADDRS),
                   &io->u.win.getacceptexsockaddrs, sizeof(io->u.win.getacceptexsockaddrs), &bytes, 0, 0);

    io->cap_ops = vd_io__count_ops_from_bytes(info->op_memory_size);
    io->num_ops = 0;
    io->ops = (VdIoOp*)info->op_memory;
    io->u.win.iocp = (uintptr_t)CreateIoCompletionPort(VD_IO__INVALID_HANDLE_VALUE, 0, 0, 0);

    for (int i = 0; i < io->cap_ops; ++i) {
        io->ops[i].used = 0;
    }

    VdIo__closesocket(s);
END:
    return err;
}

VD_IO_API VdIoErr vd_io_wait(VdIo *io)
{
    VdIoErr err = VD_IO_ERR_OK;
    VdIoDWORD bytes;
    VdIoULONG_PTR key;
    VdIoOVERLAPPED *ov;

    VdIoBOOL ok = GetQueuedCompletionStatus((VdIoHANDLE)io->u.win.iocp, &bytes, &key, &ov, 0xFFFFFFFF);
    VdIoDWORD w_err = ok ? 0 : GetLastError();

    VdIoOp *op = ((VdIoOp *)( (VdIoPCHAR)(ov) - (VdIoULONG_PTR)(&((VdIoOp *)0)->internal)));

    switch (op->kind) {
        default: break;
        case VD_IO_OP_KIND_ACCEPT: {
            VdIoEvent evt;
            evt.kind = VD_IO_EVENT_KIND_ACCEPT;
            if (w_err != 0) {
                evt.err = VD_IO_ERR_INTERNAL; // @todo(mdodis): Translate from GetLastError (w_err) 
            } else {
                evt.err = VD_IO_ERR_OK;
                evt.data.accept.socket.h = (uintptr_t)op->data.accept.client;
                CreateIoCompletionPort((VdIoHANDLE)op->data.accept.client, (VdIoHANDLE)io->u.win.iocp,
                                       (VdIoULONG_PTR)op->data.accept.client, 0);


                VdIoCallback *callback = op->callback;
                void *callback_usr = op->callback_usr;
                vd_io__free_op(io, op);

                callback(&evt, callback_usr);
            }
        } break;
    }

    return err;
}

VD_IO_API VdIoErr vd_io_make_socket(VdIo *io, const char *ip, int port, VdIoFlags flags, VdIoHn *hn)
{
    VdIoErr err = VD_IO_ERR_OK;

    int socket_type = VD_IO__SOCK_STREAM;
    int protocol = VD_IO__IPPROTO_TCP;
    if (flags & VD_IO_NET_UDP) {
        socket_type = VD_IO__SOCK_DGRAM;
        protocol = VD_IO__IPPROTO_UDP;
    }


    VdIo__SOCKADDR_IN sockaddr;
    sockaddr.sin_family = VD_IO__AF_INET;
    sockaddr.sin_port = VdIo__htons((VdIoUSHORT)port);

    {
        uint32_t ip_n;
        VdIoErr resolve_err = vd_io__win32_resolve_ip(ip, &ip_n);
        if (resolve_err != VD_IO_ERR_OK) {
            err = resolve_err;
            goto END;
        }
        sockaddr.sin_addr.S_un.S_addr = ip_n;
    }

    VdIoSOCKET socket = VdIo__WSASocketA(VD_IO__AF_INET, socket_type, protocol, 0, 0, VD_IO__WSA_FLAG_OVERLAPPED);

    if (socket == VD_IO__INVALID_SOCKET) {
        err = VD_IO_ERR_INTERNAL; // @todo(mdodis): WSAGetLastError
        goto END;
    }

    if (VdIo__bind(socket, (VdIo__SOCKADDR*)&sockaddr, sizeof(sockaddr)) != 0) {
        err = VD_IO_ERR_INTERNAL; // @todo(mdodis): WSAGetLastError
        goto END;
    }

    if (VdIo__listen(socket, VD_IO__SOMAXCONN) != 0) {
        err = VD_IO_ERR_INTERNAL; // @todo(mdodis): WSAGetLastError
        goto END;
    }

    CreateIoCompletionPort((VdIoHANDLE)socket, (VdIoHANDLE)io->u.win.iocp, (VdIoULONG_PTR)io, 0);

    hn->h = (uintptr_t)socket;
    hn->flags = flags;

END:
    return err;
}

VD_IO_API VdIoErr vd_io_hn_accept(VdIo *io, VdIoHn hn, VdIoCallback *cb, void *usr)
{
    VdIoErr err = VD_IO_ERR_OK;

    int op_idx = vd_io__make_op(io);
    if (op_idx == -1) {
        err = VD_IO_ERR_INTERNAL; // @todo(mdodis): FULL MEMORY
        goto END;
    }

    VdIoOp *op = &io->ops[op_idx];

    VD_IO_MEMSET(op->internal, 0, sizeof(op->internal));

    int socket_type = VD_IO__SOCK_STREAM;
    int protocol = VD_IO__IPPROTO_TCP;
    if (hn.flags & VD_IO_NET_UDP) {
        socket_type = VD_IO__SOCK_DGRAM;
        protocol = VD_IO__IPPROTO_UDP;
    }

    VdIoSOCKET s = VdIo__WSASocketA(VD_IO__AF_INET, socket_type, protocol, NULL, 0, VD_IO__WSA_FLAG_OVERLAPPED);
    if (s == VD_IO__INVALID_SOCKET) {
        VdIoDWORD wsa_err = VdIo__WSAGetLastError();
        err = VD_IO_ERR_INTERNAL; // @todo(mdodis): WSAGetLastError
        goto END;
    }

    VdIo__ProcAcceptEx acceptex = vd_io__win32_get_acceptex(io);
    VdIoBOOL acceptex_ok = acceptex((VdIoSOCKET)hn.h, s,
                                    op->data.accept.buf,
                                    0,
                                    32,
                                    32,
                                    NULL,
                                    (VdIoOVERLAPPED*)op->internal);

    if (!acceptex_ok) {
        VdIoDWORD wsa_err = VdIo__WSAGetLastError();
        if (wsa_err != VD_IO__WSA_IO_PENDING) {
            err = VD_IO_ERR_INTERNAL; // @todo(mdodis): WSAGetLastError
            goto END;
        }
    }

    op->kind = VD_IO_OP_KIND_ACCEPT;
    op->data.accept.listener = hn;
    op->data.accept.client = (uintptr_t)s;
    op->callback = cb;
    op->callback_usr = usr;

END:
    return err;
}

VD_IO_API VdIoErr vd_io_net_open(VdIoFlags flags, const char *ip, int port, VdIoHn *hn)
{
    vd_io__win32_check_init();
    VdIoErr err = VD_IO_ERR_OK;

    VdIoSOCKET socket = VdIo__socket(VD_IO__AF_INET, VD_IO__SOCK_STREAM, VD_IO__IPPROTO_TCP);

    uint32_t ip_n = 0;
    {
        VdIoErr resolve_err = vd_io__win32_resolve_ip(ip, &ip_n);
        if (resolve_err != VD_IO_ERR_OK) {
            err = resolve_err;
            goto END;
        }
    }

    VdIo__SOCKADDR_IN saddr = {0};
    saddr.sin_family = VD_IO__AF_INET;
    saddr.sin_addr.S_un.S_addr = ip_n;
    saddr.sin_port = VdIo__htons((VdIoUSHORT)port);

    int bind_rc = VdIo__bind(socket, (VdIo__SOCKADDR*)&saddr, sizeof(saddr));
    if (bind_rc != 0) {
        err = VD_IO_ERR_INTERNAL;
        int wsa_error = VdIo__WSAGetLastError();
        goto END;
    }
END:
    return err;
}

#endif // _WIN32

#endif // VD_IO_IMPL