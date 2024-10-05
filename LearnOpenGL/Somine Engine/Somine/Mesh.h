#pragma once
#include <vector>
#include <glad/glad.h>
#include <glm/glm/detail/type_mat.hpp>
#include <glm/glm/detail/type_mat4x4.hpp>

class Mesh
{
protected:
    std::vector<GLfloat> m_vertices;
    std::vector<GLuint> m_indices;
    std::vector<GLuint> m_textures;
    size_t m_indicesSize;
    glm::mat4 m_model_matrix; 

public:
    
    GLuint m_VAO{0};
    GLuint m_VBO{0};
    GLuint m_EBO{0};

    void generateBuffers();
    std::vector<GLfloat> getVertices();
    std::vector<GLuint> getIndices();
    GLuint64 getIndicesSize();
    glm::mat4 getModelMatrix();
    Mesh();
    ~Mesh();
};
