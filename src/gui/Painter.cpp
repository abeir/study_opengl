#include "Painter.h"

Painter::Painter() = default;

Painter::~Painter() = default;

std::string Painter::title() {
    return { "Hello" };
}

std::shared_ptr<Camera> Painter::camera() {
    return std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 3.0f));
}
