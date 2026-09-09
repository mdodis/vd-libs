/**
 * vd_stuff.h - Random Stuff
 * ---------------------------------------------------------------------------------------------------------------------
 * 
 * zlib License
 * 
 * (C) Copyright 2025-2026 Michael Dodis (michaeldodisgr@gmail.com)
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * ---------------------------------------------------------------------------------------------------------------------
 */
#ifndef VD_STUFF_H
#define VD_STUFF_H
#define VD_STUFF_VERSION_MAJOR 0
#define VD_STUFF_VERSION_MINOR 0
#define VD_STUFF_VERSION_PATCH 1
#define VD_STUFF_VERSION       ((VD_STUFF_VERSION_MAJOR << 16) | (VD_STUFF_VERSION_MINOR << 8) | (VD_STUFF_VERSION_PATCH))

#ifndef VD_STUFF_API
#   ifdef VD_STUFF_STATIC
#       define VD_STUFF_API static
#   else
#       define VD_STUFF_API extern
#   endif // VD_STUFF_STATIC
#endif // !VD_STUFF_API

// Graphics
// | Feature                                  | Include File       | Requirements
// |------------------------------------------|--------------------|---------------------------------|
// | VD_STUFF_GRAPHICS_INCLUDE                | Include vd_fw.h    | None                            |
// | VD_STUFF_GRAPHICS_VD_UM_INCLUDE          | Include vd_um.h    | VD_STUFF_GRAPHICS_INCLUDE       |
// | VD_STUFF_GRAPHICS_VD_UI_INCLUDE          | Include vd_ui.h    | VD_STUFF_GRAPHICS_INCLUDE       |
#ifndef VD_STUFF_GRAPHICS_INCLUDE
#   define VD_STUFF_GRAPHICS_INCLUDE 0
#endif // !VD_STUFF_GRAPHICS_INCLUDE

#ifndef VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#   define VD_STUFF_GRAPHICS_VD_UM_INCLUDE 0
#endif // !VD_STUFF_GRAPHICS_VD_UM_INCLUDE

#ifndef VD_STUFF_GRAPHICS_VD_UI_INCLUDE
#   define VD_STUFF_GRAPHICS_VD_UI_INCLUDE 0
#endif // !VD_STUFF_GRAPHICS_VD_UI_INCLUDE

#if VD_STUFF_GRAPHICS_INCLUDE
#   ifndef VD_FW_H
#       error "You must include vd_fw.h before using VD_STUFF_GRAPHICS_INCLUDE"
#   endif // !VD_FW_H
#   if VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#       ifndef VD_UM_H
#           error "You must include vd_um.h before using VD_STUFF_GRAPHICS_VD_UM_INCLUDE"
#       endif // !VD_UM_H
#   endif // VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#   if VD_STUFF_GRAPHICS_VD_UI_INCLUDE
#       ifndef VD_UI_H
#           error "You must include vd_ui.h before using VD_STUFF_GRAPHICS_VD_UI_INCLUDE"
#       endif // !VD_UI_H
#   endif // VD_STUFF_GRAPHICS_VD_UI_INCLUDE
#endif // VD_STUFF_GRAPHICS_INCLUDE

VD_STUFF_API void vd_stuff_init(void);
#if VD_STUFF_GRAPHICS_INCLUDE
VD_STUFF_API int vd_stuff_poll(void);
VD_STUFF_API void vd_stuff_graphics_begin(void);
VD_STUFF_API void vd_stuff_graphics_end(void);

#   if VD_STUFF_GRAPHICS_VD_UM_INCLUDE
VD_STUFF_API void vd_stuff_um_push_events_to_current_viewport(void);
#   endif // VD_STUFF_GRAPHICS_VD_UM_INCLUDE

#   if VD_STUFF_GRAPHICS_VD_UI_INCLUDE
VD_STUFF_API void vd_stuff_ui_push_events(void);
VD_STUFF_API void vd_stuff_ui_render(void);
#   endif // VD_STUFF_GRAPHICS_VD_UI_INCLUDE

#endif // VD_STUFF_GRAPHICS_INCLUDE

#endif // !VD_STUFF_H

#ifdef VD_STUFF_IMPL

#if VD_STUFF_GRAPHICS_VD_UI_INCLUDE
#endif // VD_STUFF_GRAPHICS_VD_UI_INCLUDE


typedef struct {
    int initialized;
#if VD_STUFF_GRAPHICS_INCLUDE
    VdFwEvent *events;
    int       num_events;
#   if VD_STUFF_GRAPHICS_VD_UI_INCLUDE
    GLuint    ui_vao, ui_vbo, ui_program;
#   endif // VD_STUFF_GRAPHICS_VD_UI_INCLUDE
#endif // VD_STUFF_GRAPHICS_INCLUDE
} VdStuff__InternalData;

VdStuff__InternalData Vd_Stuff_G = {0};

VD_STUFF_API void vd_stuff_init(void)
{
#if VD_STUFF_GRAPHICS_INCLUDE
    VdFwInitInfo fw_init_info = {0};
    fw_init_info.api = VD_FW_GRAPHICS_API_OPENGL;
    vd_fw_init(&fw_init_info);

#if VD_STUFF_GRAPHICS_VD_UM_INCLUDE
    vd_um_init();
#endif // VD_STUFF_GRAPHICS_VD_UM_INCLUDE

#if VD_STUFF_GRAPHICS_VD_UI_INCLUDE
    vd_ui_init();

    glGenVertexArrays(1, &Vd_Stuff_G.ui_vao);
    glBindVertexArray(Vd_Stuff_G.ui_vao);

    glGenBuffers(1, &Vd_Stuff_G.ui_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Vd_Stuff_G.ui_vbo);
    glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)vd_ui_get_min_vertex_buffer_size(), 0, GL_DYNAMIC_DRAW);

    for (int i = 0; i < vd_ui_gl_get_num_attributes(); ++i) {
        GLint size;
        GLenum type;
        GLboolean normalized;
        GLsizei stride;
        void *pointer;
        GLuint divisor;
        vd_ui_gl_get_attribute_properties(i, &size, &type, &normalized, &stride, &pointer, &divisor);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, size, type, normalized, stride, pointer);
        glVertexAttribDivisor(i, divisor);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const char *vs_src;
    size_t vs_src_len;
    const char *fs_src;
    size_t fs_src_len;
    vd_ui_gl_get_default_shader_sources(&vs_src, &vs_src_len, &fs_src, &fs_src_len);
    GLuint vs = vd_fw_compile_shader(GL_VERTEX_SHADER, vs_src);
    GLuint fs = vd_fw_compile_shader(GL_FRAGMENT_SHADER, fs_src);
    Vd_Stuff_G.ui_program = glCreateProgram();
    glAttachShader(Vd_Stuff_G.ui_program, vs);
    glAttachShader(Vd_Stuff_G.ui_program, fs);
    vd_fw_link_program(Vd_Stuff_G.ui_program);
    glDeleteShader(vs);
    glDeleteShader(fs);

#endif // VD_STUFF_GRAPHICS_VD_UI_INCLUDE

#endif // VD_STUFF_GRAPHICS_INCLUDE
}

#if VD_STUFF_GRAPHICS_INCLUDE
VD_STUFF_API int vd_stuff_poll(void)
{
    int running = vd_fw_running();

    if (running) {
        Vd_Stuff_G.events = vd_fw_poll(&Vd_Stuff_G.num_events);
    }


    return running;
}

VD_STUFF_API VdFwEvent *vd_stuff_events(void)
{
    return Vd_Stuff_G.events;
}

VD_STUFF_API int vd_stuff_events_count(void)
{
    return Vd_Stuff_G.num_events;
}

#   if VD_STUFF_GRAPHICS_VD_UM_INCLUDE
VD_STUFF_API void vd_stuff_um_push_events_to_current_viewport(void)
{
    float mouse_pos[2];
    int mouse_state = vd_fw_get_mouse_statef(&mouse_pos[0], &mouse_pos[0]);
    float mx, my;
    vd_fw_get_mouse_delta(&mx, &my);

    vd_um_event_mouse_position(mouse_pos);
    vd_um_event_mouse_button(0, mouse_state & VD_FW_MOUSE_STATE_LEFT_BUTTON_DOWN);
    float mouse_delta[2] = { mx, my };
    vd_um_event_mouse_delta(mouse_delta);
}
#   endif // VD_STUFF_GRAPHICS_VD_UM_INCLUDE

#   if VD_STUFF_GRAPHICS_VD_UI_INCLUDE
VD_STUFF_API void vd_stuff_ui_push_events(void)
{
    float ui_scale;
    vd_fw_get_scale(&ui_scale);
    vd_ui_set_scale(ui_scale);
    float mx, my;
    int mouse_state = vd_fw_get_mouse_statef(&mx, &my);
    (void)mouse_state;

    float wx, wy;
    int wheel_moved = vd_fw_get_mouse_wheel(&wx, &wy);
    (void)wheel_moved;

    int focused;
    if (vd_fw_get_focused(&focused)) {
        vd_ui_event_focus(focused);
    }

    int w, h;
    vd_fw_get_size(&w, &h);
    vd_ui_event_size((float)w, (float)h);
    for (int i = 0; i < Vd_Stuff_G.num_events; ++i) {
        VdFwEvent *evt = &Vd_Stuff_G.events[i];
        switch (evt->type) {
            case VD_FW_EVENT_TYPE_MOUSE_MOVE: {
                vd_ui_event_mouse_location((float)evt->data.mouse_move.x, (float)evt->data.mouse_move.y);
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_BUTTON_UP: {
                vd_ui_event_mouse_button(vd_ui_vd_fw_mouse_button_translate(evt->data.mouse_button_up.button), 0);
            } break;

            case VD_FW_EVENT_TYPE_MOUSE_BUTTON_DOWN: {
                vd_ui_event_mouse_button(vd_ui_vd_fw_mouse_button_translate(evt->data.mouse_button_down.button), 1);
            } break;

            case VD_FW_EVENT_TYPE_KEY_UP: {
                int shift   = (evt->data.key_up.modifiers & VD_FW_MOD_SHIFT)   ? 1 : 0;
                int control = (evt->data.key_up.modifiers & VD_FW_MOD_CONTROL) ? 1 : 0;
                int alt     = (evt->data.key_up.modifiers & VD_FW_MOD_ALT)     ? 1 : 0;
                vd_ui_event_mod(VD_UI_MOD_SHIFT, shift);
                vd_ui_event_mod(VD_UI_MOD_CONTROL, control);
                vd_ui_event_mod(VD_UI_MOD_ALT, alt);

                vd_ui_event_key_release(vd_ui_vd_fw_key_translate(evt->data.key_up.key));
            } break;

            case VD_FW_EVENT_TYPE_KEY_DOWN: {
                int shift   = (evt->data.key_down.modifiers & VD_FW_MOD_SHIFT)   ? 1 : 0;
                int control = (evt->data.key_down.modifiers & VD_FW_MOD_CONTROL) ? 1 : 0;
                int alt     = (evt->data.key_down.modifiers & VD_FW_MOD_ALT)     ? 1 : 0;

                vd_ui_event_mod(VD_UI_MOD_SHIFT, shift);
                vd_ui_event_mod(VD_UI_MOD_CONTROL, control);
                vd_ui_event_mod(VD_UI_MOD_ALT, alt);
                vd_ui_event_key_press(vd_ui_vd_fw_key_translate(evt->data.key_down.key));
            } break;

            case VD_FW_EVENT_TYPE_CHARACTER: {
                vd_ui_event_char(evt->data.character.codepoint);
            } break;

            default: break;
        }
    }
    vd_ui_event_mouse_wheel(wx, wy);
}

VD_STUFF_API void vd_stuff_ui_render(void)
{
    // Process updates
    size_t num_updates;
    VdUiUpdate *updates = vd_ui_frame_get_updates(&num_updates);

    for (size_t i = 0; i < num_updates; ++i) {
        VdUiUpdate *update = &updates[i];
        switch (update->type) {
            case VD_UI_UPDATE_TYPE_NEW_TEXTURE: {
                int width                = update->data.new_texture.width;
                int height               = update->data.new_texture.height;
                void *buffer             = update->data.new_texture.buffer;
                size_t buffer_size       = update->data.new_texture.size;
                VdUiTextureId *id        = update->data.new_texture.write_id;

                VD_UNUSED(buffer_size);

                GLuint texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);

                GLint  level;
                GLint  internal_format;
                GLint  border;
                GLenum format;
                GLenum type;
                vd_ui_gl_cv_texture_format(
                    update->data.new_texture.format,
                    &level,
                    &internal_format, 
                    &border, 
                    &format,
                    &type);

                // GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 4, 4, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
                glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                glTexImage2D(GL_TEXTURE_2D, level, internal_format, width, height, border, format, type, buffer);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glBindTexture(GL_TEXTURE_2D, 0);

                id->id = (uintptr_t)texture;
            } break;

            case VD_UI_UPDATE_TYPE_WRITE_TEXTURE: {
                GLuint texture = (GLuint)update->data.write_texture.texture.id;

                GLint  level;
                GLint  internal_format;
                GLint  border;
                GLenum format;
                GLenum type;
                vd_ui_gl_cv_texture_format(
                    update->data.write_texture.format,
                    &level,
                    &internal_format, 
                    &border, 
                    &format,
                    &type);

                glBindTexture(GL_TEXTURE_2D, texture);
                glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, update->data.write_texture.width, update->data.write_texture.height, format, type, update->data.write_texture.buffer);
                glBindTexture(GL_TEXTURE_2D, 0);
            } break;

            default: break;
        }
    }

    // Get vertex buffer
    size_t buffer_size;
    void *buffer = vd_ui_frame_get_vertex_buffer(&buffer_size);

    // Get render passes
    unsigned int num_passes;
    VdUiRenderPass *passes = vd_ui_frame_get_render_passes(&num_passes);

    int w, h;
    vd_fw_get_size(&w, &h);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_SCISSOR_TEST);
    glBindVertexArray(Vd_Stuff_G.ui_vao);
    glBindBuffer(GL_ARRAY_BUFFER, Vd_Stuff_G.ui_vbo);
    // Loop through render passes
    for (unsigned int i = 0; i < num_passes; ++i) {
        VdUiRenderPass *pass = &passes[i];
        GLuint texture_id = (GLuint)pass->selected_texture->id;
        GLint clip_width   = (GLint)pass->clip[2] - (GLint)pass->clip[0];
        GLint clip_height  = (GLint)pass->clip[3] - (GLint)pass->clip[1];
        GLint clip_x       = (GLint)pass->clip[0];
        GLint clip_lower_y = (GLint)h - (GLint)pass->clip[3];

        glScissor(clip_x, clip_lower_y, clip_width, clip_height);

        glUseProgram(Vd_Stuff_G.ui_program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glUniform2f(glGetUniformLocation(Vd_Stuff_G.ui_program, vd_ui_gl_get_uniform_name_resolution()), (float)w, (float)h);
        glUniform1i(glGetUniformLocation(Vd_Stuff_G.ui_program, vd_ui_gl_get_uniform_name_texture()), 0);
        // glUniform2f(glGetUniformLocation(Vd_Stuff_G.ui_program, vd_ui_gl_get_uniform_name_mouse()), mx, my);

        // Update vertex buffer
        glBufferSubData(GL_ARRAY_BUFFER, 0, pass->instance_count * sizeof(VdUiVertex), (unsigned char*)buffer + pass->first_instance * sizeof(VdUiVertex));

        glDrawArraysInstanced(
            GL_TRIANGLE_STRIP,
            0,
            4,
            pass->instance_count);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDisable(GL_SCISSOR_TEST);
}
#   endif // VD_STUFF_GRAPHICS_VD_UI_INCLUDE

#endif // VD_STUFF_GRAPHICS_INCLUDE

#if VD_STUFF_GRAPHICS_INCLUDE
#   ifndef VD_FW_IMPL
#       define VD_FW_IMPL
#       include "vd_fw.h"
#   endif // !VD_FW_IMPL
#   if VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#       ifndef VD_UM_IMPL
#           define VD_UM_IMPL
#           include "vd_um.h"
#       endif // !VD_UM_IMPL
#   endif // VD_STUFF_GRAPHICS_VD_UM_INCLUDE
#endif // VD_STUFF_GRAPHICS_INCLUDE
#endif // VD_STUFF_IMPL