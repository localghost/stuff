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
    
    bool compile();
    
    std::string get_log() const;
    
    void enable();
    
    void disable();

private:
    GLuint handle_;
    std::vector<shader_ptr> shaders_;
};
}

#endif
