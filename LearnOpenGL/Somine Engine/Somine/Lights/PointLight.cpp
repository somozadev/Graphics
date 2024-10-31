#include "PointLight.h"

#include <glm/glm/gtc/matrix_transform.hpp>

PointLight::PointLight(int index): BaseLight()
{
    m_index = index; 
    m_diffuse_intensity = 1.0f;
    transform->scale(0.05f,0.05f,0.05f);
}

void PointLight::calcLocalPosition(const glm::mat4& model)
{
    glm::mat4 worldToLocalMatrix = glm::inverse(model);
    glm::vec4 worldPosition = glm::vec4(transform->position, 1.0f);
    m_local_position = glm::vec3(worldToLocalMatrix * worldPosition);
}

void PointLight::setShaderRef(const Shader* shader)
{
    m_shader = shader;
}

void PointLight::setShaderData()
{
    m_shader->use();
    calculateAttenuationRadiusBased();
    m_shader->setPointLight(m_index, m_color, m_ambient_intensity, m_diffuse_intensity, getLocalPosition(), m_constant_attenuation, m_linear_attenuation, m_exponential_attenuation);
}

void PointLight::calculateAttenuationRadiusBased()
{
    m_linear_attenuation = exp( m_linear_a * log( m_radius) +  m_linear_b);
    m_exponential_attenuation = exp( m_quadratic_a * log( m_radius) +  m_quadratic_b);
}
