#pragma once
#include <unordered_map>
#include <vector>
#include <glad/glad.h>

#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "primitives/grid/Grid.h"

class Renderer
{
private:
    // GLuint m_program{0};
    Mesh m_grid;
    std::unordered_map<std::string, Shader> m_shaders;
    Camera m_camera;
    std::vector<GLfloat> bg_color;
    std::vector<Mesh> meshes;

    float current_frame;
    float last_frame; 
    
    bool m_wireframe{false};
    Window* ref_window;

public:
    float delta_time; 
    float m_aspectRatio;
    Renderer(Window* window = nullptr);
    void init();
    void initShadersMap();
    void update(); //ref camera maybe?  
    void updateProjection();
    
    
    ~Renderer();
};
