#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/Pipeline.h"

#include <memory>

namespace Dominion {

	enum class DepthTestFunc
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
		virtual void SetDepthTestFunc(DepthTestFunc func) = 0;
		virtual void DrawIndexed(const Ref<Pipeline> pipeline, uint32_t count) = 0;
		virtual void Draw(const Ref<Pipeline> pipeline, uint32_t count) = 0;

		static API GetAPI() { return s_API; }
		static Scope<RendererAPI> Create();
	private:
		static API s_API;
	};

}
