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

float radialGlow(vec2 fragPos, vec2 glowCenter, float radius, float intensity) {
    float dist = length(fragPos - glowCenter);
    return intensity * exp(-dist * dist / (radius * radius));
}

void main()
{
    // === Tunable parameters ===
    float softnessScale       = 2.0;    // Multiplier for edge softness padding
    float glowInnerFactor     = 0.2;    // Inner glow radius relative to rect size
    float glowOuterFactor     = 0.3;    // Outer glow radius relative to rect size
    float glowInnerIntensity  = 0.4;    // Strength of inner glow
    float glowOuterIntensity  = 0.2;    // Strength of outer glow
    float edgeEnhanceScale    = 0.1;    // Scale for edge distance normalization
    float edgeEnhanceBoost    = 0.5;    // Boost factor for edge proximity effect
    float glowColorBlendScale = 0.3;    // How much warm/cool glow mix varies
    float glowStrength        = 0.1;    // Overall glow strength multiplier
    float colorTempShift      = 0.2;    // Subtle color temperature mix factor
    float brightnessBoost     = 0.1;    // Brightness boost based on glow intensity

    vec3 coolGlow = vec3(0.9, 0.95, 1.0);
    vec3 warmGlow = vec3(1.0, 0.9, 0.8);

    // === Rounded rect SDF with edge softness ===
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

    // === Mouse inside check ===
    vec2 mouseLocal = uMouse - f_dc;
    float insideX = step(abs(mouseLocal.x), f_dhs.x);
    float insideY = step(abs(mouseLocal.y), f_dhs.y);
    float mouseInside = insideX * insideY;

    // === Glow calculations ===
    float glowRadius1 = length(f_dhs) * glowInnerFactor;
    float glowRadius2 = length(f_dhs) * glowOuterFactor;
    float glow1 = radialGlow(f_dp, uMouse, glowRadius1, glowInnerIntensity);
    float glow2 = radialGlow(f_dp, uMouse, glowRadius2, glowOuterIntensity);
    float combinedGlow = glow1 + glow2;

    float mouseDistance = length(f_dp - uMouse);
    float maxDistance = length(f_dhs);
    float proximityFactor = 1.0 - clamp(mouseDistance / maxDistance, 0.0, 1.0);

    float edgeDistance = sdf_rounded_rect(f_dp, f_dc, f_dhs, f_corner_radius);
    float edgeFactor = 1.0 - clamp(abs(edgeDistance) / (length(f_dhs) * edgeEnhanceScale), 0.0, 1.0);

    float finalGlowIntensity = combinedGlow * proximityFactor * (1.0 + edgeFactor * edgeEnhanceBoost) * mouseInside;

    float colorMix = sin(atan(mouseLocal.y, mouseLocal.x) * 2.0) * 0.5 + 0.5;
    vec3 finalGlowColor = mix(coolGlow, warmGlow, colorMix * glowColorBlendScale);

    // Apply glow and color effects
    color.rgb += finalGlowColor * finalGlowIntensity * glowStrength;
    color.rgb = mix(color.rgb, color.rgb * vec3(1.05, 1.0, 0.95), finalGlowIntensity * colorTempShift);
    color.rgb *= (1.0 + finalGlowIntensity * brightnessBoost);

    FragColor = color * sdf * border_factor;
}