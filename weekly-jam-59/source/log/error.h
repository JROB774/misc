/*******************************************************************************
 * ERROR
 * Provides the functionality to log errors and report them to the user.
 *
*******************************************************************************/

#ifndef __TCE_ERROR_H__ /*////////////////////////////////////////////////////*/
#define __TCE_ERROR_H__

namespace TCE
{

enum ErrorLevel
{
	ERROR_LEVEL_MIN, // Logs the error to a file.
	ERROR_LEVEL_MED, // Logs the error to a file, and displays a message box.
	ERROR_LEVEL_MAX  // Logs the error to a file, displays a message box, and terminates.
};

enum ErrorType
{
	ERROR_TYPE_STD,  // Standard program error.
	ERROR_TYPE_SDL,  // An SDL-based error.
	ERROR_TYPE_MIX   // An SDL_Mixer-based error.
};

GLOBAL FILE* error_log;
GLOBAL SDL_mutex* error_lock;

INLDEF void ErrorInit ();
INLDEF void ErrorQuit ();

// Never call the ErrorLog function, only the macro as it handles specific arguments!
STDDEF void ErrorLog (const char* _file, int _line, ErrorLevel _level, ErrorType _type, const char* _format, ...);
#define TCE_ERROR_LOG(...) TCE::ErrorLog(__FILE__, __LINE__, __VA_ARGS__)

} // TCE

#endif /* __TCE_ERROR_H__ ////////////////////////////////////////////////////*/
