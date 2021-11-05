#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Timestep.h"
#include "Dominion/Events/Event.h"

namespace Dominion {

	class Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return mDebugName; }
	protected:
		std::string mDebugName;
	};

}
