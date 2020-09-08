/*////////////////////////////////////////////////////////////////////////////*/

INLDEF Point GetScaledCursorPosition ()
{
	int x, y;
	SDL_GetMouseState(&x, &y);

	Point cursor_pos;
	cursor_pos.x = (float)x / WINDOW_SCALE;
	cursor_pos.y = (float)y / WINDOW_SCALE;

	return cursor_pos;
}
INLDEF Point GetRelativeScaledCursorPosition ()
{
	int x, y;
	SDL_GetRelativeMouseState(&x, &y);

	Point cursor_pos;
	cursor_pos.x = (float)x / WINDOW_SCALE;
	cursor_pos.y = (float)y / WINDOW_SCALE;

	return cursor_pos;
}

INLDEF bool IsCursorOver (int _x, int _y)
{
	Point cursor_pos = GetScaledCursorPosition();
	return ((int)cursor_pos.x == _x && (int)cursor_pos.y == _y);
}
INLDEF bool IsCursorOver (int _x, int _y, int _w, int _h)
{
	Point cursor_pos = GetScaledCursorPosition();
	return ((int)cursor_pos.x >= _x       && (int)cursor_pos.y >= _y &&
		    (int)cursor_pos.x < (_x + _w) && (int)cursor_pos.y < (_y + _h));
}

/*////////////////////////////////////////////////////////////////////////////*/
