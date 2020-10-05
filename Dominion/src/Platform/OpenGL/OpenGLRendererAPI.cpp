#include "dmpch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

namespace Dominion {

	void OpenGLRendererAPI::Init()
	{

	}

	void OpenGLRendererAPI::SetClearColor(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<Pipeline> pipeline, uint32_t count)
	{
		uint32_t indexCount = count ? count : pipeline->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);
	}

}
