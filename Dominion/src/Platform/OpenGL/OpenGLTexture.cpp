#include "dmpch.h"
#include "Platform/OpenGL/OpenGLTexture.h"

#include <stb_image.h>

namespace Dominion {

	uint32_t DominionInternalFormatToGL(InternalFormat internalFormat)
	{
		switch (internalFormat)
		{
			case InternalFormat::R8: return GL_R8;
			case InternalFormat::RGB8: return GL_RGB8;
			case InternalFormat::RGBA8: return GL_RGBA8;
		}

		DM_CORE_ASSERT(false, "Unknown InternalFormat");
		return 0;
	}

	uint32_t DominionDataFormatToGL(DataFormat dataFormat)
	{
		switch (dataFormat)
		{
			case DataFormat::R: return GL_RED;
			case DataFormat::RG: return GL_RG;
			case DataFormat::RGB: return GL_RGB;
			case DataFormat::RGBA: return GL_RGBA;
		}

		DM_CORE_ASSERT(false, "Unknown DataFormat");
		return 0;
	}

	uint32_t DominionTextureFilterToGL(TextureFilter textureFilter)
	{
		switch (textureFilter)
		{
			case TextureFilter::Nearest: return GL_NEAREST;
			case TextureFilter::Linear: return GL_LINEAR;
		}

		DM_CORE_ASSERT(false, "Unknown TextureFilter");
		return 0;
	}

	uint32_t DominionWrappingToGL(Wrapping wrapping)
	{
		switch (wrapping)
		{
			case Wrapping::Repeat: return GL_REPEAT;
			case Wrapping::ClampToEdge: return GL_CLAMP_TO_EDGE;
		}

		DM_CORE_ASSERT(false, "Unknown Wrapping");
		return 0;
	}

	OpenGLTexture2D::OpenGLTexture2D(const Texture2DSpecification& specification)
		: mSpecification(specification)
	{
		DM_PROFILE_FUNCTION();

		uint32_t internalFormat = DominionInternalFormatToGL(specification.internalFormat);
		mDataFormatNative = DominionDataFormatToGL(specification.dataFormat);

		glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
		glTextureStorage2D(mRendererID, 1, internalFormat, specification.width, specification.height);

		glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, DominionTextureFilterToGL(specification.minFilter));
		glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, DominionTextureFilterToGL(specification.magFilter));

		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, DominionWrappingToGL(specification.wrapS));
		glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, DominionWrappingToGL(specification.wrapT));
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: mPath(path)
	{
		DM_PROFILE_FUNCTION();

		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = nullptr;
		{
			DM_PROFILE_SCOPE("stbi_load - OpenGLTexture2D::OpenGLTexture2D(const std::string&)");
			data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		}

		if (data)
		{
			mIsLoaded = true;

			mSpecification.width = width;
			mSpecification.height = height;

			auto GetFormatFromChannels = [](uint32_t channels) -> std::pair<uint32_t, uint32_t>
			{
				switch (channels)
				{
					case 4: return { GL_RGBA8, GL_RGBA };
					case 3: return { GL_RGB8, GL_RGB };
					case 2: return { GL_RG8, GL_RG };
					case 1: return { GL_R8, GL_RED };
				}

				DM_CORE_ASSERT(false, "Invalid number of channels. Format not supported");
				return { 0, 0 };
			};

			auto [internalFormat, dataFormat] = GetFormatFromChannels(channels);
			mDataFormatNative = dataFormat;

			glCreateTextures(GL_TEXTURE_2D, 1, &mRendererID);
			glTextureStorage2D(mRendererID, 1, internalFormat, mSpecification.width, mSpecification.height);

			glTextureParameteri(mRendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(mRendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(mRendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(mRendererID, 0, 0, 0, mSpecification.width, mSpecification.height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data);
		}
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		DM_PROFILE_FUNCTION();

		glDeleteTextures(1, &mRendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		DM_PROFILE_FUNCTION();

	#ifdef DM_ENABLE_ASSERTS
		auto GetBytesPerPixel = [](uint32_t dataFormat) -> uint32_t
		{
			switch (dataFormat)
			{
				case GL_RED: return 1;
				case GL_RG: return 2;
				case GL_RGB: return 3;
				case GL_RGBA: return 4;
			}

			DM_CORE_ASSERT(false, "Unknown DataFormat");
			return 0;
		};
		DM_CORE_ASSERT(size == mSpecification.width * mSpecification.height * GetBytesPerPixel(mDataFormatNative), "Data must be entire texture!");
	#endif
		glTextureSubImage2D(mRendererID, 0, 0, 0, mSpecification.width, mSpecification.height, mDataFormatNative, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		DM_PROFILE_FUNCTION();

		glBindTextureUnit(slot, mRendererID);
	}

}
