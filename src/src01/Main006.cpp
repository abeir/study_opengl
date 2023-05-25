#include "Main006.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>


namespace Main006 {
    const char* VERTEX_SHADER = "src01/shader/006.vert.glsl";
    const char* FRAGMENT_SHADER = "src01/shader/006.frag.glsl";

    const char* TEXTURE1 = "container.jpg";
    const char* TEXTURE2 = "awesomeface.png";

    int Main006::exec() {
        createWindow("hello transformation");

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
                -0.5, -0.5, 0.0,    0.0, 0.0,
                 0.5, -0.5, 0.0,    1.0, 0.0,
                 0.5,  0.5, 0.0,    1.0, 1.0,
                -0.5,  0.5, 0.0,    0.0, 1.0
        };
        unsigned int indices[] = {
                0, 1, 2,
                0, 2, 3
        };

        // textures
        GLuint texture1 = createTexture(TEXTURE1);
        GLuint texture2 = createTexture(TEXTURE2, true);

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

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), nullptr);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        // transform
        glm::mat4 t1(1.0f);
        t1 = glm::translate(t1, glm::vec3(0.5f, -0.5f, 0.0f));
        t1 = glm::rotate(t1, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                std::cout << t1[j][i] << "    ";
            }
            std::cout << std::endl;
        }

        std::cout << "--------------" << std::endl;

        t1 = glm::mat4(1.0f);
        t1 = glm::rotate(t1, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
        t1 = glm::translate(t1, glm::vec3(0.5f, -0.5f, 0.0f));

        for(int i=0; i<4; i++){
            for(int j=0; j<4; j++){
                std::cout << t1[j][i] << "    ";
            }
            std::cout << std::endl;
        }


        glUseProgram(program);

        glUniform1i(glGetUniformLocation(program, "texture1"), 0);
        glUniform1i(glGetUniformLocation(program, "texture2"), 1);

        glUniform1f(glGetUniformLocation(program, "ratio"), 0.2f);

        while(!glfwWindowShouldClose(window())){
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            processInputEsc();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);

            glm::mat4 t(1.0f);
//            t = glm::translate(t, glm::vec3(0.5f, -0.5f, 0.0f));
//            t = glm::rotate(t, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
            t = glm::rotate(t, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));
            t = glm::translate(t, glm::vec3(0.5f, -0.5f, 0.0f));


            glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, glm::value_ptr(t));

            glBindVertexArray(VAO);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);


            glm::mat4 t2(1.0f);
            float sc = std::abs(std::sin((float)glfwGetTime()));
            t2 = glm::scale(t2, glm::vec3(sc, sc, 1.0));
            t2 = glm::translate(t2, glm::vec3(-0.5, 0.5, 0.0));

            glUniformMatrix4fv(glGetUniformLocation(program, "transform"), 1, GL_FALSE, glm::value_ptr(t2));

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
} // Main006