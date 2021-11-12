#include "SettingsPanel.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

namespace Dominion {

	void SettingsPanel::OnImGuiRender()
	{
		if (ImGui::Begin("Settings"))
		{
			ImGui::Checkbox("Show physics colliders", &mShowPhysicsColliders);
			ImGui::DragFloat("Debug Line Thickness", &mDebugLineThickness, 0.01f, 0.001f, 10.0f);
			ImGui::ColorEdit4("Debug Line Color", glm::value_ptr(m2DPhysicsCollidersColor));
		}
		ImGui::End();
	}

}
