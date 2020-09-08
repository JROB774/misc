/*******************************************************************************
 * LINUX
 * Platform independent interface implemented for Linux systems.
 *
*******************************************************************************/

#ifndef __TCE_LINUX_H__ /*////////////////////////////////////////////////////*/
#define __TCE_LINUX_H__

namespace TCE
{

enum MessageBoxButton
{
	MESSAGEBOX_BUTTON_OK,
	MESSAGEBOX_BUTTON_OKCANCEL,
	MESSAGEBOX_BUTTON_YESNO,
	MESSAGEBOX_BUTTON_YESNOCANCEL
};

enum MessageBoxType
{
	MESSAGEBOX_TYPE_INFO = SDL_MESSAGEBOX_INFORMATION,
	MESSAGEBOX_TYPE_WARNING = SDL_MESSAGEBOX_WARNING,
	MESSAGEBOX_TYPE_ERROR = SDL_MESSAGEBOX_ERROR
};

enum MessageBoxReturn
{
	MESSAGEBOX_RETURN_NULL,
	MESSAGEBOX_RETURN_OK,
	MESSAGEBOX_RETURN_YES,
	MESSAGEBOX_RETURN_NO,
	MESSAGEBOX_RETURN_CANCEL
};

INLDEF void PlatformInit ();

INLDEF MessageBoxReturn ShowMessageBox (const char* _title, const char* _msg, MessageBoxType _type, MessageBoxButton _button);

} // TCE

#endif /* __TCE_LINUX_H__ ////////////////////////////////////////////////////*/

/*******************************************************************************
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
*******************************************************************************/
