/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

FILDEF float get_window_w ()
{
    RECT rect;
    GetClientRect(window, &rect);
    return CAST(float, rect.right);
}
FILDEF float get_window_h ()
{
    RECT rect;
    GetClientRect(window, &rect);
    return CAST(float, rect.bottom);
}

FILDEF void orthographic (mat4& m, float l, float r, float b, float t, float n, float f)
{
    identity(m);

    m[0][0] =    2.0f  / (r - l);
    m[1][1] =    2.0f  / (t - b);
    m[2][2] =    2.0f  / (f - n);
    m[3][0] =  (r + l) / (r - l);
    m[3][1] =  (t + b) / (t - b);
    m[3][2] = -(f + n) / (f - n);
}
FILDEF void orthographic (mat4& m, float l, float r, float b, float t)
{
    orthographic(m, l, r, b, t, 0.0f, 1.0f);
}

FILDEF void identity (mat4& m)
{
    m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
    m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
    m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
    m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
}

FILDEF void multiply (mat4& a, const mat4& b)
{
    mat4 r = {};
    for (int n=0; n<4; ++n) {
        for (int p=0; p<4; ++p) {
            for (int m=0; m<4; ++m) {
                r[n][p] += a[n][m] * b[m][p];
            }
        }
    }
    memcpy(&a, &r, sizeof(mat4));
}

FILDEF void scale (mat4& m, float x, float y)
{
    m[0][0] = x;
    m[1][1] = y;
    m[2][2] = 0;
}

FILDEF void rotate (mat4& m, float x, float y, float z, float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);

    mat4 r;

    r[0][0] = c + (1 - c)     * x     * x;
    r[0][1] =     (1 - c) * x * y + s * z;
    r[0][2] =     (1 - c) * x * z - s * y;
    r[0][3] = 0;

    r[1][0] =     (1 - c) * y * x - s * z;
    r[1][1] = c + (1 - c) * y * y;
    r[1][2] =     (1 - c) * y * z + s * x;
    r[1][3] = 0;

    r[2][0] =     (1 - c) * z * x + s * y;
    r[2][1] =     (1 - c) * z * y - s * x;
    r[2][2] = c + (1 - c) * z * z;
    r[2][3] = 0;

    r[3][0] = 0;
    r[3][1] = 0;
    r[3][2] = 0;
    r[3][3] = 1;

    multiply(m, r);
}

FILDEF void translate (mat4& m, float x, float y)
{
    m[0][3] = x;
    m[1][3] = y;
    m[2][3] = 0;
}

FILDEF void clear_screen (float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/
