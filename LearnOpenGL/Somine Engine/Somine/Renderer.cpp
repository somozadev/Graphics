#include "Renderer.h"

#include <glm/glm/detail/type_mat4x4.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

#include "Helper.h"
#include "primitives/CubePrimitive.h"
#include "primitives/PlanePrimitive.h"
#include "primitives/PyramidPrimitive.h"
#include "primitives/SpherePrimitive.h"


Renderer::Renderer(const char* vertex_path, const char* fragment_path, Window* window): m_shaders(vertex_path,
        fragment_path), m_camera(window)
{
    ref_window = window;
    bg_color = {0.1f, 0.05f, 0.1f, 1.0f};
    init();
}

void Renderer::init()
//TODO: this should probably be done for each mesh, as each need its own vao vbo ebo (unless batch drawcalling?)
{
    Helper::initRnd();

    PlanePrimitive plane(10.0f, 10.0f);
    meshes.push_back(plane);

    CubePrimitive m_cube;
    meshes.push_back(m_cube);
    PyramidPrimitive m_pyramid;
    meshes.push_back(m_pyramid);
    SpherePrimitive m_sphere;
    meshes.push_back(m_sphere);
}

void Renderer::update()
{
    current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;

    updateProjection();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, m_wireframe ? GL_LINE : GL_FILL);
    glClearColor(bg_color[0], bg_color[1], bg_color[2], bg_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = m_camera.getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), m_aspectRatio, 0.1f, 100.0f);
    m_shaders.use();
    m_camera.update(delta_time);
    int x = -4;
    float angle = 20;
    for (size_t i = 0; i < meshes.size(); ++i)
    {
        if (i > 0)
        {
            meshes[i].moveTo(x, 1, -2);
            x += 4;
            meshes[i].rotate(angle, glm::vec3(1.0f, 1.0f, 1.0f));
            angle += 30;
        }

        glm::mat4 mvp = projection * view * meshes[i].getModelMatrix();
        m_shaders.setUniformMatrix4fv("mvp", mvp);

        glBindVertexArray(meshes[i].m_VAO);
        glDrawElements(GL_TRIANGLES, meshes[i].getIndicesSize(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void Renderer::updateProjection()
{
    int width = ref_window->m_width;
    int height = ref_window->m_height;
    if (height == 0) height = 1;
    m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

Renderer::~Renderer()
{
    //glDelete* any other globject used
    ref_window = nullptr;
    glDeleteProgram(m_shaders.id);
}
