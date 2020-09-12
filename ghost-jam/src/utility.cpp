namespace internal
{
    #if defined(PLATFORM_WINNT)
    FILDEF HWND get_window_handle (SDL_Window* _window)
    {
        SDL_SysWMinfo win_info;
        SDL_zero(win_info);

        SDL_VERSION(&win_info.version);

        if (!SDL_GetWindowWMInfo(_window, &win_info)) { return NULL; }
        else { return win_info.info.win.window; }
    }
    #endif // PLATFORM_WINNT
}

template<typename T, size_t N>
FORCEINLINE void Stack<T, N>::push (const T& _val)
{
    ASSERT(count < N);
    data[count++] = _val;
}

template<typename T, size_t N>
FORCEINLINE T Stack<T, N>::pop ()
{
    ASSERT(count > 0);
    return data[--count];
}

template<typename T, size_t N>
FORCEINLINE T& Stack<T, N>::peek ()
{
    ASSERT(count > 0);
    return data[count-1];
}

template<typename T, size_t N>
FORCEINLINE const T& Stack<T, N>::peek () const
{
    ASSERT(count > 0);
    return data[count-1];
}

#if defined(PLATFORM_WINNT)
STDDEF int show_alert (const char* _title, const char* _msg, int _type, int _buttons, Window* _window)
{
    // If the window is hidden then we do not bother passing it as the parent
    // of the alert box because on alert close the program proceeds to hang.
    HWND hwnd = NULL;
    if (_window) { hwnd = internal::get_window_handle(_window->window); }
    return MessageBoxA(hwnd, _msg, _title, (_type|_buttons));
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
STDDEF std::string get_executable_path ()
{
    constexpr size_t BUFFER_SIZE = MAX_PATH+1;
    char temp_buffer[BUFFER_SIZE] = {};

    GetModuleFileNameA(NULL, temp_buffer, BUFFER_SIZE);
    std::string path(fix_path_slashes(temp_buffer));

    // Get rid of the actual executable so it's just the path.
    size_t last_slash = path.find_last_of('/');
    if (last_slash != std::string::npos) { ++last_slash; }

    return path.substr(0, last_slash);
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
STDDEF std::string get_temporary_path ()
{
    constexpr size_t BUFFER_SIZE = MAX_PATH+1;
    char temp_buffer[BUFFER_SIZE] = {};

    GetTempPathA(BUFFER_SIZE, temp_buffer);
    std::string path(fix_path_slashes(temp_buffer));

    return path;
}
#endif // PLATFORM_WINNT

FILDEF size_t get_size_of_file (const char* _file_name)
{
    FILE* file = fopen(_file_name, "rb");
    if (!file) { return 0; }

    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);

    fclose(file);
    return size;
}

FILDEF size_t get_size_of_file (FILE* _file)
{
    if (!_file) { return 0; }

    fseek(_file, 0L, SEEK_END);
    size_t size = ftell(_file);
    rewind(_file); // Go back to avoid changing stream.

    return size;
}

#if defined(PLATFORM_WINNT)
FILDEF bool does_file_exist (const char* _file_name)
{
    DWORD attribs = GetFileAttributesA(_file_name);
    return ((attribs != INVALID_FILE_ATTRIBUTES) && !(attribs & FILE_ATTRIBUTE_DIRECTORY));
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
FILDEF bool does_path_exist (const char* _path)
{
    DWORD attribs = GetFileAttributesA(_path);
    return ((attribs != INVALID_FILE_ATTRIBUTES) && (attribs & FILE_ATTRIBUTE_DIRECTORY));
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
STDDEF void list_path_content (const char* _path, std::vector<std::string>& _content)
{
    // Clean the path in case there are trailing slashes.
    std::string path(fix_path_slashes(_path));
    while (path.back() == '/') { path.pop_back(); }

    // Check if the path even exists first.
    if (!is_path(path.c_str())) { return; }

    std::string find_path(path + "\\*");
    WIN32_FIND_DATAA find_data = {};

    HANDLE find_file = FindFirstFileA(find_path.c_str(), &find_data);
    defer { FindClose(find_file); }; // Clean-up on function return.

    if (find_file != INVALID_HANDLE_VALUE) {
        do {
            // We do not want to include the self and parent directories.
            std::string filename = find_data.cFileName;
            if (filename != "." && filename != "..") {
                _content.push_back(path + "/" + fix_path_slashes(filename.c_str()));
            }
        }
        while (FindNextFile(find_file, &find_data));
    }
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
STDDEF void list_path_content_r (const char* _path, std::vector<std::string>& _content)
{
    // Clean the path in case there are trailing slashes.
    std::string path(fix_path_slashes(_path));
    while (path.back() == '/') { path.pop_back(); }

    // We prefer to use forward slashes over backwards ones.
    std::replace(path.begin(), path.end(), '\\', '/');

    // Check if the path even exists first.
    if (!is_path(path.c_str())) { return; }

    std::string find_path(path + "\\*");
    WIN32_FIND_DATAA find_data = {};

    HANDLE find_file = FindFirstFileA(find_path.c_str(), &find_data);
    defer { FindClose(find_file); }; // Clean-up on function return.

    if (find_file != INVALID_HANDLE_VALUE) {
        do {
            std::string filename = find_data.cFileName;
            if (filename != "." && filename != "..") {
                // Make sure that we are using our preferred slashes.
                _content.push_back(path + "/" + fix_path_slashes(filename.c_str()));
                // Retrieve files from any sub-paths as well.
                if (is_path(_content.back().c_str())) {
                    list_path_content_r(_content.back().c_str(), _content);
                }
            }
        }
        while (FindNextFile(find_file, &find_data));
    }
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
STDDEF void list_path_files (const char* _path, std::vector<std::string>& _files)
{
    // Clean the path in case there are trailing slashes.
    std::string path(fix_path_slashes(_path));
    while (path.back() == '/') { path.pop_back(); }

    // Check if the path even exists first.
    if (!is_path(path.c_str())) { return; }

    std::string find_path(path + "\\*");
    WIN32_FIND_DATAA find_data = {};

    HANDLE find_file = FindFirstFileA(find_path.c_str(), &find_data);
    defer { FindClose(find_file); }; // Clean-up on function return.

    if (find_file != INVALID_HANDLE_VALUE) {
        do {
            // We do not want to include the self and parent directories.
            std::string filename = find_data.cFileName;
            if (filename != "." && filename != "..") {
                // Make sure that we are using our preferred slashes.
                std::string final(path + "/" + fix_path_slashes(filename.c_str()));
                if (is_file(final.c_str())) {
                    _files.push_back(final);
                }
            }
        }
        while (FindNextFile(find_file, &find_data));
    }
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
STDDEF void list_path_files_r (const char* _path, std::vector<std::string>& _files)
{
    // Clean the path in case there are trailing slashes.
    std::string path(fix_path_slashes(_path));
    while (path.back() == '/') { path.pop_back(); }

    // We prefer to use forward slashes over backwards ones.
    std::replace(path.begin(), path.end(), '\\', '/');

    // Check if the path even exists first.
    if (!is_path(path.c_str())) { return; }

    std::string find_path(path + "\\*");
    WIN32_FIND_DATAA find_data = {};

    HANDLE find_file = FindFirstFileA(find_path.c_str(), &find_data);
    defer { FindClose(find_file); }; // Clean-up on function return.

    if (find_file != INVALID_HANDLE_VALUE) {
        do {
            std::string filename = find_data.cFileName;
            if (filename != "." && filename != "..") {
                // Make sure that we are using our preferred slashes.
                std::string final(path + "/" + fix_path_slashes(filename.c_str()));
                if (is_file(final.c_str())) {
                    _files.push_back(final);
                } else {
                    list_path_files_r(final.c_str(), _files);
                }
            }
        }
        while (FindNextFile(find_file, &find_data));
    }
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
FILDEF bool create_path (const char* _path)
{
    // The Windows API function CreateDirectoryA only works if the path it is
    // creating is one-layer deep. We want our create_path function to create
    // all nested paths necessary so we do this by splitting up the provided
    // path and creating each sub-path until there are no more for us to add.

    std::vector<std::string> paths;
    tokenize_string(_path, "\\/", paths);

    if (!paths.empty()) {
        std::string path;
        for (auto& p: paths) {
            path += (p + "/");
            if (!does_path_exist(path.c_str())) {
                if (!CreateDirectoryA(path.c_str(), NULL)) {
                    return false;
                }
            }
        }
        return true;
    }

    return false;
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
FILDEF bool is_path_absolute (const char* _path)
{
    return !PathIsRelativeA(_path);
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
FILDEF bool is_file (const char* _file_name)
{
    return !is_path(_file_name);
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
FILDEF bool is_path (const char* _path)
{
    DWORD attribs = GetFileAttributesA(_path);
    return ((attribs != INVALID_FILE_ATTRIBUTES) && (attribs & FILE_ATTRIBUTE_DIRECTORY));
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
FILDEF u64 last_file_write_time (const char* _file_name)
{
    DWORD access = GENERIC_READ;
    DWORD share = FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE;

    HANDLE file = CreateFileA(_file_name, access, share, NULL,
      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file == INVALID_HANDLE_VALUE) { return 0; }
    defer { CloseHandle(file); };

    FILETIME file_time = {};
    if (!GetFileTime(file, NULL, NULL, &file_time)) { return 0; }

    ULARGE_INTEGER time;
    time.LowPart  = file_time.dwLowDateTime;
    time.HighPart = file_time.dwHighDateTime;

    return CAST(u64, time.QuadPart);
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
FILDEF std::string make_path_absolute (const char* _path)
{
    // If path is not absolute then we prefix the absolute executable path.
    std::string path(_path);
    if (PathIsRelativeA(path.c_str())) {
        path.insert(0, get_executable_path());
    }
    return path;
}
#endif // PLATFORM_WINNT

FILDEF std::string fix_path_slashes (const char* _path)
{
    // We prefer the use of forward over backwards slashes.
    std::string path(_path);
    std::replace(path.begin(), path.end(), '\\', '/');

    return path;
}

STDDEF void write_entire_file (const char* _file_name, const char* _content, size_t _length)
{
    FILE* file = fopen(_file_name, "wb");
    if (!file) { return; } // Failed to open file.

    fwrite(_content, sizeof(char), _length, file);
    fclose(file);
}

STDDEF char* read_entire_file (const char* _file_name)
{
    FILE* file = fopen(_file_name, "rb");
    if (!file) { return NULL; } // Failed to open file.

    defer { fclose(file); };

    size_t size = get_size_of_file(file);

    // It is the caller's responsibility to free the allocated buffer.
    char* buffer = cstd_malloc(char, size+1);
    if (!buffer) { return NULL; } // Failed to allocate.

    // If the result is not equal to the specified size then we know that
    // fread was unsuccessful so free the buffer and return NULL instead.
    size_t result = fread(buffer, sizeof(char), size, file);
    if (result != size) { cstd_free(buffer); return NULL; }

    buffer[size] = '\0';
    return buffer;
}

STDDEF std::string read_entire_file_str (const char* _file_name)
{
    std::ifstream file(_file_name);

    std::stringstream stream;
    stream << file.rdbuf();

    return stream.str();
}

#if defined(PLATFORM_WINNT)
FILDEF std::string strip_file_path (const char* _file_name)
{
    return std::string(PathFindFileNameA(_file_name));
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
FILDEF std::string strip_file_ext (const char* _file_name)
{
    char* file_name = cstd_malloc(char, strlen(_file_name)+1);
    if (!file_name) { return std::string(); }
    defer { cstd_free(file_name); };

    strcpy(file_name, _file_name);
    PathRemoveExtensionA(file_name);

    return std::string(file_name);
}
#endif // PLATFORM_WINNT

FILDEF std::string strip_file_path_and_ext (const char* _file_name)
{
    return strip_file_ext(strip_file_path(_file_name).c_str());
}

FILDEF std::string strip_file_name (const char* _file_name)
{
    std::string file_name(fix_path_slashes(_file_name));
    std::string::size_type last_slash = file_name.rfind('/');

    std::string path;
    if (last_slash != std::string::npos) {
        path = file_name.substr(0, last_slash+1);
    }
    return path;
}

FILDEF void tokenize_string (const std::string& _str, const char* _delims, std::vector<std::string>& _tokens)
{
    size_t prev = 0;
    size_t pos;

    while ((pos = _str.find_first_of(_delims, prev)) != std::string::npos) {
        if (pos > prev) { _tokens.push_back(_str.substr(prev, pos-prev)); }
        prev = pos+1;
    }
    if (prev < _str.length()) {
        _tokens.push_back(_str.substr(prev, std::string::npos));
    }
}

INLDEF std::string format_string (const char* _format, ...)
{
    std::string str;
    va_list args;

    va_start(args, _format);
    defer { va_end(args); };

    int size = vsnprintf(NULL, 0, _format, args) + 1;
    char* buffer = cstd_malloc(char, size);
    if (buffer) {
        vsnprintf(buffer, size, _format, args);
        str = buffer;
        cstd_free(buffer);
    }

    return str;
}

INLDEF std::string format_string_v (const char* _format, va_list _args)
{
    std::string str;

    int size = vsnprintf(NULL, 0, _format, _args) + 1;
    char* buffer = cstd_malloc(char, size);
    if (buffer) {
        vsnprintf(buffer, size, _format, _args);
        str = buffer;
        cstd_free(buffer);
    }

    return str;
}

FILDEF int get_line_count (const char* _str)
{
    int lines = 1; // Always have at least one line.
    for (const char* c=_str; *c; ++c) {
        if (*c == '\n') { ++lines; }
    }
    return lines;
}

FILDEF Vec2 get_mouse_pos ()
{
    int imx, imy; SDL_GetMouseState(&imx, &imy);
    return Vec2(CAST(float, imx), CAST(float, imy));
}

INLDEF std::string format_time (const char* _format)
{
    time_t     raw_time = time(NULL);
    struct tm* cur_time = localtime(&raw_time);

    size_t length = 256;
    size_t result = 0;

    // We go until our buffer is big enough.
    char* buffer = NULL;
    do {
        if (buffer) { cstd_free(buffer); }
        buffer = cstd_malloc(char, length);
        if (!buffer) { return std::string(); }

        result = strftime(buffer, length, _format, cur_time);
        length *= 2;
    }
    while (!result);

    defer { cstd_free(buffer); };
    return std::string(buffer);
}

#if defined(PLATFORM_WINNT)
FILDEF unsigned int get_thread_id ()
{
    return CAST(unsigned int, GetCurrentThreadId());
}
#endif // PLATFORM_WINNT

FILDEF bool point_in_bounds_xyxy (Vec2 _p, float _x1, float _y1, float _x2, float _y2)
{
    return (_p.x >= _x1 && _p.y >= _y1 && _p.x <= _x2 && _p.y <= _y2);
}

FILDEF bool point_in_bounds_xyxy (Vec2 _p, Quad _q)
{
    return (_p.x >= _q.x1 && _p.y >= _q.y1 && _p.x <= _q.x2 && _p.y <= _q.y2);
}

FILDEF bool point_in_bounds_xywh (Vec2 _p, float _x, float _y, float _w, float _h)
{
    return (_p.x >= _x && _p.y >= _y && _p.x < (_x+_w) && _p.y < (_y+_h));
}

FILDEF bool point_in_bounds_xywh (Vec2 _p, Quad _q)
{
    return (_p.x >= _q.x && _p.y >= _q.y && _p.x < (_q.x+_q.w) && _p.y < (_q.y+_q.h));
}

FILDEF bool insensitive_compare (const std::string& _a, const std::string& _b)
{
    if (_a.length() != _b.length()) { return false; }
    for (std::string::size_type i=0; i<_a.length(); ++i) { // A and B are same length.
        if (tolower(_a[i]) != tolower(_b[i])) { return false; }
    }
    return true;
}

FILDEF bool string_replace (std::string& _str, const std::string& _from, const std::string& _to)
{
    std::string::size_type start_pos = _str.find(_from);
    if (start_pos == std::string::npos) { return false; }
    _str.replace(start_pos, _from.length(), _to);
    return true;
}

#if defined(PLATFORM_WINNT)
FILDEF void play_error_sound ()
{
    MessageBeep(MB_ICONERROR);
}
#endif // PLATFORM_WINNT

#if defined(PLATFORM_WINNT)
FILDEF void play_warning_sound ()
{
    MessageBeep(MB_ICONWARNING);
}
#endif // PLATFORM_WINNT

FILDEF float random_float (float _min, float _max)
{
    float random = CAST(float, rand()) / CAST(float, RAND_MAX);
    float diff = _max - _min;
    float r = random * diff;
    return _min + r;
}

FILDEF GonObject load_gon_and_remove_comments (std::string _file_name)
{
    std::string buffer(read_entire_file_str(_file_name.c_str()));

    // Remove single-line comments.
    size_t pos = 0;
    while ((pos = buffer.find("//", pos)) != std::string::npos) {
        buffer.erase(pos, buffer.find("\n", pos)-pos);
    }
    // Remove multi-line comments.
    pos = 0;
    while ((pos = buffer.find("/*", pos)) != std::string::npos) {
        buffer.erase(pos, (buffer.find("*/", pos)+2)-pos);
    }

    // printf("%s\n", buffer.c_str());
    return GonObject::LoadFromBuffer(buffer);
}
