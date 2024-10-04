#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D ourTexture;
uniform sampler2D ourTextureTwo;

void main()
{
    FragColor = mix(texture(ourTexture, TexCoord),texture(ourTextureTwo, TexCoord), 0.2f) * vec4(ourColor, 1.0f);
}