#version 410

uniform sampler2D tex;

in vec3 normals;
in vec2 uv;

out vec4 out_color;


void main()
{
//    vec4 tex_color = texture(tex, uv);
//    out_color = tex_color;

    vec4 normals_color = vec4(normals, 1.);
    out_color = normals_color;
}