#version 330 core
out vec4 fragment_color;

in vec2 TexCoords;       
in vec4 fragment_pos;
in vec3 Normal; 
in mat3 TBN;
uniform sampler2D texture_diffuse1; 
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;



void main()
{vec3 normal = texture(texture_normal1, TexCoords).rgb; // Muestreo de la textura normal
    normal = normalize(normal * 2.0 - 1.0); // Transformar a rango [-1, 1]
    normal = normalize(TBN * normal); // Transformar la normal al espacio de mundo

    fragment_color = vec4(1.0, 1.0, 1.0, alpha); 

}