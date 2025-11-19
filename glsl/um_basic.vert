#version 330 core

layout (location = 0) in vec3 v_v0;
layout (location = 1) in float thickness;
layout (location = 2) in vec3 v_v1;
layout (location = 3) in uint mode;
layout (location = 4) in vec4 v_col;
layout (location = 5) in vec4 orientation;
layout (location = 6) in vec2 timeout;

uniform mat4 u_proj;
uniform mat4 u_view;

out vec4 f_col;
flat out uint f_mode;
out vec2 f_texcoord;
out vec2 f_timeout;
out vec4 f_param;

vec3 quat_rotate(vec3 v, vec4 q) {
    return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v);
}

void do_line()
{
    float line_width = thickness;
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
    vec2 uvs[6] = vec2[](
        vec2(1.0, 1.0),   // top-right
        vec2(0.0, 1.0),   // top-left
        vec2(1.0, 0.0),   // bottom-right
        vec2(1.0, 0.0),   // bottom-right
        vec2(0.0, 1.0),   // top-left
        vec2(0.0, 0.0)    // bottom-left
    );
    float L = distance(Pv1.xyz, Pv0.xyz);
    // Take the half point between the view relative points
    // This allows us to handle cases where one point starts at 0,0,0 (i.e. the camera)
    vec3 F = normalize(Pv1.xyz - Pv0.xyz);
    // Find closest point on line Pv0-Pv1 to (0,0,0)
    float t = -dot(Pv0.xyz, F) / dot(F, F);
    vec3 closest = Pv0.xyz + F * t;
    float H = line_width / 2.0;
    vec3 M = -normalize(closest);
    // vec3 M = normalize(mix(Pv0.xyz, Pv1.xyz, 0.5));
    // vec3 M = normalize(Pv0.xyz);
    vec3 Ur = M;
    // What M represents here is a direction vector that we know if aligns with line
    // the line won't be shown anyway.
    // if (abs(dot(F, Ur)) >= 0.9999) {
    //     Ur = vec3(1,0,0);
    // }
    vec3 R = cross(Ur, F);
    vec3 vertex = Pv0.xyz + movements[gl_VertexID].x * H * R + movements[gl_VertexID].y * L * F;
    gl_Position = u_proj * vec4(vertex, 1.0);
    f_col = v_col;
    f_mode = mode;
    f_texcoord = uvs[gl_VertexID];
}

void do_grid() {
    vec3 origin = vec3(v_v0);
    vec3 scale = vec3(thickness);
    vec3 verts[6] = vec3[](
        vec3(+0.5, +0.5, 0.0),
        vec3(-0.5, +0.5, 0.0),
        vec3(+0.5, -0.5, 0.0),
        vec3(+0.5, -0.5, 0.0),
        vec3(-0.5, +0.5, 0.0),
        vec3(-0.5, -0.5, 0.0)
    );
    vec2 uvs[6] = vec2[](
        vec2(1.0, 1.0),   // top-right
        vec2(0.0, 1.0),   // top-left
        vec2(1.0, 0.0),   // bottom-right
        vec2(1.0, 0.0),   // bottom-right
        vec2(0.0, 1.0),   // top-left
        vec2(0.0, 0.0)    // bottom-left
    );
    vec3 position = origin + quat_rotate(verts[gl_VertexID] * scale, orientation);
    vec4 view_pos = u_view * vec4(position, 1.0);
    vec4 clip_pos = u_proj * view_pos;
    gl_Position = clip_pos;
    f_col = v_col;
    vec3 local_x = quat_rotate(vec3(1,0,0), orientation);
    vec3 local_y = quat_rotate(vec3(0,1,0), orientation);
    vec3 rel = position - origin;
    f_texcoord = vec2(dot(rel,local_x),dot(rel, local_y));
    f_mode = mode;
}

vec3 find_orthogonal_vector(vec3 v) {
    vec3 x = vec3(1,0,0);
    if (abs(dot(v, x)) >= 0.9999) {
        x = vec3(0,1,0);
    }
    return normalize(cross(x,v));
}

void do_point() {
    vec4 Pv0 = u_view * vec4(v_v0, 1.0);
    float point_scale = thickness;
    vec2 movements[6] = vec2[](
        vec2(+0.5, +0.5),
        vec2(-0.5, +0.5),
        vec2(+0.5, -0.5),
        vec2(+0.5, -0.5),
        vec2(-0.5, +0.5),
        vec2(-0.5, -0.5)
    );
    float H = point_scale;
    vec3 M = -normalize(Pv0.xyz);
    vec3 A = find_orthogonal_vector(M);
    vec3 R = cross(A, M);
    vec3 F = cross(M, R);
    vec3 vertex = Pv0.xyz + movements[gl_VertexID].x * H * R + movements[gl_VertexID].y * H * F;
    gl_Position = u_proj * vec4(vertex, 1.0);
    f_mode = 3u;
}

void do_ring() {
    vec3 origin = vec3(v_v0);
    vec3 scale = vec3(thickness);
    vec3 verts[6] = vec3[](
        vec3(+0.5, +0.5, 0.0),
        vec3(-0.5, +0.5, 0.0),
        vec3(+0.5, -0.5, 0.0),
        vec3(+0.5, -0.5, 0.0),
        vec3(-0.5, +0.5, 0.0),
        vec3(-0.5, -0.5, 0.0)
    );
    vec2 uvs[6] = vec2[](
        vec2(1.0, 1.0),   // top-right
        vec2(0.0, 1.0),   // top-left
        vec2(1.0, 0.0),   // bottom-right
        vec2(1.0, 0.0),   // bottom-right
        vec2(0.0, 1.0),   // top-left
        vec2(0.0, 0.0)    // bottom-left
    );
    vec3 position = origin + quat_rotate(verts[gl_VertexID] * scale, orientation);
    vec4 view_pos = u_view * vec4(position, 1.0);
    vec4 clip_pos = u_proj * view_pos;
    gl_Position = clip_pos;
    f_col = v_col;
    f_texcoord = uvs[gl_VertexID];
    f_mode = 2u;
    f_param = vec4(v_v1.x,0,0,0);
}

#define CYLINDER_SLICES 8
void do_cylinder() {
    int tri = gl_VertexID / 6;      // which slice
    int vid = gl_VertexID % 6;      // which vertex inside slice
    float h = thickness;
    float radius = v_v1.x;

    float a0 = float(tri)     / float(CYLINDER_SLICES) * 6.28318530718;
    float a1 = float(tri + 1) / float(CYLINDER_SLICES) * 6.28318530718;

    vec3 p0b = vec3(cos(a0) * radius, sin(a0) * radius,  0);
    vec3 p0t = vec3(cos(a0) * radius, sin(a0) * radius,  h);
    vec3 p1b = vec3(cos(a1) * radius, sin(a1) * radius,  0);
    vec3 p1t = vec3(cos(a1) * radius, sin(a1) * radius,  h);

    vec3 local_positions[6] = vec3[](
        p0b,p0t,p1b,
        p1b,p0t,p1t
    );

    vec3 local_pos = local_positions[vid];

    vec3 nlocal = normalize(vec3(local_pos.x, 0.0, local_pos.z));

    vec3 world_pos = quat_rotate(local_pos, orientation); 
    vec3 world_nrm = quat_rotate(nlocal, orientation);

    world_pos += v_v0;
    gl_Position = u_proj * u_view * vec4(world_pos, 1.0);
    f_col = v_col;
    f_mode = 3u;
}

void main() {
    if (mode == 0u) { do_line(); }
    if (mode == 1u) { do_grid(); }
    if (mode == 2u) { do_point(); }
    if (mode == 3u) { do_ring(); }
    if (mode == 4u) { do_cylinder(); }
    f_timeout = timeout;
}
