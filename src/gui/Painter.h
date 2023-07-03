#ifndef STUDY_OPENGL_PAINTER_H
#define STUDY_OPENGL_PAINTER_H

#include <string>
#include <memory>
#include "helper/Camera.h"

class Painter {
public:
    Painter();
    virtual ~Painter();


    virtual std::string title();
    virtual std::shared_ptr<Camera> camera();
    virtual void init() = 0;
    virtual void draw(int w, int h) = 0;
};


#endif //STUDY_OPENGL_PAINTER_H
