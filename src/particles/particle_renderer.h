#pragma once

#include <list>
#include "shader.h"
#include "camera.cpp"

static const size_t maxQuadCount = 10000;
static const size_t maxVertexCount = maxQuadCount * 4;
static const size_t maxIndexCount = maxQuadCount * 6;
static const size_t instanceDataLength = 20; // mat4 ModelView matrix + vec4 for colors

class ParticleRenderer
{
public:
    ParticleRenderer(GLfloat screenScale, Shader *shaderProgram);
    ~ParticleRenderer() = default;

    void render(Camera *camera, const std::vector<Particle>& particles) ;
    void updateModelViewMatrix(glm::vec3 position, GLfloat rotation, GLfloat scale, glm::vec4 color, glm::mat4 view);

protected:
        static constexpr GLfloat particle_vertices_[8] = {
             0.5f, -0.5f,    //  3     1
             0.5f,  0.5f,    //
            -0.5f, -0.5f,    //  2     0
            -0.5f,  0.5f     // we are using GL_TRIANGLE_STRIP
        };

    void createEmptyVBO(uint32_t floatCount);
    void createQuadAttributesVBO(uint32_t attribute, uint32_t dataSize, uint32_t instancedDataLength, uint32_t offset) const;
    void updateQuadAttributesVBO(const std::vector<Particle>& particles);

    Shader *shaderProgram_;
    glm::mat4 modelViewMatrix_;
    GLfloat screenScale_;
    GLuint VAO_;
    GLuint vertexPositionsVBO_;
    GLuint quadAttributesVBO_;

    GLfloat vboAttributesBuffer[instanceDataLength * maxQuadCount];
    uint64_t vboBufferWritePosition = 0;
};

