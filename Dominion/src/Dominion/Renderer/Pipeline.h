#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Bindable.h"
#include "Dominion/Renderer/Buffer.h"
#include "Dominion/Renderer/InputLayout.h"

#include <memory>

namespace Dominion {

	class Pipeline : public Bindable
	{
	public:
		static Ref<Pipeline> Create(const Ref<VertexBuffer>& vertexBuffer, const Ref<InputLayout>& inputLayout);
		static Ref<Pipeline> Create(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer, const Ref<InputLayout>& inputLayout);

		virtual Ref<VertexBuffer> GetVertexBuffer() const = 0;
		virtual Ref<IndexBuffer> GetIndexBuffer() const = 0;
		virtual Ref<InputLayout> GetInputLayout() const = 0;
	};

}
