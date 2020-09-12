#define LOG_ERROR(...) ::internal::log_error(__FILE__, __LINE__, __VA_ARGS__)

FILDEF void quit_error_system ();

enum Error_Level
{
    ERR_MIN, // Log to file.
    ERR_MED, // Log to file, display alert.
    ERR_MAX  // Log to file, display alert, terminate.
};
