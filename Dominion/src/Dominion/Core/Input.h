#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Core/KeyCodes.h"
#include "Dominion/Core/MouseCodes.h"

namespace Dominion {

	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMousePressed(MouseCode button);
		static std::pair<F32, F32> GetMousePosition();
		static F32 GetMouseX();
		static F32 GetMouseY();
	};

}
