#pragma once

#include "Dominion/Core/Base.h"
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
		OpenGLInputLayout(const std::vector<InputLayoutElement>& elements, uint32_t stride);
		virtual ~OpenGLInputLayout() override;

		virtual void Bind() const override;
	private:
		unsigned int m_RendererID;
		unsigned int m_Stride;
		std::vector<OpenGLInputLayoutElelent> m_Elements;
	};

}
