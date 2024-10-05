#include "Camera.h"
#include <glm/glm/gtc/matrix_transform.hpp>
#include "Window.h"

Camera::Camera(Window* window): ref_window(window)
{
    m_position = glm::vec3(0.0f, 0.0f, 3.0f);
    m_target = glm::vec3(0.0f, 0.0f, 0.0f);
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
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

void Camera::update(float delta_time)
{
    if (!spin)
    {
        m_speed = 1.0f * delta_time;

        if (glfwGetKey(ref_window->getGLFWWindow(), GLFW_KEY_W) == GLFW_PRESS)
            m_position += m_speed * m_front;
        if (glfwGetKey(ref_window->getGLFWWindow(), GLFW_KEY_S) == GLFW_PRESS)
            m_position -= m_speed * m_front;
        if (glfwGetKey(ref_window->getGLFWWindow(), GLFW_KEY_A) == GLFW_PRESS)
            m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_speed;
        if (glfwGetKey(ref_window->getGLFWWindow(), GLFW_KEY_D) == GLFW_PRESS)
            m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed;

        m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
        return;
    }

    camX = sin(glfwGetTime()) * radius;
    camY = cos(glfwGetTime()) * radius;
    m_view_matrix = glm::lookAt(glm::vec3(camX, 0.0f, camY), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

Camera::~Camera()
{
}
