#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <string>
#include <memory>
#include "opengl.h"
#include "inspect.h"

namespace gl {

class shader;
#ifdef CPP11
using shader_ptr = std::shared_ptr<shader>;
#else
typedef std::shared_ptr<shader> shader_ptr;
#endif

class shader
{
public:
    static shader_ptr from_file(GLenum type, const std::string& filename);
    static shader_ptr from_source(GLenum type, const std::string& source);

    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;

    ~shader();

    bool compile() noexcept;

    std::string get_log() const;

private:
    friend class program;
    
    shader(GLenum type, const std::string& source);
    
    GLuint get_handle() const noexcept { return handle_; }
    
    GLuint handle_;
};
}

#endif
