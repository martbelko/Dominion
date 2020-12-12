#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace Dominion {

	static bool DoesIntersectTriangle(const glm::vec3& rayOrigin, const glm::vec3& rayVector, const glm::vec3& vertex0, const glm::vec3& vertex1, const glm::vec3& vertex2)
	{
		constexpr float epsilon = 0.0000001f;
		glm::vec3 edge1, edge2, h, s, q;
		float a, f, u, v;
		edge1 = vertex1 - vertex0;
		edge2 = vertex2 - vertex0;
		h = glm::cross(edge2, rayVector);
		a = glm::dot(edge1, h);
		if (a > -epsilon && a < epsilon)
			return false;

		f = 1.0f / a;
		s = rayOrigin - vertex0;
		u = f * glm::dot(s, h);
		if (u < 0.0f || u > 1.0f)
			return false;

		q = glm::cross(edge1, s);
		v = f * glm::dot(rayVector, q);
		if (v < 0.0f || u + v > 1.0f)
			return false;
		float t = f * glm::dot(edge2, q);
		if (t > epsilon)
			return true;
		else
			return false;
	}

	static bool DoesIntersect(const glm::vec3& rayOrigin, const glm::vec3& rayVector, const glm::vec3& quadPos, const glm::vec2& quadSize)
	{
		float left = quadPos.x - quadSize.x / 2.0f;
		float top = quadPos.y + quadSize.y / 2.0f;
		float right = quadPos.x + quadSize.x / 2.0f;
		float bottom = quadPos.y - quadSize.y / 2.0f;

		glm::vec3 vertex0 = glm::vec3(left, bottom, quadPos.z);
		glm::vec3 vertex1 = glm::vec3(right, top, quadPos.z);
		glm::vec3 vertex2 = glm::vec3(left, top, quadPos.z);
		glm::vec3 vertex3 = glm::vec3(right, bottom, quadPos.z);

		return DoesIntersectTriangle(rayOrigin, rayVector, vertex0, vertex1, vertex2)
			|| DoesIntersectTriangle(rayOrigin, rayVector, vertex0, vertex3, vertex1);
	}

	EditorLayer::EditorLayer()
		: Layer("Sandbox2D")
	{
	}

	void EditorLayer::OnAttach()
	{
		/* Setup camera */
		float wHeight = static_cast<float>(Application::Get().GetWindow().GetHeight());
		float wWidth = static_cast<float>(Application::Get().GetWindow().GetWidth());
		float ratio = wWidth / wHeight;
		m_Camera = EditorCamera(45.0f, 1.778f, 0.01f, 1000.0f);

		FramebufferDesc desc;
		desc.Width = Application::Get().GetWindow().GetWidth();
		desc.Height = Application::Get().GetWindow().GetHeight();
		m_Framebuffer = Framebuffer::Create(desc);

		m_ActiveScene = CreateRef<Scene>();
		m_Panel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OnUpdate(const Timestep& ts)
	{
		DM_PROFILE_FUNCTION();

		// Resize
		if (const FramebufferDesc& desc = m_Framebuffer->GetDesc();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(desc.Width != m_ViewportSize.x || desc.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
			m_Camera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		}

		// Update
		m_Camera.OnUpdate(ts);

		// Render
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		RenderCommand::ClearColorBuffer();
		RenderCommand::ClearDepthBuffer();

		// Update scene
		m_ActiveScene->OnUpdateEditor(ts, m_Camera);

		m_Framebuffer->Unbind();
	}

	void EditorLayer::OnImGuiRender()
	{
		DM_PROFILE_FUNCTION();

		static bool dockSpaceOpen = true;
		static bool opt_fullscreen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		if (ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags))
		{
			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			ImGuiStyle& style = ImGui::GetStyle();

			float defaultWindowSizeX = style.WindowMinSize.x;
			style.WindowMinSize.x = 370.0f;
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
			}
			style.WindowMinSize.x = defaultWindowSizeX;

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New", "Ctrl+N"))
					{
						NewScene();
					}

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						OpenScene();
					}

					if (ImGui::MenuItem("Save As...", "Ctrl+S"))
					{
						SaveSceneAs();
					}

					if (ImGui::MenuItem("Exit"))
						Application::Get().Close();
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Scenes"))
				{
					for (int i = 0; i < m_Scenes.size(); ++i)
					{
						Ref<Scene> scene = m_Scenes[i];
						if (ImGui::MenuItem(scene->GetName().c_str()))
						{
							m_ActiveScene = scene;
							m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
						}
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			m_Panel.OnImGuiRender();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			if (ImGui::Begin("Viewport"))
			{
				ImGuiDockNode* node = ImGui::GetWindowDockNode();
				ImVec2 tabSize{ 0.0f, 0.0f };
				if (!node->IsHiddenTabBar())
					tabSize = ImGui::TabItemCalcSize("Viewport", true);

				ImVec2 imPos = ImGui::GetWindowPos();
				m_ViewportPosition = glm::vec2(imPos.x, imPos.y + tabSize.y);

				m_ViewportFocused = ImGui::IsWindowFocused();
				m_ViewportHovered = ImGui::IsWindowHovered();
				Application::Get().GetImGuiLayer()->BlockEvents(!(m_ViewportFocused && m_ViewportHovered));

				ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
				m_ViewportSize = { viewportPanelSize.x - 1.0f, viewportPanelSize.y - 1.0f };

				ImGui::Image(reinterpret_cast<void*>(static_cast<uint64_t>(m_Framebuffer->GetColorAttachmentRendererID())), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
			}

			ImGui::End();
			ImGui::PopStyleVar();

		}

		ImGui::End();
		ImGui::PopStyleVar();

		Renderer2D::ResetStats();
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_Camera.OnEvent(e);

		e.Dispatch<KeyPressedEvent>(DM_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
		e.Dispatch<MousePressedEvent>(DM_BIND_EVENT_FN(EditorLayer::OnMousePressed));
	}


	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetRepeatCount() > 0)
			return false;

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);
		switch (e.GetKeyCode())
		{
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
				if (control && shift)
					SaveSceneAs();

				break;
			}
		}

		return false;
	}

	bool EditorLayer::OnMousePressed(MousePressedEvent& e)
	{
		const Window& wnd = Application::Get().GetWindow();

		auto [mouseX, mouseY] = Input::GetMousePosition();
		glm::vec2 screenMousePos = glm::vec2(mouseX + wnd.GetPosX(), mouseY + wnd.GetPosY());
		glm::vec2 viewportMousePos = screenMousePos - m_ViewportPosition;

		glm::vec2 viewportHalf = m_ViewportSize / 2.0f;

		mouseX = (viewportMousePos.x - viewportHalf.x) / viewportHalf.x;
		mouseY = (viewportMousePos.y - viewportHalf.y) / -viewportHalf.y;

		glm::vec4 clipCoords{ mouseX, mouseY, 1.0f, 1.0f };

		glm::vec3 worldCoords = glm::inverse(m_Camera.GetViewProjectionMatrix()) * clipCoords;
		worldCoords = glm::normalize(worldCoords);

		DM_CORE_INFO("Mouse Ray: {0}, {1}, {2}", worldCoords.x, worldCoords.y, worldCoords.z);

		Entity selectedEntity;
		const glm::vec3& cameraPos = m_Camera.GetPosition();
		const glm::vec3& ray = worldCoords;
		auto view = m_ActiveScene->m_Registry.view<TransformComponent>();
		for (entt::entity entityID : view)
		{
			Entity entity(entityID, m_ActiveScene.get());
			const TransformComponent& ts = entity.GetComponent<TransformComponent>();
			if (DoesIntersect(cameraPos, ray, ts.Position, ts.Scale))
			{
				selectedEntity = entity;
				break;
			}
		}

		m_Panel.SetSelectedEntity(selectedEntity);

		return false;
	}

	void EditorLayer::NewScene()
	{
		Ref<Scene> scene = CreateRef<Scene>();
		m_Scenes.push_back(scene);
		m_ActiveScene = scene;
		m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		m_Panel.SetContext(m_ActiveScene);
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Dominion Scene (*.dominion)\0*.dominion\0");
		if (!filepath.empty())
		{
			Ref<Scene> scene = CreateRef<Scene>();
			m_Scenes.push_back(scene);
			m_ActiveScene = scene;
			m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
			m_Panel.SetContext(m_ActiveScene);

			SceneSerializer serializer(m_ActiveScene);
			serializer.Deserialize(filepath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Dominion Scene (*.dominion)\0*.dominion\0");
		if (!filepath.empty())
		{
			SceneSerializer serializer(m_ActiveScene);
			serializer.Serialize(filepath);
		}
	}

}
