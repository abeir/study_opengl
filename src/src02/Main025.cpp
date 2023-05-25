#include "Main025.h"
#include "helper/Helper.h"
#include "helper/Camera.h"
#include "shader/Shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define MAIN025_CHECK_GL_ID(id) \
    if(id == 0){ \
        std::cout << error() << std::endl; \
        return 0; \
    }

// 平行光
namespace Main025 {
    const char* VERT_SHADER = "src02/shader/025.vert.glsl";
    const char* FRAG_SHADER = "src02/shader/025.frag.glsl";

    const char* TEXTURE = "container2.png";
    const char* TEXTURE_SPECULAR = "container2_specular.png";

    int Main025::exec() {
        createWindow("hello Light casters");

        GLuint texture = createTexture(TEXTURE, true);
        MAIN025_CHECK_GL_ID(texture)
        GLuint textureSpecular = createTexture(TEXTURE_SPECULAR, true);
        MAIN025_CHECK_GL_ID(textureSpecular)

        Shader shader(VERT_SHADER, FRAG_SHADER);

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


        Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
        // 光线方向
        glm::vec3 lightDirection(-0.2f, -1.0f, -0.3f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.7f);
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

        shader.setUniformVec3("light.direction", lightDirection);
        shader.setUniformVec3("light.ambient", ambientColor);
        shader.setUniformVec3("light.diffuse", diffuseColor);
        shader.setUniformVec3("light.specular", specularColor);

        while(!glfwWindowShouldClose(window())){
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            processInputEsc();


            glBindVertexArray(VAO);
            shader.bind();

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