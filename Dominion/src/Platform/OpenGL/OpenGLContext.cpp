#include "dmpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Dominion {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: mWindowHandle(windowHandle)
	{
		DM_CORE_ASSERT(windowHandle, "Window handle is null!")
	}

	void OpenGLContext::Init()
	{
		DM_PROFILE_FUNCTION();

		glfwMakeContextCurrent(mWindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		DM_CORE_ASSERT(status, "Failed to initialize Glad!");

		DM_CORE_INFO("OpenGL Info:");
		DM_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		DM_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		DM_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

		DM_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 6), "Dominion requires at least OpenGL version 4.6!");
	}

	void OpenGLContext::SwapBuffers()
	{
		DM_PROFILE_FUNCTION();

		glfwSwapBuffers(mWindowHandle);
	}

}
