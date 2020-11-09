#version 330 core

out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform float u_TilingFactor;

void main()
{
	color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
}
