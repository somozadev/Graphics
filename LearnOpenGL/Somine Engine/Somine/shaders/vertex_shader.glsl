#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model_matrix; 
uniform mat4 view_matrix; 
uniform mat4 projection_matrix; 

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 transform; 

void main()
{
    //gl_Position = transform * vec4(aPos, 1.0);
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(aPos, 1.0); 
    ourColor = aColor;
    TexCoord = aTexCoord;
}