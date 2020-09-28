#include "dmpch.h"
#include "WindowsWindow.h"

#include "Dominion/Events/WindowEvent.h"

#include <GLFW/glfw3.h>

namespace Dominion {

	static bool s_GLFWInitialized = false;

	std::vector<Window*> Window::s_Windows;

	Window* Window::Create(const EventCallbackFn& callback, const WindowProps& props)
	{
		Window* wnd = new WindowsWindow(callback, props);
		s_Windows.push_back(wnd);
		return wnd;
	}

	WindowsWindow::WindowsWindow(const EventCallbackFn& callback, const WindowProps& props)
		: m_Title(props.title), m_Width(props.width), m_Height(props.height), m_EventCallbackFn(callback)
	{
		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			DM_CORE_ASSERT(success, "Could not initialize GLFW!");
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)(props.width), (int)(props.height), m_Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this);
		SetVSync(true);

		glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);

		m_EventCallbackFn(WindowCreatedEvent(this, m_PosX, m_PosY, m_Width, m_Height));

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowsWindow* wnd =  static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
			WindowClosedEvent event(wnd);
			wnd->m_EventCallbackFn(event);
		});
	}

	WindowsWindow::~WindowsWindow()
	{
		if (m_Active)
			Close();
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		if (m_Active)
			glfwSwapBuffers(m_Window);
	}

	int WindowsWindow::GetPosX() const
	{
		return m_PosX;
	}

	int WindowsWindow::GetPosY() const
	{
		return m_PosY;
	}

	unsigned int WindowsWindow::GetWidth() const
	{
		return m_Width;
	}

	unsigned int WindowsWindow::GetHeight() const
	{
		return m_Height;
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		m_EventCallbackFn = callback;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_VSync;
	}

	void WindowsWindow::Close()
	{
		DM_CORE_ASSERT(m_Active, "Window is already closed!");
		glfwDestroyWindow(m_Window);
		m_Active = false;
		s_Windows.erase(std::find(s_Windows.begin(), s_Windows.end(), this));
		m_EventCallbackFn(WindowClosedEvent(this));
		if (s_Windows.empty())
			glfwTerminate();
	}

}