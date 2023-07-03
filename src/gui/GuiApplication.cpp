#include "GuiApplication.h"
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>


namespace Internal::Gui {
    // camera move
    float lastX = 0.0f;
    float lastY = 0.0f;
    bool firstMouse = false;

    static std::shared_ptr<Camera> camera;

    static void glfwErrorCallback(int error, const char* description)
    {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    static void framebufferSizeCallback(GLFWwindow* window, int w, int h)
    {
        glViewport(0, 0, w, h);
    }

    static void mouseCallback(GLFWwindow* window, double xposIn, double yposIn)
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

        camera->ProcessMouseMovement(xoffset, yoffset);
    }

    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
    {
        camera->ProcessMouseScroll(static_cast<float>(yoffset));
    }
}


class GuiApplicationPrivate
{
public:
    int width_ = 1080;
    int height_ = 720;
    std::string title_;
    std::string error_;
    GLFWwindow *window_ = nullptr;

    // timing
    float deltaTime_ = 0.0f;
    float lastFrame_ = 0.0f;
};


GuiApplication::GuiApplication() : p_{new GuiApplicationPrivate} {
}

GuiApplication::GuiApplication(const char* title) : p_{new GuiApplicationPrivate} {
    p_->title_ = title;
}

GuiApplication::~GuiApplication() {
    delete p_;
}

int GuiApplication::width() const {
    return p_->width_;
}

int GuiApplication::height() const {
    return p_->height_;
}

float GuiApplication::widthF() const {
    return (float)p_->width_;
}

float GuiApplication::heightF() const {
    return (float)p_->height_;
}

std::string GuiApplication::error() {
    return p_->error_;
}

void GuiApplication::setError(const char *err) {
    p_->error_ = err;
}

void GuiApplication::setTitle(const char *title) {
    p_->title_ = title;
}

void GuiApplication::setCamera(std::shared_ptr<Camera> camera) {
    Internal::Gui::camera = std::move(camera);
}

GLFWwindow *GuiApplication::window() {
    return p_->window_;
}

float GuiApplication::deltaTime() const {
    return p_->deltaTime_;
}


void GuiApplication::processInputEsc() {
    if(glfwGetKey(p_->window_, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(p_->window_, GLFW_TRUE);
    }
}

void GuiApplication::processInputCamera() {
    if (glfwGetKey(p_->window_, GLFW_KEY_W) == GLFW_PRESS)
        Internal::Gui::camera->ProcessKeyboard(Camera::FORWARD, p_->deltaTime_);
    if (glfwGetKey(p_->window_, GLFW_KEY_S) == GLFW_PRESS)
        Internal::Gui::camera->ProcessKeyboard(Camera::BACKWARD, p_->deltaTime_);
    if (glfwGetKey(p_->window_, GLFW_KEY_A) == GLFW_PRESS)
        Internal::Gui::camera->ProcessKeyboard(Camera::LEFT, p_->deltaTime_);
    if (glfwGetKey(p_->window_, GLFW_KEY_D) == GLFW_PRESS)
        Internal::Gui::camera->ProcessKeyboard(Camera::RIGHT, p_->deltaTime_);
}

void GuiApplication::updateDeltaTime() {
    auto currentFrame = static_cast<float>(glfwGetTime());
    p_->deltaTime_ = currentFrame - p_->lastFrame_;
    p_->lastFrame_ = currentFrame;
}

void GuiApplication::createWindow(const char *title) {
    glfwSetErrorCallback(Internal::Gui::glfwErrorCallback);

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    beforeCreateWindow();

    p_->window_ = glfwCreateWindow(p_->width_, p_->height_, title, nullptr, nullptr);
    glfwMakeContextCurrent(p_->window_);
    glfwSwapInterval(1);
    glfwSetFramebufferSizeCallback(p_->window_, Internal::Gui::framebufferSizeCallback);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void GuiApplication::createWindowWithCamera(const char *title, std::shared_ptr<Camera> camera) {
    Internal::Gui::lastX = widthF() / 2.0f;
    Internal::Gui::lastY = heightF() / 2.0f;

    Internal::Gui::camera = std::move(camera);
    createWindow(title);
    glfwSetCursorPosCallback(window(), Internal::Gui::mouseCallback);
    glfwSetScrollCallback(window(), Internal::Gui::scrollCallback);
    glfwSetInputMode(window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}


void GuiApplication::beforeCreateWindow() {

}

void GuiApplication::exec() {
    if(Internal::Gui::camera){
        createWindowWithCamera(p_->title_.c_str(), Internal::Gui::camera);
    }else{
        createWindow(p_->title_.c_str());
    }

    onInit();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(p_->window_, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    while(!glfwWindowShouldClose(p_->window_)){
        updateDeltaTime();
        processInputEsc();
        processInputCamera();

        onRender();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        onGui();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window());
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(p_->window_);
    glfwTerminate();
}

void GuiApplication::onInit() {
}

void GuiApplication::onRender() {
}

void GuiApplication::onGui() {
}




