#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Window.h"
#include "Dominion/Events/WindowEvent.h"
#include "Dominion/Core/Layer.h"
#include "Dominion/Core/LayerStack.h"
#include "Dominion/ImGui/ImGuiLayer.h"

#include "Dominion/Renderer/Shader.h"
#include "Dominion/Renderer/Buffer.h"
#include "Dominion/Renderer/InputLayout.h"
#include "Dominion/Renderer/Pipeline.h"

#include "Dominion/Scene/Camera.h"

#include <string>

namespace Dominion {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		Window& GetWindow() const;

		void Close();

		static Application& Get();
	private:
		bool OnWindowCreated(WindowCreatedEvent& e);
		bool OnWindowClosed(WindowClosedEvent& e);
		bool OnWindowResized(WindowResizedEvent& e);
	private:
		static Application* s_Application;
		Window* m_Window = nullptr;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
#if DM_INCLUDE_IMGUI == 1
		ImGuiLayer* m_ImGuiLayer = nullptr;
#endif

		std::chrono::system_clock::time_point m_LastFrameTime;
	};

	Application* CreateApplication();

}
