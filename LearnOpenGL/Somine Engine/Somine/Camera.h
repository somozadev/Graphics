#pragma once
#include <glm/glm/detail/type_mat.hpp>
#include <glm/glm/detail/type_mat4x4.hpp>
#include <glm/glm/detail/type_vec.hpp>
#include <glm/glm/detail/type_vec3.hpp>

#include "Window.h"

class Window;

class Camera
{
    static Camera* instance;

private:
    glm::vec3 up;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_front;
    glm::vec3 m_target;
    glm::vec3 m_rot_direction;

    const float radius = 5.0f;
    Window* ref_window;

    float camX;
    float camY;

    const float m_sensitivity = 0.2f;
    float yaw = -90.0f;
    float pitch;
    float last_x_mouse_pos;
    float last_y_mouse_pos;

    void translate(float delta_time);
    void rotate();
    void updateCameraVectors();

public:
    glm::vec3 m_position;
    float m_speed = 1.0f;
    glm::vec3 m_direction;
    glm::mat4 m_view_matrix;

    bool spin{false};

    Camera(Window* window);
    glm::mat4 getViewMatrix();
    ~Camera();
    void update(float delta_time);
};
