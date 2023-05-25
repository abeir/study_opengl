#include "Main024.h"
#include "helper/Helper.h"
#include "helper/Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace Main024 {
    const char* BOX_VERTEX = "src02/shader/024.vert.glsl";
    const char* BOX_FRAGMENT = "src02/shader/024.frag.glsl";

    const char* LIGHT_VERTEX = "src02/shader/024_light.vert.glsl";
    const char* LIGHT_FRAGMENT = "src02/shader/024_light.frag.glsl";

    const char* TEXTURE = "container2.png";
    const char* TEXTURE_SPECULAR = "container2_specular.png";
    const char* TEXTURE_EMISSION = "matrix.jpg";

    GLuint Main024::createBoxShaderProgram() {
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

    GLuint Main024::createLightShaderProgram() {
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

    int Main024::exec() {
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
        GLuint texture = createTexture(TEXTURE, true);
        if(texture == 0){
            std::cout << error() << std::endl;
            return 0;
        }
        GLuint textureSpecular = createTexture(TEXTURE_SPECULAR, true);
        if(textureSpecular == 0){
            std::cout << error() << std::endl;
            return 0;
        }
        GLuint textureEmission = createTexture(TEXTURE_EMISSION);
        if(textureEmission == 0){
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

        // world
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), widthF()/heightF(), 1.0f, 100.0f);

        // box
        glm::mat4 boxModel(1.0f);
        boxModel = glm::rotate(boxModel, glm::radians(30.0f), glm::vec3(1.0f, 1.0f, 0.0f));


        glUseProgram(boxProgram);
        glUniformMatrix4fv(glGetUniformLocation(boxProgram, "model"), 1, GL_FALSE, glm::value_ptr(boxModel));
        glUniformMatrix4fv(glGetUniformLocation(boxProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(boxProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(glGetUniformLocation(boxProgram, "viewPos"), 1, glm::value_ptr(camera.Position));

        glUniform1i(glGetUniformLocation(boxProgram, "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(boxProgram, "material.specular"), 1);
        glUniform1i(glGetUniformLocation(boxProgram, "material.emission"), 2);
        glUniform1f(glGetUniformLocation(boxProgram, "material.shininess"), 32.0f);


        glUseProgram(lightProgram);
        glUniformMatrix4fv(glGetUniformLocation(lightProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


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

//            lightColor.x = std::sin((float)glfwGetTime() * 2.0f);
//            lightColor.y = std::sin((float)glfwGetTime() * 0.7f);
//            lightColor.z = std::sin((float)glfwGetTime() * 1.3f);

            glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
            glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);

            glBindVertexArray(VAO);
            glUseProgram(boxProgram);
            glUniform3fv(glGetUniformLocation(boxProgram, "light.position"), 1, glm::value_ptr(lightPos));
            glUniform3fv(glGetUniformLocation(boxProgram, "light.ambient"), 1, glm::value_ptr(ambientColor));
            glUniform3fv(glGetUniformLocation(boxProgram, "light.diffuse"), 1, glm::value_ptr(diffuseColor));
            glUniform3f(glGetUniformLocation(boxProgram, "light.specular"), 1.0f, 1.0f, 1.0f);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textureSpecular);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, textureEmission);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(lightVAO);
            glUseProgram(lightProgram);
            glUniformMatrix4fv(glGetUniformLocation(lightProgram, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
            glUniform4fv(glGetUniformLocation(lightProgram, "vColor"), 1, glm::value_ptr(lightColor));
            glDrawArrays(GL_TRIANGLES, 0, 36);


            glfwSwapBuffers(window());
            glfwPollEvents();
        }

        glDeleteTextures(1, &texture);
        glDeleteTextures(1, &textureSpecular);
        glDeleteProgram(boxProgram);
        glDeleteProgram(lightProgram);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &lightVBO);
        glDeleteVertexArrays(1, &VAO);
        glDeleteVertexArrays(1, &lightVAO);
        glfwTerminate();
        return 0;
    }

} // Main021