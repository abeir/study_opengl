#include "Plane.h"
#include "Helper.h"
#include <glad/glad.h>

namespace PlaneInternal {

    std::vector<Plane::Vertex> planeVertexVector = {
            Plane::Vertex { glm::vec3(5.0f, -0.5f,  5.0f), glm::vec2(2.0f, 0.0f) },
            Plane::Vertex { glm::vec3(-5.0f, -0.5f,  5.0f), glm::vec2(0.0f, 0.0f) },
            Plane::Vertex { glm::vec3(-5.0f, -0.5f, -5.0f), glm::vec2(0.0f, 2.0f) },

            Plane::Vertex { glm::vec3(5.0f, -0.5f,  5.0f), glm::vec2(2.0f, 0.0f) },
            Plane::Vertex { glm::vec3(-5.0f, -0.5f, -5.0f), glm::vec2(0.0f, 2.0f) },
            Plane::Vertex { glm::vec3(5.0f, -0.5f, -5.0f), glm::vec2(2.0f, 2.0f) },
    };
}



Plane::Plane(const std::vector<Vertex> &vertices, const std::vector<Texture> &textures)
    : vertices_{vertices}, textures_{textures}
{
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoord));

    glBindVertexArray(0);
}

void Plane::draw(Shader &shader) {
    shader.bind();
    shader.setUniformMat4("model", model);
    shader.setUniformMat4("view", view);
    shader.setUniformMat4("projection", projection);

    for(int i=0; i<textures_.size(); i++){
        auto &texture = textures_[i];
        char name[32] = {0};
        if(texture.type == Texture::Diffuse){
            sprintf(name, "material.texture_diffuse%d", i);
        }else if(texture.type == Texture::Specular){
            sprintf(name, "material.texture_specular%d", i);
        }
        shader.setUniformInt(name, i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, texture.id);
    }
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, vertices_.size());

    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}


std::vector<Plane::Texture> Plane::createTextures(const std::string &fullDiffuse, const std::string &fullSpecular) {
    std::vector<Plane::Texture> textures;

    unsigned int diffuse = Helper::loadTextureFromFile(fullDiffuse.c_str(), true);
    if(diffuse == 0){
        return {};
    }
    textures.emplace_back(Plane::Texture {
            .id = diffuse, .type = Texture::Diffuse
    });

    if(fullSpecular.empty()){
        return textures;
    }
    unsigned int specular = Helper::loadTextureFromFile(fullSpecular.c_str(), true);
    if(specular == 0){
        return {};
    }
    textures.emplace_back(Plane::Texture {
            .id = specular, .type = Texture::Specular
    });
    return textures;
}

std::vector<Plane::Vertex>& Plane::createVertices() {
    return PlaneInternal::planeVertexVector;
}
