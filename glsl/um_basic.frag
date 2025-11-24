#version 330 core

in vec4 f_col;
in vec2 f_texcoord;
in vec2 f_timeout;
flat in uint f_mode;
out vec4 r_col;
flat in vec4 f_param;


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
    vec2 dd = fwidth(f_texcoord);
    uv = uv / (1.0 - min(dd.x, dd.y));
    float turn = ((atan(uv.y, uv.x) / PI) + 1.0) / 2.0;
    float cutoff = step(f_param.y, turn);
    float lsq = length(uv);
    float R1 = 1.0;
    float H = f_param.x;
    float R2 = R1 - H;
    float T = lsq;
    float P = 0.001;
    float a1 = smoothstep(R2, R2 + P, T);
    float a2 = 1.0 - smoothstep(R1 - P, R1, T);
    vec4 color = f_col;
    color.a *= a1 * a2 * cutoff;
    if (color.a < 0.001) {
        discard;
    }
    r_col = color;
}

float sdTriangle( in vec2 p, in vec2 p0, in vec2 p1, in vec2 p2 )
{
    vec2 e0 = p1-p0, e1 = p2-p1, e2 = p0-p2;
    vec2 v0 = p -p0, v1 = p -p1, v2 = p -p2;
    vec2 pq0 = v0 - e0*clamp( dot(v0,e0)/dot(e0,e0), 0.0, 1.0 );
    vec2 pq1 = v1 - e1*clamp( dot(v1,e1)/dot(e1,e1), 0.0, 1.0 );
    vec2 pq2 = v2 - e2*clamp( dot(v2,e2)/dot(e2,e2), 0.0, 1.0 );
    float s = sign( e0.x*e2.y - e0.y*e2.x );
    vec2 d = min(min(vec2(dot(pq0,pq0), s*(v0.x*e0.y-v0.y*e0.x)),
                     vec2(dot(pq1,pq1), s*(v1.x*e1.y-v1.y*e1.x))),
                     vec2(dot(pq2,pq2), s*(v2.x*e2.y-v2.y*e2.x)));
    return -sqrt(d.x)*sign(d.y);
}

void do_triangle() {
    // vec2 uv = f_texcoord - vec2(0.5, 0.5);
    vec2 uv = f_texcoord;
    float dist = distance(uv, vec2(0.5, 0.3));
    float stepfactor = fwidth(f_texcoord.y);
    // float alpha = 1.0 - smoothstep(0.1 - stepfactor, 0.1, dist);
    vec2 dvx = vec2(dFdx(f_texcoord.x), dFdy(f_texcoord.x));
    vec2 dvy = vec2(dFdx(f_texcoord.y), dFdy(f_texcoord.y));
    vec2 dudv = vec2(length(dvx), length(dvy));
    dudv *= 10.0;
    vec2 uv_dudv = (uv) / (dudv);
    float dudvL = max(uv_dudv.x, uv_dudv.y);
    // r_col = vec4(dudvL , 0, 0, 1.0);

    float yval = 0.00;
    vec2 tri_bot_right = vec2(+1.0, 0.0 + yval);
    vec2 tri_bot_left  = vec2(-1.0, 0.0 + yval);
    vec2 tri_top       = vec2(+0.0, 1.0 + yval);

    float alpha = sdTriangle(uv * 1.1, tri_bot_right, tri_bot_left, tri_top) * 8.0;
    // alpha = smoothstep(alpha, alpha, dudvL);
    // r_col = vec4(alpha, 0.0, 0.0, 1.0);

    r_col = vec4(f_col.xyz, f_col.a * (1.0 - alpha));
}

void do_color() {
    r_col = f_col;
}

float sdf_rounded_rect(vec2 sample_pos, vec2 rect_center, vec2 rect_half_size, float r) {
    vec2 d2 = (abs(rect_center - sample_pos) - rect_half_size + vec2(r, r));
    return min(max(d2.x, d2.y), 0.0) + length(max(d2, 0.0)) - r;
}

void do_rounded_rect()
{
    vec2 uv = (f_texcoord) * f_param.zw;

    // float radius = 0.1;
    // float falloff = 0.003;
    float radius = f_param.x * length(f_param.zw);
    float falloff = f_param.y;
    vec2 softness = vec2(falloff);
    vec2 softness_padding = vec2(max(0, falloff*2 - 1), max(0, falloff*2 - 1));
    float dist = sdf_rounded_rect(uv, vec2(0.0), (f_param.zw * 0.980) * 0.5 - softness_padding, radius);
    float sdf_factor = 1.0 - smoothstep(0, 2*falloff, dist);
    float alpha = sdf_factor;
    // r_col = vec4(alpha, 0,0,1);
    r_col = vec4(f_col.xyz, f_col.a * alpha);
    // float edge = max(abs(dudv.x), abs(dudv.y)) * (sdf_factor);

    // r_col = vec4(edge * 100.0, 0.0, 0.0, 1.0);
}

void main()
{
    if (f_mode == 0u) { do_line(); }
    else if (f_mode == 1u) { do_grid(); }
    else if (f_mode == 2u) { do_ring(); }
    else if (f_mode == 3u) { do_color(); }
    else if (f_mode == 4u) { do_triangle(); }
    else if (f_mode == 5u) { do_rounded_rect(); }
    r_col.a *= (f_timeout.x / f_timeout.y);
}
