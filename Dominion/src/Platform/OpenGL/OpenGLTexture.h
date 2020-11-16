#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Texture.h"

#include <glad/glad.h>

#include <string_view>

namespace Dominion {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string_view& path);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		virtual ~OpenGLTexture2D() override;

		virtual uint32_t GetWidth() const override;
		virtual uint32_t GetHeight() const override;
		virtual const std::string& GetPath() const override { return m_Path; }

		virtual void SetData(const void* data, uint32_t size) override;

		virtual void Bind() const override;
		virtual void Bind(uint32_t slot) const override;

		virtual uint32_t GetRendererID() const override;

		virtual bool operator==(const Texture2D& other) const override;
	private:
		uint32_t m_RendererID;
		uint32_t m_Width, m_Height;
		GLenum m_InternalFormat, m_DataFormat;
		std::string m_Path;
	};

}
