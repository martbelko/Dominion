#pragma once

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/SettingsPanel.h"
#include "Panels/PerformancePanel.h"

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
		// TODO: Rule of 5

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;

		void OnDuplicateEntityCommand();
	private:
		void RenderDebug(const EditorCamera& editorCamera);
		void RenderDebug(const SceneCamera& sceneCamera, const glm::mat4& transform);
		void RenderDebugInternal(bool positiveCamera);

		bool OnKeyPressed(KeyPressedEvent& e);
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);

		void NewScene();
		void OpenScene();
		void OpenScene(const std::filesystem::path& path);
		void SaveScene();
		void SaveSceneAs();

		static void SerializeScene(Ref<Scene>& scene, const std::filesystem::path& filepath);

		void OnScenePlay();
		void OnScenePause();
		void OnSceneUnpause();
		void OnSceneStop();
		void OnSceneReload();

		// UI Panels
		void DrawToolbar();
	private:
		// Temp
		Ref<VertexArray> mSquareVA;
		Ref<Shader> mFlatColorShader;
		Ref<Framebuffer> mFramebuffer;
		Ref<Framebuffer> mCameraFramebuffer;

		Ref<Scene> mActiveScene, mEditorScene;
		std::filesystem::path mEditorSceneFilepath = std::filesystem::path();

		Entity mHoveredEntity;

		bool mPrimaryCamera = true;

		EditorCamera mEditorCamera;

		Ref<Texture2D> mCheckerboardTexture;

		bool mViewportFocused = false, mViewportHovered = false;
		glm::vec2 mViewportSize = { 0.0f, 0.0f };
		glm::vec2 mViewportBounds[2] {};

		glm::vec2 mCameraViewViewportSize = { 0.0f, 0.0f };

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
		SettingsPanel mSettinsPanel;
		PerformancePanel mPerformancePanel;

		// Editor resources
		Ref<Texture2D> mIconPlay;
		Ref<Texture2D> mIconStop;
		Ref<Texture2D> mIconPause;
		Ref<Texture2D> mIconReload;

		// Commands
		CommandStack mCommandStack;
	};

}
