#include <corgi/opengl/program.h>
#include <glad/glad.h>

#include <iostream>

namespace corgi
{

program::program(shader& vertex_shader, shader& fragment_shader)
{
    if(vertex_shader.vertex_attributes() != fragment_shader.vertex_attributes())
        throw std::exception("Different vertex attribute between shaders");


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

    id_ = glCreateProgram();

    glAttachShader(id_, vertex_shader_->id());
    glAttachShader(id_, fragment_shader_->id());
    glLinkProgram(id_);

}

const std::vector<vertex_attribute>& program::vertex_attributes() const
{
    return vertex_shader_->vertex_attributes();
}

program::program(program&& other) noexcept 
{
    id_ = other.id_;
    vertex_shader_ = other.vertex_shader_;
    fragment_shader_ = other.fragment_shader_;

    other.id_ = 0;
    other.vertex_shader_ = nullptr;
    other.fragment_shader_ = nullptr;
}




program& program::operator=(program&& other) noexcept
{
    glDeleteProgram(id_);

    id_              = other.id_;
    vertex_shader_   = other.vertex_shader_;
    fragment_shader_ = other.fragment_shader_;

    other.id_              = 0;
    other.vertex_shader_   = nullptr;
    other.fragment_shader_ = nullptr;

    return *this;
}

program::~program()
{
    glDeleteProgram(id_);
}

void program::use() 
{
    glUseProgram(id_);
}

void program::end() {
    glUseProgram(0);
}

unsigned program::id() const
{
    return id_;
}

}    // namespace corgi