#pragma once
#include <corgi/opengl/vertex_array.h>

#include <memory>
#include <vector>

namespace corgi
{
class mesh
{
public:

    mesh(std::vector<float>            vertices,
         std::vector<unsigned>         indexes,
         std::vector<vertex_attribute> vertex_attributes,
         primitive_type primitive_type = primitive_type::triangles);

    mesh();

    mesh(const mesh& other);
    mesh(mesh&& other) noexcept;

    mesh& operator=(const mesh& other);
    mesh& operator=(mesh&& other)noexcept;

    /**
     * @brief Returns a points to the vertex array object
     */
    const vertex_array* vertex_array() const;

    /**
     * @brief Returns the buffer that contains the mesh's indexes
     */
    const buffer* index_buffer() const;

    const std::vector<unsigned>& indexes() const;

    /**
     * @brief Returns true if the mesh holds no usable data
     * Happens if the mesh is empty constructed or moved
     */
    bool empty() const;

private:

    void copy_from(const mesh& other);
    void move_from(mesh&& other)noexcept;
    void reset();
    
    std::unique_ptr<buffer>       vertex_buffer_;
    std::unique_ptr<buffer>       index_buffer_;
    std::unique_ptr<corgi::vertex_array> vertex_array_;

    std::vector<float>    vertices_;
    std::vector<unsigned> indexes_;
    primitive_type        primitive_;
};
}    // namespace corgi