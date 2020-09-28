#pragma once

#include <GLFW/glfw3.h>

#include "Dominion/Core/Window.h"

namespace Dominion {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow() override;

		void OnUpdate() override;

		unsigned int GetWidth() const override;
		unsigned int GetHeight() const override;

		void SetEventCallback(const EventCallbackFn& callback) override;
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void Close() override;
	private:
		GLFWwindow* m_Window;
		bool m_Active = true;

		std::string m_Title;
		unsigned int m_Width, m_Height;
		bool m_VSync = true;

		EventCallbackFn m_EventCallbackFn;
	};
}