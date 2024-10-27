#include "GameWindow.h"

#include <cassert>
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Color.h"

HRESULT HRESULT_HOLDER;
#define CHECK_HRESULT(func) HRESULT_HOLDER = func; assert(HRESULT_HOLDER >= 0)

//---------------------------------------------------------------------------------------------------------------------------------------------------------
const wchar_t* GameClassName = L"JuProject";
const wchar_t* WindowName = L"JuProject";
constexpr DWORD DefaultDword = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
constexpr int DefaultWindowPositionX = 320;
constexpr int DefaultWindowPositionY = 150;
constexpr int WindowSizeX = 1280;
constexpr int WindowSizeY = 720;

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
void DrawTestTriangle()
{
    struct SVertex
    {
        float x, y; // Position
        SColor color;
    };
    
    const SVertex vertices[] = {
        {-0.5f, 0.5f, SColor::Red},
        {0.0f, 0.5f, SColor::Magenta},
        {0.0f, 0.0f, SColor::Green},
        {-0.5f, 0.0f, SColor::White},
    };
    UINT sizeVertices = 4u;
    
    const USHORT indices[] = 
    {
        0, 2, 3,
        0, 2, 1,
    };
    UINT sizeindices = 6u;
    
    // Create Vertex Buffer and bind it to the pipeline
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = sizeof(vertices);
        bufferDesc.StructureByteStride = sizeof(SVertex);

        D3D11_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pSysMem = vertices;

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
        bufferDesc.ByteWidth = sizeof(indices);
        bufferDesc.StructureByteStride = sizeof(USHORT);
        
        D3D11_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pSysMem = indices;
        
        ID3D11Buffer* indexBuffer = nullptr;
        CHECK_HRESULT(DXDevice->CreateBuffer(&bufferDesc, &subResourceData, &indexBuffer));
        DXImmediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0u);
        indexBuffer->Release();
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
                { "Position", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0u },
                { "Color", 0u, DXGI_FORMAT_R8G8B8A8_UNORM, 0u, D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA, 0u }
            };
            UINT sizeInputElementDesc = 2u;
            CHECK_HRESULT(DXDevice->CreateInputLayout(inputElementDesc, sizeInputElementDesc, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout));
            DXImmediateContext->IASetInputLayout(inputLayout);
            inputLayout->Release();
        }
        vsBlob->Release();
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
 
    // Set primitive topology to triangle list
    DXImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    
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

    DXImmediateContext->OMSetRenderTargets(1u, &DXRenderTargetView, nullptr);
    
    UINT StartIndexLocation = 0u;
    INT BaseVertexLocation = 0;
    DXImmediateContext->DrawIndexed(sizeindices, StartIndexLocation, BaseVertexLocation);
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
void JuProject::DoFrame()
{
    ClearBuffer(rr, gg, bb);

    DrawTestTriangle();
    
    EndFrame();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------