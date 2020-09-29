#include "dmpch.h"
#include "WindowsWindow.h"

#include "Dominion/Events/KeyEvent.h"
#include "Dominion/Events/MouseEvent.h"
#include "Dominion/Events/WindowEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

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

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

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

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int x, int y)
		{
			WindowsWindow* wnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
			WindowMovedEvent event(wnd, x, y);
			wnd->m_EventCallbackFn(event);
			wnd->m_PosX = static_cast<unsigned int>(x);
			wnd->m_PosY = static_cast<unsigned int>(y);
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowsWindow* wnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
			WindowResizedEvent event(wnd, static_cast<unsigned int>(width), static_cast<unsigned int>(height));
			wnd->m_EventCallbackFn(event);
			wnd->m_Width = static_cast<unsigned int>(width);
			wnd->m_Height = static_cast<unsigned int>(height);
		});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowsWindow* wnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					wnd->m_EventCallbackFn(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					wnd->m_EventCallbackFn(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					wnd->m_EventCallbackFn(event);
					break;
				}
			}
		});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowsWindow* wnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
			KeyTypedEvent event(keycode);
			wnd->m_EventCallbackFn(event);
		});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowsWindow* wnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
			switch (action)
			{
				case GLFW_PRESS:
				{
					MousePressedEvent event(button);
					wnd->m_EventCallbackFn(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseReleasedEvent event(button);
					wnd->m_EventCallbackFn(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowsWindow* wnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			wnd->m_EventCallbackFn(event);
		});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowsWindow* wnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
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
			m_Context->SwapBuffers();
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

	void* WindowsWindow::GetNativeWindow() const
	{
		return m_Window;
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
		if (s_Windows.empty())
		{
			s_GLFWInitialized = false;
			glfwTerminate();
		}
	}

}
