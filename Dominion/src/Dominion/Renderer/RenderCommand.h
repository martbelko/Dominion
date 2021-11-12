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

		static void GetViewport(uint32_t& x, uint32_t& y, uint32_t& width, uint32_t& height)
		{
			sRendererAPI->GetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			sRendererAPI->SetClearColor(color);
		}

		static void Clear(RenderTarget target = RenderTarget::COLOR)
		{
			sRendererAPI->Clear(target);
		}

		static void SetLineWidth(float lineWidth)
		{
			sRendererAPI->SetLineWidth(lineWidth);
		}

		static void Draw(Topology topology, const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
		{
			sRendererAPI->Draw(topology, vertexArray, vertexCount);
		}

		static void DrawIndexed(Topology topology, const Ref<VertexArray>& vertexArray, uint32_t count = 0)
		{
			sRendererAPI->DrawIndexed(topology, vertexArray, count);
		}
	private:
		static Scope<RendererAPI> sRendererAPI;
	};

}
