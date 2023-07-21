#include <corgi/opengl/shader.h>
#include <glad/glad.h>

#include <iostream>
#include <cassert>

namespace corgi
{
shader::shader(std::string                   source,
               std::vector<vertex_attribute> vertex_attributes,
               const shader_type             shader_type)
    : source_(std::move(source))
    , shader_type_(shader_type)
    , vertex_attributes_(std::move(vertex_attributes))
{
    assert(!vertex_attributes_.empty()); // Vertex attributes must not be empty
    assert(!source_.empty()); // shader source code must not be empty

    create_shader();
    compile_shader();
    check_compile_status();
}

shader::shader(const shader_content& s)
    : source_(s.content)
    , shader_type_(s.shader_type)
    , vertex_attributes_(s.attributes)
{
    assert(!vertex_attributes_.empty());    // Vertex attributes must not be empty
    assert(!source_.empty());               // shader source code must not be empty

    create_shader();
    compile_shader();
    check_compile_status();
}

const std::string& shader::source() const
{
    return source_;
}

bool shader::compiled() const
{
    return success_;
}

const std::vector<vertex_attribute>& shader::vertex_attributes() const
{
    return vertex_attributes_;
}

void shader::create_shader()
{
    switch(shader_type_)
    {
        case shader_type::fragment:
            id_ = glCreateShader(GL_FRAGMENT_SHADER);
            break;

        case shader_type::vertex:
            id_ = glCreateShader(GL_VERTEX_SHADER);
            break;
    }
    assert("Id is equal to 0 after creation" && id_ != 0);
}

void shader::compile_shader()
{
    auto c = source_.c_str();
    glShaderSource(id_, 1, &c, NULL);
    glCompileShader(id_);
}

void shader::check_compile_status()
{
    GLint success = 0;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);

    if(success == GL_FALSE)
    {
        success_         = false;
        GLint max_length = 0;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &max_length);
        std::string errorLog(max_length, ' ');
        glGetShaderInfoLog(id_, max_length, &max_length, &errorLog[0]);
        std::cerr << errorLog << std::endl;
        glDeleteShader(id_);
        assert("Shader could not be compiled" && (success != GL_FALSE));
    }
    else
        success_ = true;
}

shader::~shader()
{
    glDeleteShader(id_);
}
}    // namespace corgi
