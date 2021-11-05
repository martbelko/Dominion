#include "dmpch.h"
#include "Dominion/Renderer/RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Dominion {

	RendererAPI::API RendererAPI::sAPI = RendererAPI::API::OpenGL;

	Scope<RendererAPI> RendererAPI::Create()
	{
		switch (sAPI)
		{
			case RendererAPI::API::None:    DM_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateScope<OpenGLRendererAPI>();
		}

		DM_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
