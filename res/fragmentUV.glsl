#version 410

uniform sampler2D tex;

in vec2 vertex_position;
in vec2 uv;

out vec4 out_color;


void main()
{
    vec4 texture_color = texture(tex, uv);
    out_color = texture_color;
}