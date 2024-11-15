#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    unsigned int m_width;
    unsigned int m_height;

    Window(int width, int height, const char* title);
    ~Window();
    static void pollEvents();
    GLFWwindow* getGLFWWindow() const;
    void swapBuffers() const;
    bool shouldClose() const;

private:
    GLFWwindow* m_window{nullptr};
    const char* m_title{nullptr};
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void initGLFW();
};
