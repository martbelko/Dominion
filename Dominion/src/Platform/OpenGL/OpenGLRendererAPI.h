#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/RendererAPI.h"

namespace Dominion {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void ClearColorBuffer() override;
		virtual void DrawIndexed(const Ref<Pipeline> pipeline, uint32_t count) override;
		virtual void Draw(const Ref<Pipeline> pipeline, uint32_t count) override;
	};

}
