#include "BaseLight.h"

BaseLight::BaseLight()
{
    m_color = glm::vec3(1.0f, 1.0f, 1.0f);
    m_ambient_intensity = 0.0f;
}

void BaseLight::setShaderRef(const Shader* shader)
{
    m_shader = shader;
    if (m_shader)
    {
        m_shader->use();
        m_shader->setLight("light.color", m_color, "light.ambient_intensity", m_ambient_intensity);
    }
}
