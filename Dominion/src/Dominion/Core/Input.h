#pragma once

namespace Dominion {

	class Input
	{
	public:
		static bool IsKeyPressed(int keycode);
		static bool IsMousePressed(int button);
		static std::pair<float, float> GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};

}
