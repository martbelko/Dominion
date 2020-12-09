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

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(float r, float g, float b, float a = 1.0f)
		{
			s_RendererAPI->SetClearColor(r, g, b, a);
		}

		static void ClearColorBuffer()
		{
			s_RendererAPI->ClearColorBuffer();
		}

		static void ClearDepthBuffer()
		{
			s_RendererAPI->ClearDepthBuffer();
		}

		static void DrawIndexed(const Ref<Pipeline> pipeline, uint32_t count = 0)
		{
			s_RendererAPI->DrawIndexed(pipeline, count);
		}

		static void Draw(const Ref<Pipeline> pipeline, uint32_t count)
		{
			s_RendererAPI->Draw(pipeline, count);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}
