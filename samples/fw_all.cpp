#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#include "vd_fw.h"
#include "stb_image.h"

#include <stdio.h>

typedef enum {
    GRAPHICS_BACKEND_OPENGL,
    GRAPHICS_BACKEND_DIRECT3D11,
    GRAPHICS_BACKEND_VULKAN,
} GraphicsBackend;

typedef struct {
    unsigned char *data;
    int w, h;
} ImageData;

static const char *graphics_backend_to_string(GraphicsBackend backend)
{
    switch (backend) {
        case GRAPHICS_BACKEND_OPENGL: return "GRAPHICS_BACKEND_OPENGL";
        case GRAPHICS_BACKEND_DIRECT3D11: return "GRAPHICS_BACKEND_DIRECT3D11";
        default: return 0;
    }
}

typedef struct {
    GraphicsBackend backend;
    void (*init)(void *internal_window_handle);
    void (*kill)(void);
    void (*resize)(int w, int h);
    void (*draw_with_background_color)(float r, float g, float b, float a);
} GraphicsBackendImpl;

static VdFwGraphicsApi cv_graphics_backend_to_fw_graphics_api(GraphicsBackend backend)
{
    switch (backend) {
        case GRAPHICS_BACKEND_OPENGL: return VD_FW_GRAPHICS_API_OPENGL;
        default: return VD_FW_GRAPHICS_API_CUSTOM;
    }
}

extern GraphicsBackendImpl OpenGL_Impl;
extern GraphicsBackendImpl Vulkan_Impl;
#ifdef _WIN32
extern GraphicsBackendImpl D3D11_Impl;
#endif

static GraphicsBackendImpl* Graphics_Backends[] = {
    &OpenGL_Impl,
    &Vulkan_Impl,
#ifdef _WIN32
    &D3D11_Impl,
#endif
};

static GraphicsBackendImpl *Current_Graphics = NULL;

static struct {
    ImageData opengl_logo;
    ImageData directx_logo;
} all;

static void switch_to_graphics_backend(GraphicsBackendImpl *impl)
{
    if (Current_Graphics != NULL) {
        printf("Switching from %s to %s\n",
               graphics_backend_to_string(Current_Graphics->backend),
               graphics_backend_to_string(impl->backend));

        Current_Graphics->kill();
    } else {
        printf("Starting with %s\n",
               graphics_backend_to_string(impl->backend));
    }

    Current_Graphics = impl;
    vd_fw_set_graphics_api(cv_graphics_backend_to_fw_graphics_api(impl->backend), 0);

    impl->init(vd_fw_get_internal_window_handle());
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    int nc;
    all.opengl_logo.data = stbi_load("./assets/opengl.png",
                                     &all.opengl_logo.w, &all.opengl_logo.h,
                                     &nc, 4);
    all.directx_logo.data = stbi_load("./assets/directx.png",
                                     &all.directx_logo.w, &all.directx_logo.h,
                                     &nc, 4);

    int count_backends = VD_FW_ARRAY_COUNT(Graphics_Backends);
    printf("%d Backends\n", count_backends);

    int current_backend_index = 0;

    VdFwInitInfo init_info = {};
    init_info.api = VD_FW_GRAPHICS_API_INVALID;
    vd_fw_init(&init_info);

    switch_to_graphics_backend(Graphics_Backends[current_backend_index]);

    while (vd_fw_running()) {
        vd_fw_poll(0);

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        vd_fw_lock();
        int w, h;
        if (vd_fw_get_size(&w, &h)) {
            Current_Graphics->resize(w, h);
        }

        Current_Graphics->draw_with_background_color(0.2f,0.2f,0.2f,1.f);

        int will_switch_to_next_backend = 0;
        if (vd_fw_get_key_pressed(VD_FW_KEY_SPACE)) {
            will_switch_to_next_backend = 1;
        }

        vd_fw_swap();
        vd_fw_unlock();

        if (will_switch_to_next_backend) {
            current_backend_index += 1;
            if (current_backend_index >= count_backends) {
                current_backend_index = 0;
            }

            switch_to_graphics_backend(Graphics_Backends[current_backend_index]);
        }
    }
    return 0;
}

#define VD_FW_IMPL
#include "vd_fw.h"

static GLuint Tex_Opengl;
static GLuint Shader_Opengl;
static GLuint Vbo_Opengl;
static GLuint Vao_Opengl;

#define GL_VERTEX_SOURCE \
"#version 330 core                                                                                                 \n" \
"layout (location = 0) in vec2 aPos;                                                                               \n" \
"layout (location = 1) in vec2 aTexCoord;                                                                          \n" \
"                                                                                                                  \n" \
"uniform vec2 rect_off;                                                                                            \n" \
"uniform vec2 rect_size;                                                                                           \n" \
"uniform mat4 projection;                                                                                          \n" \
"                                                                                                                  \n" \
"out vec2 rect_uv;                                                                                                 \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    rect_uv = aTexCoord;                                                                                          \n" \
"    gl_Position = projection * vec4(aPos * rect_size + rect_off, 0.0, 1.0f);                                      \n" \
"}                                                                                                                 \n" \

#define GL_FRAGMENT_SOURCE \
"#version 330 core                                                                                                 \n" \
"precision highp float;                                                                                            \n" \
"in  vec2 rect_uv;                                                                                                 \n" \
"out vec4 FragColor;                                                                                               \n" \
"                                                                                                                  \n" \
"/* Gradient noise from Jorge Jimenez's presentation: */                                                           \n" \
"/* http://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare */                      \n" \
"float gradientNoise(in vec2 uv)                                                                                   \n" \
"{                                                                                                                 \n" \
"    return fract(52.9829189 * fract(dot(uv, vec2(0.06711056, 0.00583715))));                                      \n" \
"}                                                                                                                 \n" \
"                                                                                                                  \n" \
"uniform vec4 rect_color;                                                                                          \n" \
"uniform sampler2D rect_texture;                                                                                   \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    vec4 tex_color = texture(rect_texture, rect_uv);                                                              \n" \
"    vec4 color = rect_color * tex_color;                                                                          \n" \
"    color.rgb *= color.a;                                                                                         \n" \
"    color.rgb += (1.0 / 128.0) * gradientNoise(gl_FragCoord.xy) - (0.5 / 128.0);                                  \n" \
"    FragColor = color;                                                                                            \n" \
"}                                                                                                                 \n" \

static GLuint opengl_load_texture(ImageData *image_data)
{
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, image_data->w, image_data->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data->data);
    return texture;
}

void opengl_init(void *internal_window_handle)
{
    (void)internal_window_handle;

    Tex_Opengl = opengl_load_texture(&all.opengl_logo);

    GLuint vs = vd_fw_compile_shader(GL_VERTEX_SHADER, GL_VERTEX_SOURCE);
    GLuint fs = vd_fw_compile_shader(GL_FRAGMENT_SHADER, GL_FRAGMENT_SOURCE);

    Shader_Opengl = glCreateProgram();
    glAttachShader(Shader_Opengl, vs);
    glAttachShader(Shader_Opengl, fs);
    vd_fw_link_program(Shader_Opengl);
    glDeleteShader(vs);
    glDeleteShader(fs);

    float rect_vertices[] = {
         0.0f,  0.0f, 0.f, 0.f,
        +1.0f,  0.0f, 1.f, 0.f,
         0.0f, +1.0f, 0.f, 1.f,
        +1.0f, +1.0f, 1.f, 1.f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(sizeof(float) * 2));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_BLEND);
    Vbo_Opengl = VBO;
    Vao_Opengl = VAO;

    int w, h;
    vd_fw_get_size(&w, &h);
    glViewport(0, 0, w, h);
}

void opengl_kill(void)
{
    glUseProgram(0);
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteProgram(Shader_Opengl);
    glDeleteTextures(1, &Tex_Opengl);
    glDeleteVertexArrays(1, &Vao_Opengl);
    glDeleteBuffers(1, &Vbo_Opengl);
}

void opengl_resize(int w, int h)
{
    float projection[16];
    vd_fw_u_ortho(0.f, (float)w, (float)h, 0.f, -1.f, 1.f, projection);
    glUniformMatrix4fv(glGetUniformLocation(Shader_Opengl, "projection"), 1, GL_FALSE, projection);
    glViewport(0, 0, w, h);
}

void opengl_draw_with_background_color(float r, float g, float b, float a)
{
    int _e_ = glGetError();
    if (_e_ != 0) {
        printf("GL Error at " __FILE__ ":%d failed with 0x%x\n", __LINE__, _e_);
    }

    int w, h;
    vd_fw_get_size(&w, &h);
    glViewport(0, 0, w, h);
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(Shader_Opengl);
    glBindVertexArray(Vao_Opengl);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float projection[16];
    vd_fw_u_ortho(0.f, (float)w, (float)h, 0.f, -1.f, 1.f, projection);
    glUniformMatrix4fv(glGetUniformLocation(Shader_Opengl, "projection"), 1, GL_FALSE, projection);

    float size = w < h ? (float)w : (float)h;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, Tex_Opengl);
    glUniform4f(glGetUniformLocation(Shader_Opengl, "rect_color"), 1.f,1.f,1.f,1.f);
    glUniform2f(glGetUniformLocation(Shader_Opengl, "rect_size"), size, size);
    glUniform2f(glGetUniformLocation(Shader_Opengl, "rect_off"), ((float)w) * 0.5f - size * 0.5f, ((float)h) * 0.5f - size * 0.5f);
    glUniform1i(glGetUniformLocation(Shader_Opengl, "rect_texture"), 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

GraphicsBackendImpl OpenGL_Impl = {
    GRAPHICS_BACKEND_OPENGL,
    opengl_init,
    opengl_kill,
    opengl_resize,
    opengl_draw_with_background_color,
};

#ifdef _WIN32
#include <d3d11.h>
#include <dxgi1_3.h>
#include <wrl.h>
#include <assert.h>
#include <comdef.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
using namespace DirectX;

template <typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#define DX_VERTEX_SOURCE \
    "cbuffer RectData : register(b0)                                                                               \n" \
    "{                                                                                                             \n" \
    "    float2 rect_off;                                                                                          \n" \
    "    float2 rect_size;                                                                                         \n" \
    "    float4x4 projection;                                                                                      \n" \
    "};                                                                                                            \n" \
    "                                                                                                              \n" \
    "struct VSInput                                                                                                \n" \
    "{                                                                                                             \n" \
    "    float2 aPos : POSITION;                                                                                   \n" \
    "    float2 aTexCoord : TEXCOORD0;                                                                             \n" \
    "};                                                                                                            \n" \
    "                                                                                                              \n" \
    "struct VSOutput                                                                                               \n" \
    "{                                                                                                             \n" \
    "    float4 pos  : SV_POSITION;                                                                                \n" \
    "    float2 rect_uv : TEXCOORD0;                                                                               \n" \
    "};                                                                                                            \n" \
    "                                                                                                              \n" \
    "VSOutput main(VSInput input)                                                                                  \n" \
    "{                                                                                                             \n" \
    "    VSOutput output;                                                                                          \n" \
    "    output.rect_uv = input.aTexCoord;                                                                         \n" \
    "                                                                                                              \n" \
    "    float2 worldPos = input.aPos * rect_size + rect_off;                                                      \n" \
    "    output.pos = mul(projection, float4(worldPos, 0.0f, 1.0f));                                               \n" \
    "    output.rect_uv = input.aTexCoord;                                                                         \n" \
    "                                                                                                              \n" \
    "    return output;                                                                                            \n" \
    "}                                                                                                             \n" \

#define DX_PIXEL_SOURCE \
    "cbuffer RectColor : register(b0)                                                                              \n" \
    "{                                                                                                             \n" \
    "    float4 rect_color;                                                                                        \n" \
    "};                                                                                                            \n" \
    "                                                                                                              \n" \
    "Texture2D rect_texture : register(t0);                                                                        \n" \
    "SamplerState rect_sampler : register(s0);                                                                     \n" \
    "                                                                                                              \n" \
    "float gradientNoise(float2 uv)                                                                                \n" \
    "{                                                                                                             \n" \
    "   return frac(52.9829189 * frac(dot(uv, float2(0.06711056, 0.00583715))));                                   \n" \
    "}                                                                                                             \n" \
    "                                                                                                              \n" \
    "struct PSInput                                                                                                \n" \
    "{                                                                                                             \n" \
    "    float4 pos : SV_POSITION;                                                                                 \n" \
    "    float2 rect_uv : TEXCOORD0;                                                                               \n" \
    "};                                                                                                            \n" \
    "                                                                                                              \n" \
    "float4 main(PSInput input) : SV_Target                                                                        \n" \
    "{                                                                                                             \n" \
    "    float4 tex_color = rect_texture.Sample(rect_sampler, input.rect_uv);                                      \n" \
    "    float4 color = rect_color * tex_color;                                                                    \n" \
    "    color.rgb *= color.a;                                                                                     \n" \
    "    float2 pixel_coord = input.pos.xy;                                                                        \n" \
    "    color.rgb += (1.0 / 128.0) * gradientNoise(pixel_coord) - (0.5 / 128.0);                                  \n" \
    "    return color;                                                                                             \n" \
    "}                                                                                                             \n" \

static IDXGIFactory2            *DXGI_Factory;
static IDXGISwapChain1          *DXGI_Swapchain;
static ID3D11Device             *D_Device;
static ID3D11DeviceContext      *D_Device_Context;
static ID3D11RenderTargetView   *Render_Target_View;
static ID3D11VertexShader       *Vertex_Shader;
static ID3D11PixelShader        *Pixel_Shader;
static ID3D11Texture2D          *Tex_Directx;
static ID3D11InputLayout        *Input_Layout;
static ID3D11Buffer             *Vertex_Buffer;
static ID3D11Buffer             *Constant_Buffer;
static ID3D11Buffer             *Constant_Buffer_Ps;
static ID3D11SamplerState       *D_Sampler;
static ID3D11ShaderResourceView *Tex_Resource_View;
static ID3D11RasterizerState    *Rasterizer_State;
static ID3D11BlendState         *Blend_State;

typedef struct {
    float rect_off[2];
    float rect_size[2];
    XMFLOAT4X4 projection;   // matches HLSL float4x4
} DirectxRectDataConstants;

typedef struct {
    float rect_color[4];
} DirectxRectPixelDataConstants;

#define CHECK_HRESULT(expr) do { HRESULT _h = (expr); if (_h != 0) { printf("Invocation: %s:%d Failed: %d\n", __FILE__, __LINE__, _h); DebugBreak(); } } while(0)

void d3d11_init(void *internal_window_handle)
{
    HWND hwnd = *((HWND*)internal_window_handle);

    CHECK_HRESULT(CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&DXGI_Factory));

    D3D_FEATURE_LEVEL device_feature_level = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
    CHECK_HRESULT(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
                                    nullptr,
                                    D3D11_CREATE_DEVICE_DEBUG,
                                    &device_feature_level,
                                    1,
                                    D3D11_SDK_VERSION,
                                    &D_Device,
                                    nullptr,
                                    &D_Device_Context));

    int width, height;
    vd_fw_get_size(&width, &height);

    DXGI_SWAP_CHAIN_DESC1 swapchain_descriptor = {};
    swapchain_descriptor.Width = width;
    swapchain_descriptor.Height = height;
    swapchain_descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapchain_descriptor.SampleDesc.Count = 1;
    swapchain_descriptor.SampleDesc.Quality = 0;
    swapchain_descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_descriptor.BufferCount = 2;
    swapchain_descriptor.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapchain_descriptor.Scaling = DXGI_SCALING::DXGI_SCALING_STRETCH;
    swapchain_descriptor.Flags = {};

    CHECK_HRESULT(DXGI_Factory->CreateSwapChainForHwnd(
        D_Device,
        hwnd,
        &swapchain_descriptor,
        NULL,
        NULL,
        &DXGI_Swapchain));

    ComPtr<ID3D11Texture2D> backbuffer = nullptr;
    CHECK_HRESULT(DXGI_Swapchain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)));

    CHECK_HRESULT(D_Device->CreateRenderTargetView(backbuffer.Get(), nullptr, &Render_Target_View));

    ID3DBlob *vs_blob, *vs_errors;
    HRESULT hr = D3DCompile(DX_VERTEX_SOURCE, sizeof(DX_VERTEX_SOURCE) - 1, "inline.vs",
                            NULL, NULL,
                            "main",
                            "vs_5_0",
                            0, 0,
                            &vs_blob, &vs_errors);
    if (hr != 0) {
        printf("Failed to compile vertex shader: %.*s\n", (int)vs_errors->GetBufferSize(), (char*)vs_errors->GetBufferPointer());
        return;
    }

    CHECK_HRESULT(D_Device->CreateVertexShader(vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(),
                  NULL, &Vertex_Shader));

    ID3DBlob *ps_blob, *ps_errors;
    hr = D3DCompile(DX_PIXEL_SOURCE, sizeof(DX_PIXEL_SOURCE) - 1, "inline.ps",
                    NULL, NULL,
                    "main",
                    "ps_5_0",
                    0, 0,
                    &ps_blob, &ps_errors);
    if (hr != 0) {
        printf("Failed to compile pixel shader: %.*s\n", (int)ps_errors->GetBufferSize(), (char*)ps_errors->GetBufferPointer());
        return;
    }

    CHECK_HRESULT(D_Device->CreatePixelShader(ps_blob->GetBufferPointer(), ps_blob->GetBufferSize(),
                  NULL, &Pixel_Shader));

    D3D11_TEXTURE2D_DESC desc = {0};
    desc.Width = all.directx_logo.w;
    desc.Height = all.directx_logo.h;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

    D3D11_SUBRESOURCE_DATA subresource_data = {0};
    subresource_data.pSysMem = all.directx_logo.data;
    subresource_data.SysMemPitch = all.directx_logo.w * sizeof(unsigned int);
    CHECK_HRESULT(D_Device->CreateTexture2D(&desc, &subresource_data, &Tex_Directx));

    D3D11_SHADER_RESOURCE_VIEW_DESC resource_view_desc = {0};
    resource_view_desc.Format = desc.Format;
    resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    resource_view_desc.Texture2D.MipLevels = (UINT)-1;
    CHECK_HRESULT(D_Device->CreateShaderResourceView(Tex_Directx, &resource_view_desc, &Tex_Resource_View));

    D3D11_INPUT_ELEMENT_DESC ia_elements[2] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0,               D3D11_INPUT_PER_VERTEX_DATA, 0},
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(float)*2, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    CHECK_HRESULT(D_Device->CreateInputLayout(ia_elements, 2,
                                              vs_blob->GetBufferPointer(), vs_blob->GetBufferSize(),
                                              &Input_Layout));

    float rect_vertices[] = {
         0.0f,  0.0f, 0.f, 0.f,
        +1.0f,  0.0f, 1.f, 0.f,
         0.0f, +1.0f, 0.f, 1.f,
        +1.0f, +1.0f, 1.f, 1.f,
    };
    {
        D3D11_BUFFER_DESC buffer_desc = {0};
        buffer_desc.ByteWidth = sizeof(rect_vertices);
        buffer_desc.Usage = D3D11_USAGE_DEFAULT;
        buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        D3D11_SUBRESOURCE_DATA buffer_subresource_data = {0};
        buffer_subresource_data.pSysMem = rect_vertices;
        CHECK_HRESULT(D_Device->CreateBuffer(&buffer_desc, &buffer_subresource_data, &Vertex_Buffer));
    }

    {
        D3D11_BUFFER_DESC buffer_desc = {0};
        buffer_desc.ByteWidth = sizeof(DirectxRectDataConstants);
        buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        CHECK_HRESULT(D_Device->CreateBuffer(&buffer_desc, NULL, &Constant_Buffer));
    }

    {
        D3D11_BUFFER_DESC buffer_desc = {0};
        buffer_desc.ByteWidth = sizeof(DirectxRectPixelDataConstants);
        buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
        buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        CHECK_HRESULT(D_Device->CreateBuffer(&buffer_desc, NULL, &Constant_Buffer_Ps));
    }

    D3D11_SAMPLER_DESC sampler_desc = {0};
    sampler_desc.Filter = D3D11_FILTER_MIN_MAG_POINT_MIP_LINEAR;
    sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    CHECK_HRESULT(D_Device->CreateSamplerState(&sampler_desc, &D_Sampler));

    D3D11_RASTERIZER_DESC rasterizer_desc = {0};
    rasterizer_desc.FillMode = D3D11_FILL_SOLID;
    rasterizer_desc.CullMode = D3D11_CULL_NONE;
    rasterizer_desc.FrontCounterClockwise = TRUE;
    CHECK_HRESULT(D_Device->CreateRasterizerState(&rasterizer_desc, &Rasterizer_State));

    D3D11_BLEND_DESC blend_desc = {0};
    blend_desc.RenderTarget[0].BlendEnable = TRUE;
    blend_desc.RenderTarget[0].SrcBlend  = D3D11_BLEND_SRC_ALPHA;
    blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
    blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    CHECK_HRESULT(D_Device->CreateBlendState(&blend_desc, &Blend_State));
}

void d3d11_kill(void)
{
    DXGI_Swapchain->Release();          DXGI_Swapchain = NULL;
    Render_Target_View->Release();      Render_Target_View = NULL;
    D_Device_Context->ClearState();
    D_Device_Context->Flush();
    D_Device_Context->Release();        D_Device_Context = NULL;
    D_Device->Release();                D_Device = NULL;
    DXGI_Factory->Release();            DXGI_Factory = NULL;
}

void d3d11_resize(int w, int h)
{
    D_Device_Context->OMSetRenderTargets(0, 0, 0);
    Render_Target_View->Release();

    CHECK_HRESULT(DXGI_Swapchain->ResizeBuffers(0,w,h,DXGI_FORMAT_B8G8R8A8_UNORM, 0));

    ComPtr<ID3D11Texture2D> backbuffer = nullptr;
    CHECK_HRESULT(DXGI_Swapchain->GetBuffer(0, IID_PPV_ARGS(&backbuffer)));

    CHECK_HRESULT(D_Device->CreateRenderTargetView(backbuffer.Get(), nullptr, &Render_Target_View));
}

static void d3d11_draw_with_background_color(float r, float g, float b, float a)
{
    int w, h;
    vd_fw_get_size(&w, &h);
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)(w);
    viewport.Height = (float)(h);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    float clearColor[] = { r,g,b,a };

    D_Device_Context->OMSetRenderTargets(
        1,
        &Render_Target_View,
        nullptr);
    float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
    UINT sample_mask = 0xffffffff;
    D_Device_Context->OMSetBlendState(Blend_State, blend_factor, sample_mask);
    D_Device_Context->RSSetViewports(
        1,
        &viewport);
    D_Device_Context->RSSetState(Rasterizer_State);

    D_Device_Context->ClearRenderTargetView(
        Render_Target_View,
        clearColor);

    DirectxRectDataConstants rect_data_constants;
    float size = w < h ? (float)w : (float)h;
    rect_data_constants.rect_off[0] = ((float)w) * 0.5f - size * 0.5f;
    rect_data_constants.rect_off[1] = ((float)h) * 0.5f - size * 0.5f;
    rect_data_constants.rect_size[0] = size;
    rect_data_constants.rect_size[1] = size;
    XMMATRIX proj = XMMatrixOrthographicOffCenterLH(0.f, (float)w, (float)h, (float)0.f, 0.1f, 100.f);
    XMStoreFloat4x4(&rect_data_constants.projection, proj);

    DirectxRectPixelDataConstants rect_pixel_data_constants;
    rect_pixel_data_constants.rect_color[0] = 1.f;
    rect_pixel_data_constants.rect_color[1] = 1.f;
    rect_pixel_data_constants.rect_color[2] = 1.f;
    rect_pixel_data_constants.rect_color[3] = 1.f;

    D3D11_MAPPED_SUBRESOURCE mapped_subresource;
    CHECK_HRESULT(D_Device_Context->Map(Constant_Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource));
    *((DirectxRectDataConstants*)mapped_subresource.pData) = rect_data_constants;
    D_Device_Context->Unmap(Constant_Buffer, 0);


    CHECK_HRESULT(D_Device_Context->Map(Constant_Buffer_Ps, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource));
    *((DirectxRectPixelDataConstants*)mapped_subresource.pData) = rect_pixel_data_constants;
    D_Device_Context->Unmap(Constant_Buffer_Ps, 0);

    D_Device_Context->VSSetConstantBuffers(0, 1, &Constant_Buffer);

    D_Device_Context->IASetInputLayout(Input_Layout);
    D_Device_Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    UINT stride = sizeof(float)*4;
    UINT offset = 0;

    D_Device_Context->IASetVertexBuffers(0, 1, &Vertex_Buffer, &stride, &offset);
    D_Device_Context->VSSetShader(Vertex_Shader, NULL, 0);

    D_Device_Context->PSSetConstantBuffers(0, 1, &Constant_Buffer_Ps);
    D_Device_Context->PSSetShader(Pixel_Shader, NULL, 0);
    D_Device_Context->PSSetShaderResources(0, 1, &Tex_Resource_View);
    D_Device_Context->PSSetSamplers(0, 1, &D_Sampler);

    D_Device_Context->Draw(4, 0);

    DXGI_Swapchain->Present(1, 0);
}

GraphicsBackendImpl D3D11_Impl = {
    GRAPHICS_BACKEND_DIRECT3D11,
    d3d11_init,
    d3d11_kill,
    d3d11_resize,
    d3d11_draw_with_background_color,
};
#endif

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>

#define VK_CHECK(expr) do { \
        VkResult __result__ = (expr); \
        if (__result__ != VK_SUCCESS) { \
            printf("Vulkan Assertion Failed: '" #expr "' at %s %d\t%s\n", __FILE__, __LINE__, string_VkResult(__result__));\
            abort(); \
        }\
    } while(0)

typedef struct {
    VkCommandBuffer                             cmd_buffer;
    VkSemaphore                                 sem_present_complete;
    VkFence                                     fnc_render_complete;
} VkFrameData;

static unsigned char Vk_Vert_Spv[1296] = {
0x03,0x02,0x23,0x07,0x00,0x03,0x01,0x00,0x0b,0x00,0x08,0x00,0x31,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x02,0x00,0x01,0x00,0x00,0x00,0x0b,0x00,0x06,0x00,
0x01,0x00,0x00,0x00,0x47,0x4c,0x53,0x4c,0x2e,0x73,0x74,0x64,0x2e,0x34,0x35,0x30,0x00,0x00,0x00,0x00,0x0e,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x0f,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x6d,0x61,0x69,0x6e,0x00,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x1c,0x00,0x00,0x00,0x29,0x00,0x00,0x00,
0x03,0x00,0x03,0x00,0x02,0x00,0x00,0x00,0xc2,0x01,0x00,0x00,0x05,0x00,0x04,0x00,0x04,0x00,0x00,0x00,0x6d,0x61,0x69,0x6e,0x00,0x00,0x00,0x00,0x05,0x00,0x05,0x00,
0x0c,0x00,0x00,0x00,0x70,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x73,0x00,0x00,0x00,0x05,0x00,0x06,0x00,0x16,0x00,0x00,0x00,0x67,0x6c,0x5f,0x50,0x65,0x72,0x56,0x65,
0x72,0x74,0x65,0x78,0x00,0x00,0x00,0x00,0x06,0x00,0x06,0x00,0x16,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x67,0x6c,0x5f,0x50,0x6f,0x73,0x69,0x74,0x69,0x6f,0x6e,0x00,
0x06,0x00,0x07,0x00,0x16,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x67,0x6c,0x5f,0x50,0x6f,0x69,0x6e,0x74,0x53,0x69,0x7a,0x65,0x00,0x00,0x00,0x00,0x06,0x00,0x07,0x00,
0x16,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x67,0x6c,0x5f,0x43,0x6c,0x69,0x70,0x44,0x69,0x73,0x74,0x61,0x6e,0x63,0x65,0x00,0x06,0x00,0x07,0x00,0x16,0x00,0x00,0x00,
0x03,0x00,0x00,0x00,0x67,0x6c,0x5f,0x43,0x75,0x6c,0x6c,0x44,0x69,0x73,0x74,0x61,0x6e,0x63,0x65,0x00,0x05,0x00,0x03,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x05,0x00,0x06,0x00,0x1c,0x00,0x00,0x00,0x67,0x6c,0x5f,0x56,0x65,0x72,0x74,0x65,0x78,0x49,0x6e,0x64,0x65,0x78,0x00,0x00,0x05,0x00,0x03,0x00,0x29,0x00,0x00,0x00,
0x75,0x76,0x00,0x00,0x47,0x00,0x03,0x00,0x16,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x48,0x00,0x05,0x00,0x16,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0b,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x48,0x00,0x05,0x00,0x16,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0b,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x48,0x00,0x05,0x00,0x16,0x00,0x00,0x00,
0x02,0x00,0x00,0x00,0x0b,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x48,0x00,0x05,0x00,0x16,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x0b,0x00,0x00,0x00,0x04,0x00,0x00,0x00,
0x47,0x00,0x04,0x00,0x1c,0x00,0x00,0x00,0x0b,0x00,0x00,0x00,0x2a,0x00,0x00,0x00,0x47,0x00,0x04,0x00,0x29,0x00,0x00,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x13,0x00,0x02,0x00,0x02,0x00,0x00,0x00,0x21,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x16,0x00,0x03,0x00,0x06,0x00,0x00,0x00,0x20,0x00,0x00,0x00,
0x17,0x00,0x04,0x00,0x07,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x15,0x00,0x04,0x00,0x08,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x2b,0x00,0x04,0x00,0x08,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x1c,0x00,0x04,0x00,0x0a,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x09,0x00,0x00,0x00,
0x20,0x00,0x04,0x00,0x0b,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,0x0b,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x06,0x00,0x00,0x00,
0x2b,0x00,0x04,0x00,0x06,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x00,0x00,0x80,0xbf,0x2c,0x00,0x05,0x00,0x07,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,
0x0d,0x00,0x00,0x00,0x2b,0x00,0x04,0x00,0x06,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0x00,0x00,0x40,0x40,0x2c,0x00,0x05,0x00,0x07,0x00,0x00,0x00,0x10,0x00,0x00,0x00,
0x0f,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x2c,0x00,0x05,0x00,0x07,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0x2c,0x00,0x06,0x00,
0x0a,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x17,0x00,0x04,0x00,0x13,0x00,0x00,0x00,0x06,0x00,0x00,0x00,
0x04,0x00,0x00,0x00,0x2b,0x00,0x04,0x00,0x08,0x00,0x00,0x00,0x14,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x1c,0x00,0x04,0x00,0x15,0x00,0x00,0x00,0x06,0x00,0x00,0x00,
0x14,0x00,0x00,0x00,0x1e,0x00,0x06,0x00,0x16,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x15,0x00,0x00,0x00,0x15,0x00,0x00,0x00,0x20,0x00,0x04,0x00,
0x17,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x16,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,0x17,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x15,0x00,0x04,0x00,
0x19,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x2b,0x00,0x04,0x00,0x19,0x00,0x00,0x00,0x1a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x04,0x00,
0x1b,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x19,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,0x1b,0x00,0x00,0x00,0x1c,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x20,0x00,0x04,0x00,
0x1e,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x2b,0x00,0x04,0x00,0x06,0x00,0x00,0x00,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2b,0x00,0x04,0x00,
0x06,0x00,0x00,0x00,0x22,0x00,0x00,0x00,0x00,0x00,0x80,0x3f,0x20,0x00,0x04,0x00,0x26,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x20,0x00,0x04,0x00,
0x28,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,0x28,0x00,0x00,0x00,0x29,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x2b,0x00,0x04,0x00,
0x06,0x00,0x00,0x00,0x2f,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x36,0x00,0x05,0x00,0x02,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,
0xf8,0x00,0x02,0x00,0x05,0x00,0x00,0x00,0x3e,0x00,0x03,0x00,0x0c,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x3d,0x00,0x04,0x00,0x19,0x00,0x00,0x00,0x1d,0x00,0x00,0x00,
0x1c,0x00,0x00,0x00,0x41,0x00,0x05,0x00,0x1e,0x00,0x00,0x00,0x1f,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x1d,0x00,0x00,0x00,0x3d,0x00,0x04,0x00,0x07,0x00,0x00,0x00,
0x20,0x00,0x00,0x00,0x1f,0x00,0x00,0x00,0x51,0x00,0x05,0x00,0x06,0x00,0x00,0x00,0x23,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x51,0x00,0x05,0x00,
0x06,0x00,0x00,0x00,0x24,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x50,0x00,0x07,0x00,0x13,0x00,0x00,0x00,0x25,0x00,0x00,0x00,0x23,0x00,0x00,0x00,
0x24,0x00,0x00,0x00,0x21,0x00,0x00,0x00,0x22,0x00,0x00,0x00,0x41,0x00,0x05,0x00,0x26,0x00,0x00,0x00,0x27,0x00,0x00,0x00,0x18,0x00,0x00,0x00,0x1a,0x00,0x00,0x00,
0x3e,0x00,0x03,0x00,0x27,0x00,0x00,0x00,0x25,0x00,0x00,0x00,0x3d,0x00,0x04,0x00,0x19,0x00,0x00,0x00,0x2a,0x00,0x00,0x00,0x1c,0x00,0x00,0x00,0x41,0x00,0x05,0x00,
0x1e,0x00,0x00,0x00,0x2b,0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x2a,0x00,0x00,0x00,0x3d,0x00,0x04,0x00,0x07,0x00,0x00,0x00,0x2c,0x00,0x00,0x00,0x2b,0x00,0x00,0x00,
0x50,0x00,0x05,0x00,0x07,0x00,0x00,0x00,0x2d,0x00,0x00,0x00,0x22,0x00,0x00,0x00,0x22,0x00,0x00,0x00,0x81,0x00,0x05,0x00,0x07,0x00,0x00,0x00,0x2e,0x00,0x00,0x00,
0x2c,0x00,0x00,0x00,0x2d,0x00,0x00,0x00,0x8e,0x00,0x05,0x00,0x07,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x2e,0x00,0x00,0x00,0x2f,0x00,0x00,0x00,0x3e,0x00,0x03,0x00,
0x29,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0xfd,0x00,0x01,0x00,0x38,0x00,0x01,0x00
};

static unsigned char Vk_Frag_Spv[556] = {
0x03,0x02,0x23,0x07,0x00,0x03,0x01,0x00,0x0b,0x00,0x08,0x00,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x11,0x00,0x02,0x00,0x01,0x00,0x00,0x00,0x0b,0x00,0x06,0x00,
0x01,0x00,0x00,0x00,0x47,0x4c,0x53,0x4c,0x2e,0x73,0x74,0x64,0x2e,0x34,0x35,0x30,0x00,0x00,0x00,0x00,0x0e,0x00,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,
0x0f,0x00,0x07,0x00,0x04,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x6d,0x61,0x69,0x6e,0x00,0x00,0x00,0x00,0x09,0x00,0x00,0x00,0x11,0x00,0x00,0x00,0x10,0x00,0x03,0x00,
0x04,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x03,0x00,0x03,0x00,0x02,0x00,0x00,0x00,0xc2,0x01,0x00,0x00,0x05,0x00,0x04,0x00,0x04,0x00,0x00,0x00,0x6d,0x61,0x69,0x6e,
0x00,0x00,0x00,0x00,0x05,0x00,0x04,0x00,0x09,0x00,0x00,0x00,0x63,0x6f,0x6c,0x6f,0x72,0x00,0x00,0x00,0x05,0x00,0x06,0x00,0x0d,0x00,0x00,0x00,0x69,0x6d,0x61,0x67,
0x65,0x54,0x65,0x78,0x74,0x75,0x72,0x65,0x00,0x00,0x00,0x00,0x05,0x00,0x03,0x00,0x11,0x00,0x00,0x00,0x75,0x76,0x00,0x00,0x47,0x00,0x04,0x00,0x09,0x00,0x00,0x00,
0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x47,0x00,0x04,0x00,0x0d,0x00,0x00,0x00,0x21,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x47,0x00,0x04,0x00,0x0d,0x00,0x00,0x00,
0x22,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x47,0x00,0x04,0x00,0x11,0x00,0x00,0x00,0x1e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x13,0x00,0x02,0x00,0x02,0x00,0x00,0x00,
0x21,0x00,0x03,0x00,0x03,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x16,0x00,0x03,0x00,0x06,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x17,0x00,0x04,0x00,0x07,0x00,0x00,0x00,
0x06,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x20,0x00,0x04,0x00,0x08,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,0x08,0x00,0x00,0x00,
0x09,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0x19,0x00,0x09,0x00,0x0a,0x00,0x00,0x00,0x06,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1b,0x00,0x03,0x00,0x0b,0x00,0x00,0x00,0x0a,0x00,0x00,0x00,0x20,0x00,0x04,0x00,0x0c,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x0b,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,0x0c,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x17,0x00,0x04,0x00,0x0f,0x00,0x00,0x00,
0x06,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x20,0x00,0x04,0x00,0x10,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x0f,0x00,0x00,0x00,0x3b,0x00,0x04,0x00,0x10,0x00,0x00,0x00,
0x11,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x36,0x00,0x05,0x00,0x02,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x00,0x00,0x00,0xf8,0x00,0x02,0x00,
0x05,0x00,0x00,0x00,0x3d,0x00,0x04,0x00,0x0b,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,0x0d,0x00,0x00,0x00,0x3d,0x00,0x04,0x00,0x0f,0x00,0x00,0x00,0x12,0x00,0x00,0x00,
0x11,0x00,0x00,0x00,0x57,0x00,0x05,0x00,0x07,0x00,0x00,0x00,0x13,0x00,0x00,0x00,0x0e,0x00,0x00,0x00,0x12,0x00,0x00,0x00,0x3e,0x00,0x03,0x00,0x09,0x00,0x00,0x00,
0x13,0x00,0x00,0x00,0xfd,0x00,0x01,0x00,0x38,0x00,0x01,0x00
};

static VkInstance                                  Vk_Instance;
static VkPhysicalDevice                            Vk_Physical_Device;
static VkDevice                                    Vk_Device;
static VkCommandPool                               Vk_Cmd_Pool;
static PFN_vkCreateDebugUtilsMessengerEXT          Vk_Debug_utils_create_messenger;
static PFN_vkDestroyDebugUtilsMessengerEXT         Vk_Debug_utils_destroy_messenger;
static VkDebugUtilsMessengerEXT                    Vk_Debug_utils_messenger;
static VkSurfaceKHR                                Vk_Surface;
static VkPresentModeKHR                            Vk_Present_Mode;
static VkSurfaceFormatKHR                          Vk_Surface_Format;
static uint32_t                                    Vk_Swapchain_Image_Count;
static uint32_t                                    Vk_Min_Swapchain_Image_Count;
static VkQueue                                     Vk_Queue;
static VkSwapchainKHR                              Vk_Swapchain;
static VkImage                                     Vk_Swapchain_Images[8];
static VkImageView                                 Vk_Swapchain_Image_Views[8];
static VkSemaphore                                 Vk_Sems_Render_Complete[8];
static VkFrameData                                 Vk_Frames[2];
static int                                         Vk_Num_Frames = 2;
static VkPipelineLayout                            Vk_Pipeline_Layout;
static VkShaderModule                              Vk_Vertex_Shader, Vk_Fragment_Shader;
static VkPipeline                                  Vk_Graphics_Pipeline;
static VkExtent2D                                  Vk_Window_Size;
static int                                         Vk_Curr_Frame = 0;
static VkDescriptorSetLayout                       Vk_Image_Set_Layout;

static VkExtent2D create_swapchain_and_image_views();
static VkBool32 debug_utils_messenger_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                               VkDebugUtilsMessageTypeFlagsEXT message_types,
                                               const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                               void *usr_data);

static void swapchain_image_barrier(VkCommandBuffer cmd, uint32_t image_index,
                                    uint32_t src_queue_family, uint32_t dst_queue_family,
                                    VkImageLayout old_layout, VkImageLayout new_layout,
                                    VkAccessFlags2 src_access, VkAccessFlags2 dst_access,
                                    VkPipelineStageFlags2 src_stage, VkPipelineStageFlags2 dst_stage);


void vulkan_init(void *internal_window_handle)
{
    VkApplicationInfo app_info;
    app_info.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pNext              = 0;
    app_info.pApplicationName   = "fw_all";
    app_info.applicationVersion = VK_MAKE_VERSION(1,0,0);
    app_info.pEngineName        = "No Engine";
    app_info.engineVersion      = VK_MAKE_VERSION(1,0,0);
    app_info.apiVersion         = VK_API_VERSION_1_4;

    VkInstanceCreateFlags instance_create_flags = 0;
    if (vd_fw_get_platform() == VD_FW_PLATFORM_MACOS) {
        instance_create_flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    }

    const char *required_instance_extensions[] = {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
        VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
        VK_KHR_SURFACE_MAINTENANCE_1_EXTENSION_NAME,
        VK_KHR_GET_SURFACE_CAPABILITIES_2_EXTENSION_NAME,
    };
    int num_required_instance_extensions = sizeof(required_instance_extensions) / sizeof(required_instance_extensions[0]);

    const char *required_instance_layers[] = {
        "VK_LAYER_KHRONOS_validation",
    };

    int num_required_instance_layers = sizeof(required_instance_layers) / sizeof(required_instance_layers[0]);

    const char *required_device_extensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME,
        // VK_KHR_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME,
    };
    int num_required_device_extensions = sizeof(required_device_extensions) / sizeof(required_device_extensions[0]);

    int num_all_instance_extensions = 0;
    const char *all_instance_extensions[12];

    int num_wsi_instance_extensions;
    const char **wsi_instance_extensions = vd_fw_vk_wsi_instance_extensions(&num_wsi_instance_extensions);

    // Copy WSI Extensions
    for (int i = 0; i < num_wsi_instance_extensions; ++i) {
        all_instance_extensions[num_all_instance_extensions++] = wsi_instance_extensions[i];
    }

    for (int i = 0; i < num_required_instance_extensions; ++i) {
        all_instance_extensions[num_all_instance_extensions++] = required_instance_extensions[i];
    }

    VkInstanceCreateInfo instance_create_info;
    instance_create_info.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_create_info.pNext                   = 0;
    instance_create_info.flags                   = instance_create_flags;
    instance_create_info.pApplicationInfo        = &app_info;
    instance_create_info.enabledLayerCount       = num_required_instance_layers;
    instance_create_info.ppEnabledLayerNames     = required_instance_layers;
    instance_create_info.enabledExtensionCount   = num_all_instance_extensions;
    instance_create_info.ppEnabledExtensionNames = all_instance_extensions;
    VK_CHECK(vkCreateInstance(&instance_create_info, NULL, &Vk_Instance));


    Vk_Debug_utils_create_messenger = (PFN_vkCreateDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(Vk_Instance, "vkCreateDebugUtilsMessengerEXT");

    Vk_Debug_utils_destroy_messenger = (PFN_vkDestroyDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(Vk_Instance, "vkDestroyDebugUtilsMessengerEXT");

    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info;
    debug_messenger_create_info.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    debug_messenger_create_info.pNext           = 0;
    debug_messenger_create_info.flags           = 0;
    debug_messenger_create_info.messageSeverity = 0
                                                  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
                                                  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
                                                  | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT
                                                  ;
    debug_messenger_create_info.messageType     = 0
                                                  | VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
                                                  | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
                                                  | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT
                                                  // | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT
                                                  ;
    debug_messenger_create_info.pfnUserCallback = debug_utils_messenger_callback;
    debug_messenger_create_info.pUserData       = 0;

    VK_CHECK(Vk_Debug_utils_create_messenger(Vk_Instance, &debug_messenger_create_info, NULL,
                                             &Vk_Debug_utils_messenger));

    vd_fw_vk_set_get_instance_proc_addr((VdFwVkGetInstanceProcAddrProc)vkGetInstanceProcAddr);
    VK_CHECK((VkResult)vd_fw_vk_wsi_surface_create((void*)Vk_Instance, NULL, &Vk_Surface));

    VkPhysicalDevice physical_devices[16];
    uint32_t num_physical_devices = 16;
    VK_CHECK(vkEnumeratePhysicalDevices(Vk_Instance, &num_physical_devices, physical_devices));

    uint32_t api_version = VK_MAKE_VERSION(1, 3, 0);

    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    for (uint32_t i = 0; i < num_physical_devices; ++i) {
        VkPhysicalDeviceProperties q_properties;
        vkGetPhysicalDeviceProperties(physical_devices[i], &q_properties);

        // Ensure the device supports the API version we want to use
        if (q_properties.apiVersion < api_version) {
            continue;
        }

        // Prefer discrete GPUs over everything else
        if ((q_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) || (q_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)) {
            physical_device = physical_devices[i];

            printf("VK: %s\n", q_properties.deviceName);
            break;
        }
    }

    VkSurfaceCapabilitiesKHR surface_caps;
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physical_device, Vk_Surface, &surface_caps));
    uint32_t q_swapchain_image_count = surface_caps.minImageCount + 1;
    if (q_swapchain_image_count > surface_caps.maxImageCount) {
        q_swapchain_image_count = surface_caps.maxImageCount;
    }

    VkSurfaceFormatKHR surface_format = {
        VK_FORMAT_B8G8R8A8_SRGB,
        VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
    };

    Vk_Surface_Format = surface_format;
    Vk_Present_Mode = VK_PRESENT_MODE_FIFO_KHR;
    Vk_Min_Swapchain_Image_Count = q_swapchain_image_count;

    VkDeviceQueueCreateInfo device_queue_create_infos[1];
    float                   device_queue_priorities[1] = { 1.f };
    int                     num_device_queue_create_infos = 1;

    device_queue_create_infos[0].sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    device_queue_create_infos[0].pNext            = 0;
    device_queue_create_infos[0].flags            = 0;
    device_queue_create_infos[0].queueFamilyIndex = 0;
    device_queue_create_infos[0].queueCount       = 1;
    device_queue_create_infos[0].pQueuePriorities = device_queue_priorities;

    VkPhysicalDeviceSwapchainMaintenance1FeaturesKHR physical_device_swapchain_maintenance1_features;
    physical_device_swapchain_maintenance1_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SWAPCHAIN_MAINTENANCE_1_FEATURES_KHR;
    physical_device_swapchain_maintenance1_features.pNext = 0;

    VkPhysicalDeviceExtendedDynamicStateFeaturesEXT ext_dynamic_state_features;
    memset(&ext_dynamic_state_features, 0, sizeof(ext_dynamic_state_features));
    ext_dynamic_state_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT;
    ext_dynamic_state_features.pNext = &physical_device_swapchain_maintenance1_features;

    VkPhysicalDeviceVulkan13Features physical_device_vk13_features;
    memset(&physical_device_vk13_features, 0, sizeof(physical_device_vk13_features));
    physical_device_vk13_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;
    physical_device_vk13_features.pNext = &ext_dynamic_state_features;

    VkPhysicalDeviceVulkan12Features physical_device_vk12_features;
    memset(&physical_device_vk12_features, 0, sizeof(physical_device_vk12_features));
    physical_device_vk12_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
    physical_device_vk12_features.pNext = &physical_device_vk13_features;

    VkPhysicalDeviceVulkan11Features physical_device_vk11_features;
    memset(&physical_device_vk11_features, 0, sizeof(physical_device_vk11_features));
    physical_device_vk11_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
    physical_device_vk11_features.pNext = &physical_device_vk12_features;

    VkPhysicalDeviceFeatures2 physical_device_features2;
    memset(&physical_device_features2, 0, sizeof(physical_device_features2));
    physical_device_features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    physical_device_features2.pNext = &physical_device_vk11_features;

    physical_device_vk11_features.shaderDrawParameters = VK_TRUE;
    physical_device_vk13_features.dynamicRendering = VK_TRUE;
    physical_device_vk13_features.synchronization2 = VK_TRUE;
    ext_dynamic_state_features.extendedDynamicState = VK_TRUE;
    physical_device_swapchain_maintenance1_features.swapchainMaintenance1 = VK_TRUE;

    VkDeviceCreateInfo device_create_info;
    device_create_info.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_create_info.pNext                   = &physical_device_features2;
    device_create_info.flags                   = 0;
    device_create_info.queueCreateInfoCount    = num_device_queue_create_infos;
    device_create_info.pQueueCreateInfos       = device_queue_create_infos;
    device_create_info.enabledLayerCount       = 0; // Deprecated
    device_create_info.ppEnabledLayerNames     = 0; // Deprecated
    device_create_info.enabledExtensionCount   = num_required_device_extensions;
    device_create_info.ppEnabledExtensionNames = required_device_extensions;
    device_create_info.pEnabledFeatures        = 0;

    Vk_Physical_Device = physical_device;
    VK_CHECK(vkCreateDevice(physical_device, &device_create_info, NULL, &Vk_Device));

    vkGetDeviceQueue(Vk_Device, 0, 0, &Vk_Queue);

    create_swapchain_and_image_views();

    VkCommandPoolCreateInfo command_pool_create_info;
    command_pool_create_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    command_pool_create_info.pNext            = 0;
    command_pool_create_info.flags            = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    command_pool_create_info.queueFamilyIndex = 0;
    VK_CHECK(vkCreateCommandPool(Vk_Device, &command_pool_create_info, NULL, &Vk_Cmd_Pool));

    for (uint32_t i = 0; i < Vk_Num_Frames; ++i) {
        VkFrameData *frame = &Vk_Frames[i];
        VkCommandBufferAllocateInfo cmd_buffer_allocate_info;
        cmd_buffer_allocate_info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        cmd_buffer_allocate_info.pNext              = 0;
        cmd_buffer_allocate_info.commandPool        = Vk_Cmd_Pool;
        cmd_buffer_allocate_info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        cmd_buffer_allocate_info.commandBufferCount = 1;
        VK_CHECK(vkAllocateCommandBuffers(Vk_Device, &cmd_buffer_allocate_info, &frame->cmd_buffer));

        VkSemaphoreCreateInfo semaphore_create_info;
        semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphore_create_info.pNext = 0;
        semaphore_create_info.flags = 0;
        VK_CHECK(vkCreateSemaphore(Vk_Device, &semaphore_create_info, NULL, &frame->sem_present_complete));

        VkFenceCreateInfo fence_create_info;
        fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_create_info.pNext = 0;
        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
        VK_CHECK(vkCreateFence(Vk_Device, &fence_create_info, NULL, &frame->fnc_render_complete));
    }

    VkDescriptorSetLayoutBinding image_binding = {0};
    image_binding.binding            = 0;
    image_binding.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    image_binding.descriptorCount    = 1;
    image_binding.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;
    image_binding.pImmutableSamplers = nullptr;

    VkDescriptorSetLayoutCreateInfo descriptor_set_layout_info = {0};
    descriptor_set_layout_info.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    descriptor_set_layout_info.bindingCount = 1;
    descriptor_set_layout_info.pBindings    = &image_binding;
    VK_CHECK(vkCreateDescriptorSetLayout(Vk_Device, &descriptor_set_layout_info, nullptr,
                                         &Vk_Image_Set_Layout));

    VkPipelineLayoutCreateInfo pipeline_layout_create_info;
    pipeline_layout_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.pNext                  = 0;
    pipeline_layout_create_info.flags                  = 0;
    pipeline_layout_create_info.setLayoutCount         = 1;
    pipeline_layout_create_info.pSetLayouts            = &Vk_Image_Set_Layout;
    pipeline_layout_create_info.pushConstantRangeCount = 0;
    pipeline_layout_create_info.pPushConstantRanges    = 0;
    VK_CHECK(vkCreatePipelineLayout(Vk_Device, &pipeline_layout_create_info, NULL, &Vk_Pipeline_Layout));

    {
        VkShaderModuleCreateInfo shader_module_create_info;
        shader_module_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_module_create_info.pNext    = 0;
        shader_module_create_info.flags    = 0;
        shader_module_create_info.codeSize = sizeof(Vk_Vert_Spv);
        shader_module_create_info.pCode    = (uint32_t*)Vk_Vert_Spv;
        VK_CHECK(vkCreateShaderModule(Vk_Device, &shader_module_create_info, NULL, &Vk_Vertex_Shader));
    }

    {
        VkShaderModuleCreateInfo shader_module_create_info;
        shader_module_create_info.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_module_create_info.pNext    = 0;
        shader_module_create_info.flags    = 0;
        shader_module_create_info.codeSize = sizeof(Vk_Frag_Spv);
        shader_module_create_info.pCode    = (uint32_t*)Vk_Frag_Spv;
        VK_CHECK(vkCreateShaderModule(Vk_Device, &shader_module_create_info, NULL, &Vk_Fragment_Shader));
    }

    VkPipelineShaderStageCreateInfo vert_shader_stage_create_info;
    vert_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_stage_create_info.pNext = 0;
    vert_shader_stage_create_info.flags = 0;
    vert_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_stage_create_info.module = Vk_Vertex_Shader;
    vert_shader_stage_create_info.pName = "main";
    vert_shader_stage_create_info.pSpecializationInfo = 0;

    VkPipelineShaderStageCreateInfo frag_shader_stage_create_info;
    frag_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_stage_create_info.pNext = 0;
    frag_shader_stage_create_info.flags = 0;
    frag_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_stage_create_info.module = Vk_Fragment_Shader;
    frag_shader_stage_create_info.pName = "main";
    frag_shader_stage_create_info.pSpecializationInfo = 0;

    VkPipelineShaderStageCreateInfo shader_stages[] = { vert_shader_stage_create_info, frag_shader_stage_create_info };

    VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info;
    vertex_input_state_create_info.sType                           = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_state_create_info.pNext                           = 0;
    vertex_input_state_create_info.flags                           = 0;
    vertex_input_state_create_info.vertexBindingDescriptionCount   = 0;
    vertex_input_state_create_info.pVertexBindingDescriptions      = 0;
    vertex_input_state_create_info.vertexAttributeDescriptionCount = 0;
    vertex_input_state_create_info.pVertexAttributeDescriptions    = 0;

    VkPipelineInputAssemblyStateCreateInfo pipeline_input_assembly_state_create_info;
    pipeline_input_assembly_state_create_info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipeline_input_assembly_state_create_info.pNext                  = 0;
    pipeline_input_assembly_state_create_info.flags                  = 0;
    pipeline_input_assembly_state_create_info.topology               = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pipeline_input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo pipeline_viewport_state_create_info;
    pipeline_viewport_state_create_info.sType         = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipeline_viewport_state_create_info.pNext         = 0;
    pipeline_viewport_state_create_info.flags         = 0;
    pipeline_viewport_state_create_info.viewportCount = 1;
    pipeline_viewport_state_create_info.pViewports    = 0;
    pipeline_viewport_state_create_info.scissorCount  = 1;
    pipeline_viewport_state_create_info.pScissors     = 0;

    VkPipelineRasterizationStateCreateInfo pipeline_rasterization_state_create_info;
    pipeline_rasterization_state_create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    pipeline_rasterization_state_create_info.pNext                   = 0;
    pipeline_rasterization_state_create_info.flags                   = 0;
    pipeline_rasterization_state_create_info.depthClampEnable        = VK_FALSE;
    pipeline_rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
    pipeline_rasterization_state_create_info.polygonMode             = VK_POLYGON_MODE_FILL;
    pipeline_rasterization_state_create_info.cullMode                = VK_CULL_MODE_BACK_BIT;
    pipeline_rasterization_state_create_info.frontFace               = VK_FRONT_FACE_CLOCKWISE;
    pipeline_rasterization_state_create_info.depthBiasEnable         = VK_FALSE;
    pipeline_rasterization_state_create_info.depthBiasConstantFactor = 0.f;
    pipeline_rasterization_state_create_info.depthBiasClamp          = 0.f;
    pipeline_rasterization_state_create_info.depthBiasSlopeFactor    = 1.f;
    pipeline_rasterization_state_create_info.lineWidth               = 1.f;

    VkPipelineMultisampleStateCreateInfo pipeline_multisample_state_create_info;
    pipeline_multisample_state_create_info.sType                 = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipeline_multisample_state_create_info.pNext                 = 0;
    pipeline_multisample_state_create_info.flags                 = 0;
    pipeline_multisample_state_create_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
    pipeline_multisample_state_create_info.sampleShadingEnable   = VK_FALSE;
    pipeline_multisample_state_create_info.minSampleShading      = 0.f;
    pipeline_multisample_state_create_info.pSampleMask           = 0;
    pipeline_multisample_state_create_info.alphaToCoverageEnable = VK_FALSE;
    pipeline_multisample_state_create_info.alphaToOneEnable      = VK_FALSE;

    VkPipelineColorBlendAttachmentState color_blend_attachment_state;
    color_blend_attachment_state.blendEnable         = VK_FALSE;
    color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment_state.colorBlendOp        = VK_BLEND_OP_ADD;
    color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    color_blend_attachment_state.alphaBlendOp        = VK_BLEND_OP_ADD;
    color_blend_attachment_state.colorWriteMask      = 0
                                                       | VK_COLOR_COMPONENT_R_BIT
                                                       | VK_COLOR_COMPONENT_G_BIT 
                                                       | VK_COLOR_COMPONENT_B_BIT 
                                                       | VK_COLOR_COMPONENT_A_BIT 
                                                       ;

    VkPipelineColorBlendStateCreateInfo pipeline_color_blend_state_create_info;
    pipeline_color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    pipeline_color_blend_state_create_info.pNext = 0;
    pipeline_color_blend_state_create_info.flags = 0;
    pipeline_color_blend_state_create_info.logicOpEnable = VK_FALSE;
    pipeline_color_blend_state_create_info.logicOp = VK_LOGIC_OP_COPY;
    pipeline_color_blend_state_create_info.attachmentCount = 1;
    pipeline_color_blend_state_create_info.pAttachments = &color_blend_attachment_state;
    pipeline_color_blend_state_create_info.blendConstants[0] = 0.f;
    pipeline_color_blend_state_create_info.blendConstants[1] = 0.f;
    pipeline_color_blend_state_create_info.blendConstants[2] = 0.f;
    pipeline_color_blend_state_create_info.blendConstants[3] = 0.f;

    VkDynamicState dynamic_states[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
    };

    VkPipelineDynamicStateCreateInfo pipeline_dynamic_state_create_info;
    pipeline_dynamic_state_create_info.sType             = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    pipeline_dynamic_state_create_info.pNext             = 0;
    pipeline_dynamic_state_create_info.flags             = 0;
    pipeline_dynamic_state_create_info.dynamicStateCount = sizeof(dynamic_states) / sizeof(dynamic_states[0]);
    pipeline_dynamic_state_create_info.pDynamicStates    = dynamic_states;

    VkPipelineRenderingCreateInfo pipeline_rendering_create_info;
    pipeline_rendering_create_info.sType                   = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO;
    pipeline_rendering_create_info.pNext                   = 0;
    pipeline_rendering_create_info.viewMask                = 0;
    pipeline_rendering_create_info.colorAttachmentCount    = 1;
    pipeline_rendering_create_info.pColorAttachmentFormats = &Vk_Surface_Format.format;
    pipeline_rendering_create_info.depthAttachmentFormat   = VK_FORMAT_UNDEFINED;
    pipeline_rendering_create_info.stencilAttachmentFormat = VK_FORMAT_UNDEFINED;

    VkGraphicsPipelineCreateInfo graphics_pipeline_create_info;
    graphics_pipeline_create_info.sType               = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    graphics_pipeline_create_info.pNext               = &pipeline_rendering_create_info;
    graphics_pipeline_create_info.flags               = 0;
    graphics_pipeline_create_info.stageCount          = 2;
    graphics_pipeline_create_info.pStages             = shader_stages;
    graphics_pipeline_create_info.pVertexInputState   = &vertex_input_state_create_info;
    graphics_pipeline_create_info.pInputAssemblyState = &pipeline_input_assembly_state_create_info;
    graphics_pipeline_create_info.pTessellationState  = 0;
    graphics_pipeline_create_info.pViewportState      = &pipeline_viewport_state_create_info;
    graphics_pipeline_create_info.pRasterizationState = &pipeline_rasterization_state_create_info;
    graphics_pipeline_create_info.pMultisampleState   = &pipeline_multisample_state_create_info;
    graphics_pipeline_create_info.pDepthStencilState  = 0;
    graphics_pipeline_create_info.pColorBlendState    = &pipeline_color_blend_state_create_info;
    graphics_pipeline_create_info.pDynamicState       = &pipeline_dynamic_state_create_info;
    graphics_pipeline_create_info.layout              = Vk_Pipeline_Layout;
    graphics_pipeline_create_info.renderPass          = VK_NULL_HANDLE;
    graphics_pipeline_create_info.subpass             = 0;
    graphics_pipeline_create_info.basePipelineHandle  = VK_NULL_HANDLE;
    graphics_pipeline_create_info.basePipelineIndex   = 0;

    VK_CHECK(vkCreateGraphicsPipelines(Vk_Device, VK_NULL_HANDLE,
                                       1, &graphics_pipeline_create_info,
                                       NULL, &Vk_Graphics_Pipeline));
}

void vulkan_kill(void)
{

    VK_CHECK(vkDeviceWaitIdle(Vk_Device));

    for (uint32_t i = 0; i < Vk_Num_Frames; ++i) {
        VkFrameData *frame = &Vk_Frames[i];
        vkDestroySemaphore(Vk_Device, frame->sem_present_complete, NULL);
        vkDestroyFence(Vk_Device, frame->fnc_render_complete, NULL);
    }


    vkDestroyCommandPool(Vk_Device, Vk_Cmd_Pool, NULL);
    vkDestroyShaderModule(Vk_Device, Vk_Vertex_Shader, NULL);
    vkDestroyShaderModule(Vk_Device, Vk_Fragment_Shader, NULL);
    vkDestroyDescriptorSetLayout(Vk_Device, Vk_Image_Set_Layout, NULL);
    vkDestroyPipeline(Vk_Device, Vk_Graphics_Pipeline, NULL);
    vkDestroyPipelineLayout(Vk_Device, Vk_Pipeline_Layout, NULL);

    for (uint32_t i = 0; i < Vk_Swapchain_Image_Count; ++i) {
        vkDestroyImageView(Vk_Device, Vk_Swapchain_Image_Views[i], NULL);
        vkDestroySemaphore(Vk_Device, Vk_Sems_Render_Complete[i], NULL);
    }

    vkDestroySwapchainKHR(Vk_Device, Vk_Swapchain, NULL);
    Vk_Swapchain = VK_NULL_HANDLE;

    vkDestroyDevice(Vk_Device, NULL);
    vkDestroySurfaceKHR(Vk_Instance, Vk_Surface, NULL);
    Vk_Debug_utils_destroy_messenger(Vk_Instance, Vk_Debug_utils_messenger, NULL);
    vkDestroyInstance(Vk_Instance, NULL);

}

void vulkan_resize(int w, int h)
{
    Vk_Window_Size = create_swapchain_and_image_views();
}

void vulkan_draw_with_background_color(float r, float g, float b, float a)
{

    VkFrameData *frame = &Vk_Frames[Vk_Curr_Frame];
    VK_CHECK(vkWaitForFences(Vk_Device, 1, &frame->fnc_render_complete, VK_TRUE, UINT64_MAX));
    vkResetFences(Vk_Device, 1, &frame->fnc_render_complete);

    uint32_t swapchain_image_idx;
    VkResult acquire_result = vkAcquireNextImageKHR(Vk_Device, Vk_Swapchain,
                                                    UINT64_MAX, frame->sem_present_complete,
                                                    VK_NULL_HANDLE, &swapchain_image_idx);

    if (acquire_result != VK_SUCCESS && acquire_result != VK_SUBOPTIMAL_KHR) {
        VK_CHECK(acquire_result);
    }


    VkSemaphore sem_render_complete = Vk_Sems_Render_Complete[swapchain_image_idx];

    VkCommandBuffer cmd = frame->cmd_buffer;
    vkResetCommandBuffer(cmd, 0);

    VkCommandBufferBeginInfo cmd_begin_info;
    cmd_begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    cmd_begin_info.pNext = 0;
    cmd_begin_info.flags = 0;
    cmd_begin_info.pInheritanceInfo = 0;
    VK_CHECK(vkBeginCommandBuffer(cmd, &cmd_begin_info));

    {
        swapchain_image_barrier(cmd, swapchain_image_idx,
                                0,                                              0,                         
                                VK_IMAGE_LAYOUT_UNDEFINED,                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
                                0,                                              VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
                                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

        static float t = 0.f;
        t += vd_fw_delta_s();

        VkClearColorValue clear_color_value = {(sinf(t) + 1.f) * 0.5f, 0.1f, 0.7f, 1.f};
        VkRenderingAttachmentInfo color_attachment_info;
        color_attachment_info.sType              = VK_STRUCTURE_TYPE_RENDERING_ATTACHMENT_INFO;
        color_attachment_info.pNext              = 0;
        color_attachment_info.imageView          = Vk_Swapchain_Image_Views[swapchain_image_idx];
        color_attachment_info.imageLayout        = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        color_attachment_info.resolveMode        = VK_RESOLVE_MODE_NONE;
        color_attachment_info.resolveImageView   = VK_NULL_HANDLE;
        color_attachment_info.resolveImageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        color_attachment_info.loadOp             = VK_ATTACHMENT_LOAD_OP_CLEAR;
        color_attachment_info.storeOp            = VK_ATTACHMENT_STORE_OP_STORE;
        color_attachment_info.clearValue.color   = clear_color_value;

        VkRenderingInfo rendering_info;
        rendering_info.sType                = VK_STRUCTURE_TYPE_RENDERING_INFO;
        rendering_info.pNext                = 0;
        rendering_info.flags                = 0;
        rendering_info.renderArea.offset    = {0, 0};
        rendering_info.renderArea.extent    = Vk_Window_Size;
        rendering_info.layerCount           = 1;
        rendering_info.viewMask             = 0;
        rendering_info.colorAttachmentCount = 1;
        rendering_info.pColorAttachments    = &color_attachment_info;
        rendering_info.pDepthAttachment     = 0;
        rendering_info.pStencilAttachment   = 0;
        vkCmdBeginRendering(cmd, &rendering_info);

        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, Vk_Graphics_Pipeline);

        VkViewport viewport = {
            0.f, 0.f,
            (float)Vk_Window_Size.width, (float)Vk_Window_Size.height,
            0.f, 1.f
        };
        vkCmdSetViewport(cmd, 0, 1, &viewport);

        VkRect2D scissor = {
            {0, 0},
            Vk_Window_Size,
        };
        vkCmdSetScissor(cmd, 0, 1, &scissor);

        // vkCmdDraw(cmd, 3, 1, 0, 0);

        vkCmdEndRendering(cmd);

        swapchain_image_barrier(cmd, swapchain_image_idx,
                                0,                                              0,                         
                                VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,       VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                                VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,           0,
                                VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,  VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

    }
    VK_CHECK(vkEndCommandBuffer(cmd));

    VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

    VkSubmitInfo submit_info;
    submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_info.pNext = 0;
    submit_info.waitSemaphoreCount = 1;
    submit_info.pWaitSemaphores = &frame->sem_present_complete;
    submit_info.pWaitDstStageMask = &wait_stage;
    submit_info.commandBufferCount = 1;
    submit_info.pCommandBuffers = &cmd;
    submit_info.signalSemaphoreCount = 1;
    submit_info.pSignalSemaphores = &sem_render_complete;

    VK_CHECK(vkQueueSubmit(Vk_Queue, 1, &submit_info, frame->fnc_render_complete));

    VkPresentInfoKHR present_info;
    present_info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_info.pNext = 0;
    present_info.waitSemaphoreCount = 1;
    present_info.pWaitSemaphores = &sem_render_complete;
    present_info.swapchainCount = 1;
    present_info.pSwapchains = &Vk_Swapchain;
    present_info.pImageIndices = &swapchain_image_idx;
    present_info.pResults = 0;
    VkResult present_result = vkQueuePresentKHR(Vk_Queue, &present_info);
    if (1
        && (present_result != VK_ERROR_OUT_OF_DATE_KHR)
        && (present_result != VK_SUBOPTIMAL_KHR)
        && (present_result != VK_SUCCESS)
        )
    {
        VK_CHECK(present_result);
    }

    Vk_Curr_Frame++;
    if (Vk_Curr_Frame >= Vk_Num_Frames) {
        Vk_Curr_Frame = 0;
    }
}

static GraphicsBackendImpl Vulkan_Impl = {
    GRAPHICS_BACKEND_VULKAN,
    vulkan_init,
    vulkan_kill,
    vulkan_resize,
    vulkan_draw_with_background_color,
};

static VkBool32 debug_utils_messenger_callback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                               VkDebugUtilsMessageTypeFlagsEXT message_types,
                                               const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                                               void *usr_data)
{
    (void)message_types;
    (void)usr_data;
    if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        printf("[Vulkan/Verbose]:  %s\n", callback_data->pMessage);
    }

    if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
        printf("[Vulkan/Info   ]:   %s\n", callback_data->pMessage);
    }

    if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        printf("[Vulkan/Warning]:   %s\n", callback_data->pMessage);
    }

    if (message_severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        printf("[Vulkan/Error  ]:  %s\n", callback_data->pMessage);
    }

    return VK_TRUE;
}

static void swapchain_image_barrier(VkCommandBuffer cmd, uint32_t image_index,
                                    uint32_t src_queue_family, uint32_t dst_queue_family,
                                    VkImageLayout old_layout, VkImageLayout new_layout,
                                    VkAccessFlags2 src_access, VkAccessFlags2 dst_access,
                                    VkPipelineStageFlags2 src_stage, VkPipelineStageFlags2 dst_stage)
{
    VkImageMemoryBarrier2 barrier;
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER_2;
    barrier.pNext = 0;
    barrier.srcStageMask = src_stage;
    barrier.srcAccessMask = src_access;
    barrier.dstStageMask = dst_stage;
    barrier.dstAccessMask = dst_access;
    barrier.oldLayout = old_layout;
    barrier.newLayout = new_layout;
    barrier.srcQueueFamilyIndex = src_queue_family;
    barrier.dstQueueFamilyIndex = dst_queue_family;
    barrier.image = Vk_Swapchain_Images[image_index];
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkDependencyInfo dependency_info = {0};
    dependency_info.sType = VK_STRUCTURE_TYPE_DEPENDENCY_INFO;
    dependency_info.imageMemoryBarrierCount = 1;
    dependency_info.pImageMemoryBarriers = &barrier;

    vkCmdPipelineBarrier2(cmd, &dependency_info);
}

static VkExtent2D create_swapchain_and_image_views()
{
    VK_CHECK(vkDeviceWaitIdle(Vk_Device));

    VkSurfaceCapabilitiesKHR surface_caps;
    VK_CHECK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(Vk_Physical_Device, Vk_Surface, &surface_caps));
    VkExtent2D size = surface_caps.maxImageExtent;

    VkSwapchainKHR old_swapchain = Vk_Swapchain;
    if (Vk_Swapchain) {
        // Free swapchain data

        for (uint32_t i = 0; i < Vk_Swapchain_Image_Count; ++i) {
            vkDestroyImageView(Vk_Device, Vk_Swapchain_Image_Views[i], NULL);
            vkDestroySemaphore(Vk_Device, Vk_Sems_Render_Complete[i], NULL);
        }
    }

    uint32_t swapchain_queues[1] = {0};
    uint32_t num_swapchain_queues = 1;
    VkSharingMode swapchain_image_sharing_mode = VK_SHARING_MODE_EXCLUSIVE;

    VkSwapchainPresentScalingCreateInfoKHR swapchain_present_scaling_create_info;
    swapchain_present_scaling_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_PRESENT_SCALING_CREATE_INFO_KHR;
    swapchain_present_scaling_create_info.pNext = 0;
    swapchain_present_scaling_create_info.scalingBehavior = VK_PRESENT_SCALING_STRETCH_BIT_KHR;
    // swapchain_present_scaling_create_info.scalingBehavior = VK_PRESENT_SCALING_ONE_TO_ONE_BIT_KHR;
    swapchain_present_scaling_create_info.presentGravityX = VK_PRESENT_GRAVITY_MIN_BIT_KHR;
    swapchain_present_scaling_create_info.presentGravityY = VK_PRESENT_GRAVITY_MIN_BIT_KHR;

    VkSwapchainCreateInfoKHR swapchain_create_info;
    swapchain_create_info.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_create_info.pNext                 = &swapchain_present_scaling_create_info;
    swapchain_create_info.flags                 = 0;
    swapchain_create_info.surface               = Vk_Surface;
    swapchain_create_info.minImageCount         = Vk_Min_Swapchain_Image_Count;
    swapchain_create_info.imageFormat           = Vk_Surface_Format.format;
    swapchain_create_info.imageColorSpace       = Vk_Surface_Format.colorSpace;
    swapchain_create_info.imageExtent           = size;
    swapchain_create_info.imageArrayLayers      = 1;
    swapchain_create_info.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_create_info.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_create_info.queueFamilyIndexCount = num_swapchain_queues;
    swapchain_create_info.pQueueFamilyIndices   = swapchain_queues;
    swapchain_create_info.preTransform          = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapchain_create_info.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchain_create_info.presentMode           = Vk_Present_Mode;
    swapchain_create_info.clipped               = VK_TRUE;
    swapchain_create_info.oldSwapchain          = old_swapchain;

    VK_CHECK(vkCreateSwapchainKHR(Vk_Device, &swapchain_create_info, NULL, &Vk_Swapchain));

    VK_CHECK(vkGetSwapchainImagesKHR(Vk_Device, Vk_Swapchain, &Vk_Swapchain_Image_Count, 0));
    if (Vk_Swapchain_Image_Count > 8) {
        Vk_Swapchain_Image_Count = 8;
    }
    VK_CHECK(vkGetSwapchainImagesKHR(Vk_Device, Vk_Swapchain, &Vk_Swapchain_Image_Count, Vk_Swapchain_Images));

    for (uint32_t i = 0; i < Vk_Swapchain_Image_Count; ++i) {
        VkImageViewCreateInfo image_view_create_info;
        image_view_create_info.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        image_view_create_info.pNext                           = 0;
        image_view_create_info.flags                           = 0;
        image_view_create_info.image                           = Vk_Swapchain_Images[i]; 
        image_view_create_info.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
        image_view_create_info.format                          = Vk_Surface_Format.format;
        image_view_create_info.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;
        image_view_create_info.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
        image_view_create_info.subresourceRange.baseMipLevel   = 0;
        image_view_create_info.subresourceRange.levelCount     = 1;
        image_view_create_info.subresourceRange.baseArrayLayer = 0;
        image_view_create_info.subresourceRange.layerCount     = 1;
        VK_CHECK(vkCreateImageView(Vk_Device, &image_view_create_info, NULL, &Vk_Swapchain_Image_Views[i]));

        VkSemaphoreCreateInfo semaphore_create_info;
        semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphore_create_info.pNext = 0;
        semaphore_create_info.flags = 0;
        VK_CHECK(vkCreateSemaphore(Vk_Device, &semaphore_create_info, NULL, &Vk_Sems_Render_Complete[i]));
    }

    if (old_swapchain != VK_NULL_HANDLE) {
        vkDestroySwapchainKHR(Vk_Device, old_swapchain, NULL);
    }

    return size;
}

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
