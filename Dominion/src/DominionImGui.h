#pragma once

#if defined(new)
	#undef new
	#pragma warning (disable: 26495 26812)
	#include "../imgui/imgui.h"
	#include "../imgui/imgui_internal.h"
	#pragma warning (default: 26945 26812)
	#define new DEBUG_NEW
#else
	#pragma warning (disable: 26495 26812)
	#include "../imgui/imgui.h"
	#include "../imgui/imgui_internal.h"
	#pragma warning (default: 26945 26812)
#endif
