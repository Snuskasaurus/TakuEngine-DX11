#pragma once

#include "../Resources/MeshResources.h"
#include "../Resources/ShadersResources.h"

class CDrawable_InstancedMesh;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;

struct SGraphicResources_Pipeline
{
    ID3D11Device* Device = nullptr;
    ID3D11DeviceContext* DeviceContext = nullptr;
    IDXGISwapChain* SwapChain = nullptr;
    ID3D11RenderTargetView* RenderTargetView = nullptr;
    ID3D11Resource* BackBufferResource = nullptr;
    ID3D11RasterizerState* rasterizerState = nullptr;

    void Release()
    {
        Device->Release();
        DeviceContext->Release();
        SwapChain->Release();
        RenderTargetView->Release();
        BackBufferResource->Release();
        rasterizerState->Release();
    }
};

struct SDepthStencilResources
{
    ID3D11DepthStencilState* State = nullptr;
    ID3D11Texture2D* Texture = nullptr;
    ID3D11DepthStencilView* View = nullptr;
    ID3D11ShaderResourceView* ResourceView = nullptr;
    
    ID3D11DepthStencilView* ViewLight = nullptr;
    ID3D11Texture2D* TextureLight = nullptr;
    ID3D11ShaderResourceView* ResourceViewLight = nullptr;

    void Release()
    {
        State->Release();
        Texture->Release();
        View->Release();
        ResourceView->Release();
        TextureLight->Release();
        ResourceViewLight->Release();
    }
};

class MGraphic
{
public:
    static ID3D11Device* GetDXDevice();
    static ID3D11DeviceContext* GetDXDeviceContext();
public:
    static void CreateDirectXWindow();
    static void SetupDraw();
    static void Draw();
    static void UninitializeGraphic();
public:
    static void CreateRasterizerState(ID3D11Device* _device, ID3D11RasterizerState** _rasterizerState);
    static void CreateDeviceAndSwapChain(ID3D11Device**, ID3D11DeviceContext**, IDXGISwapChain**);
    static void CreateDepthStencil(ID3D11Device*, ID3D11DeviceContext*, SDepthStencilResources&);
    static void CreateRenderTargetView(ID3D11Device*, IDXGISwapChain*, ID3D11Resource**, ID3D11RenderTargetView**);
    static void CreateAndSetSampleStates();
    static void CreateVertexBuffer(ID3D11Device*, ID3D11DeviceContext*, ID3D11Buffer** _vertexBuffer, void* _vertices, UINT _nbVertices, UINT _sizeStruct);
    static void CreateIndexBuffer(ID3D11Device*, ID3D11DeviceContext*, ID3D11Buffer** _indexBuffer, TVertexIndex* _indexes, UINT _nbIndex, UINT _sizeStruct);
    static void CreateVertexShaderBuffer(ID3D11Device*, ID3D11DeviceContext*, ID3D11Buffer** VertexConstantBuffer, UINT _size);
    static void SetVertexAndIndexBuffer(ID3D11DeviceContext*, ID3D11Buffer** _vertexBuffer, ID3D11Buffer* _indexBuffer, UINT VertexBufferByteStride);
public:
    static void SetPixelShaderTextureViews(ID3D11DeviceContext* _deviceContext, UINT nbTextures, ID3D11ShaderResourceView** _textureViews);
public:
    static void SetPrimitiveAndDraw_Instanced(ID3D11DeviceContext*, UINT _indexCountPerInstance, UINT _instanceCount);
    static void Rasterize(ID3D11DeviceContext* _deviceContext, ID3D11RasterizerState* _rasterizerState);
    static void ConfigureViewport(ID3D11DeviceContext*);
    static void PresentSwapChain(IDXGISwapChain*);
    static void ClearRenderTarget(ID3D11DeviceContext*, ID3D11RenderTargetView*);
    static void ClearDepthStencil(ID3D11DeviceContext*, ID3D11DepthStencilView*);
};