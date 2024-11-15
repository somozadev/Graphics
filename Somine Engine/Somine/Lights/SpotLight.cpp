#include "SpotLight.h"

SpotLight::SpotLight(int index): PointLight(index)
{
    m_cutoff = 30.0f;
    m_world_direction = {0.0f, -1.0f, 0.0f};
   transform->rotation = {0.0f, -1.0f, 0.0f};
}

void SpotLight::calcLocalDirectionAndPosition(const glm::mat4& world)
{
    calcLocalPosition(world);

    glm::mat3 rotation_scale(world);
    glm::mat3 world_to_local = glm::transpose(rotation_scale);
    m_local_direction = world_to_local * transform->rotation;
    m_local_direction = glm::normalize(m_local_direction);
}

const glm::vec3 SpotLight::getLocalDirection() const
{
    return m_local_direction;
}


void SpotLight::setShaderData()
{
    m_shader->use();
    calculateAttenuationRadiusBased();
    m_shader->setSpotLight(m_index, m_color, m_ambient_intensity, m_diffuse_intensity, getLocalPosition(),
                           m_constant_attenuation, m_linear_attenuation, m_exponential_attenuation, getLocalDirection(),
                           m_cutoff);
}
