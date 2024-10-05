#pragma once
#include <glm/glm/detail/type_mat.hpp>
#include <glm/glm/detail/type_mat4x4.hpp>
#include <glm/glm/detail/type_vec.hpp>
#include <glm/glm/detail/type_vec3.hpp>

class Window;

class Camera
{
private:
    glm::vec3 up;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_front;
    glm::vec3 m_target;

    const float radius = 5.0f;
    Window* ref_window;
    float camX;
    float camY;

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
