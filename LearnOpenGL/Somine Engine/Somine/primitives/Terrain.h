#pragma once
#include "PlanePrimitive.h"
#include "../TextureManager.h"

class Terrain : public PlanePrimitive
{
    const char* m_height_map_path;
    int m_textureID;

    int m_heightmapWidth, m_heightmapHeight;
    float m_heightMultiplier;
    std::vector<float> m_heightmapData; 
public:
    Terrain( const std::string& heightmapPath, float scale = 1.0f, int subdivisions = 20, float heightMultiplier = 5.0f);
    void generateTerrainMesh();
    void loadHeightmap(const std::string& heightmapPath);
};
