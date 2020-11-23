/*////////////////////////////////////////////////////////////////////////////*/

/* -------------------------------------------------------------------------- */

#define OPENGL_LOAD_GL_VERSION_MAJOR 3
#define OPENGL_LOAD_GL_VERSION_MINOR 3

#define OPENGL_INCLUDE_WGL_ARB_create_context
#define OPENGL_INCLUDE_WGL_EXT_swap_control

#include <entry.hpp>
#include <basic.hpp>
#include <math.hpp>
#include <opengl.hpp>
#include <defer.hpp>

/* -------------------------------------------------------------------------- */

GLOBAL bool running;
GLOBAL bool input;

GLOBAL HWND window;

/* -------------------------------------------------------------------------- */

// @Incomplete: Clean up and solidify all these other files (error handling, etc.)

#include "tga_loader.hpp"
#include "tga_loader.cpp"

#include "shader.hpp"
#include "shader.cpp"

#include "texture.hpp"
#include "texture.cpp"

#include "vertex_buffer.hpp"
#include "vertex_buffer.cpp"

#include "utility.hpp"
#include "utility.cpp"

/* -------------------------------------------------------------------------- */

// Code for the specific screensaver that is to be used by this app base.
#include "screensaver.cpp"

/* -------------------------------------------------------------------------- */

// NOTE: This macro should only be invoked from the main program function.
#define FATAL_ERROR(msg)                                \
do                                                      \
{                                                       \
MessageBoxA(window, msg, "Error!", MB_OK|MB_ICONERROR); \
return 1;                                               \
}                                                       \
while (0,0)

/* -------------------------------------------------------------------------- */

LRESULT CALLBACK window_proc (HWND hwindow, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
        // Pressing any key, button, or moving the mouse will result in the screensaver closing.
        case (WM_SYSKEYDOWN ):
        case (WM_SYSKEYUP   ):
        case (WM_KEYDOWN    ):
        case (WM_KEYUP      ):
        case (WM_LBUTTONDOWN):
        case (WM_LBUTTONUP  ):
        case (WM_MBUTTONDOWN):
        case (WM_MBUTTONUP  ):
        case (WM_RBUTTONDOWN):
        case (WM_RBUTTONUP  ):
        case (WM_XBUTTONDOWN):
        case (WM_XBUTTONUP  ):
        case (WM_MOUSEMOVE  ):
        {
            #ifndef BUILD_DEBUG
                if (input) running = false;
                return 0;
            #else
                break;
            #endif
        } break;
        case (WM_DESTROY):
        case (WM_CLOSE):
        {
            running = false;
            return 0;
        } break;
    }

    return DefWindowProc(
        hwindow,
        message,
        wparam,
        lparam);
}

int main ()
{
    WNDCLASSEXA window_class = {};
    window_class.cbSize        = sizeof(window_class);
    window_class.style         = CS_HREDRAW|CS_VREDRAW;
    window_class.lpfnWndProc   = window_proc;
    window_class.hInstance     = GetModuleHandle(0);
    window_class.hCursor       = LoadCursor(0, IDC_ARROW);
    window_class.lpszClassName = "WNDCLASSEXA::SCREENSAVER";

    if (!RegisterClassExA(&window_class))
    {
        FATAL_ERROR("Failed to create window class!");
    }

    float refresh_rate = 60.0f; // Assume 60hz until we can get an actual value.

    HMONITOR hmonitor = MonitorFromWindow(window, MONITOR_DEFAULTTONEAREST);
    MONITORINFOEXA monitor_info = {};
    monitor_info.cbSize = sizeof(MONITORINFOEXA);
    if (!GetMonitorInfoA(hmonitor, &monitor_info))
    {
        FATAL_ERROR("Failed to get monitor info!");
    }
    else
    {
        DEVMODEA dev_mode = {};
        dev_mode.dmSize = sizeof(DEVMODEA);
        if (!EnumDisplaySettingsA(monitor_info.szDevice, ENUM_CURRENT_SETTINGS, &dev_mode))
        {
            FATAL_ERROR("Failed to get monitor refresh rate!");
        }
        else
        {
            refresh_rate = CAST(float, dev_mode.dmDisplayFrequency);
        }
    }

    window = CreateWindowExA(
        0,
        window_class.lpszClassName,
        SCREENSAVER_TITLE,
        WS_POPUP,
        monitor_info.rcMonitor.left,
        monitor_info.rcMonitor.top,
        monitor_info.rcMonitor.right  - monitor_info.rcMonitor.left,
        monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
        0,
        0,
        window_class.hInstance,
        0);
    if (!window)
    {
        FATAL_ERROR("Failed to create window!");
    }

    HDC device_context = GetDC(window);

    PIXELFORMATDESCRIPTOR desired  = {};
    PIXELFORMATDESCRIPTOR obtained = {};

    desired.nSize      = sizeof(PIXELFORMATDESCRIPTOR);
    desired.nVersion   = 1;
    desired.dwFlags    = PFD_SUPPORT_OPENGL|PFD_DRAW_TO_WINDOW|PFD_DOUBLEBUFFER;
    desired.iPixelType = PFD_TYPE_RGBA;
    desired.cColorBits = 32;
    desired.cAlphaBits = 8;
    desired.iLayerType = PFD_MAIN_PLANE;

    int index = ChoosePixelFormat(device_context, &desired);

    DescribePixelFormat(device_context, index, sizeof(PIXELFORMATDESCRIPTOR), &obtained);
    SetPixelFormat(device_context, index, &obtained);

    HGLRC fake_context = wglCreateContext(device_context);
    if (!fake_context)
    {
        FATAL_ERROR("Failed to create initial OpenGL context!");
    }

    wglMakeCurrent(device_context, fake_context);

    if (!load_WGL_ARB_create_context(default_opengl_proc_loader))
    {
        FATAL_ERROR("Failed to load WGL_ARB_create_context!");
    }

    const int CONTEXT_ATTRIBS[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB,    OPENGL_LOAD_GL_VERSION_MAJOR,
        WGL_CONTEXT_MINOR_VERSION_ARB,    OPENGL_LOAD_GL_VERSION_MINOR,
        WGL_CONTEXT_FLAGS_ARB,                                       0,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0 // Used to mark the end of the context attribute list.
    };

    HGLRC opengl_context = wglCreateContextAttribsARB(device_context, NULL, CONTEXT_ATTRIBS);
    if (!opengl_context)
    {
        FATAL_ERROR("Failed to create an OpenGL context!");
    }

    wglMakeCurrent(device_context, opengl_context);

    ReleaseDC(window, device_context);
    wglDeleteContext(fake_context);

    if (!load_opengl_procs(default_opengl_proc_loader))
    {
        FATAL_ERROR("Failed to load OpenGL functions!");
    }

    if (!load_WGL_EXT_swap_control(default_opengl_proc_loader))
    {
        FATAL_ERROR("Failed to load WGL_EXT_swap_control!");
    }

    if (!screensaver_init())
    {
        FATAL_ERROR("Failed to initialize the screensaver!");
    }

    ShowCursor(false);
    ShowWindow(window, SW_SHOWDEFAULT);

    float dt = 1.0f / refresh_rate;

    running = true;
    input = false;

    int frame_counter = 0;

    wglSwapIntervalEXT(1);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    // Flush the initial message queue before starting execution.
    MSG message;
    while (PeekMessage(&message,NULL,0,0,PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    while (running)
    {
        while (PeekMessageA(&message, window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }

        device_context = GetDC(window);

        int vw = CAST(int, get_window_w());
        int vh = CAST(int, get_window_h());

        glViewport(0, 0, vw, vh);

        glClearColor(1,0,1,1);
        glClear(GL_COLOR_BUFFER_BIT);

        screensaver_update(dt);

        SwapBuffers(device_context);
        ReleaseDC(window, device_context);

        // Grace period of input not closing is over.
        if (frame_counter++ >= refresh_rate && !input)
        {
            input = true;
        }
    }

    screensaver_quit();

    return 0;
}

/* -------------------------------------------------------------------------- */

/*////////////////////////////////////////////////////////////////////////////*/
