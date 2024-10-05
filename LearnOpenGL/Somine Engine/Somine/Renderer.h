#pragma once
#include <vector>
#include <glad/glad.h>

#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"

class Renderer
{
private:
    // GLuint m_program{0};
    
    const Shader m_shaders;
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
    Renderer(const char* vertex_path = "shaders/vertex_shader.glsl",
             const char* fragment_path = "shaders/fragment_shader.glsl", Window* window = nullptr);
    void init();
    void update(float delta_time); //ref camera maybe?  
    void updateProjection();
    
    
    ~Renderer();
};
