#include "Graphic.h"

#include <d3d11.h>
#include <sstream>

#include "GameWindow.h"
#include "HResultHandler.h"
#include "Resources/MeshResources.h"
#include "Resources/ShadersResources.h"
#include "WICTextureLoader.h"
#include "World.h"
#include "Debug/Profiling.h"

#define GAME_DATA_PATH "Data/"
#define GAME_DATA_SHADER_PATH "Data/Shaders/"

///---------------------------------------------------------------------------------------------------------------------
static SGraphicResources_Pipeline G_PIPELINE;
static std::vector<CStaticMesh*> G_MESH_TO_DRAW;

void MGraphic::CreateRenderTargetView(ID3D11Device* _device, IDXGISwapChain* _swapChain,  ID3D11Resource** _backBufferResource, ID3D11RenderTargetView** _renderTargetView)
{
    CHECK_HRESULT(_swapChain->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(_backBufferResource)));
    CHECK_HRESULT(_device->CreateRenderTargetView(*_backBufferResource, nullptr, _renderTargetView));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::StartDrawPipeline()
{
    MGraphic::CreateDeviceAndSwapChain(&G_PIPELINE.Device, &G_PIPELINE.DeviceContext, &G_PIPELINE.SwapChain);
    MGraphic::CreateAndSetDepthStencilState(G_PIPELINE.Device, G_PIPELINE.DeviceContext, &G_PIPELINE.DepthStencilState);
    MGraphic::CreateDepthStencilTexture(G_PIPELINE.Device, &G_PIPELINE.DepthStencilTexture);
    MGraphic::CreateDepthStencilView(G_PIPELINE.Device, G_PIPELINE.DepthStencilTexture, &G_PIPELINE.DepthStencilView);
    MGraphic::CreateRenderTargetView(G_PIPELINE.Device, G_PIPELINE.SwapChain, &G_PIPELINE.BackBufferResource, &G_PIPELINE.RenderTargetView);
    MGraphic::SetDepthStencilViewToRenderTargetView(G_PIPELINE.DeviceContext, &G_PIPELINE.RenderTargetView, G_PIPELINE.DepthStencilView);
    MGraphic::CreateAndSetVertexShader(G_PIPELINE.Device, G_PIPELINE.DeviceContext, G_PIPELINE.VertexShaderData);
    MGraphic::CreateAndSetPixelShader(G_PIPELINE.Device, G_PIPELINE.DeviceContext, G_PIPELINE.PixelShaderData);
    MGraphic::CreatePixelShaderConstantBuffer(G_PIPELINE.Device, G_PIPELINE.DeviceContext, G_PIPELINE.PixelShaderData);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::DrawPipeline()
{
    MGraphic::SetPixelShaderConstantBuffer(G_PIPELINE.Device, G_PIPELINE.DeviceContext, G_PIPELINE.PixelShaderData);

    for (int i = 0; i < G_MESH_TO_DRAW.size(); ++i)
    {
        CStaticMesh* StaticMeshToDraw = G_MESH_TO_DRAW[i];
        
        MGraphic::SetVertexAndIndexBuffer(G_PIPELINE.DeviceContext, StaticMeshToDraw->GraphicResource);
        MGraphic::SetVertexShader(G_PIPELINE.Device, G_PIPELINE.DeviceContext, StaticMeshToDraw->GraphicResource, StaticMeshToDraw->Transform);
        MGraphic::SetPixelShader(G_PIPELINE.DeviceContext, StaticMeshToDraw->GraphicResource);
        MGraphic::SetPrimitiveAndDraw(G_PIPELINE.DeviceContext, *StaticMeshToDraw->MeshData);
    }
    
    MGraphic::ConfigureViewport(G_PIPELINE.DeviceContext);
    MGraphic::Rasterize(G_PIPELINE.Device, G_PIPELINE.DeviceContext);
    MGraphic::PresentSwapChain(G_PIPELINE.SwapChain);
    MGraphic::ClearRenderTarget(G_PIPELINE.DeviceContext, G_PIPELINE.RenderTargetView);
    MGraphic::ClearDepthStencil(G_PIPELINE.DeviceContext, G_PIPELINE.DepthStencilView);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::InitializeGraphic()
{
    MGraphic::StartDrawPipeline();
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::UninitializeGraphic()
{
    G_PIPELINE.VertexShaderData.Release();
    G_PIPELINE.PixelShaderData.Release();
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::AddMeshToDraw(const TTransform& _transform, const char* _meshName)
{
    CStaticMesh* StaticMesh = new CStaticMesh;
    StaticMesh->LoadMeshData(_transform, _meshName);
    
    MGraphic::CreateVertexBuffer(G_PIPELINE.Device, G_PIPELINE.DeviceContext, StaticMesh->GraphicResource, *StaticMesh->MeshData);
    MGraphic::CreateIndexBuffer(G_PIPELINE.Device, G_PIPELINE.DeviceContext, StaticMesh->GraphicResource, *StaticMesh->MeshData);
    MGraphic::CreateVertexShaderBuffer(G_PIPELINE.Device, G_PIPELINE.DeviceContext, StaticMesh->GraphicResource);
    
    G_MESH_TO_DRAW.push_back(StaticMesh);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateDeviceAndSwapChain(ID3D11Device** _device, ID3D11DeviceContext** _deviceContext, IDXGISwapChain** _swapChain)
{
    DXGI_SWAP_CHAIN_DESC SwapChainDesc;
    SwapChainDesc.BufferDesc.Width = MGameWindow::GetGameWindowWidth();
    SwapChainDesc.BufferDesc.Height = MGameWindow::GetGameWindowHeight();
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
void MGraphic::CreateAndSetDepthStencilState(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11DepthStencilState** _depthStencilState)
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    {
        depthStencilDesc.DepthEnable = true;
        depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    }
    CHECK_HRESULT(_device->CreateDepthStencilState(&depthStencilDesc, _depthStencilState));
    _deviceContext->OMSetDepthStencilState(*_depthStencilState, 1u);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateDepthStencilTexture(ID3D11Device* _device, ID3D11Texture2D** _depthStencilTexture)
{
    D3D11_TEXTURE2D_DESC DepthStencilTextureDesc = {};
    {
        DepthStencilTextureDesc.Width = MGameWindow::GetGameWindowWidth();
        DepthStencilTextureDesc.Height = MGameWindow::GetGameWindowHeight();
        DepthStencilTextureDesc.MipLevels = 1u;
        DepthStencilTextureDesc.ArraySize = 1u;
        DepthStencilTextureDesc.Format = DXGI_FORMAT_D32_FLOAT;
        DepthStencilTextureDesc.SampleDesc.Count = 1u;
        DepthStencilTextureDesc.SampleDesc.Quality = 0u;
        DepthStencilTextureDesc.Usage = D3D11_USAGE_DEFAULT;
        DepthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
        DepthStencilTextureDesc.CPUAccessFlags = 0u;
        DepthStencilTextureDesc.MiscFlags = 0u;
    }
    CHECK_HRESULT(_device->CreateTexture2D(&DepthStencilTextureDesc, nullptr, _depthStencilTexture));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateDepthStencilView(ID3D11Device* _device, ID3D11Texture2D* _depthStencilTexture, ID3D11DepthStencilView** _depthStencilView)
{
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
    {
        depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
        depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        depthStencilViewDesc.Texture2D.MipSlice = 0u;
    }
    CHECK_HRESULT(G_PIPELINE.Device->CreateDepthStencilView(_depthStencilTexture, &depthStencilViewDesc, _depthStencilView));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetDepthStencilViewToRenderTargetView(ID3D11DeviceContext* _deviceContext, ID3D11RenderTargetView** _renderTargetView, ID3D11DepthStencilView* _depthStencilView)
{
    _deviceContext->OMSetRenderTargets(1u, _renderTargetView, _depthStencilView);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetVertexAndIndexBuffer(ID3D11DeviceContext* _deviceContext, const SGraphicResources_Mesh& _graphicResource)
{
    constexpr UINT offset = 0u;
    _deviceContext->IASetVertexBuffers(0u, 1u, &_graphicResource.vertexBuffer, &SMeshData::VertexBuffer_StructureByteStride, &offset);
    _deviceContext->IASetIndexBuffer(_graphicResource.IndexBuffer, DXGI_FORMAT_R16_UINT, offset);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetPixelShader(ID3D11DeviceContext* _deviceContext, const SGraphicResources_Mesh& _graphicResource)
{
    _deviceContext->PSSetShaderResources(0u, 1u, &_graphicResource.TextureView);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetPrimitiveAndDraw(ID3D11DeviceContext* _deviceContext, const SMeshData& _meshData)
{
    _deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    _deviceContext->DrawIndexed(_meshData.IndexCount,  0u, 0);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::PresentSwapChain(IDXGISwapChain* _swapChain)
{
    CHECK_HRESULT(_swapChain->Present(1u, 0u));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::ClearRenderTarget(ID3D11DeviceContext* _deviceContext, ID3D11RenderTargetView* _renderTarget)
{
    constexpr float ClearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    _deviceContext->ClearRenderTargetView(_renderTarget, ClearColor);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::ClearDepthStencil(ID3D11DeviceContext* _deviceContext, ID3D11DepthStencilView* _stencilView)
{
    _deviceContext->ClearDepthStencilView(_stencilView, D3D11_CLEAR_DEPTH, 1.0f, 0u);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateAndSetVertexShader(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SVertexShader& _vertexShader)
{
    MShaderResources::CompileShader(GAME_DATA_SHADER_PATH L"VertexShader.hlsl", "vs_5_0", &_vertexShader.Blob);
    CHECK_HRESULT(_device->CreateVertexShader(_vertexShader.Blob->GetBufferPointer(), _vertexShader.Blob->GetBufferSize(), nullptr, &_vertexShader.Shader));
    // VertexShader Input Layout
    {
        constexpr D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
        {
            { "POSITION",  0u,  DXGI_FORMAT_R32G32B32_FLOAT,  0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
            { "UV",        0u,  DXGI_FORMAT_R32G32_FLOAT,     0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
            { "NORMAL",    0u,  DXGI_FORMAT_R32G32B32_FLOAT,  0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
            };
        UINT sizeInputElementDesc = 3u;
        CHECK_HRESULT(_device->CreateInputLayout(inputElementDesc, sizeInputElementDesc, _vertexShader.Blob->GetBufferPointer(), _vertexShader.Blob->GetBufferSize(), &_vertexShader.Input));
    }
    _deviceContext->VSSetShader(_vertexShader.Shader, nullptr, 0u);
    _deviceContext->IASetInputLayout(_vertexShader.Input);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateAndSetPixelShader(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SPixelShader& _pixelShader)
{
    MShaderResources::CompileShader(GAME_DATA_SHADER_PATH L"PixelShader.hlsl", "ps_5_0", &_pixelShader.Blob);
    CHECK_HRESULT(_device->CreatePixelShader(_pixelShader.Blob->GetBufferPointer(), _pixelShader.Blob->GetBufferSize(), nullptr, &_pixelShader.Shader));
    _deviceContext->PSSetShader(_pixelShader.Shader, nullptr, 0u);
    
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
    }
    
    CHECK_HRESULT(_device->CreateSamplerState(&samplerDesc, &_pixelShader.TextureSamplerState));
    _deviceContext->PSSetSamplers(0, 1, &_pixelShader.TextureSamplerState);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreatePixelShaderConstantBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SPixelShader& _pixelShader)
{
    SPixelShaderConstantBuffer ConstantBufferPixelShader;
    {
        ConstantBufferPixelShader.SunDirection = TVector3f::Zero;
        ConstantBufferPixelShader.SunAmbient = 0.0f;
    }
    D3D11_SUBRESOURCE_DATA subResourceData = {};
    {
        subResourceData.pSysMem = &ConstantBufferPixelShader;
    }
    
    D3D11_BUFFER_DESC bufferDesc = {};
    {
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = sizeof(SPixelShaderConstantBuffer);
        bufferDesc.StructureByteStride = 0u;
    }
    
    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, &_pixelShader.ConstantBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetPixelShaderConstantBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, const SPixelShader& _pixelShader)
{
    SPixelShaderConstantBuffer ConstantBufferPixelShader;
    {
        ConstantBufferPixelShader.SunDirection = MWorld::GetWorld()->GetSunDirection();
        ConstantBufferPixelShader.SunAmbient = 0.2f;
    }
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CHECK_HRESULT(_deviceContext->Map(_pixelShader.ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
    memcpy(mappedResource.pData, &ConstantBufferPixelShader, sizeof(SPixelShaderConstantBuffer));
    _deviceContext->Unmap(_pixelShader.ConstantBuffer, 0);
    _deviceContext->PSSetConstantBuffers(0u, 1u, &_pixelShader.ConstantBuffer);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateVertexBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SGraphicResources_Mesh& _meshPipeline, const SMeshData& _meshData)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    {
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = _meshData.VertexBuffer_ByteWidth;
        bufferDesc.StructureByteStride = SMeshData::VertexBuffer_StructureByteStride;
    }

    D3D11_SUBRESOURCE_DATA subResourceData = {};
    {
        subResourceData.pSysMem = _meshData.VertexBuffer.data();
    }

    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, &_meshPipeline.vertexBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateIndexBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SGraphicResources_Mesh& _meshPipeline, const SMeshData& _meshData)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    {
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = _meshData.IndexBuffer_ByteWidth;
        bufferDesc.StructureByteStride = SMeshData::IndexBuffer_StructureByteStride;
    }

    D3D11_SUBRESOURCE_DATA subResourceData = {};
    {
        subResourceData.pSysMem = _meshData.IndexBuffer.data();
    }

    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, &_meshPipeline.IndexBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateVertexShaderBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SGraphicResources_Mesh& _pipeline)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    {
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = sizeof(SVertexShaderConstantBuffer);
        bufferDesc.StructureByteStride = 0u;
    }
    
    D3D11_SUBRESOURCE_DATA subResourceData = {};
    {
        subResourceData.pSysMem = &_pipeline.VertexConstantBuffer;
    }
    
    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, &_pipeline.VertexConstantBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetVertexShader(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SGraphicResources_Mesh& _pipeline, const TTransform& _transform)
{
    SVertexShaderConstantBuffer constantBufferData;
    {
        const TMatrix4f objectWorldMatrix = TTransform::ToMatrix(_transform);
        const TMatrix4f invertedCameraMatrix = MWorld::GetWorld()->GetInverseCameraMatrix();
        const TMatrix4f perspectiveMatrix = MGameWindow::GetPerspectiveMatrix();
        TMatrix4f worldViewProjectionMatrix = TMatrix4f::Transpose(objectWorldMatrix * invertedCameraMatrix * TMatrix4f::View * perspectiveMatrix);
        
        constantBufferData.WorldViewProjection = worldViewProjectionMatrix;
    }

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CHECK_HRESULT(_deviceContext->Map(_pipeline.VertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
    memcpy(mappedResource.pData, &constantBufferData, sizeof(SVertexShaderConstantBuffer));
    _deviceContext->Unmap(_pipeline.VertexConstantBuffer, 0);
    _deviceContext->VSSetConstantBuffers(0u, 1u, &_pipeline.VertexConstantBuffer);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::Rasterize(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext)
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
    CHECK_HRESULT(_device->CreateRasterizerState(&rasterizerDesc, &rasterizerState));
    _deviceContext->RSSetState(rasterizerState);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::ConfigureViewport(ID3D11DeviceContext* _deviceContext)
{
    D3D11_VIEWPORT viewportInfos;
    {
        viewportInfos.Height = (float)MGameWindow::GetGameWindowHeight();
        viewportInfos.Width = (float)MGameWindow::GetGameWindowWidth();
        viewportInfos.TopLeftX = 0.0f;
        viewportInfos.TopLeftY = 0.0f;
        viewportInfos.MinDepth = 0.0f;
        viewportInfos.MaxDepth = 1.0f;
    }
    _deviceContext->RSSetViewports(1u, &viewportInfos);
}
///---------------------------------------------------------------------------------------------------------------------
bool CStaticMesh::LoadMeshData(const TTransform& _transform, const char* _meshName)
{
    PROFILE_FUNCTION
    
    Transform = _transform;

    // Load the mesh data from file
    {
        MeshData = MMeshResources::GetMeshDataFromFileName(_meshName);
        if (MeshData == nullptr)
            return false;
    }
    
    // Load the texture
    {
        std::wstringstream TextureFilenameStream;
        TextureFilenameStream << GAME_DATA_PATH << _meshName << ".bmp";
        CHECK_HRESULT(CreateWICTextureFromFile(G_PIPELINE.Device, G_PIPELINE.DeviceContext, TextureFilenameStream.str().c_str(), &GraphicResource.Texture, &GraphicResource.TextureView, 0));
    }
    return true;
}
///---------------------------------------------------------------------------------------------------------------------
void CStaticMesh::UninitializeStaticMeshPipeline()
{
}