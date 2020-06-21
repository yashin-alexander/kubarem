#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <ft2build.h>
#include <freetype2/freetype/freetype.h>

#include "log.h"
#include "shader.h"


struct Character {
    GLuint TextureID;   // ID handle of the glyph texture
    glm::ivec2 Size;    // Size of glyph
    glm::ivec2 Bearing; // Offset from baseline to left/top of glyph
    GLuint Advance;     // Horizontal offset to advance to next glyph
};


class TextRenderer
{
public:
    std::map<GLchar, Character> Characters;
    Shader * _shaderProgram;
    TextRenderer(GLuint width, GLuint height, Shader * shaderProgram);
    void Load(std::string font, GLuint fontSize);
    void RenderText(std::string text, glm::vec2 position, GLfloat scale, glm::vec3 color = glm::vec3(1.0f));
private:
    GLuint VAO;
    GLuint VBO;
    GLuint width;
    GLuint height;
};

#endif
