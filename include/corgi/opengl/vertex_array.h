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
    /**
     * \brief Creates an empty vertex_array. In empty state some operations are invalid
     */
    vertex_array() = default;

    vertex_array(
        std::vector<vertex_attribute>                        vertex_attributes,
        buffer<float, buffer_type::array_buffer>&            vertex_buffer,
        buffer<unsigned, buffer_type::element_array_buffer>& index_buffer);

    vertex_array(const vertex_array& other);
    vertex_array(vertex_array&& other) noexcept;

    vertex_array& operator=(const vertex_array& other);
    vertex_array& operator=(vertex_array&& other) noexcept;

    ~vertex_array();

    void set(buffer<float, buffer_type::array_buffer>&           vertex_buffer,
             buffer<unsigned, buffer_type::element_array_buffer> index_buffer,
             std::vector<vertex_attribute>                       attributes);

    const std::vector<vertex_attribute>& vertex_attributes() const;

    void bind() const;
    void end() const;

    /**
     * \brief Destroy the current vertex array and put it in an empty state
     */
    void clear();

    unsigned id() const;

private:
    void push_data();

    unsigned                                  id_ {0};
    buffer<float, buffer_type::array_buffer>* vertex_buffer_ {nullptr};
    buffer<unsigned, buffer_type::element_array_buffer>* index_buffer_ {
        nullptr};
    std::vector<vertex_attribute> vertex_attributes_;
};
}    // namespace corgi