#pragma once
#include "../assimpLoader/Model.h"
#include "../assimpLoader/Texture.h"

class Terrain : public Model
{
public:
    Terrain(int scale, int subdivision, float heightScale, const std::string& texturePath);

private:
    void initVertexAndIndices();
    void generateMesh();
    unsigned textureFromFile(const char* path, const std::string& directory);

    float m_width;
    float m_depth;
    float m_heightScale;
    float m_subdivision = 20.0f;
    std::string m_texturePath;
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<Texture> m_textures;
};
