#pragma once

#include "Dominion/Renderer/Bindable.h"
#include "Dominion/Renderer/Buffer.h"
#include "Dominion/Renderer/InputLayout.h"

#include <memory>

namespace Dominion {

	class Pipeline : public Bindable
	{
	public:
		static std::shared_ptr<Pipeline> Create(std::shared_ptr<VertexBuffer>& vertexBuffer, std::shared_ptr<IndexBuffer>& indexBuffer, std::shared_ptr<InputLayout>& inputLayout);

		virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const = 0;
	};

}
