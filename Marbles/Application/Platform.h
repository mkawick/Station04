#pragma once

#if defined _WIN32
	#define PLATFORM_WIN32
//	#include <Application/Source/platform_win32.h>
//#elif defined _IOS
//	#define PLATFORM_IOS
//#elif defined _ANDROID
//	#define PLATFORM_ANDROID
#else
	#pragma Message("Platform error", "Unknown or unsupported platform defaulting to _WIN32.")
	#define PLATFORM_WIN32
#endif

#if defined _DEBUG
#define CONFIG_DEBUG
#elif defined _PROFILE
#define CONFIG_PROFILE
#elif defined _RETAIL
#define CONFIG_RETAIL
#else
	#pragma Message("Config error", "Unknown or unsupported configuration defaulting to _DEBUG.")
	#define PLATFORM_WIN32
#endif

namespace Marbles
{
	class Platform
	{
	public:
		static Platform* Get() { ASSERT(NULL != sInstance); return sInstance; }
	private:
		static Platform* sInstance;
	};
}
