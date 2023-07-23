#pragma once
#include <corgi/opengl/buffer.h>
#include <corgi/opengl/vertex_attribute.h>

#include <vector>

namespace corgi
{

enum class primitive_type : char
{
    triangles,
    quads,
    lines
};

class vertex_array
{
public:
    vertex_array(
        std::vector<vertex_attribute>                        vertex_attributes,
        buffer<float, buffer_type::array_buffer>&            vertex_buffer,
        buffer<unsigned, buffer_type::element_array_buffer>& index_buffer);

    vertex_array(const vertex_array& other) = delete;
    vertex_array(vertex_array&& other) noexcept;

    vertex_array& operator=(const vertex_array& other) = delete;
    vertex_array& operator=(vertex_array&& other) noexcept;

    ~vertex_array();

    const std::vector<vertex_attribute>& vertex_attributes() const;

    void bind() const;
    void end() const;

    unsigned id() const;

private:
    unsigned                                             id_ {0};
    buffer<float, buffer_type::array_buffer>&            vertex_buffer_;
    buffer<unsigned, buffer_type::element_array_buffer>& index_buffer_;
    std::vector<vertex_attribute>                        vertex_attributes_;
};
}    // namespace corgi