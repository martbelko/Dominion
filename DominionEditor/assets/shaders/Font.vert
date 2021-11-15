#version 460 core

layout (location = 0) in vec4 aVertex; // <vec2 pos, vec2 tex>
layout (location = 1) in vec3 aColor;

layout(std140, binding = 1) uniform FontRenderer
{
	mat4 projection;
};

struct VertexOutput
{
	vec3 color;
	vec2 texCoord;
};

layout (location = 0) out VertexOutput oVertexOutput;

void main()
{
	gl_Position = projection * vec4(aVertex.xy, 0.0, 1.0);
	oVertexOutput.color = aColor;
	oVertexOutput.texCoord = aVertex.zw;
}
