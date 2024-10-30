#pragma once
#include "BaseLight.h"

class DirectionalLight : public  BaseLight
{
public:
    DirectionalLight();
    void setShaderRef(const Shader* shader) override;

    void calcLocalDirection(const glm::mat4 &world);
    const glm::vec3& getLocalDirection() const { return m_local_direction;}
private:
    glm::vec3 m_local_direction {10.0f, 0.0f, 0.0f}; 
};
