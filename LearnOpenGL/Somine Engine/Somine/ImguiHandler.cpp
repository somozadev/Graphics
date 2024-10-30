#include "ImguiHandler.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"

void ImguiHandler::newFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImguiHandler::startWindow(const char* name)
{
    newFrame();
    ImGui::Begin(name);
}

void ImguiHandler::addColorModifier(const char* text, vector<GLfloat>& colorReference)
{
    ImGui::ColorEdit3(text, colorReference.data());
}

void ImguiHandler::addSingleModel(const char* text, Transform* transform)
{
    float temp[3] = {transform->position.x, transform->position.y, transform->position.z};

    ImGui::SliderFloat3(text, temp, -100.0f, 100.0f);
    transform->move(temp[0], temp[1], temp[2]);
}

void ImguiHandler::addInteger(const char* text, int* reference)
{
    ImGui::SliderInt(text, reference, 2, 6);
}

void ImguiHandler::addCheckBox(const char* text, bool* reference)
{
    ImGui::Checkbox(text, reference);
}

void ImguiHandler::addModel(const char* modelName, Model* model)
{
    ImGui::BeginGroup();
    ImGui::Text(modelName);
    float tempT[3] = {model->transform->position.x, model->transform->position.y, model->transform->position.z};
    float tempR[3] = {model->transform->rotation.x, model->transform->rotation.y, model->transform->rotation.z};
    float tempS[3] = {model->transform->scalar.x, model->transform->scalar.y, model->transform->scalar.z};
    ImGui::SliderFloat3("position", tempT, -360.0f, 360.0f);
    model->transform->move(tempT[0], tempT[1], tempT[2]);
    ImGui::SliderFloat3("rotation", tempR, -360.0f, 360.0f);
    model->transform->rotate(tempR[0], tempR[1], tempR[2]);
    ImGui::SliderFloat3("scale", tempS, -360.0f, 360.0f);
    model->transform->scale(tempS[0], tempS[1], tempS[2]);
    ImGui::EndGroup();
}

void ImguiHandler::addPointLights(std::vector<PointLight*> lights)
{
    ImGui::BeginGroup();
    ImGui::Text("Point lights");

    int index = 0;
    for (const auto& light : lights)
    {
        if (light == nullptr || light->transform == nullptr) continue;

        // Posición de la luz
        std::string label = "Position##" + std::to_string(index);
        float temp[3] = {light->transform->position.x, light->transform->position.y, light->transform->position.z};
        if (ImGui::SliderFloat3(label.c_str(), temp, -20.0f, 20.0f)) {
            light->transform->move(temp[0], temp[1], temp[2]);
        }
        label = "Ambient##" + std::to_string(index);
        ImGui::SliderFloat(label.c_str(), &light->m_ambient_intensity, 0.0f, 1.0f);
        label = "Diffuse Intensity##" + std::to_string(index);
        ImGui::SliderFloat(label.c_str(), &light->m_diffuse_intensity, 0.0f, 1.0f);
        label = "Color##" + std::to_string(index);
        ImGui::ColorEdit3(label.c_str(), &light->m_color[0]);
        label = "Constant Attenuation##" + std::to_string(index);
        ImGui::SliderFloat(label.c_str(), &light->m_constant_attenuation, 0.0f, 10.0f);
        label = "Linear Attenuation##" + std::to_string(index);
        ImGui::SliderFloat(label.c_str(), &light->m_linear_attenuation, 0.0f, 10.0f);
        label = "Exponential Attenuation##" + std::to_string(index);
        ImGui::SliderFloat(label.c_str(), &light->m_exponential_attenuation, 0.0f, 10.0f);

        index++;
    }
    ImGui::EndGroup();
}

void ImguiHandler::mainLight(DirectionalLight* light)
{
    ImGui::BeginGroup();
    ImGui::Text("Main Light");
    float temp[3] = {light->transform->rotation.x, light->transform->rotation.y, light->transform->rotation.z};
    ImGui::SliderFloat3("direction", temp, -360.0f, 360.0f);
    light->transform->rotate(temp[0], temp[1], temp[2]);
    ImGui::SliderFloat("Ambient", &light->m_ambient_intensity, 0.0f, 1.0f);
    ImGui::SliderFloat("Diffuse", &light->m_diffuse_intensity, 0.0f, 1.0f);
    ImGui::ColorEdit3("Color", &light->m_color[0]);
    ImGui::EndGroup();
}

void ImguiHandler::draw()
{
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImguiHandler::endWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImguiHandler::update(bool& wireframe, bool& see_grid, Model* light, std::vector<GLfloat> bg_color)
{
    addCheckBox("wireframe", &wireframe);
    addCheckBox("grid", &see_grid);
    addSingleModel("Light position", light->transform);
    addColorModifier("bg color", bg_color);
}
