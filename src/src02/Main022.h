#ifndef STUDY_OPENGL_MAIN022_H
#define STUDY_OPENGL_MAIN022_H

#include "helper/Main.h"

namespace Main022 {

    class Main022 : public Main{
    public:
        int exec() override;
    private:
        GLuint createBoxShaderProgram();
        GLuint createLightShaderProgram();
    };

} // Main022

#endif //STUDY_OPENGL_MAIN022_H
