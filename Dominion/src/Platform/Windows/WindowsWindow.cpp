#include "dmpch.h"
#include "WindowsWindow.h"

#include "Dominion/Events/WindowEvent.h"

namespace Dominion {

	static bool s_GLFWInitialized = false;

	std::vector<Window*> Window::s_Windows;

	Window* Window::Create(const WindowProps& props)
	{
		Window* wnd = new WindowsWindow(props);
		s_Windows.push_back(wnd);
		return wnd;
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
		: m_Title(props.title), m_Width(props.width), m_Height(props.height)
	{
		DM_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

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

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowsWindow* wnd =  static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
			WindowClosedEvent event(wnd);
			wnd->m_EventCallbackFn(event);
		});
	}

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_Window);
		if (s_Windows.size() == 0)
		{

		}
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
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

}