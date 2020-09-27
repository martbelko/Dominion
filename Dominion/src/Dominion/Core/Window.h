#pragma once

#include "Dominion/Events/Event.h"

#include <string>
#include <string_view>
#include <functional>

namespace Dominion {

	class WindowProps
	{
	public:
		WindowProps(unsigned int width = 1280, unsigned int height = 720, std::string_view title = "Dominion Engine")
			: width(width), height(height), title(title) {}
	public:
		unsigned int width, height;
		std::string title;
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
	public:
		Window() = default;
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}