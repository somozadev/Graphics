#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


#include "Renderer.h"
#include "Shader.h"
#include "TextureManager.h"
#include "Window.h"

// void processInput(GLFWwindow* window)
// {
//     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//         glfwSetWindowShouldClose(window, true);
// }
//
// void framebuffer_size_callback(GLFWwindow* window, int width, int height)
// {
//     // make sure the viewport matches the new window dimensions; note that width and 
//     // height will be significantly larger than specified on retina displays.
//     glViewport(0, 0, width, height);
// }

int main()
{


    Window window(640, 360, "Somine Renderer");
    Renderer renderer = Renderer("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl", &window);
   
    // //if this is called before glad is initialize, errors
    // unsigned int texture_one = TextureManager::loadTexture("textures/container.jpg", GL_RGB);
    // unsigned int texture_two = TextureManager::loadTexture("textures/happy.png", GL_RGBA);
    //
    while (!window.shouldClose())
    {
        window.pollEvents();
        renderer.update(0.0f);
        window.swapBuffers();
    }
    //
    glfwTerminate();
    return 0;
}
