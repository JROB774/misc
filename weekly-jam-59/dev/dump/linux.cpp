/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

/* PLATFORM DEPENDENT - DO NOT USE OUTSIDE OF THIS FILE ***********************/

// MESSAGEBOX_BUTTON_OK
GLOBAL constexpr SDL_MessageBoxButtonData MESSAGEBOX_BUTTON_DATA_OK[] =
{{ SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, MESSAGEBOX_RETURN_OK,     "OK"     }};
// MESSAGEBOX_BUTTON_OKCANCEL
GLOBAL constexpr SDL_MessageBoxButtonData MESSAGEBOX_BUTTON_DATA_OKCANCEL[] =
{{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, MESSAGEBOX_RETURN_CANCEL, "Cancel" },
 { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, MESSAGEBOX_RETURN_OK,     "OK"     }};
// MESSAGEBOX_BUTTON_YESNO
GLOBAL constexpr SDL_MessageBoxButtonData MESSAGEBOX_BUTTON_DATA_YESNO[] =
{{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, MESSAGEBOX_RETURN_NO,     "No"     },
 { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, MESSAGEBOX_RETURN_YES,    "Yes"    }};
// MESSAGEBOX_BUTTON_YESNOCANCEL
GLOBAL constexpr SDL_MessageBoxButtonData MESSAGEBOX_BUTTON_DATA_YESNOCANCEL[] =
{{ SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, MESSAGEBOX_RETURN_CANCEL, "Cancel" },
 { 0,                                       MESSAGEBOX_RETURN_NO,     "No"     },
 { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, MESSAGEBOX_RETURN_YES,    "Yes"    }};

struct MessageBoxButtonInfo
{
	const SDL_MessageBoxButtonData* data = NULL;
	int count = 0;
};

GLOBAL constexpr MessageBoxButtonInfo MESSAGEBOX_BUTTON_INFO[] =
{
	{ MESSAGEBOX_BUTTON_DATA_OK,          ARRAY_COUNT(MESSAGEBOX_BUTTON_DATA_OK)          },
	{ MESSAGEBOX_BUTTON_DATA_OKCANCEL,    ARRAY_COUNT(MESSAGEBOX_BUTTON_DATA_OKCANCEL)    },
	{ MESSAGEBOX_BUTTON_DATA_YESNO,       ARRAY_COUNT(MESSAGEBOX_BUTTON_DATA_YESNO)       },
	{ MESSAGEBOX_BUTTON_DATA_YESNOCANCEL, ARRAY_COUNT(MESSAGEBOX_BUTTON_DATA_YESNOCANCEL) }
};

/******************************************************************************/

INLDEF void PlatformInit ()
{
	// There is no platform setup needed on Linux.
}

INLDEF MessageBoxReturn ShowMessageBox (const char* _title, const char* _msg, MessageBoxType _type, MessageBoxButton _button)
{
	SDL_MessageBoxData messagebox_data = {0};

	messagebox_data.flags      = CAST(u32, _type);
	messagebox_data.window     = window;
	messagebox_data.title      = _title;
	messagebox_data.message    = _msg;
	messagebox_data.numbuttons = MESSAGEBOX_BUTTON_INFO[_button].count;
	messagebox_data.buttons    = MESSAGEBOX_BUTTON_INFO[_button].data;

	int pressed_button;
	if (SDL_ShowMessageBox(&messagebox_data, &pressed_button) < 0) {
		TCE_ERROR_LOG(ERROR_LEVEL_MIN, ERROR_TYPE_SDL, "Failed to show SDL message box!");
	}

	return CAST(MessageBoxReturn, pressed_button);
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/

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
