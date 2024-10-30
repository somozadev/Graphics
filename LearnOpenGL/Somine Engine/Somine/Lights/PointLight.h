#pragma once
#include "BaseLight.h"

class PointLight : public BaseLight
{
public:
    PointLight(int index);
    void calcLocalPosition(const glm::mat4& model);
    void setShaderRef(const Shader* shader) override;
    void setShaderData();
    const glm::vec3& getLocalPosition() const { return m_local_position; }
    float m_constant_attenuation{1.0f};
    float m_linear_attenuation{1.0f};
    float m_exponential_attenuation{0.1f};
    int m_index;

private:
    glm::vec3 m_local_position{0.0f, 0.0f, 0.0f};
};
