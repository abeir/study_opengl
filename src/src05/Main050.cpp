#include "Main050.h"
#include "helper/Helper.h"



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




    Painter050::Painter050() : camera_{new Camera(glm::vec3(0.0f, 0.0f, 3.0f))} {
    }

    Painter050::~Painter050() = default;

    std::string Painter050::title() {
        return { "Shadow Mapping" };
    }

    std::shared_ptr<Camera> Painter050::camera() {
        return camera_;
    }

    void Painter050::init() {
        auto boxVertices = Boxes::createVertices();
        auto boxTextures = Boxes::createTextures(Helper::inRes("container.jpg"));
        box_ = std::make_unique<Boxes>(boxVertices, boxTextures);

        auto planeVertices = Plane::createVertices();
        auto planeTextures = Plane::createTextures(Helper::inRes("wood.png"));
        plane_ = std::make_unique<Plane>(planeVertices, planeTextures);

        lightDepthShader_ = std::make_unique<Shader>("src05/shader/050_light_depth.vert.glsl", "src05/shader/050_light_depth.frag.glsl");
        screenShader_ = std::make_unique<Shader>("src05/shader/050_screen_depth.vert.glsl", "src05/shader/050_screen_depth.frag.glsl");


        // 屏幕
        GLuint screenVBO;
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
        glGenFramebuffers(1, &depthFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);

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
        lightView_ = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        lightProjection_ = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);


        screenShader_->bind();
        screenShader_->setUniformInt("screenTexture", 0);
    }

    void Painter050::draw(int w, int h) {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model(1.0f);
        glm::mat4 view = camera_->GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera_->Zoom), (float)w/(float)h, 1.0f, 100.0f);

        // 渲染深度场景
        glViewport(0, 0, DEPTH_WIDTH, DEPTH_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthFBO);
        glEnable(GL_DEPTH_TEST);
        glClear(GL_DEPTH_BUFFER_BIT);

        renderScene(model, lightView_, lightProjection_, *plane_, *box_, *lightDepthShader_);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // 将帧缓存绘制到屏幕
        glDisable(GL_DEPTH_TEST);
        glViewport(0, 0, w, h);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        screenShader_->bind();
        glBindVertexArray(screenVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

} // Main050