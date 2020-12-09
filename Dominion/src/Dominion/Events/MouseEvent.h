#pragma once

#include "Dominion/Core/Base.h"
#include "Event.h"
#include "Dominion/Core/MouseCodes.h"

#include <string>
#include <sstream>

namespace Dominion {

	class MouseButtonEvent : public Event
	{
	public:
		MouseButtonEvent(uint32_t button)
			: m_Button(button) {}

		uint32_t GetButton() const { return m_Button; }
	protected:
		MouseCode m_Button;
	};

	class MousePressedEvent : public MouseButtonEvent
	{
	public:
		MousePressedEvent(uint32_t button)
			: MouseButtonEvent(button) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MousePressed Button: " << m_Button;
			return ss.str();
		}

		EVENT_TYPE(EventType::MousePressed)
	};

	class MouseReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseReleasedEvent(uint32_t button)
			: MouseButtonEvent(button) {}

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseReleased Button: " << m_Button;
			return ss.str();
		}

		EVENT_TYPE(EventType::MouseReleased)
	};

	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(F32 x, F32 y)
			: m_MouseX(x), m_MouseY(y) {}

		F32 GetX() const { return m_MouseX; }
		F32 GetY() const { return m_MouseY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMoved (" << m_MouseX << "; " << m_MouseY << ')';
			return ss.str();
		}

		EVENT_TYPE(EventType::MouseMoved)
	private:
		F32 m_MouseX, m_MouseY;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(F32 xOffset, F32 yOffset)
			: m_OffsetX(xOffset), m_OffsetY(yOffset) {}

		F32 GetXOffset() const { return m_OffsetX; }
		F32 GetYOffset() const { return m_OffsetY; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolled (" << m_OffsetX << "; " << m_OffsetY << ')';
			return ss.str();
		}

		EVENT_TYPE(EventType::MouseScrolled)
	private:
		F32 m_OffsetX, m_OffsetY;
	};

}
