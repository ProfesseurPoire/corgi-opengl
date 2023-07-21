#pragma once
#include <numeric>
#include <vector>

namespace corgi
{
/**
 * @brief Tells the GPU where to look for 1 attribute in the vertex
 */
struct vertex_attribute
{
    int location {0};
    int offset {0};
    int size {0};

    bool operator==(const vertex_attribute& other) const
    {
        return location == other.location && offset == other.offset &&
               size == other.size;
    }

    bool operator!=(const vertex_attribute&& other) const
    {
        return !(*this == other);
    }
};

inline int
attributes_total_size(const std::vector<vertex_attribute>& attributes)
{
    return std::accumulate(attributes.begin(), attributes.end(), 0,
                           [](const auto& v) { return v.size; });
}

namespace common_attributes
{
const inline std::vector<vertex_attribute> pos2 = {{0, 0, 2}};
const inline std::vector<vertex_attribute> pos2_col3 {{0, 0, 2}, {1, 2, 3}};
const inline std::vector<vertex_attribute> pos2_col4 {{0, 0, 2}, {1, 2, 4}};
const inline std::vector<vertex_attribute> pos2_uv {{0, 0, 2}, {1, 2, 2}};

}    // namespace common_attributes
}    // namespace corgi