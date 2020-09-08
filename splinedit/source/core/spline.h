#ifndef __SPLINE_H__ /*///////////////////////////////////////////////////////*/
#define __SPLINE_H__

GLOBAL constexpr int SPLINE_NODE_UNSELECTED = -1;

GLOBAL constexpr SDL_Color SPLINE_LINE_COLOR      = { 0x00,0x00,0xAF,0xFF };
GLOBAL constexpr SDL_Color SPLINE_CURVE_COLOR     = { 0x00,0x00,0x3F,0xFF };
GLOBAL constexpr SDL_Color SPLINE_NODE_IDLE_COLOR = { 0xFF,0xFF,0x00,0xFF };
GLOBAL constexpr SDL_Color SPLINE_NODE_HOT_COLOR  = { 0xFF,0xFF,0xFF,0xFF };
GLOBAL constexpr SDL_Color SPLINE_VERTEX_COLOR    = { 0x6F,0x6F,0x00,0xFF };

GLOBAL float spline_subdivide_step = 0.25f;

struct Spline
{
 	Array<Point> nodes;
 	int selected_node = SPLINE_NODE_UNSELECTED;

 	// This is the simplified version of the spline made up of straight lines between
 	// vertices rather than smooth curves between nodes. We cache this data so that
 	// it can easily be passed to the polygonal filling function in a proper form.
 	//
 	// This is updated every time a node for the spline is moved by the user.
 	Array<Point> vertices;
};

INLDEF void AddSplineNode (Spline& _spline, float _x, float _y);

STDDEF Point GetPointAlongSpline (const Spline& _spline, float _t);
STDDEF Point GetGradientAlongSpline (const Spline& _spline, float _t);

STDDEF void HandleSplineEvents (Spline& _spline);

INLDEF void UpdateSplineShape (Spline& _spline);
STDDEF void UpdateSpline (Spline& _spline);

STDDEF void RenderSpline (const Spline& _spline);

#endif /* __SPLINE_H__ ///////////////////////////////////////////////////////*/
