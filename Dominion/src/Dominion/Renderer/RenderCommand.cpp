#include "dmpch.h"
#include "RenderCommand.h"

namespace Dominion {

	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}
