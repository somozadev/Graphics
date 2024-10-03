#version 330

out vec4 fragment_colour;
//uniform vec4 custom_colour; 
in vec3 custom_colour;

void main()
{
    fragment_colour = vec4(custom_colour, 1.0f); // vec4(1.0, 0.5, 0.2, 1.0) Orange color
}