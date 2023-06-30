#include "Main.h"
#include "shader/ShaderLoader.h"
#include "helper/Helper.h"
#include "stbImage.h"
#include <utility>


namespace Internal {
    // camera move
    float lastX = 0.0f;
    float lastY = 0.0f;
    bool firstMouse = true;


    static std::shared_ptr<Camera> camera_;


    void mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
    {
        auto xpos = static_cast<float>(xposIn);
        auto ypos = static_cast<float>(yposIn);

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera_->ProcessMouseMovement(xoffset, yoffset);
    }

    void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera_->ProcessMouseScroll(static_cast<float>(yoffset));
    }

}

void Main::processInputEsc() {
    if(glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window_, GLFW_TRUE);
    }
}

void Main::processInputCamera()
{
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS)
        Internal::camera_->ProcessKeyboard(Camera::FORWARD, deltaTime_);
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS)
        Internal::camera_->ProcessKeyboard(Camera::BACKWARD, deltaTime_);
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
        Internal::camera_->ProcessKeyboard(Camera::LEFT, deltaTime_);
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
        Internal::camera_->ProcessKeyboard(Camera::RIGHT, deltaTime_);
}

void Main::updateDeltaTime() {
    auto currentFrame = static_cast<float>(glfwGetTime());
    deltaTime_ = currentFrame - lastFrame_;
    lastFrame_ = currentFrame;
}

void Main::framebufferSizeCallback(GLFWwindow *window, int w, int h) {
    glViewport(0, 0, w, h);
}

bool Main::createWindow(const char* title){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    beforeCreateWindow();

    window_ = glfwCreateWindow(width_, height_, title, nullptr, nullptr);
    glfwMakeContextCurrent(window_);
    glfwSetFramebufferSizeCallback(window_, framebufferSizeCallback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    return true;
}

bool Main::createWindowWithCamera(const char *title, std::shared_ptr<Camera> camera) {
    Internal::lastX = widthF() / 2.0f;
    Internal::lastY = heightF() / 2.0f;

    Internal::camera_ = std::move(camera);
    createWindow(title);
    glfwSetCursorPosCallback(window(), Internal::mouseCallback);
    glfwSetScrollCallback(window(), Internal::scrollCallback);
    glfwSetInputMode(window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    return true;
}

GLuint Main::createShader(GLenum shaderType, const char *filePath) {
    ShaderLoader loader;
    if(!loader.loadInSrc(filePath)){
        setError("Failed to load shader");
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
        setError(msg);
        return 0;
    }
    return id;
}

GLuint Main::createProgram(GLuint vert, GLuint frag) {
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
        setError(msg);
        return 0;
    }
    return id;
}


GLuint Main::createTexture(const char *filePath, bool alpha, bool flipV) {
    int w, h, nrChannel;
    stbi_set_flip_vertically_on_load(flipV);

    std::string full = Helper::inRes(filePath);
    stbi_uc* data = stbi_load(full.c_str(), &w, &h, &nrChannel, 0);
    if(!data){
        setError("Failed to load texture");
        return 0;
    }
    GLuint id;
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if(alpha){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }else{
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return id;
}

GLuint Main::createVAO() {
    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    return VAO;
}

GLuint Main::createVBO() {
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    return VBO;
}

void Main::beforeCreateWindow() {

}
