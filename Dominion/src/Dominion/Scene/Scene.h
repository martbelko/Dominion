#pragma once

#include "Dominion/Core/Timestep.h"
#include "Dominion/Core/UUID.h"

#include "Dominion/Renderer/EditorCamera.h"

#include "Dominion/Scene/Components.h"

#include "entt.hpp"

class b2World;

namespace Dominion {

	class Entity;
	struct CameraComponent;

	template<typename T>
	concept CameraConcept = std::same_as<T, CameraComponent>;

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
		void OnUpdateEditor(Timestep ts, const Camera& camera, const glm::mat4& transform);
		void OnViewportResize(uint32_t width, uint32_t height);

		void Render(const EditorCamera& editorCamera);
		void Render(const Camera& camera, const glm::mat4& cameraTransform);

		Entity DuplicateEntity(Entity entity);

		Entity GetPrimaryCameraEntity();

		template<typename... Component>
		auto GetAllEntitiesWith()
		{
			return mRegistry.view<Component...>();
		}
	private:
		void RenderInternal();

		template<typename T>
		void OnComponentAdded(Entity entity, T& component)
		{
		}

		template<typename T> requires std::same_as<T, CameraComponent>
		void OnComponentAdded(Entity entity, CameraComponent& component)
		{
			if (mViewportWidth > 0 && mViewportHeight > 0)
				component.camera.SetViewportSize(mViewportWidth, mViewportHeight);
		}
	private:
		entt::registry mRegistry;
		uint32_t mViewportWidth = 0, mViewportHeight = 0;

		b2World* mPhysics2DWorld = nullptr;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};

}
