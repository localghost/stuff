#ifndef OPENGL_PROGRAM_H
#define OPENGL_PROGRAM_H

#include <vector>
#include <string>
#include "opengl.h"
#include "shader.h"

namespace gl {
class program
{
    program() noexcept;
 
    ~program();
    
    void attach_shader(shader_ptr sh);
    
    bool compile() noexcept;
    
    std::string get_log() const noexcept;
    
    void enable() noexcept;
    
    void disable() noexcept;

private:
    GLuint handle_;
    std::vector<shader_ptr> shaders_;
};
}

#endif
