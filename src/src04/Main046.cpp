#include "Main046.h"
#include "helper/Camera.h"
#include "helper/Boxes.h"
#include "helper/Plane.h"
#include "helper/Helper.h"
#include <iostream>


namespace Main046 {

    float screenVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };

    int Main046::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        createWindowWithCamera("hello framebuffer", camera);

        auto &boxVertices = Boxes::createVertices();
        auto boxTextures = Boxes::createTextures(Helper::inRes("container.jpg"));

        auto &planeVertices = Plane::createVertices();
        auto planeTextures = Plane::createTextures(Helper::inRes("metal.png"));

        Boxes boxes(boxVertices, boxTextures);
        Plane plane(planeVertices, planeTextures);

        Shader boxPlaneShader("src04/shader/046_box_plane.vert.glsl", "src04/shader/046_box_plane.frag.glsl");
        Shader screenShader("src04/shader/046_screen.vert.glsl", "src04/shader/046_screen.frag.glsl");
        if(screenShader.hasError()){
            std::cout << screenShader.error() << std::endl;
            return 0;
        }

        // 帧缓存
        GLuint framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // 纹理
        GLuint texColorBuffer;
        glGenTextures(1, &texColorBuffer);
        glBindTexture(GL_TEXTURE_2D, texColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        // 附加纹理到帧缓存
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

        // 渲染缓冲
        GLuint RBO;
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width(), height());
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        // 附加渲染缓冲到帧缓存
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        // 检查帧缓存完整性
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 屏幕
        GLuint screenVAO;
        glGenVertexArrays(1, &screenVAO);
        glBindVertexArray(screenVAO);

        GLuint screenVBO;
        glGenBuffers(1, &screenVBO);
        glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)(sizeof(float)*2));

        glBindVertexArray(0);

        screenShader.bind();
        screenShader.setUniformInt("screenTexture", 0);


        while(!glfwWindowShouldClose(window())){

            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glm::mat4 model(1.0f);
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);

            // 1. 在帧缓存中绘制
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glEnable(GL_DEPTH_TEST);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // plane
            plane.model = glm::mat4(1.0f);
            plane.view = view;
            plane.projection = projection;
            plane.draw(boxPlaneShader);

            boxes.view = view;
            boxes.projection = projection;
            // box 1
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            boxes.model = model;
            boxes.draw(boxPlaneShader);
            // box 2
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
            boxes.model = model;
            boxes.draw(boxPlaneShader);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            // 2. 将帧缓存绘制到屏幕
            glDisable(GL_DEPTH_TEST);

            glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

//            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            screenShader.bind();
            glBindVertexArray(screenVAO);
            glBindTexture(GL_TEXTURE_2D, texColorBuffer);
            glDrawArrays(GL_TRIANGLES, 0, 6);



            glfwSwapBuffers(window());
            glfwPollEvents();
        }

        glDeleteVertexArrays(1, &screenVAO);
        glDeleteBuffers(1, &screenVBO);
        glDeleteRenderbuffers(1, &RBO);
        glDeleteFramebuffers(1, &framebuffer);
        glfwTerminate();
        return 0;
    }
} // Main046