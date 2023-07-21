#include <corgi/opengl/vertex_array.h>
#include <glad/glad.h>

#include <cassert>

namespace corgi
{
vertex_array::vertex_array(std::vector<vertex_attribute> vertex_attributes,
                           buffer&                       vertex_buffer,
                           buffer&                       index_buffer)
    : vertex_buffer_(&vertex_buffer)
    , index_buffer_(&index_buffer)
    , vertex_attributes_(std::move(vertex_attributes))
{
    assert(vertex_buffer.buffer_type() == buffer_type::array_buffer);
    assert(index_buffer.buffer_type() == buffer_type::element_array_buffer);
    assert(!vertex_attributes_.empty());

    glGenVertexArrays(1, &id_);

    assert(id_ != 0);
    bind();
    vertex_buffer_->bind();
    index_buffer_->bind();

    for(const auto& attribute : vertex_attributes_)
    {
        glEnableVertexAttribArray(attribute.location);

        glVertexAttribPointer(attribute.location, attribute.size, GL_FLOAT,
                              GL_FALSE, 0,
                              (void*)(attribute.offset * sizeof(GL_FLOAT)));
    }

    glBindVertexArray(0);

    for(const auto& attribute : vertex_attributes_)
    {
        glDisableVertexAttribArray(attribute.location);
    }
}

vertex_array::vertex_array(vertex_array&& other) noexcept
{
    id_       = other.id_;
    other.id_ = 0;
}

vertex_array& vertex_array::operator=(vertex_array&& other) noexcept
{
    glDeleteVertexArrays(1, &id_);
    id_       = other.id_;
    other.id_ = 0;
    return *this;
}

const std::vector<vertex_attribute>& vertex_array::vertex_attributes()const
{
    return vertex_attributes_;
}

vertex_array::~vertex_array()
{
    glDeleteVertexArrays(1, &id_);
}

void vertex_array::bind() const
{
    glBindVertexArray(id_);
}

void vertex_array::end()const
{
    glBindVertexArray(0);
}

unsigned corgi::vertex_array::id() const
{
    return id_;
}
}    // namespace corgi