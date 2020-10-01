#include "dmpch.h"
#include "Buffer.h"

#include "Dominion/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Dominion {

	std::shared_ptr<Dominion::VertexBuffer> VertexBuffer::Create(const float* vertices, size_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	std::shared_ptr<Dominion::IndexBuffer> IndexBuffer::Create(const uint32_t* indices, unsigned int count)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return std::make_shared<OpenGLIndexBuffer>(indices, count);
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
