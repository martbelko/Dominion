#pragma once

namespace Dominion {

	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_Time(time)
		{
		}

		operator float() const { return GetSeconds(); }

		float GetSeconds() const { return m_Time / 1000000000.0f; }
		float GetMiliseconds() const { return m_Time / 1000000.0f; }
		float GetMicroseconds() const { return m_Time / 1000.0f; }
		float GetNanoseconds() const { return m_Time; }

		float GetFPS() const { return 1000000000.0f / m_Time; }
	private:
		float m_Time; // In nanoseconds
	};

}
