#pragma once

#include "Dominion/Core/Physics.h"

#include <glm/glm.hpp>

namespace Dominion {

	class BoxCollider2DComponent
	{
	public:
		const glm::vec2& GetCenter() const { return m_Center; }
		const glm::vec2& GetScale() const { return m_Scale; }
		physx::PxMaterial* GetPhysicsMaterial() const { return m_PhysicsMaterial; }
		physx::PxRigidDynamic* GetPhysicsActor() const { return m_PhysicsActor; }
		bool IsTrigger() const { return m_IsTrigger; }


	private:
		glm::vec2 m_Center{ 0.0f, 0.0f };
		glm::vec2 m_Scale{ 1.0f, 1.0f };
		physx::PxMaterial* m_PhysicsMaterial = Physics::GetPhysXPhysics()->createMaterial(0.5f, 0.5f, 0.1f);
		physx::PxRigidDynamic* m_PhysicsActor = nullptr;
		bool m_IsTrigger = false;
	};

}
