#include "Main043.h"
#include "helper/Plane.h"
#include "helper/Boxes.h"
#include "helper/Camera.h"
#include "helper/Helper.h"


namespace Main043 {
    float grassVertices[] = {
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    std::vector<glm::vec3> vegetation = {
            glm::vec3(-1.5f, 0.0f, -0.48f),
            glm::vec3( 1.5f, 0.0f, 0.51f),
            glm::vec3( 0.0f, 0.0f, 0.7f),
            glm::vec3(-0.3f, 0.0f, -2.3f),
            glm::vec3 (0.5f, 0.0f, -0.6f)
    };

    int Main043::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        createWindowWithCamera("hello blender", camera);

        glEnable(GL_DEPTH_TEST);

        std::vector<Boxes::Vertex>& boxVertices = Boxes::createVertices();
        std::vector<Boxes::Texture> boxTextures = Boxes::createTextures(Helper::inRes("marble.jpg"));
        Boxes boxes(boxVertices, boxTextures);

        std::vector<Plane::Vertex>& planeVertices = Plane::createVertices();
        std::vector<Plane::Texture> planeTextures = Plane::createTextures(Helper::inRes("metal.png"));
        Plane plane(planeVertices, planeTextures);

        Shader boxPlaneShader("src04/shader/043_box_plane.vert.glsl", "src04/shader/043_box_plane.frag.glsl");

        // grass
        Shader grassShader("src04/shader/043_grass.vert.glsl", "src04/shader/043_grass.frag.glsl");
        GLuint grassTexture = Helper::loadTextureFromFile(Helper::inRes("grass.png").c_str(), false);

        GLuint grassVAO;
        glGenVertexArrays(1, &grassVAO);
        glBindVertexArray(grassVAO);

        GLuint grassVBO;
        glGenBuffers(1, &grassVBO);
        glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(grassVertices), grassVertices, GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*5, nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*5, (void*)(sizeof(float)*3));

        glBindVertexArray(0);

        while(!glfwWindowShouldClose(window())){
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glm::mat4 model(1.0f);
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);

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

            // grass
            glBindVertexArray(grassVAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, grassTexture);

            grassShader.bind();
            grassShader.setUniformMat4("view", view);
            grassShader.setUniformMat4("projection", projection);
            grassShader.setUniformInt("vTexture", 0);

            for(auto &v : vegetation){
                model = glm::mat4(1.0f);
                model = glm::translate(model, v);
                grassShader.setUniformMat4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }

            glBindVertexArray(0);

            glfwSwapBuffers(window());
            glfwPollEvents();
        }
        glfwTerminate();
        return 0;
    }
} // Main043