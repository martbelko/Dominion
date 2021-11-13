#include "EditorLayer.h"

#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Dominion/Scene/Scene.h"
#include "Dominion/Scene/SceneSerializer.h"

#include "Dominion/Utils/PlatformUtils.h"

#include "ImGuizmo.h"

#include "Dominion/Math/Math.h"

namespace Dominion {

	extern const std::filesystem::path gAssetPath; // TODO: Move to project

	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), mCameraController(1280.0f / 720.0f)
	{
	}

	void EditorLayer::OnAttach()
	{
		DM_PROFILE_FUNCTION();

		mCheckerboardTexture = Texture2D::Create("assets/textures/Checkerboard.png");
		mIconPlay = Texture2D::Create("Resources/Icons/PlayButton.png");
		mIconStop = Texture2D::Create("Resources/Icons/StopButton.png");
		mIconPause = Texture2D::Create("Resources/Icons/PauseButton.png");
		mIconReload = Texture2D::Create("Resources/Icons/ReloadButton.png");

		FramebufferSpecification fbSpec;
		fbSpec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.width = 1280;
		fbSpec.height = 720;
		mFramebuffer = Framebuffer::Create(fbSpec);

		fbSpec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::Depth };
		fbSpec.width = 100;
		fbSpec.height = 100;
		mCameraFramebuffer = Framebuffer::Create(fbSpec);

		mEditorScene = CreateRef<Scene>();
		mActiveScene = mEditorScene;

		auto commandLineArgs = Application::Get().GetCommandLineArgs();
		if (commandLineArgs.count > 1)
		{
			auto sceneFilePath = commandLineArgs[1];
			SceneSerializer serializer(mActiveScene);
			serializer.Deserialize(sceneFilePath);
		}

		mEditorCamera = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
		mSceneHierarchyPanel.SetContext(mActiveScene);
		mSceneHierarchyPanel.SetCommandStack(mCommandStack);
	}

	void EditorLayer::OnDetach()
	{
		DM_PROFILE_FUNCTION();
	}

	void EditorLayer::OnUpdate(Timestep ts)
	{
		DM_PROFILE_FUNCTION();

		// Resize
		if (FramebufferSpecification spec = mFramebuffer->GetSpecification();
			mViewportSize.x > 0.0f && mViewportSize.y > 0.0f && // zero sized framebuffer is invalid
			(spec.width != mViewportSize.x || spec.height != mViewportSize.y))
		{
			mFramebuffer->Resize(static_cast<uint32_t>(mViewportSize.x), static_cast<uint32_t>(mViewportSize.y));
			mCameraController.OnResize(mViewportSize.x, mViewportSize.y);
			mEditorCamera.SetViewportSize(mViewportSize.x, mViewportSize.y);
		}

		if (FramebufferSpecification spec = mCameraFramebuffer->GetSpecification();
			mCameraViewViewportSize.x > 0.0f && mCameraViewViewportSize.y > 0.0f &&
			(spec.width != mCameraViewViewportSize.x || spec.height != mCameraViewViewportSize.y))
		{
			mCameraFramebuffer->Resize(static_cast<uint32_t>(mCameraViewViewportSize.x), static_cast<uint32_t>(mCameraViewViewportSize.y));
		}

		// Render
		Renderer2D::ResetStats();
		mFramebuffer->Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear(RenderTarget::COLOR | RenderTarget::DEPTH);

		// Clear entity ID attachment to -1
		mFramebuffer->ClearAttachment(1, -1);

		switch (mSceneState)
		{
			case SceneState::Edit:
			{
				if (mViewportFocused)
					mCameraController.OnUpdate(ts);

				mEditorCamera.OnUpdate(ts);
				mEditorScene->OnUpdateEditor(ts, mEditorCamera);

				mEditorScene->Render(mEditorCamera);
				RenderDebug(mEditorCamera);

				Entity primaryCameraEntity = mEditorScene->GetPrimaryCameraEntity();
				if (primaryCameraEntity && mCameraViewViewportSize.x > 0 && mCameraViewViewportSize.y > 0)
				{
					mCameraFramebuffer->Bind();
					RenderCommand::Clear(RenderTarget::COLOR | RenderTarget::DEPTH);

					TransformComponent& tc = primaryCameraEntity.Transform();
					CameraComponent& cc = primaryCameraEntity.GetComponent<CameraComponent>();
					cc.camera.SetViewportSize(mCameraViewViewportSize.x, mCameraViewViewportSize.y);;
					mEditorScene->Render(cc.camera, tc.GetTransform());
					if (mSettinsPanel.ShowPhysicsColliders())
					{
						mEditorScene->Render(cc.camera, tc.GetTransform());
						RenderDebug(cc.camera, tc.GetTransform());
					}

					mFramebuffer->Bind();
				}

				break;
			}
			case SceneState::Play:
			{
				mActiveScene->OnUpdateRuntime(ts);
				Entity primaryCameraEntity = mActiveScene->GetPrimaryCameraEntity();
				DM_CORE_ASSERT(primaryCameraEntity, "No primary camera has been found");
				mActiveScene->Render(primaryCameraEntity.GetComponent<CameraComponent>().camera, primaryCameraEntity.Transform().GetTransform());
				if (mSettinsPanel.ShowPhysicsColliders())
				{
					Entity primaryCameraEntity = mActiveScene->GetPrimaryCameraEntity();
					if (primaryCameraEntity)
					{
						const SceneCamera& sceneCamera = primaryCameraEntity.GetComponent<CameraComponent>().camera;
						glm::mat4 transform = primaryCameraEntity.GetComponent<TransformComponent>().GetTransform();
						RenderDebug(sceneCamera, transform);
					}
				}

				break;
			}
			case SceneState::Pause:
			{
				Entity primaryCameraEntity = mActiveScene->GetPrimaryCameraEntity();
				DM_CORE_ASSERT(primaryCameraEntity, "No primary camera has been found");

				const SceneCamera& sceneCamera = primaryCameraEntity.GetComponent<CameraComponent>().camera;
				glm::mat4 cameraTransform = primaryCameraEntity.Transform().GetTransform();
				mActiveScene->Render(sceneCamera, cameraTransform);
				if (mSettinsPanel.ShowPhysicsColliders())
					RenderDebug(sceneCamera, cameraTransform);

				break;
			}
		}

		auto [mx, my] = ImGui::GetMousePos();
		mx -= mViewportBounds[0].x;
		my -= mViewportBounds[0].y;
		glm::vec2 viewportSize = mViewportBounds[1] - mViewportBounds[0];
		my = viewportSize.y - my;
		int mouseX = (int)mx;
		int mouseY = (int)my;

		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y)
		{
			int pixelData = mFramebuffer->ReadPixel(1, mouseX, mouseY);
			mHoveredEntity = pixelData == -1 ? Entity() : Entity((entt::entity)pixelData, mActiveScene.get());
		}

		mFramebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		DM_PROFILE_FUNCTION();

		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				// ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);
				if (ImGui::MenuItem("New", "Ctrl+N"))
					NewScene();

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
					OpenScene();

				{
					bool canBeSimpleSaved = !mEditorSceneFilepath.empty();
					if (!canBeSimpleSaved)
					{
						ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
						ImGui::BeginDisabled();
					}

					if (ImGui::MenuItem("Save", "Ctrl+S"))
					{
						DM_ASSERT(!mEditorSceneFilepath.empty(), "Editor scene filepath was empty");
						SaveScene();
					}

					if (!canBeSimpleSaved)
					{
						ImGui::EndDisabled();
						ImGui::PopStyleVar();
					}
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					SaveSceneAs();

				ImGui::Separator();
				if (ImGui::MenuItem("Exit")) Application::Get().Close();
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		// Render all panels
		mSceneHierarchyPanel.OnImGuiRender();
		mContentBrowserPanel.OnImGuiRender();
		mPerformancePanel.OnImGuiRender();
		mSettinsPanel.OnImGuiRender();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();
		mViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		mViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		mViewportFocused = ImGui::IsWindowFocused();
		mViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		mViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		uint64_t textureID = mFramebuffer->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2(mViewportSize.x, mViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* path = (const wchar_t*)payload->Data;
				OpenScene(std::filesystem::path(gAssetPath) / path);
			}
			ImGui::EndDragDropTarget();
		}

		if (ImGui::Begin("Primary Camera Preview"))
		{
			Entity primaryCameraEntity = mEditorScene->GetPrimaryCameraEntity();
			if (primaryCameraEntity)
			{
				std::string title = "Primary Camera Entity Name: " + primaryCameraEntity.Name();
				ImGui::Text(title.c_str());

				ImVec2 cameraViewWindowSize = ImGui::GetContentRegionAvail();
				mCameraViewViewportSize = { cameraViewWindowSize.x, cameraViewWindowSize.y };
				mCameraFramebuffer->Bind();
				uint64_t cameraTextureID = mCameraFramebuffer->GetColorAttachmentRendererID();
				ImGui::Image(reinterpret_cast<void*>(cameraTextureID), ImVec2{ mCameraViewViewportSize.x, mCameraViewViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				mCameraFramebuffer->Unbind();
			}
			else
			{
				auto TextCentered = [&](const std::string& text) -> void
				{
					auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
					auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
					auto viewportOffset = ImGui::GetWindowPos();
					glm::vec2 bounds[2];
					bounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
					bounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
					glm::vec2 windowSize = { viewportMaxRegion.x - viewportMinRegion.x, viewportMaxRegion.y - viewportMinRegion.y };

					auto textSize = ImGui::CalcTextSize(text.c_str());
					auto x = (windowSize.x - textSize.x) * 0.5f;
					auto y = (windowSize.y - textSize.y) * 0.5f;

					ImGui::SetCursorScreenPos({ bounds[0].x + x, bounds[0].y + y });
					ImGui::Text(text.c_str());
				};

				TextCentered("No Primary Camera Found");
			}
		}
		ImGui::End();

		// Gizmos
		Entity selectedEntity = mSceneHierarchyPanel.GetSelectedEntity();
		if (selectedEntity && mGizmoType != -1)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(mViewportBounds[0].x, mViewportBounds[0].y, mViewportBounds[1].x - mViewportBounds[0].x, mViewportBounds[1].y - mViewportBounds[0].y);

			// Camera

			// Runtime camera from entity
			// auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
			// const auto& camera = cameraEntity.GetComponent<CameraComponent>().Camera;
			// const glm::mat4& cameraProjection = camera.GetProjection();
			// glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());

			// Editor camera
			const glm::mat4& cameraProjection = mEditorCamera.GetProjection();
			glm::mat4 cameraView = mEditorCamera.GetViewMatrix();

			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();

			// Snapping
			bool snap = Input::IsKeyPressed(Key::LeftControl);
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (mGizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;

			float snapValues[3] = { snapValue, snapValue, snapValue };

			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)mGizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);

			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Math::DecomposeTransform(transform, translation, rotation, scale);

				glm::vec3 deltaRotation = rotation - tc.rotation;
				tc.translation = translation;
				tc.rotation += deltaRotation;
				tc.scale = scale;
			}
		}

		ImGui::End();
		ImGui::PopStyleVar();

		UI_Toolbar();

		ImGui::End();
	}

	void EditorLayer::UI_Toolbar()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		auto& colors = ImGui::GetStyle().Colors;
		const auto& buttonHovered = colors[ImGuiCol_ButtonHovered];
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(buttonHovered.x, buttonHovered.y, buttonHovered.z, 0.5f));
		const auto& buttonActive = colors[ImGuiCol_ButtonActive];
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(buttonActive.x, buttonActive.y, buttonActive.z, 0.5f));

		ImGui::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		float size = ImGui::GetWindowHeight() - 4.0f;
		if (mSceneState == SceneState::Edit)
		{
			constexpr float iconCount = 1.0f;
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (iconCount * size * 0.5f));

			if (ImGui::ImageButton((ImTextureID)mIconPlay->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0))
			{
				OnScenePlay();
			}
		}
		else if (mSceneState == SceneState::Play)
		{
			constexpr float iconCount = 3.0f;
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (iconCount * size * 0.5f));

			if (ImGui::ImageButton((ImTextureID)mIconPause->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0))
			{
				OnScenePause();
			}

			ImGui::SameLine();

			if (ImGui::ImageButton((ImTextureID)mIconStop->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0))
			{
				OnSceneStop();
			}

			ImGui::SameLine();

			if (ImGui::ImageButton((ImTextureID)mIconReload->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0))
			{
				OnSceneReload();
			}
		}
		else if (mSceneState == SceneState::Pause)
		{
			constexpr float iconCount = 3.0f;
			ImGui::SetCursorPosX((ImGui::GetWindowContentRegionMax().x * 0.5f) - (iconCount * size * 0.5f));

			if (ImGui::ImageButton((ImTextureID)mIconPlay->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0))
			{
				OnSceneUnpause();
			}

			ImGui::SameLine();

			if (ImGui::ImageButton((ImTextureID)mIconStop->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0))
			{
				OnSceneStop();
			}

			ImGui::SameLine();

			if (ImGui::ImageButton((ImTextureID)mIconReload->GetRendererID(), ImVec2(size, size), ImVec2(0, 1), ImVec2(1, 0), 0))
			{
				OnSceneReload();
			}
		}

		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(3);
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		if (mViewportFocused || mViewportHovered)
		{
			mCameraController.OnEvent(e);
			mEditorCamera.OnEvent(e);
			dispatcher.Dispatch<MouseButtonPressedEvent>(DM_BIND_EVENT_FN(EditorLayer::OnMouseButtonPressed));
		}

		dispatcher.Dispatch<KeyPressedEvent>(DM_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	void EditorLayer::OnDuplicateEntityCommand()
	{
		if (mSceneState == SceneState::Edit)
		{
			Entity selectedEntity = mSceneHierarchyPanel.GetSelectedEntity();
			if (selectedEntity)
			{
				Command* command = new DuplicateEntityCommand(selectedEntity);
				command->Do();
				mCommandStack.PushCommand(command);
			}
		}
	}

	void EditorLayer::RenderDebug(const EditorCamera& editorCamera)
	{
		Renderer2D::BeginScene(mEditorCamera);
		RenderDebugInternal(editorCamera.GetDistance() > 0.0f);
	}

	void EditorLayer::RenderDebug(const SceneCamera& sceneCamera, const glm::mat4& transform)
	{
		Renderer2D::BeginScene(sceneCamera, transform);
		float cameraDistance;
		if (sceneCamera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			RenderDebugInternal(25.0f);
		else
		{
			glm::vec3 translation, rotation, scale;
			Math::DecomposeTransform(transform, translation, rotation, scale);
			RenderDebugInternal(translation.z > 0.0f);
		}
	}

	void EditorLayer::RenderDebugInternal(bool positiveCamera)
	{
		Renderer2D::SetLineWidth(mSettinsPanel.GetDebugLineThickness());
		constexpr float zOffset = 0.001f;
		const glm::mat4& positiveOffset = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zOffset));
		const glm::mat4& negativeOffset = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -zOffset));
		// Draw box2d colliders
		{
			auto view = mActiveScene->GetAllEntitiesWith<TransformComponent, BoxCollider2DComponent>();
			for (auto eid : view)
			{
				auto [transform, boxCollider2d] = view.get<TransformComponent, BoxCollider2DComponent>(eid);
				glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(glm::vec2(transform.translation), 0.0f)) *
					glm::toMat4(glm::quat(glm::vec3(0.0f, 0.0f, transform.rotation.z))) *
					glm::scale(glm::mat4(1.0f), glm::vec3(glm::vec2(transform.scale), 1.0f));
				glm::mat4 finalMatrix = transformMatrix *
					glm::translate(glm::mat4(1.0f), glm::vec3(boxCollider2d.offset, 0.0f)) *
					glm::scale(glm::mat4(1.0f), glm::vec3(2.0f * boxCollider2d.size.x, 2.0f * boxCollider2d.size.y, 1.0f));

				Renderer2D::DrawRect((positiveCamera ? positiveOffset : negativeOffset) * finalMatrix,
					mSettinsPanel.Get2DPhysicsCollidersColor(), static_cast<int>(eid));
			}
		}

		// Draw circle colliders
		{
			auto view = mActiveScene->GetAllEntitiesWith<TransformComponent, CircleCollider2DComponent>();
			for (auto eid : view)
			{
				auto [transform, circleCollider2d] = view.get<TransformComponent, CircleCollider2DComponent>(eid);
				glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(glm::vec2(transform.translation), 0.0f)) *
					glm::toMat4(glm::quat(glm::vec3(0.0f, 0.0f, transform.rotation.z)));
				glm::mat4 finalMatrix = transformMatrix *
					glm::translate(glm::mat4(1.0f), glm::vec3(circleCollider2d.offset, 0.0f)) *
					glm::scale(glm::mat4(1.0f), glm::vec3(2.0f * circleCollider2d.radius, 2.0f * circleCollider2d.radius, 1.0f));

				Renderer2D::DrawCircle((positiveCamera ? positiveOffset : negativeOffset) * finalMatrix,
					mSettinsPanel.Get2DPhysicsCollidersColor(), 0.0125f * mSettinsPanel.GetDebugLineThickness(), 0.0f, (int)eid);
			}
		}

		Renderer2D::EndScene();
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		// Shortcuts
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);

		switch (e.GetKeyCode())
		{
			case Key::Z:
			{
				if (control)
				{
					Command* command = mCommandStack.GetLastCommand();
					Entity deletedEntity;
					if (command->GetCommandType() == CommandType::AddEntity)
					{
						AddEntityCommand* adc = static_cast<AddEntityCommand*>(command);
						deletedEntity = adc->GetCreatedEntity();
					}
					else if (command->GetCommandType() == CommandType::DuplicateEntity)
					{
						DuplicateEntityCommand* dec = static_cast<DuplicateEntityCommand*>(command);
						deletedEntity = dec->GetCreatedEntity();
					}

					if (deletedEntity && mSceneHierarchyPanel.GetSelectedEntity() == deletedEntity)
						mSceneHierarchyPanel.SetSelectedEntity(Entity());

					command->Undo();
				}

				break;
			}
			case Key::Y:
			{
				if (control)
				{
					Command* command = mCommandStack.GetNextCommand();
					command->Do();
				}

				break;
			}
			case Key::D:
			{
				if (control)
					OnDuplicateEntityCommand();

				break;
			}
			case Key::N:
			{
				if (control)
					NewScene();

				break;
			}
			case Key::O:
			{
				if (control)
					OpenScene();

				break;
			}
			case Key::S:
			{
				if (control)
				{
					if (shift)
						SaveSceneAs();
					else
						SaveScene();
				}

				break;
			}
		}

		if (mViewportFocused || mViewportHovered)
		{
			switch (e.GetKeyCode())
			{
				// Gizmos
				case Key::Q:
				{
					if (!ImGuizmo::IsUsing())
						mGizmoType = -1;
					break;
				}
				case Key::U:
				{
					if (!ImGuizmo::IsUsing())
						mGizmoType = ImGuizmo::OPERATION::UNIVERSAL;
					break;
				}
				case Key::W:
				{
					if (!ImGuizmo::IsUsing())
						mGizmoType = ImGuizmo::OPERATION::TRANSLATE;
					break;
				}
				case Key::E:
				{
					if (!ImGuizmo::IsUsing())
						mGizmoType = ImGuizmo::OPERATION::ROTATE;
					break;
				}
				case Key::R:
				{
					if (!ImGuizmo::IsUsing())
						mGizmoType = ImGuizmo::OPERATION::SCALE;
					break;
				}
			}
		}

		return false;
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			if (mViewportHovered && !ImGuizmo::IsOver() && !Input::IsKeyPressed(Key::LeftAlt))
				mSceneHierarchyPanel.SetSelectedEntity(mHoveredEntity);
		}
		return false;
	}

	void EditorLayer::NewScene()
	{
		mEditorScene = CreateRef<Scene>();
		mEditorScene->OnViewportResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
		mSceneHierarchyPanel.SetContext(mEditorScene);

		mActiveScene = mEditorScene;
		mEditorSceneFilepath = std::filesystem::path();
		mCommandStack = CommandStack();
	}

	void EditorLayer::OpenScene()
	{
		std::filesystem::path filepath = FileDialogs::OpenFile("Dominion Scene (*.dscene)\0*.dscene\0");
		if (!filepath.empty())
			OpenScene(filepath.string());
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (path.extension().string() != ".dscene")
		{
			DM_WARN("Could not load {0} - not a scene file", path.filename().string());
			return;
		}

		if (mSceneState != SceneState::Edit)
			OnSceneStop();

		Ref<Scene> newScene = CreateRef<Scene>();
		SceneSerializer serializer(newScene);
		if (serializer.Deserialize(path.string()))
		{
			mEditorScene = newScene;
			mEditorSceneFilepath = path;
			mEditorScene->OnViewportResize((uint32_t)mViewportSize.x, (uint32_t)mViewportSize.y);
			mSceneHierarchyPanel.SetContext(mEditorScene);

			mActiveScene = mEditorScene;
			mCommandStack = CommandStack();
		}
	}

	void EditorLayer::SaveScene()
	{
		if (mEditorSceneFilepath.empty())
			SaveSceneAs();
		else
			SerializeScene(mEditorScene, mEditorSceneFilepath);
	}

	void EditorLayer::SaveSceneAs()
	{
		std::filesystem::path filepath = FileDialogs::SaveFile("Dominion Scene (*.dscene)\0*.dscene\0");
		if (!filepath.empty())
		{
			SerializeScene(mEditorScene, filepath);
			mEditorSceneFilepath = filepath;
		}
	}

	void EditorLayer::SerializeScene(Ref<Scene>& scene, const std::filesystem::path& filepath)
	{
		SceneSerializer serializer(scene);
		serializer.Serialize(filepath.string());
	}

	void EditorLayer::OnScenePlay()
	{
		mActiveScene = CreateRef<Scene>(*mEditorScene);
		const FramebufferSpecification& spec = mFramebuffer->GetSpecification();
		mActiveScene->OnViewportResize(spec.width, spec.height);
		mActiveScene->OnRuntimeStart();
		mSceneState = SceneState::Play;

		mSceneHierarchyPanel.SetContext(mActiveScene);
	}

	void EditorLayer::OnScenePause()
	{
		mSceneState = SceneState::Pause;
	}

	void EditorLayer::OnSceneUnpause()
	{
		mSceneState = SceneState::Play;
	}

	void EditorLayer::OnSceneStop()
	{
		mActiveScene->OnRuntimeStop();
		mActiveScene = mEditorScene;
		mSceneState = SceneState::Edit;

		mSceneHierarchyPanel.SetContext(mEditorScene);
	}

	void EditorLayer::OnSceneReload()
	{
		OnSceneStop();
		OnScenePlay();
	}

}
