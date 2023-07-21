#include <corgi/opengl/program.h>
#include <glad/glad.h>

#include <iostream>
#include <cassert>

namespace corgi
{

program::program(shader& vertex_shader, shader& fragment_shader)
{
    // Shaders must share the same vertex attributes
    assert(vertex_shader.vertex_attributes() ==
           fragment_shader.vertex_attributes());

    // Vertex shader must actually be a vertex shader
    assert(vertex_shader.type() == shader_type::vertex);

    // Fragment shader must actually be a fragment shader
    assert(fragment_shader.type() == shader_type::fragment);

    vertex_shader_   = &vertex_shader;
    fragment_shader_ = &fragment_shader;

    id_ = glCreateProgram();

    // id must be different than 0 otherwise the program isn't created
    assert(id_ != 0);

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