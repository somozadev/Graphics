#pragma once
#include <unordered_map>
#include <vector>
#include <glad/glad.h>

#include "Camera.h"
#include "Helper.h"
#include "OldMesh.h"
#include "Shader.h"
#include "Window.h"
#include "assimpLoader/Model.h"
#include "primitives/grid/Grid.h"

class Renderer
{
private:
    // GLuint m_program{0};
    // Grid m_grid{20000, 1.0f};
    std::unordered_map<std::string, Shader> m_shaders;
    Camera m_camera;
    std::vector<GLfloat> bg_color;
    std::vector<OldMesh> meshes;

    float current_frame;
    float last_frame;

    bool m_wireframe{false};
    bool m_see_grid{true};
    Window* ref_window;
    Model cup{"resources/models/cup/coffee_cup_fbx.fbx"};
    Model backpack{"resources/models/backpack/backpack.obj"};
    Model cube{"resources/models/cube/cubeTest.fbx"};

public:
    float delta_time;
    Renderer(Window* window = nullptr);
    void init();
    void initShadersMap();
    void update(); //ref camera maybe?  
    ~Renderer();
};
