#include "Transform.h"

#include <glm/glm/gtc/matrix_transform.hpp>


Transform::Transform()
{
    position = glm::vec3(1.0f, 1.0f, 1.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scalar = glm::vec3(1.0f, 1.0f, 1.0f);
    m_model_matrix = glm::mat4(1.0f);
}


void Transform::move(float x, float y, float z)
{
    position = glm::vec3(x, y, z);
}

void Transform::scale(float x, float y, float z)
{
    scalar = glm::vec3(x, y, z);
}

void Transform::rotate(float x, float y, float z)
{
    rotation = glm::vec3(x, y, z);
}

glm::mat4 Transform::getModelMatrix()
{
    m_model_matrix = glm::mat4(1.0f);
    m_model_matrix = glm::scale(m_model_matrix, scalar);
    m_model_matrix = glm::translate(m_model_matrix, position);

    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); // Pitch
    m_model_matrix = glm::rotate(m_model_matrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)); // Roll
    return m_model_matrix;
}
