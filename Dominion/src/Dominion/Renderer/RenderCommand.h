#pragma once

#include "Dominion/Renderer/RendererAPI.h"

#include <memory>

namespace Dominion {

	class RenderCommand
	{
	public:
		static void Init()
		{
			s_RendererAPI->Init();
		}

		static void SetClearColor(float r, float g, float b, float a = 1.0f)
		{
			s_RendererAPI->SetClearColor(r, g, b, a);
		}

		static void Clear()
		{
			s_RendererAPI->Clear();
		}
	private:
		static std::unique_ptr<RendererAPI> s_RendererAPI;
	};

}
