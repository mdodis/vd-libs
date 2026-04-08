#ifndef VD_AU_H
#define VD_AU_H

#ifndef VD_AU_API
#   ifdef VD_AU_STATIC
#       define VD_AU_API static
#   else
#       define VD_AU_API extern
#   endif // VD_AU_STATIC
#endif // !VD_AU_API

typedef enum {
    VD_AU_FORMAT_NONE  = 0,
    VD_AU_FORMAT_FLOAT = 1,
} VdAuFormat;

typedef enum {
    VD_AU_STAT_OK         = 0,
    VD_AU_STAT_TOO_LARGE  = -1,
    VD_AU_STAT_DONT_WRITE = -2,
    VD_AU_STAT_DONT_KNOW  = 1,
} VdAuStat;

typedef void (*VdAuCallback)(int num_frames, float *frames, void *user_data);

// Define VD_AU_THREAD_ID_CUSTOM to whatever your thread type is
#ifndef VD_AU_THREAD_ID_CUSTOM
typedef void *VdAuThreadId;
#endif // !VD_AU_THREAD_ID_CUSTOM

typedef struct {
    // For you
    void            *user_data;
    // 1 Frame = [sizeof(format) * channels]
    // Size/Format of a single sample (Frame = [sample x channels])
    VdAuFormat      format;
    // Number of channels per frame
    int             channels;
    // Number of frames per second of audio, i.e. sample rate
    int             frame_rate;
    // Number of frames in buffer
    int             frame_count;

    VdAuCallback    callback;
    void            *callback_user_data;

    volatile int    running;

    // Backend Fields               | WASAPI                    |
    // -----------------------------|---------------------------|--------
    void            *_internal1; // | IMMDevice                 |
    void            *_internal2; // | IAudioClient              |
    void            *_internal3; // | IAudioRenderClient        |
    void            *_internal4; // | IAudioClock               |
    void            *_internal5; // | IAudioClock::GetFrequency |
    void            *_internal6; // | HANDLE (Thread)           |

    int             inflight_frames;
} VdAuEndpoint;

typedef struct {
    VdAuThreadId (*make_thread)(void *(*proc)(void *arg));
} VdAuInitInfo;

VD_AU_API void          vd_au_init(void);
VD_AU_API VdAuEndpoint  vd_au_endpoint_default(void);
VD_AU_API void          vd_au_endpoint_start(VdAuEndpoint *endpoint);
VD_AU_API VdAuStat      vd_au_endpoint_pos(VdAuEndpoint *endpoint, int *pos);
VD_AU_API void          vd_au_endpoint_stop(VdAuEndpoint *endpoint);

VD_AU_API int           vd_au_endpoint_frames_available(VdAuEndpoint *endpoint);
VD_AU_API VdAuStat      vd_au_endpoint_write_begin(VdAuEndpoint *endpoint, int num_frames, void **ptr);
VD_AU_API void          vd_au_endpoint_write_end(VdAuEndpoint *endpoint);
VD_AU_API void          vd_au_endpoint_free(VdAuEndpoint *endpoint);

#endif // !VD_AU_H

#ifdef VD_AU_IMPL
#include <stdint.h>
#ifdef _WIN32
#define VD_AU__WIN32_REFTIMES_SEC  10000000
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
DEFINE_GUID(Ksdf_SUBTYPE_IEEE_FLOAT, 0x00000003, 0x0000, 0x0010, 0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71);
DEFINE_GUID(IID_IAudioClock,         0xCD63314F, 0x3FBA, 0x4a1b, 0x81, 0x2C, 0xEF, 0x96, 0x35, 0x87, 0x28, 0xE7);

#define VD_AU__CHECK_HRESULT(expr) do {\
    HRESULT _hr = (expr);\
    if (_hr != S_OK) { printf("Failed at: %s: %d\n", #expr, _hr); DebugBreak(); } \
} while (0)

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "avrt.lib")

typedef struct {
    IMMDeviceEnumerator *enumerator;
} VdAu__Win32InternalData;

static VdAu__Win32InternalData Vd_Au_G = {0};

static DWORD vd_au__wasapi_thread_proc(LPVOID param);

VD_AU_API void vd_au_init(void)
{
    IMMDeviceEnumerator *pEnumerator = NULL;

    CoInitializeEx(0, COINIT_MULTITHREADED);
    VD_AU__CHECK_HRESULT(CoCreateInstance(&CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL,
                                          &IID_IMMDeviceEnumerator, (void**)&pEnumerator));

    Vd_Au_G.enumerator = pEnumerator;
}

VD_AU_API VdAuEndpoint vd_au_endpoint_default(void)
{
    IMMDevice           *pDevice = NULL;
    IAudioClient        *audio_client = NULL;
    IAudioRenderClient  *render = NULL;
    IAudioClock         *clock = NULL;

    VD_AU__CHECK_HRESULT(IMMDeviceEnumerator_GetDefaultAudioEndpoint(Vd_Au_G.enumerator, eRender, eConsole, &pDevice));

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
    IPropertyStore_Release(pProps);

    VD_AU__CHECK_HRESULT(IMMDevice_Activate(pDevice, &IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&audio_client));

    WAVEFORMATEX *wv;
    VD_AU__CHECK_HRESULT(IAudioClient_GetMixFormat(audio_client, &wv));

    if (wv->wFormatTag == WAVE_FORMAT_EXTENSIBLE) {
        WAVEFORMATEXTENSIBLE *wvex = (WAVEFORMATEXTENSIBLE*)wv;

        if (IsEqualGUID(&Ksdf_SUBTYPE_IEEE_FLOAT, &wvex->SubFormat)) {
            printf("Using ieee floats for PCM\n");
        }
    }

    VD_AU__CHECK_HRESULT(IAudioClient_Initialize(audio_client, AUDCLNT_SHAREMODE_SHARED,
                                                 0,
                                                 VD_AU__WIN32_REFTIMES_SEC,
                                                 0,
                                                 wv,
                                                 NULL));

    UINT32 buffer_frame_count = 0;
    VD_AU__CHECK_HRESULT(IAudioClient_GetBufferSize(audio_client, &buffer_frame_count));

    VD_AU__CHECK_HRESULT(IAudioClient_GetService(audio_client, &IID_IAudioRenderClient, &render));
    VD_AU__CHECK_HRESULT(IAudioClient_GetService(audio_client, &IID_IAudioClock, &clock));

    uint64_t freq;
    VD_AU__CHECK_HRESULT(IAudioClock_GetFrequency(clock, &freq));

    VdAuEndpoint result = {0};
    result.frame_rate  = wv->nSamplesPerSec;
    result.frame_count = (int)buffer_frame_count;
    result.channels    = wv->nChannels;
    result.format      = VD_AU_FORMAT_FLOAT;
    result._internal1  = (void*)pDevice;
    result._internal2  = (void*)audio_client;
    result._internal3  = (void*)render;
    result._internal4  = (void*)clock;
    result._internal5  = (void*)freq;
    return result;
}

VD_AU_API void vd_au_endpoint_callback_set(void (*callback)(int num_frames, float *frames, void *usr), void *usr)
{

}

VD_AU_API int vd_au_endpoint_frames_available(VdAuEndpoint *endpoint)
{
    IAudioClient *audio_client = (IAudioClient*)endpoint->_internal2;

    UINT32 buffer_padding = 0;
    IAudioClient_GetCurrentPadding(audio_client, &buffer_padding);

    int result = endpoint->frame_count - (int)buffer_padding;
    if (result < 0) {
        result = 0;
    }

    return result;
}

VD_AU_API VdAuStat vd_au_endpoint_write_begin(VdAuEndpoint *endpoint, int num_frames, uint8_t **ptr)
{
    IAudioRenderClient *render = (IAudioRenderClient*)endpoint->_internal3;
    UINT32 frames_req = (UINT32)num_frames;
    BYTE *pdata = 0;
    VdAuStat result;

    if (frames_req) {
        HRESULT hr = IAudioRenderClient_GetBuffer(render, frames_req, &pdata);

        switch (hr) {
            case 0:                          endpoint->inflight_frames = num_frames; result = VD_AU_STAT_OK;        break; 
            case AUDCLNT_E_BUFFER_ERROR:                                             result = VD_AU_STAT_DONT_KNOW; break; 
            case AUDCLNT_E_BUFFER_TOO_LARGE:                                         result = VD_AU_STAT_TOO_LARGE; break; 
            default:                                                                 result = VD_AU_STAT_DONT_KNOW; break; 
        }
    } else {
        result = VD_AU_STAT_DONT_WRITE;
    }

    *ptr = (void*)pdata;
    
    return result;
}

VD_AU_API void vd_au_endpoint_write_end(VdAuEndpoint *endpoint)
{
    IAudioRenderClient *render = (IAudioRenderClient*)endpoint->_internal3;

    HRESULT hr = IAudioRenderClient_ReleaseBuffer(render, (UINT32)endpoint->inflight_frames, 0);
    endpoint->inflight_frames = 0;
}


VD_AU_API void vd_au_endpoint_start(VdAuEndpoint *endpoint)
{
    IAudioClient *audio_client = (IAudioClient*)endpoint->_internal2;
    endpoint->running = 1;
    VD_AU__CHECK_HRESULT(IAudioClient_Start(audio_client));

    HANDLE thread = CreateThread(0, 0, vd_au__wasapi_thread_proc, (void*)endpoint, 0, 0);
    endpoint->_internal6 = (void*)thread;
}

VD_AU_API VdAuStat vd_au_endpoint_pos(VdAuEndpoint *endpoint, int *pos)
{
    IAudioClock *clock = (IAudioClock*)endpoint->_internal4;
    uint64_t freq = (uint64_t)endpoint->_internal5;

    uint64_t freq_dep_pos;
    uint64_t pc;
    HRESULT hr = IAudioClock_GetPosition(clock, &freq_dep_pos, &pc);
    uint64_t pcm_pos = (freq_dep_pos / freq);

    VdAuStat result;
    switch (hr) {
        case 0:  result = VD_AU_STAT_OK; break;
        default: result = VD_AU_STAT_DONT_KNOW; break;
    }

    return result;
}

VD_AU_API void vd_au_endpoint_stop(VdAuEndpoint *endpoint)
{
    IAudioClient *audio_client = (IAudioClient*)endpoint->_internal2;
    VD_AU__CHECK_HRESULT(IAudioClient_Stop(audio_client));
}

VD_AU_API void vd_au_endpoint_free(VdAuEndpoint *endpoint)
{

    IMMDevice           *device       = (IMMDevice*)endpoint->_internal1;
    IAudioClient        *audio_client = (IAudioClient*)endpoint->_internal2;
    IAudioRenderClient  *render       = (IAudioRenderClient*)endpoint->_internal3;
    IAudioClock         *clock        = (IAudioClock*)endpoint->_internal4;

    IAudioClient_Stop(audio_client);

    IAudioClock_Release(clock);
    IAudioRenderClient_Release(render);
    IAudioClient_Release(audio_client);
    IMMDevice_Release(device);
}

static DWORD vd_au__wasapi_thread_proc(LPVOID param)
{
    VdAuEndpoint *endpoint = (VdAuEndpoint*)param;

    int is_running = 1;
    while (is_running) {
        is_running = InterlockedCompareExchange((volatile LONG*)&endpoint->running, 0, 0);

        int frames_available = vd_au_endpoint_frames_available(endpoint);
        uint8_t *ptr = 0;
        VdAuStat stat = vd_au_endpoint_write_begin(endpoint, frames_available, &ptr);
        if (stat == VD_AU_STAT_OK) {

            endpoint->callback(frames_available, (float*)ptr, endpoint->callback_user_data);

            vd_au_endpoint_write_end(endpoint);
        } else if (stat > 0) {
            return 0x8000;
        } else {
            continue;
        }
    }

    return 0;
}

#endif
#endif // VD_AU_IMPL