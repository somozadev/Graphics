#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm/detail/type_mat.hpp>
#include <glm/glm/detail/type_mat4x4.hpp>
#include <glm/glm/detail/type_vec.hpp>
#include <glm/glm/detail/type_vec3.hpp>

class Camera
{
private:
    glm::vec3 up;
    glm::vec3 m_right;
    glm::vec3 m_up;
    glm::vec3 m_target;

    const float radius = 5.0f;
    float camX;
    float camY;

public:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::mat4 m_view_matrix;

    Camera();
    glm::mat4 getViewMatrix();
    ~Camera();
    void rotate();
};
