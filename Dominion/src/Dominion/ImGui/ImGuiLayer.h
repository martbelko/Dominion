#pragma once

#if DM_INCLUDE_IMGUI == 1

#include "Dominion/Core/Base.h"
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

		void SetDarkThemeColors();
	private:
		bool m_BlockEvents = true;
		F32 m_Time = 0.0f;
	};

}

#endif
