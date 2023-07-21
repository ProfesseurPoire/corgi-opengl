#pragma once

#include <corgi/opengl/shaders.h>
#include <corgi/opengl/vertex_attribute.h>

#include <string>
#include <vector>

namespace corgi
{

class shader
{
public:
    // Lifecycle

    /*!
     * The vertex attributes the shader is supposed to be working with
     */
    shader(std::string                   source,
           std::vector<vertex_attribute> vertex_attributes,
           shader_type                   shader_type);

    explicit shader(const shader_content& s);

    ~shader();

    shader(const shader& other) = delete;
    shader(shader&& other)      = delete;

    shader& operator=(const shader& other) = delete;
    shader& operator=(shader&& other)      = delete;

    /**
     * @brief Returns true if the shader successfully compiled
     */
    bool compiled() const;

    // Functions

    unsigned id() const { return id_; }

    const std::string& source() const;

    shader_type type() const { return shader_type_; }

    const std::vector<vertex_attribute>& vertex_attributes() const;

private:
    void create_shader();
    void compile_shader();
    void check_compile_status();

    unsigned id_ {0};

    /**
     * \brief We keep the content of the shader, mostly for debugging purposes
     */
    const std::string source_;

    shader_type shader_type_;

    bool success_ {false};

    std::vector<vertex_attribute> vertex_attributes_;
};
}    // namespace corgi
