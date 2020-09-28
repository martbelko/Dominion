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

		unsigned int GetWidth() const override { return m_Width; }
		unsigned int GetHeight() const override { return m_Height; }

		void SetEventCallback(const EventCallbackFn& callback) override { m_EventCallbackFn = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	private:
		GLFWwindow* m_Window;

		std::string m_Title;
		unsigned int m_Width, m_Height;
		bool m_VSync;

		EventCallbackFn m_EventCallbackFn;
	};
}