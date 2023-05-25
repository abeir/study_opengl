#include "Main042.h"
#include "shader/Shader.h"
#include "helper/Helper.h"
#include "helper/Boxes.h"
#include "helper/Plane.h"


namespace Main042 {


    int Main042::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        createWindowWithCamera("hello stencil", camera);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_NOTEQUAL, 1, 0xff);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        std::vector<Boxes::Vertex>& boxVertices = Boxes::createVertices();
        std::vector<Boxes::Texture> boxTextures = Boxes::createTextures(Helper::inRes("marble.jpg"));
        Boxes boxes(boxVertices, boxTextures);

        std::vector<Plane::Vertex>& planeVertices = Plane::createVertices();
        std::vector<Plane::Texture> planeTextures = Plane::createTextures(Helper::inRes("metal.png"));
        Plane plane(planeVertices, planeTextures);

        Shader boxPlaneShader("src04/shader/042_box_plane.vert.glsl", "src04/shader/042_box_plane.frag.glsl");
        Shader colorShader("src04/shader/042_color.vert.glsl", "src04/shader/042_color.frag.glsl");

        float scale = 1.1f;

        while(!glfwWindowShouldClose(window())){
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);

            glStencilMask(0x00);
            // plane
            plane.model = glm::mat4(1.0f);
            plane.view = view;
            plane.projection = projection;
            plane.draw(boxPlaneShader);

            boxes.view = view;
            boxes.projection = projection;

            // 第一次绘制box
            glStencilFunc(GL_ALWAYS, 1, 0xff);
            glStencilMask(0xff);
            // box 1
            glm::mat4 model(1.0f);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            boxes.model = model;
            boxes.draw(boxPlaneShader);
            // box 2
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            boxes.model = model;
            boxes.draw(boxPlaneShader);


            // 第二次绘制box
            glStencilFunc(GL_NOTEQUAL, 1, 0xff);
            glStencilMask(0x00);
            glDisable(GL_DEPTH_TEST);

            colorShader.bind();
            colorShader.setUniformVec3("color", glm::vec3(0.04f, 0.28f, 0.26f));
            // box 1
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            model = glm::scale(model, glm::vec3(scale, scale, scale));
            boxes.model = model;
            boxes.draw(colorShader);
            // box 2
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(scale, scale, scale));
            boxes.model = model;
            boxes.draw(colorShader);

            glStencilMask(0xFF);
            glStencilFunc(GL_ALWAYS, 0, 0xFF);
            glEnable(GL_DEPTH_TEST);

            glfwSwapBuffers(window());
            glfwPollEvents();
        }
        glfwTerminate();
        return 0;
    }
} // Main042