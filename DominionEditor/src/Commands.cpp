#include "Commands.h"

#include "Dominion/Scene/Scene.h"

namespace Dominion {

	DuplicateEntityCommand::DuplicateEntityCommand(Entity srcEntity)
		: mSrcEntity(srcEntity)
	{
		DM_ASSERT(mSrcEntity, "Invalid source entity");
	}

	void DuplicateEntityCommand::Do()
	{
		DM_ASSERT(mSrcEntity, "Invalid source entity");
		mDstEntity = mSrcEntity.GetScene()->DuplicateEntity(mSrcEntity);
	}

	void DuplicateEntityCommand::Undo()
	{
		DM_ASSERT(mDstEntity, "Invalid destionation entity");
		mDstEntity.GetScene()->DestroyEntity(mDstEntity);
		mDstEntity = Entity();
	}

	AddEntityCommand::AddEntityCommand(const Ref<Scene>& scene, const std::string& name, UUID uuid)
		: mScene(scene), mUUID(uuid), mName(name)
	{
		DM_ASSERT(mScene, "Scene was nullptr");
	}

	void AddEntityCommand::Do()
	{
		DM_ASSERT(mScene, "Scene was nullptr");
		mEntity = mScene->CreateEntity(mUUID, mName);
	}

	void AddEntityCommand::Undo()
	{
		DM_ASSERT(mEntity, "Invalid entity");
		mEntity.GetScene()->DestroyEntity(mEntity);
	}

}
