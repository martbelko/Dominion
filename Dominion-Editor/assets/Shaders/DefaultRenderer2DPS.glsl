#version 330 core

out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

void main()
{
	vec4 texColor = v_Color;

	// Determine right index into the sampler2D array
	if (v_TexIndex < 0.1)
		texColor *= texture(u_Textures[0], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 1.1)
		texColor *= texture(u_Textures[1], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 2.1)
		texColor *= texture(u_Textures[2], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 3.1)
		texColor *= texture(u_Textures[3], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 4.1)
		texColor *= texture(u_Textures[4], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 5.1)
		texColor *= texture(u_Textures[5], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 6.1)
		texColor *= texture(u_Textures[6], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 7.1)
		texColor *= texture(u_Textures[7], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 8.1)
		texColor *= texture(u_Textures[8], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 9.1)
		texColor *= texture(u_Textures[9], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 10.1)
		texColor *= texture(u_Textures[10], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 11.1)
		texColor *= texture(u_Textures[11], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 12.1)
		texColor *= texture(u_Textures[12], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 13.1)
		texColor *= texture(u_Textures[13], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 14.1)
		texColor *= texture(u_Textures[14], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 15.1)
		texColor *= texture(u_Textures[15], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 16.1)
		texColor *= texture(u_Textures[16], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 17.1)
		texColor *= texture(u_Textures[17], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 18.1)
		texColor *= texture(u_Textures[18], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 19.1)
		texColor *= texture(u_Textures[19], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 20.1)
		texColor *= texture(u_Textures[20], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 21.1)
		texColor *= texture(u_Textures[21], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 22.1)
		texColor *= texture(u_Textures[22], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 23.1)
		texColor *= texture(u_Textures[23], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 24.1)
		texColor *= texture(u_Textures[24], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 25.1)
		texColor *= texture(u_Textures[25], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 26.1)
		texColor *= texture(u_Textures[26], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 27.1)
		texColor *= texture(u_Textures[27], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 28.1)
		texColor *= texture(u_Textures[28], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 29.1)
		texColor *= texture(u_Textures[29], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 30.1)
		texColor *= texture(u_Textures[30], v_TexCoord * v_TilingFactor);
	else if (v_TexIndex < 31.1)
		texColor *= texture(u_Textures[31], v_TexCoord * v_TilingFactor);

	color = texColor;
}
