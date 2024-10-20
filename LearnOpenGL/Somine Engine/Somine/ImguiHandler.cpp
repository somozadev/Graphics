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

void ImguiHandler::addColorModifier(const char* text,vector<GLfloat>& colorReference)
{
    ImGui::ColorEdit3(text, colorReference.data());
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
