#pragma once

namespace Dominion {

	class Timestep
	{
	public:
		Timestep(F32 time = 0.0f)
			: m_Time(time)
		{
		}

		operator F32() const { return GetSeconds(); }

		F32 GetSeconds() const { return m_Time / 1000000000.0f; }
		F32 GetMiliseconds() const { return m_Time / 1000000.0f; }
		F32 GetMicroseconds() const { return m_Time / 1000.0f; }
		F32 GetNanoseconds() const { return m_Time; }

		F32 GetFPS() const { return 1000000000.0f / m_Time; }
	private:
		F32 m_Time; // In nanoseconds
	};

}
