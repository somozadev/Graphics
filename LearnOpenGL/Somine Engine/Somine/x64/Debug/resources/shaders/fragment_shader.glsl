#version 330 core
out vec4 fragment_color;

//in vec3 ourColor;
//in vec2 TexCoord;
//uniform sampler2D ourTexture;
//uniform sampler2D ourTextureTwo;

void main()
{
  //  fragment_color = mix(texture(ourTexture, TexCoord),texture(ourTextureTwo, TexCoord), 0.2f) * vec4(ourColor, 1.0f);
    fragment_color = vec4(1.0, 0.5, 0.2, 1.0); //Orange color;
}