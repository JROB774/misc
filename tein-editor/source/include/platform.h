#ifndef __PLATFORM_H_INCLUDED__
#define __PLATFORM_H_INCLUDED__

namespace Platform
{
	enum DialogType { DIALOG_TYPE_LVL, DIALOG_TYPE_GPAK };

	enum MessageBoxType { MSGBOX_TYPE_OK, MSGBOX_TYPE_YESNO, MSGBOX_TYPE_YESNOCANCEL };
	enum MessageBoxResult { MSGBOX_RESULT_NULL, MSGBOX_RESULT_OK, MSGBOX_RESULT_YES, MSGBOX_RESULT_NO, MSGBOX_RESULT_CANCEL };

	void Init(SDL_Window* _window, int _argc, std::string _arg);

	std::string SaveDialog(DialogType _type);

	std::string OpenDialog(DialogType _type);
	std::string OpenFolderDialog();

	void RunExecutable(std::string _exename, std::string _pathname);
	void LoadWebpage(std::string _webpagename);

	void SetWorkingDirectory();

	MessageBoxResult MessageBox(MessageBoxType _type, std::string _title, std::string _text);

	void Stacktrace(std::ostream& _stream);

	std::uint64_t GetFileLastWriteTime(std::string _filename);
	std::size_t GetFileSizeInBytes(std::string _filename);
	
	std::vector<std::string> ListDirectoryContents(std::string _path);

	bool CreateNewDirectory(std::string _path);

	bool IsDirectory(std::string _path);
	bool FileOrDirectoryExists(std::string _path);
}

#endif // __PLATFORM_H_INCLUDED__