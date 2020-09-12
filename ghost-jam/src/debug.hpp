#define LOG_DEBUG(_fmt, ...) ::internal::log_debug(_fmt, __VA_ARGS__)

FILDEF void quit_debug_system ();

// Used to handle indentation of debug messages to the log.
FILDEF void begin_debug_section (const char* _name = NULL);
FILDEF void end_debug_section   ();
