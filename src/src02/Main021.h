#ifndef STUDY_OPENGL_MAIN021_H
#define STUDY_OPENGL_MAIN021_H

#include "helper/Main.h"

namespace Main021 {

    class Main021 : public Main{
    public:
        int exec() override;
    private:
        GLuint createBoxShaderProgram();
        GLuint createLightShaderProgram();
    };

} // Main021

#endif //STUDY_OPENGL_MAIN021_H
