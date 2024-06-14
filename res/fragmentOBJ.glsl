#version 410

uniform vec3 light;
uniform float illumination;
uniform sampler2D tex;

in vec3 normals;
in vec2 uv;

out vec4 out_color;


void main()
{
//    vec4 tex_color = texture(tex, uv);
//    out_color = tex_color;
    float light_intensity = -dot(light, normalize(normals));
    float light_with_global_illumination = max(0, light_intensity) + illumination;

    vec4 tex_color = texture(tex, uv);
    vec4 tex_light_color = tex_color * light_with_global_illumination;

    out_color = tex_light_color;
}