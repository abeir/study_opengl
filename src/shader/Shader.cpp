#include "Shader.h"
#include "helper/Helper.h"
#include "ShaderLoader.h"
#include <cstring>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


namespace Internal {

    GLuint createShader(GLenum shaderType, const char *filePath, std::string &err) {
        ShaderLoader loader;
        if(!loader.loadInSrc(filePath)){
            err.assign("Failed to load shader");
            return 0;
        }
        const char* src = loader.get().c_str();

        GLuint id = glCreateShader(shaderType);
        glShaderSource(id, 1, &src, nullptr);
        glCompileShader(id);
        GLint success;
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if(!success){
            char msg[1024] = {0};
            glGetShaderInfoLog(id, 1024, nullptr, msg);
            err.assign(msg);
            return 0;
        }
        return id;
    }

    GLuint createProgram(GLuint vert, GLuint frag, std::string &err) {
        GLuint id = glCreateProgram();
        glAttachShader(id, vert);
        glAttachShader(id, frag);
        glLinkProgram(id);

        glDeleteShader(vert);
        glDeleteShader(frag);
        GLint success;
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if(!success){
            char msg[1024] = {0};
            glGetProgramInfoLog(id, 1024, nullptr, msg);
            err.assign(msg);
            return 0;
        }
        return id;
    }

}


Shader::Shader(const char *vertFile, const char *fragFile)
    : programId_{0}
{
    std::string err;
    GLuint vert = Internal::createShader(GL_VERTEX_SHADER, vertFile, err);
    if(vert == 0){
        setError("[vert] " + err);
        return;
    }
    GLuint frag = Internal::createShader(GL_FRAGMENT_SHADER, fragFile, err);
    if(frag == 0){
        setError("[frag] " + err);
        return;
    }
    GLuint program = Internal::createProgram(vert, frag, err);
    if(program == 0){
        setError("[prog] " + err);
        return;
    }
    programId_ = program;
}

Shader::~Shader() {
    glDeleteProgram(programId_);
}

void Shader::bind() {
    glUseProgram(programId_);
}

void Shader::unbind() {
    glUseProgram(0);
}

void Shader::setUniformInt(const char *name, int value) {
    int id = getUniformLocation(name);
    if(id > -1){
        glUniform1i(id, value);
    }
}


void Shader::setUniformFloat(const char *name, float value) {
    int id = getUniformLocation(name);
    if(id > -1){
        glUniform1f(id, value);
    }
}

void Shader::setUniformVec2(const char *name, const glm::vec2 &value) {
    int id = getUniformLocation(name);
    if(id > -1){
        glUniform2fv(id, 1, glm::value_ptr(value));
    }
}

void Shader::setUniformVec3(const char *name, const glm::vec3 &value) {
    int id = getUniformLocation(name);
    if(id > -1){
        glUniform3fv(id, 1, glm::value_ptr(value));
    }
}

void Shader::setUniformVec4(const char *name, const glm::vec4 &value) {
    int id = getUniformLocation(name);
    if(id > -1){
        glUniform4fv(id, 1, glm::value_ptr(value));
    }
}

void Shader::setUniformMat4(const char *name, const glm::mat4 &value) {
    int id = getUniformLocation(name);
    if(id > -1){
        glUniformMatrix4fv(id, 1, GL_FALSE, glm::value_ptr(value));
    }
}


unsigned int Shader::matricesBlockBinding(const char *name, unsigned int blockPoint) {
    GLuint index = glGetUniformBlockIndex(programId_, name);
    glUniformBlockBinding(programId_, index, blockPoint);

    GLuint UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4)*2, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, blockPoint, UBO, 0, sizeof(glm::mat4)*2);
    return UBO;
}

void Shader::setMatrices(unsigned int UBO, const glm::mat4 &view, const glm::mat4 &projection) {
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(view));
    glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(projection));
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

int Shader::getUniformLocation(const char *name) {
    for(auto &item : cache_){
        if(item.first == name){
            return item.second;
        }
    }
    GLint location = glGetUniformLocation(programId_, name);
    if(location > -1){
        std::pair<std::string,int> item;
        item.first = name;
        item.second = location;
        cache_.emplace_back(item);
    }
    return location;
}