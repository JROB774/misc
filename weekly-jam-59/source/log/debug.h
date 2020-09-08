/*******************************************************************************
 * DEBUG
 * Provides the functionality to log debug information for debugging/testing.
 *
*******************************************************************************/

#ifndef __TCE_DEBUG_H__ /*////////////////////////////////////////////////////*/
#define __TCE_DEBUG_H__

namespace TCE
{

// Whether or not debug logging is enabled is determined by a variable in the tweak file.
// This needs a default value in case the variable tweak value cannot be loaded!
GLOBAL bool debug_log_enabled = false;

GLOBAL FILE* debug_log;
GLOBAL SDL_mutex* debug_lock;

INLDEF void DebugInit ();
INLDEF void DebugQuit ();

// Never call the DebugLog function, only the macro as it handles disabling the function!
STDDEF void DebugLog (const char* _system, const char* _format, ...);

#if defined(TCE_DEBUG)
#define TCE_DEBUG_LOG(...) DebugLog(__VA_ARGS__)
#else
#define TCE_DEBUG_LOG(...)
#endif

} // TCE

#endif /* __TCE_DEBUG_H__ ////////////////////////////////////////////////////*/
