#ifndef STUDY_OPENGL_MESH_H
#define STUDY_OPENGL_MESH_H

#include <glm/glm.hpp>
#include <vector>
#include <string>
#include "shader/Shader.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

struct Texture {
    enum TextureType {
        Diffuse = 1,
        Specular
    };

    unsigned int id;
    TextureType type;
    std::string path;
};


class Mesh {
public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices, const std::vector<Texture> &textures);

    void draw(Shader &shader);

    [[nodiscard]] inline unsigned int vao() const { return VAO_; }

    inline std::vector<Vertex>& vertices() { return vertices_; }
    inline std::vector<unsigned int>& indices() { return indices_; }
    inline std::vector<Texture>& textures() { return textures_; }

private:
    std::vector<Vertex> vertices_;
    std::vector<unsigned int> indices_;
    std::vector<Texture> textures_;
    unsigned int VAO_, VBO_, EBO_;

    void setup();
};


#endif //STUDY_OPENGL_MESH_H
