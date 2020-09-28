#pragma once

#include "Dominion/Core/Layer.h"

namespace Dominion {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		virtual ~ImGuiLayer() override = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event& e) override;

		void Begin();
		void End();

		void BlockEvents(bool block);
	private:
		bool m_BlockEvents = true;
		float m_Time = 0.0f;
	};

}
