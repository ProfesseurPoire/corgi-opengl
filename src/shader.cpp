#include <corgi/opengl/shader.h>
#include <glad/glad.h>

#include <fstream>
#include <iostream>

namespace corgi
{
shader::shader(const std::string& content,std::vector<vertex_attribute> attributes, shader_type shader_type)
    : shader_type_(shader_type)
    , vertex_attributes_(attributes)
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

    auto c = content.c_str();

    glShaderSource(id_, 1, &c, NULL);
    glCompileShader(id_);

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
    }
    else
        success_ = true;
}

bool shader::compiled()
{
    return success_;
}

const std::vector<vertex_attribute>& shader::vertex_attributes()const
{
    return vertex_attributes_;
}

shader::~shader()
{
    glDeleteShader(id_);
}

long long shader::memory_usage() const
{
    // NOT IMPLEMENTED
    // Also not exactly sure what I should be returning here lol
    return 0;
}
}    // namespace corgi
