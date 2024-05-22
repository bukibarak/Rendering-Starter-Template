#version 410

uniform sampler2D tex;

in vec2 vertex_position;
in vec2 uv;

out vec4 out_color;


void main()
{

    vec2 damier = step(0.125, mod(uv, 0.25));

    float col = abs(damier.x - damier.y);

    out_color = vec4(col, col, col, 1.);
}