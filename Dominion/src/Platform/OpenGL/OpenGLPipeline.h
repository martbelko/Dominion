#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Pipeline.h"

namespace Dominion {

	class OpenGLPipeline : public Pipeline
	{
	public:
		OpenGLPipeline(Ref<VertexBuffer>& vertexBuffer, Ref<IndexBuffer>& indexBuffer, Ref<InputLayout>& inputLayout);

		virtual void Bind() const override;

		virtual Ref<IndexBuffer> GetIndexBuffer() const;
		virtual Ref<IndexBuffer> GetIndexBuffer() const override;
	private:
		Ref<VertexBuffer> m_VertexBuffer;
		Ref<IndexBuffer> m_IndexBuffer;
		Ref<InputLayout> m_InputLayout;
	};

}
