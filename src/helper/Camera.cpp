#include "Camera.h"
#include <cmath>
#include <iostream>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : Position{position}, Front(glm::vec3(0.0f, 0.0f, -1.0f)), Up{}, Right{}, WorldUp{up}
    , Yaw(yaw), Pitch(pitch)
    , MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM) {

    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(Camera::CameraMovement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    switch (direction) {
        case FORWARD:
            Position += Front * velocity;
            break;
        case BACKWARD:
            Position -= Front * velocity;
            break;
        case LEFT:
            Position -= Right * velocity;
            break;
        case RIGHT:
            Position += Right * velocity;
            break;
    }
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;
    if(constrainPitch){
        Pitch = Pitch > 89.0f ? 89.0f : Pitch;
        Pitch = Pitch < -89.0f ? -89.0f : Pitch;
    }
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset) {
    Zoom -= yOffset;
    Zoom = Zoom > 45.0f ? 45.0f : Zoom;
    Zoom = Zoom < 1.0f ? 1.0f : Zoom;
}

void Camera::updateCameraVectors() {
    glm::vec3 front(0.0f);
    front.x = std::cos(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));
    front.y = std::sin(glm::radians(Pitch));
    front.z = std::sin(glm::radians(Yaw)) * std::cos(glm::radians(Pitch));

    std::cout << std::cos(glm::radians(Pitch)) << std::endl;


    Front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    Up    = glm::normalize(glm::cross(Right, Front));
}
