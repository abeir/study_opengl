#include "Main004.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader/ShaderLoader.h"

namespace Main004 {

    const char *SHADER_VERTEX = "src01/shader/004.vert.glsl";
    const char *SHADER_FRAGMENT = "src01/shader/004.frag.glsl";

    int Main004::exec() {
        createWindow("hello triangle");

        std::cout << "glfw.version: " << glfwGetVersionString() << std::endl;
        std::cout << "gl.version: " << GLVersion.major << "." << GLVersion.minor << std::endl;

        // shader
        GLuint vertShader = createShader(GL_VERTEX_SHADER, SHADER_VERTEX);
        if (vertShader == 0) {
            std::cout << error() << std::endl;
            return 0;
        }
        GLuint fragShader = createShader(GL_FRAGMENT_SHADER, SHADER_FRAGMENT);
        if (fragShader == 0) {
            std::cout << error() << std::endl;
            return 0;
        }
        GLuint programId = createProgram(vertShader, fragShader);
        if (programId == 0) {
            std::cout << error() << std::endl;
            return 0;
        }


        float vertices[] = {
                -0.5, -0.5, 0.0,
                0.5, -0.5, 0.0,
                0.0, 0.5, 0.0
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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *) nullptr);
        glEnableVertexAttribArray(0);


        // mainloop

        while (!glfwWindowShouldClose(window())) {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            processInputEsc();

            auto timeValue = (float)glfwGetTime();
            float greenValue = std::sin(timeValue) / 2.0f + 0.5f;
            GLint vertColorId = glGetUniformLocation(programId, "vertColor");
            glUseProgram(programId);

            glUniform4f(vertColorId, 0.0f, greenValue, 0.0f, 1.0f);

            glBindVertexArray(VAO);

            glDrawArrays(GL_TRIANGLES, 0, 3);

            glBindVertexArray(0);

            glfwSwapBuffers(window());
            glfwPollEvents();
        }

        glDeleteProgram(programId);
        glDeleteBuffers(1, &VBO);
        glDeleteVertexArrays(1, &VAO);
        glfwTerminate();
        return 0;
    }

}