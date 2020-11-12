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

		Ref<Scene> m_ActiveScene;

		OrthographicCamera m_Camera;

		Entity m_SquareEntity;

		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused, m_ViewportHovered;
	};

}
