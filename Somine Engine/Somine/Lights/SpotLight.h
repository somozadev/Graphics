#pragma once
#include "PointLight.h"

class SpotLight : public PointLight
{
public:
    SpotLight(int index);
    glm::vec3 m_world_direction {0.0f,0.0f,0.0f};
    float m_cutoff{0.0f};

    void calcLocalDirectionAndPosition(const glm::mat4& world);
    const glm::vec3 getLocalDirection() const;
    void setShaderData() override;


private:
    glm::vec3 m_local_direction;
};
