#pragma once
#include <glm/glm/detail/type_vec.hpp>
#include <glm/glm/detail/type_vec3.hpp>

#include "../primitives/SpherePrimitive.h"

class BaseLight : public SpherePrimitive
{
public:
    BaseLight();
    void setShaderRef(const Shader* shader) override;
    glm::vec3 m_color;
    float m_ambient_intensity {1.0f};
};
