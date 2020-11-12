#include "dmpch.h"
#include "Scene.h"

#include "Dominion/Scene/Components.h"
#include "Dominion/Scene/Entity.h"
#include "Dominion/Renderer/Renderer2D.h"

#include <glm/glm.hpp>

#include <string_view>

namespace Dominion {

	Scene::Scene()
	{
		entt::entity entity = m_Registry.create();

		m_Registry.emplace<TransformComponent>(entity);

		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& t = view.get<TransformComponent>(entity);
		}
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
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TagComponent>(name);
		entity.AddComponent<TransformComponent>();
		return entity;
	}

	void Scene::OnUpdate(Timestep timestep)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}

}
