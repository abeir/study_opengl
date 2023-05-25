#include "Main007.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

namespace Main007 {
    const char* VERTEX_SHADER = "src01/shader/007.vert.glsl";
    const char* FRAGMENT_SHADER = "src01/shader/007.frag.glsl";

    const char* TEXTURE1 = "container.jpg";
    const char* TEXTURE2 = "awesomeface.png";

    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    glm::vec3 cubePositions[] = {
            glm::vec3( 0.0f,  0.0f,  0.0f),
            glm::vec3( 2.0f,  5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3( 2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f,  3.0f, -7.5f),
            glm::vec3( 1.3f, -2.0f, -2.5f),
            glm::vec3( 1.5f,  2.0f, -2.5f),
            glm::vec3( 1.5f,  0.2f, -1.5f),
            glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    void changeRatio(GLFWwindow* win, float &ratio){
        if(glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS){
            ratio += 0.001f;
        }else if(glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS){
            ratio -= 0.001f;
        }
        if(ratio < 0.0f){
            ratio = 0.0f;
        }
        if(ratio > 1.0f){
            ratio = 1.0f;
        }
    }


    int Main007::exec() {
        createWindow("hello Camera");
        GLuint vert = createShader(GL_VERTEX_SHADER, VERTEX_SHADER);
        if(vert == 0){
            std::cout << error() << std::endl;
            return 0;
        }
        GLuint frag = createShader(GL_FRAGMENT_SHADER, FRAGMENT_SHADER);
        if(frag == 0){
            std::cout << error() << std::endl;
            return 0;
        }
        GLuint program = createProgram(vert, frag);
        if(program == 0){
            std::cout << error() << std::endl;
            return 0;
        }
        GLuint texture1 = createTexture(TEXTURE1);
        GLuint texture2 = createTexture(TEXTURE2, true);

        // transform
        glm::mat4 view(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection(1.0f);
        projection = glm::perspective(glm::radians(450.f), (float)width()/(float)height(),  0.1f, 100.0f);



        glEnable(GL_DEPTH_TEST);

        // VAO
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // VBO
        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        glUseProgram(program);

        glUniform1i(glGetUniformLocation(program, "texture1"), 0);
        glUniform1i(glGetUniformLocation(program, "texture2"), 1);

//        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        auto modelLocation = glGetUniformLocation(program, "model");
        auto viewLocation = glGetUniformLocation(program, "view");

        float ratio = 0.5f;
        auto ratioLocation = glGetUniformLocation(program, "ratio");

        float radius = 10.0f;

        while(!glfwWindowShouldClose(window())){
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            processInputEsc();
            changeRatio(window(), ratio);

            glUniform1f(ratioLocation, ratio);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);

            glBindVertexArray(VAO);

            float camX = std::sin((float)glfwGetTime()) * radius;
            float camZ = std::cos((float)glfwGetTime()) * radius;

            view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0));
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

            for(int i=0; i<10; i++){
                glm::mat4 model(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = glm::radians(20.0f * (float)i);
                model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));

                glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

                glDrawArrays(GL_TRIANGLES, 0, 36);
            }
            glfwSwapBuffers(window());
            glfwPollEvents();
        }

        glDeleteTextures(1, &texture1);
        glDeleteTextures(1, &texture2);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteProgram(program);
        glfwTerminate();
        return 0;
    }
} // Main007