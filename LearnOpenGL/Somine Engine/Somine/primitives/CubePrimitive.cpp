#include "CubePrimitive.h"

#include <glm/glm/gtc/matrix_transform.hpp>

CubePrimitive::CubePrimitive()
{
    m_vertices = {
        // Front face
        -0.5f, -0.5f, 0.5f, // Bottom-left
        0.5f, -0.5f, 0.5f, // Bottom-right
        0.5f, 0.5f, 0.5f, // Top-right
        -0.5f, 0.5f, 0.5f, // Top-left
        // Back face
        -0.5f, -0.5f, -0.5f, // Bottom-left
        0.5f, -0.5f, -0.5f, // Bottom-right
        0.5f, 0.5f, -0.5f, // Top-right
        -0.5f, 0.5f, -0.5f // Top-left
    };
    m_indices = {
        0, 1, 2, 2, 3, 0, // Front face
        6, 5, 4, 4, 7, 6, // Back face
        0, 3, 7, 7, 4, 0, // Left face
        1, 5, 6, 6, 2, 1, // Right face
        3, 2, 6, 6, 7, 3, // Top face
        5, 1, 0, 0, 4, 5, // Bottom face
    };
    m_indicesSize = m_indices.size();
    generateBuffers();
}
