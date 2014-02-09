#include "shader.h"

#include <fstream>
#include <boost/scope_exit.hpp>

namespace gl {
shader_ptr shader::from_file(GLenum type, const std::string& filename)
{
    std::ifstream file(filename);
    if (!file)
        throw std::exception{};
    BOOST_SCOPE_EXIT_ALL(&file) { file.close(); };
    std::string content{std::istreambuf_iterator<char>{file},
                        std::istreambuf_iterator<char>{}};
    return from_source(type, content);
}

shader_ptr shader::from_source(GLenum type, const std::string& source)
{
    return shader_ptr{new shader{type, source}};
}

shader::shader(GLenum type, const std::string& source) noexcept
{
    handle_ = glCreateShader(type);
    const char* source_ptr = source.c_str();
    glShaderSource(handle_, 1, &source_ptr, NULL);
}

shader::~shader()
{
    if (0 != handle_)
        glDeleteShader(handle_);
}

bool shader::compile()
{
    glCompileShader(handle_);

    GLint status = GL_TRUE;
    glGetShaderiv(handle_, GL_COMPILE_STATUS, &status);
    return (GL_TRUE == status);
}

std::string shader::get_log()
{
    GLint log_length;
    glGetShaderiv(handle_, GL_INFO_LOG_LENGTH, &log_length);
    if (0 == log_length)
        return "";
    std::unique_ptr<char[]> log_data{new char[log_length]};
    glGetShaderInfoLog(handle_, log_length, NULL, log_data.get());
    return log_data.get();
}
}
