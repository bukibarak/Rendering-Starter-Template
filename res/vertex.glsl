#version 410

uniform float aspect_ratio;
uniform float time_sec;

layout(location = 0) in vec2 in_position;

void main()
{
    vec2 ratio_position = vec2(in_position.x / aspect_ratio, in_position.y);

    gl_Position = vec4(ratio_position.x + (0.4 * sin(time_sec)), ratio_position.y + (0.4 * cos(time_sec)), 0., 1.);
}