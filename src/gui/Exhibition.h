#ifndef STUDY_OPENGL_EXHIBITION_H
#define STUDY_OPENGL_EXHIBITION_H

#include <memory>
#include "Painter.h"

namespace Internal {
    class ExhibitionApplication;
}

class Exhibition {
public:
    Exhibition();
    ~Exhibition();

    void SetBackground(float r, float g, float b);
    void AddPainter(std::unique_ptr<Painter> painter);
    void Exhibit();
private:
    Internal::ExhibitionApplication* app_;
};


#endif //STUDY_OPENGL_EXHIBITION_H
