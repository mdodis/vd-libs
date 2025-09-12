#version 330 core
layout (location = 0) in vec2 a_p0;
layout (location = 1) in vec2 a_p1;
layout (location = 2) in vec2 a_p0u;
layout (location = 3) in vec2 a_p1u;
layout (location = 4) in vec4 a_color0;
layout (location = 5) in vec4 a_color1;
layout (location = 6) in vec4 a_color2;
layout (location = 7) in vec4 a_color3;
layout (location = 8) in float a_amix;
layout (location = 9) in float a_corner_radius;
layout (location = 10) in float a_edge_softness;
layout (location = 11) in float a_border_thickness;
uniform vec2 uResolution;
out vec4 f_color;
out vec2 f_uv;
out float f_amix;
out vec2  f_dp;
out vec2  f_dc;
out vec2  f_dhs;
out float f_corner_radius;
out float f_edge_softness;
out float f_border_thickness;

void main()
{
    vec2 positions[4] = vec2[](
        vec2(-1.0, -1.0),
        vec2(+1.0, -1.0),
        vec2(-1.0, +1.0),
        vec2(+1.0, +1.0)
    );
    vec4 colors[4] = vec4[](a_color0, a_color1, a_color2, a_color3);
    vec2 dhs = (a_p1 - a_p0) / 2.0;
    vec2 dc = (a_p1 + a_p0) / 2.0;
    vec2 dp  = positions[gl_VertexID] * dhs + dc;
    gl_Position = vec4(2.0 * dp.x / uResolution.x - 1.0,
                       1.0 - 2.0 * dp.y / uResolution.y,
                       0.0,
                       1.0);

    vec2 shs = (a_p1u - a_p0u) / 2.0;
    vec2 shc = (a_p1u + a_p0u) / 2.0;
    vec2 shp = positions[gl_VertexID] * shs + shc;
    f_uv = shp;
    f_color = colors[gl_VertexID];
    f_amix = a_amix;
    f_dp = dp;
    f_dc = dc;
    f_dhs = dhs;
    f_corner_radius = a_corner_radius;
    f_edge_softness = a_edge_softness;
    f_border_thickness = a_border_thickness;
}