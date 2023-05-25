#ifndef STUDY_OPENGL_MAIN024_H
#define STUDY_OPENGL_MAIN024_H

#include "helper/Main.h"

namespace Main024 {

    class Main024 : public Main{
    public:
        int exec() override;
    private:
        GLuint createBoxShaderProgram();
        GLuint createLightShaderProgram();
    };

} // Main024

#endif //STUDY_OPENGL_MAIN024_H
