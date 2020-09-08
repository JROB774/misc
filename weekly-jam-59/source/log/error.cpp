/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void ErrorInit ()
{
	// ErrorLog will be called in both main and hotloader threads, so it needs to be thread-safe.
	error_lock = SDL_CreateMutex();
	if (!error_lock) {
		TCE_ERROR_LOG(ERROR_LEVEL_MED, ERROR_TYPE_SDL, "Failed to create error log mutex!");
	}
}

INLDEF void ErrorQuit ()
{
	SDL_DestroyMutex(error_lock);
	error_lock = NULL;

	if (error_log) {
		fclose(error_log);
		error_log = NULL;
	}
}

STDDEF void ErrorLog (const char* _file, int _line, ErrorLevel _level, ErrorType _type, const char* _format, ...)
{
	// Ensures that error logging across multiple threads is safe.
	SDL_LockMutex(error_lock);

	// Opens a stream to the error log if it is not already open. This way the stream
	// is not continuously being closed and reopened if errors keep on occurring.
	if (!error_log) { error_log = fopen("error.log", "a"); }

	va_list arguments;

	// Caches the SDL error message info rather than calling the same function repeatedly.
	const char* sdl_error = (_type == ERROR_TYPE_SDL) ? SDL_GetError() : NULL;
	const char* mix_error = (_type == ERROR_TYPE_MIX) ? Mix_GetError() : NULL;

	// Gets the current date + time and formats it as a string.
	time_t raw_time = time(NULL);
	char formatted_time[256]; // @IMPROVE: handle buffer size dynamically???
	strftime(formatted_time, sizeof(formatted_time)-1, "%d/%m/%Y %H:%M:%S", localtime(&raw_time));

	// If we're in debug mode then we want to log errors to the console for convenience.
	#if defined(TCE_DEBUG)
	va_start(arguments, _format);
	fprintf(stderr, "\n[%s] Error in %s at line %d: ", formatted_time, _file, _line);
	vfprintf(stderr, _format, arguments);
	fprintf(stderr, " (%s | %s)", sdl_error, mix_error);
	va_end(arguments);
	#endif

	// Always log the error message to the 'error.log' file no matter what level.
	if (error_log) {
		va_start(arguments, _format);
		fprintf(error_log, "[%s] Error in %s at line %d: ", formatted_time, _file, _line);
		vfprintf(error_log, _format, arguments);
		fprintf(error_log, " (%s | %s)\n", sdl_error, mix_error);
		va_end(arguments);
	}

	// There is no need to carry out the extra functionality if it's just a minor error.
	if (_level < ERROR_LEVEL_MED) { SDL_UnlockMutex(error_lock); return; }

	// Displays an error message box to inform the user of the error.
	va_start(arguments, _format);
	int size = vsnprintf(NULL, 0, _format, arguments) + 1; // +1 is for the null-terminator.
	char* message = MALLOC(char, size);
	if (message) {
		message[size-1] = '\0';
		vsnprintf(message, size, _format, arguments);
		// Displays a message box with error information for the user.
		ShowMessageBox("Error!", message, MESSAGEBOX_TYPE_ERROR, MESSAGEBOX_BUTTON_OK);
		FREE(message);
	}
	va_end(arguments);

	// Terminates the applicartion if an error is fatal.
	if (_level >= ERROR_LEVEL_MAX) { running = false; }

	SDL_UnlockMutex(error_lock);
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
