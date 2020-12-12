#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Pipeline.h"

#include <memory>

namespace Dominion {

	enum class TestFunc
	{
		NEVER = 0,
		LESS,
		EQUAL,
		LESSEQUAL,
		GREATER,
		NOTEQUAL,
		GREATEREQUAL,
		ALWAYS
	};

	enum class StencilOperation
	{
		KEEP = 0,
		ZERO,
		REPLACE,
		INCR,
		INCR_WRAP,
		DECR,
		DECR_WRAP,
		INVERT
	};

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1
		};
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(F32 r, F32 g, F32 b, F32 a) = 0;
		virtual void ClearColorBuffer() = 0;

		virtual void ClearDepthBuffer() = 0;
		virtual void EnableDepthTest() = 0;
		virtual void DisableDepthTest() = 0;
		virtual void SetDepthTestFunc(TestFunc func) = 0;

		virtual void ClearStencilBuffer() = 0;
		virtual void EnableStencilTest() = 0;
		virtual void DisableStencilTest() = 0;
		virtual void SetStencilTestFunc(TestFunc func, I32F ref, U32F mask) = 0;
		virtual void SetStencilOperation(StencilOperation stencilFail, StencilOperation stencilPassDepthFail, StencilOperation stencilDepthPass) = 0;
		virtual void SetStencilMask(U32F mask) = 0;

		virtual void DrawIndexed(const Ref<Pipeline> pipeline, uint32_t count) = 0;
		virtual void Draw(const Ref<Pipeline> pipeline, uint32_t count) = 0;

		static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};

}
