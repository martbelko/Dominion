#pragma once

#include <glm/glm.hpp>

namespace Dominion {

	class SettingsPanel
	{
	public:
		void OnImGuiRender();

		bool ShowPhysicsColliders() const { return mShowPhysicsColliders; }
		float GetDebugLineThickness() const { return mDebugLineThickness; }
		const glm::vec4& Get2DPhysicsCollidersColor() const { return m2DPhysicsCollidersColor; }
	private:
		bool mShowPhysicsColliders = true;
		float mDebugLineThickness = 2.0f;
		glm::vec4 m2DPhysicsCollidersColor = glm::vec4(0.1f, 0.2f, 0.8f, 1.0f);
	};

}
