/*////////////////////////////////////////////////////////////////////////////*/

INLDEF void SetRenderColor (const SDL_Color& _color)
{
	SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
}

INLDEF void RenderPoint (float _x, float _y, const SDL_Color& _color)
{
	SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);
	SDL_RenderDrawPoint(renderer, (int)_x, (int)_y);
}

INLDEF void RenderLine (float _x1, float _y1, float _x2, float _y2, const SDL_Color& _color)
{
	// The reason we use a custom line draw routine rather than SDL_RenderDrawLine is
	// because the line does not take into consideration the current render scale. This
	// means that straight lines are scaled but diagonal lines are not and therefore
	// look out of place in a very pixelated application -- like this one.

	SDL_SetRenderDrawColor(renderer, _color.r, _color.g, _color.b, _color.a);

	int x1 = (int)_x1;
	int y1 = (int)_y1;
	int x2 = (int)_x2;
	int y2 = (int)_y2;

	bool steep = false;
	if (abs(x1 - x2) < abs(y1 - y2)) {
		Swap<int>(x1, y1);
		Swap<int>(x2, y2);
		steep = true;
	}

	if (x1 > x2) {
		Swap<int>(x1, x2);
		Swap<int>(y1, y2);
	}

	int dx = x2-x1;
	int dy = y2-y1;
	int derror2 = abs(dy) * 2;
	int error2 = 0;

	int y = y1;
	for (int x=x1; x<=x2; ++x) {
		if (steep) { SDL_RenderDrawPoint(renderer, y, x); }
		else { SDL_RenderDrawPoint(renderer, x, y); }
		error2 += derror2;
		if (error2 >= dx) {
			y += ((y2 > y1) ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

INLDEF void RenderPolygon (Array<Point> _vertices, const SDL_Color& _color)
{
	Point min = {0}, max = {0};
	for (size_t i=0; i<_vertices.count; ++i) {
		if (_vertices[i].x < min.x) { min.x = _vertices[i].x; }
		if (_vertices[i].x > max.x) { max.x = _vertices[i].x; }
		if (_vertices[i].y < min.y) { min.y = _vertices[i].y; }
		if (_vertices[i].y > max.y) { max.y = _vertices[i].y; }
	}

	Array<int> x_values;
	for (float scanline=(min.y+0.01f); scanline<=max.y; ++scanline) {
		for (size_t i=0; i<_vertices.count; ++i) {
			int x1 = (int)_vertices[i].x;
			int y1 = (int)_vertices[i].y;
			int x2 = (int)((i+1 >= _vertices.count) ? _vertices[0].x : _vertices[i+1].x);
			int y2 = (int)((i+1 >= _vertices.count) ? _vertices[0].y : _vertices[i+1].y);

			if (y2 < y1) {
				Swap<int>(x1, x2);
				Swap<int>(y1, y2);
			}

			if (scanline <= y2 && scanline >= y1) {
				int current_x;
				if ((y1 - y2) == 0) {
					current_x = x1;
				} else {
					current_x = ((x2 - x1) * ((int)scanline - y1)) / (y2 - y1);
					current_x += x1;
				}
				if (current_x <= max.x && current_x >= min.x) {
					x_values.AddElement(current_x);
				}
			}
		}

		for (size_t i=0; i<x_values.count; i+=2) {
			RenderLine((float)x_values[i], scanline, (float)x_values[i+1], scanline, _color);
		}

		x_values.Resize(0);
	}
}

/*////////////////////////////////////////////////////////////////////////////*/
