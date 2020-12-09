#pragma once

#if defined(new)
#undef new
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#define new DEBUG_NEW
#else
#include "../imgui/imgui.h"
#include "../imgui/imgui_internal.h"
#endif
