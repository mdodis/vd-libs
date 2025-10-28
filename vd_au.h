#ifndef VD_AU_H
#define VD_AU_H

#ifndef VD_AU_API
#   ifdef VD_AU_STATIC
#       define VD_AU_API static
#   else
#       define VD_AU_API extern
#   endif // VD_AU_STATIC
#endif // !VD_AU_API

VD_AU_API void vd_au_init(void);

#endif // !VD_AU_H

#ifdef VD_AU_IMPL
#ifdef _WIN32
#define INITGUID
#define COBJMACROS
#define CINTERFACE
#include <windows.h>
#include <mmdeviceapi.h>
#include <initguid.h>
#include <audioclient.h>
#include <Functiondiscoverykeys_devpkey.h>

DEFINE_GUID(CLSID_MMDeviceEnumerator,0xBCDE0395, 0xE52F, 0x467C, 0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E);
DEFINE_GUID(IID_IMMDeviceEnumerator, 0xA95664D2, 0x9614, 0x4F35, 0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6);
DEFINE_GUID(IID_IAudioClient,        0x1CB9AD4C, 0xDBFA, 0x4c32, 0xB1, 0x78, 0xC2, 0xF5, 0x68, 0xA7, 0x03, 0xB2);
DEFINE_GUID(IID_IAudioRenderClient,  0xF294ACFC, 0x3146, 0x4483, 0xA7, 0xBF, 0xAD, 0xDC, 0xA7, 0xC2, 0x60, 0xE2);

#define VD_AU__CHECK_HRESULT(expr) do {\
    if ((expr) != S_OK) { printf("Failed at: %s\n", #expr); DebugBreak(); } \
} while (0)

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "avrt.lib")
VD_AU_API void vd_au_init(void)
{
    IMMDeviceEnumerator *pEnumerator = NULL;
    IMMDevice *pDevice = NULL;
    IAudioClient *pAudioClient = NULL;

    CoInitializeEx(0, COINIT_MULTITHREADED);
    VD_AU__CHECK_HRESULT(CoCreateInstance(&CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL,
                                          &IID_IMMDeviceEnumerator, (void**)&pEnumerator));

    VD_AU__CHECK_HRESULT(IMMDeviceEnumerator_GetDefaultAudioEndpoint(pEnumerator, eRender, eConsole, &pDevice));

    LPWSTR device_id = NULL;
    VD_AU__CHECK_HRESULT(IMMDevice_GetId(pDevice, &device_id));

    IPropertyStore *pProps = NULL;
    VD_AU__CHECK_HRESULT(IMMDevice_OpenPropertyStore(pDevice, STGM_READ, &pProps));

    PROPVARIANT var_name;
    PropVariantInit(&var_name);
    {

        VD_AU__CHECK_HRESULT(IPropertyStore_GetValue(pProps, &PKEY_Device_FriendlyName, &var_name));

        if (var_name.vt != VT_EMPTY) {
            printf("Endpoint: %S\n", var_name.pwszVal);
        }

    }
    PropVariantClear(&var_name);

    VD_AU__CHECK_HRESULT(IMMDevice_Activate(pDevice, &IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&pAudioClient));

    WAVEFORMATEX *pWaveFormat;
    VD_AU__CHECK_HRESULT(IAudioClient_GetMixFormat(pAudioClient, &pWaveFormat));


}
#endif
#endif // VD_AU_IMPL