/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

PathNode::PathNode ()
{
	direction = { DIRECTION_NONE, DIRECTION_NONE };
	visited = false;
}

INLDEF void CreatePathfinder (Pathfinder& _pathfinder, Point _start, Point _end, const Zone& _zone)
{
	_pathfinder.start = _start;
	_pathfinder.end = _end;
	_pathfinder.zone = &_zone;

	// Adds the starting position as the beginning of the frontier.
	_pathfinder.frontier.AddElement(_start);
	// Preallocates enough space to map out the entire zone provided.
	size_t zone_size = _zone.width * _zone.height;
	_pathfinder.node_map.Resize(zone_size);
	_pathfinder.node_map.count = zone_size;

	// The starting point should be checked as already visited.
	_pathfinder.node_map[_start.y * _zone.width + _start.x].visited = true;
}

INLDEF void CheckFrontierNeighbour (Pathfinder& _pathfinder, Point _current, int _xoff, int _yoff)
{
	size_t index = (_current.y + _yoff) * current_zone.width + (_current.x + _xoff);
	if (!_pathfinder.node_map[index].visited) { // If we haven't already visited this node.
		Point direction = { PathNode::DIRECTION_NONE, PathNode::DIRECTION_NONE };
		// Make sure we do not bother checking and adding solid tiles to the node map.
		if (_pathfinder.zone->tile_data[index].id & TILE_TYPE_FLOOR) {
			_pathfinder.frontier.AddElement(Point { _current.x + _xoff, _current.y + _yoff });
			direction = { _current.x, _current.y };
		}
		_pathfinder.node_map[index].direction = direction;
		_pathfinder.node_map[index].visited = true;
	}
}

STDDEF Array<Point> FindPath (Point _start, Point _end)
{
	// Creates a new pathfinder to creatw a path from start to end.
	Pathfinder pathfinder;
	CreatePathfinder(pathfinder, _start, _end, current_zone);

	// Construct a node map until completion or the end point is found.
	while (pathfinder.frontier.count > 0) {
		// Pops the element from the front of the frontier "queue".
		Point current = pathfinder.frontier[0];
		pathfinder.frontier.RemoveElementAt(0);

		// We do not want to attempt to access outside of the map bounds.
		if (current.y > 0)                             { CheckFrontierNeighbour(pathfinder, current,  0, -1); }
		if (current.x < (pathfinder.zone->width - 1))  { CheckFrontierNeighbour(pathfinder, current,  1,  0); }
		if (current.y < (pathfinder.zone->height - 1)) { CheckFrontierNeighbour(pathfinder, current,  0,  1); }
		if (current.x > 0)                             { CheckFrontierNeighbour(pathfinder, current, -1,  0); }

		// Break early if we found our destination.
		if (current == _end) { break; }
	}

	// Now we can construct a path backwards from the goal to the start.
	Point current = _end;
	while (current != _start) {
		pathfinder.path.AddElement(current);
		size_t index = current.y * pathfinder.zone->width + current.x;
		current = pathfinder.node_map[index].direction;
	}
	pathfinder.path.AddElement(_start);

	return pathfinder.path;
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
