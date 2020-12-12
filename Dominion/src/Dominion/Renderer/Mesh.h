#pragma once

#include "Dominion/Core/Base.h"

#include "Dominion/Renderer/Pipeline.h"
#include "Dominion/Renderer/InputLayout.h"

#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace Dominion {

	class Texture2D;
	class Shader;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct MeshTexture
	{
		Ref<Texture2D> Texture;
		std::string Type;
		std::string Path;
	};

	class Mesh
	{
	public:
		Mesh(const F32* vertices, U32F size, const std::vector<MeshTexture>& textures);
		Mesh(const F32* vertices, U32F size, const U32* indices, U32F count, const std::vector<MeshTexture>& textures);

		const Ref<Pipeline>& GetPipeline() const { return m_Pipeline; }

	private:
		std::vector<MeshTexture> m_Textures;
		Ref<Pipeline> m_Pipeline;
	};

}
