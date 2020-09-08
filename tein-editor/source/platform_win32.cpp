// external library includes
#include <windows.h>
#include <dbghelp.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>
#include <intrin.h> // @TODO: REMOVE EVENTUALLY
#include <commdlg.h>
#include <shellapi.h>

namespace Platform
{
	namespace
	{
		constexpr const char* LVL_FILEFILTER = "All Files (*.*)\0*.*\0LVL Files (*.lvl)\0*.lvl\0";
		constexpr const char* GPAK_FILEFILTER = "All Files (*.*)\0*.*\0GPAK Files (*.gpak)\0*.gpak\0";

		constexpr int MAX_STACKSIZE = 128;

		char workingdirectory[MAX_PATH] = {};
		char dialogdirectory[MAX_PATH] = {};

		char filenamebuffer[MAX_PATH] = {};
		char foldernamebuffer[MAX_PATH] = {};

		OPENFILENAMEA openfilename = {};
		BROWSEINFO browseinfo = {};

		SDL_SysWMinfo windowinfo = {};
		HWND windowhandle = nullptr;
	}

	void Init (SDL_Window* _window, int _argc, std::string _arg)
	{
		// gets system-independent info regarding the underlying application window
		SDL_VERSION(&windowinfo.version);
		SDL_GetWindowWMInfo(_window, &windowinfo);
		if (windowinfo.subsystem == SDL_SYSWM_WINDOWS) {
			windowhandle = windowinfo.info.win.window;
		}

		// sets up the data structure used for saving and opening files
		openfilename.hwndOwner = windowhandle;
		openfilename.lStructSize = sizeof(OPENFILENAME);
		openfilename.lpstrFilter = LVL_FILEFILTER;
		openfilename.nFilterIndex = 2;
		openfilename.lpstrFile = filenamebuffer;
		openfilename.lpstrFile[0] = '\0';
		openfilename.nMaxFile = MAX_PATH;
		openfilename.lpstrDefExt = "lvl";

		// initialise appropriate systems needed to for the open folder dialog
		CoInitialize(nullptr);
		
		// sets up the data structure used for opening folders/paths
		browseinfo.hwndOwner = windowhandle;
		browseinfo.pszDisplayName = foldernamebuffer;
		browseinfo.lpszTitle = "Select a GPAK to unpack.";
		// @NOTE: BAD PRACTICE BUT DON'T CARE AS WILL NOT BE USING THIS SYSTEM IN FUTURE
		browseinfo.ulFlags = BIF_NEWDIALOGSTYLE;

		// store application working directory as open/save dialogs will change it
		// we set the working directory to the location of the exe as the user dragging
		// a level onto the exe may change the working directory on startup

		if (_argc > 1) {
			// use the command line args if a level was dragged, etc.
			std::string tempdir = _arg;
			std::size_t slashpos = tempdir.find_last_of("/\\");
			// however if its not a valid directory don't bother
			if (slashpos != std::string::npos) {
				tempdir = tempdir.substr(0, slashpos);
				std::strcpy(workingdirectory, tempdir.c_str());
				SetCurrentDirectory(workingdirectory);
				return;
			}
		}
		// just get the current directory otherwise
		GetCurrentDirectory(MAX_PATH, workingdirectory);
	}

	std::string SaveDialog (DialogType _type)
	{
		// sets the working directory to the last location the dialog was open at
		if (dialogdirectory[0] != '\0') { SetCurrentDirectory(dialogdirectory); }

		// opens a save file dialog box
		bool failed = false;
		openfilename.lpstrFilter = ((_type == DIALOG_TYPE_LVL) ? LVL_FILEFILTER : GPAK_FILEFILTER);
		openfilename.Flags = OFN_PATHMUSTEXIST|OFN_OVERWRITEPROMPT;
		if (!GetSaveFileName(&openfilename)) {
			// handles any potential problems with the dialog (user may have just cancelled)
			DWORD errcode = CommDlgExtendedError();
			if (errcode != 0) {
				std::string code = std::to_string(errcode);
				ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to save file (Error: " + code + ")!");
			}
			// if the user cancels or there's an error don't save
			failed = true;
			// we still want to reset working dir though...
		}

		// store the dialog directory and reset to working directory for rest of program
		GetCurrentDirectory(MAX_PATH, dialogdirectory);
		SetCurrentDirectory(workingdirectory);

		// return the file path of the tile to be opened
		return ((failed) ? std::string() : std::string(filenamebuffer));
	}

	std::string OpenDialog (DialogType _type)
	{
		// sets the working directory to the last location the dialog was open at
		if (dialogdirectory[0] != '\0') { SetCurrentDirectory(dialogdirectory); }

		// opens an open file dialog box
		bool failed = false;
		openfilename.lpstrFilter = ((_type == DIALOG_TYPE_LVL) ? LVL_FILEFILTER : GPAK_FILEFILTER);
		openfilename.Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST;
		if (!GetOpenFileName(&openfilename)) {
			// handles any potential problems with the dialog (user may have just cancelled)
			DWORD errcode = CommDlgExtendedError();
			if (errcode != 0) {
				std::string code = std::to_string(errcode);
				ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to open file (Error: " + code + ")!");
			}
			// if the user cancels or there's an error don't open
			failed = true;
			// we still want to reset working dir though...
		}

		// store the dialog directory and reset to working directory for rest of program
		GetCurrentDirectory(MAX_PATH, dialogdirectory);
		SetCurrentDirectory(workingdirectory);

		// return the file path of the tile to be opened
		return ((failed) ? std::string() : std::string(filenamebuffer));
	}

	std::string OpenFolderDialog ()
	{
		// sets the working directory to the last location the dialog was open at
		if (dialogdirectory[0] != '\0') { SetCurrentDirectory(dialogdirectory); }

		// opens a select folder dialog box for the user
		bool failed = false;
		LPITEMIDLIST item = SHBrowseForFolder(&browseinfo);
		if (!item) { failed = true; }
		// converts the returned item into a usable path
		char folderbuffer[MAX_PATH];
		if (!SHGetPathFromIDList(item, folderbuffer)) { failed = true; }
		// windows requires us to free the returned item
		CoTaskMemFree(item);

		// store the dialog directory and reset to working directory for rest of program
		GetCurrentDirectory(MAX_PATH, dialogdirectory);
		SetCurrentDirectory(workingdirectory);

		// returns the folder/path to the user
		return ((failed) ? std::string() : std::string(folderbuffer));
	}

	void RunExecutable (std::string _exename, std::string _pathname)
	{
		// setup for launching an executable
		STARTUPINFO startupinfo = {};
		startupinfo.cb = sizeof(STARTUPINFO);
		PROCESS_INFORMATION processinfo = {};
		
		// launch the specified executable
		bool retval = CreateProcess(
			_exename.c_str(),
			nullptr,
			nullptr,
			nullptr,
			FALSE,
			0,
			nullptr,
			_pathname.c_str(),
			&startupinfo,
			&processinfo
		);

		// win docs state that these handles must be closed
		CloseHandle(processinfo.hProcess);
		CloseHandle(processinfo.hThread);

		// inform user if the executable could not be launched
		if (!retval) { ERROR_LOG(Error::PRIORITY_LOW, Error::TYPE_STD, "Failed to launch the executable!"); }
	}

	void LoadWebpage (std::string _webpagename)
	{
		// loads a webpage in the default browser
		ShellExecute(nullptr, nullptr, _webpagename.c_str(), nullptr, nullptr, SW_SHOW);
	}

	void SetWorkingDirectory ()
	{
		// sets the current directory to the working directory, this is used by
		// error.cpp to force the working directory back to normal so the log
		// always saves in the correct location even if an error occurs during a
		// weird time
		//
		// probably shouldn't be called anywhere else

		SetCurrentDirectory(workingdirectory);
	}

	MessageBoxResult MessageBox (MessageBoxType _type, std::string _title, std::string _text)
	{
		// the reason we handle conversions between generic values and os-specific values
		// within the function itself is for cleanliness and ease-of-use, ensuring that
		// all os-specific code is contained entirely within this CPP
		//
		// the performance loss is minute due to the infrequency of calls and speed of
		// the conversions

		// set the flags based on type passed
		unsigned int flags = 0;
		switch (_type) {
			case (MSGBOX_TYPE_OK): { flags = MB_OK; } break;
			case (MSGBOX_TYPE_YESNO): { flags = MB_YESNO; } break;
			case (MSGBOX_TYPE_YESNOCANCEL): { flags = MB_YESNOCANCEL; } break;
		}

		// display appropriate message box with neccessary flags
		int result = MessageBox(windowhandle, _text.c_str(), _title.c_str(), flags);

		// return the appropriate value
		switch (result) {
			case (IDOK): { return MSGBOX_RESULT_OK; }
			case (IDYES): { return MSGBOX_RESULT_YES; }
			case (IDNO): { return MSGBOX_RESULT_NO; }
			case (IDCANCEL): { return MSGBOX_RESULT_CANCEL; }
			default: { return MSGBOX_RESULT_NULL; }
		}
	}

	void Stacktrace (std::ostream& _stream)
	{
		HANDLE process = GetCurrentProcess();
		HANDLE thread = GetCurrentThread();

		CONTEXT context = {};
		context.ContextFlags = CONTEXT_FULL;
		RtlCaptureContext(&context);

		SymInitialize(process, nullptr, TRUE);

		DWORD image;
		STACKFRAME64 stackframe = {};

		#ifdef _M_IX86
		image = IMAGE_FILE_MACHINE_I386;
		stackframe.AddrPC.Offset    = context.Eip, stackframe.AddrPC.Mode    = AddrModeFlat;
		stackframe.AddrFrame.Offset = context.Ebp, stackframe.AddrFrame.Mode = AddrModeFlat;
		stackframe.AddrStack.Offset = context.Esp, stackframe.AddrStack.Mode = AddrModeFlat;
		#elif  _M_IA64
		image = IMAGE_FILE_MACHINE_IA64;
		stackframe.AddrPC.Offset     = context.StIIP, stackframe.AddrPC.Mode     = AddrModeFlat;
		stackframe.AddrFrame.Offset  = context.IntSp, stackframe.AddrFrame.Mode  = AddrModeFlat;
		stackframe.AddrBStore.Offset = context.RsBSP, stackframe.AddrBStore.Mode = AddrModeFlat;
		stackframe.AddrStack.Offset  = context.IntSp, stackframe.AddrStack.Mode  = AddrModeFlat;
		#elif  _M_X64
		image = IMAGE_FILE_MACHINE_AMD64;
		stackframe.AddrPC.Offset    = context.Rip, stackframe.AddrPC.Mode    = AddrModeFlat;
		stackframe.AddrFrame.Offset = context.Rsp, stackframe.AddrFrame.Mode = AddrModeFlat;
		stackframe.AddrStack.Offset = context.Rsp, stackframe.AddrStack.Mode = AddrModeFlat;
		#endif

		for (std::size_t i = 0; i < MAX_STACKSIZE; ++i) {
			BOOL result = StackWalk64(image, process, thread, &stackframe, &context, nullptr, SymFunctionTableAccess64, SymGetModuleBase64, nullptr);
			if (!result) { break; }

			DWORD64 displacement = 0;
			std::uint8_t symbolbuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
			SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolbuffer);
			symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
			symbol->MaxNameLen = MAX_SYM_NAME;

			result = SymFromAddr(process, stackframe.AddrPC.Offset, &displacement, symbol);

			_stream
				<< std::dec
				<< "(" << i << "): "
				<< ((result) ? symbol->Name : "???")
				<< " [0x"
				<< std::hex << std::uppercase
				<< std::setfill('0') << std::setw(8)
				<< symbol->Address
				<< "]\n";
		}

		SymCleanup(process);
	}

	std::uint64_t GetFileLastWriteTime (std::string _filename)
	{
		// create a handle for the specified file
		HANDLE file = CreateFile(
			_filename.c_str(), GENERIC_READ, FILE_SHARE_DELETE|FILE_SHARE_READ|FILE_SHARE_WRITE,
			nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr
		);
		if (file == INVALID_HANDLE_VALUE) { return 0; }

		// get the last modified date of the file and store it for later comparison
		FILETIME filetime = {};
		if (!GetFileTime(file, nullptr, nullptr, &filetime)) { return 0; }

		// converts the filetime structure into a usable long unsigned integer (64-bits)
		return std::uint64_t((std::uint64_t)(filetime.dwHighDateTime)|((std::uint64_t)filetime.dwLowDateTime<<32));
	}

	std::size_t GetFileSizeInBytes (std::string _filename)
	{
		// opens a handle to the file, for reading
		HANDLE file = CreateFile(_filename.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		// retrieve the size of the file if possible
		std::size_t filesize;
		if (GetLastError() == ERROR_FILE_NOT_FOUND) { filesize = 0; }
		else { filesize = GetFileSize(file, nullptr); }

		// clean up and return size
		CloseHandle(file);
		return filesize;	
	}

	std::vector<std::string> ListDirectoryContents (std::string _path)
	{
		const char SELF[1] = {'.'};
		const char PARENT[2] = {'.','.'};

		// do not attempt if the path given is too long
		if (_path.length() > MAX_PATH) { return std::vector<std::string>(); }
		// this is neccessary for FindFile funcs
		std::string path = _path + "\\*";

		// gets the first file/dir in the directory
		WIN32_FIND_DATA filedata = {};
		HANDLE findfile = FindFirstFile(path.c_str(), &filedata);

		// if the handle is invalid then there are no files/dirs...
		std::vector<std::string> contents;
		if (findfile != INVALID_HANDLE_VALUE) {
			// ...go through and list the remaining files/dirs
			do {
				// prevents listing self and parent dir in content vector
				std::string filename = filedata.cFileName; // comparing cFileName directly didn't work :(
				if (filename != "." && filename != "..") {
					contents.push_back(_path + "\\" + filename);
				}
			}
			while (FindNextFile(findfile, &filedata));
		}

		// end the file/dir find operation
		FindClose(findfile);
		// return the list of files/dirs in the directory (potentially none)
		return contents;
	}

	bool CreateNewDirectory (std::string _path)
	{
		// splits the given path into separate sections
		std::vector<std::string> sections = Utility::StrToken(_path, "\\/");
		if (sections.empty()) { return false; }

		// goes through and creates each neccessary section (allows for creation of multi-layer dirs)
		std::string dir;
		for (auto& section: sections) {
			// adds the next layer of the directory
			dir += (section + "\\");
			// create the directory if it doesn't already exist
			if (!PathFileExists(dir.c_str())) {
				if (!CreateDirectory(dir.c_str(), nullptr)) {
					return false;
				}
			}
		}

		// the directory creation was successful
		return true;
	}

	bool IsDirectory (std::string _path)
	{
		// get file-system attributes regarding the specified path
		DWORD attributes = GetFileAttributes(_path.c_str());
		// query whether the specified path is a directory or not
		return ((attributes & FILE_ATTRIBUTE_DIRECTORY) ? true : false);
	}

	bool FileOrDirectoryExists (std::string _path)
	{
		// query the existence of the specified file/directory
		return PathFileExists(_path.c_str());
	}
}