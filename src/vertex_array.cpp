#include <corgi/opengl/vertex_array.h>
#include <glad/glad.h>

namespace corgi
{
vertex_array::vertex_array(std::vector<vertex_attribute> vertex_attributes,
                           buffer&                       vertex_buffer,
                           buffer&                       index_buffer)
{
    vertex_buffer_ = &vertex_buffer;
    index_buffer_  = &index_buffer;
    vertex_attributes_ = vertex_attributes;

    if(vertex_buffer_->buffer_type() != buffer_type::array_buffer)
        throw std::exception("Wrong buffer type for vertex buffer");

    if(index_buffer_->buffer_type() != buffer_type::element_array_buffer)
        throw std::exception("Wrong buffer type for index buffer");

    glGenVertexArrays(1, &id_);
    bind();
    vertex_buffer_->bind();
    index_buffer_->bind();

    for (auto attribute : vertex_attributes_)
    {
        glEnableVertexAttribArray(attribute.location);

        glVertexAttribPointer(attribute.location, attribute.size, GL_FLOAT, GL_FALSE, 0,
                              (void*)(attribute.offset * sizeof(GL_FLOAT)));
    }

    glBindVertexArray(0);
    for(auto attribute : vertex_attributes_)
    {
        glDisableVertexAttribArray(attribute.location);
    }
}

vertex_array::vertex_array(vertex_array&& other) noexcept {
    id_ = other.id_;
    other.id_ = 0;
}

vertex_array& vertex_array::operator=(vertex_array&& other) noexcept
{
    glDeleteVertexArrays(1,&id_);
    id_ = other.id_;
    other.id_ = 0;
    return *this;
}

vertex_array::~vertex_array()
{
    glDeleteVertexArrays(1, &id_);
}

void     vertex_array::bind() 
{
    glBindVertexArray(id_);
}

unsigned corgi::vertex_array::id() const
{
    return id_;
}
}    // namespace corgi