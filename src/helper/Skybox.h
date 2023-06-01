#ifndef STUDY_OPENGL_SKYBOX_H
#define STUDY_OPENGL_SKYBOX_H

#include <map>
#include <string>
#include <vector>
#include "shader/Shader.h"
#include <glm/glm.hpp>

class Skybox {
public:
    enum TextureFace {
        Right, Left, Top, Bottom, Front, Back
    };

    static std::vector<float>& createVertices();

    explicit Skybox(const std::vector<float> &vertices);
    ~Skybox();

    void loadTextures(const std::map<TextureFace, std::string> &resPathes);
    inline unsigned int textureID() { return texture_; }


    void draw(Shader &shader);

    glm::mat4 view;
    glm::mat4 projection;

    inline bool hasError() { return !error_.empty(); }
    inline std::string& error() { return error_; }
private:
    unsigned int texture_;
    unsigned int VAO_, VBO_;
    std::vector<float> vertices_;
    std::string error_;
};


#endif //STUDY_OPENGL_SKYBOX_H
