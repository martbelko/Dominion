#pragma once

#include "Dominion/Core/Layer.h"

#include "Dominion/Events/ApplicationEvent.h"
#include "Dominion/Events/KeyEvent.h"
#include "Dominion/Events/MouseEvent.h"

namespace Dominion {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block) { mBlockEvents = block; }

		void SetDarkThemeColors();
	private:
		bool mBlockEvents = false;
	};

}
