#version 410

uniform float aspect_ratio;

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 in_uv;

out vec2 vertex_position;
out vec2 uv;


void main()
{
    vertex_position = in_position;
    uv = in_uv;

    vec2 ratio_position = vec2(in_position.x / aspect_ratio, in_position.y);

    gl_Position = vec4(ratio_position, 0., 1.);
}