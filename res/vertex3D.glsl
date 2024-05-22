#version 410

uniform mat4 view_projection_matrix;

layout(location = 0) in vec2 in_position;

void main()
{
    gl_Position = view_projection_matrix * vec4(in_position, 0., 1.);
}