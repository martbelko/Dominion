#pragma once

#include <glm/glm.hpp>

#include "Dominion/Renderer/VertexArray.h"
#include "Dominion/Renderer/Topology.h"

namespace Dominion {

	enum class RenderTarget : uint32_t
	{
		DEPTH = BIT(0),
		STENCIL = BIT(1),
		COLOR = BIT(2)
	};

	static RenderTarget operator|(RenderTarget a, RenderTarget b)
	{
		return static_cast<RenderTarget>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
	}

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear(RenderTarget target) = 0;

		virtual void SetLineWidth(float lineWidth) = 0;

		virtual void Draw(Topology topology, const Ref<VertexArray>& vertexArray, uint32_t vertexCount) = 0;
		virtual void DrawIndexed(Topology topology, const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) = 0;

		static API GetAPI() { return sAPI; }
		static Scope<RendererAPI> Create();
	private:
		static API sAPI;
	};

}
