#include "Graphic.h"

#include <d3d11.h>
#include <d3dcommon.h>

#include "ShaderBuffers.h"
#include "../Resources/AssetList.h"
#include "../Resources/MeshResources.h"
#include "../Resources/ShadersResources.h"

#include "../GameWindow.h"
#include "../HResultHandler.h"
#include "../World.h"
#include "../Debug/Profiling.h"

enum class EDebugDrawTarget
{
    NONE,
    Z_BUFFER_SHADOW,
    Z_BUFFER_SCENE,
};

EDebugDrawTarget DebugDrawTarget = EDebugDrawTarget::Z_BUFFER_SCENE;
///---------------------------------------------------------------------------------------------------------------------
#define RESOLUTION_WIDTH    1920
#define RESOLUTION_HEIGHT   1080
///---------------------------------------------------------------------------------------------------------------------
int GetResolutionWidth() { return RESOLUTION_WIDTH; }
int GetResolutionHeight() { return RESOLUTION_HEIGHT; }
///---------------------------------------------------------------------------------------------------------------------
static SGraphicResources_Pipeline G_PIPELINE;
static SDepthStencilResources G_DEPTH_STENCIL_RESOURCES;
static ID3D11SamplerState* G_SAMPLER_STATES[3] = { nullptr, nullptr, nullptr};
///---------------------------------------------------------------------------------------------------------------------
static SVertexShader G_VS_SHADOW;
static SVertexShader G_VS_BASE;
static SVertexShader G_VS_2D_DEBUG;
///---------------------------------------------------------------------------------------------------------------------
static SPixelShader G_PS_SHADOW;
static SPixelShader G_PS_BASE;
static SPixelShader G_PS_2D_DEBUG;
///---------------------------------------------------------------------------------------------------------------------
static SShaderBufferHolder G_VS_BUFFERS[15];
static SShaderBufferHolder G_PS_BUFFERS[15];
///---------------------------------------------------------------------------------------------------------------------
ID3D11Device* MGraphic::GetDXDevice()
{
    return G_PIPELINE.Device;
}
///---------------------------------------------------------------------------------------------------------------------
ID3D11DeviceContext* MGraphic::GetDXDeviceContext()
{
    return G_PIPELINE.DeviceContext;
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateDirectXWindow()
{
    MGraphic::CreateDeviceAndSwapChain(&G_PIPELINE.Device, &G_PIPELINE.DeviceContext, &G_PIPELINE.SwapChain);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetupDraw()
{
    MGraphic::CreateRenderTargetView(G_PIPELINE.Device, G_PIPELINE.SwapChain, &G_PIPELINE.BackBufferResource, &G_PIPELINE.RenderTargetView);
    MGraphic::CreateDepthStencil(G_PIPELINE.Device, G_PIPELINE.DeviceContext, G_DEPTH_STENCIL_RESOURCES);
    MGraphic::CreateRasterizerState(G_PIPELINE.Device, &G_PIPELINE.rasterizerState);
    MGraphic::ConfigureViewport(G_PIPELINE.DeviceContext);
    MGraphic::CreateAndSetSampleStates();
    
    // Initialize Vertex Shaders
    {
        G_VS_2D_DEBUG.CreateVertexShader(G_PIPELINE.Device,     TAKU_ASSET_VS_2D,       VS_INPUT_DESC::POS_UV,                  ARRAYSIZE(VS_INPUT_DESC::POS_UV));
        G_VS_SHADOW.CreateVertexShader(G_PIPELINE.Device,       TAKU_ASSET_VS_SHADOW,   VS_INPUT_DESC::POS_INST,                ARRAYSIZE(VS_INPUT_DESC::POS_INST));
        G_VS_BASE.CreateVertexShader(G_PIPELINE.Device,         TAKU_ASSET_VS_BASE,     VS_INPUT_DESC::POS_NORM_TAN_UV_INST,    ARRAYSIZE(VS_INPUT_DESC::POS_NORM_TAN_UV_INST));
    }
    
    // Initialize Pixel Shaders
    {
        G_PS_BASE.CreatePixelShader(G_PIPELINE.Device,      TAKU_ASSET_PS_BASE);
        G_PS_SHADOW.CreatePixelShader(G_PIPELINE.Device,    TAKU_ASSET_PS_SHADOW);
        G_PS_2D_DEBUG.CreatePixelShader(G_PIPELINE.Device,  TAKU_ASSET_PS_2D);
    }
    
    // Initialize Vertex Shader Buffers
    {
        G_VS_BUFFERS[0].CreateShaderBuffer(EShaderType::VERTEX_SHADER, 0u, sizeof(vs_buffer_sceneEachFrame));
        G_VS_BUFFERS[1].CreateShaderBuffer(EShaderType::VERTEX_SHADER, 1u, sizeof(vs_buffer_object));
    }
    
    // Initialize Pixel Shader Buffers
    {
        G_PS_BUFFERS[0].CreateShaderBuffer(EShaderType::PIXEL_SHADER, 0u, sizeof(ps_buffer_sceneEachFrame));
    }
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::Draw()
{
    // Clear pass
    MGraphic::ClearRenderTarget(G_PIPELINE.DeviceContext, G_PIPELINE.RenderTargetView);
    MGraphic::ClearDepthStencil(G_PIPELINE.DeviceContext, G_DEPTH_STENCIL_RESOURCES.View);
    MGraphic::ClearDepthStencil(G_PIPELINE.DeviceContext, G_DEPTH_STENCIL_RESOURCES.ViewLight);
    
    const std::vector<CDrawable_InstancedMesh*>& instancedMeshes = MWorld::GetWorld()->CurrentGameScene->InstancedMeshes;
    
    // Depth only pass to get SceneLight Z-Buffer
    {
        ID3D11RenderTargetView* NullRenderTarget = nullptr;
        G_PIPELINE.DeviceContext->OMSetRenderTargets(1u, &NullRenderTarget, G_DEPTH_STENCIL_RESOURCES.ViewLight);
        G_PIPELINE.DeviceContext->VSSetShader(G_VS_SHADOW.Shader, nullptr, 0u);
        G_PIPELINE.DeviceContext->IASetInputLayout(G_VS_SHADOW.Input);
        G_PIPELINE.DeviceContext->PSSetShader(G_PS_SHADOW.Shader, nullptr, 0u);
        SShaderBufferHolder::FillBuffer_VS_SceneEachFrame(&G_VS_BUFFERS[0], true);
    
        for (int i = 0; i < instancedMeshes.size(); ++i)
        {
            CDrawable_InstancedMesh* instancedMesh = instancedMeshes[i];
            MGraphic::SetVertexAndIndexBuffer(G_PIPELINE.DeviceContext, &instancedMesh->VertexBuffer, instancedMesh->IndexBuffer, SMeshData::VertexBuffer_StructureByteStride);
            
            const UINT nbInstances = (UINT)instancedMesh->Instances.size();
            UINT nbInstancesRemainingToDraw = nbInstances;
            while (nbInstancesRemainingToDraw > 0)
            {
                const UINT nbInstancesToDraw = MMath::Min(nbInstancesRemainingToDraw, MAX_INSTANCE_COUNT - 1);
                const UINT startInstances = nbInstances - nbInstancesRemainingToDraw;
                
                SShaderBufferHolder::FillBuffer_VS_Object(&G_VS_BUFFERS[1], instancedMesh->Instances.data(), startInstances, nbInstancesToDraw);
                MGraphic::SetPrimitiveAndDraw_Instanced(G_PIPELINE.DeviceContext, instancedMesh->MeshData->IndexCount, nbInstancesToDraw + 1);
                
                nbInstancesRemainingToDraw -= nbInstancesToDraw;
            }
        }
        MGraphic::Rasterize(G_PIPELINE.DeviceContext, G_PIPELINE.rasterizerState);
    }

    // Draw the scene
    {
        G_PIPELINE.DeviceContext->OMSetRenderTargets(1u, &G_PIPELINE.RenderTargetView, G_DEPTH_STENCIL_RESOURCES.View);
        
        G_PIPELINE.DeviceContext->VSSetShader(G_VS_BASE.Shader, nullptr, 0u);
        G_PIPELINE.DeviceContext->IASetInputLayout(G_VS_BASE.Input);
        SShaderBufferHolder::FillBuffer_VS_SceneEachFrame(&G_VS_BUFFERS[0], false);
        G_PIPELINE.DeviceContext->PSSetShader(G_PS_BASE.Shader, nullptr, 0u);
        SShaderBufferHolder::FillBuffer_PS_SceneEachFrame(&G_PS_BUFFERS[0]);
        
        for (int i = 0; i < instancedMeshes.size(); ++i)
        {
            CDrawable_InstancedMesh* instancedMesh = instancedMeshes[i];
            
            MGraphic::SetVertexAndIndexBuffer(G_PIPELINE.DeviceContext, &instancedMesh->VertexBuffer, instancedMesh->IndexBuffer, SMeshData::VertexBuffer_StructureByteStride);
            
            ID3D11ShaderResourceView* ShaderResourceViews[] =
                {
                    (instancedMesh->ColorTexture->textureView),
                    (G_DEPTH_STENCIL_RESOURCES.ResourceViewLight),
                    (instancedMesh->NormalTexture == nullptr ? nullptr : instancedMesh->NormalTexture->textureView),
                    (!instancedMesh->EmissionTexture ? nullptr : instancedMesh->EmissionTexture->textureView),
                    (!instancedMesh->MROTexture ? nullptr : instancedMesh->MROTexture->textureView),
                };
            
            MGraphic::SetPixelShaderTextureViews(G_PIPELINE.DeviceContext, ARRAYSIZE(ShaderResourceViews), ShaderResourceViews);
    
            const UINT nbInstances = (UINT)instancedMesh->Instances.size();
            UINT nbInstancesRemainingToDraw = nbInstances;
            while (nbInstancesRemainingToDraw > 0)
            {
                const UINT nbInstancesToDraw = MMath::Min(nbInstancesRemainingToDraw, MAX_INSTANCE_COUNT - 1);
                const UINT startInstances = nbInstances - nbInstancesRemainingToDraw;
                
                SShaderBufferHolder::FillBuffer_VS_Object(&G_VS_BUFFERS[1], instancedMesh->Instances.data(), startInstances, nbInstancesToDraw);
                MGraphic::SetPrimitiveAndDraw_Instanced(G_PIPELINE.DeviceContext, instancedMesh->MeshData->IndexCount, nbInstancesToDraw + 1);
                
                nbInstancesRemainingToDraw -= nbInstancesToDraw;
            }
        }
        MGraphic::Rasterize(G_PIPELINE.DeviceContext, G_PIPELINE.rasterizerState);
    }
    
    // Draw screen debug
    if (DebugDrawTarget != EDebugDrawTarget::NONE)
    {
        G_PIPELINE.DeviceContext->OMSetRenderTargets(1u, &G_PIPELINE.RenderTargetView, nullptr);

        G_PIPELINE.DeviceContext->VSSetShader(G_VS_2D_DEBUG.Shader, nullptr, 0u);
        G_PIPELINE.DeviceContext->IASetInputLayout(G_VS_2D_DEBUG.Input);
        G_PIPELINE.DeviceContext->PSSetShader(G_PS_2D_DEBUG.Shader, nullptr, 0u);

        switch (DebugDrawTarget) {
        case EDebugDrawTarget::Z_BUFFER_SHADOW:
            {
                G_PIPELINE.DeviceContext->PSSetShaderResources(0, 1u, &G_DEPTH_STENCIL_RESOURCES.ResourceViewLight);
            } break;
        case EDebugDrawTarget::Z_BUFFER_SCENE:
            {
                G_PIPELINE.DeviceContext->PSSetShaderResources(0, 1u, &G_DEPTH_STENCIL_RESOURCES.ResourceView);
            } break;
        }

        struct SVertex2D { TVector4f pos; TVector2f uv; } Vertexes[4] =
        {
            {{ -1.0f, 1.0f, 0.5f, 1.0f }, {0.0f, 0.0f}},
            {{ -0.25f,  1.0f, 0.5f, 1.0f }, {1.0f, 0.0f}},
            {{ -0.25f,  0.25f, 0.5f, 1.0f }, {1.0f, 1.0f}},
            {{ -1.0f, 0.25,  0.5f, 1.0f }, {0.0f, 1.0f}}
        };

        UINT sizeVec = sizeof(TVector3f);
        UINT sizeVertex = sizeof(SVertex2D);

        TVertexIndex Indexes[6] = { 2, 1, 0, 2, 0, 3 };
        
        ID3D11Buffer* VertexBuffer = nullptr;
        MGraphic::CreateVertexBuffer(G_PIPELINE.Device, G_PIPELINE.DeviceContext, &VertexBuffer, Vertexes, ARRAYSIZE(Vertexes), sizeof(SVertex2D));
        ID3D11Buffer* IndexBuffer = nullptr;
        MGraphic::CreateIndexBuffer(G_PIPELINE.Device, G_PIPELINE.DeviceContext, &IndexBuffer, Indexes, ARRAYSIZE(Indexes), sizeof(TVertexIndex));
        MGraphic::SetVertexAndIndexBuffer(G_PIPELINE.DeviceContext, &VertexBuffer, IndexBuffer, sizeof(SVertex2D));
       
        G_PIPELINE.DeviceContext->DrawIndexed(ARRAYSIZE(Indexes), 0u, 0u);

        MGraphic::Rasterize(G_PIPELINE.DeviceContext, G_PIPELINE.rasterizerState);

        constexpr ID3D11ShaderResourceView* const nullResources[1] = { nullptr };
        G_PIPELINE.DeviceContext->PSSetShaderResources(0, 1, nullResources);

        VertexBuffer->Release();
        IndexBuffer->Release();
    }

    MGraphic::PresentSwapChain(G_PIPELINE.SwapChain);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::UninitializeGraphic()
{
    G_DEPTH_STENCIL_RESOURCES.Release();
    G_PIPELINE.Release();
    
    G_VS_SHADOW.Release();
    G_VS_BASE.Release();
    G_VS_2D_DEBUG.Release();
    
    G_PS_SHADOW.Release();
    G_PS_BASE.Release();
    G_PS_2D_DEBUG.Release();
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateRasterizerState(ID3D11Device* _device, ID3D11RasterizerState** _rasterizerState)
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
    CHECK_HRESULT(_device->CreateRasterizerState(&rasterizerDesc, _rasterizerState));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateDeviceAndSwapChain(ID3D11Device** _device, ID3D11DeviceContext** _deviceContext, IDXGISwapChain** _swapChain)
{
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    SwapChainDesc.BufferDesc.Width = GetResolutionWidth();
    SwapChainDesc.BufferDesc.Height = GetResolutionHeight();
    SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
    SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SwapChainDesc.SampleDesc.Count = 1;
    SwapChainDesc.SampleDesc.Quality = 0;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.BufferCount = 1;
    SwapChainDesc.OutputWindow = MGameWindow::GetWindowHandle();
    SwapChainDesc.Windowed = TRUE;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    SwapChainDesc.Flags = 0;

    UINT CreateDeviceAndSwapChainFlags = 0u;
#ifdef _DEBUG
    CreateDeviceAndSwapChainFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    CHECK_HRESULT(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, CreateDeviceAndSwapChainFlags, nullptr, 0,D3D11_SDK_VERSION,
        &SwapChainDesc, _swapChain, _device,nullptr, _deviceContext));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateRenderTargetView(ID3D11Device* _device, IDXGISwapChain* _swapChain,  ID3D11Resource** _backBufferResource, ID3D11RenderTargetView** _renderTargetView)
{
    CHECK_HRESULT(_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(_backBufferResource)));
    CHECK_HRESULT(_device->CreateRenderTargetView(*_backBufferResource, nullptr, _renderTargetView));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateAndSetSampleStates()
{
    D3D11_SAMPLER_DESC samplerDesc = {};
    
    {
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.BorderColor[0] = TColorI::Magenta.ToFloat().r;
        samplerDesc.BorderColor[1] = TColorI::Magenta.ToFloat().g;
        samplerDesc.BorderColor[2] = TColorI::Magenta.ToFloat().b;
        samplerDesc.BorderColor[3] = 1.0f;
        G_PIPELINE.Device->CreateSamplerState(&samplerDesc, &G_SAMPLER_STATES[0]);
    }

    {
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.BorderColor[0] = TColorI::Magenta.ToFloat().r;
        samplerDesc.BorderColor[1] = TColorI::Magenta.ToFloat().g;
        samplerDesc.BorderColor[2] = TColorI::Magenta.ToFloat().b;
        samplerDesc.BorderColor[3] = 1.0f;
        G_PIPELINE.Device->CreateSamplerState(&samplerDesc, &G_SAMPLER_STATES[1]);
    }

    {
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.BorderColor[0] = TColorI::Magenta.ToFloat().r;
        samplerDesc.BorderColor[1] = TColorI::Magenta.ToFloat().g;
        samplerDesc.BorderColor[2] = TColorI::Magenta.ToFloat().b;
        samplerDesc.BorderColor[3] = 1.0f;
        G_PIPELINE.Device->CreateSamplerState(&samplerDesc, &G_SAMPLER_STATES[2]);
    }
    
    G_PIPELINE.DeviceContext->PSSetSamplers(0, ARRAYSIZE(G_SAMPLER_STATES), G_SAMPLER_STATES);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateDepthStencil(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SDepthStencilResources& depthStencilResources)
{
    // Zbuffer for shadow mapping
    {
        // Depth Texture
        {
            D3D11_TEXTURE2D_DESC textureDesc = {};
            textureDesc.Width = 1920;
            textureDesc.Height = 1080;
            textureDesc.MipLevels = 1;
            textureDesc.ArraySize = 1;
            textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
            textureDesc.SampleDesc.Count = 1;
            textureDesc.Usage = D3D11_USAGE_DEFAULT;
            textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
            textureDesc.CPUAccessFlags = 0;
            textureDesc.MiscFlags = 0;

            CHECK_HRESULT(_device->CreateTexture2D(&textureDesc, nullptr, &depthStencilResources.TextureLight));
        }
        // Shader Resource View
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
            shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            CHECK_HRESULT(G_PIPELINE.Device->CreateShaderResourceView(depthStencilResources.TextureLight, &shaderResourceViewDesc, &G_DEPTH_STENCIL_RESOURCES.ResourceViewLight));
        }
        // Depth Stencil View
        {
            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
            depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Texture2D.MipSlice = 0u;

            CHECK_HRESULT(G_PIPELINE.Device->CreateDepthStencilView(depthStencilResources.TextureLight, &depthStencilViewDesc, &depthStencilResources.ViewLight));
        }
    }

    // ZBuffer for classic draw pass
    {
        // Depth Texture
        {
            D3D11_TEXTURE2D_DESC textureDesc = {};
            textureDesc.Width = GetResolutionWidth();
            textureDesc.Height = GetResolutionHeight();
            textureDesc.MipLevels = 1;
            textureDesc.ArraySize = 1;
            textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
            textureDesc.SampleDesc.Count = 1;
            textureDesc.Usage = D3D11_USAGE_DEFAULT;
            textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
            textureDesc.CPUAccessFlags = 0;
            textureDesc.MiscFlags = 0;

            CHECK_HRESULT(_device->CreateTexture2D(&textureDesc, nullptr, &depthStencilResources.Texture));
        }
        // Shader Resource View
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
            shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            CHECK_HRESULT(G_PIPELINE.Device->CreateShaderResourceView(depthStencilResources.Texture, &shaderResourceViewDesc, &G_DEPTH_STENCIL_RESOURCES.ResourceView));
        }
        // Depth Stencil View
        {
            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
            depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Texture2D.MipSlice = 0u;

            CHECK_HRESULT(G_PIPELINE.Device->CreateDepthStencilView(depthStencilResources.Texture, &depthStencilViewDesc, &depthStencilResources.View));
        }
    }
   
    
    // Depth Stencil State
    {
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
        G_PIPELINE.Device->CreateDepthStencilState(&depthStencilDesc, &depthStencilResources.State);
    }
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetVertexAndIndexBuffer(ID3D11DeviceContext* _deviceContext, ID3D11Buffer** _vertexBuffer, ID3D11Buffer* _indexBuffer, UINT _vertexBufferByteStride)
{
    constexpr UINT offset = 0u;
    _deviceContext->IASetVertexBuffers(0u, 1u, _vertexBuffer, &_vertexBufferByteStride, &offset);
    _deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, offset);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetPixelShaderTextureViews(ID3D11DeviceContext* _deviceContext, UINT nbTextures, ID3D11ShaderResourceView** _textureViews)
{
    _deviceContext->PSSetShaderResources(0u, nbTextures, _textureViews);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetPrimitiveAndDraw_Instanced(ID3D11DeviceContext* _deviceContext, UINT _indexCountPerInstance, UINT _instanceCount)
{
    _deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _deviceContext->DrawIndexedInstanced(_indexCountPerInstance, _instanceCount, 0u, 0, 0u);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::PresentSwapChain(IDXGISwapChain* _swapChain)
{
    CHECK_HRESULT(_swapChain->Present(1u, 0u));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::ClearRenderTarget(ID3D11DeviceContext* _deviceContext, ID3D11RenderTargetView* _renderTarget)
{
    constexpr float ClearColor[4] = { 0.059f, 0.085f, 0.141f, 1.0f };
    _deviceContext->ClearRenderTargetView(_renderTarget, ClearColor);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::ClearDepthStencil(ID3D11DeviceContext* _deviceContext, ID3D11DepthStencilView* _stencilView)
{
    _deviceContext->ClearDepthStencilView(_stencilView, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateVertexBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11Buffer** _vertexBuffer, void* _vertices, UINT _nbVertices, UINT _sizeStruct)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    {
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = _sizeStruct * _nbVertices;
        bufferDesc.StructureByteStride = _sizeStruct;
    }

    D3D11_SUBRESOURCE_DATA subResourceData = {};
    {
        subResourceData.pSysMem = _vertices;
    }

    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, _vertexBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateIndexBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11Buffer** _indexBuffer, TVertexIndex* _indexes, UINT _nbIndex, UINT _sizeStruct)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0u;
    bufferDesc.MiscFlags = 0u;
    bufferDesc.ByteWidth = _sizeStruct * _nbIndex;
    bufferDesc.StructureByteStride = _sizeStruct;
    
    D3D11_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pSysMem = _indexes;

    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, _indexBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateVertexShaderBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11Buffer** VertexConstantBuffer, UINT _size)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0u;
    bufferDesc.ByteWidth = _size;
    bufferDesc.StructureByteStride = 0u;
    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, nullptr, VertexConstantBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::Rasterize(ID3D11DeviceContext* _deviceContext, ID3D11RasterizerState* _rasterizerState)
{
    _deviceContext->RSSetState(_rasterizerState);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::ConfigureViewport(ID3D11DeviceContext* _deviceContext)
{
    D3D11_VIEWPORT viewportInfos;
    {
        viewportInfos.Height = (float)GetResolutionHeight();
        viewportInfos.Width = (float)GetResolutionWidth();
        viewportInfos.TopLeftX = 0.0f;
        viewportInfos.TopLeftY = 0.0f;
        viewportInfos.MinDepth = 0.0f;
        viewportInfos.MaxDepth = 1.0f;
    }
    _deviceContext->RSSetViewports(1u, &viewportInfos);
}