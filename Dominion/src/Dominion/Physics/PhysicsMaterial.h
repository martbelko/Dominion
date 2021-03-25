#pragma once

#include "Dominion/Core/Base.h"

#include "Dominion/Physics/Physics.h"

namespace Dominion {

	class PhysicsMaterial
	{
	public:
		PhysicsMaterial(float staticFriction = 0.5f, float dynamicFriction = 0.5f, float restitution = 0.5f)
		{
			m_PhysicsMaterial = Physics::GetPhysXPhysics()->createMaterial(staticFriction, dynamicFriction, restitution);
		}

		PhysicsMaterial(const PhysicsMaterial& other)
		{
			m_PhysicsMaterial = other.m_PhysicsMaterial;
			m_PhysicsMaterial->acquireReference();
		}

		PhysicsMaterial(PhysicsMaterial&& other) noexcept
		{
			// TODO: Check for this assert
			DM_CORE_ASSERT(false, "PhysicsMaterial(PhysicsMaterial&& other) invoked!!!");
			m_PhysicsMaterial = other.m_PhysicsMaterial;
			m_PhysicsMaterial->acquireReference();
		}

		PhysicsMaterial& operator=(const PhysicsMaterial& other)
		{
			m_PhysicsMaterial = other.m_PhysicsMaterial;
			m_PhysicsMaterial->acquireReference();
		}

		~PhysicsMaterial()
		{
			uint32_t r = m_PhysicsMaterial->getReferenceCount();
			m_PhysicsMaterial->release();
		}

		float GetStaticFriction() const { return m_PhysicsMaterial->getStaticFriction(); }
		void SetStaticFriction(float staticFriction) { m_PhysicsMaterial->setStaticFriction(staticFriction); }

		float GetDynamicFriction() const { return m_PhysicsMaterial->getDynamicFriction(); }
		void SetDynamicFriction(float dynamicFriction) { m_PhysicsMaterial->setDynamicFriction(dynamicFriction); }

		float GetRestitution() const { return m_PhysicsMaterial->getRestitution(); }
		void SetRestitution(float restitution) { return m_PhysicsMaterial->setRestitution(restitution); }

		physx::PxMaterial* GetPhysxMaterial() const { return m_PhysicsMaterial; }
	private:
		physx::PxMaterial* m_PhysicsMaterial;
	};

}
