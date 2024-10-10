#pragma once
#include "../assimpLoader/Model.h"

class PlanePrimitive : public Model
{
public:
    PlanePrimitive();
    PlanePrimitive(int scale, int subdivision);
    float m_width;
    float m_depth;

private:
    
    std::vector<Vertex> m_vertices; 
    std::vector<GLuint> m_indices;

    void generateMesh();
    void initVertexAndIndices();
    const float m_subdivision = 20.0f;
};
