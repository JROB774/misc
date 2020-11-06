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

static void split_rgba (RGBAColor color, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a)
{
    r = (color    ) & 0xFF;
    g = (color>> 8) & 0xFF;
    b = (color>>16) & 0xFF;
    a = (color>>24) & 0xFF;
}
static void split_rgb (RGBAColor color, uint8_t& r, uint8_t& g, uint8_t& b)
{
    r = (color    ) & 0xFF;
    g = (color>> 8) & 0xFF;
    b = (color>>16) & 0xFF;
}

static constexpr int SCREEN_WIDTH = 800;
static constexpr int SCREEN_HEIGHT = 800;

static RGBAColor gRenderbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];
static float gDepthbuffer[SCREEN_WIDTH*SCREEN_HEIGHT];

static void draw_clear (RGBAColor color)
{
    for (int i=0; i<(SCREEN_WIDTH*SCREEN_HEIGHT); ++i)
    {
        gRenderbuffer[i] = color;
    }
}

static void draw_display ()
{
    stbi_flip_vertically_on_write(true);
    stbi_write_png("output.png", SCREEN_WIDTH,SCREEN_HEIGHT, sizeof(RGBAColor), gRenderbuffer, SCREEN_WIDTH*sizeof(RGBAColor));
}

static void draw_pixel (int x, int y, RGBAColor color)
{
    if (x >= 0 && x < SCREEN_WIDTH && y >= 0 && y < SCREEN_HEIGHT)
    {
        gRenderbuffer[y * SCREEN_WIDTH + x] = color;
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
    Vec2i pts[3] = { t0,t1,t2 };
    Vec2i p;

    auto barycentric = [&]()
    {
        Vec3f a = Vec3f(pts[2].raw[0]-pts[0].raw[0], pts[1].raw[0]-pts[0].raw[0], pts[0].raw[0]-p.raw[0]);
        Vec3f b = Vec3f(pts[2].raw[1]-pts[0].raw[1], pts[1].raw[1]-pts[0].raw[1], pts[0].raw[1]-p.raw[1]);
        Vec3f u = a^b;
        if (abs(u.raw[2]) < 1) return Vec3f(-1,1,1); // Triangle is degnerate so return negative for discard.
        return Vec3f(1.0f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    };

    Vec2i bmin(SCREEN_WIDTH-1,SCREEN_HEIGHT-1);
    Vec2i bmax(0,0);

    Vec2i clamp(SCREEN_WIDTH-1,SCREEN_HEIGHT-1);

    for (int i=0; i<3; ++i)
    {
        for (int j=0; j<2; ++j)
        {
            bmin.raw[j] = std::max(0, std::min(bmin.raw[j], pts[i].raw[j]));
            bmax.raw[j] = std::min(clamp.raw[j], std::max(bmax.raw[j], pts[i].raw[j]));
        }
    }

    for (p.x=bmin.x; p.x<=bmax.x; ++p.x)
    {
        for (p.y=bmin.y; p.y<=bmax.y; ++p.y)
        {
            Vec3f baryscreen = barycentric();
            if (baryscreen.x < 0 || baryscreen.y < 0 || baryscreen.z < 0) continue;
            draw_pixel(p.x,p.y, color);
        }
    }
}

static void draw_model (Model& model, RGBAColor color)
{
    for (int i=0; i<model.nfaces(); ++i)
    {
        std::vector<int> face = model.face(i);
        Vec2i screen_coords[3];
        for (int j=0; j<3; j++)
        {
            Vec3f world_coords = model.vert(face[j]);
            screen_coords[j].x = (world_coords.x + 1.0f) * (float)SCREEN_WIDTH  / 2.0f;
            screen_coords[j].y = (world_coords.y + 1.0f) * (float)SCREEN_HEIGHT / 2.0f;
        }
        draw_triangle(screen_coords[0], screen_coords[1], screen_coords[2], color);
    }
}

static void fill_model (Model& model, RGBAColor color)
{
    // Light is shooting out forward from the camera/screen.
    Vec3f light_dir(0.0f,0.0f,-1.0f);

    for (int i=0; i<model.nfaces(); ++i)
    {
        std::vector<int> face = model.face(i);

        // Get the screen and world coordinates for a singble face.
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j=0; j<3; j++)
        {
            Vec3f v = model.vert(face[j]);
            screen_coords[j] = Vec2i((v.x+1)*(float)SCREEN_WIDTH/2, (v.y+1)*(float)SCREEN_HEIGHT/2);
            world_coords[j] = v;
        }

        // Calculate the face normal and determine light level using this.
        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        float intensity = n * light_dir;

        // Performs back-face culling. If the intensity is negative it means the face normal is
        // facing away from camera and therefore does not and should not be rendered to the scene.
        if (intensity > 0.0f)
        {
            uint8_t r,g,b; split_rgb(color, r,g,b);
            fill_triangle(screen_coords[0], screen_coords[1], screen_coords[2], MAKE_RGB((int)((float)r*intensity),(int)((float)g*intensity),(int)((float)b*intensity)));
        }
    }
}

static void render ()
{
    Model model("assets/african_head.obj");
    draw_clear(MAKE_RGB(0,0,0));
    fill_model(model, MAKE_RGB(255,255,255));
}

int main (int argc, char** argv)
{
    auto render_start = std::chrono::high_resolution_clock::now();

    render();

    auto render_end = std::chrono::high_resolution_clock::now();
    auto render_duration = std::chrono::duration_cast<std::chrono::milliseconds>(render_end-render_start);
    std::cout << "Render Took: " << render_duration.count() << "ms" << std::endl;

    // We don't include the write to disk as part of the render.
    draw_display();

    return 0;
}
