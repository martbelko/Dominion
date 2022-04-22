#version 460 core

layout(location = 0) in vec3 a_WorldPosition;
layout(location = 1) in vec2 a_LocalPosition;
layout(location = 2) in vec4 a_Color;
layout(location = 3) in float a_Thickness;
layout(location = 4) in float a_Fade;
layout(location = 5) in int a_EntityID;

layout(std140, binding = 0) uniform Camera
{
	mat4 u_View;
	mat4 u_Projection;
	mat4 u_ViewProjection;
};

struct VertexOutput
{
	vec2 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) out VertexOutput Output;
layout (location = 4) out flat int v_EntityID;

void main()
{
	Output.localPosition = a_LocalPosition;
	Output.color = a_Color;
	Output.thickness = a_Thickness;
	Output.fade = a_Fade;
	v_EntityID = a_EntityID;

	gl_Position = u_ViewProjection * vec4(a_WorldPosition, 1.0);
}
