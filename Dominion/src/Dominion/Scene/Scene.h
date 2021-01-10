#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Timestep.h"
#include "Dominion/Core/Physics.h"

#pragma warning (disable: 4267 26439 26495)
#include <entt.hpp>
#pragma warning (default: 4267 26439 26495)

#include <string>
#include <deque>

namespace Dominion {

	// Forward declarations
	class Entity;
	class EditorCamera;

	class Scene
	{
	public:
		Scene(const std::string& sceneName = "Unknown Scene Name");
		~Scene();

		Entity CreateEntity();
		Entity CreateEntity(const std::string& name);

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
			U32 entity1Index, entity2Index;
			Scene* scene;
		};

		class CollideCallback : public physx::PxSimulationEventCallback
		{
		public:
			std::deque<InternalCollision> collisions;
		private:
			virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) override
			{
			}

			virtual void onWake(physx::PxActor** actors, physx::PxU32 count) override
			{
			}

			virtual void onSleep(physx::PxActor** actors, physx::PxU32 count) override
			{
			}

			virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs) override
			{
				Dominion::Scene* scene = static_cast<Dominion::Scene*>(pairHeader.actors[0]->getScene()->userData);
				U32 ent1Index = reinterpret_cast<U32>(pairHeader.actors[0]->userData);
				U32 ent2Index = reinterpret_cast<U32>(pairHeader.actors[1]->userData);

				InternalCollision collision;
				collision.entity1Index = ent1Index;
				collision.entity2Index = ent2Index;
				collision.scene = scene;

				collisions.emplace_back(collision);

				const physx::PxContactPair& cp = *pairs;
				if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
				{
					int x = 5;
				}
				if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
				{
					int x = 5;
				}
				if (pairs->events & physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
				{
					int x = 5;
				}
			}

			virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) override
			{
			}

			virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count) override
			{
			}
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

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarhyPanel;
		friend class EditorLayer;
	};

}
