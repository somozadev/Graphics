#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
//
// #include "IMGUI/imgui.h"
// #include "IMGUI/imgui_impl_glfw.h"
// #include "IMGUI/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "Window.h"


int main()
{

    std::cout << "C++ version: " << __cplusplus << std::endl;

    Window window(640, 360, "Somine Renderer");
    Renderer renderer = Renderer(&window);

    // ImGui::CreateContext();
    // ImGuiIO& io = ImGui::GetIO(); (void)io;
    // ImGui::StyleColorsDark();
    // ImGui_ImplGlfw_InitForOpenGL(window.getGLFWWindow(), true);
    // ImGui_ImplOpenGL3_Init("#version 330");
    
    while (!window.shouldClose())
    {
        window.pollEvents();
        renderer.update();
        window.swapBuffers();
    }
    //
    glfwTerminate();
    return 0;
}
