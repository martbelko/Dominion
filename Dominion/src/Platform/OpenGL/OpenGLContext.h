#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Dominion {

	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		virtual void Init() override;
		virtual void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};

}
