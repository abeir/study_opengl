#include "Main048.h"
#include "helper/Plane.h"
#include "helper/Boxes.h"
#include "helper/Camera.h"
#include "helper/Helper.h"


namespace Main048 {

    int Main048::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        createWindowWithCamera("hello uniform buffer", camera);

        std::vector<Boxes::Vertex>& boxVertices = Boxes::createVertices();
        std::vector<Boxes::Texture> boxTextures = Boxes::createTextures(Helper::inRes("marble.jpg"));
        Boxes boxes(boxVertices, boxTextures);
        boxes.useUBO();

        std::vector<Plane::Vertex>& planeVertices = Plane::createVertices();
        std::vector<Plane::Texture> planeTextures = Plane::createTextures(Helper::inRes("metal.png"));
        Plane plane(planeVertices, planeTextures);
        plane.useUBO();

        Shader boxPlaneShader("src04/shader/048.vert.glsl", "src04/shader/048.frag.glsl");

        // 将 Matrices 设置绑定点为 0
        // 创建缓存，为缓存设置绑定点 0
        GLuint UBO = boxPlaneShader.matricesBlockBinding("Matrices", 0);


        glEnable(GL_DEPTH_TEST);

        while(!glfwWindowShouldClose(window())){
            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 model(1.0f);
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);

            // 使用 UBO
            boxPlaneShader.setMatrices(UBO, view, projection);

            // plane
            plane.model = glm::mat4(1.0f);
            plane.draw(boxPlaneShader);

            // box 1
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            boxes.model = model;
            boxes.draw(boxPlaneShader);
            // box 2
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            boxes.model = model;
            boxes.draw(boxPlaneShader);


            glfwSwapBuffers(window());
            glfwPollEvents();
        }
        glfwTerminate();
        return 0;
    }
} // Main048