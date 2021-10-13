#include "dmpch.h"
#include "WindowsWindow.h"

#include "Dominion/Events/KeyEvent.h"
#include "Dominion/Events/MouseEvent.h"
#include "Dominion/Events/WindowEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Dominion {

	static bool s_GLFWInitialized = false;

	Window* Window::Create(const EventCallbackFn& callback, const WindowProps& props)
	{
		Window* wnd = new WindowsWindow(callback, props);
		return wnd;
	}

	WindowsWindow::WindowsWindow(const EventCallbackFn& callback, const WindowProps& props)
		: m_Title(props.Title), m_Width(props.Width), m_Height(props.Height), m_EventCallbackFn(callback)
	{
		DM_PROFILE_FUNCTION();

		if (!s_GLFWInitialized)
		{
			DM_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			DM_CORE_ASSERT(success, "Could not initialize GLFW!");
			s_GLFWInitialized = true;
		}

		{
			DM_PROFILE_SCOPE("glfwCreateWindow");
			m_Window = glfwCreateWindow((int)(m_Width), (int)(m_Height), m_Title.c_str(), nullptr, nullptr);
		}

		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();

		glfwSetWindowUserPointer(m_Window, this);
		SetVSync(true);

		glfwGetWindowPos(m_Window, &m_PosX, &m_PosY);

		m_EventCallbackFn(WindowCreatedEvent(this, m_PosX, m_PosY, m_Width, m_Height));

		glfwSetWindowCloseCallback(m_Window, OnCloseCallback);

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int x, int y)
		{
			WindowsWindow* wnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
			WindowMovedEvent event(wnd, x, y);
			wnd->m_EventCallbackFn(event);
			wnd->m_PosX = static_cast<uint32_t>(x);
			wnd->m_PosY = static_cast<uint32_t>(y);
		});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowsWindow* wnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
			WindowResizedEvent event(wnd, static_cast<uint32_t>(width), static_cast<uint32_t>(height));
			wnd->m_EventCallbackFn(event);
			wnd->m_Width = static_cast<uint32_t>(width);
			wnd->m_Height = static_cast<uint32_t>(height);
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

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, uint32_t keycode)
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
		DM_PROFILE_FUNCTION();

		if (m_Active)
			Close();
		s_GLFWInitialized = false;
		glfwTerminate();
	}

	void WindowsWindow::OnUpdate()
	{
		DM_PROFILE_FUNCTION();

		DM_CORE_ASSERT(m_Active, "Window must be active (not closed)!");

		glfwPollEvents();
		if (m_Active)
			m_Context->SwapBuffers();
	}

	int WindowsWindow::GetPosX() const
	{
		DM_CORE_ASSERT(m_Active, "Window must be active (not closed)!");

		return m_PosX;
	}

	int WindowsWindow::GetPosY() const
	{
		DM_CORE_ASSERT(m_Active, "Window must be active (not closed)!");

		return m_PosY;
	}

	uint32_t WindowsWindow::GetWidth() const
	{
		DM_CORE_ASSERT(m_Active, "Window must be active (not closed)!");

		return m_Width;
	}

	uint32_t WindowsWindow::GetHeight() const
	{
		DM_CORE_ASSERT(m_Active, "Window must be active (not closed)!");

		return m_Height;
	}

	void* WindowsWindow::GetNativeWindow() const
	{
		DM_CORE_ASSERT(m_Active, "Window must be active (not closed)!");

		return m_Window;
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		DM_CORE_ASSERT(m_Active, "Window must be active (not closed)!");

		m_EventCallbackFn = callback;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		DM_PROFILE_FUNCTION();

		DM_CORE_ASSERT(m_Active, "Window must be active (not closed)!");

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		DM_CORE_ASSERT(m_Active, "Window must be active (not closed)!");

		return m_VSync;
	}

	void WindowsWindow::Close()
	{
		DM_PROFILE_FUNCTION();

		DM_CORE_ASSERT(m_Active, "Window is already closed!");

		glfwSetWindowShouldClose(m_Window, true);
		OnCloseCallback(m_Window);
		delete m_Context;
		m_Active = false;
	}

	void WindowsWindow::OnCloseCallback(GLFWwindow* window)
	{
		WindowsWindow* wnd = static_cast<WindowsWindow*>(glfwGetWindowUserPointer(window));
		WindowClosedEvent event(wnd);
		wnd->m_EventCallbackFn(event);
	}

}
