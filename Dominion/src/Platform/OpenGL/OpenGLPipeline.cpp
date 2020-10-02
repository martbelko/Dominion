#include "dmpch.h"
#include "OpenGLPipeline.h"

namespace Dominion {

	OpenGLPipeline::OpenGLPipeline(std::shared_ptr<VertexBuffer>& vertexBuffer, std::shared_ptr<IndexBuffer>& indexBuffer, std::shared_ptr<InputLayout>& inputLayout)
		: m_VertexBuffer(vertexBuffer), m_IndexBuffer(indexBuffer), m_InputLayout(inputLayout) {}

	void OpenGLPipeline::Bind()
	{
		m_VertexBuffer->Bind();
		m_IndexBuffer->Bind();
		m_InputLayout->Bind();
	}

	std::shared_ptr<Dominion::IndexBuffer> OpenGLPipeline::GetIndexBuffer() const
	{
		return m_IndexBuffer;
	}

}
