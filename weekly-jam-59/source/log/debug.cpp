/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void DebugInit ()
{
	// DebugLog will be called in both main and hotloader threads, so it needs to be thread-safe.
	debug_lock = SDL_CreateMutex();
	if (!debug_lock) {
		TCE_ERROR_LOG(ERROR_LEVEL_MED, ERROR_TYPE_SDL, "Failed to create debug log mutex!");
	}
}

INLDEF void DebugQuit ()
{
	SDL_DestroyMutex(debug_lock);
	debug_lock = NULL;

	if (debug_log) {
		fclose(debug_log);
		debug_log = NULL;
	}
}

STDDEF void DebugLog (const char* _system, const char* _format, ...)
{
	// Do not bother if the user does not want debug logging.
	if (!debug_log_enabled) { return; }

	// Ensures that debug logging across multiple threads is safe.
	SDL_LockMutex(debug_lock);

	// Opens a stream to the debug log if it is not already open. This way the stream
	// is not continuously being closed and reopened if errors keep on occurring.
	if (!debug_log) { debug_log = fopen("debug.log", "w"); }

	va_list arguments;

	// Gets the current date + time and formats it as a string.
	time_t raw_time = time(NULL);
	char formatted_time[256]; // @IMPROVE: handle buffer size dynamically???
	strftime(formatted_time, sizeof(formatted_time)-1, "%d/%m/%Y %H:%M:%S", localtime(&raw_time));

	// Log debug information into the console.
	va_start(arguments, _format);
	fprintf(stdout, "\n[%s] %s: ", formatted_time, _system);
	vfprintf(stdout, _format, arguments);
	va_end(arguments);

	// Log debug information to the debug log file.
	va_start(arguments, _format);
	fprintf(debug_log, "[%s] %s: ", formatted_time, _system);
	vfprintf(debug_log, _format, arguments);
	fprintf(debug_log, "\n");
	va_end(arguments);

	SDL_UnlockMutex(debug_lock);
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
