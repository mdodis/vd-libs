#include "vd_fw.h"
#include "vd_um.h"
#define VD_MACRO_ABBREVIATIONS 1
#include "vd_cg.h"

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

    vd_um_init();

    vd_fw_set_title("UM Basic");

    GLuint program = 0;
    unsigned long long program_time = 0;
    // {
    //     const char *vertex_shader_source;
    //     size_t vertex_shader_source_size;
    //     const char *fragment_shader_source;
    //     size_t fragment_shader_source_size;
    //     vd_um_gl_get_default_shader_sources(&vertex_shader_source, &vertex_shader_source_size, &fragment_shader_source, &fragment_shader_source_size);

    //     GLuint vshd = vd_fw_compile_shader(GL_VERTEX_SHADER, vertex_shader_source);
    //     GLuint fshd = vd_fw_compile_shader(GL_FRAGMENT_SHADER, fragment_shader_source);
    //     program = glCreateProgram();
    //     glAttachShader(program, vshd);
    //     glAttachShader(program, fshd);
    //     vd_fw_link_program(program);
    // }

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
        VdUmGlAttribPointerType attrib_pointer_type; vd_um_gl_get_attribute_properties(i, &size, &type, &normalized, &stride, &pointer, &divisor, &attrib_pointer_type);
        glEnableVertexAttribArray(i);
        if (attrib_pointer_type == VD_UM_GL_ATTRIB_POINTER_TYPE_INTEGER) {
            glVertexAttribIPointer(i, size, type, stride, pointer);
        } else {
            glVertexAttribPointer(i, size, type, normalized, stride, pointer);
        }
        glVertexAttribDivisor(i, divisor);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    F3 camera_origin = fzero3();
    FQuat camera_orientation = fidentityquat();
    float heading = 0.f;
    float pitch = 0.f;

    while (vd_fw_running()) {
        if (vd_fw_close_requested()) {
            vd_fw_quit();
        }

        vd_fw_compile_or_hotload_program(&program, &program_time,
                                         "./glsl/um_basic.vert", "./glsl/um_basic.frag");

        F2 mouse_pos;
        vd_fw_get_mouse_statef(&mouse_pos.x, &mouse_pos.y);

        if (vd_fw_get_key_pressed(VD_FW_KEY_ESCAPE)) {
            vd_fw_set_mouse_locked(!vd_fw_get_mouse_locked());
        }

        float dt = vd_fw_delta_s();

        if (vd_fw_get_mouse_locked()) {
            float mx, my;
            vd_fw_get_mouse_delta(&mx, &my);
            heading += mx;
            pitch += my;

            camera_orientation = feulerquat(fm3(fdeg2rad(pitch), fdeg2rad(heading), 0.f));

            F3 forward = fmulquat_3(camera_orientation, fm3(0,0,1));
            F3 right   = fmulquat_3(camera_orientation, fm3(1,0,0));

            float input_forward = (float)(vd_fw_get_key_down('W') - vd_fw_get_key_down('S'));
            float input_right   = (float)(vd_fw_get_key_down('D') - vd_fw_get_key_down('A'));

            F3 move_direction = fzero3();
            move_direction = fadd3(move_direction, fscale3(forward, input_forward));
            move_direction = fadd3(move_direction, fscale3(right, input_right));
            move_direction = fnoz3(move_direction);

            float move_speed = dt * 5.f;
            camera_origin = fadd3(camera_origin, fscale3(move_direction, move_speed));

        }

        int w,h;
        vd_fw_get_size(&w, &h);


        float aspect = (float)w/(float)h;
        F4x4 proj = fperspective4x4(fdeg2rad(60.f), aspect, 0.1f, 100.f);
        F4x4 inv_proj = vd_finverse4x4(&proj);
        F4x4 inv_view;
        {

            F4x4 V_P = ftranslation4x4(fscale3(camera_origin, -1.f));
            F4x4 V_R = fto4x4quat(vd_fconquat(camera_orientation)); 
            inv_view = fmul4x4(&V_P, &V_R);
        }

        F4x4 view;
        {
            F4x4 V_P  = ftranslation4x4(camera_origin);
            F4x4 V_R  = fto4x4quat(camera_orientation); 
            view = fmul4x4(&V_P, &V_R);
        }

        vd_um_frame_begin(dt);
        float viewport[4] = {0, 0, (float)w, (float)h};
        vd_um_viewport_begin(viewport, inv_proj.p, view.p, camera_origin.e);

        {
            vd_um_event_mouse_position(mouse_pos.e);

            vd_um_grid(fzero3().e, flookrotquat(fm3(0,1,0), fm3(0,0,1)).e, 100.f, fm4(1,1,1,1).e);
            vd_um_segment(fm3(-1,1,1).e, fm3(1,1,1).e, 0.01f, fm4(1,1,0,1).e);

            vd_um_point(fm3(0,0,0).e, 0.05f, fm4(1,0,1,1).e);
            vd_um_i_cylinder(fzero3().e, flookrotquat(fm3(0,0,1), fm3(0,1,0)).e, 1.f, 0.01f, fm4(0.2f,0.2f,0.7f,1).e, fm4(0.2f, 0.7f, 0.7f, 1).e);
            vd_um_i_cylinder(fzero3().e, flookrotquat(fm3(0,1,0), fm3(0,0,1)).e, 1.f, 0.01f, fm4(0.2f,0.7f,0.2f,1).e, fm4(0.2f, 0.7f, 0.7f, 1).e);
            vd_um_i_cylinder(fzero3().e, flookrotquat(fm3(1,0,0), fm3(0,1,0)).e, 1.f, 0.01f, fm4(0.7f,0.2f,0.2f,1).e, fm4(0.2f, 0.7f, 0.7f, 1).e);
        }
        vd_um_viewport_end();
        vd_um_frame_end();

        glViewport(0,0,w,h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, vd_um_gl_get_uniform_name_projection()), 1, GL_FALSE, proj.p);
        glUniformMatrix4fv(glGetUniformLocation(program, vd_um_gl_get_uniform_name_view()), 1, GL_FALSE, inv_view.p);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        int num_passes;
        VdUmRenderPass *passes = vd_um_frame_get_passes_for_viewport(0, &num_passes);
        int num_vertices;
        VdUmVertex *vertices = vd_um_frame_get_vertex_buffer(&num_vertices);
        for (int i = 0; i < num_passes; ++i) {
            VdUmRenderPass *pass = &passes[i];
            size_t start = pass->first_instance * sizeof(VdUmVertex);
            size_t size  = pass->instance_count * sizeof(VdUmVertex);
            glBufferSubData(GL_ARRAY_BUFFER, 0, size, (unsigned char*)vertices + start);
            glDrawArraysInstanced(GL_TRIANGLES, 0, pass->vertex_count, pass->instance_count);
        }

        vd_fw_swap_buffers();
    }

    return 0;
}

#define VD_UM_IMPL
#include "vd_um.h"

#define VD_FW_IMPL
#include "vd_fw.h"
