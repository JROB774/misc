/*******************************************************************************
 * WIN32
 * Platform independent interface implemented for Windows systems.
 *
*******************************************************************************/

#ifndef __TCE_WIN32_H__ /*////////////////////////////////////////////////////*/
#define __TCE_WIN32_H__

// We don't care about external library warnings as we can't fix them anyway.
#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif

#include <windows.h>
#include <lmcons.h>
#include <shlwapi.h>

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

namespace TCE
{

enum MessageBoxButton
{
	MESSAGEBOX_BUTTON_OK = MB_OK,
	MESSAGEBOX_BUTTON_OKCANCEL = MB_OKCANCEL,
	MESSAGEBOX_BUTTON_YESNO = MB_YESNO,
	MESSAGEBOX_BUTTON_YESNOCANCEL = MB_YESNOCANCEL,
};

enum MessageBoxType
{
	MESSAGEBOX_TYPE_INFO = MB_ICONINFORMATION,
	MESSAGEBOX_TYPE_WARNING = MB_ICONWARNING,
	MESSAGEBOX_TYPE_ERROR = MB_ICONERROR,
};

enum MessageBoxReturn
{
	MESSAGEBOX_RETURN_NULL = 0,
	MESSAGEBOX_RETURN_OK = IDOK,
	MESSAGEBOX_RETURN_YES = IDYES,
	MESSAGEBOX_RETURN_NO = IDNO,
	MESSAGEBOX_RETURN_CANCEL = IDCANCEL
};

INLDEF void PlatformInit ();

INLDEF MessageBoxReturn ShowMessageBox (const char* _title, const char* _msg, MessageBoxType _type, MessageBoxButton _button);

INLDEF String GetCurrentUsername ();

INLDEF bool DoesFileExist (const char* _filename);

} // TCE

#endif /* __TCE_WIN32_H__ ////////////////////////////////////////////////////*/
