#include "vd_fw.h"
#include "vd_um.h"

int main(int argc, char const *argv[])
{
    (void)argc;
    (void)argv;

    vd_fw_init(& (VdFwInitInfo) {
        .gl = {
            .version = VD_FW_GL_VERSION_4_1,
            .debug_on = 1,
        }
    });

    vd_fw_set_title("UM Basic");

    GLuint program;
    {
        const char *vertex_shader_source;
        size_t vertex_shader_source_size;
        const char *fragment_shader_source;
        size_t fragment_shader_source_size;
        vd_um_gl_get_default_shader_sources(&vertex_shader_source, &vertex_shader_source_size, &fragment_shader_source, &fragment_shader_source_size);

        GLuint vshd = vd_fw_compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
        GLuint fshd = vd_fw_compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
        program = glCreateProgram();
        glAttachShader(program, vshd);
        glAttachShader(program, fshd);
        vd_fw_link_program(program);
    }

    GLuint vao;
    GLuint vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VdUmVertex) * 1024, NULL, GL_DYNAMIC_DRAW);


    for (int i = 0; i < vd_um_gl_get_num_attributes(); ++i) {
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        void *pointer;
        GLuint divisor;
        VdUmGlAttribPointerType attrib_pointer_type;
        vd_um_gl_get_attribute_properties(i, &size, &type, &normalized, &stride, &pointer, &divisor, &attrib_pointer_type);
        glEnableVertexAttribArray(i);
        if (attrib_pointer_type == VD_UM_GL_ATTRIB_POINTER_TYPE_INTEGER) {
            glVertexAttribIPointer(i, size, type, stride, pointer);
        } else {
            glVertexAttribPointer(i, size, type, normalized, stride, pointer);
        }
        glVertexAttribDivisor(i, divisor);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    while (vd_fw_running()) {
        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        int w,h;
        vd_fw_get_size(&w, &h);
        glViewport(0,0,w,h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        vd_fw_swap_buffers();
    }

    return 0;
}

#define VD_UM_IMPL
#include "vd_um.h"

#define VD_FW_IMPL
#include "vd_fw.h"
