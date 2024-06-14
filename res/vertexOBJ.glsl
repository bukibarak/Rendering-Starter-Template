#version 410

uniform mat4 view_projection_matrix;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normals;
layout(location = 2) in vec2 in_uv;

out vec3 normals;
out vec2 uv;

void main()
{
    normals = in_normals;
    uv = in_uv;
    gl_Position = view_projection_matrix * vec4(in_position, 1.);
}