#include <windows.h>

#include <d3d11.h>
#include <d3dcompiler.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

struct Vertex
{
    float x, y, z;    // Position
    float r, g, b, a; // Color
};

static const Vertex VERTICES[]
{
{  0.00f,  0.5f, 0.0f,    1.0f, 0.0f, 0.0f, 1.0f },
{  0.45f, -0.5f, 0.0f,    0.0f, 1.0f, 0.0f, 1.0f },
{ -0.45f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f, 1.0f },
};

static IDXGISwapChain*         swap_chain;     // The pointer to the swap chain interface.
static ID3D11Device*           device;         // The pointer to our Direct3D device interface.
static ID3D11DeviceContext*    device_context; // The pointer to our Direct3D device context.
static ID3D11RenderTargetView* backbuffer;     // The pointer to our back buffer render target.
static ID3D11VertexShader*     vshader;        // The pointer to a vertex shader.
static ID3D11PixelShader*      pshader;        // The pointer to a pixel shader.
static ID3D11Buffer*           vertex_buffer;  // The pointer to our vertex buffer object.
static ID3D11InputLayout*      vertex_layout;  // The pointer to our vertex data layout.

static void init_d3d (HWND hwnd)
{
    // Direct 3D Initialization

    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));

    swap_chain_desc.BufferCount       = 1;                                      // One back buffer.
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;             // Use 32-bit color.
    swap_chain_desc.BufferDesc.Width  = SCREEN_WIDTH;                           // The back buffer width.
    swap_chain_desc.BufferDesc.Height = SCREEN_HEIGHT;                          // The back buffer height.
    swap_chain_desc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;        // How swap chain is to be used.
    swap_chain_desc.OutputWindow      = hwnd;                                   // The window to be used.
    swap_chain_desc.SampleDesc.Count  = 4;                                      // How many multisamples.
    swap_chain_desc.Windowed          = TRUE;                                   // Windowed/full-screen mode.
    swap_chain_desc.Flags             = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // Allow full-screen switching.

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

    // Compile and Set Shaders

    ID3DBlob* vblob;
    ID3DBlob* pblob;

    // Load and compile HLSL shaders from file.
    D3DCompileFromFile(L"shaders.shader", 0, 0, "vshader", "vs_4_0", 0, 0, &vblob, NULL);
    D3DCompileFromFile(L"shaders.shader", 0, 0, "pshader", "ps_4_0", 0, 0, &pblob, NULL);

    // Create shader objects from the bytecode.
    device->CreateVertexShader(vblob->GetBufferPointer(), vblob->GetBufferSize(), NULL, &vshader);
    device->CreatePixelShader(pblob->GetBufferPointer(), pblob->GetBufferSize(), NULL, &pshader);

    // Set the active shader objects.
    device_context->VSSetShader(vshader, 0, 0);
    device_context->PSSetShader(pshader, 0, 0);

    // Create the Vertex Buffer

    D3D11_BUFFER_DESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(buffer_desc));

    buffer_desc.Usage          = D3D11_USAGE_DYNAMIC;      // CPU write-only, GPU read-only.
    buffer_desc.ByteWidth      = sizeof(Vertex) * 3;       // Size of the buffer.
    buffer_desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER; // Use as a vertex buffer.
    buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;   // Allow CPU to write in buffer.

    device->CreateBuffer(&buffer_desc, NULL, &vertex_buffer);

    // Set Buffer Data

    D3D11_MAPPED_SUBRESOURCE mapped_subresource;
    device_context->Map(vertex_buffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mapped_subresource); // Stops the GPU accessing the buffer so we can write to it.
    memcpy(mapped_subresource.pData, VERTICES, sizeof(VERTICES));
    device_context->Unmap(vertex_buffer, NULL); // Let the GPU access and use the buffer again.

    // Describe Vertex Layout

    D3D11_INPUT_ELEMENT_DESC input_element_desc[]
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        {"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    device->CreateInputLayout(input_element_desc, 2, vblob->GetBufferPointer(), vblob->GetBufferSize(), &vertex_layout);
    device_context->IASetInputLayout(vertex_layout); // Set the current layout to use.
}

static void quit_d3d ()
{
    swap_chain->SetFullscreenState(FALSE, NULL); // Switch to windowed mode.

    vertex_buffer->Release();
    vshader->Release();
    pshader->Release();
    backbuffer->Release();
    swap_chain->Release();
    device->Release();
    device_context->Release();
}

static void render_frame ()
{
    float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };
    device_context->ClearRenderTargetView(backbuffer, color);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;

    device_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
    device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    device_context->Draw(3, 0); // Draw 3 vertices starting from vertex 0.

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
    window_class.lpszClassName = "WindowClass1";

    RegisterClassEx(&window_class);

    RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    window_handle = CreateWindowEx(NULL, window_class.lpszClassName, "My Window!", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,CW_USEDEFAULT,wr.right-wr.left,wr.bottom-wr.top, NULL, NULL, window_class.hInstance, NULL);

    init_d3d(window_handle);

    ShowWindow(window_handle, show);

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
