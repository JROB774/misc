/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void ClearEventState ()
{
	// Moves the current key state into the previous.
	memcpy(event_state.previous_key_state, event_state.current_key_state, SDL_NUM_SCANCODES * sizeof(u8));
	// Moves the current button state into the previous.
	memcpy(event_state.previous_button_state, event_state.current_button_state, SDL_BUTTON_X2 * sizeof(u8));
}

INLDEF void FillEventState ()
{
	// Determine what keys have been pressed and released during this frame.
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		event_state.current_key_state[event.key.keysym.scancode] = SDL_PRESSED;
	} else if (event.type == SDL_KEYUP) {
		event_state.current_key_state[event.key.keysym.scancode] = SDL_RELEASED;
	}

	// Determine what buttons have been pressed and released during this frame.
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		event_state.current_button_state[event.button.button-1] = SDL_PRESSED;
	} else if (event.type == SDL_MOUSEBUTTONUP) {
		event_state.current_button_state[event.button.button-1] = SDL_RELEASED;
	}
}

INLDEF void GetMousePosition (int& _x, int& _y)
{
	SDL_GetMouseState(&_x, &_y);

	_x = CAST(int, (CAST(float, _x  - viewport.x) / render_scale));
	_y = CAST(int, (CAST(float, _y  - viewport.y) / render_scale));

	if (render_offset_enabled) {
		_x += render_offset_x;
		_y += render_offset_y;
	}
}

INLDEF void GetRelativeMousePosition (int& _x, int & _y)
{
	SDL_GetRelativeMouseState(&_x, &_y);

	_x = CAST(int, (CAST(float, _x) / render_scale));
	_y = CAST(int, (CAST(float, _y) / render_scale));
}

INLDEF void GetGlobalMousePosition (int& _x, int& _y)
{
	SDL_GetGlobalMouseState(&_x, &_y);
}

INLDEF bool IsKeyDown (SDL_Scancode _scancode)
{
	// Do not attempt to handle out of range scancodes.
	if (_scancode < 0 || _scancode >= SDL_NUM_SCANCODES) { return false; }
	// If events are locked then always return false.
	if (events_locked) { return false; }

	return (event_state.current_key_state[_scancode] == 1);
}
INLDEF bool IsKeyUp (SDL_Scancode _scancode)
{
	// Do not attempt to handle out of range scancodes.
	if (_scancode < 0 || _scancode >= SDL_NUM_SCANCODES) { return false; }
	// If events are locked then always return false.
	if (events_locked) { return false; }

	return (event_state.current_key_state[_scancode] == 0);
}

INLDEF bool KeyPressed (SDL_Scancode _scancode)
{
	// Do not attempt to handle out of range scancodes.
	if (_scancode < 0 || _scancode >= SDL_NUM_SCANCODES) { return false; }
	// If events are locked then always return false.
	if (events_locked) { return false; }

	// If the key was up before but is now being held then the key was pressed.
	return (event_state.previous_key_state[_scancode] == 0 &&
		    event_state.current_key_state[_scancode] == 1);
}
INLDEF bool KeyReleased (SDL_Scancode _scancode)
{
	// Do not attempt to handle out of range scancodes.
	if (_scancode < 0 || _scancode >= SDL_NUM_SCANCODES) { return false; }
	// If events are locked then always return false.
	if (events_locked) { return false; }

	// If the key was being held before but is now up then the key was released.
	return (event_state.previous_key_state[_scancode] == 1 &&
		    event_state.current_key_state[_scancode] == 0);
}

INLDEF bool IsButtonDown (int _button)
{
	// Do not attempt to handle out of range buttons.
	if (_button < 0 || _button >= SDL_BUTTON_X2) { return 0; }
	// If events are locked then always return false.
	if (events_locked) { return false; }

	return (event_state.current_button_state[_button] == 1);
}
INLDEF bool IsButtonUp (int _button)
{
	// Do not attempt to handle out of range buttons.
	if (_button < 0 || _button >= SDL_BUTTON_X2) { return 0; }
	// If events are locked then always return false.
	if (events_locked) { return false; }

	return (event_state.current_button_state[_button] == 0);
}

INLDEF bool ButtonPressed (int _button)
{
	// Do not attempt to handle out of range buttons.
	if (_button < 0 || _button >= SDL_BUTTON_X2) { return 0; }
	// If events are locked then always return false.
	if (events_locked) { return false; }

	// If the button was up before but is now being held then the button was pressed.
	return (event_state.previous_button_state[_button] == 0 &&
		    event_state.current_button_state[_button] == 1);
}
INLDEF bool ButtonReleased (int _button)
{
	// Do not attempt to handle out of range buttons.
	if (_button < 0 || _button >= SDL_BUTTON_X2) { return 0; }
	// If events are locked then always return false.
	if (events_locked) { return false; }

	// If the button was being held before but is now up then the button was released.
	return (event_state.previous_button_state[_button] == 1 &&
		    event_state.current_button_state[_button] == 0);
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
