#pragma once

#include <glm/glm.hpp>

#include "Dominion/Core/KeyCodes.h"
#include "Dominion/Core/MouseCodes.h"

namespace Dominion {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
