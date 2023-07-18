#pragma once

#include <string>

namespace corgi
{
enum class shader_type : unsigned char
{
    vertex,
    fragment
};

class shader
{
public:
    // Lifecycle

    shader(const std::string& content, shader_type shader_type);
    ~shader();

    shader(const shader& other) = delete;
    shader(shader&& other)      = delete;

    shader& operator=(const shader& other) = delete;
    shader& operator=(shader&& other)      = delete;

    /**
     * @brief Returns true if the shader successfully compiled
     */
    bool compiled();

    // Functions

    unsigned id() const { return id_; }

    const std::string& source() const { return source_; }

    shader_type type() const { return shader_type_; }

    long long memory_usage() const;

private:
    unsigned          id_;
    const std::string source_;
    const std::string name_;
    shader_type       shader_type_;
    bool              success_ {false};
};
}    // namespace corgi
