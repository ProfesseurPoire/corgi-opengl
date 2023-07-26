#pragma once

#include <corgi/math/Matrix.h>

namespace corgi
{

struct default_ubo
{
    corgi::Matrix mvp;
    corgi::vec4   color {1.0F, 1.0F, 0.0F, 1.0F};
    int           use_color {0};
};


}    // namespace corgi