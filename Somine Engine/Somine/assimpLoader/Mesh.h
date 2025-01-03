﻿#pragma once
#include <vector>
#include <glad/glad.h>

#include "Material.h"
#include "Texture.h"
#include "Vertex.h"
#include "../Shader.h"

class Mesh
{
public:
    std::vector<Vertex> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<Texture> m_textures;
    

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    void draw(const Shader* shader);

private:
    GLuint m_VBO;
    GLuint m_EBO;
    GLuint m_VAO;
    GLuint m_white_texture;

    void createWhiteTexture();
    void setupMesh();
};
