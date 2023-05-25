#include "Main026.h"
#include "helper/Helper.h"
#include "helper/Camera.h"
#include "shader/Shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// 点光源
namespace Main026 {

    int Main026::exec() {
        createWindow("hello Point Light");

        GLuint texture = createTexture("container2.png", true);
        CHECK_GL_ID(texture)
        GLuint textureSpecular = createTexture("container2_specular.png", true);
        CHECK_GL_ID(textureSpecular)

        Shader shader("src02/shader/026.vert.glsl", "src02/shader/026.frag.glsl");
        if(shader.hasError()){
            std::cout << shader.error() << std::endl;
            return 0;
        }
        Shader lightShader("src02/shader/026_light.vert.glsl", "src02/shader/026_light.frag.glsl");
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

        // light VAO
        GLuint lightVAO = createVAO();
        GLuint lightVBO = createVBO();
        glBufferData(GL_ARRAY_BUFFER, Helper::boxVerticesSize(), Helper::boxVertices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), nullptr);
        glEnableVertexAttribArray(0);


        Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));
        // 光线方向
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.6f);
        glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);
        glm::vec3 specularColor(1.0f, 1.0f, 1.0f);

        // world
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), widthF()/heightF(), 1.0f, 100.0f);


        shader.bind();
        shader.setUniformMat4("view", view);
        shader.setUniformMat4("projection", projection);

        shader.setUniformInt("material.diffuse", 0);
        shader.setUniformInt("material.specular", 1);
        shader.setUniformFloat("material.shininess", 32.0f);

        shader.setUniformVec3("light.ambient", ambientColor);
        shader.setUniformVec3("light.diffuse", diffuseColor);
        shader.setUniformVec3("light.specular", specularColor);
        shader.setUniformFloat("light.constant", 1.0f);
        shader.setUniformFloat("light.linear", 0.07f);
        shader.setUniformFloat("light.quadratic", 0.017f);

        lightShader.bind();
        lightShader.setUniformMat4("view", view);
        lightShader.setUniformMat4("projection", projection);
        lightShader.setUniformVec3("vColor", lightColor);

        while(!glfwWindowShouldClose(window())){
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            processInputEsc();

            lightPos.x = std::sin((float)glfwGetTime()) * 3.0f;
            lightPos.y = std::cos((float)glfwGetTime()) * 3.0f;
            lightPos.z = std::sin((float)glfwGetTime()) * 3.0f;

            glm::mat4 lightModel(1.0f);
            lightModel = glm::translate(lightModel, lightPos);
            lightModel = glm::scale(lightModel, glm::vec3(0.2f));


            glBindVertexArray(VAO);
            shader.bind();
            shader.setUniformVec3("light.position", lightPos);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textureSpecular);

            for(int i=0; i<Helper::boxPositionLength(); i++){
                glm::vec3 pos = Helper::boxPositions()[i];
                glm::mat4 model(1.0f);
                model = glm::translate(model, pos);
                float angle = 20.0f * (float)i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

                shader.setUniformMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            glBindVertexArray(lightVAO);
            lightShader.bind();
            lightShader.setUniformMat4("model", lightModel);
            glDrawArrays(GL_TRIANGLES, 0, 36);


            glfwSwapBuffers(window());
            glfwPollEvents();
        }
        glDeleteTextures(1, &texture);
        glDeleteTextures(1, &textureSpecular);
        glDeleteVertexArrays(1, &VAO);
        glDeleteVertexArrays(1, &lightVAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &lightVBO);
        glfwTerminate();
        return 0;
    }
} // Main025