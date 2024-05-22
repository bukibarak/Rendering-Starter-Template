//
// Created by Dwayne on 22.05.2024.
//

#include <opengl-framework/opengl-framework.hpp>

void main() {

    // Window initialization
    gl::init("UV");
    gl::maximize_window();


    auto const rectangle_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {
                gl::VertexAttribute::Position2D{0},
                gl::VertexAttribute::UV{1}
            },
            .data   = {
                -0.5f,  0.5f, 0.f, 1.f, // Top left
                 0.5f,  0.5f, 1.f, 1.f, // Top right
                 0.5f, -0.5f, 1.f, 0.f, // Bottom right
                -0.5f, -0.5f, 0.f, 0.f, // Bottom left
            },
        }},
        .index_buffer = {
            0, 1, 2,
            0, 2, 3
        }
    }};

    // auto const texture = gl::Texture{
    //     gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
    //         .path           = "res/texture.png",
    //         .flip_y         = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
    //         .texture_format = gl::InternalFormat::RGBA8, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
    //     },
    //     gl::TextureOptions{
    //         .minification_filter  = gl::Filter::Linear, // Comment on va moyenner les pixels quand on voit l'image de loin ?
    //         .magnification_filter = gl::Filter::Linear, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
    //         .wrap_x               = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
    //         .wrap_y               = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
    //     }
    // };

    auto const uv_shaders = gl::Shader{{
        .vertex = gl::ShaderSource::File{"res/vertexUV.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragmentUV.glsl"},
    }};

    // Only draw CW triangle front face
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glFrontFace(GL_CW);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Update function
    while (gl::window_is_open()) {

        // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        glClearColor(0.f, 0.f, 1.f, 1.f);

        // Clear previous frame buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        uv_shaders.bind();
        uv_shaders.set_uniform("aspect_ratio", gl::framebuffer_aspect_ratio());
        rectangle_mesh.draw();

    }
}
