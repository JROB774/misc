/*////////////////////////////////////////////////////////////////////////////*/

INLDEF void AddSplineNode (Spline& _spline, float _x, float _y)
{
	_spline.nodes.AddElement(Point { _x, _y });
	UpdateSplineShape(_spline);
}

STDDEF Point GetPointAlongSpline (const Spline& _spline, float _t)
{
	int p1 = (int)_t;
	int p2 = (p1 + 1) % _spline.nodes.count;
	int p3 = (p2 + 1) % _spline.nodes.count;
	int p0 = (p1 >= 1) ? p1 - 1 : _spline.nodes.count - 1;

	_t = _t - (int)_t;

	const float T_SQUARED = _t * _t;
	const float T_CUBED = _t * _t * _t;

	float q0 = -T_CUBED + 2.0f * T_SQUARED - _t;
	float q1 =  3.0f * T_CUBED - 5.0f * T_SQUARED + 2.0f;
	float q2 = -3.0f * T_CUBED + 4.0f * T_SQUARED + _t;
	float q3 =  T_CUBED - T_SQUARED;

	float tx = 0.5f * (_spline.nodes[p0].x * q0 + _spline.nodes[p1].x * q1 + _spline.nodes[p2].x * q2 + _spline.nodes[p3].x * q3);
	float ty = 0.5f * (_spline.nodes[p0].y * q0 + _spline.nodes[p1].y * q1 + _spline.nodes[p2].y * q2 + _spline.nodes[p3].y * q3);

	return Point { tx, ty };
}

STDDEF Point GetGradientAlongSpline (const Spline& _spline, float _t)
{
	int p1 = (int)_t;
	int p2 = (p1 + 1) % _spline.nodes.count;
	int p3 = (p2 + 1) % _spline.nodes.count;
	int p0 = (p1 >= 1) ? p1 - 1 : _spline.nodes.count - 1;

	_t = _t - (int)_t;

	const float T_SQUARED = _t * _t;

	float q0 = -3.0f * T_SQUARED + 4.0f * _t - 1.0f;
	float q1 =  9.0f * T_SQUARED - 10.0f * _t;
	float q2 = -9.0f * T_SQUARED + 8.0f * _t + 1.0f;
	float q3 =  3.0f * T_SQUARED - 2.0f * _t;

	float tx = 0.5f * (_spline.nodes[p0].x * q0 + _spline.nodes[p1].x * q1 + _spline.nodes[p2].x * q2 + _spline.nodes[p3].x * q3);
	float ty = 0.5f * (_spline.nodes[p0].y * q0 + _spline.nodes[p1].y * q1 + _spline.nodes[p2].y * q2 + _spline.nodes[p3].y * q3);

	return Point { tx, ty };
}

STDDEF void HandleSplineEvents (Spline& _spline)
{
	switch (event.type) {
		// Handle selection of nodes and adding new nodes to the spline.
		case (SDL_MOUSEBUTTONDOWN): {
			for (size_t i=0; i<_spline.nodes.count; ++i) {
				if (IsCursorOver((int)_spline.nodes[i].x, (int)_spline.nodes[i].y)) {
					_spline.selected_node = i;
				}
			}
			if (_spline.selected_node == SPLINE_NODE_UNSELECTED) {
				Point cursor_pos = GetScaledCursorPosition();
				AddSplineNode(_spline, cursor_pos.x, cursor_pos.y);
			}
		} break;
		case (SDL_MOUSEBUTTONUP): {
			_spline.selected_node = SPLINE_NODE_UNSELECTED;
		} break;
	}
}

INLDEF void UpdateSplineShape (Spline& _spline)
{
	// Update the vertices that make up the simplified spline.
	_spline.vertices.Resize(0);

	for (size_t i=0; i<_spline.nodes.count; ++i) {
		Point old_point = GetPointAlongSpline(_spline, (float)i);
		_spline.vertices.AddElement(old_point);

		Point new_point;
		for (float t=0.0f; t<1.0f; t+=spline_subdivide_step) {
			new_point = GetPointAlongSpline(_spline, (float)i + t);
			_spline.vertices.AddElement(new_point);
			old_point = new_point;
		}
	}
}

STDDEF void UpdateSpline (Spline& _spline)
{
	// Move the currently selected spline node, if there is one.
	if (_spline.selected_node == SPLINE_NODE_UNSELECTED) { return; }
	_spline.nodes[_spline.selected_node] += relative_cursor;

	// Update the vertices that make up the simplified spline.
	UpdateSplineShape(_spline);
}

STDDEF void RenderSpline (const Spline& _spline)
{
	// Draws the curve of the spline.
	for (float t=0.0f; t<(float)_spline.nodes.count; t+=0.005f) {
		Point point = GetPointAlongSpline(_spline, t);
		RenderPoint(point.x, point.y, SPLINE_CURVE_COLOR);
	}

	// Draws the subdivided line segments of the spline.
	for (size_t i=0; i<_spline.vertices.count; ++i) {
		if (i == _spline.vertices.count-1) {
			RenderLine(_spline.vertices[i].x, _spline.vertices[i].y, _spline.vertices[0].x, _spline.vertices[0].y, SPLINE_LINE_COLOR);
		} else {
			RenderLine(_spline.vertices[i].x, _spline.vertices[i].y, _spline.vertices[i+1].x, _spline.vertices[i+1].y, SPLINE_LINE_COLOR);
		}
	}

	// Draws the filled region of the spline.
	RenderPolygon(_spline.vertices, SPLINE_LINE_COLOR);

	// Draws the simplified vertices of the spline.
	for (size_t i=0; i<_spline.vertices.count; ++i) {
		RenderPoint(_spline.vertices[i].x, _spline.vertices[i].y, SPLINE_VERTEX_COLOR);
	}

	// Draws the nodes of the spline.
	for (size_t i=0; i<_spline.nodes.count; ++i) {
		RenderPoint(_spline.nodes[i].x, _spline.nodes[i].y, SPLINE_NODE_IDLE_COLOR);
	}
	if (_spline.selected_node != SPLINE_NODE_UNSELECTED) {
		int index = _spline.selected_node;
		RenderPoint(_spline.nodes[index].x, _spline.nodes[index].y, SPLINE_NODE_HOT_COLOR);
	}
}

/*////////////////////////////////////////////////////////////////////////////*/
