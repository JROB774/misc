/*******************************************************************************
 * VECTOR2D
 * Facilities for handling 2D vector mathematics.
 *
*******************************************************************************/

#ifndef __TCE_VECTOR2D_H__ /*//////////////////////////////////////////////////*/
#define __TCE_VECTOR2D_H__

namespace TCE
{

struct Vector2D
{
	float x, y;
};

INLDEF float    GetVectorLength  (const Vector2D& _vector);
INLDEF Vector2D NormalizeVector  (const Vector2D& _vector);
INLDEF Vector2D RotateVector     (const Vector2D& _vector, float _angle);
INLDEF float    VectorDotProduct (const Vector2D& _a, const Vector2D& _b);

} // TCE

#endif /* __TCE_VECTOR2D_H__ //////////////////////////////////////////////////*/
