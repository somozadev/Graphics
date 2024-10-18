#include "Terrain.h"

#include <iostream>

#include "../stb_image.h"

Terrain::Terrain( const std::string& heightmapPath, float scale, int subdivisions, float heightMultiplier) :  PlanePrimitive(scale, subdivisions),  m_heightMultiplier(heightMultiplier)
{
    loadHeightmap(heightmapPath);
    generateTerrainMesh();
}
void Terrain::generateTerrainMesh()
{
    if (m_heightmapData.empty()) {
        std::cerr << "Heightmap data is empty. Cannot generate terrain." << std::endl;
        return;
    }

    int vertexCount = 0;

    for (int z = 0; z <= m_subdivision; ++z)
    {
        for (int x = 0; x <= m_subdivision; ++x)
        {
            // Calculate the corresponding pixel in the heightmap
            int mapX = static_cast<int>((x / static_cast<float>(m_subdivision)) * (m_heightmapWidth - 1));
            int mapZ = static_cast<int>((z / static_cast<float>(m_subdivision)) * (m_heightmapHeight - 1));

            // Get the height value from the heightmap and scale it
            float heightValue = m_heightmapData[mapZ * m_heightmapWidth + mapX];

            // Modify the Y position of the vertex
            m_vertices[vertexCount].position.y = heightValue * m_heightMultiplier;

            ++vertexCount;
        }
    }

    // Call the original function to create the mesh from vertices and indices
    generateMesh();
}
void Terrain::loadHeightmap(const std::string& heightmapPath)
{
    int channels;
    unsigned char* data = stbi_load(heightmapPath.c_str(), &m_heightmapWidth, &m_heightmapHeight, &channels, STBI_grey);
    if (!data)
    {
        std::cerr << "Failed to load heightmap: " << heightmapPath << std::endl;
        return;
    }

    // Resize the vector to store the height data
    m_heightmapData.resize(m_heightmapWidth * m_heightmapHeight);

    // Normalize the heightmap data (grayscale values between 0 and 1)
    for (int i = 0; i < m_heightmapWidth * m_heightmapHeight; ++i)
    {
        m_heightmapData[i] = data[i] / 255.0f;
    }

    // Free the image data
    stbi_image_free(data);
}
