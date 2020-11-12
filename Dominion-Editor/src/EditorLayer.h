#pragma once

#include "Panels/SceneHierarchyPanel.h"

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
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		OrthographicCameraController m_Camera;

		bool m_PrimaryCam = true;

		glm::vec2 m_ViewportSize;
		bool m_ViewportFocused, m_ViewportHovered;

		// Panels
		SceneHierarhyPanel m_Panel;
	};

}
