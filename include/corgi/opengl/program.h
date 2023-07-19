#pragma once

#include <corgi/opengl/shader.h>

namespace corgi
{
class program
{
public:
    program(shader& vertex_shader, shader& fragment_shader);

    program(const program& other)     = delete;
    program(program&& other) noexcept;

    program& operator=(const program& other)     = delete;
    program& operator=(program&& other) noexcept;
    ~program();

    const std::vector<vertex_attribute>& vertex_attributes()const;
    void use();

    void end();

    unsigned id() const;

private:
    shader* vertex_shader_;
    shader* fragment_shader_;

    unsigned id_;
};
}    // namespace corgi