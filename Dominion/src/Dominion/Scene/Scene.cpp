#include "dmpch.h"
#include "Scene.h"

#include "Dominion/Scene/Components.h"
#include "Dominion/Scene/Entity.h"
#include "Dominion/Renderer/Renderer2D.h"
#include "Dominion/Renderer/RenderCommand.h"
#include "Dominion/Scene/ScriptableEntity.h"

#include <glm/glm.hpp>

#include <string_view>

namespace Dominion {

	Scene::Scene(const std::string& sceneName)
		: m_SceneName(sceneName)
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity()
	{
		return CreateEntity("Entity");
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = Entity(m_Registry.create(), this);
		entity.AddComponent<TagComponent>(name);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		entity.Destroy();
	}

	void Scene::OnUpdateEditor(Timestep timestep, const EditorCamera& camera, Entity selectedEntity)
	{
		Renderer2D::BeginScene(camera);

		bool selectedEntityFound = false;
		auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
		for (auto entity : view)
		{
			auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);

			if (static_cast<uint32_t>(entity) == static_cast<uint32_t>(selectedEntity))
			{
				selectedEntityFound = true;
			}
			else
			{
				if (sprite.Texture != nullptr)
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.Color, sprite.TilingFactor);
				else
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}
		}

		Renderer2D::EndScene();

		if (selectedEntityFound)
		{
			RenderCommand::EnableStencilTest();

			Renderer2D::BeginScene(camera);

			RenderCommand::SetStencilOperation(Dominion::StencilOperation::KEEP, Dominion::StencilOperation::KEEP, Dominion::StencilOperation::REPLACE);
			RenderCommand::SetStencilTestFunc(Dominion::TestFunc::ALWAYS, 0, 0xFF);
			RenderCommand::SetStencilMask(0xFF);

			RenderCommand::ClearStencilBuffer();

			RenderCommand::SetStencilTestFunc(Dominion::TestFunc::ALWAYS, 1, 0xFF);

			TransformComponent& tc = selectedEntity.GetComponent<TransformComponent>();
			SpriteRendererComponent& sprite = selectedEntity.GetComponent<SpriteRendererComponent>();

			if (sprite.Texture != nullptr)
				Renderer2D::DrawQuad(tc.GetTransform(), sprite.Texture, sprite.Color, sprite.TilingFactor);
			else
				Renderer2D::DrawQuad(tc.GetTransform(), sprite.Color);

			Renderer2D::Flush();

			RenderCommand::SetStencilTestFunc(Dominion::TestFunc::NOTEQUAL, 1, 0xFF);
			RenderCommand::SetStencilMask(0x00);

			tc.Scale += glm::vec3(0.1f, 0.1f, 0.0f);
			Renderer2D::DrawQuad(tc.GetTransform(), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			tc.Scale -= glm::vec3(0.1f, 0.1f, 0.0f);

			Renderer2D::EndScene();

			RenderCommand::DisableStencilTest();
		}
	}

	void Scene::OnUpdateRuntime(Timestep ts)
	{
		// Update scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				// TODO: Move to Scene::OnScenePlay
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity(entity, this);
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		TransformComponent* mainCameraTc = nullptr;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					mainCamera = &camera.Cam;
					mainCameraTc = &transform;
					break;
				}
			}
		}

		if (mainCamera)
		{
			Renderer2D::BeginScene(*mainCamera, mainCameraTc->GetTransform());

			auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();
			for (auto entity : view)
			{
				auto [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);
				if (sprite.Texture != nullptr)
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Texture, sprite.Color, sprite.TilingFactor);
				else
					Renderer2D::DrawQuad(transform.GetTransform(), sprite.Color);
			}

			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize non-fixed aspect ratio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			CameraComponent& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				SceneCamera& cam = cameraComponent.Cam;
				cam.SetViewportSize(width, height);
			}
		}
	}

	const std::string& Scene::GetName() const
	{
		return m_SceneName;
	}

	void Scene::SetName(const std::string& sceneName)
	{
		m_SceneName = sceneName;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{
		static_assert(false, "Unknown component");
	}

	template<>
	void Scene::OnComponentAdded<TagComponent>(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(Entity entity, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(Entity entity, CameraComponent& component)
	{
		component.Cam.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(Entity entity, NativeScriptComponent& component)
	{

	}

}
