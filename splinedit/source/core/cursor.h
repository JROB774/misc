#ifndef __CURSOR_H__ /*///////////////////////////////////////////////////////*/
#define __CURSOR_H__

// This is a global so that everyone has access to it and we just update it during every
// loop so that the relative position doesnt vary wildly between calls (causing issues).
GLOBAL Point relative_cursor;

// Because of the render scaling we want the cursor position to also be scaled.
INLDEF Point GetScaledCursorPosition ();
INLDEF Point GetRelativeScaledCursorPosition ();

INLDEF bool IsCursorOver (int _x, int _y);
INLDEF bool IsCursorOver (int _x, int _y, int _w, int _h);

#endif /* __CURSOR_H__ ///////////////////////////////////////////////////////*/
