#include "vd_fw.h"
#include <windows.h>
#include <mmdeviceapi.h>
#include <initguid.h>
#include <directmanipulation.h>
#include <stdio.h>
#include <stdlib.h>
#include <comdef.h>

#define VERTEX_SOURCE \
"#version 330 core                                                                                                 \n" \
"layout (location = 0) in vec2 aPos;                                                                               \n" \
"                                                                                                                  \n" \
"uniform vec2 rect_off;                                                                                            \n" \
"uniform vec2 rect_size;                                                                                           \n" \
"uniform mat4 projection;                                                                                          \n" \
"uniform mat3 view;                                                                                                \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    vec3 p = view * vec3(aPos * rect_size + rect_off, 1.f);                                                       \n" \
"    gl_Position = projection * vec4(p, 1.0f);                                                                     \n" \
"}                                                                                                                 \n" \

#define FRAGMENT_SOURCE \
"#version 330 core                                                                                                 \n" \
"out vec4 FragColor;                                                                                               \n" \
"                                                                                                                  \n" \
"uniform vec4 rect_color;                                                                                          \n" \
"                                                                                                                  \n" \
"void main()                                                                                                       \n" \
"{                                                                                                                 \n" \
"    FragColor = rect_color;                                                                                       \n" \
"}                                                                                                                 \n" \

#define CHECK_HRESULT(expr) do {\
    HRESULT _hr = (expr); \
    if ((_hr) != S_OK) { printf("Failed at: %s: 0x%08x('%s')\n", #expr, _hr, cv_hresult_cstr(_hr)); DebugBreak(); } \
} while (0)

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "uuid.lib")

static IDirectManipulationManager       *Dm_Manager = 0;
static IDirectManipulationUpdateManager *Dm_Update_Manager = 0;
static IDirectManipulationViewport      *Dm_Viewport = 0;
static int                              Dm_Initialized = 0;
static int                              Dm_Should_Keep_Updating = 0;

static GLuint Gl_Program = 0;
static float View_Matrix[3 * 3] = {
    1, 0, 0,
    0, 1, 0,
    0, 0, 1,
};

#define NUM_PAGES   12
#define PAGE_WIDTH  400.f
#define PAGE_LENGTH 120.f

#define COLOR_COUNT 10
static float All_Colors[3 * COLOR_COUNT] = {
    0.6f, 0.1f, 0.1f,
    0.1f, 0.6f, 0.1f,
    0.1f, 0.1f, 0.6f,
    0.1f, 0.6f, 0.1f,
    0.6f, 0.1f, 0.1f,
    0.6f, 0.1f, 0.1f,
    0.1f, 0.6f, 0.6f,
    0.6f, 0.1f, 0.6f,
    0.6f, 0.6f, 0.6f,
};

static char *cv_hresult_cstr(HRESULT hr)
{
    LPSTR lpstr = 0;
    const DWORD cchmsg = FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM
                                        | FORMAT_MESSAGE_IGNORE_INSERTS
                                        | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                                        NULL, // (not used with FORMAT_MESSAGE_FROM_SYSTEM)
                                        hr,
                                        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                        (char*)&lpstr,
                                        0,
                                        NULL);
    if (cchmsg > 0) {
        lpstr[cchmsg] = 0;
        return lpstr;
    } else {
        return "(null)";
    }
}

class CFWDirectManipulationViewportEventHandler : public IDirectManipulationViewportEventHandler
{
public:
    LONG refcount;

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject) override
    {
        if (!ppvObject)
            return E_POINTER;

        if (riid == IID_IUnknown || riid == IID_IDirectManipulationViewportEventHandler)
        {
            *ppvObject = static_cast<IDirectManipulationViewportEventHandler*>(this);
            AddRef();
            return S_OK;
        }

        *ppvObject = nullptr;
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE AddRef() override
    {
        return InterlockedIncrement(&refcount);
    }

    ULONG STDMETHODCALLTYPE Release() override
    {
        ULONG ref = InterlockedDecrement(&refcount);
        if (ref == 0)
            delete this;
        return ref;
    }

    HRESULT STDMETHODCALLTYPE OnViewportStatusChanged( 
        IDirectManipulationViewport *viewport,
        DIRECTMANIPULATION_STATUS current,
        DIRECTMANIPULATION_STATUS previous) override
    {
        (void)viewport;
        (void)previous;

        if ((current >= DIRECTMANIPULATION_RUNNING) && (current <= DIRECTMANIPULATION_INERTIA)) {
            Dm_Should_Keep_Updating = 1;
        } else {
            Dm_Should_Keep_Updating = 0;
        }
        return S_OK; 
    }
    
    HRESULT STDMETHODCALLTYPE OnViewportUpdated( 
        IDirectManipulationViewport *viewport) override
    {
        (void)viewport;
        return S_OK;
    }
    
    HRESULT STDMETHODCALLTYPE OnContentUpdated( 
        IDirectManipulationViewport *viewport,
        IDirectManipulationContent *content) override
    {
        (void)viewport;
        (void)content;
        // 3x2 matrix
        // 0..1 | Xx Xy |
        // 2..3 | Yx Yy |
        // 4..5 | Ox Oy |
        float xform[6];
        CHECK_HRESULT(content->GetContentTransform(xform, 6));
        // DWORD tid = GetCurrentThreadId();
        // printf("%d OnContentUpdated %f %f %f %f %f %f\n", tid, xform[0], xform[1], xform[2], xform[3], xform[4], xform[5]);

        float m00 = xform[0]; float m01 = xform[1];
        float m10 = xform[2]; float m11 = xform[3];
        float m20 = xform[4]; float m21 = xform[5];

        View_Matrix[0 * 3 + 0] = m00; View_Matrix[0 * 3 + 1] = m01; View_Matrix[0 * 3 + 2] = 0.f;
        View_Matrix[1 * 3 + 0] = m10; View_Matrix[1 * 3 + 1] = m11; View_Matrix[1 * 3 + 2] = 0.f;
        View_Matrix[2 * 3 + 0] = m20; View_Matrix[2 * 3 + 1] = m21; View_Matrix[2 * 3 + 2] = 1.f;

        return S_OK;
    }
};

static void put_rect(float x, float y, float w, float h, float r, float g, float b)
{
    glUniform4f(glGetUniformLocation(Gl_Program, "rect_color"), r, g, b, 1.0f);
    glUniform2f(glGetUniformLocation(Gl_Program, "rect_size"), w, h);
    glUniform2f(glGetUniformLocation(Gl_Program, "rect_off"), x, y);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init(NULL);

    float rect_vertices[] = {
         0.0f,  0.0f,
        +1.0f,  0.0f,
         0.0f, +1.0f,
        +1.0f, +1.0f
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rect_vertices), rect_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    const char *vertex_shader_source = VERTEX_SOURCE;
    const char *fragment_shader_source = FRAGMENT_SOURCE;

    GLuint vertex_shader = vd_fw_compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
    GLuint fragment_shader = vd_fw_compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

    Gl_Program = glCreateProgram();
    glAttachShader(Gl_Program, vertex_shader);
    glAttachShader(Gl_Program, fragment_shader);
    vd_fw_link_program(Gl_Program);

    while (vd_fw_running()) {

        if (Dm_Initialized && Dm_Should_Keep_Updating) {
            Dm_Update_Manager->Update(nullptr);
            static int count = 0;
        }

        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        int w, h;
        vd_fw_get_size(&w, &h);

        float wheel_x, wheel_y;
        vd_fw_get_mouse_wheel(&wheel_x, &wheel_y);

        glViewport(0,0,w,h);
        glClearColor(0.2f, 0.3f, 0.4f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(Gl_Program);
        glBindVertexArray(VAO);

        {
            float projection[16];
            vd_fw_u_ortho(0.f, (float)w, (float)h, 0.f, -1.f, 1.f, projection);
            glUniformMatrix4fv(glGetUniformLocation(Gl_Program, "projection"), 1, GL_FALSE, projection);
            glUniformMatrix3fv(glGetUniformLocation(Gl_Program, "view"), 1, GL_FALSE, View_Matrix);
        }
        float x = (((float)w) * 0.5f) - PAGE_WIDTH * 0.5f;

        for (int i = 0; i < NUM_PAGES; ++i) {
            float r = All_Colors[((i * 3) % COLOR_COUNT) + 0];
            float g = All_Colors[((i * 3) % COLOR_COUNT) + 1];
            float b = All_Colors[((i * 3) % COLOR_COUNT) + 2];
            put_rect(0.f, i * PAGE_LENGTH, PAGE_WIDTH, PAGE_LENGTH, r, g, b);
        }


        vd_fw_swap_buffers();
    }
    return 0;
}

static int my_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
#define VD_FW_WIN32_ADDITIONAL_WNDPROC my_wndproc

#define VD_FW_IMPL
#include "vd_fw.h"

static void dm_update_content_transform(void)
{

}

static int my_wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    (void)lparam;

    if (!Dm_Initialized)
    {

        CoInitializeEx(0, COINIT_MULTITHREADED);

        CHECK_HRESULT(CoCreateInstance(CLSID_DirectManipulationManager,
                                       nullptr,
                                       CLSCTX_INPROC_SERVER,
                                       IID_PPV_ARGS(&Dm_Manager)));

        CHECK_HRESULT(Dm_Manager->GetUpdateManager(IID_PPV_ARGS(&Dm_Update_Manager)));
        CHECK_HRESULT(Dm_Manager->CreateViewport(nullptr, hwnd, IID_PPV_ARGS(&Dm_Viewport)));

        DIRECTMANIPULATION_CONFIGURATION configuration = DIRECTMANIPULATION_CONFIGURATION_INTERACTION |
                                                         DIRECTMANIPULATION_CONFIGURATION_TRANSLATION_X |
                                                         DIRECTMANIPULATION_CONFIGURATION_TRANSLATION_Y |
                                                         DIRECTMANIPULATION_CONFIGURATION_TRANSLATION_INERTIA |
                                                         DIRECTMANIPULATION_CONFIGURATION_RAILS_X |
                                                         DIRECTMANIPULATION_CONFIGURATION_RAILS_Y |
                                                         DIRECTMANIPULATION_CONFIGURATION_SCALING |
                                                         DIRECTMANIPULATION_CONFIGURATION_SCALING_INERTIA;

        CHECK_HRESULT(Dm_Viewport->ActivateConfiguration(configuration));
        // CHECK_HRESULT(Dm_Viewport->SetViewportOptions(DIRECTMANIPULATION_VIEWPORT_OPTIONS_MANUALUPDATE));
        CHECK_HRESULT(Dm_Viewport->SetViewportOptions(DIRECTMANIPULATION_VIEWPORT_OPTIONS_DEFAULT));
        CFWDirectManipulationViewportEventHandler *viewport_event_handler = new CFWDirectManipulationViewportEventHandler();

        DWORD viewport_event_handler_cookie = 0;
        CHECK_HRESULT(Dm_Viewport->AddEventHandler(hwnd, viewport_event_handler, &viewport_event_handler_cookie));

        RECT rect;
        rect.left = 0;
        rect.top = 0;
        rect.right = (int)PAGE_WIDTH;
        rect.bottom = (int)PAGE_LENGTH * NUM_PAGES;
        CHECK_HRESULT(Dm_Viewport->SetViewportRect(&rect));

        CHECK_HRESULT(Dm_Viewport->Enable());
        Dm_Update_Manager->Update(nullptr);

        CHECK_HRESULT(Dm_Manager->Activate(hwnd));
        Dm_Initialized = 1;
    }

    switch (msg) {
        case WM_POINTERDOWN: {
            UINT32 pointer_id = GET_POINTERID_WPARAM(wparam);
            CHECK_HRESULT(Dm_Viewport->SetContact(pointer_id));
            return 0;
        } break;

        case DM_POINTERHITTEST: {
            UINT32 pointer_id = GET_POINTERID_WPARAM(wparam);
            CHECK_HRESULT(Dm_Viewport->SetContact(pointer_id));
            return 0;
        } break;

        case WM_POINTERUP: {
            UINT32 pointer_id = GET_POINTERID_WPARAM(wparam);
            CHECK_HRESULT(Dm_Viewport->ReleaseContact(pointer_id));
            return 0;
        } break;
    }

    return 1;
}
