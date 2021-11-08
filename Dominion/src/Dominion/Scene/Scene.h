#pragma once

#include "Dominion/Core/Timestep.h"
#include "Dominion/Core/UUID.h"
#include "Dominion/Renderer/EditorCamera.h"

#include "entt.hpp"

class b2World;

namespace Dominion {

	class Entity;

	class Scene
	{
	public:
		Scene();
		Scene(Scene& other);
		Scene(Scene&& other) = delete; // TODO: Implement

		Scene& operator=(const Scene& other);
		Scene& operator=(Scene&& other) = delete; // TODO: Implement

		~Scene();
	public:
		Entity CreateEntity(const std::string& name = "");
		Entity CreateEntity(UUID uuid, const std::string& name = "");
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();

		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry mRegistry;
		uint32_t mViewportWidth = 0, mViewportHeight = 0;

		b2World* mPhysics2DWorld = nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
