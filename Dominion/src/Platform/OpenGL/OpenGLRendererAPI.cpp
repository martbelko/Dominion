#include "dmpch.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

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
			case GL_DEBUG_SEVERITY_HIGH:         DM_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       DM_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          DM_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: DM_CORE_TRACE(message); return;
		}

		DM_CORE_ASSERT(false, "Unknown severity level!");
	}

	static GLenum DominionTopologyToGLTopology(Topology topology)
	{
		switch (topology)
		{
			case Topology::POINTS: return GL_POINTS;
			case Topology::LINE_STRIP: return GL_LINE_STRIP;
			case Topology::LINE_LOOP: return GL_LINE_LOOP;
			case Topology::LINES: return GL_LINES;
			case Topology::LINE_STRIP_ADJACENCY: return GL_LINE_STRIP_ADJACENCY;
			case Topology::LINES_ADJACENCY: return GL_LINES_ADJACENCY;
			case Topology::TRIANGLE_STRIP: return GL_TRIANGLE_STRIP;
			case Topology::TRIANGLE_FAN: return GL_TRIANGLE_FAN;
			case Topology::TRIANGLES: return GL_TRIANGLES;
			case Topology::TRIANGLE_STRIP_ADJACENCY: return GL_TRIANGLE_STRIP_ADJACENCY;
			case Topology::TRIANGLES_ADJACENCY: return GL_TRIANGLES_ADJACENCY;
			case Topology::PATCHES: return GL_PATCHES;
		}

		DM_ASSERT(false, "Unknown topology");
		return 0;
	}

	void OpenGLRendererAPI::Init()
	{
		DM_PROFILE_FUNCTION();

	#ifdef DM_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::SetLineWidth(float lineWidth)
	{
		glLineWidth(lineWidth);
	}

	void OpenGLRendererAPI::Draw(Topology topology, const Ref<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();
		glDrawArrays(DominionTopologyToGLTopology(topology), 0, vertexCount);
	}

	void OpenGLRendererAPI::DrawIndexed(Topology topology, const Ref<VertexArray>& vertexArray, uint32_t indexCount)
	{
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		vertexArray->Bind();
		glDrawElements(DominionTopologyToGLTopology(topology), count, GL_UNSIGNED_INT, nullptr);
	}

}
