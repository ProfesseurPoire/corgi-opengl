#pragma once
#include <corgi/opengl/vertex_array.h>

#include <memory>
#include <vector>

namespace corgi
{
class mesh
{
    mesh(std::vector<float>            vertices,
         std::vector<unsigned>         indexes,
         std::vector<vertex_attribute> vertex_attributes,
         primitive_type primitive_type = primitive_type::triangles);

    std::unique_ptr<vertex_array> vertex_array_;

    std::unique_ptr<buffer> vertex_buffer_;
    std::unique_ptr<buffer> index_buffer_;

    std::vector<float>    vertices_;
    std::vector<unsigned> indexes_;
    primitive_type        primitive_;
};
}    // namespace corgi