namespace internal
{
    GLOBAL constexpr const char* DEBUG_LOG_NAME = "LOGS/DEBUG.log";

    GLOBAL int   current_debug_section = 0;
    GLOBAL FILE* debug_log;

    STDDEF void log_debug (const char* _format, ...)
    {
        // We only open the debug log once the first debug occurs.
        if (!debug_log) {
            std::string debug_log_name(Catalog::build_resource_path(DEBUG_LOG_NAME));
            create_path(strip_file_name(debug_log_name.c_str()).c_str());
            debug_log = fopen(debug_log_name.c_str(), "w");
            LOG_DEBUG("DEBUG LOG [%s]", format_time("%m/%d/%Y %H:%M:%S").c_str());
        }

        va_list args;

        // Writes the formatted debug message to the debug log file.
        if (debug_log) {
            va_start(args, _format);
            for (int i=0; i<current_debug_section; ++i) {
                fprintf(debug_log, "  ");
            }
            vfprintf(debug_log, _format, args);
            fprintf(debug_log, "\n");
            va_end  (args);
        }

        // In debug mode it is helpful to print to console/terminal.
        #if defined(DEBUG_BUILD)
        va_start(args, _format);
        for (int i=0; i<current_debug_section; ++i) {
            fprintf(stdout, "  ");
        }
        vfprintf(stdout, _format, args);
        fprintf(stdout, "\n");
        va_end  (args);
        #endif // DEBUG_BUILD
    }
}

FILDEF void quit_debug_system ()
{
    // This condition is important because, for some reason, calling
    // fclose(NULL) results in a long hang-time during program exit.
    if (internal::debug_log) {
        fclose(internal::debug_log);
        internal::debug_log = NULL;
    }
}

FILDEF void begin_debug_section (const char* _name)
{
    if (_name) { internal::log_debug("%s", _name); }
    ++internal::current_debug_section;
}

FILDEF void end_debug_section ()
{
    if (internal::current_debug_section > 0) {
        --internal::current_debug_section;
    }
}
