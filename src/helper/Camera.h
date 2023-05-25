#ifndef STUDY_OPENGL_CAMERA_H
#define STUDY_OPENGL_CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    constexpr static const float YAW = -90.0f;
    constexpr static const float PITCH = 0.0f;
    constexpr static const float SPEED = 2.5f;
    constexpr static const float SENSITIVITY = 0.1f;
    constexpr static const float ZOOM = 45.0f;

    enum CameraMovement {
        FORWARD, BACKWARD, LEFT, RIGHT
    };

    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler
    float Yaw;
    float Pitch;

    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    explicit Camera(glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);

    [[nodiscard]] glm::mat4 GetViewMatrix() const;
    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch  = true);
    void ProcessMouseScroll(float yOffset);
private:
    void updateCameraVectors();
};


#endif //STUDY_OPENGL_CAMERA_H
