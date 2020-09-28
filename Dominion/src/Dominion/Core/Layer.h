#pragma once

#include "Dominion/Events/Event.h"

namespace Dominion {

	class Layer
	{
	public:
		Layer(const std::string_view& debugName = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};

}