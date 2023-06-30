#include "Main050.h"
#include "helper/Boxes.h"
#include "helper/Plane.h"
#include "helper/Helper.h"
#include "shader/Shader.h"


// Shadow Mapping
// 渲染阴影深度图
namespace Main050 {

    const int DEPTH_WIDTH = 1024;
    const int DEPTH_HEIGHT = 1024;

    float screenVertices[] = {
            // positions   // texCoords
            -1.0f,  1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f,  0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f,  1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };

    void renderScene(glm::mat4 model, glm::mat4 view, glm::mat4 projection,
                     Plane &plane, Boxes &box, Shader &shader);

    int Main050::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        createWindowWithCamera("hello Shadow Mapping", camera);

        auto boxVertices = Boxes::createVertices();
        auto boxTextures = Boxes::createTextures(Helper::inRes("container.jpg"));
        Boxes box(boxVertices, boxTextures);

        auto planeVertices = Plane::createVertices();
        auto planeTextures = Plane::createTextures(Helper::inRes("wood.png"));
        Plane plane(planeVertices, planeTextures);

        Shader boxPlaneShader("src05/shader/050_box_plane.vert.glsl", "src05/shader/050_box_plane.frag.glsl");
        Shader lightDepthShader("src05/shader/050_light_depth.vert.glsl", "src05/shader/050_light_depth.frag.glsl");
        Shader screenShader("src05/shader/050_screen_depth.vert.glsl", "src05/shader/050_screen_depth.frag.glsl");


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



        // 创建深度缓存和深度贴图
        GLuint depthFBO;
        glGenFramebuffers(1, &depthFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);

        GLuint depthMap;
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, DEPTH_WIDTH, DEPTH_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT,
                     nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glReadBuffer(GL_NONE);
        glDrawBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // 光源空间的变换矩阵
        glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);



        screenShader.bind();
        screenShader.setUniformInt("screenTexture", 0);


        while(!glfwWindowShouldClose(window())){
            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glm::mat4 model(1.0f);
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);

            // 渲染深度场景
            glViewport(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT);
            glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
            glEnable(GL_DEPTH_TEST);
            glClear(GL_DEPTH_BUFFER_BIT);

            renderScene(model, lightView, lightProjection, plane, box, lightDepthShader);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);


            // 将帧缓存绘制到屏幕
            glDisable(GL_DEPTH_TEST);
            glViewport(0, 0, width(), height());
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            screenShader.bind();
            glBindVertexArray(screenVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            glDrawArrays(GL_TRIANGLES, 0, 6);


//            // 渲染真实场景
//            glViewport(0, 0, width(), height());
//            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//            renderScene(model, view, projection, plane, box, boxPlaneShader);


            glfwSwapBuffers(window());
            glfwPollEvents();
        }
        glfwTerminate();
        return 0;
    }

    void renderScene(glm::mat4 model, glm::mat4 view, glm::mat4 projection,
                     Plane &plane, Boxes &box, Shader &shader)
    {
        // plane
        plane.model = model;
        plane.view = view;
        plane.projection = projection;
        plane.draw(shader);

        // boxes
        box.view = view;
        box.projection = projection;

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0));
        model = glm::scale(model, glm::vec3(0.5f));
        box.model = model;
        box.draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0));
        model = glm::scale(model, glm::vec3(0.5f));
        box.model = model;
        box.draw(shader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0));
        model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
        model = glm::scale(model, glm::vec3(0.25));
        box.model = model;
        box.draw(shader);
    }
} // Main050