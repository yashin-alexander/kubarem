#include "box.h"


Box::Box(const char *textureName, Shader *_shaderProgram, GLfloat _screenScale, glm::vec3 size, glm::vec3 position):
    _textureName(textureName),
    _shaderProgram(_shaderProgram),
    _screenScale(_screenScale),
    _size(size),
    _position(position)
    {};


void Box::Init(){
    glGenVertexArrays(2, &VAO);
    glGenBuffers(2, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(_textureName, &width, &height, &nrChannels, 0);
    if (data)
    {
        log_dbg("Box texture is loaded");
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        log_err("Failed to load BOX texture");

    stbi_image_free(data);

    _shaderProgram->Use();
    _shaderProgram->SetInteger("texture1", 0);
}


void Box::Render(Camera *camera){
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

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
