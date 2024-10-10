#pragma once
#include <vector>
#include <glad/glad.h>

#include "../../OldMesh.h"

class Grid : public OldMesh
{
private:
    void init();

public:
    Grid();
    Grid(int size, float spacing);
    int m_size = 1500;
    float m_spacing = 1.0f;
};
