#ifndef STUDY_OPENGL_MAIN023_H
#define STUDY_OPENGL_MAIN023_H

#include "helper/Main.h"

namespace Main023 {

    class Main023 : public Main{
    public:
        int exec() override;
    private:
        GLuint createBoxShaderProgram();
        GLuint createLightShaderProgram();
    };

} // Main023

#endif //STUDY_OPENGL_MAIN023_H
