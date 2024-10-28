#pragma once
#include "../assimpLoader/Model.h"

class SpherePrimitive : public Model
{
public:
    SpherePrimitive();
    SpherePrimitive(int stacks, int sectors);

private:
    
    std::vector<Vertex> m_vertices; 
    std::vector<GLuint> m_indices;
    void generateMesh();
    void initVertexAndIndices();
    const int m_stacks;
    const int m_sectors;
};
