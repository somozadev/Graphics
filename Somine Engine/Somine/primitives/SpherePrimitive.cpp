#include "SpherePrimitive.h"

#include <glm/glm/gtc/constants.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

SpherePrimitive::SpherePrimitive(): m_sectors(20), m_stacks(20)
{
    initVertexAndIndices();
    generateMesh();

}

SpherePrimitive::SpherePrimitive(int stacks, int sectors) : m_sectors(sectors), m_stacks(stacks)
{
    initVertexAndIndices();
    generateMesh();
}
void SpherePrimitive::generateMesh()
{
    Mesh sphereMesh(m_vertices, m_indices, std::vector<Texture>());
    addMesh(sphereMesh);
}

/*
 * The stacks loop generates latitud points from top to bot, sectors loop generates altitud points for
 * each latitude, basically figuring out the cartesian coordinates of each point in a sphere based on
 * radius r, longitud phi (0 - 2pi) and latitud thetha (0 - pi) angles 
 */
void SpherePrimitive::initVertexAndIndices()
{
    for (int i = 0; i <= m_stacks; ++i)
    {
        float stackAngle = glm::pi<float>() / 2 - i * (glm::pi<float>() / m_stacks); // from pi/2 to -pi/2
        float xy = cos(stackAngle); // r * cos(phi)
        float z = sin(stackAngle); // r * sin(phi)

        // Add (sectors + 1) vertices per stack
        for (int j = 0; j <= m_sectors; ++j)
        {
            float sectorAngle = j * (2 * glm::pi<float>() / m_sectors); // from 0 to 2pi

            float x = xy * cos(sectorAngle); // r * cos(phi) * cos(theta)
            float y = xy * sin(sectorAngle); // r * cos(phi) * sin(theta)
            m_vertices.emplace_back(Vertex{
                            { x, y, z },  // Position
                            { 0.0f, 0.0f, 0.0f },  // Normal (empty)
                            { 0.0f, 0.0f }         // Texture coordinates (empty)
                        });
        }
    }
    for (int i = 0; i < m_stacks; ++i)
    {
        for (int j = 0; j < m_sectors; ++j)
        {
            int first = (i * (m_sectors + 1)) + j;
            int second = first + m_sectors + 1;

            // First triangle
            m_indices.push_back(first);
            m_indices.push_back(second);
            m_indices.push_back(first + 1);

            // Second triangle
            m_indices.push_back(second);
            m_indices.push_back(second + 1);
            m_indices.push_back(first + 1);
        }
    }
}
