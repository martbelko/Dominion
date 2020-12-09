#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Bindable.h"

#include <memory>

namespace Dominion {

	class VertexBuffer : public Bindable
	{
	public:
		virtual void SetData(const void* data, uint32_t size) = 0;
		virtual uint32_t GetSize() const = 0;

		static Ref<VertexBuffer> Create(uint32_t size);
		static Ref<VertexBuffer> Create(const F32* vertices, uint32_t size);
	};

	class IndexBuffer : public Bindable
	{
	public:
		virtual uint32_t GetCount() const = 0;

		static Ref<IndexBuffer> Create(const uint32_t* indices, uint32_t count);
	};

}
