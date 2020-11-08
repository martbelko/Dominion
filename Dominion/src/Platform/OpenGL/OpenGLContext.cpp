#include "dmpch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Dominion {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		DM_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		DM_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DM_CORE_ASSERT(status, "Failed to initialize Glad!");

		DM_CORE_INFO("OpenGL Info:");
		DM_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		DM_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		DM_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		DM_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Dominion requires at least OpenGL version 4.5!");
	}

	void OpenGLContext::SwapBuffers()
	{
		DM_PROFILE_FUNCTION();

		glfwSwapBuffers(m_WindowHandle);
	}

}
