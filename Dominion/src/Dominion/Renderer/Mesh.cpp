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

	void Mesh::Draw(Ref<Shader>& shader)
	{
		U32F diffuseNr = 1;
		U32F specularNr = 1;
		U32F normalNr = 1;
		U32F heightNr = 1;
		for (U32F i = 0; i < m_Textures.size(); ++i)
		{
			std::string number;
			const std::string& name = m_Textures[i].Type;
			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);

			std::string fullname = name + number;
			if (shader->HasUniform(fullname))
				shader->SetInt(fullname, i);

			m_Textures[i].Texture->Bind(i);
		}

		Dominion::Renderer::Submit(shader, m_Pipeline);
	}

}
