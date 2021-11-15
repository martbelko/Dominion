#version 460 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2D text;
uniform vec3 textColor;

void main()
{
	float alpha = texture(text, TexCoords).r;
	if (alpha == 0.0)
		discard;
	color = vec4(textColor, 1.0) * vec4(1.0, 1.0, 1.0, alpha);
}
