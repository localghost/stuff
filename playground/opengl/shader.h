#ifndef OPENGL_SHADER_H
#define OPENGL_SHADER_H

#include <string>
#include <memory>
#include "opengl.h"

namespace gl {
class shader;
using shader_ptr = std::shared_ptr<shader>;

class shader
{
public:
    static shader_ptr from_file(GLenum type, const std::string& filename);
    static shader_ptr from_source(GLenum type, const std::string& source);

    shader(const shader&) = delete;
    shader& operator=(const shader&) = delete;

    ~shader();

    bool compile();

    std::string get_log();

private:
    friend class program;
    
    shader(GLenum type, const std::string& source) noexcept;
    
    GLuint get_handle() const noexcept { return handle_; }
    
    GLuint handle_;
};
}

#endif
