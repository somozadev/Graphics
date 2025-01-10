#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "ImguiHandler.h"
#include "IMGUI/imgui.h"
#include "IMGUI/imgui_impl_glfw.h"
#include "IMGUI/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Window.h"
#include "Macros.h"

int main()
{
    ENABLE_MEMORY_LEAK_DETECTION();
    std::cout << "C++ version: " << __cplusplus << std::endl;
    Window* window = NEW(Window, 640, 360, "Somine Renderer");
    Renderer* renderer = NEW(Renderer, window);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window->getGLFWWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330");
    while (!window->shouldClose())
    {
    glfwSwapInterval(0); 
        window->pollEvents();
        renderer->update();
        window->swapBuffers();
    }
    ImguiHandler::endWindow();

    glfwTerminate();
    DELETE(window, Window);
    DELETE(renderer, Renderer);
    return 0;
}
