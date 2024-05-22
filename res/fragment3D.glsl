#version 410

in vec3 vertex_position;

out vec4 out_color;


void main()
{
    out_color = vec4(vertex_position, 1.);
}