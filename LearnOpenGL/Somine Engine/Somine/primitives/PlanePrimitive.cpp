#include "PlanePrimitive.h"


PlanePrimitive::PlanePrimitive() : m_width(1.0f), m_depth(1.0f), m_subdivision(20)
{
    initVertexAndIndices();
    generateMesh();
}

PlanePrimitive::PlanePrimitive(int scale, int subdivision) : m_width(scale), m_depth(scale), m_subdivision(subdivision)
{
    initVertexAndIndices();
    generateMesh();
}

void PlanePrimitive::generateMesh()
{
    Mesh planeMesh(m_vertices, m_indices, std::vector<Texture>());
    addMesh(planeMesh);
}

void PlanePrimitive::initVertexAndIndices() 
{
    float halfWidth = m_width / 2.0f;
    float halfDepth = m_depth / 2.0f;
    float deltaX = m_width / m_subdivision;
    float deltaZ = m_depth / m_subdivision;

    // Generate vertices
    for (int z = 0; z <= m_subdivision; ++z)
    {
        for (int x = 0; x <= m_subdivision; ++x)
        {
            float posX = -halfWidth + x * deltaX;
            float posZ = -halfDepth + z * deltaZ;

            // Create a vertex with position, empty normal, and empty texture coordinates
            m_vertices.emplace_back(Vertex{
                { posX, 0.0f, posZ },  // Position
                { 0.0f, 0.0f, 0.0f },  // Normal (empty)
                { 0.0f, 0.0f }         // Texture coordinates (empty)
            });
        }
    }

    // Generate indices
    for (int z = 0; z < m_subdivision; ++z)
    {
        for (int x = 0; x < m_subdivision; ++x)
        {
            int topLeft = z * (m_subdivision + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (m_subdivision + 1) + x;
            int bottomRight = bottomLeft + 1;

            // First triangle (topLeft, bottomLeft, bottomRight)
            m_indices.push_back(topLeft);
            m_indices.push_back(bottomLeft);
            m_indices.push_back(bottomRight);

            // Second triangle (topLeft, bottomRight, topRight)
            m_indices.push_back(topLeft);
            m_indices.push_back(bottomRight);
            m_indices.push_back(topRight);
        }
    }
}
