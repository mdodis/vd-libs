#version 410 core 

layout (location = 0) in vec3 v_po;
layout (location = 1) in vec3 v_no;

uniform mat4 u_modl;
uniform mat4 u_proj;
uniform mat4 u_view;

out vec3 f_no;

void main()
{
    gl_Position = (u_proj) * (u_view) * u_modl * vec4(v_po, 1.0);
    f_no = v_no;
}