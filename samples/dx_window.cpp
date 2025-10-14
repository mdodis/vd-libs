#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#include "vd_fw.h"
#include <stdio.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <assert.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

static IDXGIFactory2        *DXGI_Factory;
static IDXGISwapChain1      *DXGI_Swapchain;
static ID3D11Device         *D_Device;
static ID3D11DeviceContext  *D_Device_Context;

#define CHECK_HRESULT(expr) if ((expr) != 0) { DebugBreak(); }

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    VdFwInitInfo init_info = {};
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
    swapchain_descriptor.Format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
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

    while (vd_fw_running()) {

        int w, h;
        vd_fw_get_size(&w, &h);

        int minimized;
        if (vd_fw_get_minimized(&minimized)) {
            printf("Minimization Changed: %d\n", minimized);
        }

        int maximized;
        if (vd_fw_get_maximized(&maximized)) {
            printf("Maximization Changed: %d\n", maximized);
        }

        if (vd_fw_get_key_pressed(11)) {
            vd_fw_set_fullscreen(!vd_fw_get_fullscreen());
        }

        glViewport(0, 0, w, h);
        glClearColor(0.5f, 0.3f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        vd_fw_swap_buffers();
    }

    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
