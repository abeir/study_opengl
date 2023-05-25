#include "Main021.h"
#include "helper/Helper.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace Main021 {
    const char* BOX_VERTEX = "src02/shader/021.vert.glsl";
    const char* BOX_FRAGMENT = "src02/shader/021.frag.glsl";

    const char* LIGHT_VERTEX = "src02/shader/021_light.vert.glsl";
    const char* LIGIT_FRAGMENT = "src02/shader/021_light.frag.glsl";

    const char* TEXTURE = "awesomeface.png";

    GLuint Main021::createBoxShaderProgram() {
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

    GLuint Main021::createLightShaderProgram() {
        GLuint vert = createShader(GL_VERTEX_SHADER, LIGHT_VERTEX);
        if(vert == 0){
            return 0;
        }
        GLuint frag = createShader(GL_FRAGMENT_SHADER, LIGIT_FRAGMENT);
        if(frag == 0){
            return 0;
        }
        GLuint program = createProgram(vert, frag);
        if(program == 0){
            return 0;
        }
        return program;
    }

    int Main021::exec() {
        createWindow("hello light");

        GLuint texture = createTexture(TEXTURE, true);
        if(texture == 0){
            std::cout << error() << std::endl;
            return 0;
        }
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

        glBindVertexArray(VAO);

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
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, Helper::boxVerticesStride(), (void*)Helper::boxVerticesTexCoordOffset());
        glEnableVertexAttribArray(1);

        glBindVertexArray(lightVAO);


        // transform
        glm::mat4 model(1.0f);
        model = glm::rotate(model, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, -1.0f, -4.0f));
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(60.0f), widthF()/heightF(), 1.0f, 100.0f);

        glm::mat4 lightModel(1.0f);
        lightModel = glm::translate(lightModel, glm::vec3(1.5f, 1.0f, -2.0f));
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));


        glm::vec4 lightColor(1.0f, 1.0f, 0.0f, 1.0f);

        glUseProgram(boxProgram);
        glUniformMatrix4fv(glGetUniformLocation(boxProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(boxProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(boxProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform4fv(glGetUniformLocation(boxProgram, "lightColor"), 1, glm::value_ptr(lightColor));
        glUniform1i(glGetUniformLocation(boxProgram, "uTexture"), 0);

        glUseProgram(lightProgram);
        glUniformMatrix4fv(glGetUniformLocation(lightProgram, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
        glUniformMatrix4fv(glGetUniformLocation(lightProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lightProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform4fv(glGetUniformLocation(lightProgram, "vColor"), 1, glm::value_ptr(lightColor));


        while(!glfwWindowShouldClose(window())){
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            processInputEsc();


            glBindVertexArray(VAO);
            glUseProgram(boxProgram);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture);

            glDrawArrays(GL_TRIANGLES, 0, 36);

            glBindVertexArray(lightVAO);
            glUseProgram(lightProgram);
            glDrawArrays(GL_TRIANGLES, 0, 36);


            glfwSwapBuffers(window());
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }

} // Main021