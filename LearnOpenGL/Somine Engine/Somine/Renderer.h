#pragma once
#include <unordered_map>
#include <vector>
#include <glad/glad.h>

#include "Camera.h"
#include "Helper.h"
#include "Macros.h"
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
    Camera m_camera;
    std::unordered_map<std::string, const Shader*> m_shaders;
    std::vector<GLfloat> m_bg_color;
    std::vector<Model*> m_models;

    float m_current_frame;
    float m_last_frame;

    bool m_wireframe{true};
    bool m_see_grid{true};

    SpherePrimitive* m_light = NEW(SpherePrimitive);


    GLuint m_FBO{0};
    GLuint m_textureFBO{0};
    GLuint m_quadMeshVAO{0};
    GLuint m_RBO{0};


    void drawGrid(glm::mat4 projection, glm::mat4 view);
    void setupMatrices(glm::mat4 projection, glm::mat4 view);
    void calcDeltaTime();

public:
    Window* m_window;
    float m_delta_time;
    Renderer(Window* window = nullptr);
    void init();
    void initModels();
    void initShadersMap();
    void update();
    ~Renderer();
    void resizeFramebuffer(int width, int height);
};
