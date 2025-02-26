#include "Graphic.h"

#include <d3d11.h>
#include <d3dcommon.h>

#include "../Resources/AssetList.h"
#include "../Resources/MeshResources.h"
#include "../Resources/ShadersResources.h"

#include "../GameWindow.h"
#include "../HResultHandler.h"
#include "../World.h"
#include "../Debug/Profiling.h"

///---------------------------------------------------------------------------------------------------------------------
#define RESOLUTION_WIDTH    1920
#define RESOLUTION_HEIGHT   1080
///---------------------------------------------------------------------------------------------------------------------
int GetResolutionWidth() { return RESOLUTION_WIDTH; }
int GetResolutionHeight() { return RESOLUTION_HEIGHT; }
///---------------------------------------------------------------------------------------------------------------------
static SGraphicResources_Pipeline G_PIPELINE;
SVSConstantBuffer_InstanceObject G_VS_CONSTANT_BUFFER;
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
    MGraphic::CreateDepthStencilTexture(G_PIPELINE.Device, &G_PIPELINE.DepthStencilTexture);
    MGraphic::CreateDepthStencilView(G_PIPELINE.Device, G_PIPELINE.DepthStencilTexture, &G_PIPELINE.DepthStencilView);
    MGraphic::CreateRenderTargetView(G_PIPELINE.Device, G_PIPELINE.SwapChain, &G_PIPELINE.BackBufferResource, &G_PIPELINE.RenderTargetView);
    MGraphic::SetDepthStencilViewToRenderTargetView(G_PIPELINE.DeviceContext, &G_PIPELINE.RenderTargetView, G_PIPELINE.DepthStencilView);
    MGraphic::CreateAndSetVertexShader(G_PIPELINE.Device, G_PIPELINE.DeviceContext, G_PIPELINE.VertexShaderData);
    MGraphic::CreateAndSetPixelShader(G_PIPELINE.Device, G_PIPELINE.DeviceContext, G_PIPELINE.PixelShaderData);
    MGraphic::CreatePixelShaderConstantBuffer(G_PIPELINE.Device, G_PIPELINE.DeviceContext, G_PIPELINE.PixelShaderData);
    MGraphic::CreateRasterizerState(G_PIPELINE.Device, &G_PIPELINE.rasterizerState);

    MGraphic::CreateVertexShaderBuffer(G_PIPELINE.Device,G_PIPELINE.DeviceContext, &G_PIPELINE.VSConstantBuffer_Frame, sizeof(SVSConstantBuffer_Frame));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::Draw()
{
    MGraphic::SetPixelShaderConstantBuffer(G_PIPELINE.Device, G_PIPELINE.DeviceContext, G_PIPELINE.PixelShaderData);

    SetVSConstantBuffer_Frame(G_PIPELINE.Device, G_PIPELINE.DeviceContext, &G_PIPELINE.VSConstantBuffer_Frame);

    
    const std::vector<CDrawable_InstancedMesh*>& instancedMeshes = MWorld::GetWorld()->CurrentGameScene->InstancedMeshes;
    for (int i = 0; i < instancedMeshes.size(); ++i)
    {
        CDrawable_InstancedMesh* instancedMesh = instancedMeshes[i];
        
        MGraphic::SetVertexAndIndexBuffer(G_PIPELINE.DeviceContext, &instancedMesh->VertexBuffer, instancedMesh->IndexBuffer);
        
        std::vector<ID3D11ShaderResourceView*> _textureViews;
        _textureViews.push_back(instancedMesh->ColorTexture->textureView);
        if (instancedMesh->NormalTexture != nullptr)
        {
            _textureViews.push_back(instancedMesh->NormalTexture == nullptr ? nullptr : instancedMesh->NormalTexture->textureView);
            _textureViews.push_back(!instancedMesh->EmissionTexture ? nullptr : instancedMesh->EmissionTexture->textureView);
            _textureViews.push_back(!instancedMesh->MROTexture ? nullptr : instancedMesh->MROTexture->textureView);
        }
        MGraphic::SetPixelShader(G_PIPELINE.DeviceContext, _textureViews);

        const UINT nbInstances = (UINT)instancedMesh->Instances.size();
        UINT nbInstancesRemainingToDraw = nbInstances;
        while (nbInstancesRemainingToDraw > 0)
        {
            const UINT nbInstancesToDraw = MMath::Min(nbInstancesRemainingToDraw, MAX_INSTANCE_COUNT - 1);
            const UINT startInstances = nbInstances - nbInstancesRemainingToDraw;
            
            MGraphic::SetVSConstantBuffer_Instanced(G_PIPELINE.Device, G_PIPELINE.DeviceContext, &instancedMesh->VSConstantBuffer_InstancedObject, instancedMesh->Instances, startInstances, nbInstancesToDraw);
            MGraphic::SetPrimitiveAndDraw_Instanced(G_PIPELINE.DeviceContext, instancedMesh->MeshData->IndexCount, nbInstancesToDraw + 1);
            
            nbInstancesRemainingToDraw -= nbInstancesToDraw;
        }
    }
    
    MGraphic::ConfigureViewport(G_PIPELINE.DeviceContext);
    MGraphic::Rasterize(G_PIPELINE.DeviceContext, G_PIPELINE.rasterizerState);
    MGraphic::PresentSwapChain(G_PIPELINE.SwapChain);
    MGraphic::ClearRenderTarget(G_PIPELINE.DeviceContext, G_PIPELINE.RenderTargetView);
    MGraphic::ClearDepthStencil(G_PIPELINE.DeviceContext, G_PIPELINE.DepthStencilView);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::UninitializeGraphic()
{
    
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateRasterizerState(ID3D11Device* _device, ID3D11RasterizerState** _rasterizerState)
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
        DepthStencilTextureDesc.Width = GetResolutionWidth();
        DepthStencilTextureDesc.Height = GetResolutionHeight();
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
void MGraphic::SetVertexAndIndexBuffer(ID3D11DeviceContext* _deviceContext, ID3D11Buffer** _vertexBuffer, ID3D11Buffer* _indexBuffer)
{
    constexpr UINT offset = 0u;
    _deviceContext->IASetVertexBuffers(0u, 1u, _vertexBuffer, &SMeshData::VertexBuffer_StructureByteStride, &offset);
    _deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, offset);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetPixelShader(ID3D11DeviceContext* _deviceContext, const std::vector<ID3D11ShaderResourceView*>& _textureViews)
{
    _deviceContext->PSSetShaderResources(0u, (UINT)_textureViews.size(), _textureViews.data());
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
void MGraphic::CreateAndSetVertexShader(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SVertexShaderData& _vertexShader)
{
    _vertexShader.Blob = MShaderResources::GetBlobFromFileName(TAKU_ASSET_VS_BASE);
    CHECK_HRESULT(_device->CreateVertexShader(_vertexShader.Blob->GetBufferPointer(), _vertexShader.Blob->GetBufferSize(), nullptr, &_vertexShader.Shader));
    // VertexShader Input Layout
    {
        constexpr D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
        {
            { "POSITION",  0u,  DXGI_FORMAT_R32G32B32_FLOAT,  0u,  0u,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
            { "NORMAL",    0u,  DXGI_FORMAT_R32G32B32_FLOAT,  0u,  16u,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
            { "TANGENT",    0u,  DXGI_FORMAT_R32G32B32_FLOAT,  0u,  32u,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
            { "TEXCOORD",  0u,  DXGI_FORMAT_R32G32_FLOAT,     0u,  48u,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
            { "SV_InstanceID",    0u,  DXGI_FORMAT_R32_UINT,  1u,  56u,  D3D11_INPUT_PER_INSTANCE_DATA, 0u },
            };
        UINT sizeInputElementDesc = ARRAYSIZE(inputElementDesc);
        CHECK_HRESULT(_device->CreateInputLayout(inputElementDesc, sizeInputElementDesc, _vertexShader.Blob->GetBufferPointer(), _vertexShader.Blob->GetBufferSize(), &_vertexShader.Input));
    }
    _deviceContext->VSSetShader(_vertexShader.Shader, nullptr, 0u);
    _deviceContext->IASetInputLayout(_vertexShader.Input);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateAndSetPixelShader(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SPixelShaderData& _pixelShader)
{
    _pixelShader.Blob = MShaderResources::GetBlobFromFileName(TAKU_ASSET_PS_BASE);
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
void MGraphic::CreatePixelShaderConstantBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SPixelShaderData& _pixelShader)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    {
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = sizeof(SPixelShaderConstantBuffer);
        bufferDesc.StructureByteStride = 0u;
    }
    
    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, nullptr, &_pixelShader.ConstantBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetPixelShaderConstantBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, const SPixelShaderData& _pixelShader)
{
    const SLightInfo& lightInfo = MWorld::GetWorld()->GetCurrentScene()->GetSceneLight()->GetSceneLightInfo();
    SPixelShaderConstantBuffer ConstantBufferPixelShader;
    {
        ConstantBufferPixelShader.camDir = MWorld::GetWorld()->FreeLookCamera.GetCameraWorldViewDir();
        ConstantBufferPixelShader.lightDir = lightInfo.Direction;
        ConstantBufferPixelShader.lightColor = { lightInfo.Color.r, lightInfo.Color.g, lightInfo.Color.b };
        ConstantBufferPixelShader.lightColorIntensity = lightInfo.ColorIntensity;
        ConstantBufferPixelShader.lightAmbientIntensity = lightInfo.AmbientIntensity;
    }
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CHECK_HRESULT(_deviceContext->Map(_pixelShader.ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
    memcpy(mappedResource.pData, &ConstantBufferPixelShader, sizeof(SPixelShaderConstantBuffer));
    _deviceContext->Unmap(_pixelShader.ConstantBuffer, 0);
    _deviceContext->PSSetConstantBuffers(0u, 1u, &_pixelShader.ConstantBuffer);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateVertexBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11Buffer** _vertexBuffer, const SMeshData& _meshData)
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

    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, _vertexBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateIndexBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11Buffer** _indexBuffer, const SMeshData& _meshData)
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

    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, _indexBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::CreateVertexShaderBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11Buffer** VertexConstantBuffer, UINT _size)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    {
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = _size;
        bufferDesc.StructureByteStride = 0u;
    }
    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, nullptr, VertexConstantBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetVSConstantBuffer_Frame(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11Buffer** _objectBuffer)
{
    const TMatrix4f invertedCameraMatrix = MWorld::GetWorld()->FreeLookCamera.GetCameraWorldInverseMatrix();
    const TMatrix4f perspectiveMatrix = MGameWindow::GetPerspectiveMatrix();
    
    SVSConstantBuffer_Frame buffer;
    buffer.CameraViewProjection = TMatrix4f::Transpose(invertedCameraMatrix * TMatrix4f::World * perspectiveMatrix);
    
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CHECK_HRESULT(_deviceContext->Map(*_objectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
    memcpy(mappedResource.pData, &buffer, sizeof(SVSConstantBuffer_Frame));
    _deviceContext->Unmap(*_objectBuffer, 0);
    _deviceContext->VSSetConstantBuffers(0u, 1u, _objectBuffer);
}
///---------------------------------------------------------------------------------------------------------------------
void MGraphic::SetVSConstantBuffer_Instanced(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11Buffer** _objectBuffer, const std::vector<TTransform>& _transforms, UINT _start, UINT _nbInstances)
{
    int indexGlobal = 0;
    const UINT End = _start + _nbInstances;
    for (UINT i = _start; i < End ; ++i)
    {
        const TMatrix4f objectWorldMatrix = TTransform::ToMatrix(_transforms[i]);
        indexGlobal++;
        G_VS_CONSTANT_BUFFER.instancedObject[indexGlobal].world = TMatrix4f::Transpose(objectWorldMatrix);
    }

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CHECK_HRESULT(_deviceContext->Map(*_objectBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
    memcpy(mappedResource.pData, &G_VS_CONSTANT_BUFFER, sizeof(SVSConstantBuffer_InstanceObject));
    _deviceContext->Unmap(*_objectBuffer, 0);
    _deviceContext->VSSetConstantBuffers(1u, 1u, _objectBuffer);
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