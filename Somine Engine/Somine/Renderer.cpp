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
#include "IMGUI/imgui.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Renderer::Renderer(Window* window) : m_camera(window)
{
    m_window = window;
    m_bg_color = {0.1f, 0.05f, 0.1f, 1.0f};
    init();
    initShadersMap();
    initModels();
    initFramebuffer();

    glfwSetFramebufferSizeCallback(window->getGLFWWindow(), framebuffer_size_callback);
    glfwSetWindowUserPointer(window->getGLFWWindow(), this);
}

void Renderer::initShadersMap()
{
    m_shaders.insert({
        "depth_pass", NEW(Shader, "resources/shaders/multipass/forward/depth_pass/vertex_shader.glsl",
                          "resources/shaders/multipass/forward/depth_pass/fragment_shader.glsl")
    });
    m_shaders.insert({
        "stencil_pass", NEW(Shader, "resources/shaders/multipass/forward/stencil_pass/vertex_shader.glsl",
                            "resources/shaders/multipass/forward/stencil_pass/fragment_shader.glsl")
    });
    m_shaders.insert({
        "color_pass", NEW(Shader, "resources/shaders/multipass/forward/color_pass/vertex_shader.glsl",
                          "resources/shaders/multipass/forward/color_pass/fragment_shader.glsl")
    });
    m_shaders.insert({
        "light_pass", NEW(Shader, "resources/shaders/multipass/forward/light_pass/vertex_shader.glsl",
                          "resources/shaders/multipass/forward/light_pass/fragment_shader.glsl")
    });
    // m_shaders.insert({
    //     "shadowmap_pass", NEW(Shader, "resources/shaders/multipass/forward/shadowmap_pass/vertex_shader.glsl",
    //                           "resources/shaders/multipass/forward/shadowmap_pass/fragment_shader.glsl")
    // });
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
    m_shaders.insert({
        "antialiasing",NEW(Shader, "resources/shaders/antialiasing/vertex_shader.glsl",
                           "resources/shaders/antialiasing/fragment_shader.glsl")
    });
    m_shaders.insert({
        "dof",NEW(Shader, "resources/shaders/dof/vertex_shader.glsl",
                  "resources/shaders/dof/fragment_shader.glsl")
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
    m_models.emplace_back(NEW(Terrain, 20, 60, 0.20f, "resources/textures/tough_grass.jpg"));
    Model* terrain = m_models.back();
    terrain->transform->move(0.0f, -2.0f, 0.0f);
    terrain->transform->scale(1.0f, 1.0f, 1.0f);
    terrain->transform->rotate(0.0f, 0.0f, 0.0f);

    m_models.emplace_back(m_ar47);
    Model* ar47 = m_models.back();
    ar47->transform->move(-8.0f, 0.0f, 0.0f);
    ar47->transform->scale(0.5f, 0.5f, 0.5f);
    ar47->transform->rotate(-90.0f, 0.0f, 0.0f);

    m_models.emplace_back(NEW(Model, "resources/models/backpack/backpack.obj"));
    Model* backpack = m_models.back();
    backpack->transform->move(0.0f, 0.0f, 0.0f);
    backpack->transform->scale(0.25f, 0.25f, 0.25f);
    backpack->transform->rotate(0.0f, 0.0f, 0.0f);

    m_models.emplace_back(NEW(Model, "resources/models/cup/coffee_cup_fbx.fbx"));
    Model* cup = m_models.back();
    cup->transform->move(2.0f, 0.0f, 0.0f);
    cup->transform->scale(1.5f, 1.5f, 1.5f);
    cup->transform->rotate(0.0f, 0.0f, 0.0f);


    m_models.emplace_back(m_light);
    Model* light = m_models.back();
    light->transform->move(0.0, 2.0, 5.0);
    light->transform->scale(0.2, 0.2, 0.2);

    for (int i = 0; i < 1; ++i)
    {
        PointLight* p_light = NEW(PointLight, i);
        m_point_lights.emplace_back(p_light);

        m_models.emplace_back(p_light);
        Model* pm_light = m_models.back();
        pm_light->transform->move(1 + i, 1.0, 0.0);
    }

    for (int i = 0; i < 1; ++i)
    {
        SpotLight* s_light = NEW(SpotLight, i);
        m_spot_lights.emplace_back(s_light);

        m_models.emplace_back(s_light);
        Model* sm_light = m_models.back();
        sm_light->transform->move(1.0 + i, 2.0, 0.0);
    }
}


void Renderer::initFramebuffer()
{
    //quad-mesh 
    float quadVertices[] = {

        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    std::vector<glm::vec2> m_quad_vertices(4);
    m_quad_vertices[0] = glm::vec2(-1, -1);
    m_quad_vertices[1] = glm::vec2(1, -1);
    m_quad_vertices[2] = glm::vec2(1, 1);
    m_quad_vertices[3] = glm::vec2(-1, 1);

    GLuint quadMeshVBO;
    GLuint quadMeshEBO;

    glGenVertexArrays(1, &m_quadMeshVAO);
    glGenBuffers(1, &quadMeshVBO);
    glGenBuffers(1, &quadMeshEBO);

    glBindVertexArray(m_quadMeshVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadMeshVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadMeshEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLint m_viewport_size[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport_size);


    //quadmesh to texture
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glGenTextures(1, &m_textureFBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_textureFBO);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, (GLint)m_window->m_width, (GLint)m_window->m_height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_textureFBO, 0);
    glBindTexture(GL_TEXTURE_2D, 0);


    //depth info -> replaced RBO for depth + stencil for a depthTexture bc the DoF postpro needs to read it in the shader
    // glGenRenderbuffers(1, &m_RBO);
    // glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLint)m_window->m_width, (GLint)m_window->m_height);
    // glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_RBO);

    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, (GLint)m_window->m_width, (GLint)m_window->m_height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Error: FBO incomplete" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::initShadowmap()
{
    glGenFramebuffers(1, &m_shadowmap_FBO);
    glDisable(GL_BLEND);

    glGenTextures(1, &m_shadowmap_texture_FBO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_shadowmap_texture_FBO); //bind for reading
    glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT16, 512 * m_shadow_resolution, 512 * m_shadow_resolution, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, m_shadowmap_FBO); //bind for writing 
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_shadowmap_texture_FBO, 0);
    glDrawBuffer(GL_NONE);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Error: Shadowmap FBO no es completo" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::depthPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClear(GL_DEPTH_BUFFER_BIT);
    glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
    m_shaders["depth_pass"]->use();
    for (auto& model : m_models)
    {
        model->setShaderRef(m_shaders["depth_pass"]);
        model->draw(m_shaders["depth_pass"]);
    }
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::stencilPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //write control over stencil
    glStencilFunc(GL_ALWAYS, 1, 0xFF); //write a ref value 1
    glStencilMask(0xFF); //activate writing in stencil buffer
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    m_shaders["stencil_pass"]->use();
    for (auto& model : m_models)
    {
        model->setShaderRef(m_shaders["stencil_pass"]);
        model->draw(m_shaders["stencil_pass"]);
    }
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilMask(0x00); //block stencil writing for next passes
    glDisable(GL_STENCIL_TEST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::shadowmapPass()
{
    initShadowmap();

    //first sub-pass 
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_shadowmap_FBO);
    glViewport(0, 0, 512 * m_shadow_resolution, 512 * m_shadow_resolution);
    glClear(GL_DEPTH_BUFFER_BIT);
    m_shaders["shadowmap_pass"]->use();
    glm::mat4 shadowmap_view = glm::lookAt(m_light->transform->position, m_light->getLocalDirection(),
                                           glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 shadowmap_projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
    m_shaders["shadowmap_pass"]->setUniformMatrix4fv("view", shadowmap_view);
    m_shaders["shadowmap_pass"]->setUniformMatrix4fv("projection", shadowmap_projection);
    for (auto& model : m_models)
    {
        model->draw(m_shaders["shadowmap_pass"]);
    }

    // //second sub-pass 
    // glViewport(0, 0, m_window->m_width, m_window->m_height);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // m_shaders["shadowmap_pass"]->setInt("texture_shadowmap", 0);
    // glActiveTexture(0);
    // glBindTexture(GL_TEXTURE_2D, m_shadowmap_texture_FBO);
    // glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    // //RenderScene
}

void Renderer::colorPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glClearColor(m_bg_color[0], m_bg_color[1], m_bg_color[2], m_bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shaders["color_pass"]->use();
    for (auto& model : m_models)
    {
        /*if (model->isTransparent())
        {
            glEnable(GL_BLEND);  // Activate for transparent objects
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }*/
        model->setShaderRef(m_shaders["color_pass"]);
        model->draw(m_shaders["color_pass"]);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::lightPass()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glClearColor(m_bg_color[0], m_bg_color[1], m_bg_color[2], m_bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_shaders["light_pass"]->use();
    for (auto& model : m_models)
    {
        model->setShaderRef(m_shaders["light_pass"]);

        m_light->calcLocalDirection(model->transform->getModelMatrix());
        m_light->setShaderRef(m_shaders["light_pass"]);
        m_shaders["light_pass"]->setVec3("camera_local_position",
                                         m_camera.getCameraLocalPosRelativeTo(model->transform->getModelMatrix()));

        for (const auto point_light : m_point_lights)
        {
            point_light->setShaderRef(m_shaders["light_pass"]);
            point_light->calcLocalPosition(model->transform->getModelMatrix());
            point_light->setShaderData();
        }

        for (const auto spot_light : m_spot_lights)
        {
            spot_light->setShaderRef(m_shaders["light_pass"]);
            spot_light->calcLocalDirectionAndPosition(model->transform->getModelMatrix());
            spot_light->setShaderData();
        }
        model->draw(m_shaders["light_pass"]);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::update()
{
    calcDeltaTime();
    ImguiHandler::startWindow("settings");

    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);

    glClearColor(m_bg_color[0], m_bg_color[1], m_bg_color[2], m_bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_camera.getViewMatrix();
    glm::mat4 projection = m_camera.getProjectionMatrix();
    setupMatrices(projection, view);


    depthPass();
    stencilPass();
    colorPass();
    lightPass();
    // shadowmapPass();
    drawGrid(projection, view);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_window->m_width, m_window->m_height);
    glClearColor(m_bg_color[0], m_bg_color[1], m_bg_color[2], m_bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    if (m_dof)
    {
        m_shaders["dof"]->use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureFBO);
        m_shaders["dof"]->setInt("color_texture", 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, m_depthTexture);
        m_shaders["dof"]->setInt("depth_texture", 1);

        const auto inverse_screen_size = glm::vec2(1.0f / m_window->m_width, 1.0f / m_window->m_height);
        m_shaders["dof"]->setVec2("inverse_screen_size", inverse_screen_size);

        m_shaders["dof"]->setFloat("DOF_FOCUS_DISTANCE", m_dof_focus_distance);
        m_shaders["dof"]->setFloat("DOF_FOCUS_RANGE", m_dof_focus_range);
        m_shaders["dof"]->setFloat("DOF_BLUR_STRENGTH", m_dof_blur_strength);

        glBindVertexArray(m_quadMeshVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }

    else if (m_antialiasing)
    {
        glDisable(GL_DEPTH_TEST);

        m_shaders["antialiasing"]->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureFBO);
        m_shaders["antialiasing"]->setInt("screen_texture", 0);
        m_shaders["antialiasing"]->setInt("debug_mode", m_antialiasing_debug_mode);
        m_shaders["antialiasing"]->setFloat("FXAA_MAX_SPAN", m_antialiasing_max_span);
        m_shaders["antialiasing"]->setFloat("FXAA_LUMA_THRESHOLD", m_antialiasing_luma_threshold);
        m_shaders["antialiasing"]->setFloat("FXAA_MUL_REDUCE", (1.0 / m_antialiasing_mul_reduce));
        m_shaders["antialiasing"]->setFloat("FXAA_MIN_REDUCE", (1.0 / m_antialiasing_min_reduce));

        const auto inverse_screen_size = glm::vec2(1.0f / m_window->m_width, 1.0f / m_window->m_height);
        m_shaders["antialiasing"]->setVec2("inverse_screen_size", inverse_screen_size);

        glBindVertexArray(m_quadMeshVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        m_shaders["fbo"]->use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textureFBO);
        m_shaders["fbo"]->setInt("source_texture", 0);

        glBindVertexArray(m_quadMeshVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }

    if (ImGui::CollapsingHeader("Stats"))
    {
        ImguiHandler::showFrameRate("FPS", m_delta_time);
        ImguiHandler::showMs("ms", m_current_ms);
    }
    if (ImGui::CollapsingHeader("depth of field"))
    {
        ImguiHandler::addCheckBox("dof enabled", &m_dof);
        ImguiHandler::addFloat("focus distance", &m_dof_focus_distance, 0.01f, 1000.0f);
        ImguiHandler::addFloat("focus range", &m_dof_focus_range, 0.1f, 1000.0f);
        ImguiHandler::addFloat("blur strength", &m_dof_blur_strength, 0.0f, 10.0f);
    }
    if (ImGui::CollapsingHeader("antialiasing"))
    {
        ImguiHandler::addCheckBox("antialiasing enabled", &m_antialiasing);
        ImguiHandler::addInteger("debug mode", &m_antialiasing_debug_mode, 0, 3);
        ImguiHandler::addFloat("mul reduce", &m_antialiasing_mul_reduce, 1, 64);
        ImguiHandler::addFloat("min reduce", &m_antialiasing_min_reduce, 1, 128);
        ImguiHandler::addFloat("luma threshold", &m_antialiasing_luma_threshold, 0.00001f, 1.0f);
        ImguiHandler::addInteger("max span", &m_antialiasing_max_span, 1, 128);
    }
    if (ImGui::CollapsingHeader("Lights"))
    {
        ImguiHandler::mainLight(m_light);
        ImguiHandler::addPointLights(m_point_lights);
        ImguiHandler::addSpotLights(m_spot_lights);
    }
    if (ImGui::CollapsingHeader("Other Settings"))
    {
        ImguiHandler::addCheckBox("wireframe", &m_wireframe);
        ImguiHandler::addCheckBox("grayscale shading", &m_greyscale_shading);
        ImguiHandler::addCheckBox("dark n white shading", &m_dnw_shading);
        ImguiHandler::addCheckBox("cell shading", &m_cell_shading);
        ImguiHandler::addInteger("cell shading levels", &m_cell_shading_levels);
        ImguiHandler::addCheckBox("grid", &m_see_grid);
        ImguiHandler::addColorModifier("bg color", m_bg_color);
    }
    ImguiHandler::draw();
}

void Renderer::setupMatrices(glm::mat4 projection, glm::mat4 view)
{
    m_shaders["light_pass"]->use();
    m_shaders["light_pass"]->setInt("n_point_lights", 1);
    m_shaders["light_pass"]->setInt("n_spot_lights", 1);
    m_shaders["light_pass"]->setUniformMatrix4fv("view", view);
    m_shaders["light_pass"]->setUniformMatrix4fv("projection", projection);
    m_shaders["light_pass"]->setBool("use_cell_shading", m_cell_shading);
    m_shaders["light_pass"]->setBool("use_greyscale_shading", m_greyscale_shading);
    m_shaders["light_pass"]->setBool("use_dnw_shading", m_dnw_shading);
    m_shaders["light_pass"]->setInt("cell_shading_levels", m_cell_shading_levels);

    m_shaders["depth_pass"]->use();
    m_shaders["depth_pass"]->setUniformMatrix4fv("view", view);
    m_shaders["depth_pass"]->setUniformMatrix4fv("projection", projection);

    m_shaders["stencil_pass"]->use();
    m_shaders["stencil_pass"]->setUniformMatrix4fv("view", view);
    m_shaders["stencil_pass"]->setUniformMatrix4fv("projection", projection);

    m_shaders["color_pass"]->use();
    m_shaders["color_pass"]->setUniformMatrix4fv("view", view);
    m_shaders["color_pass"]->setUniformMatrix4fv("projection", projection);
}

void Renderer::calcDeltaTime()
{
    m_current_frame = static_cast<float>(glfwGetTime());
    m_delta_time = m_current_frame - m_last_frame;
    m_last_frame = m_current_frame;
    m_current_ms = m_delta_time * 1000.0f;
    m_camera.update(m_delta_time);
}

void Renderer::drawGrid(glm::mat4 projection, glm::mat4 view)
{
    if (!m_see_grid) return;
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    glBindVertexArray(m_grid.m_VAO);
    glm::mat4 mvp = projection * view * m_grid.getModelMatrix();
    m_shaders["grid"]->use();
    m_shaders["grid"]->setUniformMatrix4fv("mvp", mvp);
    glDrawArrays(GL_LINES, 0, m_grid.getVertices().size() / 3);
    glBindVertexArray(0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
    // glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    // glBindRenderbuffer(GL_RENDERBUFFER, 0);
    //
    glDeleteTextures(1, &m_depthTexture);
    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
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
