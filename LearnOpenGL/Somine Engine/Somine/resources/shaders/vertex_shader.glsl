#version 330 core
layout (location = 0) in vec3 vertex_position;
//layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec2 aTexCoord;

uniform mat4 mvp; 

//out vec3 ourColor;
//out vec2 TexCoord;

void main()
{
    gl_Position = mvp * vec4(vertex_position, 1.0); 
  //  ourColor = aColor;
    //TexCoord = aTexCoord;
}