#include "ImguiHandler.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"
#include "Lights/DirectionalLight.h"
#include "Lights/PointLight.h"
#include "Lights/SpotLight.h"

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

void ImguiHandler::showFrameRate(const char* text, float deltaTime)
{
    if (ImGui::TreeNode(text))
    {
        float frameRate = 1.0f / deltaTime;
        ImGui::Text("Frame Rate: %.1f FPS", frameRate);
        ImGui::TreePop();
    }
}

void ImguiHandler::showMs(const char* text, float ms)
{
    if (ImGui::TreeNode(text))
    {
        ImGui::Text("Latency: %.1f ms", ms);
        ImGui::TreePop();
    }
}

void ImguiHandler::addColorModifier(const char* text, vector<GLfloat>& colorReference)
{
    if (ImGui::TreeNode(text))
    {
        ImGui::ColorEdit3(text, colorReference.data());
        ImGui::TreePop();
    }
}

void ImguiHandler::addSingleModel(const char* text, Transform* transform)
{
    if (ImGui::TreeNode(text))
    {
        float temp[3] = {transform->position.x, transform->position.y, transform->position.z};
        ImGui::SliderFloat3(text, temp, -100.0f, 100.0f);
        transform->move(temp[0], temp[1], temp[2]);
        ImGui::TreePop();
    }
}

void ImguiHandler::addInteger(const char* text, int* reference, int min, int max)
{
    if (ImGui::TreeNode(text))
    {
        ImGui::SliderInt(text, reference, min, max);
        ImGui::TreePop();
    }
}

void ImguiHandler::addFloat(const char* text, float* reference, float min, float max)
{
    if (ImGui::TreeNode(text))
    {
        ImGui::SliderFloat(text, reference, min, max);
        ImGui::TreePop();
    }
}


void ImguiHandler::addInteger(const char* text, int* reference)
{
    if (ImGui::TreeNode(text))
    {
        ImGui::SliderInt(text, reference, 2, 6);
        ImGui::TreePop();
    }
}

void ImguiHandler::addCheckBox(const char* text, bool* reference)
{
    if (ImGui::TreeNode(text))
    {
        ImGui::Checkbox(text, reference);
        ImGui::TreePop();
    }
}

void ImguiHandler::addModel(const char* modelName, Model* model)
{
    if (ImGui::CollapsingHeader("Model"))
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
}

void ImguiHandler::addPointLights(std::vector<PointLight*> lights)
{
    if (ImGui::CollapsingHeader("Point lights"))
    {
        int index = 0;
        for (const auto& light : lights)
        {
            if (ImGui::CollapsingHeader("Point Light##"))
            {
                ImGui::BeginGroup();

                if (light == nullptr || light->transform == nullptr) continue;

                // Posición de la luz
                std::string label = "PointLight_Position##" + std::to_string(index);
                float temp[3] = {
                    light->transform->position.x, light->transform->position.y, light->transform->position.z
                };
                if (ImGui::SliderFloat3(label.c_str(), temp, -20.0f, 20.0f))
                {
                    light->transform->move(temp[0], temp[1], temp[2]);
                }
                ImGui::EndGroup();
                ImGui::BeginGroup();
                label = "PointLight_Ambient##" + std::to_string(index);
                ImGui::SliderFloat(label.c_str(), &light->m_ambient_intensity, 0.0f, 1.0f);
                label = "PointLight_Diffuse Intensity##" + std::to_string(index);
                ImGui::SliderFloat(label.c_str(), &light->m_diffuse_intensity, 0.0f, 1.0f);
                label = "PointLight_Color##" + std::to_string(index);
                ImGui::ColorEdit3(label.c_str(), &light->m_color[0]);

                label = "PointLight_Radius##" + std::to_string(index);
                ImGui::SliderFloat(label.c_str(), &light->m_radius, 1.0f, 100.0f);
                ImGui::EndGroup();
                ImGui::BeginGroup();
                label = "PointLight_Constant Attenuation##" + std::to_string(index);

                ImGui::SliderFloat(label.c_str(), &light->m_constant_attenuation, 0.0f, 10.0f);

                label = "PointLight_Linear Attenuation##" + std::to_string(index);

                ImGui::SliderFloat(label.c_str(), &light->m_linear_attenuation, 0.0f, 10.0f);

                label = "PointLight_Exponential Attenuation##" + std::to_string(index);

                ImGui::SliderFloat(label.c_str(), &light->m_exponential_attenuation, 0.0f, 10.0f);

                index++;
                ImGui::EndGroup();
            }
        }
    }
}

void ImguiHandler::addSpotLights(std::vector<SpotLight*> lights)
{
    if (ImGui::CollapsingHeader("Spot lights"))
    {
        int index = 0;
        for (const auto& light : lights)
        {
            if (ImGui::CollapsingHeader("Spot Light##"))
            {
                ImGui::BeginGroup();
                if (light == nullptr || light->transform == nullptr) continue;

                // Posición de la luz
                std::string label = "SpotLight_Position##" + std::to_string(index);
                float temp[3] = {
                    light->transform->position.x, light->transform->position.y, light->transform->position.z
                };
                if (ImGui::SliderFloat3(label.c_str(), temp, -20.0f, 20.0f))
                {
                    light->transform->move(temp[0], temp[1], temp[2]);
                }
                label = "SpotLight_Direction##" + std::to_string(index);
                float tempD[3] = {
                    light->transform->rotation.x, light->transform->rotation.y, light->transform->rotation.z
                };
                if (ImGui::SliderFloat3(label.c_str(), tempD, -360.0f, 360.0f))
                {
                    light->transform->rotate(tempD[0], tempD[1], tempD[2]);
                }
                ImGui::EndGroup();
                ImGui::BeginGroup();
                label = "SpotLight_Ambient##" + std::to_string(index);
                ImGui::SliderFloat(label.c_str(), &light->m_ambient_intensity, 0.0f, 1.0f);
                label = "SpotLight_Diffuse Intensity##" + std::to_string(index);
                ImGui::SliderFloat(label.c_str(), &light->m_diffuse_intensity, 0.0f, 1.0f);
                label = "SpotLight_Color##" + std::to_string(index);
                ImGui::ColorEdit3(label.c_str(), &light->m_color[0]);

                label = "SpotLight_Cutoff##" + std::to_string(index);
                ImGui::SliderFloat(label.c_str(), &light->m_cutoff, 0.0f, 90.0f);

                label = "SpotLight_Radius##" + std::to_string(index);
                ImGui::SliderFloat(label.c_str(), &light->m_radius, 1.0f, 100.0f);
                ImGui::EndGroup();
                ImGui::BeginGroup();
                label = "SpotLight_Constant Attenuation##" + std::to_string(index);

                ImGui::SliderFloat(label.c_str(), &light->m_constant_attenuation, 0.0f, 10.0f);

                label = "SpotLight_Linear Attenuation##" + std::to_string(index);

                ImGui::SliderFloat(label.c_str(), &light->m_linear_attenuation, 0.0f, 10.0f);

                label = "SpotLight_Exponential Attenuation##" + std::to_string(index);

                ImGui::SliderFloat(label.c_str(), &light->m_exponential_attenuation, 0.0f, 10.0f);

                index++;
                ImGui::EndGroup();
            }
        }
    }
}

void ImguiHandler::mainLight(DirectionalLight* light)
{
    if (ImGui::CollapsingHeader("Main Light"))
    {
        ImGui::BeginGroup();
        ImGui::Text("Main Light");

        float temp[3] = {light->transform->rotation.x, light->transform->rotation.y, light->transform->rotation.z};
        ImGui::SliderFloat3("direction", temp, -360.0f, 360.0f);
        light->transform->rotate(temp[0], temp[1], temp[2]);
        ImGui::EndGroup();
        ImGui::BeginGroup();
        ImGui::SliderFloat("Ambient", &light->m_ambient_intensity, 0.0f, 1.0f);
        ImGui::SliderFloat("Diffuse", &light->m_diffuse_intensity, 0.0f, 1.0f);
        ImGui::ColorEdit3("Color", &light->m_color[0]);
        ImGui::EndGroup();
    }
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
