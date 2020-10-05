#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/RendererAPI.h"
#include "Dominion/Renderer/Pipeline.h"

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

		static void DrawIndexed(const Ref<Pipeline> pipeline, uint32_t count = 0)
		{
			s_RendererAPI->DrawIndexed(pipeline, count);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}
