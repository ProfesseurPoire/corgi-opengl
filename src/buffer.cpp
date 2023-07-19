#include <corgi/opengl/buffer.h>
#include <glad/glad.h>

namespace corgi
{

buffer::buffer(corgi::buffer_type t)
    : buffer_type_(t)
{
    glGenBuffers(1, &id_);
}

buffer::buffer(buffer&& buffer) noexcept
{
    id_          = buffer.id_;
    buffer.id_   = 0;
    buffer_type_ = buffer.buffer_type_;
}

buffer& buffer::operator=(buffer&& buffer)
{
    if(id_ != 0)
        glDeleteBuffers(1, &id_);

    id_          = buffer.id_;
    buffer.id_   = 0;
    buffer_type_ = buffer.buffer_type_;

    return *this;
}

buffer::~buffer()
{
    glDeleteBuffers(1, &id_);
}

unsigned buffer::id() const noexcept
{
    return id_;
}

buffer_type buffer::buffer_type() const noexcept
{
    return buffer_type_;
}

void buffer::set_data(std::vector<float>& data)
{
    bind();

    const auto s = static_cast<GLsizei>(sizeof(float) * data.size());

    switch(buffer_type_)
    {
        case buffer_type::array_buffer:
            glBufferData(GL_ARRAY_BUFFER, s, data.data(), GL_STATIC_DRAW);
            break;

        case buffer_type::element_array_buffer:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, s, data.data(),
                         GL_STATIC_DRAW);
            break;
    }
}

void buffer::set_data(std::vector<unsigned>& data)
{
    bind();

    const auto s = static_cast<GLsizei>(sizeof(unsigned) * data.size());

    switch(buffer_type_)
    {
        case buffer_type::array_buffer:
            glBufferData(GL_ARRAY_BUFFER, s, data.data(), GL_STATIC_DRAW);
            break;

        case buffer_type::element_array_buffer:
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, s, data.data(),
                         GL_STATIC_DRAW);
            break;
    }
}

void buffer::bind()
{
    switch(buffer_type_)
    {
        case buffer_type::array_buffer:
            glBindBuffer(GL_ARRAY_BUFFER, id_);
            break;

        case buffer_type::element_array_buffer:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
            break;
    }
}

}    // namespace corgi
