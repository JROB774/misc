namespace Error
{
	enum Priority
	{
		PRIORITY_LOG, // logs an error message, no message box
		PRIORITY_LOW, // displays an error message box
		PRIORITY_MED, // displays an error message box and logs
		PRIORITY_MAX  // no message box, logs, and crashes due to fatality
	};

	enum Type
	{
		TYPE_STD, // standard/generic errors
		TYPE_SDL, // sdl related errors
		TYPE_TTF, // sdl ttf related errors
		TYPE_GPU  // sdl gpu related errors
	};
	
	namespace
	{
		constexpr const char* LOG_FILENAME = "error.log";

		bool rethrow = false;
		bool errorlogged = false;
	}

	void Terminate ()
	{
		// ensure errors are logged to the executable directory
		Platform::SetWorkingDirectory();

		// attempts to find some information about the unknown exception
		std::string expinfo = "?";
		if (!errorlogged && !rethrow) {
			try { rethrow = true; throw; }
			catch (std::exception& _exp) { expinfo = _exp.what(); }
			catch (...) { /* NOTHING */ }
		}

		// gets the current real-time
		std::time_t rawtime = std::time(nullptr);
		std::tm curtime = *std::localtime(&rawtime);

		// opens the log file for writing a stacktrace
		std::ofstream file;
		file.exceptions(std::ofstream::failbit|std::ofstream::badbit);
		file.open(LOG_FILENAME, std::ofstream::app);

		// logs that an unknown exception occurred if no error was logged
		if (!errorlogged) {
			file << std::put_time(&curtime, "(%d/%m/%Y %H:%M:%S) | ");
			if (expinfo != "?") { file << "Exception occurred: " << expinfo << "\n"; }
			else { file << "An unknown exception occurred!\n"; }
		}

		// logs the current stack for error tracking
		file << "Application Stacktrace Dump:\n";
		Platform::Stacktrace(file);
		file << "\n";
		// closes the file after use
		file.close();

		// aborts the program (potentially unwinds stack)
		std::abort();
	}

	void __log__ (Priority _priority, Type _type, std::string _message, const char* _file, int _line)
	{
		// displays an error message regarding the error.
		if (_priority != PRIORITY_LOG) { Platform::MessageBox(Platform::MSGBOX_TYPE_OK, "Error", _message); }
		// stops here if its just a low priority error
		if (_priority == PRIORITY_LOW) { return; }

		// ensure errors are logged to the executable directory
		Platform::SetWorkingDirectory();
		
		// get the current real-time
		std::time_t rawtime = std::time(nullptr);
		std::tm curtime = *std::localtime(&rawtime);

		// format the time and message into the output format
		std::ostringstream message;
		message << std::put_time(&curtime, "(%d/%m/%Y %H:%M:%S) | Error at ") \
		<< _file << " line " << _line << ": " << _message;

		// append additional library error information
		switch (_type) {
			case (TYPE_SDL): { message << " (" << SDL_GetError() << ")"; break; }
			case (TYPE_TTF): { message << " (" << TTF_GetError() << ")"; break; }
			case (TYPE_GPU): {
				char* errordetails = GPU_PopErrorCode().details;
				if (errordetails) { message << " (" << errordetails << ")"; }
				break;
			}
			// prevents compiler warnings
			default: { break; }
		}

		// open the log file and write the formatted error contents
		std::ofstream file;
		file.exceptions(std::ofstream::failbit|std::ofstream::badbit);
		file.open(LOG_FILENAME, std::ofstream::app);
		file << message.str() << "\n";
		file.close();

		// stops here if it was just a medium priority error
		if (_priority == PRIORITY_MED || PRIORITY_LOG) { return; }

		// informs the terminate function that an error was logged
		errorlogged = true;
		// terminates application gracefully
		std::terminate();
	}
}

#define ERROR_LOG(_priority, _type, _message) Error::__log__(_priority, _type, _message, __FILE__, __LINE__)