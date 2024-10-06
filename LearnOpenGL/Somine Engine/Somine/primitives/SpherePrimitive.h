#pragma once
#include "../Mesh.h"

class SpherePrimitive : public Mesh
{
public:
    SpherePrimitive();
    SpherePrimitive(int stacks, int sectors);

private:
    void createVertexAndIndices();
    const int m_stacks = 20;
    const int m_sectors = 20;
};
