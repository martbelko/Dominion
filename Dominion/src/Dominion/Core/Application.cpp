#include "dmpch.h"
#include "Application.h"

#include "Dominion/Core/Input.h"
#include "Dominion/Renderer/RenderCommand.h"
#include "Dominion/Renderer/Buffer.h"
#include "Dominion/Renderer/InputLayout.h"
#include "Dominion/Renderer/Renderer.h"

#if DM_INCLUDE_IMGUI == 1

	#if defined(new)
		#undef new
		#include <imgui.h>
		#define new DEBUG_NEW
	#else
		#include <imgui.h>
	#endif

#endif

namespace Dominion {

	Application* Application::s_Application = nullptr;

	Application::Application()
	{
		if (s_Application == nullptr)
		{
			s_Application = this;
			m_Window = Window::Create(DM_BIND_EVENT_FN(Application::OnEvent));

			#if DM_INCLUDE_IMGUI == 1
				m_ImGuiLayer = new ImGuiLayer();
				PushOverlay(m_ImGuiLayer);
			#endif

			Renderer::Init();

			m_LastFrameTime = std::chrono::system_clock::now();
		}
		else
		{
			DM_DEBUGBREAK();
		}
	}

	Application::~Application()
	{
	#if DM_INCLUDE_IMGUI == 1
		m_LayerStack.PopOverlay(m_ImGuiLayer);
		delete m_ImGuiLayer;
	#endif
		Renderer::Shutdown();
		delete m_Window;
	}

	void Application::Run()
	{
		while (m_Running)
		{
			auto end = std::chrono::system_clock::now();
			Timestep ts = static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_LastFrameTime).count());
			m_LastFrameTime = std::chrono::system_clock::now();

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(ts);
			}

			#if DM_INCLUDE_IMGUI == 1
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack)
					layer->OnImGuiRender();
				m_ImGuiLayer->End();
			#endif

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
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
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	Window& Application::GetWindow() const
	{
		return *m_Window;
	}

	bool Application::OnWindowCreated(WindowCreatedEvent& event)
	{
		return false;
	}

	bool Application::OnWindowClosed(WindowClosedEvent& event)
	{
		m_Window->Close();
		m_Running = false;
		return false;
	}

	bool Application::OnWindowResized(WindowResizedEvent& e)
	{
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
