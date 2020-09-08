/*******************************************************************************
 * COLLISION
 * Functions for checking collision between different primitives.
 *
*******************************************************************************/

#ifndef __TCE_COLLISION_H__ /*////////////////////////////////////////////////*/
#define __TCE_COLLISION_H__

namespace TCE
{

INLDEF bool PointAndBoxCollision (int _x, int _y, const SDL_Rect& _box);
INLDEF bool BoxAndBoxCollision (const SDL_Rect& _a, const SDL_Rect& _b);

} // TCE

#endif /* __TCE_COLLISION_H__ ////////////////////////////////////////////////*/
