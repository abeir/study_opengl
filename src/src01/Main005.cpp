#include "Main005.h"
#include <iostream>

namespace Main005 {
    const char* VERTEX_SHADER = "src01/shader/005.vert.glsl";
    const char* FRAGMENT_SHADER = "src01/shader/005.frag.glsl";

    const char* TEXTURE1 = "container.jpg";
    const char* TEXTURE2 = "awesomeface.png";

    void changeRatio(float &ratio, GLFWwindow* win){
        if(glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS){
            ratio += 0.001f;
            ratio = ratio > 1.0f ? 1.0f : ratio;
        }else if(glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS){
            ratio -= 0.001f;
            ratio = ratio < 0.0f ? 0.0f : ratio;
        }
    }

    int Main005::exec() {
        createWindow("2 textures & color");

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

        float vertices[] = {
                -0.5, -0.5,    0.8, 0.0, 0.0,    0.0, 0.0,
                0.5, -0.5,     0.0, 0.8, 0.0,    1.0, 0.0,
                0.5, 0.5,      0.0, 0.0, 0.8,    1.0, 1.0,
                -0.5, 0.5,     0.6, 0.6, 0.0,    0.0, 1.0
        };
        unsigned int indices[] = {
                0, 1, 2,
                0, 2, 3
        };

        // VAO;
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // VBO;
        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // EBO;
        GLuint EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*7, nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*7, (void*)(sizeof(float)*2));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float)*7, (void*)(sizeof(float)*5));
        glEnableVertexAttribArray(2);


        GLuint texture1 = createTexture(TEXTURE1);
        if(texture1 == 0){
            std::cout << error() << std::endl;
            return 0;
        }
        GLuint texture2 = createTexture(TEXTURE2, true);
        if(texture2 == 0){
            std::cout << error() << std::endl;
            return 0;
        }

        glUseProgram(program);
        GLint ratioLocation = glGetUniformLocation(program, "ratio");

        glUniform1i(glGetUniformLocation(program, "texture1"), 0);
        glUniform1i(glGetUniformLocation(program, "texture2"), 1);

        float ratio = 0.5f;
        while(!glfwWindowShouldClose(window())){
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            processInputEsc();
            changeRatio(ratio, window());

            glUniform1f(ratioLocation, ratio);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);

            glBindVertexArray(VAO);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

            glfwSwapBuffers(window());
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteProgram(program);
        glfwTerminate();
        return 0;
    }
} // Main005