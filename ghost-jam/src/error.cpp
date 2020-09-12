namespace internal
{
    GLOBAL constexpr const char* ERROR_LOG_NAME = "LOGS/ERROR.log";

    GLOBAL FILE* error_log;

    STDDEF void log_error (const char* _file, int _line, Error_Level _level, const char* _format, ...)
    {
        // We only open the error log once the first error occurs.
        if (!error_log) {
            std::string error_log_name(Catalog::build_resource_path(ERROR_LOG_NAME));
            create_path(strip_file_name(error_log_name.c_str()).c_str());
            error_log = fopen(error_log_name.c_str(), "a");
        }

        va_list args;

        // Make sure file slashes are consistent -- we prefer forward slashes;
        // and format the time so we can print it with the error in the log.
        std::string file_str = fix_path_slashes(_file);
        std::string time_str = format_time("%m/%d/%Y %H:%M:%S");

        const char* format = "[%s] Error in %s at line %d: ";

        // Writes the formatted error message to the error log file.
        va_start(args, _format);
        fprintf (error_log,  format, time_str.c_str(), file_str.c_str(), _line);
        vfprintf(error_log, _format, args);
        fprintf (error_log, "\n");
        va_end  (args);

        // In debug mode it is helpful to print to console/terminal.
        #if defined(DEBUG_BUILD)
        va_start(args, _format);
        fprintf (stderr,  format, time_str.c_str(), file_str.c_str(), _line);
        vfprintf(stderr, _format, args);
        fprintf (stderr, "\n");
        va_end  (args);
        #endif // DEBUG_BUILD

        if (_level == ERR_MIN) { return; }

        va_start(args, _format);
        std::string msg = format_string_v(_format, args);
        show_alert("Error!", msg.c_str(), ALERT_TYPE_ERROR, ALERT_BUTTON_OK, &get_window("WINMAIN"));
        va_end(args);

        if (_level == ERR_MED) { return; }

        main_running = false;
    }
}

FILDEF void quit_error_system ()
{
    // This condition is important because, for some reason, calling
    // fclose(NULL) results in a long hang-time during program exit.
    if (internal::error_log) {
        fclose(internal::error_log);
        internal::error_log = NULL;
    }
}
