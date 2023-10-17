#ifdef _WIN32
	#ifdef _WIN64
		#define DIX_PLATFORM_WINDOWS
	#else
		#error "Windows x64 platform is currently only supported!"
	#endif
#endif