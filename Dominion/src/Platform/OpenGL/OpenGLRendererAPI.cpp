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
			case Topology::Points: return GL_POINTS;
			case Topology::LineStrip: return GL_LINE_STRIP;
			case Topology::LineLoop: return GL_LINE_LOOP;
			case Topology::Lines: return GL_LINES;
			case Topology::LineStripAdjacency: return GL_LINE_STRIP_ADJACENCY;
			case Topology::LinesAdjacency: return GL_LINES_ADJACENCY;
			case Topology::TriangleStrip: return GL_TRIANGLE_STRIP;
			case Topology::TriangleFan: return GL_TRIANGLE_FAN;
			case Topology::Triangles: return GL_TRIANGLES;
			case Topology::TriangleStripAdjacency: return GL_TRIANGLE_STRIP_ADJACENCY;
			case Topology::TrianglesAdjacency: return GL_TRIANGLES_ADJACENCY;
			case Topology::Patches: return GL_PATCHES;
		}

		DM_ASSERT(false, "Unknown topology");
		return 0;
	}

	static GLbitfield DominionRenderTargetToGLRenderTarget(RenderTarget target)
	{
		GLbitfield mask = 0;
		if (static_cast<uint32_t>(target) & static_cast<uint32_t>(RenderTarget::Depth))
			mask |= GL_DEPTH_BUFFER_BIT;
		if (static_cast<uint32_t>(target) & static_cast<uint32_t>(RenderTarget::Stencil))
			mask |= GL_STENCIL_BUFFER_BIT;
		if (static_cast<uint32_t>(target) & static_cast<uint32_t>(RenderTarget::Color))
			mask |= GL_COLOR_BUFFER_BIT;

		return mask;
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

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::GetViewport(uint32_t& x, uint32_t& y, uint32_t& width, uint32_t& height)
	{
		GLint viewport[4];
		glGetIntegerv(GL_VIEWPORT, viewport);
		x = viewport[0];
		y = viewport[1];
		width = viewport[2];
		height = viewport[3];
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear(RenderTarget target)
	{
		glClear(DominionRenderTargetToGLRenderTarget(target));
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
