/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF void RenderPoint (int _x, int _y, const SDL_Color& _color)
{
	if (render_offset_enabled) {
		_x -= render_offset_x,
		_y -= render_offset_y;
	}

	SetRendererColor(_color);
	SDL_RenderDrawPoint(renderer, _x, _y);
}

INLDEF void RenderLine (int _x1, int _y1, int _x2, int _y2, const SDL_Color& _color)
{
	// The reason we use a custom line draw routine rather than SDL_RenderDrawLine is
	// because the line does not take into consideration the current render scale. This
	// means that straight lines are scaled but diagonal lines are not and therefore
	// look out of place in a very pixelated application -- like this one.

	if (render_offset_enabled) {
		_x1 -= render_offset_x;
		_y1 -= render_offset_y;
		_x2 -= render_offset_x;
		_y2 -= render_offset_y;
	}

	SetRendererColor(_color);

	bool steep = false;
	if (abs(_x1 - _x2) < abs(_y1 - _y2)) {
		Swap<int>(_x1, _y1);
		Swap<int>(_x2, _y2);
		steep = true;
	}

	if (_x1 > _x2) {
		Swap<int>(_x1, _x2);
		Swap<int>(_y1, _y2);
	}

	int dx = _x2-_x1;
	int dy = _y2-_y1;
	int derror2 = abs(dy) * 2;
	int error2 = 0;

	int y = _y1;
	for (int x=_x1; x<=_x2; ++x) {
		if (steep) { SDL_RenderDrawPoint(renderer, y, x); }
		else { SDL_RenderDrawPoint(renderer, x, y); }
		error2 += derror2;
		if (error2 >= dx) {
			y += ((_y2 > _y1) ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

INLDEF void RenderRectOutline (int _x, int _y, int _w, int _h, const SDL_Color& _color)
{
	if (render_offset_enabled) {
		_x -= render_offset_x,
		_y -= render_offset_y;
	}

	SDL_Rect rect = { _x, _y, _w, _h };

	SetRendererColor(_color);
	SDL_RenderDrawRect(renderer, &rect);
}

INLDEF void RenderRectFilled (int _x, int _y, int _w, int _h, const SDL_Color& _color)
{
	if (render_offset_enabled) {
		_x -= render_offset_x,
		_y -= render_offset_y;
	}

	SDL_Rect rect = { _x, _y, _w, _h };

	SetRendererColor(_color);
	SDL_RenderFillRect(renderer, &rect);
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
