#pragma once

#include "Dominion/Renderer/RendererAPI.h"

namespace Dominion {

	class RenderCommand
	{
	public:
		static void Init()
		{
			sRendererAPI->Init();
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			sRendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			sRendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			sRendererAPI->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			sRendererAPI->DrawIndexed(vertexArray, count);
		}
	private:
		static Scope<RendererAPI> sRendererAPI;
	};

}
