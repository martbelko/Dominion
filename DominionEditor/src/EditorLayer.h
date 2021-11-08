#pragma once

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "CommandStack.h"

#include "Dominion.h"
#include "Dominion/Renderer/EditorCamera.h"

#include <vector>

namespace Dominion {

	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() override = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& e) override;

		void OnDuplicateEntityCommand();
	private:
		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		static void SerializeScene(Ref<Scene>& scene, const std::filesystem::path& filepath);

		void OnScenePlay();
		void OnSceneStop();

		// UI Panels
		void UI_Toolbar();
	private:
		OrthographicCameraController mCameraController;

		// Temp
		Ref<VertexArray> mSquareVA;
		Ref<Shader> mFlatColorShader;
		Ref<Framebuffer> mFramebuffer;

		Ref<Scene> mActiveScene, mEditorScene;
		std::filesystem::path mEditorSceneFilepath = std::filesystem::path();

		Entity mSquareEntity;
		Entity mCameraEntity;
		Entity mSecondCamera;

		Entity mHoveredEntity;

		bool mPrimaryCamera = true;

		EditorCamera mEditorCamera;

		Ref<Texture2D> mCheckerboardTexture;

		bool mViewportFocused = false, mViewportHovered = false;
		glm::vec2 mViewportSize = { 0.0f, 0.0f };
		glm::vec2 mViewportBounds[2];

		glm::vec4 mSquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		int mGizmoType = -1;

		enum class SceneState
		{
			Edit = 0,
			Play,
			Pause
		};
		SceneState mSceneState = SceneState::Edit;

		// Panels
		SceneHierarchyPanel mSceneHierarchyPanel;
		ContentBrowserPanel mContentBrowserPanel;

		// Editor resources
		Ref<Texture2D> mIconPlay, mIconStop, mIconPause;

		// Commands
		CommandStack mCommandStack;
	};

}
