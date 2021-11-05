#pragma once

#include "Dominion/Core/Base.h"

#include "Dominion/Core/Window.h"
#include "Dominion/Core/LayerStack.h"
#include "Dominion/Events/Event.h"
#include "Dominion/Events/ApplicationEvent.h"

#include "Dominion/Core/Timestep.h"

#include "Dominion/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace Dominion {

	struct ApplicationCommandLineArgs
	{
		int count = 0;
		char** args = nullptr;

		const char* operator[](int index) const
		{
			DM_CORE_ASSERT(index < count);
			return args[index];
		}
	};

	class Application
	{
	public:
		Application(const std::string& name = "Dominion App", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return *mWindow; }

		void Close();

		ImGuiLayer* GetImGuiLayer() { return mImGuiLayer; }

		static Application& Get() { return *sInstance; }

		ApplicationCommandLineArgs GetCommandLineArgs() const { return mCommandLineArgs; }
	private:
		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		ApplicationCommandLineArgs mCommandLineArgs;
		Scope<Window> mWindow;
		ImGuiLayer* mImGuiLayer;
		bool mRunning = true;
		bool mMinimized = false;
		LayerStack mLayerStack;
		float mLastFrameTime = 0.0f;
	private:
		static Application* sInstance;
		friend int ::main(int argc, char** argv);
	};

	// To be defined in CLIENT
	Application* CreateApplication(ApplicationCommandLineArgs args);

}
