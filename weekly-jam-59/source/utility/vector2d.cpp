/*////////////////////////////////////////////////////////////////////////////*/

namespace TCE
{

INLDEF float GetVectorLength (const Vector2D& _vector)
{
	return sqrtf((_vector.x * _vector.x) + (_vector.y * _vector.y));
}

INLDEF Vector2D NormalizeVector (const Vector2D& _vector)
{
	float length = GetVectorLength(_vector);
	return Vector2D { _vector.x / length, _vector.y / length };
}

INLDEF Vector2D RotateVector (const Vector2D& _vector, float _angle)
{
	float c = cosf(_angle);
	float s = sinf(_angle);

	Vector2D rotated;
	rotated.x = _vector.x * c - _vector.y * s;
	rotated.y = _vector.x * s + _vector.y * c;
	return rotated;
}

INLDEF float VectorDotProduct (const Vector2D& _a, const Vector2D& _b)
{
	return (_a.x * _b.x) + (_a.y * _b.y);
}

} // TCE

/*////////////////////////////////////////////////////////////////////////////*/
