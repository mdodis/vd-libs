#version 330 core
in vec4 f_color;
in vec2 f_uv;
in float f_amix;
in vec2  f_dp;
in vec2  f_dc;
in vec2  f_dhs;
in float f_corner_radius;
in float f_edge_softness;
in float f_border_thickness;

uniform sampler2D uTexture;
uniform vec2 uResolution;
uniform vec2 uMouse;

out vec4 FragColor;

float sdf_rounded_rect(vec2 sample_pos, vec2 rect_center, vec2 rect_half_size, float r) {
    vec2 d2 = (abs(rect_center - sample_pos) - rect_half_size + vec2(r, r));
    return min(max(d2.x, d2.y), 0.0) + length(max(d2, 0.0)) - r;
}

float smoothFalloff(float distance, float radius, float softness) {
    return 1.0 - smoothstep(radius - softness, radius + softness, distance);
}

void main()
{
    vec2 softness_padding = vec2(max(0, f_edge_softness * 2 - 1.0));
    float dist = sdf_rounded_rect(f_dp, f_dc, f_dhs - softness_padding, f_corner_radius);
    float border_factor = 1.0;
    if (f_border_thickness != 0.0) {
        vec2 interior_hs = f_dhs - vec2(f_border_thickness);

        float interior_radius_reduce_f = min(interior_hs.x / f_dhs.x, interior_hs.y / f_dhs.y);
        float interior_corner_radius = f_corner_radius * interior_radius_reduce_f * interior_radius_reduce_f;

        float inside_d = sdf_rounded_rect(f_dp, f_dc, interior_hs - softness_padding, interior_corner_radius);

        float inside_f = smoothstep(0, 2 * f_edge_softness, inside_d);
        border_factor = inside_f;
    }
    float sdf = 1.0 - smoothstep(0.0, 2.0 * f_edge_softness, dist);


    // === Base color sampling and mask ===
    vec4 sample = texture(uTexture, f_uv);
    vec4 normal_color = sample * f_color;
    vec4 mask_color = vec4(f_color.rgb, f_color.a * sample.r);
    vec4 color = mix(normal_color, mask_color, f_amix);

    FragColor = color * sdf * border_factor;
}