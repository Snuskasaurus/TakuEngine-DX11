#include "GameWindow.h"

#include <cassert>
#include <d3d11.h>

//---------------------------------------------------------------------------------------------------------------------------------------------------------
const wchar_t* GameClassName = L"JuProject";
const wchar_t* WindowName = L"JuProject";
constexpr DWORD DefaultDword = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
constexpr int DefaultPositionX = 320;
constexpr int DefaultPositionY = 150;
constexpr int DefaultSizeX = 1280;
constexpr int DefaultSizeY = 720;

float rr, gg, bb = 0.0f;

//---------------------------------------------------------------------------------------------------------------------------------------------------------

HWND GameWindow;

ID3D11Device* DXDevice = nullptr;
ID3D11DeviceContext* DXImmediateContext = nullptr;
IDXGISwapChain* DXSwapChain = nullptr;
ID3D11RenderTargetView* DXRenderTargetView = nullptr;

//---------------------------------------------------------------------------------------------------------------------------------------------------------
void InitializeDirectX11()
{
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    SwapChainDesc.BufferDesc.Width = 0;
    SwapChainDesc.BufferDesc.Height = 0;
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.OutputWindow = GameWindow;
    SwapChainDesc.Windowed = TRUE;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    SwapChainDesc.Flags = 0;

    const HRESULT Result_D3D11CreateDeviceAndSwapChain = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
                                                                0, nullptr, 0,D3D11_SDK_VERSION,
                                                                &SwapChainDesc, &DXSwapChain, &DXDevice,nullptr, &DXImmediateContext);
    assert(Result_D3D11CreateDeviceAndSwapChain >= 0);

    // Create Render target view from back buffer
    ID3D11Resource* DXBackBuffer = nullptr;
    const HRESULT Result_DXSwapChain_GetBuffer = DXSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&DXBackBuffer));
    assert(Result_DXSwapChain_GetBuffer >= 0);
    const HRESULT Result_DXDevice_CreateRenderTargetView =  DXDevice->CreateRenderTargetView(DXBackBuffer, nullptr, &DXRenderTargetView);
    assert(Result_DXDevice_CreateRenderTargetView >= 0);
    DXBackBuffer->Release();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void UninitializeDirectX11()
{
    DXDevice->Release();
    DXDevice = nullptr;
    
    DXSwapChain->Release();
    DXSwapChain = nullptr;
    
    DXImmediateContext->Release();
    DXImmediateContext = nullptr;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK GameWindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        {
            PostQuitMessage(1);
        }
        break;
    case WM_CHAR:
        {
        }
    case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE)
            {
                PostQuitMessage(1);
            }
            if (wParam == 'R') rr > 0.0f ? rr = 0.0f : rr = 1.0f;
            if (wParam == 'G') gg > 0.0f ? gg = 0.0f : gg = 1.0f;
            if (wParam == 'B') bb > 0.0f ? bb = 0.0f : bb = 1.0f;
        }
        break;
    case WM_LBUTTONUP:
    case WM_LBUTTONDOWN:
        {
            const POINTS pt = MAKEPOINTS(lParam);	
        }
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void JuProject::CreateGameWindow(const HINSTANCE hInstance)
{
    WNDCLASSEX wc = { 0 };
    wc.cbSize = sizeof(wc);
    wc.style = CS_OWNDC;
    wc.lpfnWndProc = GameWindowProcedure;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hCursor = nullptr;
    wc.hIcon = nullptr;
    wc.hbrBackground = nullptr;
    wc.lpszMenuName = nullptr;
    wc.hIconSm = nullptr;
    wc.lpszClassName = GameClassName;
    RegisterClassEx(&wc);
	
    GameWindow = CreateWindowEx(0, GameClassName, WindowName, DefaultDword,
        DefaultPositionX, DefaultPositionY, DefaultSizeX, DefaultSizeY,
        nullptr, nullptr, hInstance, nullptr);
    
    ShowWindow(GameWindow, SW_SHOW);
    
    InitializeDirectX11();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void JuProject::DestroyGameWindow()
{
    assert(GameWindow != nullptr);
    
    UninitializeDirectX11();
    
    DestroyWindow(GameWindow);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
JuProject::SExitResult JuProject::HandleGameWindowMessage()
{
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            return { true, (int)msg.wParam };
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return {false, -1 };
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void EndFrame()
{
    DXSwapChain->Present(1u, 0u);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void ClearBuffer(float r, float g, float b)
{
    const float color[] = { r, g, b, 1.0f };
    DXImmediateContext->ClearRenderTargetView(DXRenderTargetView, color);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void JuProject::DoFrame()
{
    //ClearBuffer(1.0f, 0.0f, 0.0f);
    ClearBuffer(rr, gg, bb);
    EndFrame();
}