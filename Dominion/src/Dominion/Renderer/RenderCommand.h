#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/RendererAPI.h"
#include "Dominion/Renderer/Pipeline.h"

#include <memory>

namespace Dominion {

	class RenderCommand
	{
	public:
		static void Init()
		{
			s_RendererAPI->Init();
		}

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(F32 r, F32 g, F32 b, F32 a = 1.0f)
		{
			s_RendererAPI->SetClearColor(r, g, b, a);
		}

		static void ClearColorBuffer()
		{
			s_RendererAPI->ClearColorBuffer();
		}

		static void ClearDepthBuffer()
		{
			s_RendererAPI->ClearDepthBuffer();
		}

		static void EnableDepthTest()
		{
			s_RendererAPI->EnableDepthTest();
		}

		static void DisableDepthTest()
		{
			s_RendererAPI->DisableDepthTest();
		}

		static void SetDepthTestFunc(TestFunc func)
		{
			s_RendererAPI->SetDepthTestFunc(func);
		}

		static void ClearStencilBuffer()
		{
			s_RendererAPI->ClearStencilBuffer();
		}

		static void EnableStencilTest()
		{
			s_RendererAPI->EnableStencilTest();
		}

		static void DisableStencilTest()
		{
			s_RendererAPI->DisableStencilTest();
		}

		static void SetStencilTestFunc(TestFunc func, I32F ref, U32F mask)
		{
			s_RendererAPI->SetStencilTestFunc(func, ref, mask);
		}

		static void SetStencilOperation(StencilOperation stencilFail, StencilOperation stencilPassDepthFail, StencilOperation stencilDepthPass)
		{
			s_RendererAPI->SetStencilOperation(stencilFail, stencilPassDepthFail, stencilDepthPass);
		}

		static void SetStencilMask(U32F mask)
		{
			s_RendererAPI->SetStencilMask(mask);
		}

		static void DrawIndexed(const Ref<Pipeline> pipeline, uint32_t count = 0)
		{
			s_RendererAPI->DrawIndexed(pipeline, count);
		}

		static void Draw(const Ref<Pipeline> pipeline, uint32_t count)
		{
			s_RendererAPI->Draw(pipeline, count);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};

}
