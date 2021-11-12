#include "PerformancePanel.h"

#include "Dominion/Scene/Entity.h"
#include "Dominion/Scene/Components.h"

#include "Dominion/Renderer/Renderer2D.h"

#include <ImGui/imgui.h>

#include <string>

namespace Dominion {

	void PerformancePanel::OnImGuiRender()
	{
		if (ImGui::Begin("Stats"))
		{
			auto stats = Renderer2D::GetStats();
			ImGui::Text("Renderer2D Stats:");
			ImGui::Text("Draw Calls: %d", stats.drawCalls);
			ImGui::Text("Quads: %d", stats.quadCount);
			ImGui::Text("Circles: %d", stats.circleCount);
			ImGui::Text("Lines: %d", stats.lineCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		}
		ImGui::End();
	}

}
