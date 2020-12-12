#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Renderer/RenderCommand.h"
#include "Dominion/Renderer/Renderer2D.h"
#include "Dominion/Renderer/Shader.h"
#include "Dominion/Renderer/Pipeline.h"
#include "Dominion/Renderer/OrthographicCamera.h"
#include "Dominion/Renderer/PerspectiveCamera.h"

namespace Dominion {

	// Forward declarations
	class Model;
	class Mesh;

	class Renderer
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const OrthographicCamera& camera);
		static void BeginScene(const PerspectiveCamera& camera);
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<Pipeline>& pipeline, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Mesh& mesh, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));
		static void Submit(const Ref<Model>& model, const Ref<Shader>& shader, const glm::mat4& transform = glm::mat4(1.0f));

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		static glm::mat4 m_ProjectionViewMatrix;
	};

}
