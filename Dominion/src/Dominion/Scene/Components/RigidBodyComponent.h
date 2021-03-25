#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Physics/Physics.h"

#include <glm/glm.hpp>

namespace Dominion {

	struct RigidBody2DComponent
	{
	public:
		float GetMass() const { return m_DynamicActor->getMass(); }
		void SetMass(float mass) { m_DynamicActor->setMass(mass); }

		void SetLinearVelocity(const glm::vec3& linearVelocity)
		{
			const physx::PxVec3* vel = reinterpret_cast<const physx::PxVec3*>(&linearVelocity);
			m_DynamicActor->setLinearVelocity(*vel);
		}

		glm::vec3 GetLinearVelocity() const { return *reinterpret_cast<glm::vec3*>(&m_DynamicActor->getLinearVelocity()); }

		void EnableGravity(bool enableGravity) { m_DynamicActor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !enableGravity); }

		void SetMassSpaceInertiaTensor(const glm::vec3& inertiaTensor)
		{
			const physx::PxVec3* tensor = reinterpret_cast<const physx::PxVec3*>(&inertiaTensor);
			m_DynamicActor->setMassSpaceInertiaTensor(*tensor);
		}

		void SetKinematic(bool isKinematic) { m_DynamicActor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, isKinematic); }
	private:
		physx::PxRigidDynamic* m_DynamicActor;

		friend class Scene;
	};

}
