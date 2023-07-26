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
            #version 430 core
            layout(location = 0) in vec2 position;
            void main() { gl_Position =  vec4(position, 0.0, 1.0); 
        })",
                                                     shader_type::vertex};

const inline shader_content simple_2d_fragment_shader {common_attributes::pos2,
                                                       R"(
            #version 430 core
            out vec4 color;

            layout(std140, binding = 2) uniform ubo
            {
 	        vec4 main_color;
            };

            void main()
            {
                color	= main_color;
            }
        )",
                                                       shader_type::fragment};

const inline shader_content simple_2d_texture_vertex_shader {
    common_attributes::pos2_uv,
    R"(
#version 430 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uv;

layout(location = 1) out vec2 out_uv;

void main() {
    gl_Position     =  vec4(position,0.0, 1.0);
    out_uv    = uv;
})",
    shader_type::vertex};

const inline shader_content simple_2d_texture_fragment_shader {
    common_attributes::pos2_uv, R"(

#version 430 core

layout(location = 1) in vec2 uv;

layout(location = 0) out vec4 out_color;
layout(binding = 0) uniform sampler2D texture_sampler;

void main() 
{
    out_color = texture(texture_sampler, uv);
}

)",
    shader_type::fragment

}

;
}    // namespace common_shaders

}    // namespace corgi
