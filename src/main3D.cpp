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

    // Camera initialization
    auto camera = gl::Camera{};
    gl::set_events_callbacks({
        camera.events_callbacks(),
        {
            .on_mouse_pressed = [&](gl::MousePressedEvent const& e) {
                std::cout << "Mouse pressed at " << e.position.x << " " << e.position.y << '\n';
            },
        },
    });

    // auto const rectangle_mesh = gl::Mesh{{
    //     .vertex_buffers = {{
    //         .layout = {gl::VertexAttribute::Position2D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
    //         .data   = {
    //             -0.5f, 0.5f, // Top left
    //             0.5f, 0.5f, // Top right
    //             0.5f, -0.5f, // Bottom right
    //             -0.5f, -0.5f, // Bottom left
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
                    .layout = {gl::VertexAttribute::Position3D{0}},
                    .data = {
                        -1.f,  1.f,  1.f,
                         1.f,  1.f,  1.f,
                        -1.f, -1.f,  1.f,
                         1.f, -1.f,  1.f,
                        -1.f,  1.f, -1.f,
                         1.f,  1.f, -1.f,
                        -1.f, -1.f, -1.f,
                         1.f, -1.f, -1.f,
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
            0.000001f /*near plane*/
        );
        glm::mat4 const ortho_matrix = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);

        glm::mat4 const translation_matrix = glm::translate(
            glm::mat4{1.f},
            //glm::vec3{0.5f * sin(gl::time_in_seconds()), 0.f, -0.2f} /* déplacement */
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


        // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        glClearColor(0.f, 0.f, 1.f, 1.f);

        // Clear previous frame buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        screen_shader.bind();
        screen_shader.set_uniform("view_projection_matrix", view_projection_matrix);
        cube_mesh.draw();

    }
}
