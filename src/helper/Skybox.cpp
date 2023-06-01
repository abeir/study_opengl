#include "Skybox.h"
#include "helper/stbImage.h"
#include <glad/glad.h>


namespace SkyboxInternal {
    std::vector<float> skyboxVertices = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };
}


Skybox::Skybox(const std::vector<float> &vertices)
    : texture_{0}, VAO_{0}, VBO_{0}
    , vertices_{vertices}
    , view{1.0f}, projection{1.0f}
{
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices_.size(), &vertices_[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, nullptr);

    glBindVertexArray(0);
}

Skybox::~Skybox() {
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
    glDeleteTextures(1, &texture_);
}


void Skybox::loadTextures(const std::map<TextureFace, std::string> &resPathes) {
    glGenTextures(1, &texture_);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);

    int w, h, n;
    for(const auto & it : resPathes){

        stbi_uc* data = stbi_load(it.second.c_str(), &w, &h, &n, 0);
        if(!data){
            error_ = "Failed to load texture: " + it.second;
            return;
        }
        GLint format = GL_RGB;
        if(n == 1){
            format = GL_RED;
        }else if(n == 3){
            format = GL_RGB;
        }else if(n == 4){
            format = GL_RGBA;
        }

        switch (it.first) {
            case Right:
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
                break;
            case Left:
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
                break;
            case Top:
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
                break;
            case Bottom:
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
                break;
            case Front:
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
                break;
            case Back:
                glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
                break;
            default:
                break;
        }
        stbi_image_free(data);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Skybox::draw(Shader &shader) {
    glDepthFunc(GL_LEQUAL);
    shader.bind();
    shader.setUniformMat4("view", view);
    shader.setUniformMat4("projection", projection);
    shader.setUniformInt("skybox", 0);

    glBindVertexArray(VAO_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

std::vector<float> &Skybox::createVertices() {
    return SkyboxInternal::skyboxVertices;
}
