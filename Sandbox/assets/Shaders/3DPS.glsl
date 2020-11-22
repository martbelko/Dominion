#version 330 core

in vec4 v_Color;

uniform vec4 u_Color;

out vec4 color;

void main()
{
	color = v_Color * u_Color;
}
