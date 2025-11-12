/*
 * vd_dlg.h - Dialogs for platforms that support them
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
 * - Message Box options and rt value
 * - Default Path
 * - Save File
 * - Open Multiple Files
 * - Notifications
 */
#ifndef VD_DLG_H
#define VD_DLG_H
#define VD_DLG_VERSION_MAJOR    0
#define VD_DLG_VERSION_MINOR    0
#define VD_DLG_VERSION_PATCH    1
#define VD_DLG_VERSION          ((VD_DLG_VERSION_MAJOR << 16) | (VD_DLG_VERSION_MINOR << 8) | (VD_DLG_VERSION_PATCH))

#ifndef VD_DLG_API
#   ifdef VD_DLG_STATIC
#       define VD_DLG_API static
#   else
#       define VD_DLG_API extern
#   endif // VD_DLG_STATIC
#endif // !VD_DLG_API

#ifndef VD_DLG_INL
#   define VD_DLG_INL static inline
#endif // VD_DLG_INL

typedef enum {
    VD_DLG_MESSAGE_BOX_OPTION_OK     = 0,
    VD_DLG_MESSAGE_BOX_OPTION_CANCEL = 1 << 1,
} VdDlgMessageBoxOptions;

typedef enum {
    VD_DLG_OPEN_FILE_OPTION_DEFAULT = 0,
} VdDlgOpenFileOptions;

typedef struct {
    int        description_len;
    const char *description;

    int        extensions_len;
    const char *extensions;
} VdDlgFileFilter;

typedef struct {
    char *buf;
    int  len;
    int  buf_cap;
} VdDlgFileResult;

/**
 * @brief Show a message box
 * @param  title_len       The length of the title, in bytes
 * @param  title           The title, as a UTF-8 string
 * @param  description_len The length of the description, in bytes
 * @param  description     The description, as a UTF-8 string
 * @param  options         The options
 * @return                 The option the user selected
 */
VD_DLG_API int              vd_dlg_message_box(int title_len, const char *title, int description_len, const char *description, VdDlgMessageBoxOptions options);

/**
 * @brief Show a 'Select File' dialog for a single file
 * @param  title_len      The length of the prompt title, in bytes
 * @param  title          The prompt title, as a UTF-8 string
 * @param  path_len       (Optional) The length of the path to default to, in bytes
 * @param  path           (Optional) The path to default to, as a UTF-8 string
 * @param  num_filters    The count of the file type filters
 * @param  filters        The file type filters
 * @param  default_filter Zero based index into filters to use as the default filter
 * @param  options        Options for the file dialog
 * @return                The selected file, all zeroes if 'Cancel' was pressed
 */
VD_DLG_API VdDlgFileResult  vd_dlg_open_file(int title_len, const char *title, int path_len, const char *path, int num_filters, VdDlgFileFilter *filters, int default_filter, VdDlgOpenFileOptions options);

/**
 * @brief Set the context for any upcoming calls (this is usually the window owner)
 * @param  pf_context Win32(HWND*)
 */
VD_DLG_API void             vd_dlg_set_context(void *pf_context);

#endif // !VD_DLG_H

#ifdef VD_DLG_IMPL

#ifndef VD_DLG_ABORT
#   define VD_DLG_ABORT(message) do { *(char*)0 = *message; } while(0)
#endif // !VD_DLG_ABORT

#ifdef _WIN32

#pragma pack(push, 1)
/* ----WIN32 BASE---------------------------------------------------------------------------------------------------- */
#ifndef _MINWINDEF_
#define VD_DLG_DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name

typedef unsigned long        VdDlgDWORD;
typedef int                  VdDlgBOOL;
typedef unsigned char        VdDlgBYTE;
typedef unsigned short       VdDlgWORD;
typedef float                VdDlgFLOAT;
typedef VdDlgFLOAT*          VdDlgPFLOAT;
typedef VdDlgBOOL*           VdDlgPBOOL;
typedef VdDlgBOOL*           VdDlgLPBOOL;
typedef VdDlgBYTE*           VdDlgPBYTE;
typedef VdDlgBYTE*           VdDlgLPBYTE;
typedef int*                 VdDlgPINT;
typedef int*                 VdDlgLPINT;
typedef VdDlgWORD*           VdDlgPWORD;
typedef VdDlgWORD*           VdDlgLPWORD;
typedef long*                VdDlgLPLONG;
typedef VdDlgDWORD*          VdDlgPDWORD;
typedef VdDlgDWORD*          VdDlgLPDWORD;
typedef void*                VdDlgLPVOID;
typedef const void*          VdDlgLPCVOID;
typedef int                  VdDlgINT;
typedef unsigned int         VdDlgUINT;
typedef unsigned int*        VdDlgPUINT;
typedef void*                VdDlgHANDLE;
typedef VdDlgHANDLE*         VdDlgPHANDLE;
typedef char                 VdDlgCHAR;
typedef const VdDlgCHAR*     VdDlgLPCSTR, *VdDlgPCSTR;
typedef long                 VdDlgLONG;
typedef unsigned long        VdDlgULONG;
typedef VdDlgULONG*          VdDlgPULONG;
typedef unsigned short       VdDlgUSHORT;
typedef VdDlgUSHORT*         VdDlgPUSHORT;
typedef unsigned char        VdDlgUCHAR;
typedef VdDlgUCHAR*          VdDlgPUCHAR;
typedef VdDlgBYTE            VdDlgBOOLEAN;
typedef VdDlgCHAR*           VdDlgPCHAR, * VdDlgLPCH, * VdDlgPCH;
typedef long                 VdDlgHRESULT;
typedef __int64              VdDlgLONGLONG;
typedef unsigned __int64     VdDlgULONGLONG;
typedef VdDlgULONGLONG       VdDlgDWORDLONG;
typedef VdDlgDWORDLONG*      VdDlgPDWORDLONG;
typedef __int64              VdDlgINT_PTR, * VdDlgPINT_PTR;
typedef unsigned __int64     VdDlgUINT_PTR, * VdDlgPUINT_PTR;
typedef __int64              VdDlgLONG_PTR, * VdDlgPLONG_PTR;
typedef unsigned __int64     VdDlgULONG_PTR, * VdDlgPULONG_PTR;
typedef VdDlgUINT_PTR        VdDlgWPARAM;
typedef VdDlgLONG_PTR        VdDlgLPARAM;
typedef VdDlgLONG_PTR        VdDlgLRESULT;
typedef wchar_t              VdDlgWCHAR;
typedef const VdDlgWCHAR*    VdDlgLPCWSTR, * VdDlgPCWSTR;
typedef VdDlgWORD            VdDlgATOM;
typedef VdDlgULONG_PTR       VdDlgDWORD_PTR, * VdDlgPDWORD_PTR;
typedef short                VdDlgSHORT;

VD_DLG_DECLARE_HANDLE(VdDlgHWND);
VD_DLG_DECLARE_HANDLE(VdDlgHINSTANCE);
typedef VdDlgHINSTANCE VdDlgHMODULE;

#else
typedef HWND      VdDlgHWND;
typedef DWORD     VdDlgDWORD;
typedef WORD      VdDlgWORD;
typedef LPWSTR    VdDlgLPWSTR;
typedef LPCWSTR   VdDlgLPCWSTR;
typedef LPCSTR    VdDlgLPCSTR;
typedef DWORD     VdDlgDWORD;
typedef UINT      VdDlgUINT;
typedef HMODULE   VdDlgHMODULE;
typedef HRESULT   VdDlgHRESULT;
typedef ULONG     VdDlgULONG;
#endif // !_WINDOWS_

typedef struct VdDlg_GUID {
    unsigned long  Data1;
    unsigned short Data2;
    unsigned short Data3;
    unsigned char  Data4[ 8 ];
} VdDlgGUID;

VD_DLG_INL int vd_dlg__guid_compare(const VdDlgGUID *a, const VdDlgGUID *b)
{
    return (a->Data1 == b->Data1)       && (a->Data2 == b->Data2)       && (a->Data3 == b->Data3) &&
           (a->Data4[0] == b->Data4[0]) && (a->Data4[1] == b->Data4[1]) &&
           (a->Data4[2] == b->Data4[2]) && (a->Data4[3] == b->Data4[3]) &&
           (a->Data4[4] == b->Data4[4]) && (a->Data4[5] == b->Data4[5]) &&
           (a->Data4[6] == b->Data4[6]) && (a->Data4[7] == b->Data4[7]);
}

typedef VdDlgGUID            VdDlgIID;
typedef VdDlgGUID*           VdDlgREFGUID;
#define VdDlgREFCLSID        const VdDlgIID *
typedef VdDlgIID*            VdDlgREFIID;
typedef struct VdDlgIUnknown VdDlgIUnknown;
typedef VdDlgIUnknown*       VdDlgLPUNKNOWN;
typedef VdDlgULONG           VdDlgSFGAOF;
typedef VdDlgDWORD           VdDlgSICHINTF;
#pragma pack(pop)

/* ----Kernel32.dll-------------------------------------------------------------------------------------------------- */
#ifndef _MINWINDEF_
extern VdDlgHMODULE __stdcall LoadLibraryA(VdDlgLPCSTR lpLibFileName);
extern void*        __stdcall GetProcAddress(VdDlgHMODULE hModule, VdDlgLPCSTR lpProcName);
extern int                    MultiByteToWideChar(VdDlgUINT CodePage, VdDlgDWORD dwFlags, VdDlgDWORD *lpMultiByteStr, int cbMultiByte, VdDlgLPWSTR cbMultiByte, VdDlgLPWSTR lpWideCharStr, int cchWideChar);

#endif

/* ----User32.dll---------------------------------------------------------------------------------------------------- */
typedef int (*VdDlgProcMessageBoxW)(VdDlgHWND hWnd, VdDlgLPCWSTR lpText, VdDlgLPCWSTR lpCaption, VdDlgUINT uType);
static VdDlgProcMessageBoxW VdDlgMessageBoxW;

/* ----Ole32.dll----------------------------------------------------------------------------------------------------- */
typedef VdDlgHRESULT (*VdDlgProcCoInitialize)(void *pvReserved);
static VdDlgProcCoInitialize VdDlgCoInitialize;

typedef VdDlgHRESULT (*VdDlgProcCoCreateInstance)(VdDlgREFCLSID rclsid, VdDlgLPUNKNOWN pUnkOuter, VdDlgDWORD dwClsContext, VdDlgREFIID riid, void **ppv);
static VdDlgProcCoCreateInstance VdDlgCoCreateInstance;

typedef void (*VdDlgProcCoTaskMemFree)(void *pv);
static VdDlgProcCoTaskMemFree VdDlgCoTaskMemFree;

typedef struct {
    VdDlgHRESULT (__stdcall *QueryInterface)(VdDlgIUnknown *This, VdDlgREFIID riid, void **ppvObject);
    VdDlgULONG   (__stdcall *AddRef)(VdDlgIUnknown *This);
    VdDlgULONG   (__stdcall *Release)(VdDlgIUnknown *This);
} VdDlgIUnknownVtbl;
struct VdDlgIUnknown { const VdDlgIUnknownVtbl *lpVtbl; };

typedef enum {
    VD_DLG_SIGDN_NORMALDISPLAY = 0,
    VD_DLG_SIGDN_PARENTRELATIVEPARSING = ( int  )0x80018001,
    VD_DLG_SIGDN_DESKTOPABSOLUTEPARSING    = ( int  )0x80028000,
    VD_DLG_SIGDN_PARENTRELATIVEEDITING = ( int  )0x80031001,
    VD_DLG_SIGDN_DESKTOPABSOLUTEEDITING    = ( int  )0x8004c000,
    VD_DLG_SIGDN_FILESYSPATH   = ( int  )0x80058000,
    VD_DLG_SIGDN_URL   = ( int  )0x80068000,
    VD_DLG_SIGDN_PARENTRELATIVEFORADDRESSBAR   = ( int  )0x8007c001,
    VD_DLG_SIGDN_PARENTRELATIVE    = ( int  )0x80080001,
    VD_DLG_SIGDN_PARENTRELATIVEFORUI   = ( int  )0x80094001
} VdDlgSIGDN;

typedef enum {
    VD_DLG_FDEOR_DEFAULT   = 0,
    VD_DLG_FDEOR_ACCEPT    = 1,
    VD_DLG_FDEOR_REFUSE    = 2
} VdDlgFDE_OVERWRITE_RESPONSE;

typedef enum {
    VD_DLG_FDESVR_DEFAULT  = 0,
    VD_DLG_FDESVR_ACCEPT   = 1,
    VD_DLG_FDESVR_REFUSE   = 2
} VdDlgFDE_SHAREVIOLATION_RESPONSE;

enum {
    VD_DLG_FOS_OVERWRITEPROMPT = 0x2,
    VD_DLG_FOS_STRICTFILETYPES = 0x4,
    VD_DLG_FOS_NOCHANGEDIR = 0x8,
    VD_DLG_FOS_PICKFOLDERS = 0x20,
    VD_DLG_FOS_FORCEFILESYSTEM = 0x40,
    VD_DLG_FOS_ALLNONSTORAGEITEMS  = 0x80,
    VD_DLG_FOS_NOVALIDATE  = 0x100,
    VD_DLG_FOS_ALLOWMULTISELECT    = 0x200,
    VD_DLG_FOS_PATHMUSTEXIST   = 0x800,
    VD_DLG_FOS_FILEMUSTEXIST   = 0x1000,
    VD_DLG_FOS_CREATEPROMPT    = 0x2000,
    VD_DLG_FOS_SHAREAWARE  = 0x4000,
    VD_DLG_FOS_NOREADONLYRETURN    = 0x8000,
    VD_DLG_FOS_NOTESTFILECREATE    = 0x10000,
    VD_DLG_FOS_HIDEMRUPLACES   = 0x20000,
    VD_DLG_FOS_HIDEPINNEDPLACES    = 0x40000,
    VD_DLG_FOS_NODEREFERENCELINKS  = 0x100000,
    VD_DLG_FOS_OKBUTTONNEEDSINTERACTION    = 0x200000,
    VD_DLG_FOS_DONTADDTORECENT = 0x2000000,
    VD_DLG_FOS_FORCESHOWHIDDEN = 0x10000000,
    VD_DLG_FOS_DEFAULTNOMINIMODE   = 0x20000000,
    VD_DLG_FOS_FORCEPREVIEWPANEON  = 0x40000000,
    VD_DLG_FOS_SUPPORTSTREAMABLEITEMS  = 0x80000000
};
typedef VdDlgDWORD VdDlgFILEOPENDIALOGOPTIONS;

typedef enum {
    VD_DLG_FDAP_BOTTOM = 0,
    VD_DLG_FDAP_TOP    = 1
} VdDlgFDAP;

typedef enum {
    VD_DLG_SIATTRIBFLAGS_AND   = 0x1,
    VD_DLG_SIATTRIBFLAGS_OR    = 0x2,
    VD_DLG_SIATTRIBFLAGS_APPCOMPAT = 0x3,
    VD_DLG_SIATTRIBFLAGS_MASK  = 0x3,
    VD_DLG_SIATTRIBFLAGS_ALLITEMS  = 0x4000
} VdDlgSIATTRIBFLAGS;

typedef enum {
    VD_DLG_GPS_DEFAULT = 0,
    VD_DLG_GPS_HANDLERPROPERTIESONLY = 0x1,
    VD_DLG_GPS_READWRITE = 0x2,
    VD_DLG_GPS_TEMPORARY = 0x4,
    VD_DLG_GPS_FASTPROPERTIESONLY = 0x8,
    VD_DLG_GPS_OPENSLOWITEM = 0x10,
    VD_DLG_GPS_DELAYCREATION = 0x20,
    VD_DLG_GPS_BESTEFFORT = 0x40,
    VD_DLG_GPS_NO_OPLOCK = 0x80,
    VD_DLG_GPS_PREFERQUERYPROPERTIES = 0x100,
    VD_DLG_GPS_EXTRINSICPROPERTIES = 0x200,
    VD_DLG_GPS_EXTRINSICPROPERTIESONLY = 0x400,
    VD_DLG_GPS_VOLATILEPROPERTIES = 0x800,
    VD_DLG_GPS_VOLATILEPROPERTIESONLY = 0x1000,
    VD_DLG_GPS_MASK_VALID = 0x1fff
} VdDlgGETPROPERTYSTOREFLAGS;

typedef struct VdDlg_tagpropertykey {
    VdDlgGUID  fmtid;
    VdDlgDWORD pid;
} VdDlgPROPERTYKEY;

typedef VdDlgPROPERTYKEY* VdDlgREFPROPERTYKEY;

typedef enum VdDlgtagCLSCTX {
    VD_DLG_CLSCTX_INPROC_SERVER = 0x1,
    VD_DLG_CLSCTX_INPROC_HANDLER = 0x2,
    VD_DLG_CLSCTX_LOCAL_SERVER = 0x4,
    VD_DLG_CLSCTX_INPROC_SERVER16 = 0x8,
    VD_DLG_CLSCTX_REMOTE_SERVER = 0x10,
    VD_DLG_CLSCTX_INPROC_HANDLER16 = 0x20,
    VD_DLG_CLSCTX_RESERVED1 = 0x40,
    VD_DLG_CLSCTX_RESERVED2 = 0x80,
    VD_DLG_CLSCTX_RESERVED3 = 0x100,
    VD_DLG_CLSCTX_RESERVED4 = 0x200,
    VD_DLG_CLSCTX_NO_CODE_DOWNLOAD = 0x400,
    VD_DLG_CLSCTX_RESERVED5 = 0x800,
    VD_DLG_CLSCTX_NO_CUSTOM_MARSHAL = 0x1000,
    VD_DLG_CLSCTX_ENABLE_CODE_DOWNLOAD = 0x2000,
    VD_DLG_CLSCTX_NO_FAILURE_LOG = 0x4000,
    VD_DLG_CLSCTX_DISABLE_AAA = 0x8000,
    VD_DLG_CLSCTX_ENABLE_AAA = 0x10000,
    VD_DLG_CLSCTX_FROM_DEFAULT_CONTEXT = 0x20000,
    VD_DLG_CLSCTX_ACTIVATE_X86_SERVER = 0x40000,
    VD_DLG_CLSCTX_ACTIVATE_32_BIT_SERVER,
    VD_DLG_CLSCTX_ACTIVATE_64_BIT_SERVER = 0x80000,
    VD_DLG_CLSCTX_ENABLE_CLOAKING = 0x100000,
    VD_DLG_CLSCTX_APPCONTAINER = 0x400000,
    VD_DLG_CLSCTX_ACTIVATE_AAA_AS_IU = 0x800000,
    VD_DLG_CLSCTX_RESERVED6 = 0x1000000,
    VD_DLG_CLSCTX_ACTIVATE_ARM32_SERVER = 0x2000000,
    VD_DLG_CLSCTX_ALLOW_LOWER_TRUST_REGISTRATION,
    VD_DLG_CLSCTX_PS_DLL = 0x80000000
} VdDlgCLSCTX;

/* ----ShObjidl.h---------------------------------------------------------------------------------------------------- */
typedef struct VdDlgIShellItem          VdDlgIShellItem;
typedef struct VdDlgIBindCtx            VdDlgIBindCtx;
typedef struct VdDlgIFileDialogEvents   VdDlgIFileDialogEvents;
typedef struct VdDlgIFileDialog         VdDlgIFileDialog;
typedef struct VdDlgIFileOpenDialog     VdDlgIFileOpenDialog;
typedef struct VdDlgIShellItemArray     VdDlgIShellItemArray;
typedef struct VdDlgIEnumShellItems     VdDlgIEnumShellItems;

typedef struct VdDlg_COMDLG_FILTERSPEC {
    VdDlgLPCWSTR pszName;
    VdDlgLPCWSTR pszSpec;
} VdDlgCOMDLG_FILTERSPEC;

typedef struct VdDlgtagBIND_OPTS {
    VdDlgDWORD cbStruct;
    VdDlgDWORD grfFlags;
    VdDlgDWORD grfMode;
    VdDlgDWORD dwTickCountDeadline;
} VdDlgBIND_OPTS;

typedef struct {
    VdDlgHRESULT (__stdcall *QueryInterface)(VdDlgIBindCtx *This, VdDlgREFIID riid, void **ppvObject);
    VdDlgULONG   (__stdcall *AddRef)(VdDlgIBindCtx *This);
    VdDlgULONG   (__stdcall *Release)(VdDlgIBindCtx *This);
    
    VdDlgHRESULT (__stdcall *RegisterObjectBound)(VdDlgIBindCtx *This, VdDlgIUnknown *punk);
    VdDlgHRESULT (__stdcall *RevokeObjectBound)(VdDlgIBindCtx *This, VdDlgIUnknown *punk);
    VdDlgHRESULT (__stdcall *ReleaseBoundObjects)(VdDlgIBindCtx *This);
    VdDlgHRESULT (__stdcall *SetBindOptions)(VdDlgIBindCtx *This, VdDlgBIND_OPTS *pbindopts);
    VdDlgHRESULT (__stdcall *GetBindOptions)(VdDlgIBindCtx *This, VdDlgBIND_OPTS *pbindopts);
    VdDlgHRESULT (__stdcall *GetRunningObjectTable)(VdDlgIBindCtx *This, /*IRunningObjectTable*/ VdDlgIUnknown **pprot);
    VdDlgHRESULT (__stdcall *RegisterObjectParam)(VdDlgIBindCtx *This, VdDlgLPWSTR pszKey, VdDlgIUnknown *punk);
    VdDlgHRESULT (__stdcall *GetObjectParam)(VdDlgIBindCtx *This, VdDlgLPWSTR pszKey, VdDlgIUnknown **ppunk);
    VdDlgHRESULT (__stdcall *EnumObjectParam)(VdDlgIBindCtx *This, /*IEnumString*/ VdDlgIUnknown **ppenum);
    VdDlgHRESULT (__stdcall *RevokeObjectParam)(VdDlgIBindCtx *This, VdDlgLPWSTR pszKey);
} VdDlgIBindCtxVtbl;
struct VdDlgIBindCtx { const VdDlgIBindCtxVtbl *lpVtbl; };

typedef struct {
    VdDlgHRESULT (__stdcall *QueryInterface)(VdDlgIShellItem *This, VdDlgREFIID riid, void **ppvObject);
    VdDlgULONG   (__stdcall *AddRef)(VdDlgIShellItem *This);
    VdDlgULONG   (__stdcall *Release)(VdDlgIShellItem *This);
    
    VdDlgHRESULT (__stdcall *BindToHandler)(VdDlgIShellItem *This, VdDlgIBindCtx *pbc, VdDlgREFGUID bhid, VdDlgREFIID riid, void **ppv);
    VdDlgHRESULT (__stdcall *GetParent)(VdDlgIShellItem *This, VdDlgIShellItem **ppsi);
    VdDlgHRESULT (__stdcall *GetDisplayName)(VdDlgIShellItem *This, VdDlgSIGDN sigdnName, VdDlgLPWSTR *ppszName);
    VdDlgHRESULT (__stdcall *GetAttributes)(VdDlgIShellItem *This, VdDlgSFGAOF sfgaoMask, VdDlgSFGAOF *psfgaoAttribs);
    VdDlgHRESULT (__stdcall *Compare)(VdDlgIShellItem *This, VdDlgIShellItem *psi, VdDlgSICHINTF hint, int *piOrder);
} VdDlgIShellItemVtbl;
struct VdDlgIShellItem { const VdDlgIShellItemVtbl *lpVtbl; };

typedef struct {
    VdDlgHRESULT (__stdcall *QueryInterface)(VdDlgIFileDialogEvents *This, VdDlgREFIID riid, void **ppvObject);
    VdDlgULONG   (__stdcall *AddRef)(VdDlgIFileDialogEvents *This);
    VdDlgULONG   (__stdcall *Release)(VdDlgIFileDialogEvents *This);
    
    VdDlgHRESULT (__stdcall *OnFileOk)(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd);
    VdDlgHRESULT (__stdcall *OnFolderChanging)(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd, VdDlgIShellItem *psiFolder);
    VdDlgHRESULT (__stdcall *OnFolderChange)(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd);
    VdDlgHRESULT (__stdcall *OnSelectionChange)(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd);
    VdDlgHRESULT (__stdcall *OnShareViolation)(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd, VdDlgIShellItem *psi, VdDlgFDE_SHAREVIOLATION_RESPONSE *pResponse);
    VdDlgHRESULT (__stdcall *OnTypeChange)(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd);
    VdDlgHRESULT (__stdcall *OnOverwrite)(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd, VdDlgIShellItem *psi, VdDlgFDE_OVERWRITE_RESPONSE *pResponse);
} VdDlgIFileDialogEventsVtbl;
struct VdDlgIFileDialogEvents { const VdDlgIFileDialogEventsVtbl *lpVtbl; };

typedef struct {
    VdDlgHRESULT (__stdcall *QueryInterface)(VdDlgIFileDialog *This, VdDlgREFIID riid, void **ppvObject);
    VdDlgULONG   (__stdcall *AddRef)(VdDlgIFileDialog *This);
    VdDlgULONG   (__stdcall *Release)(VdDlgIFileDialog *This);
    
    VdDlgHRESULT (__stdcall *Show)(VdDlgIFileDialog *This, VdDlgHWND hwndOwner);
    VdDlgHRESULT (__stdcall *SetFileTypes)(VdDlgIFileDialog *This, VdDlgUINT cFileTypes, const VdDlgCOMDLG_FILTERSPEC *rgFilterSpec);
    VdDlgHRESULT (__stdcall *SetFileTypeIndex)(VdDlgIFileDialog *This, VdDlgUINT iFileType);
    VdDlgHRESULT (__stdcall *GetFileTypeIndex)(VdDlgIFileDialog *This, VdDlgUINT *piFileType);
    VdDlgHRESULT (__stdcall *Advise)(VdDlgIFileDialog *This, VdDlgIFileDialogEvents *pfde, VdDlgDWORD *pdwCookie);
    VdDlgHRESULT (__stdcall *Unadvise)(VdDlgIFileDialog *This, VdDlgDWORD dwCookie);
    VdDlgHRESULT (__stdcall *SetOptions)(VdDlgIFileDialog *This, VdDlgFILEOPENDIALOGOPTIONS fos);
    VdDlgHRESULT (__stdcall *GetOptions)(VdDlgIFileDialog *This, VdDlgFILEOPENDIALOGOPTIONS *pfos);
    VdDlgHRESULT (__stdcall *SetDefaultFolder)(VdDlgIFileDialog *This, VdDlgIShellItem *psi);
    VdDlgHRESULT (__stdcall *SetFolder)(VdDlgIFileDialog *This, VdDlgIShellItem *psi);
    VdDlgHRESULT (__stdcall *GetFolder)(VdDlgIFileDialog *This, VdDlgIShellItem **ppsi);
    VdDlgHRESULT (__stdcall *GetCurrentSelection)(VdDlgIFileDialog *This, VdDlgIShellItem **ppsi);
    VdDlgHRESULT (__stdcall *SetFileName)(VdDlgIFileDialog *This, VdDlgLPCWSTR pszName);
    VdDlgHRESULT (__stdcall *GetFileName)(VdDlgIFileDialog *This, VdDlgLPWSTR *pszName);
    VdDlgHRESULT (__stdcall *SetTitle)(VdDlgIFileDialog *This, VdDlgLPCWSTR pszTitle);
    VdDlgHRESULT (__stdcall *SetOkButtonLabel)(VdDlgIFileDialog *This, VdDlgLPCWSTR pszText);
    VdDlgHRESULT (__stdcall *SetFileNameLabel)(VdDlgIFileDialog *This, VdDlgLPCWSTR pszLabel);
    VdDlgHRESULT (__stdcall *GetResult)(VdDlgIFileDialog *This, VdDlgIShellItem **ppsi);
    VdDlgHRESULT (__stdcall *AddPlace)(VdDlgIFileDialog *This, VdDlgIShellItem *psi, VdDlgFDAP fdap);
    VdDlgHRESULT (__stdcall *SetDefaultExtension)(VdDlgIFileDialog *This, VdDlgLPCWSTR pszDefaultExtension);
    VdDlgHRESULT (__stdcall *Close)(VdDlgIFileDialog *This, VdDlgHRESULT hr);
    VdDlgHRESULT (__stdcall *SetClientGuid)(VdDlgIFileDialog *This, VdDlgREFGUID guid);
    VdDlgHRESULT (__stdcall *ClearClientData)(VdDlgIFileDialog *This);
    VdDlgHRESULT (__stdcall *SetFilter)(VdDlgIFileDialog *This, /*IShellItemFilter*/ VdDlgIUnknown *pFilter);
} VdDlgIFileDialogVtbl;
struct VdDlgIFileDialog { const VdDlgIFileDialogVtbl *lpVtbl; };

typedef struct {
    VdDlgHRESULT (__stdcall *QueryInterface)(VdDlgIEnumShellItems *This, VdDlgREFIID riid, void **ppvObject);
    VdDlgULONG   (__stdcall *AddRef)(VdDlgIEnumShellItems *This);
    VdDlgULONG   (__stdcall *Release)(VdDlgIEnumShellItems *This);
    
    VdDlgHRESULT (__stdcall *Next)(VdDlgIEnumShellItems *This, VdDlgULONG celt, VdDlgIShellItem **rgelt, VdDlgULONG *pceltFetched);
    VdDlgHRESULT (__stdcall *Skip)(VdDlgIEnumShellItems *This, VdDlgULONG celt);
    VdDlgHRESULT (__stdcall *Reset)(VdDlgIEnumShellItems *This);
    VdDlgHRESULT (__stdcall *Clone)(VdDlgIEnumShellItems * This, VdDlgIEnumShellItems **ppenum);
} VdDlgIEnumShellItemsVtbl;
struct VdDlgIEnumShellItems { const VdDlgIEnumShellItemsVtbl *lpVtbl; };

typedef struct {
    VdDlgHRESULT (__stdcall *QueryInterface)(VdDlgIShellItemArray *This, VdDlgREFIID riid, void **ppvObject);
    VdDlgULONG   (__stdcall *AddRef)(VdDlgIShellItemArray *This);
    VdDlgULONG   (__stdcall *Release)(VdDlgIShellItemArray *This);
    
    VdDlgHRESULT (__stdcall *BindToHandler)(VdDlgIShellItemArray *This, VdDlgIBindCtx *pbc, VdDlgREFGUID bhid, VdDlgREFIID riid, void **ppvOut);
    VdDlgHRESULT (__stdcall *GetPropertyStore)(VdDlgIShellItemArray *This, VdDlgGETPROPERTYSTOREFLAGS flags, VdDlgREFIID riid, void **ppv);
    VdDlgHRESULT (__stdcall *GetPropertyDescriptionList)(VdDlgIShellItemArray *This, VdDlgREFPROPERTYKEY keyType, VdDlgREFIID riid, void **ppv);
    VdDlgHRESULT (__stdcall *GetAttributes)(VdDlgIShellItemArray *This, VdDlgSIATTRIBFLAGS AttribFlags, VdDlgSFGAOF sfgaoMask, VdDlgSFGAOF *psfgaoAttribs);
    VdDlgHRESULT (__stdcall *GetCount)(VdDlgIShellItemArray *This, VdDlgDWORD *pdwNumItems);
    VdDlgHRESULT (__stdcall *GetItemAt)(VdDlgIShellItemArray *This, VdDlgDWORD dwIndex, VdDlgIShellItem **ppsi);
    VdDlgHRESULT (__stdcall *EnumItems)(VdDlgIShellItemArray *This, VdDlgIEnumShellItems **ppenumShellItems);
} VdDlgIShellItemArrayVtbl;
struct VdDlgIShellItemArray { const VdDlgIShellItemArrayVtbl *lpVtbl; };

typedef struct {
    VdDlgHRESULT (__stdcall *QueryInterface)(VdDlgIFileOpenDialog *This, VdDlgREFIID riid, void **ppvObject);
    VdDlgULONG   (__stdcall *AddRef)(VdDlgIFileOpenDialog *This);
    VdDlgULONG   (__stdcall *Release)(VdDlgIFileOpenDialog *This);
    
    VdDlgHRESULT (__stdcall *Show)(VdDlgIFileOpenDialog *This, VdDlgHWND hwndOwner);
    VdDlgHRESULT (__stdcall *SetFileTypes)(VdDlgIFileOpenDialog *This, VdDlgUINT cFileTypes, const VdDlgCOMDLG_FILTERSPEC *rgFilterSpec);
    VdDlgHRESULT (__stdcall *SetFileTypeIndex)(VdDlgIFileOpenDialog *This, VdDlgUINT iFileType);
    VdDlgHRESULT (__stdcall *GetFileTypeIndex)(VdDlgIFileOpenDialog *This, VdDlgUINT *piFileType);
    VdDlgHRESULT (__stdcall *Advise)(VdDlgIFileOpenDialog *This, VdDlgIFileDialogEvents *pfde, VdDlgDWORD *pdwCookie);
    VdDlgHRESULT (__stdcall *Unadvise)(VdDlgIFileOpenDialog *This, VdDlgDWORD dwCookie);
    VdDlgHRESULT (__stdcall *SetOptions)(VdDlgIFileOpenDialog *This, VdDlgFILEOPENDIALOGOPTIONS fos);
    VdDlgHRESULT (__stdcall *GetOptions)(VdDlgIFileOpenDialog *This, VdDlgFILEOPENDIALOGOPTIONS *pfos);
    VdDlgHRESULT (__stdcall *SetDefaultFolder)(VdDlgIFileOpenDialog *This, VdDlgIShellItem *psi);
    VdDlgHRESULT (__stdcall *SetFolder)(VdDlgIFileOpenDialog *This, VdDlgIShellItem *psi);
    VdDlgHRESULT (__stdcall *GetFolder)(VdDlgIFileOpenDialog *This, VdDlgIShellItem **ppsi);
    VdDlgHRESULT (__stdcall *GetCurrentSelection)(VdDlgIFileOpenDialog *This, VdDlgIShellItem **ppsi);
    VdDlgHRESULT (__stdcall *SetFileName)(VdDlgIFileOpenDialog *This, VdDlgLPCWSTR pszName);
    VdDlgHRESULT (__stdcall *GetFileName)(VdDlgIFileOpenDialog *This, VdDlgLPWSTR *pszName);
    VdDlgHRESULT (__stdcall *SetTitle)(VdDlgIFileOpenDialog *This, VdDlgLPCWSTR pszTitle);
    VdDlgHRESULT (__stdcall *SetOkButtonLabel)(VdDlgIFileOpenDialog *This, VdDlgLPCWSTR pszText);
    VdDlgHRESULT (__stdcall *SetFileNameLabel)(VdDlgIFileOpenDialog *This, VdDlgLPCWSTR pszLabel);
    VdDlgHRESULT (__stdcall *GetResult)(VdDlgIFileOpenDialog *This, VdDlgIShellItem **ppsi);
    VdDlgHRESULT (__stdcall *AddPlace)(VdDlgIFileOpenDialog *This, VdDlgIShellItem *psi, VdDlgFDAP fdap);
    VdDlgHRESULT (__stdcall *SetDefaultExtension)(VdDlgIFileOpenDialog *This, VdDlgLPCWSTR pszDefaultExtension);
    VdDlgHRESULT (__stdcall *Close)(VdDlgIFileOpenDialog *This, VdDlgHRESULT hr);
    VdDlgHRESULT (__stdcall *SetClientGuid)(VdDlgIFileOpenDialog *This, VdDlgREFGUID guid);
    VdDlgHRESULT (__stdcall *ClearClientData)(VdDlgIFileOpenDialog *This);
    VdDlgHRESULT (__stdcall *SetFilter)(VdDlgIFileOpenDialog *This, /*IShellItemFilter*/ VdDlgIUnknown *pFilter);
    VdDlgHRESULT (__stdcall *GetResults)(VdDlgIFileOpenDialog *This, VdDlgIShellItemArray **ppenum);
    VdDlgHRESULT (__stdcall *GetSelectedItems)(VdDlgIFileOpenDialog *This, VdDlgIShellItemArray **ppsai);
} VdDlgIFileOpenDialogVtbl;
struct VdDlgIFileOpenDialog { const VdDlgIFileOpenDialogVtbl *lpVtbl; };

#define VD_DLG__HRESULT_TYPEDEF_(_sc) ((VdDlgHRESULT)_sc)
#define VD_DLG__E_NOINTERFACE         VD_DLG__HRESULT_TYPEDEF_(0x80004002L)
#define VD_DLG__E_POINTER             VD_DLG__HRESULT_TYPEDEF_(0x80004003L)

#define VD_DLG_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) VdDlgGUID name = { l, w1, w2, { b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 } }
VD_DLG_DEFINE_GUID(VD_DLG_IID_IUnknown,         0x00000000, 0x0000, 0x0000, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46);
VD_DLG_DEFINE_GUID(VD_DLG_IID_FileDialogEvents, 0x973510db, 0x7d7f, 0x452b, 0x89, 0x75, 0x74, 0xa8, 0x58, 0x28, 0xd3, 0x54);
VD_DLG_DEFINE_GUID(VD_DLG_IID_FileOpenDialog,   0xd57c7288, 0xd4ad, 0x4768, 0xbe, 0x02, 0x9d, 0x96, 0x95, 0x32, 0xd9, 0x60);
VD_DLG_DEFINE_GUID(VD_DLG_CLSID_FileOpenDialog, 0xDC1C5A9C, 0xE88A, 0x4dde, 0xA5, 0xA1, 0x60, 0xF8, 0x2A, 0x20, 0xAE, 0xF7);

typedef struct {
    int name;
    int spec;
} VdDlgFilterSpecCaps;

typedef struct {
    int                     initialized;
    VdDlgHWND               *hwnd;

/* ----INPUTS TO FUNCTIONS------------------------------------------------------------------------------------------- */
    wchar_t                 *title_buf;
    int                     title_buf_cap;
    wchar_t                 *message_buf;
    int                     message_buf_cap;
    VdDlgCOMDLG_FILTERSPEC  *file_types;
    int                     file_types_cap;
    VdDlgFilterSpecCaps     *file_types_caps;
    int                     file_types_caps_cap;

/* ----OUTPUTS FROM FUNCTIONS---------------------------------------------------------------------------------------- */
    VdDlgFileResult     *file_results;
    int                 file_results_cap;

} VdDlg__Globals;

static VdDlgIFileDialogEvents       Vd_Dlg__CFileDialogEvents = {};
static VdDlgIFileDialogEventsVtbl   Vd_Dlg__CFileDialogEventsIImpl = {};
VdDlgULONG vd_dlg__CFileDialogEventsIImpl_AddRef(VdDlgIFileDialogEvents *This)
{
    (void)This;
    return 0;
}

VdDlgHRESULT vd_dlg__CFileDialogEventsIImpl_QueryInterface(VdDlgIFileDialogEvents *This, VdDlgREFIID riid, void **ppvObject)
{
    if (!ppvObject) {
        return VD_DLG__E_POINTER;
    }

    if (vd_dlg__guid_compare(riid, &VD_DLG_IID_IUnknown) || vd_dlg__guid_compare(riid, &VD_DLG_IID_FileDialogEvents)) {
        *ppvObject = (VdDlgIFileDialogEvents*)This;
        vd_dlg__CFileDialogEventsIImpl_AddRef(This);
        return 0;
    }

    *ppvObject = NULL;
    return VD_DLG__E_NOINTERFACE;
}

VdDlgULONG vd_dlg__CFileDialogEventsIImpl_Release(VdDlgIFileDialogEvents *This)
{
    (void)This;
    return 0;
}

VdDlgHRESULT vd_dlg__CFileDialogEventsIImpl_OnFileOk(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd)
{
    (void)This;
    (void)pfd;
    return 0;
}

VdDlgHRESULT vd_dlg__CFileDialogEventsIImpl_OnFolderChanging(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd, VdDlgIShellItem *psiFolder)
{
    (void)This;
    (void)pfd;
    (void)psiFolder;
    return 0;
}

VdDlgHRESULT vd_dlg__CFileDialogEventsIImpl_OnFolderChange(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd)
{
    (void)This;
    (void)pfd;
    return 0;
}

VdDlgHRESULT vd_dlg__CFileDialogEventsIImpl_OnSelectionChange(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd)
{
    (void)This;
    (void)pfd;
    return 0;
}

VdDlgHRESULT vd_dlg__CFileDialogEventsIImpl_OnShareViolation(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd, VdDlgIShellItem *psi, VdDlgFDE_SHAREVIOLATION_RESPONSE *pResponse)
{
    (void)This;
    (void)pfd;
    (void)psi;
    (void)pResponse;
    return 0;
}

VdDlgHRESULT vd_dlg__CFileDialogEventsIImpl_OnTypeChange(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd)
{
    (void)This;
    (void)pfd;
    return 0;
}

VdDlgHRESULT vd_dlg__CFileDialogEventsIImpl_OnOverwrite(VdDlgIFileDialogEvents *This, VdDlgIFileDialog *pfd, VdDlgIShellItem *psi, VdDlgFDE_OVERWRITE_RESPONSE *pResponse)
{
    (void)This;
    (void)pfd;
    (void)psi;
    (void)pResponse;
    return 0;
}

static VdDlg__Globals Vd_Dlg__Globals = {0};
static void      vd_dlg__win32_initialize(void);
static void*     vd_dlg__win32_realloc_mem(void *prev_ptr, size_t size);
static void      vd_dlg__win32_free_mem(void *memory);
static void*     vd_dlg__win32_resize_buffer(void *buffer, size_t element_size, int required_capacity, int *cap);
static int       vd_dlg__win32_cv_utf8_to_utf16(const char *ustr, int ustr_len, wchar_t **buffer, int *cap);
static int       vd_dlg__win32_cv_utf16_to_utf8(const wchar_t *wstr, int wstr_len, char **buffer, int *cap);
static VdDlgHWND vd_dlg__get_hwnd(void);

#define VD_DLG__WIN32_CHECK_HRESULT(expr) do { VdDlgHRESULT _hr_ = (expr); if (_hr_ != 0) { VD_DLG_ABORT(#expr " failed"); } } while(0)

VD_DLG_API int vd_dlg_message_box(int title_len, const char *title, int description_len, const char *description, VdDlgMessageBoxOptions options)
{
    vd_dlg__win32_initialize();

    vd_dlg__win32_cv_utf8_to_utf16(title, title_len,
                                   &Vd_Dlg__Globals.title_buf, &Vd_Dlg__Globals.title_buf_cap);

    vd_dlg__win32_cv_utf8_to_utf16(description, description_len,
                                   &Vd_Dlg__Globals.message_buf, &Vd_Dlg__Globals.message_buf_cap);

    int result = VdDlgMessageBoxW(vd_dlg__get_hwnd(),
                                  Vd_Dlg__Globals.message_buf,
                                  Vd_Dlg__Globals.title_buf,
                                  0x00000000L);

    return 1;
}

VD_DLG_API VdDlgFileResult vd_dlg_open_file(int title_len, const char *title, int path_len, const char *path, int num_filters, VdDlgFileFilter *filters, int default_filter, VdDlgOpenFileOptions options)
{
    VdDlgIFileDialog *pfd = NULL;
    VD_DLG__WIN32_CHECK_HRESULT(VdDlgCoCreateInstance(&VD_DLG_CLSID_FileOpenDialog, NULL,
                                                      VD_DLG_CLSCTX_INPROC_SERVER,
                                                      &VD_DLG_IID_FileOpenDialog, &pfd));

    VdDlgDWORD dw_cookie;
    VD_DLG__WIN32_CHECK_HRESULT(pfd->lpVtbl->Advise(pfd, &Vd_Dlg__CFileDialogEvents, &dw_cookie));

    VdDlgFILEOPENDIALOGOPTIONS dw_options;
    VD_DLG__WIN32_CHECK_HRESULT(pfd->lpVtbl->GetOptions(pfd, &dw_options));
    VD_DLG__WIN32_CHECK_HRESULT(pfd->lpVtbl->SetOptions(pfd, dw_options | VD_DLG_FOS_FORCEFILESYSTEM));

    if (title_len > 0) {

        vd_dlg__win32_cv_utf8_to_utf16(title, title_len,
                                       &Vd_Dlg__Globals.title_buf, &Vd_Dlg__Globals.title_buf_cap);

        VD_DLG__WIN32_CHECK_HRESULT(pfd->lpVtbl->SetTitle(pfd, Vd_Dlg__Globals.title_buf));
    }

    if (num_filters > 0) {
        Vd_Dlg__Globals.file_types = vd_dlg__win32_resize_buffer(Vd_Dlg__Globals.file_types,
                                                                 sizeof(VdDlgCOMDLG_FILTERSPEC), num_filters,
                                                                 &Vd_Dlg__Globals.file_types_cap);
        Vd_Dlg__Globals.file_types_caps = vd_dlg__win32_resize_buffer(Vd_Dlg__Globals.file_types_caps,
                                                                      sizeof(VdDlgFilterSpecCaps), num_filters,
                                                                      &Vd_Dlg__Globals.file_types_caps_cap);
        for (int i = 0; i < num_filters; ++i) {
            VdDlgCOMDLG_FILTERSPEC *ft  = &Vd_Dlg__Globals.file_types[i];
            VdDlgFilterSpecCaps    *cap = &Vd_Dlg__Globals.file_types_caps[i];

            vd_dlg__win32_cv_utf8_to_utf16(filters[i].description, filters[i].description_len,
                                           &ft->pszName, &cap->name);
            vd_dlg__win32_cv_utf8_to_utf16(filters[i].extensions, filters[i].extensions_len,
                                           &ft->pszSpec, &cap->spec);
        }

        VD_DLG__WIN32_CHECK_HRESULT(pfd->lpVtbl->SetFileTypes(pfd, num_filters, Vd_Dlg__Globals.file_types));
        VD_DLG__WIN32_CHECK_HRESULT(pfd->lpVtbl->SetFileTypeIndex(pfd, default_filter + 1));
    }

    VdDlgHRESULT hr = pfd->lpVtbl->Show(pfd, vd_dlg__get_hwnd());

    VdDlgFileResult rt_result = {0,0,0};

    if (hr == 0) {
        VdDlgIShellItem *result;
        hr = pfd->lpVtbl->GetResult(pfd, &result);
        if (hr == 0) {
            VdDlgLPWSTR result_path = NULL;
            VD_DLG__WIN32_CHECK_HRESULT(result->lpVtbl->GetDisplayName(result, VD_DLG_SIGDN_FILESYSPATH, &result_path));

            Vd_Dlg__Globals.file_results = vd_dlg__win32_resize_buffer(Vd_Dlg__Globals.file_results,
                                                                       sizeof(VdDlgFileResult), 1,
                                                                       &Vd_Dlg__Globals.file_results_cap);
            Vd_Dlg__Globals.file_results[0].len = vd_dlg__win32_cv_utf16_to_utf8(result_path, -1,
                                                                                 &Vd_Dlg__Globals.file_results[0].buf,
                                                                                 &Vd_Dlg__Globals.file_results[0].buf_cap);

            rt_result = Vd_Dlg__Globals.file_results[0];
        }
    }

    VD_DLG__WIN32_CHECK_HRESULT(pfd->lpVtbl->Unadvise(pfd, dw_cookie));
    VD_DLG__WIN32_CHECK_HRESULT(pfd->lpVtbl->Release(pfd));

    return rt_result;
}

VD_DLG_API void vd_dlg_set_context(void *pf_context)
{
    vd_dlg__win32_initialize();
    Vd_Dlg__Globals.hwnd = (VdDlgHWND*)pf_context;
}

static void vd_dlg__win32_initialize(void)
{
    if (Vd_Dlg__Globals.initialized) {
        return;
    }

    Vd_Dlg__Globals.initialized = 1;

    // User32.dll
    {
        VdDlgHMODULE mod = LoadLibraryA("User32.dll");
        VdDlgMessageBoxW = (VdDlgProcMessageBoxW)GetProcAddress(mod, "MessageBoxW");
    }

    // Ole32.dll
    {
        VdDlgHMODULE mod = LoadLibraryA("Ole32.dll");
        VdDlgCoInitialize     =     (VdDlgProcCoInitialize)GetProcAddress(mod, "CoInitialize");
        VdDlgCoCreateInstance = (VdDlgProcCoCreateInstance)GetProcAddress(mod, "CoCreateInstance");
        VdDlgCoTaskMemFree    =    (VdDlgProcCoTaskMemFree)GetProcAddress(mod, "CoTaskMemFree");

    }

    VdDlgCoInitialize(NULL);

    Vd_Dlg__CFileDialogEventsIImpl.QueryInterface    = vd_dlg__CFileDialogEventsIImpl_QueryInterface;
    Vd_Dlg__CFileDialogEventsIImpl.AddRef            = vd_dlg__CFileDialogEventsIImpl_AddRef;
    Vd_Dlg__CFileDialogEventsIImpl.Release           = vd_dlg__CFileDialogEventsIImpl_Release;
    Vd_Dlg__CFileDialogEventsIImpl.OnFileOk          = vd_dlg__CFileDialogEventsIImpl_OnFileOk;
    Vd_Dlg__CFileDialogEventsIImpl.OnFolderChanging  = vd_dlg__CFileDialogEventsIImpl_OnFolderChanging;
    Vd_Dlg__CFileDialogEventsIImpl.OnFolderChange    = vd_dlg__CFileDialogEventsIImpl_OnFolderChange;
    Vd_Dlg__CFileDialogEventsIImpl.OnSelectionChange = vd_dlg__CFileDialogEventsIImpl_OnSelectionChange;
    Vd_Dlg__CFileDialogEventsIImpl.OnShareViolation  = vd_dlg__CFileDialogEventsIImpl_OnShareViolation;
    Vd_Dlg__CFileDialogEventsIImpl.OnTypeChange      = vd_dlg__CFileDialogEventsIImpl_OnTypeChange;
    Vd_Dlg__CFileDialogEventsIImpl.OnOverwrite       = vd_dlg__CFileDialogEventsIImpl_OnOverwrite;
    Vd_Dlg__CFileDialogEvents.lpVtbl = &Vd_Dlg__CFileDialogEventsIImpl;
}

static void *vd_dlg__win32_realloc_mem(void *prev_ptr, size_t size)
{
    if (prev_ptr == 0) {
        return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size);
    } else {
        return HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, prev_ptr, size);
    }
}

static void vd_dlg__win32_free_mem(void *memory)
{
    HeapFree(GetProcessHeap(), 0, memory);
}

static void *vd_dlg__win32_resize_buffer(void *buffer, size_t element_size, int required_capacity, int *cap)
{
    if (required_capacity <= *cap) {
        return buffer;
    }

    int resize_capacity = required_capacity;
    buffer = vd_dlg__win32_realloc_mem(buffer, element_size * resize_capacity);
    *cap = resize_capacity;
    return buffer;
}

static int vd_dlg__win32_cv_utf8_to_utf16(const char *ustr, int ustr_len, wchar_t **buffer, int *cap)
{
    int wstr_len = MultiByteToWideChar(65001, 8, ustr, ustr_len, NULL, 0);
    *buffer = vd_dlg__win32_resize_buffer(*buffer, sizeof(wchar_t), wstr_len + 1, cap);

    MultiByteToWideChar(65001, 8, ustr, ustr_len, *buffer, wstr_len);
    (*buffer)[wstr_len] = 0;

    return wstr_len;
}

static int vd_dlg__win32_cv_utf16_to_utf8(const wchar_t *wstr, int wstr_len, char **buffer, int *cap)
{
    int req = WideCharToMultiByte(65001, 0, wstr, wstr_len, 0, 0, NULL, NULL);
    *buffer = vd_dlg__win32_resize_buffer(*buffer, sizeof(char), req + 1, cap);

    WideCharToMultiByte(65001, 0, wstr, wstr_len, *buffer, req, NULL, NULL);
    (*buffer)[req] = 0;

    return req;
}

static VdDlgHWND vd_dlg__get_hwnd(void)
{
    if (Vd_Dlg__Globals.hwnd != 0) {
        return *Vd_Dlg__Globals.hwnd;
    } else {
        return (VdDlgHWND)NULL;
    }
}

#endif // _WIN32

#endif // VD_DLG_IMPL
