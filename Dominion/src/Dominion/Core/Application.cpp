#include "dmpch.h"
#include "Application.h"

#include "Dominion/Core/Input.h"
#include "Dominion/Renderer/RenderCommand.h"
#include "Dominion/Renderer/Buffer.h"
#include "Dominion/Renderer/InputLayout.h"
#include "Dominion/Renderer/Renderer.h"

#include "Dominion/Physics/Physics.h"

/* TODO: Temporary */
#include "Dominion/Scene/Scene.h"
#include "Dominion/Scene/Entity.h"
#include "Dominion/Scene/Components/BaseComponent.h"
#include "Dominion/Scene/Components/TransformComponent.h"
#include "Dominion/Scene/Components/SpriteRendererComponent.h"
#include "Dominion/Scene/Components/CameraComponent.h"
#include "Dominion/Scene/Components/NativeScriptComponent.h"
#include "Dominion/Scene/Components/ColliderComponent.h"
#include "Dominion/Scene/Components/RigidBodyComponent.h"

#include "Dominion/Renderer/PerspectiveCameraController.h"
#include "Dominion/Renderer/Model.h"
#include "Dominion/Renderer/Shader.h"
#include "Dominion/Events/KeyEvent.h"
#include "Dominion/Utils/Random.h"

#if DM_INCLUDE_IMGUI
	#include "DominionImGui.h"
#endif

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Dominion {

	Application* Application::s_Application = nullptr;

	Application::Application(const std::string_view& name)
	{
		DM_PROFILE_FUNCTION();

		if (s_Application == nullptr)
		{
			s_Application = this;
			WindowProps wndProps;
			wndProps.Title = name;
			wndProps.Width = 1280;
			wndProps.Height = 720;
			m_Window = Window::Create(DM_BIND_EVENT_FN(Application::OnEvent), wndProps);

			Physics::Init();
			Renderer::Init();

#if DM_INCLUDE_IMGUI
			m_ImGuiLayer = new ImGuiLayer();
			PushOverlay(m_ImGuiLayer);
#endif
		}
		else
		{
			DM_DEBUGBREAK();
		}
	}

	Application::~Application()
	{
		DM_PROFILE_FUNCTION();

#if DM_INCLUDE_IMGUI
		m_LayerStack.PopOverlay(m_ImGuiLayer);
		delete m_ImGuiLayer;
#endif
		Renderer::Shutdown();
		delete m_Window;
	}

	void Application::Run()
	{
		DM_PROFILE_FUNCTION();

		m_LastFrameTime = std::chrono::system_clock::now();
		while (m_Running)
		{
			DM_PROFILE_SCOPE("Run Loop");

			auto end = std::chrono::system_clock::now();
			Timestep ts = static_cast<F32>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_LastFrameTime).count());
			m_LastFrameTime = std::chrono::system_clock::now();

			if (!m_Minimized)
			{
				{
					DM_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : m_LayerStack)
						layer->OnUpdate(ts);
				}
			}

			{
				DM_PROFILE_SCOPE("LayerStack OnImGuiRenderr");

#if DM_INCLUDE_IMGUI
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
#endif
			}

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		DM_PROFILE_FUNCTION();

		e.Dispatch<WindowCreatedEvent>(DM_BIND_EVENT_FN(Application::OnWindowCreated));
		e.Dispatch<WindowClosedEvent>(DM_BIND_EVENT_FN(Application::OnWindowClosed));
		e.Dispatch<WindowResizedEvent>(DM_BIND_EVENT_FN(Application::OnWindowResized));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
		{
			if (e.Handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		DM_PROFILE_FUNCTION();

		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		DM_PROFILE_FUNCTION();

		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	ImGuiLayer* Application::GetImGuiLayer()
	{
		return m_ImGuiLayer;
	}

	Window& Application::GetWindow() const
	{
		return *m_Window;
	}

	void Application::Close()
	{
		m_Running = false;
	}

	bool Application::OnWindowCreated(WindowCreatedEvent& e)
	{
		return false;
	}

	bool Application::OnWindowClosed(WindowClosedEvent& e)
	{
		m_Running = false;
		return false;
	}

	bool Application::OnWindowResized(WindowResizedEvent& e)
	{
		DM_PROFILE_FUNCTION();

		uint32_t width = e.GetWidth();
		uint32_t height = e.GetHeight();
		if (width == 0 || height == 0)
		{
			m_Minimized = true;
			return false;
		}

		Renderer::OnWindowResize(width, height);

		m_Minimized = false;
		return false;
	}

	Application& Application::Get()
	{
		return *s_Application;
	}

}
