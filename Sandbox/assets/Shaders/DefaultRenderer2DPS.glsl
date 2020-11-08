#version 330 core

out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;

void main()
{
	color = u_Color;
}
