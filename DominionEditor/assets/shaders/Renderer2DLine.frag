#version 460 core

layout(location = 0) out vec4 oColor;
layout(location = 1) out int oEntityID;

struct VertexOutput
{
	vec4 color;
};

layout (location = 0) in VertexOutput Input;
layout (location = 1) in flat int vEntityID;

void main()
{
	oColor = Input.color;
	oEntityID = vEntityID;
}
