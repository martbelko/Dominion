#include "dmpch.h"
#include "Pipeline.h"

#include "Dominion/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLPipeline.h"

namespace Dominion {

	std::shared_ptr<Dominion::Pipeline> Pipeline::Create(std::shared_ptr<VertexBuffer>& vertexBuffer, std::shared_ptr<IndexBuffer>& indexBuffer, std::shared_ptr<InputLayout>& inputLayout)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None: DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return std::make_shared<OpenGLPipeline>(vertexBuffer, indexBuffer, inputLayout);
		}
	}

}
