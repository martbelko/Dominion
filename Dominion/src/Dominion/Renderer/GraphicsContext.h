#pragma once

#include "Dominion/Core/Base.h"

namespace Dominion {

	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;
	};

}
