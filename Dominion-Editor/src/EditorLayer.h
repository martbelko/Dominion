#pragma once

#include <Dominion.h>

namespace Dominion {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual void OnAttach() override;
		virtual void OnUpdate(const Timestep& timestep) override;
		virtual void OnEvent(Event& e) override;
		virtual void OnImGuiRender() override;
	private:
		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_Texture2D;
		Ref<Texture2D> m_TestTexture;

		OrthographicCamera m_Camera;

		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused, m_ViewportHovered;

		float m_Rotation = 0.0f;
		float m_RotationSpeed = 40.0f;

		int m_Count = 10;
	};

}
