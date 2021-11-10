#pragma once

#include "Dominion/Renderer/RendererAPI.h"

namespace Dominion {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void SetLineWidth(float lineWidth) override;

		virtual void Draw(Topology topology, const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;
		virtual void DrawIndexed(Topology topology, const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
	};

}
