#pragma once

#include "Dominion/Renderer/NativeShader.h"
#include "Dominion/Renderer/UniformBuffer.h"

#include <filesystem>

namespace Dominion {

	class Camera;
	class VertexArray;
	class Shader;

	class FontRenderer
	{
	public:
		FontRenderer();
		~FontRenderer();
		// TODO: Rule of 5
	public:
		void LoadFont(const std::filesystem::path& fontPath);

		void RenderText(const std::string& text, glm::vec2 position, float scale, const glm::vec3& color);
	private:
		Ref<Shader> mShader;
		Ref<VertexArray> mVao;

		struct FontRendererData
		{
			glm::mat4 projection;
		};

		FontRendererData mFontRendererData;
		Ref<UniformBuffer> mFontRendererUniformBuffer;
	};

}
