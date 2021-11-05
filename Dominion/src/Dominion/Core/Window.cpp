#include "dmpch.h"
#include "Dominion/Core/Window.h"

#ifdef DM_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace Dominion {

	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef DM_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		DM_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}

}
