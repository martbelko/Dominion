#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/RenderCommand.h"

namespace Dominion {

	class Renderer
	{
	public:
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};

}
