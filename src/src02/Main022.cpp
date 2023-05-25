#include "Main022.h"
#include "helper/Helper.h"
#include "helper/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace Main022 {
    const char* BOX_VERTEX = "src02/shader/022.vert.glsl";
    const char* BOX_FRAGMENT = "src02/shader/022.frag.glsl";

    const char* LIGHT_VERTEX = "src02/shader/022_light.vert.glsl";
    const char* LIGHT_FRAGMENT = "src02/shader/022_light.frag.glsl";


    GLuint Main022::createBoxShaderProgram() {
        GLuint vert = createShader(GL_VERTEX_SHADER, BOX_VERTEX);
        if(vert == 0){
            return 0;
        }
        GLuint frag = createShader(GL_FRAGMENT_SHADER, BOX_FRAGMENT);
        if(frag == 0){
            return 0;
        }
        GLuint program = createProgram(vert, frag);
        if(program == 0){
            return 0;
        }
        return program;
    }

    GLuint Main022::createLightShaderProgram() {
        GLuint vert = createShader(GL_VERTEX_SHADER, LIGHT_VERTEX);
        if(vert == 0){
            return 0;
        }
        GLuint frag = createShader(GL_FRAGMENT_SHADER, LIGHT_FRAGMENT);
        if(frag == 0){
            return 0;
        }
        GLuint program = createProgram(vert, frag);
        if(program == 0){
            return 0;
        }
        return program;
    }

    int Main022::exec() {
        createWindow("hello light");

        GLuint boxProgram = createBoxShaderProgram();
        if(boxProgram == 0){
            std::cout << error() << std::endl;
            return 0;
        }
        GLuint lightProgram = createLightShaderProgram();
        if(lightProgram == 0){
            std::cout << error() << std::endl;
            return 0;
        }

        glEnable(GL_DEPTH_TEST);

        // VAO
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // VBO
        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, Helper::boxVerticesSize(), Helper::boxVertices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), nullptr);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), (void*)Helper::boxVerticesTexCoordOffset());
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), (void*)Helper::boxVerticesNormalOffset());
        glEnableVertexAttribArray(2);

        // light
        GLuint lightVAO;
        glGenVertexArrays(1, &lightVAO);
        glBindVertexArray(lightVAO);

        GLuint lightVBO;
        glGenBuffers(1, &lightVBO);
        glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
        glBufferData(GL_ARRAY_BUFFER, Helper::boxVerticesSize(), Helper::boxVertices(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), nullptr);
        glEnableVertexAttribArray(0);


        Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
        glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
        float ambientStrength = 0.1f; // 环境光强度
        float specularStrength = 0.8f; // 镜面高光强度

        // world
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), widthF()/heightF(), 1.0f, 100.0f);

        // box
        glm::vec3 boxColor(1.0f, 0.5f, 0.31f);
        glm::mat4 boxModel(1.0f);
        boxModel = glm::rotate(boxModel, glm::radians(30.0f), glm::vec3(1.0f, 1.0f, 0.0f));


        while(!glfwWindowShouldClose(window())){
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            processInputEsc();

            lightPos.x = std::sin((float)glfwGetTime());
            lightPos.y = std::cos((float)glfwGetTime());
            lightPos.z = std::sin((float)glfwGetTime());

            glm::mat4 lightModel(1.0f);
            lightModel = glm::translate(lightModel, lightPos);
            lightModel = glm::scale(lightModel, glm::vec3(0.2f));

            glUseProgram(boxProgram);

            glUniformMatrix4fv(glGetUniformLocation(boxProgram, "model"), 1, GL_FALSE, glm::value_ptr(boxModel));
            glUniformMatrix4fv(glGetUniformLocation(boxProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(boxProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniform3fv(glGetUniformLocation(boxProgram, "boxColor"), 1, glm::value_ptr(boxColor));
            glUniform1f(glGetUniformLocation(boxProgram, "ambientStrength"), ambientStrength);
            glUniform1f(glGetUniformLocation(boxProgram, "specularStrength"), specularStrength);
            glUniform3fv(glGetUniformLocation(boxProgram, "lightColor"), 1, glm::value_ptr(lightColor));
            glUniform3fv(glGetUniformLocation(boxProgram, "lightPos"), 1, glm::value_ptr(lightPos));
            glUniform3fv(glGetUniformLocation(boxProgram, "viewPos"), 1, glm::value_ptr(camera.Position));

            glUseProgram(lightProgram);
            glUniformMatrix4fv(glGetUniformLocation(lightProgram, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
            glUniformMatrix4fv(glGetUniformLocation(lightProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(glGetUniformLocation(lightProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
            glUniform4fv(glGetUniformLocation(lightProgram, "vColor"), 1, glm::value_ptr(lightColor));

            glBindVertexArray(VAO);
            glUseProgram(boxProgram);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(lightVAO);
            glUseProgram(lightProgram);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_TRIANGLES, 0, 36);


            glfwSwapBuffers(window());
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }

} // Main021