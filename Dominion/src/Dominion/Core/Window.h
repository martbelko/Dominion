#pragma once

#include "Dominion/Core/Base.h"
#include "Dominion/Events/Event.h"

#include <string>
#include <string_view>
#include <vector>
#include <functional>

namespace Dominion {

	struct WindowProps
	{
		std::string Title = "Dominion App";
		unsigned int Width = 1600, Height = 900;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
	public:
		Window() = default;
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual int GetPosX() const = 0;
		virtual int GetPosY() const = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void Close() = 0;

		static Window* Create(const EventCallbackFn& callback, const WindowProps& props = WindowProps());
	protected:
		static std::vector<Window*> s_Windows;
	};

}
