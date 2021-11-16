#version 460 core

struct VertexOutput
{
	vec3 color;
	vec2 texCoord;
};

layout (location = 0) in VertexOutput Input;

layout (binding = 0) uniform sampler2D uText;

layout (location = 0) out vec4 oColor;

void main()
{
	float alpha = texture(uText, Input.texCoord).r;
	if (alpha == 0.0)
		discard;
	oColor = vec4(Input.color, 1.0) * vec4(1.0, 1.0, 1.0, alpha);
}
