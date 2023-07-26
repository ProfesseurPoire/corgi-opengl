#include <corgi/opengl/vertex_array.h>
#include <glad/glad.h>

namespace corgi
{

vertex_array::vertex_array(
    std::vector<vertex_attribute>                        vertex_attributes,
    buffer<float, buffer_type::array_buffer>&            vertex_buffer,
    buffer<unsigned, buffer_type::element_array_buffer>& index_buffer)
    : vertex_buffer_(&vertex_buffer)
    , index_buffer_(&index_buffer)
    , vertex_attributes_(std::move(vertex_attributes))
{
    push_data();
}

vertex_array::vertex_array(const vertex_array& other)
    : vertex_buffer_(other.vertex_buffer_)
    , index_buffer_(other.index_buffer_)
    , vertex_attributes_(other.vertex_attributes_)
{
    push_data();
}

vertex_array& vertex_array::operator=(vertex_array&& other) noexcept
{
    clear();

    id_                = other.id_;
    vertex_buffer_     = other.vertex_buffer_;
    index_buffer_      = other.index_buffer_;
    vertex_attributes_ = other.vertex_attributes_;

    other.clear();
    return *this;
}

vertex_array& vertex_array::operator=(const vertex_array& other)
{
    clear();

    vertex_buffer_     = other.vertex_buffer_;
    index_buffer_      = other.index_buffer_;
    vertex_attributes_ = other.vertex_attributes_;

    push_data();
    return *this;
}

void vertex_array::set(
    buffer<float, buffer_type::array_buffer>&           vertex_buffer,
    buffer<unsigned, buffer_type::element_array_buffer> index_buffer,
    std::vector<vertex_attribute>                       attributes)
{
    // I'm not sure how glBindVertexArray works so for now
    // I'll go with "delete and recreate another id" if needed

    clear();

    vertex_buffer_     = &vertex_buffer;
    index_buffer_      = &index_buffer;
    vertex_attributes_ = std::move(attributes);

    push_data();
}

vertex_array::vertex_array(vertex_array&& other) noexcept
    : id_(other.id_)
    , vertex_buffer_(other.vertex_buffer_)
    , index_buffer_(other.index_buffer_)
    , vertex_attributes_(std::move(other.vertex_attributes_))
{
    other.clear();
}

const std::vector<vertex_attribute>& vertex_array::vertex_attributes() const
{
    return vertex_attributes_;
}

vertex_array::~vertex_array()
{
    clear();
}

void vertex_array::bind() const
{
    if(id_ == 0)
        throw std::logic_error(
            "vertex_array::bind : Can't bind a VAO with 0 as id");
    glBindVertexArray(id_);
}

void vertex_array::clear()
{
    glDeleteVertexArrays(1, &id_);
    vertex_buffer_ = nullptr;
    index_buffer_  = nullptr;
    vertex_attributes_.clear();
    id_ = 0;
}

void vertex_array::end() const
{
    glBindVertexArray(0);
}

unsigned corgi::vertex_array::id() const
{
    return id_;
}

void vertex_array::push_data()
{
    if(vertex_buffer_->empty())
        throw std::invalid_argument(
            "vertex_array::set : vertex_buffer in empty state");

    if(index_buffer_->empty())
        throw std::invalid_argument(
            "vertex_array::set : index_buffer in empty state");

    if(vertex_attributes_.empty())
        throw std::invalid_argument(
            "vertex_array::set : attributes vector is empty");

    glGenVertexArrays(1, &id_);

    if(id_ == 0)
        throw std::logic_error(
            "vertex_array::set : generated vertex_array id equals 0");
    bind();
    vertex_buffer_->bind();
    index_buffer_->bind();

    for(const auto& attribute : vertex_attributes_)
    {
        glEnableVertexAttribArray(attribute.location);

        glVertexAttribPointer(
            attribute.location, attribute.size, GL_FLOAT, GL_FALSE,
            attributes_total_size(vertex_attributes_) * sizeof(float),
            (void*)(attribute.offset * sizeof(GL_FLOAT)));
    }

    glBindVertexArray(0);

    for(const auto& attribute : vertex_attributes_)
    {
        glDisableVertexAttribArray(attribute.location);
    }
}
}    // namespace corgi