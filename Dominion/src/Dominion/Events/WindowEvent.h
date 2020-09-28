#pragma once

#include "Event.h"

namespace Dominion {

	class WindowEvent : public Event
	{
	public:
		WindowEvent(void* window)
			: m_Window(window) {}

		void* GetWindow() const { return m_Window; }
	private:
		void* m_Window;
	};

	class WindowCreatedEvent : public WindowEvent
	{
	public:
		WindowCreatedEvent(void* window, int x, int y, unsigned int width, unsigned int height)
			: WindowEvent(window), m_PosX(x), m_PosY(y), m_Width(width), m_Height(height) {}

		int GetX() const { return m_PosX; }
		int GetY() const { return m_PosY; }

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		virtual std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowCreated: Position: (" << m_PosX << "; " << m_PosY << ") / Size: (" << m_Width << "; " << m_Height << ')';
			return ss.str();
		}

		EVENT_TYPE(EventType::WindowCreated);
	private:
		int m_PosX, m_PosY;
		unsigned int m_Width, m_Height;
	};

	class WindowClosedEvent : public WindowEvent
	{
	public:
		WindowClosedEvent(void* window)
			: WindowEvent(window) {}

		virtual std::string ToString() const override
		{
			return "WindowClosed";
		}

		EVENT_TYPE(EventType::WindowClosed);
	};

	class WindowResizedEvent : public WindowEvent
	{
	public:
		WindowResizedEvent(void* window, unsigned int width, unsigned int height)
			: WindowEvent(window), m_Width(width), m_Height(height) {}

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		EVENT_TYPE(EventType::WindowResized);
	private:
		unsigned int m_Width, m_Height;
	};

	class WindowMovedEvent : public WindowEvent
	{
	public:
		WindowMovedEvent(void* window, int x, int y)
			: WindowEvent(window), m_PosX(x), m_PosY(y) {}

		int GetX() const { return m_PosX; }
		int GetY() const { return m_PosY; }

		EVENT_TYPE(EventType::WindowMoved);
	private:
		int m_PosX, m_PosY;
	};

	class WindowFocusEvent : public WindowEvent
	{
	public:
		WindowFocusEvent(void* window)
			: WindowEvent(window) {}

		EVENT_TYPE(EventType::WindowFocus);
	};

	class WindowFocusLostEvent : public WindowEvent
	{
	public:
		WindowFocusLostEvent(void* window)
			: WindowEvent(window) {}

		EVENT_TYPE(EventType::WindowLostFocus);

	};

}