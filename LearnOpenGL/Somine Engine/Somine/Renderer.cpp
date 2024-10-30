#include "Renderer.h"

#include <glm/glm/detail/type_mat4x4.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <GL/gl.h>

#include "Helper.h"
#include "ImguiHandler.h"
#include "Macros.h"
#include "stb_image.h"
#include "assimpLoader/Model.h"
#include "Transform.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Renderer::Renderer(Window* window) : m_camera(window)
{
    m_window = window;
    m_bg_color = {0.1f, 0.05f, 0.1f, 1.0f};
    init();
    initShadersMap();
    initModels();
    glfwSetFramebufferSizeCallback(window->getGLFWWindow(), framebuffer_size_callback);
    glfwSetWindowUserPointer(window->getGLFWWindow(), this);
}

void Renderer::initShadersMap()
{
    m_shaders.insert({
        "default", NEW(Shader, "resources/shaders/vertex_shader.glsl", "resources/shaders/fragment_shader.glsl")
    });
    m_shaders.insert({
        "grid",NEW(Shader, "resources/shaders/grid/vertex_shader.glsl", "resources/shaders/grid/fragment_shader.glsl")
    });
    m_shaders.insert({
        "assimp",NEW(Shader, "resources/shaders/assimp/vertex_shader.glsl",
                     "resources/shaders/assimp/fragment_shader.glsl")
    });
    m_shaders.insert({
        "fbo",NEW(Shader, "resources/shaders/multipass/TextureFBO/vertex_shader.glsl",
                  "resources/shaders/multipass/TextureFBO/fragment_shader.glsl")
    });
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
    m_models.emplace_back(m_light);
    Model* light = m_models.back();
    light->transform->move(0.0, 2.0, 5.0);
    light->transform->scale(0.2, 0.2, 0.2);
    light->setShaderRef(m_shaders["assimp"]);

    for (int i = 0; i < 1; ++i)
    {
        PointLight* p_light = NEW(PointLight, i);
        m_point_lights.emplace_back(p_light);
    
        m_models.emplace_back(p_light);
        Model* pm_light = m_models.back();
        pm_light->transform->move(1 + i, 1.0, 0.0);
        pm_light->setShaderRef(m_shaders["assimp"]);
    }
    
    m_models.emplace_back(NEW(Terrain, 20, 60, 0.20f, "resources/textures/tough_grass.jpg"));
    Model* terrain = m_models.back();
    terrain->transform->move(0.0f, -2.0f, 0.0f);
    terrain->transform->scale(1.0f, 1.0f, 1.0f);
    terrain->transform->rotate(0.0f, 0.0f, 0.0f);
    terrain->setShaderRef(m_shaders["assimp"]);

    m_models.emplace_back(m_ar47);
    Model* ar47 = m_models.back();
    ar47->transform->move(-8.0f, 0.0f, 0.0f);
    ar47->transform->scale(0.5f, 0.5f, 0.5f);
    ar47->transform->rotate(-90.0f, 0.0f, 0.0f);
    ar47->setShaderRef(m_shaders["assimp"]);

    m_models.emplace_back(NEW(Model, "resources/models/backpack/backpack.obj"));
    Model* backpack = m_models.back();
    backpack->transform->move(0.0f, 0.0f, 0.0f);
    backpack->transform->scale(0.25f, 0.25f, 0.25f);
    backpack->transform->rotate(0.0f, 0.0f, 0.0f);
    backpack->setShaderRef(m_shaders["assimp"]);

    m_models.emplace_back(NEW(Model, "resources/models/cup/coffee_cup_fbx.fbx"));
    Model* cup = m_models.back();
    cup->transform->move(2.0f, 0.0f, 0.0f);
    cup->transform->scale(1.5f, 1.5f, 1.5f);
    cup->transform->rotate(0.0f, 0.0f, 0.0f);
    cup->setShaderRef(m_shaders["assimp"]);


    //quad-mesh 

    std::vector<glm::vec2> m_quad_vertices(4);
    m_quad_vertices[0] = glm::vec2(-1, -1);
    m_quad_vertices[1] = glm::vec2(1, -1);
    m_quad_vertices[2] = glm::vec2(1, 1);
    m_quad_vertices[3] = glm::vec2(-1, 1);

    GLuint quadMeshVBO;

    glGenVertexArrays(1, &m_quadMeshVAO);
    glBindVertexArray(m_quadMeshVAO);

    glGenBuffers(1, &quadMeshVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadMeshVBO);
    glBufferData(GL_ARRAY_BUFFER, m_quad_vertices.size() * sizeof(glm::vec2), m_quad_vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2,GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLint m_viewport_size[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport_size);


    //quadmesh to texture
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glGenTextures(1, &m_textureFBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureFBO);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, (GLint)m_window->m_width, (GLint)m_window->m_height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureFBO, 0);


    //depth info
    glGenRenderbuffers(1, &m_RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLint)m_window->m_width, (GLint)m_window->m_height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);
}


void Renderer::update()
{
    calcDeltaTime();
    ImguiHandler::startWindow("settings");

    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);

    glClearColor(m_bg_color[0], m_bg_color[1], m_bg_color[2], m_bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_camera.getViewMatrix();
    glm::mat4 projection = m_camera.getProjectionMatrix();
    setupMatrices(projection, view);


    for (auto model : m_models)
    {
        m_light->calcLocalDirection(model->transform->getModelMatrix());
        m_light->setShaderRef(m_shaders["assimp"]);
        m_shaders["assimp"]->setVec3("camera_local_position", m_camera.getCameraLocalPosRelativeTo(model->transform->getModelMatrix()));
         for (const auto point_light : m_point_lights)
        {
            point_light->calcLocalPosition(model->transform->getModelMatrix());
            point_light->setShaderData();
        }
        model->draw();
    }
    drawGrid(projection, view);


    glBindFramebuffer(GL_FRAMEBUFFER, 0); //draw to the screen
    glViewport(0, 0, m_window->m_width, m_window->m_height);

    glClearColor(m_bg_color[0], m_bg_color[1], m_bg_color[2], m_bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT);

    //use shader corresponding the fbo technicque (4.ex. fxaa)


    m_shaders["fbo"]->use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureFBO);
    m_shaders["fbo"]->setInt("source_texture", 0);


    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(m_quadMeshVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBindVertexArray(0);


    ImguiHandler::addCheckBox("wireframe", &m_wireframe);
    ImguiHandler::addCheckBox("cell shading", &m_cell_shading);
    ImguiHandler::addInteger("cell shading levels", &m_cell_shading_levels);
    ImguiHandler::addCheckBox("grid", &m_see_grid);
    ImguiHandler::addColorModifier("bg color", m_bg_color);
    ImguiHandler::addModel("ar-47", m_ar47);
    ImguiHandler::mainLight(m_light);
    ImguiHandler::addPointLights(m_point_lights);
    ImguiHandler::draw();
}

void Renderer::setupMatrices(glm::mat4 projection, glm::mat4 view)
{
    m_shaders["assimp"]->use();
    m_shaders["assimp"]->setInt("n_point_lights",1);
    m_shaders["assimp"]->setUniformMatrix4fv("view", view);
    m_shaders["assimp"]->setUniformMatrix4fv("projection", projection);
    
    m_shaders["assimp"]->setBool("use_cell_shading", m_cell_shading);
    m_shaders["assimp"]->setInt("cell_shading_levels", m_cell_shading_levels);
}

void Renderer::calcDeltaTime()
{
    m_current_frame = static_cast<float>(glfwGetTime());
    m_delta_time = m_current_frame - m_last_frame;
    m_last_frame = m_current_frame;
    m_camera.update(m_delta_time);
}

void Renderer::drawGrid(glm::mat4 projection, glm::mat4 view)
{
    if (!m_see_grid) return;
    glBindVertexArray(m_grid.m_VAO);
    glm::mat4 mvp = projection * view * m_grid.getModelMatrix();
    m_shaders["grid"]->use();
    m_shaders["grid"]->setUniformMatrix4fv("mvp", mvp);
    glDrawArrays(GL_LINES, 0, m_grid.getVertices().size() / 3);
    glBindVertexArray(0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    renderer->m_window->m_width = width;
    renderer->m_window->m_height = height;
    if (renderer) renderer->resizeFramebuffer(width, height);
    glViewport(0, 0, width, height);
}

void Renderer::resizeFramebuffer(int width, int height)
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    // Delete old texture
    glDeleteTextures(1, &m_textureFBO);
    // Generate and bind a new texture with the updated size
    glGenTextures(1, &m_textureFBO);
    glBindTexture(GL_TEXTURE_2D, m_textureFBO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureFBO, 0);

    // Resize depth and stencil buffer if needed
    glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


Renderer::~Renderer()
{
    //glDelete* any other globject used

    for (auto shader : m_shaders)
        DELETE(const_cast<Shader*>(shader.second), Shader);


    for (auto model : m_models)
        DELETE(model, Model);
}
