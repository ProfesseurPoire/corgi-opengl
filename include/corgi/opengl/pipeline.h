#pragma once

#include <corgi/opengl/program.h>
#include <corgi/opengl/uniform_buffer_object.h>
#include <corgi/opengl/texture.h>

#include <memory>

namespace corgi
{

struct sampler
{
    corgi::texture* texture;
    // Value associated to the sampler2D on the glsl shader file
    // layout(binding = 0) uniform sampler2D texture_sampler;
    int binding;
};

class pipeline
{
public:
    corgi::program* program_ {nullptr};

    std::vector<sampler> samplers_;
    std::vector<std::unique_ptr<uniform_buffer_object_interface>>
        uniform_buffer_objects_;

private:
};
}    // namespace corgi