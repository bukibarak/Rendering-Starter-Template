#version 410

uniform sampler2D tex;

in vec3 vertex_position;
in vec2 uv;
out vec4 out_color;


void main()
{
    vec4 tex_color = texture(tex, uv);
    // out_color = vec4(vertex_position, 1.);
    // out_color = vec4(uv.x, uv.y, 0., 1.);
    out_color = tex_color;
}