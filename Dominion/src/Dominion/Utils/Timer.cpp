#include "dmpch.h"
#include "Timer.h"

#include <chrono>

namespace Dominion {

	float Timer::PeekNano()
	{
		auto end = std::chrono::system_clock::now();
		return static_cast<float>(std::chrono::duration_cast<std::chrono::nanoseconds>(end - m_LastTime).count());
	}

	float Timer::PeekMicro()
	{
		auto end = std::chrono::system_clock::now();
		return static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(end - m_LastTime).count());
	}

	float Timer::PeekMili()
	{
		auto end = std::chrono::system_clock::now();
		return static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(end - m_LastTime).count());
	}

	float Timer::PeekSeconds()
	{
		auto end = std::chrono::system_clock::now();
		return static_cast<float>(std::chrono::duration_cast<std::chrono::seconds>(end - m_LastTime).count());
	}

}
