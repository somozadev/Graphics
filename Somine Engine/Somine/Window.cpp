#include "Window.h"
#include <iostream>


Window::Window(const int width, const int height, const char* title): m_width(width), m_height(height), m_title(title)
{
    initGLFW();
    m_window = glfwCreateWindow(width, height, title,NULL, NULL);
    if (!m_window)
    {
        std::cout << "Failed to create GLFW window \n";
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cerr << "Failed to initialize GLAD\n";
}

void Window::initGLFW()
{
    //init GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }
    //tell glfw what version of opengl is being used 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //tell glfw we are using the core profile (only have the modern functions)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


GLFWwindow* Window::getGLFWWindow() const
{
    return m_window;
}

void Window::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::swapBuffers() const
{
    glfwSwapBuffers(m_window);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

Window::~Window()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
    glfwTerminate();
}
