#include "dmpch.h"
#include "OpenGLPipeline.h"

namespace Dominion {

	OpenGLPipeline::OpenGLPipeline(const Ref<VertexBuffer>& vertexBuffer, const Ref<InputLayout>& inputLayout)
		: m_VertexBuffer(vertexBuffer), m_IndexBuffer(nullptr), m_InputLayout(inputLayout) {}

	OpenGLPipeline::OpenGLPipeline(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer, const Ref<InputLayout>& inputLayout)
		: m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_InputLayout(inputLayout) {}

	void OpenGLPipeline::Bind() const
	{
		DM_PROFILE_FUNCTION();

		m_VertexBuffer->Bind();
		if (m_IndexBuffer)
			m_IndexBuffer->Bind();
		m_InputLayout->Bind();
	}

	Ref<IndexBuffer> OpenGLPipeline::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

}
