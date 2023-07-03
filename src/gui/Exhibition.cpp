#include "Exhibition.h"
#include "GuiApplication.h"
#include <vector>
#include <imgui/imgui.h>

namespace Internal {
    struct PainterState {
        bool display;
        bool init;

        PainterState() : display{false}, init{false} {
        }
    };

    class ExhibitionApplication : public GuiApplication {
    public:
        ~ExhibitionApplication() override{
            for(auto p : Painters){
                delete p;
            }
        }

        void beforeCreateWindow() override {
        }

        void onInit() override {

        }

        void onRender() override {
            glClearColor(BackgroundColor.r, BackgroundColor.g, BackgroundColor.b, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            for(int i=0; i<PainterStates.size(); i++){
                if(!PainterStates[i].display){
                    continue;
                }
                Painter* painter = Painters[i];
                if(!PainterStates[i].init){
                    PainterStates[i].init = true;
                    setTitle(painter->title().c_str());
                    setCamera(painter->camera());
                    painter->init();
                }
                painter->draw(width(), height());
            }
        }

        void onGui() override {
            if(showPaintersWindow_){
                const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 10, main_viewport->WorkPos.y + 10), ImGuiCond_None);

                ImGui::Begin("Painters", &showPaintersWindow_, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

                for(int i=0; i<Painters.size(); i++){
                    if (ImGui::Button(Painters[i]->title().c_str())){
                        for(auto &state : PainterStates){
                            state.display = false;
                        }
                        PainterStates[i].display = true;
                    }
                }

                ImGui::End();
            }else {
                if (glfwGetKey(window(), GLFW_KEY_TAB) == GLFW_PRESS) {
                    showPaintersWindow_ = !showPaintersWindow_;
                }
            }
        }

        std::vector<Painter*> Painters;
        std::vector<PainterState> PainterStates;
        glm::vec3 BackgroundColor = glm::vec3(0.8f, 0.8f, 0.8f);
    private:
        bool showPaintersWindow_ = true;
    };
}



Exhibition::Exhibition() : app_{new Internal::ExhibitionApplication} {

}

Exhibition::~Exhibition() {
    delete app_;
}

void Exhibition::SetBackground(float r, float g, float b) {
    app_->BackgroundColor = glm::vec3(r, g, b);
}

void Exhibition::AddPainter(std::unique_ptr<Painter> painter) {
    app_->Painters.emplace_back(painter.release());

    Internal::PainterState state;
    app_->PainterStates.emplace_back(state);
}

void Exhibition::Exhibit() {
    app_->exec();
}

