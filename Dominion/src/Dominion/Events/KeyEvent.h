#pragma once

#include "Event.h"

namespace Dominion {

	class KeyEvent : public Event
	{
	public:
		KeyEvent(unsigned int keyCode)
			: m_KeyCode(keyCode) {}

		unsigned int GetKeyCode() const { return m_KeyCode; }
	protected:
		unsigned int m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(unsigned int keyCode, unsigned int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		unsigned int GetRepeatCount() const { return m_RepeatCount; }

		std::string Tostring() const
		{
			std::stringstream ss;
			ss << "KeyPressed: " << m_KeyCode << " (" << m_RepeatCount << ')';
			return ss.str();
		}

		EVENT_TYPE(EventType::KeyPressed)
	private:
		unsigned int m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(unsigned int keyCode)
			: KeyEvent(keyCode) {}

		std::string Tostring() const
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_TYPE(EventType::KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(unsigned int keyCode)
			: KeyEvent(keyCode) {}

		std::string Tostring() const
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_TYPE(EventType::KeyTyped)
	};

}