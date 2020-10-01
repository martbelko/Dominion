#pragma once

#include "Dominion/Renderer/InputLayout.h"

namespace Dominion {

	class OpenGLInputLayout : public InputLayout
	{
	public:
		OpenGLInputLayout(const std::vector<InputLayoutElement>& elements, unsigned int stride);
		virtual ~OpenGLInputLayout() override;

		virtual void Bind() override;
	private:
		unsigned int m_RendererID;
	};

}
