#pragma once

#include "dmpch.h"

#if DM_INCLUDE_IMGUI == 1

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#if defined(new)
	#undef new
	#include <examples/imgui_impl_opengl3.cpp>
	#include <examples/imgui_impl_glfw.cpp>
	#define new DEBUG_NEW
#else
	#include <examples/imgui_impl_opengl3.cpp>
	#include <examples/imgui_impl_glfw.cpp>
#endif

#endif
