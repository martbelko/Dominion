#include "Commands.h"

#include "Dominion/Scene/Scene.h"

namespace Dominion {

	DuplicateEntityCommand::DuplicateEntityCommand(Entity srcEntity)
		: mSrcEntity(srcEntity)
	{
	}

	void DuplicateEntityCommand::Do()
	{
		mDstEntity = mSrcEntity.GetScene()->DuplicateEntity(mSrcEntity);
	}

	void DuplicateEntityCommand::Undo()
	{
		mDstEntity.GetScene()->DestroyEntity(mDstEntity);
		mDstEntity = Entity();
	}

	AddEntityCommand::AddEntityCommand(Ref<Scene>& scene, const std::string& name, UUID uuid)
		: mScene(scene), mUUID(uuid), mName(name)
	{
	}

	void AddEntityCommand::Do()
	{
		mEntity = mScene->CreateEntity(mUUID, mName);
	}

	void AddEntityCommand::Undo()
	{
		mEntity.GetScene()->DestroyEntity(mEntity);
	}

}
