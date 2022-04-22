#pragma once

#include <string>

#include "Dominion/Core/Base.h"

namespace Dominion {

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0;
	};

	enum class DataFormat
	{
		R = 0,
		RG,
		RGB,
		RGBA
	};

	enum class InternalFormat
	{
		R8 = 0,
		RGB8,
		RGBA8
	};

	enum class Wrapping
	{
		Repeat = 0,
		ClampToEdge
		// TODO: Add moooooree
	};

	enum class TextureFilter
	{
		Nearest = 0,
		Linear
	};

	struct Texture2DSpecification
	{
		uint32_t width, height;
		InternalFormat internalFormat;
		DataFormat dataFormat;
		Wrapping wrapS;
		Wrapping wrapT;
		TextureFilter minFilter;
		TextureFilter magFilter;
	};

	class Texture2D : public Texture
	{
	public:
		static Ref<Texture2D> Create(const Texture2DSpecification& textureSpecification);
		static Ref<Texture2D> Create(const std::string& path);
	};

}
