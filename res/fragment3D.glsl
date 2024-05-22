#version 410



in vec3 vertex_position;
in vec2 uv;

out vec4 out_color;


void main()
{
    out_color = vec4(uv.x, uv.y, 0., 1.);
}