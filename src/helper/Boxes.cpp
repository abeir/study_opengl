#include "Boxes.h"
#include "helper/Helper.h"
#include <glad/glad.h>


namespace BoxesInternal {

    std::vector<Boxes::Vertex> boxesVertexVector = {
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f) },

            //
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(1.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f,  0.0f, 1.0f), glm::vec2(0.0f, 0.0f) },

            Boxes::Vertex { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },

            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) },

            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },

            Boxes::Vertex { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f) },
            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f) },
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) },
    };

    std::vector<glm::vec3> boxesTransPositions = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };
}



Boxes::Boxes(const std::vector<Vertex> &vertices, const std::vector<Texture> &textures)
    : model{1.0f}, view{1.0f}, projection{1.0f}
    , vertices_{vertices}, textures_{textures}
    , VAO_{0}, VBO_{0}
{
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, texCoord));

    glBindVertexArray(0);
}

void Boxes::draw(Shader &shader) {
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

std::vector<Boxes::Texture> Boxes::createTextures(const std::string &fullDiffuse, const std::string &fullSpecular) {
    std::vector<Boxes::Texture> textures;

    unsigned int diffuse = Helper::loadTextureFromFile(fullDiffuse.c_str(), true);
    if(diffuse == 0){
        return {};
    }
    textures.emplace_back(Boxes::Texture {
            .id = diffuse, .type = Texture::Diffuse
    });

    if(fullSpecular.empty()){
        return textures;
    }
    unsigned int specular = Helper::loadTextureFromFile(fullSpecular.c_str(), true);
    if(specular == 0){
        return {};
    }
    textures.emplace_back(Boxes::Texture {
            .id = specular, .type = Texture::Specular
    });
    return textures;
}

std::vector<Boxes::Vertex>& Boxes::createVertices() {
    return BoxesInternal::boxesVertexVector;
}

std::vector<glm::vec3>& Boxes::createTransformPositions() {
    return BoxesInternal::boxesTransPositions;
}
