#include "Grid.h"

Grid::Grid()
{
    init();
    generateBuffers();
    moveTo(0, 0, 0);
    rotate(45.0f, glm::vec3(0, 1, 0));
}

Grid::Grid(int size, float spacing): m_size(size), m_spacing(spacing)
{
    init();
    generateBuffers();
}


void Grid::init()
{
    for (int i = -m_size; i <= m_size; ++i)
    {
        m_vertices.push_back(i * m_spacing); // x1
        m_vertices.push_back(0); // y1
        m_vertices.push_back(-m_size * m_spacing); // z1

        m_vertices.push_back(i * m_spacing); // x2
        m_vertices.push_back(0); // y2
        m_vertices.push_back(m_size * m_spacing); // z2
    }
    //horizontal lilnes
    for (int i = -m_size; i <= m_size; ++i)
    {
        m_vertices.push_back(-m_size * m_spacing); // x1
        m_vertices.push_back(0); // y1
        m_vertices.push_back(i * m_spacing); // z1

        m_vertices.push_back(m_size * m_spacing); // x2
        m_vertices.push_back(0); // y2
        m_vertices.push_back(i * m_spacing); // z2
    }
}
