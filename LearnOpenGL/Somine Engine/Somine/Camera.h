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
    float m_aspectRatio;

    const float m_radius = 5.0f;
    Window* m_window;

    float m_camX;
    float m_camY;

    const float m_sensitivity = 0.1f;
    float m_yaw = -90.0f;
    float m_pitch;
    float m_last_x_mouse_pos;
    float m_last_y_mouse_pos;
    glm::mat4 m_view_matrix;
    glm::mat4 m_projection_matrix;
    
    void translate(float delta_time);
    void rotate();
    void updateCameraVectors();
    void updateAspectRatio();

public:
    glm::vec3 m_position;
    float m_speed = 1.0f;
    glm::vec3 m_direction;


    bool spin{false};

    Camera(Window* window);
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    ~Camera();
    void update(float delta_time);
};
