#pragma once

#include "Dominion/Core/KeyCodes.h"
#include "Dominion/Core/MouseCodes.h"

namespace Dominion {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMousePressed(MouseCode button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
