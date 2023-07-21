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
    assert("Error : vertex_attributes is empty" && !vertex_attributes.empty());
    assert("Error : vertices is empty" && !vertices_.empty());
    assert("Error : indexes vector is empty" && !indexes_.empty());

    assert("Error : vertices count isn't a multiple of vertex_attribute "
           "size" &&
           vertices_.size() % attributes_total_size(vertex_attributes) != 0);

    switch(primitive_)
    {
        case primitive_type::lines:
            assert("Error : indexes count isn't a multiple of 2 (lines)" &&
                   indexes_.size() % 2 != 0);

        case primitive_type::quads:
            assert(indexes_.size() % 4 != 0) throw std::exception(
                "Error : indexes count isn't a multiple of 4 (quads)");

        case primitive_type::triangles:
            assert(indexes_.size() % 3 != 0) throw std::exception(
                "Error : indexes count isn't a "
                "multiple of 3 (triangles)");
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