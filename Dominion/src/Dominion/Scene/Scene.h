#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Timestep.h"

#include "Dominion/Scene/Entity.h"

#include "Dominion/Physics/Physics.h"
#include "Dominion/Physics/PhysicsMaterial.h"

#pragma warning (disable: 4267 26439 26451 26495 28020)
#include <entt.hpp>
#pragma warning (default: 4267 26439 26451 26495 28020)

#include <string>
#include <deque>

namespace Dominion {

	// Forward declarations
	class EditorCamera;

	class Scene
	{
	public:
		Scene(const std::string& sceneName = "Unknown Scene Name");
		~Scene();

		Entity CreateEntity(const std::string& name = "Entity");
		void DestroyEntity(Entity entity);

		void OnUpdateEditor(Timestep timestep, const EditorCamera& camera, Entity selectedEntity);
		void OnUpdateRuntime(Timestep timestep);
		void OnViewportResize(uint32_t width, uint32_t height);

		const std::string& GetName() const;
		void SetName(const std::string& sceneName);

		physx::PxScene* GetPhysicsScene() { return m_PhysicsScene; }
		const physx::PxScene* GetPhysicsScene() const { return m_PhysicsScene; }
	private:
		struct InternalCollision
		{
			enum class Flag
			{
				COLLISION_START = 0,
				COLLISION_STAY,
				COLLISION_END
			};

			U32 entity1Index, entity2Index;
			Scene* scene;
			Flag flag;
		};

		class CollideCallback : public physx::PxSimulationEventCallback
		{
		public:
			std::deque<InternalCollision> collisions;
		private:
			virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override;
			virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override;
			virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override;
			virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override;
			virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override;
			virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override;
		};
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		std::string m_SceneName;
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		physx::PxDefaultCpuDispatcher* m_PhysicsCPUDispatcher = nullptr;
		physx::PxScene* m_PhysicsScene = nullptr;
		CollideCallback m_PhysicsSimulationEventCallback = CollideCallback();

		std::vector<PhysicsMaterial> m_PhysicsMaterials;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarhyPanel;
		friend class EditorLayer;
	};

}
