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
		WindowCreatedEvent(void* window, float x, float y, unsigned int width, unsigned int height)
			: WindowEvent(window), m_PosX(x), m_PosY(y), m_Width(width), m_Height(height) {}

		float GetX() const { return m_PosX; }
		float GetY() const { return m_PosY; }

		unsigned int GetWidth() const { return m_Width; }
		unsigned int GetHeight() const { return m_Height; }

		EVENT_TYPE(EventType::WindowCreated);
	private:
		float m_PosX, m_PosY;
		unsigned int m_Width, m_Height;
	};

	class WindowClosedEvent : public WindowEvent
	{
	public:
		WindowClosedEvent(void* window)
			: WindowEvent(window) {}

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
		WindowMovedEvent(void* window, float x, float y)
			: WindowEvent(window), m_PosX(x), m_PosY(y) {}

		float GetX() const { return m_PosX; }
		float GetY() const { return m_PosY; }

		EVENT_TYPE(EventType::WindowMoved);
	private:
		float m_PosX, m_PosY;
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