/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void ConsoleInit ()
{
	// Do not bother, if the console is not meant to be enabled
	if (!console_enabled) { return; }
	// Load the font used by the console for text rendering.
	console_font = LoadFontFromFile("data/fonts/console.font");
}

INLDEF void AddConsoleHistoryElement (String _element)
{
	console_history.AddElement(_element);
	console_history_position = console_history.count;
}

INLDEF void CycleUpConsoleHistory ()
{
	if (console_history.count == 0) { return; }

	// Find the first non-blank line in the history.
	size_t counter = 0;
	do {
		// Wrap back around if we hit the end.
		if (console_history_position == 0) { console_history_position = console_history.count; }
		--console_history_position;

		// Because there is a chance the history is entirely blank lines we want to
		// exit the function if we have searched the entire history for a line to use.
		if (counter++ >= console_history.count) { return; }
	}
	while (console_history[console_history_position].length == 0);

	console_input = console_history[console_history_position];
	console_cursor.position = console_input.length;
}

INLDEF void CycleDownConsoleHistory ()
{
	if (console_history.count == 0) { return; }

	// Find the first non-blank line in the history.
	size_t counter = 0;
	do {
		// Wrap back around if we hit the start.
		++console_history_position;
		if (console_history_position >= console_history.count) { console_history_position = 0; }

		// Because there is a chance the history is entirely blank lines we want to
		// exit the function if we have searched the entire history for a line to use.
		if (counter++ >= console_history.count) { return; }
	}
	while (console_history[console_history_position].length == 0);

	console_input = console_history[console_history_position];
	console_cursor.position = console_input.length;
}

INLDEF void MoveConsoleCursorRight ()
{
	if (console_cursor.position < console_input.length) { ++console_cursor.position; }
}

INLDEF void MoveConsoleCursorLeft ()
{
	if (console_cursor.position > 0) { --console_cursor.position; }
}

INLDEF void MoveConsoleCursorToStart ()
{
	console_cursor.position = 0;
}
INLDEF void MoveConsoleCursorToEnd ()
{
	console_cursor.position = console_input.length;
}

INLDEF void RemoveCharacterBeforeCursor ()
{
	if (console_cursor.position == 0) { return; }
	console_input.RemoveCharacterAt(--console_cursor.position);
}

INLDEF void RemoveCharacterAfterCursor ()
{
	if (console_cursor.position >= console_input.length) { return; }
	console_input.RemoveCharacterAt(console_cursor.position);
}

INLDEF void ClearConsoleInput ()
{
	console_input.Clear();
	console_cursor.position = 0;
}

INLDEF void ExecuteConsoleInput ()
{
	// Add the entered line to the console's history
	AddConsoleHistoryElement(console_input);

	// Split the input into tokens for easy parsing.
	String code_to_execute;
	Array<String> tokens = console_input.Tokenize(" ");
	ClearConsoleInput();

	// No point executing if we were given nothing to execute.
	if (tokens.count == 0) { return; }

	// Check to make sure that the command provided is a valid command.
	if (lua_getglobal(lua_main_state, "Console") == LUA_TTABLE) {
		if (lua_getfield(lua_main_state, 1, tokens[0].c_string) != LUA_TFUNCTION) {
			String error_message = String("ERORR: Invalid command: ") + tokens[0];
			AddConsoleHistoryElement(error_message);
			return;
		}
	} else {
		AddConsoleHistoryElement("ERROR: Console not available!");
		return;
	}

	// Form the console module function call.
	code_to_execute += String(LUA_CONSOLE_MODULE_NAME) + String(".") + tokens[0];
	// Add the arguments to the fucntion call.
	code_to_execute.AddCString("(");
	for (size_t i=1; i<tokens.count; ++i) {
		code_to_execute += tokens[i];
		if (i != tokens.count-1) {
			code_to_execute += String(",");
		}
	}
	code_to_execute.AddCString(")");

	// Run the newly formed Lua code string.
	if (luaL_dostring(lua_main_state, code_to_execute.c_string) != 0) {
		AddConsoleHistoryElement("ERROR: Invalid command!");
		LogLuaError(lua_main_state);
	}
}

INLDEF void ConsoleHandleTextInput ()
{
	// Do not bother, if the console is not meant to be enabled
	if (!console_enabled) { return; }
	// We don't want to let people type whilst inactive!
	if (console_state == CONSOLE_STATE_INACTIVE) { return; }

	switch (event.type) {
		// Handles special character input.
		case (SDL_KEYDOWN): {
			switch (event.key.keysym.scancode) {
				case (SDL_SCANCODE_UP):        { CycleUpConsoleHistory();       } break;
				case (SDL_SCANCODE_DOWN):      { CycleDownConsoleHistory();     } break;
				case (SDL_SCANCODE_RIGHT):     { MoveConsoleCursorRight();      } break;
				case (SDL_SCANCODE_LEFT):      { MoveConsoleCursorLeft();       } break;
				case (SDL_SCANCODE_HOME):      { MoveConsoleCursorToStart();    } break;
				case (SDL_SCANCODE_END):       { MoveConsoleCursorToEnd();      } break;
				case (SDL_SCANCODE_BACKSPACE): { RemoveCharacterBeforeCursor(); } break;
				case (SDL_SCANCODE_DELETE):    { RemoveCharacterAfterCursor();  } break;
				case (SDL_SCANCODE_ESCAPE):    { ClearConsoleInput();           } break;
				case (SDL_SCANCODE_RETURN):    { ExecuteConsoleInput();         } break;
			}
			// Handle special keydown inputs with mods.
			console_cursor.blink_timer = 0.0f;
		} break;
		// Handles the standard text input.
		case (SDL_TEXTINPUT): {
			if (event.text.text[0] != '`') {
				console_input.AddCharacterAtPosition(event.text.text[0], console_cursor.position);
				++console_cursor.position;
			}
			console_cursor.blink_timer = 0.0f;
		} break;
	}
}

INLDEF void ConsoleUpdate (float _dt)
{
	// @TEMPORARY: WE PROBABLY WANT A BETTER SYSTEM LATER WHERE THE ENTIRE GAME STATE IS FROZEN
	// INSTEAD BUT FOR NOW THIS IS FINE AND I CAN IMPLEMENT THE OTHER SYSTEM LUA-SIDE LATER
	//
	// E.G. WITH AN IS CONSOLE DOWN FUNCTION THAT JUST SKIPS THE UPDATE LOOP IF THAT IS TRUE
	// AND PAUSES ALL THE GAME'S ANIMATIONS, ETC TO AVOID ANY WEIRDNESS.

	// We still want the console to be able to handle any input events.
	events_locked = false;

	// If the user presses the grave key we want to cycle through the different console states.
	if (KeyPressed(SDL_SCANCODE_GRAVE)) {
		// Cycle through the states and wrap back if we go past the last state.
		console_state = CAST(ConsoleState, console_state + 1);
		if (console_state == CONSOLE_STATE_TOTAL) {
			console_state = CONSOLE_STATE_INACTIVE;
		}
		// Set the new target height for the console to smoothly lerp to.
		switch (console_state) {
			case (CONSOLE_STATE_INACTIVE): { console_target_height = 0.0f;                 } break;
			case (CONSOLE_STATE_MINI):     { console_target_height = console_mini_height;  } break;
			case (CONSOLE_STATE_LARGE):    { console_target_height = console_large_height; } break;
		}
	}

	// Blinks the cursor whilst it is inactive (actions in ConsoleHandleTextInput() reset the timer).
	console_cursor.blink_timer += _dt;
	while (console_cursor.blink_timer >= (console_cursor.blink_interval * 2.0f)) {
		console_cursor.blink_timer -= (console_cursor.blink_interval * 2.0f);
	}

	// Whilst the console is not at its target height we want to transition there.
	if (console_current_height != console_target_height) {
		if (console_scroll_speed > 0.0f) {
			// Lerp between the current and target height.
			float speed = console_scroll_speed * _dt;
			console_current_height = (1.0f - speed) * console_current_height + speed * console_target_height;
			if (console_current_height >= FLT_MAX) { // We cap this to avoid breaking the lerp!
				console_current_height = console_target_height;
			}
		} else {
			// Instantaneously snap to target height.
			console_current_height = console_target_height;
		}
	}

	// If the console is down we don't want the game to handle events.
	if (console_state != CONSOLE_STATE_INACTIVE) { events_locked = true; }
}

INLDEF void ConsoleRender ()
{
	int glyph_width_scaled = CAST(int, CAST(float, GetFontGlyphWidth(console_font)) * console_text_scale);
	int glyph_height_scaled = CAST(int, CAST(float, GetFontGlyphHeight(console_font)) * console_text_scale);

	int console_bottom = CAST(int, CAST(float, viewport.h) * console_current_height);
	int text_y = (console_bottom - glyph_height_scaled) - console_bottom_padding;

	// Draw the background boundary of the console.
	RenderRectFilled(0, 0, viewport.w, console_bottom, console_back_color);

	// Draws the input text at the bottom of the console.
	SetFontColor(console_font, console_input_color);
	RenderText(console_font, console_input.c_string, console_left_padding, text_y, console_text_scale, console_text_scale);
	// Draws the blinking cursor if it shouldn't currently be hidden.
	if (console_cursor.blink_timer <= console_cursor.blink_interval) {
		int cursor_x = console_left_padding + (glyph_width_scaled * console_cursor.position);
		RenderLine(cursor_x, text_y+2, cursor_x, (text_y + glyph_height_scaled)-4, console_input_color);
	}

	// Draws the console history text, line-by-line (bottom-to-top).
	SetFontColor(console_font, console_history_color);
	for (int i=console_history.count-1; i>=0; --i) {
		// If the bottom of the text is off-screen we know it is safe to stop drawing.
		if (text_y < 0) { break; }
		else {
			text_y -= glyph_height_scaled;
			RenderText(console_font, console_history[i].c_string, console_left_padding, text_y, console_text_scale, console_text_scale);
		}
	}
}

INLDEF void ConsoleUpdateAndRender (float _dt)
{
	// Do not update if the console is not meant to be enabled
	if (!console_enabled) { return; }
	ConsoleUpdate(_dt);

	// Do not render if the console is completely hidden.
	if (console_current_height <= 0.01f) { return; }
	ConsoleRender();
}

INLDEF void ClearConsole ()
{
	console_history.Clear();
	console_history_position = 0;
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
