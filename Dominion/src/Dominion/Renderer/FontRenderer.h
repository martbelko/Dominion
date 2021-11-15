#pragma once

#include "Dominion/Renderer/NativeShader.h"

#include <filesystem>

namespace Dominion {

	class Camera;
	class VertexArray;

	class FontRenderer
	{
	public:
		FontRenderer();
		// TODO: Rule of 5
	public:
		void LoadFont(const std::filesystem::path& fontPath);

		void RenderText(const std::string& text, glm::vec2 position, float scale, const glm::vec3& color);
	private:
		NativeShader mShader;
		Ref<VertexArray> mVao;
	};

}
