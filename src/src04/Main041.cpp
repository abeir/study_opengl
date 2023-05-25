#include "Main041.h"
#include <glad/glad.h>
#include "helper/Boxes.h"
#include "helper/Helper.h"


namespace Main041 {
    int Main041::exec() {
        std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
        createWindowWithCamera("hello depth test", camera);

        std::vector<Boxes::Texture> textures = Boxes::createTextures(Helper::inRes("container2.png"), Helper::inRes("container2_specular.png"));
        std::vector<Boxes::Vertex>& vertices = Boxes::createVertices();
        std::vector<glm::vec3>& transformPos = Boxes::createTransformPositions();

        Boxes boxes(vertices, textures);

        Shader shader("src04/shader/041.vert.glsl", "src04/shader/041.frag.glsl");

        glEnable(GL_DEPTH_TEST);

        while(!glfwWindowShouldClose(window())){
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            updateDeltaTime();
            processInputEsc();
            processInputCamera();

            glm::mat4 view = camera->GetViewMatrix();
            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), widthF()/heightF(), 1.0f, 100.0f);

            boxes.view = view;
            boxes.projection = projection;

            for(int i=0; i<transformPos.size(); i++){
                glm::vec3 pos = transformPos[i];
                glm::mat4 model(1.0f);
                model = glm::translate(model, pos);
                float angle = 20.0f * (float)i;
                if(angle < 1.0f){
                    angle = 60.0f;
                }
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

                boxes.model = model;
                boxes.draw(shader);
            }


            glfwSwapBuffers(window());
            glfwPollEvents();
        }
        glfwTerminate();
        return 0;
    }
} // Main041