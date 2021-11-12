#pragma once

#include "Commands.h"

#include <deque>

namespace Dominion {

	class CommandStack
	{
	public:
		CommandStack();
		CommandStack(const CommandStack&) = delete; // Block copying
		CommandStack(CommandStack&&) = default;

		CommandStack& operator=(const CommandStack&) = delete; // Block copying
		CommandStack& operator=(CommandStack&&) = default;

		~CommandStack();
	public:
		void PushCommand(Command* command);

		Command* GetNextCommand();
		Command* GetLastCommand();
	private:
		std::deque<Command*> mCommands;
		size_t mCurrentIndex = 0;
	};

}
