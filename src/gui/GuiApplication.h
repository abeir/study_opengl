#ifndef STUDY_OPENGL_GUIAPPLICATION_H
#define STUDY_OPENGL_GUIAPPLICATION_H

#include <string>
#include <memory>
#include "glad/glad.h"
#include <glfw/glfw3.h>
#include "helper/Camera.h"


class GuiApplicationPrivate;

class GuiApplication {
public:
    GuiApplication();
    GuiApplication(const char* title);
    virtual ~GuiApplication();

    int width() const;
    int height() const;
    float widthF() const;
    float heightF() const;

    std::string error();

    void setTitle(const char* title);
    void setCamera(std::shared_ptr<Camera> camera);

    virtual void beforeCreateWindow();
    virtual void exec();

    virtual void onInit();
    virtual void onRender();
    virtual void onGui();

protected:
    void setError(const char* err);
    GLFWwindow* window();
    float deltaTime() const;

private:
    void processInputEsc();
    void processInputCamera();
    void updateDeltaTime();

    void createWindow(const char* title);
    void createWindowWithCamera(const char* title, std::shared_ptr<Camera> camera);
private:
    GuiApplicationPrivate* p_;
};


#endif //STUDY_OPENGL_GUIAPPLICATION_H
