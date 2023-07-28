#pragma once

#include <corgi/opengl/program.h>
#include <corgi/opengl/uniform_buffer_object.h>
#include <corgi/opengl/texture.h>
#include <corgi/opengl/color.h>

#include <memory>
#include <map>

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

    bool write_color {true};
    bool enable_depth_test {true};
    bool depth_mask {false};
    
    color clear_color;

    template<class T>
    uniform_buffer_object<T>& add_ubo(unsigned location) 
    {
        if (uniform_buffer_objects_.contains(location))
            throw std::logic_error(
                "pipeline::add_ubo : Location already registered");


        auto new_ubo = new uniform_buffer_object<T>(location);
        
        uniform_buffer_objects_.emplace(location, new_ubo);
        return *new_ubo;
    }

    template<class T>
    uniform_buffer_object<T>& get_ubo(unsigned location)
    {
        return *dynamic_cast<uniform_buffer_object<T>*>(
            uniform_buffer_objects_.at(location).get());
    }

    std::vector<sampler> samplers_;
    std::map<unsigned, std::unique_ptr<uniform_buffer_object_interface>>
        uniform_buffer_objects_;

private:

};
}    // namespace corgi