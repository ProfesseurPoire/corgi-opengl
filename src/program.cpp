#include <corgi/opengl/program.h>
#include <glad/glad.h>

#include <iostream>

namespace corgi
{

program::program(shader& vertex_shader, shader& fragment_shader)
{
    if(vertex_shader.type() != shader_type::vertex)
    {
        std::cerr << "Given vertex shader isn't actually a vertex shader"
                  << std::endl;

        return;
    }

    if(fragment_shader.type() != shader_type::fragment)
    {
        std::cerr << "Given fragment shader isn't actually a fragment shader"
                  << std::endl;
        return;
    }

    vertex_shader_   = &vertex_shader;
    fragment_shader_ = &fragment_shader;
}

program::~program()
{
    glDeleteProgram(id_);
}

}    // namespace corgi