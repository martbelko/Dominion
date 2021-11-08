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

}
