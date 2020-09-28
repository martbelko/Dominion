#pragma once

#if defined(DM_DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW
#endif

#include "dmpch.h"
#include "Dominion/Core/Base.h"
#include "Dominion/Core/Log.h"
#include "Dominion/Core/Application.h"
#include "Dominion/Events/Event.h"
#include "Dominion/Events/KeyEvent.h"
#include "Dominion/Events/MouseEvent.h"
#include "Dominion/Events/WindowEvent.h"
#include "Dominion/Core/Window.h"
#include "Dominion/Core/Layer.h"
#include "Dominion/Core/LayerStack.h"
#include "Dominion/Core/Input.h"

#include "Dominion/Core/EntryPoint.h"
