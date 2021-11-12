#include "CommandStack.h"

namespace Dominion {

	CommandStack::CommandStack()
		: mCurrentIndex(0)
	{
		mCommands.push_back(new NullCommand());
	}

	CommandStack::~CommandStack()
	{
		for (Command* command : mCommands)
			delete command;
	}

	void CommandStack::PushCommand(Command* command)
	{
		if (mCurrentIndex == mCommands.size() - 1)
		{
			mCommands.push_back(command);
			++mCurrentIndex;
		}
		else
		{
			for (size_t i = mCurrentIndex + 1; i < mCommands.size(); ++i)
				delete mCommands[i];

			mCommands.resize(mCurrentIndex + 1);
			mCommands.push_back(command);
			++mCurrentIndex;
		}
	}

	Command* CommandStack::GetNextCommand()
	{
		if (mCurrentIndex + 1 < mCommands.size())
			return mCommands[++mCurrentIndex];

		return mCommands[0];
	}

	Command* CommandStack::GetLastCommand()
	{
		if (mCurrentIndex > 0)
			return mCommands[mCurrentIndex--];

		return mCommands[0];
	}

}
