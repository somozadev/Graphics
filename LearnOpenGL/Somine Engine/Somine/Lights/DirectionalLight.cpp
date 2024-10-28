#include "DirectionalLight.h"

void DirectionalLight::calcLocalDirection(const glm::mat4& world)
{
    glm::mat3 rotation_scale(world); //ment to be the 3x3 matrix discarding translations and for now scalars (assuming objs are uniformly scaled)
  //  glm::mat3 world_to_local = glm::transpose(rotation_scale);

    glm::mat3 world_to_local = glm::transpose(glm::inverse(rotation_scale)); // this is supossed to accept any scale, well check later

    
    m_local_direction = world_to_local * transform->rotation;
    m_local_direction = glm::normalize(m_local_direction);
    
}
void DirectionalLight::setShaderRef(const Shader* shader)
{
    m_shader = shader;
    if (m_shader)
    {
        m_shader->use();
        m_shader->setDirectionalLight("directional_light.color", m_color, "directional_light.ambient_intensity", m_ambient_intensity,
            "directional_light.diffuse_intensity", m_diffuse_intensity, "directional_light.direction", getLocalDirection());
    }
}
