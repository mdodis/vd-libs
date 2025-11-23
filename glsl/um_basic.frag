#version 330 core

in vec4 f_col;
in vec2 f_texcoord;
in vec2 f_timeout;
flat in uint f_mode;
out vec4 r_col;
in vec4 f_param;


void do_line() {
    vec2 uv = f_texcoord * 2.0 - 1.0;
    float w = 1.0;
    vec2 centers = vec2(0.5, -0.5);
    vec2 distances = 0.7 * abs(vec2(uv.x, uv.x) - centers);
    vec2 widths = vec2(fwidth(distances.x), fwidth(distances.y));
    vec2 alphas = vec2(
        1.0 - smoothstep(w - widths.x, w + widths.x, distances.x),
        1.0 - smoothstep(w - widths.x, w + widths.x, distances.y));

    float alpha = alphas.y * alphas.x;
    // float alpha = mix(alphas.y, 1.0f, alphas.x);

    // Final color (example: white)
    r_col = vec4(f_col.rgb, f_col.a * alpha);
}

float grid(vec2 lineWidth, vec2 texcoord) {
    vec2 uv = texcoord * 1;
    vec2 ddx = dFdx(uv);
    vec2 ddy = dFdy(uv);
    vec2 uvDeriv = vec2(length(vec2(ddx.x, ddy.x)), length(vec2(ddx.y, ddy.y)));
    bvec2 invertLine = bvec2(lineWidth.x > 0.5, lineWidth.y > 0.5);
    vec2 targetWidth = vec2(
      invertLine.x ? 1.0 - lineWidth.x : lineWidth.x,
      invertLine.y ? 1.0 - lineWidth.y : lineWidth.y
      );
    vec2 drawWidth = clamp(targetWidth, uvDeriv, vec2(0.5));
    vec2 lineAA = uvDeriv * 3.0;
    vec2 gridUV = abs(fract(uv) * 2.0 - 1.0);
    gridUV.x = invertLine.x ? gridUV.x : 1.0 - gridUV.x;
    gridUV.y = invertLine.y ? gridUV.y : 1.0 - gridUV.y;
    vec2 grid2 = smoothstep(drawWidth + lineAA, drawWidth - lineAA, gridUV);
    grid2 *= clamp(targetWidth / drawWidth, 0.0, 1.0);
    grid2 = mix(grid2, targetWidth, clamp(uvDeriv * 2.0 - 1.0, 0.0, 1.0));
    grid2.x = invertLine.x ? 1.0 - grid2.x : grid2.x;
    grid2.y = invertLine.y ? 1.0 - grid2.y : grid2.y;
    return mix(grid2.x, 1.0, grid2.y);
}
void do_grid() {
    float grid1 = grid(vec2(0.005), f_texcoord * 1.0);
    float grid2 = grid(vec2(0.005), f_texcoord * 2.0);
    vec3 color1 = vec3(1.0,1.0,1.0);
    vec3 color2 = vec3(0.7,0.7,0.7);
    float a1 = grid1;
    float a2 = grid2 * (1.0 - a1);
    vec3 color = mix(color1, color2, a2);
    float alpha = a1 + a2;
    r_col = vec4(color, alpha);
    if (alpha < 0.01) discard;
}

#define PI 3.14159265359

void do_ring() {
    vec2 uv = f_texcoord * 2.0 - 1.0;
    vec2 p = uv;
    float turn = ((atan(uv.y, uv.x) / PI) + 1.0) / 2.0;
    float cutoff = step(f_param.y, turn);
    float lsq = length(uv);
    float R1 = 1.0;
    float H = f_param.x;
    float R2 = R1 - H;
    float T = lsq;
    float a1 = smoothstep(R2, R2 + 0.001, T);
    float a2 = 1.0 - smoothstep(R1 - 0.001, R1, T);
    vec4 color = f_col;
    color.a *= a1 * a2 * cutoff;
    if (color.a < 0.01) {
        discard;
    }
    r_col = color;
}

void do_color() {
    r_col = f_col;
}

void main()
{
    if (f_mode == 0u) { do_line(); }
    else if (f_mode == 1u) { do_grid(); }
    else if (f_mode == 2u) { do_ring(); }
    else if (f_mode == 3u) { do_color(); }
    r_col.a *= (f_timeout.x / f_timeout.y);
}
