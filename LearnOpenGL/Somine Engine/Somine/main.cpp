#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>


#include "Renderer.h"
#include "Window.h"


int main()
{

    std::cout << "C++ version: " << __cplusplus << std::endl;

    Window window(640, 360, "Somine Renderer");
    Renderer renderer = Renderer(&window);
 
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
