#pragma once

#include <corgi/opengl/vertex_attribute.h>

#include <string>
#include <vector>

namespace corgi
{

enum class shader_type : unsigned char
{
    vertex,
    fragment
};

// I want to link attributes, vertex and fragment shaders
// Feels like we're in material territory here
struct material
{
    std::vector<vertex_attribute> attributes;
    std::string                   vertex_shader;
    std::string                   fragment_shader;
};

// Maybe I could just have a static function to make the whole thing?
// auto mat = materials::simple_2d();

// I'd like a way to automatically assign a vertex attribute

// Meshes using this shader only stores x,y position

struct shader_content
{
    /**
     * @brief Vertex attributes the shader is compatible with
     */
    std::vector<vertex_attribute> attributes;

    /**
     *  @brief The actual content of the shader
     */
    std::string content;

    /**
     * @brief Type of the shader
     */
    corgi::shader_type shader_type {corgi::shader_type::fragment};
};

namespace common_shaders
{

const inline shader_content simple_2d_vertex_shader {common_attributes::pos2,
                                                     R"(
            #version 330 core
            layout(location = 0) in vec2 position;
            void main() { gl_Position =  vec4(position, 0.0, 1.0); 
        })",
                                                     shader_type::vertex};

const inline shader_content simple_2d_fragment_shader {common_attributes::pos2,
                                                       R"(
            #version 330 core
            out vec4 color;
            void main()
            {
                color	= vec4(1.0, 0.0, 1.0, 1.0);
            }
        )",
                                                       shader_type::fragment};

;
}    // namespace common_shaders

}    // namespace corgi
