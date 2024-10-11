#pragma once
#include "PlanePrimitive.h"
#include "../TextureManager.h"

class Terrain : public PlanePrimitive
{
    const char* m_height_map_path;
    TextureManager m_texture_manager;
    int m_textureID;
    Shader* ref_shader;
public:
    Terrain(Shader* shader);
    void loadTexture();
    void applyVertexDeviations();
};
