/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF bool PointAndBoxCollision (int _x, int _y, const SDL_Rect& _box)
{
	return ((_x >= _box.x) && (_x < (_box.x + _box.w)) &&
		    (_y >= _box.y) && (_y < (_box.y + _box.h)));
}

INLDEF bool BoxAndBoxCollision (const SDL_Rect& _a, const SDL_Rect& _b)
{
	return ((_a.x < (_b.x + _b.w)) && ((_a.x + _a.w) > _b.x) &&
		    (_a.y < (_b.y + _b.h)) && ((_a.y + _a.h) > _b.y));
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
