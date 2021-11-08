#pragma once

#include <Dominion/Scene/Entity.h>

namespace Dominion {

	class Command
	{
	public:
		virtual void Do() = 0;
		virtual void Undo() = 0;
	};

	class NullCommand final : public Command
	{
	public:
		virtual void Do() override { };
		virtual void Undo() override { };
	};

	class DuplicateEntityCommand : public Command
	{
	public:
		DuplicateEntityCommand(Entity srcEntity);

		virtual void Do() override;
		virtual void Undo() override;
	private:
		Entity mSrcEntity;
		Entity mDstEntity;
	};

}
