#ifndef STUDY_OPENGL_MAIN_H
#define STUDY_OPENGL_MAIN_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "helper/Camera.h"
#include <memory>


#define CHECK_GL_ID(id) \
    if(id == 0){ \
        std::cout << error() << std::endl; \
        return 0; \
    }


class Main {
public:
    [[nodiscard]] inline int width() const { return width_; }
    [[nodiscard]] inline int height() const { return height_; }
    [[nodiscard]] inline float widthF() const { return (float)width_; }
    [[nodiscard]] inline float heightF() const { return (float)height_; }

    [[nodiscard]] inline std::string error() const { return error_; }

    virtual int exec() = 0;

protected:
    inline void setError(const char* err) { error_ = err; }
    inline GLFWwindow* window() { return window_; }
    inline float deltaTime() { return deltaTime_; }

    static void framebufferSizeCallback(GLFWwindow* window, int w, int h);

    void processInputEsc();
    void processInputCamera();
    void updateDeltaTime();

    bool createWindow(const char* title);
    bool createWindowWithCamera(const char* title, std::shared_ptr<Camera> camera);
    GLuint createShader(GLenum shaderType, const char* filePath);
    GLuint createProgram(GLuint vert, GLuint frag);

    GLuint createVAO();
    GLuint createVBO();

    GLuint createTexture(const char* filePath, bool alpha = false);

private:
    int width_ = 1080;
    int height_ = 720;
    std::string error_;
    GLFWwindow *window_;

    // timing
    float deltaTime_ = 0.0f;
    float lastFrame_ = 0.0f;
};

#endif //STUDY_OPENGL_MAIN_H
