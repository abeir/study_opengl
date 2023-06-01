#include "Main049.h"
#include "helper/Camera.h"
#include "helper/Helper.h"
#include "helper/Model.h"
#include "shader/Shader.h"
#include <iostream>


namespace Main049 {

    const int ROCK_AMOUNT = 8000;
    const float ROCK_RADIUS = 150.0f;
    const float ROCK_OFFSET = 25.0f;

    void genRockModelMatrices(std::vector<glm::mat4> &modelMatrices){
        srand((unsigned int)glfwGetTime());

        for(int i=0; i<ROCK_AMOUNT; i++){
            glm::mat4 model(1.0f);
            // 1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
            float angle = (float)i / (float)ROCK_AMOUNT * 360.0f;
            float displacement = (rand() % (int)(2 * ROCK_OFFSET * 100)) / 100.0f - ROCK_OFFSET;
            float x = sin(angle) * ROCK_RADIUS + displacement;
            displacement = (rand() % (int)(2 * ROCK_OFFSET * 100)) / 100.0f - ROCK_OFFSET;
            float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
            displacement = (rand() % (int)(2 * ROCK_OFFSET * 100)) / 100.0f - ROCK_OFFSET;
            float z = cos(angle) * ROCK_RADIUS + displacement;
            model = glm::translate(model, glm::vec3(x, y, z));

            // 2. 缩放：在 0.05 和 0.25f 之间缩放
            float scale = (rand() % 20) / 100.0f + 0.05;
            model = glm::scale(model, glm::vec3(scale));

            // 3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转
            float rotAngle = (rand() % 360);
            model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

            // 4. 添加到矩阵的数组中
            modelMatrices.emplace_back(model);
        }
    }


    int Main049::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 55.0f));
        createWindowWithCamera("hello instance", camera);

        Shader planetShader("src04/shader/049_planet.vert.glsl", "src04/shader/049_planet.frag.glsl");
        if(planetShader.hasError()){
            std::cout << planetShader.error() << std::endl;
            return 0;
        }
        Shader rockShader("src04/shader/049_rock.vert.glsl", "src04/shader/049_rock.frag.glsl");
        if(rockShader.hasError()){
            std::cout << rockShader.error() << std::endl;
            return 0;
        }

        Model planetModel(Helper::inRes("planet/planet.obj"));
        if(!planetModel.load()){
            std::cout << planetModel.error() << std::endl;
            return 0;
        }
        Model rockModel(Helper::inRes("rock/rock.obj"));
        if(!rockModel.load()){
            std::cout << rockModel.error() << std::endl;
            return 0;
        }

        std::vector<glm::mat4> rockModelMatrices;
        genRockModelMatrices(rockModelMatrices);

        GLuint buffer;
        glGenBuffers(1, &buffer);
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4)*rockModelMatrices.size(), &rockModelMatrices[0], GL_STATIC_DRAW);

        GLsizei vec4Size = sizeof(glm::vec4);
        for(auto &mesh : rockModel.meshes()){
            GLuint rockVAO = mesh.vao();
            glBindVertexArray(rockVAO);

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, nullptr);
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
            glEnableVertexAttribArray(5);
            glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

            glVertexAttribDivisor(3, 1);
            glVertexAttribDivisor(4, 1);
            glVertexAttribDivisor(5, 1);
            glVertexAttribDivisor(6, 1);

            glBindVertexArray(0);
        }


        glEnable(GL_DEPTH_TEST);

        while(!glfwWindowShouldClose(window())) {
            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 model(1.0f);
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 1000.0f);

            // planet
            model = glm::translate(model, glm::vec3(0.0f, -3.0f, 0.0f));
            model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));

            planetShader.bind();
            planetShader.setUniformMat4("model", model);
            planetShader.setUniformMat4("view", view);
            planetShader.setUniformMat4("projection", projection);

            planetModel.draw(planetShader);

            // rock
            rockShader.bind();
            rockShader.setUniformMat4("view", view);
            rockShader.setUniformMat4("projection", projection);
            rockShader.setUniformInt("material.texture_diffuse0", 0);

            glActiveTexture(GL_TEXTURE0);
            // TODO 此处 rockModel.textures() 为空？？？
            glBindTexture(GL_TEXTURE_2D, rockModel.textures()[0].id);
            for(auto &mesh : rockModel.meshes())
            {
                glBindVertexArray(mesh.vao());
                glDrawElementsInstanced(GL_TRIANGLES, mesh.indices().size(), GL_UNSIGNED_INT, 0, rockModelMatrices.size());
                glBindVertexArray(0);
            }


            glfwSwapBuffers(window());
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
} // Main049