#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_WINDOWS
#include "vd_fw.h"
#include <stdio.h>

#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <assert.h>

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

static IDXGIFactory2            *DXGI_Factory;
static IDXGISwapChain1          *DXGI_Swapchain;
static ID3D11Device             *D_Device;
static ID3D11DeviceContext      *D_Device_Context;
static ID3D11RenderTargetView   *Render_Target_View;

#define CHECK_HRESULT(expr) if ((expr) != 0) { OutputDebugStringA("Failed: " __FILE__ "\n"); DebugBreak(); }

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
        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

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

        vd_fw_swap_buffers();
    }

    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"
