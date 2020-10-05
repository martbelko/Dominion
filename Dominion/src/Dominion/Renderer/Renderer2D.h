#pragma once

#include "Dominion/Core/Base.h"

namespace Dominion {

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene();
		static void EndScene();

		static void Submit();
	};

}
