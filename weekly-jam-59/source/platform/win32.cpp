/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

/* PLATFORM DEPENDENT - DO NOT USE OUTSIDE OF THIS FILE ***********************/

GLOBAL HWND window_handle;

/******************************************************************************/

INLDEF void PlatformInit ()
{
	// Get the underlying Win32 window handle for use in platform specific function calls.
	SDL_SysWMinfo window_info = {0};
	SDL_VERSION(&window_info.version);

	if (SDL_GetWindowWMInfo(window, &window_info) == SDL_FALSE) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to get underlying Win32 window handle!");
	} else {
		if (window_info.subsystem == SDL_SYSWM_WINDOWS) {
			window_handle = window_info.info.win.window;
		}
	}
}

INLDEF MessageBoxReturn ShowMessageBox (const char* _title, const char* _msg, MessageBoxType _type, MessageBoxButton _button)
{
	return CAST(MessageBoxReturn, MessageBox(window_handle, _msg, _title, (_type | _button)));
}

INLDEF String GetCurrentUsername ()
{
	DWORD output_length = UNLEN+1;
	String username;
	username.Resize(output_length); // MSDN says this is large enough for any username.

	GetUserName(username.c_string, &output_length);
	username.length = output_length;

	return username;
}

INLDEF bool DoesFileExist (const char* _filename)
{
	return PathFileExists(_filename);
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
