#pragma once

#include <string>
#include <vector>
#include <corgi/opengl/vertex_attribute.h>

namespace corgi
{

    // I want to link attributes, vertex and fragment shaders
    // Feels like we're in material territory here
    struct material
    {
        std::vector<vertex_attribute> attributes;
        std::string                   vertex_shader;
        std::string                   fragment_shader;
    };

    // Maybe I could just have a static function to make the whole thing?
    // like 
    auto mat = materials::simple_2d();

// I'd like a way to automatically assign a vertex attribute


// Meshes using this shader only stores x,y position
const inline std::string simplest_2d_vertex_shader =
    R"(
            #version 330 core
            layout(location = 0) in vec2 position;
            void main() { gl_Position =  vec4(position, 0.0, 1.0); 
        })";

const inline std::string simplest_2d_fragment_shader =
    R"(
            #version 330 core
            out vec4 color;
            void main()
            {
                color	= vec4(1.0, 0.0, 1.0, 1.0);
            }
        )";

}    // namespace corgi
