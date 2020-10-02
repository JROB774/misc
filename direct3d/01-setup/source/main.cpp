#include <windows.h>
#include <d3d11.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

struct Color { float r, g, b, a; };

static IDXGISwapChain*         swap_chain;     // The pointer to the swap chain interface.
static ID3D11Device*           device;         // The pointer to our Direct3D device interface.
static ID3D11DeviceContext*    device_context; // The pointer to our Direct3D device context.
static ID3D11RenderTargetView* backbuffer;     // The pointer to our back buffer render target.

static void init_d3d (HWND hwnd)
{
    // Direct 3D Initialization

    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

    swap_chain_desc.BufferCount       = 1;                               // One back buffer.
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // Use 32-bit color.
    swap_chain_desc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT; // How swap chain is to be used.
    swap_chain_desc.OutputWindow      = hwnd;                            // The window to be used.
    swap_chain_desc.SampleDesc.Count  = 4;                               // How many multisamples.
    swap_chain_desc.Windowed          = TRUE;                            // Windowed/full-screen mode.

    // Create a device, device context, and swap chain using the provided info.
    D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
        D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &device, NULL, &device_context);

    // Set Render Target

    // Get the address of the back buffer.
    ID3D11Texture2D* backbuffer_texture;
    swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer_texture);
    // Use the back buffer address to create the render target.
    device->CreateRenderTargetView(backbuffer_texture, NULL, &backbuffer);
    backbuffer_texture->Release();
    // Set the render target as the back buffer.
    device_context->OMSetRenderTargets(1, &backbuffer, NULL);

    // Set Viewport

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(viewport));

    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width    = SCREEN_WIDTH;
    viewport.Height   = SCREEN_HEIGHT;

    device_context->RSSetViewports(1, &viewport);
}

static void quit_d3d ()
{
    backbuffer->Release();
    swap_chain->Release();
    device->Release();
    device_context->Release();
}

static void render_frame ()
{
    float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    device_context->ClearRenderTargetView(backbuffer, color);
    swap_chain->Present(0,0);
}

LRESULT CALLBACK WinProc (HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case (WM_DESTROY):
        {
            PostQuitMessage(0);
            return 0;
        } break;
    }

    return DefWindowProc(hwnd, msg, wparam, lparam);
}

int WINAPI WinMain (HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmdline, int show)
{
    WNDCLASSEX window_class;
    HWND window_handle;

    ZeroMemory(&window_class, sizeof(window_class));

    window_class.cbSize        = sizeof(WNDCLASSEX);
    window_class.style         = CS_HREDRAW|CS_VREDRAW;
    window_class.lpfnWndProc   = WinProc;
    window_class.hInstance     = instance;
    window_class.hCursor       = LoadCursor(NULL, IDC_ARROW);
    window_class.hbrBackground = (HBRUSH)COLOR_WINDOW;
    window_class.lpszClassName = "WindowClass1";

    RegisterClassEx(&window_class);

    RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    window_handle = CreateWindowEx(NULL, window_class.lpszClassName, "My Window!", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,wr.right-wr.left,wr.bottom-wr.top, NULL, NULL, window_class.hInstance, NULL);

    ShowWindow(window_handle, show);

    init_d3d(window_handle);

    MSG msg;
    while (true)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT) break;
        }

        render_frame();
    }

    quit_d3d();

    // Return this part of the WM_QUIT message.
    return (int)msg.wParam;
}
