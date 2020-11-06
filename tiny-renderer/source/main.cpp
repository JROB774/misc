#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "external/stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IAMGE_STATIC
#include "external/stb_image.h"

#include "external/geometry.hpp"
#include "external/model.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <algorithm>

typedef uint32_t RGBAColor;

// Some useful macros for converting separate components into a U32 ARGB color.
#define MAKE_RGBA(r,g,b,a) ((RGBAColor)((((a   )<<24)|((b)<<16)|((g)<<8)|(r))))
#define MAKE_RGB( r,g,b)   ((RGBAColor)((((0xFF)<<24)|((b)<<16)|((g)<<8)|(r))))

static constexpr int RENDERBUFFER_WIDTH = 800;
static constexpr int RENDERBUFFER_HEIGHT = 800;

static RGBAColor gRenderbuffer[RENDERBUFFER_WIDTH*RENDERBUFFER_HEIGHT];

static void draw_clear (RGBAColor color)
{
    for (int i=0; i<(RENDERBUFFER_WIDTH*RENDERBUFFER_HEIGHT); ++i)
    {
        gRenderbuffer[i] = color;
    }
}

static void draw_display ()
{
    stbi_flip_vertically_on_write(true);
    stbi_write_png("output.png", RENDERBUFFER_WIDTH,RENDERBUFFER_HEIGHT, sizeof(RGBAColor), gRenderbuffer, RENDERBUFFER_WIDTH*sizeof(RGBAColor));
}

static void draw_pixel (int x, int y, RGBAColor color)
{
    if (x >= 0 && x < RENDERBUFFER_WIDTH && y >= 0 && y < RENDERBUFFER_HEIGHT)
    {
        gRenderbuffer[y * RENDERBUFFER_WIDTH + x] = color;
    }
}

static void draw_line (int x0, int y0, int x1, int y1, RGBAColor color)
{
    bool steep = false;
    if (abs(x0-x1) < abs(y0-y1))
    {
        std::swap(x0,y0);
        std::swap(x1,y1);
        steep = true;
    }

    if (x0 > x1)
    {
        std::swap(x0,x1);
        std::swap(y0,y1);
    }

    int dx = x1-x0;
    int dy = y1-y0;

    int derror2 = abs(dy)*2;
    int error2 = 0;

    int y = y0;
    for (int x=x0; x<=x1; ++x)
    {
        if (steep)
        {
            draw_pixel(y,x,color);
        }
        else
        {
            draw_pixel(x,y,color);
        }

        error2 += derror2;
        if (error2 > dx)
        {
            y += ((y1 > y0) ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

static void draw_model (Model& model, RGBAColor color)
{
    for (int i=0; i<model.nfaces(); ++i)
    {
        std::vector<int> face = model.face(i);
        for (int j=0; j<3; ++j)
        {
            Vec3f v0 = model.vert(face[ j     ]);
            Vec3f v1 = model.vert(face[(j+1)%3]);

            // Convert normalized coordinates into screen coordinates.
            int x0 = (v0.x + 1.0f) * (float)RENDERBUFFER_WIDTH  / 2.0f;
            int y0 = (v0.y + 1.0f) * (float)RENDERBUFFER_HEIGHT / 2.0f;
            int x1 = (v1.x + 1.0f) * (float)RENDERBUFFER_WIDTH  / 2.0f;
            int y1 = (v1.y + 1.0f) * (float)RENDERBUFFER_HEIGHT / 2.0f;

            draw_line(x0,y0, x1,y1, color);
        }
    }
}

int main (int argc, char** argv)
{
    Model model("assets/african_head.obj");

    draw_clear(MAKE_RGB(0,0,0));
    draw_model(model, MAKE_RGB(255,255,255));
    draw_display();

    return 0;
}
