#pragma once

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
};
}