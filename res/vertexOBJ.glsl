#version 410

uniform mat4 view_projection_matrix;

layout(location = 0) in vec3 in_position;
layout(location = 2) in vec2 in_uv;

out vec2 uv;

void main()
{
    uv = in_uv;
    gl_Position = view_projection_matrix * vec4(in_position, 1.);
}