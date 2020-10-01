#include "dmpch.h"
#include "RenderCommand.h"

namespace Dominion {

	std::unique_ptr<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();

}
