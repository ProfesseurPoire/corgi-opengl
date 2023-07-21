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

    assert(vertices_.size() % attributes_total_size(vertex_attributes) != 0);

    switch(primitive_)
    {
        case primitive_type::lines:
            assert(indexes_.size() % 2 != 0);
            break;

        case primitive_type::quads:
            assert(indexes_.size() % 4 != 0);
            break;

        case primitive_type::triangles:
            assert(indexes_.size() % 3 != 0);
            break;
    }

    vertex_buffer_ = std::make_unique<buffer>(buffer_type::array_buffer);
    vertex_buffer_->bind();
    vertex_buffer_->set_data(vertices_);
    vertex_buffer_->end();

    index_buffer_ = std::make_unique<buffer>(buffer_type::element_array_buffer);
    index_buffer_->bind();
    index_buffer_->set_data(indexes);
    index_buffer_->end();

    vertex_array_ = std::make_unique<vertex_array>(
        vertex_attributes, *vertex_buffer_, *index_buffer_);
}

}    // namespace corgi