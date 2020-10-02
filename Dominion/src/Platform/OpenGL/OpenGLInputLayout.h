#pragma once

#include "Dominion/Renderer/InputLayout.h"

namespace Dominion {


	class OpenGLInputLayoutElelent
	{
	public:
		OpenGLInputLayoutElelent(const InputLayoutElement& element);
	public:
		uint32_t type;
		uint32_t componentCount;
		void* offset;
	};

	class OpenGLInputLayout : public InputLayout
	{
	public:
		OpenGLInputLayout(const std::vector<InputLayoutElement>& elements, unsigned int stride);
		virtual ~OpenGLInputLayout() override;

		virtual void Bind() override;
	private:
		unsigned int m_RendererID;
		unsigned int m_Stride;
		std::vector<OpenGLInputLayoutElelent> m_Elements;
	};

}
