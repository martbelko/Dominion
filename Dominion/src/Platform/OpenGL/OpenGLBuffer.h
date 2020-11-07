#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Buffer.h"

namespace Dominion {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(const float* vertices, size_t size);
		virtual ~OpenGLVertexBuffer() override;

		virtual void Bind() const override;
	private:
		unsigned int m_RendererID;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t* indices, unsigned int count);
		virtual ~OpenGLIndexBuffer() override;

		virtual void Bind() const override;

		virtual unsigned int GetCount() const override;
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;
	};

}
