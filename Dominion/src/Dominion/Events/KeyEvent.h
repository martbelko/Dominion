#pragma once

#include "Dominion/Core/Base.h"
#include "Event.h"
#include "Dominion/Core/KeyCodes.h"

#include <sstream>

namespace Dominion {

	class KeyEvent : public Event
	{
	public:
		KeyEvent(uint32_t keyCode)
			: m_KeyCode(keyCode) {}

		uint32_t GetKeyCode() const { return m_KeyCode; }
	protected:
		KeyCode m_KeyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
		KeyPressedEvent(uint32_t keyCode, uint32_t repeatCount)
			: KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

		uint32_t GetRepeatCount() const { return m_RepeatCount; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyPressed: " << m_KeyCode << " (" << m_RepeatCount << ')';
			return ss.str();
		}

		EVENT_TYPE(EventType::KeyPressed)
	private:
		uint32_t m_RepeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(uint32_t keyCode)
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
		KeyTypedEvent(uint32_t keyCode)
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
