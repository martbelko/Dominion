#include "dmpch.h"
#include "Dominion/Renderer/VertexArray.h"

#include "Dominion/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Dominion {

	Ref<VertexArray> VertexArray::Create()
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
