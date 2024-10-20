#include "Camera.h"
#include <glm/glm/gtc/matrix_transform.hpp>
#include "Window.h"


Camera::Camera(Window* window): m_window(window)
{
    double xpos, ypos;
    glfwGetCursorPos(m_window->getGLFWWindow(), &xpos, &ypos);

    m_last_x_mouse_pos = xpos;
    m_last_y_mouse_pos = ypos;

    m_position = glm::vec3(0.0f, 2.0f, 3.0f);
    m_target = glm::vec3(0.0f, 0.0f, 0.0f);
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
    m_direction = glm::normalize(m_position - m_target);

    up = glm::vec3(0.0f, 1.0f, 0.0f);
    m_right = glm::normalize(glm::cross(up, m_direction));
    m_up = glm::normalize(glm::cross(m_direction, m_right));

    m_view_matrix = glm::lookAt(m_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_projection_matrix = glm::perspective(glm::radians(45.0f), m_aspectRatio, 0.1f, 100.0f);
}

glm::mat4 Camera::getViewMatrix()
{
    return m_view_matrix;
}

glm::mat4 Camera::getProjectionMatrix()
{
    m_projection_matrix = glm::perspective(glm::radians(45.0f), m_aspectRatio, 0.1f, 100.0f);
    return m_projection_matrix;
}

void Camera::translate(float delta_time)
{
    if (glfwGetKey(m_window->getGLFWWindow(), GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        m_speed = 5.0f * delta_time;
    else m_speed = 1.0f * delta_time;
    if (glfwGetKey(m_window->getGLFWWindow(), GLFW_KEY_W) == GLFW_PRESS)
        m_position += m_speed * m_front;
    if (glfwGetKey(m_window->getGLFWWindow(), GLFW_KEY_S) == GLFW_PRESS)
        m_position -= m_speed * m_front;
    if (glfwGetKey(m_window->getGLFWWindow(), GLFW_KEY_A) == GLFW_PRESS)
        m_position -= glm::normalize(glm::cross(m_front, m_up)) * m_speed;
    if (glfwGetKey(m_window->getGLFWWindow(), GLFW_KEY_D) == GLFW_PRESS)
        m_position += glm::normalize(glm::cross(m_front, m_up)) * m_speed;

    m_view_matrix = glm::lookAt(m_position, m_position + m_front, m_up);
}

void Camera::rotate()
{
    double xpos, ypos;
    glfwGetCursorPos(m_window->getGLFWWindow(), &xpos, &ypos);

    if (glfwGetMouseButton(m_window->getGLFWWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        glfwSetInputMode(m_window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        m_last_x_mouse_pos = xpos;
        m_last_y_mouse_pos = ypos;
        return;
    }

    glfwSetInputMode(m_window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    float x_offset = xpos - m_last_x_mouse_pos;
    float y_offset = ypos - m_last_y_mouse_pos;

    m_last_x_mouse_pos = xpos;
    m_last_y_mouse_pos = ypos;

    x_offset *= m_sensitivity;
    y_offset *= m_sensitivity;


    m_yaw += x_offset;
    m_pitch -= y_offset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    front.y = sin(glm::radians(m_pitch));
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(front);
    // also re-calculate the Right and Up vector
    m_right = glm::normalize(glm::cross(m_front, up));
    // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void Camera::update(float delta_time)
{
    updateAspectRatio();
    if (!spin)
    {
        translate(delta_time);
        rotate();
        return;
    }

    m_camX = sin(glfwGetTime()) * m_radius;
    m_camY = cos(glfwGetTime()) * m_radius;
    m_view_matrix = glm::lookAt(glm::vec3(m_camX, 0.0f, m_camY), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::updateAspectRatio()
{
    int width = m_window->m_width;
    int height = m_window->m_height;
    if (height == 0) height = 1;
    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

Camera::~Camera()
{
}


/**
* 
    static int last_mouse_state = GLFW_RELEASE;
    static glm::vec2 initial_mouse_pos;

    double xpos, ypos;
    glfwGetCursorPos(ref_window->getGLFWWindow(), &xpos, &ypos);

    int mouse_state = glfwGetMouseButton(ref_window->getGLFWWindow(), GLFW_MOUSE_BUTTON_LEFT);
    if (mouse_state != last_mouse_state)
    {
        if (mouse_state == GLFW_PRESS)
            glfwSetInputMode(ref_window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(ref_window->getGLFWWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        last_mouse_state = mouse_state;

        double xpos, ypos;
        glfwGetCursorPos(ref_window->getGLFWWindow(), &xpos, &ypos);

        initial_mouse_pos = glm::vec2((float)xpos, (float)ypos);
        m_rot_direction = glm::vec3(0);
    }
    else if (mouse_state == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(ref_window->getGLFWWindow(), &xpos, &ypos);

        float pitchChange = ((float)xpos - initial_mouse_pos.x) * 0.001f * m_speed;
        float yawChange = ((float)ypos - initial_mouse_pos.y) * 0.001f * m_speed;

        pitch = pitchChange;
        yaw = yawChange;
    }
 */
