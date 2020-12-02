#pragma once

#include <chrono>

namespace Dominion {

	class Timer
	{
	public:
		Timer() = default;
		~Timer() = default;

		void Start() { m_LastTime = std::chrono::system_clock::now(); }
		float PeekNano();
		float PeekMicro();
		float PeekMili();
		float PeekSeconds();
	private:
		std::chrono::system_clock::time_point m_LastTime;
	};

}
