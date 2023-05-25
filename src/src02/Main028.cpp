#include "Main028.h"
#include "helper/Helper.h"
#include "helper/Camera.h"
#include "shader/Shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <cstring>

// 聚光（手电筒）
namespace Main028 {

    char* pointLightName(char* in, size_t inSize, int index, const char* prop){
        memset(in, 0, inSize);
        sprintf(in, "pointLights[%d].%s", index, prop);
        return in;
    }

    int Main028::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));

        createWindowWithCamera("hello Flashlight", camera);

        GLuint texture = createTexture("container2.png", true);
        CHECK_GL_ID(texture)
        GLuint textureSpecular = createTexture("container2_specular.png", true);
        CHECK_GL_ID(textureSpecular)

        Shader shader("src02/shader/028.vert.glsl", "src02/shader/028.frag.glsl");
        if(shader.hasError()){
            std::cout << shader.error() << std::endl;
            return 0;
        }
        Shader lightBoxShader("src02/shader/026_light.vert.glsl", "src02/shader/026_light.frag.glsl");
        if(lightBoxShader.hasError()){
            std::cout << lightBoxShader.error() << std::endl;
            return 0;
        }

        glEnable(GL_DEPTH_TEST);

        // box
        GLuint VAO = createVAO();
        GLuint VBO = createVBO();
        glBufferData(GL_ARRAY_BUFFER, Helper::boxVerticesSize(), Helper::boxVertices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), (void*)Helper::boxVerticesTexCoordOffset());
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), (void*)Helper::boxVerticesNormalOffset());
        glEnableVertexAttribArray(2);

        // light box
        GLuint lightBoxVAO = createVAO();
        GLuint lightBoxVBO = createVBO();
        glBufferData(GL_ARRAY_BUFFER, Helper::boxVerticesSize(), Helper::boxVertices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), nullptr);
        glEnableVertexAttribArray(0);

        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 pointLightAmbientColor = lightColor * glm::vec3(0.05f);
        glm::vec3 pointLightDiffuseColor = lightColor * glm::vec3(0.8f);
        glm::vec3 spotLightAmbientColor(0.0f);
        glm::vec3 spotLightDiffuseColor = lightColor;

        lightBoxShader.bind();
        lightBoxShader.setUniformVec3("vColor", lightColor);


        shader.bind();
        shader.setUniformInt("material.diffuse", 0);
        shader.setUniformInt("material.specular", 1);
        shader.setUniformFloat("material.shininess", 32.0f);

        // 平行光
        shader.setUniformVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
        shader.setUniformVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        shader.setUniformVec3("dirLight.diffuse", glm::vec3(0.4f, 0.4f, 0.4f));
        shader.setUniformVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

        // 点光源
        int pointLightCount = 4;
        glm::vec3 pointLightPositions[] = {
                glm::vec3( 0.7f,  0.2f,  2.0f),
                glm::vec3( 2.3f, -3.3f, -4.0f),
                glm::vec3(-4.0f,  2.0f, -12.0f),
                glm::vec3( 0.0f,  0.0f, -3.0f)
        };

        for(int i=0; i<pointLightCount; i++){
            glm::vec3 pos = pointLightPositions[i];
            char name[64] = {0};
            shader.setUniformVec3(pointLightName(name, 64, i, "position"), pos);
            shader.setUniformVec3(pointLightName(name, 64, i, "ambient"), pointLightAmbientColor);
            shader.setUniformVec3(pointLightName(name, 64, i, "diffuse"), pointLightDiffuseColor);
            shader.setUniformVec3(pointLightName(name, 64, i, "specular"), lightColor);
            shader.setUniformFloat(pointLightName(name, 64, i, "constant"), 1.0f);
            shader.setUniformFloat(pointLightName(name, 64, i, "linear"), 0.09f);
            shader.setUniformFloat(pointLightName(name, 64, i, "quadratic"), 0.032f);
        }

        // 聚光
        shader.setUniformVec3("spotLight.ambient", spotLightAmbientColor);
        shader.setUniformVec3("spotLight.diffuse", spotLightDiffuseColor);
        shader.setUniformVec3("spotLight.specular", lightColor);
        shader.setUniformFloat("spotLight.constant", 1.0f);
        shader.setUniformFloat("spotLight.linear", 0.09f);
        shader.setUniformFloat("spotLight.quadratic", 0.032f);
        shader.setUniformFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        shader.setUniformFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


        while(!glfwWindowShouldClose(window())){
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);

            // light box
            glBindVertexArray(lightBoxVAO);
            lightBoxShader.bind();
            lightBoxShader.setUniformMat4("view", view);
            lightBoxShader.setUniformMat4("projection", projection);
            for(int i=0; i<pointLightCount; i++){
                glm::vec3 pos = pointLightPositions[i];
                glm::mat4 model(1.0f);
                model = glm::translate(model, pos);
                model = glm::scale(model, glm::vec3(0.2f));
                lightBoxShader.setUniformMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }


            // box
            glBindVertexArray(VAO);

            shader.bind();
            shader.setUniformVec3("spotLight.position", camera->Position);
            shader.setUniformVec3("spotLight.direction", camera->Front);

            shader.setUniformVec3("viewPos", camera->Position);

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
        glDeleteVertexArrays(1, &lightBoxVAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &lightBoxVBO);
        glfwTerminate();
        return 0;
    }
} // Main028