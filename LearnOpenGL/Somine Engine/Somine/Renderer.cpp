#include "Renderer.h"

#include <glm/glm/detail/type_mat4x4.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <iostream>
#include "Helper.h"
#include "ImguiHandler.h"
#include "Macros.h"
#include "stb_image.h"
#include "assimpLoader/Model.h"
#include "Transform.h"
#include "primitives/CubePrimitive.h"
#include "primitives/PlanePrimitive.h"
#include "primitives/PyramidPrimitive.h"
#include "primitives/SpherePrimitive.h"


Renderer::Renderer(Window* window) : m_camera(window)
{
    m_window = window;
    m_bg_color = {0.1f, 0.05f, 0.1f, 1.0f};
    init();
    initShadersMap();
    initModels();
}

void Renderer::initShadersMap()
{
    m_shaders.insert({
        "default", NEW(Shader, "resources/shaders/vertex_shader.glsl", "resources/shaders/fragment_shader.glsl")
    });
    m_shaders.insert({
    "light", NEW(Shader,"resources/shaders/lightSource/vertex_shader.glsl", "resources/shaders/lightSource/fragment_shader.glsl")
});
    m_shaders.insert({
        "grid",NEW(Shader,"resources/shaders/grid/vertex_shader.glsl", "resources/shaders/grid/fragment_shader.glsl")
    });
    m_shaders.insert({
        "assimp",NEW(Shader,"resources/shaders/assimp/vertex_shader.glsl", "resources/shaders/assimp/fragment_shader.glsl")
    });
    //
    // m_shaders.insert({
    //     "terrain", Shader("resources/shaders/terrain/vertex_shader.glsl", "resources/shaders/terrain/fragment_shader.glsl")
    // });
}

void Renderer::init()
{
    Helper::initRnd();
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_grid = Grid(20000, 1.0f);
}

void Renderer::initModels()
{
    Terrain terrain{"resources/textures/happy.png", 1.0f, 20, 1.0f};
    
    PlanePrimitive primitiveTestPlane{};
    SpherePrimitive primitiveTestSphere{};
    PyramidPrimitive primitiveTestPyramid{};
    
    m_models.emplace_back(SpherePrimitive {});
    auto &light = m_models.back();
    light.transform.move(22.0, 20.0, 15.0);
    light.transform.scale(0.2, 0.2, 0.2);
    light.setShaderRef(m_shaders["light"]);
    
    m_models.emplace_back( Model {"resources/models/ar/Ar-47.fbx"});
    auto &ar47 = m_models.back();
    ar47.transform.move(-8.0f, 0.0f, 0.0f);
    ar47.transform.scale(1.0f, 1.0f, 1.0f);
    ar47.transform.rotate(-90.0f, 0.0f, 0.0f);
    ar47.setShaderRef(m_shaders["assimp"]);

    m_models.emplace_back( Model {"resources/models/backpack/backpack.obj"});
    auto &backpack = m_models.back();
    backpack.transform.move(0.0f, 0.0f, 0.0f);
    backpack.transform.scale(1.0f, 1.0f, 1.0f);
    backpack.transform.rotate(0.0f, 0.0f, 0.0f);
    backpack.setShaderRef(m_shaders["assimp"]);

    m_models.emplace_back( Model {"resources/models/cup/coffee_cup_fbx.fbx"});
    auto &cup = m_models.back();
    cup.transform.move(2.0f, 0.0f, 0.0f);
    cup.transform.scale(1.5f, 1.5f, 1.5f);
    cup.transform.rotate(270.0f, 0.0f, 0.0f);
    cup.setShaderRef(m_shaders["assimp"]);

     m_models.emplace_back(CubePrimitive {});
     auto &primitiveTestCube = m_models.back();
     primitiveTestCube.transform.move(-5.0f, 0.0f, 0.0f);
     primitiveTestCube.transform.scale(0.5f, 0.5f, 0.5f);
     primitiveTestCube.transform.rotate(0.0f, 0.0f, 0.0f);
     primitiveTestCube.setShaderRef(m_shaders["assimp"]);
    
    // primitiveTestSphere.transform.move(-10.0f, 0.0f, 0.0f);
    // primitiveTestSphere.transform.scale(0.5f, 0.5f, 0.5f);
    // primitiveTestSphere.transform.rotate(0.0f, 0.0f, 0.0f);
    // m_models.push_back(primitiveTestSphere);
    // primitiveTestSphere.setShaderRef(m_shaders["assimp"]);
    //
    // primitiveTestPlane.transform.move(10.0f, 0.0f, 0.0f);
    // primitiveTestPlane.transform.scale(0.5f, 0.5f, 0.5f);
    // primitiveTestPlane.transform.rotate(0.0f, 0.0f, 0.0f);
    // m_models.push_back(primitiveTestPlane);
    // primitiveTestPlane.setShaderRef(m_shaders["assimp"]);
    //
    // primitiveTestPyramid.transform.move(15.0f, 0.0f, 0.0f);
    // primitiveTestPyramid.transform.scale(0.5f, 0.5f, 0.5f);
    // primitiveTestPyramid.transform.rotate(0.0f, 0.0f, 0.0f);
    // m_models.push_back(primitiveTestPyramid);
    // primitiveTestPyramid.setShaderRef(m_shaders["assimp"]);
    //
    // terrain.transform.move(10.0f, 0.0f, 0.0f);
    // terrain.transform.scale(0.5f, 0.5f, 0.5f);
    // terrain.transform.rotate(0.0f, 0.0f, 0.0f);
    // m_models.push_back(terrain);
    // terrain.setShaderRef(m_shaders["assimp"]);



}


void Renderer::update()
{
    ImguiHandler::startWindow("settings");
    
    m_current_frame = static_cast<float>(glfwGetTime());
    m_delta_time = m_current_frame - m_last_frame;
    m_last_frame = m_current_frame;
    m_camera.update(m_delta_time);

    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
    glClearColor(m_bg_color[0], m_bg_color[1], m_bg_color[2], m_bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_camera.getViewMatrix();
    glm::mat4 projection = m_camera.getProjectionMatrix();
    setupMatrices(projection, view);

    for (auto model : m_models)
    {
        model.draw();
    }

    drawGrid(projection, view);

    ImguiHandler::addCheckBox("wireframe", &m_wireframe);
    ImguiHandler::addCheckBox("grid", &m_see_grid);
    
    ImguiHandler::addColorModifier("bg color", m_bg_color);
    ImguiHandler::draw();

    

}
void Renderer::setupMatrices(glm::mat4 projection,glm::mat4 view)
{
    m_shaders["assimp"]->use();
    m_shaders["assimp"]->setUniformMatrix4fv("view", view);
    m_shaders["assimp"]->setUniformMatrix4fv("projection", projection);

    m_shaders["light"]->use();
    m_shaders["light"]->setUniformMatrix4fv("view", view);
    m_shaders["light"]->setUniformMatrix4fv("projection", projection);

    
    m_shaders["assimp"]->use();
    m_shaders["assimp"]->setVec3("light_position", m_light.transform.position);


}

void Renderer::drawGrid(glm::mat4 projection,glm::mat4 view)
{
    
    if (!m_see_grid) return;
    glBindVertexArray(m_grid.m_VAO);
    glm::mat4 mvp = projection * view * m_grid.getModelMatrix();
    m_shaders["grid"]->use();
    m_shaders["grid"]->setUniformMatrix4fv("mvp", mvp);
    glDrawArrays(GL_LINES, 0, m_grid.getVertices().size() / 3);
    glBindVertexArray(0);
}

Renderer::~Renderer()
{
    //glDelete* any other globject used
    
    for (auto shader : m_shaders)
        DELETE(const_cast<Shader*>(shader.second), Shader);

}
