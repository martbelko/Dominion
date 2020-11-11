#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Dominion {

	EditorLayer::EditorLayer()
		: Layer("Sandbox2D")
	{
	}

	void EditorLayer::OnAttach()
	{
		m_Texture2D = Texture2D::Create("assets/Textures/TestTexture.jpg");
		m_TestTexture = Texture2D::Create("assets/Textures/unnamed.png");

		/* Setup camera */
		float wHeight = static_cast<float>(Application::Get().GetWindow().GetHeight());
		float wWidth = static_cast<float>(Application::Get().GetWindow().GetWidth());
		float ratio = wWidth / wHeight;
		m_Camera = OrthographicCamera(ratio, 1.0f);

		FramebufferDesc desc;
		desc.Width = Application::Get().GetWindow().GetWidth();
		desc.Height = Application::Get().GetWindow().GetHeight();
		m_Framebuffer = Framebuffer::Create(desc);
	}

	void EditorLayer::OnUpdate(const Timestep& timestep)
	{
		DM_PROFILE_FUNCTION();

		static int sign = -1;
		m_Rotation += timestep * sign * m_RotationSpeed;
		m_Rotation = std::clamp(m_Rotation, -180.0f, 180.0f);
		if (m_Rotation == -180.0f)
			sign = 1;
		else if (m_Rotation == 180.0f)
			sign = -1;

		// Update
		if (m_ViewportFocused)
		{
			m_Camera.OnUpdate(timestep);
			m_Camera.Refresh();
		}

		// Render
		{
			DM_PROFILE_SCOPE("Render Prep");
			m_Framebuffer->Bind();
			RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			RenderCommand::Clear();
		}

		{
			DM_PROFILE_SCOPE("Render Draw");
			Renderer2D::BeginScene(m_Camera);

			float cap = m_Count / 2.0f;
			for (float y = -cap; y < cap; y += 0.5f)
			{
				for (float x = -cap; x < cap; x += 0.5f)
				{
					glm::vec4 color = { (x + cap) / static_cast<float>(m_Count),
						0.3f,
						(y + cap) / static_cast<float>(m_Count),
						1.0f };
					Renderer2D::DrawRotatedQuad({ x, y }, { 0.45f, 0.45f }, glm::radians(m_Rotation), color);
				}
			}

			Renderer2D::DrawQuad(glm::mat4(1.0f), m_Texture2D);
			Renderer2D::DrawQuad(glm::mat4(1.0f), m_TestTexture, 5.0f);

			Renderer2D::EndScene();

			m_Framebuffer->Unbind();
		}
	}

	void EditorLayer::OnEvent(Event& e)
	{
		DM_PROFILE_FUNCTION();
		m_Camera.OnEvent(e);
	}

	void EditorLayer::OnImGuiRender()
	{
		DM_PROFILE_FUNCTION();

		static bool dockingEnabled = true;
		if (dockingEnabled)
		{
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
			else
			{
				dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
			}

			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			if (ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags))
			{
				if (opt_fullscreen)
					ImGui::PopStyleVar(2);

				ImGuiIO& io = ImGui::GetIO();
				if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
				{
					ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
					ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
				}

				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("File"))
					{
						if (ImGui::MenuItem("Exit"))
							Application::Get().Close();
						ImGui::EndMenu();
					}

					ImGui::EndMenuBar();
				}

				if (ImGui::Begin("Camera Control"))
				{
					glm::vec3& pos = m_Camera.GetPosition();
					ImGui::DragFloat("X", &pos.x, 0.01f, -5.0f, 5.0f);
					ImGui::DragFloat("Y", &pos.y, 0.01f, -5.0f, 5.0f);
					ImGui::DragFloat("Z", &pos.z, 0.01f, -5.0f, 5.0f);

					if (ImGui::Button("Reset Position"))
						pos = glm::vec3(0.0f, 0.0f, 0.0f);

					ImGui::SliderFloat("Rotation speed", &m_RotationSpeed, 0.0f, 500.0f);
					ImGui::SliderInt("Number of Quads", &m_Count, 0, 100);
				}

				ImGui::End();

				if (ImGui::Begin("Renderer2D Stats"))
				{
					const auto& stats = Renderer2D::GetStats();
					ImGui::Text("Draw calls: %d", stats.DrawCalls);
					ImGui::Text("Quads: %d", stats.QuadCount);
					ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
					ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
				}

				ImGui::End();

				ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
				if (ImGui::Begin("Viewport"))
				{
					m_ViewportFocused = ImGui::IsWindowFocused();
					m_ViewportHovered = ImGui::IsWindowHovered();
					Application::Get().GetImGuiLayer()->BlockEvents(!(m_ViewportFocused && m_ViewportHovered));

					ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
					if (m_ViewportSize != *reinterpret_cast<glm::vec2*>(&viewportPanelSize))
					{
						m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
						m_Framebuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
						m_Camera.Resize(m_ViewportSize.x, m_ViewportSize.y);
					}

					ImGui::Image((void*)m_Framebuffer->GetColorAttachmentRendererID(), ImVec2(m_ViewportSize.x, m_ViewportSize.y), ImVec2(0, 1), ImVec2(1, 0));
				}

				ImGui::End();
				ImGui::PopStyleVar();
			}

			ImGui::End();
		}

		Renderer2D::ResetStats();
	}

}
