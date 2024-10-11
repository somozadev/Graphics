#include "Terrain.h"

Terrain::Terrain(Shader* shader) : ref_shader(shader) 
{
    m_texture_manager = TextureManager();
}


void Terrain::loadTexture()
{
   m_textureID = TextureManager::loadTexture(m_height_map_path, GL_RGBA);
}
