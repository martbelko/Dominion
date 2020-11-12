#include "EditorLayer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		m_Camera = OrthographicCameraController(ratio, 1.0f);

		FramebufferDesc desc;
		desc.Width = Application::Get().GetWindow().GetWidth();
		desc.Height = Application::Get().GetWindow().GetHeight();
		m_Framebuffer = Framebuffer::Create(desc);

		m_ActiveScene = CreateRef<Scene>();

		m_SquareEntity = m_ActiveScene->CreateEntity("Square");
		m_SquareEntity.AddComponent<SpriteRendererComponent>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

		m_CameraEntity = m_ActiveScene->CreateEntity("Camera");
		auto& cc = m_CameraEntity.AddComponent<CameraComponent>();
		cc.Primary = true;

		m_SecondCamera = m_ActiveScene->CreateEntity("Second Camera");
		cc = m_SecondCamera.AddComponent<CameraComponent>();

		class CameraController : public ScriptableEntity
		{
		public:
			void OnCreate()
			{

			}

			void OnDestroy()
			{

			}

			void OnUpdate(Timestep ts)
			{
				auto& trans = GetComponent<TransformComponent>().Transform;
				float speed = 5.0f;

				if (Input::IsKeyPressed(Key::A))
					trans[3][0] -= speed * ts;
				else if (Input::IsKeyPressed(Key::D))
					trans[3][0] += speed * ts;
				if (Input::IsKeyPressed(Key::W))
					trans[3][1] += speed * ts;
				else if (Input::IsKeyPressed(Key::S))
					trans[3][1] -= speed * ts;
			}
		};

		m_CameraEntity.AddComponent<NativeScriptComponent>().Bind<CameraController>();
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
			m_Camera.OnResize(m_ViewportSize.x, m_ViewportSize.y);

			m_ActiveScene->OnViewportResize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
		}

		// Update
		if (m_ViewportFocused)
			m_Camera.OnUpdate(ts);

		// Render
		m_Framebuffer->Bind();
		RenderCommand::SetClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		RenderCommand::Clear();

		// Update scene
		m_ActiveScene->OnUpdate(ts);

		m_Framebuffer->Unbind();
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

				if (ImGui::Begin("Test"))
				{
					if (m_SquareEntity)
					{
						ImGui::Separator();
						std::string& tag = m_SquareEntity.GetComponent<TagComponent>().Tag;
						ImGui::Text("%s", tag.c_str());
					}

					glm::vec4& color = m_SquareEntity.GetComponent<SpriteRendererComponent>().Color;
					ImGui::ColorEdit4("Square Color", glm::value_ptr(color));

					ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().Transform[3]));

					if (ImGui::Checkbox("Primary Camera", &m_PrimaryCam))
					{
						m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCam;
						m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCam;
					}

					{
						auto& cam = m_SecondCamera.GetComponent<CameraComponent>().Cam;
						float orthoSize = cam.GetOrthohraphicSize();
						if (ImGui::DragFloat("Second Camera Size", &orthoSize))
						{
							cam.SetOrthographicSize(orthoSize);
						}
					}
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
