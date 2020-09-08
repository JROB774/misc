/*******************************************************************************
 * PATHFIND
 * Implementation of A* algorithm for pathfinding in the game.
 *
*******************************************************************************/

#ifndef __TCE_PATHFIND_H__ /*/////////////////////////////////////////////////*/
#define __TCE_PATHFIND_H__

namespace TCE
{
struct PathNode
{
	static constexpr int DIRECTION_NONE = -1;

	Point direction;
	bool visited;

	PathNode(); // Because C++ will make and call one anyway.
};

struct Pathfinder
{
	Array<Point> frontier;
	Array<PathNode> node_map;
	Array<Point> path;

	Point start;
	Point end;

	const Zone* zone;
};

INLDEF void CreatePathfinder (Pathfinder& _pathfinder, Point _start, Point _end, const Zone& _zone);
INLDEF void CheckFrontierNeighbour (Pathfinder& _pathfinder, Point _current, int _xoff, int _yoff);

STDDEF Array<Point> FindPath (Point _start, Point _end);

} // TCE

#endif /* __TCE_PATHFIND_H__ /////////////////////////////////////////////////*/
