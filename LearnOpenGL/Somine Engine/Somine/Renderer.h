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
#include "primitives/Terrain.h"
#include "primitives/grid/Grid.h"

class Renderer
{
private:

    OldMesh m_grid;
    std::unordered_map<std::string,const Shader*> m_shaders;
    Camera m_camera;
    std::vector<GLfloat> m_bg_color;
    std::vector<Model> m_models;

    float m_current_frame;
    float m_last_frame;

    bool m_wireframe{false};
    bool m_see_grid{true};
    Window* m_window;
    Terrain m_terrain{"resources/textures/happy.png", 1.0f, 20, 1.0f};
    Model m_ar47{"resources/models/ar/Ar-47.fbx"};
    Model m_cup{"resources/models/cup/coffee_cup_fbx.fbx"};
    CubePrimitive m_primitiveTestCube{};
    PlanePrimitive m_primitiveTestPlane{};
    SpherePrimitive m_primitiveTestSphere{};
    SpherePrimitive m_light{};
    PyramidPrimitive m_primitiveTestPyramid{};
    Model m_backpack{"resources/models/backpack/backpack.obj"};
    void drawGrid(glm::mat4 projection,glm::mat4 view);
    void setupMatrices(glm::mat4 projection,glm::mat4 view);
public:
    float m_delta_time;
    Renderer(Window* window = nullptr);
    void init();
    void initModels();
    void initShadersMap();
    void update();
    ~Renderer();
};
