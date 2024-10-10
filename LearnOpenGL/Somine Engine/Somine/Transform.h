#pragma once
#include <glm/glm/detail/type_mat.hpp>
#include <glm/glm/detail/type_mat4x4.hpp>

class Transform
{
private:
    glm::mat4 m_model_matrix;

public:
    Transform();
    glm::vec3 position;
    glm::vec3 scalar;
    glm::vec3 rotation;
    void move(float x, float y, float z);
    void scale(float x, float y, float z);
    void rotate(float x, float y, float z);

    glm::mat4 getModelMatrix();
};
