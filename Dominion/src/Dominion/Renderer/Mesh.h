#pragma once

#include "Dominion/Core/Base.h"

#include "Dominion/Renderer/Pipeline.h"
#include "Dominion/Renderer/InputLayout.h"

namespace Dominion {

	class Mesh
	{
	public:
		Mesh(const float* vertices, uint32_t size, const Ref<InputLayout>& layout);
		Mesh(const float* vertices, uint32_t size, const uint32_t* indices, uint32_t count, const Ref<InputLayout>& layout);

		const Ref<Pipeline>& GetPipeline() const { return m_Pipeline; }

		static Ref<Mesh> Create(const float* vertices, uint32_t size, const Ref<InputLayout>& layout) { return CreateRef<Mesh>(vertices, size, layout); }
		static Ref<Mesh> Create(const float* vertices, uint32_t size, const uint32_t* indices, uint32_t count, const Ref<InputLayout>& layout) { return CreateRef<Mesh>(vertices, size, indices, size, layout); }
	private:
		Ref<Pipeline> m_Pipeline;
	};

}
