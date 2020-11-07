#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Bindable.h"

namespace Dominion {

	class Texture : public Bindable
	{
	public:
		virtual ~Texture() = default;

		virtual void Bind() const = 0;
		virtual void Bind(uint32_t slot) const = 0;
	};

	class Texture2D : public Texture
	{
	public:
		virtual ~Texture2D() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		static Ref<Texture2D> Create(const std::string_view& filepath);
	private:
	};

}
