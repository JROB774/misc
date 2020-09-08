/*////////////////////////////////////////////////////////////////////////////*/

template<typename T> INLDEF void Swap (T& _a, T& _b)
{
	T temp = _a;

	_a = _b;
	_b = temp;
}

Point& Point::operator+= (const Point& _rhs)
{
	x += _rhs.x;
	y += _rhs.y;

	return *this;
}
Point& Point::operator-= (const Point& _rhs)
{
	x -= _rhs.x;
	y -= _rhs.y;

	return *this;
}
Point& Point::operator*= (const Point& _rhs)
{
	x *= _rhs.x;
	y *= _rhs.y;

	return *this;
}
Point& Point::operator/= (const Point& _rhs)
{
	x /= _rhs.x;
	y /= _rhs.y;

	return *this;
}

INLDEF Point operator+ (Point _lhs, const Point& _rhs)
{
	_lhs += _rhs;
	return _lhs;
}
INLDEF Point operator- (Point _lhs, const Point& _rhs)
{
	_lhs -= _rhs;
	return _lhs;
}
INLDEF Point operator* (Point _lhs, const Point& _rhs)
{
	_lhs *= _rhs;
	return _lhs;
}
INLDEF Point operator/ (Point _lhs, const Point& _rhs)
{
	_lhs /= _rhs;
	return _lhs;
}

/*////////////////////////////////////////////////////////////////////////////*/
