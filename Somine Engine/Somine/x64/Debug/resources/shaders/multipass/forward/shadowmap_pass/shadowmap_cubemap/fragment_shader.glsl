#version 330 core

out float fragment_depth;
in vec3 fragment_position;   

uniform vec3 light_position;
uniform float far_plane;

void main()
{
  float distance = length(fragment_position.xyz - light_position);
    fragment_depth = distance / far_plane;

}

