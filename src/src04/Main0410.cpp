#include "Main0410.h"
#include "helper/Boxes.h"

#include <iostream>


// MSAA 离屏渲染
namespace Main0410 {

    float screenVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };

    int Main0410::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        createWindowWithCamera("hello MSAA", camera);

        std::vector<Boxes::Vertex>& boxVertices = Boxes::createVertices();

        Boxes boxes(boxVertices, {});
        Shader boxShader("src04/shader/0410_box.vert.glsl", "src04/shader/0410_box.frag.glsl");
        if(boxShader.hasError()){
            std::cout << boxShader.error() << std::endl;
            return 1;
        }
        Shader screenShader("src04/shader/0410_screen.vert.glsl", "src04/shader/0410_screen.frag.glsl");
        if(screenShader.hasError()){
            std::cout << screenShader.error() << std::endl;
            return 1;
        }

        // 屏幕
        GLuint screenVAO, screenVBO;
        glGenVertexArrays(1, &screenVAO);
        glBindVertexArray(screenVAO);
        glGenBuffers(1, &screenVBO);
        glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));



        const int sampleCount = 4;

        // 创建帧缓存
        GLuint framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // 创建多重采样纹理
        GLuint textureBuffer;
        glGenTextures(1, &textureBuffer);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureBuffer);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, sampleCount, GL_RGB, width(), height(), GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        // 将多重采样纹理附加到帧缓存中
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureBuffer, 0);
        // 创建多重采样渲染缓存
        GLuint RBO;
        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, sampleCount, GL_DEPTH24_STENCIL8, width(), height());
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        // 将多重采样渲染缓存附加到帧缓存中
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
            return 1;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);



        // 第二个帧缓存
        GLuint intermediateFBO;
        glGenFramebuffers(1, &intermediateFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

        // 创建屏幕材质
        GLuint screenTexture;
        glGenTextures(1, &screenTexture);
        glBindTexture(GL_TEXTURE_2D, screenTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width(), height(), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 向第二个帧缓存中附加纹理
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
            std::cout << "ERROR::FRAMEBUFFER:: Intermediate Framebuffer is not complete!" << std::endl;
            return 1;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        boxShader.bind();
        boxShader.setUniformVec3("uColor", glm::vec3(0.0f, 1.0f, 0.0f));

        screenShader.bind();
        screenShader.setUniformInt("screenTexture", 0);


        while(!glfwWindowShouldClose(window())){
            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            // 1. 绘制帧缓存
            glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glEnable(GL_DEPTH_TEST);

            boxShader.bind();

            glm::mat4 model(1.0f);
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);

            boxes.model = glm::rotate(model, glm::radians(60.0f), glm::vec3(1.0f, 0.3f, 0.5f));
            boxes.view = view;
            boxes.projection = projection;
            boxes.draw(boxShader);

            // 2. 将图像位块传送(Blit)到默认的帧缓冲
            glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
            glBlitFramebuffer(0, 0, width(), height(), 0, 0, width(), height(), GL_COLOR_BUFFER_BIT, GL_NEAREST);

            // 3. 渲染到屏幕
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            glDisable(GL_DEPTH_TEST);

            screenShader.bind();
            glBindVertexArray(screenVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, screenTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);


            glfwSwapBuffers(window());
            glfwPollEvents();
        }
        glfwTerminate();
        return 0;
    }

    void Main0410::beforeCreateWindow() {

    }
} // Main0410