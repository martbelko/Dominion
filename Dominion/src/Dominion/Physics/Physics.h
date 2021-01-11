#pragma once

#pragma warning (disable: 26115 26451 26495 26812 33010)
#include <PxPhysicsAPI.h>
#pragma warning (default: 26115 26451 26495 26812 33010)

namespace Dominion {

	class Physics
	{
	public:
		static void Init();
		static void Shutdown();

		static physx::PxPhysics* GetPhysXPhysics() { return s_Physics; }
	private:
		static physx::PxFoundation* s_Foundation;
		static physx::PxPhysics* s_Physics;
		static physx::PxPvd* s_Pvd;

		static physx::PxDefaultAllocator s_Allocator;
		static physx::PxDefaultErrorCallback s_ErrorCallback;
	};

}
