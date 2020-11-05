#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/RenderCommand.h"
#include "Dominion/Renderer/Renderer2D.h"
#include "Dominion/Renderer/Shader.h"
#include "Dominion/Renderer/Pipeline.h"
#include "Dominion/Scene/Camera.h"

namespace Dominion {

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<Pipeline>& pipeline);

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		static glm::mat4& m_ProjectionMatrix;
	};

}
