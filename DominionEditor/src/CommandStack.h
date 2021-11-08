#pragma once

#include "Commands.h"

#include <deque>

namespace Dominion {

	class CommandStack
	{
	public:
		CommandStack();
		// TODO: Rule of 5

		void PushCommand(Command* command);

		Command* GetNextCommand();
		Command* GetLastCommand();
	private:
		std::deque<Command*> mCommands;
		size_t mCurrentIndex = 0;
	};

}
