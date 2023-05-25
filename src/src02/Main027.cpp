#include "Main027.h"
#include "helper/Helper.h"
#include "helper/Camera.h"
#include "shader/Shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

// 聚光（手电筒）
namespace Main027 {

    int Main027::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

        createWindowWithCamera("hello Flashlight", camera);

        GLuint texture = createTexture("container2.png", true);
        CHECK_GL_ID(texture)
        GLuint textureSpecular = createTexture("container2_specular.png", true);
        CHECK_GL_ID(textureSpecular)

        Shader shader("src02/shader/027.vert.glsl", "src02/shader/027.frag.glsl");
        if(shader.hasError()){
            std::cout << shader.error() << std::endl;
            return 0;
        }

        glEnable(GL_DEPTH_TEST);

        // VAO
        GLuint VAO = createVAO();
        GLuint VBO = createVBO();
        glBufferData(GL_ARRAY_BUFFER, Helper::boxVerticesSize(), Helper::boxVertices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), (void*)Helper::boxVerticesTexCoordOffset());
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), (void*)Helper::boxVerticesNormalOffset());
        glEnableVertexAttribArray(2);


        shader.bind();
        shader.setUniformInt("material.diffuse", 0);
        shader.setUniformInt("material.specular", 1);
        shader.setUniformFloat("material.shininess", 32.0f);

        while(!glfwWindowShouldClose(window())){
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glBindVertexArray(VAO);

            shader.bind();

            shader.setUniformVec3("light.position", camera->Position);
            shader.setUniformVec3("light.direction", camera->Front);
            shader.setUniformFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
            shader.setUniformFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

            shader.setUniformVec3("light.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
            shader.setUniformVec3("light.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
            shader.setUniformVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
            shader.setUniformFloat("light.constant", 1.0f);
            shader.setUniformFloat("light.linear", 0.09f);
            shader.setUniformFloat("light.quadratic", 0.032f);

            shader.setUniformVec3("viewPos", camera->Position);

            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);

            shader.setUniformMat4("view", view);
            shader.setUniformMat4("projection", projection);


            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textureSpecular);

            for(int i=0; i<Helper::boxPositionLength(); i++){
                glm::vec3 pos = Helper::boxPositions()[i];
                glm::mat4 model(1.0f);
                model = glm::translate(model, pos);
                float angle = 20.0f * (float)i;
                if(angle < 1.0f){
                    angle = 60.0f;
                }
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

                shader.setUniformMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            glfwSwapBuffers(window());
            glfwPollEvents();
        }
        glDeleteTextures(1, &texture);
        glDeleteTextures(1, &textureSpecular);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glfwTerminate();
        return 0;
    }
} // Main025