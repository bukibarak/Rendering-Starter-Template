#include "opengl-framework/opengl-framework.hpp" // Inclue la librairie qui va nous servir à faire du rendu

int main()
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

    auto const rect_shader = gl::Shader{{
        .vertex   = gl::ShaderSource::File{"res/vertex.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragment.glsl"},
    }};

    while (gl::window_is_open())
    {
        // Rendu à chaque frame

        // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
        glClearColor(0.f, 0.f, 1.f, 1.f);

        // Exécute concrètement l'action d'appliquer sur tout l'écran la couleur choisie au-dessus
        glClear(GL_COLOR_BUFFER_BIT);

        // On a besoin qu'un shader soit bind (i.e. "actif") avant de draw(). On en reparle dans la section d'après.
        rect_shader.bind();

        // C'est ce qu'on appelle un "draw call" : on envoie l'instruction à la carte graphique de dessiner notre mesh.
        rectangle_mesh.draw();
    }
}