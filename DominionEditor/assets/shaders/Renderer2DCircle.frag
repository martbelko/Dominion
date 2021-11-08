#version 460 core

layout(location = 0) out vec4 oColor;
layout(location = 1) out int oEntityID;

struct VertexOutput
{
	vec3 localPosition;
	vec4 color;
	float thickness;
	float fade;
};

layout (location = 0) in VertexOutput Input;
layout (location = 4) in flat int v_EntityID;

void main()
{
	// Calculate distance and fill circle with white
	float distance = 1.0 - length(Input.localPosition);
	float circle = smoothstep(0.0, Input.fade, distance);
	circle *= smoothstep(Input.thickness + Input.fade, Input.thickness, distance);

	if (circle < 0.01)
		discard;

	// Set output color
	oColor = Input.color;
	oColor.a *= circle;

	oEntityID = v_EntityID;
}
