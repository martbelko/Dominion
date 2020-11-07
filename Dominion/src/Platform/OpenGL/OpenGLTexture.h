#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Texture.h"

#include <string_view>

namespace Dominion {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string_view& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;

		virtual void Bind() const override;
		virtual void Bind(uint32_t slot) const override;
	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
#ifdef DM_DEBUG
		std::string path;
#endif
	};

}
