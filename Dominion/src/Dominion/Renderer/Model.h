#pragma once

#include "Dominion/Renderer/Mesh.h"
#include "Dominion/Renderer/Shader.h"

#include <string>
#include <vector>

struct aiNode;
struct aiMesh;
struct aiScene;
struct aiMaterial;
enum aiTextureType;

namespace Dominion {

	class Model
	{
	public:
		Model(const std::string& path);
		void Draw(Ref<Shader>& shader);
	private:
		void LoadModel(const std::string& path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName);
	private:
		std::vector<MeshTexture> m_TexturesLoaded;
		std::vector<Mesh> m_Meshes;
		std::string m_Directory;
	};

}
