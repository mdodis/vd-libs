#define VD_FW_NO_CRT 0
#define VD_FW_WIN32_SUBSYSTEM VD_FW_WIN32_SUBSYSTEM_CONSOLE
#include "vd_fw.h"
#include "stb_image.h"

#include <stdio.h>

typedef enum {
    GRAPHICS_BACKEND_OPENGL,
    GRAPHICS_BACKEND_DIRECT3D11,
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
#ifdef _WIN32
extern GraphicsBackendImpl D3D11_Impl;
#endif

static GraphicsBackendImpl* Graphics_Backends[] = {
    &OpenGL_Impl,
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
    VdFwOpenGLOptions options;
    options.version = VD_FW_GL_VERSION_4_5;
    options.debug_on = 1;
    vd_fw_set_graphics_api(cv_graphics_backend_to_fw_graphics_api(impl->backend), &options);

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
        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        int w, h;
        if (vd_fw_get_size(&w, &h)) {
            Current_Graphics->resize(w, h);
        }

        Current_Graphics->draw_with_background_color(0.2f,0.2f,0.2f,1.f);

        int will_switch_to_next_backend = 0;
        if (vd_fw_get_key_pressed(VD_FW_KEY_SPACE)) {
            will_switch_to_next_backend = 1;
        }

        vd_fw_swap_buffers();

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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
