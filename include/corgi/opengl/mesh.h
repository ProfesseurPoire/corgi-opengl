#pragma once
#include <corgi/opengl/vertex_array.h>

namespace corgi
{
class mesh
{
    mesh(vertex_array& va);

    vertex_array vertex_array_;
    primitive_type primitive_;
};
}    // namespace corgi