#pragma once

#include "Log.h"
#include "Deltatime.h"


#define RKT_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

#ifdef RKT_DEBUG
	#define RKT_ASSERT(x, ...) { if(!(x)) { RKT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define RKT_CORE_ASSERT(x, ...) { if(!(x)) { RKT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define RKT_ASSERT(x, ...)
	#define RKT_CORE_ASSERT(x, ...)
#endif