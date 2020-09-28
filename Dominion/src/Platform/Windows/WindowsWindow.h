#pragma once

#include "Dominion/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Dominion {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const EventCallbackFn& callback, const WindowProps& props);
		virtual ~WindowsWindow() override;

		void OnUpdate() override;

		int GetPosX() const override;
		int GetPosY() const override;
		unsigned int GetWidth() const override;
		unsigned int GetHeight() const override;

		virtual void* GetNativeWindow() const override;

		void SetEventCallback(const EventCallbackFn& callback) override;
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		virtual void Close() override;
	private:
		GLFWwindow* m_Window;
		bool m_Active = true;

		std::string m_Title;
		int m_PosX, m_PosY;
		unsigned int m_Width, m_Height;
		bool m_VSync = true;

		EventCallbackFn m_EventCallbackFn;
	};
}
