#include "Main045.h"
#include "helper/Boxes.h"
#include "helper/Helper.h"
#include "shader/Shader.h"

namespace Main045 {

    std::vector<Boxes::Vertex> boxVertices = {
            // Back face
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 0.0f) }, // Bottom-left
            Boxes::Vertex { glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f),  glm::vec2(1.0f, 1.0f) }, // top-right
            Boxes::Vertex { glm::vec3(0.5f, -0.5f, -0.5f),  glm::vec3(0.0f),  glm::vec2(1.0f, 0.0f) }, // bottom-right
            Boxes::Vertex { glm::vec3(0.5f,  0.5f, -0.5f),  glm::vec3(0.0f),  glm::vec2(1.0f, 1.0f) }, // top-right
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f),  glm::vec3(0.0f),  glm::vec2(0.0f, 0.0f) }, // bottom-left
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f, -0.5f),  glm::vec3(0.0f),  glm::vec2(0.0f, 1.0f) }, // top-left
            // Front face
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 0.0f) }, // bottom-left
            Boxes::Vertex { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 0.0f) }, // bottom-right
            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 1.0f) }, // top-right
            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 1.0f) }, // top-right
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 1.0f) }, // top-left
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 0.0f) }, // bottom-left
            // Left face
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 0.0f) }, // top-right
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 1.0f) }, // top-left
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 1.0f) }, // bottom-left
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 1.0f) }, // bottom-left
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 0.0f) }, // bottom-right
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 0.0f) }, // top-right
            // Right face
            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 0.0f) }, // top-left
            Boxes::Vertex { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 1.0f) }, // bottom-right
            Boxes::Vertex { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 1.0f) }, // top-right
            Boxes::Vertex { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 1.0f) }, // bottom-right
            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 0.0f) }, // top-left
            Boxes::Vertex { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 0.0f) }, // bottom-left
            // Bottom face
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 1.0f) }, // top-right
            Boxes::Vertex { glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 1.0f) }, // top-left
            Boxes::Vertex { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 0.0f) }, // bottom-left
            Boxes::Vertex { glm::vec3(0.5f, -0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 0.0f) }, // bottom-left
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 0.0f) }, // bottom-right
            Boxes::Vertex { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 1.0f) }, // top-right
            // Top face
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 1.0f) }, // top-left
            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 0.0f) }, // bottom-right
            Boxes::Vertex { glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 1.0f) }, // top-right
            Boxes::Vertex { glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(1.0f, 0.0f) }, // bottom-right
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 1.0f) }, // top-left
            Boxes::Vertex { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f),  glm::vec2(0.0f, 0.0f) }  // bottom-left
    };


    int Main045::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        createWindowWithCamera("hello Cull Face", camera);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CW);

        std::vector<Boxes::Texture> boxTextures = Boxes::createTextures(Helper::inRes("marble.jpg"));
        Boxes boxes(boxVertices, boxTextures);

        Shader shader("src04/shader/045.vert.glsl", "src04/shader/045.frag.glsl");


        while(!glfwWindowShouldClose(window())){
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glm::mat4 model(1.0f);
            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);


            boxes.view = view;
            boxes.projection = projection;
            boxes.model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
            boxes.draw(shader);


            glfwSwapBuffers(window());
            glfwPollEvents();
        }

        glfwTerminate();
        return 0;
    }
} // Main045