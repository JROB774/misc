#ifndef __RENDER_H__ /*///////////////////////////////////////////////////////*/
#define __RENDER_H__

GLOBAL constexpr SDL_Color WHITE       = { 0xFF,0xFF,0xFF,0xFF };
GLOBAL constexpr SDL_Color BLACK       = { 0x00,0x00,0x00,0xFF };
GLOBAL constexpr SDL_Color GREY        = { 0x7F,0x7F,0x7F,0xFF };
GLOBAL constexpr SDL_Color DARK_GREY   = { 0x20,0x20,0x20,0xFF };
GLOBAL constexpr SDL_Color RED         = { 0xFF,0x00,0x00,0xFF };
GLOBAL constexpr SDL_Color DARK_RED    = { 0x7F,0x00,0x00,0xFF };
GLOBAL constexpr SDL_Color GREEN       = { 0x00,0xFF,0x00,0xFF };
GLOBAL constexpr SDL_Color DARK_GREEN  = { 0x00,0x7F,0x00,0xFF };
GLOBAL constexpr SDL_Color BLUE        = { 0x00,0x00,0xFF,0xFF };
GLOBAL constexpr SDL_Color DARK_BLUE   = { 0x00,0x00,0x7F,0xFF };
GLOBAL constexpr SDL_Color YELLOW      = { 0xFF,0xFF,0x00,0xFF };
GLOBAL constexpr SDL_Color DARK_YELLOW = { 0x7F,0x7F,0x00,0xFF };

// We have this wrapper just because it is inconvenient to pass each
// individual color component when setting SDL's renderer draw color.
INLDEF void SetRenderColor (const SDL_Color& _color);

INLDEF void RenderPoint (float _x, float _y, const SDL_Color& _color);
INLDEF void RenderLine (float _x1, float _y1, float _x2, float _y2, const SDL_Color& _color);

INLDEF void RenderPolygon (Array<Point> _vertices, const SDL_Color& _color);

#endif /* __RENDER_H__ ///////////////////////////////////////////////////////*/
