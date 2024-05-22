//
// Created by Dwayne on 03.05.2024.
//

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <opengl-framework/opengl-framework.hpp>

void main() {

    // Window initialization
    gl::init("OpenGL 3D");
    gl::maximize_window();

    // Render target initialization
    auto render_target = gl::RenderTarget{gl::RenderTarget_Descriptor{
        .width          = gl::framebuffer_width_in_pixels(),
        .height         = gl::framebuffer_height_in_pixels(),
        .color_textures = {
            gl::ColorAttachment_Descriptor{
                .format  = gl::InternalFormat_Color::RGBA8,
                .options = {
                    .minification_filter  = gl::Filter::NearestNeighbour, // On va toujours afficher la texture à la taille de l'écran,
                    .magnification_filter = gl::Filter::NearestNeighbour, // donc les filtres n'auront pas d'effet. Tant qu'à faire on choisit le moins coûteux.
                    .wrap_x               = gl::Wrap::ClampToEdge,
                    .wrap_y               = gl::Wrap::ClampToEdge,
                },
            },
        },
        .depth_stencil_texture = gl::DepthStencilAttachment_Descriptor{
            .format  = gl::InternalFormat_DepthStencil::Depth32F,
            .options = {
                .minification_filter  = gl::Filter::NearestNeighbour,
                .magnification_filter = gl::Filter::NearestNeighbour,
                .wrap_x               = gl::Wrap::ClampToEdge,
                .wrap_y               = gl::Wrap::ClampToEdge,
            },
        },
    }};

    // Camera initialization
    auto camera = gl::Camera{};
    gl::set_events_callbacks({
        camera.events_callbacks(),
        {
            .on_mouse_pressed = [&](gl::MousePressedEvent const& e) {
                std::cout << "Mouse pressed at " << e.position.x << " " << e.position.y << '\n';
            },
            .on_framebuffer_resized = [&](gl::FramebufferResizedEvent const& e) {
                render_target.resize(e.width_in_pixels, e.height_in_pixels);
            },
        },
    });

    // auto const rectangle_mesh = gl::Mesh{{
    //     .vertex_buffers = {{
    //         .layout = {
    //             gl::VertexAttribute::Position2D{0},
    //             gl::VertexAttribute::UV{1}
    //         },
    //         .data   = {
    //             -0.5f,  0.5f, 0.f, 0.f, // Top left
    //              0.5f,  0.5f, 0.f, 1.f, // Top right
    //              0.5f, -0.5f, 1.f, 1.f, // Bottom right
    //             -0.5f, -0.5f, 1.f, 0.f, // Bottom left
    //         },
    //     }},
    //     .index_buffer = {
    //         0, 1, 2,
    //         0, 2, 3
    //     }
    // }};



    auto const cube_mesh = gl::Mesh{
        {
            .vertex_buffers = {
                {
                    .layout = {
                        gl::VertexAttribute::Position3D{0},
                        gl::VertexAttribute::UV{1}},
                    .data = {
                        -1.f,  1.f,  1.f, 0.f, 0.f,
                         1.f,  1.f,  1.f, 0.f, 1.f,
                        -1.f, -1.f,  1.f, 1.f, 0.f,
                         1.f, -1.f,  1.f, 1.f, 1.f,
                        -1.f,  1.f, -1.f, 0.f, 0.f,
                         1.f,  1.f, -1.f, 0.f, 1.f,
                        -1.f, -1.f, -1.f, 1.f, 0.f,
                         1.f, -1.f, -1.f, 1.f, 1.f,
                    },
                }},
            .index_buffer = {
                0, 1, 2,
                1, 3, 2,

                4, 5, 6,
                5, 7, 6,

                4, 0, 6,
                0, 2, 6,

                1, 5, 3,
                5, 7, 3,

                4, 5, 1,
                4, 1, 0,

                6, 7, 3,
                6, 3, 2,
            }
        }};

    auto const texture = gl::Texture{
        gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
            .path           = "res/texture.png",
            .flip_y         = true, // Il n'y a pas de convention universelle sur la direction de l'axe Y. Les fichiers (.png, .jpeg) utilisent souvent une direction différente de celle attendue par OpenGL. Ce booléen flip_y est là pour inverser la texture si jamais elle n'apparaît pas dans le bon sens.
            .texture_format = gl::InternalFormat::RGBA8, // Format dans lequel la texture sera stockée. On pourrait par exemple utiliser RGBA16 si on voulait 16 bits par canal de couleur au lieu de 8. (Mais ça ne sert à rien dans notre cas car notre fichier ne contient que 8 bits par canal, donc on ne gagnerait pas de précision). On pourrait aussi stocker en RGB8 si on ne voulait pas de canal alpha. On utilise aussi parfois des textures avec un seul canal (R8) pour des usages spécifiques.
        },
        gl::TextureOptions{
            .minification_filter  = gl::Filter::Linear, // Comment on va moyenner les pixels quand on voit l'image de loin ?
            .magnification_filter = gl::Filter::Linear, // Comment on va interpoler entre les pixels quand on zoom dans l'image ?
            .wrap_x               = gl::Wrap::Repeat,   // Quelle couleur va-t-on lire si jamais on essaye de lire en dehors de la texture ?
            .wrap_y               = gl::Wrap::Repeat,   // Idem, mais sur l'axe Y. En général on met le même wrap mode sur les deux axes.
        }
    };


    auto const screen_shader = gl::Shader{{
        .vertex = gl::ShaderSource::File{"res/vertex3D.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment3D.glsl"},
    }};

    // Only draw CW triangle front face
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glFrontFace(GL_CW);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Update function
    while (gl::window_is_open()) {
        glm::mat4 const view_matrix = camera.view_matrix();

        glm::mat4 const projection_matrix = glm::infinitePerspective(
            glm::radians(90.f),
            gl::framebuffer_aspect_ratio() /*aspect ratio*/,
            0.0001f /*near plane*/
        );
        glm::mat4 const ortho_matrix = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);

        glm::mat4 const translation_matrix = glm::translate(
            glm::mat4{1.f},
            // glm::vec3{0.5f * sin(gl::time_in_seconds()), 0.f, -0.2f} /* déplacement */
            glm::vec3{-2.f, 0.f, -1.f}
        );
        glm::mat4 const rotation_matrix = glm::rotate(
            glm::mat4{1.f},
            // gl::time_in_seconds() /*angle de la rotation*/,
            0.f,
            glm::vec3{0.f, 0.f, 1.f} /* axe autour duquel on tourne */
        );
        glm::mat4 const model_matrix = rotation_matrix * translation_matrix;


        glm::mat4 const view_projection_matrix =  projection_matrix * view_matrix * model_matrix;

        screen_shader.bind();
        screen_shader.set_uniform("view_projection_matrix", view_projection_matrix);
        screen_shader.set_uniform("tex", texture);



        render_target.render([&] {
            // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
            glClearColor(0.f, 0.f, 1.f, 1.f);

            // Clear previous frame buffers
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            cube_mesh.draw();
        });


    }
}
