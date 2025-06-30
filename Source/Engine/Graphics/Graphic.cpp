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
#include "../Debug/DebugDraw.h"
#include "../Debug/DebugGUIManager.h"
#include "../Debug/Profiling.h"

enum class EDebugDrawTarget
{
    NONE,
    Z_BUFFER_SHADOW,
    Z_BUFFER_SCENE,
};

EDebugDrawTarget DebugDrawTarget = EDebugDrawTarget::NONE;
///---------------------------------------------------------------------------------------------------------------------
int GetResolutionWidth() { return MGameWindow::GetGameWindowWidth(); }
int GetResolutionHeight() { return MGameWindow::GetGameWindowHeight(); }
///---------------------------------------------------------------------------------------------------------------------
static SDepthStencilResources G_DEPTH_STENCIL_RESOURCES;
static ID3D11SamplerState* G_SAMPLER_STATES[3] = { nullptr, nullptr, nullptr};
///---------------------------------------------------------------------------------------------------------------------
ID3D11Device* G_DEVICE = nullptr;
ID3D11DeviceContext* G_DEVICE_CONTEXT = nullptr;
IDXGISwapChain* G_SWAP_CHAIN = nullptr;
ID3D11RasterizerState* G_RASTERIZER_STATE = nullptr;
///---------------------------------------------------------------------------------------------------------------------
ID3D11RenderTargetView* G_RENDER_TARGET_VIEW = nullptr;
ID3D11Resource* G_BACK_BUFFER_RESOURCE = nullptr;
ID3D11Texture2D* G_BACK_BUFFER_TEXTURE = nullptr;
///---------------------------------------------------------------------------------------------------------------------
static SVertexShader G_VS_SHADOW;
static SVertexShader G_VS_BASE;
static SVertexShader G_VS_2D_DEBUG;
static SVertexShader G_VS_DEBUG_DRAW;
///---------------------------------------------------------------------------------------------------------------------
static SPixelShader G_PS_BASE;
static SPixelShader G_PS_2D_DEBUG;
static SPixelShader G_PS_POST_PROCESS_1;
static SPixelShader G_PS_SIMPLE_COLOR;
///---------------------------------------------------------------------------------------------------------------------
static SShaderBufferHolder G_VS_BUFFERS[14];
static SShaderBufferHolder G_PS_BUFFERS[14];
///---------------------------------------------------------------------------------------------------------------------
ID3D11Device* MGraphic::GetDXDevice()
{
    return G_DEVICE;
}
///---------------------------------------------------------------------------------------------------------------------
ID3D11DeviceContext* MGraphic::GetDXDeviceContext()
{
    return G_DEVICE_CONTEXT;
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::ReportLiveObjects(bool _showDetails)
{
#if DEBUG_ENABLE_DIRECTX_DEVICE_DEBUG
    OutputDebugStringA("ReportLiveDeviceObjects - Start --------------------------\n");
    ID3D11Debug* debug = nullptr;
    CHECK_HRESULT(G_DEVICE->QueryInterface(__uuidof(ID3D11Debug), (void**)&debug));
    CHECK_HRESULT(debug->ReportLiveDeviceObjects(_showDetails == true ? D3D11_RLDO_DETAIL : D3D11_RLDO_SUMMARY));
    debug->Release();
    OutputDebugStringA("ReportLiveDeviceObjects - End --------------------------\n");
#endif
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::InitializeGraphic()
{
    MGraphic::CreateDeviceAndSwapChain(&G_DEVICE, &G_DEVICE_CONTEXT, &G_SWAP_CHAIN);
    MGraphic::CreateRenderTargetView();
    MGraphic::CreateDepthStencil(G_DEVICE, G_DEVICE_CONTEXT, G_DEPTH_STENCIL_RESOURCES);
    MGraphic::CreateRasterizerState(G_DEVICE, &G_RASTERIZER_STATE);
    MGraphic::ConfigureViewport(G_DEVICE_CONTEXT);
    MGraphic::CreateAndSetSampleStates();
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::InitializeShaders()
{ 
    // Initialize Vertex Shaders
    {
        G_VS_2D_DEBUG.CreateVertexShader(G_DEVICE,     TAKU_ASSET_SHADER_VS_2D,         VS_INPUT_DESC::POS_UV,                  ARRAYSIZE(VS_INPUT_DESC::POS_UV));
        G_VS_SHADOW.CreateVertexShader(G_DEVICE,       TAKU_ASSET_SHADER_VS_SHADOW,     VS_INPUT_DESC::POS_INST,                ARRAYSIZE(VS_INPUT_DESC::POS_INST));
        G_VS_BASE.CreateVertexShader(G_DEVICE,         TAKU_ASSET_SHADER_VS_BASE,       VS_INPUT_DESC::POS_NORM_TAN_UV_INST,    ARRAYSIZE(VS_INPUT_DESC::POS_NORM_TAN_UV_INST));
        G_VS_DEBUG_DRAW.CreateVertexShader(G_DEVICE,   TAKU_ASSET_SHADER_VS_DEBUG_DRAW, VS_INPUT_DESC::ID_INST,                 ARRAYSIZE(VS_INPUT_DESC::ID_INST));
    }
    
    // Initialize Pixel Shaders
    {
        G_PS_BASE.CreatePixelShader(G_DEVICE,            TAKU_ASSET_SHADER_PS_BASE);
        G_PS_2D_DEBUG.CreatePixelShader(G_DEVICE,        TAKU_ASSET_SHADER_PS_DEBUG_SCREEN);
        G_PS_POST_PROCESS_1.CreatePixelShader(G_DEVICE,  TAKU_ASSET_SHADER_PS_POST_PROCESS_1);
        G_PS_SIMPLE_COLOR.CreatePixelShader(G_DEVICE,    TAKU_ASSET_SHADER_PS_SIMPLE_COLOR);
    }
    
    // Initialize Vertex Shader Buffers
    {
        G_VS_BUFFERS[0].CreateShaderBuffer(EShaderType::VERTEX_SHADER, 0u, sizeof(b00_vs_buffer_sceneEachFrame));
        G_VS_BUFFERS[1].CreateShaderBuffer(EShaderType::VERTEX_SHADER, 1u, sizeof(b01_vs_buffer_object));
        G_VS_BUFFERS[13].CreateShaderBuffer(EShaderType::VERTEX_SHADER, 13u, sizeof(b13_vs_buffer_debug_draw_line));
    }
    
    // Initialize Pixel Shader Buffers
    {
        G_PS_BUFFERS[0].CreateShaderBuffer(EShaderType::PIXEL_SHADER, 0u, sizeof(b00_ps_buffer_sceneEachFrame));
    }
}
///---------------------------------------------------------------------------------------------------------------------
void DrawRectToScreenSpace(TVector2f TopLeft, TVector2f BotRight)
{
    ID3D11Buffer* VertexBuffer = nullptr;
    ID3D11Buffer* IndexBuffer = nullptr;
    
    struct SVertex2D { TVector4f pos; TVector2f uv; } Vertexes[4] =
    {
        {{ TopLeft.x, TopLeft.y, 0.5f, 1.0f }, {0.0f, 0.0f}},
        {{ BotRight.x, TopLeft.y, 0.5f, 1.0f }, {1.0f, 0.0f}},
        {{ BotRight.x, BotRight.y, 0.5f, 1.0f }, {1.0f, 1.0f}},
        {{ TopLeft.x, BotRight.y,  0.5f, 1.0f }, {0.0f, 1.0f}}
    };
    UINT sizeVec = sizeof(TVector3f);
    UINT sizeVertex = sizeof(SVertex2D);
    TVertexIndex Indexes[6] = { 2, 1, 0, 2, 0, 3 };
    MGraphic::CreateVertexBuffer(G_DEVICE, G_DEVICE_CONTEXT, &VertexBuffer, Vertexes, ARRAYSIZE(Vertexes), sizeof(SVertex2D));
    MGraphic::CreateIndexBuffer(G_DEVICE, G_DEVICE_CONTEXT, &IndexBuffer, Indexes, ARRAYSIZE(Indexes), sizeof(TVertexIndex));
    MGraphic::SetVertexAndIndexBuffer(G_DEVICE_CONTEXT, &VertexBuffer, IndexBuffer, sizeof(SVertex2D));

    G_DEVICE_CONTEXT->DrawIndexed(ARRAYSIZE(Indexes), 0u, 0u);
    
    VertexBuffer->Release();
    IndexBuffer->Release();
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::RenderFrame_SceneShadowMap()
{
    const std::vector<CDrawable_InstancedMesh*>& instancedMeshes = MWorld::GetWorld()->CurrentGameScene->InstancedMeshes;

    ID3D11RenderTargetView* NullRenderTarget = nullptr;
    G_DEVICE_CONTEXT->OMSetRenderTargets(1u, &NullRenderTarget, G_DEPTH_STENCIL_RESOURCES.ViewLight);
    G_DEVICE_CONTEXT->VSSetShader(G_VS_SHADOW.Shader, nullptr, 0u);
    G_DEVICE_CONTEXT->IASetInputLayout(G_VS_SHADOW.Input);
    G_DEVICE_CONTEXT->PSSetShader(nullptr, nullptr, 0u);
    SShaderBufferHolder::FillBuffer_VS_SceneEachFrame(&G_VS_BUFFERS[0], true);
    
    for (int i = 0; i < instancedMeshes.size(); ++i)
    {
        CDrawable_InstancedMesh* instancedMesh = instancedMeshes[i];
        if (instancedMesh->CastShadow == false) continue;
        
        MGraphic::SetVertexAndIndexBuffer(G_DEVICE_CONTEXT, &instancedMesh->VertexBuffer, instancedMesh->IndexBuffer, SMeshData::VertexBuffer_StructureByteStride);
        
        const UINT nbInstances = (UINT)instancedMesh->Instances.size();
        UINT nbInstancesRemainingToDraw = nbInstances;
        while (nbInstancesRemainingToDraw > 0)
        {
            const UINT nbInstancesToDraw = MMath::Min(nbInstancesRemainingToDraw, MAX_INSTANCE_COUNT - 1);
            const UINT startInstances = nbInstances - nbInstancesRemainingToDraw;
            
            SShaderBufferHolder::FillBuffer_VS_Object(&G_VS_BUFFERS[1], instancedMesh->Instances.data(), startInstances, nbInstancesToDraw);
            MGraphic::SetPrimitiveAndDraw_Instanced(G_DEVICE_CONTEXT, instancedMesh->MeshData->IndexCount, nbInstancesToDraw + 1);
            
            nbInstancesRemainingToDraw -= nbInstancesToDraw;
        }
    }
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::RenderFrame_Scene()
{
    const std::vector<CDrawable_InstancedMesh*>& instancedMeshes = MWorld::GetWorld()->CurrentGameScene->InstancedMeshes;
    
    G_DEVICE_CONTEXT->OMSetRenderTargets(1u, &G_RENDER_TARGET_VIEW, G_DEPTH_STENCIL_RESOURCES.View);
    G_DEVICE_CONTEXT->VSSetShader(G_VS_BASE.Shader, nullptr, 0u);
    G_DEVICE_CONTEXT->IASetInputLayout(G_VS_BASE.Input);
    SShaderBufferHolder::FillBuffer_VS_SceneEachFrame(&G_VS_BUFFERS[0], false);
    G_DEVICE_CONTEXT->PSSetShader(G_PS_BASE.Shader, nullptr, 0u);
    SShaderBufferHolder::FillBuffer_PS_SceneEachFrame(&G_PS_BUFFERS[0]);
    
    for (int i = 0; i < instancedMeshes.size(); ++i)
    {
        CDrawable_InstancedMesh* instancedMesh = instancedMeshes[i];
        
        MGraphic::SetVertexAndIndexBuffer(G_DEVICE_CONTEXT, &instancedMesh->VertexBuffer, instancedMesh->IndexBuffer, SMeshData::VertexBuffer_StructureByteStride);
        
        ID3D11ShaderResourceView* ShaderResourceViews[] =
            {
                (instancedMesh->ColorTexture->textureView),
                (G_DEPTH_STENCIL_RESOURCES.ResourceViewLight),
                (instancedMesh->NormalTexture == nullptr ? nullptr : instancedMesh->NormalTexture->textureView),
                (!instancedMesh->EmissionTexture ? nullptr : instancedMesh->EmissionTexture->textureView),
                (!instancedMesh->MROTexture ? nullptr : instancedMesh->MROTexture->textureView),
            };
        
        MGraphic::SetPixelShaderTextureViews(G_DEVICE_CONTEXT, ARRAYSIZE(ShaderResourceViews), ShaderResourceViews);
   
        // TODO Julien Rogel (12/03/2025): Try Structured Buffers to avoid filling multiples times the buffers
        const UINT nbInstances = (UINT)instancedMesh->Instances.size();
        UINT nbInstancesRemainingToDraw = nbInstances;
        while (nbInstancesRemainingToDraw > 0)
        {
            const UINT nbInstancesToDraw = MMath::Min(nbInstancesRemainingToDraw, MAX_INSTANCE_COUNT - 1);
            const UINT startInstances = nbInstances - nbInstancesRemainingToDraw;

            SShaderBufferHolder::FillBuffer_VS_Object(&G_VS_BUFFERS[1], instancedMesh->Instances.data(), startInstances, nbInstancesToDraw);
            MGraphic::SetPrimitiveAndDraw_Instanced(G_DEVICE_CONTEXT, instancedMesh->MeshData->IndexCount, nbInstancesToDraw + 1);
            
            nbInstancesRemainingToDraw -= nbInstancesToDraw;
        }
    }
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::RenderFrame_PostProcess()
{
    // Copy the render target to a new texture
    D3D11_TEXTURE2D_DESC desc;
    G_BACK_BUFFER_TEXTURE->GetDesc(&desc);
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    desc.Usage = D3D11_USAGE_DEFAULT; 
    desc.CPUAccessFlags = 0;
    desc.MiscFlags = 0;
    ID3D11Texture2D* renderTexture;
    ID3D11ShaderResourceView* renderShaderResourceView;
    CHECK_HRESULT(G_DEVICE->CreateTexture2D(&desc, nullptr, &renderTexture));
    CHECK_HRESULT(G_DEVICE->CreateShaderResourceView(renderTexture, nullptr, &renderShaderResourceView));
    G_DEVICE_CONTEXT->CopyResource(renderTexture, G_BACK_BUFFER_RESOURCE);

    // setups shaders and resources
    G_DEVICE_CONTEXT->OMSetRenderTargets(1u, &G_RENDER_TARGET_VIEW, nullptr);
    G_DEVICE_CONTEXT->VSSetShader(G_VS_2D_DEBUG.Shader, nullptr, 0u);
    G_DEVICE_CONTEXT->IASetInputLayout(G_VS_2D_DEBUG.Input);
    G_DEVICE_CONTEXT->PSSetShader(G_PS_POST_PROCESS_1.Shader, nullptr, 0u);
    ID3D11ShaderResourceView* ShaderResources[] = { renderShaderResourceView, G_DEPTH_STENCIL_RESOURCES.ResourceView };
    G_DEVICE_CONTEXT->PSSetShaderResources(0u, ARRAYSIZE(ShaderResources), ShaderResources);
        
    DrawRectToScreenSpace({-1.0f, 1.0f}, {1.0f, -1.0f} );

    renderShaderResourceView->Release();
    renderTexture->Release();
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::RenderFrame_DebugLines()
{
    ID3D11Buffer* VertexBuffer = nullptr;
    ID3D11Buffer* IndexBuffer = nullptr;
    
    TVertexIndex indexBufferData[] = { 0, 1 };
    struct SInputVertexBuffer
    {
        uint32_t index;
    };
    SInputVertexBuffer vertexBufferData[] = { 0, 1 };
    
    
    D3D11_RASTERIZER_DESC rasterizerDesc;
    G_RASTERIZER_STATE->GetDesc(&rasterizerDesc);
    rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
    rasterizerDesc.CullMode = D3D11_CULL_NONE;
    ID3D11RasterizerState* newRasterizerState;
    G_DEVICE->CreateRasterizerState(&rasterizerDesc, &newRasterizerState);
    G_DEVICE_CONTEXT->RSSetState(newRasterizerState);
    
    MGraphic::CreateIndexBuffer(G_DEVICE, G_DEVICE_CONTEXT, &IndexBuffer, indexBufferData, ARRAYSIZE(indexBufferData), sizeof(TVertexIndex));
    MGraphic::CreateVertexBuffer(G_DEVICE, G_DEVICE_CONTEXT, &VertexBuffer, vertexBufferData, ARRAYSIZE(vertexBufferData), sizeof(SInputVertexBuffer));
    
    G_DEVICE_CONTEXT->OMSetRenderTargets(1u, &G_RENDER_TARGET_VIEW, nullptr);
    G_DEVICE_CONTEXT->VSSetShader(G_VS_DEBUG_DRAW.Shader, nullptr, 0u);
    G_DEVICE_CONTEXT->PSSetShader(G_PS_SIMPLE_COLOR.Shader, nullptr, 0u);
    G_DEVICE_CONTEXT->IASetInputLayout(G_VS_DEBUG_DRAW.Input);
    
    MGraphic::SetVertexAndIndexBuffer(G_DEVICE_CONTEXT, &VertexBuffer, IndexBuffer, sizeof(SInputVertexBuffer));
    
    const std::vector<SDrawDebugHolder>& DebugDraws = MDebugDraw::GetDebugLines();
    const UINT nbInstances = (UINT)DebugDraws.size();
    std::vector<SDebugLine> debugLines;
    debugLines.reserve(nbInstances);
    for (auto debugDraw : DebugDraws)
    {
        debugLines.push_back(debugDraw.DebugLine);
    }
    
    // TODO Julien Rogel (12/03/2025): Try Structured Buffers to avoid filling multiples times the buffers
    UINT nbInstancesRemainingToDraw = nbInstances;
    while (nbInstancesRemainingToDraw > 0)
    {
        const UINT nbInstancesToDraw = MMath::Min(nbInstancesRemainingToDraw, 416u);
        const UINT startInstances = nbInstances - nbInstancesRemainingToDraw;

        
        SShaderBufferHolder::FillBuffer_VS_DebugLine(&G_VS_BUFFERS[13], debugLines.data(), startInstances, nbInstancesToDraw);
        G_DEVICE_CONTEXT->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
        G_DEVICE_CONTEXT->DrawIndexedInstanced(2u, nbInstancesToDraw + 1u, 0u, 0, 0u);
        
        nbInstancesRemainingToDraw -= nbInstancesToDraw;
    }
    
    G_DEVICE_CONTEXT->RSSetState(G_RASTERIZER_STATE);
    
    VertexBuffer->Release();
    IndexBuffer->Release();
    newRasterizerState->Release();
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::RenderFrame_DebugScreen()
{
    if (DebugDrawTarget != EDebugDrawTarget::NONE)
    {
        G_DEVICE_CONTEXT->OMSetRenderTargets(1u, &G_RENDER_TARGET_VIEW, nullptr);

        G_DEVICE_CONTEXT->VSSetShader(G_VS_2D_DEBUG.Shader, nullptr, 0u);
        G_DEVICE_CONTEXT->IASetInputLayout(G_VS_2D_DEBUG.Input);
        G_DEVICE_CONTEXT->PSSetShader(G_PS_2D_DEBUG.Shader, nullptr, 0u);

        switch (DebugDrawTarget) {
        case EDebugDrawTarget::Z_BUFFER_SHADOW:
            {
                G_DEVICE_CONTEXT->PSSetShaderResources(0, 1u, &G_DEPTH_STENCIL_RESOURCES.ResourceViewLight);
            } break;
        case EDebugDrawTarget::Z_BUFFER_SCENE:
            {
                G_DEVICE_CONTEXT->PSSetShaderResources(0, 1u, &G_DEPTH_STENCIL_RESOURCES.ResourceView);
            } break;
        }
        DrawRectToScreenSpace({-1.0f, 1.0f}, {-0.25f, 0.25f} );
    }
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::RenderFrame()
{
    MGraphic::SetRasterizerState(G_DEVICE_CONTEXT, G_RASTERIZER_STATE);
    
    RenderFrame_SceneShadowMap();
    RenderFrame_Scene();
    RenderFrame_PostProcess();
    RenderFrame_DebugLines();
    RenderFrame_DebugScreen();

    MDebugGUI::StartFrameDebugGUI();
    MDebugGUI::EndFrameDebugGUI();
    
    MGraphic::PresentSwapChain(G_SWAP_CHAIN);
    
    constexpr ID3D11ShaderResourceView* const nullResources[4] = { nullptr, nullptr, nullptr, nullptr };
    G_DEVICE_CONTEXT->PSSetShaderResources(0, ARRAYSIZE(nullResources), nullResources);
    MGraphic::ClearRenderTarget(G_DEVICE_CONTEXT, G_RENDER_TARGET_VIEW);
    MGraphic::ClearDepthStencil(G_DEVICE_CONTEXT, G_DEPTH_STENCIL_RESOURCES.View);
    MGraphic::ClearDepthStencil(G_DEVICE_CONTEXT, G_DEPTH_STENCIL_RESOURCES.ViewLight);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::PrepareUninitializeGraphic()
{
    ID3D11Buffer* nullBuffer[14] = { nullptr };
    G_DEVICE_CONTEXT->VSSetConstantBuffers(0, ARRAYSIZE(nullBuffer), nullBuffer);
    G_DEVICE_CONTEXT->PSSetConstantBuffers(0, ARRAYSIZE(nullBuffer), nullBuffer);

    ID3D11Buffer* nullIndexBuffer = nullptr;
    UINT zero = 0;
    G_DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &nullIndexBuffer, &zero, &zero);
    G_DEVICE_CONTEXT->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
    
    ID3D11RenderTargetView* nullRTV[] = { nullptr };
    ID3D11ShaderResourceView* nullSRV[6] = { nullptr };
    G_DEVICE_CONTEXT->OMSetRenderTargets(1, nullRTV, nullptr);
    G_DEVICE_CONTEXT->PSSetShaderResources(0, ARRAYSIZE(nullSRV), nullSRV);
    G_DEVICE_CONTEXT->OMSetRenderTargets(0, nullptr, nullptr);
    
    CHECK_HRESULT(G_SWAP_CHAIN->SetFullscreenState(FALSE, nullptr));

    G_DEVICE_CONTEXT->ClearState();
    G_DEVICE_CONTEXT->Flush();
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::UninitializeGraphic()
{
    G_DEPTH_STENCIL_RESOURCES.Release();
    G_RASTERIZER_STATE->Release();
    
    G_BACK_BUFFER_TEXTURE->Release();
    G_BACK_BUFFER_RESOURCE->Release();
    G_RENDER_TARGET_VIEW->Release();
    G_SWAP_CHAIN->Release();
    
    G_VS_SHADOW.Release();
    G_VS_BASE.Release();
    G_VS_2D_DEBUG.Release();
    G_PS_BASE.Release();
    G_PS_2D_DEBUG.Release();
    G_PS_POST_PROCESS_1.Release();

    for (auto VertexShaderBuffer : G_VS_BUFFERS)
    {
        VertexShaderBuffer.Release();
    }
    for (auto PixelShaderBuffer : G_PS_BUFFERS)
    {
        PixelShaderBuffer.Release();
    }
    for (auto samplerState : G_SAMPLER_STATES)
    {
        samplerState->Release();
    }
    
    G_DEVICE_CONTEXT->Release();
    
    G_DEVICE_CONTEXT->Flush();
    
#if DEBUG_DIRECTX_REPORT_AT_QUIT
        ReportLiveObjects(true);
#endif
    
    G_DEVICE->Release();
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
#if DEBUG_ENABLE_DIRECTX_DEVICE_DEBUG
    CreateDeviceAndSwapChainFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    
    CHECK_HRESULT(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
        nullptr, CreateDeviceAndSwapChainFlags, nullptr, 0,D3D11_SDK_VERSION,
        &SwapChainDesc, _swapChain, _device,nullptr, _deviceContext));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateRenderTargetView()
{
    CHECK_HRESULT(G_SWAP_CHAIN->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&G_BACK_BUFFER_TEXTURE));
    CHECK_HRESULT(G_SWAP_CHAIN->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&G_BACK_BUFFER_RESOURCE)));
    CHECK_HRESULT(G_DEVICE->CreateRenderTargetView(G_BACK_BUFFER_RESOURCE, nullptr, &G_RENDER_TARGET_VIEW));
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
        samplerDesc.BorderColor[0] = TColor::Magenta.ToFloat().r;
        samplerDesc.BorderColor[1] = TColor::Magenta.ToFloat().g;
        samplerDesc.BorderColor[2] = TColor::Magenta.ToFloat().b;
        samplerDesc.BorderColor[3] = 1.0f;
        G_DEVICE->CreateSamplerState(&samplerDesc, &G_SAMPLER_STATES[0]);
    }

    {
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.BorderColor[0] = TColor::Magenta.ToFloat().r;
        samplerDesc.BorderColor[1] = TColor::Magenta.ToFloat().g;
        samplerDesc.BorderColor[2] = TColor::Magenta.ToFloat().b;
        samplerDesc.BorderColor[3] = 1.0f;
        G_DEVICE->CreateSamplerState(&samplerDesc, &G_SAMPLER_STATES[1]);
    }

    {
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.BorderColor[0] = TColor::Magenta.ToFloat().r;
        samplerDesc.BorderColor[1] = TColor::Magenta.ToFloat().g;
        samplerDesc.BorderColor[2] = TColor::Magenta.ToFloat().b;
        samplerDesc.BorderColor[3] = 1.0f;
        G_DEVICE->CreateSamplerState(&samplerDesc, &G_SAMPLER_STATES[2]);
    }
    
    G_DEVICE_CONTEXT->PSSetSamplers(0, ARRAYSIZE(G_SAMPLER_STATES), G_SAMPLER_STATES);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateDepthStencil(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SDepthStencilResources& depthStencilResources)
{
    // Zbuffer for shadow mapping
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

            CHECK_HRESULT(_device->CreateTexture2D(&textureDesc, nullptr, &depthStencilResources.TextureLight));
        }
        // Shader Resource View
        {
            D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc = {};
            shaderResourceViewDesc.Format = DXGI_FORMAT_R32_FLOAT;
            shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
            shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
            shaderResourceViewDesc.Texture2D.MipLevels = 1;

            CHECK_HRESULT(G_DEVICE->CreateShaderResourceView(depthStencilResources.TextureLight, &shaderResourceViewDesc, &G_DEPTH_STENCIL_RESOURCES.ResourceViewLight));
        }
        // Depth Stencil View
        {
            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
            depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Texture2D.MipSlice = 0u;

            CHECK_HRESULT(G_DEVICE->CreateDepthStencilView(depthStencilResources.TextureLight, &depthStencilViewDesc, &depthStencilResources.ViewLight));
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

            CHECK_HRESULT(G_DEVICE->CreateShaderResourceView(depthStencilResources.Texture, &shaderResourceViewDesc, &G_DEPTH_STENCIL_RESOURCES.ResourceView));
        }
        // Depth Stencil View
        {
            D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
            depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
            depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            depthStencilViewDesc.Texture2D.MipSlice = 0u;

            CHECK_HRESULT(G_DEVICE->CreateDepthStencilView(depthStencilResources.Texture, &depthStencilViewDesc, &depthStencilResources.View));
        }
    }
    
    // Depth Stencil State
    {
        D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
        G_DEVICE->CreateDepthStencilState(&depthStencilDesc, &depthStencilResources.State);
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
#if DEBUG_ENABLE_DIRECTX_DEVICE_DEBUG
    constexpr char name[] = "TAKU_VERTEX_BUFFER";
    CHECK_HRESULT((*_vertexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, ARRAYSIZE(name), name));
#endif
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
#if DEBUG_ENABLE_DIRECTX_DEVICE_DEBUG
    constexpr char name[] = "TAKU_INDEX_BUFFER";
    CHECK_HRESULT((*_indexBuffer)->SetPrivateData(WKPDID_D3DDebugObjectName, ARRAYSIZE(name), name));
#endif
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetRasterizerState(ID3D11DeviceContext* _deviceContext, ID3D11RasterizerState* _rasterizerState)
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