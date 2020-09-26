#pragma once

namespace Dominion {

	enum class EventType
	{
		None = 0,
		WindowCreated, WindowClosed, WindowResized, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MousePressed, MouseReleased, MouseMoved, MouseScrolled
	};

	#define EVENT_TYPE(type)    static EventType GetStaticType()    { return type; } \
                                virtual EventType GetType() const   { return type; }

	class Event
	{
	public:
		Event() = default;
		~Event() = default;

		void Handled() { m_Handled = true; }
		bool IsHandled() const { return m_Handled; }

		virtual EventType GetType() const = 0;

		template <typename T, typename F>
		void Dispatch(F f)
		{
			if (T::GetStaticType() == GetType())
				f(static_cast<T&>(*this));
		}
	private:
		bool m_Handled = false;
	};

}
