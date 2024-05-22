#version 410

uniform mat4 view_projection_matrix;

layout(location = 0) in vec3 in_position;

out vec3 vertex_position;


void main()
{
    vertex_position = in_position;

    gl_Position = view_projection_matrix * vec4(in_position, 1.);
}