#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Timestep.h"
#include "Dominion/Events/Event.h"

namespace Dominion {

	class Layer
	{
	public:
		Layer(const std::string_view& debugName = "Layer") : m_DebugName(debugName) {}
		virtual ~Layer() = default;

		// TODO: Rule of 5

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(const Timestep& timestep) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};

}
