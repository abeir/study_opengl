#include "Main003.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader/ShaderLoader.h"
#include <iostream>


namespace Main003 {
    const char *SHADER_VERTEX = "src01/shader/003.vert.glsl";
    const char *SHADER_FRAGMENT = "src01/shader/003.frag.glsl";

    int Main003::exec() {
        createWindow("hello multi triangles");

        GLuint vert = createShader(GL_VERTEX_SHADER, SHADER_VERTEX);
        if(vert == 0){
            std::cout << error() << std::endl;
            return 0;
        }
        GLuint frag = createShader(GL_FRAGMENT_SHADER, SHADER_FRAGMENT);
        if(frag == 0){
            std::cout << error() << std::endl;
            return 0;
        }
        GLuint programId = createProgram(vert, frag);

        // 第一个三角形组
        float vertices[] = {
                0.0, 0.5, 0.0,   1.0f, 0.0f, 0.0f, 1.0f,
                -0.5, 0.0, 0.0,  0.0f, 1.0f, 0.0f, 1.0f,
                0.0, 0.0, 0.0,   0.0f, 0.0f, 1.0f, 1.0f,
                0.0, -0.5, 0.0,  0.0f, 1.0f, 0.0f, 1.0f,
                0.5, 0.0, 0.0,   1.0f, 0.0f, 0.0f, 1.0f
        };
        unsigned int indices[] = {
                0, 1, 2,
                2, 3, 4
        };
        // VAO
        GLuint VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        // VBO
        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // EBO
        GLuint EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*7, nullptr);
        glEnableVertexAttribArray(0);

        GLintptr colorOffset = 3 * sizeof(float);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*7, (GLvoid*)colorOffset);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        // 第二个三角形组
        float vertices2[] = {
                0.5, 0.0, 0.0,   0.5f, 0.5f, 0.0f, 1.0f,
                1.0, 0.5, 0.0,  0.0f, 0.5f, 0.5f, 1.0f,
                0.0, 0.5, 0.0,   0.5f, 0.0f, 0.5f, 1.0f,
                0.5, 1.0, 0.0,   0.5f, 0.5f, 0.0f, 1.0f
        };
        unsigned int indices2[] = {
                0, 1, 2,
                2, 1, 3
        };

        // VAO 2
        GLuint VAO2;
        glGenVertexArrays(1, &VAO2);
        glBindVertexArray(VAO2);

        // VBO 2
        GLuint VBO2;
        glGenBuffers(1, &VBO2);
        glBindBuffer(GL_ARRAY_BUFFER, VBO2);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

        // EBO 2
        GLuint EBO2;
        glGenBuffers(1, &EBO2);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*7, nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float)*7, (GLvoid*)colorOffset);
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        while(!glfwWindowShouldClose(window())){
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            processInputEsc();

            glUseProgram(programId);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);

            glBindVertexArray(VAO2);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);


            glfwSwapBuffers(window());
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
        glDeleteProgram(programId);
        glfwTerminate();
        return 0;
    }

}