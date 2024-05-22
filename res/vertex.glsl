#version 410

uniform float aspect_ratio;
layout(location = 0) in vec2 in_position;

void main()
{
    vec2 ratio_position = vec2(in_position.x / aspect_ratio, in_position.y);

    gl_Position = vec4(ratio_position + 0.4, 0., 1.);
}