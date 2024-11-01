#include "GameWindow.h"

#include <Windows.h>
#include <Windowsx.h>
#include <cassert>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <valarray>
#include <cmath>
#include <DirectXMath.h>

namespace dx = DirectX;

#include "Color.h"

HRESULT HRESULT_HOLDER;
#define CHECK_HRESULT(func) HRESULT_HOLDER = func; assert(HRESULT_HOLDER >= 0)

//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Window
//---------------------------------------------------------------------------------------------------------------------------------------------------------
HWND GameWindow;
const wchar_t* GameClassName = L"JuProject";
const wchar_t* WindowName = L"JuProject";
constexpr DWORD DefaultDword = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
constexpr int DefaultWindowPositionX = 320;
constexpr int DefaultWindowPositionY = 150;
constexpr int WindowSizeX = 1680;
constexpr int WindowSizeY = 600;
float ScreenRatio = (float)WindowSizeY / (float)WindowSizeX;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
// DirectX
//---------------------------------------------------------------------------------------------------------------------------------------------------------
ID3D11Device* DXDevice = nullptr;
ID3D11DeviceContext* DXImmediateContext = nullptr;
IDXGISwapChain* DXSwapChain = nullptr;
ID3D11RenderTargetView* DXRenderTargetView = nullptr;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Others
//---------------------------------------------------------------------------------------------------------------------------------------------------------
float rr, gg, bb = 0.0f;
float XPositionCursor, YPositionCursor = 0.0f;
float YPositionCube = 0.0f;
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

    UINT CreateDeviceAndSwapChainFlags = 0u;
#ifdef _DEBUG
    CreateDeviceAndSwapChainFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    CHECK_HRESULT(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, CreateDeviceAndSwapChainFlags, nullptr, 0,D3D11_SDK_VERSION, &SwapChainDesc, &DXSwapChain, &DXDevice,nullptr, &DXImmediateContext));

    // Create Render target view from back buffer
    ID3D11Resource* DXBackBuffer = nullptr;
    CHECK_HRESULT(DXSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&DXBackBuffer)));
    CHECK_HRESULT(DXDevice->CreateRenderTargetView(DXBackBuffer, nullptr, &DXRenderTargetView));
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
        } break;
    case WM_CHAR:
        {
            
        } break;
    case WM_MOUSEMOVE:
        {
            short xPos = GET_X_LPARAM(lParam); 
            short yPos = GET_Y_LPARAM(lParam);
            XPositionCursor = (float) xPos;
            YPositionCursor = (float) yPos;
        } break;
    case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE)
            {
                PostQuitMessage(1);
            }
            if (wParam == 'R') rr > 0.0f ? rr = 0.0f : rr = 1.0f;
            if (wParam == 'G') gg > 0.0f ? gg = 0.0f : gg = 1.0f;
            if (wParam == 'B') bb > 0.0f ? bb = 0.0f : bb = 1.0f;
        } break;
    case WM_MOUSEWHEEL:
        {
            short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            YPositionCube += (float)zDelta * 0.0025f;
        } break;
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
        DefaultWindowPositionX, DefaultWindowPositionY, WindowSizeX, WindowSizeY,
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
void DrawCube(const float xOffset, const float yOffset,  const float zOffset, const float Angle)
{
    struct SVertex
    {
        float x, y, z;
    };
    
    const SVertex vertexBufferData[] = {
        {-1.0f, -1.0f, -1.0f},
        {1.0f, -1.0f, -1.0f},
        {-1.0f, 1.0f, -1.0f},
        {1.0f, 1.0f, -1.0f},
        {-1.0f, -1.0f, 1.0f},
        {1.0f, -1.0f, 1.0f},
        {-1.0f, 1.0f, 1.0f},
        {1.0f, 1.0f, 1.0f},
    };
    constexpr UINT sizeVertexBufferData = (UINT)std::size(vertexBufferData);
    
    const USHORT indexBufferData[] = 
    {
        0,2,1, 2,3,1,
        1,3,5, 3,7,5,
        2,6,3, 3,6,7,
        4,5,7, 4,7,6,
        0,4,2, 2,4,6,
        0,1,4, 1,5,4,
    };
    constexpr UINT sizeIndexBufferData = (UINT)std::size(indexBufferData);
    
    // Create Vertex Buffer and bind it to the pipeline
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = sizeof(vertexBufferData);
        bufferDesc.StructureByteStride = sizeof(SVertex);

        D3D11_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pSysMem = vertexBufferData;

        ID3D11Buffer* vertexBuffer = nullptr;
        CHECK_HRESULT(DXDevice->CreateBuffer(&bufferDesc, &subResourceData, &vertexBuffer));
        constexpr UINT stride = sizeof(SVertex);
        constexpr UINT offset = 0u;
        DXImmediateContext->IASetVertexBuffers(0u, 1u, &vertexBuffer, &stride, &offset);
        vertexBuffer->Release();
    }
    
    // Create Index Buffer and bind it to the pipeline
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = sizeof(indexBufferData);
        bufferDesc.StructureByteStride = sizeof(USHORT);
        
        D3D11_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pSysMem = indexBufferData;
        
        ID3D11Buffer* indexBuffer = nullptr;
        CHECK_HRESULT(DXDevice->CreateBuffer(&bufferDesc, &subResourceData, &indexBuffer));
        DXImmediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0u);
        indexBuffer->Release();
    }
    
   // Create a vertex shader constant buffer with the transformation matrix and bind it to the pipeline
   {
       struct SMatrix
       {
           float values[4][4];
       };
       struct SConstantBuffer
       {
           dx::XMMATRIX transform; 
       } constantBufferData =
       {
           dx::XMMatrixTranspose(
               dx::XMMatrixRotationZ(Angle) 
               * dx::XMMatrixRotationX(Angle)
               * dx::XMMatrixScaling(0.45f, 0.45f, 0.45f)
               * dx::XMMatrixTranslation(xOffset, yOffset, zOffset + 4.0f)
               * dx::XMMatrixPerspectiveLH(1.0f, ScreenRatio, 0.5f, 10.0f)
               )
       };
   
       D3D11_BUFFER_DESC bufferDesc = {};
       bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
       bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
       bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
       bufferDesc.MiscFlags = 0u;
       bufferDesc.ByteWidth = sizeof(constantBufferData);
       bufferDesc.StructureByteStride = 0u;
       
       D3D11_SUBRESOURCE_DATA subResourceData = {};
       subResourceData.pSysMem = &constantBufferData;
       
       ID3D11Buffer* constantBuffer = nullptr;
       CHECK_HRESULT(DXDevice->CreateBuffer(&bufferDesc, &subResourceData, &constantBuffer));
       DXImmediateContext->VSSetConstantBuffers(0u, 1u, &constantBuffer);
       constantBuffer->Release();
   }
    
    // Create VertexShader and bind it to the pipeline
    {
        ID3DBlob* vsBlob = nullptr;
        CHECK_HRESULT(D3DReadFileToBlob(L"VertexShader.cso", &vsBlob));
        ID3D11VertexShader* vertexShader = nullptr;
        CHECK_HRESULT(DXDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader));
        DXImmediateContext->VSSetShader(vertexShader, nullptr, 0u);
        vertexShader->Release();

        // Input Layout for 2d vertex
        {
            ID3D11InputLayout* inputLayout;
            constexpr D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
            {
                { "Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0u },
                { "Color", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0u }
            };
            UINT sizeInputElementDesc = 2u;
            CHECK_HRESULT(DXDevice->CreateInputLayout(inputElementDesc, sizeInputElementDesc, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout));
            DXImmediateContext->IASetInputLayout(inputLayout);
            inputLayout->Release();
        }
        vsBlob->Release();
    }
    
    // Create a pixel shader constant buffer with the face colors
    {
        struct SConstantBufferPixelShader
        {
            SColorFloat FaceColors[6];
        };

        SConstantBufferPixelShader ConstantBufferPixelShader =
        {
            SColor::Red.ToFloat(),
            SColor::Green.ToFloat(),
            SColor::Blue.ToFloat(),
            SColor::Magenta.ToFloat(),
            SColor::Yellow.ToFloat(),
            SColor::Cyan.ToFloat(),
        };

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = sizeof(ConstantBufferPixelShader);
        bufferDesc.StructureByteStride = 0u;
       
        D3D11_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pSysMem = &ConstantBufferPixelShader;
       
        ID3D11Buffer* constantBuffer = nullptr;
        CHECK_HRESULT(DXDevice->CreateBuffer(&bufferDesc, &subResourceData, &constantBuffer));
        DXImmediateContext->PSSetConstantBuffers(0u, 1u, &constantBuffer);
        constantBuffer->Release();
    }
    
    // Create PixelShader and bind it to the pipeline
    {
        ID3DBlob* psBlob = nullptr;
        CHECK_HRESULT(D3DReadFileToBlob(L"PixelShader.cso", &psBlob));
        ID3D11PixelShader* pixelShader = nullptr;
        CHECK_HRESULT(DXDevice->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader));
        psBlob->Release();
        DXImmediateContext->PSSetShader(pixelShader, nullptr, 0u);
        pixelShader->Release();
    }
    
    // Configure Viewport
    {
        D3D11_VIEWPORT viewport;
        viewport.Width = WindowSizeX;
        viewport.Height = WindowSizeY;
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
        DXImmediateContext->RSSetViewports(1u, &viewport);
    }

    DXImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DXImmediateContext->OMSetRenderTargets(1u, &DXRenderTargetView, nullptr);
    DXImmediateContext->DrawIndexed(sizeIndexBufferData,  0u, 0);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void EndFrame()
{
    CHECK_HRESULT(DXSwapChain->Present(1u, 0u));
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void ClearBuffer(float r, float g, float b)
{
    const float color[] = { r, g, b, 1.0f };
    DXImmediateContext->ClearRenderTargetView(DXRenderTargetView, color);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void JuProject::DoFrame(const float dt)
{
    ClearBuffer(rr, gg, bb);

    static float AngleShape = 0.0f;
    AngleShape += 0.85f * dt;
    DrawCube((XPositionCursor / WindowSizeX * 2.0f) - 1.0f, -(YPositionCursor / WindowSizeY * 2.0f) + 1.0f, YPositionCube, AngleShape);
    DrawCube(0.0f, 0.0f, 0.0f, AngleShape);
    
    EndFrame();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------