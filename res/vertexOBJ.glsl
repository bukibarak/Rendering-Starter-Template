#version 410

uniform mat4 model_matrix;
uniform mat4 view_projection_matrix;

layout(location = 0) in vec3 in_position_OS;
layout(location = 1) in vec3 in_normal_OS;
layout(location = 2) in vec2 in_uv;

out vec3 position_WS;
out vec3 normal_WS;
out vec2 uv;

vec3 ObjectSpaceToWorldSpace(mat4 world, vec3 object)
{
    vec4 tmp = world * vec4(object, 1.);
    return tmp.xyz / tmp.w;
}

vec3 ObjectDirectionToWorldDirection(mat4 world, vec3 direction)
{
    vec4 tmp = world * vec4(direction, 0.);
    return normalize(tmp.xyz);
}

void main()
{
    position_WS = ObjectSpaceToWorldSpace(model_matrix, in_position_OS);
    normal_WS = ObjectDirectionToWorldDirection(inverse(transpose(model_matrix)), in_normal_OS);
    uv = in_uv;
    gl_Position = view_projection_matrix * vec4(in_position_OS, 1.);
}