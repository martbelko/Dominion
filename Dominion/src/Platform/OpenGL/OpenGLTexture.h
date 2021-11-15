#pragma once

#include "Dominion/Renderer/Texture.h"

#include <glad/glad.h>

namespace Dominion {

	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const Texture2DSpecification& textureSpecification);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return mSpecification.width;  }
		virtual uint32_t GetHeight() const override { return mSpecification.height; }
		virtual uint32_t GetRendererID() const override { return mRendererID; }

		virtual void SetData(void* data, uint32_t size) override;

		virtual void Bind(uint32_t slot = 0) const override;

		virtual bool IsLoaded() const override { return mIsLoaded; }

		virtual bool operator==(const Texture& other) const override
		{
			return mRendererID == ((OpenGLTexture2D&)other).mRendererID;
		}
	private:
		std::string mPath;
		bool mIsLoaded = false;
		uint32_t mRendererID;
		GLenum mDataFormatNative;
		Texture2DSpecification mSpecification;
	};

}
