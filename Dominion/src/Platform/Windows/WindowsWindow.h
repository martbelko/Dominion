#pragma once

#include "Dominion/Core/Window.h"
#include "Dominion/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Dominion {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		virtual void OnUpdate() override;

		virtual unsigned int GetWidth() const override { return mData.width; }
		virtual unsigned int GetHeight() const override { return mData.height; }

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) override { mData.eventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		virtual void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		Scope<GraphicsContext> mContext;

		struct WindowData
		{
			std::string title;
			unsigned int width, height;
			bool vSync;

			EventCallbackFn eventCallback;
		};

		WindowData mData;
	};

}
