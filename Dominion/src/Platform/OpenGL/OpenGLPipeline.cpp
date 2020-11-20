#include "dmpch.h"
#include "OpenGLPipeline.h"

namespace Dominion {

	OpenGLPipeline::OpenGLPipeline(Ref<VertexBuffer>& vertexBuffer, Ref<InputLayout>& inputLayout)
		: m_VertexBuffer(vertexBuffer), m_IndexBuffer(nullptr), m_InputLayout(inputLayout) {}

	OpenGLPipeline::OpenGLPipeline(Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<InputLayout>& inputLayout)
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
