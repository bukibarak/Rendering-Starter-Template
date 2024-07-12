//
// Created by Dwayne on 03.05.2024.
//

#include <iostream>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <opengl-framework/opengl-framework.hpp>

struct PointLight {

    glm::vec3 Position;
    float Intensity;

    PointLight() : Position(0, 0, 0), Intensity(100.0f) {}
    explicit PointLight(const glm::vec3& in_Position): Position(in_Position), Intensity(100.0f) {}
    explicit PointLight(const glm::vec3& in_Position, const float &in_Intensity) : Position(in_Position), Intensity(in_Intensity) {}
};

int main() {

    // Window initialization
    gl::init("OpenGL OBJ");
    gl::maximize_window();
    std::string obj_path = gl::make_absolute_path("res/obj/boat/fourareen.obj").string();

    tinyobj::ObjReader reader;

    if (!reader.ParseFromFile(obj_path)) {
        if (!reader.Error().empty()) {
            std::cerr << "TinyObjReader: " << reader.Error();
        }
        exit(1);
    }

    if (!reader.Warning().empty()) {
        std::cout << "TinyObjReader: " << reader.Warning();
    }

    auto& attrib = reader.GetAttrib();
    auto& shapes = reader.GetShapes();

    std::vector<float> data{};

    // Loop over shapes
    for (unsigned int s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        unsigned int index_offset = 0;
        for (unsigned int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            unsigned int fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (unsigned int v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                data.push_back(attrib.vertices[3*int(idx.vertex_index)+0]);                         // VERTEX X

                data.push_back(attrib.vertices[3*int(idx.vertex_index)+2]);                         // VERTEX Z
                data.push_back(attrib.vertices[3*int(idx.vertex_index)+1]);                         // VERTEX Y

                // Check if `normal_index` is zero or positive. negative = no normal data
                if (idx.normal_index >= 0) {
                    glm::vec3 normals_normalized = glm::normalize(glm::vec3{
                        attrib.normals[3*int(idx.normal_index)+0],                                  // NORMAL X
                        attrib.normals[3*int(idx.normal_index)+2],                                  // NORMAL Z
                        attrib.normals[3*int(idx.normal_index)+2]                                   // NORMAL Y
                    });
                    data.push_back(normals_normalized.x);
                    data.push_back(normals_normalized.y);
                    data.push_back(normals_normalized.z);
                }

                // Check if `texcoord_index` is zero or positive. negative = no texcoord data
                if (idx.texcoord_index >= 0) {
                    data.push_back(attrib.texcoords[2*int(idx.texcoord_index)+0]);                  // UV X
                    data.push_back(attrib.texcoords[2*int(idx.texcoord_index)+1]);                  // UV Y
                }

                // Optional: vertex colors
                // data.push_back(attrib.colors[3*int(idx.vertex_index)+0]);                           // COLOR R
                // data.push_back(attrib.colors[3*int(idx.vertex_index)+1]);                           // COLOR G
                // data.push_back(attrib.colors[3*int(idx.vertex_index)+2]);                           // COLOR B


            }
            index_offset += fv;

        }
    }

    auto const boat_mesh = gl::Mesh{
            {
                .vertex_buffers = {
                    {
                        .layout = {
                            gl::VertexAttribute::Position3D{0},
                            gl::VertexAttribute::Normal3D{1},
                            gl::VertexAttribute::UV{2}},
                            // gl::VertexAttribute::ColorRGB{3}},
                        .data = data,
                    }}
            }};



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

    auto const texture = gl::Texture{
        gl::TextureSource::File{ // Peut être un fichier, ou directement un tableau de pixels
            .path           = "res/obj/boat/fourareen2K_albedo.jpg",
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
    

    auto const screen_mesh = gl::Mesh{{
        .vertex_buffers = {{
            .layout = {
                gl::VertexAttribute::Position2D{0 },
                gl::VertexAttribute::UV{1 }
            },
            .data   = {
                -1.f, 1.f, 0.f, 1.f, // Top left
                1.f, 1.f, 1.f, 1.f, // Top right
                1.f, -1.f, 1.f, 0.f, // Bottom right
                -1.f, -1.f, 0.f, 0.f, // Bottom left
            },
        }},
        .index_buffer = {
            0, 1, 2,
            0, 2, 3
        }
    }};


    auto const obj_shader = gl::Shader{{
        .vertex = gl::ShaderSource::File{"res/vertexOBJ.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragmentOBJ.glsl"},
    }};

    auto const screen_shader = gl::Shader{{
        .vertex = gl::ShaderSource::File{"res/vertexScreen3D.glsl"},
        .fragment = gl::ShaderSource::File{"res/fragmentScreen3D.glsl"},
    }};

    // Only draw CW triangle front face
    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_FRONT);
    // glFrontFace(GL_CW);

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    PointLight point_light;
    point_light.Intensity = 10.0f;

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
            glm::vec3{0.f, 0.f, 0.f}
        );
        glm::mat4 const rotation_matrix = glm::rotate(
            glm::mat4{1.f},
            gl::time_in_seconds() /*angle de la rotation*/,
            // 0.f,
            glm::vec3{1.f, 0.f, 0.f} /* axe autour duquel on tourne */
        );
        glm::mat4 const model_matrix = rotation_matrix * translation_matrix;


        glm::mat4 const view_projection_matrix =  projection_matrix * view_matrix * model_matrix;





        render_target.render([&] {
            // point_light.Position = glm::vec3{cos(gl::time_in_seconds()), sin(gl::time_in_seconds()), 0.f};
            point_light.Position = glm::vec3{1.0f, -2.5f, -0.25f};
            // point_light.Position *= 1000.0f;


            obj_shader.bind();
            obj_shader.set_uniform("view_projection_matrix", view_projection_matrix);
            obj_shader.set_uniform("model_matrix", model_matrix);
            obj_shader.set_uniform("tex", texture);
            obj_shader.set_uniform("global_light_WS", glm::normalize(glm::vec3{0.5, -1., -1.}));
            obj_shader.set_uniform("global_light_intensity", 0.3f);
            // obj_shader.set_uniform("global_light", glm::normalize(glm::vec3{0., 0., 0.}));
            obj_shader.set_uniform("point_light_WS", point_light.Position);
            obj_shader.set_uniform("point_light_intensity", point_light.Intensity);
            obj_shader.set_uniform("global_illumination", 0.015f);

            // Choisis la couleur à utiliser. Les paramètres sont R, G, B, A avec des valeurs qui vont de 0 à 1
            glClearColor(0.f, 0.f, 1.f, 1.f);

            // Clear previous frame buffers
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            boat_mesh.draw();
        });

        // Clear previous frame buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        screen_shader.bind();
        screen_shader.set_uniform("tex", render_target.color_texture(0));
        screen_mesh.draw();

    }
}
