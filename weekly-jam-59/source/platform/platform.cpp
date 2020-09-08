/*////////////////////////////////////////////////////////////////////////////*/

#if   defined(TCE_PLATFORM_WIN32)
#include "win32.cpp"
#include "hotload_win32.cpp"
#elif defined(TCE_PLATFORM_MAC)
#error Mac OS is not supported!
#elif defined(TCE_PLATFORM_LINUX)
#error "Linux is not supported!"
#endif

/*////////////////////////////////////////////////////////////////////////////*/
