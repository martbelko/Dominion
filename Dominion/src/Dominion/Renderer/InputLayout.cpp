#include "dmpch.h"
#include "InputLayout.h"

#include "Dominion/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLInputLayout.h"

namespace Dominion {

	Ref<InputLayout> InputLayout::Create(const std::initializer_list<InputLayoutElement>& elements_t)
	{
		std::vector<InputLayoutElement> elements(elements_t);

		uint32_t stride = 0;
		for (auto& element : elements)
		{
			element.offset = stride;
			stride += element.size;
		}

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLInputLayout>(elements, stride);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
