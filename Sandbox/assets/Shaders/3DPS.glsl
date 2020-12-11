#version 330 core

in vec2 v_TexCoords;

uniform sampler2D texture_diffuse;

out vec4 color;

void main()
{
	color = texture(texture_diffuse, v_TexCoords);
}
