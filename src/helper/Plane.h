#ifndef STUDY_OPENGL_PLANE_H
#define STUDY_OPENGL_PLANE_H

#include <glm/glm.hpp>
#include <vector>
#include "shader/Shader.h"

class Plane {
public:
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoord;
    };
    struct Texture {
        enum TextureType {
            Diffuse = 1,
            Specular
        };

        unsigned int id;
        TextureType type;
    };

    static std::vector<Texture> createTextures(const std::string &fullDiffuse, const std::string &fullSpecular = std::string());
    static std::vector<Vertex>& createVertices();

    explicit Plane(const std::vector<Vertex> &vertices, const std::vector<Texture> &textures);

    void draw(Shader &shader);

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 projection;
private:
    std::vector<Vertex> vertices_;
    std::vector<Texture> textures_;
    unsigned int VAO_, VBO_;
};


#endif //STUDY_OPENGL_PLANE_H
