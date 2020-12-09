#include "dmpch.h"
#include "Mesh.h"

namespace Dominion {

	Mesh::Mesh(const F32* vertices, uint32_t size, const Ref<InputLayout>& layout)
	{
		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, size);
		m_Pipeline = Pipeline::Create(vb, layout);
	}

	Mesh::Mesh(const F32* vertices, uint32_t size, const uint32_t* indices, uint32_t count, const Ref<InputLayout>& layout)
	{
		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, size);
		Ref<IndexBuffer> ib = IndexBuffer::Create(indices, count);
		m_Pipeline = Pipeline::Create(vb, ib, layout);
	}

}
