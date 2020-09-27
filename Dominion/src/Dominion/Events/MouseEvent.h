#pragma once

#include "Event.h"

#include <string>
#include <sstream>

namespace Dominion {

	class MouseButtonEvent : public Event
	{
	public:
		MouseButtonEvent(unsigned int button)
			: m_Button(button) {}

		unsigned int GetButton() const { return m_Button; }
	protected:
		unsigned int m_Button;
	};

	class MousePressedEvent : public MouseButtonEvent
	{
	public:
		MousePressedEvent(unsigned int button)
			: MouseButtonEvent(button) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MousePressedEvent Button: " << m_Button;
			return ss.str();
		}

		EVENT_TYPE(EventType::MousePressed)
	};

	class MouseReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseReleasedEvent(unsigned int button)
			: MouseButtonEvent(button) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseReleasedEvent Button: " << m_Button;
			return ss.str();
		}

		EVENT_TYPE(EventType::MouseReleased)
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(float x, float y)
			: m_MouseX(x), m_MouseY(y) {}

		float GetX() const { return m_MouseX; }
		float GetY() const { return m_MouseY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent (" << m_MouseX << "; " << m_MouseY << ')';
			return ss.str();
		}

		EVENT_TYPE(EventType::MouseMoved)
	private:
		float m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			: m_OffsetX(xOffset), m_OffsetY(yOffset) {}

		float GetXOffset() const { return m_OffsetX; }
		float GetYOffset() const { return m_OffsetY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent (" << m_OffsetX << "; " << m_OffsetY << ')';
			return ss.str();
		}

		EVENT_TYPE(EventType::MouseScrolled)
	private:
		float m_OffsetX, m_OffsetY;
	};

}