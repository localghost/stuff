#include "program.h"

#include <memory>
#include <utility>

namespace gl {
program::program() noexcept
{
    handle_ = glCreateProgram();
}

program::~program()
{
    if (0 != handle_)
        glDeleteProgram(handle_);
}

void program::attach_shader(shader_ptr sh)
{
    shaders_.push_back(std::move(sh));
}

bool program::compile()
{
    for (const auto& sh : shaders_)
        glAttachShader(handle_, sh->get_handle());

    glLinkProgram(handle_);

    for (const auto& sh : shaders_)
        glDetachShader(handle_, sh->get_handle());

    GLint status;
    glGetProgramiv(handle_, GL_LINK_STATUS, &status);
    return (GL_TRUE == status);
}

std::string program::get_log() const
{
    GLint log_length;
    glGetProgramiv(handle_, GL_INFO_LOG_LENGTH, &log_length);
    if (0 == log_length)
        return "";
    std::unique_ptr<char[]> log_data{new char[log_length]};
    glGetProgramInfoLog(handle_, log_length, NULL, log_data.get());
    return log_data.get();
}

void program::enable()
{
    glUseProgram(handle_);
}

void program::disable()
{
    glUseProgram(0);
}
}