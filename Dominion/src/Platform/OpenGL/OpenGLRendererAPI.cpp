#include "dmpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Dominion {

	static void OpenGLMessageCallback(
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

	static GLenum TestFuncToOpenGLFunc(TestFunc func)
	{
		switch (func)
		{
			case TestFunc::NEVER:        return GL_NEVER;
			case TestFunc::LESS:         return GL_LESS;
			case TestFunc::EQUAL:        return GL_EQUAL;
			case TestFunc::LESSEQUAL:    return GL_LEQUAL;
			case TestFunc::GREATER:      return GL_GREATER;
			case TestFunc::NOTEQUAL:     return GL_NOTEQUAL;
			case TestFunc::GREATEREQUAL: return GL_GEQUAL;
			case TestFunc::ALWAYS:       return GL_ALWAYS;
		#ifdef DM_DEBUG
			default: DM_CORE_ASSERT(false, "Unknown TestFunc!"); return 0;
		#endif
		}
	}

	static GLenum StencilOperationToGLOperation(StencilOperation op)
	{
		switch (op)
		{
			case Dominion::StencilOperation::KEEP:      return GL_KEEP;
			case Dominion::StencilOperation::ZERO:      return GL_ZERO;
			case Dominion::StencilOperation::REPLACE:   return GL_REPLACE;
			case Dominion::StencilOperation::INCR:      return GL_INCR;
			case Dominion::StencilOperation::INCR_WRAP: return GL_INCR_WRAP;
			case Dominion::StencilOperation::DECR:      return GL_DECR;
			case Dominion::StencilOperation::DECR_WRAP: return GL_DECR_WRAP;
			case Dominion::StencilOperation::INVERT:    return GL_INVERT;
		#if DM_DEBUG
			default: DM_CORE_ASSERT(false, "Unknown StencilOperation!"); return 0;
		#endif
		}
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

		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glStencilMask(0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
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

	void OpenGLRendererAPI::SetDepthTestFunc(TestFunc func)
	{
		glDepthFunc(TestFuncToOpenGLFunc(func));
	}

	void OpenGLRendererAPI::ClearStencilBuffer()
	{
		glClear(GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRendererAPI::EnableStencilTest()
	{
		glEnable(GL_STENCIL_TEST);
	}

	void OpenGLRendererAPI::DisableStencilTest()
	{
		glDisable(GL_STENCIL_TEST);
	}

	void OpenGLRendererAPI::SetStencilTestFunc(TestFunc func, I32F ref, U32F mask)
	{
		glStencilFunc(TestFuncToOpenGLFunc(func), ref, mask);
	}

	void OpenGLRendererAPI::SetStencilOperation(StencilOperation sfail, StencilOperation dpfail, StencilOperation dppass)
	{
		glStencilOp(StencilOperationToGLOperation(sfail), StencilOperationToGLOperation(dpfail), StencilOperationToGLOperation(dppass));
	}

	void OpenGLRendererAPI::SetStencilMask(U32F mask)
	{
		glStencilMask(mask);
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
