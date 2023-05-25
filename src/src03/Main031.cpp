#include "Main031.h"
#include "helper/Model.h"
#include "helper/Helper.h"
#include "shader/Shader.h"
#include <iostream>


namespace Main031 {
    const char* NANOSUIT_OBJ = "nanosuit/nanosuit.obj";


    int Main031::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        createWindowWithCamera("hello nanosuit", camera);

        glEnable(GL_DEPTH_TEST);

        Shader shader("src03/shader/031.vert.glsl", "src03/shader/031.frag.glsl");

        std::string objPath = Helper::inRes(NANOSUIT_OBJ);

        Model ourModel(objPath);
        if(!ourModel.load()){
            std::cout << ourModel.error() << std::endl;
            return 0;
        }


        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

        while(!glfwWindowShouldClose(window())){
            glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);

            shader.bind();
            shader.setUniformMat4("model", model);
            shader.setUniformMat4("view", view);
            shader.setUniformMat4("projection", projection);

            ourModel.draw(shader);

            glfwSwapBuffers(window());
            glfwPollEvents();
        }
        glfwTerminate();
        return 0;
    }
} // Main031