#ifndef STUDY_OPENGL_SHADER_H
#define STUDY_OPENGL_SHADER_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader {
public:
    explicit Shader(const char* vertFile, const char* fragFile);
    ~Shader();

    void bind();
    void unbind();

    void setUniformInt(const char* name, int value);
    void setUniformFloat(const char* name, float value);
    void setUniformVec2(const char* name, const glm::vec2 &value);
    void setUniformVec3(const char* name, const glm::vec3 &value);
    void setUniformVec4(const char* name, const glm::vec4 &value);
    void setUniformMat4(const char* name, const glm::mat4 &value);

    inline std::string& error() { return error_; }
    inline bool hasError() { return !error_.empty(); }
private:
    int getUniformLocation(const char* name);

    inline void setError(const char* err) { error_ = err; }
    inline void setError(const std::string &err) { error_ = err; }

    std::string error_;
    unsigned int programId_;

    std::vector<std::pair<std::string,int>> cache_;
};


#endif //STUDY_OPENGL_SHADER_H
