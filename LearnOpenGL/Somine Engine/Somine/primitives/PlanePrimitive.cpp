#include "PlanePrimitive.h"

#include <glm/glm/gtc/matrix_transform.hpp>

PlanePrimitive::PlanePrimitive()
{
    initVertexAndIndices();
    m_indicesSize = m_indices.size();
    generateBuffers();
}

PlanePrimitive::PlanePrimitive(int scale, int subdivision) : m_subdivision(subdivision)
{
    m_scale = scale;
    m_width = m_scale;
    m_depth = m_scale;
    initVertexAndIndices();
    m_indicesSize = m_indices.size();
    generateBuffers();
}

void PlanePrimitive::initVertexAndIndices() 
{
    float halfWidth = m_width / 2.0f;
    float halfDepth = m_depth / 2.0f;
    float deltaX = m_width / m_subdivision;
    float deltaZ = m_depth / m_subdivision;
    for (int z = 0; z <= m_subdivision; ++z)
    {
        for (int x = 0; x <= m_subdivision; ++x)
        {
            float posX = -halfWidth + x * deltaX;
            float posZ = -halfDepth + z * deltaZ;

            m_vertices.push_back(posX);
            m_vertices.push_back(0.0f); // plane is Y = 0
            m_vertices.push_back(posZ);
        }
    }
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
