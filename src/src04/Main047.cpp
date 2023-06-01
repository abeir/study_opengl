#include "Main047.h"
#include "helper/Camera.h"
#include "helper/Boxes.h"
#include "helper/Helper.h"
#include "helper/Skybox.h"
#include <iostream>

namespace Main047 {

    int Main047::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        createWindowWithCamera("hello Skybox", camera);

        auto &boxVertices = Boxes::createVertices();
        auto boxTextures = Boxes::createTextures(Helper::inRes("container.jpg"));

        auto &skyboxVertices = Skybox::createVertices();
        std::map<Skybox::TextureFace, std::string> skyboxTexture = {
                { Skybox::Right, Helper::inRes("skybox/right.jpg") },
                { Skybox::Left, Helper::inRes("skybox/left.jpg") },
                { Skybox::Top, Helper::inRes("skybox/top.jpg") },
                { Skybox::Bottom, Helper::inRes("skybox/bottom.jpg") },
                { Skybox::Front, Helper::inRes("skybox/front.jpg") },
                { Skybox::Back, Helper::inRes("skybox/back.jpg") }
        };
        Skybox skybox(skyboxVertices);
        skybox.loadTextures(skyboxTexture);
        if(skybox.hasError()){
            std::cout << skybox.error() << std::endl;
            return 0;
        }

        Boxes boxes(boxVertices, boxTextures);


        Shader boxPlaneShader("src04/shader/047_box.vert.glsl", "src04/shader/047_box.frag.glsl");
        Shader skyboxShader("src04/shader/047_skybox.vert.glsl", "src04/shader/047_skybox.frag.glsl");

        glEnable(GL_DEPTH_TEST);

        while(!glfwWindowShouldClose(window())){

            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glm::mat4 model(1.0f);
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

            // 反射天空盒的box
            boxPlaneShader.bind();
            boxPlaneShader.setUniformVec3("cameraPos", camera->Position);
            boxPlaneShader.setUniformInt("skybox", 1);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.textureID());

            boxes.view = view;
            boxes.projection = projection;
            boxes.model = model;
            boxes.draw(boxPlaneShader);

            // skybox
            skybox.view = glm::mat4(glm::mat3(view));
            skybox.projection = projection;
            skybox.draw(skyboxShader);

            glfwSwapBuffers(window());
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
} // Main047