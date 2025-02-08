#include "GameWindow.h"

#include <Windows.h>
#include <Windowsx.h>
#include <wincodec.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgiformat.h>

#include "Math.h"
#include "HResultHandler.h"
#include "GraphicPipeline.h"
#include "MeshManager.h"

#define GAME_DATA_PATH "Game/Data/"
#define GAME_DATA_SHADER_PATH "Game/Data/Shaders/"

#define MESH_TO_IMPORT_SQUARE "Square"
#define MESH_TO_IMPORT_CUBE "Cube"
#define MESH_TO_IMPORT_SPHERE "Sphere"
#define MESH_TO_IMPORT_SUZANNE "Suzanne"
#define MESH_TO_IMPORT_CRATE "Crate"
#define MESH_TO_IMPORT_MONSTER "Monster"

//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Window
//---------------------------------------------------------------------------------------------------------------------------------------------------------
HWND GameWindowHandle;
const wchar_t* GameClassName = L"JuProject";
const wchar_t* WindowName = L"JuProject";
constexpr DWORD DefaultDword = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
constexpr int DefaultWindowSizeX = (int)(1920 * 0.85);
constexpr int DefaultWindowSizeY = (int)(1080 * 0.85);
constexpr int DefaultWindowPositionX = (int)(DefaultWindowSizeX * 0.1);
constexpr int DefaultWindowPositionY = (int)(DefaultWindowSizeY * 0.1);
float WindowSizeX = (float)DefaultWindowSizeX;
float WindowSizeY = (float)DefaultWindowSizeY;
bool HasWindowFocus = false;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
// DirectX
//---------------------------------------------------------------------------------------------------------------------------------------------------------
ID3D11Device* DXDevice = nullptr;
ID3D11DeviceContext* DXImmediateContext = nullptr;
IDXGISwapChain* DXSwapChain = nullptr;
ID3D11RenderTargetView* DXRenderTargetView = nullptr;
ID3D11DepthStencilView* DXDepthStencilView = nullptr;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Actors
//---------------------------------------------------------------------------------------------------------------------------------------------------------
CStaticMesh MeshTest1;
CStaticMesh MeshTest2;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
// GameWindow Class
//---------------------------------------------------------------------------------------------------------------------------------------------------------
HWND GameWindow::GetWindowHandle() { return GameWindowHandle; }

bool GameWindow::HasFocus() { return HasWindowFocus; }

ID3D11Device* GameWindow::GetDevice() { return DXDevice; }

ID3D11DeviceContext* GameWindow::GetImmediateContext()
{
    return DXImmediateContext;
}
TMatrix4f GameWindow::GetPerspectiveMatrix()
{
    // TODO Julien Rogel (02/02/2025): no need to compute it each time we call it but only when ScreenRatio change
    const float ScreenRatio = WindowSizeY / WindowSizeX;
    const TMatrix4f PerspectiveMatrix = TMatrix4f::MatrixPerspectiveFovRH(0.4f * 3.14f, ScreenRatio, 0.0001f, 1000.0f);
    return PerspectiveMatrix;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void InitializeDirectX11()
{
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    SwapChainDesc.BufferDesc.Width = DefaultWindowSizeX;
    SwapChainDesc.BufferDesc.Height = DefaultWindowSizeY;
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.OutputWindow = GameWindowHandle;
    SwapChainDesc.Windowed = TRUE;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    SwapChainDesc.Flags = 0;

    UINT CreateDeviceAndSwapChainFlags = 0u;
#ifdef _DEBUG
    CreateDeviceAndSwapChainFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    CHECK_HRESULT(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, CreateDeviceAndSwapChainFlags, nullptr, 0,D3D11_SDK_VERSION, &SwapChainDesc, &DXSwapChain, &DXDevice,nullptr, &DXImmediateContext));
    
    // Create Render target view from back buffer
    {
        ID3D11Resource* DXBackBuffer = nullptr;
        CHECK_HRESULT(DXSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&DXBackBuffer)));
        CHECK_HRESULT(DXDevice->CreateRenderTargetView(DXBackBuffer, nullptr, &DXRenderTargetView));
        DXBackBuffer->Release();
    }

    // Create Stencil buffer and the Z buffer
    {
        // Create depth stencil state and bind it to the pipeline
        {
            D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
            depthStencilDesc.DepthEnable = true;
            depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
        
            ID3D11DepthStencilState* depthStencilState = nullptr;
            CHECK_HRESULT(DXDevice->CreateDepthStencilState(&depthStencilDesc, &depthStencilState));
            DXImmediateContext->OMSetDepthStencilState(depthStencilState, 1u);
            depthStencilState->Release();
        }
        // Create depth stencil texture
        ID3D11Texture2D* depthStencilTexture = nullptr;
        {
            D3D11_TEXTURE2D_DESC depthStencilTextureDesc;
            depthStencilTextureDesc.Width = DefaultWindowSizeX;
            depthStencilTextureDesc.Height = DefaultWindowSizeY;
            depthStencilTextureDesc.MipLevels = 1u;
            depthStencilTextureDesc.ArraySize = 1u;
            depthStencilTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
            depthStencilTextureDesc.SampleDesc.Count = 1u;
            depthStencilTextureDesc.SampleDesc.Quality = 0u;
            depthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
            depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            depthStencilTextureDesc.CPUAccessFlags = 0u;
            depthStencilTextureDesc.MiscFlags = 0u;
            CHECK_HRESULT(DXDevice->CreateTexture2D(&depthStencilTextureDesc, nullptr, &depthStencilTexture));
        }
        // Create view of depth stencil texture
        {
            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
            depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Texture2D.MipSlice = 0u;
            CHECK_HRESULT(DXDevice->CreateDepthStencilView(depthStencilTexture, &depthStencilViewDesc, &DXDepthStencilView));
        }

        DXImmediateContext->OMSetRenderTargets(1u, &DXRenderTargetView, DXDepthStencilView);

        depthStencilTexture->Release();
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void UninitializeDirectX11()
{
    DXDepthStencilView->Release();
    DXDepthStencilView = nullptr;
    
    DXRenderTargetView->Release();
    DXRenderTargetView = nullptr;
    
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
    case WM_SIZE:
        {
            UINT width = LOWORD(lParam);
            UINT height = HIWORD(lParam);
            WindowSizeX = (float)height;
            WindowSizeY = (float)width;
        } break;
    case WM_DESTROY:
        {
            PostQuitMessage(1);
        } break;
    case WM_SETFOCUS:
        {
            HasWindowFocus = true;
        } break ;
    case WM_KILLFOCUS:
        {
            HasWindowFocus = false;
        } break ;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void CreateGameWindow(const HINSTANCE hInstance)
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
	
    GameWindowHandle = CreateWindowEx(0, GameClassName, WindowName, DefaultDword,
        DefaultWindowPositionX, DefaultWindowPositionY, DefaultWindowSizeX, DefaultWindowSizeY,
        nullptr, nullptr, hInstance, nullptr);
    
    ShowWindow(GameWindowHandle, SW_SHOW);
    
    InitializeDirectX11();

    CStaticMesh::InitializeCommonPipeline();
    MeshTest1.InitializeStaticMeshPipeline({TVector3f{0.0f, 0.0f, -1.0f}}, MESH_TO_IMPORT_CRATE);
    MeshTest2.InitializeStaticMeshPipeline({}, MESH_TO_IMPORT_MONSTER);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void DestroyGameWindow()
{
    assert(GameWindowHandle != nullptr);
    
    UninitializeDirectX11();
    
    DestroyWindow(GameWindowHandle);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
SExitResult HandleGameWindowMessage()
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
void DrawFrame()
{
    {
        D3D11_RASTERIZER_DESC rasterizerDesc = {};
        {
            rasterizerDesc.AntialiasedLineEnable = true;
            rasterizerDesc.CullMode = D3D11_CULL_BACK;
            rasterizerDesc.DepthBias = 0;
            rasterizerDesc.DepthBiasClamp = 0.0f;
            rasterizerDesc.DepthClipEnable = true;
            rasterizerDesc.FillMode = D3D11_FILL_SOLID;
            rasterizerDesc.FrontCounterClockwise = true;
            rasterizerDesc.MultisampleEnable = false;
            rasterizerDesc.ScissorEnable = false;
            rasterizerDesc.SlopeScaledDepthBias = 0.0f;
        }
    
        ID3D11RasterizerState* rasterizerState = nullptr;
        CHECK_HRESULT(DXDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState));
        DXImmediateContext->RSSetState(rasterizerState);
    }
    
    // Configure Viewport
    {
        D3D11_VIEWPORT viewport;
        viewport.Width = DefaultWindowSizeX;
        viewport.Height = DefaultWindowSizeY;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        DXImmediateContext->RSSetViewports(1u, &viewport);
    }
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void ClearBuffer(float r, float g, float b)
{
    const float color[] = { r, g, b, 1.0f };
    DXImmediateContext->ClearRenderTargetView(DXRenderTargetView, color);
    DXImmediateContext->ClearDepthStencilView(DXDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void EndFrame()
{
    CHECK_HRESULT(DXSwapChain->Present(1u, 0u));
    ClearBuffer(0.0f, 0.0f, 0.0f);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void DoFrame(const float dt)
{
    if (HasWindowFocus == false)
        return;

    static float AngleShape = 0.0f;
    AngleShape += dt;

    CStaticMesh::UpdateCommonPipeline();
    
    MeshTest1.DrawStaticMesh();
    MeshTest2.DrawStaticMesh();

    DrawFrame();
    EndFrame();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------