#include "dmpch.h"
#include "SubTexture.h"

namespace Dominion {

	SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
		: m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };
		m_TexCoords[1] = { max.x, min.y };
		m_TexCoords[2] = { max.x, max.y };
		m_TexCoords[3] = { min.x, max.y };
	}

	Ref<SubTexture2D> SubTexture2D::CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& size)
	{
		glm::vec2 min = { (coords.x * size.x) / texture->GetWidth(), (coords.y * size.y) / texture->GetHeight() };
		glm::vec2 max = { ((coords.x + 1) * size.x) / texture->GetWidth(), ((coords.y + 1) * size.x) / texture->GetHeight() };
		return CreateRef<SubTexture2D>(texture, min, max);
	}

}
