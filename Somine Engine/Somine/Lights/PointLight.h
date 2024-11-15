#pragma once
#include "BaseLight.h"

class PointLight : public BaseLight
{
public:
    PointLight(int index);
    void calcLocalPosition(const glm::mat4& model);
    void setShaderRef(const Shader* shader) override;
    virtual void setShaderData();
    const glm::vec3& getLocalPosition() const { return m_local_position; }
    float m_constant_attenuation{1.0f};
    float m_linear_attenuation{1.0f};
    float m_exponential_attenuation{0.1f};
    int m_index;
    float m_radius{5.0f};
private:
    glm::vec3 m_local_position{0.0f, 0.0f, 0.0f};
    const float m_linear_a = -0.7, m_linear_b = 0.14;
    const float m_quadratic_a = -1.5, m_quadratic_b = 0.07;
protected:
    void calculateAttenuationRadiusBased();
};
