#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Pipeline.h"

namespace Dominion {

	class OpenGLPipeline : public Pipeline
	{
	public:
		OpenGLPipeline(std::shared_ptr<VertexBuffer>& vertexBuffer, std::shared_ptr<IndexBuffer>& indexBuffer, std::shared_ptr<InputLayout>& inputLayout);

		virtual void Bind() override;

		virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const;
	private:
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		std::shared_ptr<InputLayout> m_InputLayout;
	};

}
