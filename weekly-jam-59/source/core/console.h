/*******************************************************************************
 * CONSOLE
 * Debug console capable of calling Lua functions as commands.
 *
*******************************************************************************/

#ifndef __TCE_CONSOLE_H__ /*//////////////////////////////////////////////////*/
#define __TCE_CONSOLE_H__

namespace TCE
{

enum ConsoleState
{
	CONSOLE_STATE_INACTIVE,
	CONSOLE_STATE_MINI,
	CONSOLE_STATE_LARGE,

	CONSOLE_STATE_TOTAL
};

struct ConsoleCursor
{
	size_t position = 0;

	float blink_timer;
	float blink_interval = 1.0f;
};

GLOBAL constexpr const char* LUA_CONSOLE_MODULE_NAME = "Console";

// These need default values in case the variable tweak value cannot be loaded!
GLOBAL float console_mini_height = 0.25f;
GLOBAL float console_large_height = 0.75f;

GLOBAL int console_left_padding = 4;
GLOBAL int console_bottom_padding = 4;

GLOBAL SDL_Color console_input_color   = { 255, 255, 255, 255 };
GLOBAL SDL_Color console_history_color = { 155, 155, 155, 255 };
GLOBAL SDL_Color console_back_color    = {   0,   0,   0, 240 };

GLOBAL float console_scroll_speed = 10.0f;

GLOBAL float console_text_scale = 1.0f;

// Whether or not the console is enabled is determined by a variable in the tweak file.
// This needs a default value in case the variable tweak value cannot be loaded!
GLOBAL bool console_enabled = false;

GLOBAL ConsoleState console_state;
GLOBAL ConsoleCursor console_cursor;

GLOBAL FontHandle console_font;

// It is not the most efficent or smartest thing to implement the history in this
// way it would make more sense to use a circular queue so we don't just keep
// allocating more and more memory but as this is only a developer-based feature
// it really doesn't matter and it would take too long to add a circular queue.
GLOBAL Array<String> console_history;
GLOBAL String console_input;

GLOBAL size_t console_history_position = 0;

GLOBAL float console_current_height = 0.0f;
GLOBAL float console_target_height = 0.0f;

INLDEF void ConsoleInit ();

INLDEF void AddConsoleHistoryElement (String _element);

INLDEF void ConsoleHandleTextInput ();
INLDEF void ConsoleUpdateAndRender (float _dt);

INLDEF void ClearConsole ();

} // TCE

#endif /* __TCE_CONSOLE_H__ //////////////////////////////////////////////////*/
