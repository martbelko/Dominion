#pragma once

#include "Panels/SceneHierarchyPanel.h"

#include <Dominion.h>
#include <DominionImGui.h>

#include <vector>

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
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMousePressed(MousePressedEvent& e);

		void NewScene();
		void OpenScene();
		void SaveSceneAs();
	private:
		Ref<Framebuffer> m_Framebuffer;

		Ref<Texture2D> m_Texture2D;
		Ref<Texture2D> m_TestTexture;

		EditorCamera m_Camera;

		Ref<Scene> m_ActiveScene;

		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportPosition;
		bool m_ViewportFocused, m_ViewportHovered;

		// Panels
		SceneHierarhyPanel m_Panel;

		// Scenes
		std::vector<Ref<Scene>> m_Scenes;
	};

}
