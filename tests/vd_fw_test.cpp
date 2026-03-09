#define VD_USE_CRT 1
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#define VD_INCLUDE_TESTS 1
#include "vd.h"
#include "vd_fw.h"

#ifdef _WIN32
#include <windows.h>
#pragma comment(lib, "User32.lib")
#endif

#define ENABLE_SLOW_TESTS 0
#ifndef ENABLE_SLOW_TESTS
#   define ENABLE_SLOW_TESTS 1
#endif // !ENABLE_SLOW_TESTS

static void get_fw_window_rect(int *left, int *top, int *right, int *bottom);
static void move_cursor(int x, int y);
static void sleep_ms(int s);

VD_TEST("Basic/Init Quit Multiple") {

    for (int i = 0; i < 10; ++i) {
        vd_fw_init(0);

        while (vd_fw_running()) {
            vd_fw_poll(0);

            vd_fw_quit();
            vd_fw_lock();
            vd_fw_unlock();
        }

        vd_fw_exit();
    }

    VD_TEST_OK();
}

#if ENABLE_SLOW_TESTS
VD_TEST("Basic/Borderless Window Mouse locations") {
    VdFwInitInfo init_info = {0};
    init_info.window_options.borderless = 1;
    vd_fw_init(&init_info);

    vd_fw_set_size(640, 480);

    struct {
        int x, y;
    } move_positions[4] = {
        {0, 0},
        {0, 1},
        {1, 0},
        {1, 1},
    };

    int i = 0;
    int moved = 0;

    while (vd_fw_running()) {
        sleep_ms(1000);
        vd_fw_poll(0);

        if (i < 4) {
            int left, top, right, bottom;
            get_fw_window_rect(&left, &top, &right, &bottom);

            if (!moved) {
                int x = left + move_positions[i].x * (right - left - 1);
                int y = top  + move_positions[i].y * (bottom - top - 1);

                // VD_TEST_LOG("Moving cursor to %d %d", x, y);
                move_cursor(x, y);
                sleep_ms(1000);

                moved = 1;
            } else {
                int x, y;
                vd_fw_get_mouse_state(&x, &y);

                int e_x = move_positions[i].x * 640;
                if (e_x != 0) {
                    e_x -= 1;
                }

                int e_y = move_positions[i].y * 480;
                if (e_y != 0) {
                    e_y -= 1;
                }

#undef VD_TEST_ERR_CODE
#define VD_TEST_ERR_CODE {vd_fw_exit();}
                // VD_TEST_LOG("Checking %d %d against %d %d", x, y, e_x, e_y);
                VD_TEST_EQ("X Eq to Corner X", x, e_x);
                VD_TEST_EQ("Y Eq to Corner Y", y, e_y);
#undef VD_TEST_ERR_CODE
#define VD_TEST_ERR_CODE

                i++;
                moved = 0;
            }
        } else {
            vd_fw_quit();
        }

        vd_fw_lock();
        vd_fw_unlock();
    }

    vd_fw_exit();

    VD_TEST_OK();
}

#endif // !ENABLE_SLOW_TESTS

VD_TEST("GL/Multiple Configs") {
    VdFwGlExtension bogus_extensions[] = {
        { "GL_DRM_threater" },
        0
    };

    VdFwGlConfig bogus_config1;
    VD_MEMSET(&bogus_config1, 0, sizeof(bogus_config1));
    bogus_config1.version = VD_FW_GL_VERSION_3_3;
    bogus_config1.req_extensions = bogus_extensions;

    VdFwGlConfig bogus_config2;
    VD_MEMSET(&bogus_config2, 0, sizeof(bogus_config2));
    bogus_config2.version = VD_FW_GL_VERSION_1_5;
    bogus_config2.req_extensions = bogus_extensions;

    VdFwGlConfig actual_config;
    VD_MEMSET(&actual_config, 0, sizeof(actual_config));
    actual_config.version = VD_FW_GL_VERSION_3_3;

    VdFwGlConfig configs[] = {
        bogus_config1,
        bogus_config2,
        actual_config,
        {VD_FW_GL_VERSION_BASIC}
    };

    VdFwInitInfo info;
    VD_MEMSET(&info, 0, sizeof(info));
    info.gl.configs = configs;
    vd_fw_init(&info);

    VD_TEST_EQ("1st, 2nd configs are bogus; must select 3rd config", info.gl.selected_config, 2);

    vd_fw_exit();

    VD_TEST_OK();
}

#ifdef _WIN32
#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

static IDXGIFactory2            *DXGI_Factory;
static IDXGISwapChain1          *DXGI_Swapchain;
static ID3D11Device             *D_Device;
static ID3D11DeviceContext      *D_Device_Context;
static ID3D11RenderTargetView   *Render_Target_View;

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#define CHECK_HRESULT(expr) if ((expr) != 0) { VD_TEST_ERR(#expr); }

VD_TEST("DX11/Window") {
#undef VD_TEST_ERR_CODE
#define VD_TEST_ERR_CODE {vd_fw_exit();}
    VdFwInitInfo init_info = {0};
    init_info.api = VD_FW_GRAPHICS_API_CUSTOM;


    vd_fw_init(&init_info);

    CHECK_HRESULT(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGI_Factory));

    int width, height;
    vd_fw_get_size(&width, &height);

    D3D_FEATURE_LEVEL device_feature_level = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
    CHECK_HRESULT(D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        &device_feature_level,
        1,
        D3D11_SDK_VERSION,
        &D_Device,
        nullptr,
        &D_Device_Context));

    HWND *hwnd = (HWND*)vd_fw_get_internal_window_handle();

    DXGI_SWAP_CHAIN_DESC1 swapchain_descriptor = {};
    swapchain_descriptor.Width = width;
    swapchain_descriptor.Height = height;
    swapchain_descriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    swapchain_descriptor.SampleDesc.Count = 1;
    swapchain_descriptor.SampleDesc.Quality = 0;
    swapchain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_descriptor.BufferCount = 2;
    swapchain_descriptor.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchain_descriptor.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
    swapchain_descriptor.Flags = {};

    CHECK_HRESULT(DXGI_Factory->CreateSwapChainForHwnd(
        D_Device,
        *hwnd,
        &swapchain_descriptor,
        NULL,
        NULL,
        &DXGI_Swapchain));

    {
        ComPtr<ID3D11Texture2D> backbuffer = nullptr;
        CHECK_HRESULT(DXGI_Swapchain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)));

        CHECK_HRESULT(D_Device->CreateRenderTargetView(backbuffer.Get(), nullptr, &Render_Target_View));
    }

    while (vd_fw_running()) {
        vd_fw_poll(0);

        vd_fw_lock();
        int w, h;
        if (vd_fw_get_size(&w, &h)) {
            D_Device_Context->OMSetRenderTargets(0, 0, 0);
            Render_Target_View->Release();

            CHECK_HRESULT(DXGI_Swapchain->ResizeBuffers(0,w,h,DXGI_FORMAT_B8G8R8A8_UNORM, 0));

            ComPtr<ID3D11Texture2D> backbuffer = nullptr;
            CHECK_HRESULT(DXGI_Swapchain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)));

            CHECK_HRESULT(D_Device->CreateRenderTargetView(backbuffer.Get(), nullptr, &Render_Target_View));

        }

        if (vd_fw_get_key_pressed(VD_FW_KEY_F11)) {
            vd_fw_set_fullscreen(!vd_fw_get_fullscreen());
        }

        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0;
        viewport.TopLeftY = 0;
        viewport.Width = (float)(w);
        viewport.Height = (float)(h);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        float clearColor[] = { 0.5f, 0.1f, 0.1f, 1.0f };

        D_Device_Context->ClearRenderTargetView(
            Render_Target_View,
            clearColor);
        D_Device_Context->RSSetViewports(
            1,
            &viewport);
        D_Device_Context->OMSetRenderTargets(
            1,
            &Render_Target_View,
            nullptr);
        DXGI_Swapchain->Present(1, 0);

        vd_fw_unlock();

        vd_fw_quit();
    }

    vd_fw_exit();

    VD_TEST_OK();    
#undef VD_TEST_ERR_CODE
#define VD_TEST_ERR_CODE
}
#endif // _WIN32

int main(int argc, char const *argv[])
{
    VD_UNUSED(argc);
    VD_UNUSED(argv);
    vd_test_main();
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"

#define VD_IMPL
#include "vd.h"

#ifdef _WIN32
static void get_fw_window_rect(int *left, int *top, int *right, int *bottom)
{
    RECT r;
    GetWindowRect((HWND)VD_FW_G.hwnd, &r);

    *left = r.left;
    *top = r.top;
    *right = r.right;
    *bottom = r.bottom;
}

static void move_cursor(int x, int y)
{
    SetCursorPos(x, y);
}

static void sleep_ms(int s)
{
    Sleep(s);
}
#endif
