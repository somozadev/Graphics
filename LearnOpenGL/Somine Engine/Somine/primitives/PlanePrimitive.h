#pragma once
#include "../Mesh.h"

class PlanePrimitive : public Mesh
{
public:
    PlanePrimitive();
    PlanePrimitive(int scale, int subdivision);
    float m_width;
    float m_depth;

private:
    void initVertexAndIndices();
    const float m_subdivision = 20.0f;
};
