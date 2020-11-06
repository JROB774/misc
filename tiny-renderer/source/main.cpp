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
#include <chrono> // For timing renders.
#include <iostream>

typedef uint32_t RGBAColor;

// Some useful macros for converting separate components into a U32 ARGB color.
#define MAKE_RGBA(r,g,b,a) ((RGBAColor)((((a   )<<24)|((b)<<16)|((g)<<8)|(r))))
#define MAKE_RGB( r,g,b)   ((RGBAColor)((((0xFF)<<24)|((b)<<16)|((g)<<8)|(r))))

static constexpr int RENDERBUFFER_WIDTH = 200;
static constexpr int RENDERBUFFER_HEIGHT = 200;

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

static void draw_triangle (Vec2i t0, Vec2i t1, Vec2i t2, RGBAColor color)
{
    draw_line(t0.x,t0.y, t1.x,t1.y, color);
    draw_line(t1.x,t1.y, t2.x,t2.y, color);
    draw_line(t2.x,t2.y, t0.x,t0.y, color);
}

static void fill_triangle (Vec2i t0, Vec2i t1, Vec2i t2, RGBAColor color)
{
    // No height triangles are discarded.
    if (t0.y == t1.y && t0.y == t2.y) return;

    // Sort the vertices so t0 is lowest and t2 is highest.
    if (t0.y > t1.y) std::swap(t0, t1);
    if (t0.y > t2.y) std::swap(t0, t2);
    if (t1.y > t2.y) std::swap(t1, t2);

    int total_height = t2.y-t0.y;

    for (int i=0; i<total_height; ++i)
    {
        // We're in the second half if we're past the middle vertex or the triangle has a flat top.
        bool in_second_half = ((i > t1.y-t0.y) || (t1.y == t0.y));
        int segment_height = (in_second_half) ? t2.y-t1.y : t1.y-t0.y; // Calculate the height of either the top or bottom segment.

        // Get the points across the slopes of each of the two vertical lines.
        float alpha = (float)i/total_height;
        float beta = (float)(i-((in_second_half) ? t1.y-t0.y : 0)) / segment_height;
        Vec2i a = t0 + (t2-t0) * alpha;
        Vec2i b = (in_second_half) ? t1 + (t2-t1) * beta : t0 + (t1-t0) * beta;
        if (a.x > b.x) std::swap(a,b);

        // Draw the horizontal line between the two points.
        for (int j=a.x; j<=b.x; ++j)
        {
            draw_pixel(j,t0.y+i, color);
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

static void render ()
{
    Vec2i t0[3] = {Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80)};
    Vec2i t1[3] = {Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180)};
    Vec2i t2[3] = {Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180)};

    draw_clear(MAKE_RGB(0,0,0));

    fill_triangle(t0[0], t0[1], t0[2], MAKE_RGB(255,0,0));
    fill_triangle(t1[0], t1[1], t1[2], MAKE_RGB(0,255,0));
    fill_triangle(t2[0], t2[1], t2[2], MAKE_RGB(0,0,255));
}

int main (int argc, char** argv)
{
    auto render_start = std::chrono::high_resolution_clock::now();

    render();

    auto render_end = std::chrono::high_resolution_clock::now();
    auto render_duration = std::chrono::duration_cast<std::chrono::microseconds>(render_end-render_start);
    std::cout << "Render Took: " << render_duration.count() << "ms" << std::endl;

    // We don't include the write to disk as part of the render.
    draw_display();

    return 0;
}
