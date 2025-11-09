#version 410 core

layout (location = 0) in vec3 v_v0;
layout (location = 1) in vec3 v_v1;
layout (location = 2) in vec4 v_col;

uniform mat4 u_proj;
uniform mat4 u_view;
uniform vec2 u_resolution;
out vec4 f_col;

void main()
{
    // Transform to view space
    // This will allow us to treat 0,0,0 as camera origin
    vec4 Pv0 = u_view * vec4(v_v0, 1.0);
    vec4 Pv1 = u_view * vec4(v_v1, 1.0);

    // For each of the 6 vertices we do:
    // V = V0 + Table{ID}x * hL * R + Table{ID}y * L * F
    // Where:
    // - V:  The final output vertex
    // - V0: The starting vertex
    // - ID: gl_VertexID
    // - hL: half line width in the direction orthogonal to the normalized line direction
    // - R:  The right vector. orthogonal to F
    // - L:  The length of the line in the F direction
    // - F:  The direction of the line
    vec2 movements[6] = vec2[](
        vec2(+1.0, 1.0),
        vec2(-1.0, 1.0),
        vec2(+1.0, 0.0),
        vec2(+1.0, 0.0),
        vec2(-1.0, 1.0),
        vec2(-1.0, 0.0)
    );

    float L = distance(Pv1.xyz, Pv0.xyz);
    // Take the half point between the view relative points
    // This allows us to handle cases where one point starts at 0,0,0 (i.e. the camera)
    vec3 F = normalize(Pv1.xyz - Pv0.xyz);

    vec3 M = normalize(mix(Pv0.xyz, Pv1.xyz, 0.5));
    vec3 Ur = M;
    // What M represents here is a direction vector that we know if aligns with line
    // the line won't be shown anyway. 

    if (abs(dot(F, Ur)) >= 0.998) {
        Ur = vec3(1,0,0);
    }
    vec3 R = cross(Ur, F);
    vec3 U = cross(F, R);
    vec3 vertex = Pv0.xyz + movements[gl_VertexID].x * 0.1 * R + movements[gl_VertexID].y * L * F;
    gl_Position = u_proj * vec4(vertex, 1.0);
    f_col = v_col;
}