#include "dmpch.h"
#include "Physics.h"

namespace Dominion {

	physx::PxFoundation* Physics::s_Foundation = nullptr;
	physx::PxPhysics* Physics::s_Physics = nullptr;
	physx::PxPvd* Physics::s_Pvd = nullptr;

	physx::PxDefaultAllocator Physics::s_Allocator;
	physx::PxDefaultErrorCallback Physics::s_ErrorCallback;

	void Physics::Init()
	{
		s_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, s_Allocator, s_ErrorCallback);

		s_Pvd = physx::PxCreatePvd(*s_Foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
		s_Pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);

		s_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *s_Foundation, physx::PxTolerancesScale(), true, s_Pvd);
	}

	void Physics::Shutdown()
	{
		if (s_Physics)
			s_Physics->release();

		if (s_Pvd)
		{
			physx::PxPvdTransport* transport = s_Pvd->getTransport();
			s_Pvd->release();
			if (transport)
				transport->release();
		}

		if (s_Foundation)
			s_Foundation->release();
	}

}
