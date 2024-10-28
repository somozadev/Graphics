#include "ImguiHandler.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"

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


void ImguiHandler::addCheckBox(const char* text, bool* reference)
{
    ImGui::Checkbox(text, reference);
}

void ImguiHandler::scrollGroup(std::vector<Model> models)
{
    ImGui::BeginChild("Meshes");
    for (auto model : models)
    {
        //modelSettings, probably need to make the models vector of pointers to the models
    }
}

void ImguiHandler::mainLight(BaseLight* light)
{
    ImGui::BeginGroup(); 
    ImGui::Text("Main Light");
    float temp[3] = {light->transform->rotation.x, light->transform->rotation.y, light->transform->rotation.z};
    ImGui::SliderFloat3("direction", temp, -360.0f, 360.0f);
    light->transform->rotate(temp[0], temp[1], temp[2]);
    ImGui::SliderFloat("Ambient", &light->m_ambient_intensity, 0.0f, 1.0f);
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

void ImguiHandler::update(bool &wireframe,bool &see_grid, Model* light,std::vector<GLfloat> bg_color)
{
   addCheckBox("wireframe", &wireframe);
   addCheckBox("grid", &see_grid);
   addSingleModel("Light position", light->transform);
   addColorModifier("bg color", bg_color);
}
