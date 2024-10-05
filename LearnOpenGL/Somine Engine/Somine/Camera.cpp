#include "Camera.h"
#include <glm/glm/gtc/matrix_transform.hpp>

Camera::Camera()
{
    m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    m_target = glm::vec3(0.0f, 0.0f, 0.0f);
    m_direction = glm::normalize(m_position - m_target);

    up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_right = glm::normalize(glm::cross(up, m_direction));
    m_up = glm::normalize(glm::cross(m_direction, m_right));

    m_view_matrix = glm::lookAt(m_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getViewMatrix()
{
    return m_view_matrix;
}

Camera::~Camera()
{
}

void Camera::rotate()
{
    camX = sin(glfwGetTime()) * radius;
    camY = cos(glfwGetTime()) * radius;
    m_view_matrix = glm::lookAt(glm::vec3(camX, 0.0f, camY), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}
