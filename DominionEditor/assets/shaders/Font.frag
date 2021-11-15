#version 460 core

struct VertexOutput
{
	vec3 color;
	vec2 texCoord;
};

layout (location = 0) in VertexOutput Input;

out vec4 color;

uniform sampler2D text;

void main()
{
	float alpha = texture(text, Input.texCoord).r;
	if (alpha == 0.0)
		discard;
	color = vec4(Input.color, 1.0) * vec4(1.0, 1.0, 1.0, alpha);
}
