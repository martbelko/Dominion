#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Buffer.h"

namespace Dominion {

	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(const float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer() override;

		virtual void Bind() const override;

		virtual void SetData(const void* data, uint32_t size) override;
		virtual uint32_t GetSize() const override { return m_Size; };
	private:
		unsigned int m_RendererID;
		uint32_t m_Size;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer() override;

		virtual void Bind() const override;

		virtual unsigned int GetCount() const override;
	private:
		unsigned int m_RendererID;
		unsigned int m_Count;
	};

}
