#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu

void main()
{
    // Initialisation
    gl::init("TPs de Rendering"); // On crée une fenêtre et on choisit son nom
    gl::maximize_window(); // On peut la maximiser si on veut

    // auto const triangle_mesh = gl::Mesh{{
    //     .vertex_buffers = {{
    //         .layout = {gl::VertexAttribute::Position2D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
    //         .data   = {
    //             -1.f, -1.f, // Position2D du 1er sommet
    //             +1.f, -1.f, // Position2D du 2ème sommet
    //              0.f, +1.f  // Position2D du 3ème sommet
    //         },
    //     }},
    // }};

    auto const rectangle_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
            .data   = {
                -0.5f, 0.5f, // Top left
                0.5f, 0.5f, // Top right
                0.5f, -0.5f, // Bottom right
                -0.5f, -0.5f, // Bottom left
            },
        }},
        .index_buffer = {
            0, 1, 2,
            0, 2, 3
        }
    }};

    auto const screen_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {gl::VertexAttribute::Position2D{0 /*Index de l'attribut dans le shader, on en reparle juste après*/}},
            .data   = {
                -1.f, 1.f, // Top left
                1.f, 1.f, // Top right
                1.f, -1.f, // Bottom right
                -1.f, -1.f, // Bottom left
            },
        }},
        .index_buffer = {
            0, 1, 2,
            0, 2, 3
        }
    }};

    auto const rect_shader = gl::Shader{{
        .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    auto const screen_shader = gl::Shader{{
            .vertex = gl::ShaderSource::File{"res/vertexScreen.glsl"},
            .fragment = gl::ShaderSource::File{"res/fragmentScreen.glsl"},
    }};

    // Allow alpha
    glEnable(GL_BLEND);

    // On peut configurer l'équation qui mélange deux couleurs, comme pour faire différents blend mode dans Photoshop. Cette équation-ci donne le blending "normal" entre pixels transparents.
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA, GL_ONE);



    while (gl::window_is_open())
    {
        // RENDU A CHAQUE FRAME

        // Clear previous frame buffers
        // glClear(GL_COLOR_BUFFER_BIT);
        // glClear(GL_DEPTH_BUFFER_BIT);
        // glClear(GL_STENCIL_BUFFER_BIT);

        // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        // glClearColor(0.f, 0.f, 1.f, 1.f);

        // Bind shader with screen
        rect_shader.bind();

        // Set vertex shader uniform variables
        rect_shader.set_uniform("aspect_ratio", gl::framebuffer_aspect_ratio());
        rect_shader.set_uniform("time_sec", gl::time_in_seconds());

        // C'est ce qu'on appelle un "draw call" : on envoie l'instruction à la carte graphique de dessiner notre mesh.
        rectangle_mesh.draw();


        // Bind screen shader
        screen_shader.bind();

        // Draw screen shader with very low alpha
        screen_mesh.draw();



    }
}