#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include <glm/glm/glm.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>


#include "Shader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/*Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
    "}\n\0";

*/
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
int main()
{
    //init GLFW
    glfwInit();
    //tell glfw what version of opengl is being used 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //tell glfw we are using the core profile (only have the modern functions)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* GLfloat vertices[] =
    // {
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, -0.5f, 0.0f,
    //     0.5f, 0.5f, 0.0f,
    //
    //     -0.5f, -0.5f, 0.0f,
    //     0.5f, 0.5f, 0.0f,
    //     -0.5f, 0.5f, 0.0f
     };*/

    GLfloat vertices[] = { // positions       colors     texture coords
        0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,        1.0f, 1.0f,// top right
        0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,       1.0f, 0.0f,// bottom right
       -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,        0.0f, 0.0f,// bottom left
       -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 0.0f,       0.0f, 1.0f // top left 
   };
    unsigned int indices[] = { 
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

     
    //Create a glfwWindow object of 800 by 800 pixels and give it a name 
    GLFWwindow* window = glfwCreateWindow(800, 600, "Somine Renderer", NULL, NULL);
    //error check if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to crete GLFW window \n";
        glfwTerminate();
        return -1;
    }
    //introduce the window into current opengl context 
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //load glad so it configures opengl 
    // gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    //specify the viewport of opengl window, from x0 y0 to x80 y800
    glViewport(0, 0, 800, 600);


    /* GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // glShaderSource(vertexShader, 1, &vertexShaderSource,NULL);
    // glCompileShader(vertexShader);
    //
    // GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // glShaderSource(fragmentShader, 1, &fragmentShaderSource,NULL);
    // glCompileShader(fragmentShader);
    //
    // GLuint shaderProgram = glCreateProgram();
    // glAttachShader(shaderProgram, vertexShader);
    // glAttachShader(shaderProgram, fragmentShader);
    // glLinkProgram(shaderProgram);
    //
    // glDeleteShader(vertexShader);
    // glDeleteShader(fragmentShader); */
    const Shader shaders("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl"); //if this is called before glad is initialize, errors

    int width, height, nr_channels;
    unsigned char *texture_data = stbi_load("textures/container.jpg", &width, &height, &nr_channels, 0);
    unsigned int texture, texture_two;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    if(texture_data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(texture_data);
    glGenTextures(1, &texture_two);
    glBindTexture(GL_TEXTURE_2D, texture_two);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    unsigned char *texture_data_two = stbi_load("textures/happy.png", &width, &height, &nr_channels, 0);
    
    if(texture_data_two)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data_two);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(texture_data_two);
    

    
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    GLuint VAO, VBO;    
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (void*)(6*sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0); 

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    



    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //normal mode



    glm::mat4 model_matrix = glm::mat4(1.0f);
    glm::mat4 view_matrix  = glm::mat4(1.0f);
    glm::mat4 projection_matrix;
    model_matrix = glm::rotate(model_matrix, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view_matrix = glm::translate(view_matrix, glm::vec3(0.0f, 0.0f, -3.0f )); 
    projection_matrix = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f );

    


    shaders.use();
    shaders.setInt("ourTexture", 0);
    shaders.setInt("ourTextureTwo", 1);
    
    //main while loop
    while (!glfwWindowShouldClose(window))
    {
        //specify the background color 
        glClearColor(0.1f, 0.05f, 0.1f, 1.0f);
        //clean the back buffer and assign the new color to it 
        glClear(GL_COLOR_BUFFER_BIT);
        // glUseProgram(shaderProgram);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_two);

        shaders.setUniformMatrix4fv("model_matrix",model_matrix);
        shaders.setUniformMatrix4fv("view_matrix",view_matrix);
        shaders.setUniformMatrix4fv("projection_matrix",projection_matrix);

        shaders.use();
        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, 6);        
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //swap the back buffer with the front buffer 
        glfwSwapBuffers(window);

        //handle all glfw events
        processInput(window);


        glfwPollEvents();
    }
    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glDeleteProgram(shaderProgram);

    //destroy window
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
