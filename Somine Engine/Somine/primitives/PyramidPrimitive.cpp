#include "PyramidPrimitive.h"

#include <glm/glm/gtc/matrix_transform.hpp>

PyramidPrimitive::PyramidPrimitive(): Model()
{
     std::vector<Vertex>  vertices = {
         
         {{-0.5f, 0.0f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},  // Bottom-left
         {{0.5f, 0.0f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Bottom-right 
         {{0.5f, 0.0f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Top-right 
         {{  -0.5f, 0.0f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}, // Top-left 
            {{  0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}}         // ApexF

    };

    
   std::vector<unsigned int> indices  = {
        0, 1, 4, // Front face
        1, 2, 4, // Right face
        2, 3, 4, // Back face
        3, 0, 4, // Left face

        2, 1, 0, // First triangle of the base
        0, 3, 2 // Second triangle of the base
    };
    Mesh cubeMesh(vertices, indices, std::vector<Texture>());
    addMesh(cubeMesh);
}
