#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


class Shader
{
public:
    GLuint program_ID_;
    Shader() { }
    Shader  &Use();
    void    Compile(const GLchar *vertex_source, const GLchar *fragment_source);
    // Utility functions
    void    SetFloat    (const GLchar *name, GLfloat value, GLboolean use_shader = false);
    void    SetInteger  (const GLchar *name, GLint value, GLboolean use_shader = false);
    void    SetVector2f (const GLchar *name, GLfloat x, GLfloat y, GLboolean use_shader = false);
    void    SetVector2f (const GLchar *name, const glm::vec2 &value, GLboolean use_shader = false);
    void    SetVector3f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean use_shader = false);
    void    SetVector3f (const GLchar *name, const glm::vec3 &value, GLboolean use_shader = false);
    void    SetVector4f (const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean use_shader = false);
    void    SetVector4f (const GLchar *name, const glm::vec4 &value, GLboolean use_shader = false);
    void    SetMatrix4  (const GLchar *name, const glm::mat4 &matrix, GLboolean use_shader = false);

    static Shader* LoadFromFile(const GLchar *v_shader_file, const GLchar *f_shader_file);

private:
    void    CheckCompileErrors(GLuint object, const char *type);
};

