#pragma once

#include "Dominion/Renderer/Bindable.h"

#include <memory>

namespace Dominion {

	class VertexBuffer : public Bindable
	{
	public:
		static std::shared_ptr<VertexBuffer> Create(const float* vertices, size_t size);
	};

	class IndexBuffer : public Bindable
	{
	public:
		virtual unsigned int GetCount() const = 0;

		static std::shared_ptr<IndexBuffer> Create(const uint32_t* indices, unsigned int count);
	};

}
