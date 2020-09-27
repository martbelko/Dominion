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
                                virtual EventType GetType() const   { return type; } \
                                virtual const char* GetName() const override { return #type; }

	class Event
	{
	public:
		Event() = default;
		~Event() = default;

		void Handled() { m_Handled = true; }
		bool IsHandled() const { return m_Handled; }

		virtual EventType GetType() const = 0;
		virtual const char* GetName() const = 0;
		virtual std::string ToString() const { return GetName(); }

		template <typename T, typename F>
		bool Dispatch(F f)
		{
			if (T::GetStaticType() == GetType())
			{
				m_Handled = f(static_cast<T&>(*this));
				return true;
			}

			return false;
		}
	private:
		bool m_Handled = false;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}
