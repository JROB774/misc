#ifndef UTILITY_HPP__ /*//////////////////////////////////////////////////////*/
#define UTILITY_HPP__

#ifdef COMPILER_HAS_PRAGMA_ONCE
#pragma once
#endif

/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

FILDEF float get_window_w ();
FILDEF float get_window_h ();

FILDEF void orthographic (mat4& m, float l, float r, float b, float t, float n, float f);
FILDEF void orthographic (mat4& m, float l, float r, float b, float t);
FILDEF void identity     (mat4& m);
FILDEF void multiply     (mat4& a, const mat4& b);
FILDEF void scale        (mat4& m, float x, float y);
FILDEF void rotate       (mat4& m, float x, float y, float z, float angle);
FILDEF void translate    (mat4& m, float x, float y);

FILDEF void clear_screen (float r, float g, float b, float a);

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/

#endif /* UTILITY_HPP__ //////////////////////////////////////////////////////*/
