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

		static Application& Get();
	private:
		bool OnWindowCreated(WindowCreatedEvent& event);
		bool OnWindowClosed(WindowClosedEvent& event);
	private:
		static Application* s_Application;
		Window* m_Window = nullptr;
		bool m_Running = true;
		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer = nullptr;
	};

	Application* CreateApplication();

}
