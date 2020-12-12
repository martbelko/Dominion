#include "dmpch.h"
#include "Mesh.h"

#include "Dominion/Renderer/Shader.h"
#include "Dominion/Renderer/Texture.h"
#include "Dominion/Renderer/Renderer.h"

namespace Dominion {

	Mesh::Mesh(const F32* vertices, U32F size, const std::vector<MeshTexture>& textures)
		: m_Textures(textures)
	{
		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, size);
		Ref<InputLayout> il = InputLayout::Create(
			{
				{ "Position", DataType::Float3 },
				{ "Normal", DataType::Float3 },
				{ "TexCoord", DataType::Float2 },
				{ "Tangent", DataType::Float3 },
				{ "Bitangent", DataType::Float3 }
			});

		m_Pipeline = Pipeline::Create(vb, il);
	}

	Mesh::Mesh(const F32* vertices, U32F size, const U32* indices, U32F count, const std::vector<MeshTexture>& textures)
		: m_Textures(textures)
	{
		Ref<VertexBuffer> vb = VertexBuffer::Create(vertices, size);
		Ref<IndexBuffer> ib = IndexBuffer::Create(indices, count);
		Ref<InputLayout> il = InputLayout::Create(
			{
				{ "Position", DataType::Float3 },
				{ "Normal", DataType::Float3 },
				{ "TexCoord", DataType::Float2 },
				{ "Tangent", DataType::Float3 },
				{ "Bitangent", DataType::Float3 }
			});

		m_Pipeline = Pipeline::Create(vb, ib, il);
	}

}
