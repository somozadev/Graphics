#version 330 core
out vec4 fragment_color;


 uniform sampler2D texture_diffuse1; 
 uniform sampler2D texture_normal1; 
 uniform sampler2D texture_specular1; 



void main()
{
    fragment_color = vec4(1.0, 1.0, 1.0, 1.0);
}