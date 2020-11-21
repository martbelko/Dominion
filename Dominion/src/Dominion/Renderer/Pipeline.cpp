#include "dmpch.h"
#include "Pipeline.h"

#include "Dominion/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLPipeline.h"

namespace Dominion {

	Ref<Pipeline> Pipeline::Create(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer, const Ref<InputLayout>& inputLayout)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLPipeline>(vertexBuffer, indexBuffer, inputLayout);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Pipeline> Pipeline::Create(const Ref<VertexBuffer>& vertexBuffer, const Ref<InputLayout>& inputLayout)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLPipeline>(vertexBuffer, inputLayout);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
