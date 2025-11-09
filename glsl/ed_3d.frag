#version 410 core 

in vec3 f_no;

out vec4 r_co;

void main()
{
    r_co = vec4(f_no, 1.0);
}
