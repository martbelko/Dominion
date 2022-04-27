#include "SettingsPanel.h"

#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>

namespace Dominion {

	void SettingsPanel::OnImGuiRender()
	{
		if (ImGui::Begin("Settings"))
		{
			constexpr float rightPadding = 10.0f;
			{
				constexpr const char* text = "Show physics colliders";
				float width = ImGui::CalcTextSize(text).x + rightPadding;
				ImGui::PushItemWidth(-width);
				ImGui::Checkbox(text, &mShowPhysicsColliders);
				ImGui::PopItemWidth();
			}

			{
				constexpr const char* text = "Debug Line Thickness";
				float width = ImGui::CalcTextSize(text).x + rightPadding;
				ImGui::PushItemWidth(-width);
				ImGui::DragFloat(text, &mDebugLineThickness, 0.01f, 0.001f, 10.0f);
				ImGui::PopItemWidth();
			}

			{
				constexpr const char* text = "Debug Line Color";
				float width = ImGui::CalcTextSize(text).x + rightPadding;
				ImGui::PushItemWidth(-width);
				ImGui::ColorEdit4(text, glm::value_ptr(m2DPhysicsCollidersColor));
				ImGui::PopItemWidth();
			}
		}
		ImGui::End();
	}

}
