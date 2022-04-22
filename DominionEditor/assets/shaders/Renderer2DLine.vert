#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec4 aColor;
layout(location = 2) in int aEntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec4 color;
};

layout (location = 0) out VertexOutput Output;
layout (location = 1) out flat int vEntityID;

void main()
{
	Output.color = aColor;
	vEntityID = aEntityID;

	gl_Position = u_ViewProjection * vec4(aPosition, 1.0);
}
