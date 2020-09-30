#pragma once

#include "Dominion/Renderer/RendererAPI.h"

namespace Dominion {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetClearColor(float r, float g, float b, float a) override;
		virtual void Clear() override;
	};

}
