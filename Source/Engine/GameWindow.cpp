#include "GameWindow.h"

#include <Windows.h>
#include <Windowsx.h>
#include <wincodec.h>

#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgiformat.h>

#include "Math.h"
#include "Color.h"
#include "HResultHandler.h"
#include "Shaders.h"
#include "WICTextureLoader.h"
#include "FreeLookCamera.h"
#include "MeshManager.h"

#define GAME_DATA_PATH "Game/Data/"
#define GAME_DATA_SHADER_PATH "Game/Data/Shaders/"

//#define MESH_TO_IMPORT "Square"
//#define MESH_TO_IMPORT "Cube"
//#define MESH_TO_IMPORT "Sphere"
//#define MESH_TO_IMPORT "Suzanne"
//#define MESH_TO_IMPORT "Crate"
#define MESH_TO_IMPORT "Monster"

//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Window
//---------------------------------------------------------------------------------------------------------------------------------------------------------
HWND GameWindow;
HWND JuProject::GetGameWindow() { return GameWindow; }

const wchar_t* GameClassName = L"JuProject";
const wchar_t* WindowName = L"JuProject";
constexpr DWORD DefaultDword = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
constexpr int DefaultWindowSizeX = (int)(1920 * 0.85);
constexpr int DefaultWindowSizeY = (int)(1080 * 0.85);
constexpr int DefaultWindowPositionX = (int)(DefaultWindowSizeX * 0.1);
constexpr int DefaultWindowPositionY = (int)(DefaultWindowSizeY * 0.1);
float WindowSizeX = (float)DefaultWindowSizeX;
float WindowSizeY = (float)DefaultWindowSizeY;
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
float ZOffsetPositionCamera = -5.0f;
float AngleXShape = 0.0f;
float AngleZShape = 0.0f;
float AngleYShape = 0.0f;
JuProject::SMeshData MeshData = {};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Game Camera
//---------------------------------------------------------------------------------------------------------------------------------------------------------
TFreeLookCamera GameCamera;
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

struct Input
{
    float CameraForwardMovement = 0.0f;
    float CameraRightMovement = 0.0f;
    float CameraRotationPitch = 0.0f;
    float CameraRotationYaw = 0.0f;
};
Input Input;
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
    case WM_CHAR:
        {
            
        } break;
    case WM_MOUSEMOVE:
        {
            short xPos = GET_X_LPARAM(lParam); 
            short yPos = GET_Y_LPARAM(lParam);
            float NewXPositionCursor = (float) xPos;
            float NewYPositionCursor = (float) yPos;
            //CameraRotationYaw = (XPositionCursor - NewXPositionCursor) * 0.0001f;
            //CameraRotationPitch = (YPositionCursor - NewYPositionCursor) * 0.0001f;
            XPositionCursor = NewXPositionCursor;
            YPositionCursor = NewYPositionCursor;
            
        } break;
    case WM_KEYDOWN:
        {
            if (wParam == VK_ESCAPE)
            {
                PostQuitMessage(1);
            }
            
            if (wParam == 'Q') Input.CameraRotationYaw += 0.08f;
            if (wParam == 'E') Input.CameraRotationYaw -= 0.08f;
            if (wParam == 'R') Input.CameraRotationPitch += 0.08f;
            if (wParam == 'F') Input.CameraRotationPitch -= 0.08f;
            
            if (wParam == 'W') Input.CameraForwardMovement += 0.2f;
            if (wParam == 'S') Input.CameraForwardMovement -= 0.2f;
            if (wParam == 'D') Input.CameraRightMovement += 0.2f;
            if (wParam == 'A') Input.CameraRightMovement -= 0.2f;
            
            //if (wParam == 'R') rr > 0.0f ? rr = 0.0f : rr = 1.0f;
            //if (wParam == 'G') gg > 0.0f ? gg = 0.0f : gg = 1.0f;
            //if (wParam == 'B') bb > 0.0f ? bb = 0.0f : bb = 1.0f;
        } break;
    case WM_MOUSEWHEEL:
        {
            short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
            ZOffsetPositionCamera += (float)zDelta * 0.0025f;
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
        DefaultWindowPositionX, DefaultWindowPositionY, DefaultWindowSizeX, DefaultWindowSizeY,
        nullptr, nullptr, hInstance, nullptr);
    
    ShowWindow(GameWindow, SW_SHOW);
    
    InitializeDirectX11();

    // Import Mesh
    {
        // TODO Julien Rogel (28/01/2025): Move it elsewhere
        const bool successImportingMesh = MeshManager::TryToImportOBJ(GAME_DATA_PATH MESH_TO_IMPORT ".obj", &MeshData);
        assert(successImportingMesh);
    }
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
void DrawMesh(const float xOffset, const float yOffset,  const float zOffset, const float AngleX, const float AngleY, const float AngleZ)
{
    // Create Vertex Buffer and bind it to the pipeline
    {
        constexpr UINT strideVertex = JuProject::SMeshData::VertexBuffer_StructureByteStride;
        
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = MeshData.VertexBuffer_ByteWidth;
        bufferDesc.StructureByteStride = strideVertex;

        D3D11_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pSysMem = MeshData.VertexBuffer.data();

        ID3D11Buffer* vertexBuffer = nullptr;
        CHECK_HRESULT(DXDevice->CreateBuffer(&bufferDesc, &subResourceData, &vertexBuffer));
        constexpr UINT offset = 0u;
        DXImmediateContext->IASetVertexBuffers(0u, 1u, &vertexBuffer, &strideVertex, &offset);
        vertexBuffer->Release();
    }
    
    // Create Index Buffer and bind it to the pipeline
    {
        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = MeshData.IndexBuffer_ByteWidth;
        bufferDesc.StructureByteStride = MeshData.IndexBuffer_StructureByteStride;
        
        D3D11_SUBRESOURCE_DATA subResourceData = {};
        subResourceData.pSysMem = MeshData.IndexBuffer.data();
        
        ID3D11Buffer* indexBuffer = nullptr;
        CHECK_HRESULT(DXDevice->CreateBuffer(&bufferDesc, &subResourceData, &indexBuffer));
        DXImmediateContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0u);
        indexBuffer->Release();
    }

    // Create a vertex shader constant buffer with the transformation matrix and bind it to the pipeline
    {
        struct SConstantBuffer
        {
            TMatrix4f WorldViewProjection; 
        };
        
        TMatrix4f ObjectWorldMatrix = TMatrix4f::Identity;
        TMatrix4f rotation = TMatrix4f::MatrixRotationPitchRollYaw(AngleX, AngleY, AngleZ);
        TMatrix4f translation = TMatrix4f::MatrixTranslation({xOffset, yOffset, zOffset });
        ObjectWorldMatrix = rotation * translation;

        TMatrix4f InvertedCameraMatrix = GameCamera.GetCameraWorldInverseMatrix();
        
        const float ScreenRatio = WindowSizeY / WindowSizeX;
        const TMatrix4f PerspectiveMatrix = TMatrix4f::MatrixPerspectiveFovRH(0.4f * 3.14f, ScreenRatio, 0.0001f, 1000.0f);

        TMatrix4f FinalMatrix = ObjectWorldMatrix * InvertedCameraMatrix * TMatrix4f::View * PerspectiveMatrix;
        
        SConstantBuffer constantBufferData;
        constantBufferData.WorldViewProjection = TMatrix4f::Transpose(FinalMatrix);

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
        CompileShader(GAME_DATA_SHADER_PATH L"VertexShader.hlsl", "vs_5_0", &vsBlob);
        //CHECK_HRESULT(D3DReadFileToBlob(, &vsBlob));
        ID3D11VertexShader* vertexShader = nullptr;
        CHECK_HRESULT(DXDevice->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader));
        DXImmediateContext->VSSetShader(vertexShader, nullptr, 0u);
        vertexShader->Release();

        // Input Layout for 3d vertex
        {
            constexpr D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
            {
                { "POSITION",  0u,  DXGI_FORMAT_R32G32B32_FLOAT,  0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
                { "UV",  0u,  DXGI_FORMAT_R32G32_FLOAT,     0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
                { "NORMAL",    0u,  DXGI_FORMAT_R32G32B32_FLOAT,  0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
                };
            UINT sizeInputElementDesc = 3u;
            ID3D11InputLayout* inputLayout;
            CHECK_HRESULT(DXDevice->CreateInputLayout(inputElementDesc, sizeInputElementDesc, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout));
            DXImmediateContext->IASetInputLayout(inputLayout);
            inputLayout->Release();
        }
        vsBlob->Release();
    }

    __declspec(align(16)) struct SWorldLight
    {
        TVector3f Direction = {0.37f, 0.93f, 0.0};
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
        CompileShader(GAME_DATA_SHADER_PATH L"PixelShader.hlsl", "ps_5_0", &psBlob);
        //CHECK_HRESULT(D3DReadFileToBlob(L".cso", &psBlob));
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
        samplerDesc.BorderColor[0] = SColor::Magenta.ToFloat().r;
        samplerDesc.BorderColor[1] = SColor::Magenta.ToFloat().g;
        samplerDesc.BorderColor[2] = SColor::Magenta.ToFloat().b;
        samplerDesc.BorderColor[3] = 1.0f;
        
        ID3D11SamplerState* samplerState = nullptr;
        CHECK_HRESULT(DXDevice->CreateSamplerState(&samplerDesc, &samplerState));
        DXImmediateContext->PSSetSamplers(0, 1, &samplerState);
    }

    {
        D3D11_RASTERIZER_DESC rasterizerDesc = {};
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
    DXImmediateContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    DXImmediateContext->DrawIndexed(MeshData.IndexCount,  0u, 0);
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
    AngleShape += dt;
    
    GameCamera.UpdateCamera(dt);
    
    DrawMesh(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, AngleShape);
    //DrawMesh(0.0f, 0.0f, -5.0f, AngleShape + 5.0f, AngleShape, AngleShape);
    //DrawMesh(0.0f, 0.0f, 0.0f, AngleShape * 1.0f, AngleShape, AngleShape * 0.5f);
    //DrawMesh(0.0f, 0.0f, 5.0f, AngleShape + 1.0f, AngleShape, AngleShape);
    //DrawMesh(0.0f, 0.0f, 10.0f, AngleShape + 5.0f, AngleShape, AngleShape);
    
    EndFrame();

    Input = {};
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------