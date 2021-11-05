#pragma once

#include "Dominion/Core/PlatformDetection.h"

#ifdef DM_PLATFORM_WINDOWS
	#ifndef NOMINMAX
		// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <fstream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <filesystem>

#include <string>
#include <sstream>
#include <array>
#include <queue>
#include <deque>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

#include "Dominion/Core/Base.h"
#include "Dominion/Core/Log.h"
#include "Dominion/Debug/Instrumentor.h"

#ifdef DM_PLATFORM_WINDOWS
	#include <Windows.h>
#endif
