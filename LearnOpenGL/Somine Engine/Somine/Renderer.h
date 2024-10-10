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
#include "primitives/CubePrimitive.h"
#include "primitives/PlanePrimitive.h"
#include "primitives/PyramidPrimitive.h"
#include "primitives/SpherePrimitive.h"
#include "primitives/grid/Grid.h"

class Renderer
{
private:
    // GLuint m_program{0};
    // Grid m_grid{20000, 1.0f};

    OldMesh m_grid;
    std::unordered_map<std::string, Shader> m_shaders;
    Camera m_camera;
    std::vector<GLfloat> bg_color;
    std::vector<Model> m_models;

    float current_frame;
    float last_frame;

    bool m_wireframe{false};
    bool m_see_grid{true};
    Window* ref_window;
    Model ar47{"resources/models/ar/Ar-47.fbx"};
    Model cup{"resources/models/cup/coffee_cup_fbx.fbx"};
    CubePrimitive primitiveTestCube{};
    PlanePrimitive primitiveTestPlane{};
    SpherePrimitive primitiveTestSphere{};
    PyramidPrimitive primitiveTestPyramid{};
    Model backpack{"resources/models/backpack/backpack.obj"};

public:
    float delta_time;
    Renderer(Window* window = nullptr);
    void init();
    void initShadersMap();
    void update(); 
    ~Renderer();
};
