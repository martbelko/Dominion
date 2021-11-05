#include "dmpch.h"
#include "Dominion/Core/Application.h"

#include "Dominion/Core/Log.h"

#include "Dominion/Renderer/Renderer.h"

#include "Dominion/Core/Input.h"

#include <GLFW/glfw3.h>

namespace Dominion {

	Application* Application::sInstance = nullptr;

	Application::Application(const std::string& name, ApplicationCommandLineArgs args)
		: mCommandLineArgs(args)
	{
		DM_PROFILE_FUNCTION();

		DM_CORE_ASSERT(!sInstance, "Application already exists!");
		sInstance = this;
		mWindow = Window::Create(WindowProps(name));
		mWindow->SetEventCallback(DM_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);
	}

	Application::~Application()
	{
		DM_PROFILE_FUNCTION();

		Renderer::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		DM_PROFILE_FUNCTION();

		mLayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		DM_PROFILE_FUNCTION();

		mLayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		mRunning = false;
	}

	void Application::OnEvent(Event& e)
	{
		DM_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(DM_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(DM_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = mLayerStack.rbegin(); it != mLayerStack.rend(); ++it)
		{
			if (e.handled)
				break;
			(*it)->OnEvent(e);
		}
	}

	void Application::Run()
	{
		DM_PROFILE_FUNCTION();

		while (mRunning)
		{
			DM_PROFILE_SCOPE("RunLoop");

			float time = (float)glfwGetTime();
			Timestep timestep = time - mLastFrameTime;
			mLastFrameTime = time;

			if (!mMinimized)
			{
				{
					DM_PROFILE_SCOPE("LayerStack OnUpdate");

					for (Layer* layer : mLayerStack)
						layer->OnUpdate(timestep);
				}

				mImGuiLayer->Begin();
				{
					DM_PROFILE_SCOPE("LayerStack OnImGuiRender");

					for (Layer* layer : mLayerStack)
						layer->OnImGuiRender();
				}
				mImGuiLayer->End();
			}

			mWindow->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		DM_PROFILE_FUNCTION();

		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			mMinimized = true;
			return false;
		}

		mMinimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}
