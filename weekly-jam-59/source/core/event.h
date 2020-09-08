/*******************************************************************************
 * EVENT
 * Stores events (mouse, keyboard, gamepad) for the game to respond to.
 *
*******************************************************************************/

#ifndef __TCE_EVENT_H__ /*////////////////////////////////////////////////////*/
#define __TCE_EVENT_H__

namespace TCE
{

struct EventState
{
	u8 previous_key_state[SDL_NUM_SCANCODES];
	u8 current_key_state[SDL_NUM_SCANCODES];

	u8 previous_button_state[SDL_BUTTON_X2];
	u8 current_button_state[SDL_BUTTON_X2];
};

GLOBAL EventState event_state;
GLOBAL bool events_locked = false;

INLDEF void FlipEventState ();
INLDEF void FillEventState ();

INLDEF void GetMousePosition (int& _x, int& _y);
INLDEF void GetRelativeMousePosition (int& _x, int & _y);
INLDEF void GetGlobalMousePosition (int& _x, int& _y);

INLDEF bool IsKeyDown (SDL_Scancode _scancode);
INLDEF bool IsKeyUp (SDL_Scancode _scancode);

INLDEF bool KeyPressed (SDL_Scancode _scancode);
INLDEF bool KeyReleased (SDL_Scancode _scancode);

INLDEF bool IsButtonDown (int _button);
INLDEF bool IsButtonUp (int _button);

INLDEF bool ButtonPressed (int _button);
INLDEF bool ButtonReleased (int _button);

} // TCE

#endif /* __TCE_EVENT_H__ ////////////////////////////////////////////////////*/
