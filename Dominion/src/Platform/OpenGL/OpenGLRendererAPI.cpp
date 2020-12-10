#include "dmpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Dominion {

	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
			case GL_DEBUG_SEVERITY_HIGH:         DM_CORE_FATAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       DM_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          DM_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: DM_CORE_TRACE(message); return;
		}

		DM_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init()
	{
	#ifdef DM_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::ClearColorBuffer()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::ClearDepthBuffer()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::EnableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::DisableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void OpenGLRendererAPI::SetDepthTestFunc(DepthTestFunc func)
	{
		GLenum glFunc;
		switch (func)
		{
			case Dominion::DepthTestFunc::NEVER:        glFunc = GL_NEVER;    break;
			case Dominion::DepthTestFunc::LESS:         glFunc = GL_LESS;     break;
			case Dominion::DepthTestFunc::EQUAL:        glFunc = GL_EQUAL;    break;
			case Dominion::DepthTestFunc::LESSEQUAL:    glFunc = GL_LEQUAL;   break;
			case Dominion::DepthTestFunc::GREATER:      glFunc = GL_GREATER;  break;
			case Dominion::DepthTestFunc::NOTEQUAL:     glFunc = GL_NOTEQUAL; break;
			case Dominion::DepthTestFunc::GREATEREQUAL: glFunc = GL_GEQUAL;   break;
			case Dominion::DepthTestFunc::ALWAYS:       glFunc = GL_ALWAYS;   break;
		#ifdef DM_DEBUG
			default: DM_CORE_ASSERT(false, "Unknown DepthTestFunc!"); glFunc = 0; break;
		#endif
		}

		glDepthFunc(glFunc);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<Pipeline> pipeline, uint32_t count)
	{
		uint32_t indexCount = count ? count : pipeline->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::Draw(const Ref<Pipeline> pipeline, uint32_t count)
	{
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

}
