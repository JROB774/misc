/*******************************************************************************
 * TEXTURE
 * Systems for loading, handling, and rendering textures.
 *
*******************************************************************************/

#ifndef __TCE_PRIMITIVE_H__ /*////////////////////////////////////////////////*/
#define __TCE_PRIMITIVE_H__

// @NOTE: SHOULD WE BOTHER ADDING ERROR HANDLING TO THIS???

namespace TCE
{

INLDEF void RenderPoint (int _x, int _y, const SDL_Color& _color);
INLDEF void RenderLine (int _x1, int _y1, int _x2, int _y2, const SDL_Color& _color);

INLDEF void RenderRectOutline (int _x, int _y, int _w, int _h, const SDL_Color& _color);
INLDEF void RenderRectFilled (int _x, int _y, int _w, int _h, const SDL_Color& _color);

} // TCE

#endif /* __TCE_PRIMITIVE_H__ ////////////////////////////////////////////////*/
