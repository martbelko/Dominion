#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Timestep.h"

#include "Dominion/Scene/Components.h"

#pragma warning (disable: 4267 26439 26495)
#include <entt.hpp>
#pragma warning (default: 4267 26439 26495)

#include <string>

namespace Dominion {

	class Entity;

	class Scene
	{
	public:
		Scene(const std::string& sceneName = "Unknown Scene Name");
		~Scene();

		Entity CreateEntity();
		Entity CreateEntity(const std::string& name);

		void DestroyEntity(Entity entity);

		void OnUpdate(Timestep timestep);
		void OnViewportResize(uint32_t width, uint32_t height);

		const std::string& GetName() const;
		void SetName(const std::string& sceneName);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		std::string m_SceneName;
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarhyPanel;
	};

}
