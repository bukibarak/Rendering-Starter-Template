#version 410

uniform vec3 global_light_WS;
uniform float global_light_intensity;
uniform vec3 point_light_WS;
uniform float point_light_intensity;
uniform float global_illumination;
uniform sampler2D tex;

in vec3 position_WS;
in vec3 normal_WS;
in vec2 uv;

out vec4 out_color;


void main()
{
//    vec4 tex_color = texture(tex, uv);
//    out_color = tex_color;

    vec3 normalized_normal = normalize(normal_WS);

    // Calculate global light intensity
    float global_intensity = max(0, -dot(global_light_WS, normalized_normal)); // global light is normalized in cpp
    global_intensity *= global_light_intensity;

    // Calculate point light intensity
    vec3 point_light_vector = position_WS - point_light_WS;
    float point_light_length = length(point_light_vector);
    float point_intensity = max(0, -dot(normalize(position_WS - point_light_WS), normalized_normal)) / pow(point_light_length, 2);
    point_intensity *= point_light_intensity;

    float light_intensity = global_intensity + point_intensity;
    float light_with_global_illumination = light_intensity + global_illumination;

    vec4 tex_color = texture(tex, uv);
    vec4 tex_light_color =  vec4( vec3(tex_color) * light_with_global_illumination, 1.);

    // out_color = (vec4(point_intensity, point_intensity, point_intensity, 1.));
    out_color = tex_light_color;
}