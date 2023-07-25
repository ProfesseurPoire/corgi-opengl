#pragma once

#include <corgi/opengl/program.h>
#include <corgi/opengl/uniform_buffer_object.h>

#include <memory>

namespace corgi
{

struct sampler
{
    // Texture* texture;
    int binding;
};

class pipeline
{
public:
    corgi::program* program_ {nullptr};

    std::vector<std::unique_ptr<uniform_buffer_object_interface>>
        uniform_buffer_objects_;

private:
};
}    // namespace corgi