#include <assert.h>
#include <corgi/opengl/mesh.h>

#include <iostream>

namespace corgi
{
mesh::mesh(std::vector<float>            vertices,
           std::vector<unsigned>         indexes,
           std::vector<vertex_attribute> vertex_attributes,
           primitive_type                primitive_type)
    : vertices_(std::move(vertices))
    , indexes_(std::move(indexes))
    , primitive_(primitive_type)
{
    assert(!vertex_attributes.empty());
    assert(!vertices_.empty());
    assert(!indexes_.empty());

    assert(vertices_.size() % attributes_total_size(vertex_attributes) == 0);

    switch(primitive_)
    {
        case primitive_type::lines:
            assert(indexes_.size() % 2 == 0);
            break;

        case primitive_type::quads:
            assert(indexes_.size() % 4 == 0);
            break;

        case primitive_type::triangles:
            assert(indexes_.size() % 3 == 0);
            break;
    }

    vertex_buffer_.set_data(vertices_);
    index_buffer_.set_data(indexes_);

    vertex_array_ = std::make_unique<corgi::vertex_array>(
        vertex_attributes, vertex_buffer_, index_buffer_);
}

bool mesh::empty() const
{
    return vertex_array_.get() == nullptr;
}

mesh::mesh() {}

void mesh::copy_from(const mesh& other)
{
    vertices_  = other.vertices_;
    indexes_   = other.indexes_;
    primitive_ = other.primitive_;

    vertex_buffer_ = other.vertex_buffer_;
    index_buffer_  = other.index_buffer_;

    vertex_array_ = std::make_unique<corgi::vertex_array>(
        other.vertex_array_->vertex_attributes(), vertex_buffer_,
        index_buffer_);
}

void mesh::move_from(mesh&& other) noexcept
{
    vertices_  = std::move(other.vertices_);
    indexes_   = std::move(other.indexes_);
    primitive_ = other.primitive_;

    vertex_buffer_ = std::move(other.vertex_buffer_);
    index_buffer_  = std::move(other.index_buffer_);

    vertex_array_ = std::make_unique<corgi::vertex_array>(
        other.vertex_array_->vertex_attributes(), vertex_buffer_,
        index_buffer_);

    other.vertex_array_.reset();
}

const vertex_array* mesh::vertex_array() const
{
    return vertex_array_.get();
}

void mesh::reset()
{
    vertex_buffer_.clear();
    index_buffer_.clear();
    vertex_array_.reset();
}

mesh& mesh::operator=(const mesh& other)
{
    reset();

    // We do nothing more but reset the mesh if the
    // other one is empty
    if(other.empty())
        return *this;

    copy_from(other);
    return *this;
}

mesh& mesh::operator=(mesh&& other) noexcept
{
    reset();

    if(other.empty())
        return *this;

    move_from(std::move(other));
    return *this;
}

mesh::mesh(mesh&& other) noexcept
{
    move_from(std::move(other));
}

mesh::mesh(const mesh& other)
{
    copy_from(other);
}

const buffer<unsigned, buffer_type::element_array_buffer>*
mesh::index_buffer() const
{
    return &index_buffer_;
}

const std::vector<unsigned>& mesh::indexes() const
{
    return index_buffer_.data();
}

}    // namespace corgi