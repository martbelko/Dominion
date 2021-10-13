#pragma once

#pragma warning(push, 0) // Disable warnings for external header files
#include <PxPhysicsAPI.h>
#pragma warning (pop)

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
