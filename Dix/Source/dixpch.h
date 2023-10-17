#pragma once

#include "Dix/Core/PlatformDetection.h"

//#ifdef DIX_PLATFORM_WINDOWS
//	#ifndef NOMINMAX
//	// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
//		#define NOMINMAX
//	#endif
//#endif

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <string>
#include <sstream>
#include <array>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Dix/Core/Base.h"

#include "Dix/Core/Log.h"

//#include "Dix/Debug/Instrumentor.h"

//#ifdef DIX_PLATFORM_WINDOWS
//	#include <Windows.h>
//#endif