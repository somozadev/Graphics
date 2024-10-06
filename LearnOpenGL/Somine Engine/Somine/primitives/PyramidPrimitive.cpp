#include "PyramidPrimitive.h"

#include <glm/glm/gtc/matrix_transform.hpp>

PyramidPrimitive::PyramidPrimitive()
{
    m_vertices = {
        -0.5f, 0.0f, 0.5f, // Bottom-left 
        0.5f, 0.0f, 0.5f, // Bottom-right 
        0.5f, 0.0f, -0.5f, // Top-right 
        -0.5f, 0.0f, -0.5f, // Top-left 
        0.0f, 1.0f, 0.0f // Apex
    };
    m_indices = {
        0, 1, 4, // Front face
        1, 2, 4, // Right face
        2, 3, 4, // Back face
        3, 0, 4, // Left face

        2, 1, 0, // First triangle of the base
        0, 3, 2 // Second triangle of the base
    };
    m_indicesSize = m_indices.size();
    generateBuffers();
}
