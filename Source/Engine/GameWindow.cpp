#include "GameWindow.h"

#include <Windows.h>
#include <Windowsx.h>
#include <wincodec.h>

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <dxgiformat.h>
#include <iterator>

#include "Color.h"
#include "HResultHandler.h"
#include "MeshImporter.h"
#include "WICTextureLoader.h"

namespace dx = DirectX;

//#define GAME_DATA_PATH L"D:/Projects/JuProject/Game/Data/" // 1st PC
#define GAME_DATA_PATH L"E:/Perso/JuProject/Game/Data/" // 2nd PC

//#define MESH_TO_IMPORT L"Square"
//#define MESH_TO_IMPORT L"Cube"
//#define MESH_TO_IMPORT L"Sphere"
#define MESH_TO_IMPORT L"Suzanne"
//#define MESH_TO_IMPORT L"Crate"

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
ID3D11DepthStencilView* DXDepthStencilView = nullptr;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Others
//---------------------------------------------------------------------------------------------------------------------------------------------------------
float rr, gg, bb = 0.0f;
float XPositionCursor, YPositionCursor = 0.0f;
float ZPositionCube = 0.0f;
float AngleXShape = 0.0f;
float AngleZShape = 0.0f;
float AngleYShape = 0.0f;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void InitializeDirectX11()
{
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    SwapChainDesc.BufferDesc.Width = WindowSizeX;
    SwapChainDesc.BufferDesc.Height = WindowSizeY;
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
            depthStencilTextureDesc.Width = WindowSizeX;
            depthStencilTextureDesc.Height = WindowSizeY;
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
            
            if (wParam == 'Q') AngleZShape += 0.05f;
            if (wParam == 'E') AngleZShape -= 0.05f;
            
            if (wParam == 'W') AngleXShape += 0.05f;
            if (wParam == 'S') AngleXShape -= 0.05f;
            
            if (wParam == 'D') AngleYShape -= 0.05f;
            if (wParam == 'A') AngleYShape += 0.05f;
            
            if (wParam == 'R') rr > 0.0f ? rr = 0.0f : rr = 1.0f;
            if (wParam == 'G') gg > 0.0f ? gg = 0.0f : gg = 1.0f;
            if (wParam == 'B') bb > 0.0f ? bb = 0.0f : bb = 1.0f;
        } break;
    case WM_MOUSEWHEEL:
        {
            short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            ZPositionCube += (float)zDelta * 0.0025f;
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
void DrawCube(const float xOffset, const float yOffset,  const float zOffset, const float AngleX, const float AngleY, const float AngleZ)
{
    SMeshInfo meshInfo = {};
    bool successImportingMesh = TryToImportMeshInfoFromOBJFile(GAME_DATA_PATH MESH_TO_IMPORT L".obj", &meshInfo);
    assert(successImportingMesh);
 
    // Create Vertex Buffer and bind it to the pipeline
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = sizeof(SVertex) * meshInfo.nbVertexBuffer;
        bufferDesc.StructureByteStride = sizeof(SVertex);

        D3D11_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pSysMem = &meshInfo.VertexBuffer;

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
        bufferDesc.ByteWidth = sizeof(TVertexIndex) * meshInfo.nbIndexBuffer;
        bufferDesc.StructureByteStride = sizeof(TVertexIndex);
        
        D3D11_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pSysMem = meshInfo.IndexBuffer.Indexes;
        
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
        };

        SConstantBuffer constantBufferData;
        constantBufferData.transform = dx::XMMatrixRotationX(AngleX);
        constantBufferData.transform *= dx::XMMatrixRotationY(AngleY);
        constantBufferData.transform *= dx::XMMatrixRotationZ(AngleZ);
        constantBufferData.transform *= dx::XMMatrixScaling(0.45f, 0.45f, 0.45f);
        constantBufferData.transform *= dx::XMMatrixTranslation(xOffset, yOffset, zOffset);
        constantBufferData.transform *= dx::XMMatrixPerspectiveRH(1.0f, ScreenRatio, 0.5f, 1000.0f);

        constantBufferData.transform = dx::XMMatrixTranspose(constantBufferData.transform);
        
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

        // Input Layout for 3d vertex
        {
            ID3D11InputLayout* inputLayout;
            constexpr D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
            {
                { "POSITION",  0u,  DXGI_FORMAT_R32G32B32_FLOAT,  0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
                { "UV",  0u,  DXGI_FORMAT_R32G32_FLOAT,     0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
                { "NORMAL",    0u,  DXGI_FORMAT_R32G32B32_FLOAT,  0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
				};
            UINT sizeInputElementDesc = 3u;
            CHECK_HRESULT(DXDevice->CreateInputLayout(inputElementDesc, sizeInputElementDesc, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout));
            DXImmediateContext->IASetInputLayout(inputLayout);
            inputLayout->Release();
        }
        vsBlob->Release();
    }

    __declspec(align(16)) struct SWorldLight
    {
        float3 Direction = {0.37f, 0.93f, 0.0};
        float Ambient = 0.0f;
    };
    
    // Create a pixel shader constant buffer with the face colors
    {
        __declspec(align(16)) struct SConstantBufferPixelShader
        {
            SWorldLight WorldLight;
        };

        SConstantBufferPixelShader ConstantBufferPixelShader =
        {
            {{ 0.44f, 0.87f, 0.22f }, 0.15f }
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
    
    // Import, Create and Bind texture to the pixel shader
    {
        // TODO Julien Rogel (03/11/2024): Extract init of texture & textureView out of CreateWICTextureFromFile
        
        ID3D11Resource* texture = nullptr;
        ID3D11ShaderResourceView* textureView = nullptr;
        CHECK_HRESULT(CreateWICTextureFromFile(DXDevice, DXImmediateContext, GAME_DATA_PATH MESH_TO_IMPORT L".bmp", &texture, &textureView, 0));
        texture->Release();
        DXImmediateContext->PSSetShaderResources(0u, 1u, &textureView);
    }

    // Create and bind sampler state to the pixel shader
    {
        D3D11_SAMPLER_DESC samplerDesc = {};
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.BorderColor[0] = TFloatColor::Magenta.r;
        samplerDesc.BorderColor[1] = TFloatColor::Magenta.g;
        samplerDesc.BorderColor[2] = TFloatColor::Magenta.b;
        samplerDesc.BorderColor[3] = TFloatColor::Magenta.a;
        
        ID3D11SamplerState* samplerState = nullptr;
        CHECK_HRESULT(DXDevice->CreateSamplerState(&samplerDesc, &samplerState));
        DXImmediateContext->PSSetSamplers(0, 1, &samplerState);
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
    DXImmediateContext->DrawIndexed(meshInfo.nbIndexBuffer,  0u, 0);
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
    DXImmediateContext->ClearDepthStencilView(DXDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
void JuProject::DoFrame(const float dt)
{
    ClearBuffer(rr, gg, bb);

    static float AngleShape = 0.0f;
    AngleShape += 0.25f * dt;
    
    //DrawCube(0.0f, 0.0f, 4.0f, AngleShape, AngleShape, AngleShape);

    float XPositionCube = (XPositionCursor / WindowSizeX * 2.0f) - 1.0f;
    float YPositionCube = -(YPositionCursor / WindowSizeY * 2.0f) + 1.0f;
    DrawCube(0.0f, 0.0f, -5.0f + ZPositionCube, AngleXShape, AngleYShape, AngleZShape);
    
    EndFrame();
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------