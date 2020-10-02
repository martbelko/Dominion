#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Bindable.h"

#include <memory>

namespace Dominion {

	class VertexBuffer : public Bindable
	{
	public:
		static Ref<VertexBuffer> Create(const float* vertices, size_t size);
	};

	class IndexBuffer : public Bindable
	{
	public:
		virtual unsigned int GetCount() const = 0;

		static Ref<IndexBuffer> Create(const uint32_t* indices, unsigned int count);
	};

}
