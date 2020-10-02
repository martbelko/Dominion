#pragma once

#include "Dominion/Core/Base.h"
#include "Event.h"
#include "Dominion/Core/KeyCodes.h"

#include <sstream>

namespace Dominion {

	class KeyEvent : public Event
	{
	public:
		KeyEvent(unsigned int keyCode)
			: m_KeyCode(keyCode) {}

		unsigned int GetKeyCode() const { return m_KeyCode; }
	protected:
		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(unsigned int keyCode, unsigned int repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		unsigned int GetRepeatCount() const { return m_RepeatCount; }

		virtual std::string ToString() const override
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

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleased: " << m_KeyCode;
			return ss.str();
		}

		EVENT_TYPE(EventType::KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(unsigned int keyCode)
			: KeyEvent(keyCode) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTyped: " << m_KeyCode;
			return ss.str();
		}

		EVENT_TYPE(EventType::KeyTyped)
	};

}
