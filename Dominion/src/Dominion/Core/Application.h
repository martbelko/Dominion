#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Window.h"
#include "Dominion/Events/WindowEvent.h"
#include "Dominion/Core/Layer.h"
#include "Dominion/Core/LayerStack.h"
#include "Dominion/ImGui/ImGuiLayer.h"

#include "Dominion/Renderer/Shader.h"

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
		ImGuiLayer* m_ImGuiLayer;

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		Shader* m_Shader;
	};

	Application* CreateApplication();

}
