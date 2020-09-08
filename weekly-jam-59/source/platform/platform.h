/*******************************************************************************
 * PLATFORM
 * Determines which platform we're on and loads the specific platform code.
 *
*******************************************************************************/

#ifndef __TCE_PLATFORM_H__ /*/////////////////////////////////////////////////*/
#define __TCE_PLATFORM_H__

#if   defined(TCE_PLATFORM_WIN32)
#include "win32.h"
#elif defined(TCE_PLATFORM_MAC)
#error "Mac OS is not supported!"
#elif defined(TCE_PLATFORM_LINUX)
#error "Linux is not supported!"
#endif

// The hotloading source code files have been placed in with the platform code
// because they are so closely tied to platform-specific API features that it
// just makes it easier to implement this way. Rather than desigining a fully
// functional platform layer, with specific function utilities, which would be
// overcompilcating the code for an engine of this size and scope.

#include "hotload.h"

#endif /* __TCE_PLATFORM_H__ /////////////////////////////////////////////////*/
