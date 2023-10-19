#pragma once

namespace Dix
{
	class Profiler
	{
	public:
		Profiler() {}
		~Profiler() {}
	};
}

#ifdef DIX_DEBUG
	#define DIX_PROFILE(...)
#else
	DIX_PROFILE(...)
#endif