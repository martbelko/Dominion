#pragma once

#include "Texture.h"

namespace Dominion {

	class SubTexture2D
	{
	public:
		SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		const Ref<Texture2D>& GetTexture() const { return m_Texture; }
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }
	public:
		static Ref<SubTexture2D> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& size);
	private:
		Ref<Texture2D> m_Texture;
		glm::vec2 m_TexCoords[4];
	};

}
