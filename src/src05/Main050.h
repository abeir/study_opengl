#ifndef STUDY_OPENGL_MAIN050_H
#define STUDY_OPENGL_MAIN050_H

#include "helper/Main.h"
#include "gui/Painter.h"
#include "gui/Exhibition.h"
#include "helper/Boxes.h"
#include "helper/Plane.h"
#include "shader/Shader.h"

namespace Main050 {

    class Painter050 : public Painter {
    public:
        Painter050();
        ~Painter050() override;

        std::string title() override;

        std::shared_ptr<Camera> camera() override;

        void init() override;

        void draw(int w, int h) override;
    private:
        GLuint depthFBO{};
        GLuint depthMap{};
        GLuint screenVAO{};
        std::shared_ptr<Camera> camera_;
        std::unique_ptr<Boxes> box_;
        std::unique_ptr<Plane> plane_;
        std::unique_ptr<Shader> lightDepthShader_;
        std::unique_ptr<Shader> screenShader_;
        glm::mat4 lightView_{};
        glm::mat4 lightProjection_{};
    };

} // Main050

#endif //STUDY_OPENGL_MAIN050_H
