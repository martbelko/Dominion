#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/RendererAPI.h"

namespace Dominion {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const Ref<Pipeline> pipeline, uint32_t count) override;
	};

}
