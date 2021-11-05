#include "dmpch.h"
#include "Platform/Windows/WindowsWindow.h"

#include "Dominion/Core/Input.h"

#include "Dominion/Events/ApplicationEvent.h"
#include "Dominion/Events/MouseEvent.h"
#include "Dominion/Events/KeyEvent.h"

#include "Dominion/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace Dominion {

	static uint8_t sGLFWWindowCount = 0;

	static void GLFWErrorCallback(int error, const char* description)
	{
		DM_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		DM_PROFILE_FUNCTION();

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		DM_PROFILE_FUNCTION();

		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		DM_PROFILE_FUNCTION();

		mData.title = props.title;
		mData.width = props.width;
		mData.height = props.height;

		DM_CORE_INFO("Creating window {0} ({1}, {2})", props.title, props.width, props.height);

		if (sGLFWWindowCount == 0)
		{
			DM_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			DM_CORE_ASSERT(success, "Could not initialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
		}

		{
			DM_PROFILE_SCOPE("glfwCreateWindow");
		#if defined(DM_DEBUG)
			if (Renderer::GetAPI() == RendererAPI::API::OpenGL)
				glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
		#endif
			mWindow = glfwCreateWindow((int)props.width, (int)props.height, mData.title.c_str(), nullptr, nullptr);
			++sGLFWWindowCount;
		}

		mContext = GraphicsContext::Create(mWindow);
		mContext->Init();

		glfwSetWindowUserPointer(mWindow, &mData);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.eventCallback(event);
		});

		glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.eventCallback(event);
		});

		glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.eventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.eventCallback(event);
					break;
				}
			}
		});

		glfwSetCharCallback(mWindow, [](GLFWwindow* window, unsigned int keycode)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.eventCallback(event);
		});

		glfwSetMouseButtonCallback(mWindow, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.eventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.eventCallback(event);
					break;
				}
			}
		});

		glfwSetScrollCallback(mWindow, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.eventCallback(event);
		});

		glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);
			data.eventCallback(event);
		});
	}

	void WindowsWindow::Shutdown()
	{
		DM_PROFILE_FUNCTION();

		glfwDestroyWindow(mWindow);
		--sGLFWWindowCount;

		if (sGLFWWindowCount == 0)
		{
			glfwTerminate();
		}
	}

	void WindowsWindow::OnUpdate()
	{
		DM_PROFILE_FUNCTION();

		glfwPollEvents();
		mContext->SwapBuffers();
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		DM_PROFILE_FUNCTION();

		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		mData.vSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return mData.vSync;
	}

}
