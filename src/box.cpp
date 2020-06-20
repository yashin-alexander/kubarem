#include "box.h"


Box::Box(const char *textureName, Shader *_shaderProgram, GLfloat _screenScale, glm::vec3 size, glm::vec3 position):
    _textureName(textureName),
    _shaderProgram(_shaderProgram),
    _screenScale(_screenScale),
    _size(size),
    _position(position)
    {
        this->Init();
    };


void Box::Init(){
    glGenVertexArrays(2, &VAO);
    glGenBuffers(2, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normales coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // texture
    texture1 = loadTexture(_textureName);

    _shaderProgram->Use();
    _shaderProgram->SetInteger("texture1", 0);
}


void Box::Render(Camera *camera, glm::vec3 lightPoint){
    glUseProgram(_shaderProgram->ID);
    _shaderProgram->SetVector3f("light.position", lightPoint);
    _shaderProgram->SetVector3f("viewPos", camera->Position);

    // light properties
    GLfloat time = glfwGetTime();
    _shaderProgram->SetVector3f("light.ambient", 1.f, 1.f, 1.f);
    _shaderProgram->SetVector3f("light.diffuse", 0.1f, cos(2*time), sin(time));
    _shaderProgram->SetVector3f("light.specular", 1.0f, .0f, .0f);

    // material properties
    _shaderProgram->SetVector3f("material.specular", 0.5f, 0.5f, 0.5f);
    _shaderProgram->SetFloat("material.shininess", 256.0f);


    // bind textures on corresponding texture units
    // bind diffuse map
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, texture1);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, _position);
    model = glm::scale(model, _size);

    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), _screenScale, 0.1f, 1200.0f);

    _shaderProgram->SetMatrix4("model", model);
    _shaderProgram->SetMatrix4("view", view);
    _shaderProgram->SetMatrix4("projection", projection);

    // render
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
};


GLuint Box::loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        log_err("Failed to load texture %s", path);
        stbi_image_free(data);
    }

    return textureID;
}

Lamp::Lamp(const char *textureName, Shader *_shaderProgram, GLfloat _screenScale, glm::vec3 size, glm::vec3 position):
      Box(textureName, _shaderProgram, _screenScale, size, position){};

